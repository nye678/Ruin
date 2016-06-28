#include "ruin_memory.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

typedef uint32_t BoundaryTag;

#define BLOCK_LENGTH(tag_ptr) (*tag_ptr & ~0x01)
#define BLOCK_FREE(tag_ptr) (*tag_ptr & 0x00)
#define SET_BLOCK_FREE(tag_ptr) (*tag_ptr &= ~0x01)
#define SET_BLOCK_ALLOCATED(tag_ptr) (*tag_ptr |= 0x01)

#define BLOCK_TAG(ptr) ((BoundaryTag*)((uintptr_t)ptr - sizeof(BoundaryTag)))
#define PREV_BLOCK_TAG(ptr) ((BoundaryTag*)((uintptr_t)ptr - (2 * sizeof(BoundaryTag))))
#define BLOCK_END_TAG(ptr, size) ((BoundaryTag*)((uintptr_t)ptr + (uintptr_t)size))
#define NEXT_BLOCK_TAG(ptr, size) ((BoundaryTag*)((uintptr_t)ptr + (uintptr_t)size + sizeof(BoundaryTag)))

#define BLOCK_OVERHEAD_SIZE 2 * sizeof(BoundaryTag) + sizeof(FreeList)

struct FreeList
{
    FreeList* next;
};

struct Allocator
{
    uintptr_t head;
    uintptr_t tail;

    size_t capacity;
    size_t allocated;
    size_t num_blocks;

    FreeList* free_list;
};

Allocator* g_allocator = nullptr;

void SetBoundaryTagsAlloc(uintptr_t ptr, size_t block_size)
{
    auto start_tag = BLOCK_TAG(ptr);
    auto end_tag = BLOCK_END_TAG(ptr, block_size);
    *start_tag = (BoundaryTag)block_size;
    *end_tag = (BoundaryTag)block_size;
    SET_BLOCK_ALLOCATED(start_tag);
    SET_BLOCK_ALLOCATED(end_tag);
}

void SetBoundaryTagsFree(uintptr_t ptr, size_t block_size)
{
    auto start_tag = BLOCK_TAG(ptr);
    auto end_tag = BLOCK_END_TAG(ptr, block_size);
    *start_tag = (BoundaryTag)block_size;
    *end_tag = (BoundaryTag)block_size;
    SET_BLOCK_FREE(start_tag);
    SET_BLOCK_FREE(end_tag);
}

uintptr_t CreateBoundaryTagsAlloc(uintptr_t ptr, size_t size)
{
    auto block_ptr = ptr + sizeof(BoundaryTag);
    SetBoundaryTagsAlloc(block_ptr, size - sizeof(BoundaryTag));
    return block_ptr;
}

uintptr_t CreateBoundaryTagsFree(uintptr_t ptr, size_t size)
{
    auto block_ptr = ptr + sizeof(BoundaryTag);
    SetBoundaryTagsFree(block_ptr, size - sizeof(BoundaryTag));
    return block_ptr;
}

bool InitAllocator(Allocator* allocator, uintptr_t head, size_t capacity)
{
    if (!allocator)
        return false;

    allocator->head = head;
    allocator->tail = head + capacity;
    allocator->capacity = capacity;
    allocator->allocated = 0;

    auto head_tag = (BoundaryTag*)(allocator->head);
    auto head_end_tag = (BoundaryTag*)(allocator->tail - sizeof(BoundaryTag));
    *head_tag = 0;
    *head_end_tag = 0;
    SET_BLOCK_ALLOCATED(head_tag);
    SET_BLOCK_ALLOCATED(head_end_tag);

    size_t block_length = (uintptr_t)head_end_tag - (uintptr_t)head_tag - 2 * sizeof(BoundaryTag);
    allocator->free_list = (FreeList*)CreateBoundaryTagsFree((uintptr_t)head_tag + sizeof(BoundaryTag), block_length);     
    allocator->free_list->next = nullptr;
    allocator->num_blocks = 1;

    return true;
}

uintptr_t SplitBlock(Allocator* allocator, uintptr_t block, size_t size)
{
    auto start_tag = BLOCK_TAG(block);
    size_t block_length = BLOCK_LENGTH(start_tag);

    // If the spliting the block would produce a tiny block (0-3 bytes) then don't.
    // A block has to at least fit a FreeList pointer in it.
    if (block_length < size + BLOCK_OVERHEAD_SIZE)
    {
        return block;
    }

    SetBoundaryTagsFree(block, size);

    FreeList* new_block = (FreeList*)CreateBoundaryTagsFree(
        block + size + sizeof(BoundaryTag),
        block_length - size - sizeof(BoundaryTag));
    new_block->next = allocator->free_list;
    allocator->free_list = new_block;
    allocator->num_blocks++;
    
    return block;
}

void MergeBlock(Allocator* allocator, BoundaryTag* start_tag, BoundaryTag* end_tag)
{
}

void* AllocateBlock(Allocator* allocator, size_t size)
{
    assert(allocator);
    if (size < 0 || (allocator && !(allocator->free_list)))
        return nullptr;
   
    // Size required to be a multiple of 2. This is so the 1 bit 
    // can be used as a flag.
    size_t adjusted_size = (size + 1) & ~(size_t)1;

    uintptr_t free_block = 0;
    FreeList* previous = nullptr;
    for (auto itor = allocator->free_list; itor != nullptr; itor = itor->next)
    {
        auto tag_ptr = BLOCK_TAG(itor);
        size_t block_length = BLOCK_LENGTH(tag_ptr);
        if (block_length >= adjusted_size)
        {
            if (previous)
            {
                previous->next = itor->next;
            }
            else
            {
                allocator->free_list = itor->next;
            }

            free_block = SplitBlock(allocator, (uintptr_t)itor, adjusted_size);
            SetBoundaryTagsAlloc(free_block, adjusted_size);
            allocator->allocated += adjusted_size;
            break;
        }

        previous = itor;
    }

    if (!free_block)
    {
        return nullptr;
    }

    return (void*)free_block;
}

