#ifndef __RUIN_HASH_TABLE_
#define __RUIN_HASH_TABLE_

#include "ruin_allocator.h"

namespace Ruin
{
    template <typename K, typename V>
    struct rsHashNode
    {
        K key;
        V value;
        rsHashNode* next;
    };

    template <typename K, typename V, int32_t (*HashFunc)(K), int32_t HashSize = 193>
    struct rsHashTable
    {
        rsAllocator allocator;
        rsHashNode<K, V>* storage[HashSize];

        rsHashTable(rsAllocator&);

        void insert(K, V);
        bool lookup(K, V&);
    };
}

template <typename K, typename V, int32_t (*HashFunc)(K), int32_t HashSize>
Ruin::rsHashTable<K, V, HashFunc, HashSize>::rsHashTable(rsAllocator &allocator)
{
    this->allocator = allocator;
	for (int i = 0; i < HashSize; ++i)
		storage[i] = nullptr;
}

template <typename K, typename V, int32_t (*HashFunc)(K), int32_t HashSize>
void Ruin::rsHashTable<K, V, HashFunc, HashSize>::insert(K key, V value)
{
    int32_t hashIndex = HashFunc(key) % HashSize;
    if (!storage[hashIndex])
    {
        auto newNode = (rsHashNode<K, V>*)rAlloc(allocator, sizeof(rsHashNode<K, V>));
        (*newNode) = { key, value, nullptr };
        storage[hashIndex] = newNode;
    }
    else
    {
        auto itor = storage[hashIndex];
        auto prev = itor;
        while (itor != nullptr && itor->key != key)
        {
            prev = itor;
            itor = itor->next;
        }
        if (itor)
        {
            itor->value = value;
        }
        else
        {
            auto newNode = (rsHashNode<K, V>*)rAlloc(allocator, sizeof(rsHashNode<K, V>));
            (*newNode) = { key, value, nullptr };

            prev->next = newNode;
        }
    }
}

template <typename K, typename V, int32_t (*HashFunc)(K), int32_t HashSize>
bool Ruin::rsHashTable<K, V, HashFunc, HashSize>::lookup(K key, V &value)
{
    int32_t hashIndex = HashFunc(key) % HashSize;
    if (storage[hashIndex])
    {
        auto itor = storage[hashIndex];
        while (itor != nullptr && itor->key != key)
            itor = itor->next;
        if (itor)
        {
            value = itor->value;
            return true;
        }
    }

    return false;
}

#endif