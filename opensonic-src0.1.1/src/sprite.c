/*
 * sprite.c - code for the sprites/animations
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
#include "global.h"
#include "sprite.h"
#include "video.h"
#include "util.h"


/* private data */
#define SPRITE_MAX_ANIM         100
#define SPRITE_MAX_ANIMDATA     256
#define SPRITE_CONFIGFILE       "config/sprite.def"



/* private functions */
animation_t *read_animation(FILE *fp); /* reads animation data from fp */
void load_sprite(spriteinfo_t *spr); /* loads the images used by spr */
void register_sprite(char *sprite_name, spriteinfo_t *spr); /* adds spr to the main hash */




/*
 * spritedata_load()
 * Loads the sprite data from the
 * sprite.def definiton file
 */
void spritedata_load()
{
    FILE *fp;
    char line[128], identifier[128], arg[128], abs_path[1024];

    resource_filepath(abs_path, SPRITE_CONFIGFILE, sizeof(abs_path), RESFP_READ);
    logfile_message("spritedata_load()...");

    if(NULL != (fp=fopen(abs_path, "r"))) {
        while(fgets(line, 128, fp)) {

            /* two parameters are expected at this level */
            if(sscanf(line, "%127s %127s", identifier, arg) < 2)
                continue;

            /* read sprite */
            if(strcmp(identifier, "sprite") == 0) {
                logfile_message("Reading sprite '%s'", arg);
                register_sprite(arg, read_sprite(fp));
            }

        }
        fclose(fp);
        logfile_message("spritedata_load() ok!");
    }
    else
        fatal_error("FATAL ERROR: can't read file '%s'.\nPlease make sure the game is installed properly.", abs_path);
}



/*
 * spriteinfo_create()
 * Creates a new spriteinfo_t object
 * with no animations
 */
spriteinfo_t *spriteinfo_create()
{
    spriteinfo_t *info = mallocx(sizeof *info);

    info->source_file[0] = '\0';
    info->rect_x = info->rect_y = info->rect_w = info->rect_h = 0;
    info->frame_w = info->frame_h = 0;
    info->hot_spot = v2d_new(0,0);

    info->frame_count = 0;
    info->frame_data = NULL;

    info->animation_count = 0;
    info->animation_data = NULL;

    return info;
}



/*
 * spriteinfo_destroy()
 * Destroys a loaded spriteinfo_t object
 */
void spriteinfo_destroy(spriteinfo_t *info)
{
    int i;

    for(i=0; i<info->frame_count; i++)
        image_destroy(info->frame_data[i]);

    for(i=0; i<info->animation_count; i++) {
        free(info->animation_data[i]->data);
        free(info->animation_data[i]);
    }

    free(info->frame_data);
    free(info->animation_data);
    free(info);
}




/*
 * sprite_get_animation()
 * Receives the sprite name and the desired animation number.
 * Returns a pointer to an animation object.
 */
animation_t *sprite_get_animation(char *sprite_name, int anim_id)
{
    spriteinfo_t *info = (spriteinfo_t*)hash_search(sprite_name);
    anim_id = clip(anim_id, 0, info->animation_count-1);
    return info ? info->animation_data[anim_id] : NULL;

}



/*
 * sprite_get_image()
 * Receives an animation and the desired frame number.
 * Returns an image.
 */
image_t *sprite_get_image(animation_t *anim, int frame_id)
{
    frame_id = clip(frame_id, 0, anim->frame_count-1);
    return anim->frame_data[ anim->data[frame_id] ];
}









/*
 * read_sprite()
 * Reads the sprite data from fp. Returns
 * a pointer to a sprite that is already loaded
 */
