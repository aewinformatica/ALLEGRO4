/*
 * util.c - utilities
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "video.h"
#include "sprite.h"
#include "audio.h"



/* public variables */
volatile int game_over, fps;
volatile uint32 game_elapsed;
double game_dt;




/* Memory management */


/*
 * mallocx()
 * Similar to malloc(), but aborts the
 * program if it does not succeed.
 */
void *mallocx(size_t bytes)
{
    void *p = malloc(bytes);

    if(!p)
        fatal_error("FATAL ERROR: mallocx() failed.\n");

    return p;
}





/* Game routines */

/*
 * game_quit()
 * Quit game?
 */
void game_quit(void)
{
    game_over = TRUE;
}
END_OF_FUNCTION(game_quit)


/*
 * game_timer()
 * Return the number of msecs elapsed since the
 * game was launched
 */
uint32 game_timer()
{
    return (game_elapsed*10);
}


/*
 * game_update_timer()
 * Updates game_elapsed
 */
void game_update_timer()
{
    game_elapsed++;
}
END_OF_FUNCTION(game_update_timer)


/*
 * game_fps()
 * Frames per second
 */
int game_fps()
{
    return fps;
}


/*
 * game_delta()
 * Interval between two consecutive game cycles
 */
double game_delta()
{
    return game_dt;
}




/* 2D vectors */

/*
 * v2d_new()
 * Creates a v2d_t from its coordinates
 */
v2d_t v2d_new(double x, double y)
{
    v2d_t v = { x , y };
    return v;
}


/*
 * v2d_add()
 * Adds two vectors
 */
v2d_t v2d_add(v2d_t u, v2d_t v)
{
    v2d_t w = { u.x + v.x , u.y + v.y };
    return w;
}


/*
 * v2d_subtract()
 * Subtracts two vectors
 */
v2d_t v2d_subtract(v2d_t u, v2d_t v)
{
    v2d_t w = { u.x - v.x , u.y - v.y };
    return w;
}


/*
 * v2d_multiply()
 * Multiplies a vector by a scalar
 */
v2d_t v2d_multiply(v2d_t u, double h)
{
    v2d_t v = { h * u.x    , h * u.y };
    return v;
}


/*
 * v2d_magnitude()
 * Returns the magnitude of a given vector
 */
double v2d_magnitude(v2d_t v)
{
    return sqrt( (v.x*v.x) + (v.y*v.y) );
}


/*
 * v2d_dotproduct()
 * Dot product: u.v
 */
double v2d_dotproduct(v2d_t u, v2d_t v)
{
    return (u.x*v.x + u.y*v.y);
}


/*
 * v2d_rotate()
 * Rotates a vector. Angle in radians.
 */
v2d_t v2d_rotate(v2d_t v, double ang)
{
    double x = v.x, y = v.y;
    v2d_t w;

    w.x = x*cos(ang) - y*sin(ang);
    w.y = y*cos(ang) + x*sin(ang);

    return w;
}


/*
 * v2d_normalize()
 * The same thing as v = v / |v|,
 * where |v| is the magnitude of v.
 */
v2d_t v2d_normalize(v2d_t v)
{
    double m = v2d_magnitude(v);
    v2d_t w = (m > EPSILON) ? v2d_new(v.x/m,v.y/m) : v2d_new(0,0);
    return w;
}







/* Logfiles */
#define LOGFILE_PATH        "logfile.txt"
static FILE *logfile;

/*
 * logfile_init()
 * Initializes the logfile module.
 */
void logfile_init()
{
    char abs_path[1024];

    resource_filepath(abs_path, LOGFILE_PATH, sizeof(abs_path), RESFP_WRITE);

    if(NULL == (logfile = fopen(abs_path, "w")))
        logfile_message("WARNING: couldn't open %s for writing.\n", LOGFILE_PATH);
    else {
        logfile_message("%s version %d.%d.%d", GAME_TITLE, GAME_VERSION, GAME_SUB_VERSION, GAME_WIP_VERSION);
        logfile_message("logfile_init()");
    }
}


/*
 * logfile_message()
 * Prints a message on the logfile
 * (printf() format)
 */
void logfile_message(char *fmt, ...)
{
    char buf[2048];
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    fprintf(logfile ? logfile : stderr, "%s\n", buf);
    fflush(logfile ? logfile : stderr);

    /*printf("%s\n", buf);
    fflush(stdout);*/
}



/* 
 * logfile_release()
 * Releases the logfile module
 */
void logfile_release()
{
    logfile_message("logfile_release()");
    if(logfile)
        fclose(logfile);
}







/* Strings */

/*
 * str_to_hash()
 * Generates a hash key
 */
int str_to_hash(char *str)
{
    int hash = 0;
    char *p;

    for(p=str; *p; p++)
        hash = *p + (hash << 6) + (hash << 16) - hash;

    return hash;
}


/*
 * str_to_upper()
 * Upper-case convert
 */
char *str_to_upper(char *str)
{
    static char buf[1024];
    char *p;

    for(p=buf; *str; str++,p++)
        *p = toupper(*str);
    *p = '\0';

    return buf;
}



/*
 * str_to_lower()
 * Lower-case convert
 */
char *str_to_lower(char *str)
{
    static char buf[1024];
    char *p;

    for(p=buf; *str; str++,p++)
        *p = tolower(*str);
    *p = '\0';

    return buf;
}


/*
 * str_icmp()
 * Case-insensitive compare function. Returns
 * 0 if s1==s2, -1 if s1<s2 or 1 if s1>s2.
 */
