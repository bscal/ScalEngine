#pragma once

#include "Core/Application.h"
#include "Logger.h"
#include "SMemory.h"

extern Scal::Game* Scal::CreateGame(Scal::ApplicationCmdLineArgs args);

int main(int argc, char** argv)
{
	Scal::AppInitializeMemory();
	Scal::ApplicationCmdLineArgs args = { argc, argv };
	Scal::Game* gameInstance = Scal::CreateGame(args);
	if (!Scal::AppInitialize(args, gameInstance))
	{
		SERROR("App failed to create!");
		return 1;
	}
	Scal::AppRun();
	return 0;
}