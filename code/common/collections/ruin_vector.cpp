#include "ruin_vector.h"

#include "ruin_assert.h"
#include "ruin_memory.h"

using namespace Ruin;

void* get(Vector* vec, size_t index)
{
    if (index >= 0 && index < vec->length)
        return OffsetPointer(vec->array, (vec->itemSize * index));
    else
        return nullptr;
}

void* front(Vector* vec)
{
    if (vec->length > 0)
        return vec->array;
    else
        return nullptr;
}

void* back(Vector* vec)
{
    if (vec->length > 0)
        return OffsetPointer(vec->array, (vec->itemSize * (vec->length - 1)));
    else
        return nullptr;
}

void pushFront(Vector* vec, void* item)
{
    rAssert(item);
    if (vec->length == vec->capacity)
    {
        void* tempArray = Alloc(vec->itemSize * vec->capacity * 2);
        void* offsetPointer = OffsetPointer(tempArray, vec->itemSize);
        MemCopy(
            offsetPointer,
            vec->array,
            vec->itemSize * vec->length);
        Release(vec->array);
        vec->array = tempArray;
        vec->capacity *= 2;
    }
    else
    {
        void* offsetPointer = OffsetPointer(vec->array, vec->itemSize);
        MemCopy(
            offsetPointer,
            vec->array,
            vec->itemSize * vec->length);
    }
    
    MemCopy(vec->array, item, vec->itemSize);
    vec->length++;
}

void pushBack(Vector* vec, void* item)
{
    rAssert(item);
    if (vec->length == vec->capacity)
        resize(vec, vec->capacity * 2);

    void* ptr = OffsetPointer(vec->array, (vec->itemSize * vec->length));
    MemCopy(ptr, item, vec->itemSize);
    vec->length++;
}

void popFront(Vector* vec)
{
    rAssert(vec->length > 0);
    vec->length--;
    MemCopy(
        vec->array,
        OffsetPointer(vec->array, vec->itemSize),
        vec->itemSize * vec->length);

}

void popBack(Vector* vec)
{
    rAssert(vec->length > 0);
    vec->length--;
}

void clear(Vector* vec)
{
    vec->length = 0;
}

void resize(Vector* vec, size_t size)
{
    rAssert(size > vec->capacity);

    void* tempArray =Alloc(vec->itemSize * size);

    MemCopy(tempArray, vec->array, vec->itemSize * vec->length);

    Release(vec->array);
    vec->array = tempArray;
    vec->capacity = size;
}

size_t length(Vector* vec)
{
    return vec->length;
}

bool isEmpty(Vector* vec)
{
    return vec->length == 0;
}

Vector* createVector(size_t capacity, size_t itemSize)
{
    rAssert(capacity > 0);
    rAssert(itemSize > 0);

    Vector* vec = (Vector*)Alloc(sizeof(Vector));
    vec->array = Alloc(itemSize * capacity);
    vec->capacity = capacity;
    vec->length = 0;
    vec->itemSize = itemSize;

    return vec;
}

Vector* clone(Vector* vec)
{
    Vector* clone = createVector(vec->capacity, vec->itemSize);
    MemCopy(clone->array, vec->array, vec->itemSize * vec->length);
    clone->length = vec->length;

    return clone;
}

void Release(Vector* vec)
{
    Release(vec->array);
    Release((void*)vec);
}

void Swap(Vector* vec, size_t index1, size_t index2)
{
    if (index1 != index2)
    {
        void* item1 = get(vec, index1);
        pushBack(vec, item1);
        void* item2 = get(vec, index2);
        MemCopy(item1, item2, vec->itemSize);
        MemCopy(item2, back(vec), vec->itemSize);
        popBack(vec);
    }
}

#define LT(cmp) ((cmp) < 0)
#define EQ(cmp) ((cmp) == 0)
#define GT(cmp) ((cmp) > 0)

int32_t Partition(Vector* vec, CompareFunc compare, int32_t low, int32_t high)
{
    int i, pivotLoc;
    void* pivotKey;

    Swap(vec, low, (low + high) / 2);
    pivotKey = get(vec, low);
    pivotLoc = low;
    
    for (i = low + 1; i <= high; ++i)
        if (LT(compare(get(vec,i), pivotKey)))
            Swap(vec, ++pivotLoc, i);
    
    Swap(vec, low, pivotLoc);
    return pivotLoc;
}

void SortPartition(Vector* vec, CompareFunc compare, int32_t low, int32_t high)
{
    int pivotLoc;
    if (low < high)
    {
        pivotLoc = Partition(vec, compare, low, high);
        SortPartition(vec, compare, low, pivotLoc - 1);
        SortPartition(vec, compare, pivotLoc + 1, high);
    }
}

void Sort(Vector* vec, CompareFunc compare)
{
    SortPartition(vec, compare, 0, (int32_t)vec->length - 1);
}

void* Find(Vector* vec, void* key, CompareFunc compare)
{
    for (int i = 0; i < vec->length; ++i)
    {
        void* test = get(vec, i);
        if (compare(key, test) == 0)
            return test;
    }

    return nullptr;
}

void Map(Vector* vec, MapFunc mapFunc)
{
    for (int i = 0; i < vec->length; ++i)
    {
        mapFunc(get(vec, i));
    }
}