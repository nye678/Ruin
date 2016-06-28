#ifndef __RUIN_RENDER_SYSTEM_
#define __RUIN_RENDER_SYSTEM_

#include "ruin_allocator.h"
#include "ruin_entity.h"
#include "ruin_spritecomponent.h"
#include "ruin_transformcomponent.h"
#include "ruin_animationcomponent.h"
#include "ruin_componentsystem.h"
#include "ruin_render.h"

namespace Ruin
{
    void rRenderSystemComponentUpdate(rsSpriteComponent*, rsAnimationComponent*, double dt);
    void rRenderSystemRenderSprite(Renderer*, rsSpriteComponent*, rsTransformComponent*);
}

#endif