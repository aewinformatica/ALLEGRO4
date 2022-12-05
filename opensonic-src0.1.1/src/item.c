/*
 * item.c - code for the items
 * Copyright (C) 2008-2009  Alexandre Martins < alemartf [.a.t.] gmail [.d.o.t.] com >
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "global.h"
#include "player.h"
#include "item.h"
#include "enemy.h"
#include "level.h"
#include "audio.h"
#include "quest.h"


/* private functions */
item_t *find_closest_item(item_t *me, item_list_t *list, int desired_type, double *distance);



/*
 * item_create()
 * Creates a new item
 */
item_t *item_create(int type)
{
    int i;
    item_t *item = mallocx(sizeof *item);
    item->type = type;
    item->state = IS_IDLE;
    item->obstacle = FALSE;
    item->bring_to_back = FALSE;
    item->actor = actor_create();
    for(i=0; i<ITEM_MAXVALUES; i++)
        item->value[i] = 0;

    switch(type) {
        case IT_RING:
            item->preserve = TRUE;
            item->actor->maxspeed = 220 + random(140);
            item->actor->jump_strength = (350 + random(50)) * 1.2;
            item->actor->input = input_create_computer();
            actor_change_animation(item->actor, sprite_get_animation("SD_RING", 0));
            break;

        case IT_LIFEBOX:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 0));
            break;

        case IT_RINGBOX:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 3));
            break;

        case IT_STARBOX:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 4));
            break;

        case IT_SPEEDBOX:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 5));
            break;

        case IT_GLASSESBOX:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 6));
            break;

        case IT_SHIELDBOX:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 7));
            break;

        case IT_EGGMANBOX:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 8));
            break;

        case IT_EMPTYBOX:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 9));
            break;

        case IT_CRUSHEDBOX:
            item->preserve = TRUE;
            item->obstacle = FALSE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ITEMBOX", 10));
            break;

        case IT_ICON:
            item->preserve = FALSE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ICON", 0));
            break;

        case IT_FALGLASSES:
            item->preserve = FALSE;
            actor_change_animation(item->actor, sprite_get_animation("SD_GLASSES", 4));
            item->actor->hot_spot.y /= 2;
            break;

        case IT_EXPLOSION:
            item->preserve = FALSE;
            actor_change_animation(item->actor, sprite_get_animation("SD_EXPLOSION", random(2)));
            break;

        case IT_FLYINGTEXT:
            item->preserve = FALSE;
            /*item->value[1] = SD_FONT0; <- TODO */
            actor_change_animation(item->actor, sprite_get_animation("FT_FONT0", 0));
            break;

        case IT_PIXEL:
            item->preserve = FALSE;
            item->value[0] = 0;
            actor_change_animation(item->actor, sprite_get_animation("SD_PIXEL", 0));
            break;

        case IT_ANIMAL:
            item->preserve = FALSE;
            item->value[0] = 0;
            item->value[1] = 0;
            item->actor->maxspeed = 45 + random(21);
            item->actor->input = input_create_computer();
            actor_change_animation(item->actor, sprite_get_animation("SD_ANIMAL", 0));
            break;

        case IT_LOOPRIGHT:
            item->preserve = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_LOOPRIGHT", 0));
            break;

        case IT_LOOPMIDDLE: /* loop top */
            item->preserve = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_LOOPMIDDLE", 0));
            break;

        case IT_LOOPLEFT:
            item->preserve = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_LOOPLEFT", 0));
            break;

        case IT_LOOPNONE:
            item->preserve = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_LOOPNONE", 0));
            break;

        case IT_LOOPFLOOR:
            item->preserve = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_LOOPFLOOR", 0));
            break;

        case IT_LOOPFLOORNONE:
            item->preserve = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_LOOPFLOORNONE", 0));
            break;

        case IT_LOOPFLOORTOP:
            item->preserve = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_LOOPFLOORTOP", 0));
            break;

        case IT_YELLOWSPRING:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_YELLOWSPRING", 0));
            break;

        case IT_REDSPRING:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_REDSPRING", 0));
            break;

        case IT_RREDSPRING:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_RREDSPRING", 0));
            break;

        case IT_LREDSPRING:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_LREDSPRING", 0));
            break;

           case IT_BLUERING:
            item->preserve = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_BLUERING", 0));
            break;

        case IT_SWITCH:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_SWITCH", 0));
            break;

        case IT_DOOR:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            item->obstacle = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_DOOR", 0));
            break;

        case IT_TELEPORTER:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_TELEPORTER", 0));
            break;

        case IT_BIGRING:
            item->preserve = TRUE;
            item->bring_to_back = FALSE;
            actor_change_animation(item->actor, sprite_get_animation("SD_BIGRING", 0));
            break;

        case IT_CHECKPOINT:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_CHECKPOINT", 0));
            break;

        case IT_GOAL:
            item->preserve = TRUE;
            item->bring_to_back = FALSE;
            actor_change_animation(item->actor, sprite_get_animation("SD_GOAL", 0));
            break;

        case IT_ENDSIGN:
            item->preserve = TRUE;
            item->bring_to_back = FALSE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ENDSIGN", 0));
            break;

        case IT_ENDLEVEL:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_ENDLEVEL", 0));
            break;

        case IT_BUMPER:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_BUMPER", 0));
            break;

        case IT_DANGER:
            item->preserve = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_DANGER", 0));
            break;

        case IT_SPIKES:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_SPIKES", 0));
            break;

        case IT_DNADOOR:
            item->preserve = TRUE;
            item->obstacle = TRUE;
            item->bring_to_back = FALSE;
            actor_change_animation(item->actor, sprite_get_animation("SD_DNADOOR", 0));
            break;

        case IT_DANGPOWER:
            item->preserve = FALSE;
            item->bring_to_back = TRUE;
            actor_change_animation(item->actor, sprite_get_animation("SD_DANGPOWER", 0));
            break;

        case IT_FIREBALL:
            item->preserve = FALSE;
            item->bring_to_back = FALSE;
            actor_change_animation(item->actor, sprite_get_animation("SD_FIREBALL", 0));
    }

    return item;
}



/*
 * item_destroy()
 * Destroys an item
 */
