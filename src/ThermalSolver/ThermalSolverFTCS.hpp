#ifndef __ThermalSolverFTCS__
#define __ThermalSolverFTCS__

#include "ThermalSolver.hpp"
#include <string>
#include "src/Common.hpp"
#include "src/SceneLoader.hpp"

namespace LTFP
{
    /// @brief FTCS thermal solver.
    class ThermalSolverFTCS : public ThermalSolver
    {
    protected:
        void updateTempSingleCell(const unsigned int &i, const unsigned int &j, const unsigned int &k);
        Real computeCellDudt(const unsigned int &i, const unsigned int &j, const unsigned int &k);

    public:
        ThermalSolverFTCS(thermalConfig config);
        ~ThermalSolverFTCS(){};

        void updateTemperature();
    };

}

#endif
