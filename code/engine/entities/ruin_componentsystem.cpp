#include "ruin_componentsystem.h"

#include "ruin_transformcomponent.h"
#include "ruin_spritecomponent.h"
#include "ruin_animationcomponent.h"
#include "ruin_rendersystem.h"
#include "ruin_vector.h"
#include "ruin_int.h"
#include "ruin_memory.h"

#include "ruin_hashtable.h"

using namespace Ruin;

typedef rsVector<rsTransformComponent> rsTransformComponentVector;
typedef rsVector<rsSpriteComponent> rsSpriteComponentVector;
typedef rsVector<rsAnimationComponent> rsAnimationComponentVector;
typedef rsHashTable<rsEntity, uint32_t, rEntityHashFunc, 3079> rsTransformComponentHashTable;
typedef rsHashTable<rsEntity, uint32_t, rEntityHashFunc, 3079> rsSpriteComponentHashTable;
typedef rsHashTable<rsEntity, uint32_t, rEntityHashFunc, 3079> rsAnimationComponentHashTable;

struct Ruin::rsComponentSystem
{
    rsAllocator allocator;
    rsTransformComponentVector transforms;
    rsSpriteComponentVector sprites;
    rsAnimationComponentVector anims;
    rsTransformComponentHashTable* transformLookup;
    rsSpriteComponentHashTable* spritesLookup;
    rsAnimationComponentHashTable* animsLookup;
};

rsComponentSystem* Ruin::rCreateComponentSystem(rsAllocator& allocator)
{
    rsComponentSystem* system = (rsComponentSystem*)rAlloc(allocator, sizeof(rsComponentSystem));
    system->allocator = allocator;

    rInitialize(system->transforms);
    rInitialize(system->sprites);
    rInitialize(system->anims);

    system->transformLookup = (rsTransformComponentHashTable*)rAlloc(allocator, sizeof(rsTransformComponentHashTable));
    *(system->transformLookup) = rsTransformComponentHashTable(allocator);
    system->spritesLookup = (rsSpriteComponentHashTable*)rAlloc(allocator, sizeof(rsSpriteComponentHashTable));
    *(system->spritesLookup) = rsSpriteComponentHashTable(allocator);
    system->animsLookup = (rsAnimationComponentHashTable*)rAlloc(allocator, sizeof(rsAnimationComponentHashTable));
    *(system->animsLookup) = rsAnimationComponentHashTable(allocator);

    return system;
}

void* Ruin::rCreateComponent(rsComponentSystem* system, rsEntity entity, reComponentType type)
{
    switch (type)
    {
        case rCT_Transform:
        {
            rsTransformComponent transform = {0};
            transform.entity = entity;
            
            rPushBack(system->transforms, transform);
            rsTransformComponent* newComp = rBack(system->transforms);
            system->transformLookup->insert(entity, system->transforms.length - 1);
            return newComp;
        }
        break;
        case rCT_Sprite:
        {
            rsSpriteComponent sprite = {0};
            sprite.entity = entity;

            rPushBack(system->sprites, sprite);
            rsSpriteComponent* newComp = rBack(system->sprites);
            system->spritesLookup->insert(entity, system->sprites.length - 1);
            return newComp;
        }
        break;
        case rCT_Animation:
        {
            rsAnimationComponent anims = {0};
            anims.entity = entity;

            rPushBack(system->anims, anims);
            rsAnimationComponent* newComp = rBack(system->anims);
            system->animsLookup->insert(entity, system->anims.length - 1);
            return newComp;
        }
        break;
        default:
            return nullptr;
    }
}

template <typename T>
int32_t rComponentSortFunc(void* c1, void* c2)
{
    T* tc1 = (T*)c1;
    T* tc2 = (T*)c2;
    return tc1->entity < tc2->entity ? -1 : (tc1->entity > tc2->entity ? 1 : 0);
}

void Ruin::rUpdateComponentSystem(rsComponentSystem* components, double dt)
{
    // Render System Update     
    /*for (int spriteIndex = 0; spriteIndex < components->sprites.length; ++spriteIndex)
    {
        rsSpriteComponent* sprite = rGet(components->sprites, spriteIndex);
		uint32_t animsIndex = 0;
        if (sprite && components->animsLookup->lookup(sprite->entity, animsIndex))
        {
			rsAnimationComponent* anims = rGet(components->anims, animsIndex);
            rRenderSystemComponentUpdate(sprite, anims, dt);
        }
    }*/

	rsSpriteComponent* sprites = components->sprites.array;
	rsAnimationComponent* anims = components->anims.array;

	size_t spriteIndex = 0;
	size_t animIndex = 0;

	for (; spriteIndex < components->sprites.length 
		&& animIndex < components->anims.length;
		++spriteIndex)
	{
		rsSpriteComponent* sprite = sprites + spriteIndex;
		rsAnimationComponent* anim = anims + animIndex;
		if (sprite->entity == anim->entity)
		{
			rRenderSystemComponentUpdate(sprite, anim, dt);
			++animIndex;
		}
	}
}

void Ruin::rRenderComponentSystem(rsComponentSystem* components, Renderer* renderer)
{
    rsSpriteComponent* sprites = components->sprites.array;
    rsTransformComponent* transforms = components->transforms.array;

    int spriteIndex = 0; 
    int transformIndex = 0;
    for (; spriteIndex < components->sprites.length
        && transformIndex < components->transforms.length;
        ++spriteIndex)
    {
        rsSpriteComponent* sprite = sprites + spriteIndex;
        rsTransformComponent* transform = transforms + transformIndex;
        if (sprite->entity == transform->entity)
        {
            rRenderSystemRenderSprite(renderer, sprite, transform);
            ++transformIndex;
        }
    }

    /*rsSpriteComponent* sprites = components->sprites.array;
    rsTransformComponent* transforms = components->transforms.array;

    int spriteIndex = 0; 
    for (; spriteIndex < components->sprites.length; ++spriteIndex)
    {
        rsSpriteComponent* sprite = sprites + spriteIndex;
		uint32_t transformIndex = 0;
        if (sprite != nullptr && components->transformLookup->lookup(sprite->entity, transformIndex))
        {
			rsTransformComponent* transform = rGet(components->transforms, transformIndex);
            rRenderSystemRenderSprite(renderer, sprite, transform);
        }
    }*/
}
