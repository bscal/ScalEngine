#include "Platform.h"

#ifdef SCAL_PLATFORM_WINDOWS

#include "Core/Core.h"
#include "Core/Logger.h"
#include "Core/Application.h"
#include "Core/SMemory.h"

#include <Windows.h>
#include <windowsx.h>
#include <stdlib.h>

struct InternalState
{
	HINSTANCE Instance;
	HWND Window;
};

namespace Scal
{

	global_var LARGE_INTEGER Frequency;
	global_var LARGE_INTEGER StartTime;

	LRESULT CALLBACK Win32WindowProcessMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	bool Startup(PlatformState* platformState, const char* applicationName, int x, int y, int width, int height)
	{
		platformState->InternalState = malloc(sizeof(InternalState));
		InternalState* state = (InternalState*)platformState->InternalState;
		if (!state) return false; // TODO is it safe to ignore this? handle crashing
		state->Instance = GetModuleHandleA(0);

		WNDCLASSA windowClass = { 0 };
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
			x,
			y,
			width,
			height,
			0,
			0,
			state->Instance,
			0);

		if (!state->Window)
		{
			return false;
		}

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

	// TODO move
	global_var BITMAPINFO BitmapInfo;
	global_var void* BitmapMemory;
	global_var int BitmapWidth;
	global_var int BitmapHeight;
	constexpr int BytesPerPixel = 4;

	internal void Win32UpdateWindow(HDC deviceCtx, const RECT& windowRect, int x, int y, int width, int height)
	{
		int windowWidth = windowRect.right - windowRect.left;
		int windowHeight = windowRect.bottom - windowRect.top;
		StretchDIBits(deviceCtx,
			0, 0, BitmapWidth, BitmapHeight,
			0, 0, windowWidth, windowHeight,
			BitmapMemory,
			&BitmapInfo,
			DIB_RGB_COLORS,
			SRCCOPY);
	}


	// TODO move testing purposes
	void Render(PlatformState* platformState, int xOffset, int yOffset)
	{
		int pitch = BitmapWidth * BytesPerPixel;
		uint8_t* row = (uint8_t*)BitmapMemory;
		for (int y = 0; y < BitmapHeight; ++y)
		{
			uint32_t* pixel = (uint32_t*)row;
			for (int x = 0; x < BitmapWidth; ++x)
			{
				uint8_t r = (uint8_t)(x + xOffset);
				uint8_t g = (uint8_t)(y + xOffset);
				uint8_t b = 0;

				// BB GG RR AA in memory
				// AA RR GG BB in register
				*pixel++ = ((r << 16) | (g << 8) | b);
			}
			row += pitch;
		}

		auto state = (InternalState*)platformState->InternalState;
		auto deviceCtx = GetDC(state->Window);
		RECT clientRect;
		GetClientRect(state->Window, &clientRect);
		Win32UpdateWindow(deviceCtx, clientRect, 0, 0,
			clientRect.right - clientRect.left,
			clientRect.bottom - clientRect.top);
		ReleaseDC(state->Window, deviceCtx);
	}

	internal void Win32ResizeDIBSection(int width, int height)
	{
		if (BitmapMemory)
		{
			VirtualFree(BitmapMemory, 0, MEM_RELEASE);
		}

		BitmapWidth = width;
		BitmapHeight = height;

		BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
		BitmapInfo.bmiHeader.biWidth = width;
		BitmapInfo.bmiHeader.biHeight = height;
		BitmapInfo.bmiHeader.biPlanes = 1;
		BitmapInfo.bmiHeader.biBitCount = 32;
		BitmapInfo.bmiHeader.biCompression = BI_RGB;

		int size = width * height * BytesPerPixel;
		BitmapMemory = VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
	}

	internal LRESULT CALLBACK Win32WindowProcessMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;
		switch (message)
		{
			case WM_CLOSE:
			{
				AppStop();
				SDEBUG("Closed!");
			} break;
			case WM_DESTROY:
			{
				AppStop();
				SDEBUG("Destroy");

			} break;
			case WM_SIZE:
			{
				RECT rect;
				GetClientRect(window, &rect);
				int w = rect.right - rect.left;
				int h = rect.bottom - rect.top;
				Win32ResizeDIBSection(w, h);

				SDEBUG("Resized!");
			} break;
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				bool pressed = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;

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
				RECT clientRect;
				GetClientRect(window, &clientRect);
				Win32UpdateWindow(deviceCtx,
					clientRect,
					paint.rcPaint.left,
					paint.rcPaint.top,
					paint.rcPaint.right - paint.rcPaint.left,
					paint.rcPaint.bottom - paint.rcPaint.top);
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
