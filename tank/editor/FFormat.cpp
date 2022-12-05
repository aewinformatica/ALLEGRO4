//		FFormat.cpp
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "allegro/internal/aintern.h"
//#include "tdgui.h"
#include "../scorch.h"
#include "../dialogs.h"
#include "editor.h"


/* The following are used by the billalert dialog */
#define A_S1  0
#define A_S2  1
#define A_S3  2
#define A_B1  3
#define A_B2  4
#define A_B3  5


static DIALOG agupalert_dialog[] =
{
   /* (dialog proc)				(x)   (y)   (w)   (h)   (fg)				(bg)			(key) (flags)  (d1)  (d2)  (dp) */
   { d_agup_window_proc,        0,    0,    0,    0,    0,					0,				0,    0,       0,    0,    NULL },
   { d_ctext_proc,				0,    0,    0,    0,    agup_fg_color,		agup_bg_color,  0,    0,       0,    0,    NULL },
   { d_ctext_proc,				0,    0,    0,    0,    agup_fg_color,		agup_bg_color,  0,    0,       0,    0,    NULL },
   { d_agup_button_proc,		0,    0,    0,    0,    0,					0,				0,    D_EXIT,  0,    0,    NULL },
   { d_agup_button_proc,		0,    0,    0,    0,    0,					0,				0,    D_EXIT,  0,    0,    NULL },
   { d_agup_button_proc,		0,    0,    0,    0,    0,					0,				0,    D_EXIT,  0,    0,    NULL },
   { NULL }
};

