#pragma once

#include <Scal.h>
#include <Core/Application.h>

struct GameState
{
	int XOffset;
	int YOffset;
	float DeltaTime;
};

bool GameInitialize(Scal::ApplicationGame* gameInstance);

bool GameUpdate(Scal::ApplicationGame* gameInstance,
	Scal::ApplicationWindowBuffer* windowBuffer,
	float dt);

void GameOnResize(Scal::ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight);