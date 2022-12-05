/*
 * video.c - video manager
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

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <allegro.h>
#include "global.h"
#include "video.h"
#include "sprite.h"
#include "util.h"
#include "brick.h"
#include "font.h"




/* private data */


/* video manager */
image_t *video_buffer;
static int video_2xsai, video_supereagle, video_fast2x;
static void fast2x_blit(BITMAP *src, BITMAP *dest, int bpp);
static void window_switch_in();
static void window_switch_out();
static int window_active = TRUE;

/* Fade-in & fade-out */
#define FADEFX_NONE            0
#define FADEFX_IN            1
#define FADEFX_OUT            2
static int fadefx_type;
static int fadefx_end;
static uint32 fadefx_color;
static double fadefx_elapsed_time;
static double fadefx_total_time;

/* Video Message */
#define VIDEOMSG_TIMEOUT    5000
static uint32 videomsg_endtime;
static char videomsg_data[512];









/* fade effects */

/*
 * fadefx_in()
 * Fade-in effect
 */
void fadefx_in(uint32 color, double seconds)
{
    if(fadefx_type == FADEFX_NONE) {
        fadefx_type = FADEFX_IN;
        fadefx_end = FALSE;
        fadefx_color = color;
        fadefx_elapsed_time = 0;
        fadefx_total_time = seconds;
    }
}


/*
 * fadefx_out()
 * Fade-out effect
 */
void fadefx_out(uint32 color, double seconds)
{
    if(fadefx_type == FADEFX_NONE) {
        fadefx_type = FADEFX_OUT;
        fadefx_end = FALSE;
        fadefx_color = color;
        fadefx_elapsed_time = 0;
        fadefx_total_time = seconds;
    }
}



/*
 * fadefx_over()
 * Asks if the fade effect has ended
 */
int fadefx_over()
{
    return fadefx_end;
}













/* video manager */

/*
 * video_init()
 * Initializes the video manager
 */
void video_init(int use_2xsai, int use_supereagle, int use_fast2x)
{
    /* video init */
    logfile_message("video_init()");
    video_buffer = image_create(VIDEO_SCREEN_W, VIDEO_SCREEN_H);
    image_clear(video_buffer, image_rgb(0,0,0));
    spritedata_load();
    font_init();

    window_active = TRUE;
    if(set_display_switch_mode(SWITCH_BACKGROUND) == 0) {
        if(set_display_switch_callback(SWITCH_IN, window_switch_in) != 0)
            logfile_message("can't set_display_switch_callback(SWTICH_IN, window_switch_in)");

        if(set_display_switch_callback(SWITCH_OUT, window_switch_out) != 0)
            logfile_message("can't set_display_switch_callback(SWTICH_OUT, window_switch_out)");
    }
    else
        logfile_message("can't set_display_switch_mode(SWITCH_BACKGROUND)");

    video_2xsai = use_2xsai;
    video_supereagle = use_supereagle;
    video_fast2x = use_fast2x;

    if(video_fast2x)
        video_2xsai = video_supereagle = FALSE;

    if(video_2xsai)
        video_supereagle = FALSE;

    if(video_2xsai || video_supereagle)
        Init_2xSaI(get_color_depth());

    /* video message */
    videomsg_endtime = 0;
}


/*
 * video_render()
 * Updates the video manager and the screen
 */
