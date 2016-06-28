#include "ruin_list.h"

#include "ruin_assert.h"
#include "ruin_memory.h"

using namespace Ruin;

List* createList(size_t itemSize)
{
    rAssert(itemSize > 0);
    List* l = (List*)Alloc(sizeof(List));
    l->head = nullptr;
    l->length = 0;
    l->itemSize = itemSize;

    return l;
}

void* get(List*, size_t index)
{
    return 0;
}

void insert(List*, void* item, size_t index)
{

}

void append(List*, void* item)
{

}

void clear(List*)
{

}

ListIterator begin(List*)
{
    return ListIterator();
}

ListIterator end(List*)
{
    return ListIterator();
}

void remove(ListIterator*)
{

}

ListIterator &operator++ (ListIterator &itor)
{
    return itor;
}

ListIterator &operator-- (ListIterator &itor)
{
    return itor;
}

ListIterator operator++ (ListIterator &itor, int)
{
    return itor;
}

ListIterator operator-- (ListIterator &itor, int)
{
    return itor;
}

void* operator*(ListIterator &)
{
    return 0;
}
