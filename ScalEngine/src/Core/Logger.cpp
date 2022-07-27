#include "Logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

namespace Scal
{
	int InitializeLogging()
	{
		return 0;
	}

	void ShutdownLogging()
	{

	}

	SAPI void Log(LogLevel level, const char* msg, ...)
	{
		const char* prefixes[6] = { "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };
		bool isError = level < 2;

		char outMessage[16000];
		memset(outMessage, 0, sizeof(outMessage));

		va_list argPtr;
		va_start(argPtr, msg);
		vsnprintf(outMessage, 16000, msg, argPtr);
		va_end(argPtr);

		sprintf(outMessage, "%s%s\n", prefixes[level], outMessage);
		
		printf("%s", outMessage);
	}
}
