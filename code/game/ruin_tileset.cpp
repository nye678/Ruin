#include "ruin_tileset.h"

#include "ruin_file.h"
#include "ruin_memory.h"
#include "ruin_texture.h"

#include <rapidxml/rapidxml.hpp>
#include <cstdio>

using namespace rapidxml;

TileSet* LoadTileSet(const char* filePath)
{
    printf("Loading Tile Set: %s\n", filePath);

    char* buffer = nullptr;
    size_t bufferSize = LoadTextFile(filePath, buffer);

    assert(bufferSize > 0);

    xml_document<> doc;
    doc.set_allocator(Alloc, Release);

    doc.parse<0>(buffer);
    xml_node<> *node = doc.first_node();

    TileSet* tileSet = (TileSet*)Alloc(sizeof(TileSet));

    const char* tileWidthAttrName = "tilewidth";
    int tileWidth = atoi(node->first_attribute(tileWidthAttrName)->value());
    const char* tileHeightAttrName = "tileheight";
    int tileHeight = atoi(node->first_attribute(tileHeightAttrName)->value());

    xml_node<> *imageNode = node->first_node("image");
    int imageWidth = atoi(imageNode->first_attribute("width")->value());
    int imageHeight = atoi(imageNode->first_attribute("height")->value());

    BitMap* lamesetbmp = LoadBMP("D:\\Projects\\Ruin\\data\\textures\\lameset.bmp");
    tileSet->texture = CreateTexture(lamesetbmp, Int2(imageWidth/tileWidth, imageHeight/tileHeight), Int2(tileWidth, tileHeight));

    return tileSet;
}

TileDefinition GetTileDefinition(TileSet*, size_t tileIndex)
{
    return TileDefinition();
}