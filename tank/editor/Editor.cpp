//	Editor.cpp
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include <ctype.h>
#include "tdgui.h"
#include "../scorch.h"
#include "../dialogs.h"
#include "../particles.h"
#include "fblend.h"
#include "editor.h"

BITMAP	*editor_guibuffer;

// Level sprites
levelsprite_t		editorsprites[NUM_WOBJECTS];
int					   num_editorsprites;

// Universal file path
char	save_filename[256];
char	pic_filename[256];
char	music_filename[256];

// Quit editor flag, when this is true, close
// the editor down
bool	editorquitflag=false;
bool	editorrefresh=true;

//	Tank position array, can be changed dynamically
tank_placement		tankpos[MAX_TANKS];
int		tanksel;
//int		tankxpos[MAX_TANKS];
//int		tankypos[MAX_TANKS];

//	Extra information for levels
int		unusedcolour;
int		CONCRETE;		// color of concrete now dynmic not static.
int		SOLID;			// color of solid now dynmic not static.
int		GOLD;				// color of treasure.
int		goldX;			// x location of treasure.
int		goldY;			// y location of treasure.

// General
void	GetWaterLevel(void);

// For HUD display
void	drawHUD(BITMAP *b);
bool	showHUD = false;
char	HUDoff[] = "Show HUD";
char	HUDon[]  = "Hide HUD";

// For file-menu
int		newlevel_proc(void);
int		loadlevel_proc(void);
int		savelevel_proc(void);
int		dumplevel_proc(void);
int		quit_proc(void);

// For level-menu
int		selectforeground_proc(void);
int		selectbackground_proc(void);

int		setlevelinfo_proc(void);
int		waterlevelset_proc(void);
int		watercolorset_proc(void);
int		setlevelinfo_proc(void);
int   placewater_proc(void);
int		flowwater_proc(void);
int   clearwater_proc(void);
int		selectscript_proc(void);
int		gravitywall_proc(void);

// For sprites-menu
int		spriteselect_proc(void);
int		placetanks_proc(void);
int		clearsprites_proc(void);
int		drawHUD_proc(void);

// For music-menu
int		importsong_proc(void);
int		playsong_proc(void);
int		stopsong_proc(void);

// Keyboard commands
int	placewater(void);
int	clearwater(void);
int setwatercolour(void);
int flowwater(void);

MENU file_menu[] =
{
	{ "&New",						 newlevel_proc,		NULL, 0, NULL  },
	{ "&Load Level...",	 loadlevel_proc,	NULL, 0, NULL  },
	{ "&Save Level...",	 savelevel_proc,	NULL, 0, NULL  },
	{ "&Dump Level...", dumplevel_proc,	NULL, 0, NULL  },
	{ "",							   NULL,					  NULL, 0, NULL  },
	{ "&Return"		,				 quit_proc,				NULL, 0, NULL  },
	{ NULL,							 NULL,					  NULL, 0, NULL  }
};

MENU level_menu[] =
{
	{ "&Select Foreground...",			  selectforeground_proc,	NULL, 0, NULL  },
	{ "S&elect Background...",			  selectbackground_proc,	NULL, 0, NULL  },
	{ "&Wall Type / Gravity Options",	gravitywall_proc,				NULL, 0, NULL  },
	{ "Se&t Static Water Level",		  waterlevelset_proc,			NULL, 0, NULL  },
	{ "Set Water &Color",				      watercolorset_proc,			NULL, 0, NULL  },
	{ "Place Water",				          placewater_proc,				NULL, 0, NULL  },
	{ "Flow Water",				            flowwater_proc,					NULL, 0, NULL  },
	{ "Remove Water",				          clearwater_proc,				NULL, 0, NULL  },
	{ "",								NULL,					NULL, 0, NULL  },
	{ "&Modify Author / Level Info.",	setlevelinfo_proc,			NULL, 0, NULL  },
	{ NULL,								NULL,					NULL, 0, NULL  }
};

MENU sprites_menu[] =
{
	{ "&Sprite Selection Tool",	spriteselect_proc,	NULL, 0, NULL  },
	{ "&Clear All Sprites",		clearsprites_proc,	NULL, 0, NULL  },
	{ "",						NULL,				NULL, 0, NULL  },
	{ "&Place Tanks",			placetanks_proc,	NULL, 0, NULL  },
	{ "",						NULL,				NULL, 0, NULL  },
	{ HUDoff,			drawHUD_proc,	NULL, 0, NULL  },
	{ NULL,						NULL,				NULL, 0, NULL  }
};

MENU music_menu[] =
{
	{ "&Import Music",	importsong_proc,	NULL, 0, NULL  },
	{ "",				NULL,				NULL, 0, NULL  },
	{ "&Play Song",		playsong_proc,		NULL, 0, NULL  },
	{ "&Stop Song",		stopsong_proc,		NULL, 0, NULL  },
	{ NULL,				NULL,				NULL, 0, NULL  }
};

MENU editormain_menu[] =
{
   { "&File",			NULL,	file_menu,		0, NULL  },
   { "&Level",			NULL,	level_menu,		0, NULL  },
   { "S&prites",		NULL,	sprites_menu,	0, NULL  },
   { "&Music",			NULL,	music_menu,		0, NULL  },
   { NULL,				NULL,	NULL,			0, NULL  }
};

int	watercolorset_proc(void)
{
alert("NOTICE", "to set water color pres c and then click mouse on colour required.", "Understand?", "Yes", "No", 0, 0);
editorrefresh = true;
return	D_O_K;
}

int	placewater_proc(void)
{
alert("NOTICE", 0, "to place 100 pixels of water press w and then click mouse to place.", "Thanks", "I knew that", 0, 0);
editorrefresh = true;
return	D_O_K;
}

int	flowwater_proc(void)
{
alert("NOTICE", 0, "to cause the water to flow -- press f.", "OK", "I knew that", 0, 0);
editorrefresh = true;
return	D_O_K;
}

int	clearwater_proc(void)
{
alert("NOTICE", "to remove ALL water", "Press backspace", "OK", 0, 0, 0);
editorrefresh = true;
return	D_O_K;
}

int		d_agup_text(int msg, DIALOG *d, int c)
{
	if(msg == MSG_DRAW)
		gui_textout(screen, (char *)d->dp, d->x, d->y, agup_fg_color, FALSE);

	return D_O_K;
}


