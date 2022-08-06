#pragma once

#include <Scal.h>
#include <Core/Application.h>

struct GameState
{
	float DeltaTime;
};

bool GameInitialize(Scal::ApplicationGame* gameInstance);

bool GameUpdate(Scal::ApplicationGame* gameInstance, float dt);

bool GameRender(Scal::ApplicationGame* gameInstance, float dt);

void GameOnResize(Scal::ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight);