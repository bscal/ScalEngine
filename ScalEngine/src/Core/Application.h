#pragma once

#include "Core.h"

namespace Scal
{
	struct ApplicationWindowBuffer
	{
		void* Memory;
		int Width;
		int Height;
		int Pitch;
	};

	struct ApplicationConfig
	{
		const char* Name;
		uint16_t X;
		uint16_t Y;
		uint16_t Width;
		uint16_t Height;
	};

	struct GameMemory
	{
		uint64_t PermenantSize;
		void* PermenantStoragePtr;
	};

	// Defined by the Game
	struct Game
	{
		ApplicationConfig Config;
		bool (*Initialize)(struct ApplicationState* gameInstance);
		bool (*Update)(struct ApplicationState* gameInstance,
			struct ApplicationWindowBuffer* windowBuffer,
			float dt);
		void (*OnResize)(struct ApplicationState* gameInstance, uint32_t newWidth, uint32_t newHeight);
		void* State; // GameState, this is managed by the game
	};

	struct ApplicationState
	{
		GameMemory GameMemory;
		Game* Game;

		int AudioHertz;
		bool IsInitialized;
		bool IsRunning;
		bool IsSuspended;
	};

	SAPI bool AppInitialize(Game* gameInstance);

	SAPI bool AppRun();

	void AppStop();

	struct ApplicationCmdLineArgs
	{
		int Count;
		char** Args;

		const char* operator[](int index) const;
	};

	// defined in client
	Game* CreateApplication(ApplicationCmdLineArgs args);

}
