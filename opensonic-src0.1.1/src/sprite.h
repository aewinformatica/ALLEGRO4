/*
 * sprite.h - code for the sprites/animations
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

#ifndef _SPRITE_H
#define _SPRITE_H

#include <stdio.h>
#include "video.h"

/* animation */
typedef struct {
    int repeat;
    int fps;
    int frame_count; /* how many frames does this animation have? */
    int *data; /* frame vector */
    v2d_t hot_spot; /* hot spot */
    image_t **frame_data; /* reference to spriteinfo->frame_data (not another vector) */
} animation_t;

/* sprite info */
/* spriteinfo_t represents only ONE sprite,
 * with several animations */
typedef struct {
    char source_file[64];
    int rect_x, rect_y, rect_w, rect_h;
    int frame_w, frame_h;
    v2d_t hot_spot;

    int frame_count; /* every frame related to this sprite */
    image_t **frame_data; /* image_t* vector */

    int animation_count;
    animation_t **animation_data; /* animation_t* vector */
} spriteinfo_t;

/* sprite data & memory */
void spritedata_load(); /* reads the sprite.def file */
spriteinfo_t *spriteinfo_create();
void spriteinfo_destroy(spriteinfo_t *info);
spriteinfo_t *read_sprite(FILE *fp); /* reads sprite data from fp */

/* sprite access (uses the main hash) */
animation_t *sprite_get_animation(char *sprite_name, int anim_id);
image_t *sprite_get_image(animation_t *anim, int frame_id);

#endif
