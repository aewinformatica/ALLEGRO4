/*
I've modified the Allegro scaling example to show how to switch 
from normal Allegro scaling to AASTR2 scaling.  

The following changes were made:

1.  Changed the include files.  I changed "allegro.h" to <allegro.h> and 
added an include for "aastr.h".  

2.  Changed stretch_blit to aa_stretch_blit on line 54.  

*/

/*
 *    Example program for the Allegro library, by Grzegorz Ludorowski.
 *
 *    This example demonstrates how to use pcx files, palettes and stretch
 *    blits. It loads a pcx file, sets its palette and does some random
 *    stretch_blits. Don't worry - it's VERY slowed down using vsync().
 */


#include <allegro.h>
#include "aastr.h"



int main(int argc, char *argv[])
{
   PALETTE my_palette;
   BITMAP *scr_buffer;
   char pcx_name[256];

   allegro_init();
   install_keyboard();
   if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
      return 1;
   }

#if !defined WIN32
   replace_filename(pcx_name, argv[0], "mysha.pcx", sizeof(pcx_name));
   scr_buffer = load_pcx(pcx_name, my_palette);
#else
   scr_buffer = load_pcx("mysha.pcx", my_palette);
#endif
   if (!scr_buffer) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Error loading %s!\n", pcx_name);
      return 1;
   }

   set_palette(my_palette);
   blit(scr_buffer, screen, 0, 0, 0, 0, scr_buffer->w, scr_buffer->h);

   while (!keypressed()) {
      aa_stretch_blit(scr_buffer, screen, 0, 0, rand()%scr_buffer->w,
		   rand()%scr_buffer->h, rand()%SCREEN_W, rand()%SCREEN_H,
		   rand()%SCREEN_W, rand()%SCREEN_H);
      vsync();
   }

   destroy_bitmap(scr_buffer);
   return 0;
}

END_OF_MAIN();
