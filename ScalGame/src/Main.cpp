#include "Game.h"

GlobalVariable Scal::GameState State;

int main()
{ 
	Scal::ApplicationGame gameInstance;
	gameInstance.Config = 
	{
		"Scal Game",
		64,
		64,
		1280,
		720
	};
	gameInstance.Initialize = Scal::GameInitialize;
	gameInstance.Update = Scal::GameUpdate;
	gameInstance.Render = Scal::GameRender;
	gameInstance.OnResize = Scal::GameOnResize;
	gameInstance.State = &State;

	if (!Scal::AppCreate(&gameInstance))
	{
		SERROR("App failed to create!");
		return 1;
	}

	Scal::AppRun();

	return 0;
}