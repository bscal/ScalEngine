#pragma once

#include "Core.h"

#include <string>

namespace Scal
{

enum class MemoryTag
{
	Unknown = 0,
	Array,
	DArray,
	Dictionary,
	RingQueue,
	BST,
	String,
	Application,
	Job,
	Texture,
	MaterialInstance,
	Renderer,
	Game,
	Transform,
	Entity,
	EntityNode,
	Scene,

	MaxTags
};

SAPI void InitializeMemory();
void ShutdownMemory();

SAPI void* SAlloc(uint64_t size, MemoryTag tag);
SAPI void* SRealloc(void* address, uint64_t oldSize, uint64_t newSize, MemoryTag tag);
SAPI void  SFree(void* address, uint64_t size, MemoryTag tag);
SAPI void* SZero(void* address, uint64_t size);
SAPI void* SCopy(void* dest, const void* src, uint64_t size);
SAPI void* SSet(void* dest, int value, uint64_t size);


SAPI std::string GetMemoryUsage();

}