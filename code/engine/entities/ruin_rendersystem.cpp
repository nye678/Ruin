#include "ruin_rendersystem.h"

using namespace Ruin;

void Ruin::rRenderSystemComponentUpdate(rsSpriteComponent* sprite, rsAnimationComponent* anims, double dt)
{
    anims->timer += dt;
    if (anims->timer >= anims->spf)
    {
        sprite->currentFrameIndex += 1;
        sprite->currentFrameIndex = sprite->currentFrameIndex >= 24 ? 0 : sprite->currentFrameIndex;
        anims->timer = 0;
    }

}

void Ruin::rRenderSystemRenderSprite(Renderer* renderer, rsSpriteComponent* sprite, rsTransformComponent* transform)
{
    Draw(
        renderer,
        sprite->texture,
        Float2(transform->position.x, transform->position.y),
        transform->position.z,
        sprite->currentFrameIndex,
        (reFullScreenLayer)(uint32_t)0);
}
