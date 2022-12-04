#ifndef ENGINE_H
#define ENGINE_H



#include "keymap.h"
#include "sprite.h"



#define MAX_ENERGY  100



#define animation_delay 5



#define ACTION_STAND      0x00
#define ACTION_WALK_FWD   0x01
#define ACTION_WALK_BWD   0x02
#define ACTION_JUMP       0x03
#define ACTION_JUMP_FWD   0x04
#define ACTION_JUMP_BWD   0x05
#define ACTION_HKICK      0x06
#define ACTION_LKICK      0x07
#define ACTION_HPUNCH     0x08
#define ACTION_LPUNCH     0x09
#define ACTION_BLOCK      0x0A
#define ACTION_CROUCH     0x0B
#define ACTION_SHOT       0x0C
#define ACTION_HIT        0x0D
#define ACTION_LAY        0x0E



#define ST_NONE     0x01
#define ST_STAND    ST_NONE
#define ST_WALK     0x02
#define ST_LPUNCH   0x03
#define ST_HPUNCH   0x04
#define ST_LKICK    0x05
#define ST_HKICK    0x06
#define ST_CROUCH   0x07
#define ST_JUMP     0x08
#define ST_BLOCK    0x09
#define ST_HIT      0x0A
#define ST_SHOT     0x0B
#define ST_LAY      0x0C



#define EV_UP     1
#define EV_DOWN   2
#define EV_FWD    3
#define EV_BWD    4



#define SPECIAL    15
#define STRONG     10
#define WEAK       5



#define HUMAN      0
#define CPU        1



typedef struct FIGHTER FIGHTER;
typedef struct EVENT EVENT;



struct EVENT
{
        int vdir;
        int hdir;
        int hpunch;
        int lpunch;
        int hkick;
        int lkick;
        int block;
};



struct FIGHTER
{
        char *name;
        char *dialog;
        int energy;
        int type;
        int dir; // 1 si mira a la izquierda, 0 de lo contrario
        int state;
        int strength;
        char *datafile;
        SPRITE *sprite;
        KEYMAP *keymap;
        FIGHTER *enemy;
        int (*init) (FIGHTER *self);
        int (*parse_event) (FIGHTER *self, EVENT event);
        void (*action_to_sprite) (FIGHTER *self, int action);
        void (*update_sprite) (FIGHTER *self);
        void (*destroy) (FIGHTER *self);
};



extern FIGHTER arina;
extern FIGHTER juliette;



void reset_fighter (FIGHTER *player);
void update_fighter (FIGHTER *player);
void fire_action (FIGHTER *player, int action);
void default_update_sprite (FIGHTER *player);



#endif
