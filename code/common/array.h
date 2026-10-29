#pragma once
//
//  array.h
//  Sufi
//
//  An dynamic array type which allows for a growing collection of items. Uses virtual memory
//  and pinned memory to maintain stable pointers
//
//  Created by Gustav Sterbrant on 2026-10-29.
//

#include "mem.h"
struct ArrayCreateInfo
{
    size_t itemSize;
    size_t maxNumItems;
};

struct Array
{
    size_t size;
    size_t capacity;
    VirtualMemoryAllocator allocator;
};

/// Create a new array
Array CreateArray(const ArrayCreateInfo& info);
/// Destroy array and free memory
void DestroyArray(Array* arr);

/// Push item to end of array, return pointer to address
void* ArrayPush(Array* arr);
/// Pop item at end of array
void ArrayPop(Array* arr);
/// Remove item from array using index
void ArrayDeleteIndex(Array* arr, size_t index);
/// Remove item from array using pointer
void ArrayDeletePtr(Array* arr, void* ptr);
