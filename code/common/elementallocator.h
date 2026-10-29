/**
    Element allocator interface

    Retrieves a new ID from a pool of available IDs, and informs the allocee if a previously unused ID was returned or not
*/ 
#include <cstdint>

typedef Id uint32_t;
struct IdPool;

struct IdPoolSetupInfo
{
    uint32_t maxElements;
};

/// Construct a new allocator
IdPool CreateAllocator(const IdPoolSetupInfo info);
/// Destroy allocator
void DestroyAllocator(IdPool* pool);

/// Allocates a new ID, returns true if a new value was allocated
bool Allocate(IdPool* pool, Id& id);
/// Frees an ID
void Free(IdPool* pool, const Id id);