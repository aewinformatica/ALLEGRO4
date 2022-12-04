#include <allegro.h>
#include "keymap.h"
#include "sprite.h"
#include "fighter.h"



EVENT read_user_input (FIGHTER *player)
{
        EVENT event = { 0, 0, 0, 0, 0, 0 };
        if (player->keymap->device == DEV_KBD)
        {
                if (key [player->keymap->up]) event.vdir = EV_UP;
                if (key [player->keymap->down]) event.vdir = EV_DOWN;
                if (key [player->keymap->left]) event.hdir = (player->dir ? EV_FWD : EV_BWD);
                if (key [player->keymap->right]) event.hdir = (player->dir ? EV_BWD : EV_FWD);
                event.hpunch = key [player->keymap->hpunch];
                event.lpunch = key [player->keymap->lpunch];
                event.hkick = key [player->keymap->hkick];
                event.lkick = key [player->keymap->lkick];
                event.block = key [player->keymap->block];
        }
        else
        {
                /* write joystick code here */
        }
        return event;
}



int event_to_action (EVENT event)
{
        int action = ACTION_STAND;
        if (event.vdir == EV_UP)
        {
                action = ACTION_JUMP;
        }
        else if (event.vdir == EV_DOWN)
        {
                action = ACTION_CROUCH;
        }
        if (event.hdir == EV_FWD)
        {
                action = (action == ACTION_JUMP)? ACTION_JUMP_FWD : ACTION_WALK_FWD;
        }
        if (event.hdir == EV_BWD)
        {
                action = (action == ACTION_JUMP)? ACTION_JUMP_BWD : ACTION_WALK_BWD;
        }
        if (event.hpunch) action = ACTION_HPUNCH;
        if (event.lpunch) action = ACTION_LPUNCH;
        if (event.hkick) action = ACTION_HKICK;
        if (event.lkick) action = ACTION_LKICK;
        if (event.block) action = ACTION_BLOCK;
        return action;
}



int action_to_state (int action)
{
        switch (action)
        {
                case ACTION_STAND:
                        return ST_STAND;
                case ACTION_WALK_FWD:
                case ACTION_WALK_BWD:
                        return ST_WALK;
                case ACTION_JUMP:
                case ACTION_JUMP_FWD:
                case ACTION_JUMP_BWD:
                        return ST_JUMP;
                case ACTION_LPUNCH:
                        return ST_LPUNCH;
                case ACTION_HPUNCH:
                        return ST_HPUNCH;
                case ACTION_LKICK:
                        return ST_LKICK;
                case ACTION_HKICK:
                        return ST_HKICK;
                case ACTION_BLOCK:
                        return ST_BLOCK;
                case ACTION_CROUCH:
                        return ST_CROUCH;
                case ACTION_HIT:
                        return ST_HIT;
                case ACTION_SHOT:
                        return ST_SHOT;
        }
        return ST_STAND;
}



void update_state (FIGHTER *player)
{
        if (player->sprite->flags == SPRITE_DONE)
        {
                if (player->state == ST_LPUNCH || player->state == ST_HPUNCH
                    || player->state == ST_LKICK || player->state == ST_HKICK
                    || player->state == ST_JUMP || player->state == ST_CROUCH
                    || player->state == ST_BLOCK || player->state == ST_HIT
                    || player->state == ST_SHOT)
                        player->state = ST_NONE;
        }
}



void change_state (FIGHTER *player, int action)
{
        int new_state = action_to_state (action);
        if (player->state == new_state && player->sprite->flags != SPRITE_DONE)
        {
                return;
        }
        update_state (player);
        switch (player->state)
        {
                case ST_STAND:
                                reset_sprite (player->sprite);
                                player->action_to_sprite (player, action);
                                player->state = new_state;
                                break;
                case ST_WALK:
                                reset_sprite (player->sprite);
                                player->action_to_sprite (player, action);
                                player->state = new_state;
                                break;
                case ST_LPUNCH:
                case ST_HPUNCH:
                case ST_LKICK:
                case ST_HKICK:
                case ST_CROUCH:
                                if (new_state == ST_HIT)
                                {
                                        reset_sprite (player->sprite);
                                        player->action_to_sprite (player, action);
                                        player->state = new_state;
                                }
                                break;
                case ST_JUMP:
                case ST_BLOCK:
                case ST_SHOT:
                                break;
                case ST_HIT:
                                if (player->energy == 0)
                                {
                                        reset_sprite (player->sprite);
                                        player->action_to_sprite (player, ACTION_LAY);
                                        player->state = ST_LAY;
                                }
                                break;
        }
}



