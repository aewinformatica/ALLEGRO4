/*
 * level.c - code for the game levels
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
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "global.h"
#include "input.h"
#include "scene.h"
#include "video.h"
#include "audio.h"
#include "sprite.h"
#include "util.h"
#include "brick.h"
#include "player.h"
#include "item.h"
#include "enemy.h"
#include "level.h"
#include "font.h"
#include "quest.h"
#include "boss.h"


/* camera */
struct {
    v2d_t position;
    v2d_t dest;
    double speed;
} camera;
static actor_t *camera_focus;
static void camera_init(v2d_t pos);
static void camera_update();
static void camera_release();
static void camera_move_to(v2d_t newpos, double seconds);
static void camera_teleport(v2d_t newpos);


/* backgrounds */
#define BG_MAX                  50
#define BGBEHAVIOR_MAXARGS      5

/* bg behavior list */
#define BGB_DEFAULT             0
#define BGB_CIRCULAR            1
#define BGB_LINEAR              2

typedef struct {
    spriteinfo_t *data; /* this is not stored in the main hash */
    actor_t *actor;
    double x, y; /* useful -> circular behavior */
    int repeat_x, repeat_y;
    int behavior;
    double behavior_arg[BGBEHAVIOR_MAXARGS];
} background_t;

static int background_count; /* how many bgs? */
static background_t *background[BG_MAX]; /* bg data */

static void background_load(char *filename); /* loads the bg data from a file (background theme) */
static void background_unload(); /* unloads all the background data at level_unload() */
static void background_update(); /* update backgrounds */
static void background_render(); /* render backgrounds */
static background_t *read_background(FILE *fp); /* reads a bg definition from a file */



/* particles */
typedef struct {
    image_t *image;
    v2d_t position;
    v2d_t speed;
    int destroy_on_brick;
} particle_t;

typedef struct particle_list_t {
    particle_t *data;
    struct particle_list_t *next;
} particle_list_t;

static void particle_init();
static void particle_release();
static void particle_update_all(brick_list_t *brick_list);
static void particle_render_all();



/* dialog regions (regions that if the player comes inside, a dialog box appears) */
#define DIALOGREGION_MAX 100
typedef struct {
    int rect_x, rect_y, rect_w, rect_h; /* region data */
    char title[128], message[1024]; /* dialog box data */
    int disabled; /* TRUE if this region is disabled */
} dialogregion_t;

static int dialogregion_size; /* size of the vector */
static dialogregion_t dialogregion[DIALOGREGION_MAX];
static dialogregion_t read_dialogregion(char *line);
static char* write_dialogregion(dialogregion_t dlg);
static void update_dialogregions();


/* boss stuff */
static boss_t *boss;
static int player_inside_boss_area;
static int boss_fight_activated;
static int got_boss(); /* does this level have a boss? */

/* private data */
#define MAX_ANIMALS 12
static char file[1024], name[1024], musicfile[1024], theme[1024], bgtheme[1024];
static char author[1024], version[1024];
static int act, requires[3];
static brick_list_t *brick_list;
static double gravity;
static int level_width, level_height; /* dimensions of this level (in pixels) */
static item_list_t *item_list;
static enemy_list_t *enemy_list;
static particle_list_t *particle_list;
static double level_timer;
static v2d_t spawn_point;
static music_t *music;
static sound_t *override_music;
static int block_music;

/* level editor */
#define EDITOR_GRID 1
static int editor; /* we're running the level editor? */
static int editor_locked; /* we can't open the level editor */
static input_t *mouse, *keyboard;
static v2d_t editor_camera, editor_cursor;
static enum { EDT_BRICK, EDT_ITEM, EDT_ENEMY } editor_cursor_objtype;
static int editor_cursor_objid, editor_cursor_itemid;
static font_t *editor_cursor_font;
static char* editor_object_category(int id);
static char* editor_object_info(int objtype, int objid);

/* level editor item list */
static int index_editor_item_list(int item_id);
static int editor_item_list_size; /* counted automatically */
static int editor_item_list[] = {
    IT_RING, IT_LIFEBOX, IT_RINGBOX, IT_STARBOX, IT_SPEEDBOX, IT_GLASSESBOX,
    IT_SHIELDBOX, IT_EGGMANBOX, IT_LOOPRIGHT, IT_LOOPMIDDLE, IT_LOOPLEFT,
    IT_LOOPNONE, IT_YELLOWSPRING, IT_REDSPRING, IT_RREDSPRING, IT_LREDSPRING,
    IT_BLUERING, IT_SWITCH, IT_DOOR, IT_TELEPORTER, IT_BIGRING, IT_CHECKPOINT,
    IT_GOAL, IT_ENDSIGN, IT_ENDLEVEL, IT_LOOPFLOOR, IT_LOOPFLOORNONE,
    IT_LOOPFLOORTOP, IT_BUMPER, IT_DANGER, IT_SPIKES, IT_DNADOOR,
    -1 /* -1 represents the end of this list */
};

/* moveable platforms */
static void brick_move(brick_t *brick);

/* player data */
static player_t *team[3];
static player_t *player; /* current player */
static int player_id; /* current player id (0, 1 or 2) */


/* gui / hud */
#define MAX_POWERUPS 10
static actor_t *maingui, *lifegui;
static font_t *lifefnt, *mainfnt[3];
static font_t *fps;
static void render_powerups();


/* end of act (reached the goal) */
#define ACTCLEAR_BONUSMAX 3 /* ring bonus, secret bonus, total */
static int level_cleared;
static uint32 actclear_starttime, actclear_endtime, actclear_sampletimer;
static int actclear_prepare_next_level, actclear_goto_next_level, actclear_played_song;
static double actclear_ringbonus, actclear_secretbonus, actclear_totalbonus;
static font_t *actclear_teamname, *actclear_gotthrough, *actclear_bonusfnt[ACTCLEAR_BONUSMAX];
static actor_t *actclear_levelact, *actclear_bonus[ACTCLEAR_BONUSMAX];


/* opening animation */
static actor_t *levelop, *levelact;
static font_t *leveltitle;


/* dialog box */
#define DLGBOX_MAXTIME          7000
static int dlgbox_active;
static uint32 dlgbox_starttime;
static actor_t *dlgbox;
static font_t *dlgbox_title, *dlgbox_message;
static void update_dlgbox();
static void render_dlgbox();


/* private functions */
#define DEFAULT_MARGIN          VIDEO_SCREEN_W
#define WORDWRAP_MINLENGTH      6
#define WORDWRAP_MAXLENGTH      11
static int inside_screen(int x, int y, int w, int h, int margin);
static brick_list_t* brick_list_clip();
static item_list_t* item_list_clip();
static void brick_list_unclip(brick_list_t *list);
static void item_list_unclip(item_list_t *list);
static int get_brick_id(brick_t *b);
static void insert_brick_sorted(brick_list_t *b);
static brick_t *create_fake_brick(int width, int height, v2d_t position);
static void destroy_fake_brick(brick_t *b);
static void update_level_size();
static void restart();
static void render_players(int bring_to_back);
static void update_music();
static void spawn_players();
static void remove_dead_bricks();





/* level-specific functions */

/*
 * level_load()
 * Loads a level from some file
 */
void level_load(char *filepath)
{
    FILE *fp;
    char line[1024], word[64], *p;
    char abs_path[1024];
    int r, n;

    /* searching the level file... */
    resource_filepath(abs_path, filepath, sizeof(abs_path), RESFP_READ);

    /* default values */
    strcpy(name, "Untitled");
    strcpy(musicfile, "");
    strcpy(theme, "");
    strcpy(bgtheme, "");
    strcpy(author, "");
    strcpy(version, "");
    str_cpy(file, abs_path, sizeof(file));
    spawn_point = v2d_new(0,0);
    background_count = 0;
    dialogregion_size = 0;
    boss = NULL;
    act = 1;
    requires[0] = GAME_VERSION;
    requires[1] = GAME_SUB_VERSION;
    requires[2] = GAME_WIP_VERSION;
    editor_locked = FALSE;


    /* loads the file level */
    logfile_message("level_load(%s)", abs_path);
    if(NULL == (fp=fopen(abs_path, "r"))) {
        logfile_message("Warning: could not load level %s", abs_path);
        return;
    }


    /* reading data */
    while(fgets(line, 1024, fp)) {
        p=line; r=sscanf(p, "%63s %n", word, &n); p+=n;
        if(r > 0 && word[0] != '#') {

            /* let's read a command */

            if(strcmp(word, "theme") == 0) {
                /* set theme */
                str_trim(theme, p);
                brickdata_load(theme);
            }
            else if(strcmp(word, "bgtheme") == 0) {
                /* set background theme */
                str_trim(bgtheme, p);
                background_load(bgtheme);
            }
            else if(strcmp(word, "music") == 0) {
                /* set music file */
                str_trim(musicfile, p);
            }
            else if(strcmp(word, "name") == 0) {
                /* level name */
                str_trim(name, p);
            }
            else if(strcmp(word, "author") == 0) {
                /* author */
                str_trim(author, p);
            }
            else if(strcmp(word, "version") == 0) {
                /* version */
                str_trim(version, p);
            }
            else if(strcmp(word, "requires") == 0) {
                /* this level requires at least which game version? */
                int i;
                sscanf(p, "%d.%d.%d", &requires[0], &requires[1], &requires[2]);
                for(i=0; i<3; i++) requires[i] = clip(requires[i], 0, 99);
                if(requires[0]*10000 + requires[1]*100 + requires[2] > GAME_VERSION*10000 + GAME_SUB_VERSION*100 + GAME_WIP_VERSION)
                    fatal_error(
                        "This level requires %s version %d.%d.%d or greater.\nCheck our for new versions at %s",
                        GAME_TITLE, requires[0], requires[1], requires[2], GAME_WEBSITE
                    );
            }
            else if(strcmp(word, "act") == 0) {
                /* level act */
                sscanf(p, "%d", &act);
                act = clip(act, 1, 3);
            }
            else if(strcmp(word, "spawn_point") == 0) {
                /* spawn point */
                int x, y;
                sscanf(p, "%d %d", &x, &y);
                spawn_point = v2d_new(x,y);
            }
            else if(strcmp(word, "boss") == 0) {
                /* boss */
                int type, x, y, rx, ry, rw, rh;
                if(!got_boss()) {
                    sscanf(p, "%d %d %d %d %d %d %d", &type, &x, &y, &rx, &ry, &rw, &rh);
                    boss = boss_create(type, v2d_new(x,y), rx, ry, rw, rh);
                }
            }
            else if(strcmp(word, "dialogbox") == 0) {
                /* dialog region */
                dialogregion[dialogregion_size++] = read_dialogregion(line);
            }
            else if(strcmp(word, "readonly") == 0) {
                /* is this level 'read only' ? */
                editor_locked = TRUE;
            }
            else if(strcmp(word, "brick") == 0) {
                /* add brick */
                int brick_id, x, y;
                if(strcmp(theme,"") != 0) {
                    sscanf(p, "%d %d %d", &brick_id, &x, &y);
                    brick_id = clip(brick_id, 0, brickdata_size()-1);
                    level_create_brick(brick_id, v2d_new(x,y));
                }
                else
                    logfile_message("level_load() warning: cannot create new brick if the theme is not defined");
            }
            else if(strcmp(word, "item") == 0) {
                /* add item */
                int item_type, x, y;
                sscanf(p, "%d %d %d", &item_type, &x, &y);
                item_type = clip(item_type, 0, ITEMDATA_MAX-1);
                level_create_item(item_type, v2d_new(x,y));
            }
            else if(strcmp(word, "enemy") == 0) {
                /* add enemy */
                int enemy_type, x, y;
                sscanf(p, "%d %d %d", &enemy_type, &x, &y);
                enemy_type = clip(enemy_type, 0, ENEMYDATA_MAX-1);
                level_create_enemy(enemy_type, v2d_new(x,y));
            }
        }
    }



    fclose(fp);

    /* misc */
    music = music_load(musicfile);
    if(music) music_play(music, MUSIC_LOOPFOREVER);
    block_music = FALSE;
    update_level_size();

    logfile_message("level_load() ok");
}


