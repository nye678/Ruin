#ifndef __RUIN_UTILITY_H_
#define __RUIN_UTILITY_H_

#include "memory.h"

class rString : public rObject
{
private:
    char* _cstr;
    size_t _len;

public:
    rString(const rAllocator &, const char*);
    ~rString();
};

rString* LoadTextFile(const char* fileName);
rString* LoadTextFile(const char* fileName, rMemoryPool pool);

class rEngineSettings : public rEngineObject
{
private:
    static rEngineSettings* sInstance;

public:
    static void Create();
    static void Destroy();

private:
    
};

size_t loadTextFile(const char* fileName, char* &buffer);

#endif