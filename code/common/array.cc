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
    ret.maxCapacity = info.maxNumItems * info.itemSize;
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
    DestroyVirtualMemoryAllocator(&arr->allocator);
    arr->size = 0;
    arr->capacity = 0;
    arr->maxCapacity = 0;
    arr->allocator = VirtualMemoryAllocator();
}

//-------------------------------------------------------------------------
/**
*/
void*
ArrayPush(Array* arr)
{
    assert(arr->size + 1 < arr->maxCapacity);

    // Ensure space is committed
    AllocateVirtualMemory(&arr->allocator, arr->itemSize);
    void* ret = arr->allocator.data + arr->size * arr->itemSize;
    arr->size++;
    return ret;
}

//-------------------------------------------------------------------------
/**
*/
void
ArrayPop(Array* arr)
{
    assert(arr->size > 0);
    arr->size--;
}

//-------------------------------------------------------------------------
/**
*/
void
ArrayDeleteIndex(Array* arr, size_t index)
{
    assert(index < arr->size);
    void* ptr = arr->allocator.data + index * arr->itemSize;
    arr->size--;

    // Shift elements to the left
    memmove(ptr, ptr + arr->itemSize, (arr->size - index) * arr->itemSize);
}

//-------------------------------------------------------------------------
/**
*/
void
ArrayDeletePtr(Array* arr, void* ptr)
{
    assert(ptr >= arr->allocator.data && ptr < arr->allocator.data + arr->maxCapacity);
    size_t index = (ptr - arr->allocator.data) / arr->itemSize;
    ArrayDeleteIndex(arr, index);
}
