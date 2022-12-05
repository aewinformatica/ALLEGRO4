/* (C)2001 Robin Burrows! Playback demo 1 */

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

#define SWIDTH 320		/* Standard VGA for this demo */
#define SHEIGHT 200

int mapxoff = 0;		/* X and Y offset in pixels */
int mapyoff = 0;
BITMAP * mapdisplaybuffer;	/* Memory bitmap for double buffer */

int main (void)
{
int quitit;
BITMAP * escbmpt;		/* For hit ESC message */

	allegro_init();	/* Normal Allegro calls to start with */
	install_timer();
	install_keyboard();

	if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) exit (0);
	if (MapLoad ("TEST.FMP")) {			/* Load the FMP file, exit if error */
		allegro_message ("Can't find TEST.FMP, please run exe from same folder");
		exit (0);
	}
	MapSetPal8 ();					/* Set palette */

	MapInitAnims ();					/* Reset animations */

	mapdisplaybuffer = create_bitmap (SWIDTH, SHEIGHT);	/* Double buffer */
	if (mapdisplaybuffer==NULL) { MapFreeMem (); exit (0); }
	clear (mapdisplaybuffer);

/* Next, put '* Hit ESC to exit*' text on a BITMAP */
	escbmpt = create_bitmap (136, 8);
	if (escbmpt==NULL) { destroy_bitmap (mapdisplaybuffer); MapFreeMem (); exit (0); }
	clear (escbmpt); text_mode (-1);
		textout (escbmpt, font, "*Hit ESC to exit*", 0, 0, makecol (255,255,255));

/* Now draw a border to show that DrawMap clips properly */
	rect (mapdisplaybuffer, 3, 3, SWIDTH-4, SHEIGHT-4, makecol (255,255,255));
	rect (mapdisplaybuffer, 9, 9, SWIDTH-10, SHEIGHT-10, makecol (255,255,255));

/* This is the main loop, which is repeated until SPACE or ESC is pressed */
	quitit = 0; while (!quitit)
	{
/* copy the doublebuffer to display it */
		vsync(); /* simple timing for this demo */
		blit (mapdisplaybuffer, screen, 0, 0, 0, 0, SWIDTH, SHEIGHT);

/* Call this next function to update the animations */
		MapUpdateAnims ();

		if (key[KEY_RIGHT]) mapxoff++; if (mapxoff > (75*16)) mapxoff = (75*16);
		if (key[KEY_LEFT]) mapxoff--; if (mapxoff < 0) mapxoff = 0;
		if (key[KEY_UP]) mapyoff--; if (mapyoff < 0) mapyoff = 0;
		if (key[KEY_DOWN]) mapyoff++; if (mapyoff > (80*16)) mapyoff = (80*16);
		if (key[KEY_ESC]) quitit = 1;
		if (key[KEY_SPACE]) quitit = 1;

/* Here the Map Background layer is drawn... */
/* For a really psychadelic experience, change the next line to MapDraw8BGT */
		MapDrawBG (mapdisplaybuffer, mapxoff, mapyoff, 10, 10, 300, 180);
/* Anything you want BETWEEN back and foreground goes in here... */
		masked_blit (escbmpt, mapdisplaybuffer, 0, 0, SWIDTH/2-68, SHEIGHT-30, 136, 8);
/* Lastly, the foreground is drawn, you can call this 0 to 3 times, depending
 * on how many foreground layers you are using, or which you want displayed.
 * The last paramater is the foreground layer number, and can be 0, 1, or 2 */
		MapDrawFG (mapdisplaybuffer, mapxoff, mapyoff, 10, 10, 300, 180, 0);
	}

/* When ESC or SPACE has been pressed, it's time to clean up and leave */
	destroy_bitmap (mapdisplaybuffer);
	destroy_bitmap (escbmpt);
	MapFreeMem ();
	allegro_exit();
	return 0;
}
#ifdef END_OF_MAIN
END_OF_MAIN()
#endif

