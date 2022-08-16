#include "SMemory.h"

#include <string>

#include "Logger.h"
#include "Platform/Platform.h"

namespace Scal
{

struct MemoryStats
{
	uint64_t TotalMemAllocated;
	uint64_t TaggedAllocations[(int)MemoryTag::MaxTags];
};

global_var MemoryStats Stats;

void InitializeMemory()
{
	Stats = { 0 };
}

void ShutdownMemory()
{

}

void* SAlloc(uint64_t size, MemoryTag tag)
{
	if (tag == MemoryTag::Unknown)
	{
		SWARN("SAlloc called with tag: MemoryTag::Unknown!");
	}

	Stats.TotalMemAllocated += size;
	Stats.TaggedAllocations[(int)tag] += size;

	return Platform::Allocate(size, false);
}

void* SRealloc(void* address, uint64_t oldSize, uint64_t newSize, MemoryTag tag)
{
	if (tag == MemoryTag::Unknown)
	{
		SWARN("SAlloc called with tag: MemoryTag::Unknown!");
	}

	Stats.TotalMemAllocated -= oldSize;
	Stats.TotalMemAllocated += newSize;
	Stats.TaggedAllocations[(int)tag] -= oldSize;
	Stats.TaggedAllocations[(int)tag] += newSize;
	return Platform::Realloc(address, newSize);
}

void SFree(void* address, uint64_t size, MemoryTag tag)
{
	if (tag == MemoryTag::Unknown)
	{
		SWARN("SAlloc called with tag: MemoryTag::Unknown!");
	}
	Stats.TotalMemAllocated -= size;
	Stats.TaggedAllocations[(int)tag] -= size;
	Platform::Free(address, false);
}

void* SZero(void* address, uint64_t size)
{
	return Platform::ZeroMem(address, size);
}

void* SCopy(void* dest, const void* src, uint64_t size)
{
	return Platform::CopyMem(dest, src, size);
}

void* SSet(void* dest, int value, uint64_t size)
{
	return Platform::SetMem(dest, value, size);
}

struct MemorySizeData
{
	float MemorySize;
	const char* Suffix;
};

constexpr global_var const char* MemoryTagStrings[(size_t)MemoryTag::MaxTags] =
{
	"Unknown",
	"Array",
	"DArray",
	"Dictionary",
	"RingQueue",
	"BST",
	"String",
	"Application",
	"Job",
	"Texture",
	"MaterialInstance",
	"Renderer",
	"Game",
	"Transform",
	"Entity",
	"EntityNode",
	"Scene"
};

constexpr MemorySizeData FindMemSize(uint64_t size)
{
	const uint64_t gb = 1024 * 1024 * 1024;
	const uint64_t mb = 1024 * 1024;
	const uint64_t kb = 1024;

	if (size > gb)
		return { (float)size / (float)gb, "GB" };
	else if (size > mb)
		return { (float)size / (float)mb, "MB" };
	else if (size > kb)
		return { (float)size / (float)kb, "KB" };
	else
		return { (float)size, "B" };
}

internal std::string FormatFloat(uint64_t value)
{
	MemorySizeData sizeData0 = FindMemSize(value);
	std::string s(16, '\0');
	auto written = std::snprintf(&s[0], s.size(), "%.2f%s",
		sizeData0.MemorySize, sizeData0.Suffix);
	s.resize(written);
	return s;
}

std::string GetMemoryUsage()
{
	std::string outputMessage;
	outputMessage.reserve(1024);
	outputMessage.append("System Memory Usage\n");
	outputMessage.append("Total Memory: ");
	outputMessage.append(std::to_string(Stats.TotalMemAllocated));
	outputMessage.append("B, ");
	outputMessage.append(FormatFloat(Stats.TotalMemAllocated));
	outputMessage.append("\n");
	for (uint32_t i = 0; i < (uint32_t)MemoryTag::MaxTags; ++i)
	{
		outputMessage.append(MemoryTagStrings[i]);
		outputMessage.append(": ");
		outputMessage.append(FormatFloat(Stats.TaggedAllocations[i]));
		outputMessage.append("\n");
	}
	return outputMessage;
}
}