#include "Game.h"

#include <Core/EntryPoint.h>
#include <Core/SMemory.h>
#include <Core/Structures/SArray.h>
#include <Core/Input.h>
#include <Core/Audio.h>

using namespace Scal::Input;

bool GameInitialize(Scal::ApplicationGame* gameInstance)
{
	SINFO("GameInitialized!");
	return true;
}

bool GameUpdate(Scal::ApplicationGame* gameInstance, float dt)
{
	if (IsKeyPressed(Keys::KEY_W))
	{
		SINFO("W Pressed!");
	}

	if (IsKeyHeld(Keys::KEY_A))
	{
		SINFO("A Held!");
	}

	if (IsKeyReleased(Keys::KEY_D))
	{
		SINFO("D Released!");
	}

	if (IsKeyDown(Keys::KEY_S))
	{
		SINFO("S DOWN!");
	}

	if (IsKeyPressed(Keys::KEY_SPACE))
	{
		Scal::SoundPlayFromFile("assets/Audio/VSM.mp3");
	}

	if (IsMouseDown(Buttons::ButtonRight))
	{
		SINFO("Right Mouse Is Down!");
	}

	if (WasMouseDown(Buttons::ButtonLeft))
	{
		SINFO("Left Mouse Was Down!");
	}

	return true;
}

bool GameRender(Scal::ApplicationGame* gameInstance,
	Scal::ApplicationWindowBuffer* windowBuffer, int xOffset, int yOffset, float dt)
{
	uint8_t* row = (uint8_t*)windowBuffer->Memory;
	for (int y = 0; y < windowBuffer->Height; ++y)
	{
		uint32_t* pixel = (uint32_t*)row;
		for (int x = 0; x < windowBuffer->Width; ++x)
		{
			uint8_t r = (uint8_t)(x + xOffset);
			uint8_t g = (uint8_t)(y + yOffset);
			uint8_t b = 0;

			// BB GG RR AA in memory
			// AA RR GG BB in register
			*pixel++ = ((r << 16) | (g << 8) | b);
		}
		row += windowBuffer->Pitch;
	}
	return true;
}

void GameOnResize(Scal::ApplicationGame* gameInstance, uint32_t newWidth, uint32_t newHeight)
{
}

global_var GameState State;

Scal::ApplicationGame* Scal::CreateApplication(ApplicationCmdLineArgs args)
{
	ApplicationGame* gameInstance = (ApplicationGame*)SAlloc(sizeof(ApplicationGame), MemoryTag::Application);
	gameInstance->Config =
	{
		"Scal Game",
		64,
		64,
		1280,
		720
	};
	gameInstance->Initialize = GameInitialize;
	gameInstance->Update = GameUpdate;
	gameInstance->Render = GameRender;
	gameInstance->OnResize = GameOnResize;
	gameInstance->State = &State;

	Scal::Structures::SArray* sArray = Scal::Structures::SArrayCreate(int);
	SArrayPush(sArray, 5);
	SArrayPush(sArray, 255);
	SArrayPush(sArray, -100000);
	SArrayPush(sArray, -500 + 250);
	SArrayPush(sArray, 65000);
	
	int* i = (int*)Scal::Structures::ArrayPeekAt(sArray, 1);
	SINFO("i = %d", *i);
	++* i;
	SINFO("i = %d", *i);
	int* ii = (int*)Scal::Structures::ArrayPeekAt(sArray, 1);
	SINFO("i = %d", *ii);

	int val;
	Scal::Structures::ArrayPop(sArray, &val);
	SINFO("pop: %d", val);
	
	SINFO("length: %d, capacity: %d, stride %d, sizeof: %d", sArray->Length, sArray->Capacity, sArray->Stride, sizeof(Scal::Structures::SArray));
	SINFO(Scal::GetMemoryUsage().c_str());

	return gameInstance;
}
