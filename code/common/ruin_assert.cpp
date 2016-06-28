#include "ruin_assert.h"

#include <cassert>

using namespace Ruin;

void Ruin::rAssert(rsResult result)
{
    assert(result.result == reResultType::rrt_Success);
}

void Ruin::rAssert(bool t)
{
    assert(t);
}

void Ruin::rAssert(void* p)
{
    assert(p);
}