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
		static TimeManager *current;
		Real _currentTime;
		Real _timeStepSize;
		Real _minTimeStepSize;
		Real _maxTimeStepSize;
		int _timeStepCount;
		Real _endTime;
		int _maxTimeSteps;

	public:
		TimeManager();
		TimeManager(const TimeManager &) = delete;
		TimeManager &operator=(const TimeManager &) = delete;
		~TimeManager();

		static TimeManager *getCurrent();
		void init();
		bool advance();
		Real setTimeStepSize(Real dt);

		inline Real getTime() const { return _currentTime; };
		inline Real getTimeStepSize() const { return _timeStepSize; };
		inline int getTimeStepCount() const { return _timeStepCount; };
	};
}

#endif
