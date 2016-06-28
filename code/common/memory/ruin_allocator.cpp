#include "ruin_allocator.h"

using namespace Ruin;

rsAllocator Ruin::rCreateAllocator(void* pool, rfAllocFunc alloc, rfReleaseFunc release)
{
    return { pool, alloc, release };
}