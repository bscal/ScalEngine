#pragma once

#include "Core/Core.h"

namespace Scal
{
    struct InternalState;
    
    struct PlatformState
    {
        InternalState* InternalState;
    };

    bool Startup(PlatformState* platformState, const char* applicationName, int x, int y, int height, int width);

    void ProcessMessages(PlatformState* platformState);

    void TestRender(PlatformState* platformState, int xOffset, int yOffset);

    void* Allocate(uint64_t size, bool aligned);
    void  Free(void* block, bool aligned);
    void* ZeroMem(void* block, uint64_t size);
    void* CopyMem(void* dest, const void* src, uint64_t size);
    void* SetMem(void* dest, int value, uint64_t size);
    
    void ConsoleWrite(const char* message, uint8_t color);
    void ConsoleWriteError(const char* message, uint8_t color);
    
    uint64_t GetPlatformTime();
    
    void PlatformSleep(uint32_t ms);

}
