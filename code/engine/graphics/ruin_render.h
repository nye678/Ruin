#ifndef __RUIN_RENDER_
#define __RUIN_RENDER_

#include "ruin_vec.h"
#include "ruin_texture.h"

enum reFullScreenLayer
{
    Layer0 = 0x0,
    Layer1 = 0x1,
    Layer2 = 0x2,
    Layer3 = 0x3,
};

struct Renderer;

Renderer* CreateRenderer();
void Draw(Renderer* r, float2 position, float depth, uint32_t index, reFullScreenLayer layer);
void Draw(Renderer* r, Texture* texture, float2 position, float depth, uint32_t index, reFullScreenLayer layer);
void DebugDrawSquare(Renderer* r, float2 position, float2 size, bool filled);
void BeginRender(Renderer*);
void EndRender(Renderer*);

#endif