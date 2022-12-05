/*
 * endofdemo.c - end of demo scene
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

#include <math.h>
#include <string.h>
#include "endofdemo.h"
#include "scene.h"
#include "util.h"
#include "video.h"
#include "font.h"
#include "actor.h"


/* private data */
#define SCENE_TIMEOUT 30000
#define RING_MAX 10
static uint32 starttime;
static font_t *fnt, *title;
static actor_t *ring[RING_MAX];

/* public functions */

/*
 * endofdemo_init()
 * Initializes the scene
 */
void endofdemo_init()
{
    int i;

    starttime = game_timer();

    fnt = font_create(8);
    fnt->position = v2d_new(5, 35);

    title = font_create(4);
    font_set_text(title, "THANK YOU");
    title->position = v2d_new( (VIDEO_SCREEN_W - (int)font_get_charsize(title).x*strlen(font_get_text(title)))/2 , 5 );

    for(i=0; i<RING_MAX; i++) {
        ring[i] = actor_create();
        ring[i]->spawn_point = ring[i]->position = v2d_new( VIDEO_SCREEN_W*i/RING_MAX+15 , 200);
        actor_change_animation(ring[i], sprite_get_animation(i%2==1 ? "SD_BLUERING" : "SD_RING", 0));
    }

    fadefx_in(image_rgb(0,0,0), 2.0);
}


/*
 * endofdemo_release()
 * Releases the scene
 */
void endofdemo_release()
{
    int i;

    for(i=0; i<RING_MAX; i++)
        actor_destroy(ring[i]);

    font_destroy(title);
    font_destroy(fnt);
}


/*
 * endofdemo_update()
 * Updates the scene
 */
void endofdemo_update()
{
    uint32 now = game_timer();
    int i, sec = max(0, (SCENE_TIMEOUT - (int)(now-starttime)) / 1000);
    char txt[] =
        "Thank you for playing %s!\n"
        "I hope you enjoyed this game as\n"
        "much as I did making it! :)\n\n"

        "This is the end of this demo,\n"
        "but not the end of the game.\n"
        "Now you should try to clear the\n"
        "Master Quest, a true challenge!\n\n"

        "This game is still in development.\n"
        "Check out for new versions at:\n"
        "%s\n\n"

        "Please wait %d seconds..."
    ;

    font_set_text(fnt, txt, GAME_TITLE, GAME_WEBSITE, sec);

    if(now >= starttime + SCENE_TIMEOUT) {
        if(fadefx_over()) {
            scenestack_pop();
            scenestack_push(storyboard[SCENE_QUESTOVER]);
            return;
        }
        fadefx_out(image_rgb(0,0,0), 2.0);
    }

    for(i=0; i<RING_MAX; i++)
        ring[i]->position.y = ring[i]->spawn_point.y + 20*sin(PI * (now*0.001) + (2*PI/RING_MAX)*i);
}



/*
 * endofdemo_render()
 * Renders the scene
 */
void endofdemo_render()
{
    int i;
    v2d_t cam = v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2);

    image_clear(video_buffer, image_rgb(0,0,0));

    for(i=0; i<RING_MAX; i++)
        actor_render(ring[i], cam);

    font_render(title, cam);
    font_render(fnt, cam);
}
