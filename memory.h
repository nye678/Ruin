#ifndef __RUIN_MEMORY_H_
#define __RUIN_MEMORY_H_

#include <cstdint>
#include <cstdlib>
#include <new>

// Enumerations
enum struct rMemoryPool : int32_t
{
    Invalid = -1,
    Permanent,
    Temporary,
    Heap,
    MemoryPoolCount,
};

// Forward Declarations
class rObject;
class rMemoryManager;
class rAllocatorBase;
class rAllocator;
class rBoundaryTagAllocator;
class rLinearAllocator;
class rHeapAllocator;

template <typename T> class rSmartPointer;
typedef rSmartPointer<rObject> rObjectPtr;
typedef rSmartPointer<rAllocator> rAllocatorPtr;
typedef rSmartPointer<rBoundaryTagAllocator> rBoundaryTagAllocatorPtr;
typedef rSmartPointer<rLinearAllocator> rLinearAllocatorPtr;
typedef rSmartPointer<rHeapAllocator> rHeapAllocatorPtr;

class rEngineObject
{
public:
    virtual ~rEngineObject() {};
};

class rMemoryManager : public rEngineObject
{
private:
    static rMemoryManager* sInstance;

public:
    static void Create();
    static void Destroy();

    static rMemoryManager* Get();
    static rAllocator GetAllocator(rMemoryPool allocatorPool);

private:
    rAllocatorBase* _allocatorPools[(size_t)rMemoryPool::MemoryPoolCount];
    
public:
    rMemoryManager();
    rMemoryManager(const rMemoryManager &) = delete;
    rMemoryManager &operator= (const rMemoryManager &) = delete;
    ~rMemoryManager();
};

class rAllocatorBase : public rEngineObject
{
public:
    virtual void* alloc(size_t size) = 0;
    virtual void free(void* ptr) = 0;
};

class rHeapAllocator : public rAllocatorBase
{
private:
    size_t _allocations;

public:
    void* alloc(size_t size);
    void free(void* ptr);

    rHeapAllocator();
    rHeapAllocator(const rHeapAllocator &) = delete;
    rHeapAllocator &operator= (const rHeapAllocator &) = delete;
    ~rHeapAllocator();
};

class rBoundaryTagAllocator : public rAllocatorBase
{
private:
    struct FreeList { FreeList* next; };
    typedef uint32_t BoundaryTag;

    size_t _size;
    size_t _allocated;
    size_t _numBlocks;

    uintptr_t _head;
    uintptr_t _tail;

    FreeList* _freeList;

    void setup(rAllocator* allocator, size_t size);
    uintptr_t splitBlock(uintptr_t block, size_t size);
    void mergeBlock(uint32_t* startTag, uint32_t* endTag);
    uintptr_t createBoundaryTagsAlloc(uintptr_t ptr, size_t size);
    uintptr_t createBoundaryTagsFree(uintptr_t ptr, size_t size);
    void setBoundaryTagsAlloc(uintptr_t ptr, size_t blockSize);
    void setBoundaryTagsFree(uintptr_t ptr, size_t blockSize);

public:
    void* alloc(size_t size);
    void free(void* ptr);

    rBoundaryTagAllocator(size_t capacity);
    rBoundaryTagAllocator(const rBoundaryTagAllocator &) = delete;
    rBoundaryTagAllocator &operator= (const rBoundaryTagAllocator &) = delete;
    ~rBoundaryTagAllocator();
};

class rLinearAllocator : public rAllocatorBase
{
private:
    uintptr_t _start;
    uintptr_t _end;
    uintptr_t _current;

public:
    void* alloc(size_t size);
    void free(void* ptr);

    rLinearAllocator(size_t capacity);
    rLinearAllocator(const rLinearAllocator &) = delete;
    rLinearAllocator &operator= (const rLinearAllocator &) = delete;
    ~rLinearAllocator();
};

class rAllocator : public rEngineObject
{
private:
    rAllocatorBase* _allocator;

public:
    void* alloc(size_t);
    void free(void*);

    rAllocator(rAllocatorBase*);
};

class rObject : public rEngineObject
{
    rAllocator _allocator;
    uint32_t _refCount;

public:
    inline uint32_t addRef() { return ++_refCount; }
    inline uint32_t release() { return --_refCount; }
    inline uint32_t getRefCount() { return _refCount; }

    rAllocator getAllocator();

    rObject(const rAllocator &);
    virtual ~rObject();
};

template <typename T, typename... Args> T* Instantiate(rMemoryPool pool, Args... args)
{
    T* newObject = nullptr;

    if (pool > rMemoryPool::Invalid && pool < rMemoryPool::MemoryPoolCount)
    {
        rAllocator allocator = rMemoryManager::GetAllocator(pool);
        newObject = new(allocator.alloc(sizeof(T))) T(allocator, args...);
    }
    
    return newObject;
}

template <typename T> void Destroy(T* obj)
{
    if (obj && obj->getRefCount() == 0)
    {
        rAllocator allocator = obj->getAllocator();

        obj->~T();
        allocator.free(obj);
    }
}

template <typename T> class rSmartPointer
{
private:
    T* _ptr;

    void addReference()
    {
        if (_ptr) _ptr->addRef();
    }

    void release()
    {
        if (_ptr && _ptr->release() == 0)
        {
            Destroy<T>(_ptr);
            _ptr = nullptr;
        }
    }
public:
    T& operator* () { return _ptr; }
    T* operator-> () { return _ptr; }

    rSmartPointer() : _ptr(nullptr) {}

    rSmartPointer(T* ptr)
        : _ptr(ptr)
    {
        addReference();
    }

    rSmartPointer(const rSmartPointer<T> &ptr)
        : _ptr(nullptr)
    {
        *this = other;
    }

    rSmartPointer<T> &operator= (const rSmartPointer<T> &ptr)
    {
        if (this != &ptr)
        {
            release();

            _ptr = ptr._ptr;

            addReference();
        }

        return *this;
    }

    rSmartPointer<T> &operator= (T* ptr)
    {
        if (_ptr != ptr)
        {
            release();

            _ptr = ptr;

            addReference();
        }

        return *this;
    }

    ~rSmartPointer()
    {
        release();
        _ptr = nullptr;
    }

    inline operator T*() const { return _ptr; }

    inline bool operator==(const T* rhs)
    {
        return _ptr == rhs;
    }
};

template <typename T> void Destroy(rSmartPointer<T> &smtptr)
{
    smtptr = nullptr;
}

#endif