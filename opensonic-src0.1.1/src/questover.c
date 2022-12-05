/*
 * questover.c - quest over scene
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

#include <string.h>
#include "questover.h"
#include "scene.h"
#include "util.h"
#include "video.h"
#include "font.h"
#include "audio.h"
#include "quest.h"
#include "actor.h"
#include "player.h"
#include "input.h"


/* private data */
static uint32 starttime;
static font_t *fnt, *title;
static actor_t *sonic;
static input_t *input;
static int quit;

/* public functions */

/*
 * questover_init()
 * Initializes the scene
 */
void questover_init()
{
    starttime = game_timer();
    quit = FALSE;

    fnt = font_create(8);
    fnt->position = v2d_new(5, 35);

    title = font_create(4);
    font_set_text(title, "QUEST CLEARED");
    title->position = v2d_new( (VIDEO_SCREEN_W - (int)font_get_charsize(title).x*strlen(font_get_text(title)))/2 , 5 );

    sonic = actor_create();
    actor_change_animation(sonic, sprite_get_animation("SD_SONIC", 24));
    sonic->position = v2d_new(20, 150);

    music_play( music_load("musics/endofquest.it") , 0);
    input = input_create_keyboard(NULL);
    fadefx_in(image_rgb(0,0,0), 2.0);
}


/*
 * questover_release()
 * Releases the scene
 */
void questover_release()
{
    input_destroy(input);
    actor_destroy(sonic);
    font_destroy(title);
    font_destroy(fnt);
}


/*
 * questover_update()
 * Updates the scene
 */
void questover_update()
{
    uint32 now = game_timer();
    char *name = quest_getname();
    int score = player_get_score();
    int time_h = quest_getvalue(QUESTVALUE_TOTALTIME)/3600;
    int time_m = (int)(quest_getvalue(QUESTVALUE_TOTALTIME)/60) % 60;
    int time_s = (int)(quest_getvalue(QUESTVALUE_TOTALTIME)) % 60;
    int glasses = quest_getvalue(QUESTVALUE_GLASSES);
    int bigrings = quest_getvalue(QUESTVALUE_BIGRINGS);
    char txt[] =
        "Congratulations!\n"
        "\"%s\"\n"
        "is now cleared!\n\n\n\n"

        "     Total score: %d\n\n"
        "     Total time: %dh %dm %ds\n\n"
        "     Magic glasses: %d\n\n"
        "     Big rings: %d\n\n\n\n"

        "Thank you for playing. Wait for\n"
        "new versions of %s!\n\n"
        "Visit us on the web: $PLAYER_SONIC\n"
        "%s\n"
        "Press ENTER to restart the game."
    ;

    font_set_text(fnt, txt, name, score, time_h, time_m, time_s, glasses, bigrings, GAME_TITLE, GAME_WEBSITE);

    if(input_button_pressed(input, IB_FIRE3) && now >= starttime + 3000) {
        music_stop();
        quit = TRUE;
    }

    if(quit) {
        if(fadefx_over()) {
            scenestack_pop();
            scenestack_push( storyboard[SCENE_MENU] );
            return;
        }
        fadefx_out(image_rgb(0,0,0), 2.0);
    }

}



/*
 * questover_render()
 * Renders the scene
 */
void questover_render()
{
    v2d_t cam = v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2);
    image_clear(video_buffer, image_rgb(0,0,0));
    font_render(title, cam);
    font_render(fnt, cam);
    actor_render(sonic, cam);
}