/*
 * level_unload()
 * Call manually after level_load() whenever
 * this level has to be released or changed
 */
void level_unload()
{
    brick_list_t *node, *next;
    item_list_t *inode, *inext;
    enemy_list_t *enode, *enext;

    logfile_message("level_unload()");
    music_stop();

    /* clears the brick_list */
    logfile_message("releasing brick list...");
    for(node=brick_list; node; node=next) {
        next = node->next;
        free(node->data);
        free(node);
    }
    brick_list = NULL;

    /* clears the item list */
    logfile_message("releasing item list...");
    for(inode=item_list; inode; inode=inext) {
        inext = inode->next;
        item_destroy(inode->data);
        free(inode);
    }
    item_list = NULL;

    /* clears the enemy list */
    logfile_message("releasing enemy list...");
    for(enode=enemy_list; enode; enode=enext) {
        enext = enode->next;
        enemy_destroy(enode->data);
        free(enode);
    }
    enemy_list = NULL;

    /* releasing the boss */
    if(got_boss()) {
        logfile_message("releasing the boss...");
        boss_destroy(boss);
        boss = NULL;
    }

    /* unloading brickdata and backgrounds */
    logfile_message("unloading brickdata...");
    brickdata_unload();

    logfile_message("unloading backgrounds...");
    background_unload();

    logfile_message("level_unload() ok");
}


/*
 * level_save()
 * Saves the current level to a file
 */
void level_save(char *filepath)
{
    int i;
    FILE *fp;
    char abs_path[1024];
    brick_list_t *itb;
    item_list_t *iti;
    enemy_list_t *ite;

    resource_filepath(abs_path, filepath, sizeof(abs_path), RESFP_WRITE);

    /* open for writing */
    logfile_message("level_save(%s)", abs_path);
    if(NULL == (fp=fopen(abs_path, "w"))) {
        logfile_message("Warning: could not open \"%s\" for writing.", abs_path);
        return;
    }

    /* meta information */
    fprintf(fp,
    "# ------------------------------------------------------------\n"
    "# %s %d.%d.%d level\n"
    "# This file is human-readable (umm, maybe not :). Make sure to\n"
    "# check out the level editor!\n"
    "# ------------------------------------------------------------\n"
    "\n"
    "# header\n"
    "name %s\n"
    "author %s\n"
    "version %s\n"
    "requires %d.%d.%d\n"
    "act %d\n"
    "theme %s\n"
    "bgtheme %s\n"
    "spawn_point %d %d\n"
    "music %s\n",
    GAME_TITLE, GAME_VERSION, GAME_SUB_VERSION, GAME_WIP_VERSION,
    name, author, version, requires[0], requires[1], requires[2], act, theme, bgtheme,
    (int)spawn_point.x, (int)spawn_point.y, musicfile);

    /* boss? */
    if(got_boss())
        fprintf(fp, "boss %d %d %d %d %d %d %d\n", boss->type, (int)boss->actor->spawn_point.x, (int)boss->actor->spawn_point.y, boss->rect_x, boss->rect_y, boss->rect_w, boss->rect_h);

    /* read only? */
    if(editor_locked)
        fprintf(fp, "readonly\n");

    /* dialog regions */
    fprintf(fp, "\n# dialog regions (x y w h title message)\n");
    for(i=0; i<dialogregion_size; i++)
        fprintf(fp, "%s\n", write_dialogregion( dialogregion[i] ));

    /* brick list */
    fprintf(fp, "\n# brick list\n");
    for(itb=brick_list; itb; itb=itb->next) 
        fprintf(fp, "brick %d %d %d\n", get_brick_id(itb->data), (int)itb->data->sx, (int)itb->data->sy);

    /* item list */
    fprintf(fp, "\n# item list\n");
    for(iti=item_list; iti; iti=iti->next)
        fprintf(fp, "item %d %d %d\n", iti->data->type, (int)iti->data->actor->spawn_point.x, (int)iti->data->actor->spawn_point.y);

    /* enemy list */
    fprintf(fp, "\n# enemy list\n");
    for(ite=enemy_list; ite; ite=ite->next)
        fprintf(fp, "enemy %d %d %d\n", ite->data->type, (int)ite->data->actor->spawn_point.x, (int)ite->data->actor->spawn_point.y);

    /* done! */
    fprintf(fp, "\n# EOF");
    fclose(fp);
    logfile_message("level_save() ok");
}




/* scene functions */

/*
 * level_init()
 * Initializes the scene
 */
void level_init()
{
    int i;
    int editor_keybmap[] = { KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_N, KEY_B, KEY_LCONTROL, KEY_F12 };

    /* main init */
    logfile_message("level_init()");
    brick_list = NULL;
    item_list = NULL;
    gravity = 800;
    level_width = level_height = 0;
    keyboard = input_create_keyboard(editor_keybmap);
    mouse = input_create_mouse();
    editor = 0;
    level_timer = 0;
    dialogregion_size = 0;
    override_music = NULL;
    level_cleared = FALSE;
    actclear_starttime = actclear_endtime = actclear_sampletimer = 0;
    actclear_ringbonus = actclear_secretbonus = actclear_totalbonus = 0;
    actclear_prepare_next_level = actclear_goto_next_level = FALSE;
    actclear_played_song = FALSE;
    editor_item_list_size = -1;
    while(editor_item_list[++editor_item_list_size] >= 0) {}
    particle_init();

    /* level init */
    level_load(file);

    /* loading players */
    logfile_message("Creating players...");
    team[0] = player_create(PL_SONIC);
    team[1] = player_create(PL_TAILS);
    team[2] = player_create(PL_KNUCKLES);
    spawn_players();
    player_id = 0;
    player = team[player_id]; 
    camera_init(player->actor->position);
    player_set_rings(0);
    level_set_camera_focus(player->actor);
    player_inside_boss_area = FALSE;
    boss_fight_activated = FALSE;

    /* gui */
    logfile_message("Loading hud...");
    maingui = actor_create();
    maingui->position = v2d_new(5,5);
    actor_change_animation(maingui, sprite_get_animation("SD_MAINGUI", 0));
    lifegui = actor_create();
    lifegui->position = v2d_new(5, VIDEO_SCREEN_H-21);
    actor_change_animation(lifegui, sprite_get_animation("SD_LIFEGUI", 0));
    lifefnt = font_create(0);
    lifefnt->position = v2d_add(lifegui->position, v2d_new(30,11)); 
    for(i=0; i<3; i++) {
        mainfnt[i] = font_create(2);
        mainfnt[i]->position = v2d_add(maingui->position, v2d_new(50, i*16+2));
    }
    fps = font_create(0);
    fps->position = v2d_new(VIDEO_SCREEN_W-73, VIDEO_SCREEN_H-10);
    editor_cursor_font = font_create(0);

    /* level opening */
    levelop = actor_create();
    levelop->position = v2d_new(0,-240);
    actor_change_animation(levelop, sprite_get_animation("SD_LEVELOP", 0));
    levelact = actor_create();
    levelact->position = v2d_new(260,250);
    actor_change_animation(levelact, sprite_get_animation("SD_LEVELACT", act-1));
    leveltitle = font_create(3);
    leveltitle->position = v2d_new(330,50);
    font_set_text_wordwrap(leveltitle, WORDWRAP_MINLENGTH, WORDWRAP_MAXLENGTH, str_to_upper(name));

    /* end of act */
    actclear_teamname = font_create(4);
    actclear_gotthrough = font_create(7);
    actclear_levelact = actor_create();
    for(i=0; i<ACTCLEAR_BONUSMAX; i++) {
        actclear_bonusfnt[i] = font_create(2);
        actclear_bonus[i] = actor_create();
    }

    /* dialog box */
    dlgbox_active = FALSE;
    dlgbox_starttime = 0;
    dlgbox = actor_create();
    dlgbox->position.y = VIDEO_SCREEN_H;
    actor_change_animation(dlgbox, sprite_get_animation("SD_DIALOGBOX", 0));
    dlgbox_title = font_create(8);
    dlgbox_message = font_create(8);

    logfile_message("level_init() ok");
}


/*
 * level_update()
 * Updates the scene (this one runs
 * every cycle of the program)
 */
