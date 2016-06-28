#ifndef __RUIN_ALLOCATOR_
#define __RUIN_ALLOCATOR_

#include "ruin_int.h"

namespace Ruin
{
    typedef void* (*rfAllocFunc)(void*, size_t);
    typedef void (*rfReleaseFunc)(void*, void*);

    /**
     * Allocator struct
     * Generic alloctor object used to allocate objects from a particular memory pool.
     */
    struct rsAllocator
    {
        void* pool;
        rfAllocFunc alloc;
        rfReleaseFunc release;
    };

    rsAllocator rCreateAllocator(void* pool, rfAllocFunc alloc, rfReleaseFunc release);

    inline void* rAlloc(rsAllocator &allocator, size_t bytes)
    {
        return allocator.alloc(allocator.pool, bytes);
    }

    inline void rRelease(rsAllocator &allocator, void* ptr)
    {
        return allocator.release(allocator.pool, ptr);
    }
}

#endif