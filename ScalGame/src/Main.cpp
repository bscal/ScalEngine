#include <Scal.h>
#include <Core/Logger.h>
#include <Core/Asserts.h>
#include <Core/Timing.h>

int main()
{ 
	{
		//Scal::Timer timer = Scal::Timer();
		Scal::PersistentTimer persistentTimer = {};

		Scal::Application* app = new Scal::Application();
		app->Print(5);

		SINFO("Logging %s values, %d", "a", 123);
		SFATAL("FATAL %f ERROR", 0.00525f);
		STRACE("HELLO %i", 1234109123);
		SERROR("%i %f %s", 99, 15.28, "This");

		persistentTimer.Stop();
		
		//SASSERT(1 == 2);
		//SASSERT_MSG(1 == 2, "Dummy assert test!");
	}
	return 0;
}