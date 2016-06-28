#ifndef __RUIN_SPRITE_COMPONENT_
#define __RUIN_SPRITE_COMPONENT_

#include "ruin_entity.h"
#include "ruin_int.h"
#include "ruin_texture.h"

namespace Ruin
{
    /*
     * Sprite Component struct
     * Entities with a sprite component renders a 2d, usually animated image.
     */
    struct rsSpriteComponent
    {
        rsEntity entity;
        Texture* texture;

        uint32_t currentFrameIndex;
    };
}

#endif