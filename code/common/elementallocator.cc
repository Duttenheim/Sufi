#include "elementallocator.h"
#include "common/mem.h"

struct IdPool
{
    VirtualMemoryAllocator allocator;
    size_t freeIdsSize;
    size_t freeIdsMaxSize;

    size_t ids;
};

//-------------------------------------------------------------------------
/**
*/
IdPool 
CreateAllocator(const IdPoolSetupInfo info)
{
    IdPool ret;

    VirtualMemoryAllocatorCreateInfo allocatorInfo;
    allocatorInfo.maxSize = info.maxSize * sizeof(Id);
    ret.allocator = CreateVirtualMemoryAllocator(allocatorInfo);
}

//-------------------------------------------------------------------------
/**
*/
void
DestroyAllocator(const IdPool* pool)
{
    DestroyVirtualMemoryAllocator(pool->allocator);
}

//-------------------------------------------------------------------------
/**
*/
bool
Allocate(const IdPool* pool, Id& id)
{
    if (pool.freeIdsSize > 0)
    {
        auto ids = (Id*)pool->allocator.data;
        id = ids[pool.freeIdsSize-1];
        pool.freeIdsSize--;
        return false;
    }
    else
    {
        id = pool.ids++;
        return true;
    }
}

//-------------------------------------------------------------------------
/**
*/
void
Free(const IdPool* pool,const Id id)
{
    assert(pool.freeIdsSize + 1 <= pool.freeIdsMaxSize);
    auto ids = (Id*)pool->allocator.data;

    // Make sure there is space to add the Id to free
    AllocateVirtualMemory(pool->allocator, sizeof(Id));
    ids[pool.freeIdsSize] = id;
    pool.freeIdsSize++;
}