int str_icmp(char *s1, char *s2)
{
    char *p, *q, a, b;

    for(p=s1,q=s2; *p && *q; p++,q++) {
        a = tolower(*p);
        b = tolower(*q);
        if(a < b)
            return -1;
        else if(a > b)
            return 1;
    }

    if(!*p && *q)
        return -1;
    else if(*p && !*q)
        return 1;
    else
        return 0;
}



/*
 * str_cpy()
 * Safe version of strcpy(). Returns a
 * pointer to dest.
 * If we have something like char str[32], then
 * the dest_size is 32, ie, sizeof(str)
 */
char* str_cpy(char *dest, char *src, size_t dest_size)
{
    int c;

    for(c=0; c<dest_size-1; c++) {
        dest[c] = src[c];
        if(dest[c] == 0) break;
    }
    dest[dest_size-1] = 0;

    return dest;
}



/*
 * str_trim()
 * Trim
 */
void str_trim(char *dest, char *src)
{
    char *p, *q, *t;

    for(p=src; *p && isspace(*p); p++);
    for(q=src+strlen(src)-1; q>p && isspace(*q); q--);
    for(q++,t=p; t!=q; t++) *(dest++) = *t;
    *dest = 0;
}



/* Hash table - data management */
#define HASH_INITIALCAPACITY            997
#define HASH_KEYMAXLEN                  256
typedef struct {
    char key[HASH_KEYMAXLEN];
    void *data;
    int type;
} hash_t;
static int hash_size, hash_capacity;
static hash_t *hash_table;
static void hash_init_ex()
{
    int i;

    hash_table = mallocx(hash_capacity * sizeof(hash_t));
    for(i=0; i<hash_capacity; i++) {
        hash_table[i].key[0] = '\0';
        hash_table[i].data = NULL;
        hash_table[i].type = DATATYPE_NONE;
    }
}




/*
 * hash_init()
 * Initializes the hash table
 */
void hash_init()
{
    logfile_message("hash_init()");
    hash_size = 0;
    hash_capacity = HASH_INITIALCAPACITY;
    hash_init_ex();
}



/*
 * hash_release()
 * Releases the hash table
 */
void hash_release()
{
    int i;

    logfile_message("hash_release()");
    for(i=0; i<hash_capacity; i++) {
        switch(hash_table[i].type) {
            case DATATYPE_IMAGE:
                image_destroy((image_t*)hash_table[i].data);
                break;

            case DATATYPE_SPRITE:
                spriteinfo_destroy((spriteinfo_t*)hash_table[i].data);
                break;

            case DATATYPE_MUSIC:
                music_destroy((music_t*)hash_table[i].data);
                break;

            case DATATYPE_SOUND:
                sound_destroy((sound_t*)hash_table[i].data);
                break;

            default:
                break;
        }
    }

    free(hash_table);
}



/*
 * hash_insert()
 * Inserts a new element into the hash table
 * (linear probing)
 */
void hash_insert(char *key, void *data, int type)
{
    int hc = hash_capacity;
    int h = ((str_to_hash(key)%hc)+hc) % hc, i = h;
    hash_t *old_hash;

    logfile_message("hash_insert('%s', %p, %d)...", key, data, type);
    if(strlen(key) >= HASH_KEYMAXLEN-1)
        logfile_message("Warning: %d = strlen(key) >= %d = HASH_MAXKEYLEN-1", strlen(key), HASH_KEYMAXLEN-1);

    while(hash_table[i].data) i = (i+1)%hash_capacity;
    strcpy(hash_table[i].key, key);
    hash_table[i].data = data;
    hash_table[i].type = type;

    if(++hash_size >= hash_capacity/2) {
        logfile_message("hash_insert() - Resizing hash table...");
        hash_capacity *= 2;
        hash_size = 0;
        old_hash = hash_table;
        hash_init_ex();
        for(i=0; i<hash_capacity/2; i++) {
            if(old_hash[i].data)
                hash_insert(old_hash[i].key, old_hash[i].data, old_hash[i].type);
        }
        free(old_hash);
    }
}



/*
 * hash_search()
 * Finds a key in the hash table. Returns NULL
 * if the given key isn't found.
 */
void* hash_search(char *key)
{
    int hc = hash_capacity;
    int h = ((str_to_hash(key)%hc)+hc) % hc, i;

    for(i=h; hash_table[i].data; i=(i+1)%hash_capacity) {
        if(strcmp(hash_table[i].key, key) == 0)
            return hash_table[i].data;
    }

    logfile_message("hash_search('%s') returned NULL", key);
    return NULL;
}




/* Other */


/*
 * bounding_box()
 * bounding box collision method
 * r[4] = x1, y1, x2(=x1+w), y2(=y1+h)
 */
int bounding_box(double a[4], double b[4])
{
    return (a[0]<b[2] && a[2]>b[0] && a[1]<b[3] && a[3]>b[1]);
}



/*
 * circular_collision()
 * Circular collision method
 * a, b = points to test
 * r_a = radius of a
 * r_b = radius of b
 */
int circular_collision(v2d_t a, double r_a, v2d_t b, double r_b)
{
    return ( v2d_magnitude(v2d_subtract(a,b)) <= r_a + r_b );
}



/*
 * swap_ex()
 * Swaps two variables. Use the
 * swap() macro instead of this.
 */
void swap_ex(void *a, void *b, size_t size)
{
    unsigned char *sa=a, *sb=b, c;
    size_t i;

    for(i=0; i<size; i++) {
        c = sa[i];
        sa[i] = sb[i];
        sb[i] = c;
    }
}





/*
 * fatal_error()
 * Displays a fatal error and aborts the application
 * (printf() format)
 */
void fatal_error(char *fmt, ...)
{
    char buf[1024];
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    logfile_message(buf);
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message(buf);
    exit(1);
}
