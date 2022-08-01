#include <Scal.h>
#include <Core/Logger.h>
#include <Core/Asserts.h>
#include <Core/Timing.h>
#include "Core/Platform/Platform.h"

int main()
{ 
	Scal::PersistentTimer persistentTimer = {};

	SINFO("Logging %s values, %d", "a", 123);
	SFATAL("FATAL %f ERROR", 0.00525f);
	STRACE("HELLO %i", 1234109123);
	SERROR("%i %f %s", 99, 15.28, "This");

	Scal::Platform::PlatformState platformState;
	if (Scal::Platform::Startup(&platformState, "Scal Game", 0, 0, 0, 0))
	{
		while (true)
		{
			if (!Scal::Platform::ProcessMessages(&platformState))
				break;
		}
	}
	persistentTimer.Stop();
	return 0;
}