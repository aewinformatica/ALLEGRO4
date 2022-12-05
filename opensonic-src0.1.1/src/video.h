/*
 * video.h - video manager
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

#ifndef _VIDEO_H
#define _VIDEO_H

#include <allegro.h>
#include "2xsai.h"
#include "util.h"

/* images */
#define IF_NONE                    0
#define IF_HFLIP                1
#define IF_VFLIP                2

typedef struct {
    BITMAP *data;
    int w, h;
} image_t;

image_t *image_load(char *path); /* will be unloaded automatically */
image_t *image_create(int width, int height);
void image_destroy(image_t *img); /* call this after image_create() */
uint32 image_getpixel(image_t *img, int x, int y);
void image_putpixel(image_t *img, int x, int y, uint32 color);
uint32 image_rgb(uint8 r, uint8 g, uint8 b);
void image_clear(image_t *img, uint32 color);
void image_blit(image_t *src, image_t *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void image_draw(image_t *src, image_t *dest, int x, int y, uint32 flags);
void image_draw_scaled(image_t *src, image_t *dest, int x, int y, v2d_t scale, uint32 flags);
void image_draw_rotated(image_t *src, image_t *dest, int x, int y, int cx, int cy, double ang, uint32 flags);
void image_save(image_t *img, char *path);


/* Fade-in & fade-out */
void fadefx_in(uint32 color, double seconds);
void fadefx_out(uint32 color, double seconds);
int fadefx_over(); /* end of fade effect? */


/* video manager */
#define VIDEO_SCREEN_W            320
#define VIDEO_SCREEN_H            240
#define VIDEO_DEFAULT_BPP       16
extern image_t *video_buffer;

void video_init(int use_2xsai, int use_supereagle, int use_fast2x);
void video_render();
void video_release();
void video_showmessage(char *fmt, ...);
int video_is_window_active();



#endif
