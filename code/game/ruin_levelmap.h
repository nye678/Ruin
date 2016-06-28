#ifndef __RUIN_LEVELMAP_
#define __RUIN_LEVELMAP_

#include "ruin_int.h"

struct LevelMap
{
    uint16_t **tiles;
    uint32_t numLayers;
    uint32_t width;
    uint32_t height;
};

LevelMap* LoadLevelMap(const char* filePath);

#endif