#include "ruin_entity.h"

using namespace Ruin;

rsEntity Ruin::rCreateEntity()
{
    static uint32_t s_idSource = 0;
    //rsEntity newEntity = {++s_idSource};
    return ++s_idSource;
}