#pragma once

#ifdef SCAL_PLATFORM_WINDOWS
	#ifdef SCAL_BUILD_DLL
		#define SCAL_API __declspec(dllexport)
	#else
		#define SCAL_API __declspec(dllimport)
	#endif // SCAL_BUILD_DLL
#else
	#define SCAL_API
#endif // SCAL_PLATFORM_WINDOWS