char *levelsprites_getter(int index, int *list_size)
{
if(index < 0)
	{
  *list_size = num_editorsprites;
  return NULL;
	}
else
  return editorsprites[index].name;
}
/*
DIALOG
   int x, y, w, h;               // position and size of the object
   int fg, bg;                   // foreground and background colors
   int key;                      // keyboard shortcut (ASCII code)
   int flags;                    // flags about the object state
   int d1, d2;                   // any data the object might require
   void *dp, *dp2, *dp3;         // pointers to more object data

*/
//
DIALOG editor_main[] =
{
    { d_agup_menu_proc,       0,    0,    0,    0,    255,  0,    0,    0,       0,    0,    editormain_menu,          NULL, NULL  },

	  // Level info dialog [1-8]
	  { d_agup_window_proc,		177,   149,  428,  147,   255,  0,    0,    (D_HIDDEN | D_DISABLED),       0,		 0,    (char *)"Level Information" },
	  { d_agup_edit_proc,       265,   177,  295,  20,    255,  0,    0,    (D_HIDDEN | D_DISABLED),       64-1,		 0,    currentlevel.name },
	  { d_agup_edit_proc,       265,   205,  295,  20,    255,  0,    0,    (D_HIDDEN | D_DISABLED),       64-1,		 0,    currentlevel.author },
	  { d_agup_edit_proc,       265,   249,  295,  20,    255,  0,    0,    (D_HIDDEN | D_DISABLED),       128-1,		 0,    currentlevel.leveldesc },
	  { d_agup_text,			184,   177,    0,    0,   agup_fg_color,  agup_bg_color,    0,    (D_HIDDEN | D_DISABLED),       0,			 0,    (char *)"Name" },
	  { d_agup_text,			184,   205,    0,    0,   agup_fg_color,  agup_bg_color,    0,    (D_HIDDEN | D_DISABLED),       0,			 0,    (char *)"Author" },
	  { d_agup_text,			    184,   249,    0,    0,   agup_fg_color,  agup_bg_color,    0,    (D_HIDDEN | D_DISABLED),       0,			 0,    (char *)"Brief Desc." },
	  { d_agup_button_proc,		536,   275,   58,   16,   255,  0,    0,  (D_HIDDEN | D_DISABLED | D_EXIT),  0,    0,    (char *)"Done" },

	  // Sprite selection dialog [9-12]
	  { d_agup_window_proc,		126,   124,  529,  358,   255,  0,    0,    (D_HIDDEN | D_DISABLED),          0,	  0,    (char *)"Sprite Selection" },
	  { d_agup_list_proc,		  147,   158,  482,  267,   255,  0,    0,    (D_HIDDEN | D_DISABLED),		      0,	  0,    (char *)levelsprites_getter },
	  { d_agup_button_proc,		358,   436,  133,   34,   255,  0,    0,    (D_HIDDEN | D_DISABLED | D_EXIT), 0,    0,    (char *)"Place" },
	  { d_agup_button_proc,		502,   436,  133,   34,   255,  0,    0,    (D_HIDDEN | D_DISABLED | D_EXIT), 0,    0,    (char *)"Cancel" },

	  // The wall type/gravity dialog [13-26]
	  { d_agup_window_proc,	125,   150,  500,  320,   255,  0,    0,    (D_HIDDEN | D_DISABLED),         0,	   0,    (char *)"Wall & Gravity Options" },
	  { d_agup_text,				167,   189,    0,    0, agup_fg_color,  agup_bg_color,    0,    (D_HIDDEN | D_DISABLED),       0,			 0,    (char *)"Wall Types" },
	  { d_agup_text,				167,   328,    0,    0, agup_fg_color,  agup_bg_color,    0,    (D_HIDDEN | D_DISABLED),       0,			 0,    (char *)"Gravity Options" },
	  { d_agup_radio_proc,	167,   210,  150,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 1,    0,	 (char *)"Normal Walls",    0,     0 },
	  { d_agup_radio_proc,	167,   237,  150,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 1,    0,	 (char *)"Rubber Walls",    0,     0 },
	  { d_agup_radio_proc,	167,   264,  240,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 1,    0,	 (char *)"Cement Walls",    0,     0 },
	  { d_agup_radio_proc,	167,   291,  240,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 1,    0,	 (char *)"Wrap-Around Walls",0,     0 },
	  { d_agup_check_proc,	167,   350,  240,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 0,    0,	 (char *)"No Dirt Gravity",  0,     0 },
	  { d_agup_check_proc,	167,   375,  240,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 0,    0,	 (char *)"No Tank Gravity",  0,     0 },
	  { d_agup_check_proc,	167,   400,  240,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 0,    0,	 (char *)"No Water Gravity", 0,     0 },
	  { d_agup_check_proc,	420,   210,  150,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 0,    0,	 (char *)"Use Concrete",		 0,     0 },
	  { d_agup_check_proc,	420,   237,  150,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 0,    0,	 (char *)"Use Solid",				 0,     0 },
	  { d_agup_check_proc,	420,   264,  150,   20,		0,  0,    0,    (D_HIDDEN | D_DISABLED),		 0,    0,	 (char *)"Have GOLD",				 0,     0 },
	  { d_agup_button_proc,	531,   428,   88,   20,   255,0,    0,    (D_HIDDEN | D_DISABLED | D_EXIT),0,    0,    (char *)"Done" },

    // water
    { d_keyboard_proc,		0,      0,    0,    0,     0,  0,    0,    0,                           KEY_W,      0,	  (void *)placewater,    NULL, NULL  },
	  { d_keyboard_proc,		0,      0,    0,    0,     0,  0,    0,    0,                           KEY_BACKSPACE,    0,	  (void *)clearwater,    NULL, NULL  },

    // Set Water Colour
		{ d_keyboard_proc,		0,   0,    0,   0,   0,  0,  0,   0,  KEY_C,    0,	(void *)setwatercolour,   NULL, NULL  },
    // Flow Water
		{ d_keyboard_proc,		0,   0,    0,   0,   0,  0,  0,   0,  KEY_F,    0,	(void *)flowwater,   NULL, NULL  },

    // need to place an ESC trap [31]
		{d_keyboard_proc,0,0,0,0,0,0,0,0,KEY_ESC,0,(void *)quit_proc,NULL,NULL}, // using NULL for quit_proc causes an abort

	  // Sprite selection dialog [32-35]
	  { d_agup_window_proc,		126,   124,  529,  358,   255,  0,    0,    (D_HIDDEN | D_DISABLED),          0,	  0,    (char *)"Treasure Chest Contents" },
	  { d_agup_list_proc,		  147,   158,  482,  267,   255,  0,    0,    (D_HIDDEN | D_DISABLED),		      0,	  0,    (char *)level_chest_prize },
	  { d_agup_button_proc,		358,   436,  133,   34,   255,  0,    0,    (D_HIDDEN | D_DISABLED | D_EXIT), 0,    0,    (char *)"Select" },
	  { d_agup_button_proc,		502,   436,  133,   34,   255,  0,    0,    (D_HIDDEN | D_DISABLED | D_EXIT), 0,    0,    (char *)"Cancel" },

	  { NULL,                 0,   0,    0,   0,   0,  0,  0,   0,       0,    0,  0,            0,     0 }

};

//
// setwatercolour - key press C to select colour under cursor when mouse clicked.
//
int	setwatercolour(void)
{
int c;
// User needs to click the mouse
if(!(mouse_b & 1)) return D_O_K;
c = getpixel(ground.bmp, mouse_x, mouse_y);

if(c == TRANSCOLOR)
		c = getpixel(background, mouse_x, mouse_y);

currentlevel.watercolor = c;
rest(150);

editorrefresh = true;
return D_O_K;
}

//
// placewater - Places 100 pixels on the ground whereever the
// mouse is at. Colour stays at current watercolor.
//
int	placewater(void)
{
	// User needs to click the mouse
	if(!(mouse_b & 1))
		return D_O_K;
	DropStaticWater(ground.bmp, mouse_x, mouse_y, currentlevel.watercolor, 50);
	rest(150);
	editorrefresh = true;
	return D_O_K;
}

//
// flowwater -- this actually moves the water particles
//  so they don't flow on first shot.
int   flowwater(void)
{
StaticWaterFall(pwater) ; // uses ground.bmp -- swap for editor
editorrefresh = true;
return D_O_K;
}

//
// clearwater - Not what it sounds like =)
// this actually removes all the water particles (in case you mess up)
int clearwater(void)
{
	pwater->DeleteAll();
	currentlevel.staticwaterlevel = 0;
	currentlevel.watercolor = makecol(0, 50, 255);
	editorrefresh = true;

	return D_O_K;
}

//
// set static water --  colour changes to watercolor
int	waterlevelset_proc(void)
{
	GetWaterLevel();
	editorrefresh = true;
	return D_O_K;
}

//
// default_foreground - creates a really crappy default foreground
//

void	default_foreground(void)
{
clear_to_color(ground.bmp, TRANSCOLOR);
rectfill(ground.bmp, 0, (SCREEN_H*0.98), SCREENWIDTH, SCREEN_H, makecol(0,100,0));
 CONCRETE = 150;	// give it a colour other than black or green
}

//
// DrawTankPos - Draws small indicators for the player spawn spots
//
void DrawTankPos(BITMAP	*buf)
{
	int		x,y,i;

	for(i=0; i<MAX_TANKS; i++)
	{
		x = (int)(ta[i].x-ta[i].sizex/2);
		y = (int)(ta[i].y-ta[i].sizey/2);
		rectfill(buf, x, y, x+ta[i].sizex, y+ta[i].sizey, makecol(ta[i].colorr, ta[i].colorg, ta[i].colorb));
		rect(buf, x, y, x+ta[i].sizex, y+ta[i].sizey, makecol(255,255,255));
	}
}

//
// EditorDeleteSprite - Deletes a sprite
//
extern	sprite_t*	lastsprite;

void EditorDeleteSprite(sprite_t*	t)
{

	sprite_t	*s = sprites;
	sprite_t	*prev = NULL;
	sprite_t	*temp = NULL;

	// Send the kill flag and cycle through the sprites
	// so this sprite is deleted immediatly
	t->Kill();

	while (s)
	{

		// See if the sprite is ready to be wiped out
		if(s->needstodie > 0)
			{
			// Only one element
			if(sprites == lastsprite)
			{
				delete sprites;
				sprites = lastsprite = NULL;

				numsprites = 0;
				return;
			}

			// Front of the list
			else if(s == sprites)
			{
				temp = s;
				s = sprites = sprites->next;

				numsprites--;
				delete temp;
			}

			// End of the list
			else if(s == lastsprite)
			{
				prev->next = NULL;
				lastsprite = prev;

				temp = s;
				s = s->next;

				numsprites--;
				delete temp;
			}

			// In the middle of the list
			else
			{
				if(prev)
					prev->next = s->next;
				else
					sprites = s->next;

				temp = s;
				s = s->next;

				numsprites--;
				delete temp;

			}

		}
		else		// Advance the sprite
		{
			prev = s;
			s = s->next;
		}
	}
}

//
// EditorSpriteDrawer - Draws all the current sprites on the screen
//
void	EditorSpriteDrawer(BITMAP *buf)
{
	// Draw the initial sprite
	G_SpriteDrawer(buf);
}

