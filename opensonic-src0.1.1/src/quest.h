/*
 * quest.h - quest module
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

#ifndef _QUEST_H
#define _QUEST_H

#include "video.h"



/* quest structure */
#define QUEST_MAXLEVELS 128
typedef struct {
    /* meta data */
    char file[1024]; /* file (absolute path) */
    char name[256]; /* quest name */
    char author[256]; /* author */
    char version[64]; /* version string */
    char description[1024]; /* description */
    image_t *image; /* thumbnail */
    int show_ending; /* shows the ending scene when this quest gets over */

    /* quest data */
    int level_count; /* how many levels? */
    char level_path[QUEST_MAXLEVELS][96]; /* relative paths of the levels */
} quest_t;

quest_t *load_quest(char *abs_path);
quest_t *load_default_quest();
void unload_quest(quest_t *qst);


/* quest values (player's values) */
#define QUESTVALUE_MAX          3
#define QUESTVALUE_TOTALTIME    0 /* total quest time, in seconds */
#define QUESTVALUE_BIGRINGS     1 /* how many bigrings the player has got so far? */
#define QUESTVALUE_GLASSES      2 /* how many glasses the player has got so far? */
void quest_setvalue(int key, double value);
double quest_getvalue(int key);



/* public scene functions */
/* call quest_run() before executing this scene! */
void quest_init();
void quest_update();
void quest_render();
void quest_release();
void quest_run(quest_t *qst); /* executes a quest */
void quest_setlevel(int lev); /* jumps to the given level (0..n-1) */
void quest_abort(); /* aborts the current quest */
char *quest_getname(); /* returns the name of the last running quest */



#endif
