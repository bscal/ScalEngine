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

bool AppInitialize(Game* gameInstance)
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
	
	//AppState = (ApplicationState*)SAlloc(sizeof(ApplicationState), MemoryTag::Application);
	//SZero(AppState, sizeof(ApplicationState));

	AppState.Game = gameInstance;

	InitializeLogging();
	Input::InitializeInput();
	InitializeAudio(&AppState);

	AppState.GameMemory = {};
	AppState.GameMemory.PermenantSize = Megabytes(16);
	AppState.GameMemory.PermenantStoragePtr = SAlloc(
		AppState.GameMemory.PermenantSize,
		MemoryTag::Application);

	if (!Platform::Startup(
		gameInstance->Config.Name,
		gameInstance->Config.X,
		gameInstance->Config.Y,
		gameInstance->Config.Width,
		gameInstance->Config.Height))
	{
		SERROR("App failed to startup!");
		return false;
	}

	AppState.IsRunning = true;
	AppState.IsSuspended = false;
	AppState.IsInitialized = gameInstance->Initialize(&AppState);

	SINFO("Application Initialized: %b", AppState.IsInitialized);
	SINFO(Scal::GetMemoryUsage().c_str());
	return AppState.IsInitialized;
}

bool AppRun()
{
	float dt = 0.0f;
	while (AppState.IsRunning)
	{
		Timer timer("RunLoop");
		Platform::ProcessMessages();

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

	return true;
}

void AppStop()
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
