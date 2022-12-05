/*
 * audio.h - audio module
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

#ifndef _AUDIO_H
#define _AUDIO_H

#include <allegro.h>
#include <aldumb.h>

/* music management */
#define MUSIC_LOOPFOREVER -1
typedef struct {
    DUH *data;
} music_t;

music_t *music_load(char *path); /* will be unloaded automatically */
void music_destroy(music_t *music); /* usually you don't need this. */
void music_play(music_t *music, int loop); /* plays and loops the given music */
void music_stop();
void music_pause();
void music_resume();
void music_set_volume(double volume); /* 0.0 <= volume <= 1.0 (default) */
double music_get_volume();
int music_is_playing();


/* sample management */
typedef struct {
    SAMPLE *data;
    int voice_id;
} sound_t;

sound_t *sound_load(char *path); /* will be unloaded automatically */
void sound_destroy(sound_t *sample);
void sound_play(sound_t *sample);
void sound_play_ex(sound_t *sample, double vol, double pan, double freq, int loop); /* 0.0<=volume<=1.0; (left) -1.0<=pan<=1.0 (right); 1.0 = default frequency; 0 = no loops */
void sound_stop(sound_t *sample);
int sound_is_playing(sound_t *sample);

/* audio manager */
void audio_init(int nomusic);
void audio_update();
void audio_release();

#endif
