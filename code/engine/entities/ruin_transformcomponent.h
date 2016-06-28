#ifndef __RUIN_TRANSFORM_COMPONENT_
#define __RUIN_TRANSFORM_COMPONENT_

#include "ruin_entity.h"
#include "ruin_vec.h"

namespace Ruin
{
    struct rsTransformComponent
    {
        rsEntity entity;
        float4 position;
        float4 orientation;
        float3 scale;
    };
}

#endif