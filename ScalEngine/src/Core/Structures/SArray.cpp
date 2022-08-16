#include "SArray.h"

#include "Core/SAssert.h"
#include "Core/Logger.h"
#include "Core/SMemory.h"

namespace Scal
{
namespace Structures
{
SAPI SArray* ArrayCreate(uint64_t capacity, uint64_t stride)
{
	// TODO think how we want to create SArray struct.
	SArray* sArray = (SArray*)SAlloc(sizeof(SArray), MemoryTag::DArray);
	sArray->Memory = (SArray*)SAlloc(capacity * stride, MemoryTag::DArray);
	sArray->Capacity = capacity;
	sArray->Length = 0;
	sArray->Stride = stride;
	return sArray;
}

SAPI void ArrayDestroy(SArray* array)
{
	SFree(array, GetArrayMemorySize(array), MemoryTag::DArray);
}

SAPI SArray* ArrayResize(SArray* array)
{
	uint64_t oldMemorySizeInBytes = GetArrayMemorySize(array);
	array->Capacity *= DEFAULT_RESIZE;
	uint64_t newMemorySizeInBytes = GetArrayMemorySize(array);
	SRealloc(array->Memory,
		oldMemorySizeInBytes,
		newMemorySizeInBytes,
		MemoryTag::DArray);
	return array;
}

SAPI constexpr uint64_t GetArrayMemorySize(SArray* array)
{
	return array->Capacity * array->Stride;
}

SAPI SArray* ArrayPush(SArray* array, const void* valuePtr)
{
	if (array->Length >= array->Capacity)
	{
		ArrayResize(array);
	}
	SASSERT(array->Length < array->Capacity);
	uint64_t offset = array->Length * array->Stride;
	char* memory = (char*)array->Memory;
	SCopy(memory + offset, valuePtr, array->Stride);
	++array->Length;
	return array;
}

SAPI void ArrayPop(SArray* array, void* dest)
{
	if (array->Length == 0) return;
	uint64_t offset = (array->Length - 1) * array->Stride;
	const char* mem = (char*)(array->Memory);
	SCopy(dest, &mem[offset], array->Stride);
	--array->Length;
}

SAPI void* ArraySetAt(SArray* array, uint64_t index, const void* valuePtr)
{
	SASSERT(index < array->Length);
	uint64_t offset = index * array->Stride;
	char* mem = (char*)(array->Memory);
	void* dest = &mem[offset];
	SCopy(&dest, valuePtr, array->Stride);
	return dest;
}

SAPI void ArrayPopAt(SArray* array, uint64_t index, void* dest)
{
	SASSERT(index < array->Length);
	uint64_t offset = index * array->Stride;
	char* mem = (char*)(array->Memory);
	SCopy(dest, &mem[offset], array->Stride);

	if (index != array->Length)
	{
		// Moves last element in array popped position
		SCopy(&mem[offset], &mem[array->Length * array->Stride], array->Stride);
	}

	--array->Length;
}

SAPI void* ArrayPeekAt(SArray* array, uint64_t index)
{
	char* mem = (char*)array->Memory;
	return &mem[index * array->Stride];
}

}
}