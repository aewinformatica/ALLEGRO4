#include <allegro.h>
#include "sprite.h"
#include "power.h"
#include "fighter.h"
#include "juliette.h"



static int juliette_init (FIGHTER *self);
static int juliette_parse_event (FIGHTER *self, EVENT event);
static void juliette_action_to_sprite (FIGHTER *self, int action);
static void juliette_update_sprite (FIGHTER *self);
static void juliette_destroy (FIGHTER *self);



static ANIMATION juliette_anims [] =
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



static SPRITE juliette_sprite =
{
                0,               // flags
                NULL,            // image
                { 0, 0 },        // speed
                { 200, 200 },    // size
                { 300, 200 },    // pos
                { 0, 0, 0, 0 },  // bound
                JUL_S,           // default anim
                JUL_S,           // current anim
                0,               // current frame
                0,               // frame timer
                juliette_anims,  // anims
                NULL,            // data
};



FIGHTER juliette =
{
        "Juliette",                 // name
        "",                         // dialog
        MAX_ENERGY,                 // energy
        HUMAN,                      // type
        1,                          // dir
        ST_NONE,                    // state
        0,                          // strength
        "juliette.dat",             // datafile
        &juliette_sprite,           // sprite
        &default_keymap_2,          // keymap
        NULL,                       // enemy
        juliette_init,              // init
        juliette_parse_event,       // parse_event
        juliette_action_to_sprite,  // action_to_sprite
        juliette_update_sprite,     // update_sprite
        juliette_destroy,           // destroy
};



static int juliette_init (FIGHTER *player)
{
        juliette_sprite.pos.x = 300;
        juliette_sprite.pos.y = 200;
        juliette_sprite.data = load_datafile (player->datafile);
        if (juliette_sprite.data == NULL)
                return 0;
        juliette_sprite.image = (BITMAP *) juliette_sprite.data [juliette_sprite.default_anim].dat;
        return 1;
}



static void juliette_destroy (FIGHTER *player)
{
        if (juliette_sprite.data != NULL)
                unload_datafile (juliette_sprite.data);
}



static int juliette_state = 0;



static int juliette_parse_event (FIGHTER *player, EVENT event)
{
        switch (juliette_state)
        {
                case 0:
                        if (event.vdir == EV_DOWN)
                                juliette_state = 1;
                        break;
                case 1:
                        juliette_state = 0;
                        if (event.hdir == EV_FWD && event.hpunch)
                        {
                                return ACTION_SHOT;
                        }
                        break;
        }
        return event_to_action (event);
}



static void juliette_action_to_sprite (FIGHTER *player, int action)
{
        SPRITE *sprite = player->sprite;
        reset_sprite (sprite);
        switch (action)
        {
                case ACTION_STAND:
                        sprite->current_anim = JUL_S;
                        break;
                case ACTION_WALK_FWD:
                        sprite->current_anim = JUL_W;
                        sprite->speed.x = 5 * (player->dir ? -1 : 1);
                        break;
                case ACTION_WALK_BWD:
                        sprite->current_anim = JUL_W;
                        sprite->speed.x = 5 * (player->dir ? 1 : -1);
                        break;
                case ACTION_JUMP:
                        sprite->current_anim = JUL_J;
                        break;
                case ACTION_JUMP_FWD:
                        sprite->current_anim = JUL_J;
                        sprite->speed.x = 5 * (player->dir ? -1 : 1);
                        break;
                case ACTION_JUMP_BWD:
                        sprite->current_anim = JUL_J;
                        sprite->speed.x = 5 * (player->dir ? 1 : -1);
                        break;
                case ACTION_HPUNCH:
                case ACTION_LPUNCH:
                        sprite->current_anim = JUL_P;
                        break;
                case ACTION_HKICK:
                case ACTION_LKICK:
                        sprite->current_anim = JUL_K;
                        break;
                case ACTION_BLOCK:
                        sprite->current_anim = JUL_B;
                        break;
                case ACTION_CROUCH:
                        sprite->current_anim = JUL_C;
                        break;
                case ACTION_HIT:
                        player->energy -= player->enemy->strength;
                        if (player->energy < 0)
                                player->energy = 0;
                        sprite->current_anim = JUL_H;
                        sprite->speed.x = 5 * ( player->dir ? 1 : -1);
                        break;
                case ACTION_SHOT:
                        sprite->current_anim = JUL_P;
                        break;
                case ACTION_LAY:
                        sprite->current_anim = JUL_L;
                        break;
        }
}



static void juliette_update_sprite (FIGHTER *player)
{
        default_update_sprite (player);
        switch (player->state)
        {
                case ST_JUMP: player->dialog = "Whoop!"; break;
                case ST_LPUNCH: player->dialog = "Eat that!"; break;
                case ST_LKICK: player->dialog = "Loser"; break;
                case ST_HPUNCH: player->dialog = "Juliette punch!"; break;
                case ST_HKICK: player->dialog = "Juliette kick!"; break;
                case ST_SHOT: player->dialog = "Hyper Fractal Attack!"; break;
                case ST_HIT: player->dialog = "Awww!!"; break;
        }
}

