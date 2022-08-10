#include "Game.h"

#include <Core/EntryPoint.h>
#include <Core/SMemory.h>

bool GameInitialize(Scal::ApplicationGame* gameInstance)
{
	SINFO("GameInitialized!");
	return true;
}

bool GameUpdate(Scal::ApplicationGame* gameInstance, float dt)
{
	return true;
}

bool GameRender(Scal::ApplicationGame* gameInstance, float dt)
{
	return true;
}

void GameOnResize(Scal::ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight)
{
}

global_var GameState State;

Scal::ApplicationGame* Scal::CreateApplication(ApplicationCmdLineArgs args)
{
	ApplicationGame* gameInstance = (ApplicationGame*) SAlloc(sizeof(ApplicationGame), MemoryTag::Application);
	gameInstance->Config =
	{
		"Scal Game",
		64,
		64,
		1280,
		720
	};
	gameInstance->Initialize = GameInitialize;
	gameInstance->Update = GameUpdate;
	gameInstance->Render = GameRender;
	gameInstance->OnResize = GameOnResize;
	gameInstance->State = &State;

	SINFO(Scal::GetMemoryUsage().c_str());

	return gameInstance;
}
