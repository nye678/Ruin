#ifndef __RUIN_TEXTURE_
#define __RUIN_TEXTURE_

#include "ruin_int.h"
#include "ruin_math.h"

struct BitMap
{
    uint32_t* data;
    uint32_t width, height;
};

BitMap* LoadBMP(const char* filePath);

struct Texture
{
    uint32_t unit;
    uint32_t handle;
    uint32_t width;
    uint32_t height;
    int2 tileDim;
    int2 tileSize;
};

struct TextureManager
{
    Texture textures[1024];
    size_t count;
};

void InitializeTextureManager(TextureManager*);
Texture* CreateTexture(BitMap* bitmap, int2 tileDim, int2 tileSize);

#endif