void ReleaseBlock(Allocator* allocator, void* ptr)
{
    assert(allocator);
    if (!allocator || (ptr && ((uintptr_t)ptr < allocator->head || (uintptr_t)ptr > allocator->tail)))
        return;

    auto freed_block = (uintptr_t)ptr;
    auto tag_ptr = BLOCK_TAG(freed_block);
    auto block_length = BLOCK_LENGTH(tag_ptr);

    auto prev_block_tag = PREV_BLOCK_TAG(freed_block);
    auto next_block_tag = NEXT_BLOCK_TAG(freed_block, block_length);

    if (BLOCK_FREE(prev_block_tag) && BLOCK_FREE(next_block_tag))
    {
        // Previous block is already in free list, but we will need to remove entry for next.
        auto prev_block_length = BLOCK_LENGTH(prev_block_tag);
        auto prev_block = (uintptr_t)prev_block_tag - prev_block_length;

        auto next_block_length = BLOCK_LENGTH(next_block_tag);
        auto next_block = (uintptr_t)next_block_tag + sizeof(BoundaryTag);

        // Need to remove the entry from the freelist for the next block.
        FreeList *prev = nullptr, *itor = allocator->free_list;
        for (itor; itor != nullptr; itor = itor->next)
        {
            if (itor == (FreeList*)next_block)
                break;
            prev = itor;
        }

        if (prev)
            prev->next = itor->next;
        else
            allocator->free_list->next = itor->next; 

        SetBoundaryTagsFree(prev_block, prev_block_length + block_length + next_block_length + 4 * sizeof(BoundaryTag));
        allocator->num_blocks -= 2;
    }
    else if (BLOCK_FREE(prev_block_tag))
    {
        // Previous block is already in free list, no need to mess with free list.
        auto prev_block_length = BLOCK_LENGTH(prev_block_tag);
        auto prev_block = (uintptr_t)prev_block_tag + prev_block_length;
        SetBoundaryTagsFree(prev_block, prev_block_length + block_length + 2 * sizeof(BoundaryTag));
        allocator->num_blocks--;
    }
    else if (BLOCK_FREE(next_block_tag))
    {
        auto next_block = (uintptr_t)next_block_tag + sizeof(BoundaryTag);

        // Need to move free list pointer to point to the beginning of the freed block.
        FreeList *prev = nullptr, *itor = allocator->free_list;
        for (itor; itor != nullptr; itor = itor->next)
        {
            if (itor == (FreeList*)next_block)
                break;
            prev = itor;
        }

        auto next_block_length = BLOCK_LENGTH(next_block_tag);
        SetBoundaryTagsFree(freed_block, block_length + next_block_length + 2 * sizeof(BoundaryTag));
        allocator->num_blocks--;

        if (!prev)
        {
            allocator->free_list = (FreeList*)freed_block;
            allocator->free_list->next = itor->next;
        }
        else
        {
            prev->next = (FreeList*)freed_block;
            prev->next->next = itor->next;
        }
    }
    else
    {
        SetBoundaryTagsFree(freed_block, block_length);
        auto free_list = (FreeList*)freed_block;
        free_list->next = allocator->free_list;
        allocator->free_list = free_list;
    }

    allocator->allocated -= block_length;
}

bool InitGlobalMemory()
{
    const size_t memory_pool_size = 500 * 1024 * 1024;

    if (!g_allocator)
    {
        g_allocator = (Allocator*)malloc(sizeof(Allocator) + memory_pool_size);

        return InitAllocator(
            g_allocator,
            (uintptr_t)g_allocator + sizeof(Allocator),
            memory_pool_size);
    }

    return false;
}

void ReleaseGlobalMemory()
{
    if (g_allocator)
    {
        free(g_allocator);
    }
}

void* Alloc(size_t size)
{
    assert(g_allocator);

    if (g_allocator)
    {
        return AllocateBlock(g_allocator, size);
    }

    return nullptr;
}

void Release(void* ptr)
{
    assert(g_allocator);

    if (g_allocator && ptr)
    {
        ReleaseBlock(g_allocator, ptr);
    }
}

StackAllocator CreateStackAllocator(size_t size)
{
    StackAllocator stack = {};
    stack.head = (uintptr_t)Alloc(size);
    stack.tail = stack.head + size;
    stack.current = stack.head;

    return stack;
}

void* StackAlloc(StackAllocator &stack, size_t size)
{
    void* ptr = nullptr;

    if (stack.current + size < stack.tail)
    {
        ptr = (void*)stack.current;
        stack.current += size;
    }

    return ptr;
}

void StackRewind(StackAllocator &stack, void* ptr)
{
    uintptr_t intptr = (uintptr_t)ptr;
    if (intptr >= stack.head && intptr < stack.tail)
    {
        stack.current = intptr;
    }
}

void StackRelease(StackAllocator &stack)
{
    Release((void*)stack.head);
}

void MemCopy(void* &_dest, const void* _source, size_t bytes)
{
    memcpy(_dest, _source, bytes);
}
