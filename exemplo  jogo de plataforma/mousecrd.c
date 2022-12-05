/* (C)2001 Robin Burrows! Playback demo mousecrd.c */

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
BITMAP * escbmpt;		/* For hit ESC message */
char mousettxt[80];

	allegro_init();	/* Normal Allegro calls to start with */
	install_timer();
	install_keyboard();
	install_mouse();

	if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) exit (0);

	if (MapLoad ("HEXTST.FMP")) {			/* Load the FMP file, exit if error */
		allegro_message ("Can't find HEXTST.FMP, please run exe from same folder");
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

/* This is the main loop, which is repeated until SPACE or ESC is pressed */
	while (1)
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
		if (key[KEY_ESC]) break;
		if (key[KEY_SPACE]) break;

/* Here the Map Background layer is drawn... */
		MapDrawBG (mapdisplaybuffer, mapxoff, mapyoff, 0, 0, SWIDTH, SHEIGHT);

/* Anything you want BETWEEN back and foreground goes in here... */
		blit (escbmpt, mapdisplaybuffer, 0, 0, SWIDTH/2-68, SHEIGHT-30, 136, 8);

/* Lastly, the foreground is drawn, you can call this 0 to 3 times, depending
 * on how many foreground layers you are using, or which you want displayed.
 * The last paramater is the foreground layer number, and can be 0, 1, or 2 */
		MapDrawFG (mapdisplaybuffer, mapxoff, mapyoff, 0, 0, SWIDTH, SHEIGHT, 0);

		text_mode (0);
		textout (mapdisplaybuffer, font, "X", mouse_x-3, mouse_y-3, makecol (255,255,255));
/* Find out exact block (pixel perfect!) */
		sprintf (mousettxt, "Mouse over block at %d, %d",
			MapGetXOffset (mouse_x+mapxoff, mouse_y+mapyoff),
			MapGetYOffset (mouse_x+mapxoff, mouse_y+mapyoff));
		textout (mapdisplaybuffer, font, mousettxt, 0, 0, makecol (255,255,255));
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