void level_update()
{
    int i, j;
    int got_dieing_player = FALSE, got_dead_brick = FALSE;
    int block_pause = FALSE;
    double dt = game_delta();
    brick_list_t *major_bricks, *fake_bricks, *bnode, *bnext;
    item_list_t *major_items, *inode, *iprev, *inext;
    enemy_list_t *enode, *eprev, *enext;

    if(!editor) {

        /* quit? */
        if(input_button_pressed(player->actor->input, IB_FIRE4)) {
            scenestack_pop();
            quest_abort();
            return;
        }

        /* open level editor */
        if(input_button_pressed(keyboard, IB_FIRE4)) {
            if(!editor_locked) {
                editor = TRUE;
                /*editor_camera = camera.position;*/
                editor_camera.x = (int)camera.position.x;
                editor_camera.y = (int)camera.position.y;
                editor_cursor = v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2);
                editor_cursor_objtype = EDT_ITEM;
                editor_cursor_objid = 0;
                video_showmessage("Current %s: %03d %s", editor_object_category(editor_cursor_objtype), editor_cursor_objid, editor_object_info(editor_cursor_objtype, editor_cursor_objid));
            }
            else {
                video_showmessage("No way!");
                sound_play( sound_load("samples/deny.wav") );
            }
        }

        /* pause game */
        block_pause = level_timer < 5; /* are we viewing opening animation? */
        for(i=0; i<3 && !block_pause; i++)
            block_pause = team[i]->dieing || team[i]->dead;
        if(input_button_pressed(player->actor->input, IB_FIRE3) && !block_pause) {
            player->spin_dash = player->braking = FALSE;
            music_pause();
            scenestack_push(storyboard[SCENE_PAUSE]);
            return;
        }




        /* gui */
        actor_change_animation(maingui, sprite_get_animation("SD_MAINGUI", player_get_rings()>0 ? 0 : 1));
        actor_change_animation(lifegui, sprite_get_animation("SD_LIFEGUI", player_id));
        font_set_text(lifefnt, "%2d", player_get_lives());
        font_set_text(mainfnt[0], "% 5d", player_get_score());
        font_set_text(mainfnt[1], "%d:%02d", (int)level_timer/60, (int)level_timer%60);
        font_set_text(mainfnt[2], "%d", player_get_rings());
        font_set_text(fps, "FPS:% 4d", game_fps());

        /* level opening */
        if(level_timer < 5) {
            if(level_timer < 1.5) {
                levelop->position.y += 360*dt;
                if(levelop->position.y > -2)
                    levelop->position.y = -2;

                leveltitle->position.x -= 320*dt;
                if(leveltitle->position.x < 140)
                    leveltitle->position.x = 140;

                levelact->position.y -= 200*dt;
                if(levelact->position.y < 200)
                    levelact->position.y = 200;
            }
            else if(level_timer > 3.5) {
                levelop->position.x -= 320*dt;
            }
        }
        else {
            levelop->visible = FALSE;
            leveltitle->visible = FALSE;
            levelact->visible = FALSE;
        }


        /* end of act (reached the goal) */
        if(level_cleared) {
            double total = 0;
            uint32 tmr = game_timer();
            sound_t *ring = sound_load("samples/ringcount.wav");
            sound_t *cash = sound_load("samples/cash.wav");
            sound_t *glasses = sound_load("samples/glasses.wav");

            /* level music fadeout */
            if(music_is_playing())
                music_set_volume(1.0 - (double)(tmr-actclear_starttime)/2000.0);

            /* show scores */
            if(tmr >= actclear_starttime + 2000) {
                /* lock characters */
                for(i=0; i<3; i++)
                    team[i]->actor->speed.x = 0;

                /* set positions... */
                actclear_teamname->position.x = min(actclear_teamname->position.x + 800*dt, 30);
                actclear_gotthrough->position.x = min(actclear_gotthrough->position.x + 700*dt, 12);
                actclear_levelact->position.x = max(actclear_levelact->position.x - 700*dt, 250);

                for(i=0; i<ACTCLEAR_BONUSMAX; i++) {
                    actclear_bonus[i]->position.x = min(actclear_bonus[i]->position.x + (400-50*i)*dt, 50);
                    actclear_bonusfnt[i]->position.x = max(actclear_bonusfnt[i]->position.x - (400-50*i)*dt, 230);
                }

                /* counters (bonus) */
                total = actclear_totalbonus - (actclear_ringbonus + actclear_secretbonus);
                font_set_text(actclear_bonusfnt[0], "%d", (int)actclear_ringbonus);
                font_set_text(actclear_bonusfnt[1], "%d", (int)actclear_secretbonus);
                font_set_text(actclear_bonusfnt[ACTCLEAR_BONUSMAX-1], "%d", (int)total);

                /* reached the goal song */
                if(!actclear_played_song) {
                    music_stop();
                    sound_play( sound_load("samples/goal.wav") );
                    actclear_played_song = TRUE;
                }
            }

            /* decreasing counters (bonus) */
            if(tmr >= actclear_starttime + 6000 && !actclear_prepare_next_level) {
                /* decreasing */
                actclear_ringbonus = max(0, actclear_ringbonus-400*dt);
                actclear_secretbonus = max(0, actclear_secretbonus-2000*dt);

                /* sound effects */
                if(actclear_ringbonus > 0 || actclear_secretbonus > 0) {
                    /* ring */
                    if(ring && tmr >= actclear_sampletimer) {
                        actclear_sampletimer = tmr+100;
                        sound_play(ring);
                    }
                }
                else {
                    /* cash */
                    if(cash) {
                        actclear_prepare_next_level = TRUE;
                        actclear_endtime = tmr + 4000;
                        sound_play(cash);
                    }

                    /* got glasses? */
                    for(i=0; i<3 && glasses; i++) {
                        if(team[i]->got_glasses) {
                            sound_play(glasses);
                            break;
                        }
                    }
                }
            }

            /* go to next level? */
            if(actclear_prepare_next_level && tmr >= actclear_endtime)
                actclear_goto_next_level = TRUE;
        }

        /* dialog box */
        update_dialogregions();
        update_dlgbox();



        major_items = item_list_clip();
        remove_dead_bricks();
        major_bricks = brick_list_clip();
        fake_bricks = NULL;

        /* update background */
        background_update();

        /* update bricks */
        for(bnode=major_bricks; bnode; bnode=bnode->next) {

            /* breakable bricks */
            if(bnode->data->brick_ref->behavior == BRB_BREAKABLE) {
                int brkw = bnode->data->brick_ref->image->w;
                int brkh = bnode->data->brick_ref->image->h;
                double a[4], b[4] = { bnode->data->x, bnode->data->y, bnode->data->x + brkw, bnode->data->y + brkh };

                for(i=0; i<3; i++) {
                    a[0] = team[i]->actor->position.x - team[i]->actor->hot_spot.x - 3;
                    a[1] = team[i]->actor->position.y - team[i]->actor->hot_spot.y - 3;
                    a[2] = a[0] + actor_image(team[i]->actor)->w + 6;
                    a[3] = a[1] + actor_image(team[i]->actor)->h + 6;

                    if((team[i]->spin_dash || team[i]->spin) && bounding_box(a,b)) {

                        /* particles */
                        for(j=0; j<9; j++) {
                            v2d_t brkpos = v2d_new(bnode->data->x + (j%3)*(brkw/3), bnode->data->y + (j/3)*(brkh/3));
                            v2d_t brkspeed = v2d_new(-team[i]->actor->speed.x*0.3, -100-random(50));
                            image_t *brkimg = image_create(brkw/3, brkh/3);

                            image_blit(bnode->data->brick_ref->image, brkimg, (j%3)*(brkw/3), (j/3)*(brkh/3), 0, 0, brkw/3, brkh/3);
                            if(fabs(brkspeed.x) > EPSILON) brkspeed.x += (brkspeed.x>0?1:-1) * random(50);
                            level_create_particle( brkimg, brkpos, brkspeed, FALSE);
                        }

                        /* bye bye, brick! */
                        sound_play( sound_load("samples/break.wav") );
                        bnode->data->state = BRS_DEAD;
                        got_dead_brick = TRUE;
                    }
                }
            }

            /* falling bricks */
            if(bnode->data->brick_ref->behavior == BRB_FALL && bnode->data->state == BRS_ACTIVE) {
                brick_t *brick_down = bnode->data;
                brick_down->value[1] += game_delta(); /* timer */
                if(brick_down->value[1] >= BRB_FALL_TIME) {
                    int bi, bj, bw, bh;
                    int right_oriented = ((int)brick_down->brick_ref->behavior_arg[2] != 0);
                    image_t *brkimg = brick_down->brick_ref->image;
                
                    /* particles */
                    bw = max(brick_down->brick_ref->behavior_arg[0], 1);
                    bh = max(brick_down->brick_ref->behavior_arg[1], 1);
                    for(bi=0; bi<bw; bi++) {
                        for(bj=0; bj<bh; bj++) {
                            v2d_t piecepos = v2d_new(brick_down->x + (bi*brkimg->w)/bw, brick_down->y + (bj*brkimg->h)/bh);
                            v2d_t piecespeed = v2d_new(0, 20+bj*20+ (right_oriented?bi:bw-bi)*20);
                            image_t *piece = image_create(brkimg->w/bw, brkimg->h/bh);

                            image_blit(brkimg, piece, (bi*brkimg->w)/bw, (bj*brkimg->h)/bh, 0, 0, piece->w, piece->h);
                            level_create_particle(piece, piecepos, piecespeed, FALSE);
                        }
                    }

                    /* bye, brick! :] */
                    sound_play( sound_load("samples/break.wav") );
                    brick_down->state = BRS_DEAD;
                    got_dead_brick = TRUE;
                }            
            }

            /* moveable bricks */
            brick_move(bnode->data);
        }

        if(got_dead_brick) {
            remove_dead_bricks();
            brick_list_unclip(major_bricks);
            major_bricks = brick_list_clip();
        }


        /* update items */
        iprev = NULL;
        for(i=0;i<3;i++) team[i]->entering_loop=FALSE;
        for(inode = item_list; inode; inode=inext) {
            double x = inode->data->actor->position.x;
            double y = inode->data->actor->position.y;
            double w = actor_image(inode->data->actor)->w;
            double h = actor_image(inode->data->actor)->h;

            inext = inode->next;
            if(inode->data->state == IS_DEAD) {
                item_destroy(inode->data);
                if(iprev)
                    iprev->next = inode->next;
                else
                    item_list = inode->next;
                free(inode);
            }
            else {
                if(inside_screen(x, y, w, h, DEFAULT_MARGIN)) {
                    item_update(inode->data, team, major_bricks, item_list /*major_items*/); /* major_items bugs the switch/teleporter */
                    if(inode->data->obstacle) { /* is this item an obstacle? */
                        /* we'll create a fake brick here */
                        brick_list_t *bn1, *bn2;
                        int offset = 1;
                        v2d_t v = v2d_add(inode->data->actor->hot_spot, v2d_new(0,-offset));
                        image_t *img = actor_image(inode->data->actor);
                        brick_t *fake = create_fake_brick(img->w, img->h-offset, v2d_subtract(inode->data->actor->position,v));

                        /* add to the fake bricks list */
                        bn1 = mallocx(sizeof *bn1);
                        bn1->next = fake_bricks;
                        bn1->data = fake;
                        fake_bricks = bn1;

                        /* add to the major bricks list */
                        bn2 = mallocx(sizeof *bn2);
                        bn2->next = major_bricks;
                        bn2->data = fake;
                        major_bricks = bn2;
                    }
                }
                else {
                    /* this item is outside the screen... */
                    if(!inode->data->preserve)
                        inode->data->state = IS_DEAD;
                }
                iprev = inode;
            }
        }



        /* update enemies */
        eprev = NULL;
        for(enode = enemy_list; enode; enode=enext) {
            double x = enode->data->actor->position.x;
            double y = enode->data->actor->position.y;
            double w = actor_image(enode->data->actor)->w;
            double h = actor_image(enode->data->actor)->h;

            enext = enode->next;
            if(enode->data->state == ES_DEAD) {
                enemy_destroy(enode->data);
                if(eprev)
                    eprev->next = enode->next;
                else
                    enemy_list = enode->next;
                free(enode);
            }
            else {
                if(inside_screen(x, y, w, h, DEFAULT_MARGIN)) {
                    if(!input_is_ignored(player->actor->input))
                        enemy_update(enode->data, team, major_bricks);
                }
                else if(!inside_screen(enode->data->actor->spawn_point.x, enode->data->actor->spawn_point.y, w, h, DEFAULT_MARGIN))
                    enode->data->actor->position = enode->data->actor->spawn_point;
                eprev = enode;
            }
        }


        /* update boss */
        if(got_boss()) {
            actor_t *pa = player->actor;
            double ba[4] = { pa->position.x, pa->position.y, pa->position.x+1, pa->position.y+1 };
            double bb[4] = { boss->rect_x, boss->rect_y, boss->rect_x+boss->rect_w, boss->rect_y+boss->rect_h };

            /* boss fight! */
            boss_update(boss, team, brick_list); /* bouken deshou, deshou!? */
            if(!boss_defeated(boss) && bounding_box(ba, bb)) { /* honto ga uso ni kawaru sekai de */
                player_inside_boss_area = TRUE; /* yume ga aru kara tsuyoku naru no yo */
                boss_fight_activated = TRUE; /* dare no tame janai */
                level_hide_dialogbox();
            }

            /* only the active player can enter the boss area */
            if(!boss_defeated(boss)) {
                int br = 30; /* border */
                actor_t *ta;

                for(i=0; i<3; i++) {
                    if(team[i] == player)
                        continue;

                    ta = team[i]->actor;

                    if(ta->position.x > boss->rect_x-br && ta->position.x < boss->rect_x) {
                        ta->position.x = boss->rect_x-br;
                        ta->speed.x = 0;
                    }

                    if(ta->position.x > boss->rect_x+boss->rect_w && ta->position.x < boss->rect_x+boss->rect_w+br) {
                        ta->position.x = boss->rect_x+boss->rect_w+br;
                        ta->speed.x = 0;
                    }

                    /*if(ta->position.y > boss->rect_y-br && ta->position.y < boss->rect_y)
                        ta->position.y = boss->rect_y-br;

                    if(ta->position.y > boss->rect_y+boss->rect_h && ta->position.y < boss->rect_y+boss->rect_h+br)
                        ta->position.y = boss->rect_y+boss->rect_h+br;*/
                        
                }
            }

            /* the boss has been defeated... */
            if(boss_defeated(boss) || player->dieing) {
                player_inside_boss_area = FALSE;
                if(music) { /* fade-out music */
                    music_set_volume(music_get_volume() - 0.5*dt);
                    if(music_get_volume() < EPSILON) {
                        music_stop();
                        music_set_volume(1.0);
                        block_music = TRUE;
                    }
                }
            }
        }



        /* update players */
        for(i=0; i<3; i++)
            input_ignore(team[i]->actor->input);

        if(level_timer >= 3.5 && camera_focus == player->actor) /* not (opening animation) */
            input_restore(player->actor->input);

        for(i=0; i<3; i++) {
            double x = team[i]->actor->position.x;
            double y = team[i]->actor->position.y;
            double w = actor_image(team[i]->actor)->w;
            double h = actor_image(team[i]->actor)->h;
            double hy = team[i]->actor->hot_spot.y;

            /* is there a player dieing? */
            if(team[i]->dieing)
                got_dieing_player = TRUE;


            /* somebody is hurt! show this to the user */
            if(i != player_id) {
                if(team[i]->getting_hit)
                    level_change_player(i);

                if(team[i]->dieing) {
                    level_change_player(i);
                    camera_teleport(team[i]->actor->position);
                }
            }

            /* death */
            if(team[i]->dead) {
                if(player_get_lives() > 1) {
                    /* restart the level! */
                    if(fadefx_over()) {
                        quest_setvalue(QUESTVALUE_TOTALTIME, quest_getvalue(QUESTVALUE_TOTALTIME)+level_timer);
                        player_set_lives(player_get_lives()-1);
                        restart();
                        return;
                    }
                    fadefx_out(image_rgb(0,0,0), 1.0);
                }
                else {
                    /* game over */
                    scenestack_pop();
                    scenestack_push( storyboard[SCENE_GAMEOVER] );
                    return;
                }
            }

            /* level cleared! */
            if(actclear_goto_next_level) {
                if(fadefx_over()) {
                    scenestack_pop();
                    return;
                }
                fadefx_out(image_rgb(0,0,0), 1.0);
            }

            /* updating... */
            if(inside_screen(x, y, w, h, DEFAULT_MARGIN/4) || team[i]->dieing)
                player_update(team[i], major_bricks);

            /* clipping... */
            if(team[i]->actor->position.y < hy && !team[i]->dieing) {
                team[i]->actor->position.y = hy;
                team[i]->actor->speed.y = 0;
            }
            else if(team[i]->actor->position.y > level_height-(h-hy)) {
                if(inside_screen(x,y,w,h,DEFAULT_MARGIN/4))
                    player_kill(team[i]);
            }
        }

        /* change the active team member */
        if(!got_dieing_player && !level_cleared) {
            level_timer += game_delta();
            if(input_button_pressed(player->actor->input, IB_FIRE2))  {
                if(fabs(player->actor->speed.y) < EPSILON && !player->on_moveable_platform && !player_inside_boss_area)
                    level_change_player((player_id+1) % 3);
                else
                    sound_play( sound_load("samples/deny.wav") );
            }
        }

        /* boss area */
        if(got_boss() && player_inside_boss_area) {
            actor_t *pa = player->actor;

            if(pa->position.x < boss->rect_x) {
                pa->position.x = boss->rect_x;
                pa->speed.x = max(0, pa->speed.x);
            }
            else if(pa->position.x > boss->rect_x+boss->rect_w) {
                pa->position.x = boss->rect_x+boss->rect_w;
                pa->speed.x = min(pa->speed.x, 0);
            }

            pa->position.y = clip(pa->position.y, boss->rect_y, boss->rect_y+boss->rect_h);
        }

        /* if someone is dieing, fade out the music */
        if(got_dieing_player)
            music_set_volume(music_get_volume() - 0.5*dt);

        /* update particles */
        particle_update_all(major_bricks);


        /* cleanup the fake bricks list */
        for(bnode=fake_bricks; bnode; bnode=bnext) {
            bnext = bnode->next;
            destroy_fake_brick(bnode->data);
            free(bnode);
        }
        fake_bricks = NULL;


        brick_list_unclip(major_bricks);
        item_list_unclip(major_items);


        /* update camera */
        if(level_cleared)
            camera_move_to(v2d_add(camera_focus->position, v2d_new(0, -90)), 0.17);
        else if(player_inside_boss_area) {
            double lock[2] = { boss->rect_x+VIDEO_SCREEN_W/2, boss->rect_x+boss->rect_w-VIDEO_SCREEN_W/2 };
            v2d_t offv = v2d_new( clip(camera_focus->position.x, lock[0], lock[1]), camera_focus->position.y );
            camera_move_to(v2d_add(offv, v2d_new(0, -90)), 0.17);
        }
        else if(!got_dieing_player)
            camera_move_to(camera_focus->position, 0.17);

        camera_update();
    }
    else {

        /* level editor */
        int holded = input_button_down(keyboard, IB_FIRE3);
        double camspd = (holded ? 5 : 1) * 500; /* camera speed */
        v2d_t topleft = v2d_subtract(editor_camera, v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2));
        item_list_t *item_iter; /* this was commented */
        image_t *cursor_arrow = sprite_get_image(sprite_get_animation("SD_ARROW", 0), 0);

        /* update items - this was commented */
        for(item_iter=item_list; item_iter; item_iter=item_iter->next)
            item_update(item_iter->data, team, brick_list, item_list);

        /* keyboard stuff */
        if(input_button_pressed(keyboard, IB_FIRE4)) {
            if(input_button_down(keyboard, IB_FIRE3)) { /* save? */
                char mypath[1024];

                sprintf(mypath, "levels/%s", basename(file));
                level_save(mypath);

                sound_play( sound_load("samples/glasses.wav") );
                video_showmessage("Level saved.");
            }

            update_level_size();
            editor = 0;
            return;
        }

        if(holded) { /* change category */
            if(input_button_pressed(keyboard, IB_FIRE1)) {
                editor_cursor_objtype = (editor_cursor_objtype+1)%3;
                editor_cursor_objid = 0;
                editor_cursor_itemid = 0;
                video_showmessage("Current category: %s", editor_object_category(editor_cursor_objtype));
            }
            if(input_button_pressed(keyboard, IB_FIRE2)) {
                editor_cursor_objtype = ((editor_cursor_objtype-1)+3)%3;
                editor_cursor_objid = 0;
                editor_cursor_itemid = 0;
                video_showmessage("Current category: %s", editor_object_category(editor_cursor_objtype));
            }
        }
        else {
            /* change object */
            int lim = (editor_cursor_objtype==EDT_BRICK) ? brickdata_size() :
                      (editor_cursor_objtype==EDT_ITEM) ? editor_item_list_size :
                       ENEMYDATA_MAX;

            /* next object */
            if(input_button_pressed(keyboard, IB_FIRE1)) {
                if(editor_cursor_objtype == EDT_ITEM) {
                    editor_cursor_itemid = (editor_cursor_itemid+1)%lim;
                    editor_cursor_objid = editor_item_list[editor_cursor_itemid];
                }
                else
                    editor_cursor_objid = (editor_cursor_objid+1)%lim;

                video_showmessage("Current %s: %03d %s", editor_object_category(editor_cursor_objtype), editor_cursor_objid, editor_object_info(editor_cursor_objtype, editor_cursor_objid));
            }

            /* previous object */
            if(input_button_pressed(keyboard, IB_FIRE2)) {
                if(editor_cursor_objtype == EDT_ITEM) {
                    editor_cursor_itemid = ((editor_cursor_itemid-1)+lim)%lim;
                    editor_cursor_objid = editor_item_list[editor_cursor_itemid];
                }
                else
                    editor_cursor_objid = ((editor_cursor_objid-1)+lim)%lim;

                video_showmessage("Current %s: %03d %s", editor_object_category(editor_cursor_objtype), editor_cursor_objid, editor_object_info(editor_cursor_objtype, editor_cursor_objid));
            }
        }


        /* mouse cursor */
        /*editor_cursor.x += mouse->dx*dt;
        editor_cursor.y += mouse->dy*dt;*/
        editor_cursor.x = clip(mouse->x, 0, VIDEO_SCREEN_W-cursor_arrow->w);
        editor_cursor.y = clip(mouse->y, 0, VIDEO_SCREEN_H-cursor_arrow->h);

        /* new spawn point */
        if(input_button_pressed(mouse, IB_FIRE1) && input_button_down(keyboard, IB_FIRE3)) {
            v2d_t nsp = v2d_new((int)(editor_cursor.x+topleft.x)/EDITOR_GRID*EDITOR_GRID, (int)(editor_cursor.y+topleft.y)/EDITOR_GRID*EDITOR_GRID);
            level_set_spawn_point(nsp);
            spawn_players();
            video_showmessage("New spawn point: %d,%d", (int)nsp.x, (int)nsp.y);
        }

        /* new object */
        if(input_button_pressed(mouse, IB_FIRE1) && !input_button_down(keyboard, IB_FIRE3)) {
            switch(editor_cursor_objtype) {
                case EDT_BRICK:
                    level_create_brick(editor_cursor_objid, v2d_new((int)(editor_cursor.x+topleft.x)/EDITOR_GRID*EDITOR_GRID, (int)(editor_cursor.y+topleft.y)/EDITOR_GRID*EDITOR_GRID));
                    break;

                case EDT_ITEM:
                    level_create_item(editor_cursor_objid, v2d_new((int)(editor_cursor.x+topleft.x)/EDITOR_GRID*EDITOR_GRID, (int)(editor_cursor.y+topleft.y)/EDITOR_GRID*EDITOR_GRID));
                    break;

                case EDT_ENEMY:
                    level_create_enemy(editor_cursor_objid, v2d_new((int)(editor_cursor.x+topleft.x)/EDITOR_GRID*EDITOR_GRID, (int)(editor_cursor.y+topleft.y)/EDITOR_GRID*EDITOR_GRID));
                    break;
            }
        }

        /* pick or delete object */
        if(input_button_pressed(mouse, IB_FIRE2) || input_button_pressed(mouse, IB_FIRE3)) {
            int pick = input_button_pressed(mouse, IB_FIRE3);
            int picked_something = FALSE;
            brick_list_t *itb, *itb_old = NULL;
            item_list_t *iti, *iti_old = NULL;
            enemy_list_t *ite, *ite_old = NULL;

            switch(editor_cursor_objtype) {
                case EDT_BRICK:
                    for(itb=brick_list;itb;itb=itb->next) {
                        double a[4] = {itb->data->x, itb->data->y, itb->data->x + itb->data->brick_ref->image->w, itb->data->y + itb->data->brick_ref->image->h};
                        double b[4] = { editor_cursor.x+topleft.x , editor_cursor.y+topleft.y , editor_cursor.x+topleft.x+1 , editor_cursor.y+topleft.y+1 };
                        if(bounding_box(a,b)) {
                            if(pick) {
                                editor_cursor_objid = get_brick_id(itb->data);
                                picked_something = TRUE;
                            }
                            else {
                                if(itb == brick_list)
                                    brick_list = itb->next;
                                else
                                    itb_old->next = itb->next;

                                free(itb->data);
                                free(itb);
                                break;
                            }
                        }
                        itb_old = itb;
                    }
                    break;

                case EDT_ITEM:
                    for(iti=item_list;iti;iti=iti->next) {
                        double a[4] = {iti->data->actor->position.x-iti->data->actor->hot_spot.x, iti->data->actor->position.y-iti->data->actor->hot_spot.y, iti->data->actor->position.x-iti->data->actor->hot_spot.x + actor_image(iti->data->actor)->w, iti->data->actor->position.y-iti->data->actor->hot_spot.y + actor_image(iti->data->actor)->h};
                        double b[4] = { editor_cursor.x+topleft.x , editor_cursor.y+topleft.y , editor_cursor.x+topleft.x+1 , editor_cursor.y+topleft.y+1 };

                        if(bounding_box(a,b)) {
                            if(pick) {
                                int index = index_editor_item_list(iti->data->type);
                                if(index >= 0) {
                                    editor_cursor_itemid = index;
                                    editor_cursor_objid = editor_item_list[index];
                                    picked_something = TRUE;
                                }
                            }
                            else {
                                if(iti == item_list)
                                    item_list = iti->next;
                                else
                                    iti_old->next = iti->next;
    
                                free(iti);
                                break;
                            }
                        }
                        iti_old = iti;
                    }
                    break;

                case EDT_ENEMY:
                    for(ite=enemy_list;ite;ite=ite->next) {
                        double a[4] = {ite->data->actor->position.x-ite->data->actor->hot_spot.x, ite->data->actor->position.y-ite->data->actor->hot_spot.y, ite->data->actor->position.x-ite->data->actor->hot_spot.x + actor_image(ite->data->actor)->w, ite->data->actor->position.y-ite->data->actor->hot_spot.y + actor_image(ite->data->actor)->h};
                        double b[4] = { editor_cursor.x+topleft.x , editor_cursor.y+topleft.y , editor_cursor.x+topleft.x+1 , editor_cursor.y+topleft.y+1 };
                        if(bounding_box(a,b)) {
                            if(pick) {
                                editor_cursor_objid = ite->data->type;
                                picked_something = TRUE;
                            }
                            else {
                                if(ite == enemy_list)
                                    enemy_list = ite->next;
                                else
                                    ite_old->next = ite->next;
    
                                free(ite);
                                break;
                            }
                        }
                        ite_old = ite;
                    }
                    break;
            }

            /* show some information... */
            if(picked_something)
                video_showmessage("Picked up %s %03d %s", editor_object_category(editor_cursor_objtype), editor_cursor_objid, editor_object_info(editor_cursor_objtype, editor_cursor_objid));
        }

        /* scrolling */
        if(input_button_down(keyboard, IB_UP)) editor_camera.y -= camspd*dt;
        if(input_button_down(keyboard, IB_DOWN)) editor_camera.y += camspd*dt;
        if(input_button_down(keyboard, IB_LEFT)) editor_camera.x-= camspd*dt;
        if(input_button_down(keyboard, IB_RIGHT)) editor_camera.x += camspd*dt;
        editor_camera.x = max(editor_camera.x, VIDEO_SCREEN_W/2);
        editor_camera.y = max(editor_camera.y, VIDEO_SCREEN_H/2);
        camera.position = editor_camera;
    }


    /* other stuff */
    update_music();
}



