#include <allegro.h>

int main()
{  
   	SAMPLE *SomTiro = NULL;
           			SomTiro = load_sample("tiro.wav");
   	
    static int Buffer_Teclado =0;
    
   allegro_init();
   set_color_depth(32);
   install_keyboard();
   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
   install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

   

  

   int buffer_teclado = 0;

   

   while (!key[KEY_ESC])
   {
      if (buffer_teclado == 0)
      {
         if (key[KEY_SPACE])
         {
            play_sample(SomTiro,255,128,1000,FALSE);
            buffer_teclado=30;
         }
      }else buffer_teclado--;
      vsync();
   }
   stop_midi();
   
   delete SomTiro;
   allegro_exit();
   return 0;
}
END_OF_MAIN();
