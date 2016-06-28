#ifndef __RUIN_ENTITY_
#define __RUIN_ENTITY_

#include "ruin_int.h"

namespace Ruin
{
    typedef uint32_t rsEntity;

    inline int32_t rEntityHashFunc(rsEntity entity)
    {
       return entity;
    }

    rsEntity rCreateEntity();
}

#endif