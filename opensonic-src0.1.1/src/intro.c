/*
 * intro.c - introduction scene
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

#include "intro.h"
#include "scene.h"
#include "util.h"
#include "video.h"


/* private data */
static image_t *bg;
static uint32 starttime;


/* public functions */

/*
 * intro_init()
 * Initializes the introduction scene
 */
void intro_init()
{
    starttime = game_timer();
    bg = image_load("images/intro.png");
    fadefx_in(image_rgb(0,0,0), 1.0);
}


/*
 * intro_release()
 * Releases the introduction scene
 */
void intro_release()
{
}


/*
 * intro_update()
 * Updates the introduction scene
 */
void intro_update()
{
    if(game_timer() >= starttime + 4000) {
        if(fadefx_over()) {
            scenestack_pop();
            scenestack_push(storyboard[SCENE_MENU]);
            return;
        }
        fadefx_out(image_rgb(0,0,0), 1.0);
    }
}



/*
 * intro_render()
 * Renders the introduction scene
 */
void intro_render()
{
    image_blit(bg, video_buffer, 0, 0, 0, 0, bg->w, bg->h);
}
