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

		SCAL_API Timer();
		SCAL_API ~Timer();
	};

	struct PersistentTimer
	{
		LARGE_INTEGER Frequency;
		LARGE_INTEGER LastCounter;
		uint64_t LastCycleCount;

		SCAL_API PersistentTimer();
		SCAL_API void Stop();
	};

}
