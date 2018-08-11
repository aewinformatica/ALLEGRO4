#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro.h>

#include "music.h"
//#include "main.h"
#include "data.h"
int vol=130; // sound volume

void install_music(){
   /* install sound drivers */
   if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)!=0) {exit(1);}
   set_volume(vol,vol);
}
/*
void play_music()
{
    play_midi((MIDI*)game[MIDI001].dat, TRUE);
}
*/
void control_music(){
if (key[KEY_MINUS_PAD])  {vol=vol-10;if (vol<0) {vol=0;}
                         set_volume(vol,vol);
                         }
if (key[KEY_PLUS_PAD])  {vol=vol+10;if (vol>180) {vol=180;}
                         set_volume(vol,vol);
                         }
}