//
// PlaceSprite
//
void	PlaceSprite(sprite_t *s)
{
	BITMAP	*sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	// Clear mouse flags
	mouseflags = 0;

	// Hide the mouse
	show_mouse(NULL);

	float h = sbmp->h/2.0;
	float w = sbmp->w/2.0;
	float x,y;


	while (!(mouseflags & MOUSE_FLAG_RIGHT_DOWN))
		{
		// Set some coordinates
		s->x = mouse_x;
		s->y = mouse_y;
		if(s->x > 1000)s->x=0;  //  check the values are sane.
	  if(s->y > 1000)s->y=0;  //  negative numbers can wrap to 65k

    // Setup the x and y coords so that they point to the center of
    // the sprite
    if(s->flags & MF_CENTERCOORD)
      {
      x = s->x;
      y = s->y;
      }
    else // top left
      {
      x = s->x+w;
      y = s->y+h;
      }

		// Check if this object requires gravity, if so it immediatly
		// goes onto teh flewr </french_accent>
    if(s->flags & (MF_ONGROUND | MF_TPGRAVITY | MF_GROUNDCLIP) )
      {
			y=OnGround(x,y,ground.bmp)-h;//zaq1
			if(s->flags & MF_TPGRAVITY ) y +=5;// for towerpod
      }

    // Reset the s->x and s->y coords so that they point to the correct
    // spot on the sprite.
    if(s->flags & MF_CENTERCOORD)
      {
      s->x = x;
      s->y = y;
      }
    else // top left
      {
      s->x = x-w;
      s->y = y-h;
      }
		// Blit everything, do timer...etc
		EditorDrawer(buffer);

    rect(buffer, x-w, y-h, x+w, y+h, makecol(55,255,55));
		textout_centre(buffer, font, "Right click to place.", SCREENWIDTH/2, 2, makecol(255,200,200));

		// Draw the buffer to the screen
		blit(buffer, screen, 0,0,0,0, SCREENWIDTH, SCREEN_H);
		ticker();
		}

	editorrefresh = true;
}

int      spriteselected = -1;
//
// EditorSpriteThinker
//
void EditorSpriteThinker(BITMAP *buf)
{
	BITMAP			*sbmp;
	sprite_t*		s = sprites;
	int				color = makecol(255,0,0);
	int				colorselected = makecol(255,255,255);
	int				drawcolor = 0;
	bool			anyselected = false;

	while (s)
	{
		// Grab the current bitmap sprite
		sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	  float h = sbmp->h/2.0;
	  float w = sbmp->w/2.0;
    float x,y;
		char* pstr;
		char	str[200];

    // Setup the x and y coords so that they point to the center of
    // the sprite
    if(s->flags & MF_CENTERCOORD)
      {
      x = s->x;
      y = s->y;
      }
    else
      {
      x = s->x+w;
      y = s->y+h;
      }

		if(GUI_MouseInside(x-w, y-h, x+w, y+h))
      {
			if(mouse_b & 1)
				PlaceSprite(s);

			// Delete a sprite!
			if(key[KEY_DEL])
        {
				EditorDeleteSprite(s);
				editorrefresh = true;
				return;
        }
			drawcolor = colorselected;
			anyselected = true;
      if(spriteselected != s->wobj)
				rectfill(buf, 220, 0, 780, 20, 0);// blot out old text
			pstr = wobjects[s->wobj].name;
			if(s->wobj == WO_TREASURECHEST)
				{
				sprintf(str,"%s [%s]",wobjects[s->wobj].name, &prize_list[s->misc2][5] );
				pstr = str;
				}
		 	textout_centre(buf, font, pstr, 500, 2, makecol(255,255,255));
		 	spriteselected = s->wobj;
      }
		else
			{
//      rectfill(buf, 400, 0, 600, 20, 0);// blot out old text
      drawcolor = color;
			}

		// Draw the rectangle -- centre coordinates
		rect(buf, x-w, y-h, x+w, y+h, drawcolor);

		s = s->next;
	}
	if(!anyselected) // if nothing selected
    {
		if(spriteselected != -1) // is there old text?
			rectfill(buf, 220, 0, 780, 20, 0);// then blot it out.
		spriteselected = -1;
		}
}

//
// EditorDrawer - Finishes blitting and draws to the screen
//
void EditorDrawer(BITMAP	*buf)
{
	// Draw our background and foreground
	draw_sprite(buf, background, 0,0);
	draw_sprite(buf, ground.bmp, 0,0);

	// Draw some active sprites
	EditorSpriteDrawer(buf);

	// Draw the tanks current location
	DrawTankPos(buf);

	if(currentlevel.staticwaterlevel > 0)
	{
		BITMAP	*ewater = create_bitmap(SCREENWIDTH, SCREEN_H-currentlevel.staticwaterlevel);
		clear_to_color(ewater, currentlevel.watercolor);

		fblend_trans(ewater, buf, 0, currentlevel.staticwaterlevel, 100);

		destroy_bitmap(ewater);
	}
	// Draw static water
	pwater->Draw(1, buf);
	if(showHUD) drawHUD(buf);

}

//
// InitEditorSprites - Builds a list of sprites that we can use based
// on the world objects
//
void	InitEditorSprites(void)
{
	int		i;

	num_editorsprites = 0;

	for(i=0; i<NUM_WOBJECTS; i++)
	{
		// To tell what can be used in the list and what cannot be used
		editorsprites[num_editorsprites].wobj = -1;

		if(wobjects[i].editoruse)
		{
			ustrcpy(editorsprites[num_editorsprites].name, wobjects[i].name);
			editorsprites[num_editorsprites].wobj = i;

			num_editorsprites++;
		}

	}
}

//
// InitEditor - Starts the editor and sets up the bitmaps
// ready for editing.
//
void InitEditor(void)
{
	// Setup the particle system
	InitParticles();

	Init_Dialog_Stuff();

	// Init our two main bitmaps
	if(background==NULL)background = create_bitmap(SCREENWIDTH, SCREEN_H);
	if(ground.bmp==NULL)ground.bmp = create_bitmap(SCREENWIDTH, SCREEN_H);
	if(editor_guibuffer==NULL)editor_guibuffer = create_bitmap(SCREENWIDTH, SCREEN_H);

	// Clear them to black
	clear_to_color(background, makecol(0,0,0));
	default_foreground();
	clear_to_color(editor_guibuffer, TRANSCOLOR);

	// Set our quit flag
	editorquitflag = false;
	editorrefresh = true;

	// Default new level
	newlevel_proc();

	// Setup sprite system
	InitEditorSprites();

	// Setup random tank positions
	InitTankPlacement(ground.bmp);
}

//
// InitTankPlacement - Finds random spots on the map to place
// tanks...kinda as a default
//
void	InitTankPlacement(BITMAP	*bmp)
{
	int			i;

	for(i=0; i<MAX_TANKS; i++)
	{
		ta[i].y = rand()%SCREENHEIGHT;
		ta[i].FindGoodXPOS();
		ta[i].sizex = 20;
		ta[i].sizey = 10;
		ta[i].index = rand()%10;

		tankpos[i].x = (int)ta[i].x;
		tankpos[i].y = (int)ta[i].y;

		// Hammer it down
		ta[i].Onground(bmp);
	}
}

//
// GroundSprites - All sprites that have gravity are automatically bound to the ground
//
void	GroundSprites(BITMAP *bmp)
{
	sprite_t*		s = sprites;

	while (s)
	{
		if(!(s->flags & MF_NOGRAVITY))
			s->y=OnGround(s->x,s->y,bmp); // zaq1

		s = s->next;
	}
}

//
// GatherTankPosArray - Gathers all the tank X,Y coords into the tankxpos & tankypos array
//
void	GatherTankPosArray(void)
{
	int		i;

	for(i=0; i<MAX_TANKS; i++)	//	update currentlevel why??
	{
		currentlevel.tankxpos[i] = (int)ta[i].x;
		currentlevel.tankypos[i] = (int)ta[i].y;
	}
}

//
// GetNewXPos - Waits for the user to click the mouse button to indicate
// a new X position for our tank (doubled buffered)
//
void	GetNewXPos(void)
{
	mouseflags = 0;

	show_mouse(NULL);
	while (!(mouseflags & MOUSE_FLAG_RIGHT_DOWN))
	{
		// Clipping from FindGoodXPos
		if(mouse_x <= 10)
		{ mouse_x = 10; }
		if(mouse_x >= (SCREENWIDTH-14))
		{ mouse_x = SCREENWIDTH-14; }

		ta[tanksel].x = mouse_x;
		ta[tanksel].y = mouse_y;
		ta[tanksel].Onground(ground.bmp);

		// Blit everything, do timer...etc
		EditorDrawer(buffer);
		draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
		//show_mouse(buffer);

		textout_centre(buffer, font, "Right click to release", SCREENWIDTH/2, 2, makecol(255,255,255));

		// Draw the buffer to the screen
		blit(buffer, screen, 0,0,0,0, SCREENWIDTH, SCREEN_H);

		ticker();
	}

	editorrefresh = true;
	tanksel = -1;

	// Gather the new coordinates
	GatherTankPosArray();
}


//
// GetNewXPos - Waits for the user to click the mouse button to indicate
// a new X position for our tank (doubled buffered)
//
void	GetWaterLevel(void)
{
	mouseflags = 0;

	show_mouse(NULL);

	while (!(mouseflags & MOUSE_FLAG_RIGHT_DOWN))
	{
		// Use the mouse to set the water level
		currentlevel.staticwaterlevel = mouse_y;

		// Blit everything, do timer...etc
		EditorDrawer(buffer);

		// Draw the line
		line(buffer, 0, currentlevel.staticwaterlevel, SCREENWIDTH, currentlevel.staticwaterlevel, makecol(255,0,0));

		textout_centre(buffer, font, "Right click = end", SCREENWIDTH/2, 2, makecol(255,255,255));

		// Draw the buffer to the screen
		blit(buffer, screen, 0,0,0,0, SCREENWIDTH, SCREEN_H);

		ticker();
	}

	editorrefresh = true;
}

