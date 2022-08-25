#include "Timing.h"

#include "Core/Platform/Platform.h"
#include "Core/Logger.h"

namespace Scal
{
	Timer::Timer(const char* name)
	{
		Name = name;
		StartCounter = Platform::GetPlatformPerformanceTime();
		StartCycleCount = Platform::GetPlatformCycleCount();
	}

	Timer::~Timer()
	{
		auto endCounter = Platform::GetPlatformPerformanceTime();
		auto endCycleCount = Platform::GetPlatformCycleCount();

		auto frequency = Platform::GetPlatformPerformanceFrequency();
		int64_t elapsed = endCounter - StartCounter;
		double elapsedMS = (1000.0f * (double)elapsed) / (double)frequency;

		uint64_t elapsedCycles = endCycleCount - StartCycleCount;
		double megaCycles = (double)elapsedCycles / (1000.0f * 1000.0f);
		SDEBUG("[ %s ] Elasped time: %.03fms/f, %.03fMc/f", Name, elapsedMS, megaCycles);
	}

	PersistentTimer::PersistentTimer()
	{
		Frequency = Platform::GetPlatformPerformanceFrequency();
		LastCounter = 0;
		LastCycleCount = 0;
	}

	void PersistentTimer::Start()
	{
		LastCounter = Platform::GetPlatformPerformanceTime();
		LastCycleCount = Platform::GetPlatformCycleCount();
	}

	void PersistentTimer::Stop()
	{
		auto endCounter = Platform::GetPlatformPerformanceTime();
		auto endCycleCount = Platform::GetPlatformCycleCount();

		int64_t elapsed = endCounter - LastCounter;
		double elapsedMS = (1000.0f * (double)elapsed) / (double)Frequency;
		
		double FPS = (double)Frequency / (double)elapsed; 

		uint64_t elapsedCycles = endCycleCount - LastCycleCount;
		double megaCycles = (double)elapsedCycles / (1000.0f * 1000.0f);

		LastCounter = endCounter;
		LastCycleCount = endCycleCount;

		SDEBUG("Elasped time: %.03fms/f, %.03ffps %f.03Mc/f", elapsedMS, FPS, megaCycles);
	}

}
