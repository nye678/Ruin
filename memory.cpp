#include "memory.h"

#define MEGABYTE(x) (x * 1024 * 1024)

rMemoryManager* rMemoryManager::sInstance = nullptr;

void rMemoryManager::Create()
{
    if (!sInstance)
    {
        sInstance = new rMemoryManager();
    }
}

void rMemoryManager::Destroy()
{
    if (sInstance)
    {
        delete sInstance; 
        sInstance = nullptr;
    }
}

rMemoryManager* rMemoryManager::Get()
{
    return sInstance;
}

rAllocator rMemoryManager::GetAllocator(rMemoryPool allocatorPool)
{
    return rAllocator(sInstance->_allocatorPools[(size_t)allocatorPool]);
}

rMemoryManager::rMemoryManager()
{
    _allocatorPools[(size_t)rMemoryPool::Heap] = new rHeapAllocator();
    _allocatorPools[(size_t)rMemoryPool::Permanent] = new rBoundaryTagAllocator(MEGABYTE(200));
    _allocatorPools[(size_t)rMemoryPool::Temporary] = new rLinearAllocator(MEGABYTE(200));
}

rMemoryManager::~rMemoryManager()
{
    for (size_t i = 0; i < (size_t)rMemoryPool::MemoryPoolCount; ++i)
    {
        if (_allocatorPools[i])
        {
            delete _allocatorPools[i];
            _allocatorPools[i] = nullptr;
        }
    }
}

void* rHeapAllocator::alloc(size_t size)
{
    if (size > 0)
    {
        ++_allocations;
        return malloc(size);
    }
    else
        return nullptr;
}

void rHeapAllocator::free(void* ptr)
{
    if (ptr)
    {
        --_allocations;
        free(ptr);
    }
    ptr = nullptr;
}

rHeapAllocator::rHeapAllocator()
    : _allocations(0)
{}

rHeapAllocator::~rHeapAllocator()
{}

#define BLOCK_LENGTH(tagPtr) (*tagPtr & ~0x01)
#define BLOCK_FREE(tagPtr) (*tagPtr & 0x00)
#define SET_BLOCK_FREE(tagPtr) (*tagPtr &= ~0x01)
#define SET_BLOCK_ALLOCATED(tagPtr) (*tagPtr |= 0x01)

#define BLOCK_TAG(ptr) ((rBoundaryTagAllocator::BoundaryTag*)((uintptr_t)ptr - sizeof(rBoundaryTagAllocator::BoundaryTag)))
#define PREV_BLOCK_TAG(ptr) ((rBoundaryTagAllocator::BoundaryTag*)((uintptr_t)ptr - (2 * sizeof(rBoundaryTagAllocator::BoundaryTag))))
#define BLOCK_END_TAG(ptr, size) ((rBoundaryTagAllocator::BoundaryTag*)((uintptr_t)ptr + (uintptr_t)size))
#define NEXT_BLOCK_TAG(ptr, size) ((rBoundaryTagAllocator::BoundaryTag*)((uintptr_t)ptr + (uintptr_t)size + sizeof(rBoundaryTagAllocator::BoundaryTag)))

#define BLOCK_OVERHEAD_SIZE 2 * sizeof(rBoundaryTagAllocator::BoundaryTag) + sizeof(rBoundaryTagAllocator::FreeList)

void* rBoundaryTagAllocator::alloc(size_t size)
{
    if (size < 0 || !_freeList)
    {
        return nullptr;
    }

    // Size required to be a multiple of 2. This is so the 1 bit 
    // can be used as a flag.
    size_t adjustedSize = (size + 1) & ~(size_t)1;

    uintptr_t freeBlock = 0;
    FreeList* previous = nullptr;
    for (auto itor = _freeList; itor != nullptr; itor = itor->next)
    {
        auto tagPtr = BLOCK_TAG(itor);
        size_t blockLength = BLOCK_LENGTH(tagPtr);
        if (blockLength >= adjustedSize)
        {
            if (previous)
            {
                previous->next = itor->next;
            }
            else
            {
                _freeList = itor->next;
            }

            freeBlock = splitBlock((uintptr_t)itor, adjustedSize);
            setBoundaryTagsAlloc(freeBlock, adjustedSize);
            _allocated += adjustedSize;
            break;
        }

        previous = itor;
    }

    if (!freeBlock)
    {
        return nullptr;
    }

    return (void*)freeBlock;
}

