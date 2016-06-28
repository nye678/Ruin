#include "ruin_input.h"
#include "ruin_memory.h"

#include <Windows.h>
#include <xinput.h>
#include <cstring>

#define DPAD_UP(gamePad)         (gamePad->wButtons & XINPUT_GAMEPAD_DPAD_UP)
#define DPAD_DOWN(gamePad)       (gamePad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
#define DPAD_LEFT(gamePad)       (gamePad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
#define DPAD_RIGHT(gamePad)      (gamePad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
#define START(gamePad)           (gamePad->wButtons & XINPUT_GAMEPAD_START)
#define BACK(gamePad)            (gamePad->wButtons & XINPUT_GAMEPAD_BACK)
#define LEFT_THUMB(gamePad)      (gamePad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
#define RIGHT_THUMB(gamePad)     (gamePad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
#define LEFT_SHOULDER(gamePad)   (gamePad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
#define RIGHT_SHOULDER(gamePad)  (gamePad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
#define A(gamePad)               (gamePad->wButtons & XINPUT_GAMEPAD_A)
#define B(gamePad)               (gamePad->wButtons & XINPUT_GAMEPAD_B)
#define X(gamePad)               (gamePad->wButtons & XINPUT_GAMEPAD_X)
#define Y(gamePad)               (gamePad->wButtons & XINPUT_GAMEPAD_Y)

namespace Ruin
{
    reButtonState rUpdateGamePadButton(XINPUT_GAMEPAD* pad, uint32_t button, reButtonState prevState);
}

using namespace Ruin;

struct Ruin::rsInputSystem
{
    rsKeyboardState keyboard;
    rsControllerState controllers[XUSER_MAX_COUNT];
};

reButtonState Ruin::rUpdateButton(reButtonState current, reButtonState prev)
{
    if (current == rBS_Pressed)
    {
        if (prev == rBS_Pressed || prev == rBS_Held)
            return rBS_Held;
        else
            return rBS_Pressed;
    }
    else if (prev == rBS_Pressed || prev == rBS_Held)
        return rBS_Released;
    else
        return rBS_None;
}

void Ruin::rUpdateKeys(rsKeyboardState* keyboard)
{
    size_t numButtons = sizeof(keyboard->keys) / sizeof(keyboard->keys[0]);
    for (size_t i = 0; i < numButtons; ++i)
    {
        if (keyboard->keys[i] == rBS_Pressed)
            keyboard->keys[i] = rBS_Held;
        else if (keyboard->keys[i] == rBS_Released)
            keyboard->keys[i] = rBS_None;
    }
}

rsInputSystem* Ruin::rCreateInputSystem(rsAllocator& allocator)
{
    rsInputSystem* is = (rsInputSystem*)rAlloc(allocator, sizeof(rsInputSystem));
    memset(is, 0, sizeof(rsInputSystem));
    return is;
}

void Ruin::rDestroyInputSystem(rsInputSystem* is)
{
    if (is)
        Release(is);
}

reButtonState Ruin::rUpdateGamePadButton(XINPUT_GAMEPAD* pad, uint32_t button, reButtonState prevState)
{
    reButtonState currentState = (pad->wButtons & button) == 0 ? rBS_Released : rBS_Pressed;
    return rUpdateButton(currentState, prevState);
}

void Ruin::rUpdateControllerState(rsInputSystem* is)
{
    DWORD result;
    XINPUT_STATE state;
    
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        result = XInputGetState(i, &state);
        if (result == ERROR_SUCCESS)
        {
            XINPUT_GAMEPAD *pad = &state.Gamepad;
            rsControllerState* cs = &is->controllers[i];

            cs->up          = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_DPAD_UP, cs->up);
            cs->down        = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_DPAD_DOWN, cs->down);
            cs->left        = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_DPAD_LEFT, cs->left);
            cs->right       = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_DPAD_RIGHT, cs->right);
            cs->start       = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_START, cs->start);
            cs->back        = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_BACK, cs->back);
            cs->lThumb      = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_LEFT_THUMB, cs->lThumb);
            cs->rThumb      = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_RIGHT_THUMB, cs->rThumb);
            cs->lShoulder   = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_LEFT_SHOULDER, cs->lShoulder);
            cs->rShoulder   = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_RIGHT_SHOULDER, cs->rShoulder);
            cs->aButton     = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_A, cs->aButton);
            cs->bButton     = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_B, cs->bButton);
            cs->xButton     = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_X, cs->xButton);
            cs->yButton     = rUpdateGamePadButton(pad, XINPUT_GAMEPAD_Y, cs->yButton);
    
            cs->lStickX = pad->sThumbLX;
            cs->lStickY = pad->sThumbLY;
            cs->rStickX = pad->sThumbRX;
            cs->rStickY = pad->sThumbRY;
        }
    }
}

rsControllerState Ruin::rGetControllerState(rsInputSystem* is, size_t number)
{
    if (number < XUSER_MAX_COUNT)
        return is->controllers[number];
    return rsControllerState();
}

void Ruin::rUpdateKeyboardState(rsInputSystem* is)
{
    rUpdateKeys(&is->keyboard);
}

void Ruin::rUpdateKeyState(rsInputSystem* is, uint8_t key, reButtonState state)
{
    is->keyboard.keys[key] = rUpdateButton(state, is->keyboard.keys[key]);
}

reButtonState Ruin::rGetKeyState(rsInputSystem* is, uint8_t key)
{
    return is->keyboard.keys[key];
}

bool Ruin::rIsKeyDown(rsInputSystem* is, uint8_t key)
{
    return is->keyboard.keys[key] == rBS_Released || is->keyboard.keys[key] == rBS_Held;
}

rsKeyboardState Ruin::rGetKeyboardState(rsInputSystem* is)
{
    return is->keyboard;
}