#include "Game.h"

#include <Core/EntryPoint.h>
#include <Core/SMemory.h>
#include <Core/Structures/SArray.h>
#include <Core/Input.h>
#include <Core/Audio.h>

using namespace Scal::Input;

global_var GameState State;

bool GameInitialize(Scal::ApplicationState* gameInstance)
{
	SINFO("GameInitialized!");
	gameInstance->AudioHertz = 256;
	return true;
}

bool GameUpdate(Scal::ApplicationState* gameInstance,
	Scal::ApplicationWindowBuffer* windowBuffer,
	float dt)
{
	if (IsKeyPressed(Keys::KEY_W))
	{
		SINFO("W Pressed!");
	}

	if (IsKeyDown(Keys::KEY_A))
	{
		SINFO("A Down!");
		--gameInstance->AudioHertz;
	}

	if (IsKeyDown(Keys::KEY_D))
	{
		SINFO("D Down!");
		++gameInstance->AudioHertz;
	}

	if (IsKeyReleased(Keys::KEY_S))
	{
		SINFO("S Released!");

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

	State.XOffset++;
	State.YOffset++;

	uint8_t* row = (uint8_t*)windowBuffer->Memory;
	for (int y = 0; y < windowBuffer->Height; ++y)
	{
		uint32_t* pixel = (uint32_t*)row;
		for (int x = 0; x < windowBuffer->Width; ++x)
		{
			uint8_t r = (uint8_t)(x + State.XOffset);
			uint8_t g = (uint8_t)(y + State.YOffset);
			uint8_t b = 0;

			// BB GG RR AA in memory
			// AA RR GG BB in register
			*pixel++ = ((r << 16) | (g << 8) | b);
		}
		row += windowBuffer->Pitch;
	}

	return true;
}

void GameOnResize(Scal::ApplicationState* gameInstance, uint32_t newWidth, uint32_t newHeight)
{
}

Scal::Game* Scal::CreateApplication(ApplicationCmdLineArgs args)
{
	Game* gameInstance = (Game*)SAlloc(sizeof(Game), MemoryTag::Game);
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

	return gameInstance;
}