void rBoundaryTagAllocator::free(void* ptr)
{
    auto freedBlock = (uintptr_t)ptr;
    auto tagPtr = BLOCK_TAG(freedBlock);
    auto blockLength = BLOCK_LENGTH(tagPtr);

    auto prevBlockTag = PREV_BLOCK_TAG(freedBlock);
    auto nextBlockTag = NEXT_BLOCK_TAG(freedBlock, blockLength);

    if (BLOCK_FREE(prevBlockTag) && BLOCK_FREE(nextBlockTag))
    {
        // Previous block is already in free list, but we will need to remove entry for next.
        auto prevBlockLength = BLOCK_LENGTH(prevBlockTag);
        auto prevBlock = (uintptr_t)prevBlockTag - prevBlockLength;

        auto nextBlockLength = BLOCK_LENGTH(nextBlockTag);
        auto nextBlock = (uintptr_t)nextBlockTag + sizeof(BoundaryTag);

        // Need to remove the entry from the freelist for the next block.
        FreeList *prev = nullptr, *itor = _freeList;
        for (itor; itor != nullptr; itor = itor->next)
        {
            if (itor == (FreeList*)nextBlock)
                break;
            prev = itor;
        }

        if (prev)
            prev->next = itor->next;
        else
            _freeList->next = itor->next; 

        setBoundaryTagsFree(prevBlock, prevBlockLength + blockLength + nextBlockLength + 4 * sizeof(BoundaryTag));
        _numBlocks -= 2;
    }
    else if (BLOCK_FREE(prevBlockTag))
    {
        // Previous block is already in free list, no need to mess with free list.
        auto prevBlockLength = BLOCK_LENGTH(prevBlockTag);
        auto prevBlock = (uintptr_t)prevBlockTag + prevBlockLength;
        setBoundaryTagsFree(prevBlock, prevBlockLength + blockLength + 2 * sizeof(BoundaryTag));
        _numBlocks--;
    }
    else if (BLOCK_FREE(nextBlockTag))
    {
        auto nextBlock = (uintptr_t)nextBlockTag + sizeof(BoundaryTag);

        // Need to move free list pointer to point to the beginning of the freed block.
        FreeList *prev = nullptr, *itor = _freeList;
        for (itor; itor != nullptr; itor = itor->next)
        {
            if (itor == (FreeList*)nextBlock)
                break;
            prev = itor;
        }

        auto nextBlockLength = BLOCK_LENGTH(nextBlockTag);
        setBoundaryTagsFree(freedBlock, blockLength + nextBlockLength + 2 * sizeof(BoundaryTag));
        _numBlocks--;

        if (!prev)
        {

            _freeList = (FreeList*)freedBlock;
            _freeList->next = itor->next;
        }
        else
        {
            prev->next = (FreeList*)freedBlock;
            prev->next->next = itor->next;
        }
    }
    else
    {
        setBoundaryTagsFree(freedBlock, blockLength);
        auto freeList = (FreeList*)freedBlock;
        freeList->next = _freeList;
        _freeList = freeList;
    }

    _allocated -= blockLength;
}

rBoundaryTagAllocator::rBoundaryTagAllocator(size_t capacity)
    : _size(0), _allocated(0), _numBlocks(0), _head(0), _tail(0), _freeList(nullptr)
{
    _head = (uintptr_t)malloc(capacity);
    _tail = _head + capacity;
    _allocated = 0;

    auto headTag = (BoundaryTag*)(_head);
    auto headEndTag = (BoundaryTag*)(_tail - sizeof(BoundaryTag));
    *headTag = 0;
    *headEndTag = 0;
    SET_BLOCK_ALLOCATED(headTag);
    SET_BLOCK_ALLOCATED(headEndTag);

    _size = (uintptr_t)headEndTag - (uintptr_t)headTag - 2 * sizeof(BoundaryTag);
    _freeList = (FreeList*)createBoundaryTagsFree((uintptr_t)headTag + sizeof(BoundaryTag), _size);     
    _freeList->next = nullptr;
    _numBlocks = 1;
}

