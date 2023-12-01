#include "ThermalSolverFTCS.hpp"
#include "utilities/Logger.hpp"
#include "src/MeshData.hpp"
#include "src/TimeManager.hpp"
#include "src/MaterialProperty.hpp"
#include "src/LaserSource.hpp"
#include "src/ThermalBoundary/BoundaryManager.hpp"

namespace LTFP
{
    ThermalSolverFTCS::ThermalSolverFTCS(thermalConfig config) : ThermalSolver(config)
    {
    }

    void ThermalSolverFTCS::updateTemperature()
    {
        MeshData *md = MeshData::getCurrent();
        MaterialProperty *mp = MaterialProperty::getCurrent();

        static const Real minDx = fmin(fmin(md->getIntervalX(), md->getIntervalY()), md->getIntervalZ());

        // Find time step size using Von Neumann stability condition
        Real minKappa = REAL_MAX;
#pragma omp parallel for collapse(3) schedule(static) reduction(min : minKappa)
        for (size_t i = 0; i < md->getSizeX(); i++)
        {
            for (size_t j = 0; j < md->getSizeY(); j++)
            {
                for (size_t k = 0; k < md->getSizeZ(); k++)
                {
                    const Real &T = md->getTemperature(i, j, k);
                    const Real &kappa = mp->getDiffusionCoefficient(T);
                    if (kappa < minKappa)
                        minKappa = kappa;
                }
            }
        }
        Real maxDt = minDx * minDx / (6.0f * minKappa);
        TimeManager::getCurrent()->setTimeStepSize(maxDt * _cflNumber);

        // Copy temperature to temperatureOld
        md->copyTempToOld();

        // Compute cell flux and update temperature
#pragma omp parallel for collapse(3) schedule(dynamic)
        for (size_t i = 0; i < md->getSizeX(); i++)
        {
            for (size_t j = 0; j < md->getSizeY(); j++)
            {
                for (size_t k = 0; k < md->getSizeZ(); k++)
                {
                    updateTempSingleCell(i, j, k);
                }
            }
        }
    }

    /// @brief Compute du/dt of a cell
    /// @details du/dt = (k * (dT/dx) * S + flux * S) / (rho * V); unit is J/(kg*s)
    Real ThermalSolverFTCS::computeCellDudt(const unsigned int &i, const unsigned int &j, const unsigned int &k)
    {
        MeshData *md = MeshData::getCurrent();
        BoundaryManager *bm = BoundaryManager::getCurrent();
        MaterialProperty *mp = MaterialProperty::getCurrent();

        static const Real dx = md->getIntervalX();
        static const Real dy = md->getIntervalY();
        static const Real dz = md->getIntervalZ();
        static const Real AreaX = dy * dz;
        static const Real AreaY = dz * dx;
        static const Real AreaZ = dx * dy;
        static const Real AreaX_dx = dy * dz / dx;
        static const Real AreaY_dy = dz * dx / dy;
        static const Real AreaZ_dz = dx * dy / dz;
        static const Real Volume = dx * dy * dz;

        const Vector3r pos = md->getCenterPos(i, j, k);
        const Real T = md->getTemperatureOld(i, j, k);
        const Real k = mp->getProperty<CONDUCTIVITY>(T);
        const Real rho = mp->getProperty<DENSITY>(T);

        Real flowRate = 0.0;

        // X Positive Neighbor
        if (i == md->getSizeX() - 1)
        {
            if (bm->isTempBC(XPOSITIVE))
            {
                const Real &Tn = bm->getTempBC(XPOSITIVE, pos, T);
                const Real &kn = mp->getProperty<CONDUCTIVITY>(Tn);
                Real kAvg = 2 * k * kn / (k + kn);
                flowRate += kAvg * (Tn - T) * AreaX_dx;
            }
            else
            {
                flowRate += bm->getFluxBC(XPOSITIVE, pos, T) * AreaX;
            }
        }
        else
        {
            const Real &Tn = md->getTemperatureOld(i + 1, j, k);
            const Real &kn = mp->getProperty<CONDUCTIVITY>(Tn);
            Real kAvg = 2 * k * kn / (k + kn);
            flowRate += kAvg * (Tn - T) * AreaX_dx;
        }

        // X Negative Neighbor
        if (i == 1)
        {
            if (bm->isTempBC(XNEGATIVE))
            {
                const Real &Tn = bm->getTempBC(XNEGATIVE, pos, T);
                const Real &kn = mp->getProperty<CONDUCTIVITY>(Tn);
                Real kAvg = 2 * k * kn / (k + kn);
                flowRate += kAvg * (Tn - T) * AreaX_dx;
            }
            else
            {
                flowRate += bm->getFluxBC(XNEGATIVE, pos, T) * AreaX;
            }
        }
        else
        {
            const Real &Tn = md->getTemperatureOld(i - 1, j, k);
            const Real &kn = mp->getProperty<CONDUCTIVITY>(Tn);
            Real kAvg = 2 * k * kn / (k + kn);
            flowRate += kAvg * (Tn - T) * AreaX_dx;
        }

        flowRate += LaserSource::getCurrent()->getLaserPower(i, j, k);
        return flowRate / (rho * Volume);
    }

    /// @brief update temperature of a single cell
    void ThermalSolverFTCS::updateTempSingleCell(const unsigned int &i, const unsigned int &j, const unsigned int &k)
    {
        MeshData *md = MeshData::getCurrent();
        const Real &du_dt = computeCellDudt(i, j, k);
        const Real &dt = TimeManager::getCurrent()->getTimeStepSize();
        Real T = md->getTemperatureOld(i, j, k);
        MaterialProperty::getCurrent()->updateTempDu(T, du_dt * dt);
        md->setTemperature(i, j, k, T);
    }
}
