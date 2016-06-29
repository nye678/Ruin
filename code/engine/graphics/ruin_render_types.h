#ifndef __RUIN_RENDER_TYPES_
#define __RUIN_RENDER_TYPES_

#include "ruin_texture.h"

namespace Ruin
{
    union RenderKey
    {
        uint64_t key;
        struct
        {
            uint64_t materialId: 30;
            uint64_t depth: 24;
            uint64_t translucencyType: 2;
            uint64_t viewportLayer: 3;
            uint64_t viewport: 3;
            uint64_t fullscreenLayer: 2;
        };
    };

    struct RenderCommand
    {
        RenderKey key;
        Texture* texture;
        float2 position;
        float depth;
        uint32_t index;
    };
};
#endif