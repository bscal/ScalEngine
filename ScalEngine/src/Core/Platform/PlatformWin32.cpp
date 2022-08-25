#include "Platform.h"

#ifdef SCAL_PLATFORM_WINDOWS

#include "Core/Core.h"
#include "Core/Logger.h"
#include "Core/Application.h"
#include "Core/SMemory.h"
#include "Core/Input.h"

#include <Windows.h>
#include <windowsx.h>
#include <stdlib.h>

namespace Scal
{
namespace Platform
{

struct InternalPlatformState
{
	HINSTANCE Instance;
	HWND Window;
};

struct WindowBuffer
{
	BITMAPINFO Info;
	void* Memory;
	int Width;
	int Height;
	int Pitch;
};

global_var PlatformState State;
global_var WindowBuffer MainBackBuffer;
global_var LARGE_INTEGER Frequency;

internal void Win32InitializeWindowBuffer(WindowBuffer* buffer, int width, int height)
{
	if (buffer->Memory)
	{
		VirtualFree(buffer->Memory, 0, MEM_RELEASE);
	}

	buffer->Width = width;
	buffer->Height = height;
	int bytesPerPixel = 4;
	buffer->Pitch = width * bytesPerPixel;

	buffer->Info.bmiHeader.biSize = sizeof(buffer->Info.bmiHeader);
	buffer->Info.bmiHeader.biWidth = width;
	buffer->Info.bmiHeader.biHeight = height;
	buffer->Info.bmiHeader.biPlanes = 1;
	buffer->Info.bmiHeader.biBitCount = 32;
	buffer->Info.bmiHeader.biCompression = BI_RGB;

	int size = width * height * bytesPerPixel;
	// TODO look into replacing or moving into SMemory
	buffer->Memory = VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
}

LRESULT CALLBACK Win32WindowProcessMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

bool Startup(const char* applicationName, int x, int y, int width, int height)
{
	State.InternalState = (InternalPlatformState*)SAlloc(sizeof(InternalPlatformState), MemoryTag::Application);
	if (!State.InternalState) return false; // TODO is it safe to ignore this? handle crashing
	State.InternalState->Instance = GetModuleHandleA(0);

	WNDCLASSA windowClass = {};
	windowClass.style = CS_DBLCLKS;
	windowClass.lpfnWndProc = Win32WindowProcessMessage;
	windowClass.hInstance = State.InternalState->Instance;
	//windowClass.hIcon;
	windowClass.lpszClassName = "ScalWindowClass";

	if (!RegisterClassA(&windowClass))
	{
		MessageBoxA(0, "Failed to register window class", "Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	State.InternalState->Window = CreateWindowExA(
		0,
		windowClass.lpszClassName,
		applicationName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		x, y, width, height,
		0, 0,
		State.InternalState->Instance,
		0);

	if (!State.InternalState->Window)
	{
		return false;
	}

	Win32InitializeWindowBuffer(&MainBackBuffer, width, height);

	QueryPerformanceFrequency(&Frequency);

	return true;
}

void ProcessMessages()
{
	MSG message;
	while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
	{
		if (message.message == WM_QUIT) AppStop();
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}
}

ApplicationWindowBuffer GetWindowBuffer()
{
	ApplicationWindowBuffer buffer;
	buffer.Memory = MainBackBuffer.Memory;
	buffer.Width = MainBackBuffer.Width;
	buffer.Height = MainBackBuffer.Height;
	buffer.Pitch = MainBackBuffer.Pitch;
	return buffer;
}

void* Allocate(uint64_t size, bool aligned)
{
	return malloc(size);
}

void* Realloc(void* block, uint64_t newSize)
{
	return realloc(block, newSize);
}

void  Free(void* block, bool aligned)
{
	return free(block);
}

void* ZeroMem(void* block, uint64_t size)
{
	return memset(block, 0, size);
}

void* CopyMem(void* dest, const void* src, uint64_t size)
{
	return memcpy(dest, src, size);
}

void* SetMem(void* dest, int value, uint64_t size)
{
	return memset(dest, value, size);
}

void ConsoleWrite(const char* message, uint8_t color)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	const uint8_t levels[6] = { 64, 4, 6, 2, 1, 8 };
	SetConsoleTextAttribute(consoleHandle, levels[color]);

	OutputDebugStringA(message);
	size_t length = strlen(message);
	WriteConsoleA(consoleHandle, message, (DWORD)length, 0, 0);
}

void ConsoleWriteError(const char* message, uint8_t color)
{
	HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
	const uint8_t levels[6] = { 64, 4, 6, 2, 1, 8 };
	SetConsoleTextAttribute(consoleHandle, levels[color]);

	OutputDebugStringA(message);
	size_t length = strlen(message);
	WriteConsoleA(consoleHandle, message, (DWORD)length, 0, 0);
}

int64_t GetPlatformPerformanceTime()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return now.QuadPart;
}

int64_t GetPlatformPerformanceFrequency()
{
	return Frequency.QuadPart;
}

uint64_t GetPlatformCycleCount()
{
	return __rdtsc();
}

uint64_t GetPlatformSystemTimeInMS()
{
	LPSYSTEMTIME sysTime;
	GetSystemTime(sysTime);
	return sysTime->wMilliseconds;
}

void PlatformSleep(uint32_t ms)
{
	Sleep(ms);
}

struct WindowDimension
{
	int Width;
	int Height;
};

WindowDimension GetWindowDimension(HWND window)
{
	RECT clientRect;
	GetClientRect(window, &clientRect);
	return { clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };
}

internal void DrawToWindow(WindowBuffer* buffer, HDC deviceCtx, int windowWidth, int windowHeight)
{
	StretchDIBits(deviceCtx,
		0, 0, windowWidth, windowHeight,
		0, 0, buffer->Width, buffer->Height,
		buffer->Memory,
		&buffer->Info,
		DIB_RGB_COLORS,
		SRCCOPY);
}

// TODO remove
void PlatformDrawToWindow()
{
	auto deviceCtx = GetDC(State.InternalState->Window);
	auto windowDimension = GetWindowDimension(State.InternalState->Window);
	DrawToWindow(&MainBackBuffer, deviceCtx, windowDimension.Width, windowDimension.Height);
	ReleaseDC(State.InternalState->Window, deviceCtx);
}

internal LRESULT CALLBACK Win32WindowProcessMessage(HWND window, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (message)
	{
		case WM_CLOSE:
		{
			AppStop();
		} break;
		case WM_DESTROY:
		{
			AppStop();
		} break;
		case WM_SIZE:
		{
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			bool wasDown = (lParam & (1 << 30)) != 0;
			bool isDown = (lParam & (1 << 31)) == 0;
			bool pressed = !wasDown && isDown;
			bool held = wasDown && isDown;
			bool released = wasDown && !isDown;
			uint32_t keycode = wParam;
			Input::ProcessKey((Input::Keys)keycode, isDown, pressed, held, released);
		} break;
		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			Input::ProcessMouseMove(xPos, yPos);
		} break;
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta != 0)
				delta = (delta > 0) ? 1 : -1; // Flatten between -1 to 1
			Input::ProcessMouseWheel(delta);
		} break;
		case WM_LBUTTONDOWN:
			Input::ProcessMouse(Input::Buttons::ButtonLeft, true);
			break;
		case WM_MBUTTONDOWN:
			Input::ProcessMouse(Input::Buttons::ButtonMiddle, true);
			break;
		case WM_RBUTTONDOWN:
			Input::ProcessMouse(Input::Buttons::ButtonRight, true);
			break;
		case WM_LBUTTONUP:
			Input::ProcessMouse(Input::Buttons::ButtonLeft, false);
			break;
		case WM_MBUTTONUP:
			Input::ProcessMouse(Input::Buttons::ButtonMiddle, false);
			break;
		case WM_RBUTTONUP:
			Input::ProcessMouse(Input::Buttons::ButtonRight, false);
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC deviceCtx = BeginPaint(window, &paint);
			auto windowDimension = GetWindowDimension(window);
			DrawToWindow(&MainBackBuffer, deviceCtx,
				windowDimension.Width, windowDimension.Height);
			EndPaint(window, &paint);
		} break;
		default:
		{
			result = DefWindowProcA(window, message, wParam, lParam);
		} break;
	}
	return result;
}

}
}
#endif
