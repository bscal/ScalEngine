#pragma once

#include <stdint.h>

//#define internal        static
//#define local_persist	static
#define global_var		static

#ifdef SCAL_PLATFORM_WINDOWS
	#ifdef SCAL_BUILD_DLL
		#define SCAL_API __declspec(dllexport)
	#else
		#define SCAL_API __declspec(dllimport)
	#endif // SCAL_BUILD_DLL
#else
	#define SCAL_API
#endif // SCAL_PLATFORM_WINDOWS
