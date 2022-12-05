/*
 * boss.c - boss module
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

#include "boss.h"
#include "global.h"
#include "audio.h"
#include "actor.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "item.h"
#include "brick.h"
#include "level.h"

/* private functions */
static int got_attacked(boss_t *boss, player_t *team[3]);
static void render_details(boss_t *boss, v2d_t camera_position, int before_boss_render);

/* boss programming */
static void bossprog_simpleboss(boss_t *boss, player_t *team[3], brick_list_t *brick_list, brick_t *corners[8]);
static void bossprog_mechashadow(boss_t *boss, player_t *team[3], brick_list_t *brick_list, brick_t *corners[8]);
static void bossprog_simplebossex(boss_t *boss, player_t *team[3], brick_list_t *brick_list, brick_t *corners[8]);
static void bossprog_mechashadowex(boss_t *boss, player_t *team[3], brick_list_t *brick_list, brick_t *corners[8]);


/*
 * boss_create()
 * Creates a new boss
 */
boss_t *boss_create(int type, v2d_t spawn_point, int rx, int ry, int rw, int rh)
{
    int i;
    actor_t *act;
    boss_t *boss = mallocx(sizeof *boss);

    boss->type = type;
    boss->state = BS_IDLE;
    boss->rect_x = rx;
    boss->rect_y = ry;
    boss->rect_w = rw;
    boss->rect_h = rh;
    for(i=0; i<BOSSDATA_MAXVALUES; i++)
        boss->value[i] = 0;

    boss->bring_to_front = FALSE;
    boss->actor = act = actor_create();
    act->spawn_point = act->position = spawn_point;
    act->input = input_create_computer();

    switch(type) {

        /* Simple Boss */
        case BT_SIMPLEBOSS:
        case BT_SIMPLEBOSSEX:
            actor_change_animation(act, sprite_get_animation("SD_SIMPLEBOSS", 0));
            boss->initial_hp = boss->hp = 10;
            boss->direction = BD_LEFT;
            act->maxspeed = 100;
            act->acceleration = 100;
            break;

        /* Mecha Shadow */
        case BT_MECHASHADOW:
        case BT_MECHASHADOWEX:
            actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 0));
            boss->initial_hp = boss->hp = 10;
            boss->direction = BD_LEFT;
            act->maxspeed = 100;
            act->acceleration = 100;
            break;
    }

    return boss;
}


/*
 * boss_destroy()
 * Destroys a boss
 */
void boss_destroy(boss_t *boss)
{
    actor_destroy(boss->actor);
    free(boss);
}


/*
 * boss_render()
 * Renders a boss
 */
void boss_render(boss_t *boss, v2d_t camera_position)
{
    render_details(boss, camera_position, TRUE);
    actor_render(boss->actor, camera_position);
    render_details(boss, camera_position, FALSE);
}


/*
 * boss_update()
 * Runs every cycle of the game to update a boss
 */
void boss_update(boss_t *boss, player_t *team[3], brick_list_t *brick_list)
{
    actor_t *act = boss->actor;
    brick_t *up = NULL, *upright = NULL, *right = NULL, *downright = NULL;
    brick_t *down = NULL, *downleft = NULL, *left = NULL, *upleft = NULL;
    brick_t *corners[8];
    actor_corners(act, 2, -2, brick_list, &up, &upright, &right, &downright, &down, &downleft, &left, &upleft);
    corners[0] = up; corners[1] = upright; corners[2] = right; corners[3] = downright;
    corners[4] = down; corners[5] = downleft; corners[6] = left; corners[7] = upleft;

    /* the player must be fighting against me! bwahawhawh! */
    if(!level_boss_battle())
        return;

    /* noooooooooooooo...! i've lost the battle :'-( */
    if(boss->hp <= 0 && boss->state != BS_DEAD) {
        level_kill_all_baddies();
        boss->state = BS_DEAD;
    }

    /* which boss? */
    switch(boss->type) {

        /* Simple Boss (what a great name!) */
        case BT_SIMPLEBOSS:
            bossprog_simpleboss(boss, team, brick_list, corners);
            break;

        /* Mecha Shadow */
        case BT_MECHASHADOW:
            bossprog_mechashadow(boss, team, brick_list, corners);
            break;

        /* Simple Boss Ex */
        case BT_SIMPLEBOSSEX:
            bossprog_simplebossex(boss, team, brick_list, corners);
            break;

        /* Mecha Shadow Ex */
        case BT_MECHASHADOWEX:
            bossprog_mechashadowex(boss, team, brick_list, corners);
            break;
    }
}