void item_destroy(item_t *item)
{
    actor_destroy(item->actor);
    free(item);
}




/*
 * item_render()
 * Renders an item
 */
void item_render(item_t *item, v2d_t camera_position)
{
    actor_render(item->actor, camera_position);
}



/*
 * item_update()
 * Runs every cycle of the game to update an item
 */
void item_update(item_t *item, player_t *team[3], brick_list_t *brick_list, item_list_t *item_list)
{
    actor_t *act = item->actor;
    double dt = game_delta();

    switch(item->type) {

        /*
         * Ring :: alterable values
         * value 0: life time (useful only if ring->state == IS_ACTIVE)
         */
        case IT_RING:
        {
            int i;

            for(i=0; i<3; i++) {
                if((item->state == IS_IDLE || (item->state == IS_ACTIVE && !team[i]->getting_hit)) && !team[i]->dieing && actor_collision(act, team[i]->actor)) { /* the player got this ring */
                    actor_change_animation(act, sprite_get_animation("SD_RING", 1));
                    player_set_rings(player_get_rings()+1);
                    item->state = IS_DIEING;
                    sound_play( sound_load("samples/ring.wav") );
                    break;
                }
            }

            if(item->state == IS_DIEING) {
                if(actor_animation_finished(act))
                    item->state = IS_DEAD;
               }
            else if(item->state == IS_ACTIVE) { /* this ring jumps */
                brick_t *left = NULL, *right = NULL, *down = NULL;
                actor_corners(act, 2, -2, brick_list, NULL, NULL, &right, NULL, &down, NULL, &left, NULL);
                input_simulate_button_down(act->input, IB_FIRE1);

                item->preserve = FALSE;
                item->value[0] += game_delta();
                if(item->value[0] > 5) {
                    int val = 240 + random(20);
                    act->visible = ((int)(game_timer() % val) < val/2);
                    if(item->value[0] > 8)
                        item->state = IS_DEAD;
                }
                if(right && act->speed.x > 0) act->speed.x = -fabs(act->speed.x);
                if(left && act->speed.x < 0) act->speed.x = fabs(act->speed.x);
                if(down && act->speed.y > 0) act->jump_strength *= 0.95;
                actor_move(act, actor_platform_movement(act, brick_list, level_gravity()));
            }

            break;
        }



        /*
         * Life box
         */
        case IT_LIFEBOX:
        {
            int i;
            item_t *icon;

            act->position.y -= 5; /* fake brick fix */
            for(i=0; i<3; i++) {
                if(team[i]->actor->is_jumping && team[i]->actor->speed.y < -10) continue;
                if(item->state == IS_IDLE && actor_collision(act, team[i]->actor) && player_attacking(team[i])) {
                    player_set_lives(player_get_lives()+1);
                    icon = level_create_item(IT_ICON, act->position);
                    actor_change_animation(icon->actor, sprite_get_animation("SD_ICON", level_player_id()));
                    level_create_item(IT_EXPLOSION, v2d_add(act->position, v2d_new(0,-15)));
                    level_create_item(IT_CRUSHEDBOX, v2d_add(act->position, v2d_new(0,5)));
                    level_add_to_score(100);
                    item->state = IS_DEAD;
                    if(team[i]->actor->is_jumping)
                        player_bounce(team[i]);
                    sound_play( sound_load("samples/destroypop.wav") );
                    level_override_music( sound_load("samples/1up.wav") );
                }
            }
            act->position.y += 5;

            if(item->state == IS_IDLE)
                actor_change_animation(act, sprite_get_animation("SD_ITEMBOX", level_player_id()));

            break;
        }


        /*
         * Ring box
         */
        case IT_RINGBOX:
        {
            int i;
            item_t *icon;

            act->position.y -= 5;
            for(i=0; i<3; i++) {
                if(team[i]->actor->is_jumping && team[i]->actor->speed.y < -10) continue;
                if(item->state == IS_IDLE && actor_collision(act, team[i]->actor) && player_attacking(team[i])) {
                    player_set_rings(player_get_rings()+10);
                    icon = level_create_item(IT_ICON, act->position);
                    actor_change_animation(icon->actor, sprite_get_animation("SD_ICON", 3));
                    level_create_item(IT_EXPLOSION, v2d_add(act->position, v2d_new(0,-15)));
                    level_create_item(IT_CRUSHEDBOX, v2d_add(act->position, v2d_new(0,5)));
                    level_add_to_score(100);
                    item->state = IS_DEAD;
                    if(team[i]->actor->is_jumping)
                        player_bounce(team[i]);
                    sound_play( sound_load("samples/destroypop.wav") );
                    sound_play( sound_load("samples/ring.wav") );
                }
            }
            act->position.y += 5;

            break;
        }



        /*
         * Star box
         */
        case IT_STARBOX:
        {
            int i;
            item_t *icon;

            act->position.y -= 5;
            for(i=0; i<3; i++) {
                if(team[i]->actor->is_jumping && team[i]->actor->speed.y < -10) continue;
                if(item->state == IS_IDLE && actor_collision(act, team[i]->actor) && player_attacking(team[i])) {
                    team[i]->invencible = TRUE;
                    team[i]->invtimer = 0;
                    icon = level_create_item(IT_ICON, act->position);
                    actor_change_animation(icon->actor, sprite_get_animation("SD_ICON", 4));
                    level_create_item(IT_EXPLOSION, v2d_add(act->position, v2d_new(0,-15)));
                    level_create_item(IT_CRUSHEDBOX, v2d_add(act->position, v2d_new(0,5)));
                    level_add_to_score(100);
                    item->state = IS_DEAD;
                    if(team[i]->actor->is_jumping)
                        player_bounce(team[i]);
                    sound_play( sound_load("samples/destroypop.wav") );
                    music_play( music_load("musics/invencible.it"), 0 );
                }
            }
            act->position.y += 5;

            break;
        }



        /*
         * Speed shoes box
         */
        case IT_SPEEDBOX:
        {
            int i;
            item_t *icon;

            act->position.y -= 5;
            for(i=0; i<3; i++) {
                if(team[i]->actor->is_jumping && team[i]->actor->speed.y < -10) continue;
                if(item->state == IS_IDLE && actor_collision(act, team[i]->actor) && player_attacking(team[i])) {
                    team[i]->got_speedshoes = TRUE;
                    team[i]->speedshoes_timer = 0;
                    icon = level_create_item(IT_ICON, act->position);
                    actor_change_animation(icon->actor, sprite_get_animation("SD_ICON", 5));
                    level_create_item(IT_EXPLOSION, v2d_add(act->position, v2d_new(0,-15)));
                    level_create_item(IT_CRUSHEDBOX, v2d_add(act->position, v2d_new(0,5)));
                    level_add_to_score(100);
                    item->state = IS_DEAD;
                    if(team[i]->actor->is_jumping)
                        player_bounce(team[i]);
                    sound_play( sound_load("samples/destroypop.wav") );
                    music_play( music_load("musics/speed.it"), 0 );
                }
            }
            act->position.y += 5;

            break;
        }



        /*
         * Glasses box
         */
        case IT_GLASSESBOX:
        {
            int i;
            item_t *icon;

            act->position.y -= 5;
            for(i=0; i<3; i++) {
                if(team[i]->actor->is_jumping && team[i]->actor->speed.y < -10) continue;
                if(item->state == IS_IDLE && actor_collision(act, team[i]->actor) && player_attacking(team[i])) {
                    team[i]->got_glasses = TRUE;
                    icon = level_create_item(IT_ICON, act->position);
                    actor_change_animation(icon->actor, sprite_get_animation("SD_ICON", 6));
                    level_create_item(IT_EXPLOSION, v2d_add(act->position, v2d_new(0,-15)));
                    level_create_item(IT_CRUSHEDBOX, v2d_add(act->position, v2d_new(0,5)));
                    level_add_to_score(100);
                    item->state = IS_DEAD;
                    if(team[i]->actor->is_jumping)
                        player_bounce(team[i]);
                    sound_play( sound_load("samples/destroypop.wav") );
                }
            }
            act->position.y += 5;

            break;
        }

        /*
         * Shield box
         */
        case IT_SHIELDBOX:
        {
            int i;
            item_t *icon;

            act->position.y -= 5;
            for(i=0; i<3; i++) {
                if(team[i]->actor->is_jumping && team[i]->actor->speed.y < -10) continue;
                if(item->state == IS_IDLE && actor_collision(act, team[i]->actor) && player_attacking(team[i])) {
                    team[i]->shield_type = SH_SHIELD;
                    icon = level_create_item(IT_ICON, act->position);
                    actor_change_animation(icon->actor, sprite_get_animation("SD_ICON", 7));
                    level_create_item(IT_EXPLOSION, v2d_add(act->position, v2d_new(0,-15)));
                    level_create_item(IT_CRUSHEDBOX, v2d_add(act->position, v2d_new(0,5)));
                    level_add_to_score(100);
                    item->state = IS_DEAD;
                    if(team[i]->actor->is_jumping)
                        player_bounce(team[i]);
                    sound_play( sound_load("samples/destroypop.wav") );
                    sound_play( sound_load("samples/shield.wav") );
                }
            }
            act->position.y += 5;

            break;
        }


        /*
         * Eggman box
         */
        case IT_EGGMANBOX:
        {
            int i;
            item_t *icon;

            act->position.y -= 5;
            for(i=0; i<3; i++) {
                if(team[i]->actor->is_jumping && team[i]->actor->speed.y < -10) continue;
                if(item->state == IS_IDLE && actor_collision(act, team[i]->actor) && player_attacking(team[i])) {
                    player_hit(team[i]);
                    icon = level_create_item(IT_ICON, act->position);
                    actor_change_animation(icon->actor, sprite_get_animation("SD_ICON", 8));
                    level_create_item(IT_EXPLOSION, v2d_add(act->position, v2d_new(0,-15)));
                    level_create_item(IT_CRUSHEDBOX, v2d_add(act->position, v2d_new(0,5)));
                    item->state = IS_DEAD;
                    if(team[i]->actor->is_jumping)
                        player_bounce(team[i]);
                    sound_play( sound_load("samples/destroypop.wav") );
                }
            }
            act->position.y += 5;

            break;
        }

        /*
         * Icon :: alterable values
         * value 0: elapsed time since creation
         */
        case IT_ICON:
        {
            item->value[0] += dt;
            if(item->value[0] < 1)
                act->position.y -= 40*dt;
            else if(item->value[0] > 2.5) {
                int i, j;
                int x = (int)(act->position.x-act->hot_spot.x);
                int y = (int)(act->position.y-act->hot_spot.y);
                image_t *img = actor_image(act), *particle;

                /* particle party! :) */
                for(i=0; i<img->h; i++) {
                    for(j=0; j<img->w; j++) {
                        particle = image_create(1,1);
                        image_clear(particle, image_getpixel(img, j, i));
                        level_create_particle(particle, v2d_new(x+j, y+i), v2d_new((j-img->w/2) + (random(img->w)-img->w/2), i-random(img->h/2)), FALSE);
                    }
                }

                item->state = IS_DEAD;
            }

            break;
        }


        /*
         * Falling glasses
         */
        case IT_FALGLASSES:
        {
            v2d_t ds = actor_particle_movement(act, level_gravity());
            double s = fabs(act->speed.x)>EPSILON ? fabs(act->speed.x)/act->speed.x : -1;
            act->angle += s*(6*PI*dt);
            act->position = v2d_add(act->position,ds);

            break;
        }


        /*
         * Explosion
         */
        case IT_EXPLOSION:
        {
            if(actor_animation_finished(act))
                item->state = IS_DEAD;

            break;
        }


        /*
         * Flying text (actually, it's a flying single character)
         * value 0: elapsed time
         * value 1: font sprite (SD_FONT0 for example) <-- TODO?
         * value 2: character code (frame id, 0..38 for example)
         */
        case IT_FLYINGTEXT:
        {
            /*int s = (int)item->value[1];*/
            int f = (int)item->value[2];

            item->value[0] += dt;
            if(item->value[0] < 0.5)
                act->position.y -= 100*dt;
            else if(item->value[0] > 2)
                item->state = IS_DEAD;

            actor_change_animation(act, sprite_get_animation("FT_FONT0", 0));
            actor_change_animation_frame(act, f);
            break;
        }


        /*
         * Pixel particle
         * value 0: color scheme (0..n - see items.png)
         */
        case IT_PIXEL:
        {
            int s = (int)item->value[0];
            brick_t *brk;

            actor_move(act, actor_particle_movement(act, level_gravity()));
            actor_corners(act, 1, 0, brick_list, NULL, NULL, NULL, NULL, &brk, NULL, NULL, NULL);
            if(brk && act->speed.y > 0)
                item->state = IS_DEAD;   

            actor_change_animation(act, sprite_get_animation("SD_PIXEL", s));
            break;
        }


        /*
         * Little animal
         * value 0: animal ID (0..n)
         * value 1: 0 if stopped, 1 if running
         */
        case IT_ANIMAL:
        {
            int running = (int)item->value[1];
            int anim_id = 2*(int)item->value[0] + (running?1:0);
            brick_t *up, *down, *left, *right;

            input_simulate_button_down(act->input, IB_FIRE1);
            act->jump_strength = (200 + random(50)) * 1.3;
            if(act->speed.x > EPSILON) {
                act->speed.x = act->maxspeed;
                act->mirror = IF_NONE;
            }
            else if(act->speed.x < -EPSILON) {
                act->speed.x = -act->maxspeed;
                act->mirror = IF_HFLIP;
            }

            actor_change_animation(act, sprite_get_animation("SD_ANIMAL", anim_id));
            actor_corners(act, 2, -2, brick_list, &up, NULL, &right, NULL, &down, NULL, &left, NULL);
            if(down && !running) {
                item->value[1] = 1;
                act->speed.x = (random(2)?-1:1) * act->maxspeed;
            }
            if(left && !up) act->speed.x = act->maxspeed;
            if(right && !up) act->speed.x = -act->maxspeed;
            if(!running && ((down && up) || (left && right))) item->state = IS_DEAD; /* i'm stuck! */
            actor_move(act, actor_platform_movement(act, brick_list, level_gravity()));

            break;
        }


        /*
         * Loop Right
         */
        case IT_LOOPRIGHT:
        {
            int i;

            item->actor->visible = level_editmode();
            for(i=0; i<3; i++) {
                if(actor_collision(act, team[i]->actor)) {
                    /*team[i]->disable_wall = PLAYER_WALL_LEFT;*/
                    team[i]->disable_wall |= PLAYER_WALL_LEFT;
                    team[i]->entering_loop = TRUE;
                    team[i]->bring_to_back = FALSE;
                }
            }
            break;
        }


        /*
         * Loop Middle (x-axis)
         */
        case IT_LOOPMIDDLE:
        {
            int i, b;

            item->actor->visible = level_editmode();
            for(i=0; i<3; i++) {
                if(actor_collision(act, team[i]->actor)) {
                    b = team[i]->actor->speed.x > 0;
                    /*team[i]->disable_wall = b ? PLAYER_WALL_RIGHT : PLAYER_WALL_LEFT;*/
                    team[i]->disable_wall &= ~(PLAYER_WALL_LEFT | PLAYER_WALL_RIGHT);
                    team[i]->disable_wall |= b ? PLAYER_WALL_RIGHT : PLAYER_WALL_LEFT;
                    team[i]->bring_to_back = b;
                }
            }
            break;
        }


        /*
         * Loop Left
         */
        case IT_LOOPLEFT:
        {
            int i;

            item->actor->visible = level_editmode();
            for(i=0; i<3; i++) {
                if(actor_collision(act, team[i]->actor)) {
                    /*team[i]->disable_wall = PLAYER_WALL_RIGHT;*/
                    team[i]->disable_wall |= PLAYER_WALL_RIGHT;
                    team[i]->entering_loop = TRUE;
                    team[i]->bring_to_back = TRUE;
                }
            }
            break;
        }


        /*
         * Loop None - deactivate loops (x-axis)
         */
        case IT_LOOPNONE:
        {
            int i;

            item->actor->visible = level_editmode();
            for(i=0; i<3; i++) {
                if(!team[i]->entering_loop && actor_collision(act, team[i]->actor)) {
                    team[i]->disable_wall = PLAYER_WALL_NONE;
                    /*team[i]->disable_wall &= ~(PLAYER_WALL_LEFT | PLAYER_WALL_RIGHT);*/
                    team[i]->bring_to_back = FALSE;
                }
            }
            break;
        }

        /*
         * Loop Floor (y-axis)
         */
        case IT_LOOPFLOOR:
        {
            int i;

            item->actor->visible = level_editmode();
            for(i=0; i<3; i++) {
                if(!team[i]->at_loopfloortop && actor_collision(act, team[i]->actor)) {
                    team[i]->disable_wall |= PLAYER_WALL_BOTTOM;
                    team[i]->entering_loop = TRUE;
                    team[i]->bring_to_back = TRUE;
                }
            }
            break;
        }

        /*
         * Loop Floor None - deactivate loops (y-axis)
         */
        case IT_LOOPFLOORNONE:
        {
            int i;

            item->actor->visible = level_editmode();
            for(i=0; i<3; i++) {
                if(!team[i]->at_loopfloortop && !team[i]->entering_loop && actor_collision(act, team[i]->actor)) {
                    team[i]->disable_wall &= ~PLAYER_WALL_BOTTOM;
                    team[i]->bring_to_back = FALSE;
                }
            }
            break;
        }

        /*
         * Loop Floor Top - activate left and right walls (x-axis)
         */
        case IT_LOOPFLOORTOP:
        {
            int i, b;

            item->actor->visible = level_editmode();
            for(i=0; i<3; i++) {
                team[i]->at_loopfloortop = actor_collision(act, team[i]->actor);
                if(team[i]->at_loopfloortop) {
                    if(team[i]->disable_wall & PLAYER_WALL_BOTTOM) {
                        /* behave like IT_LOOPMIDDLE (loop top) */
                        b = team[i]->actor->speed.x > 0;
                        team[i]->disable_wall &= ~(PLAYER_WALL_LEFT | PLAYER_WALL_RIGHT);
                        team[i]->disable_wall |= b ? PLAYER_WALL_RIGHT : PLAYER_WALL_LEFT;
                        team[i]->bring_to_back = TRUE;
                    }
                    else {
                        /* lock the left & right walls (only the floor will be disabled) */
                        team[i]->disable_wall &= ~(PLAYER_WALL_LEFT | PLAYER_WALL_RIGHT);
                        team[i]->bring_to_back = TRUE;
                    }
                }
            }
            break;
        }

        /*
         * Loop Ceil - TODO?
         */

        /*
         * Loop Ceil None - TODO?
         */

        /*
         * Yellow spring
         */
        case IT_YELLOWSPRING:
        {
            int i;
            animation_t *idle = sprite_get_animation("SD_YELLOWSPRING", 0);

            /* spring animation */
            if(item->actor->animation == idle)
                item->actor->hot_spot = v2d_new(16, 16);
            else {
                if(actor_animation_finished(item->actor))
                    actor_change_animation(item->actor, idle);
                else
                    item->actor->hot_spot = v2d_new(16, 32);
            }

            /* and the player jumps... */
            for(i=0; i<3; i++) {
                if((int)team[i]->actor->speed.y > 0 && !team[i]->dieing && actor_collision(act, team[i]->actor)) {
                    team[i]->actor->speed.y = -500;
                    team[i]->spring = TRUE;
                    team[i]->flying = team[i]->climbing = team[i]->landing = team[i]->getting_hit = FALSE;
                    actor_change_animation(item->actor, sprite_get_animation("SD_YELLOWSPRING", 1));
                    sound_play( sound_load("samples/spring.wav") );
                }
            }

            break;           
        }


        /*
         * Red spring
         */
        case IT_REDSPRING:
        {
            int i;
            animation_t *idle = sprite_get_animation("SD_REDSPRING", 0);

            /* spring animation */
            if(item->actor->animation == idle)
                item->actor->hot_spot = v2d_new(16, 16);
            else {
                if(actor_animation_finished(item->actor))
                    actor_change_animation(item->actor, idle);
                else
                    item->actor->hot_spot = v2d_new(16, 32);
            }

            /* and the player jumps... */
            for(i=0; i<3; i++) {
                if((int)team[i]->actor->speed.y > 0 && !team[i]->dieing && actor_collision(act, team[i]->actor)) {
                    team[i]->actor->speed.y = -750;
                    team[i]->spring = TRUE;
                    team[i]->flying = team[i]->climbing = team[i]->landing = team[i]->getting_hit = FALSE;
                    actor_change_animation(item->actor, sprite_get_animation("SD_REDSPRING", 1));
                    sound_play( sound_load("samples/spring.wav") );
                }
            }

            break;           
        }

        /*
         * Right red spring
         */
        case IT_RREDSPRING:
        {
            int i;
            animation_t *idle = sprite_get_animation("SD_RREDSPRING", 0);

            /* spring animation */
            if(item->actor->animation == idle)
                item->actor->hot_spot = v2d_new(16, 16);
            else {
                if(actor_animation_finished(item->actor))
                    actor_change_animation(item->actor, idle);
                else
                    item->actor->hot_spot = v2d_new(0, 16);
            }

            /* and the player runs... */
            for(i=0; i<3; i++) {
                if(!team[i]->dieing && actor_collision(act, team[i]->actor)) {
                    team[i]->actor->speed.x = 1200;
                    input_simulate_button_down(team[i]->actor->input, IB_RIGHT);
                    team[i]->flying = team[i]->climbing = team[i]->landing = team[i]->getting_hit = FALSE;
                    actor_change_animation(item->actor, sprite_get_animation("SD_RREDSPRING", 1));
                    sound_play( sound_load("samples/spring.wav") );
                }
            }

            break;
        }

        /*
         * Left red spring
         */
        case IT_LREDSPRING:
        {
            int i;
            animation_t *idle = sprite_get_animation("SD_LREDSPRING", 0);

            /* spring animation */
            if(item->actor->animation == idle)
                item->actor->hot_spot = v2d_new(16, 16);
            else {
                if(actor_animation_finished(item->actor))
                    actor_change_animation(item->actor, idle);
                else
                    item->actor->hot_spot = v2d_new(32, 16);
            }

            /* and the player runs... */
            for(i=0; i<3; i++) {
                if(!team[i]->dieing && actor_collision(act, team[i]->actor)) {
                    team[i]->actor->speed.x = -1200;
                    input_simulate_button_down(team[i]->actor->input, IB_LEFT);
                    team[i]->flying = team[i]->climbing = team[i]->landing = team[i]->getting_hit = FALSE;
                    actor_change_animation(item->actor, sprite_get_animation("SD_LREDSPRING", 1));
                    sound_play( sound_load("samples/spring.wav") );
                }
            }

            break;           
        }

        /*
         * Blue ring
         */
        case IT_BLUERING:
        {
            player_t *p = level_player();

            act->visible = p->got_glasses || level_editmode();
            if(item->state == IS_IDLE) {
                if(p->got_glasses && !p->dieing && actor_collision(act, p->actor)) {
                    actor_change_animation(act, sprite_get_animation("SD_BLUERING", 1));
                    player_set_rings(player_get_rings()+5);
                    item->state = IS_DIEING;
                    sound_play( sound_load("samples/ring.wav") );
                }
            }
            else if(item->state == IS_DIEING) {
                if(actor_animation_finished(act))
                    item->state = IS_DEAD;
               }

            break;
        }



        /*
         * Switch
         * This object may be linked to a teleporter OR to a door (but not both)
         *
         * teleporter switch
         * value 0: button has been pressed once? 0 (false) or 1 (true)
         */
        case IT_SWITCH:
        {
            int i;
            double d1, d2, a[4], b[4];
            item_t *door = find_closest_item(item, item_list, IT_DOOR, &d1);
            item_t *teleporter = find_closest_item(item, item_list, IT_TELEPORTER, &d2);
            if(door && d1<d2) teleporter = NULL;
            if(teleporter && d2<d1) door = NULL;

            a[0] = act->position.x - act->hot_spot.x;
            a[1] = act->position.y - act->hot_spot.y;
            a[2] = a[0]+actor_image(act)->w;
            a[3] = a[1]+actor_image(act)->h;

            /* Door switch? */
            if(door) {
                int open = FALSE;

                /* someone has pressed this switch... */
                for(i=0; i<3; i++) {

                    b[0] = team[i]->actor->position.x - team[i]->actor->hot_spot.x + actor_image(team[i]->actor)->w*0.3;
                    b[1] = team[i]->actor->position.y - team[i]->actor->hot_spot.y + actor_image(team[i]->actor)->h/2;
                    b[2] = b[0] + actor_image(team[i]->actor)->w*0.4;
                    b[3] = b[1] + actor_image(team[i]->actor)->h/2;

                    /* player 'i' has activated it. */
                    if(!team[i]->dieing && !team[i]->climbing && !team[i]->flying && bounding_box(a,b)) {

                        /* open the door! */
                        open = TRUE;
                        if(act->animation == sprite_get_animation("SD_SWITCH", 0)) {
                            sound_play( sound_load("samples/switch.wav") );
                            sound_play( sound_load("samples/door1.wav") );
                        }
                        actor_change_animation(act, sprite_get_animation("SD_SWITCH", 1));
                        door->state = IS_ACTIVE;
                        break;

                    }
                }

                /* nobody is standing on this switch! */
                if(!open) { /* the door is opened and must be closed */

                    /* close the door! */
                    if(act->animation == sprite_get_animation("SD_SWITCH", 1))
                        sound_play( sound_load("samples/door2.wav") );
                    actor_change_animation(act, sprite_get_animation("SD_SWITCH", 0));
                    door->state = IS_IDLE;

                }
            }



            /* Teleporter switch? */
            if(teleporter) {
                int who = -1;
                int activate = (teleporter->state == IS_ACTIVE);

                /* someone has pressed this switch... */
                for(i=0; i<3; i++) {

                    b[0] = team[i]->actor->position.x - team[i]->actor->hot_spot.x + actor_image(team[i]->actor)->w*0.3;
                    b[1] = team[i]->actor->position.y - team[i]->actor->hot_spot.y + actor_image(team[i]->actor)->h/2;
                    b[2] = b[0] + actor_image(team[i]->actor)->w*0.4;
                    b[3] = b[1] + actor_image(team[i]->actor)->h/2;

                    if(!team[i]->dieing && !team[i]->climbing && !team[i]->flying && bounding_box(a,b)) {

                        /* activate the teleporter! */
                        activate = TRUE;
                        who = i; /* who activated this switch? */
                        input_ignore(team[who]->actor->input);
                        if(act->animation == sprite_get_animation("SD_SWITCH", 0))
                            sound_play( sound_load("samples/switch.wav") );
                        actor_change_animation(act, sprite_get_animation("SD_SWITCH", 1));
                        break;

                    }
                }
                if(who == -1) /* nobody really cares about this switch. */
                    actor_change_animation(act, sprite_get_animation("SD_SWITCH", 0));


                /* should we activate the teleporter? */
                if(activate && (int)item->value[0] == 0) {
                    /* are the other players far from the teleporter? */
                    int far = FALSE;
                    v2d_t diff;
                    for(i=0; i<3 && !far; i++) {
                        if(i == who) continue;
                        diff = v2d_subtract(team[i]->actor->position, teleporter->actor->position);
                        if(v2d_magnitude(diff) >= 300)
                            far = TRUE;
                    }

                    /* okay, let's teleport'em */
                    if(far && teleporter->state == IS_IDLE) {
                        teleporter->value[0] = who;
                        teleporter->value[1] = 0;
                        teleporter->state = IS_ACTIVE;
                        level_set_camera_focus(teleporter->actor);
                        sound_play( sound_load("samples/teleporter.wav") );
                    }
                    
                    /* disable this switch */
                    item->value[0] = 1;
                }
            }

            break;
        }

        /*
         * Door
         * state = IS_IDLE (closed) ; IS_ACTIVE (open)
         */
        case IT_DOOR:
        {
            int speed = 2000;

            if(item->state == IS_IDLE) /* closed */
                act->position.y = min(act->position.y + speed*dt, act->spawn_point.y);
            else if(item->state == IS_ACTIVE) /* opened */
                act->position.y = max(act->position.y - speed*dt, act->spawn_point.y - actor_image(act)->h*0.8);
            break;
        }

        /*
         * Teleporter
         * value 0: id of the player who activated this teleporter
         * value 1: teleporter timer
         */
        case IT_TELEPORTER:
        {
            int who = (int)item->value[0];
            actor_change_animation(act, sprite_get_animation("SD_TELEPORTER", (item->state==IS_ACTIVE)?1:0));

            if(item->state == IS_ACTIVE) {
                item->value[1] += dt;
                if(item->value[1] >= 3.0) {
                    int i, k;
                    item->value[1] = 0;
                    item->state = IS_IDLE;

                    input_restore(team[who]->actor->input);
                    level_change_player(who);

                    for(i=k=0; i<3; i++) {
                        if(i != who) {
                            team[i]->actor->position = v2d_add(act->position, v2d_new(-20 + 40*(k++), -30));
                            team[i]->actor->speed = v2d_new(0,0);
                            team[i]->actor->is_jumping = team[i]->flying = team[i]->climbing = team[i]->getting_hit = team[i]->spring = FALSE;
                            team[i]->actor->angle = 0;
                            team[i]->disable_wall = PLAYER_WALL_NONE;
                            team[i]->entering_loop = FALSE;
                            team[i]->at_loopfloortop = FALSE;
                            team[i]->bring_to_back = FALSE;
                        }
                    }
                }
            }

            break;
        }

        /*
         * Big Ring
         * TODO: bonus stage?
         */
        case IT_BIGRING:
        {
            int i;
            for(i=0; i<3; i++) {
                if(!team[i]->dieing && actor_collision(team[i]->actor, act)) {
                    item->state = IS_DEAD;
                    player_set_rings(player_get_rings() + 50);
                    level_add_to_secret_bonus(5000);
                    sound_play( sound_load("samples/bigring.wav") );
                    level_call_dialogbox("BONUS", "Bonus stages not yet implemented ;)");
                    quest_setvalue(QUESTVALUE_BIGRINGS, quest_getvalue(QUESTVALUE_BIGRINGS)+1);
                    break;
                }
            }
            break;
        }

        /*
         * Checkpoint
         */
        case IT_CHECKPOINT:
        {
            int i;

            /* not active */
            if(item->state == IS_IDLE) {
                for(i=0; i<3; i++) {
                    if(!team[i]->dieing && actor_collision(team[i]->actor, act)) {
                        sound_play( sound_load("samples/checkpoint.wav") );
                        level_set_spawn_point(act->position);
                        actor_change_animation(act, sprite_get_animation("SD_CHECKPOINT", 1));
                        item->state = IS_ACTIVE;
                        break;
                    }
                }
            }

            /* active */
            else if(item->state == IS_ACTIVE) {
                if(actor_animation_finished(act))
                    actor_change_animation(act, sprite_get_animation("SD_CHECKPOINT", 2));
            }

            break;
        }

        /*
         * Goal sign
         */
        case IT_GOAL:
        {
            /* where is the IT_ENDSIGN? */
            item_t *endsign = find_closest_item(item, item_list, IT_ENDSIGN, NULL);
            int anim = endsign ? ((endsign->actor->position.x > act->position.x) ? 0 : 1) : 0;
            actor_change_animation(act, sprite_get_animation("SD_GOAL", anim));
            break;
        }

        /*
         * End sign
         * value 0: type of the player who touched this sign
         */
        case IT_ENDSIGN:
        {
            int i, anim_id;

            /* not active */
            if(item->state == IS_IDLE) {
                for(i=0; i<3; i++) {
                    if(!team[i]->dieing && actor_collision(team[i]->actor, act)) {
                        sound_play( sound_load("samples/endsign.wav") );
                        actor_change_animation(act, sprite_get_animation("SD_ENDSIGN", 1));
                        item->value[0] = team[i]->type;
                        item->state = IS_ACTIVE;
                        level_clear(item->actor);
                        level_kill_all_baddies();
                        break;
                    }
                }
            }

            /* active */
            else if(item->state == IS_ACTIVE) {
                if(actor_animation_finished(act)) {
                    switch((int)item->value[0]) {
                        case PL_SONIC: anim_id = 2; break;
                        case PL_TAILS: anim_id = 3; break;
                        case PL_KNUCKLES: anim_id = 4; break;
                        default: anim_id = 2; break;
                    }
                    actor_change_animation(act, sprite_get_animation("SD_ENDSIGN", anim_id));
                }
            }
            break;
        }

        /*
         * End of level (this object shows just after the boss)
         * value 0: hit count
         * value 1: explosion start time
         * value 2: explosion "every" timer
         * value 3: controles the end of the explosion effect
         */
        case IT_ENDLEVEL:
        {
            double a[4], b[4], t = game_timer()*0.001;
            int i, maxhits = 3;
            actor_t *pl;

            /* not broken */
            if(item->state == IS_IDLE) {
                for(i=0; i<3; i++) {

                    /* bounding box */
                    pl = team[i]->actor;
                    a[0] = pl->position.x - pl->hot_spot.x;
                    a[1] = pl->position.y - pl->hot_spot.y;
                    a[2] = a[0] + actor_image(pl)->w;
                    a[3] = a[1] + actor_image(pl)->h;
                    b[0] = act->position.x - act->hot_spot.x + 5;
                    b[1] = act->position.y - act->hot_spot.y;
                    b[2] = b[0] + actor_image(act)->w - 10;
                    b[3] = b[1] + actor_image(act)->h/2;

                    /* the user is attacking this object */
                    if(player_attacking(team[i]) && bounding_box(a,b) && act->animation == sprite_get_animation("SD_ENDLEVEL", 0)) {
                        actor_change_animation(act, sprite_get_animation("SD_ENDLEVEL", 1));
                        sound_play( sound_load("samples/bosshit.wav") );
                        player_bounce(team[i]);
                        team[i]->actor->speed.x *= -0.5;
                        if(++item->value[0] >= maxhits) {
                            item->state = IS_ACTIVE;
                            item->value[1] = t;
                        }
                        break;
                    }
                }
                if(actor_animation_finished(act))
                    actor_change_animation(act, sprite_get_animation("SD_ENDLEVEL", 0));
            }

            /* broken */
            else if(item->state == IS_ACTIVE) {
                if(t <= item->value[1] + 2.0) { /* exploding */
                    if(t >= item->value[2]+0.1) {
                        v2d_t pos = v2d_new(act->position.x-act->hot_spot.x+random(actor_image(act)->w), act->position.y-act->hot_spot.y+random(actor_image(act)->h/2));
                        level_create_item(IT_EXPLOSION, pos);
                        sound_play( sound_load("samples/bosshit.wav") );
                        item->value[2] = t;
                    }
                }
                else { /* this object is now totally broken. :P */
                    actor_change_animation(act, sprite_get_animation("SD_ENDLEVEL", 2));

                    /* create jumping animals... */
                    if((int)item->value[3] == 0) {
                        v2d_t anpos;
                        for(i=0; i<20; i++) {
                            anpos = v2d_new(act->position.x-act->hot_spot.x+random(actor_image(act)->w), act->position.y-act->hot_spot.y+random(actor_image(act)->h/2));
                            level_create_animal(anpos);
                        }
                        level_clear(item->actor);
                        item->value[3] = 1;
                    }
                }
            }
            break;
        }


        /*
         * Bumper
         */
        case IT_BUMPER:
        {
            int i, sig;

            for(i=0; i<3; i++) {
                if(!team[i]->dieing && actor_collision(team[i]->actor, act)) {
                    if(act->animation == sprite_get_animation("SD_BUMPER", 0))
                        sound_play( sound_load("samples/bumper.wav") );

                    actor_change_animation(act, sprite_get_animation("SD_BUMPER", 1));

                    sig = (team[i]->actor->mirror<0) ? 1 : -1;
                    team[i]->actor->speed.x = sig * max(300, fabs(team[i]->actor->speed.x));

                    if(!team[i]->flying && !team[i]->landing && !team[i]->climbing)
                        team[i]->actor->speed.y = -400;
                }
            }

            if(actor_animation_finished(act))
                actor_change_animation(act, sprite_get_animation("SD_BUMPER", 0));

            break;
        }

        /*
         * Danger (useful on spikes)
         */
        case IT_DANGER:
        {
            int i;

            item->actor->visible = level_editmode();
            for(i=0; i<3; i++) {
                if(!team[i]->dieing && actor_collision(act, team[i]->actor))
                    player_hit(team[i]);
            }

            break;
        }


        /*
         * Spikes
         */
        case IT_SPIKES:
        {
            int i;
            item_t *danger = find_closest_item(item, item_list, IT_DANGER, NULL);

            if(danger && actor_collision(act, danger->actor)) { /* this danger object is associated to me. */
                for(i=0; i<3; i++) {
                    if(!team[i]->dieing && !team[i]->blinking && !team[i]->invencible && actor_collision(team[i]->actor, danger->actor)) {
                        sound_t *spk = sound_load("samples/spikes.wav");
                        if(!sound_is_playing(spk))
                            sound_play(spk);
                    }
                }
            }

            break;
        }


        /*
         * DNA Door
         */
        case IT_DNADOOR:
        {
            /* only Sonic can pass these doors. */
            int i;

            item->obstacle = TRUE;
            for(i=0; i<3 && item->obstacle; i++) {
                if(!team[i]->dieing && actor_collision(act, team[i]->actor)) {

                    if(team[i]->type == PL_SONIC)
                        item->obstacle = FALSE;

                }
            }

            if(!item->obstacle && level_player() && level_player()->type != PL_SONIC)
                item->obstacle = TRUE;

            break;
        }


        /*
         * Dangerous Power (Mecha Shadow's attack)
         * value 0: x-speed
         * value 1: y-speed
         */
        case IT_DANGPOWER:
        {
            int i;
            v2d_t ds = v2d_new(item->value[0]*dt, item->value[1]*dt);
            brick_t *bu, *bd, *bl, *br, *brk=NULL;

            /* hit player */
            for(i=0; i<3; i++) {
                if(!team[i]->dieing && actor_collision(act, team[i]->actor)) {
                    player_hit(team[i]);
                    item->state = IS_DEAD;
                    break;
                }
            }

            /* hit brick */
            actor_corners(act, 2, -2, brick_list, &bu, NULL, &br, NULL, &bd, NULL, &bl, NULL);
            if( NULL != (brk = (bd ? bd : (br ? br : (bl ? bl : (bu ? bu : NULL))))) ) { /* huahuahua :] */
                /* destroy brick */
                if(brk->brick_ref->angle == 0 && brk->y >= 1800) {
                    image_t *brkimg = brk->brick_ref->image;
                    int bw=brkimg->w/5, bh=brkimg->h/5, bi, bj;

                    /* particles */
                    for(bi=0; bi<bw; bi++) {
                        for(bj=0; bj<bh; bj++) {
                            v2d_t piecepos = v2d_new(brk->x + (bi*brkimg->w)/bw, brk->y + (bj*brkimg->h)/bh);
                            v2d_t piecespeed = v2d_new(-40+random(80), -70-random(70));
                            image_t *piece = image_create(brkimg->w/bw, brkimg->h/bh);
    
                            image_blit(brkimg, piece, (bi*brkimg->w)/bw, (bj*brkimg->h)/bh, 0, 0, piece->w, piece->h);
                            level_create_particle(piece, piecepos, piecespeed, FALSE);
                        }
                    }
    
                    /* bye! */
                    sound_play( sound_load("samples/break.wav") );
                    brk->state = BRS_DEAD;
                }

                /* destroy this power */
                item->state = IS_DEAD;
            }

            /* movement */
            act->position = v2d_add(act->position, ds);
            break;
        }


        /*
         * Fireball
         */
        case IT_FIREBALL:
        {
            int i;
            brick_t *down;

            actor_corners(act, 2, -2, brick_list, NULL, NULL, NULL, NULL, &down, NULL, NULL, NULL);

            /* behavior */
            if(act->animation == sprite_get_animation("SD_FIREBALL", 0)) {

                act->speed.x = 0;
                act->mirror = act->speed.y < 0 ? IF_VFLIP : IF_NONE;
                actor_move(act, actor_particle_movement(act, level_gravity()));

                if(down) {
                    int n = 2*random(2)+3;
                    item_t *k;
                    for(i=0; i<n; i++) {
                        k = level_create_item(IT_FIREBALL, act->position);
                        k->actor->speed.x = ((double)i/(double)n)*400 - 200;
                        k->actor->speed.y = -120-random(240);
                        actor_change_animation(k->actor, sprite_get_animation("SD_FIREBALL", 2));
                    }
                    actor_change_animation(act, sprite_get_animation("SD_FIREBALL", 1));
                    sound_play( sound_load("samples/fire2.wav") );
                }

            }
            else if(act->animation == sprite_get_animation("SD_FIREBALL", 1)) {
                if(actor_animation_finished(act))
                    item->state = IS_DEAD;
            }
            else {
                actor_move(act, actor_particle_movement(act, level_gravity()));
                if(down && act->speed.y > 0)
                    item->state = IS_DEAD;
            }

            /* player hit */
            for(i=0; i<3; i++) {
                if(!team[i]->dieing && actor_collision(act, team[i]->actor)) {
                    item->state = IS_DEAD;
                    player_hit(team[i]);
                    break;
                }
            }

            break;
        }

        /* end of list. */
    }
}







/* finds the closest item (minimal distance) of
 * a given type relative to 'me'. Returns NULL
 * if nothing nice is found */
item_t *find_closest_item(item_t *me, item_list_t *list, int desired_type, double *distance)
{
    double min_dist = 1000000; /* = 'infinity' */
    item_list_t *it;
    item_t *ret = NULL;
    v2d_t v;

    for(it=list; it; it=it->next) { /* this list must be small enough */
        if(it->data->type == desired_type) {
            v = v2d_subtract(it->data->actor->position, me->actor->position);
            if(v2d_magnitude(v) < min_dist) {
                ret = it->data;
                min_dist = v2d_magnitude(v);
            }
        }
    }

    if(distance)
        *distance = min_dist;
    return ret;
}