static void update_sprite (FIGHTER *player)
{
        player->update_sprite (player);
}



static void turn_around (FIGHTER *player)
{
        int half1 = (player->sprite->bound.x1 + player->sprite->bound.x2) >> 1;
        int half2 = (player->enemy->sprite->bound.x1 + player->enemy->sprite->bound.x2) >> 1;
        if (half1 > half2)
                player->dir = 1;
        else
                player->dir = 0;
}



void reset_fighter (FIGHTER *player)
{
        player->state = ST_NONE;
        player->energy = MAX_ENERGY;
}



void update_fighter (FIGHTER *player)
{
        EVENT event;
        int action;
        if (player->energy && player->enemy->energy)
        {
                if (player->type == HUMAN)
                {
                        event  = read_user_input (player);
                        action = player->parse_event (player, event);
                }
                else
                {
                        action = get_ai_action (player);
                }
        }
        else
        {
                action = ACTION_STAND;
        }
        change_state (player, action);
        update_sprite (player);
        turn_around (player);
}



void fire_action (FIGHTER *player, int action)
{
        change_state (player, action);
        update_sprite (player);
}



void default_update_sprite (FIGHTER *player)
{
        SPRITE *s = player->sprite;
        player->dialog = "";
        s->bound.x1 = player->dir ? s->pos.x + (s->size.x >> 1) : s->pos.x;
        s->bound.x2 = player->dir ? s->pos.x + s->size.x : s->pos.x + (s->size.x >> 1);
        s->bound.y1 = s->pos.y;
        s->bound.y2 = s->pos.y + s->size.y;
        move_sprite (player->sprite);
        switch (player->state)
        {
                case ST_JUMP:
                        switch (s->current_frame)
                        {
                                case 0: s->pos.y = 70; break;
                                case 1: s->pos.y = 50; break;
                                case 2: s->pos.y = 10; break;
                                case 3: s->pos.y = 50; break;
                                case 4: s->pos.y = 70; break;
                                case 5: s->pos.y = 200; break;
                        }
                        break;
                case ST_CROUCH:
                        s->bound.y1 = s->pos.y + (s->size.y >> 1);
                        break;
                case ST_LPUNCH:
                case ST_LKICK:
                        player->strength = WEAK;
                        if (s->current_frame == 3)
                        {
                                s->bound.x1 = s->pos.x;
                                s->bound.x2 = s->pos.x + s->size.x;
                                if (collision (player->sprite, player->enemy->sprite) && player->enemy->state != ST_BLOCK  && player->enemy->state != ST_JUMP)
                                {
                                        start_explosion ((player->dir ? player->enemy->sprite->bound.x2 : player->enemy->sprite->bound.x1),
                                                          player->enemy->sprite->bound.y1);
                                        if (player->enemy->state != ST_BLOCK)
                                        {
                                                fire_action (player->enemy, ACTION_HIT);
                                        }
                                }
                        }
                        break;
                case ST_HPUNCH:
                case ST_HKICK:
                        player->strength = STRONG;
                        if (s->current_frame == 3)
                        {
                                s->bound.x1 = s->pos.x;
                                s->bound.x2 = s->pos.x + s->size.x;
                                if (collision (player->sprite, player->enemy->sprite) && player->enemy->state != ST_BLOCK  && player->enemy->state != ST_JUMP)
                                {
                                        start_explosion ((player->dir ? player->enemy->sprite->bound.x2 : player->enemy->sprite->bound.x1),
                                                          player->enemy->sprite->bound.y1);
                                        if (player->enemy->state != ST_BLOCK)
                                        {
                                                fire_action (player->enemy, ACTION_HIT);
                                        }
                                }
                        }
                        break;
                case ST_SHOT:
                        player->strength = SPECIAL;
                        if (s->current_frame == 0)
                        activate_power ();
                        s->bound.x1 = s->pos.x;
                        s->bound.x2 = s->pos.x + s->size.x;
                        if (collision (player->sprite, player->enemy->sprite)
                            && player->enemy->state != ST_JUMP)
                        {
                                        start_explosion ((player->dir ? player->enemy->sprite->bound.x2 : player->enemy->sprite->bound.x1),
                                                          player->enemy->sprite->bound.y1);
                                        if (player->enemy->state != ST_SHOT && player->enemy->state != ST_BLOCK)
                                                fire_action (player->enemy, ACTION_HIT);
                        }
                        if (s->current_frame == 3)
                                deactivate_power ();
                        break;
                case ST_STAND:
                        if (!player->enemy->energy)
                                player->dialog = "You win";
                        break;
                case ST_LAY:
                        player->dialog = "You lose";
                        break;
        }
}