/*
 * boss_defeated()
 * Returns TRUE if the boss has been defeated
 */
int boss_defeated(boss_t *boss)
{
    return (boss->state == BS_DEAD);
}




/* private functions */

/*
 * got_attacked()
 * Returns TRUE if the player attacked the boss,
 * FALSE otherwise.
 *
 * TODO: got_attacked_region(boss_t* boss, player_t* team[], rectangle_t weak_region)
 */
int got_attacked(boss_t *boss, player_t *team[3])
{
    int i;

    for(i=0; i<3 && boss->state != BS_DEAD; i++) {
        if(actor_collision(boss->actor, team[i]->actor)) {
            if(player_attacking(team[i]) || team[i]->invencible) {
                return TRUE;
            }
            else {
                player_hit(team[i]);
                return FALSE;
            }
        }
    }

    return FALSE;
}


/*
 * render_details()
 * Renders additional details about a boss.
 * If before_boss_render == TRUE, this function will
 * be called before the engine renders the boss' actor
 */
void render_details(boss_t *boss, v2d_t camera_position, int before_boss_render)
{
    actor_t *act = boss->actor;
    double t = game_timer() * 0.001;

    /* boss type */
    switch(boss->type) {

        /* Mecha Shadow Ex */
        case BT_MECHASHADOWEX:
        {
            int i, n = 5;
            v2d_t pos;
            image_t *star = sprite_get_image( sprite_get_animation("SD_INVSTAR", 0), 1 );

            if(boss->state != BS_DEAD) {
                for(i=0; i<n; i++) {
                    pos.x = act->position.x + 20*cos(2*PI*t + (i*2*PI)/n);
                    pos.y = act->position.y + 20*sin(PI*t + (i*2*PI)/n);
                    if( (before_boss_render && pos.y < act->position.y) || (!before_boss_render && pos.y >= act->position.y) )
                        image_draw(star, video_buffer, pos.x+VIDEO_SCREEN_W/2-camera_position.x, pos.y+VIDEO_SCREEN_H/2-camera_position.y, IF_NONE);
                }
            }

            break;
        }
    }
}




/* boss programming */


/*
 * bossprog_simpleboss()
 * A very simple boss that walks and throws enemies
 */
void bossprog_simpleboss(boss_t *boss, player_t *team[3], brick_list_t *brick_list, brick_t *corners[8])
{
    player_t *player = level_player();
    double t = game_timer()*0.001;
    actor_t *act = boss->actor;

    double *lastthrow = &boss->value[0];
    double *explosiontimer = &boss->value[1];

    if(boss->state == BS_DEAD) {
        /* dead */
        boss->bring_to_front = TRUE;
        actor_change_animation(act, sprite_get_animation("SD_SIMPLEBOSS", 2));
        act->position.y += 100*game_delta();
        if(t >= *explosiontimer + 0.15) {
            v2d_t pos = v2d_new(act->position.x-act->hot_spot.x+random(actor_image(act)->w), act->position.y-act->hot_spot.y+random(actor_image(act)->h));
            level_create_item(IT_EXPLOSION, pos);
            if(act->position.y <= act->spawn_point.y + 1.5*VIDEO_SCREEN_H)
                sound_play( sound_load("samples/bosshit.wav") );
            *explosiontimer = t;
        }
    }
    else {
        /* walk & float */
        act->maxspeed = 200 - (100/boss->initial_hp)*boss->hp;
        act->acceleration = act->maxspeed;
        if(act->position.x < act->spawn_point.x - 200) boss->direction = BD_RIGHT;
        if(act->position.x > act->spawn_point.x) boss->direction = BD_LEFT;
        if(boss->direction == BD_RIGHT) input_simulate_button_down(act->input, IB_RIGHT);
        else if(boss->direction == BD_LEFT) input_simulate_button_down(act->input, IB_LEFT);
        act->position.y = act->spawn_point.y + 20*cos(PI * t);
        act->mirror = (boss->direction == BD_RIGHT) ? IF_NONE : IF_HFLIP;
        actor_move(act, actor_eightdirections_movement(act));

        /* throw enemies */
        if(t >= *lastthrow + (2.0/boss->initial_hp)*boss->hp) {
            int en_type[2] = { ET_JOAN, ET_KLEPS };
            enemy_t *en = level_create_enemy(en_type[ random(2) ], act->position);
            en->actor->speed.y = -150-random(50);
            en->direction = (en->actor->position.x < player->actor->position.x) ? ED_RIGHT : ED_LEFT;    
            *lastthrow = t;        
        }

        /* ouch! i'm being attacked! */
        if(got_attacked(boss, team) && act->animation == sprite_get_animation("SD_SIMPLEBOSS", 0)) {
            actor_change_animation(act, sprite_get_animation("SD_SIMPLEBOSS", 1));
            sound_play( sound_load("samples/bosshit.wav") );
            player->actor->speed.x *= -1;
            player->actor->speed.y = 100;
            boss->hp--;
        }
        if(actor_animation_finished(act))
            actor_change_animation(act, sprite_get_animation("SD_SIMPLEBOSS", 0));
    }
}



