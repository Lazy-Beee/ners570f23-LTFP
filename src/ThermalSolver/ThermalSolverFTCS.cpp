#include "ThermalSolverFTCS.hpp"
#include "utilities/Logger.hpp"
#include "src/MeshData.hpp"
#include "src/TimeManager.hpp"
#include "src/MaterialProperty.hpp"
#include "src/LaserSource.hpp"
#include "src/ThermalBoundary/BoundaryManager.hpp"

using namespace std;

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
                    const Real &kappa = mp->getKappa(T);
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
// #pragma omp parallel for collapse(3) schedule(dynamic)
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
        LaserSource *ls = LaserSource::getCurrent();

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

        const Vector3r pos_p = md->getCenterPos(i, j, k);
        const Real T_p = md->getTemperatureOld(i, j, k);
        const Real k_p = mp->getProperty<CONDUCTIVITY>(T_p);
        const Real rho_p = mp->getProperty<DENSITY>(T_p);

        Real flowRate = 0.0;

        // X Positive Neighbor
        if (i == md->getSizeX() - 1)
        {
            if (bm->isTempBC(XPOSITIVE))
            {
                const Real &T_n = bm->getTempBC(XPOSITIVE, pos_p, T_p);
                const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
                Real kAvg = 2 * k_p * k_n / (k_p + k_n);
                flowRate += kAvg * (T_n - T_p) * AreaX_dx;
            }
            else
            {
                flowRate += bm->getFluxBC(XPOSITIVE, pos_p, T_p) * AreaX;
            }
        }
        else
        {
            const Real &T_n = md->getTemperatureOld(i + 1, j, k);
            const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
            Real kAvg = 2 * k_p * k_n / (k_p + k_n);
            flowRate += kAvg * (T_n - T_p) * AreaX_dx;
        }

        // X Negative Neighbor
        if (i == 0)
        {
            if (bm->isTempBC(XNEGATIVE))
            {
                const Real &T_n = bm->getTempBC(XNEGATIVE, pos_p, T_p);
                const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
                Real kAvg = 2 * k_p * k_n / (k_p + k_n);
                flowRate += kAvg * (T_n - T_p) * AreaX_dx;
            }
            else
            {
                flowRate += bm->getFluxBC(XNEGATIVE, pos_p, T_p) * AreaX;
            }
        }
        else
        {
            const Real &T_n = md->getTemperatureOld(i - 1, j, k);
            const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
            Real kAvg = 2 * k_p * k_n / (k_p + k_n);
            flowRate += kAvg * (T_n - T_p) * AreaX_dx;
        }

        // Y Positive Neighbor
        if (j== md->getSizeY() - 1)
        {
            if (bm->isTempBC(YPOSITIVE))
            {
                const Real &T_n = bm->getTempBC(YPOSITIVE, pos_p, T_p);
                const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
                Real kAvg = 2 * k_p * k_n / (k_p + k_n);
                flowRate += kAvg * (T_n - T_p) * AreaY_dy;
            }
            else
            {
                flowRate += bm->getFluxBC(YPOSITIVE, pos_p, T_p) * AreaY;
            }
        }
        else
        {
            const Real &T_n = md->getTemperatureOld(i, j + 1, k);
            const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
            Real kAvg = 2 * k_p * k_n / (k_p + k_n);
            flowRate += kAvg * (T_n - T_p) * AreaY_dy;
        }

        // Y Negative Neighbor
        if (j == 0)
        {
            if (bm->isTempBC(YNEGATIVE))
            {
                const Real &T_n = bm->getTempBC(YNEGATIVE, pos_p, T_p);
                const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
                Real kAvg = 2 * k_p * k_n / (k_p + k_n);
                flowRate += kAvg * (T_n - T_p) * AreaY_dy;
            }
            else
            {
                flowRate += bm->getFluxBC(YNEGATIVE, pos_p, T_p) * AreaY;
            }
        }
        else
        {
            const Real &T_n = md->getTemperatureOld(i, j - 1, k);
            const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
            Real kAvg = 2 * k_p * k_n / (k_p + k_n);
            flowRate += kAvg * (T_n - T_p) * AreaY_dy;
        }

        // Z Positive Neighbor
        if (k == md->getSizeZ() - 1)
        {
            if (bm->isTempBC(ZPOSITIVE))
            {
                const Real &T_n = bm->getTempBC(ZPOSITIVE, pos_p, T_p);
                const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
                Real kAvg = 2 * k_p * k_n / (k_p + k_n);
                flowRate += kAvg * (T_n - T_p) * AreaZ_dz;
            }
            else
            {
                flowRate += bm->getFluxBC(ZPOSITIVE, pos_p, T_p) * AreaZ;
            }
        }
        else
        {
            const Real &T_n = md->getTemperatureOld(i, j, k + 1);
            const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
            Real kAvg = 2 * k_p * k_n / (k_p + k_n);
            flowRate += kAvg * (T_n - T_p) * AreaZ_dz;
        }

        // Z Negative Neighbor
        if (k == 0)
        {
            if (bm->isTempBC(ZNEGATIVE))
            {
                const Real &T_n = bm->getTempBC(ZNEGATIVE, pos_p, T_p);
                const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
                Real kAvg = 2 * k_p * k_n / (k_p + k_n);
                flowRate += kAvg * (T_n - T_p) * AreaZ_dz;
            }
            else
            {
                flowRate += bm->getFluxBC(ZNEGATIVE, pos_p, T_p) * AreaZ;
            }
        }
        else
        {
            const Real &T_n = md->getTemperatureOld(i, j, k - 1);
            const Real &k_n = mp->getProperty<CONDUCTIVITY>(T_n);
            Real kAvg = 2 * k_p * k_n / (k_p + k_n);
            flowRate += kAvg * (T_n - T_p) * AreaZ_dz;
        }

        // Laser heat source
        if (ls->laserActive())
            flowRate += LaserSource::getCurrent()->getLaserPower(i, j, k);

        return flowRate / (rho_p * Volume);
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
