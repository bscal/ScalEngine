#pragma once

#include "Core/Core.h"

namespace Scal
{
namespace Structures
{

struct SArray
{
	void* Memory;
	uint64_t Capacity;
	uint64_t Length;
	uint64_t Stride;
};

#define DEFAULT_INITIAL_SIZE 8
#define DEFAULT_RESIZE 2

SAPI SArray* _ArrayCreate(uint64_t capacity, uint64_t stride);
SAPI void  ArrayDestroy(SArray* sArray);

SAPI SArray* ArrayResize(SArray* array);

SAPI uint64_t GetArrayMemorySize(SArray* array);

//SAPI void _ArraySetField(void* array, uint64_t field, uint64_t value);
//SAPI uint64_t _ArraySetField(void* array, uint64_t field);

SAPI void* ArrayPush(SArray* array, const void* valuePtr);
SAPI void  ArrayPop(SArray* array, void* dest);

SAPI void* ArraySetAt(SArray* array, uint64_t index, const void* valuePtr);
SAPI void  ArrayGetAt(SArray* array, uint64_t index, void* dest);

#define SArrayCreate(type) _ArrayCreate(DEFAULT_INITIAL_SIZE, sizeof(type));
#define SArrayReserve(type, capacity) _ArrayCreate(capacity, sizeof(type));
#define SArrayPush(sArray, value) \
{ \
	auto tmp = value; \
	ArrayPush(sArray, &tmp); \
} \

#define SArrayPop(sArray, value) \
{ \
	auto tmp = value; \
	ArrayPush(sArray, &tmp); \
} \

}
}
