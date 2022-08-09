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
	SINFO("GameUpdate!");
	return true;
}

bool GameRender(Scal::ApplicationGame* gameInstance, float dt)
{
	SINFO("GameRender!");
	return true;
}

void GameOnResize(Scal::ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight)
{
}

global_var GameState State;
global_var Scal::ApplicationGame gameInstance;

Scal::ApplicationGame* Scal::CreateApplication(ApplicationCmdLineArgs args)
{
	//Scal::ApplicationGame gameInstance;
	gameInstance.Config =
	{
		"Scal Game",
		64,
		64,
		1280,
		720
	};
	gameInstance.Initialize = GameInitialize;
	gameInstance.Update = GameUpdate;
	gameInstance.Render = GameRender;
	gameInstance.OnResize = GameOnResize;
	gameInstance.State = &State;

	auto block = SAlloc(512, Scal::MemoryTag::Entity);
	auto block1 = SAlloc(2500, Scal::MemoryTag::Array);
	auto block2 = SAlloc(1024 * 1024 + 250, Scal::MemoryTag::Application);

	SINFO(Scal::GetMemoryUsage().c_str());

	return &gameInstance;
}