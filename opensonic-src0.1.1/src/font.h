/*
 * font.h - font module
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

#ifndef _FONT_H
#define _FONT_H

#include "util.h"

/* font struct */
#define FONT_TEXTMAXLENGTH          640
#define FONT_MAXVALUES              5
typedef struct {
    int type;
    char text[FONT_TEXTMAXLENGTH];

    v2d_t position;
    int visible;
    double value[FONT_MAXVALUES]; /* alterable values */
    int hspace, vspace;
} font_t;


/* public functions */
font_t *font_create(int type);
void font_destroy(font_t *f);
void font_set_text(font_t *f, char *fmt, ...);
void font_set_text_wordwrap(font_t *f, int minlength, int maxlength, char *fmt, ...);
char *font_get_text(font_t *f);
v2d_t font_get_charsize(font_t *f);
void font_render(font_t *f, v2d_t camera_position);

/* misc */
void font_init(); /* initializes the font module */


#endif