void video_render()
{
    /* fade effect */
    fadefx_end = FALSE;
    if(fadefx_type != FADEFX_NONE) {
        int n;
        fadefx_elapsed_time += game_delta();
        if(fadefx_elapsed_time < fadefx_total_time) {
            n = (int)( (double)255 * (fadefx_elapsed_time*1.25 / fadefx_total_time) );
            n = clip(n, 0, 255);
            n = (fadefx_type == FADEFX_IN) ? 255-n : n;
            drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
            set_trans_blender(0, 0, 0, n);
            rectfill(video_buffer->data, 0, 0, VIDEO_SCREEN_W, VIDEO_SCREEN_H, fadefx_color);
            solid_mode();
        }
        else {
            if(fadefx_type == FADEFX_OUT)
                rectfill(video_buffer->data, 0, 0, VIDEO_SCREEN_W, VIDEO_SCREEN_H, fadefx_color);
            fadefx_type = FADEFX_NONE;
            fadefx_total_time = fadefx_elapsed_time = 0;
            fadefx_color = 0;
            fadefx_end = TRUE;
        }
    }

    /* video message */
    if(game_timer() < videomsg_endtime)
        textout_ex(video_buffer->data, font, videomsg_data, 0, VIDEO_SCREEN_H-text_height(font), makecol(255,255,255), makecol(0,0,0));

    /* render */
    if(video_2xsai || video_supereagle || video_fast2x) {
        BITMAP *tmp = create_bitmap(VIDEO_SCREEN_W*2, VIDEO_SCREEN_H*2);

        if(video_fast2x)
            fast2x_blit(video_buffer->data, tmp, VIDEO_DEFAULT_BPP);
        else if(video_2xsai)
            Super2xSaI(video_buffer->data, tmp, 0, 0, 0, 0, VIDEO_SCREEN_W, VIDEO_SCREEN_H);
        else
            SuperEagle(video_buffer->data, tmp, 0, 0, 0, 0, VIDEO_SCREEN_W, VIDEO_SCREEN_H);

        if(video_2xsai || video_supereagle) {
            int i, j, k=2;
            for(i=0; i<tmp->h; i++) { /* 2xSaI/SuperEagle fix */
                for(j=0; j<k; j++) {
                    if(video_supereagle)
                        putpixel(tmp, VIDEO_SCREEN_W*2-1-j, i, getpixel(tmp, VIDEO_SCREEN_W*2-1-k, i));
                    else
                        putpixel(tmp, j, i, getpixel(tmp, k, i));
                }
            }
        }

        blit(tmp, screen, 0, 0, 0, 0, VIDEO_SCREEN_W*2, VIDEO_SCREEN_H*2);
        destroy_bitmap(tmp);
    }
    else
        blit(video_buffer->data, screen, 0, 0, 0, 0, VIDEO_SCREEN_W, VIDEO_SCREEN_H);
}


/*
 * video_release()
 * Releases the video manager
 */
void video_release()
{
    logfile_message("video_release()");
    image_destroy(video_buffer);
}


/*
 * video_showmessage()
 * Shows a text message to the user
 */
void video_showmessage(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vsprintf(videomsg_data, fmt, args);
    va_end(args);

    videomsg_endtime = game_timer() + VIDEOMSG_TIMEOUT;
}

/*
 * video_is_window_active()
 * Returns TRUE if the game window is active,
 * or FALSE otherwise
 */
int video_is_window_active()
{
    return window_active;
}













/* images */

/*
 * image_load()
 * Loads a image from a file.
 * Supported types: PNG, BMP, PCX, TGA
 */
image_t *image_load(char *path)
{
    image_t *img;
    char abs_path[1024];

    resource_filepath(abs_path, path, sizeof(abs_path), RESFP_READ);
    logfile_message("image_load(%s)", abs_path);

    if(NULL == (img=(image_t*)hash_search(path))) {
        img = mallocx(sizeof *img);    
        img->data = load_bitmap(abs_path, NULL);
        if(img->data) {
            img->w = img->data->w;
            img->h = img->data->h;
            hash_insert(path, (void*)img, DATATYPE_IMAGE); /* do NOT use abs_path on the hash! */
            logfile_message("image_load() ok");
        }
        else {
            logfile_message("image_load() error! %s", allegro_error);
            free(img);
            img = NULL;
        }
    }

    return img;
}


/*
 * image_save()
 * Saves a image to a file
 */
void image_save(image_t *img, char *path)
{
    char abs_path[1024];

    resource_filepath(abs_path, path, sizeof(abs_path), RESFP_WRITE);
    logfile_message("image_save(%p,%s)", img, abs_path);

    save_bitmap(abs_path, img->data, NULL);
}



/*
 * image_create()
 * Creates a new image of a given size
 */
image_t *image_create(int width, int height)
{
    image_t *img = mallocx(sizeof *img);

    img->data = create_bitmap(width, height);
    img->w = width;
    img->h = height;
    image_clear(img, image_rgb(0,0,0));

    return img;
}


/*
 * image_destroy()
 * Destroys an image
 */
void image_destroy(image_t *img)
{
    destroy_bitmap(img->data);
    free(img);
}


/*
 * image_getpixel()
 * Returns the pixel at the given position on the image
 */
