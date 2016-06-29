#include "ruin_application.h"
#include "ruin_memory.h"
#include "ruin_render.h"
#include "ruin_math.h"
#include "ruin_input.h"
#include "ruin_levelmap.h"
#include "ruin_tileset.h"
#include "ruin_int.h"

#include <Windows.h>
#include <GL\gl3w.h>

#include "ruin_transformcomponent.h"
#include "ruin_spritecomponent.h"
#include "ruin_animationcomponent.h"
#include <cstdlib>
#include <cstdio>

using namespace Ruin;

rsApplicationData g_appData;

double time()
{
    static LARGE_INTEGER startingTime = {0};
    static LARGE_INTEGER frequency = {0};
    if (startingTime.QuadPart == 0)
    {
        QueryPerformanceCounter(&startingTime);
        QueryPerformanceFrequency(&frequency);
        return 0.0;
    }

    LARGE_INTEGER counter = {0};
    QueryPerformanceCounter(&counter);

    return (double)((counter.QuadPart - startingTime.QuadPart) / double(frequency.QuadPart));
}

void testCreateStuff()
{
    BitMap* bmp = LoadBMP("D:\\Projects\\Ruin\\data\\textures\\rpg_sprite_walk.bmp");
    Texture* tex = CreateTexture(bmp, Int2(8, 4), Int2(24, 32));
    BitMap* bgBmp = LoadBMP("D:\\Projects\\Ruin\\data\\textures\\lameset.bmp");
    Texture* bgTex = CreateTexture(bgBmp, Int2(32, 32), Int2(16, 16));

    int k = 0;
    for (int i = 0; i < 32; ++i)
    {
        for (int j = 0; j < 32; ++j)
        {
            rsEntity ent = rCreateEntity();
            rsTransformComponent* transform = (rsTransformComponent*)rCreateComponent(g_appData.componentSystem, ent, rCT_Transform);
            transform->position.x = 16.0f * j;
            transform->position.y = 16.0f * i;
            transform->position.z = 1.0f;
            rsSpriteComponent* sprite = (rsSpriteComponent*)rCreateComponent(g_appData.componentSystem, ent, rCT_Sprite);
            sprite->texture = bgTex;
            sprite->currentFrameIndex = k++;
        }
    }

    for (int i = 20; i >= 0; --i)
    {
        rsEntity ent = rCreateEntity();
        rsTransformComponent* transform = (rsTransformComponent*)rCreateComponent(g_appData.componentSystem, ent, rCT_Transform);
        transform->position.x = 24.0f + (8.0f * i);
        transform->position.y = 16.0f * i + 16.0f;
        transform->position.z = transform->position.y;
        rsSpriteComponent* sprite = (rsSpriteComponent*)rCreateComponent(g_appData.componentSystem, ent, rCT_Sprite);
        sprite->texture = tex;
        sprite->currentFrameIndex = rand() % 24;
        rsAnimationComponent* anims = (rsAnimationComponent*)rCreateComponent(g_appData.componentSystem, ent, rCT_Animation);
        anims->spf = 1/8.0;
    }
}

int main(int argc, char* argv[])
{
    if (!InitGlobalMemory())
        return 0;

    rsResult result = rInitializeApplication(&g_appData);
    if (!rSuccessfulResult(result))
    {
        ReleaseGlobalMemory();
        return 0;
    }

    testCreateStuff();

    double t = 0.0, dt = 0.01;
    double currentTime = 0.0;
    double accumulator = 0.0;

    double timer = 0.0;
    int frameCount = 0;

    MSG msg = {};
    bool running = true;
    while (running)
    {
        double newTime = time();
        double deltaTime = newTime - currentTime;
        currentTime = newTime;

        timer += deltaTime;
        if (timer > 1.0)
        {
            printf("%d\n", frameCount);
            frameCount = 0;
            timer -= 1.0;
        }
        ++frameCount;

        if (deltaTime > 0.25)
            deltaTime = 0.25;

        accumulator += deltaTime;

        while (accumulator >= dt)
        {
            accumulator -= dt;

            // Update Game
            while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    running = false;
                    break;
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            rUpdateComponentSystem(g_appData.componentSystem, dt);

            t += dt;
        }

        double alpha = accumulator / dt;

        // RENDER
        BeginRender(g_appData.renderer);
        rRenderComponentSystem(g_appData.componentSystem, g_appData.renderer);
        EndRender(g_appData.renderer);

        rSwapBuffers(&g_appData);
    }

    rDestroyInputSystem(g_appData.inputSystem);
    rDestroyWindow(&g_appData);
    ReleaseGlobalMemory();
    return 0;
}