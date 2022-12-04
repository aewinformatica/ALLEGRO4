#include <allegro.h>
#include "keymap.h"
#include "fighter.h"



KEYMAP default_keymap_1 =
{
        DEV_KBD, // device
        KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, // up, down, left, right
        KEY_A, KEY_S, // hpunch, lpunch
        KEY_W, KEY_E, // hkick, lkick
        KEY_Z, // block
};



KEYMAP default_keymap_2 =
{
        DEV_KBD,
        KEY_5_PAD, KEY_2_PAD, KEY_1_PAD, KEY_3_PAD,
        KEY_J, KEY_K,
        KEY_I, KEY_O,
        KEY_L,
};

