#pragma once

#include "Core/Core.h"
#include "Core/Application.h"

namespace Scal
{
namespace Platform
{

struct InternalPlatformState;

struct PlatformState
{
	InternalPlatformState* InternalState;
};

bool Startup(const char* applicationName, int x, int y, int height, int width);

void ProcessMessages();

// TODO this is just to test draw
// with no gpu acceleration
void PlatformDrawToWindow();

ApplicationWindowBuffer GetWindowBuffer();

void* Allocate(uint64_t size, bool aligned);
void* Realloc(void* block, uint64_t newSize);
void  Free(void* block, bool aligned);
void* ZeroMem(void* block, uint64_t size);
void* CopyMem(void* dest, const void* src, uint64_t size);
void* SetMem(void* dest, int value, uint64_t size);

void ConsoleWrite(const char* message, uint8_t color);
void ConsoleWriteError(const char* message, uint8_t color);

int64_t GetPlatformPerformanceTime();
int64_t GetPlatformPerformanceFrequency();
uint64_t GetPlatformCycleCount();
uint64_t GetPlatformSystemTimeInMS();

void PlatformSleep(uint32_t ms);

}
}
