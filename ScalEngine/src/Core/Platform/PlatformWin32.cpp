#include "Platform.h"

#ifdef SCAL_PLATFORM_WINDOWS

#include <Windows.h>
#include <windowsx.h>

struct InternalState
{
    HINSTANCE Instance;
    HWND Window;
};

namespace Scal { namespace Platform
{

    LRESULT CALLBACK Win32WindowProcessMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            case WM_SIZE:
            {

            } break;
        }
        return 0;
    }

    bool Startup(PlatformState* platformState, const char* applicationName, int x, int y, int height, int width)
    {
        platformState->InternalState = malloc(sizeof(InternalState));
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

        return true;
    }

    void Shutdown(PlatformState* platformState);

    bool ProcessMessage(PlatformState* platformState);

    void* Allocate(uint64_t size, bool aligned);
    void  Free(void* block, bool aligned);
    void* ZeroMem(void* block, uint64_t size);
    void* CopyMem(void* dest, const void* src, uint64_t size);
    void* SetMem(void* dest, int value, uint64_t size);

    void ConsoleWrite(const char* message, uint8_t color);
    void ConsoleWriteError(const char* message, uint8_t color);

    uint64_t GetPlatformTime();

    void Sleep(uint32_t ms);

}}
#endif
