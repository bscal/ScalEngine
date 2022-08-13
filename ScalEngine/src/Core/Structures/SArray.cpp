#include "SArray.h"

#include "Core/SAssert.h"
#include "Core/SMemory.h"

namespace Scal
{
namespace Structures
{

constexpr void* SArray::operator[](int index) const
{
	SASSERT(index < Length);
	SASSERT(Memory);
	return (void*)((char*)Memory)[index * Stride];
}

SAPI SArray* _ArrayCreate(uint64_t length, uint64_t stride)
{
	uint64_t capacity = length * stride;
	SArray* sArray = (SArray*)SAlloc(capacity, MemoryTag::DArray);
	sArray->Memory = SZero(sArray, capacity);
	sArray->Capacity = capacity;
	sArray->Length = 0;
	sArray->Stride = stride;
	return sArray;
}

SAPI void _ArrayDestroy(SArray* array)
{
	SFree(array, array->Length, MemoryTag::DArray);
}

SAPI SArray* _ArrayResize(SArray* array)
{
	uint64_t newCapicty = array->Capacity * DEFAULT_RESIZE;
	SArray* newArray = _ArrayCreate(newCapicty, array->Stride);
	newArray->Length = array->Length;
	SCopy(newArray, array, array->Length);
	_ArrayDestroy(array);
	return newArray;
}

SAPI void* _ArrayPush(SArray* array, const void* valuePtr)
{
	return SCopy(&array[array->Length], valuePtr, array->Stride);
}

SAPI void _ArrayPop(SArray* array, void* dest)
{
	uint64_t index = (array->Length < 2) ? 0 : array->Length - 1;
	SCopy(dest, &array[index], array->Stride);
}

SAPI void* _ArraySetAt(SArray* array, uint64_t index, const void* valuePtr)
{
	return SCopy(&array[index], valuePtr, array->Stride);
}

SAPI void  _ArrayGetAt(SArray* array, uint64_t index, void* dest)
{
	SCopy(dest, &array[index], array->Stride);
}

}
}