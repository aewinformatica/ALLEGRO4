/* (C)2001 Robin Burrows! Proja demo */

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

#define SWIDTH 320
#define SHEIGHT 240

int mapxoff = 16;		/* X and Y offset in pixels */
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
	if (MapLoad ("PROJA.FMP")) {			/* Load the FMP file, exit if error */
		allegro_message ("Can't find PROJA.FMP, please run exe from same folder");
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

	mapyoff = (mapheight*mapblockheight)-(SHEIGHT*2);
/* This is the main loop, which is repeated until SPACE or ESC is pressed */
	quitit = 0; while (!quitit)
	{
/* copy the doublebuffer to display it */
		vsync(); /* simple timing for this demo */
		blit (mapdisplaybuffer, screen, 0, 0, 0, 0, SWIDTH, SHEIGHT);

		MapUpdateAnims ();

		if (mapyoff > 0) mapyoff--;
		if (key[KEY_ESC]) quitit = 1;
		if (key[KEY_SPACE]) quitit = 1;

		MapChangeLayer (0);
		MapDrawBG (mapdisplaybuffer, mapxoff/2, mapyoff/2, 0, 0, SWIDTH, SHEIGHT);
/* Anything you want BETWEEN back and foreground goes in here... */
		masked_blit (escbmpt, mapdisplaybuffer, 0, 0, SWIDTH/2-68, SHEIGHT-30, 136, 8);
		MapChangeLayer (1);
		MapDrawBGT (mapdisplaybuffer, mapxoff, mapyoff, 0, 0, SWIDTH, SHEIGHT);
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