/*
 * bossprog_mechashadow()
 * Mecha Shadow
 */
void bossprog_mechashadow(boss_t *boss, player_t *team[3], brick_list_t *brick_list, brick_t *corners[8])
{
    player_t *player = level_player();
    actor_t *act = boss->actor;
    double t = game_timer()*0.001, dt = game_delta();
    int being_hit;

    /* custom variables */
    double *lastshot = &boss->value[0];
    double *lastatk = &boss->value[1];
    double *explosiontimer = &boss->value[2];

    /* states */
    switch(boss->state) {
        /* stopped & shooting */
        case BS_IDLE:
        {
            /* movement */
            boss->direction = (player->actor->position.x < act->position.x) ? BD_LEFT : BD_RIGHT;
            act->position.y = act->spawn_point.y + 30*cos(PI*t);

            /* animation */
            if(actor_animation_finished(act))
                actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 0));

            /* shot! */
            if(t >= *lastshot + 5.0) {
                v2d_t v = v2d_new(player->actor->position.x - act->position.x, player->actor->position.y - act->position.y);
                item_t *shot;

                v = v2d_normalize(v);
                shot = level_create_item(IT_DANGPOWER, act->position);
                shot->value[0] = 200*v.x;
                shot->value[1] = 200*v.y;

                sound_play( sound_load("samples/bigshot.wav") );
                *lastshot = t;
            }

            /* attack mode? */
            if(t >= *lastatk + 15.0) {
                double med = boss->rect_x + boss->rect_w/2;
                boss->direction = (act->position.x > med) ? BD_LEFT : BD_RIGHT;
                actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 2));
                boss->state = BS_ACTIVE;
                *lastatk = t;
            }

            break;
        }



        /* moving around */
        case BS_ACTIVE:
        {
            double delta = 100, amplitude = 100, xspeed = 200;
            double a = boss->rect_x + delta;
            double b = boss->rect_x + boss->rect_w - delta;
            double x = (b - a) + (act->position.x - a);

            /* movement */
            int sig = (boss->direction == BD_LEFT) ? -1 : 1;
            act->position.x += (sig*xspeed) * dt;
            act->position.y = act->spawn_point.y - amplitude * sin( (PI/(b-a)) * x );

            /* animation */
            if(actor_animation_finished(act))
                actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 2));

            /* back to normal mode */
            if( (sig == -1 && act->position.x <= a) || (sig == 1 && act->position.x >= b) ) {
                actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 0));
                boss->state = BS_IDLE;
            }

            break;
        }


        /* dead */
        case BS_DEAD:
        {
            boss->bring_to_front = TRUE;
            actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 4));
            act->position.y += 100*game_delta();
            if(t >= *explosiontimer + 0.15) {
                v2d_t pos = v2d_new(act->position.x-act->hot_spot.x+random(actor_image(act)->w), act->position.y-act->hot_spot.y+random(actor_image(act)->h));
                level_create_item(IT_EXPLOSION, pos);
                if(act->position.y <= act->spawn_point.y + 1.5*VIDEO_SCREEN_H)
                    sound_play( sound_load("samples/bosshit.wav") );
                *explosiontimer = t;
            }

            break;
        }
    }

    /* ouch! i'm being attacked! */
    being_hit = (act->animation==sprite_get_animation("SD_MECHASHADOW",1)) || (act->animation==sprite_get_animation("SD_MECHASHADOW",3));
    if(got_attacked(boss, team) && !being_hit && boss->state != BS_DEAD) {
        act->animation = sprite_get_animation("SD_MECHASHADOW", (boss->state == BS_ACTIVE) ? 3 : 1);
        sound_play( sound_load("samples/bosshit.wav") );
        boss->hp--;
        player->actor->speed.x *= -0.5;
        player->actor->speed.y = player->actor->jump_strength;
    }

    /* misc */
    act->mirror = (boss->direction == BD_RIGHT) ? IF_NONE : IF_HFLIP;
}



