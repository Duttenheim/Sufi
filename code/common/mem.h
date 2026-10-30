#pragma once
#include <cstddef>
#include <assert.h>
#include "system.h"

typedef char byte;

/// Allocate virtual memory space
void* vmalloc(size_t maxSize);
/// Commit region of virtual memory
void vcommit(void* mem, size_t size);
/// Decommit virtual memory
void vdecommit(void* mem, size_t size);
/// Free virtual memory (and automatically decommit)
void vfree(void* mem, size_t size);

/// Align size to nearest power of two
size_t align(size_t alignant, size_t alignment);

struct VirtualMemoryAllocator
{
    void* data;
    size_t maxSize;
    size_t size;
    size_t capacity;
};

struct VirtualMemoryAllocatorCreateInfo
{
    size_t maxSize;
};

/// Create a new virtual memory allocator
VirtualMemoryAllocator CreateVirtualMemoryAllocator(const VirtualMemoryAllocatorCreateInfo info);
/// Destroy a virtual memory allocator, free it's memory
void DestroyVirtualMemoryAllocator(VirtualMemoryAllocator* allocator);
/// Grow the committed memory range, commits new pages as needed
void AllocateVirtualMemory(VirtualMemoryAllocator* allocator, size_t size, size_t alignment = 1);
/// Deallocate virtual memory, decommits pages at the tail end if the new size requires fewer pages than before
void DeallocateVirtualMemory(VirtualMemoryAllocator* allocator, size_t size);