/*
 * level_render()
 * Rendering function
 */
void level_render()
{
    int i;
    brick_list_t *major_bricks, *p;
    item_list_t *inode;
    enemy_list_t *enode;

    /* background */
    background_render();

    /* render players (bring to back?) */
    render_players(TRUE);

    /* render bricks which are not obstacles */
    major_bricks = brick_list_clip();
    for(p=major_bricks; p; p=p->next) {
        brick_animate(p->data);
        if(p->data->brick_ref->property == BRK_NONE)
            image_draw(brick_image(p->data), video_buffer, p->data->x-((int)camera.position.x-VIDEO_SCREEN_W/2), p->data->y-((int)camera.position.y-VIDEO_SCREEN_H/2), IF_NONE);
    }

    /* render items (bring to back) */
    for(inode=item_list; inode; inode=inode->next) {
        if(inode->data->bring_to_back)
            item_render(inode->data, camera.position);
    }

    /* render obstacle bricks */
    for(p=major_bricks; p; p=p->next) {
        if(p->data->brick_ref->property != BRK_NONE && p->data->brick_ref->angle % 180 != 0) /* render walls/slopes first */
            image_draw(brick_image(p->data), video_buffer, p->data->x-((int)camera.position.x-VIDEO_SCREEN_W/2), p->data->y-((int)camera.position.y-VIDEO_SCREEN_H/2), IF_NONE);
    }
    for(p=major_bricks; p; p=p->next) {
        if(p->data->brick_ref->property != BRK_NONE && p->data->brick_ref->angle % 180 == 0) /* render floor/ceil later... */
            image_draw(brick_image(p->data), video_buffer, p->data->x-((int)camera.position.x-VIDEO_SCREEN_W/2), p->data->y-((int)camera.position.y-VIDEO_SCREEN_H/2), IF_NONE);
    }
    brick_list_unclip(major_bricks);


    /* render boss (bring to back) */
    if(got_boss() && !boss->bring_to_front)
        boss_render(boss, camera.position);

    /* render players (bring to front?) */
    render_players(FALSE);

    /* render enemies */
    for(enode=enemy_list; enode; enode=enode->next)
        enemy_render(enode->data, camera.position);

    /* render boss (bring to front) */
    if(got_boss() && boss->bring_to_front)
        boss_render(boss, camera.position);

    /* render items (bring to front) */
    for(inode=item_list; inode; inode=inode->next) {
        if(!inode->data->bring_to_back)
            item_render(inode->data, camera.position);
    }

    /* render particles */
    particle_render_all();

    /* level editor */
    if(editor) {
        v2d_t topleft = v2d_subtract(editor_camera, v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2));
        v2d_t offset = v2d_new(0,0);
        int ymagic = 0;
        image_t *cursor = NULL, *cursor_arrow=sprite_get_image(sprite_get_animation("SD_ARROW", 0), 0);
        item_t *item;
        enemy_t *enemy;
        switch(editor_cursor_objtype) {
            case EDT_BRICK:
                cursor = brickdata_get(editor_cursor_objid)->image;
                ymagic = 0;
                break;
            case EDT_ITEM:
                item = item_create(editor_cursor_objid);
                cursor = actor_image(item->actor);
                offset = item->actor->hot_spot;
                item_destroy(item);
                ymagic = 2;
                break;
            case EDT_ENEMY:
                enemy = enemy_create(editor_cursor_objid);
                cursor = actor_image(enemy->actor);
                offset = enemy->actor->hot_spot;
                enemy_destroy(enemy);
                ymagic = 2;
                break;
        }
        editor_cursor_font->position.x = max(editor_cursor.x, 5);
        editor_cursor_font->position.y = max(editor_cursor.y, 50);
        font_set_text(editor_cursor_font, "%d.%d", (int)(editor_cursor.x+topleft.x), (int)(editor_cursor.y+topleft.y));
        image_draw(cursor, video_buffer, (int)(editor_cursor.x+topleft.x-offset.x)/EDITOR_GRID*EDITOR_GRID-topleft.x, (int)(editor_cursor.y+topleft.y-offset.y)/EDITOR_GRID*EDITOR_GRID-topleft.y + ymagic, IF_NONE);
        image_draw(cursor_arrow, video_buffer, (int)editor_cursor.x, (int)editor_cursor.y, IF_NONE);
        font_render(editor_cursor_font, v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_W/2));
    }
    else {
        v2d_t fixedcam = v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2);
        if(!level_cleared) {
            /* gui */
            actor_render(maingui, fixedcam);
            actor_render(lifegui, fixedcam);
            font_render(lifefnt, fixedcam);
            for(i=0;i<3;i++) font_render(mainfnt[i], fixedcam);
            font_render(fps, fixedcam);

            /* powerups */
            render_powerups();
        }
        else {
            /* reached the goal */
            actor_render(actclear_levelact, fixedcam);
            font_render(actclear_teamname, fixedcam);
            font_render(actclear_gotthrough, fixedcam);
            for(i=0; i<ACTCLEAR_BONUSMAX; i++) {
                actor_render(actclear_bonus[i], fixedcam);
                font_render(actclear_bonusfnt[i], fixedcam);
            }
        }

        /* level opening */
        if(level_timer < 2.5) 
            image_clear(video_buffer, image_rgb(0,0,0));
        actor_render(levelop, fixedcam);
        actor_render(levelact, fixedcam);
        font_render(leveltitle, fixedcam);

        /* dialog box */
        render_dlgbox(fixedcam);
    }
}



