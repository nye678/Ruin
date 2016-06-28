#ifndef __RUIN_COLLECTIONS_H_
#define __RUIN_COLLECTIONS_H_

#include "memory.h"

template <typename T>
class rVector : public rObject
{
private:
    size_t _capacity;
    size_t _length;

    T* _array;

    void resize(size_t size);
    void resizeWithOffset(size_t size, size_t offset);

public:
    T* get(size_t index)
    {
        if (index >= 0 && index < _length)
            return _array[_index];
        else
            return nullptr;
    }

    T* front()
    {
        if (_length > 0)
            return _array;
        else
            return nullptr;
    }

    T* back()
    {
        if (_length > 0)
            return _array[_length - 1];
        else
            return nullptr;
    }

    void pushFront(T* item)
    {
        if (item)
        {
            if (_length == _capacity)
            {
                
            }
        }
    }

    void pushBack(T* item)
    {

    }

    void popFront()
    {

    }

    void popBack()
    {

    }

    void clear()
    {

    }

};

#endif