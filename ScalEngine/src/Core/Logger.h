#pragma once

#include "Core.h"

namespace Scal
{

#define LOG_WARN 1
#define LOG_INFO 1
#define LOG_DEBUG 1
#define LOG_TRACE 1

#ifdef SCAL_RELEASE
#define LOG_DEBUG 0
#define LOG_TRACE 0
#endif

	enum LogLevel
	{
		Fatal,
		Error,
		Warn,
		Info,
		Debug,
		Trace
	};


	int InitializeLogging();

	void ShutdownLogging();

	SAPI void Log(LogLevel level, const char* msg, ...);


#define SFATAL(msg, ...) Log(Scal::LogLevel::Fatal, msg, ##__VA_ARGS__)
#define SERROR(msg, ...) Log(Scal::LogLevel::.Error, msg, ##__VA_ARGS__)

#if LOG_WARN
#define SWARN(msg, ...) Log(Scal::LogLevel::Warn, msg, ##__VA_ARGS__)
#else
#define SWARN
#endif

#if LOG_INFO
#define SINFO(msg, ...) Log(Scal::LogLevel::Info, msg, ##__VA_ARGS__)
#else
#define SINFO
#endif

#if LOG_DEBUG
#define SDEBUG(msg, ...) Log(Scal::LogLevel::Debug, msg, ##__VA_ARGS__)
#else
#define SWARN
#endif

#if LOG_TRACE
#define STRACE(msg, ...) Log(Scal::LogLevel::Trace, msg, ##__VA_ARGS__)
#else
#define STRACE
#endif

}
