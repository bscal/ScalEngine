#pragma once

#include "Core.h"

namespace Scal
{
enum class LogLevel : uint8_t
{
	Fatal = 0,
	Error,
	Warn,
	Info,
	Debug,
	Trace
};

int InitializeLogging();

SAPI void Log(LogLevel level, const char* msg, ...);


#define LOG_WARN 1
#define LOG_INFO 1
#define LOG_DEBUG 1
#define LOG_TRACE 1

#ifndef SCAL_RELEASE
#define LOG_DEBUG 1
#define LOG_TRACE 1
#endif

#define SFATAL(msg, ...) Scal::Log(Scal::LogLevel::Fatal, msg, ##__VA_ARGS__)
#define SERROR(msg, ...) Scal::Log(Scal::LogLevel::Error, msg, ##__VA_ARGS__)

#if LOG_WARN
#define SWARN(msg, ...) Scal::Log(Scal::LogLevel::Warn, msg, ##__VA_ARGS__)
#else
#define SWARN
#endif

#if LOG_INFO
#define SINFO(msg, ...) Scal::Log(Scal::LogLevel::Info, msg, ##__VA_ARGS__)
#else
#define SINFO
#endif

#if LOG_DEBUG
#define SDEBUG(msg, ...) Scal::Log(Scal::LogLevel::Debug, msg, ##__VA_ARGS__)
#else
#define SWARN
#endif

#if LOG_TRACE
#define STRACE(msg, ...) Scal::Log(Scal::LogLevel::Trace, msg, ##__VA_ARGS__)
#else
#define STRACE
#endif

}
