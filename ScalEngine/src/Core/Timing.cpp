#include "Timing.h"

#include "Logger.h"

namespace Scal
{
	Timer::Timer()
	{
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartCounter);
		StartCycleCount = __rdtsc();
	}

	Timer::~Timer()
	{
		LARGE_INTEGER endCounter;
		QueryPerformanceCounter(&endCounter);
		uint64_t endCycleCount = __rdtsc();

		int64_t elapsed = endCounter.QuadPart - StartCounter.QuadPart;
		double elapsedMS = (1000.0f * (double)elapsed) / (double)Frequency.QuadPart;

		uint64_t elapsedCycles = endCycleCount - StartCycleCount;
		double megaCycles = (double)elapsedCycles / (1000.0f * 1000.0f);
		SDEBUG("Elasped time: %.03fms/f, %.03fMc/f", elapsedMS, megaCycles);
	}

	PersistentTimer::PersistentTimer()
	{
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&LastCounter);
		LastCycleCount = __rdtsc();
	}

	void PersistentTimer::Stop()
	{
		LARGE_INTEGER endCounter;
		QueryPerformanceCounter(&endCounter);
		uint64_t endCycleCount = __rdtsc();

		int64_t elapsed = endCounter.QuadPart - LastCounter.QuadPart;
		double elapsedMS = (1000.0f * (double)elapsed) / (double)Frequency.QuadPart;
		
		double FPS = (double)Frequency.QuadPart / (double)elapsed; 

		uint64_t elapsedCycles = endCycleCount - LastCycleCount;
		double megaCycles = (double)elapsedCycles / (1000.0f * 1000.0f);

		LastCounter = endCounter;
		LastCycleCount = endCycleCount;

		SDEBUG("Elasped time: %.03fms/f, %.03ffps %f.03Mc/f", elapsedMS, FPS, megaCycles);
	}

}
