#include <allegro.h>
#include "keymap.h"
#include "sprite.h"
#include "fighter.h"
#include "arina.h"



static int arina_init (FIGHTER *self);
static int arina_parse_event (FIGHTER *self, EVENT event);
static void arina_action_to_sprite (FIGHTER *self, int action);
static void arina_update_sprite (FIGHTER *self);
static void arina_destroy (FIGHTER *self);


static ANIMATION arina_anims [] =
{
        { 0, 1, 5, 0 }, // block
        { 0, 1, 5, 0 }, // crouch
        { 0, 4, 5, 0 }, // hit
        { 0, 6, 5, 0 }, // jump
        { 0, 4, 5, 0 }, // kick
        { 0, 1, 5, 0 }, // lay
        { 0, 4, 5, 0 }, // punch
        { 0, 5, 5, 0 }, // stand
        { 0, 4, 5, 0 }, // walk
};



static SPRITE arina_sprite =
{
                0,              // flags
                NULL,           // image
                { 0, 0 },       // speed
                { 200, 200 },   // size
                { 50, 200 },    // pos
                { 0, 0, 0, 0 }, // bound
                ARINA_S,        // default anim
                ARINA_S,        // current anim
                0,              // current frame
                0,              // frame timer
                arina_anims,    // anims
                NULL,           // data
};



FIGHTER arina =
{
        "Arina",                 // name
        "",                      // dialog
        MAX_ENERGY,              // energy
        HUMAN,                   // type
        0,                       // dir
        ST_NONE,                 // state
        0,                       // strength
        "arina.dat",             // datafile
        &arina_sprite,           // sprite
        &default_keymap_1,       // keymap
        NULL,                    // enemy
        arina_init,              // init
        arina_parse_event,       // parse_event
        arina_action_to_sprite,  // action_to_sprite
        arina_update_sprite,     // update_sprite
        arina_destroy,           // destroy
};



static int arina_init (FIGHTER *player)
{
        arina_sprite.pos.x = 50;
        arina_sprite.pos.y = 200;
        arina_sprite.data = load_datafile (player->datafile);
        if (arina_sprite.data == NULL)
                return 0;
        arina_sprite.image = (BITMAP *) arina_sprite.data [arina_sprite.default_anim].dat;
        return 1;
}



static void arina_destroy (FIGHTER *player)
{
        if (arina_sprite.data != NULL)
                unload_datafile (arina_sprite.data);
}



static int arina_state = 0;



static int arina_parse_event (FIGHTER *player, EVENT event)
{
        switch (arina_state)
        {
                case 0:
                        if (event.vdir == EV_DOWN)
                                arina_state = 1;
                        break;
                case 1:
                        arina_state = 0;
                        if (event.hdir == EV_FWD && event.hpunch)
                        {
                                return ACTION_SHOT;
                        }
                        break;
        }
        return event_to_action (event);
}



static void arina_action_to_sprite (FIGHTER *player, int action)
{
        SPRITE *sprite = player->sprite;
        reset_sprite (sprite);
        switch (action)
        {
                case ACTION_STAND:
                        sprite->current_anim = ARINA_S;
                        break;
                case ACTION_WALK_FWD:
                        sprite->current_anim = ARINA_W;
                        sprite->speed.x = 5 * (player->dir ? -1 : 1);
                        break;
                case ACTION_WALK_BWD:
                        sprite->current_anim = ARINA_W;
                        sprite->speed.x = 5 * (player->dir ? 1 : -1);
                        break;
                case ACTION_JUMP:
                        sprite->current_anim = ARINA_J;
                        break;
                case ACTION_JUMP_FWD:
                        sprite->current_anim = ARINA_J;
                        sprite->speed.x = 5 * (player->dir ? -1 : 1);
                        break;
                case ACTION_JUMP_BWD:
                        sprite->current_anim = ARINA_J;
                        sprite->speed.x = 5 * (player->dir ? 1 : -1);
                        break;
                case ACTION_HPUNCH:
                case ACTION_LPUNCH:
                        sprite->current_anim = ARINA_P;
                        break;
                case ACTION_HKICK:
                case ACTION_LKICK:
                        sprite->current_anim = ARINA_K;
                        break;
                case ACTION_BLOCK:
                        sprite->current_anim = ARINA_B;
                        break;
                case ACTION_CROUCH:
                        sprite->current_anim = ARINA_C;
                        break;
                case ACTION_HIT:
                        player->energy -= player->enemy->strength;
                        if (player->energy < 0)
                                player->energy = 0;
                        sprite->current_anim = ARINA_H;
                        sprite->speed.x = 5 * (player->dir ? 1 : -1);
                        break;
                case ACTION_SHOT:
                        sprite->current_anim = ARINA_P;
                        break;
                case ACTION_LAY:
                        sprite->current_anim = ARINA_L;
                        break;
        }
}



static void arina_update_sprite (FIGHTER *player)
{
        default_update_sprite (player);
        switch (player->state)
        {
                case ST_JUMP: player->dialog = "Hah!"; break;
                case ST_LPUNCH: player->dialog = "Take this!"; break;
                case ST_LKICK: player->dialog = "Have a kick"; break;
                case ST_HPUNCH: player->dialog = "Arina punch!"; break;
                case ST_HKICK: player->dialog = "Arina kick!"; break;
                case ST_SHOT: player->dialog = "Super Fractal Attack!"; break;
                case ST_HIT: player->dialog = "Aaarghhh!"; break;
        }
}

