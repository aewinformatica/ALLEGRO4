/*
 * quest.c - quest module
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include "util.h"
#include "quest.h"
#include "level.h"
#include "scene.h"



/* private data */
#define QUESTIMAGE_WIDTH 100
#define QUESTIMAGE_HEIGHT 75
static quest_t* current_quest;
static int current_level;
static int abort_quest;
static double questvalue[QUESTVALUE_MAX];
static char lastname[512] = "NO_QUEST_NAME";


/* private functions */
static image_t *load_quest_image(char *file);
static void read_quest_line(quest_t *qst, char *line);








/* quest methods */



/*
 * load_quest()
 * Loads and returns a quest from a file
 * (abs_path must be an ABSOLUTE path)
 */
quest_t *load_quest(char *abs_path)
{
    FILE *fp;
    char line[2][1024];
    quest_t *q = mallocx(sizeof *q);

    logfile_message("load_quest('%s')", abs_path);

    /* default values */
    str_cpy(q->file, abs_path, sizeof(q->file));
    strcpy(q->name, "null");
    strcpy(q->author, "null");
    strcpy(q->version, "null");
    strcpy(q->description, "null");
    q->image = NULL;
    q->level_count = 0;
    q->show_ending = FALSE;

    /* reading from file */
    if(NULL != (fp=fopen(q->file, "r"))) {
        logfile_message("Reading quest \"%s\"...", q->file);
        while( fgets(line[0], 1024, fp) ) {
            str_trim(line[1], line[0]);
            read_quest_line(q, line[1]);
        }
        fclose(fp);
        logfile_message("ok!");
    }
    else
        fatal_error("FATAL ERROR: couldn't open quest file \"%s\" for reading.", q->file);

    /* done! :] */
    if(!q->image)
        q->image = load_quest_image(NULL);

    return q;
}

/*
 * load_default_quest()
 * Loads the default quest
 */
quest_t *load_default_quest()
{
    quest_t *q = mallocx(sizeof *q);
    char lev[][128] = {
        "levels/blue_ocean_1.lev",
        "levels/blue_ocean_2.lev",
        "levels/blue_ocean_3.lev",
        "levels/exotic_1.lev",
        "levels/exotic_2.lev",
        "levels/exotic_3.lev",

        /* end of list */
        ""
    };
    int i;

    /* default values */
    strcpy(q->file, "\201");
    strcpy(q->name, "Default quest");
    strcpy(q->author, "Alexandre Martins");
    sprintf(q->version, "%d.%d.%d", GAME_VERSION, GAME_SUB_VERSION, GAME_WIP_VERSION);
    strcpy(q->description, "The default quest of Open Sonic");
    q->image = load_quest_image(NULL);
    q->level_count = 0;
    q->show_ending = TRUE;

    /* reading level data */
    logfile_message("Reading default quest...");
    for(i=0; lev[i][0] != 0; i++,q->level_count++)
        strcpy(q->level_path[i], lev[i]);
    logfile_message("ok!");

    /* done! :] */
    return q;
}



/*
 * unload_quest()
 * Unloads a quest
 */
void unload_quest(quest_t *qst)
{
    image_destroy(qst->image);
    free(qst);
}




/* quest values */

/*
 * quest_setvalue()
 * Sets a new value to a quest value
 *
 * key = QUESTVALUE_*
 * value = new value
 */
void quest_setvalue(int key, double value)
{
    key = clip(key, 0, QUESTVALUE_MAX-1);
    questvalue[key] = value;
}


/*
 * quest_getvalue()
 * Returns a quest value
 */
double quest_getvalue(int key)
{
    key = clip(key, 0, QUESTVALUE_MAX-1);
    return questvalue[key];
}







/* public scene functions */

/*
 * quest_init()
 * Initializes the quest scene. Remember to load
 * some quest before running this scene!
 */
void quest_init()
{
    int i;

    abort_quest = FALSE;
    for(i=0; i<QUESTVALUE_MAX; i++)
        questvalue[i] = 0;
}


/*
 * quest_release()
 * Releases the quest scene
 */
void quest_release()
{
    unload_quest(current_quest);
}


/*
 * quest_render()
 * Actually, this function does nothing
 */
void quest_render()
{
}


/*
 * quest_update()
 * Updates the quest manager
 */
void quest_update()
{
    /* invalid quest */
    if(current_quest->level_count == 0) {
        logfile_message("Quest '%s' has no levels.", current_quest->file);
        game_quit();
        return;
    }

    /* quest manager */
    if(current_level < current_quest->level_count && !abort_quest) {
        /* next level... */
        level_setfile(current_quest->level_path[current_level]);
        scenestack_push( storyboard[SCENE_LEVEL] );
        current_level++;
    }
    else {
        /* end of the quest! */
        scenestack_pop();
        scenestack_push( storyboard[abort_quest ? SCENE_MENU : (current_quest->show_ending ? SCENE_ENDOFDEMO : SCENE_QUESTOVER)] );
        return;
    }
}


/*
 * quest_run()
 * Executes the given quest
 */
void quest_run(quest_t *qst)
{
    current_quest = qst;
    strcpy(lastname, qst->name);
    player_set_lives(PLAYER_INITIAL_LIVES);
    player_set_score(0);
    logfile_message("Running quest %s, '%s'...", qst->file, qst->name);
    quest_setlevel(0);
}


/*
 * quest_setlevel()
 * Jumps to the given level, 0 <= lev < n
 */
void quest_setlevel(int lev)
{
    current_level = max(0, lev);
}

/*
 * quest_abort()
 * Aborts the current quest
 */
void quest_abort()
{
    logfile_message("Quest aborted!");
    abort_quest = TRUE;
}


/*
 * quest_getname()
 * Returns the name of the last running quest
 */
char *quest_getname()
{
    return lastname;
}



/* private functions */


/* returns the quest image */
image_t *load_quest_image(char *file)
{
    char no_image[] = "images/null.png";
    image_t *ret, *img = image_load(file ? file : no_image);
    if(!img) img = image_load(no_image);

    ret = image_create(QUESTIMAGE_WIDTH, QUESTIMAGE_HEIGHT);
    image_blit(img, ret, 0, 0, 0, 0, ret->w, ret->h);
    return ret;
}

/* reads and interprets a line from a .qst file */
void read_quest_line(quest_t *qst, char *line)
{
    char cmd[1024], *args;
    int n;

    /* reading data */
    if( EOF == sscanf(line, "%1023s %n", cmd, &n) ) return;
    args = line+n;

    /* which command? */
    if(str_icmp(cmd, "name") == 0)
        str_cpy(qst->name, args, sizeof(qst->name));
    else if(str_icmp(cmd, "author") == 0)
        str_cpy(qst->author, args, sizeof(qst->author));
    else if(str_icmp(cmd, "version") == 0)
        str_cpy(qst->version, args, sizeof(qst->version));
    else if(str_icmp(cmd, "description") == 0)
        str_cpy(qst->description, args, sizeof(qst->description));
    else if(str_icmp(cmd, "image") == 0 && !qst->image)
        qst->image = load_quest_image(args);
    else if(str_icmp(cmd, "level") == 0 && qst->level_count < QUEST_MAXLEVELS) {
        str_cpy(qst->level_path[qst->level_count], args, sizeof(qst->level_path[qst->level_count]));
        qst->level_count++;
    }
}