/*
 * level_release()
 * Releases the scene
 */
void level_release()
{
    int i;

    logfile_message("level_release()");

    input_destroy(mouse);
    input_destroy(keyboard);
    particle_release();
    level_unload();
    for(i=0; i<3; i++)
        player_destroy(team[i]);
    camera_release();

    font_destroy(editor_cursor_font);
    actor_destroy(lifegui);
    actor_destroy(maingui);
    font_destroy(lifefnt);
    for(i=0; i<3; i++)
        font_destroy(mainfnt[i]);
    font_destroy(fps);

    actor_destroy(levelop);
    actor_destroy(levelact);
    font_destroy(leveltitle);

    font_destroy(actclear_teamname);
    font_destroy(actclear_gotthrough);
    actor_destroy(actclear_levelact);
    for(i=0; i<ACTCLEAR_BONUSMAX; i++) {
        font_destroy(actclear_bonusfnt[i]);
        actor_destroy(actclear_bonus[i]);
    }

    font_destroy(dlgbox_title);
    font_destroy(dlgbox_message);
    actor_destroy(dlgbox);

    logfile_message("level_release() ok");
}




/*
 * level_setfile()
 * Call this before initializing this scene. This
 * function tells the scene what level it must
 * load then it gets initialized.
 */
void level_setfile(char *level)
{
    strcpy(file, level);
    logfile_message("level_setfile('%s')", level);
}


/*
 * level_create_particle()
 * Creates a new particle.
 */
void level_create_particle(image_t *image, v2d_t position, v2d_t speed, int destroy_on_brick)
{
    particle_t *p;
    particle_list_t *node;

    p = mallocx(sizeof *p);
    p->image = image;
    p->position = position;
    p->speed = speed;
    p->destroy_on_brick = destroy_on_brick;

    node = mallocx(sizeof *node);
    node->data = p;
    node->next = particle_list;
    particle_list = node;
}


/*
 * level_player()
 * Returns the current player
 */
player_t* level_player()
{
    return player;
}



/*
 * level_change_player()
 * Changes the current player
 */
void level_change_player(int id)
{
    player->spin_dash = player->braking = FALSE;
    player_id = id;
    player = team[player_id];
    level_set_camera_focus(player->actor);
    input_restore(player->actor->input);
}

/*
 * level_create_brick()
 * Creates and adds a brick to the level. This function
 * returns a pointer to the created brick.
 */
brick_t* level_create_brick(int type, v2d_t position)
{
    int i;
    brick_list_t *node;

    node = mallocx(sizeof *node);
    node->data = mallocx(sizeof(brick_t));

    node->data->brick_ref = brickdata_get(type);
    node->data->animation_frame = 0;
    node->data->x = node->data->sx = (int)position.x;
    node->data->y = node->data->sy = (int)position.y;
    node->data->enabled = TRUE;
    node->data->state = BRS_IDLE;
    for(i=0; i<BRICK_MAXVALUES; i++)
        node->data->value[i] = 0;

    insert_brick_sorted(node);
    return node->data;
}



/*
 * level_create_item()
 * Creates and adds an item to the level. Returns the
 * created item.
 */
item_t* level_create_item(int type, v2d_t position)
{
    item_list_t *node;

    node = mallocx(sizeof *node);
    node->data = item_create(type);
    node->data->actor->spawn_point = position;
    node->data->actor->position = position;
    node->next = item_list;
    item_list = node;

    return node->data;
}



/*
 * level_create_enemy()
 * Creates and adds an enemy to the level. Returns the
 * created enemy.
 */
enemy_t* level_create_enemy(int type, v2d_t position)
{
    enemy_list_t *node;

    node = mallocx(sizeof *node);
    node->data = enemy_create(type);
    node->data->actor->spawn_point = position;
    node->data->actor->position = position;
    node->next = enemy_list;
    enemy_list = node;

    return node->data;
}


/*
 * level_item_list()
 * Returns the item list
 */
item_list_t* level_item_list()
{
    return item_list;
}



/*
 * level_enemy_list()
 * Returns the enemy list
 */
enemy_list_t* level_enemy_list()
{
    return enemy_list;
}



/*
 * level_gravity()
 * Returns the gravity of the level
 */
double level_gravity()
{
    return gravity;
}


/*
 * level_player_id()
 * Returns the ID of the current player
 */
int level_player_id()
{
    return player_id;
}



/*
 * level_add_to_score()
 * Adds a value to the player's score.
 * It also creates a flying text that
 * shows that score.
 */
void level_add_to_score(int score)
{
    int i, d;
    v2d_t pos;
    item_t *digit;

    score = max(0, score);
    player_set_score(player_get_score() + score);
    for(i=0; score > 0; i++,score/=10) {
        d = score % 10;
        pos = v2d_new(9*(1-i), 0);
        digit = level_create_item(IT_FLYINGTEXT, v2d_add(player->actor->position, pos));
        /*digit->value[1] = SD_FONT0; <-- TODO */
        digit->value[2] = d+26;
    }
}



