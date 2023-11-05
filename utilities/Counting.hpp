#ifndef __Counting_H__
#define __Counting_H__

#include <iostream>
#include <unordered_map>
#include "Logger.hpp"
#include "Timing.hpp"

namespace Utilities
{
	#define INCREASE_COUNTER(counterName, increaseBy) \
		Utilities::Counting::increaseCounter(counterName, increaseBy);

	#define INIT_COUNTING \
		std::unordered_map<std::string, Utilities::AverageCount> Utilities::Counting::m_averageCounts; 

	struct AverageCount
	{
		double sum;
		unsigned int numberOfCalls;
	};

	class Counting
	{
	public:
		static std::unordered_map<std::string, AverageCount> m_averageCounts;

		static void reset()
		{
			m_averageCounts.clear();
		}

		inline static void increaseCounter(const std::string& name, const double increaseBy)
		{
			std::unordered_map<std::string, AverageCount>::iterator iter;
			iter = Counting::m_averageCounts.find(name);
			if (iter != Counting::m_averageCounts.end())
			{
				iter->second.sum += increaseBy;
				iter->second.numberOfCalls++;
			}
			else
			{
				AverageCount ac;
				ac.sum = increaseBy;
				ac.numberOfCalls = 1;
				Counting::m_averageCounts[name] = ac;
			}
		}

		inline static void printAverageCounts()
		{
			std::unordered_map <std::string, AverageCount>::iterator iter;
			for (iter = Counting::m_averageCounts.begin(); iter != Counting::m_averageCounts.end(); iter++)
			{
				AverageCount &ac = iter->second;
				const double avgCount = ac.sum / ac.numberOfCalls;
				LOG_INFO << "Average number: " << iter->first.c_str() << ": " << avgCount;
			}
			LOG_INFO << "---------------------------------------------------------------------------";
		}

		inline static void printCounterSums()
		{
			std::unordered_map <std::string, AverageCount>::iterator iter;
			for (iter = Counting::m_averageCounts.begin(); iter != Counting::m_averageCounts.end(); iter++)
			{
				AverageCount &ac = iter->second;
				LOG_INFO << "Total number: " << iter->first.c_str() << ": " << ac.sum;
			}
			LOG_INFO << "---------------------------------------------------------------------------";
		}
	};
}

#endif // __Counting_H__