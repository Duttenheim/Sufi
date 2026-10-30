#include "mem.h"
#include "elementallocator.h"
#include <cmath>
#include <algorithm>

#if WINDOWS
#else
    #include <sys/mman.h>
    #include <string.h>
#endif

//-------------------------------------------------------------------------
/**
*/
void*
vmalloc(size_t size)
{
#if __WIN32__
    return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
#else
    return mmap(nullptr, std::max(size, SystemPageSize), PROT_NONE, MAP_ANON | MAP_PRIVATE, 0, 0);
#endif
}

//-------------------------------------------------------------------------
/**
*/
void
vcommit(void* mem, size_t size)
{
    assert(size >= SystemPageSize);
#if __WIN32__
    auto ret = VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
#else
    auto ret = mprotect(mem, size, PROT_READ | PROT_WRITE);
    assert(ret == 0);
    memset(mem, 0x0, size);
#endif
}

//-------------------------------------------------------------------------
/**
*/
void
vdecommit(void* mem, size_t size)
{
#if __WIN32__
#else
    auto ret = madvise(mem, size, MADV_DONTNEED);
    assert(ret == 0);
    ret = mprotect(mem, size, PROT_NONE);
    assert(ret == 0);
#endif
}

//-------------------------------------------------------------------------
/**
*/
void
vfree(void* mem, size_t size)
{
#if __WIN32__
    auto ret = VirtualFree(ptr, 0, MEM_RELEASE);
    assert(ret != nullptr)
#else
    auto ret = madvise(mem, size, MADV_DONTNEED);
    assert(ret == 0);
    ret = munmap(mem, size);
    assert(ret == 0);
#endif
}

//-------------------------------------------------------------------------
/**
*/
size_t
align(size_t alignant, size_t alignment)
{
    return (alignant + alignment - 1) & ~(alignment - 1);
}

//-------------------------------------------------------------------------
/**
*/
VirtualMemoryAllocator 
CreateVirtualMemoryAllocator(const VirtualMemoryAllocatorCreateInfo info)
{
    VirtualMemoryAllocator ret;
    ret.data = vmalloc(info.maxSize);
    ret.size = 0;
    ret.capacity = 0;
    ret.maxSize = info.maxSize;
    return ret;
}

//-------------------------------------------------------------------------
/**
*/
void
DestroyVirtualMemoryAllocator(VirtualMemoryAllocator* allocator)
{
    vfree(allocator->data, allocator->maxSize);
}

//-------------------------------------------------------------------------
/**
*/
void 
AllocateVirtualMemory(VirtualMemoryAllocator* allocator, size_t size, size_t alignment)
{
    assert(SystemPageSize != 0);

    // Determine the number of pages needed
    size_t numCommitedPages = allocator->capacity / SystemPageSize;
    size_t numPagesNeeded = ceil((align(allocator->size + size, alignment) / float(SystemPageSize)));

    // If we need more space, commit more memory
    if (numCommitedPages < numPagesNeeded)
    {
        size_t newPagesNeeded = numPagesNeeded - numCommitedPages;
        vcommit((byte*)allocator->data + allocator->capacity, newPagesNeeded * SystemPageSize);
        allocator->capacity += newPagesNeeded * SystemPageSize;
    }
    allocator->size += size;
}

//-------------------------------------------------------------------------
/**
*/
void 
DeallocateVirtualMemory(VirtualMemoryAllocator* allocator, size_t size)
{
    size_t numPagesNeeded = ceil(size / float(SystemPageSize));
    size_t numCommitedPages = allocator->capacity / SystemPageSize;
    size_t startPage = (allocator->capacity / SystemPageSize) - numPagesNeeded;
    vdecommit((byte*)allocator->data + startPage * SystemPageSize, size);
    allocator->capacity = (numCommitedPages - numPagesNeeded) * SystemPageSize;
}
