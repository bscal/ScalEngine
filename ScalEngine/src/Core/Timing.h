#pragma once

#include "Core.h"

namespace Scal
{
	struct Timer
	{
		int64_t StartCounter;
		int64_t StartCycleCount;
		const char* Name;

		SAPI Timer(const char* name);
		SAPI ~Timer();
	};

	struct PersistentTimer
	{
		uint64_t Frequency;
		uint64_t LastCounter;
		uint64_t LastCycleCount;

		SAPI PersistentTimer();
		SAPI void Start();
		SAPI void Stop();
	};

}
