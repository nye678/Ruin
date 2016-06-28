#include "ruin_application.h"

#include "ruin_assert.h"
#include "ruin_memory.h"

#include <Windows.h>
#include <GL\gl3w.h>

namespace Ruin
{
    struct rsWindowHandle
    {
        HWND hwnd;
        HDC hdc;
        HGLRC hglrc;
    };

    LRESULT CALLBACK rWindowProc(HWND, UINT, WPARAM, LPARAM);
}

using namespace Ruin;

rsResult Ruin::rInitializeApplication(rsApplicationData* appData)
{
    if (!appData)
        return rResult(rrt_Fail, "app data null.");

    appData->allocator = rCreateAllocator(
        nullptr,
        ([](void* pool, size_t bytes) -> void* { return Alloc(bytes); }),
        ([](void* pool, void* ptr) -> void { Release(ptr); }));

    appData->windowOptions = (rsWindowOptions*)rAlloc(appData->allocator, sizeof(rsWindowOptions));
    if (!appData->windowOptions)
        return rResult(rrt_Fail, "failed to alloc window options.");
    appData->windowOptions->width = 1024;
    appData->windowOptions->height = 768;
    appData->windowOptions->title = "Ruin";

    appData->inputSystem = rCreateInputSystem(appData->allocator);
    if (!appData->inputSystem)
        return rResult(rrt_Fail, "failed to create input system.");

    appData->componentSystem = rCreateComponentSystem(appData->allocator);

    return rCreateWindow(appData);
}

rsResult Ruin::rCreateWindow(rsApplicationData* appData)
{
    if (!appData)
        return rResult(rrt_Fail, "app data null.");
    if (!appData->windowOptions)
        return rResult(rrt_Fail, "no window options while trying to create window.");
    if (appData->windowHandle)
        return rResult(rrt_Fail, "a window has already been created.");

    appData->windowHandle = (rsWindowHandle*)rAlloc(appData->allocator, sizeof(rsWindowHandle));

    HMODULE instance = GetModuleHandle(NULL);

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = rWindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = "RuinWindowClass";

    if (!RegisterClassEx(&wc))
        return rResult(rrt_Fail, "failed to register window class.");

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        appData->windowOptions->title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, appData->windowOptions->width, appData->windowOptions->height,
        0, 0,
        instance,
        (void*)appData);

    return rResult();
}

void Ruin::rDestroyWindow(rsApplicationData* appData)
{
    if (appData && appData->windowHandle)
    {
        wglDeleteContext(appData->windowHandle->hglrc);
        DestroyWindow(appData->windowHandle->hwnd);

        rRelease(appData->allocator, appData->windowHandle);
        appData->windowHandle = nullptr;
    }
}

void Ruin::rSwapBuffers(rsApplicationData* appData)
{
    SwapBuffers(appData->windowHandle->hdc);
}

LRESULT CALLBACK Ruin::rWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCCREATE)
    {
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(lpcs->lpCreateParams));
        return TRUE;
    }

    rsApplicationData* appData = (rsApplicationData*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (appData)
    {
        switch (message)
        {
            case WM_KEYDOWN:
                rUpdateKeyState(appData->inputSystem, (uint8_t)wParam, rBS_Pressed);
                break;
            case WM_KEYUP:
                rUpdateKeyState(appData->inputSystem, (uint8_t)wParam, rBS_Released);
                break;
            case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            break;
            case WM_CREATE:
            {
                rsWindowHandle* window_handle = appData->windowHandle;
                window_handle->hwnd = hwnd;

                PIXELFORMATDESCRIPTOR pfd =
                {
                    sizeof(PIXELFORMATDESCRIPTOR),
                    1,
                    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
                    PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
                    32,                        //Colordepth of the framebuffer.
                    0, 0, 0, 0, 0, 0,
                    0,
                    0,
                    0,
                    0, 0, 0, 0,
                    24,                        //Number of bits for the depthbuffer
                    8,                        //Number of bits for the stencilbuffer
                    0,                        //Number of Aux buffers in the framebuffer.
                    PFD_MAIN_PLANE,
                    0,
                    0, 0, 0
                };

                window_handle->hdc = GetDC(hwnd);

                int pixelFormat = ChoosePixelFormat(window_handle->hdc, &pfd);
                SetPixelFormat(window_handle->hdc, pixelFormat, &pfd);

                window_handle->hglrc = wglCreateContext(window_handle->hdc);
                wglMakeCurrent(window_handle->hdc, window_handle->hglrc);

                gl3wInit();

                appData->renderer = CreateRenderer();
            }
            break;
            default:
            {
                return DefWindowProc(hwnd, message, wParam, lParam);
            }
        }
    }

    return 0;
}