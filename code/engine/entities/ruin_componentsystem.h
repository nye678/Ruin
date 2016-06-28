#ifndef __RUIN_COMPONENT_SYSTEM_
#define __RUIN_COMPONENT_SYSTEM_

#include "ruin_allocator.h"
#include "ruin_entity.h"
#include "ruin_render.h"

namespace Ruin
{
    enum reComponentType
    {
        rCT_Transform,
        rCT_Sprite,
        rCT_Animation
    };

    struct rsComponentSystem;

    rsComponentSystem* rCreateComponentSystem(rsAllocator&);
    void* rCreateComponent(rsComponentSystem*, rsEntity, reComponentType);
    void rUpdateComponentSystem(rsComponentSystem*, double dt);
    void rRenderComponentSystem(rsComponentSystem*, Renderer*);
}

#endif