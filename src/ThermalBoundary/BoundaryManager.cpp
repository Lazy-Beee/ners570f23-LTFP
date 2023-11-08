#include "BoundaryManager.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"
#include "utilities/Counting.hpp"

namespace LTFP
{
    BoundaryManager *BoundaryManager::current = nullptr;

    BoundaryManager::BoundaryManager()
    {
        
    }

    BoundaryManager::~BoundaryManager()
    {
        current = nullptr;
    }

    BoundaryManager *BoundaryManager::getCurrent()
    {
        if (current == nullptr)
            current = new BoundaryManager();

        return current;
    }

    /// @brief Load configuration from SceneLoader
    void BoundaryManager::init()
    {
        SceneLoader::TimeConfig timeConfig = SceneLoader::getCurrent()->getTimeConfig();

        // _minTimeStepSize = timeConfig.minTimeStepSize;
        // _maxTimeStepSize = timeConfig.maxTimeStepSize;
        // _timeStepSize = _minTimeStepSize;
        // _endTime = timeConfig.endTime;
        // _maxTimeSteps = timeConfig.maxTimeSteps;

        // if (_minTimeStepSize < _maxTimeStepSize)
        // {
        //     LOG_ERR << "Maximum time step size is smaller than Minimum time step size, abort simulation";
        //     exit(1);
        // }

        // if (_endTime < 0.0 && _maxTimeSteps < 0)
        // {
        //     LOG_ERR << "Terminate condition undefined, abort simulation";
        //     exit(1);
        // }
        // else if (_endTime < 0.0)
        //     _endTime = REAL_MAX;
        // else if (_maxTimeSteps < 0)
        //     _maxTimeSteps = INT_MAX;
    }

    
}