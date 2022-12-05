/*
 * audio.c - audio module
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
#include "audio.h"
#include "util.h"


/* music management */
static AL_DUH_PLAYER *dp; /* music player */
static int disable_music;
static int must_stop_music;
static int loop_total, loop_current;
static int loop_callback(void *data);
static double music_volume;



/*
 * music_load()
 * Loads a music from a file
 */
music_t *music_load(char *path)
{
    char abs_path[1024];
    music_t *m;

    resource_filepath(abs_path, path, sizeof(abs_path), RESFP_READ);
    logfile_message("music_load('%s')", abs_path);

    if(disable_music) {
        logfile_message("music_load(): musics are disabled.");
        return NULL;
    }

    if(NULL == (m=(music_t*)hash_search(path))) {
        m = mallocx(sizeof *m);
        if(NULL == (m->data = dumb_load_mod_quick(abs_path))) {
            if(NULL == (m->data = dumb_load_it_quick(abs_path))) {
                if(NULL == (m->data = dumb_load_xm_quick(abs_path))) {
                    if(NULL == (m->data = dumb_load_s3m_quick(abs_path))) {
                        logfile_message("music_load() error!");
                        free(m);
                        return NULL;
                    }
                }
            }
        }
        hash_insert(path, (void*)m, DATATYPE_MUSIC);
    }

    logfile_message("music_load() ok");
    return m;
}


/*
 * music_destroy()
 * Destroys a music. This is done automatically
 * while unloading the main hash.
 */
void music_destroy(music_t *music)
{
    if(!disable_music) {
        unload_duh(music->data);
        free(music);
    }
}


/*
 * music_play()
 * Plays the given music and loops [loop] times.
 * If [loop] == MUSIC_LOOPFOREVER, it loops forever.
 */
void music_play(music_t *music, int loop)
{
    DUH_SIGRENDERER *sig;
    DUMB_IT_SIGRENDERER *itsig;

    if(disable_music)
        return;

    if(music) {
        if(dp) al_stop_duh(dp);
        dp = al_start_duh(music->data, 1, 0, 1.0, 4096, 44100);

        loop_total = loop;
        loop_current = 0;
        must_stop_music = FALSE;

        sig = al_duh_get_sigrenderer(dp);
        itsig = duh_get_it_sigrenderer(sig);
        dumb_it_set_loop_callback(itsig, loop_callback, NULL);
        music_set_volume(1.0);
    }
}


/*
 * music_stop()
 * Stops the current music (if any)
 */
void music_stop()
{
    if(!disable_music && dp) {
        al_stop_duh(dp);
        dp = NULL;
    }
}


/*
 * music_pause()
 * Pauses the current music
 */
void music_pause()
{
    if(!disable_music && dp)
        al_pause_duh(dp);
}



/*
 * music_resume()
 * Resumes the current music
 */
void music_resume()
{
    if(!disable_music && dp)
        al_resume_duh(dp);
}


/*
 * music_set_volume()
 * Changes the volume of the current music.
 * 0.0 <= volume <= 1.0 (default)
 *
 * Note: the current music must be playing already!
 */
void music_set_volume(double volume)
{
    /* if we reduce the music volume a little bit,
     * the samples can be better heard by the user */
    volume = clip(volume, 0.0, 1.0);
    if(!disable_music && dp) {
        al_duh_set_volume(dp, (float)volume * 0.5);
        music_volume = volume;
    }
}


/*
 * music_get_volume()
 * Returns the volume of the current music.
 * 0.0 <= volume <= 1.0
 */
double music_get_volume()
{
    return ((!disable_music && dp) ? music_volume : 0.0);
}



/*
 * music_is_playing()
 * Returns TRUE if a music is playing, FALSE
 * otherwise.
 */
int music_is_playing()
{
    return (dp != NULL);
}







/* sound management */
#define SOUND_INVALID_VOICE         -1