/*
 * bossprog_simplebossex()
 * Simple Boss Extended!
 */
void bossprog_simplebossex(boss_t *boss, player_t *team[3], brick_list_t *brick_list, brick_t *corners[8])
{
    player_t *player = level_player();
    double t = game_timer()*0.001;
    actor_t *act = boss->actor;

    double *lastthrow = &boss->value[0];
    double *explosiontimer = &boss->value[1];
    double *lastfbthrow = &boss->value[2];

    if(boss->state == BS_DEAD) {
        /* dead */
        boss->bring_to_front = TRUE;
        actor_change_animation(act, sprite_get_animation("SD_SIMPLEBOSS", 2));
        act->position.y += 100*game_delta();
        if(t >= *explosiontimer + 0.15) {
            v2d_t pos = v2d_new(act->position.x-act->hot_spot.x+random(actor_image(act)->w), act->position.y-act->hot_spot.y+random(actor_image(act)->h));
            level_create_item(IT_EXPLOSION, pos);
            if(act->position.y <= act->spawn_point.y + 1.5*VIDEO_SCREEN_H)
                sound_play( sound_load("samples/bosshit.wav") );
            *explosiontimer = t;
        }
    }
    else {
        /* walk & float */
        act->maxspeed = 200 - (100/boss->initial_hp)*boss->hp;
        act->acceleration = act->maxspeed;
        if(act->position.x < act->spawn_point.x - 200) boss->direction = BD_RIGHT;
        if(act->position.x > act->spawn_point.x) boss->direction = BD_LEFT;
        if(boss->direction == BD_RIGHT) input_simulate_button_down(act->input, IB_RIGHT);
        else if(boss->direction == BD_LEFT) input_simulate_button_down(act->input, IB_LEFT);
        act->position.y = act->spawn_point.y + 20*cos(PI * t);
        act->mirror = (boss->direction == BD_RIGHT) ? IF_NONE : IF_HFLIP;
        actor_move(act, actor_eightdirections_movement(act));

        /* throw enemies */
        if(t >= *lastthrow + (2.0/boss->initial_hp)*boss->hp) {
            int en_type[2] = { ET_CHEF, ET_ORANJECTION };
            enemy_t *en = level_create_enemy(en_type[ random(2) ], act->position);
            en->actor->speed.y = -150-random(50);
            en->direction = (en->actor->position.x < player->actor->position.x) ? ED_RIGHT : ED_LEFT;    
            *lastthrow = t;        
        }

        /* throw fireballs */
        if(t >= *lastfbthrow + (3.0/boss->initial_hp)*boss->hp) {
            item_t *it = level_create_item(IT_FIREBALL, act->position);
            it->actor->speed.y = 100;
            sound_play( sound_load("samples/fire.wav") );
            *lastfbthrow = t;
        }

        /* ouch! i'm being attacked! */
        if(got_attacked(boss, team) && act->animation == sprite_get_animation("SD_SIMPLEBOSS", 0)) {
            actor_change_animation(act, sprite_get_animation("SD_SIMPLEBOSS", 1));
            sound_play( sound_load("samples/bosshit.wav") );
            player->actor->speed.x *= -1;
            player->actor->speed.y = 100;
            boss->hp--;
        }
        if(actor_animation_finished(act))
            actor_change_animation(act, sprite_get_animation("SD_SIMPLEBOSS", 0));
    }
}


/*
 * bossprog_mechashadowex()
 * Mecha Shadow Ex
 */
