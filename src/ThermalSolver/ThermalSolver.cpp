#include "ThermalSolver.hpp"
#include "ThermalSolverFTCS.hpp"
#include "utilities/Logger.hpp"
#include "src/MeshData.hpp"
#include "src/TimeManager.hpp"

namespace LTFP
{
    /// @brief Create different thermal solvers from configuration
    ThermalSolver* createThermalSolver(thermalConfig config)
    {
        if (config.type == FTCS)
        {
            ThermalSolverFTCS *ts = new ThermalSolverFTCS(config);
            return static_cast<ThermalSolver *>(ts);
        }
        else
        {
            LOG_ERR << "Unsupported thermal solver type " << config.type;
            exit(1);
        }

        return nullptr;
    }

    ThermalSolver::ThermalSolver(thermalConfig config) : _type(static_cast<ThermalSolverType>(config.type)), _cflNumber(config.cflNumber)
    {
        LOG_DEBUG << "Thermal solver of type " << ThermalSolverTypeName[_type] << " is created";
    }

    /// @brief Compute temperature gradient
    void ThermalSolver::computeTemperatureGrad()
    {
        MeshData *md = MeshData::getCurrent();
        const size_t xSize = md->getSizeX();
        const size_t ySize = md->getSizeY();
        const size_t zSize = md->getSizeZ();
        const Real xInterval = md->getIntervalX();
        const Real yInterval = md->getIntervalY();
        const Real zInterval = md->getIntervalZ();

        Real TempGradX, TempGradY, TempGradZ;

#pragma omp parallel for collapse(3) schedule(static)
        for (size_t i = 0; i < xSize; i++)
        {
            for (size_t j = 0; j < ySize; j++)
            {
                for (size_t k = 0; k < zSize; k++)
                {
                    if (i == 0) // Boundary (left x)
                    {
                        TempGradX = (md->getTemperature(i + 1, j, k) - md->getTemperature(i, j, k)) / xInterval;
                    }
                    else if (i == xSize - 1) // Boundary (right x)
                    {
                        TempGradX = (md->getTemperature(i, j, k) - md->getTemperature(i - 1, j, k)) / xInterval;
                    }
                    else // Center x
                    {
                        TempGradX = (md->getTemperature(i + 1, j, k) - md->getTemperature(i - 1, j, k)) / xInterval * 0.5;
                    }

                    if (j == 0) // Boundary (bottom y)
                    {
                        TempGradY = (md->getTemperature(i, j + 1, k) - md->getTemperature(i, j, k)) / yInterval;
                    }
                    else if (j == ySize - 1) // Boundary (top y)
                    {
                        TempGradY = (md->getTemperature(i, j, k) - md->getTemperature(i, j - 1, k)) / yInterval;
                    }
                    else // Center y
                    {
                        TempGradY = (md->getTemperature(i, j + 1, k) - md->getTemperature(i, j - 1, k)) / yInterval * 0.5;
                    }

                    if (k == 0) // Boundary (left z)
                    {
                        TempGradZ = (md->getTemperature(i, j, k + 1) - md->getTemperature(i, j, k)) / zInterval;
                    }
                    else if (k == zSize - 1) // Boundary (right z)
                    {
                        TempGradZ = (md->getTemperature(i, j, k) - md->getTemperature(i, j, k - 1)) / zInterval;
                    }
                    else // Center z
                    {
                        TempGradZ = (md->getTemperature(i, j, k + 1) - md->getTemperature(i, j, k - 1)) / zInterval * 0.5;
                    }

                    md->setTemperatureGrad(i, j, k, {TempGradX, TempGradY, TempGradZ});
                }
            }
        }
    }

    /// @brief Compute cooling rate
    void ThermalSolver::computeCoolingRate()
    {
        MeshData *md = MeshData::getCurrent();
        Real dt = TimeManager::getCurrent()->getTimeStepSize();

#pragma omp parallel for collapse(3) schedule(static)
        for (size_t i = 0; i < md->getSizeX(); i++)
        {
            for (size_t j = 0; j < md->getSizeY(); j++)
            {
                for (size_t k = 0; k < md->getSizeZ(); k++)
                {
                    md->setCoolingRate(i, j, k, (md->getTemperature(i, j, k) - md->getTemperatureOld(i, j, k)) / dt);
                }
            }
        }
    }

}
