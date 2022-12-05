/*
 * menu.c - menu scene
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "global.h"
#include "menu.h"
#include "util.h"
#include "scene.h"
#include "video.h"
#include "actor.h"
#include "font.h"
#include "audio.h"
#include "quest.h"



/* private data */

/* menu screens */
#define MENU_MAIN 0 /* main menu */
#define MENU_QUEST 1 /* custom quests */
static int menu_screen; /* SCREEN_* */
static input_t *input;
static scene_t *jump_to;



/* background effect */
#define BG_MAXLINES 13
static image_t *bgline[BG_MAXLINES];
static double bgline_xpos[BG_MAXLINES];


/* main menu */
#define MENU_MAXOPTIONS 4
static char menu[MENU_MAXOPTIONS][32] = { "1P GAME", "CUSTOM QUESTS", "TUTORIAL", "EXIT" };
static int menuopt; /* current option */
static font_t *menufnt[MENU_MAXOPTIONS][2]; /* 0. white ; 1. yellow */
static actor_t *menufoot;
static int sonic_entering;
static actor_t *sonic, *sonicbg, *gametitle;
static font_t *website, *credit, *version;


/* quest menu */
#define MENU_QUESTSPERPAGE 14
static font_t *qstselect[2], *qstdetail;
static int qstmenuopt;
static int qstcount;
static font_t **qstfnt; /* vector of font_t* */
static quest_t **qstdata; /* vector of quest_t* */



/* private functions */
static void select_option(int opt);
static void load_quest_list();
static void release_quest_list();
static int dirfill(const char *filename, int attrib, void *param);
static int dircount(const char *filename, int attrib, void *param);
static int sort_cmp(const void *a, const void *b);
static int qstmenuopt_getpage(int val);
static int qstmenuopt_getmaxpages();
static void game_start(quest_t *q);






/* public functions */

/*
 * menu_init()
 * Initializes the menu
 */
void menu_init()
{
    int i, j, height = (int)(VIDEO_SCREEN_H / BG_MAXLINES);
    image_t *sheet = image_load("images/sourcecode.png");

    /* initializing... */
    menu_screen = MENU_MAIN;
    jump_to = NULL;
    input = input_create_keyboard(NULL);
    input_ignore(input);
    load_quest_list();
    music_play( music_load("musics/title.it") , MUSIC_LOOPFOREVER);


    /* background init */
    for(i=0; i<BG_MAXLINES; i++) {
        bgline_xpos[i] = 0;
        bgline[i] = image_create( sheet->w , height );
        image_blit(sheet, bgline[i], 0, height*i, 0, 0, sheet->w, height);
    }


    /* main actors */
    sonic_entering = TRUE;

    sonic = actor_create();
    actor_change_animation(sonic, sprite_get_animation("SD_TITLESONIC", 0));
    sonic->position.x = (VIDEO_SCREEN_W-actor_image(sonic)->w)/2 + 5;
    sonic->position.y = -15;

    sonicbg = actor_create();
    actor_change_animation(sonicbg, sprite_get_animation("SD_TITLESONICBG", 0));
    sonicbg->position.x = (VIDEO_SCREEN_W-actor_image(sonicbg)->w)/2;
    sonicbg->position.y = sonic->position.y+25;

    gametitle = actor_create();
    actor_change_animation(gametitle, sprite_get_animation("SD_TITLEGAMENAME", 0));
    gametitle->position.x = (VIDEO_SCREEN_W-actor_image(gametitle)->w)/2;
    gametitle->position.y = sonic->position.y+actor_image(sonic)->h-9;

    website = font_create(8);
    website->position = v2d_new(3, VIDEO_SCREEN_H-24);
    font_set_text(website, "%s", GAME_WEBSITE);
    credit = font_create(8);
    credit->position = v2d_new(3, VIDEO_SCREEN_H-12);
    font_set_text(credit, "by Alexandre Martins      2008-2009");
    version = font_create(0);
    version->position = v2d_new(VIDEO_SCREEN_W-75, 3);
    font_set_text(version, "FREEWARE\nV%d.%d.%d", GAME_VERSION, GAME_SUB_VERSION, GAME_WIP_VERSION);

    /* main menu */
    menuopt = 0;
    menufoot = actor_create();
    actor_change_animation(menufoot, sprite_get_animation("SD_TITLEFOOT", 0));
    for(i=0; i<2; i++) {
        for(j=0; j<MENU_MAXOPTIONS; j++) {
            menufnt[j][i] = font_create(i);
            menufnt[j][i]->position = v2d_new(112, gametitle->position.y+60+10*j);
            font_set_text(menufnt[j][i], menu[j]);
        }
    }

    /* quest menu */
    qstselect[0] = font_create(8);
    qstselect[0]->position = v2d_new(3, 3);

    qstselect[1] = font_create(8);
    qstselect[1]->position = v2d_new(3, VIDEO_SCREEN_H-15);

    qstdetail = font_create(8);
    qstdetail->position = v2d_new(5, 170);


    /* fade in */
    fadefx_in(image_rgb(0,0,0), 1.0);
}


/*
 * menu_update()
 * Updates the menu
 */
