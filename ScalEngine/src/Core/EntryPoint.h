#pragma once

#include "Core/Application.h"
#include "Logger.h"
#include "SMemory.h"

extern Scal::ApplicationGame* Scal::CreateApplication(Scal::ApplicationCmdLineArgs args);

int main(int argc, char** argv)
{
	Scal::InitializeMemory();

	auto gameInstancePtr = Scal::CreateApplication({ argc, argv });

	if (!Scal::AppInitialize(gameInstancePtr))
	{
		SERROR("App failed to create!");
		return 1;
	}

	Scal::AppRun();
	return 0;
}