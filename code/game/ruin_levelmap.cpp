#include "ruin_levelmap.h"

#include "ruin_file.h"
#include "ruin_memory.h"

#include <rapidxml/rapidxml.hpp>
#include <cstdio>

using namespace rapidxml;

LevelMap* LoadLevelMap(const char* filePath)
{
    char* buffer = nullptr;
    size_t bufferSize = LoadTextFile(filePath, buffer);

    assert(bufferSize > 0);

    xml_document<> doc;
    doc.set_allocator(Alloc, Release);

    doc.parse<0>(buffer);
    xml_node<> *node = doc.first_node();
    //printf("Name of my first node is: %s\n", node->name());
    //for (xml_attribute<> *attr = node->first_attribute();
    //    attr; attr = attr->next_attribute())
    //{
    //    printf("Node has attribute %s with value %s.\n", attr->name(), attr->value());
    //}

    xml_node<> *layer = node->first_node("layer");
    if (layer)
    {
        xml_node<> *data = layer->first_node("data");
        printf("%s\n", data->value());
    }

    //for (xml_node<> *n = node->first_node(); n; n = n->next_sibling())
    //{
    //    printf("Node has child node %s.\n", n->name());
    //    for (xml_attribute<> *attr = n->first_attribute();
    //    attr; attr = attr->next_attribute())
    //    {
    //        printf("\tNode has attribute %s with value %s.\n", attr->name(), attr->value());
    //    }
    //}



    return 0;
}