/*
 * sound_load()
 * Loads a sample from a file
 */
sound_t *sound_load(char *path)
{
    char abs_path[1024];
    sound_t *s;

    resource_filepath(abs_path, path, sizeof(abs_path), RESFP_READ);

    if(NULL == (s=(sound_t*)hash_search(path))) {
        logfile_message("sound_load('%s')", abs_path);
        s = mallocx(sizeof *s);
        s->voice_id = SOUND_INVALID_VOICE;
        if(NULL == (s->data = load_sample(abs_path))) {
            logfile_message("sound_load() error! %s", allegro_error);
            free(s);
            return NULL;
        }
        hash_insert(path, (void*)s, DATATYPE_SOUND);
    }

    return s;
}


/*
 * sound_destroy()
 * Releases the given sample. This is done
 * automatically while releasing the main hash
 */
void sound_destroy(sound_t *sample)
{
    destroy_sample(sample->data);
    free(sample);
}


/*
 * sound_play()
 * Plays the given sample
 */
void sound_play(sound_t *sample)
{
    sound_play_ex(sample, 1.0, 0.0, 1.0, 0);
}


/*
 * sound_play_ex()
 * Plays the given sample with extra options! :)
 *
 * 0.0 <= volume <= 1.0
 * (left speaker) -1.0 <= pan <= 1.0 (right speaker)
 * 1.0 = default frequency
 * 0 = no loops
 */
void sound_play_ex(sound_t *sample, double vol, double pan, double freq, int loop)
{
    int id;

    if(sample) {
        vol = clip(vol, 0.0, 1.0);
        pan = clip(pan, -1.0, 1.0);
        freq = max(freq, 0.0);
        id = play_sample(sample->data, (int)(255.0*vol), min(255,128+(int)(128.0*pan)), (int)(1000.0*freq), loop);
        sample->voice_id = id < 0 ? SOUND_INVALID_VOICE : id;
    }
}



/*
 * sound_stop()
 * Stops a sample
 */
void sound_stop(sound_t *sample)
{
    if(sample)
        stop_sample(sample->data);
}


/*
 * sound_is_playing()
 * Checks if a given sound is playing or not
 */
int sound_is_playing(sound_t *sample)
{
    if(sample->voice_id != SOUND_INVALID_VOICE)
        return (voice_check(sample->voice_id) == sample->data);
    else
        return FALSE;
}










/* audio manager */

/*
 * audio_init()
 * Initializes the Audio Manager
 */
void audio_init(int nomusic)
{
    logfile_message("audio_init()");


    logfile_message("Initializing Allegro sound...");
    if(install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) < 0)
        logfile_message("Warning: unable to install sound.\n%s\n", allegro_error);


    dp = NULL;
    disable_music = nomusic;
    must_stop_music = FALSE;
    loop_total = loop_current = 0;
    music_volume = 1.0;
    if(!disable_music) {
        logfile_message("Initializing DUMB...");
        dumb_register_stdfiles();
        dumb_resampling_quality = 1; /* is DUMB slow? */
        dumb_it_max_to_mix = 64;
    }
    else
        logfile_message("The user has disabled the musics.");


    logfile_message("audio_init() ok");
}


/*
 * audio_release()
 * Releases the audio manager
 */
void audio_release()
{
    logfile_message("audio_release()");
    if(!disable_music) {
        if(dp) {
            al_stop_duh(dp);
            dp = NULL;
        }
        dumb_exit();
    }
    logfile_message("audio_release() ok");
}


/*
 * audio_update()
 * Updates the audio manager
 */
void audio_update()
{
    if(!disable_music) {
        if(must_stop_music) {
            music_stop();
            must_stop_music = FALSE;
        }

        if(dp)
            al_poll_duh(dp);
    }
}




/* private stuff */

/* controls the music loops */
int loop_callback(void *data)
{
    if(loop_current++ == loop_total)
        must_stop_music = TRUE;

    return 0;
}