/*
  ==============================
  = BILL GUI dialogs           =
  ==============================
*/
char * MT = "";
/* Stolen directly from Allegro */
int agup_alert3(char *s1, char *s2, char *s3, char *b1, char *b2, char *b3, int c1, int c2, int c3)
{
   int maxlen = 0;
   int len1, len2, len3;
   int avg_w = text_length(font, " ");
   int avg_h = text_height(font);
   int buttons = 0;
   int b[3];
   int c;

   #define SORT_OUT_BUTTON(x) {                                            \
      if (b##x) {                                                          \
         agupalert_dialog[A_B##x].flags &= ~D_HIDDEN;                          \
         agupalert_dialog[A_B##x].key = c##x;                                  \
         agupalert_dialog[A_B##x].dp = b##x;                                   \
	 len##x = gui_strlen(b##x);                                        \
	 b[buttons++] = A_B##x;                                            \
      }                                                                    \
      else {                                                               \
         agupalert_dialog[A_B##x].flags |= D_HIDDEN;                           \
	 len##x = 0;                                                       \
      }                                                                    \
   }

   //agupalert_dialog[A_S1].dp = agupalert_dialog[A_S2].dp = agupalert_dialog[A_S3].dp =
   //                        agupalert_dialog[A_B1].dp = agupalert_dialog[A_B2].dp = "";

   // [Sean] - hack i threw in here to make this compile correctly with MSVC & Dev-C++
//#ifdef	USE_MSVC
   agupalert_dialog[A_S1].dp = MT;
//#else
//   (const char *)agupalert_dialog[A_S1].dp = "";
//#endif

   if (s1) {
      agupalert_dialog[A_S1].dp = s1;
      maxlen = text_length(font, s1);
   }

   if (s2) {
      agupalert_dialog[A_S2].dp = s2;
      len1 = text_length(font, s2);
      if (len1 > maxlen)
	 maxlen = len1;
   }

   if (s3) {
      agupalert_dialog[A_S3].dp = s3;
      len1 = text_length(font, s3);
      if (len1 > maxlen)
	 maxlen = len1;
   }

   SORT_OUT_BUTTON(1);
   SORT_OUT_BUTTON(2);
   SORT_OUT_BUTTON(3);

   len1 = MAX(len1, MAX(len2, len3)) + avg_w*3;
   if (len1*buttons > maxlen)
      maxlen = len1*buttons;

   maxlen += avg_w*4;
   agupalert_dialog[0].w = maxlen;
   agupalert_dialog[A_S2].x = agupalert_dialog[A_S3].x =
                                                agupalert_dialog[0].x + maxlen/2;

   agupalert_dialog[A_B1].w = agupalert_dialog[A_B2].w = agupalert_dialog[A_B3].w = len1;

   agupalert_dialog[A_B1].x = agupalert_dialog[A_B2].x = agupalert_dialog[A_B3].x =
                                       agupalert_dialog[0].x + maxlen/2 - len1/2;

   if (buttons == 3) {
      agupalert_dialog[b[0]].x = agupalert_dialog[0].x + maxlen/2 - len1*3/2 - avg_w;
      agupalert_dialog[b[2]].x = agupalert_dialog[0].x + maxlen/2 + len1/2 + avg_w;
   }
   else if (buttons == 2) {
      agupalert_dialog[b[0]].x = agupalert_dialog[0].x + maxlen/2 - len1 - avg_w;
      agupalert_dialog[b[1]].x = agupalert_dialog[0].x + maxlen/2 + avg_w;
   }

   agupalert_dialog[0].h = avg_h*8;
   agupalert_dialog[A_S2].y = agupalert_dialog[0].y + avg_h*2 +5;
   agupalert_dialog[A_S3].y = agupalert_dialog[0].y + avg_h*3 +5;
   agupalert_dialog[A_S2].h = avg_h;

   agupalert_dialog[A_B1].y = agupalert_dialog[A_B2].y = agupalert_dialog[A_B3].y =
                                                agupalert_dialog[0].y + avg_h*5;

   agupalert_dialog[A_B1].h = agupalert_dialog[A_B2].h = agupalert_dialog[A_B3].h = avg_h*2;

   centre_dialog(agupalert_dialog);

   clear_keybuf();

   do {
   } while (mouse_b);

   c = do_dialog(agupalert_dialog, A_B1);
   text_mode(-1);

   if (c == A_B1)
      return 1;
   else if (c == A_B2)
      return 2;
   else
      return 3;
}

int agup_alert(char *s1, char *s2, char *s3, char *b1, char *b2, int c1, int c2)
{
   int ret;

   ret = agup_alert3(s1, s2, s3, b1, b2, NULL, c1, c2, 0);

   if (ret > 2)
      ret = 2;

   return ret;
}



void savebmpchunk(BITMAP*	bmp, PACKFILE *f)
{
	int		x,y;
	int		color;

	for (x=0; x<bmp->w; x++)
	{
		for (y=0; y<bmp->h; y++)
		{
			// Get the color
			color = getpixel(bmp, x,y);

			// Store the data
			pack_iputw(color, f);
		}
	}
   return;
}




BITMAP	*loadbmpchunk(BITMAP *bmp, PACKFILE *f)
{
	int		x,y;
	int		color, map[200];

	if (!bmp)
		bmp = create_bitmap(SCREENWIDTH, SCREENHEIGHT);

	for (x=0; x<200; x++)map[x]=0;// Clear map

	for (x=0; x<bmp->w; x++)
		{
		for (y=0; y<bmp->h; y++)
			{
			// Get the color
			color = pack_igetw(f);
			if(color < 200)map[color]++;
			// Put the pixel into the bitmap	
			putpixel(bmp, x,y, color);
			}
		}

/*
unusedcolour is global (not good)
if it is set to a value less than 200 then it was probably set
in the last call to this routine.
 ie when reading background bmp don't use same colour
 as was selected for the foreground bmp.
 default code sets first pass to 0 ie black.

Setup like this so that calls expecting the original loadbmpchunk still work.
*/

	if(unusedcolour < 200)	 // don't use this colour.
		{
		map[unusedcolour]++;
		unusedcolour=TRANSCOLOR;	// set to transparent safety colour.
		}
	for (x=200;x>50;x--)
		if(map[x]==0)
			{
			unusedcolour=x;			// Set colour for legacy levels
			break;
			}

	return bmp;
}