spriteinfo_t *read_sprite(FILE *fp)
{
    int i, j;
    char line[128], identifier[128];
    animation_t *anim_buffer[SPRITE_MAX_ANIM];

    /* data that must be read */
    char source_file[64];
    int rect_x, rect_y, rect_w, rect_h;
    int frame_w, frame_h;
    int hx=0, hy=0;

    /* to be determined */
    int frame_count=0, animation_count=0;




    for(i=0; i<SPRITE_MAX_ANIM; i++)
        anim_buffer[i] = NULL;

    while(fgets(line, 128, fp)) {

        if(sscanf(line, "%127s", identifier) < 1)
            continue;

        if(strcmp(identifier, "source_file") == 0)
            sscanf(line, "%*s %63s", source_file);
        else if(strcmp(identifier, "source_rect") == 0)
            sscanf(line, "%*s %d %d %d %d", &rect_x, &rect_y, &rect_w, &rect_h);
        else if(strcmp(identifier, "frame_size") == 0)
            sscanf(line, "%*s %d %d", &frame_w, &frame_h);
        else if(strcmp(identifier, "hot_spot") == 0)
            sscanf(line, "%*s %d %d", &hx, &hy);
        else if(strcmp(identifier, "animation") == 0) {
            int anim_id;

            if(sscanf(line, "%*s %d", &anim_id) < 1)
                anim_id = 0;

            anim_id = clip(anim_id, 0, SPRITE_MAX_ANIM-1);
            if(anim_buffer[anim_id]) {
                free(anim_buffer[anim_id]->data);
                free(anim_buffer[anim_id]);
            }

            anim_buffer[anim_id] = read_animation(fp);
            animation_count = max(anim_id+1, animation_count);
        }
        else if(strcmp(identifier, "}") == 0) {
            spriteinfo_t *sprite;

            frame_count = (rect_w/frame_w) * (rect_h/frame_h);
            if(rect_w%frame_w > 0 || rect_h%frame_h > 0)
                logfile_message("Warning: read_sprite() incompatible source_rect x frame_size");
            frame_w = min(frame_w, rect_w);
            frame_h = min(frame_h, rect_h);

            sprite = spriteinfo_create();
            strcpy(sprite->source_file, source_file);
            sprite->rect_x = rect_x;
            sprite->rect_y = rect_y;
            sprite->rect_w = rect_w;
            sprite->rect_h = rect_h;
            sprite->frame_w = frame_w;
            sprite->frame_h = frame_h;
            sprite->hot_spot = v2d_new(hx, hy);
            sprite->frame_count = frame_count;
            sprite->frame_data = mallocx(frame_count * sizeof(image_t*));
            sprite->animation_count = animation_count;

            sprite->animation_data = mallocx(animation_count * sizeof(animation_t*));
            for(i=0; i<animation_count; i++) {
                sprite->animation_data[i] = anim_buffer[i];
                sprite->animation_data[i]->hot_spot = sprite->hot_spot;
                sprite->animation_data[i]->frame_data = sprite->frame_data;
                for(j=0; j<sprite->animation_data[i]->frame_count; j++)
                    sprite->animation_data[i]->data[j] = clip(sprite->animation_data[i]->data[j], 0, frame_count-1);
            }

            load_sprite(sprite);
            return sprite;
        }
    }


    logfile_message("Warning: read_sprite() returned NULL (did you forget the '}' ?)");
    return NULL;
}






/* private functions */

/* read_animation(): reads animation data from fp */
animation_t *read_animation(FILE *fp)
{
    char line[1024], identifier[128], arg[16];
    int i, data_buffer[SPRITE_MAX_ANIMDATA];

    /* default values */
    int repeat=TRUE, fps=8;
    int frame_count=1;

    

    for(i=0; i<SPRITE_MAX_ANIMDATA; i++)
        data_buffer[i] = 0;

    while(fgets(line, 1024, fp)) {

        if(sscanf(line, "%127s", identifier) < 1)
            continue;

        if(strcmp(identifier, "repeat") == 0) {
            sscanf(line, "%*s %15s", arg);
            repeat = (str_icmp(arg, "TRUE") == 0);
        }
        else if(strcmp(identifier, "fps") == 0) {
            sscanf(line, "%*s %d", &fps);
            fps = max(1, fps);
        }
        else if(strcmp(identifier, "data") == 0) {
            int f, n;
            char *p;

            frame_count = 0;
            sscanf(line, "%*s%n", &n);
            for(p=line+n; (sscanf(p, "%d%n", &f, &n) != EOF) && (frame_count < SPRITE_MAX_ANIMDATA); p+=n)
                data_buffer[frame_count++] = f;
        }
        else if(strcmp(identifier, "}") == 0) {
            animation_t *anim = mallocx(sizeof *anim);
            anim->repeat = repeat;
            anim->fps = fps;
            anim->frame_data = NULL;
            anim->frame_count = frame_count;

            anim->data = mallocx(frame_count * sizeof(int));
            for(i=0; i<frame_count; i++)
                anim->data[i] = data_buffer[i];

            return anim;
        }
    }

    logfile_message("Warning: read_animation() returned NULL (did you forget the '}' ?)");
    return NULL;
}


/* load_sprite(): reads the spritesheet */
void load_sprite(spriteinfo_t *spr)
{
    int i, cur_x, cur_y;
    image_t *sheet;

    logfile_message("load_sprite() with %d animations and %d frames", spr->animation_count, spr->frame_count);

    /* reading the images... */
    sheet = image_load(spr->source_file);
    cur_x = spr->rect_x;
    cur_y = spr->rect_y;
    for(i=0; i<spr->frame_count; i++) {
        spr->frame_data[i] = image_create(spr->frame_w, spr->frame_h);
        image_blit(sheet, spr->frame_data[i], cur_x, cur_y, 0, 0, spr->frame_w, spr->frame_h);
        cur_x += spr->frame_w;
        if(cur_x >= spr->rect_x+spr->rect_w) {
            cur_x = spr->rect_x;
            cur_y += spr->frame_h;
        }
    }

    logfile_message("load_sprite() ok!");
}


/* register_sprite(): adds spr to the main hash */
void register_sprite(char *sprite_name, spriteinfo_t *spr)
{
    logfile_message("Registering sprite '%s'...", sprite_name);
    hash_insert(sprite_name, spr, DATATYPE_SPRITE);
}
