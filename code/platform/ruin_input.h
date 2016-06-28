#ifndef __RUIN_INPUT_
#define __RUIN_INPUT_

#include "ruin_allocator.h"
#include "ruin_int.h"

#define KEY_BACKSPACE 0x08
#define KEY_TAB       0x09

#define KEY_ENTER     0x0D

#define KEY_SHIFT     0x10
#define KEY_CONTROL   0x11

#define KEY_ESCAPE    0x1B

#define KEY_SPACE     0x20
#define KEY_PAGEUP    0x21
#define KEY_PAGEDOWN  0x22
#define KEY_END       0x23
#define KEY_HOME      0x24
#define KEY_LEFT      0x25
#define KEY_UP        0x26
#define KEY_RIGHT     0x27
#define KEY_DOWN      0x28

#define KEY_INSERT    0x2D
#define KEY_DELETE    0x2E

#define KEY_0         '0'
#define KEY_1         '1'
#define KEY_2         '2'
#define KEY_3         '3'
#define KEY_4         '4'
#define KEY_5         '5'
#define KEY_6         '6'
#define KEY_7         '7'
#define KEY_8         '8'
#define KEY_9         '9'

#define KEY_A         'A'
#define KEY_B         'B'
#define KEY_C         'C'
#define KEY_D         'D'
#define KEY_E         'E'
#define KEY_F         'F'
#define KEY_G         'G'
#define KEY_H         'H'
#define KEY_I         'I'
#define KEY_J         'J'
#define KEY_K         'K'
#define KEY_L         'L'
#define KEY_M         'M'
#define KEY_N         'N'
#define KEY_O         'O'
#define KEY_P         'P'
#define KEY_Q         'Q'
#define KEY_R         'R'
#define KEY_S         'S'
#define KEY_T         'T'
#define KEY_U         'U'
#define KEY_V         'V'
#define KEY_W         'W'
#define KEY_X         'X'
#define KEY_Y         'Y'
#define KEY_Z         'Z'

#define KEY_F1        0x70
#define KEY_F2        0x71
#define KEY_F3        0x72
#define KEY_F4        0x73
#define KEY_F5        0x74
#define KEY_F6        0x75
#define KEY_F7        0x76
#define KEY_F8        0x77
#define KEY_F9        0x78
#define KEY_F10       0x79
#define KEY_F11       0x7A
#define KEY_F12       0x7B

#define KEY_LSHIFT    0xA0
#define KEY_RSHIFT    0xA1
#define KEY_LCONTROL  0xA2
#define KEY_RCONTROL  0xA3

#define KEY_COLON 0xBA
#define KEY_PLUS 0xBB
#define KEY_COMMA 0xBC
#define KEY_MINUS 0xBD
#define KEY_PERIOD 0xBE
#define KEY_GRAVETILDA 0xC0
#define KEY_LEFTBRACKET 0xDB
#define KEY_BACKSLASH 0xDC
#define KEY_RIGHTBRACKET 0xDD
#define KEY_QUOTE 0xDE

namespace Ruin
{
    enum reButtonState
    {
        rBS_None = 0, rBS_Released, rBS_Pressed, rBS_Held
    };

    struct rsControllerState
    {
        int16_t lStickX;
        int16_t lStickY;
        int16_t rStickX;
        int16_t rStickY;
        reButtonState up;
        reButtonState down;
        reButtonState left;
        reButtonState right;
        reButtonState start;
        reButtonState back;
        reButtonState lThumb;
        reButtonState rThumb;
        reButtonState lShoulder;
        reButtonState rShoulder;
        reButtonState aButton;
        reButtonState bButton;
        reButtonState xButton;
        reButtonState yButton;
    };

    struct rsKeyboardState
    {
        reButtonState keys[0xFF];
    };

    struct rsInputSystem;

    reButtonState rUpdateButton(reButtonState current, reButtonState prev);
    void rUpdateKeys(rsKeyboardState* keyboard);

    rsInputSystem* rCreateInputSystem(rsAllocator&);
    void rDestroyInputSystem(rsInputSystem*);

    void rUpdateControllerState(rsInputSystem*);
    rsControllerState rGetControllerState(rsInputSystem*, size_t controller);

    void rUpdateKeyboardState(rsInputSystem*);
    void rUpdateKeyState(rsInputSystem*, uint8_t key, reButtonState state);
    reButtonState rGetKeyState(rsInputSystem*, uint8_t key);
    bool rIsKeyDown(rsInputSystem*, uint8_t key);
    rsKeyboardState rGetKeyboardState(rsInputSystem*);
}

#endif