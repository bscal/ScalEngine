#pragma once

#include "Core.h"
#include "SAssert.h"

int main(int argc, char** argv);

namespace Scal
{

	struct ApplicationCmdLineArgs
	{
		int Count;
		char** Args;

		constexpr const char* operator[](int index) const
		{
			SASSERT(index < Count);
			return Args[index];
		}
	};

	struct ApplicationConfig
	{
		const char* Name;
		uint16_t X;
		uint16_t Y;
		uint16_t Width;
		uint16_t Height;
	};

	// TODO maybe think about making these methods
	// Struct represting the state of the game, will be passed to the engine
	struct ApplicationGame
	{
		ApplicationConfig Config;
		bool (*Initialize)(struct ApplicationGame* gameInstance);
		bool (*Update)(struct ApplicationGame* gameInstance, float deltaTime);
		bool (*Render)(struct ApplicationGame* gameInstance, float deltaTime);
		void (*OnResize)(struct ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight);
		void* State; // GameState, this is managed by the game
	};

	SAPI bool AppCreate(ApplicationGame* gameInstance);

	SAPI bool AppRun();

	extern SAPI ApplicationGame* CreateApplication(ApplicationCmdLineArgs args);

}
