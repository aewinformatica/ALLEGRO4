/* (C)2001 Robin Burrows - Isodemo */

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

#define SWIDTH 640
#define SHEIGHT 480
#define PLAYWIDTH (SWIDTH-20)
#define PLAYHEIGHT (SHEIGHT-20)

/* Sprite types */
#define SPR_PLAYER 1
#define SPR_ENEMY 2

/* Sprite structure */
typedef struct {
int type, xoff, yoff, xadd, yadd, delay;
/* You could expand this to add animation, hits and other info */
} ASPRITE;

static ASPRITE *** spritearray = NULL;	/* Wow, pointer to pointer to pointer */
static ASPRITE * playersprite = NULL;	/* Quick reference to find player */
int playerx, playery;
int scrlxoff = 0;		/* X and Y offset in pixels */
int scrlyoff = 0;
int redraw = 0;			/* Needed if surfaces are lost */
int hwaccel = 0;		/* Enable hardware acceleration if available */
BITMAP * display1, * display2, * backbuffer;	/* For hardware page flipping */
BITMAP * escbmpt;		/* For hit ESC message */

int GameInit (void)
{
int i, j;

	scrlxoff = 0; scrlyoff = 0;

/* Allocate memory for the spritearray and init all cells NULL */
	spritearray = (ASPRITE ***) malloc (mapheight*sizeof(ASPRITE **));
	if (spritearray == NULL) return -1;
	for (j=0;j<mapheight;j++) {
		spritearray[j] = (ASPRITE **) malloc (mapwidth*sizeof(ASPRITE *));
		if (spritearray[j] == NULL) return -1;
		for (i=0;i<mapwidth;i++) spritearray[j][i] = NULL;
	}

/* Switch to map layer 1 (sprite layer) and find all the sprites,
 * then allocate them into the sprite array
 */
	MapChangeLayer (1);
	for  (j=0;j<mapheight;j++) {
		for  (i=0;i<mapwidth;i++) {
			switch (MapGetBlock (i, j)->user1) {
				case SPR_ENEMY: /* Enemy */
					spritearray[j][i] = (ASPRITE *) malloc (sizeof (ASPRITE));
					if (spritearray[j][i] == NULL) return -1;
					spritearray[j][i]->type = SPR_ENEMY;
					spritearray[j][i]->xoff = 0;
					spritearray[j][i]->yoff = 0;
					spritearray[j][i]->xadd = 0;
					spritearray[j][i]->yadd = 0;
					spritearray[j][i]->delay = 0;
					break;
				case SPR_PLAYER: /* Player */
					spritearray[j][i] = (ASPRITE *) malloc (sizeof (ASPRITE));
					playersprite = spritearray[j][i];
					if (spritearray[j][i] == NULL) return -1;
					spritearray[j][i]->type = SPR_PLAYER;
					spritearray[j][i]->xoff = 0;
					spritearray[j][i]->yoff = 0;
					spritearray[j][i]->xadd = 0;
					spritearray[j][i]->yadd = 0;
					playerx = i;
					playery = j;
					break;
				default:
					break;
			}
		}
	}
	MapChangeLayer (0);

	if (playersprite == NULL) return -1;
	return 0;
}

void GameEnd (void)
{
int i, j;

/* Free up all the game resources allocated in GameInit */
	if (spritearray != NULL) {
		for (j=0;j<mapheight;j++) {
			if (spritearray[j] != NULL) {
				for (i=0;i<mapwidth;i++) {
					if (spritearray[j][i] != NULL) free (spritearray[j][i]);
				}
				free (spritearray[j]);
			}
		}
		free (spritearray); spritearray = NULL;
	}
}

void RowCallback (int cx, int cy, int dx, int dy)
{
int col = 0;

	if (spritearray[cy][cx] == NULL) return;
	if (spritearray[cy][cx]->type == SPR_PLAYER) col = makecol (255,255,0);
	if (spritearray[cy][cx]->type == SPR_ENEMY) col = makecol (255,0,0);

	dx += spritearray[cy][cx]->xoff;
	dy += spritearray[cy][cx]->yoff;
	rectfill (backbuffer, dx+4, dy-80, dx+60, dy+15, col);
}

