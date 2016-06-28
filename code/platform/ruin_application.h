#ifndef __RUIN_APPLICATION_DATA_
#define __RUIN_APPLICATION_DATA_

#include "ruin_assert.h"
#include "ruin_input.h"
#include "ruin_memory.h"
#include "ruin_render.h"
#include "ruin_allocator.h"
#include "ruin_componentsystem.h"

namespace Ruin
{
    /**
     * Window Handle struct
     * Platform specific handle to the window.
     */
    struct rsWindowHandle;

    /**
     * Window Options struct
     * Options used during window construction.
     */
    struct rsWindowOptions
    {
        int width;
        int height;
        const char* title;
    };

    /**
     * ApplicationData
     * Struct holding all the global systems.
     */
    struct rsApplicationData
    {
        rsAllocator allocator;
        rsWindowHandle* windowHandle;
        rsWindowOptions* windowOptions;
        rsInputSystem* inputSystem;
        Renderer* renderer;
        rsComponentSystem* componentSystem;
    };

    /**
     */
    rsResult rInitializeApplication(rsApplicationData*);

    /**
     * Create Window function
     * Creates the new window.
     */
    rsResult rCreateWindow(rsApplicationData*);

    /**
     * Destroy Window function
     * Destroys a previously created window.
     */
    void rDestroyWindow(rsApplicationData*);

    void rSwapBuffers(rsApplicationData*);
}

#endif