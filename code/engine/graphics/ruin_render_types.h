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
            uint64_t materialId: 29;
            uint64_t depth: 24;
            uint64_t command: 1;
            uint64_t translucencyType: 2;
            uint64_t viewportLayer: 3;
            uint64_t viewport: 3;
            uint64_t fullscreenLayer: 2;
        };
    };

    struct rsDrawTexture
    {
        Texture* texture;
        float2 position;
        float depth;
        uint32_t index;
    };

    struct rsDrawLine
    {
        float2 start;
        float2 end;
    };

    struct RenderCommand
    {
        RenderKey key;
        union
        {        
            rsDrawTexture drawTexture;
            rsDrawLine drawLine;
        };
    };
};
#endif