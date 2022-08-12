#include "Logger.h"
#include "SAssert.h"
#include "Platform/Platform.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

namespace Scal
{
	global_var const char* Prefixes[6] = { "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };

	int InitializeLogging()
	{
		return 0;
	}

	void ShutdownLogging()
	{

	}

	void Log(LogLevel level, const char* msg, ...)
	{
		bool isError = (uint8_t)level < (uint8_t)LogLevel::Warn;

		char outMessage[1024];
		Platform::ZeroMem(outMessage, sizeof(outMessage));

		va_list argPtr;
		va_start(argPtr, msg);
		vsnprintf(outMessage, sizeof(outMessage), msg, argPtr);
		va_end(argPtr);

		char outMessageTemp[sizeof(outMessage) + 10];
		sprintf_s(outMessageTemp, "%s%s\n", Prefixes[(int)level], outMessage);

		if (isError)
			Platform::ConsoleWriteError(outMessageTemp, (uint8_t)level);
		else
			Platform::ConsoleWrite(outMessageTemp, (uint8_t)level);
	}

	void ReportAssertionFailure(const char* expression, const char* msg, const char* file, int line)
	{
		Log(LogLevel::Fatal, "Assertion Failure: %s, Message: %s\n  File: %s, Line: %d\n", expression, msg, file, line);
	}
}
