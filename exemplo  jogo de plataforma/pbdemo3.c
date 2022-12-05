/* (C)2001 Robin Burrows Playback demo 3 */

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

#define SWIDTH 320
#define SHEIGHT 200

int mapxoff = 670;		/* X and Y offset in pixels */
int mapyoff = 650;
int fps = 0;
int fpscounter = 0;
BITMAP * dbuffer;		/* Memory bitmap for double buffer */

void fpsinterrupt (void)
{
	fps = fpscounter; fpscounter = 0;
}
END_OF_FUNCTION (fpsinterrupt);

void DisplayFPS (void)
{
char fpstext[8];

	sprintf (fpstext, "FPS=%d", fps);
	text_mode (0);
	textout (dbuffer, font, fpstext, 0, 0, 255);
}

int main (void)
{
PALETTE pal;
int quitit;
BITMAP * escbmpt;		/* For hit ESC message */
BITMAP * parallaxbm, * tempbm;

	allegro_init();	/* Normal Allegro calls to start with */
	install_timer();
	install_keyboard();

	if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) exit (0);	/* Lovely VGA */
	if (MapLoad ("TEST.FMP")) {			/* Load the FMP file, exit if error */
		allegro_message ("Can't find TEST.FMP, please run exe from same folder");
		exit (0);
	}
	MapSetPal8 ();					/* Set palette */
	MapCorrectColours ();				/* Not needed for 8bit, but call anyway */

	MapInitAnims ();					/* Reset animations */
	tempbm = load_bitmap ("PARALLAX.BMP", pal);
	parallaxbm = NULL;
/* If loaded BMP OK, make it into a parallax bitmap */
	if (tempbm != NULL) parallaxbm = MapMakeParallaxBitmap (tempbm, 0);

	dbuffer = create_bitmap (SWIDTH, SHEIGHT);	/* Double buffer */
	if (dbuffer==NULL) { MapFreeMem (); exit (0); }
	clear (dbuffer);

/* Next, put '* Hit ESC to exit*' text on a BITMAP */
	escbmpt = create_bitmap (136, 8);
	if (escbmpt==NULL) { destroy_bitmap (dbuffer); MapFreeMem (); exit (0); }
	clear (escbmpt); text_mode (-1);
		textout (escbmpt, font, "*Hit ESC to exit*", 0, 0, makecol(255,255,255));

/* Now draw a border to show that DrawMap clips properly */
	rect (dbuffer, 3, 3, SWIDTH-4, SHEIGHT-4, makecol(255,255,255));
	rect (dbuffer, 9, 9, SWIDTH-10, SHEIGHT-10, makecol(255,255,255));

/* See allegro docs for interrupts... */
	LOCK_VARIABLE (fps); LOCK_VARIABLE (fpscounter); LOCK_FUNCTION (fpsinterrupt);
	install_int (fpsinterrupt, 1000);
/* This is the main loop, which is repeated until SPACE or ESC is pressed */
	quitit = 0; while (!quitit)
	{
/* No Vsync to see how fast we can go, copy the doublebuffer to display it */
		blit (dbuffer, screen, 0, 0, 0, 0, SWIDTH, SHEIGHT);

/* Call this next function to update the animations */
		MapUpdateAnims ();

		if (key[KEY_RIGHT]) mapxoff++; if (mapxoff > (75*16)) mapxoff = (75*16);
		if (key[KEY_LEFT]) mapxoff--; if (mapxoff < 0) mapxoff = 0;
		if (key[KEY_UP]) mapyoff--; if (mapyoff < 0) mapyoff = 0;
		if (key[KEY_DOWN]) mapyoff++; if (mapyoff > (80*16)) mapyoff = (80*16);
		if (key[KEY_ESC]) quitit = 1;
		if (key[KEY_SPACE]) quitit = 1;

		if (parallaxbm != NULL)
			MapDrawParallax (dbuffer, parallaxbm, mapxoff, mapyoff, 10, 10, 300, 180);
/* Here the Map Background layer is drawn... */
		MapDrawBGT (dbuffer, mapxoff, mapyoff, 10, 10, 300, 180);
/* Anything you want BETWEEN back and foreground goes in here... */
		masked_blit (escbmpt, dbuffer, 0, 0, SWIDTH/2-68, SHEIGHT-30, 136, 8);
/* Lastly, the foreground is drawn, you can call this 0 to 3 times, depending
 * on how many foreground layers you are using, or which you want displayed.
 * The last paramater is the foreground layer number, and can be 0, 1, or 2 */
		MapDrawFG (dbuffer, mapxoff, mapyoff, 10, 10, 300, 180, 0);
		fpscounter++; DisplayFPS ();
	}

/* When ESC or SPACE has been pressed, it's time to clean up and leave */
	remove_int (fpsinterrupt);
	destroy_bitmap (dbuffer);
	destroy_bitmap (escbmpt);
	MapFreeMem ();
	allegro_exit();
	return 0;
}
#ifdef END_OF_MAIN
END_OF_MAIN()
#endif

