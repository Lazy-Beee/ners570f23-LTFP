#ifndef __ThermalSolver__
#define __ThermalSolver__

#include <string>
#include "src/Common.hpp"
#include "src/SceneLoader.hpp"

// TODO: distinguish all using of config structs
using thermalConfig = LTFP::SceneLoader::ThermalSolverConfig;

namespace LTFP
{
    /// @brief Solver type
    enum ThermalSolverType
    {
        FTCS = 0
    };
    static std::vector<std::string> ThermalSolverTypeName = {
        "[FTCS]"};

    /// @brief Base class of thermal solvers.
    class ThermalSolver
    {
    protected:
        const ThermalSolverType _type;
        const Real _cflNumber;

    public:
        ThermalSolver(thermalConfig config);
        virtual ~ThermalSolver(){};

        /// @brief Initialize the thermal solver
        virtual void init(){};
        /// @brief Update the thermal field in one time step
        virtual void updateTemperature(){};

        void computeCoolingRate();
        void computeTemperatureGrad();

        inline ThermalSolverType getType() const { return _type; };
        inline Real getCflNumber() const { return _cflNumber; };
    };

    ThermalSolver* createThermalSolver(thermalConfig config);
}

#endif
