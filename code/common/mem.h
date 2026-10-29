
#include <cstdint>

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

extern size_t SystemPageSize;

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

VirtualMemoryAllocator CreateVirtualMemoryAllocator(const VirtualMemoryAllocatorCreateInfo info);
void AllocateVirtualMemory(VirtualMemoryAllocator* allocator, size_t size);
void DeallocateVirtualMemory(VirtualMemoryAllocator* allocator, size_t size);