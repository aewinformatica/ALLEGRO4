/*
 * input.h - input management
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

#ifndef _INPUT_H
#define _INPUT_H

/* input button mapping */
#define IB_MAX                8
#define IB_UP                0
#define IB_DOWN                1
#define IB_RIGHT            2
#define IB_LEFT                3
#define IB_FIRE1            4 /* jump */
#define IB_FIRE2            5
#define IB_FIRE3            6 /* pause */
#define IB_FIRE4            7 /* quit */

/* input routines */
typedef struct {
    int state[IB_MAX], oldstate[IB_MAX]; /* states */
    int x, y; /* mouse-related, cursor position */
    int dx, dy; /* mouse-related, |dx|<=128, |dy|<=128 */
    int type; /* keyboard, mouse or whatever */
    int keybmap[IB_MAX]; /* keyboard-related, key mappings */
    int enabled; /* enable input? */
} input_t;

void input_init();
void input_update();
void input_release();
input_t *input_create_computer(); /* computer-controlled "input" */
input_t *input_create_keyboard(int keybmap[]);
input_t *input_create_mouse();
void input_destroy(input_t *in);
int input_button_down(input_t *in, int button);
int input_button_pressed(input_t *in, int button);
int input_button_up(input_t *in, int button);
void input_simulate_button_down(input_t *in, int button);
void input_ignore(input_t *in);
void input_restore(input_t *in);
int input_is_ignored(input_t *in);
void input_clear(input_t *in);

#endif
