#include "Logger.h"
#include "Asserts.h"

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
		bool isError = (uint8_t)level < 2u;

		char outMessage[4096];
		memset(outMessage, 0, sizeof(outMessage));

		va_list argPtr;
		va_start(argPtr, msg);
		vsnprintf(outMessage, 4096, msg, argPtr);
		va_end(argPtr);

		char outMessageTemp[4112];
		sprintf_s(outMessageTemp, "%s%s\n", Prefixes[(int)level], outMessage);
		printf("%s", outMessageTemp);
	}

	void ReportAssertionFailure(const char* expression, const char* msg, const char* file, int line)
	{
		Log(LogLevel::Fatal, "Assertion Failure: %s, Message: %s\n  File: %s, Line: %d\n", expression, msg, file, line);
	}
}
