#pragma once

#include "Core.h"

namespace Scal
{

	struct ApplicationCmdLineArgs
	{
		int Count;
		char** Args;

		const char* operator[](int index) const;
	};

	struct ApplicationConfig
	{
		const char* Name;
		uint16_t X;
		uint16_t Y;
		uint16_t Width;
		uint16_t Height;
	};

	struct ApplicationWindowBuffer
	{
		void* Memory;
		int Width;
		int Height;
		int Pitch;
	};

	// TODO maybe think about making these methods
	// Struct represting the state of the game, will be passed to the engine
	struct ApplicationGame
	{
		ApplicationConfig Config;
		bool (*Initialize)(struct ApplicationGame* gameInstance);
		bool (*Update)(struct ApplicationGame* gameInstance,
			struct ApplicationWindowBuffer* windowBuffer,
			float dt);
		void (*OnResize)(struct ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight);
		void* State; // GameState, this is managed by the game

		int AudioHertz;
	};

	SAPI bool AppInitialize(ApplicationGame* gameInstance);

	SAPI bool AppRun();

	void AppStop();

	// defined in client
	ApplicationGame* CreateApplication(ApplicationCmdLineArgs args);

}
