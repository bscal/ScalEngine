#include "Platform.h"

#ifdef SCAL_PLATFORM_WINDOWS

#include "Core/Core.h"
#include "Core/Logger.h"

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

        WNDCLASSA windowClass = {0};
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

    void Shutdown(PlatformState* platformState)
    {
        InternalState* state = (InternalState*)platformState->InternalState;
        if (state->Window)
        {
            DestroyWindow(state->Window);
        }
    }

    bool ProcessMessages(PlatformState* platformState)
    {
        // TODO what is the best way to do messages? 1 per frame, or until empty?
        MSG message;
        BOOL result = GetMessageA(&message, 0, 0, 0);
        if (result > 0)
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        else
        {
            return false;
        }
        return true;
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

    LRESULT CALLBACK Win32WindowProcessMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        LRESULT result = 0;
        switch (message)
        {
            case WM_CLOSE:
            {
                SDEBUG("Closed!");
                PostQuitMessage(0);
            } break;
            case WM_DESTROY:
            {
                SDEBUG("Destroy");

            } break;
            case WM_SIZE:
            {
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
            default:
            {
                result = DefWindowProcA(window, message, wParam, lParam);
            } break;
        }
        return result;
    }

}
#endif
