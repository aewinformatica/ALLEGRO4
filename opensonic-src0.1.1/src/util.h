/*
 * util.h - utilities
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

#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>
#include "global.h"
#include "osspec.h"



/* redefinitions */
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef round
#undef round
#endif


/* Useful macros */
#define randomize()             (srand(time(NULL)))
#define random(n)               ((int)(((double)rand() / ((double)(RAND_MAX)+(double)(1)))*(n)))
#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define round(x)                ((int)(((x)>0.0)?((x)+0.5):((x)-0.5)))
#define clip(val,a,b)           ( ((val)<min((a),(b))) ? min((a),(b)) : ( ((val)>max((a),(b))) ? max((a),(b)) : (val)  ) )
#define swap(a,b)               swap_ex(&(a), &(b), sizeof(a))


/* Game routines */
uint32 game_timer();
double game_delta();
int game_fps();
void game_quit(void);



/* Memory management */
void* mallocx(size_t bytes);



/* 2D vectors */
typedef struct {
    double x, y;
} v2d_t;
v2d_t v2d_new(double x, double y);
v2d_t v2d_add(v2d_t u, v2d_t v);
v2d_t v2d_subtract(v2d_t u, v2d_t v);
v2d_t v2d_multiply(v2d_t u, double h);
v2d_t v2d_rotate(v2d_t v, double ang); /* angle in radians */
v2d_t v2d_normalize(v2d_t v);
double v2d_magnitude(v2d_t v);
double v2d_dotproduct(v2d_t u, v2d_t v);


/* Logfiles */
void logfile_init();
void logfile_message(char *fmt, ...);
void logfile_release();


/* Strings */
int str_to_hash(char *str); /* generates a hash key */
char* str_to_upper(char *str); /* returns a pointer to a static variable */
char* str_to_lower(char *str); /* returns a pointer to a static variable */
int str_icmp(char *s1, char *s2); /* case-insensitive compare function */
char* str_cpy(char *dest, char *src, size_t dest_size); /* safe version of strcpy() */
void str_trim(char *dest, char *src); /* trim */



/* Hash table - data management */
#define DATATYPE_NONE                       0
#define DATATYPE_IMAGE                      1
#define DATATYPE_SPRITE                     2
#define DATATYPE_MUSIC                      3
#define DATATYPE_SOUND                      4
void hash_init();
void hash_release();
void hash_insert(char *key, void *data, int type);
void* hash_search(char *key);



/* Misc */
int bounding_box(double a[4], double b[4]);
int circular_collision(v2d_t a, double r_a, v2d_t b, double r_b);
void swap_ex(void *a, void *b, size_t size);
void fatal_error(char *fmt, ...);


#endif
