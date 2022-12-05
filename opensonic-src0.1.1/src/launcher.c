/*
 * launcher.c - a very simple launcher program
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
#include <ctype.h>
#include "global.h"
#include "osspec.h"
#include "lexgui.h"


/* functions */
static void quit(void);
static int play(int id);
static int find(int id);
static char *clone_string(char *str);
static int d_update_proc(int msg, DIALOG *d, int c);
static int d_group_proc(int msg, DIALOG *d, int c);
static int d_cbitmap_proc(int msg, DIALOG *d, int c);


/* globals :( */
static volatile int gshutdown = FALSE;
static int fullscreen;
static int improve_gfx;
static int fast2x;
static int custom_level;
static int disable_music;
static char levelpath[1024] = "";

/* dialog */
#define LAUNCHER_BACKGROUND             0
#define LAUNCHER_LOGO                   2
#define LAUNCHER_GAMEVERSION            3
#define LAUNCHER_WEBSITE                4
#define LAUNCHER_FULLSCREEN             6
#define LAUNCHER_WINDOWED               7
#define LAUNCHER_FAST2X                 9
#define LAUNCHER_IMPROVESPEED           10
#define LAUNCHER_BETTERGRAPHICS         11
#define LAUNCHER_CUSTOMLEVEL            13
#define LAUNCHER_LEVELPATH              14
#define LAUNCHER_DISABLEMUSIC           17
static DIALOG launcher[] = 
{
    /* dialog_proc x y w h fg bg key flags d1 d2 dp dp2 dp3 */
    { d_clear_proc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_update_proc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_cbitmap_proc, 160, 5, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_rtext_proc, 315, 265, 0, 0, 0, -1, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_ctext_proc, 160, 50, 0, 0, 0, -1, 0, 0, 0, 0, GAME_WEBSITE, NULL, NULL },

    { d_group_proc, 5, 78, 120, 72, 0, 0, 0, 0, 0, 0, "Display mode", NULL, NULL },
    { lex_radio_proc, 10, 88, 100, 20, 0, -1, 0, D_SELECTED, 1, 1, "Fullscreen", NULL, NULL },
    { lex_radio_proc, 10, 108, 100, 20, 0, -1, 0, 0, 1, 1, "Windowed", NULL, NULL },

    { d_group_proc, 130, 78, 185, 72, 0, 0, 0, 0, 0, 0, "Performance", NULL, NULL },
    { lex_radio_proc, 135, 88, 160, 20, 0, -1, 0, D_SELECTED, 2, 1, "Normal", NULL, NULL },
    { lex_radio_proc, 135, 108, 160, 20, 0, -1, 0, 0, 2, 1, "Improve speed", NULL, NULL },
    { lex_radio_proc, 135, 128, 160, 20, 0, -1, 0, 0, 2, 1, "Improve graphics", NULL, NULL },

    { d_group_proc, 5, 160, 310, 50, 0, -1, 0, 0, 0, 0, "Custom level", NULL, NULL },
    { lex_check_proc, 10, 170, 180, 15, 0, -1, 0, 0, 1, 0, "Play a custom level", NULL, NULL },
    { lex_edit_proc, 10, 190, 210, 15, 0, -1, 0, 0, 255, 0, levelpath, NULL, NULL },
    { lex_button_proc, 230, 190, 70, 15, 0, -1, 0, D_EXIT, 0, 0, "Find...", find, NULL },

    { d_group_proc, 5, 220, 310, 30, 0, -1, 0, 0, 0, 0, "Other options", NULL, NULL },
    { lex_check_proc, 10, 230, 120, 15, 0, -1, 0, 0, 1, 0, "Disable music", NULL, NULL },

    { lex_button_proc, 135, 255, 50, 20, 0, -1, 0, D_EXIT, 0, 0, "Play!", play, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL }
};







/*
 * main()
 * Main function
 */
int main(int argc, char *argv[])
{
    BITMAP *logo;
    char version[64], abs_path[1024];
    int bgcolor;

    /* initializing */
    allegro_init();
    install_timer();
    install_keyboard();
    install_mouse();
    set_color_depth(16);
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 280, 0, 0) < 0) {
        allegro_message("FATAL ERROR: couldn't start the launcher. Impossible to set_gfx_mode(): %s", allegro_error);
        return 1;
    }
    sprintf(version, "v%d.%d.%d", GAME_VERSION, GAME_SUB_VERSION, GAME_WIP_VERSION);
    set_window_title(GAME_TITLE);
    set_close_button_callback(quit);
    init_os_specifics();

    /* let's do some testing... */
    absolute_filepath(abs_path, "gui/logo.tga", sizeof(abs_path));
    if(NULL == (logo=load_bitmap(abs_path, NULL))) {
        allegro_message("FATAL ERROR: couldn't load \"%s\". Please reinstall the game.", abs_path);
        return 1;
    }

    absolute_filepath(abs_path, "gui/gui.dat", sizeof(abs_path));
    if(!lex_load_skin(abs_path)) {
        allegro_message("FATAL ERROR: lex_load_skin(\"%s\") error.", abs_path);
        destroy_bitmap(logo);
        return 1;
    }
    
    /* gui stuff */
    bgcolor = makecol(247,247,247);
    gui_bg_color = bgcolor;
    launcher[LAUNCHER_BACKGROUND].bg = bgcolor;
    launcher[LAUNCHER_FULLSCREEN].bg = bgcolor;
    launcher[LAUNCHER_WINDOWED].bg = bgcolor;
    launcher[LAUNCHER_FAST2X].bg = bgcolor;
    launcher[LAUNCHER_IMPROVESPEED].bg = bgcolor;
    launcher[LAUNCHER_BETTERGRAPHICS].bg = bgcolor;
    launcher[LAUNCHER_CUSTOMLEVEL].bg = bgcolor;
    launcher[LAUNCHER_DISABLEMUSIC].bg = bgcolor;
    launcher[LAUNCHER_WEBSITE].fg = makecol(0,0,160);
    launcher[LAUNCHER_GAMEVERSION].fg = makecol(128,128,128);
    launcher[LAUNCHER_LOGO].dp = logo;
    launcher[LAUNCHER_GAMEVERSION].dp = version;
    /*lex_do_dialog(launcher, -1, buf, render); */ /* file_select_ex() doesn't work. */
    do_dialog(launcher, -1);

    /* bye */
    destroy_bitmap(logo);
    lex_gui_shutdown();
    return 0;
}
END_OF_MAIN()


