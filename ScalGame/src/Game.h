#pragma once

#include <Scal.h>
#include <stdint.h>

namespace Scal
{
	struct GameState
	{
		float DeltaTime;
	};

	bool GameInitialize(ApplicationGame* gameInstance);

	bool GameUpdate(ApplicationGame* gameInstance, float dt);

	bool GameRender(ApplicationGame* gameInstance, float dt);

	void GameOnResize(ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight);
}
