#ifndef __RUIN_FILE_
#define __RUIN_FILE_

struct File;

File* OpenFile(const char* filepath);
void CloseFile(File* &file);
size_t ReadFile(File*, void* buffer, size_t num);
bool SetFilePos(File*, size_t position);
size_t LoadTextFile(const char* fileName, char* &buffer);
bool FileExists(const char* filePath);

namespace Ruin
{
    struct rcLibrary;

    rcLibrary* rLoadLibrary(const char* libname);
    void* rGetProcAddress(rcLibrary*, const char* procName);
}

#endif