/*
 * quit()
 * This procedure closes the launcher
 */
void quit(void)
{
    gshutdown = TRUE;
}


/*
 * play()
 * Starts the game...
 */
int play(int id)
{
    char exec[256];
    char *argv[20];
    int i, argc = 0;

    /* validating */
    if(custom_level && !exists(levelpath)) {
        if(1 == alert("This custom level file does NOT", "exist! Do you want to play the", "usual game instead?", "Yes", "No", 0, 0))
            custom_level = FALSE;
        else {
            alert("Please select a valid file.", "", "", "OK", NULL, 0, 0);
            return find(id);
        }
    }

    /* game path */
    get_executable_name(exec, sizeof(exec));
    replace_filename(exec, exec, GAME_UNIXNAME, sizeof(exec));

    /* filling the argv[] array */
    argv[argc++] = clone_string(exec);
    if(!fullscreen) argv[argc++] = clone_string("--windowed");
    if(improve_gfx) argv[argc++] = clone_string("--2xsai");
    if(fast2x) argv[argc++] = clone_string("--fast2x");
    if(disable_music) argv[argc++] = clone_string("--nomusic");
    if(custom_level){
        argv[argc++] = clone_string("--level");
        argv[argc++] = clone_string(levelpath);
    }

    /* starting the game */
    create_process(exec, argc, argv);

    /* done! */
    for(i=0; i<argc; i++)
        free(argv[i]);

    return D_CLOSE;
}

/*
 * find()
 * Opens the find dialog
 */
int find(int id)
{
    char buf[1024];

    if(!custom_level) {
        char msg[3][1024] = {"Please check the box", "", "to play on a specific level."};
        sprintf(msg[1], "\"%s\" if you want", (char*)launcher[LAUNCHER_CUSTOMLEVEL].dp);
        alert(msg[0], msg[1], msg[2], "&OK", NULL, 'o', 0);
        return D_O_K;
    }

#ifndef __WIN32__
    if(alert("Please select a level folder. If", "you're not sure, choose \"Official\"", "(see the docs for more information)", "&Official", "&Home", 'o', 'h') == 1)
        absolute_filepath(buf, "levels/", sizeof(buf));
    else
        home_filepath(buf, "levels/", sizeof(buf));
#else
    absolute_filepath(buf, "levels/", sizeof(buf));
#endif

    if(file_select_ex("Custom level", buf, "lev;/-h", 255, (int)(SCREEN_W*0.7), (int)(SCREEN_H*0.9))) {
        strcpy(levelpath, buf);
        object_message(&launcher[LAUNCHER_LEVELPATH], MSG_DRAW, 0);
    }

    return D_O_K;
}

/*
 * clone_string()
 * Clones a string
 */
char *clone_string(char *str)
{
    char *p = malloc( (strlen(str)+1) * sizeof(char) );

    if(p)
        strcpy(p, str);
    else {
        allegro_message("FATAL ERROR: clone_string() - out of memory!");
        exit(1);
    }

    return p;
}



/*
 * d_update_proc()
 * Integrate some state variables with the GUI
 */
int d_update_proc(int msg, DIALOG *d, int c)
{
    fast2x = launcher[LAUNCHER_FAST2X].flags & D_SELECTED;
    fullscreen = launcher[LAUNCHER_FULLSCREEN].flags & D_SELECTED;
    improve_gfx = launcher[LAUNCHER_BETTERGRAPHICS].flags & D_SELECTED;
    custom_level = launcher[LAUNCHER_CUSTOMLEVEL].flags & D_SELECTED;
    disable_music = launcher[LAUNCHER_DISABLEMUSIC].flags & D_SELECTED;

    return gshutdown ? D_CLOSE : D_O_K;
}

/*
 * d_group_proc()
 * Group of objects
 */
int d_group_proc(int msg, DIALOG *d, int c)
{
    if(msg == MSG_DRAW) {
        int fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
        BITMAP *gui_bmp = gui_get_screen();
        d->bg = makecol(247,247,247);
        rectfill(gui_bmp, d->x+1, d->y+1, d->x+d->w-2, d->y+d->h-2, d->bg);
        rect(gui_bmp, d->x, d->y, d->x+d->w-1, d->y+d->h-1, fg);
        gui_textout_ex(gui_bmp, d->dp, d->x+5, d->y-4, fg, d->bg, FALSE);
    }

    return D_O_K;
}

/*
 * d_cbitmap_proc()
 * Draws a bitmap centered at d->x
 */
int d_cbitmap_proc(int msg, DIALOG *d, int c)
{
    if(msg == MSG_DRAW) {
        BITMAP *b = (BITMAP*)d->dp;
        masked_blit(b, gui_get_screen(), 0, 0, d->x - b->w/2, d->y, b->w, b->h);
    }

    return D_O_K;
}
