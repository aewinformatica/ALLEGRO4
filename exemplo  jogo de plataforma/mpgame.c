/*
 * MPGAME - A test game to demonstrate the MappyAL playback library
 * By Robin Burrows <rburrows@bigfoot.com>
 * Mappy available from:
 * http://www.geocities.com/SiliconValley/Vista/7336/robcstf.htm
 * http://www.rbsite.freeserve.co.uk/robcstf.htm
 *
 * Don't forget to check out Molefest and Alien Epidemic!
 */

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"
#include "mpgame.h"

#define SWIDTH 320
#define SHEIGHT 240

int playerx = 80;		/* X and Y offset of player in pixels */
int playery = 700;		/* from top left of map */
BITMAP * mapdisplaybuffer;	/* Memory bitmap for double buffer */

/* This is an extremely basic collision detection routine, a proper game would
 * need a better one
 */
int Collision (int x, int y)
{
BLKSTR * blkdatapt;

	blkdatapt = MapGetBlock (x/mapblockwidth, y/mapblockheight);

	if (blkdatapt->tl) return 1; else return 0;
}

/* This simply removes an object once it's collected */
void ClearCell (int x, int y)
{
	MapSetBlock (x/mapblockwidth, y/mapblockheight, 0);
}

int main (void)
{
int quitit, mapxoff, mapyoff, oldpy, oldpx, playerfacing = 0, jump = 1234;
RGB blackrgb = { 0, 0, 0 };
BITMAP * escbmpt;		/* For hit ESC message */
BITMAP * playersprite;		/* Player graphic */
DATAFILE * mydata;

	allegro_init();	/* Normal Allegro calls to start with */
	install_timer();
	install_keyboard();

/* Try 16, 15 then 8 bit colour depths...
 * I recommend doing this if you are requesting a non 8bit screenmode
 * as some cards do NOT have high colour modes in some resolutions
 */
	set_color_depth (16);
	if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) {
		set_color_depth (15);
		if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) {
			set_color_depth (8);
			if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) {
				printf ("320*240 in 16, 15 or 8bit unavailable!\n");
				return -1;
			}
		}
	}

/* Normally it is not a good idea to keep your other data in the same
 * datafile as the FMP maps, as the maps just waste memory when you
 * use load_datafile. It's best to have two datafiles, one with
 * recognised allegro file formats, the other with maps. For this
 * example, I'm just using one to keep it simple.
 */
	mydata = load_datafile ("mpgame.dat");
	if (mydata == NULL) {
		allegro_message ("Can't find mpgame.dat, please run exe from same folder");
		exit (0);
	}

/* Hmmmm, slightly messy this, if in 8bit I need to set the palette,
 * then free the datafile, then load it back in to remap the truecolour
 * graphics to the now correct colour palette.
 * To make your palette, use smacker (excellent, www.smacker.com )
 * make sure colour 0 is pink!
 */
	if (bitmap_color_depth (screen)==8) {
		set_palette (mydata[GAMEPAL].dat);
		unload_datafile (mydata);
/* Load back in to remap graphics to palette */
		mydata = load_datafile ("mpgame.dat");
	}
	playersprite = (BITMAP *) mydata[PLAYER].dat;

/* Now load the map in, note you can put your FMP files in datafiles,
 * as well as just specifying the filename such as "MYMAP.FMP"
 * Just have them as type 'other', using compression can save diskspace
 */
	if (MapLoad ("mpgame.dat#GAMEMAP")) exit (0);	/* Load the FMP file, exit if error */

/* The next function is new, you don't have to call it, but it may
 * speed up some things and allows you to use maparraypt, eg:
 * if (maparraypt[y][x]==0) blah blah; see readmeal.txt
 */
	MapGenerateYLookup (); /* This can speed up things like collision checking */

/* Now all graphics are loaded, I can set colour 0 (the border) to
 * black if we are in 8bit
 */
	if (bitmap_color_depth(screen)==8) set_color (0, &blackrgb);

	MapInitAnims ();					/* Reset animations */

/* I'll use software scroll, which is good for lower resolutions,
 * so I need a memory bitmap for rendering to
 */
	mapdisplaybuffer = create_bitmap (SCREEN_W, SCREEN_H);	/* Double buffer */
	if (mapdisplaybuffer==NULL) { MapFreeMem (); exit (0); }
	clear (mapdisplaybuffer);

/* Next, put '* Hit ESC to exit*' text on a BITMAP */
	escbmpt = create_bitmap (136, 8);
	if (escbmpt==NULL) { destroy_bitmap (mapdisplaybuffer); MapFreeMem (); exit (0); }
	if (bitmap_color_depth(screen)==8) clear (escbmpt);
	else clear_to_color (escbmpt, makecol(255,0,255));
	text_mode (-1);
	textout (escbmpt, font, "*Hit ESC to exit*", 0, 0, makecol (255,255,255));

