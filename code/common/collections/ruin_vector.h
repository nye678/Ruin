#ifndef __RUIN_VECTOR_
#define __RUIN_VECTOR_

#include "ruin_int.h"

namespace Ruin
{
    template <typename T>
    struct rsVector
    {
        size_t capacity;
        size_t length;
        T* array;
    };

    template <typename T>
    inline T* rGet(rsVector<T>* vec, size_t index)
    {
        return &vec->array[index];
    }

    template <typename T>
    inline T* rGet(rsVector<T> &vec, size_t index)
    {
        return rGet(&vec, index);
    }

    template <typename T>
    inline T* rFront(rsVector<T>* vec)
    {
        return &vec->array[0]
    }

    template <typename T>
    inline T* rBack(rsVector<T>& vec)
    {
        return rBack(&vec);
    }

    template <typename T>
    inline T* rBack(rsVector<T>* vec)
    {
        return &vec->array[vec->length-1];
    }

    template <typename T>
    inline void rPushBack(rsVector<T> &vec, const T &value)
    {
        rPushBack(&vec, value);
    }

    template <typename T>
    void rPushBack(rsVector<T>* vec, const T &value)
    {
        if (vec->length == vec->capacity)
            rResize(vec, vec->capacity * 2);

        vec->array[vec->length] = value;
        vec->length++;
    }

    template <typename T>
    inline void rPopBack(rsVector<T>* vec)
    {
        vec->length--;
    }

    template <typename T>
    inline void rClear(rsVector<T>* vec)
    {
        vec->length = 0;
    }

    template <typename T>
    void rResize(rsVector<T>* vec, size_t size)
    {
        void* tempArray = Alloc(sizeof(T) * size);
        MemCopy(tempArray, vec->array, sizeof(T) * vec->length);
        Release(vec->array);
        vec->array = (T*)tempArray;
        vec->capacity = size;
    }

    template <typename T>
    void rSwap(rsVector<T>* vec, size_t index1, size_t index2)
    {
        T* item1 = rGet<T>(vec, index1);
        T* item2 = rGet<T>(vec, index2);
        T temp = *item1;
        *item1 = *item2;
        *item2 = temp;
    }

    template <typename T, size_t C = 10>
    void rInitialize(rsVector<T> &vec, size_t capacity = C)
    {
        vec.capacity = capacity;
        vec.length = 0;
        vec.array = (T*)Alloc(capacity * sizeof(T));
    }

    template <typename T, size_t C = 10>
    void rInitialize(rsVector<T>* vec, size_t capacity = C)
    {
        vec->capacity = capacity;
        vec->length = 0;
        vec->array = (T*)Alloc(capacity * sizeof(T));
    }

    #define LT(cmp) ((cmp) < 0)
    #define LTE(cmp) ((cmp) <= 0)
    #define EQ(cmp) ((cmp) == 0)
    #define GT(cmp) ((cmp) > 0)

    template <typename T, int32_t(*compare)(const T&, const T&)>
    int32_t rPartition(rsVector<T>* vec, int32_t low, int32_t high)
    {
        T pivotElement = vec->array[(high-low)/2];
        int32_t lb = low, ub = high;
        
        while (low <= high)
        {
			while (LTE(compare(vec->array[low], pivotElement)))
                ++low;
            while (GT(compare(vec->array[high], pivotElement)))
                --high;
            if (low <= high)
            {
                T temp = vec->array[low];
                vec->array[low] = vec->array[high];
                vec->array[high] = temp;
            }
        }
        
        vec->array[(ub-lb)/2] = vec->array[high];
        vec->array[high] = pivotElement;
        return high;
    }

    template <typename T>
    void rSortPartition(T* a, int32_t n)
    {
        int32_t i, j;
        T p, t;

        if (n < 10)
        {
            for (i = 1; i < n; ++i)
            {
                t = a[i];
                j = i;
                while (j > 0 && t < a[j - 1])
                {
                    a[j] = a[j - 1];
                    --j;
                }
                a[j] = t;
            }
            return;
        }
        p = a[n / 2];
        for (i = 0, j = n - 1;; i++, j--)
        {
            while (a[i] < p)
                i++;
            while (p < a[j])
                j--;
            if (i >= j)
                break;
            t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
        rSortPartition(a, i);
        rSortPartition(a + i, n - i);
    }

    template <typename T>
    void rSort(const rsVector<T> &vec)
    {
        rSortPartition<T>(vec.array, (int32_t)vec.length);
    }
}

#endif