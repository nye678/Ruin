#ifndef __RUIN_MEMORY_
#define __RUIN_MEMORY_

#include "ruin_int.h"
#include "ruin_allocator.h"

bool InitGlobalMemory();
void ReleaseGlobalMemory();

void* Alloc(size_t size);
void Release(void* ptr);

struct StackAllocator
{
    uintptr_t head;
    uintptr_t tail;
    uintptr_t current;
};

StackAllocator CreateStackAllocator(size_t size);
void* StackAlloc(StackAllocator&, size_t size);
void StackRewind(StackAllocator&, void* ptr);
void StackRelease(StackAllocator&);

inline uintptr_t AlignPointerNext(uintptr_t ptr, size_t align)
{
    return (ptr + align - 1) & ~(uintptr_t)(align - 1);
}

inline void* OffsetPointer(void* ptr, size_t offset)
{
    return (void*)(((uintptr_t)ptr) + offset);
}

void MemCopy(void* &_dest, const void* _source, size_t bytes);

namespace Ruin
{
    template <typename T, typename... Args>
    T* rInstantiate(rsAllocator allocator, Args... args)
    {
        T* obj = (T*)rAlloc(allocator, sizeof(T)); 
        return obj;
    }
}

#endif