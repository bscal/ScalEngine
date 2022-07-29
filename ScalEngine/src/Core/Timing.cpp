#include "Timing.h"

#include "Logger.h"

namespace Scal
{
	Timer::Timer() : Frequency({}), StartCounter({}), EndCounter({})
	{
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartCounter);
		StartCycleCount = __rdtsc();
	}

	Timer::~Timer()
	{
		QueryPerformanceCounter(&EndCounter);
		int64_t endCycleCount = __rdtsc();

		int64_t elasped = EndCounter.QuadPart - StartCounter.QuadPart;
		float elaspedMS = (1000.0f * (float)elasped) / (float)Frequency.QuadPart;

		int64_t elapsedCycles = endCycleCount - StartCycleCount;
		float megaCycles = (float)elapsedCycles / (1000.0f * 1000.0f);
		SDEBUG("Elasped time: %fms/f, %fMc/f", elaspedMS, megaCycles);
	}

}
