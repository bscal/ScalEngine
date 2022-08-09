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
        Stats = {0};
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

        void* block = Allocate(size, false);
        return block;
    }

    void SFree(void* address, uint64_t size, MemoryTag tag)
    {
        if (tag == MemoryTag::Unknown)
        {
            SWARN("SAlloc called with tag: MemoryTag::Unknown!");
        }
        Stats.TotalMemAllocated -= size;
        Stats.TaggedAllocations[(int)tag] -= size;
        Free(address, false);        
    }

    void* SZero(void* address, uint64_t size)
    {
        return ZeroMem(address, size);
    }

    void* SCopy(void* dest, const void* src, uint64_t size)
    {
        return CopyMem(dest, src, size);
    }

    void* SSet(void* dest, int value, uint64_t size)
    {
        return SetMem(dest, value, size);
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

    std::string GetMemoryUsage()
    {
        std::string outputMessage;
        outputMessage.append("System Memory Usage:\n");
        for (uint32_t i = 0; i < (uint32_t) MemoryTag::MaxTags; ++i)
        {
            outputMessage.append(MemoryTagStrings[i]);
            outputMessage.append(": ");

            MemorySizeData sizeData = FindMemSize(Stats.TaggedAllocations[i]);
            std::string memSize(16, '\0');
            auto written = std::snprintf(&memSize[0], memSize.size(), "%.2f", sizeData.MemorySize);
            memSize.resize(written);
            outputMessage.append(memSize);
            outputMessage.append(sizeData.Suffix);
            outputMessage.append("\n");
        }
        return outputMessage;
    }
}