//
// CheckMouseSelection
//

void	CheckMouseSelection(void)
{
	int		i,tankx,tanky;

	// Checks to see if the user has clicked on a tank posistion
	// If so, than modify the cursor sprite and allow them to move
	// to the correct position
	for(i=0; i<MAX_TANKS; i++)
	{
		tankx = (int)(ta[i].x-ta[i].sizex/2);
		tanky = (int)(ta[i].y-ta[i].sizey/2);

		// Inside a tank spot? Clicked the button?
		if(GUI_MouseInside(tankx, tanky, tankx+ta[i].sizex, tanky+ta[i].sizey) &&
			(mouse_b & 1) &&
			tanksel != i)
		{

			tanksel = i;

			GetNewXPos();

			editorrefresh = true;

			return;
		}
	}
}


//
// CDEdit - Main loop
//

void CDEdit(void)
{
	DIALOG_PLAYER	*plyr;

	// Make sure we're not already in a game
	if(gamestate != GS_TITLE)
	{
		if(alert("Alert", "This will end the current game.", "Continue Anyway?", "Yes", "No", 0, 0)==2)
			return;
	}

	// First thing is first!
	InitEditor();

	// Set the game state
	gamestate = GS_EDITOR;

	// Set agup_buffer
	agup_buffer = screen;

	// Init our dialog
	plyr = init_dialog(editor_main, -1);

	while (update_dialog(plyr))
	{
		if(editorrefresh)
		{
			show_mouse(NULL);
			EditorDrawer(buffer);
			editorrefresh=false;

			// Draw the buffer to the screen
			blit(buffer, screen, 0,0,0,0, SCREENWIDTH, SCREEN_H);

			broadcast_dialog_message(MSG_DRAW, 0);
			show_mouse(screen);
		}

		EditorSpriteThinker(screen);
		CheckMouseSelection();

	}

	show_mouse(NULL);
}





///////////////////////////////////////////////////////////////////
////////////////// M E N U   F U N C T I O N S ////////////////////


//
// newlevel_proc - Ripped about 2/3 from InitEditor() thats all
//
int		newlevel_proc(void)
{
	// Free the previous bitmaps first
	destroy_bitmap(background);
	destroy_bitmap(ground.bmp);
	destroy_bitmap(editor_guibuffer);

	// Clear out all sprites
	G_DestroyAllSprites();

	// Init our two main bitmaps
	background = create_bitmap(SCREENWIDTH, SCREEN_H);
	ground.bmp = create_bitmap(SCREENWIDTH, SCREEN_H);
	editor_guibuffer = create_bitmap(SCREENWIDTH, SCREEN_H);
	currentlevel.staticwaterlevel = 0;
	currentlevel.watercolor = makecol(0, 50, 255);

	// Setup the level desc flags
	sprintf(currentlevel.name, "Untitled");
	sprintf(currentlevel.author, "Undefined");
	sprintf(currentlevel.leveldesc, "No info given.");
	currentlevel.flags = 0;

	// Set default wall & gravity options
	currentlevel.walltype = WALLS_NORMAL;
	currentlevel.gravityflags = NOGRAVITY_NORMAL;

	// Make sure the water buffer is clear
	clearwater();

	// Clear them to black
	clear_to_color(background, makecol(0,0,0));
	default_foreground();
	clear_to_color(editor_guibuffer, TRANSCOLOR);

	InitTankPlacement(ground.bmp);

	// Set our quit flag
	editorquitflag = false;
	editorrefresh = true;

	// Stop any playing songs
	// Music is played from menu selection..
	StopLevelOGG();

	return D_O_K;
}


//
// loadlevel_proc - Prompts for a file and then loads
//

int		loadlevel_proc(void)
{
	int		ret;
	int		border=200;

	// Default path
	ret = file_select_ex("Load Level (*.CDL)",
		save_filename,
		"CDL",
		sizeof(save_filename),  //		128*10,
		SCREENWIDTH-border,SCREEN_H-border);

	// Cancel button was clicked
	if(ret == 0)
		return D_O_K;

	// If the file exists, prompt the user for overwrite
	if(!exists(save_filename))
	{
		alert("Error", "File not foundz0rz!", "", "Ok", NULL, 0,0);
		return D_O_K;
	}

	// Bring up default settings
	newlevel_proc();

	// Load das level
	loadgamelevel(save_filename);

	// Request to refresh the screen
	editorrefresh = true;
	return D_O_K;
}

//
// saveleve_proc - Prompts for a filename and then saves
//

int		savelevel_proc(void)
{
	int		ret;
	int		border=200;

	// Default path
begin:
	ret = file_select_ex("Save level (*.CDL)",
		save_filename,
		"CDL",
		sizeof(save_filename),  //		128*10,
		SCREENWIDTH-border,SCREEN_H-border);

	// Cancel button was clicked
	if(ret == 0)
		return D_O_K;

	// If the file exists, prompt the user for overwrite
	if(exists(save_filename))
	{
		ret = alert("Question...of the sort", "That file already exists!", "Overwrite?", "Yes", "No", 0,0);

		// Ask for another name
		if(ret == 2)
			goto begin;
	}

	// Save
	savelevel(save_filename);

	// Request to refresh the screen
	editorrefresh = true;

	return D_O_K;
}

//
// dumplevel_proc - Prompts for a filename and then saves level in parts
//

int		dumplevel_proc(void)
{
int		ret;
int		border=200;

// Default path
char path_buffer[_MAX_PATH];
char drive[_MAX_DRIVE];
char dir[_MAX_DIR];
char fname[_MAX_FNAME];
char ext[_MAX_EXT];

   _splitpath( save_filename, drive, dir, fname, ext ); // C4996
   _makepath( path_buffer, drive, dir, fname, "txt" );
begin:
	ret = file_select_ex("Break Up level (*.TXT)",
		path_buffer,
		"txt",
		sizeof(path_buffer),
		SCREENWIDTH-border,SCREEN_H-border);

	// Cancel button was clicked
	if(ret == 0)
		return D_O_K;

	// If the file exists, prompt the user for overwrite
	if(exists(path_buffer))
		{
		ret = alert("Creating a text file", "of a level that already exists!", "Write?", "Yes", "No", 0,0);

		// Ask for another name
		if(ret == 2)
			goto begin;
		}

	// Save
	dumplevel(path_buffer);

	// Request to refresh the screen
	editorrefresh = true;

	return D_O_K;
}

//
// quit_proc - Quit the editor and head back to the main menu
//

int		quit_proc(void)
{
	// Stop any playing songs
	StopLevelOGG();

	// Release bitmap memory used for the editor
	destroy_bitmap(background);
	destroy_bitmap(ground.bmp);
	destroy_bitmap(editor_guibuffer);
	background = NULL;
	ground.bmp = NULL;
	editor_guibuffer = NULL;

	// Bring up the main menu
	gamestate = GS_TITLE;
	GUI_SetupMenu (main_menu);
	PlayLevelOGG("title.mp3");

	return D_EXIT;
}


//
// selectforeground_proc - Prompts for a filename and then loads a foreground
//

int		selectforeground_proc(void)
{
	BITMAP	*loadbmp;
	PALETTE	pal;
	int		ret;
	int		border=200;

	// Default path
	ret = file_select_ex("Select a foreground (*.BMP)",
		pic_filename,
		"BMP",
		sizeof(pic_filename),
		SCREENWIDTH-border,SCREEN_H-border);

	// Cancel button was clicked
	if(ret == 0)
		return D_O_K;

	// Attempt to load the bitmap
	loadbmp = load_bmp(pic_filename, pal);

	if(!loadbmp)
	{
		alert("Error", "Error loading file.", NULL, "Ok", NULL, 0,0);
		return D_O_K;
	}

	// Make sure we have the correct size
	if(loadbmp->w != SCREENWIDTH || loadbmp->h != SCREEN_H)
	{
		ret = alert("Error", "The bitmap's dimensions are not 800x600", "Use anyway?", "Yes", "No", 0,0);
		if(ret == 2)
			return D_O_K;
	}

	// Everything passed, go ahead and draw the file onto the foreground
	clear_to_color(ground.bmp, TRANSCOLOR);
	draw_sprite(ground.bmp, loadbmp, 0,0);

	// If a new foreground has been selected, re-arrange the tanks
	for(int i=0; i<MAX_TANKS; i++)	ta[i].Onground(ground.bmp);

	GroundSprites(ground.bmp);

	// Free the memory
	destroy_bitmap(loadbmp);

	// Request to refresh the screen
	editorrefresh = true;

	return D_O_K;
}



//
// prompts the user for a background selection
//