void DrawSorted (void)
{
int i;

/* Keep player in the middle of the playarea,
 * But don't display anything outside the map array!
  */
	scrlxoff = ((playerx*mapblockgapx)+playersprite->xoff+(mapblockwidth/2))-(PLAYWIDTH/2);
	scrlyoff = (((playery/2)*mapblockgapy)+playersprite->yoff+(mapblockheight/2))-(PLAYHEIGHT/2);
	if (playery&1) { scrlxoff += mapblockstaggerx; scrlyoff += mapblockstaggery; }

	if (scrlxoff < 0) scrlxoff = 0;
	if (scrlyoff < 0) scrlyoff = 0;
	if (scrlxoff > ((mapwidth*mapblockgapx)-PLAYWIDTH))
		scrlxoff = ((mapwidth*mapblockgapx)-PLAYWIDTH);
	if (scrlyoff > (((mapheight/2)*mapblockgapy)-(PLAYHEIGHT-400)))
		scrlyoff = (((mapheight/2)*mapblockgapy)-(PLAYHEIGHT-400));

/* Here the Map Background layer is drawn... */
	MapDrawBG (backbuffer, scrlxoff, scrlyoff, 10, 10, SWIDTH-20, SHEIGHT-20);

	for (i=0;i<((SCREEN_H/(mapblockgapy/2))+14);i++) {
		MapDrawRow (backbuffer, scrlxoff, scrlyoff, 10, 10, SWIDTH-20, SHEIGHT-20, i, RowCallback);
	}

	masked_blit (escbmpt, backbuffer, 0, 0, SWIDTH/2-68, SHEIGHT-30, 136, 8);
	textout_centre (backbuffer, font, "Move in 8 directions with arrow keys",
		SWIDTH/2, SHEIGHT-50, makecol (255,255,255));
}

void MovePlayer (void)
{
int xdiff, ydiff;

	if (playersprite->xoff || playersprite->yoff) {
/* Move the sprite in the direction it is going */
		playersprite->xoff += playersprite->xadd;
		playersprite->yoff += playersprite->yadd;

/* If it has reached destination, stop it */
		if (playersprite->xoff == 0 && playersprite->yoff == 0) {
			playersprite->xadd = 0;
			playersprite->yadd = 0;
		}
	} else {
		xdiff = 0; ydiff = 0;
		if (key[KEY_LEFT]) xdiff = -1;
		if (key[KEY_RIGHT]) xdiff = 1;
		if (key[KEY_UP]) ydiff = -1;
		if (key[KEY_DOWN]) ydiff = 1;

		if (xdiff == 0 && ydiff == 0) return;

		if ((playerx+xdiff) < 2) return;
		if ((playerx+xdiff) >= (mapwidth-2)) return;
		if ((playery+ydiff) < 2) return;
		if ((playery+ydiff) >= (mapheight-2)) return;

		if (xdiff == 0) ydiff *= 2;
		if (ydiff != 0 && xdiff != 0) {
			if (!(playery&1)) {
				if (xdiff == 1) xdiff = 0;
			} else {
				if (xdiff == -1) xdiff = 0;
			}
		}

		if (spritearray[playery+ydiff][playerx+xdiff] != NULL) return;
		if (MapGetBlock(playerx+xdiff, playery+ydiff)->tl) return;

		spritearray[playery][playerx] = NULL;
		spritearray[playery+ydiff][playerx+xdiff] = playersprite;
		playery += ydiff; playerx += xdiff;

 		if (ydiff == 0) {
			playersprite->xoff = (xdiff* -62);
			playersprite->xadd = (xdiff* 2);
			return;
		}

		if (ydiff == -2 || ydiff == 2) {
			playersprite->yoff = ((ydiff/2)* -31);
			playersprite->yadd = (ydiff/2);
			return;
		}

		if ((playery&1)) {
			if (xdiff == 0) xdiff = 1;
		} else {
			if (xdiff == 0) xdiff = -1;
		}

		playersprite->xoff = (xdiff* -30);
		playersprite->xadd = (xdiff* 2);
		playersprite->yoff = (ydiff* -15);
		playersprite->yadd = (ydiff* 1);

	}
}

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
	allegro_init();	/* Normal Allegro calls to start with */
	install_timer();
	install_keyboard();
	install_mouse();

	set_color_depth (16);
	if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) {
		set_color_depth (15);
		if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) {
			set_color_depth (32);
			if (set_gfx_mode (GFX_AUTODETECT, SWIDTH, SHEIGHT, 0, 0)<0) {
				return -1;
			}
		}
	}

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
		if (MapLoadVRAM ("ISOTST.FMP")) { allegro_exit(); return 0; }
		hwaccel = 1;
	} else {
		backbuffer = create_bitmap (SWIDTH, SHEIGHT);	/* Double buffer */
		if (backbuffer==NULL) { MapFreeMem (); exit (0); }
		clear (screen); clear (backbuffer);
		if (bitmap_color_depth(screen)==8) set_palette (desktop_palette);
		alert ("No Hardware Acceleration!", "Using software", NULL, "&OK", NULL, 'o', 0);
/* Load the FMP file, exit if error */
		if (MapLoad ("ISOTST.FMP")) { allegro_exit(); return 0; }
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

	GameInit ();

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

		MovePlayer ();
		DrawSorted ();
		if (key[KEY_ESC]) break;
		if (key[KEY_SPACE]) break;
	}

/* When ESC or SPACE has been pressed, it's time to clean up and leave */
	GameEnd ();
	if (!hwaccel) destroy_bitmap (backbuffer);
	destroy_bitmap (escbmpt);
	MapFreeMem ();
	allegro_exit();
	return 0;
}
#ifdef END_OF_MAIN
END_OF_MAIN ();
#endif