rBoundaryTagAllocator::~rBoundaryTagAllocator()
{
    free((void*)_head);
}

uintptr_t rBoundaryTagAllocator::splitBlock(uintptr_t block, size_t size)
{
    auto startTag = BLOCK_TAG(block);
    size_t blockLength = BLOCK_LENGTH(startTag);

    // If the spliting the block would produce a tiny block (0-3 bytes) then don't.
    // A block has to at least fit a FreeList pointer in it.
    if (blockLength < size + BLOCK_OVERHEAD_SIZE)
    {
        return block;
    }

    setBoundaryTagsFree(block, size);

    FreeList* newBlock = (FreeList*)createBoundaryTagsFree(
        block + size + sizeof(BoundaryTag),
        blockLength - size - sizeof(BoundaryTag));
    newBlock->next = _freeList;
    _freeList = newBlock;
    _numBlocks++;
    
    return block;
}

void rBoundaryTagAllocator::mergeBlock(uint32_t* startTag, uint32_t* endTag)
{
}

uintptr_t rBoundaryTagAllocator::createBoundaryTagsAlloc(uintptr_t ptr, size_t size)
{
    auto blockptr = ptr + sizeof(BoundaryTag);
    setBoundaryTagsAlloc(blockptr, size - sizeof(BoundaryTag));
    return blockptr;
}

uintptr_t rBoundaryTagAllocator::createBoundaryTagsFree(uintptr_t ptr, size_t size)
{
    auto blockptr = ptr + sizeof(BoundaryTag);
    setBoundaryTagsFree(blockptr, size - sizeof(BoundaryTag));
    return blockptr;
}

void rBoundaryTagAllocator::setBoundaryTagsAlloc(uintptr_t ptr, size_t blockSize)
{
    auto startTag = BLOCK_TAG(ptr);
    auto endTag = BLOCK_END_TAG(ptr, blockSize);
    *startTag = (uint32_t)blockSize;
    *endTag = (uint32_t)blockSize;
    SET_BLOCK_ALLOCATED(startTag);
    SET_BLOCK_ALLOCATED(endTag);
}

void rBoundaryTagAllocator::setBoundaryTagsFree(uintptr_t ptr, size_t blockSize)
{
    auto startTag = BLOCK_TAG(ptr);
    auto endTag = BLOCK_END_TAG(ptr, blockSize);
    *startTag = (uint32_t)blockSize;
    *endTag = (uint32_t)blockSize;
    SET_BLOCK_FREE(startTag);
    SET_BLOCK_FREE(endTag);
}

void* rLinearAllocator::alloc(size_t size)
{
    if (_current + size > _end)
    {
        return nullptr;
    }

    void* ptr = (void*)_current;
    _current += size;

    return ptr;
}

void rLinearAllocator::free(void* ptr)
{
    uintptr_t p = (uintptr_t)ptr;
    if (p >= _start && p < _end)
    {
        _current = p;
    }
}

rLinearAllocator::rLinearAllocator(size_t capacity)
{
    _start = (uintptr_t)malloc(capacity);
    _end = _start + capacity;
    _current = _start;
}

rLinearAllocator::~rLinearAllocator()
{
    free((void*)_start);
}

rAllocator::rAllocator(rAllocatorBase* allocator)
    : _allocator(allocator)
{}

void* rAllocator::alloc(size_t size)
{
    return _allocator->alloc(size);
}

void rAllocator::free(void* ptr)
{
    _allocator->free(ptr);
}

#include <cstdio>
rObject::rObject(const rAllocator &allocator)
    : _allocator(allocator), _refCount(0)
{
    fprintf(stdout, "Debug - object created\n");
}

rObject::~rObject()
{
    fprintf(stdout, "Debug - object destroyed\n");
}

rAllocator rObject::getAllocator()
{
    return _allocator;
}