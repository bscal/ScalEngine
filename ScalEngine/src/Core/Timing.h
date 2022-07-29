#pragma once

#include "Core.h"
#include <Windows.h>

namespace Scal
{
	struct Timer
	{
		LARGE_INTEGER Frequency;
		LARGE_INTEGER StartCounter;
		LARGE_INTEGER EndCounter;
		int64_t StartCycleCount;

		SCAL_API Timer();
		SCAL_API ~Timer();
	};
}