uint32 image_getpixel(image_t *img, int x, int y)
{
    return getpixel(img->data, x, y);
}


/*
 * image_putpixel()
 * Plots a pixel into the given image
 */
void image_putpixel(image_t *img, int x, int y, uint32 color)
{
    putpixel(img->data, x, y, color);
}

/*
 * image_rgb()
 * Generates an uint32 color
 */
uint32 image_rgb(uint8 r, uint8 g, uint8 b)
{
    return makecol(r,g,b);
}


/*
 * image_clear()
 * Clears an given image with some color
 */
void image_clear(image_t *img, uint32 color)
{
    clear_to_color(img->data, color);
}


/*
 * image_blit()
 * Blits a surface onto another
 */
void image_blit(image_t *src, image_t *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height)
{
    blit(src->data, dest->data, source_x, source_y, dest_x, dest_y, width, height);
}


/*
 * image_draw()
 * Draws an image onto the destination surface
 * at the specified position
 *
 * flags: refer to the IF_* defines (Image Flags)
 */
void image_draw(image_t *src, image_t *dest, int x, int y, uint32 flags)
{
    if((flags & IF_HFLIP) && !(flags & IF_VFLIP))
        draw_sprite_h_flip(dest->data, src->data, x, y);
    else if(!(flags & IF_HFLIP) && (flags & IF_VFLIP))
        draw_sprite_v_flip(dest->data, src->data, x, y);
    else if((flags & IF_HFLIP) && (flags & IF_VFLIP))
        draw_sprite_vh_flip(dest->data, src->data, x, y);
    else
        draw_sprite(dest->data, src->data, x, y);
}



/*
 * image_draw_scaled()
 * Draws a scaled image onto the destination surface
 * at the specified position
 *
 * scale: (1.0, 1.0) is the original size
 *           (2.0, 2.0) stands for a double-sized image
 *           (0.5, 0.5) stands for a smaller image
 */
void image_draw_scaled(image_t *src, image_t *dest, int x, int y, v2d_t scale, uint32 flags)
{
    image_t *tmp;
    int w = (int)(scale.x * src->w);
    int h = (int)(scale.y * src->h);

    tmp = image_create(w, h);
    stretch_blit(src->data, tmp->data, 0, 0, src->w, src->h, 0, 0, w, h);
    image_draw(tmp, dest, x, y, flags);
    image_destroy(tmp);
}


/*
 * image_draw_rotated()
 * Draws a rotated image onto the destination bitmap at the specified position 
 *
 * ang: angle given in radians
 * cx, cy: pivot positions
 */
void image_draw_rotated(image_t *src, image_t *dest, int x, int y, int cx, int cy, double ang, uint32 flags)
{
    double conv = (-ang * (180.0/PI)) * (64.0/90.0);

    if((flags & IF_HFLIP) && !(flags & IF_VFLIP))
        pivot_sprite_v_flip(dest->data, src->data, x, y, cx, 0, ftofix((float)(conv+128)));
    else if(!(flags & IF_HFLIP) && (flags & IF_VFLIP))
        pivot_sprite_v_flip(dest->data, src->data, x, y, 0, cy, ftofix((float)conv));
    else if((flags & IF_HFLIP) && (flags & IF_VFLIP))
        pivot_sprite(dest->data, src->data, x, y, 0, 0, ftofix((float)(conv+128)));
    else
        pivot_sprite(dest->data, src->data, x, y, cx, cy, ftofix((float)conv));
}







/* private stuff */

/* fast2x_blit resizes the src image by a
 * factor of 2. It assumes that:
 *
 * src is a memory bitmap
 * dest is a previously created memory bitmap
 * dest->w == 2 * src->w
 * dest->h == 2 * src->h */
void fast2x_blit(BITMAP *src, BITMAP *dest, int bpp)
{
    int i, j;

    switch(bpp)
    {
        case 16:
            for(j=0; j<dest->h; j++) {
                for(i=0; i<dest->w; i++)
                    ((short*)dest->line[j])[i] = ((short*)src->line[j/2])[i/2];
            }
            break;

        case 8:
        case 24:
        case 32:
            /* TODO */
            break;
    }
}

/* this window is active */
void window_switch_in()
{
    window_active = TRUE;
}


/* this window is not active */
void window_switch_out()
{
    window_active = FALSE;
}
