/*
 * enemy.c - code for the enemies
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
#include "audio.h"
#include "actor.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "item.h"
#include "brick.h"
#include "level.h"



/* private data */
#define WALK_LEFTRIGHTSMART                 0
#define WALK_LEFTRIGHTDUMB                  1
#define WALK_LEVITATE                       2
#define WALK_LEVITATELEFTRIGHT              3
#define WALK_KAMIKAZE                       4 /* goldfish-like (Goldfish is an enemy!) */
#define WALK_SMARTKAMIKAZE                  5
static void walk(enemy_t *enemy, int walk_type, brick_list_t *brick_list, brick_t *corners[8]);

#define AOD_NONE                            0 /* nothing happened */
#define AOD_ATTACK                          1 /* the enemy has attacked the player */
#define AOD_DIE                             2 /* the enemy was killed */
static int attack_or_die(enemy_t *enemy, player_t *team[3]);



/*
 * enemy_create()
 * Creates a new enemy
 */
enemy_t *enemy_create(int type)
{
    actor_t *act;
    enemy_t *enemy = mallocx(sizeof *enemy);
    enemy->type = type;
    enemy->state = ES_IDLE;
    enemy->actor = act = actor_create();
    enemy->mytimer = 0;
    act->input = input_create_computer();

    switch(type) {
        /* Joan */
        case ET_JOAN:
            actor_change_animation(act, sprite_get_animation("SD_JOAN", 0));
            act->maxspeed = 100;
            act->acceleration = 100;
            break;

        /* Flying Boy */
        case ET_FLYINGBOY:
            actor_change_animation(act, sprite_get_animation("SD_FLYINGBOY", 0));
            act->maxspeed = 100;
            act->acceleration = 100;
            break;

        /* Flying Eyes */
        case ET_FLYINGEYES:
            actor_change_animation(act, sprite_get_animation("SD_FLYINGEYES", 0));
            act->maxspeed = 100;
            act->acceleration = 100;
            break;

        /* Kleps */
        case ET_KLEPS:
            actor_change_animation(act, sprite_get_animation("SD_KLEPS", 0));
            act->maxspeed = 50;
            act->acceleration = 100;
            break;

        /* Goldfish */
        case ET_GOLDFISH:
            actor_change_animation(act, sprite_get_animation("SD_GOLDFISH", 0));
            act->maxspeed = 150;
            act->acceleration = 75;
            break;

        /* Surpreyes */
        case ET_SURPREYES:
            actor_change_animation(act, sprite_get_animation("SD_SURPREYES", 0));
            act->maxspeed = 200;
            act->acceleration = 75;
            break;

        /* Oranjection */
        case ET_ORANJECTION:
            actor_change_animation(act, sprite_get_animation("SD_ORANJECTION", 0));
            act->maxspeed = 100+random(20);
            act->acceleration = 100;
            break;

        /* Roboxer */
        case ET_ROBOXER:
            actor_change_animation(act, sprite_get_animation("SD_ROBOXER", 0));
            act->maxspeed = 100+random(20);
            act->acceleration = 100;
            break;

        /* Nafder (Redfan) */
        case ET_NAFDER:
            actor_change_animation(act, sprite_get_animation("SD_NAFDER", 0));
            act->maxspeed = 100+random(50);
            act->acceleration = 120;
            break;

        /* Chef */
        case ET_CHEF:
            actor_change_animation(act, sprite_get_animation("SD_CHEF", 0));
            act->maxspeed = 120;
            act->acceleration = 100;
            break;
    }

    return enemy;
}


/*
 * enemy_destroy()
 * Destroys an enemy
 */
void enemy_destroy(enemy_t *enemy)
{
    actor_destroy(enemy->actor);
    free(enemy);
}


/*
 * enemy_render()
 * Renders an enemy
 */
void enemy_render(enemy_t *enemy, v2d_t camera_position)
{
    actor_render(enemy->actor, camera_position);
}