int		selectbackground_proc(void)
{
	BITMAP	*loadbmp;
	PALETTE	pal;
	int		ret;
	int		border=200;

	if(GOLD != 0)
		{
		ret = alert("WARNING", "A new background may destroy any treasure prizes", "Continue?", "Yes", "No", 0,0);
		if(ret == 2) return D_O_K;
		}

	// Default path
	ret = file_select_ex("Select a background (*.BMP)",
		pic_filename,
		"BMP",
		sizeof(pic_filename),
		SCREENWIDTH-border,SCREEN_H-border);

	// Cancel button was clicked
	if(ret == 0)
		return D_O_K;

	// Attempt to load the bitmap
	loadbmp = load_bmp(pic_filename, pal);

	if(!loadbmp)
	{
		alert("Error", "Error loading file.", NULL, "Ok", NULL, 0,0);
		return D_O_K;
	}

	// Make sure we have the correct size
	if(loadbmp->w != SCREENWIDTH || loadbmp->h != SCREEN_H)
	{
		char *fname = get_filename(pic_filename);
		ret = alert(fname, "The bitmap's dimensions are not 800x600", "Use anyway?", "Yes", "No", 0,0);
		if(ret == 2)
			return D_O_K;
	}

	// Everything passed, go ahead, clear and draw the file onto the foreground
	clear_to_color(background, makecol(0,0,0));
	draw_sprite(background, loadbmp, 0,0);

	// Free the memory
	destroy_bitmap(loadbmp);

	// Request to refresh the screen
	editorrefresh = true;

	return D_O_K;
}

int		spriteselect_proc(void)
{
	sprite_t	*s;
	int			i, ret;
	//int			spritex,spritey;
	_wobject	newwobj;

	// Un-hide the rest of our controls
	for(i=9; i<=12; i++)
		{ editor_main[i].flags = 0; }
	editor_main[11].flags = D_EXIT;
	editor_main[12].flags = D_EXIT;
	editor_main[0].flags = (D_HIDDEN | D_DISABLED);

	// Run das dialog
	ret = do_agup_dialog(editor_main, screen, NULL);

	// Hide the controls again
	for(i=9; i<=12; i++)
		{ editor_main[i].flags = (D_HIDDEN | D_DISABLED); }
	editor_main[0].flags = 0;

	// Are we placing a sprite?
	if(ret == 11)
		{
		// Find out what the new sprite is
		newwobj = (_wobject)editor_main[10].d1;

		// Create a new sprite for us to work with
		s = G_SpawnWO(newwobj, mouse_x, mouse_y);
		s->SetState(wobjects[editorsprites[newwobj].wobj].icon);
		s->flags &= ~MF_NOTVISABLE;  // make visible in editor
		// Allow the user to place the sprite in a nice little loop
		PlaceSprite(s);
		if(newwobj == WO_TREASURECHEST)
			{ 
			s->misc2 = get_chest_prize(s->misc2); // return the index into prize_list
			Printf("PlaceSprite  %d\n",s->misc2 ); 
			if(GOLD == 0)GOLD = 1; //	flag we have gold
			}
		}

	editorrefresh = true;

	return D_O_K;
}

int		clearsprites_proc(void)
{
G_DestroyAllSprites();
editorrefresh = true;
return D_O_K;
}

int		gravitywall_proc(void)
{
	int		i;

	// Un-hide the rest of our controls
	for(i=13; i<=26; i++)
	{ editor_main[i].flags = 0; }
	editor_main[26].flags = D_EXIT;
	editor_main[0].flags = (D_HIDDEN | D_DISABLED);

	// Set the options (wall type first)
	if(currentlevel.walltype == WALLS_NORMAL)
		editor_main[16].flags |= D_SELECTED;
	else if(currentlevel.walltype == WALLS_RUBBER)
		editor_main[17].flags |= D_SELECTED;
	else if(currentlevel.walltype == WALLS_CEMENT)
		editor_main[18].flags |= D_SELECTED;
	else if(currentlevel.walltype == WALLS_WRAP)
		editor_main[19].flags |= D_SELECTED;

	// Set menu gravity options
	if(currentlevel.gravityflags & NOGRAVITY_GROUND)
		editor_main[20].flags |= D_SELECTED;
	if(currentlevel.gravityflags & NOGRAVITY_TANKS)
		editor_main[21].flags |= D_SELECTED;
	if(currentlevel.gravityflags & NOGRAVITY_WATER)
		editor_main[22].flags |= D_SELECTED;
	if(currentlevel.gravityflags & NOGRAVITY_CONCRETE)
		editor_main[23].flags |= D_SELECTED;
	if(currentlevel.gravityflags & NOGRAVITY_SOLID)
		editor_main[24].flags |= D_SELECTED;
	if(currentlevel.gravityflags & NOGRAVITY_GOLD)
		editor_main[25].flags |= D_SELECTED;

	// Set to nothing
	currentlevel.gravityflags = currentlevel.walltype = 0;

	// Run das dialog
	do_agup_dialog(editor_main, screen, NULL);

	// Set the options (wall type first)
	if(editor_main[16].flags & D_SELECTED)
		currentlevel.walltype = WALLS_NORMAL;
	else if(editor_main[17].flags & D_SELECTED)
		currentlevel.walltype = WALLS_RUBBER;
	else if(editor_main[18].flags & D_SELECTED)
		currentlevel.walltype = WALLS_CEMENT;
	else if(editor_main[19].flags & D_SELECTED)
		currentlevel.walltype = WALLS_WRAP;

	// Set the new gravity options
	if(editor_main[20].flags & D_SELECTED)
		currentlevel.gravityflags |= NOGRAVITY_GROUND;
	if(editor_main[21].flags & D_SELECTED)
		currentlevel.gravityflags |= NOGRAVITY_TANKS;
	if(editor_main[22].flags & D_SELECTED)
		currentlevel.gravityflags |= NOGRAVITY_WATER;
	if(editor_main[23].flags & D_SELECTED)
		currentlevel.gravityflags |= NOGRAVITY_CONCRETE;
	if(editor_main[24].flags & D_SELECTED)
		currentlevel.gravityflags |= NOGRAVITY_SOLID;
	if(editor_main[25].flags & D_SELECTED)
		currentlevel.gravityflags |= NOGRAVITY_GOLD;
	
	if(currentlevel.gravityflags & NOGRAVITY_GOLD)
		GOLD = FAST_GETPIXEL(ground.bmp,1,0);
	else GOLD = 0;

	// Hide the controls again
	for(i=13; i<=26; i++)
		{ editor_main[i].flags = (D_HIDDEN | D_DISABLED); }
	editor_main[0].flags = 0;

	editorrefresh = true;

	return D_O_K;
}


char *level_chest_prize(int index, int *list_size)
{
if(index < 0)
	{
  *list_size = num_prizes;
  return NULL;
	}
else
  return &prize_list[index][4];
}

int get_chest_prize(int old)
{		//		return the index into prize_list from dialog
int	i,j,k;

	// Un-hide the rest of our controls
	for(i=32; i<=35; i++)
		{ editor_main[i].flags = 0; }
	editor_main[33].flags = D_EXIT; // double click
	editor_main[34].flags = D_EXIT; // select
	editor_main[35].flags = D_EXIT; // cancel
	editor_main[0].flags = (D_HIDDEN | D_DISABLED);

	// Run das dialog
	editor_main[33].d1 = old;
	k = do_agup_dialog(editor_main, screen, NULL);

	// Hide the controls again
	for(i=32; i<=35; i++)
		{ editor_main[i].flags = (D_HIDDEN | D_DISABLED); }
	editor_main[0].flags = 0;

	if(k == 35)	return -2;	//	cancel

	// we click ok?
	i = editor_main[33].d1;
	Printf("Prize %s\n", prize_list[i] );
	return i;
}


int		setlevelinfo_proc(void)
{
	int		i;

	// Un-hide the rest of our controls
	for(i=1; i<=8; i++)
	{ editor_main[i].flags = 0; }
	editor_main[8].flags = D_EXIT;
	editor_main[0].flags = (D_HIDDEN | D_DISABLED);

	// Run das dialog
	do_agup_dialog(editor_main, screen, NULL);

	// Hide the controls again
	for(i=1; i<=8; i++)
	{ editor_main[i].flags = (D_HIDDEN | D_DISABLED); }
	editor_main[0].flags = 0;

	editorrefresh = true;

	return D_O_K;
}

int		placetanks_proc(void)
{
	InitTankPlacement(ground.bmp);
	editorrefresh= true;
	return D_O_K;
}

//
// selectforeground_proc - Prompts for a filename and then loads a foreground
//

int		importsong_proc(void)
{
	int		ret;
	int		border=200;

	get_executable_name(music_filename, sizeof(music_filename));

	sprintf(music_filename, ".\\music\\");

	// Default path
	ret = file_select_ex("Select a File (*.MP3, *.OGG *.IT, *.S3M, *.MOD)",
		music_filename,
		"MP3;OGG;IT;S3M;MOD;/-d",
		sizeof(music_filename),  //		128,
		SCREENWIDTH-border,SCREEN_H-border);

	// Cancel button was clicked
	if(ret == 0)
		return D_O_K;

	ustrcpy(currentlevel.song, get_filename(music_filename));

	/*if(LoadLevelOGG(currentlevel.song) <= 0)
	{
		char	errormsg[128];
		sprintf(errormsg, "Unable to load: %s", currentlevel.song);
		alert("Error", errormsg, NULL, "Ok", NULL, 0,0);
	}*/

	// Request to refresh the screen
	editorrefresh = true;

	return D_O_K;
}


