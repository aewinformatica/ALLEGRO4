/*
 * actor.h - actor module
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

#ifndef _ACTOR_H
#define _ACTOR_H

#include "sprite.h"
#include "input.h"
#include "util.h"
#include "brick.h"

/* actor structure */
typedef struct {

    /* movement data */
    v2d_t position, spawn_point;
    v2d_t speed;
    double maxspeed; /* on the x-axis */
    double acceleration; /* on the x-axis */
    double angle; /* angle = ang( actor's x-axis , real x-axis ) */
    double jump_strength;
    int is_jumping;
    int ignore_horizontal;
    input_t *input; /* NULL by default (no input) */

    /* animation */
    animation_t *animation;
    double animation_frame; /* controlled by game_timer() */
    int mirror; /* see the IF_* flags at video.h */
    int visible;
    v2d_t hot_spot;

} actor_t;


/* actor functions */
actor_t* actor_create();
void actor_destroy(actor_t *act);
void actor_render(actor_t *act, v2d_t camera_position);
void actor_render_repeat_xy(actor_t *act, v2d_t camera_position, int repeat_x, int repeat_y);
void actor_move(actor_t *act, v2d_t delta_space); /* uses the orientation angle (you must call the delta timer yourself) ; s = vt */
void actor_change_animation_frame(actor_t *act, int frame);
void actor_change_animation(actor_t *act, animation_t *anim);
int actor_animation_finished(actor_t *act); /* true if the current animation has finished */
int actor_collision(actor_t *a, actor_t *b);
int actor_brick_collision(actor_t *act, brick_t *brk);
image_t* actor_image(actor_t *act);
void actor_corners_ex(actor_t *act, double sqrsize, v2d_t vup, v2d_t vupright, v2d_t vright, v2d_t vdownright, v2d_t vdown, v2d_t vdownleft, v2d_t vleft, v2d_t vupleft, brick_list_t *brick_list, brick_t **up, brick_t **upright, brick_t **right, brick_t **downright, brick_t **down, brick_t **downleft, brick_t **left, brick_t **upleft);
void actor_corners_set_floor_priority(int floor); /* floor x wall */
void actor_corners_restore_floor_priority();
void actor_corners_set_slope_priority(int slope); /* slope x floor */
void actor_corners_restore_slope_priority();
void actor_corners(actor_t *act, double sqrsize, double diff, brick_list_t *brick_list, brick_t **up, brick_t **upright, brick_t **right, brick_t **downright, brick_t **down, brick_t **downleft, brick_t **left, brick_t **upleft);
v2d_t actor_platform_movement(actor_t *act, brick_list_t *brick_list, double gravity);
v2d_t actor_particle_movement(actor_t *act, double gravity);
v2d_t actor_eightdirections_movement(actor_t *act);

#endif
