#ifndef __RUIN_TILESET_
#define __RUIN_TILESET_

#include "ruin_vector.h"
#include "ruin_texture.h"

struct TileDefinition
{
    bool block;
    bool stair;
    bool visible;
    bool animated;
    uint8_t frames;
};

struct TileSet
{
    Texture* texture;
    //Ruin::rsVector* tileDefinitions;
};

TileSet* LoadTileSet(const char* filePath);

TileDefinition GetTileDefinition(TileSet*, size_t tileIndex);

#endif