int		playsong_proc(void)
{
	if(PlayLevelOGG(currentlevel.song)==-1)
		alert("Errrrrr0rz", "Song not loaded.", NULL, "Bummer", NULL, 0,0);

	return D_O_K;
}

int		stopsong_proc(void)
{
	if(StopLevelOGG()==-1)
		alert("Errrrrr0rz", "Song not loaded.", NULL, "Bummer", NULL, 0,0);

	return D_O_K;
}

int savelevel(AL_CONST char *filename)
{
   PACKFILE *f;
   int		i;

   particlenode_t*	p;
   sprite_t*		s;

   f = pack_fopen(filename, F_WRITE_PACKED);
   if(!f)
   { return *allegro_errno; }


   // Scan sprites and save chest prize in line zero of the background bitmap
   s = sprites;
	 i = 1; // start at one, zero is used for SOLID
   while (s)
		{
		putpixel(background, i, 0, s->misc2); // save misc2 for all sprites
		i++; // hope there are less than 800 sprites!!
		s = s->next;
		}

   // Store file format version
   pack_iputw((int)FILE_FORMAT_VERSION, f);

   // Level name
   pack_fwrite(currentlevel.name, 64, f);

   // Author
   pack_fwrite(currentlevel.author, 64, f);

   // Desc.
   pack_fwrite(currentlevel.leveldesc, 128, f);


   // Now, store the tank x,y coords
   for(i=0; i<MAX_TANKS; i++)
   {
	   pack_iputw((int)ta[i].x, f);
	   pack_iputw((int)ta[i].y, f);
   }

   // Store number of particles
   pack_iputw((int)pwater->numparticles, f);

   // Save each particles info
   p = pwater->particles;
   while(p)
   {
	   pack_iputw((int)(p->x*pwater->SCALE_P), f);
	   pack_iputw((int)(p->y*pwater->SCALE_P), f);
	   pack_iputw((int)p->color, f);
	   p = p->next;
   }

   // Store bitmap data
   savebmpchunk(ground.bmp, f);
   savebmpchunk(background, f);

   // Store the song name
   pack_fwrite(currentlevel.song, 64, f);

   // Store the water level
   pack_iputw((int)currentlevel.staticwaterlevel, f);

   // Store the water color
   pack_iputw((int)currentlevel.watercolor, f);

   // Store the number of sprites
   pack_iputw((int)numsprites, f);

   s = sprites;
   while (s)
   {
	   pack_iputw((int)s->wobj, f);
	   pack_iputw((int)s->x, f);
	   pack_iputw((int)s->y, f);
	   pack_iputw((int)s->startstate.currentstate, f);

	   s = s->next;
   }

   // Record wall & gravity options
   pack_iputw((int)currentlevel.walltype, f);

   pack_iputw((int)currentlevel.gravityflags, f);

   pack_fclose(f);
   return *allegro_errno;
}

void	quitgame(void);

//
// loadgamelevel- Uses the realtime varibles to load a level,
//
bool loadgamelevel(AL_CONST char *filename)
{
	PACKFILE *f;
	int		i,j;
	int		nparticles=0,px,py;
	int		pcolor;
	int		ver;
	int		nsprites=0, sx, sy, newwobj;
	int		testpixel;
	sprite_t *s;
	_state	state;
	bool	editormode = (gamestate == GS_EDITOR);

	Printf("\n\n###########################################\n"
		"Loading level %s\n###########################################\n",filename);

	// Print out some info to the console so we can trace any errors.
	if(filename == NULL) // LoadRandomLevel returns NULL if no files found.
		{
		Printf("No level files found to Load\n");
		return  false;
		}

	f = pack_fopen(filename, F_READ_PACKED);
	if(!f)
		{
		Printf("Could not open:%s:\n", filename);
		return false;
		}

	// Read the version # and make sure its accurate
	ver = pack_igetw(f);

	if(ver != FILE_FORMAT_VERSION)
		{
		pack_fclose(f); // release the file buffer.
		char msg[200];
		sprintf(msg, "Level [%s] is from version [%d] that is incompatible with the version [%d] currently running.",
			ver, FILE_FORMAT_VERSION );
#ifdef		DEDICATED_SERVER
		Printf("%s\n",msg);
#else
		alert("Error",
			msg,
			"The editor will be unable to load this map",
			"Well, poo", NULL, 0,0);
#endif
		return false;
		}

	// Read level name
	pack_fread(currentlevel.name, 64, f);

	// Read author
	pack_fread(currentlevel.author, 64, f);

	// Read level desc
	pack_fread(currentlevel.leveldesc, 128, f);

	// Read tank position data
	for(i=0; i<MAX_TANKS; i++)
		{
		currentlevel.tankxpos[i] = ta[i].x = pack_igetw(f);
		currentlevel.tankypos[i] = ta[i].y = pack_igetw(f);
		currentlevel.posoccupied[i] = false;
		}

	// Read number of static water particles
	nparticles = pack_igetw(f);

	// Restore all particles
	if(nparticles > 0)
		{
		while (nparticles--)
			{
			px = pack_igetw(f);
			py = pack_igetw(f);
			pcolor = pack_igetw(f);
			pwater->InsertParticle(px,py, 0, 0, pcolor, 1, (PFLAGS_LIQUID|PFLAGS_WATER), 0, 0);
			}
		}

	//	CONCRETE is set to an unused colour after forground is loaded.
	//	if no unused colour is found in the first 200 (dark blue) then
	//	unusedcolour is set to transparent.
	//	transparent is preset. CONCRETE is required for weapon use and thus
	//	must be set to a non-transparent colour else it is ignored.
	//	SOLID is a unique colour in the background that is copied to the foreground
	//	and is non-destructable & non-falling. It is only applicable if the level
	//	designer sets it. To cover the occasions that players just set the SOLID flag &
	//	not the top left pixel this is set to transparent by default, ie non working.
	//

	// Read bitmap data
	unusedcolour = 0; // clear any leftovers from previous level
	ground.bmp = loadbmpchunk(ground.bmp, f);	// sets unusedcolour to suitable value
	CONCRETE = unusedcolour;	// sets CONCRETE colour for legacy levels

	//	SOLID is set to an unused colour after background is loaded.
	background = loadbmpchunk(background, f);
	SOLID = unusedcolour;	// sets SOLID colour for legacy levels (though not used its nice to have a value)

	// Get the song name
	pack_fread(currentlevel.song, 64, f);

	// Get the static water level
	currentlevel.staticwaterlevel = pack_igetw(f);

	// Get the static water color
	currentlevel.watercolor = pack_igetw(f);

	// Read number of sprites
	nsprites = pack_igetw(f);
	i = 0;
	while(nsprites--)
		{
		i++; // chest count
		newwobj = pack_igetw(f);
		sx = pack_igetw(f);
		sy = pack_igetw(f);
		state = (_state)pack_igetw(f);	// client will use this as start point for sprite
																		//	except that its not used by the code. G_SpawnWO will set it.

		// Don't spawn sprites on the client side when in multiplayer.
		// The server will set this up for us
		// But we need to do this in editor mode.
		if(!multiplayer || multiplayermode == MP_SERVER || editormode)
			{
			if(sx <   0)sx=   0;  							// keep sprite within screen edges
			if(sx > 800)sx= 780;
			if(sy < -20)sy= -10;                // the top may be in the clouds
			if(sy > 700)sy= -10;                // just in case the negative mouse return value wrapped to 65k
			s = G_SpawnWO((_wobject)newwobj, sx, sy);
			if(s)
				{
				s->tankfrom = -1;												//	Mark as a system object.
				s->misc2 = getpixel(background, i, 0);	//	changed to save misc2 for all sprites
				//s->SetState(state);	//	not used in code
				if( editormode)
					{
					s->SetState(wobjects[editorsprites[newwobj].wobj].icon);	//	editor display icon
					s->flags &= ~MF_NOTVISABLE;  // make visible in editor
					}

				if(s->wobj == WO_TREASURECHEST)
					{
					Printf("Loaded TREASURECHEST (%d,%d) Prize = %d.\n",sx, sy, s->misc2);
					}
				// If this is a maggen, then we hack it.
				if(s->wobj == WO_MAGGEN)
					{
					s->misc1 = 120;	//	hard coded
					s->misc2 = 3;
					}
				// Post turn sprite?
				if(s->flags & MF_POSTTURNATTACK)
					s->endlevelshoot = 1;
				}
			}
		} 	// end while(nsprites--)

	// Read wall & gravity options
	currentlevel.walltype = pack_igetw(f);
	currentlevel.gravityflags = pack_igetw(f);

	pack_fclose(f); // read all this version needs -- release file buffer

	//	=========================================================
	// Make sure the data is legit (might be corrupt)

	if(currentlevel.walltype < 0) { currentlevel.walltype = 0; }
	if(currentlevel.gravityflags < 0) { currentlevel.gravityflags = 0; }

	// Does the game have custom map settings? If so, apply them here
	// and ingore what we've just read from the CDL file
	// DEGAUSS but only in singleplayer else it stuffs up the network play.
		if(!multiplayer && !editormode)
			{
			if(walltype->value > 0)
				currentlevel.walltype = walltype->value;
			if(gravityflags->value > 0)
				currentlevel.gravityflags = gravityflags->value;
			}

	//	CONCRETE is set to an unused colour after forground is loaded.
	if(currentlevel.gravityflags & NOGRAVITY_CONCRETE)
		{
		testpixel = getpixel(ground.bmp,0,0);
		if(testpixel == TRANSCOLOR)
			{
			// transpareny not allowed (assume just default sky)
			// as it stuffs up the tunnel laser
			// so leave CONCRETE as an unused colour
			currentlevel.gravityflags &= ~NOGRAVITY_CONCRETE;
			}
		else
			{
			CONCRETE = testpixel;	// level maker has set colour
			}
		}

	//	SOLID is set to an unused colour after background is loaded.
	//	not used as yet but it's nice to have a value.
	if(currentlevel.gravityflags & NOGRAVITY_SOLID)
		{
		testpixel = getpixel(ground.bmp,0,0);
		if(testpixel == TRANSCOLOR)
			{
			// transpareny not allowed (assume pixel has not been set)
			// so leave SOLID as an unused colour
			currentlevel.gravityflags &= ~NOGRAVITY_SOLID;
			SOLID = -1; // not sure if this is required
			}
		else
			{
			SOLID = testpixel;	// level maker has set colour
			}
		}

	// GOLD 
	GOLD = goldX = goldY = 0;
	if(currentlevel.gravityflags & NOGRAVITY_GOLD)
		{
		int		x,y;
		int		gx[51], gy[51];	//	gold locations found
		int		gld = 0;	//	number of gold locations found

		GOLD = getpixel(ground.bmp,1,0);
		if(GOLD == TRANSCOLOR)GOLD = -1;	//	kill the use of gold

		for(y=1; y<SCREENHEIGHT; y++) // skip line 0
			{
			for(x=0; x<SCREENWIDTH; x++)
				{
				if(GOLD != getpixel(ground.bmp, x,y) )continue;
				gx[gld] = x;
				gy[gld] = y; // found it
				gld++;
				if(gld > 50)break;
				}
			if(gld > 50)break;
			}
#ifdef	_DEBUG
		Printf("Found %d gold pixels on loading.\n",gld);
#endif
		//	randomly select a location from the nominated points
		if(gld > 0)
			{
			x = rand()%gld;
			goldX = gx[x];
			goldY = gy[x];
			}
		else
			{
			GOLD = 0; // didn't find a treasure location so clear flag.
			currentlevel.gravityflags &= ~NOGRAVITY_GOLD;
			}
		}
#ifdef	_DEBUG
	else
		{
		Printf("GOLD flag not set for this level.\n");
		}
#endif

	//	so users can check if required
	//	Printf("Color depth for this level is %d bits\n", get_color_depth() );
	Printf("Color for CONCRETE: RGB(%d,%d,%d) [0x%X] [%d]\n",getr(CONCRETE),getg(CONCRETE),getb(CONCRETE), CONCRETE, CONCRETE );
	if(GOLD == 0)
		Printf("No Gold\n");
	else
		Printf("Color for GOLD: RGB(%d,%d,%d)  [0x%X] [%d]\n",getr(GOLD),getg(GOLD),getb(GOLD), GOLD, GOLD );


	// Store the levelname
	ustrcpy(currentlevel.filename, filename);

	// Make sure the data is not corrupted
	if(!ground.bmp)
		{
		alert("Error", "Foreground data is corrupted", "", "Oh S**t!",0 , 0,0);
		Printf("Error -- Foreground data is corrupted\n");
		return  false;
		}
	if(!background)
		{
		alert("Error", "Background data is corrupted", "", "O poo!",0 , 0,0);
		Printf("Error -- Background data is corrupted\n");
		return  false;
		}
   // Check the song
   //if(ustrlen(currentlevel.song) > 0 && LoadLevelOGG(currentlevel.song)==-1)
   //alert("Error", "Unable to load", currentlevel.song, "Ok", NULL, 0,0);


	// Reset number of shots
	currentlevel.numshotsfired = 0;

	// Print out some info about the level in the console
	Printf(	"\n================================================================\n"
					"Loaded %s by %s from file: %s\n"
					"================================================================\n",
					currentlevel.name, currentlevel.author, filename);

	return true;  //  all is OK
}