/*
 * enemy_update()
 * Runs every cycle of the game to update an enemy
 */
void enemy_update(enemy_t *enemy, player_t *team[3], brick_list_t *brick_list)
{
    actor_t *act = enemy->actor;
    brick_t *up = NULL, *upright = NULL, *right = NULL, *downright = NULL;
    brick_t *down = NULL, *downleft = NULL, *left = NULL, *upleft = NULL;
    brick_t *corners[8];
    actor_corners(act, 2, -2, brick_list, &up, &upright, &right, &downright, &down, &downleft, &left, &upleft);
    corners[0] = up; corners[1] = upright; corners[2] = right; corners[3] = downright;
    corners[4] = down; corners[5] = downleft; corners[6] = left; corners[7] = upleft;
    enemy->mytimer += game_delta();

    /* which enemy? */
    switch(enemy->type) {

        /* Joan */
        case ET_JOAN:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(100);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_LEFTRIGHTSMART, brick_list, corners);

            break;

        /* Flying Boy */
        case ET_FLYINGBOY:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(100);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_LEVITATELEFTRIGHT, brick_list, corners);

            break;

        /* Flying Eyes */
        case ET_FLYINGEYES:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(100);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_LEVITATE, brick_list, corners);

            break;

        /* Kleps */
        case ET_KLEPS:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(50);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_LEFTRIGHTDUMB, brick_list, corners);

            break;

        /* Goldfish */
        case ET_GOLDFISH:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(100);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_KAMIKAZE, brick_list, corners);

            break;

        /* Surpreyes */
        case ET_SURPREYES:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(150);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_KAMIKAZE, brick_list, corners);

            break;

        /* Oranjection */
        case ET_ORANJECTION:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(150);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_SMARTKAMIKAZE, brick_list, corners);

            break;

        /* Roboxer */
        case ET_ROBOXER:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(100);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_LEFTRIGHTSMART, brick_list, corners);

            break;

        /* Nafder */
        case ET_NAFDER:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(150);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_LEFTRIGHTSMART, brick_list, corners);

            break;

        /* Chef */
        case ET_CHEF:
            if(attack_or_die(enemy, team) == AOD_DIE) {
                level_add_to_score(100);
                level_create_animal(act->position);
            }
            else
                walk(enemy, WALK_LEFTRIGHTSMART, brick_list, corners);

            break;
    }
}




/* private functions */

/* attack_or_die()
 * Default enemy attack behavior. Returns AOD_ATTACK
 * if the player was attacked, AOD_DIE if the player
 * kills this enemy or AOD_NONE otherwise */
int attack_or_die(enemy_t *enemy, player_t *team[3])
{
    int i;

    for(i=0; i<3; i++) {
        if(actor_collision(enemy->actor, team[i]->actor)) {
            if(player_attacking(team[i]) || team[i]->invencible) {
                enemy->state = ES_DEAD;
                if(team[i]->actor->is_jumping) /* bounce */
                    player_bounce(team[i]);
                /*else if(team[i]->type == PL_KNUCKLES && team[i]->flying) -- bounce, but not so much <-- TODO
                    team[i]->actor->speed.y = -team[i]->actor->jump_strength*0.2;*/

                level_create_item(IT_EXPLOSION, v2d_add(enemy->actor->position, v2d_new(0,-15)));
                sound_play( sound_load("samples/destroypop.wav") );
                return AOD_DIE;
            }
            else {
                player_hit(team[i]);
                return AOD_ATTACK;
            }
        }
    }

    return AOD_NONE;
}



/* walk()
 * Walks using a given behavior (walk_type) */
