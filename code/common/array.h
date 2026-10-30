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
    size_t _itemSize;
    size_t size;
    size_t capacity;
    size_t maxCapacity;
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

/// Push typed item to array and return typed pointer
template<typename T>
inline T* ArrayPush(Array* arr, const T& t)
{
    void* ptr = ArrayPush(arr);
    T* typedPtr = static_cast<T*>(ptr);
    *typedPtr = t;
    return typedPtr;
}

/// Get typed item from array using index
template<typename T>
inline const T& ArrayGet(Array* arr, size_t index)
{
    return ((T*)arr->allocator.data)[index];
}

/// Get typed pointer from array using index
template<typename T>
inline T* ArrayGetPtr(Array* arr, size_t index)
{
    return &((T*)arr->allocator.data)[index];
}
