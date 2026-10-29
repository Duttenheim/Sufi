#pragma once
/**
    Element allocator interface

    Retrieves a new ID from a pool of available IDs, and informs the allocee if a previously unused ID was returned or not

*/
#include <cstdint>
#include "mem.h"

typedef uint32_t Id;

struct IdPool
{
    VirtualMemoryAllocator allocator;
    size_t freeIdsSize;
    size_t freeIdsMaxSize;

    size_t ids;
};

struct IdPoolSetupInfo
{
    uint32_t maxElements;
};

/// Construct a new allocator
IdPool CreateIdPool(const IdPoolSetupInfo info);
/// Destroy allocator
void DestroyIdPool(IdPool* pool);

/// Allocates a new ID, returns true if a new value was allocated
bool IdPoolAllocate(IdPool* pool, Id& id);
/// Frees an ID
void IdPoolFree(IdPool* pool, const Id id);