void walk(enemy_t *enemy, int walk_type, brick_list_t *brick_list, brick_t *corners[8])
{
    double t = enemy->mytimer;
    actor_t *act = enemy->actor;
    input_t *in = act->input;
    brick_t *up, *upright, *right, *downright, *down, *downleft, *left, *upleft;
    up = corners[0]; upright = corners[1]; right = corners[2]; downright = corners[3];
    down = corners[4]; downleft = corners[5]; left = corners[6]; upleft = corners[7];

    switch(walk_type) {

        /* the enemy walks left and right, but it's smart
         * enough to avoid falling from platforms */
        case WALK_LEFTRIGHTSMART:
            if(!downright || (downright && downright->brick_ref->angle != 0) || right) {
                enemy->direction = ED_LEFT;
                act->speed.x = -fabs(act->speed.x);
            }
            else if(!downleft || (downleft && downleft->brick_ref->angle != 0) || left) {
                enemy->direction = ED_RIGHT;
                act->speed.x = fabs(act->speed.x);
            }
            input_simulate_button_down(in, enemy->direction == ED_RIGHT ? IB_RIGHT : IB_LEFT);
            actor_move(act, actor_platform_movement(act, brick_list, level_gravity()));
            break;


        /* the enemy walks left and right, but it may fall
         * from the platforms (it can't see too much) */
        case WALK_LEFTRIGHTDUMB:
            if(downright && downright->brick_ref->angle != 0) {
                enemy->direction = ED_LEFT;
                act->speed.x = -fabs(act->speed.x);
            }
            else if(downleft && downleft->brick_ref->angle != 0) {
                enemy->direction = ED_RIGHT;
                act->speed.x = fabs(act->speed.x);
            }
            input_simulate_button_down(in, enemy->direction == ED_RIGHT ? IB_RIGHT : IB_LEFT);
            actor_move(act, actor_platform_movement(act, brick_list, level_gravity()));
            break;


        /* the enemy levitates a little bit */
        case WALK_LEVITATE:
            act->position.y = act->spawn_point.y + 20*cos( PI * t );
            act->mirror = (level_player()->actor->position.x > act->position.x) ? IF_NONE : IF_HFLIP;
            break;           

        /* it levitates and walks left and right... */
        case WALK_LEVITATELEFTRIGHT:
            if((right || downright || upright) && act->speed.x > 0) {
                enemy->direction = ED_LEFT;
                act->speed.x = -fabs(act->speed.x);
            }
            else if((left || downleft || upleft) && act->speed.x < 0) {
                enemy->direction = ED_RIGHT;
                act->speed.x = fabs(act->speed.x);
            }
            input_simulate_button_down(in, enemy->direction == ED_RIGHT ? IB_RIGHT : IB_LEFT);
            actor_move(act, actor_eightdirections_movement(act));
            act->position.y = act->spawn_point.y + 20*cos( PI * t );
            act->mirror = (enemy->direction == ED_RIGHT) ? IF_NONE : IF_HFLIP;
            break;

        /* it shoots itself towards the player! (goldfish-like) */
        case WALK_KAMIKAZE:
            if(fabs(act->speed.x) < EPSILON) { /* run only once */
                enemy->direction = (level_player()->actor->position.x > act->position.x) ? ED_RIGHT : ED_LEFT;
                act->mirror = (enemy->direction == ED_RIGHT) ? IF_NONE : IF_HFLIP;
            }
            input_simulate_button_down(in, enemy->direction == ED_RIGHT ? IB_RIGHT : IB_LEFT);
            actor_move(act, actor_eightdirections_movement(act));
            break;

        /* smarter than WALK_KAMIKAZE because it follows the player all the time */
        case WALK_SMARTKAMIKAZE:
            enemy->direction = (level_player()->actor->position.x > act->position.x) ? ED_RIGHT : ED_LEFT;
            act->mirror = (enemy->direction == ED_RIGHT) ? IF_NONE : IF_HFLIP;
            input_simulate_button_down(in, enemy->direction == ED_RIGHT ? IB_RIGHT : IB_LEFT);
            input_simulate_button_down(in, level_player()->actor->position.y < act->position.y ? IB_UP : IB_DOWN);
            actor_move(act, actor_eightdirections_movement(act));
            break;
    }
}
