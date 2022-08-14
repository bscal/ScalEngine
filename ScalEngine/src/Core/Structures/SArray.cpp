#include "SArray.h"

#include "Core/SAssert.h"
#include "Core/Logger.h"
#include "Core/SMemory.h"

namespace Scal
{
namespace Structures
{
SAPI SArray* _ArrayCreate(uint64_t capacity, uint64_t stride)
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
	void* newMemory = SAlloc(newMemorySizeInBytes, MemoryTag::DArray);
	SZero(newMemory, newMemorySizeInBytes);
	SCopy(newMemory, array->Memory, oldMemorySizeInBytes);
	SFree(array->Memory, oldMemorySizeInBytes, MemoryTag::DArray);
	array->Memory = newMemory;
	return array;
}

SAPI uint64_t GetArrayMemorySize(SArray* array)
{
	return array->Capacity * array->Stride;
}

SAPI void* ArrayPush(SArray* array, const void* valuePtr)
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
	SCopy(dest, &array[offset], array->Stride);
	--array->Length;
}

SAPI void* ArraySetAt(SArray* array, uint64_t index, const void* valuePtr)
{
	SASSERT(index < array->Length);
	uint64_t offset = index * array->Stride;
	return SCopy(&array[offset], valuePtr, array->Stride);
}

SAPI void ArrayGetAt(SArray* array, uint64_t index, void* dest)
{
	uint64_t offset = index * array->Stride;
	SCopy(dest, &array[offset], array->Stride);
}

}
}