//
// FreeGameData - After we're done with a level, this function frees all the
// memory allocated from loadgamelevel()
//

void FreeGameData(void)
{
	if(ground.bmp != NULL)destroy_bitmap(ground.bmp);
	if(background != NULL)destroy_bitmap(background);
	if(numsprites != 0)		G_DestroyAllSprites();	// Make sure all the sprites have been freed
	ground.bmp = NULL;
	background = NULL;
}

int dumplevel(AL_CONST char *dumpfilename)
{
FILE	*f;
int		i, x, y, gld=0;
char	msg[128];
char filename[_MAX_PATH];
char drive[_MAX_DRIVE];
char dir[_MAX_DIR];
char fname[_MAX_FNAME];
char ext[_MAX_EXT];

char *sprite_text[] =
		{
		"Tower Pod Wall",
		"Missile Turret",
		"Fire Sentry Pod",
		"Energy Sentry Pod",
		"Ballistic Sentry Pod",
		"Lava Rock Spawner",
		"Environmental Snow",
		"Cave Spike 1",
		"Cave Spike 2",
		"Cave Spike 3",
		"Cave Spike 4",
		"Cave Spike 5",
		"Cherry Blossom Spawner",
		"Raindrop Spawner",
		"Lava Spawner",
		"Heat Corona Spawner",
		"Lava Floor Damage",
		"Ice Spike 1",
		"Ice Spike 2",
		"Ice Spike 3",
		"Fuel Cylinder",
		"Treasure Chest",
		"MagGen",
		"????????"
		};
#define sprite_text_len 23
	particlenode_t*	p;
	sprite_t*		s;
_splitpath( dumpfilename, drive, dir, fname, ext ); // C4996

_makepath( filename, drive, dir, fname, "txt" );
f = fopen( filename, "w");
if(!f)
	{
	sprintf(msg, "Couldn't create file '%s'", filename);
	G_Printf(msg);
	return *allegro_errno;
	}

fprintf(f, "// --------------------------------------\n");
fprintf(f, "// CHARRED DIRT LEVEL  %s\n", filename );
fprintf(f, "// --------------------------------------\n\n");

fprintf(f, "FILE FORMAT VERSION: %d\n", FILE_FORMAT_VERSION );   // Store file format version
fprintf(f, "Level Name: %s\n",		currentlevel.name );				// Level name
fprintf(f, "Level Author:	%s\n",	currentlevel.author );			// Author
fprintf(f, "Level Desc: %s\n",	currentlevel.leveldesc );				// Desc.

fprintf(f, "Tank x,y Coords\n" );								// Now, store the tank x,y coords
for(i=0; i<MAX_TANKS; i++)
	fprintf(f, " %d: %d , %d\n", i+1, (int)ta[i].x, (int)ta[i].y );

// Store number of particles
fprintf(f, "Num Water Particles: %d\n", (int)pwater->numparticles );
if( (int)pwater->numparticles != 0)
	{
	fprintf(f, "  X  ,   Y   ,  Colour\n" );
	p = pwater->particles;
	while(p)
		{
		fprintf(f, " %d  ,  %d  ,  0X%X \n",
			(int)(p->x*pwater->SCALE_P), (int)(p->y*pwater->SCALE_P), (int)p->color );
		p = p->next;
		}
	}
fprintf(f, "Song name: %s\n",currentlevel.song );   // Store the song name
fprintf(f, "Water level: %d\n",(int)currentlevel.staticwaterlevel );   // Store the water level
fprintf(f, "Water color: 0X%X\n",(int)currentlevel.watercolor );   // Store the water color
fprintf(f, "Num of Sprites: %d\n",(int)numsprites );   // Store the number of sprites
if( (int)numsprites != 0)
	{
	fprintf (f, "  obj,    X,    Y, startstate, Description\n" );
	s = sprites;
	while (s)
		{
		fprintf(f, " %4d,", (int)s->wobj );
		fprintf(f, " %4d,", (int)s->x );
		fprintf(f, " %4d,", (int)s->y );
		fprintf(f, " %10d,", (int)s->startstate.currentstate );
		fprintf(f, " (%s)",
		sprite_text[ ( (int)s->wobj > sprite_text_len)?sprite_text_len:(int)s->wobj ] );
		if(s->wobj == WO_TREASURECHEST)
			{
			fprintf(f, " [%s]", prize_list[s->misc2] ); // misc2 is index into the prize_list array
			}
		fprintf(f, "\n");
		s = s->next;
		}
	}
// Record wall & gravity options
// Various wall-types
fprintf (f, "walltype: " );
switch((int)currentlevel.walltype )
	{
	case WALLS_NORMAL :	fprintf(f,"NORMAL\n" ); break;
	case WALLS_RUBBER :	fprintf(f,"RUBBER\n" ); break;
	case WALLS_CEMENT :	fprintf(f,"CEMENT\n" ); break;
	case WALLS_WRAP	  :	fprintf(f,"WRAP  \n" );	break;
	default :			fprintf(f,"????  \n" );
	}

// Gravity option flags (can be more than 1)
fprintf (f, "gravityflags: ");
if(currentlevel.gravityflags == NOGRAVITY_NORMAL ) fprintf(f,"Normal Gravity. " );
if(currentlevel.gravityflags &  NOGRAVITY_GROUND ) fprintf(f,"Dirt don't fall " );
if(currentlevel.gravityflags &  NOGRAVITY_TANKS  ) fprintf(f,"Tanks don't fall " );
if(currentlevel.gravityflags &  NOGRAVITY_WATER  ) fprintf(f,"Water don't fall " );
if(currentlevel.gravityflags &  NOGRAVITY_CONCRETE)fprintf(f,"Use Concrete [0x%x]. ",CONCRETE );
if(currentlevel.gravityflags &  NOGRAVITY_SOLID)		fprintf(f,"Use Solid [0x%x]. ",SOLID );
if(currentlevel.gravityflags &  NOGRAVITY_GOLD)
	{
	fprintf(f,"Have GOLD [0x%x].\nGold found at:\n",GOLD );
		for(y=1; y<SCREENHEIGHT; y++) // skip line 0
			{
			for(x=0; x<SCREENWIDTH; x++)
				{
				if(GOLD != getpixel(ground.bmp, x,y) )continue;
				fprintf(f," [%d,%d]\n",x,y ); // found it
				gld++;
				if(gld > 50)break;
				}
			if(gld > 50)break;
			}
	}
fprintf(f,"\n## FIN ##\n");

fclose(f);

// Store bitmap data as Background & Foreground
// _makepath( filename, drive, dir, fname+"B", "bmp" );
i = strlen(filename); // change filename the hard way!
filename[i-4] = 'B'; // Background
filename[i-3] = '.';
filename[i-2] = 'b';
filename[i-1] = 'm';
filename[i  ] = 'p';
filename[i+1] =  0 ;
save_bmp(filename, background, NULL);
sprintf(msg, "Wrote '%s'", filename);	G_Printf(msg);

filename[i-4] = 'F'; // Foreground
save_bmp(filename, ground.bmp, NULL);
sprintf(msg, "Wrote '%s'", filename);	G_Printf(msg);

return *allegro_errno;
}

