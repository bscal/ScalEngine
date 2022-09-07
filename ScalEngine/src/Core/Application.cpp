#include "Application.h"

#include "Core/Logger.h"
#include "Core/SAssert.h"
#include "Core/Audio.h"
#include "Core/Input.h"
#include "Core/Timing.h"
#include "Core/SMemory.h"
#include "Core/Platform/Platform.h"

namespace Scal
{
global_var ApplicationState AppState;

SAPI bool AppInitializeMemory()
{
	if (!Platform::Initialize())
	{
		SERROR("Platform failed to initialize!");
		return false;
	}
	Scal::InitializeMemory();
	return true;
}

bool AppInitialize(ApplicationCmdLineArgs args, Game* gameInstance)
{
	if (AppState.IsInitialized)
	{
		SERROR("App is already initialized!");
		return false;
	}
	if (!gameInstance)
	{
		SERROR("Game Instance cannot be a nullptr!");
		return false;
	}
	if (!gameInstance->Initialize || !gameInstance->Update || !gameInstance->OnResize)
	{
		SERROR("Game Instance function pointers are not all set!");
		return false;
	}
	AppState.Game = gameInstance;

	InitializeLogging();
	Input::InitializeInput();
	InitializeAudio(&AppState);

	if (!Platform::Startup(
		gameInstance->Config.Name,
		gameInstance->Config.X,
		gameInstance->Config.Y,
		gameInstance->Config.Width,
		gameInstance->Config.Height))
	{
		SERROR("Platform failed to startup!");
		return false;
	}

	AppState.PerminentMemory.Size = Megabytes(64);
	AppState.PerminentMemory.Address = SAllocPage(
		AppState.PerminentMemory.Size,
		MemoryTag::Application);

	AppState.TransientMemory.Size = Gigabytes((uint64_t)2);
	AppState.TransientMemory.Address = SAllocPage(
		AppState.TransientMemory.Size,
		MemoryTag::Application);

	AppState.IsRunning = true;
	AppState.IsSuspended = false;
	AppState.IsInitialized = gameInstance->Initialize(&AppState);

	SINFO("Application Initialized: %b", AppState.IsInitialized);
	SINFO(Scal::GetMemoryUsage().c_str());
	return AppState.IsInitialized;
}

void AppRun()
{
	float dt = 0.0f;
	while (AppState.IsRunning)
	{
		//Timer timer("RunLoop");
		Platform::ProcessMessages(&AppState);

		if (!AppState.IsSuspended)
		{
			auto windowBuffer = Platform::GetWindowBuffer();
			if (!AppState.Game->Update(&AppState, &windowBuffer, 0.0f))
			{
				SFATAL("GameInstance Update failed. Exiting...");
				break;
			}
			Platform::PlatformDrawToWindow();
		}

		Input::UpdateInput();
	}
}

SAPI void AppShutdown()
{
	AppState.IsRunning = false;
	ShutdownAudio();
}

const char* ApplicationCmdLineArgs::operator[](int index) const
{
	SASSERT(index < Count);
	return Args[index];
}

}
