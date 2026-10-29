#include "common/system.h"
#include "common/mem.h"
#include "common/elementallocator.h"

int main(int argc, char** argv)
{
    SystemSetup();
    
    // Create new pool of IDs
    IdPoolSetupInfo poolInfo;
    poolInfo.maxElements = 2048;
    IdPool pool = CreateIdPool(poolInfo);
    
    // Allocate new ID
    Id newId;
    IdPoolAllocate(&pool, newId);
    assert(newId == 0);
    
    // Now free it and realloc, it should be the same ID
    IdPoolFree(&pool, newId);
    IdPoolAllocate(&pool, newId);
    assert(newId == 0);
}
