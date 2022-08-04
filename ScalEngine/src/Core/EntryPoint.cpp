#include "Application.h"
#include "Logger.h"

extern Scal::ApplicationGame* CreateApplication(Scal::ApplicationCmdLineArgs args);

int main(int count, char** args)
{
	if (!*CreateApplication)
	{
		SERROR("Could not location Application entry point CreateApplication()");
		return 1;
	}

	auto gameInstancePtr = CreateApplication({ count, args });

	if (!Scal::AppCreate(gameInstancePtr))
	{
		SERROR("App failed to create!");
		return 2;
	}

	Scal::AppRun();
	return 0;
}