void menu_update()
{
    int i;
    double dt = game_delta();
    double t = game_timer()*0.001;

    /* game start */
    if(jump_to != NULL && fadefx_over()) {
        scenestack_pop();
        scenestack_push(jump_to);
        return;
    }

    /* background movement */
    for(i=0; i<BG_MAXLINES; i++) {
        bgline_xpos[i] -= (50.0 * (1.0 + 0.2*i)) * dt;
        if(bgline_xpos[i] <= -bgline[i]->w)
            bgline_xpos[i] += bgline[i]->w;
    }


    /* menu programming */
    switch(menu_screen) {

        /* ------ main menu ------ */
        case MENU_MAIN:
        {
            /* sonic & stuff */
            if(sonic_entering && actor_animation_finished(sonic)) {
                sonic_entering = FALSE;
                actor_change_animation(sonic, sprite_get_animation("SD_TITLESONIC", 1));
                input_restore(input);
            }
            gametitle->visible = !sonic_entering;

            /* quit? */
            /*if(input_button_pressed(input, IB_FIRE4)) {
                game_quit();
                return;
            }*/

            /* current option */
            menufoot->position.x = menufnt[menuopt][0]->position.x - 20 + 3*cos(2*PI * t);
            menufoot->position.y = menufnt[menuopt][0]->position.y;

            if(input_button_pressed(input, IB_UP)) {
                sound_play( sound_load("samples/choose.wav") );
                menuopt--;
            }
            if(input_button_pressed(input, IB_DOWN)) {
                sound_play( sound_load("samples/choose.wav") );
                menuopt++;
            }
            menuopt = (menuopt%MENU_MAXOPTIONS + MENU_MAXOPTIONS) % MENU_MAXOPTIONS;

            if(input_button_pressed(input, IB_FIRE1) || input_button_pressed(input, IB_FIRE3)) {
                sound_play( sound_load("samples/select.wav") );
                select_option(menuopt);
                return;
            }

            break;
        }





        /* ------ quest menu ----- */
        case MENU_QUEST:
        {
            /* go back to the main menu */
            if(input_button_pressed(input, IB_FIRE4)) {
                sound_play( sound_load("samples/return.wav") );
                menu_screen = MENU_MAIN;
            }

            /* font position */
            for(i=0; i<qstcount; i++) {
                qstfnt[i]->position = v2d_new(30, 20 + 10*(i%MENU_QUESTSPERPAGE));
                qstfnt[i]->visible = (qstmenuopt_getpage(i) == qstmenuopt_getpage(qstmenuopt));
            }

            /* selected option? */
            menufoot->position.x = 10;
            menufoot->position.y = qstfnt[qstmenuopt]->position.y;

            if(input_button_pressed(input, IB_UP)) {
                sound_play( sound_load("samples/choose.wav") );
                qstmenuopt--;
            }
            if(input_button_pressed(input, IB_DOWN)) {
                sound_play( sound_load("samples/choose.wav") );
                qstmenuopt++;
            }
            qstmenuopt = (qstmenuopt%qstcount + qstcount) % qstcount;

            /* quest details */
            font_set_text(qstselect[0], "Please select a quest: [page %d/%d]", qstmenuopt_getpage(qstmenuopt), qstmenuopt_getmaxpages());
            font_set_text(qstselect[1], "Press ESC to go back.");
            font_set_text(qstdetail, "Quest info: (version %s)\n\nTitle:  %s\nAuthor: %s\n%s", qstdata[qstmenuopt]->version, qstdata[qstmenuopt]->name, qstdata[qstmenuopt]->author, qstdata[qstmenuopt]->description);

            /* game start! */
            if(input_button_pressed(input, IB_FIRE1) || input_button_pressed(input, IB_FIRE3)) {
                quest_t *clone = load_quest(qstdata[qstmenuopt]->file);
                sound_play( sound_load("samples/select.wav") );
                game_start(clone);
                return;
            }

            break;
        }
    }
}


/*
 * menu_render()
 * Renders the menu
 */
void menu_render()
{
    int i;
    v2d_t camera = v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2);

    /* background */
    image_clear(video_buffer, image_rgb(0,0,0));
    for(i=0; i<BG_MAXLINES; i++) {
        image_blit(bgline[i], video_buffer, 0, 0, bgline_xpos[i], bgline[i]->h*i, bgline[i]->w, bgline[i]->h);
        image_blit(bgline[i], video_buffer, 0, 0, bgline_xpos[i]+bgline[i]->w, bgline[i]->h*i, bgline[i]->w, bgline[i]->h);
    }

    /* rendering menus... :) */
    switch(menu_screen) {
        /* ----- main menu ----- */
        case MENU_MAIN:
        {
            /* menu */
            for(i=0; i<MENU_MAXOPTIONS; i++)
                font_render(menufnt[i][i == menuopt ? 1 : 0], camera);
            actor_render(menufoot, camera);

            /* sonic & stuff */
            font_render(website, camera);
            font_render(credit, camera);
            font_render(version, camera);
            actor_render(sonicbg, camera);
            if(sonic_entering) image_clear(video_buffer, image_rgb(0,0,0));
            actor_render(sonic, camera);
            actor_render(gametitle, camera);
            break;
        }

        /* ----- quest menu ----- */
        case MENU_QUEST:
        {
            /* quest details */
            image_t *thumb = qstdata[qstmenuopt]->image;
            font_render(qstdetail, camera);
            image_blit(thumb, video_buffer, 0, 0, VIDEO_SCREEN_W - thumb->w - 5, (int)qstfnt[0]->position.y, thumb->w, thumb->h);

            /* texts */
            font_render(qstselect[0], camera);
            font_render(qstselect[1], camera);
            for(i=0; i<qstcount; i++)
                font_render(qstfnt[i], camera);
            actor_render(menufoot, camera);
            break;
        }
    }
}



