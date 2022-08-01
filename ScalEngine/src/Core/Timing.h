#pragma once

#include "Core.h"
#include <Windows.h>

namespace Scal
{
	struct Timer
	{
		LARGE_INTEGER Frequency;
		LARGE_INTEGER StartCounter;
		uint64_t StartCycleCount;

		SAPI Timer();
		SAPI ~Timer();
	};

	struct PersistentTimer
	{
		LARGE_INTEGER Frequency;
		LARGE_INTEGER LastCounter;
		uint64_t LastCycleCount;

		SAPI PersistentTimer();
		SAPI void Stop();
	};

}
