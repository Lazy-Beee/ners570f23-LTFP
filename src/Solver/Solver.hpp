#ifndef __Solver__
#define __Solver__

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "src/Common.hpp"
#include "src/SceneLoader.hpp"

using Config = LTFP::SceneLoader::SolverConfig;

namespace LTFP
{	
	/// @brief Solver type
    enum SolverType
    {
        FE = 0
    };
    static std::vector<std::string> SolverTypeName = {
        "[Forward_Euler]"
    };

    /// @brief Base class of solvers.
    class Solver
    {
    protected:
        const int _index;
        const SolverType _type;

    public:
        Solver(Config *config);
        virtual ~Solver(){};

        inline int getIndex() const { return _index; };
        inline SolverType getType() const { return _type; };
    };
	
	
}

#endif
