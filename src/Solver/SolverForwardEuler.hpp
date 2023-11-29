#ifndef __MeshData__
#define __MeshData__

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "Common.hpp"

namespace LTFP
{	
	/// @brief Solver type
    enum SolverType
    {
        FE = 0;
    };
    static std::vector<std::string> SolverTypeName = {
        "[Forward_Euler]"};

	
	
}

#endif