int	drawHUD_proc(void)
{
showHUD = !showHUD;
sprites_menu[5].text = (showHUD)?HUDon:HUDoff;
editorrefresh = true;
return D_O_K;
}

void	drawHUD(BITMAP *b)
{
// no error checks -- just hope all is ok!!
BITMAP	*hudleft;
BITMAP	*hudright;
int			hudrightx;

hudleft  = (BITMAP *)maindata[HUD_TOPLEFT].dat;
hudright = (BITMAP *)maindata[HUD_TOPRIGHT].dat;

hudrightx = (SCREENWIDTH-hudright->w);

draw_sprite(b, hudleft, 0,0);
draw_sprite(b, hudright, hudrightx, 0);
}

//########################################################
//////////////////////////  prize list  //////////////////

int NumberOfPrizes(void)
	{
	return num_prizes;
	}

int GetPrizeCode(int index)
	{
	if(index < 0)index = -index;
	if(index >= num_prizes)return -7;	//	ie nothing
	return atoi(prize_list[index] );
	}

char* GetPrizeName(int index)
	{
	if(index < 0)index = -index;
	if(index >= num_prizes)return NULL;
	return &prize_list[index][4];
	}

int GetCodeIndex(char* code)
	{
	int	index;

	if(strlen(code) != 3)return -1;

	code[0] = toupper(code[0]);
	code[1] = toupper(code[1]);
	code[2] = toupper(code[2]);

	for(index=0; index<num_prizes; index++)
		{
		if(prize_list[index][5] != '['    )continue;
		if(prize_list[index][6] != code[0])continue;
		if(prize_list[index][7] != code[1])continue;
		if(prize_list[index][8] != code[2])continue;
		return index;
		}
	return -2;
	}

//	Treasure Chest Data

//	treasure chest prize list (is global)
int	num_prizes = 96;
char *prize_list[] =
{
"-001 ****** Random Selection ******",       
"0000 $2,000",                               
"0001 $20,000",                              
"0002 Health 250 Points",                    
"0003 Health 150 & 5 Shields",               
"-004 ****** Random Defence Item ******",    
"0004 [BAT] Battery Charge (10)",                  
"0104 [PAR] Parachutes (10)",                      
"0204 [FLO] Flotation Device (10)",                
"0304 [SCO] Conventional Shield (10)",             
"0404 [SEL] Electric Shield (10)",                 
"0504 [SFI] Fire Shield (10)",                     
"0604 [SBA] Ballistic Shield (10)",                
"0704 [SSC] Super Conventional Shield (10)",       
"0804 [NIV] N.I.V. Shield (10)",                   
"0904 [FUE] Fuel (100)",
"1004 [SIG] Laser Sight (5)",
"1104 [AUT] Auto Defense (5)",
"-005 ****** Random Super Weapon ******",    
"7605 [DEA] Death's Head",                         
"7805 [GIG] Giga Nuke",                            
"7905 [MOM] Monkey Mortar",                        
"8005 [MPO] Polaris Missile",                      
"8105 [BHB] Black Hole Bomb",                      
"8205 [TEL] Twin Electric Lances",                 
"8405 [FNU] Funky Nuke",                           
"8605 [AIR] Air Strike",                           
"8705 [OBC] Orbital Beam Cannon",                  
"8805 [RIC] Rail Ricochet",                        
"9005 [ROF] Ring of Fire",                         
"9105 [SIL] Silver Bullet",                        
"-006 ****** Random Weapon ******",          
"0006 [MBA] Baby Missile",                         
"0106 [MMM] Mini-MIRV",                            
"0306 [MNE] Needle Missile",                       
"0406 [MIS] Missile",                              
"0606 [WAM] Water Mine",                           
"0706 [DIR] Dirt Ball",                            
"0806 [TOD] Ton-O-Dirt",                           
"0906 [BLA] Riot Blast",                           
"1006 [HRB] Heavy Riot Bomb",                      
"1106 [WAS] Water Splash",                         
"1206 [ROD] Molten Rod",                           
"1306 [ELA] Electric Laser",                       
"1406 [UZI] Uzi Spray",                            
"1606 [TOP] Tower Pod",                            
"1706 [TRA] Tracers",                              
"1806 [AFR] A.F.R.A.M.M.",                         
"1906 [ARC] Electric Arc",                         
"2006 [FLA] Flame Thrower",                        
"2106 [MIR] M.I.R.V.",                             
"2206 [MVS] MIRV Shot",                            
"2306 [FUN] Funky Bomb",                           
"2506 [TOA] Fire Toad",                            
"2806 [BNU] Baby Nuke",                            
"2906 [MTU] Missile Turret",                       
"3006 [ROL] Heavy Roller",                         
"3106 [LFB] Large Fireball",                       
"3206 [SBS] Sparkball",                            
"3306 [MDR] Drill Missile",                        
"3406 [IOB] Ion Battery",                          
"3606 [HCN] Heat Cannon",                          
"3706 [LEA] Lead Shell",                           
"3806 [MBL] Blue Missile",                         
"3906 [CAN] Cannon Ball",                          
"4006 [SGB] Shotgun Blast",                        
"4206 [FLY] Firefly",                              
"4406 [PRO] Proton Flare",                         
"4606 [GUN] Machine Gun",                          
"4806 [LAS] Laser Pulse",                          
"4906 [TUN] Tunnel Laser",                         
"5006 [URA] Uranium Shell",                        
"5106 [SBL] Large Sparkball",                      
"5206 [WAV] HeatWave",                             
"5306 [EMP] EMP Missile",                          
"5406 [LAV] Large Lava Rock",                      
"5506 [NAP] Napalm",                               
"5606 [ACI] Acid Splash",                          
"5706 [PLA] Plasma",                               
"5806 [PFS] Fire Sentry Pod",                      
"5906 [PES] Electric Sentry Pod",                  
"6006 [PBS] Ballistic Sentry Pod",                 
"6106 [PMG] Magnetic Generator Pod",               
"6206 [HRA] Heat Ray",                             
"6306 [ION] Ion Cannon",                           
"6406 [RAG] Railgun",                              
"6506 [NAS] Super Napalm",                         
"6606 [PLS] Super Plasma",                         
"6706 [SAC] Super Acid",                           
"6806 [NUK] Nuke",                                 
"6906 [MOR] Mortar Shell",                         
"7006 [MOO] Moon Shot",                            
"7106 [LIT] Lightning Rod",                        
"7206 [FLK] Flak Detonator",                       
"7506 [FIR] Fire Starter",                         
"-007   Nothing!! The Treasure Chest is Empty!"};

// ############ End-of-File #############