/*
 * level_create_animal()
 * Creates a random animal
 * TODO: theme based animals
 */
item_t* level_create_animal(v2d_t position)
{
    item_t *animal = level_create_item(IT_ANIMAL, position);
    animal->value[0] = random(MAX_ANIMALS);
    return animal;
}



/*
 * level_set_camera_focus()
 * Sets a new focus to the camera
 */
void level_set_camera_focus(actor_t *act)
{
    camera_focus = act;
}



/*
 * level_editmode()
 * Are we running the level editor?
 */
int level_editmode()
{
    return editor;
}


/*
 * level_size()
 * Returns the size of the level
 */
v2d_t level_size()
{
    return v2d_new(level_width, level_height);
}


/*
 * level_override_music()
 * Stops the music while the given sample is playing.
 * After it gets finished, the music gets played again.
 */
void level_override_music(sound_t *sample)
{
    if(music) music_stop();
    override_music = sample;
    sound_play(override_music);
}



/*
 * level_set_spawn_point()
 * Defines a new spawn point
 */
void level_set_spawn_point(v2d_t newpos)
{
    spawn_point = newpos;
}


/*
 * level_clear()
 * Call this when the player clears this level
 */
void level_clear(actor_t *end_sign)
{
    int i;

    if(level_cleared)
        return;

    /* act cleared! */
    level_cleared = TRUE;
    actclear_starttime = game_timer();

    /* bonus */
    actclear_ringbonus = player_get_rings()*10;
    actclear_totalbonus += actclear_ringbonus;
    for(i=0; i<3; i++) {
        if(team[i]->got_glasses) {
            level_add_to_secret_bonus(5000);
            quest_setvalue(QUESTVALUE_GLASSES, quest_getvalue(QUESTVALUE_GLASSES)+1);
        }
    }
    player_set_score( player_get_score() + actclear_totalbonus );
    quest_setvalue(QUESTVALUE_TOTALTIME, quest_getvalue(QUESTVALUE_TOTALTIME)+level_timer);

    /* ignore input and focus the camera on the end sign */
    for(i=0; i<3; i++) {
        input_ignore(team[i]->actor->input);
        team[i]->spin_dash = FALSE;
    }
    level_set_camera_focus(end_sign);
    level_hide_dialogbox();

    /* initializing resources... */
    font_set_text(actclear_teamname, "TEAM SONIC");
    actclear_teamname->position = v2d_new(-500, 20);

    font_set_text(actclear_gotthrough, "GOT THROUGH");
    actclear_gotthrough->position = v2d_new(-500, 46);

    actor_change_animation(actclear_levelact, sprite_get_animation("SD_LEVELACT", act-1));
    actclear_levelact->position = v2d_new(820, 25);

    for(i=0; i<ACTCLEAR_BONUSMAX; i++) {
        actclear_bonus[i]->position = v2d_new(-500, 120+i*20);
        actclear_bonusfnt[i]->position = v2d_new(820, 120+i*20);
    }

    actor_change_animation(actclear_bonus[0], sprite_get_animation("SD_RINGBONUS", 0));
    actor_change_animation(actclear_bonus[1], sprite_get_animation("SD_SECRETBONUS", 0));
    actor_change_animation(actclear_bonus[ACTCLEAR_BONUSMAX-1], sprite_get_animation("SD_TOTAL", 0));
}


/*
 * level_add_to_secret_bonus()
 * Adds a value to the secret bonus
 */
void level_add_to_secret_bonus(int value)
{
    actclear_secretbonus += value;
    actclear_totalbonus += value;
}



/*
 * level_call_dialogbox()
 * Calls a dialog box
 */
void level_call_dialogbox(char *title, char *message)
{
    int minlength = 22;
    int maxlength = 29;

    if(dlgbox_active && strcmp(font_get_text(dlgbox_title), title) == 0 && strcmp(font_get_text(dlgbox_message), message) == 0)
        return;

    dlgbox_active = TRUE;
    dlgbox_starttime = game_timer();
    font_set_text(dlgbox_title, title);
    font_set_text_wordwrap(dlgbox_message, minlength, maxlength, message);
}



/*
 * level_hide_dialogbox()
 * Hides the current dialog box (if any)
 */
void level_hide_dialogbox()
{
    dlgbox_active = FALSE;
}



/*
 * level_boss_battle()
 * Is/was the player fighting against the level boss (if any)?
 */
int level_boss_battle()
{
    return boss_fight_activated;
}


/*
 * level_kill_all_baddies()
 * Kills all the baddies on the level
 */
void level_kill_all_baddies()
{
    enemy_list_t *it;
    enemy_t *en;

    for(it=enemy_list; it; it=it->next) {
        en = it->data;
        en->state = ES_DEAD;
        level_create_item(IT_EXPLOSION, en->actor->position);
    }
}





/* private functions */

/* returns TRUE if a given region is
 * inside the screen position (camera-related) */
int inside_screen(int x, int y, int w, int h, int margin)
{
    v2d_t cam = level_editmode() ? editor_camera : camera.position;
    double a[4] = { x, y, x+w, y+h };
    double b[4] = {
        cam.x-VIDEO_SCREEN_W/2 - margin,
        cam.y-VIDEO_SCREEN_H/2 - margin,
        cam.x+VIDEO_SCREEN_W/2 + margin,
        cam.y+VIDEO_SCREEN_H/2 + margin
    };
    return bounding_box(a,b);
}

/* returns a list with every brick
 * inside an area of a given rectangle */
brick_list_t* brick_list_clip()
{
    brick_list_t *list = NULL, *p, *q;
    int bx, by, bw, bh;

    for(p=brick_list; p; p=p->next) {
        bx = min(p->data->x, p->data->sx);
        by = min(p->data->y, p->data->sy);
        bw = p->data->brick_ref->image->w;
        bh = p->data->brick_ref->image->h;
        if(inside_screen(bx,by,bw,bh,DEFAULT_MARGIN*2) || p->data->brick_ref->behavior == BRB_CIRCULAR) {
            q = mallocx(sizeof *q);
            q->data = p->data;
            q->next = list;
            list = q;
        }
    }

    return list;
}

/* returns a list with every item
 * inside an area of a given rectangle */
item_list_t* item_list_clip(int x, int y, int w, int h)
{
    item_list_t *list = NULL, *p, *q;
    int ix, iy, iw, ih;
    image_t *img;

    for(p=item_list; p; p=p->next) {
        img = actor_image(p->data->actor);
        ix = (int)p->data->actor->position.x;
        iy = (int)p->data->actor->position.y;
        iw = img->w;
        ih = img->h;
        if(inside_screen(ix,iy,iw,ih,DEFAULT_MARGIN)) {
            q = mallocx(sizeof *q);
            q->data = p->data;
            q->next = list;
            list = q;
        }
    }

    return list;
}

/* deletes the list generated by
 * brick_list_clip() */
void brick_list_unclip(brick_list_t *list)
{
    brick_list_t *next;

    while(list) {
        next = list->next;
        free(list);
        list = next;
    }
}


/* deletes the list generated by
 * item_list_clip() */
void item_list_unclip(item_list_t *list)
{
    item_list_t *next;

    while(list) {
        next = list->next;
        free(list);
        list = next;
    }
}


/* calculates the size of the
 * current level */
void update_level_size()
{
    int max_x=INT_MIN, max_y=INT_MIN;
    brick_list_t *p;

    for(p=brick_list;p;p=p->next) {
        if(p->data->brick_ref->property != BRK_NONE) {
            max_x = max(max_x, p->data->sx + p->data->brick_ref->image->w);
            max_y = max(max_y, p->data->sy + p->data->brick_ref->image->h);
        }
    }

    level_width = max(max_x, VIDEO_SCREEN_W);
    level_height = max(max_y, VIDEO_SCREEN_H);
}

/* returns the ID of a given brick,
 * or -1 if it was not found */
int get_brick_id(brick_t *b)
{
    int i;

    for(i=0; i<brickdata_size(); i++) {
        if(b->brick_ref == brickdata_get(i))
            return i;
    }

    return -1;
}

/* inserts a brick into brick_list, a linked
 * list that's always sorted by decreasing y
 * position */
void insert_brick_sorted(brick_list_t *b)
{
    brick_list_t *p;

    /* note that brick_list_clip() will reverse
     * part of this list later */
    if(brick_list) {
        if(b->data->y >= brick_list->data->sy) {
            b->next = brick_list;
            brick_list = b;
        }
        else {
            p = brick_list;
            while(p->next && p->next->data->sy > b->data->sy)
                p = p->next;
            b->next = p->next;
            p->next = b;
        }
    }
    else {
        b->next = NULL;
        brick_list = b;
    }
}


/* restarts the level preserving
 * the current spawn point */
void restart()
{
    v2d_t sp = spawn_point;
    level_release();
    level_init();
    spawn_point = sp;
    spawn_players();
}



/* creates a fake brick (useful on
 * item-generated bricks) */
brick_t *create_fake_brick(int width, int height, v2d_t position)
{
    int i;
    brick_t *b = mallocx(sizeof *b);
    brickdata_t *d = mallocx(sizeof *d);

    d->data = NULL;
    d->image = image_create(width, height);
    d->angle = 0;
    d->property = BRK_OBSTACLE;
    d->behavior = BRB_DEFAULT;
    for(i=0; i<BRICKBEHAVIOR_MAXARGS; i++)
        d->behavior_arg[i] = 0;

    b->brick_ref = d;
    b->animation_frame = 0;
    b->enabled = TRUE;
    b->x = b->sx = (int)position.x;
    b->y = b->sy = (int)position.y;
    for(i=0; i<BRICK_MAXVALUES; i++)
        b->value[i] = 0;

    return b;
}


/* destroys a previously created
 * fake brick */
void destroy_fake_brick(brick_t *b)
{
    image_destroy(b->brick_ref->image);
    free(b->brick_ref);
    free(b);
}

/* returns a static char* containing a string
 * that corresponds to the given editor category
 * object id */
char *editor_object_category(int id)
{
    static char buf[128];

    switch(id) {
        case EDT_BRICK: strcpy(buf, "brick"); break;
        case EDT_ITEM: strcpy(buf, "item"); break;
        case EDT_ENEMY: strcpy(buf, "enemy"); break;
        default: strcpy(buf, "unknown"); break;
    }

    return buf;
}


/* returns a static char* containing information
 * about a given object */
char *editor_object_info(int objtype, int objid)
{
    static char buf[128];

    switch(objtype) {
        case EDT_BRICK:
        {
            brickdata_t *x = brickdata_get(objid);
            sprintf(buf, "(ang:%d,prp:%d,bh:%d)", x->angle, x->property, x->behavior);
            break;
        }

        case EDT_ITEM:
        {
            item_t *x = item_create(objid);
            sprintf(buf, "(obstacle:%c,back:%c)", x->obstacle ? 'T' : 'F', x->bring_to_back ? 'T' : 'F');
            item_destroy(x);
            break;
        }

        case EDT_ENEMY:
        {
            strcpy(buf, "(normal)");
            break;
        }

        default:
        {
            strcpy(buf, "(null)");
            break;
        }
    }

    return buf;
}


/* returns the index of item_id on
 * editor_item_list or -1 if the search fails */
int index_editor_item_list(int item_id)
{
    int i;

    for(i=0; i<editor_item_list_size; i++) {
        if(item_id == editor_item_list[i])
            return i;
    }

    return -1;
}




/* renders the players */
void render_players(int bring_to_back)
{
    int i;

    for(i=2; i>=0; i--) {
        if(team[i] != player && (team[i]->bring_to_back?TRUE:FALSE) == bring_to_back)
            player_render(team[i], camera.position);
    }

    if((player->bring_to_back?TRUE:FALSE) == bring_to_back) /* comparing two booleans */
        player_render(player, camera.position);
}


