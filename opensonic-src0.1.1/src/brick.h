/*
 * brick.h - brick module
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

#ifndef _BRICK_H
#define _BRICK_H

#include "video.h"
#include "sprite.h"



/* brick properties */
#define BRK_NONE                0
#define BRK_OBSTACLE            1

/* brick behavior */
#define BRICKBEHAVIOR_MAXARGS   5
#define BRB_DEFAULT             0
#define BRB_CIRCULAR            1
#define BRB_BREAKABLE           2
#define BRB_FALL                3

/* brick state */
#define BRS_IDLE                0
#define BRS_DEAD                1
#define BRS_ACTIVE              2 /* generic action */

/* misc */
#define BRB_FALL_TIME          1.0 /* time in seconds before a BRB_FALL gets destroyed */

/* brick data */
typedef struct {
    spriteinfo_t *data; /* this is not stored in the main hash */
    image_t *image; /* usually, a shortcut to data->frame_data[0] (*). This is also useful to make fake bricks */
    int property; /* BRK_* */
    int behavior; /* BRB_* */
    int angle; /* in degrees, 0 <= angle < 360 */
    double behavior_arg[BRICKBEHAVIOR_MAXARGS];

    /* (*) since every animation frame has the same dimensions, this
       is useful to check out quickly the size of a brick */
} brickdata_t;




/* real bricks */
#define BRICK_MAXVALUES            3
typedef struct { /* a real, concrete brick */
    brickdata_t *brick_ref; /* brick metadata */
    int x, y; /* current position */
    int sx, sy; /* spawn point */
    int enabled; /* useful on sonic loops */
    int state; /* BRS_* */
    double value[BRICK_MAXVALUES]; /* alterable values */
    double animation_frame; /* controlled by game_timer() */
} brick_t;

typedef struct brick_list_t { /* linked list of bricks */
    brick_t *data;
    struct brick_list_t *next;
} brick_list_t;


/* brick data */
void brickdata_load(char *filename);
void brickdata_unload();
brickdata_t *brickdata_get(int id);
int brickdata_size();

/* brick useful stuff */
void brick_animate(brick_t *brk);
image_t *brick_image(brick_t *brk);


#endif
