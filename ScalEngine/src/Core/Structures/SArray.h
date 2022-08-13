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

	constexpr void* operator[](int index) const;
};

#define DEFAULT_INITIAL_SIZE 8
#define DEFAULT_RESIZE 2

SAPI SArray* _ArrayCreate(uint64_t length, uint64_t stride);
SAPI void  _ArrayDestroy(SArray* sArray);

SAPI SArray* _ArrayResize(SArray* array);

//SAPI void _ArraySetField(void* array, uint64_t field, uint64_t value);
//SAPI uint64_t _ArraySetField(void* array, uint64_t field);

SAPI void* _ArrayPush(SArray* array, const void* valuePtr);
SAPI void  _ArrayPop(SArray* array, void* dest);

SAPI void* _ArraySetAt(SArray* array, uint64_t index, const void* valuePtr);
SAPI void  _ArrayGetAt(SArray* array, uint64_t index, void* dest);

#define SArrayCreate(type) _ArrayCreate(DEFAULT_INITIAL_SIZE, sizeof(type));
#define SArrayReserve(array, capacity) _ArrayCreate(capacity, sizeof(type));
#define SArrayDestroy(array) _ArrayDestroy(array);
#define SArrayPush(array, value) \
{ \
	typeof(value) tmp = value; \
	array = _ArrayPush(array, &tmp); \
} \

}
}