/*
 * menu_release()
 * Releases the menu
 */
void menu_release()
{
    int i, j;

    /* no more music... */
    music_stop();

    /* main menu stuff */
    font_destroy(website);
    font_destroy(credit);
    font_destroy(version);
    for(i=0; i<2; i++) {
        for(j=0; j<MENU_MAXOPTIONS; j++)
            font_destroy(menufnt[j][i]);
    }
    actor_destroy(sonicbg);
    actor_destroy(gametitle);
    actor_destroy(sonic);

    /* quest menu */
    font_destroy(qstselect[0]);
    font_destroy(qstselect[1]);
    font_destroy(qstdetail);

    /* background */
    for(i=0; i<BG_MAXLINES; i++)
        image_destroy(bgline[i]);

    /* misc */
    actor_destroy(menufoot);
    input_destroy(input);
    release_quest_list();
}








/* private functions */


/* the player selected some option at the main menu */
void select_option(int opt)
{
    switch(opt) {
        /* 1P GAME */
        case 0:
            game_start( load_default_quest() );
            return;

        /* CUSTOM QUESTS */
        case 1:
            menu_screen = MENU_QUEST;
            qstmenuopt = 0;
            break;

        /* TUTORIAL */
        case 2:
            game_start( load_quest("quests/tutorial.qst") );
            return;

        /* EXIT */
        case 3:
            game_quit();
            return;
    }
}



/* reads the quest list from the quest/ folder */
void load_quest_list()
{
    int i, j, deny_flags = FA_DIREC | FA_LABEL, c = 0;
    int max_paths;
    char path[] = "quests/*.qst";
    char abs_path[2][1024];

    logfile_message("load_quest_list()");

    /* official and $HOME quests */
    absolute_filepath(abs_path[0], path, sizeof(abs_path[0]));
    home_filepath(abs_path[1], path, sizeof(abs_path[1]));
    max_paths = (strcmp(abs_path[0], abs_path[1]) == 0) ? 1 : 2;

    /* loading quest data */
    qstcount = 0;
    for(j=0; j<max_paths; j++)
        for_each_file_ex(abs_path[j], 0, deny_flags, dircount, NULL);

    qstdata = mallocx(qstcount * sizeof(quest_t*));
    for(j=0; j<max_paths; j++)
        for_each_file_ex(abs_path[j], 0, deny_flags, dirfill, (void*)&c);
    qsort(qstdata, qstcount, sizeof(quest_t*), sort_cmp);

    /* fatal error */
    if(qstcount == 0)
        fatal_error("FATAL ERROR: no quests found! Please reinstall the game.");
    else
        logfile_message("%d quests found.", qstcount);

    /* other stuff */
    qstfnt = mallocx(qstcount * sizeof(font_t*));
    for(i=0; i<qstcount; i++) {
        qstfnt[i] = font_create(8);
        font_set_text(qstfnt[i], "%2d %s", i+1, qstdata[i]->name);
    }
}

int dirfill(const char *filename, int attrib, void *param)
{
    int *c = (int*)param;
    qstdata[ (*c)++ ] = load_quest((char*)filename);
    return 0;
}

int dircount(const char *filename, int attrib, void *param)
{
    qstcount++;
    return 0;
}

/* releases the quest list */
void release_quest_list()
{
    int i;

    logfile_message("release_quest_list()");

    for(i=0; i<qstcount; i++) {
        unload_quest(qstdata[i]);
        font_destroy(qstfnt[i]);
    }

    free(qstdata);
    free(qstfnt);
    qstcount = 0;
}


/* comparator */
int sort_cmp(const void *a, const void *b)
{
    quest_t *q[2] = { *((quest_t**)a), *((quest_t**)b) };
    return (int)( file_time( (const char*)(q[1]->file) ) - file_time( (const char*)(q[0]->file) ) );
}


/* returns a page number... */
int qstmenuopt_getpage(int val)
{
    return val / MENU_QUESTSPERPAGE + 1;
}

/* how many pages there are? */
int qstmenuopt_getmaxpages()
{
    return qstcount / MENU_QUESTSPERPAGE + 1;
}


/* closes the menu and starts the game. Call return after this. */
void game_start(quest_t *q)
{
    quest_run(q);
    jump_to = storyboard[SCENE_QUEST];
    input_ignore(input);
    fadefx_out(image_rgb(0,0,0), 0.5);
}
