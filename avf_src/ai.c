#include <allegro.h>
#include <stdlib.h>
#include "timers.h"
#include "fighter.h"



int get_ai_action (FIGHTER *player)
{
        int action;
        int val;
        srand (seconds_timer);
        if (player->enemy->state == ST_STAND)
        {
                if (abs (player->sprite->pos.x - player->enemy->sprite->pos.x) < (player->sprite->size.x >> 1))
                        action = (seconds_timer & 0x01) ? ACTION_HPUNCH : ACTION_LKICK;
                else
                        action = ACTION_WALK_FWD;
        }
        else if (player->enemy->state == ST_WALK)
        {
                if (abs (player->sprite->pos.x - player->enemy->sprite->pos.x) < (player->sprite->size.x >> 1))
                        action = (seconds_timer & 0x01) ? ACTION_BLOCK : ACTION_SHOT;
                else
                        action = (seconds_timer & 0x01) ? ACTION_LPUNCH : ACTION_HKICK;
        }
        else if (player->enemy->state == ST_JUMP)
        {
                if (abs (player->sprite->pos.x - player->enemy->sprite->pos.x) < (player->sprite->size.x >> 1))
                        action = (seconds_timer & 0x01) ? ACTION_BLOCK : ACTION_WALK_BWD;
                else
                        action = (seconds_timer & 0x01) ? ACTION_JUMP_BWD : ACTION_HPUNCH;
        }
        else if (player->enemy->state == ST_BLOCK)
        {
                if (abs (player->sprite->pos.x - player->enemy->sprite->pos.x) < (player->sprite->size.x >> 1))
                        action = (seconds_timer & 0x01) ? ACTION_HPUNCH : ACTION_LKICK;
                else
                        action = ACTION_JUMP_FWD;
        }
        else if (player->enemy->state == ST_SHOT)
        {
                action = seconds_timer & 0x01 ? ACTION_BLOCK : ACTION_SHOT;
        }
        else
        {
                if ((player->enemy->state == ST_HPUNCH || player->enemy->state == ST_LPUNCH
                    || player->enemy->state == ST_HKICK || player->enemy->state == ST_LKICK)
                    && (abs (player->sprite->pos.x - player->enemy->sprite->pos.x) < (player->sprite->size.x >> 1)))
                {
                        if (seconds_timer & 0x01)
                        {
                                return ACTION_SHOT;
                        }
                }
                val = rand () % 6;
                switch (val)
                {
                        case 0: action = ACTION_JUMP; break;
                        case 1: action = ACTION_JUMP_FWD; break;
                        case 2: action = ACTION_JUMP_BWD; break;
                        case 3: action = ACTION_WALK_FWD; break;
                        case 4: action = ACTION_WALK_BWD; break;
                        case 6: action = ACTION_SHOT; break;
                }
        }
        return action;
}

