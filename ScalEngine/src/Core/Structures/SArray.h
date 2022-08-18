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

SAPI SArray* ArrayCreate(uint64_t capacity, uint64_t stride);
SAPI void ArrayDestroy(SArray* sArray);

SAPI SArray* ArrayResize(SArray* array);
SAPI constexpr uint64_t GetArrayMemorySize(SArray* array);

SAPI SArray* ArrayPush(SArray* array, const void* valuePtr);
SAPI void  ArrayPop(SArray* array, void* dest);
SAPI void* ArraySetAt(SArray* array, uint64_t index, const void* valuePtr);
SAPI void  ArrayPopAt(SArray* array, uint64_t index, void* dest);
SAPI void* ArrayPeekAt(SArray* array, uint64_t index);

#define SArrayCreate(type) ArrayCreate(DEFAULT_INITIAL_SIZE, sizeof(type));
#define SArrayReserve(type, capacity) ArrayCreate(capacity, sizeof(type));
#define SArrayPush(sArray, value) \
{ \
	auto tmp = value; \
	ArrayPush(sArray, &tmp); \
} \

}
}
