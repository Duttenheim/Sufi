#include "system.h"

#include <cstdlib>

struct SystemInfo
{
    size_t pageSize;
    size_t allocationGranularity;
    size_t cacheLineSize;
    size_t logicalCoreCount;
    size_t physicalCoreCount;
    size_t physicalMemory;
};

#if WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif MACOS
#include <sys/sysctl.h>
#include <unistd.h>
#elif LINUX
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#endif

size_t SystemPageSize;
size_t SystemAllocationGranularity;
size_t SystemCacheLineSize;
size_t SystemLogicalCoreCount;
size_t SystemPhysicalCoreCount;
size_t SystemPhysicalMemory;

#if WINDOWS
static void QueryProcessorTopology(SystemInfo* info)
{
    info->physicalCoreCount = 0;
    info->cacheLineSize = 64;

    DWORD length = 0;
    GetLogicalProcessorInformation(nullptr, &length);
    if (length == 0)
        return;

    auto* entries = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(length);
    if (entries == nullptr)
        return;
    if (!GetLogicalProcessorInformation(entries, &length))
    {
        free(entries);
        return;
    }

    size_t count = length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
    for (size_t i = 0; i < count; i++)
    {
        if (entries[i].Relationship == RelationProcessorCore)
            info->physicalCoreCount++;
        else if (entries[i].Relationship == RelationCache && entries[i].Cache.Level == 1 && entries[i].Cache.LineSize != 0)
            info->cacheLineSize = entries[i].Cache.LineSize;
    }
    free(entries);
}
#endif

#if LINUX
static size_t LinuxPhysicalCoreCount(size_t logicalCores)
{
    FILE* file = fopen("/proc/cpuinfo", "r");
    if (file == nullptr || logicalCores == 0)
    {
        if (file != nullptr)
            fclose(file);
        return logicalCores;
    }

    auto* keys = (uint64_t*)malloc(sizeof(uint64_t) * logicalCores);
    if (keys == nullptr)
    {
        fclose(file);
        return logicalCores;
    }

    size_t count = 0;
    unsigned physicalId = 0;
    unsigned coreId = 0;
    bool havePhysical = false;
    bool haveCore = false;
    char line[256];
    while (fgets(line, sizeof(line), file) != nullptr)
    {
        if (strncmp(line, "physical id", 11) == 0)
        {
            char* colon = strchr(line, ':');
            if (colon != nullptr)
            {
                physicalId = (unsigned)strtoul(colon + 1, nullptr, 10);
                havePhysical = true;
            }
        }
        else if (strncmp(line, "core id", 7) == 0)
        {
            char* colon = strchr(line, ':');
            if (colon != nullptr)
            {
                coreId = (unsigned)strtoul(colon + 1, nullptr, 10);
                haveCore = true;
            }
        }

        if (line[0] == '\n' && havePhysical && haveCore)
        {
            uint64_t key = ((uint64_t)physicalId << 32) | coreId;
            bool found = false;
            for (size_t i = 0; i < count; i++)
            {
                if (keys[i] == key)
                {
                    found = true;
                    break;
                }
            }
            if (!found && count < logicalCores)
                keys[count++] = key;
            havePhysical = false;
            haveCore = false;
        }
    }

    fclose(file);
    free(keys);
    return count == 0 ? logicalCores : count;
}
#endif

static SystemInfo QuerySystemInfo()
{
    SystemInfo info{};

#if WINDOWS
    SYSTEM_INFO sys;
    GetSystemInfo(&sys);
    info.pageSize = sys.dwPageSize;
    info.allocationGranularity = sys.dwAllocationGranularity;
    info.logicalCoreCount = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);

    MEMORYSTATUSEX memory{};
    memory.dwLength = sizeof(memory);
    if (GlobalMemoryStatusEx(&memory))
        info.physicalMemory = memory.ullTotalPhys;

    QueryProcessorTopology(&info);
    if (info.physicalCoreCount == 0)
        info.physicalCoreCount = info.logicalCoreCount;
#elif MACOS
    int pageSize = 0;
    size_t size = sizeof(pageSize);
    sysctlbyname("hw.pagesize", &pageSize, &size, nullptr, 0);
    info.pageSize = pageSize > 0 ? (size_t)pageSize : (size_t)sysconf(_SC_PAGESIZE);
    info.allocationGranularity = info.pageSize;

    int cacheLine = 0;
    size = sizeof(cacheLine);
    if (sysctlbyname("hw.cachelinesize", &cacheLine, &size, nullptr, 0) == 0 && cacheLine > 0)
        info.cacheLineSize = (size_t)cacheLine;
    else
        info.cacheLineSize = 64;

    int logical = 0;
    size = sizeof(logical);
    sysctlbyname("hw.logicalcpu", &logical, &size, nullptr, 0);
    info.logicalCoreCount = logical > 0 ? (size_t)logical : 1;

    int physical = 0;
    size = sizeof(physical);
    sysctlbyname("hw.physicalcpu", &physical, &size, nullptr, 0);
    info.physicalCoreCount = physical > 0 ? (size_t)physical : info.logicalCoreCount;

    uint64_t memory = 0;
    size = sizeof(memory);
    sysctlbyname("hw.memsize", &memory, &size, nullptr, 0);
    info.physicalMemory = memory;
#elif LINUX
    long pageSize = sysconf(_SC_PAGESIZE);
    info.pageSize = pageSize > 0 ? (size_t)pageSize : 4096;
    info.allocationGranularity = info.pageSize;

#if defined(_SC_LEVEL1_DCACHE_LINESIZE)
    long cacheLine = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    info.cacheLineSize = cacheLine > 0 ? (size_t)cacheLine : 64;
#else
    info.cacheLineSize = 64;
#endif

    long logical = sysconf(_SC_NPROCESSORS_ONLN);
    info.logicalCoreCount = logical > 0 ? (size_t)logical : 1;
    info.physicalCoreCount = LinuxPhysicalCoreCount(info.logicalCoreCount);

    long pages = sysconf(_SC_PHYS_PAGES);
    if (pages > 0)
        info.physicalMemory = (size_t)pages * info.pageSize;
#endif

    return info;
}

//-------------------------------------------------------------------------
/**
*/
void SystemSetup()
{
    SystemInfo info = QuerySystemInfo();
    SystemPageSize = info.pageSize;
    SystemAllocationGranularity = info.allocationGranularity;
    SystemCacheLineSize = info.cacheLineSize;
    SystemLogicalCoreCount = info.logicalCoreCount;
    SystemPhysicalCoreCount = info.physicalCoreCount;
    SystemPhysicalMemory = info.physicalMemory;
}

struct SystemGlobalsInit
{
    SystemGlobalsInit()
    {
        SystemSetup();
    }
};

static SystemGlobalsInit SystemGlobalsInitInstance;
