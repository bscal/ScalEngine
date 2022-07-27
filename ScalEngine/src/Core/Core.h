#pragma once

#ifdef SCAL_PLATFORM_WINDOWS
	#ifdef SCAL_BUILD_DLL
		#define SAPI __declspec(dllexport)
	#else
		#define SAPI __declspec(dllimport)
	#endif // SCAL_BUILD_DLL
#else
	#define SAPI
#endif // SCAL_PLATFORM_WINDOWS
