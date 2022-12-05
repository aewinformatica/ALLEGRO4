/*
 * font.c - font module
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

/*
 * font_set_text() variables
 *
 * $PLAYER_SONIC - translates to \200 - displays sonic's face
 * $PLAYER_TAILS - translates to \201 - tails
 * $PLAYER_KNUCKLES - translates to \202 - knuckles
 *
 * See get_variable(...) for more variables.
 *
 * Example: font_set_text(fnt, "Knuckles $PLAYER_KNUCKLES said that he likes pizza.");
 */

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "font.h"
#include "sprite.h"
#include "video.h"


/* font internal data */
#define FONT_MAX            9 /* how many fonts do we have? */
typedef struct {
    image_t *ch[256];
} fontdata_t;


/* private data */
static fontdata_t fontdata[FONT_MAX];
static void get_font_size(font_t *f, int *w, int *h);
static char* get_variable(char *key);
static void expand_variables(char *str);
static void fix_wordwrap(char *str, int minlength, int maxlength);


/*
 * font_init()
 * Initializes the font module
 */
void font_init()
{
    int i, j;
    char sheet[32];
    char *p, alphabet[FONT_MAX][257] = {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*.:",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*.:",
        "0123456789:",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_´abcdefghijklmnopqrstuvwxyz{|}~\200\201\202"
    };

    logfile_message("font_init()");
    for(i=0; i<FONT_MAX; i++) {
        for(j=0; j<256; j++)
            fontdata[i].ch[j] = NULL;

        sprintf(sheet, "FT_FONT%d", i);
        for(p=alphabet[i],j=0; *p; p++,j++)
            fontdata[i].ch[(int)*p] = sprite_get_image(sprite_get_animation(sheet, 0), j);
    }
    logfile_message("font_init() ok");
}



/*
 * font_create()
 * Creates a new font object
 */
font_t *font_create(int type)
{
    int i;
    font_t *f = mallocx(sizeof *f);

    f->type = clip(type, 0, FONT_MAX-1);
    f->text[0] = 0;
    f->visible = TRUE;
    f->hspace = f->vspace = 1;
    for(i=0; i<FONT_MAXVALUES; i++)
        f->value[i] = 0;    

    return f;
}




/*
 * font_destroy()
 * Destroys an existing font object
 */
void font_destroy(font_t *f)
{
    free(f);
}




/*
 * font_set_text()
 * Sets the text...
 */
void font_set_text(font_t *f, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vsprintf(f->text, fmt, args);
    va_end(args);

    expand_variables(f->text);
}


/*
 * font_set_text_wordwrap()
 * Like font_set_text(), but this one includes the wordwrap feature
 */
void font_set_text_wordwrap(font_t *f, int minlength, int maxlength, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vsprintf(f->text, fmt, args);
    va_end(args);

    expand_variables(f->text);
    fix_wordwrap(f->text, minlength, maxlength);
}


/*
 * font_get_text()
 * Gets the text
 */
char *font_get_text(font_t *f)
{
    return f->text;
}



/*
 * font_render()
 * Render
 */
void font_render(font_t *f, v2d_t camera_position)
{
    int offx = 0, offy = 0, w, h;
    char *p;
    image_t *ch;

    get_font_size(f, &w, &h);
    if(f->visible) {
        for(p=f->text; *p; p++) {
            ch = fontdata[f->type].ch[(int)*p];
            if(*p != '\n') {
                if(ch) image_draw(ch, video_buffer, (int)(f->position.x+offx-(camera_position.x-VIDEO_SCREEN_W/2)), (int)(f->position.y+offy-(camera_position.y-VIDEO_SCREEN_H/2)), IF_NONE);
                offx += w + f->hspace;
            }
            else {
                offx = 0;
                offy += h + f->vspace;
            }
        }
    }
}


/*
 * font_get_charsize()
 * Returns the size of any character of a given font
 */
v2d_t font_get_charsize(font_t *fnt)
{
    int w, h;
    get_font_size(fnt, &w, &h);
    return v2d_new(w,h);
}






/* private functions */

/* if you want to know the size of the given font... */
void get_font_size(font_t *f, int *w, int *h)
{
    int i;
    image_t *ch;

    *w = *h = 0;
    for(i=0; i<256; i++) {
        if(NULL != (ch=fontdata[f->type].ch[i])) {
            *w = ch->w;
            *h = ch->h;
            return;
        }
    }
}


/* returns a static char* (case insensitive search) */
char* get_variable(char *key)
{
    /* since our table is very small,
     * we can perform a linear search */

    /* '$' symbol */
    if(str_icmp(key, "$") == 0 || str_icmp(key, "$$") == 0)
        return "$";

    /* newline */
    if(str_icmp(key, "$NEWLINE") == 0)
        return "\n";

    /* sonic, tails & knuckles faces */
    if(str_icmp(key, "$PLAYER_SONIC") == 0)
        return "\200";
    if(str_icmp(key, "$PLAYER_TAILS") == 0)
        return "\201";
    if(str_icmp(key, "$PLAYER_KNUCKLES") == 0)
        return "\202";

    /* input variables */
    if(str_icmp(key, "$INPUT_DIRECTIONAL") == 0)
        return "ARROW KEYS";
    if(str_icmp(key, "$INPUT_LEFT") == 0)
        return "LEFT KEY";
    if(str_icmp(key, "$INPUT_RIGHT") == 0)
        return "RIGHT KEY";
    if(str_icmp(key, "$INPUT_UP") == 0)
        return "UP KEY";
    if(str_icmp(key, "$INPUT_DOWN") == 0)
        return "DOWN KEY";
    if(str_icmp(key, "$INPUT_FIRE1") == 0)
        return "SPACE BAR";
    if(str_icmp(key, "$INPUT_FIRE2") == 0)
        return "LEFT CTRL KEY";
    if(str_icmp(key, "$INPUT_FIRE3") == 0)
        return "RETURN KEY";
    if(str_icmp(key, "$INPUT_FIRE4") == 0)
        return "ESC KEY";

    /* search failed */
    return "null";
}


/* expands the variables, e.g.,
 * 1) Please press the $INPUT_LEFT to go left
 * 2) Please press the LEFT CTRL KEY to go left */
void expand_variables(char *str)
{
    char buf[FONT_TEXTMAXLENGTH];
    char varname[FONT_TEXTMAXLENGTH];
    char *p=str, *q=buf, *r;

    while(*p) {
        /* looking for variables... */
        while(*p && *p != '$')
            *(q++) = *(p++);

        /* i've found a variable! */
        if(*p == '$') {
            /* detect the name of this variable */
            r = varname;
            while(*p && !isspace(*p))
                *(r++) = *(p++);
            *r = 0;

            /* put it into buf */
            for(r=get_variable(varname); *r; r++,q++)
                *q = *r;
        }
    }
    *q = 0;

    strcpy(str, buf);
}



/* fix_wordwrap() */
void fix_wordwrap(char *str, int minlength, int maxlength)
{
    char buf[1024];
    char *p, *q;
    int lc = 0; /* line counter */

    for(p=str,q=buf; *p; p++,q++) {
        if(isspace(*p) && lc >= minlength) {
            *q = '\n';
            lc = 0;
        }
        else if(lc >= maxlength) {
            *q = '\n';
            *(++q) = *p;
            lc = 0;
        }
        else {
            *q = *p;
            lc++;
        }
    }

    *q = 0;
    strcpy(str, buf);
}
