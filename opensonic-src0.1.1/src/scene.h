/*
 * scene.h - scene management
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

#ifndef _SCENE_H
#define _SCENE_H


#define STORYBOARD_CAPACITY         255 /* up to 255 scenes */
#define SCENE_INTRO                    0
#define SCENE_LEVEL                 1
#define SCENE_PAUSE                    2
#define SCENE_GAMEOVER              3
#define SCENE_QUEST                 4
#define SCENE_ENDOFDEMO             5
#define SCENE_MENU                  6
#define SCENE_QUESTOVER             7


/* Scenes */
typedef struct {
    void (*init)();
    void (*update)();
    void (*render)();
    void (*release)();
} scene_t;

scene_t *scene_create(void (*init_func)(), void (*update_func)(), void (*render_func)(), void (*release_func)());
void scene_destroy(scene_t *scn);


/* Scene stack */
void scenestack_init();
void scenestack_release();
void scenestack_push(scene_t *scn);
void scenestack_pop();
scene_t *scenestack_top();
int scenestack_empty();


/* Storyboard */
extern scene_t *storyboard[STORYBOARD_CAPACITY];


#endif
