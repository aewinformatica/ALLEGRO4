/*
 * enemy.h - code for the enemies
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

#ifndef _ENEMY_H
#define _ENEMY_H

#include "util.h"
#include "player.h"

/* enemy list */
#define ENEMYDATA_MAX        10
#define ET_JOAN                0
#define ET_FLYINGBOY        1
#define ET_FLYINGEYES       2
#define ET_KLEPS            3
#define ET_GOLDFISH         4
#define ET_SURPREYES        5
#define ET_ORANJECTION      6
#define ET_ROBOXER          7
#define ET_NAFDER           8
#define ET_CHEF             9


/* enemy state */
#define ES_IDLE                0
#define ES_DIEING            1
#define ES_DEAD                2
#define ES_ACTIVE            3 /* generic action */


/* enemy direction */
#define ED_UP               0
#define ED_RIGHT            1
#define ED_DOWN             2
#define ED_LEFT             3


/* enemy struct */
typedef struct {
    int type;
    int state;
    int direction;
    double mytimer;
    actor_t *actor;
} enemy_t;

typedef struct enemy_list_t {
    enemy_t *data;
    struct enemy_list_t *next;
} enemy_list_t;



/* public functions */
enemy_t *enemy_create(int type);
void enemy_destroy(enemy_t *enemy);
void enemy_update(enemy_t *enemy, player_t *team[3], brick_list_t *brick_list);
void enemy_render(enemy_t *enemy, v2d_t camera_position);

#endif
