#include "TimeManager.hpp"
#include "../utilities/Logger.hpp"
#include "../utilities/Counting.hpp"

namespace LTFP
{
    TimeManager* TimeManager::current = nullptr;

    TimeManager::TimeManager()
    {
        _currentTime = 0;
		_timeStepSize = 0.01;
        _timeStepCount = 0;
    }

    TimeManager::~TimeManager()
    {
        current = nullptr;
    }

    TimeManager* TimeManager::getCurrent()
    {
        if (current == nullptr)
            current = new TimeManager();

        return current;
    }

    /// @brief One step forward in time
    void TimeManager::advance()
    {
        _currentTime += _timeStepSize;
        _timeStepCount++;
        INCREASE_COUNTER("TimeStep", 1);
    }
}