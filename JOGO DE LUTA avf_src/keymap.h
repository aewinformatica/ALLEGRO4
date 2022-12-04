#ifndef KEYMAP_H
#define KEYMAP_H



#define DEV_KBD 0
#define DEV_JOY 1



typedef struct KEYMAP KEYMAP;



struct KEYMAP
{
        int device;
        int up, down, left, right;
        int hpunch, lpunch;
        int hkick, lkick;
        int block;
};



extern KEYMAP default_keymap_1;
extern KEYMAP default_keymap_2;



#endif

