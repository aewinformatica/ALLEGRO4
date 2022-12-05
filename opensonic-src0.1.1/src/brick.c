/*
 * brick.c - brick module
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
#include "global.h"
#include "video.h"
#include "brick.h"


/* private data */
#define BRKDATA_MAX                 500 /* this engine supports 500 bricks (max) */
static int brickdata_count; /* size of brickdata and spritedata */
static brickdata_t* brickdata[BRKDATA_MAX]; /* brick data */


/* private functions */
brickdata_t *read_brick(FILE *fp);



/*
 * brickdata_load()
 * Loads all the brick data from a file
 */
void brickdata_load(char *filename)
{
    FILE *fp;
    char line[128], identifier[128], abs_path[1024];
    int brick_id, i;

    resource_filepath(abs_path, filename, sizeof(abs_path), RESFP_READ);

    for(i=0; i<BRKDATA_MAX; i++) 
        brickdata[i] = NULL;

    logfile_message("brickdata_load('%s')", abs_path);
    if(NULL != (fp=fopen(abs_path, "r"))) {
        while(fgets(line, 128, fp)) {

            if(sscanf(line, "%127s %d", identifier, &brick_id) < 2)
                continue;

            if(strcmp(identifier, "brick") == 0) {
                brick_id = clip(brick_id, 0, BRKDATA_MAX-1);
                logfile_message("Reading brick %d", brick_id);

                if(brickdata[brick_id]) {
                    spriteinfo_destroy(brickdata[brick_id]->data);
                    free(brickdata[brick_id]);
                }

                brickdata[brick_id] = read_brick(fp);
                brickdata_count = max(brick_id+1, brickdata_count);
            }
        }
        fclose(fp);
        logfile_message("brickdata_load() ok!");
    }
    else
        fatal_error("FATAL ERROR: can't read file '%s'", abs_path);
}



/*
 * brickdata_unload()
 * Unloads brick data
 */
void brickdata_unload()
{
    int i;

    for(i=0; i<brickdata_count; i++) {
        spriteinfo_destroy(brickdata[i]->data);
        free(brickdata[i]);
    }

    brickdata_count = 0;
}


/*
 * brickdata_get()
 * Gets a brickdata_t* object
 */
brickdata_t *brickdata_get(int id)
{
    id = clip(id, 0, brickdata_count-1);
    return brickdata[id];
}


/*
 * brickdata_size()
 * How many bricks are loaded?
 */
int brickdata_size()
{
    return brickdata_count;
}


/*
 * brick_image()
 * Returns the image of an (animated?) brick
 */
image_t *brick_image(brick_t *brk)
{
    spriteinfo_t *s = brk->brick_ref->data;
    int f = clip((int)brk->animation_frame, 0, s->animation_data[0]->frame_count-1);
    return s->frame_data[ s->animation_data[0]->data[f] ];
}


/*
 * brick_animate()
 * Animates a brick
 */
void brick_animate(brick_t *brk)
{
    if(brk->brick_ref->data) { /* not a fake brick? */
        int c = brk->brick_ref->data->animation_data[0]->frame_count;
        brk->animation_frame += brk->brick_ref->data->animation_data[0]->fps * game_delta();
        if((int)brk->animation_frame >= c)
            brk->animation_frame = (int)brk->animation_frame % c;
    }
}





/* private functions */

/* read_brick(): reads a brick from a file */
brickdata_t *read_brick(FILE *fp)
{
    char line[128], identifier[128], arg[128];
    int i, property=BRK_NONE, angle=0, behavior=BRB_DEFAULT;
    double behavior_arg[BRICKBEHAVIOR_MAXARGS];
    spriteinfo_t *sprite = NULL;

    for(i=0; i<BRICKBEHAVIOR_MAXARGS; i++)
        behavior_arg[i] = 0;

    while(fgets(line, 128, fp)) {

        if(sscanf(line, "%127s", identifier) < 1)
            continue;

        if(strcmp(identifier, "type") == 0) {
            sscanf(line, "%*s %127s", arg);
            if(str_icmp(arg, "OBSTACLE") == 0)
                property = BRK_OBSTACLE;
            else if(str_icmp(arg, "PASSABLE") == 0)
                property = BRK_NONE;
        }
        else if(strcmp(identifier, "behavior") == 0) {
            int n;
            char *p;

            sscanf(line, "%*s %127s%n", arg, &n);
            for(p=line+n,i=0; (i<BRICKBEHAVIOR_MAXARGS) && (sscanf(p, "%lf%n", &behavior_arg[i], &n)!=EOF); p+=n,i++);

            if(str_icmp(arg, "DEFAULT") == 0)
                behavior = BRB_DEFAULT;
            else if(str_icmp(arg, "CIRCULAR") == 0)
                behavior = BRB_CIRCULAR;
            else if(str_icmp(arg, "BREAKABLE") == 0)
                behavior = BRB_BREAKABLE;
            else if(str_icmp(arg, "FALL") == 0)
                behavior = BRB_FALL;
        }
        else if(strcmp(identifier, "angle") == 0) {
            sscanf(line, "%*s %d", &angle);
            angle = ((angle%360)+360)%360;
        }
        else if(strcmp(identifier, "sprite") == 0) {
            if(sprite)
                spriteinfo_destroy(sprite);

            logfile_message("Reading brick sprite...");
            sprite = read_sprite(fp);
            logfile_message("done.");
        }
        else if(strcmp(identifier, "}") == 0) {
            brickdata_t *brk = mallocx(sizeof *brk);

            brk->data = sprite;
            brk->image = sprite->frame_data[0];
            brk->property = property;
            brk->angle = angle;
            brk->behavior = behavior;
            for(i=0; i<BRICKBEHAVIOR_MAXARGS; i++)
                brk->behavior_arg[i] = behavior_arg[i];

            return brk;
        }
    }

    logfile_message("Warning: read_brick() returned NULL (did you forget some '}' ?) ");
    return NULL;
}
