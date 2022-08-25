#pragma once

#include <Scal.h>
#include <Core/Application.h>

struct GameState
{
	int XOffset;
	int YOffset;
	float DeltaTime;
};

bool GameInitialize(Scal::ApplicationState* gameInstance);

bool GameUpdate(Scal::ApplicationState* gameInstance,
	Scal::ApplicationWindowBuffer* windowBuffer,
	float dt);

void GameOnResize(Scal::ApplicationState* gameInstance, uint32_t newWidth, uint32_t newHeight);