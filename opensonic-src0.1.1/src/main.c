/*
 * main.c - main file of the game
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
#include <alpng.h>
#include <string.h>
#include "util.h"
#include "scene.h"
#include "video.h"
#include "audio.h"
#include "input.h"
#include "actor.h"
#include "brick.h"
#include "player.h"
#include "item.h"
#include "enemy.h"
#include "level.h"
#include "quest.h"
#include "menu.h"
#include "intro.h"
#include "endofdemo.h"
#include "questover.h"
#include "screenshot.h"


/* command line arguments */
typedef struct {
    /* video stuff */
    int use_2xsai;
    int use_supereagle;
    int use_fast2x;
    int fullscreen; /* fullscreen mode? */
    int color_depth; /* bits per pixel */

    /* run custom level */
    int custom_level; /* user needs to run a custom level? */
    char custom_level_path[1024]; /* filepath */

    /* other */
    int disable_music; /* disable music? */
} cmdline_t;

static cmdline_t cmdline_read(int argc, char **argv);



/* game control */
#define MIN_FRAME_INTERVAL 10 /* (1/10) * 1000 = 100 fps max */
#define MAX_FRAME_INTERVAL 20 /* (1/20) * 1000 =  50 fps min */
extern volatile int game_over, fps;
extern volatile uint32 game_elapsed;
extern double game_dt;
static int partial_fps;
static uint32 fps_accum;
extern void game_update_timer();








/*
 * main()
 * Main function
 */
int main(int argc, char *argv[])
{
    int i, video_mode, screen_w, screen_h;
    int double_size;
    char window_title[128];
    uint32 last_time, current_time, delta_time;
    scene_t *scn;
    cmdline_t cmd;



    /* Initializing Allegro */
    allegro_init();
    install_timer();
    install_keyboard();
    install_mouse();


    /* helpers */
    randomize();
    init_os_specifics();
    logfile_init();
    cmd = cmdline_read(argc, argv);


    /* initializing the game window... */
    double_size = (cmd.use_2xsai || cmd.use_supereagle || cmd.use_fast2x);
    set_color_depth(cmd.color_depth);
    video_mode = cmd.fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED;
    screen_w = (double_size?2:1) * VIDEO_SCREEN_W;
    screen_h = (double_size?2:1) * VIDEO_SCREEN_H;
    if(set_gfx_mode(video_mode, screen_w, screen_h, 0, 0) < 0) {
        logfile_message("Unable to set gfx mode.\n%s\nTrying GFX_AUTODETECT...", allegro_error);
        if(set_gfx_mode(GFX_AUTODETECT, VIDEO_SCREEN_W, VIDEO_SCREEN_H, 0, 0) < 0) {
            logfile_message("Unable to set gfx mode.\n%s\n", allegro_error);
            allegro_message("Unable to set gfx mode.\n%s\n", allegro_error);
            return 1;
        }
    }

    game_elapsed = 0;
    partial_fps = 0;
    fps_accum = 0;
    LOCK_VARIABLE(game_elapsed);
    LOCK_FUNCTION(game_update_timer);
    install_int(game_update_timer, 10);
    LOCK_FUNCTION(game_quit);
    set_close_button_callback(game_quit);
    sprintf(window_title, GAME_WINDOWTITLE, GAME_TITLE, GAME_VERSION, GAME_SUB_VERSION, GAME_WIP_VERSION);
    set_window_title(window_title);

    alpng_init();


    /* Initializing managers */
    audio_init(cmd.disable_music);
    hash_init();
    video_init(cmd.use_2xsai, cmd.use_supereagle, cmd.use_fast2x);
    input_init();


    /* screenshots module */
    screenshot_init();


    /* Storyboard */
    for(i=0; i<STORYBOARD_CAPACITY; i++)
        storyboard[i] = NULL;
    storyboard[SCENE_LEVEL] = scene_create(level_init, level_update, level_render, level_release);
    storyboard[SCENE_PAUSE] = scene_create(pause_init, pause_update, pause_render, pause_release);
    storyboard[SCENE_GAMEOVER] = scene_create(gameover_init, gameover_update, gameover_render, gameover_release);
    storyboard[SCENE_QUEST] = scene_create(quest_init, quest_update, quest_render, quest_release);
    storyboard[SCENE_MENU] = scene_create(menu_init, menu_update, menu_render, menu_release);
    storyboard[SCENE_INTRO] = scene_create(intro_init, intro_update, intro_render, intro_release);
    storyboard[SCENE_ENDOFDEMO] = scene_create(endofdemo_init, endofdemo_update, endofdemo_render, endofdemo_release);
    storyboard[SCENE_QUESTOVER] = scene_create(questover_init, questover_update, questover_render, questover_release);


    /* Initializing game info... */
    game_over = FALSE;
    scenestack_init();
    player_set_lives(PLAYER_INITIAL_LIVES);
    player_set_score(0);


    /* initial scene */
    if(!cmd.custom_level) {
        scenestack_push( storyboard[SCENE_INTRO] );
    }
    else {
        level_setfile(cmd.custom_level_path);
        scenestack_push( storyboard[SCENE_LEVEL] );
    }


    /* Main loop */
    while(!game_over && !scenestack_empty()) {
        last_time = game_timer();

        /* updating managers */
        input_update();
        audio_update();

        /* current scene */
        scn = scenestack_top();
        scn->update();

        /* rendering */
        if(scn == scenestack_top())
            scn->render();
        screenshot_update();
        video_render();
        partial_fps++;

        /* time control */
        for(delta_time = 0; delta_time < MIN_FRAME_INTERVAL; ) {
            current_time = game_timer();
            delta_time = (current_time > last_time) ? (current_time - last_time) : 0;
            last_time = (current_time >= last_time) ? last_time : current_time;
        }
        delta_time = min(delta_time, MAX_FRAME_INTERVAL);
        game_dt = (double)delta_time * 0.001;

        /* FPS (frames per second) */
        fps_accum += delta_time;
        if(fps_accum >= 1000) {
            fps = partial_fps;
            partial_fps = 0;
            fps_accum = 0;
        }

    }
    scenestack_release();


    /* releasing the screenshot module */
    screenshot_release();


    /* Releasing managers */
    input_release();
    video_release();
    hash_release();
    audio_release();
    logfile_release();


    /* end of storyboard... */
    for(i=0; i<STORYBOARD_CAPACITY; i++) {
        if(storyboard[i])
            scene_destroy(storyboard[i]);
    }


    /* bye */
    allegro_exit();
    return 0;
}
END_OF_MAIN()




