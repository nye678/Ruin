#ifndef __RUIN_ANIMATION_COMPONENT_
#define __RUIN_ANIMATION_COMPONENT_

#include "ruin_entity.h"

namespace Ruin
{
    enum reAnimationState
    {
        rAS_Idle,
        rAS_Walk_Up,
        rAS_Walk_Down,
        rAS_Walk_Right,
        rAS_Walk_Left
    };

    struct rsAnimationComponent
    {
        rsEntity entity;
        reAnimationState state;
        double timer, spf;
    };
}

#endif