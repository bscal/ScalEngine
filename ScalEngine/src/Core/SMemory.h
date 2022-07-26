#pragma once

#include "Core.h"

#include <string>

namespace Scal
{

enum class MemoryTag : int
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

SAPI void* SAlloc(uint64_t size, MemoryTag tag);
SAPI void* SAllocPage(uint64_t size, MemoryTag tag);
SAPI void* SRealloc(void* block, uint64_t oldSize, uint64_t newSize, MemoryTag tag);
SAPI void  SFree(void* block, uint64_t size, MemoryTag tag);
SAPI void  SFree(void* block, uint64_t size, MemoryTag tag);
SAPI void* SZero(void* block, uint64_t size);
SAPI void* SCopy(void* dest, const void* src, uint64_t size);
SAPI void* SSet(void* dest, int value, uint64_t size);


SAPI std::string GetMemoryUsage();

}