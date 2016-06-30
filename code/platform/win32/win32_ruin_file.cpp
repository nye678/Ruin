 #include "ruin_file.h"

#include "ruin_assert.h"
#include "ruin_memory.h"

#include <Windows.h>
#include "Shlwapi.h"

struct File
{
    HANDLE handle;
};

void DestoryFile(void* ptr)
{
    if (ptr)
    {
        CloseHandle(((File*)ptr)->handle);
    }
}

File* OpenFile(const char* filepath)
{
    HANDLE fileHandle = CreateFile(
        filepath,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (fileHandle == INVALID_HANDLE_VALUE)
        return nullptr;
    else
    {
        File* file = (File*)Alloc(sizeof(File));
        file->handle = fileHandle;
        return file;
    }
}

void CloseFile(File* &file)
{
    if (file)
    {   
        CloseHandle(file->handle);
        Release(file);

        file = nullptr;
    }
}

size_t ReadFile(File* file, void* buffer, size_t num)
{
    DWORD bytesRead = 0;
    ::ReadFile(file->handle, buffer, (DWORD)num, &bytesRead, nullptr);
    return (size_t)bytesRead;
}

bool SetFilePos(File* file, size_t position)
{
    LARGE_INTEGER largeInt;
    largeInt.QuadPart = position;
    
    return SetFilePointerEx(file->handle, largeInt, nullptr, FILE_BEGIN) == TRUE;
}

size_t LoadTextFile(const char* fileName, char* &buffer)
{
    File* textFile = OpenFile(fileName);
    if (!textFile)
        return 0;

    LARGE_INTEGER largeInt;

    if (!GetFileSizeEx(textFile->handle, &largeInt))
    {
        CloseFile(textFile);
        return 0;
    }

    size_t fileSize = (size_t)largeInt.QuadPart;

    buffer = (char*)Alloc((size_t)fileSize + 1);
    if (!buffer)
    {
        CloseFile(textFile);
        return 0;
    }

    memset(buffer, 0x0, (size_t)fileSize + 1);

    size_t bytesRead = ReadFile(textFile, buffer, fileSize);

    CloseFile(textFile);
    return bytesRead;
}

bool FileExists(const char* filePath)
{
    return PathFileExists(filePath) == 1;
}

struct Ruin::rcLibrary
{
    HMODULE lib;
};

Ruin::rcLibrary* Ruin::rLoadLibrary(const char* libname)
{
    HMODULE lib = LoadLibrary(libname);
    if (lib)
    {
        rcLibrary* rcLib = (rcLibrary*)Alloc(sizeof(rcLibrary));
        rcLib->lib = lib;
        return rcLib;
    }

    return nullptr;
}

void* Ruin::rGetProcAddress(rcLibrary* lib, const char* procName)
{
    if (lib && lib->lib)
    {
        return (void*)(GetProcAddress(lib->lib, procName));
    }

    return nullptr;
}
