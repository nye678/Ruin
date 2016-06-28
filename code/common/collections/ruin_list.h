#ifndef __RUIN_LIST_
#define __RUIN_LIST_

#include "ruin_int.h"

struct ListNode
{
    ListNode* next;
    ListNode* prev;
    void* item;
};

struct List
{
    ListNode* head;
    size_t length;
    size_t itemSize;
};

struct ListIterator
{
    List* list;
    ListNode* node;
};

List* createList(size_t itemSize);
void* get(List*, size_t index);
void insert(List*, void* item, size_t index);
void append(List*, void* item);
void clear(List*);

ListIterator begin(List*);
ListIterator end(List*);
void remove(ListIterator*);

ListIterator &operator++ (ListIterator &);
ListIterator &operator-- (ListIterator &);
ListIterator operator++ (ListIterator &, int);
ListIterator operator-- (ListIterator &, int);

void* operator*(ListIterator &);

#endif