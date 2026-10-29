#pragma once
#include <cstddef>

extern size_t SystemPageSize;
extern size_t SystemAllocationGranularity;
extern size_t SystemCacheLineSize;
extern size_t SystemLogicalCoreCount;
extern size_t SystemPhysicalCoreCount;
extern size_t SystemPhysicalMemory;

void SystemSetup();
