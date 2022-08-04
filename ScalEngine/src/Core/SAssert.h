#pragma once

#include "Core.h"

namespace Scal
{
	SAPI void ReportAssertionFailure(const char* expression, const char* msg, const char* file, int line);
}

#define SCAL_ASSERTIONS_ENABLED

#ifdef SCAL_ASSERTIONS_ENABLED
	#ifdef SCAL_PLATFORM_WINDOWS
		#include <intrin.h>
		#define DebugBreak(void) __debugbreak()
	#else
		#define DebugBreak(void) __builtin_trap()
	#endif

#define SASSERT(expr)											\
{																\
	if (!(expr))												\
	{															\
		Scal::ReportAssertionFailure(#expr, "", __FILE__, __LINE__);	\
		DebugBreak();											\
	}															\
}

#define SASSERT_MSG(expr, msg)									\
{																\
	if (!(expr))												\
	{															\
		Scal::ReportAssertionFailure(#expr, msg, __FILE__, __LINE__);	\
		DebugBreak();											\
	}															\
}
#else
#define SASSERT
#define SASSERT_MSG
#endif // SCAL_ASSERTIONS_ENABLED