void bossprog_mechashadowex(boss_t *boss, player_t *team[3], brick_list_t *brick_list, brick_t *corners[8])
{
    player_t *player = level_player();
    actor_t *act = boss->actor;
    double t = game_timer()*0.001, dt = game_delta();
    int being_hit;

    /* custom variables */
    double *lastshot = &boss->value[0];
    double *lastatk = &boss->value[1];
    double *explosiontimer = &boss->value[2];
    double *lastfb = &boss->value[3];

    /* states */
    switch(boss->state) {
        /* stopped & shooting */
        case BS_IDLE:
        {
            /* movement */
            boss->direction = (player->actor->position.x < act->position.x) ? BD_LEFT : BD_RIGHT;
            act->position.y = act->spawn_point.y + 30*cos(PI*t);

            /* animation */
            if(actor_animation_finished(act))
                actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 0));

            /* shot! */
            if(t >= *lastshot + 4.0) {
                v2d_t v = v2d_new(player->actor->position.x - act->position.x, player->actor->position.y - act->position.y);
                item_t *shot;

                v = v2d_normalize(v);
                shot = level_create_item(IT_DANGPOWER, act->position);
                shot->value[0] = 200*v.x;
                shot->value[1] = 200*v.y;

                sound_play( sound_load("samples/bigshot.wav") );
                *lastshot = t;
            }

            /* attack mode? */
            if(t >= *lastatk + 8.0) {
                double med = boss->rect_x + boss->rect_w/2;
                boss->direction = (act->position.x > med) ? BD_LEFT : BD_RIGHT;
                actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 2));
                boss->state = BS_ACTIVE;
                *lastatk = t;
            }

            break;
        }



        /* moving around */
        case BS_ACTIVE:
        {
            double delta = 100, amplitude = 100, xspeed = 200;
            double a = boss->rect_x + delta;
            double b = boss->rect_x + boss->rect_w - delta;
            double x = (b - a) + (act->position.x - a);

            /* movement */
            int sig = (boss->direction == BD_LEFT) ? -1 : 1;
            act->position.x += (sig*xspeed) * dt;
            act->position.y = act->spawn_point.y - amplitude * sin( (PI/(b-a)) * x );

            /* fireballs */
            if(t >= *lastfb + 0.2) {
                item_t *it = level_create_item(IT_FIREBALL, act->position);
                it->actor->speed.y = -200;
                sound_play( sound_load("samples/fire.wav") );
                *lastfb = t;
            }

            /* animation */
            if(actor_animation_finished(act))
                actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 2));

            /* back to normal mode */
            if( (sig == -1 && act->position.x <= a) || (sig == 1 && act->position.x >= b) ) {
                actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 0));
                boss->state = BS_IDLE;
            }

            break;
        }


        /* dead */
        case BS_DEAD:
        {
            boss->bring_to_front = TRUE;
            actor_change_animation(act, sprite_get_animation("SD_MECHASHADOW", 4));
            act->position.y += 100*game_delta();
            if(t >= *explosiontimer + 0.15) {
                v2d_t pos = v2d_new(act->position.x-act->hot_spot.x+random(actor_image(act)->w), act->position.y-act->hot_spot.y+random(actor_image(act)->h));
                level_create_item(IT_EXPLOSION, pos);
                if(act->position.y <= act->spawn_point.y + 1.5*VIDEO_SCREEN_H)
                    sound_play( sound_load("samples/bosshit.wav") );
                *explosiontimer = t;
            }

            break;
        }
    }

    /* ouch! i'm being attacked! */
    being_hit = (act->animation==sprite_get_animation("SD_MECHASHADOW",1)) || (act->animation==sprite_get_animation("SD_MECHASHADOW",3));
    if(got_attacked(boss, team) && !being_hit && boss->state != BS_DEAD) {
        act->animation = sprite_get_animation("SD_MECHASHADOW", (boss->state == BS_ACTIVE) ? 3 : 1);
        sound_play( sound_load("samples/bosshit.wav") );
        boss->hp--;
        player->actor->speed.x *= -0.5;
        player->actor->speed.y = player->actor->jump_strength;
    }

    /* misc */
    act->mirror = (boss->direction == BD_RIGHT) ? IF_NONE : IF_HFLIP;
}
