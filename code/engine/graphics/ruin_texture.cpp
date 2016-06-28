#include "ruin_texture.h"

#include "ruin_assert.h"
#include "ruin_file.h"
#include "ruin_gl.h"
#include "ruin_memory.h"

#pragma pack(1)
struct BMPFileHeader
{
    char type1;
    char type2;
    int fileSize;
    short res1;
    short res2;
    int pixelOffset;
};

struct BMPImageHeader
{
    int headerSize;
    int imageWidth;
    int imageHeight;
    short planes;
    short bitCount;
    int compression;
    int imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    int usedColorMapEntries;
    int numSignificantColors;
};
#pragma pack()

BitMap* LoadBMP(const char* filePath)
{
    BMPFileHeader fileHeader;
    BMPImageHeader imageHeader;

    File* bmpFile = OpenFile(filePath);

    if (!bmpFile)
        return nullptr;

    size_t bytesRead = 0;
    
    bytesRead = ReadFile(bmpFile, (void*)(&fileHeader), sizeof(BMPFileHeader));
    if (bytesRead != sizeof(BMPFileHeader))
    {
        CloseFile(bmpFile);
        return nullptr;
    }

    bytesRead = ReadFile(bmpFile, (void*)(&imageHeader), sizeof(BMPImageHeader));
    if (bytesRead != sizeof(BMPImageHeader))
    {
        CloseFile(bmpFile);
        return nullptr;
    }

    if (!SetFilePos(bmpFile, fileHeader.pixelOffset))
    {
        CloseFile(bmpFile);
        return nullptr;
    }

    size_t imageBufferSize = imageHeader.imageWidth * imageHeader.imageHeight * sizeof(uint32_t);
    uintptr_t bitMapMemory = (uintptr_t)Alloc(sizeof(BitMap) + imageBufferSize);

    BitMap* bitMap = (BitMap*)bitMapMemory;
    bitMap->data = (uint32_t*)(bitMapMemory + sizeof(BitMap));
    bitMap->width = imageHeader.imageWidth;
    bitMap->height = imageHeader.imageHeight;

    for (int i = imageHeader.imageHeight - 1; i >= 0 ; --i)
    //for (int i = 0; i < imageHeader.imageHeight; ++i)
    {
        uint32_t* row = bitMap->data + i * imageHeader.imageWidth;
        ReadFile(bmpFile, (void*)row, sizeof(uint32_t) * imageHeader.imageWidth);
    }

    CloseFile(bmpFile);
    return bitMap;
}

void InitializeTextureManager(TextureManager* tm)
{
    tm->count = 0;

}

Texture* CreateTexture(BitMap* bitmap, int2 tileDim, int2 tileSize)
{
    static size_t texture_count = 0;
    //assert(tm->count < 1024);

    //Texture* t = &tm->textures[tm->count];
    Texture* t = (Texture*)Alloc(sizeof(Texture));
    //t->unit = (uint32_t)tm->count;
    t->unit = (uint32_t)texture_count;
    t->handle = 0;
    t->width = bitmap->width;
    t->height = bitmap->height;
    t->tileDim = tileDim;
    t->tileSize = tileSize;

    glGenTextures(1, &t->handle);
    glBindTexture(GL_TEXTURE_2D, t->handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, bitmap->width, bitmap->height);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0, 0, 0,
        bitmap->width,
        bitmap->height, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE,
        bitmap->data);

    glActiveTexture(GL_TEXTURE0 + t->unit);
    glBindTexture(GL_TEXTURE_2D, 0);

    //++(tm->count);
    ++texture_count;

    return t;
}