/* Now draw a border to show that DrawMap clips properly */
	rect (mapdisplaybuffer, 3, 3, SCREEN_W-4, SCREEN_H-4, makecol (255,255,255));
	rect (mapdisplaybuffer, 9, 9, SCREEN_W-10, SCREEN_H-10, makecol (255,255,255));

/* This is the main loop, which is repeated until SPACE or ESC is pressed */
	quitit = 0; while (!quitit)
	{
/* copy the doublebuffer to display it */
		vsync();
		blit (mapdisplaybuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

/* Call this next function to update the animations */
		MapUpdateAnims ();

/* This next bit is just to get some sort of game going :) */
		oldpy = playery; oldpx = playerx;
		if (key[KEY_RIGHT]) { playerfacing = 1; playerx+=2; }
		if (key[KEY_LEFT]) { playerfacing = 0; playerx-=2; }
		if (key[KEY_UP] && jump == 1234) jump = 24;
		if (jump!=1234) { playery -= jump/4; jump--; }
		else { if (!Collision (playerx+(playersprite->w/2), (playery+playersprite->h))) jump = 0; }
		if (jump<0) { if (Collision (playerx+(playersprite->w/2), (playery+playersprite->h)+2))
			{ jump = 1234; while (Collision (playerx+(playersprite->w/2), (playery+playersprite->h))) playery -= 2; } }
		if (!playerfacing) { if (Collision (playerx, playery+(playersprite->h-10))) playerx = oldpx; }
		else { if (Collision (playerx+playersprite->w, playery+(playersprite->h-10))) playerx = oldpx; }
		if (key[KEY_ESC]) quitit = 1;
		if (key[KEY_SPACE]) quitit = 1;

		mapxoff = (playerx+(playersprite->w/2)+10)-(SCREEN_W/2);
		mapyoff = (playery+(playersprite->h/2)+10)-(SCREEN_H/2);

/* Sanity check. MappyAL will Bomb out if you draw bits of map that aren't there... */
		if (mapxoff < 0) mapxoff = 0;
		if (mapxoff > (((mapwidth*mapblockwidth)-SCREEN_W)+20)) mapxoff = (((mapwidth*mapblockwidth)-SCREEN_W)+20);
		if (mapyoff < 0) mapyoff = 0;
		if (mapyoff > (((mapheight*mapblockheight)-SCREEN_H)+20)) mapyoff = (((mapheight*mapblockheight)-SCREEN_H)+20);

/* Here the Map Background layer is drawn...
 * I'm using BGT as it will ignore block 0 which should be faster than MapDrawBG in this case,
 * but more often MapDrawBG is faster, try and see
 */
		MapDrawBGT (mapdisplaybuffer, mapxoff, mapyoff, 10, 10, SCREEN_W-20, SCREEN_H-20);
/* Anything you want BETWEEN back and foreground goes in here... */
		if (playerfacing) draw_sprite (mapdisplaybuffer, playersprite, (playerx-mapxoff)+10, (playery-mapyoff)+10);
		else draw_sprite_h_flip (mapdisplaybuffer, playersprite, (playerx-mapxoff)+10, (playery-mapyoff)+10);
		draw_sprite (mapdisplaybuffer, escbmpt, SCREEN_W/2-68, SCREEN_H-30);
/* Lastly, the foreground is drawn, you can call this 0 to 3 times, depending
 * on how many foreground layers you are using, or which you want displayed.
 * The last paramater is the foreground layer number, and can be 0, 1, or 2 */
		MapDrawFG (mapdisplaybuffer, mapxoff, mapyoff, 10, 10, SCREEN_W-20, SCREEN_H-20, 0);
/* Change layer to objects, see if player touches, draw, then change back to layer 0 */
		if (MapChangeLayer (1) == 1) {
		if (Collision (playerx+(playersprite->w/2), playery+(playersprite->h-10)))
			ClearCell (playerx+(playersprite->w/2), playery+(playersprite->h-10));
		MapDrawFG (mapdisplaybuffer, mapxoff, mapyoff, 10, 10, SCREEN_W-20, SCREEN_H-20, 0);
		}
		MapChangeLayer (0);
	}

/* When ESC or SPACE has been pressed, it's time to clean up and leave */
	destroy_bitmap (mapdisplaybuffer);
	destroy_bitmap (escbmpt);
	unload_datafile (mydata);
	MapFreeMem ();
	allegro_exit();
	return 0;
}
#ifdef END_OF_MAIN
END_OF_MAIN()
#endif