/*
 * cmdline_read()
 * Reads the command line arguments
 */
cmdline_t cmdline_read(int argc, char **argv)
{
    int i, default_bpp = VIDEO_DEFAULT_BPP;
    cmdline_t cmd;

    /* default values */
    cmd.use_2xsai = FALSE;
    cmd.use_supereagle = FALSE;
    cmd.use_fast2x = FALSE;
    cmd.fullscreen = TRUE;
    cmd.color_depth = default_bpp;
    cmd.disable_music = FALSE;
    cmd.custom_level = FALSE;
    cmd.custom_level_path[0] = '\0';

    /* logfile */
    logfile_message("game arguments:");
    for(i=0; i<argc; i++)
        logfile_message("argv[%d]: '%s'", i, argv[i]);

    /* reading data... */
    for(i=1; i<argc; i++) {

        if(str_icmp(argv[i], "--help") == 0) {
            printf(
                "%s usage:\n"
                "    %s [options ...]\n"
                "\n"
                "where options include:\n"
                "    --help                    displays this message\n"
                "    --version                 shows the version of this program\n"
                "    --fast2x                  doubles the size of the game window\n"
                "    --2xsai                   doubles the size of the game window and improves the graphic quality (*)\n"
                "    --supereagle              doubles the size of the game window and improves the graphic quality (**)\n"
                "    --windowed                windowed mode\n"
                "    --level \"FILEPATH\"        runs the level located at FILEPATH\n"
                "    --nomusic                 disables all the musics\n"
                "\n"
                "(*) That option may be used to improve the graphic quality using the 2xSaI algorithm.\n"
                "    You should NOT use this option on slow computers, since it may imply a severe performance hit.\n"
                "\n"
                "(**) Same as above, but uses the SuperEagle algorithm.\n"
                "\n"
                "Please read the docs for more information.\n",
            GAME_TITLE, GAME_UNIXNAME);
            exit(0);
        }

        else if(str_icmp(argv[i], "--version") == 0) {
            printf("%s %d.%d.%d\n", GAME_TITLE, GAME_VERSION, GAME_SUB_VERSION, GAME_WIP_VERSION);
            exit(0);
        }

        else if(str_icmp(argv[i], "--2xsai") == 0)
            cmd.use_2xsai = TRUE;

        else if(str_icmp(argv[i], "--supereagle") == 0)
            cmd.use_supereagle = TRUE;

        else if(str_icmp(argv[i], "--fast2x") == 0)
            cmd.use_fast2x = TRUE;

        else if(str_icmp(argv[i], "--windowed") == 0)
            cmd.fullscreen = FALSE;

        /* TODO: 32 bpp doesn't work properly at this time */
        /*else if(str_icmp(argv[i], "--color-depth") == 0) {
            if(++i < argc) {
                cmd.color_depth = atoi(argv[i]);
                if(cmd.color_depth != 16 && cmd.color_depth != 32) {
                    printf("WARNING: invalid color depth (%d). Changing to %d...\n", cmd.color_depth, default_bpp);
                    cmd.color_depth = default_bpp;
                }
            }
        }*/

        else if(str_icmp(argv[i], "--level") == 0) {
            if(++i < argc) {
                cmd.custom_level = TRUE;
                resource_filepath(cmd.custom_level_path, argv[i], sizeof(cmd.custom_level_path), RESFP_READ);
                if(!filepath_exists(cmd.custom_level_path))
                    fatal_error("FATAL ERROR: file '%s' does not exist!\n", cmd.custom_level_path);
            }
        }

        else if(str_icmp(argv[i], "--nomusic") == 0)
            cmd.disable_music = TRUE;

        else { /* unknown option */
            printf("%s: bad command line option \"%s\".\nRun %s --help to get more information.\n", GAME_UNIXNAME, argv[i], GAME_UNIXNAME);
            exit(0);
        }

    }

    /* done! */
    return cmd;
}
