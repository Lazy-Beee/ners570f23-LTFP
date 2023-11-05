#ifndef __TimeManager__
#define __TimeManager__

#include "Common.hpp"

namespace LTFP
{
	/// @brief Class managing current simulation time and time step size. 
	/// @note This class is a singleton.
	class TimeManager
	{
	private:
        static TimeManager* current;
		Real _currentTime;
		Real _timeStepSize;
        int _timeStepCount;

	public:
		TimeManager();
		TimeManager(const TimeManager&) = delete;
		TimeManager& operator=(const TimeManager&) = delete;
		~TimeManager();

		static TimeManager* getCurrent();
        void advance();

		inline Real getTime() const { return _currentTime; };
		// inline void setTime(const Real& t) { _currentTime = t; };
		inline Real getTimeStepSize() const { return _timeStepSize; };
		inline void setTimeStepSize(const Real& dt) { _timeStepSize = dt; };
		inline int 	getTimeStepCount() const { return _timeStepCount; };
		// inline void setTimeStepCount(const int& n) { _timeStepCount = n; };
	};
}

#endif
