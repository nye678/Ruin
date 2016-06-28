#include <stdio.h>
#include <string.h>

#include "utility.h"

rString::rString(const rAllocator &allocator, const char* str)
    : rObject(allocator)
{
    _len = strlen(str);
    _cstr = (char*)getAllocator().alloc(strlen(str + 1));

    strncpy(_cstr, str, _len + 1);
}

rString::~rString()
{
    if (_cstr)
    {
        getAllocator().free(_cstr);
    }
}

rString* LoadTextFile(const char* fileName)
{
    return LoadTextFile(fileName, rMemoryPool::Permanent);
}

rString* LoadTextFile(const char* fileName, rMemoryPool pool)
{
    rString* textFile = nullptr;

    size_t read = 0;
    FILE* file = nullptr;
    errno_t error = fopen_s(&file, fileName, "r");

    if (error == 0)
    {
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);

        rAllocator tempAllocator = rMemoryManager::GetAllocator(rMemoryPool::Temporary);
        char* buffer = (char*)tempAllocator.alloc(size + 1);
        memset(buffer, 0x00, size + 1);
        read = fread_s(buffer, size + 1, 1, size, file);
        buffer[read] = 0x00;

        if (read == 0)
        {
            tempAllocator.free(buffer);
        }
        else
        {
            textFile = Instantiate<rString>(pool, buffer);
        }

        fclose(file);
    }

    return textFile;
}

size_t loadTextFile(const char* fileName, char* &buffer)
{
    size_t read = 0;
    FILE* file = nullptr;
    errno_t error = fopen_s(&file, fileName, "r");

    if (error == 0)
    {
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);

        buffer = new char[size + 1];
        memset(buffer, 0x00, size + 1);
        read = fread_s(buffer, size + 1, 1, size, file);
        buffer[read] = 0x00;

        if (read == 0)
        {
            delete[] buffer;
        }

        fclose(file);
    }

    return read;
}