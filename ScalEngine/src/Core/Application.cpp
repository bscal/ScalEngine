#include "Application.h"

#include "Core/Logger.h"
#include "Core/SAssert.h"
#include "Core/Audio.h"
#include "Core/Input.h"
#include "Core/Platform/Platform.h"

namespace Scal
{

struct ApplicationState
{
	ApplicationGame* GameInstance;
	uint64_t LastTime;
	uint16_t Width;
	uint16_t Height;
	bool IsRunning;
	bool IsSuspended;
};

global_var bool IsInitialized;
global_var ApplicationState AppState;

bool AppInitialize(ApplicationGame* gameInstance)
{
	if (IsInitialized)
	{
		SERROR("App is already initialized!");
		return false;
	}
	if (!gameInstance->Initialize || !gameInstance->Update || !gameInstance->Render || !gameInstance->OnResize)
	{
		SERROR("Game Instance function pointers are not all set!");
		return false;
	}
	AppState.GameInstance = gameInstance;

	InitializeLogging();
	Input::InitializeInput();
	InitializeAudio();

	SINFO("Logging %s values, %d", "a", 123);
	SFATAL("FATAL %f ERROR", 0.00525f);
	STRACE("HELLO %i", 1234109123);
	SERROR("%i %f %s", 99, 15.28, "This");
	SDEBUG("DEBUGING");
	SWARN("Warn %.000f", 12395.123914);

	AppState.IsRunning = true;

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

	gameInstance->Initialize(gameInstance);

	return IsInitialized = true;
}

bool AppRun()
{
	float dt = 0.0f;
	int x = 0;
	int y = 0;

	while (AppState.IsRunning)
	{
		Platform::ProcessMessages();

		if (!AppState.IsSuspended)
		{

			if (!AppState.GameInstance->Update(AppState.GameInstance, 0.0f))
			{
				SFATAL("GameInstance Update failed. Exiting...");
				break;
			}

			auto windowBuffer = Platform::GetWindowBuffer();
			if (!AppState.GameInstance->Render(AppState.GameInstance,
				&windowBuffer, x++, y++, 0.0f))
			{
				SFATAL("GameInstance Render failed. Exiting...");
				break;
			}

			Platform::TestRender();
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

constexpr const char* ApplicationCmdLineArgs::operator[](int index) const
{
	SASSERT(index < Count);
	return Args[index];
}

}
