//
//  array.cc
//  Sufi
//
//  Created by Gustav Sterbrant on 2026-10-29.
//

#include "array.h"

//-------------------------------------------------------------------------
/**
*/
Array
CreateArray(const ArrayCreateInfo& info)
{
    Array ret;
    ret.size = 0;
    ret.capacity = 0;
    
    VirtualMemoryAllocatorCreateInfo allocInfo;
    allocInfo.maxSize = info.maxNumItems * info.itemSize;
    ret.allocator = CreateVirtualMemoryAllocator(allocInfo);
    return ret;
}

//-------------------------------------------------------------------------
/**
*/
void
DestroyArray(Array* arr)
{
    
}

//-------------------------------------------------------------------------
/**
*/
void*
ArrayPush(Array* arr)
{
    assert(arr->size + 1 < arr->)
}

//-------------------------------------------------------------------------
/**
*/
void
ArrayPop(Array* arr)
{
    
}

//-------------------------------------------------------------------------
/**
*/
void
ArrayDeleteIndex(Array* arr, size_t index)
{
    
}

//-------------------------------------------------------------------------
/**
*/
void
ArrayDeletePtr(Array* arr, void* ptr)
{
    
}
