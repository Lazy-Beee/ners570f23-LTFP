#include "TimeManager.hpp"
#include "SceneLoader.hpp"
#include "../utilities/Logger.hpp"
#include "../utilities/Counting.hpp"

namespace LTFP
{
    TimeManager* TimeManager::current = nullptr;

    TimeManager::TimeManager()
    {
        _currentTime = 0;
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

    /// @brief Load configuration from SceneLoader
    void TimeManager::init()
    {
        SceneLoader::TimeConfig timeConfig = SceneLoader::getCurrent()->getTimeConfig();

        _minTimeStepSize = timeConfig.minTimeStepSize;
        _maxTimeStepSize = timeConfig.maxTimeStepSize;
		_timeStepSize = _minTimeStepSize;
        _endTime = timeConfig.endTime;
        _maxTimeSteps = timeConfig.maxTimeSteps;
    }

    /// @brief One step forward in time
    /// @return True if end time is not reached
    /// @note Do nothing on first call
    bool TimeManager::advance()
    {
        static bool firstCall = true;
        if (firstCall)
        {
            firstCall = false;
            return true;
        }

        if (_currentTime >= _endTime)
        {
            LOG_INFO << "End time reached, simulation stopped.";
            return false;
        }
        else if (_timeStepCount >= _maxTimeSteps)
        {
            LOG_INFO << "Maximum time steps reached, simulation stopped.";
            return false;
        }
        else
        {
            _currentTime += _timeStepSize;
            _timeStepCount++;
            INCREASE_COUNTER("TimeStep", 1);
        }

        return true;
    }

    /// @brief Set time step size
    /// @param dt Target time step size
    /// @return Actual time step size
    /// @note The actual time step size is bounded by TimeConfig.minTimeStepSize and TimeConfig.maxTimeStepSize
    Real TimeManager::setTimeStepSize(Real dt)
    {
        _timeStepSize = fmax(fmin(dt, _maxTimeStepSize), _minTimeStepSize);
        return _timeStepSize;
    }
}