#include <allegro.h>
#include <stdlib.h>
#include <math.h>
#include "audio.h"
#include "timers.h"



#define BUFFER_SIZE    2048



int use_audio = 0;
int sound_volume = 64;
static AUDIOSTREAM *stream;



static void fill_buffer (unsigned char *buffer)
{
        int i, delay;
        float val, factor;
        int c;
        buffer [0] = 128;
        factor = rand () % 128;
        delay = 0;
        for (i = 1; i < BUFFER_SIZE; i++)
        {
                if (!delay)
                {
                        c = factor * 0.90 * (1 - factor);
                        c &= 0xFF;
                        factor = c;
                }
                buffer [i] = (int) c % 255;
                delay = (++delay) % 8;
        }
}



void init_audio ()
{
        if (!use_audio)
                return;
        stream = play_audio_stream (BUFFER_SIZE, 8, 0, 11025, sound_volume, 128);
        srand (11025);
}



void update_audio ()
{
        unsigned char *buffer;
        if (stream == NULL || !use_audio)
                return;
        buffer = get_audio_stream_buffer (stream);
        if (buffer)
        {
                fill_buffer (buffer);
                free_audio_stream_buffer (stream);
        }
}



void end_audio ()
{
        if (stream == NULL || !use_audio)
                return;
        stop_audio_stream (stream);
}

