#include "elementallocator.h"

//-------------------------------------------------------------------------
/**
*/
IdPool 
CreateIdPool(const IdPoolSetupInfo info)
{
    IdPool ret;

    ret.freeIdsSize = 0;
    ret.freeIdsMaxSize = info.maxElements;
    ret.ids = 0;
    VirtualMemoryAllocatorCreateInfo allocatorInfo;
    allocatorInfo.maxSize = info.maxElements * sizeof(Id);
    ret.allocator = CreateVirtualMemoryAllocator(allocatorInfo);
    return ret;
}

//-------------------------------------------------------------------------
/**
*/
void
DestroyIdPool(IdPool* pool)
{
    DestroyVirtualMemoryAllocator(&pool->allocator);
}

//-------------------------------------------------------------------------
/**
*/
bool
IdPoolAllocate(IdPool* pool, Id& id)
{
    if (pool->freeIdsSize > 0)
    {
        auto ids = (Id*)pool->allocator.data;
        id = ids[pool->freeIdsSize-1];
        pool->freeIdsSize--;
        return false;
    }
    else
    {
        id = pool->ids++;
        return true;
    }
}

//-------------------------------------------------------------------------
/**
*/
void
IdPoolFree(IdPool* pool,const Id id)
{
    assert(pool->freeIdsSize + 1 <= pool->freeIdsMaxSize);
    auto ids = (Id*)pool->allocator.data;

    // Make sure there is space to add the Id to free
    AllocateVirtualMemory(&pool->allocator, sizeof(Id));
    ids[pool->freeIdsSize] = id;
    pool->freeIdsSize++;
}
