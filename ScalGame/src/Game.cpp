#include <Game.h>

namespace Scal
{
	bool GameInitialize(ApplicationGame* gameInstance)
	{
		SINFO("GameInitialized!");
		return true;
	}

	bool GameUpdate(ApplicationGame* gameInstance, float dt)
	{
		SINFO("GameUpdate!");
		return true;
	}

	bool GameRender(ApplicationGame* gameInstance, float dt)
	{
		SINFO("GameRender!");
		return true;
	}

	void GameOnResize(ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight)
	{
	}
}
