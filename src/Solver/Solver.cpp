#include "Solver.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{	
	Solver::Solver(Config *config)
        : _index(config->index),
          _type(static_cast<SolverType>(config->type))
    {
        LOG_DEBUG << "Solver " << _index
                  << " of type " << SolverTypeName[_type];
    }
	
}