/* updates the music */
void update_music()
{
    if(music && !level_cleared && !block_music) {

        if(override_music && !sound_is_playing(override_music)) {
            override_music = NULL;
            if(!player->invencible && !player->got_speedshoes)
                music_play(music, MUSIC_LOOPFOREVER);
        }

        if(!override_music && !music_is_playing())
            music_play(music, MUSIC_LOOPFOREVER);

    }
}


/* puts the players at the spawn point */
void spawn_players()
{
    int i, v;

    for(i=0; i<3; i++) {
        v = ((int)spawn_point.x <= level_width/2) ? 2-i : i;
        team[i]->actor->mirror = ((int)spawn_point.x <= level_width/2) ? IF_NONE : IF_HFLIP;
        team[i]->actor->spawn_point.x = team[i]->actor->position.x = spawn_point.x + 15*v;
        team[i]->actor->spawn_point.y = team[i]->actor->position.y = spawn_point.y;
    }
}


/* removes every brick that satisfies (brick->state == BRS_DEAD) */
void remove_dead_bricks()
{
    brick_list_t *bprev, *bnode, *bnext;

    for(bprev=NULL,bnode=brick_list; bnode; bnode=bnext) {
        bnext = bnode->next;

        if(bnode->data->state == BRS_DEAD) {
            free(bnode->data);
            if(bprev)
                bprev->next = bnext;
            else
                brick_list = bnext;
            free(bnode);
        }

        bprev = bnode;
    }
}


/* updates the dialog box */
void update_dlgbox()
{
    double speed = 100.0; /* y speed */
    double dt = game_delta();
    uint32 t = game_timer();

    if(dlgbox_active) {
        if(t >= dlgbox_starttime + DLGBOX_MAXTIME) {
            dlgbox_active = FALSE;
            return;
        }
        dlgbox->position.x = (VIDEO_SCREEN_W - actor_image(dlgbox)->w)/2;
        dlgbox->position.y = max(dlgbox->position.y - speed*dt, VIDEO_SCREEN_H - actor_image(dlgbox)->h*1.3);

    }
    else {
        dlgbox->position.y = min(dlgbox->position.y + speed*dt, VIDEO_SCREEN_H);
    }

    dlgbox_title->position = v2d_add(dlgbox->position, v2d_new(7, 8));
    dlgbox_message->position = v2d_add(dlgbox->position, v2d_new(7, 20));
}


/* renders the dialog box */
void render_dlgbox(v2d_t camera_position)
{
    actor_render(dlgbox, camera_position);
    font_render(dlgbox_title, camera_position);
    font_render(dlgbox_message, camera_position);
}


/* is this a boss level? */
int got_boss()
{
    return (boss != NULL);
}









/* camera-related stuff */
void camera_init(v2d_t pos)
{
    camera.position = camera.dest = pos;
}

void camera_update()
{
    double dt = game_delta();
    v2d_t diff = v2d_subtract(camera.dest, camera.position);

    if(v2d_magnitude(diff) > 10) {
        diff = v2d_normalize(diff);
        camera.position.x += diff.x*camera.speed*dt;
        camera.position.y += diff.y*camera.speed*dt;
    }

    camera.position.x = clip(camera.position.x, VIDEO_SCREEN_W/2, level_width-VIDEO_SCREEN_W/2);
    camera.position.y = clip(camera.position.y, VIDEO_SCREEN_H/2, level_height-VIDEO_SCREEN_H/2);
}

void camera_release()
{
    camera.position = camera.dest = v2d_new(0,0);
    camera.speed = 0;
}

void camera_move_to(v2d_t newpos, double seconds)
{
    /* clipping */
    if(newpos.x < VIDEO_SCREEN_W/2) newpos.x = VIDEO_SCREEN_W/2;
    if(newpos.x > level_width-VIDEO_SCREEN_W/2) newpos.x = level_width-VIDEO_SCREEN_W/2;
    if(newpos.y < VIDEO_SCREEN_H/2) newpos.y = VIDEO_SCREEN_H/2;
    if(newpos.y > level_height-VIDEO_SCREEN_H/2) newpos.y = level_height-VIDEO_SCREEN_H/2;
    camera.dest = newpos;

    /* don't move too fast... */
    if(seconds > EPSILON)
        camera.speed = v2d_magnitude( v2d_subtract(camera.position,newpos) ) / seconds;
    else
        camera.speed = 1000;
}

void camera_teleport(v2d_t newpos)
{
    if(newpos.x < VIDEO_SCREEN_W/2) newpos.x = VIDEO_SCREEN_W/2;
    if(newpos.x > level_width-VIDEO_SCREEN_W/2) newpos.x = level_width-VIDEO_SCREEN_W/2;
    if(newpos.y < VIDEO_SCREEN_H/2) newpos.y = VIDEO_SCREEN_H/2;
    if(newpos.y > level_height-VIDEO_SCREEN_H/2) newpos.y = level_height-VIDEO_SCREEN_H/2;
    camera.dest = camera.position = newpos;
}












/* background stuff */

/* background_load(): loads a background theme from a file */
void background_load(char *filename)
{
    FILE *fp;
    char line[128], identifier[128], abs_path[1024];
    int i;

    resource_filepath(abs_path, filename, sizeof(abs_path), RESFP_READ);

    background_count = 0;
    for(i=0; i<BG_MAX; i++)
        background[i] = NULL;

    logfile_message("background_load('%s')", abs_path);
    if(NULL != (fp=fopen(abs_path, "r"))) {
        while(fgets(line, 128, fp)) {

            if(sscanf(line, "%127s", identifier) < 1)
                continue;

            if(strcmp(identifier, "background") == 0) {
                logfile_message("Reading background %d...", background_count);
                background[background_count++] = read_background(fp);
                background_count = min(background_count, BG_MAX-1);
            }

        }
        fclose(fp);
        logfile_message("background_load() ok!");
    }
    else
        fatal_error("FATAL ERROR: can't read file '%s'");
}


/* background_unload(): unloads all the previously loaded bgs */
void background_unload()
{
    int i;

    for(i=0; i<background_count; i++) {
        actor_destroy(background[i]->actor);
        spriteinfo_destroy(background[i]->data);
        free(background[i]);
    }

    background_count = 0;
}


/* background_update(): updates all the bgs */
void background_update()
{
    int i;
    double dt = game_delta(), t = 0.001*game_timer(), rx, ry, sx, sy;
    v2d_t topleft = v2d_new(camera.position.x-VIDEO_SCREEN_W/2, camera.position.y-VIDEO_SCREEN_H/2);

    for(i=0; i<background_count; i++) {
        /* bg movement */
        switch(background[i]->behavior) {
            case BGB_DEFAULT:
                break;

            case BGB_CIRCULAR:
                rx = background[i]->behavior_arg[0]; /* x-dist */
                ry = background[i]->behavior_arg[1]; /* y-dist */
                sx = background[i]->behavior_arg[2] * (2*PI); /* x-speed */
                sy = background[i]->behavior_arg[3] * (2*PI); /* y-speed */
                background[i]->x = background[i]->actor->spawn_point.x + rx*cos(sx*t);
                background[i]->y = background[i]->actor->spawn_point.y + ry*sin(sy*t);
                break;

            case BGB_LINEAR:
                sx = background[i]->behavior_arg[0]; /* x-speed */
                sy = background[i]->behavior_arg[1]; /* y-speed */
                background[i]->x += sx*dt;
                background[i]->y += sy*dt;
                break;
        }

        /* parallax scrolling */
        background[i]->actor->position.x = background[i]->x + topleft.x*background[i]->actor->speed.x;
        background[i]->actor->position.y = background[i]->y + topleft.y*background[i]->actor->speed.y;
    }
}

/* background_render(): renders all the bgs */
void background_render()
{
    int i;
    v2d_t v = v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2);

    for(i=0; i<background_count; i++)
        actor_render_repeat_xy(background[i]->actor, v, background[i]->repeat_x, background[i]->repeat_y);
}

/* read_background(): reads a bg from fp */
background_t *read_background(FILE *fp)
{
    char line[128], identifier[128], arg[128];
    double behavior_arg[BGBEHAVIOR_MAXARGS];
    int i, repeat_x=TRUE, repeat_y=TRUE, behavior=BGB_DEFAULT;
    v2d_t initial_position=v2d_new(0,0), scroll_speed=v2d_new(0,0);
    spriteinfo_t *sprite=NULL;

    for(i=0; i<BGBEHAVIOR_MAXARGS; i++)
        behavior_arg[i] = 0;

    while(fgets(line, 128, fp)) {

        if(sscanf(line, "%127s", identifier) < 1)
            continue;

        if(strcmp(identifier, "initial_position") == 0)
            sscanf(line, "%*s %lf %lf", &initial_position.x, &initial_position.y);
        else if(strcmp(identifier, "scroll_speed") == 0)
            sscanf(line, "%*s %lf %lf", &scroll_speed.x, &scroll_speed.y);
        else if(strcmp(identifier, "behavior") == 0) {
            int n;
            char *p;

            sscanf(line, "%*s %127s%n", arg, &n);
            for(p=line+n,i=0; (i<BGBEHAVIOR_MAXARGS) && (sscanf(p, "%lf%n", &behavior_arg[i], &n)!=EOF); p+=n,i++);

            if(str_icmp(arg, "DEFAULT") == 0)
                behavior = BGB_DEFAULT;
            else if(str_icmp(arg, "CIRCULAR") == 0)
                behavior = BGB_CIRCULAR;
            else if(str_icmp(arg, "LINEAR") == 0)
                behavior = BGB_LINEAR;
        }
        else if(strcmp(identifier, "repeat_x") == 0) {
            sscanf(line, "%*s %127s", arg);
            repeat_x = (str_icmp(arg, "TRUE") == 0);
        }
        else if(strcmp(identifier, "repeat_y") == 0) {
            sscanf(line, "%*s %127s", arg);
            repeat_y = (str_icmp(arg, "TRUE") == 0);
        }
        else if(strcmp(identifier, "sprite") == 0) {
            if(sprite)
                spriteinfo_destroy(sprite);

            logfile_message("Reading background sprite/animation data...");
            sprite = read_sprite(fp);
            logfile_message("done.");
        }
        else if(strcmp(identifier, "}") == 0) {
            background_t *bg = mallocx(sizeof *bg);

            bg->actor = actor_create();
            bg->data = sprite;
            bg->x = bg->actor->position.x = bg->actor->spawn_point.x = initial_position.x;
            bg->y = bg->actor->position.y = bg->actor->spawn_point.y = initial_position.y;
            bg->actor->speed.x = scroll_speed.x;
            bg->actor->speed.y = scroll_speed.y;
            bg->repeat_x = repeat_x;
            bg->repeat_y = repeat_y;
            bg->behavior = behavior;
            for(i=0; i<BGBEHAVIOR_MAXARGS; i++)
                bg->behavior_arg[i] = behavior_arg[i];

            actor_change_animation(bg->actor, sprite->animation_data[0]);
            return bg;
        }
    }

    logfile_message("Warning: read_background() returned NULL (did you forget a '}' ?)");
    return NULL;
}












/* particle programming */

/* particle_init(): initializes the particle module */
void particle_init()
{
    particle_list = NULL;
}


/* particle_release(): releases the particle module */
void particle_release()
{
    particle_list_t *it, *next;
    particle_t *p;

    for(it=particle_list; it; it=next) {
        p = it->data;
        next = it->next;

        image_destroy(p->image);
        free(p);
        free(it);
    }

    particle_list = NULL;
}


