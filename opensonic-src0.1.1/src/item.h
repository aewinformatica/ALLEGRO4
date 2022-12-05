/*
 * item.h - code for the items
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

#ifndef _ITEM_H
#define _ITEM_H

#include "util.h"
#include "player.h"

/* item list */
#define ITEMDATA_MAX        42
#define IT_RING                0 /* ordinary ring */
#define IT_LIFEBOX          1 /* life box */
#define IT_RINGBOX          2 /* ring box */
#define IT_STARBOX          3 /* invencibility stars */
#define IT_SPEEDBOX         4 /* speed shoes */
#define IT_GLASSESBOX       5 /* glasses */
#define IT_SHIELDBOX        6 /* shield */
#define IT_EGGMANBOX        7 /* eggman box */
#define IT_EMPTYBOX         8 /* free slot */
#define IT_CRUSHEDBOX       9 /* crushed box */
#define IT_ICON             10 /* box-related icon */
#define IT_FALGLASSES       11 /* falling glasses */
#define IT_EXPLOSION        12 /* explosion sprite */
#define IT_FLYINGTEXT       13 /* flying text */
#define IT_PIXEL            14 /* pixel particle */
#define IT_ANIMAL           15 /* little animal */
#define IT_LOOPRIGHT        16 /* loop right */
#define IT_LOOPMIDDLE       17 /* loop middle */
#define IT_LOOPLEFT         18 /* loop left */
#define IT_LOOPNONE         19 /* loop none */
#define IT_YELLOWSPRING     20 /* yellow spring */
#define IT_REDSPRING        21 /* red spring */
#define IT_RREDSPRING       22 /* right red spring */
#define IT_LREDSPRING       23 /* left red spring */
#define IT_BLUERING         24 /* blue ring */
#define IT_SWITCH           25 /* switch */
#define IT_DOOR             26 /* door */
#define IT_TELEPORTER       27 /* teleporter */
#define IT_BIGRING          28 /* big ring */
#define IT_CHECKPOINT       29 /* checkpoint */
#define IT_GOAL             30 /* goal sign */
#define IT_ENDSIGN          31 /* end sign */
#define IT_ENDLEVEL         32 /* end level */
#define IT_LOOPFLOOR        33 /* loop floor */
#define IT_LOOPFLOORNONE    34 /* loop floor none */
#define IT_LOOPFLOORTOP     35 /* loop floor top */
#define IT_BUMPER           36 /* bumper */
#define IT_DANGER           37 /* danger */
#define IT_SPIKES           38 /* spikes */
#define IT_DNADOOR          39 /* DNA door */
#define IT_DANGPOWER        40 /* dangerous power (boss attack) */
#define IT_FIREBALL         41 /* fireball */



/* item state */
#define IS_IDLE            0 /* idle: default state */
#define IS_DIEING        1 /* dieing state */
#define IS_DEAD            2 /* dead items are automatically removed from the item list */
#define IS_ACTIVE       3 /* generic action */


/* item struct */
#define ITEM_MAXVALUES  5
typedef struct {
    int type;
    int state;
    int obstacle;
    int preserve; /* should we delete this item when it's outside the screen? */
    int bring_to_back;
    double value[ITEM_MAXVALUES]; /* alterable values */
    actor_t *actor;
} item_t;

typedef struct item_list_t {
    item_t *data;
    struct item_list_t *next;
} item_list_t;



/* public functions */
item_t *item_create(int type);
void item_destroy(item_t *item);
void item_update(item_t *item, player_t *team[3], brick_list_t *brick_list, item_list_t *item_list);
void item_render(item_t *item, v2d_t camera_position);

#endif
