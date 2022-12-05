/* (C)2001 Robin Burrows! Playback demo 2 */

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

#define SWIDTH 640
#define SHEIGHT 480

int mapxoff = 0;		/* X and Y offset in pixels */
int mapyoff = 0;
int redraw = 0;			/* Needed if surfaces are lost */
int hwaccel = 0;		/* Enable hardware acceleration if available */
BITMAP * display1, * display2, * backbuffer;	/* For hardware page flipping */

void RenderInitDisplays (void)
{
/* Now draw a border to show that DrawMap clips properly */
	if (!hwaccel) {
		rect (backbuffer, 3, 3, SWIDTH-4, SHEIGHT-4, makecol (255, 255, 255));
		rect (backbuffer, 9, 9, SWIDTH-10, SHEIGHT-10, makecol (255, 255, 255));
	} else {
		acquire_bitmap (display1);
		clear (display1);
		rect (display1, 3, 3, SWIDTH-4, SHEIGHT-4, makecol (255, 255, 255));
		rect (display1, 9, 9, SWIDTH-10, SHEIGHT-10, makecol (255, 255, 255));
		release_bitmap (display1);
		acquire_bitmap (display2);
		clear (display2);
		rect (display2, 3, 3, SWIDTH-4, SHEIGHT-4, makecol (255, 255, 255));
		rect (display2, 9, 9, SWIDTH-10, SHEIGHT-10, makecol (255, 255, 255));
		release_bitmap (display2);
	}
}

void RedrawNeeded (void)
{
	redraw = 1;
}

int main (int argc, char *argv[])
{
BITMAP * escbmpt;		/* For hit ESC message */

	allegro_init();	/* Normal Allegro calls to start with */
	install_timer();
	install_keyboard();
	install_mouse();

/* Change set_color_depth to see maps change depth automagically... */
/* eg. set_color_depth (16); */
	if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) return -1;

	set_display_switch_callback (SWITCH_IN, RedrawNeeded);

	if (gfx_capabilities&GFX_HW_VRAM_BLIT && gfx_capabilities&GFX_HW_VRAM_BLIT_MASKED) {
		display1 = create_video_bitmap (SWIDTH, SHEIGHT);
		display2 = create_video_bitmap (SWIDTH, SHEIGHT);
		if (display1==NULL || display2==NULL) { MapFreeMem (); exit (0); }
		backbuffer = display2;
		show_video_bitmap (display1);
		if (bitmap_color_depth(screen)==8) set_palette (desktop_palette);
		alert ("Hardware Acceleration detected!", "Vroom, vroom", NULL, "&OK", NULL, 'o', 0);
/* Load the FMP file, exit if error */
		if (MapLoadVRAM ("TEST2.FMP")) { 
			allegro_message ("Can't find TEST2.FMP, please run exe from same folder");
			exit (0);
		}
		hwaccel = 1;
	} else {
		backbuffer = create_bitmap (SWIDTH, SHEIGHT);	/* Double buffer */
		if (backbuffer==NULL) { MapFreeMem (); exit (0); }
		clear (screen); clear (backbuffer);
		if (bitmap_color_depth(screen)==8) set_palette (desktop_palette);
		alert ("No Hardware Acceleration!", "Using software", NULL, "&OK", NULL, 'o', 0);
/* Load the FMP file, exit if error */
		if (MapLoad ("TEST2.FMP")) {
			allegro_message ("Can't find TEST2.FMP, please run exe from same folder");
			exit (0);
		}
		hwaccel = 0;
	}

	if (bitmap_color_depth(screen)==8) MapSetPal8 ();		/* Set palette */
	MapInitAnims ();					/* Reset animations */

/* Next, put '* Hit ESC to exit*' text on a BITMAP */
	escbmpt = create_bitmap (136, 8);
	if (escbmpt==NULL) { MapFreeMem (); exit (0); }
	clear (escbmpt); text_mode (-1);
		textout (escbmpt, font, "*Hit ESC to exit*", 0, 0, makecol (255, 255, 255));

	RenderInitDisplays ();

/* This is the main loop, which is repeated until SPACE or ESC is pressed */
	while (1)
	{
	if (!hwaccel) {
/* Sync in with the screen, and then copy the doublebuffer to display it (software) */
		vsync ();
		blit (backbuffer, screen, 0, 0, 0, 0, SWIDTH, SHEIGHT);
	} else {
/* Sync in with the screen, and then swap display bitmaps (hardware) */
		if (backbuffer==display1) {
			show_video_bitmap (display1); backbuffer = display2;
		} else {
			show_video_bitmap (display2); backbuffer = display1;
		}
	}

	if (redraw) {
		RenderInitDisplays ();
		MapRestore ();
		redraw = 0;
	}

/* Call this next function to update the animations */
		MapUpdateAnims ();

		if (key[KEY_RIGHT]) mapxoff+=2; if (mapxoff > (mapwidth*mapblockwidth-SWIDTH)) mapxoff = mapwidth*mapblockwidth-SWIDTH;
		if (key[KEY_LEFT]) mapxoff-=2; if (mapxoff < 0) mapxoff = 0;
		if (key[KEY_UP]) mapyoff-=2; if (mapyoff < 0) mapyoff = 0;
		if (key[KEY_DOWN]) mapyoff+=2; if (mapyoff > (mapheight*mapblockheight-SHEIGHT)) mapyoff = mapheight*mapblockheight-SHEIGHT;
		if (key[KEY_ESC]) break;
		if (key[KEY_SPACE]) break;

/* Here the Map Background layer is drawn... */
		MapDrawBG (backbuffer, mapxoff, mapyoff, 10, 10, SWIDTH-20, SHEIGHT-20);

/* Anything you want BETWEEN back and foreground goes in here... */
		masked_blit (escbmpt, backbuffer, 0, 0, SWIDTH/2-68, SHEIGHT-30, 136, 8);

/* Lastly, the foreground is drawn, you can call this 0 to 3 times, depending
 * on how many foreground layers you are using, or which you want displayed.
 * The last paramater is the foreground layer number, and can be 0, 1, or 2 */
		MapDrawFG (backbuffer, mapxoff, mapyoff, 10, 10, SWIDTH-20, SHEIGHT-20, 0);
	}

/* When ESC or SPACE has been pressed, it's time to clean up and leave */
	if (!hwaccel) destroy_bitmap (backbuffer);
	destroy_bitmap (escbmpt);
	MapFreeMem ();
	allegro_exit();
	return 0;
}
#ifdef END_OF_MAIN
END_OF_MAIN ();
#endif