/* particle_update_all(): updates every particle on this level */
void particle_update_all(brick_list_t *brick_list)
{
    double dt = game_delta(), g = level_gravity();
    int got_brick, inside_area;
    particle_list_t *it, *prev = NULL, *next;
    particle_t *p;

    for(it=particle_list; it; it=next) {
        p = it->data;
        next = it->next;
        inside_area = inside_screen(p->position.x, p->position.y, p->position.x+p->image->w, p->position.y+p->image->h, DEFAULT_MARGIN);

        /* collided with bricks? */
        got_brick = FALSE;
        if(p->destroy_on_brick && inside_area && p->speed.y > 0) {
            double a[4] = { p->position.x, p->position.y, p->position.x+p->image->w, p->position.y+p->image->h };
            brick_list_t *itb;
            for(itb=brick_list; itb && !got_brick; itb=itb->next) {
                brick_t *brk = itb->data;
                if(brk->brick_ref->property == BRK_OBSTACLE && brk->brick_ref->angle == 0) {
                    double b[4] = { brk->x, brk->y, brk->x+brk->brick_ref->image->w, brk->y+brk->brick_ref->image->h };
                    if(bounding_box(a,b))
                        got_brick = TRUE;
                }
            }
        }

        /* update particle */
        if(inside_area && !got_brick) {
            /* update this particle */
            p->position.x += p->speed.x*dt;
            p->position.y += p->speed.y*dt + 0.5*g*(dt*dt);
            p->speed.y += g*dt;
            prev = it;
        }
        else {
            /* remove this particle */
            if(prev)
                prev->next = next;
            else
                particle_list = next;

            image_destroy(p->image);
            free(p);
            free(it);
        }

    }
}


/* particle_render_all(): renders the particles */
void particle_render_all()
{
    particle_list_t *it;
    particle_t *p;
    v2d_t topleft = v2d_new(camera.position.x-VIDEO_SCREEN_W/2, camera.position.y-VIDEO_SCREEN_H/2);

    for(it=particle_list; it; it=it->next) {
        p = it->data;
        image_draw(p->image, video_buffer, (int)(p->position.x-topleft.x), (int)(p->position.y-topleft.y), IF_NONE);
    }
}





/* dialog regions */

/* read_dialogregion(): given a string in the form
 * dialogbox x y w h TITLE MESSAGE WITH SPACES
 * returns a corresponding dialogregion_t struct */
dialogregion_t read_dialogregion(char *line)
{
    int n;
    dialogregion_t tmp;

    tmp.disabled = FALSE;
    sscanf(line, "%*s %d %d %d %d %127s %n", &tmp.rect_x, &tmp.rect_y, &tmp.rect_w, &tmp.rect_h, tmp.title, &n);
    str_trim(tmp.message, line+n);

    return tmp;
}


/* write_dialogregion(): given a dialogregion_t struct,
 * returns a string in the form
 * dialogbox x y w h TITLE MESSAGE WITH SPACES */
char *write_dialogregion(dialogregion_t dlg)
{
    static char buf[1024];

    sprintf(buf, "dialogbox %d %d %d %d %s %s", dlg.rect_x, dlg.rect_y, dlg.rect_w, dlg.rect_h, dlg.title, dlg.message);
    return buf;
}



/* update_dialogregions(): updates all the dialog regions.
 * This checks if the player enters one region, and if he/she does,
 * this function shows the corresponding dialog box */
void update_dialogregions()
{
    int i;
    double a[4], b[4];

    if(level_timer < 2.0)
        return;

    a[0] = player->actor->position.x;
    a[1] = player->actor->position.y;
    a[2] = a[0] + actor_image(player->actor)->w;
    a[3] = a[1] + actor_image(player->actor)->h;

    for(i=0; i<dialogregion_size; i++) {
        if(dialogregion[i].disabled)
            continue;

        b[0] = dialogregion[i].rect_x;
        b[1] = dialogregion[i].rect_y;
        b[2] = b[0]+dialogregion[i].rect_w;
        b[3] = b[1]+dialogregion[i].rect_h;

        if(bounding_box(a, b)) {
            dialogregion[i].disabled = TRUE;
            level_call_dialogbox(dialogregion[i].title, dialogregion[i].message);
            break;
        }
    }
}



/* moveable platforms */


/*
 * level_brick_move_actor()
 * If the given brick moves, then the actor
 * must move too. Returns a delta_speed vector
 */
v2d_t level_brick_move_actor(brick_t *brick, actor_t *act)
{
    double t, rx, ry, sx, sy;

    if(!brick)
        return v2d_new(0,0);

    t = brick->value[0]; /* time elapsed ONLY FOR THIS brick */
    switch(brick->brick_ref->behavior) {
        case BRB_CIRCULAR:
            rx = brick->brick_ref->behavior_arg[0];             /* x-dist */
            ry = brick->brick_ref->behavior_arg[1];             /* y-dist */
            sx = brick->brick_ref->behavior_arg[2] * (2*PI);    /* x-speed */
            sy = brick->brick_ref->behavior_arg[3] * (2*PI);    /* y-speed */

            /* take the derivative. e.g.,
               d[ sx + A*cos(PI*t) ]/dt = -A*PI*sin(PI*t) */
            return v2d_new( (-rx*sx)*sin(sx*t), (ry*sy)*cos(sy*t) );

        default:
            return v2d_new(0,0);
    }
}



/* brick_move()
 * moves a brick depending on its type */
void brick_move(brick_t *brick)
{
    double t, rx, ry, sx, sy;

    if(!brick)
        return;

    brick->value[0] += game_delta();
    t = brick->value[0]; /* time elapsed ONLY FOR THIS brick */
    switch(brick->brick_ref->behavior) {
        case BRB_CIRCULAR:
            rx = brick->brick_ref->behavior_arg[0];             /* x-dist */
            ry = brick->brick_ref->behavior_arg[1];             /* y-dist */
            sx = brick->brick_ref->behavior_arg[2] * (2*PI);    /* x-speed */
            sy = brick->brick_ref->behavior_arg[3] * (2*PI);    /* y-speed */

            brick->x = brick->sx + round(rx*cos(sx*t));
            brick->y = brick->sy + round(ry*sin(sy*t));
            break;

        default:
            break;
    }
}


/* misc */

/* render powerups */
void render_powerups()
{
    image_t *icon[MAX_POWERUPS]; /* icons */
    int visible[MAX_POWERUPS]; /* is icon[i] visible? */
    int i, c = 0; /* c is the icon count */
    double t = game_timer() * 0.001;

    for(i=0; i<MAX_POWERUPS; i++)
        visible[i] = TRUE;

    if(player) {
        if(player->got_glasses)
            icon[c++] = sprite_get_image( sprite_get_animation("SD_ICON", 6) , 0 );

        if(player->shield_type == SH_SHIELD)
            icon[c++] = sprite_get_image( sprite_get_animation("SD_ICON", 7) , 0 );

        if(player->invencible) {
            icon[c++] = sprite_get_image( sprite_get_animation("SD_ICON", 4) , 0 );
            if(player->invtimer >= PLAYER_MAX_INVENCIBILITY*0.75) { /* it blinks */
                /* we want something that blinks faster as player->invtimer tends to PLAYER_MAX_INVENCIBLITY */
                double x = ((PLAYER_MAX_INVENCIBILITY-player->invtimer)/(PLAYER_MAX_INVENCIBILITY*0.25)); /* 1 = x --> 0 */
                visible[c-1] = sin( (0.5*PI*t) / (x+0.1) ) >= 0; /* the period of this function keeps going down... */
            }
        }

        if(player->got_speedshoes) {
            icon[c++] = sprite_get_image( sprite_get_animation("SD_ICON", 5) , 0 );
            if(player->speedshoes_timer >= PLAYER_MAX_SPEEDSHOES*0.75) { /* it blinks */
                /* we want something that blinks faster as player->speedshoes_timer tends to PLAYER_MAX_SPEEDSHOES */
                double x = ((PLAYER_MAX_SPEEDSHOES-player->speedshoes_timer)/(PLAYER_MAX_SPEEDSHOES*0.25)); /* 1 = x --> 0 */
                visible[c-1] = sin( (0.5*PI*t) / (x+0.1) ) >= 0; /* the period of this function keeps going down... */
            }
        }
    }

    for(i=0; i<c; i++) {
        if(visible[i])
            image_draw(icon[i], video_buffer, VIDEO_SCREEN_W - (icon[i]->w+5)*(i+1), 5, IF_NONE);
    }
}














/* pause screen */
static image_t *pause_buf;
static input_t *pause_keyb;
static int pause_ready;

/*
 * pause_init()
 * Initializes the pause screen
 */
void pause_init()
{
    pause_keyb = input_create_keyboard(NULL);
    pause_buf = image_create(video_buffer->w, video_buffer->h);
    pause_ready = FALSE;
    image_blit(video_buffer, pause_buf, 0, 0, 0, 0, pause_buf->w, pause_buf->h);
}


/*
 * pause_release()
 * Releases the pause screen
 */
void pause_release()
{
    image_destroy(pause_buf);
    input_destroy(pause_keyb);
}


/*
 * pause_update()
 * Updates the pause screen
 */
void pause_update()
{
    if(input_button_pressed(pause_keyb, IB_FIRE4)) {
        scenestack_pop();
        quest_abort();
        scenestack_pop();
        return;
    }

    if(pause_ready) {
        if(input_button_pressed(pause_keyb, IB_FIRE3)) {
            music_resume();
            scenestack_pop();
            return;
        }
    }
    else {
        if(input_button_up(pause_keyb, IB_FIRE3))
            pause_ready = TRUE;
    }
}


/* 
 * pause_render()
 * Renders the pause screen
 */
void pause_render()
{
    image_t *p = sprite_get_image(sprite_get_animation("SD_PAUSE", 0), 0);
    double t = game_timer()*0.001;
    double scale = 1+0.5*fabs(cos(PI/2*t));
    v2d_t pos = v2d_new((VIDEO_SCREEN_W-p->w)/2 - (scale-1)*p->w/2, (VIDEO_SCREEN_H-p->h)/2 - (scale-1)*p->h/2);

    image_blit(pause_buf, video_buffer, 0, 0, 0, 0, pause_buf->w, pause_buf->h);
    image_draw_scaled(p, video_buffer, (int)pos.x, (int)pos.y, v2d_new(scale,scale), IF_NONE);
}





/* Game over */
static font_t *gameover_fnt[2];
static image_t *gameover_buf;
static double gameover_timer;

/*
 * gameover_init()
 * Initializes the game over screen
 */
void gameover_init()
{
    gameover_timer = 0;

    gameover_fnt[0] = font_create(7);
    gameover_fnt[0]->position = v2d_new(-50, 112);
    font_set_text(gameover_fnt[0], "GAME");

    gameover_fnt[1] = font_create(7);
    gameover_fnt[1]->position = v2d_new(298, 112);
    font_set_text(gameover_fnt[1], "OVER");

    gameover_buf = image_create(video_buffer->w, video_buffer->h);
    image_blit(video_buffer, gameover_buf, 0, 0, 0, 0, gameover_buf->w, gameover_buf->h);
}



/*
 * gameover_update()
 * Updates the game over screen
 */
void gameover_update()
{
    double dt = game_delta();

    /* timer */
    gameover_timer += dt;
    if(gameover_timer > 5) {
        if(fadefx_over()) {
            quest_abort();
            scenestack_pop();
            return;
        }
        fadefx_out(image_rgb(0,0,0), 2);
    }

    /* "game over" text */
    gameover_fnt[0]->position.x += 200*dt;
    if(gameover_fnt[0]->position.x > 70)
        gameover_fnt[0]->position.x = 70;

    gameover_fnt[1]->position.x -= 200*dt;
    if(gameover_fnt[1]->position.x < 178)
        gameover_fnt[1]->position.x = 178;
}



/*
 * gameover_render()
 * Renders the game over screen
 */
void gameover_render()
{
    v2d_t v = v2d_new(VIDEO_SCREEN_W/2, VIDEO_SCREEN_H/2);

    image_blit(gameover_buf, video_buffer, 0, 0, 0, 0, gameover_buf->w, gameover_buf->h);
    font_render(gameover_fnt[0], v);
    font_render(gameover_fnt[1], v);
}



/*
 * gameover_release()
 * Releases the game over screen
 */
void gameover_release()
{
    image_destroy(gameover_buf);
    font_destroy(gameover_fnt[1]);
    font_destroy(gameover_fnt[0]);
    quest_abort();
}
