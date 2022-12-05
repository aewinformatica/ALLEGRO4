/*
I've modified the Allegro alpha example to show the differences 
between Allegro alpha stuff and AASTR2 alpha stuff.  

The following changes were made:

1.  I modified the #includes (line 32), changing 
"allegro.h" to <allegro.h> and adding "aastr.h"

1.  Just before the main loop (line 160), I added a call to 
convert_alpha() to convert the sprite to AASTR2 format.  

2.  Inside the main loop (line 167-171), I replaced the call to 
set_alpha_blender() with a call to aa_set_mode().  

3.  Inside the main loop, I replaced the call to 
draw_trans_sprite() with a call to aa_stretch_sprite().  

I left comments indicating which lines were new, and which were removed


*/

/*
 *    Example program for the Allegro library, by Shawn Hargreaves.
 *
 *    This program demonstrates how to use the 32 bit RGBA translucency
 *    functions to store an alpha channel along with a bitmap graphic.
 */


#include <allegro.h>
#include "aastr.h"


int main(int argc, char *argv[])
{
   char buf[256];
   BITMAP *background;
   BITMAP *alpha;
   BITMAP *sprite;
   BITMAP *buffer;
   int bpp = -1;
   int ret = -1;
   int x, y, c, a;

   allegro_init(); 
   install_keyboard(); 
   install_mouse(); 
   install_timer();

   /* what color depth should we use? */
   if (argc > 1) {
      if ((argv[1][0] == '-') || (argv[1][0] == '/'))
	 argv[1]++;
      bpp = atoi(argv[1]);
      if ((bpp != 15) && (bpp != 16) && (bpp != 24) && (bpp != 32)) {
	 allegro_message("Invalid color depth '%s'\n", argv[1]);
	 return 1;
      }
   }

   if (bpp > 0) {
      /* set a user-requested color depth */
      set_color_depth(bpp);
      ret = set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
   }
   else {
      /* autodetect what color depths are available */
      static int color_depths[] = { 16, 15, 32, 24, 0 };
      for (a=0; color_depths[a]; a++) {
	 bpp = color_depths[a];
	 set_color_depth(bpp);
	 ret = set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
	 if (ret == 0)
	    break;
      }
   }

   /* did the video mode set properly? */
   if (ret != 0) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Error setting %d bit graphics mode\n%s\n", bpp, allegro_error);
      return 1;
   }

   /* load the background picture */
   replace_filename(buf, argv[0], "allegro.pcx", sizeof(buf));
   background = load_bitmap(buf, NULL);
   if (!background) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Error reading %s!\n", buf);
      return 1;
   }

   /* make a copy of it */
   set_color_depth(32);
   sprite = create_bitmap(background->w, background->h);
   blit(background, sprite, 0, 0, 0, 0, background->w, background->h);

   /* load the alpha sprite image. Note that we specifically force this
    * to load in a 32 bit format by calling set_color_depth(). That is
    * because the disk file is actually only a 256 color graphic: if it
    * was already a 32 bit RGBA sprite, we would probably want to use
    * set_color_conversion(COLORCONV_NONE) instead.
    */
   replace_filename(buf, argv[0], "mysha.pcx", sizeof(buf));
   alpha = load_bitmap(buf, NULL);
   if (!alpha) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Error reading %s!\n", buf);
      return 1;
   }

   /* normally we would have loaded an RGBA image directly from disk. Since
    * I don't have one lying around, and am too lazy to draw one (or I could
    * rationalise this by saying that I'm trying to save download size by
    * reusing graphics :-) I'll just have to generate an alpha channel in
    * code. I do this by using greyscale values from the mouse picture as an
    * alpha channel for the Allegro image. Don't worry about this code: you
    * wouldn't normally need to write anything like this, because you'd just
    * get the right graphics directly out of a datafile.
    */
   drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
   set_write_alpha_blender();

   for (y=0; y<sprite->h; y++) {
      for (x=0; x<sprite->w; x++) {
	 c = getpixel(alpha, x, y);
	 a = getr(c) + getg(c) + getb(c);
	 a = MID(0, a/2-128, 255);

	 putpixel(sprite, x, y, a);
      }
   }

   destroy_bitmap(alpha);

   set_color_depth(bpp);

   /* darken the background image down a bit */
   drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
   set_multiply_blender(0, 0, 0, 255);
   rectfill(background, 0, 0, background->w, background->h, makecol(32, 16, 128));
   solid_mode();

   /* create a double buffer bitmap */
   buffer = create_bitmap(SCREEN_W, SCREEN_H);

   /* scale the background image to be the same size as the screen */
   stretch_blit(background, buffer, 0, 0, background->w, background->h, 0, 0, SCREEN_W, SCREEN_H);

   text_mode(-1);
   textprintf(buffer, font, 0, 0, makecol(255, 255, 255), "%dx%d, %dbpp", SCREEN_W, SCREEN_H, bpp);

   destroy_bitmap(background);
   background = create_bitmap(SCREEN_W, SCREEN_H);
   blit(buffer, background, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

   convert_alpha ( sprite );     //added for AASTR2 example

   while (!keypressed()) {
      /* draw the alpha sprite */
      x = mouse_x - sprite->w/2;
      y = mouse_y - sprite->h/2;

      //set_alpha_blender();                    //removed for AASTR2 example
      //draw_trans_sprite(buffer, sprite, x, y);//removed for AASTR2 example
      aa_set_mode(AA_ALPHA | AA_BLEND); //added for AASTR2 example
      aa_stretch_sprite(buffer, sprite, //added for AASTR2 example
		  x, y, sprite->w, sprite->h);

      /* flip it across to the screen */
      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

      /* replace the background where we drew the sprite */
      blit(background, buffer, x, y, x, y, sprite->w, sprite->h);
   }

   clear_keybuf();

   destroy_bitmap(background);
   destroy_bitmap(sprite);
   destroy_bitmap(buffer);

   return 0;
}

END_OF_MAIN();
