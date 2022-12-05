/*
 * input.c - input management
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

#include <allegro.h>
#include "input.h"
#include "util.h"
#include "video.h"


/* input constants */
#define IT_KEYBOARD            0
#define IT_MOUSE            1
#define IT_COMPUTER         2
#define MOUSE_XSPEED        2
#define MOUSE_YSPEED        2


/* input routines */
struct input_t_list{
    input_t *data;
    struct input_t_list *next;
};
static struct input_t_list *inlist;

static void input_register(input_t *in)
{
    struct input_t_list *node = mallocx(sizeof *node);

    node->data = in;
    node->next = inlist;
    inlist = node;
}


static void input_unregister(input_t *in)
{
    struct input_t_list *node, *next;

    if(inlist->data == in) {
        next = inlist->next;
        free(inlist);
        inlist = next;
    }
    else {
        node = inlist;
        while(node->next && node->next->data != in) node = node->next;
        if(node->next) {
            next = node->next->next; 
            free(node->next);
            node->next = next;
        }
    }
}


/*
 * input_init()
 * Initializes the input module
 * */
void input_init()
{
    logfile_message("input_init()");
    inlist = NULL;
}

/*
 * input_update()
 * Updates all the registered input objects
 */
void input_update()
{
    struct input_t_list *it;
    int i, lock_mouse = FALSE;

    /* polling devices */
    if(keyboard_needs_poll())
        poll_keyboard();

    if(mouse_needs_poll())
        poll_mouse();

    /* updating input objects */
    for(it = inlist; it; it=it->next) {

        for(i=0; i<IB_MAX; i++)
            it->data->oldstate[i] = it->data->state[i];

        switch(it->data->type) {
            case IT_KEYBOARD:
                for(i=0; i<IB_MAX; i++)
                    it->data->state[i] = key[ it->data->keybmap[i] ];
                break;

            case IT_MOUSE:
                get_mouse_mickeys(&it->data->dx, &it->data->dy);
                it->data->x = mouse_x;
                it->data->y = mouse_y;
                it->data->dx *= (double)128/(double)MOUSE_XSPEED;
                it->data->dy *= (double)128/(double)MOUSE_YSPEED;
                it->data->state[IB_UP] = (it->data->dy <= -32);
                it->data->state[IB_DOWN] = (it->data->dy >= 32);
                it->data->state[IB_LEFT] = (it->data->dx <= -32);
                it->data->state[IB_RIGHT] = (it->data->dx >= 32);
                it->data->state[IB_FIRE1] = (mouse_b & 1);
                it->data->state[IB_FIRE2] = (mouse_b & 2);
                it->data->state[IB_FIRE3] = (mouse_b & 4);
                it->data->state[IB_FIRE4] = key[KEY_ESC];
                /*lock_mouse = TRUE;*/
                break;

            case IT_COMPUTER:
                for(i=0; i<IB_MAX; i++)
                    it->data->state[i] = FALSE;
                break;
        }
    }

    /* lock mouse? */
    if(lock_mouse && video_is_window_active())
        position_mouse(SCREEN_W/2, SCREEN_H/2);

    /* quit game */
    if(key[KEY_ALT] && key[KEY_F4])
        game_quit();
}


/*
 * input_release()
 * Releases the input module
 */
void input_release()
{
    struct input_t_list *it, *next;

    logfile_message("input_release()");
    for(it = inlist; it; it=next) {
        next = it->next;
        free(it->data);
        free(it);
    }
}


/*
 * input_button_down()
 * Checks if a given button is down
 */
int input_button_down(input_t *in, int button)
{
    return in->enabled ? in->state[button] : FALSE;
}


/*
 * input_button_pressed()
 * Checks if a given button is pressed, not holded
 */
int input_button_pressed(input_t *in, int button)
{
    return in->enabled ? (in->state[button] && !in->oldstate[button]) : FALSE;
}


/*
 * input_button_up()
 * Checks if a given button is up
 */
int input_button_up(input_t *in, int button)
{
    return in->enabled ? (!in->state[button] && in->oldstate[button]) : FALSE;
}



/*
 * input_create_keyboard()
 * Creates an input object based on the keyboard
 *
 * keybmap: array of IB_MAX integers. Use NULL
 *          to use the default settings.
 */
input_t *input_create_keyboard(int keybmap[])
{
    input_t *in = mallocx(sizeof *in);
    int i;

    in->type = IT_KEYBOARD;
    in->enabled = TRUE;
    in->dx = in->dy = in->x = in->y = 0;
    for(i=0; i<IB_MAX; i++)
        in->state[i] = in->oldstate[i] = FALSE;

    if(keybmap) {
        /* custom keyboard map */
        for(i=0; i<IB_MAX; i++)
            in->keybmap[i] = keybmap[i];
    }
    else {
        /* default settings */
        in->keybmap[IB_UP] = KEY_UP;
        in->keybmap[IB_DOWN] = KEY_DOWN;
        in->keybmap[IB_RIGHT] = KEY_RIGHT;
        in->keybmap[IB_LEFT] = KEY_LEFT;
        in->keybmap[IB_FIRE1] = KEY_SPACE;
        in->keybmap[IB_FIRE2] = KEY_LCONTROL;
        in->keybmap[IB_FIRE3] = KEY_ENTER;
        in->keybmap[IB_FIRE4] = KEY_ESC;
    }

    input_register(in);
    return in;
}




/* 
 * input_create_mouse()
 * Creates an input object based on the mouse
 */
input_t *input_create_mouse()
{
    input_t *in = mallocx(sizeof *in);
    int i;

    in->type = IT_MOUSE;
    in->enabled = TRUE;
    in->dx = in->dy = in->x = in->y = 0;
    for(i=0; i<IB_MAX; i++)
        in->state[i] = in->oldstate[i] = FALSE;

    input_register(in);
    return in;
}




/*
 * input_create_computer()
 * Creates an object that receives "input" from
 * the computer
 */
input_t *input_create_computer()
{
    input_t *in = mallocx(sizeof *in);
    int i;

    in->type = IT_COMPUTER;
    in->enabled = TRUE;
    in->dx = in->dy = in->x = in->y = 0;
    for(i=0; i<IB_MAX; i++)
        in->state[i] = in->oldstate[i] = FALSE;

    input_register(in);
    return in;
}



/*
 * input_destroy()
 * Destroys an input object
 */
void input_destroy(input_t *in)
{
    input_unregister(in);
    free(in);
}


/*
 * input_ignore()
 * Ignore Control
 */
void input_ignore(input_t *in)
{
    in->enabled = FALSE;
}


/*
 * input_restore()
 * Restore Control
 */
void input_restore(input_t *in)
{
    in->enabled = TRUE;
}



/*
 * input_is_ignored()
 * Returns TRUE if the input is ignored,
 * or FALSE otherwise
 */
int input_is_ignored(input_t *in)
{
    return !in->enabled;
}




/*
 * input_clear()
 * Clears all the input buttons
 */
void input_clear(input_t *in)
{
    int i;
    for(i=0; i<IB_MAX; i++)
        in->state[i] = in->oldstate[i] = FALSE;
}



/*
 * input_simulate_button_down()
 * Useful for computer-controlled input objects
 */
void input_simulate_button_down(input_t *in, int button)
{
    in->state[button] = TRUE;
}
