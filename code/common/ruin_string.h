#ifndef __RUIN_STRING_
#define __RUIN_STRING_

#include "ruin_int.h"
#include "ruin_allocator.h"

struct String
{
    Allocator* allocator;

    size_t length;
    char* cstring;

    char &operator[] (size_t);
    char operator[] (size_t) const;

    ~String()

    static String Create(const char* str, Allocator* allocator);
    static String Clone(const String &);
    static String Clone(const String &, Allocator* allocator);

    static void Release(String*);
};

#endif