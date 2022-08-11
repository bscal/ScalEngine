#include "Platform.h"

#ifdef SCAL_PLATFORM_WINDOWS

#include "Core/Core.h"
#include "Core/Logger.h"
#include "Core/Application.h"
#include "Core/SMemory.h"

#include <Windows.h>
#include <windowsx.h>
#include <stdlib.h>

namespace Scal
{

	global_var LARGE_INTEGER Frequency;
	global_var LARGE_INTEGER StartTime;

	struct InternalState
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

	global_var WindowBuffer MainBackBuffer;

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

	bool Startup(PlatformState* platformState, const char* applicationName, int x, int y, int width, int height)
	{
		platformState->InternalState = (InternalState*)SAlloc(sizeof(InternalState), MemoryTag::Application);
		InternalState* state = (InternalState*)platformState->InternalState;
		if (!state) return false; // TODO is it safe to ignore this? handle crashing
		state->Instance = GetModuleHandleA(0);

		WNDCLASSA windowClass = {};
		windowClass.style = CS_DBLCLKS;
		windowClass.lpfnWndProc = Win32WindowProcessMessage;
		windowClass.hInstance = state->Instance;
		//windowClass.hIcon;
		windowClass.lpszClassName = "ScalWindowClass";

		if (!RegisterClassA(&windowClass))
		{
			MessageBoxA(0, "Failed to register window class", "Error", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		state->Window = CreateWindowExA(
			0,
			windowClass.lpszClassName,
			applicationName,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			x, y, width, height,
			0, 0,
			state->Instance,
			0);

		if (!state->Window)
		{
			return false;
		}

		Win32InitializeWindowBuffer(&MainBackBuffer, width, height);

		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartTime);

		return true;
	}

	void ProcessMessages(PlatformState* platformState)
	{
		MSG message;
		while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) AppStop();
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
	}

	void* Allocate(uint64_t size, bool aligned)
	{
		return malloc(size);
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

	uint64_t GetPlatformTime()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return now.QuadPart;
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

	// TODO move testing purposes
	internal void UpdatePixels(WindowBuffer* buffer, int xOffset, int yOffset)
	{
		uint8_t* row = (uint8_t*)buffer->Memory;
		for (int y = 0; y < buffer->Height; ++y)
		{
			uint32_t* pixel = (uint32_t*)row;
			for (int x = 0; x < buffer->Width; ++x)
			{
				uint8_t r = (uint8_t)(x + xOffset);
				uint8_t g = (uint8_t)(y + xOffset);
				uint8_t b = 0;

				// BB GG RR AA in memory
				// AA RR GG BB in register
				*pixel++ = ((r << 16) | (g << 8) | b);
			}
			row += buffer->Pitch;
		}
	}

	// TODO just for testing
	void TestRender(PlatformState* platformState, int xOffset, int yOffset)
	{
		UpdatePixels(&MainBackBuffer, xOffset, yOffset);

		auto deviceCtx = GetDC(platformState->InternalState->Window);
		auto windowDimension = GetWindowDimension(platformState->InternalState->Window);
		DrawToWindow(&MainBackBuffer, deviceCtx, 
			windowDimension.Width, windowDimension.Height);
		ReleaseDC(platformState->InternalState->Window, deviceCtx);
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
				uint32_t keycode = wParam;
				if (keycode == 'W')
				{
					SINFO("Key: %c, WasDown: %d, IsDown: %d", 'W', wasDown, isDown);
				}
			} break;
			case WM_MOUSEMOVE:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
			} break;
			case WM_MOUSEWHEEL:
			{
				int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				if (delta != 0)
					delta = (delta > 0) ? 1 : -1; // Flatten between -1 to 1
			} break;
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			{
				bool pressed = message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN;
			} break;
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
#endif
