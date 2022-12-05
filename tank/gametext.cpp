#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "dialogs.h"
#include "scorch.h"
#include "fblend.h"

gametext_t		gametext[MAX_GAMETEXT];
gameline_t		gamelines[MAX_GAMELINES];


//
// FindGameTextSpot - Searches the global array for a free spot.
// Returns -1 on error
//

int FindGameTextSpot(void)
{
	int		i;

	for (i=0; i<MAX_GAMETEXT; i++)
	{
		if (gametext[i].timer <= 0)
			return i;
	}

	// Loop completed with no results
	return -1;
}

//
// SpawnGameText - Searches the global array and setup's a position for
// some moving / dynamic text. Only provide these 7 basic parameters, if any
// more are needed the function will return the number in the array in which
// the code can later modify
//

int		SpawnGameText(char *text, int timer, int x, int y, int xvel, int yvel, int color, FONT *font, int flags)
{
	
	int		i;

	// Search for open spot in array
	i = FindGameTextSpot();

	// Error.
	if (i < 0)
		return i;

	// Setup the new array
	strcpy(gametext[i].text, text);
	gametext[i].opacity = 255;		// Solid by default
	gametext[i].timer = timer;
	gametext[i].x = x;
	gametext[i].y = y;
	gametext[i].xvel = xvel;
	gametext[i].yvel = yvel;
	gametext[i].color = color;
	gametext[i].flags = flags;

	// Create our bitmap
	gametext[i].bitmap = create_bitmap(text_length(font, gametext[i].text)+10, 
		text_height(font)+10); // Add +10 to each just incase we have a shadow or something
	clear_to_color(gametext[i].bitmap, TRANSCOLOR);

	// Print the text out onto our bitmap
	textout(gametext[i].bitmap, font, text, 0+2, 0+2, makecol(0,0,0));	//	shadow
	textout(gametext[i].bitmap, font, text, 0, 0, color);
	return i;
}

//
// DestroyGameText - Free the array and deletes the bitmap.
//

void	DestroyGameText(int	ref)
{
	// Now free up some memory and what not
	destroy_bitmap(gametext[ref].bitmap);

	gametext[ref].timer = -1;
}

//
// DestroyAllGameText - Kills out the entire array so no text will display on the screen
// (also frees some memory up too  =)
//

void	DestroyAllGameText(void)
{
	int		i;

	for (i=0; i<MAX_GAMETEXT; i++)
	{
		// Active?
		if (gametext[i].timer > 0)
			DestroyGameText(i);

		gametext[i].timer = -1;
	}
}

//
// GameTextThinker - Does all the logical stuff. Kinda boring...but....
//

void	GameTextThinker(void)
{
	int		i;

	for (i=0; i<MAX_GAMETEXT; i++)
	{
		// Active?
		if (gametext[i].timer <= 0)
			continue;

		// Process velocity
		gametext[i].x += gametext[i].xvel;
		gametext[i].y += gametext[i].yvel;

		if (gametext[i].flags & TEXT_INVERSEGRAVITY)
		{
			gametext[i].xvel *= 0.8;
			gametext[i].yvel -= (GRAVITY/4);
		}
		else
		{
			gametext[i].xvel *= 0.9; // was 0.8; // ???
			gametext[i].yvel *= 0.9; // was 0.8; // ???
		}
		
		if (gametext[i].timer < 70)  // ???
			if (gametext[i].flags & TEXT_FADEOUT)
						gametext[i].opacity *= 0.9; // gametext[i].opacity -= 5;

		// Run any function that might have been setup
		if (gametext[i].loopfunc)
			gametext[i].loopfunc(i);

		// Process the clock
		gametext[i].timer--;

		if (gametext[i].timer <= 0 || gametext[i].opacity <= 0)
			DestroyGameText(i);
	}
}


//
// DrawGameText - Simple. Draws text to the buffer
//

void DrawGameText(BITMAP *bmp)
{
	int		i;
	float	x,y;
	int		finx,finy;

	for(i=0; i<MAX_GAMETEXT; i++)
		{
		// Active?
		if (gametext[i].timer <= 0)
			continue;

		// When we draw, the x and y coords are the MIDDLE of
		// the bitmap in relation to the buffer
		x = (float)(gametext[i].bitmap->w/2);
		y = (float)(gametext[i].bitmap->h/2);

		finx = (int)(gametext[i].x-x);
		finy = (int)(gametext[i].y-y);

		// Keep this on the screen
		if((finx+gametext[i].bitmap->w) > SCREENWIDTH)
			{ finx = (SCREENWIDTH - gametext[i].bitmap->w); }

		if(finx < 0)
			{ finx = 0; }

		// Add dirty rectangle & draw the sprite
		AddRectangle(bmp, finx,finy, gametext[i].bitmap->w, gametext[i].bitmap->h);
		if (gametext[i].opacity == 255)
			draw_sprite(bmp, gametext[i].bitmap, finx, finy);
		else
			fblend_trans(gametext[i].bitmap, bmp, finx, finy, gametext[i].opacity);
		}

	// Draw tank textanimation
	for(i=0; i<MAX_TANKS; i++)
		{
		if(ta[i].exist <= 0)
			continue;

		if(ta[i].texttimer > 0)
			{
			TankSayThinker(i);
			return;
			}
		}
}



//
// CharacterSay - Small loop that pauses the gameplay and waits for a certain character to say something
//
void	CharacterSay(char *text, int tank, int timer)
{
	FONT		*f = (FONT *)maindata[COMIC].dat;

	if (!text)
		return;
	ta[tank].textlettercount = 0;
	ta[tank].textlettertimer = 3;
	ta[tank].texttimer = timer;

	strcpy(ta[tank].textbuf, text);

	ta[tank].textw = text_length(f, ta[tank].textbuf)+(text_length(f, ta[tank].textbuf)/2);
	ta[tank].texth = (text_height(f)+6)*2;
	
	// Set x and y positions
	ta[tank].textx = (ta[tank].x-(ta[tank].textw/2));
	ta[tank].texty = (ta[tank].y-(ta[tank].texth*2));

	// Keep this on the screen
	if ((ta[tank].textx + ta[tank].textw) > SCREENWIDTH)
	{ ta[tank].textx = (SCREENWIDTH - ta[tank].textw); }

	if (ta[tank].textx < 0)
	{ ta[tank].textx = 0; }
}

int	AreTanksTalking(void)
{
	int		i;
	int		ret = 0;

	for (i=0; i<MAX_TANKS; i++)
	{ 
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].texttimer > 0)
			ret = 1;
	}

	return ret;
}



void	TankSayThinker(int tank)
{
	int		i;

	// Decrease the timer
	ta[tank].texttimer --;

	// Create ourselves a rectangle here
	AddRectangle(buffer, ta[tank].textx, ta[tank].texty, ta[tank].textw, ta[tank].texth);


	for (i=0; i<ta[tank].textlettertimer; i++)
	{
		if (ustrsize(ta[tank].textbuf) < ta[tank].textlettercount)
			continue;
	
		ta[tank].textblitbuf[ta[tank].textlettercount] = ta[tank].textbuf[ta[tank].textlettercount];
		ta[tank].textblitbuf[ta[tank].textlettercount+1] = '\0';
		ta[tank].textlettercount++;
	}
	
	triangle(buffer,ta[tank].textx+(ta[tank].textw/2)-7, ta[tank].texty+(ta[tank].texth/2), 
					ta[tank].textx+(ta[tank].textw/2)+7, ta[tank].texty+(ta[tank].texth/2),
					ta[tank].x, ta[tank].y-15,
					makecol(0,0,0));
	
	rectfill(buffer, 
		ta[tank].textx, 
		ta[tank].texty, 
		(ta[tank].textx+ta[tank].textw), 
		(ta[tank].texty+ta[tank].texth), 
		makecol(0,0,0));

	rectfill(buffer, 
		ta[tank].textx+2, 
		ta[tank].texty+2, 
		(ta[tank].textx+ta[tank].textw)-2, 
		(ta[tank].texty+ta[tank].texth)-2, 
		makecol(255,255,255));
			
			
	triangle(buffer,(ta[tank].textx+(ta[tank].textw/2)-7)+3, (ta[tank].texty+(ta[tank].texth/2)), 
					(ta[tank].textx+(ta[tank].textw/2)+7)-3, (ta[tank].texty+(ta[tank].texth/2)),
					(ta[tank].x), (ta[tank].y-15)-3,
					makecol(255,255,255));


	// Set this to read the comic fonts palette correctly
	set_palette((RGB *)maindata[COMIC_FONT_PAL].dat);
	
			
	textout_centre(buffer, 
		(FONT *)maindata[COMIC].dat, 
		ta[tank].textblitbuf, 
		ta[tank].textx+(ta[tank].textw/2), 
		(ta[tank].texty+((ta[tank].texth/2)-(text_height(font)/2))+2), 
		-1);
	

	if (ta[tank].texttimer == 0)
	{
		// Free some memory
		//destroy_bitmap(textbuf);

		// Refresh the screen
		ground.needsupdate = true;
	}
}


//
// TankSay - Pauses the gameplay just long enough for us to do a small animation
// for a tank to say somthing. Either good (kill) or bad (killed)
//

void	TankSay(char	*text, int tank, int timer)
{
	// Servers will broadcast to clients
	if (multiplayer)
	{
		if (multiplayermode == MP_SERVER)
		{
			BCast_WriteByte(STOC_TANKTALK);
			BCast_WriteByte(tank);
			BCast_WriteString(text);
			BCast_WriteShort(timer);
		}
		else
			return;		// Clients ingore this completely
	}

	// Begin (local mode or server)
	CharacterSay(text, tank, timer);
}



//
// FindGameTextSpot - Searches the global array for a free spot.
// Returns -1 on error
//

int FindGameLineSpot(void)
{
	int		i;
	int		firstone;
	int		lowesty=1000;

	for (i=0; i<MAX_GAMELINES; i++)
	{
		if (gamelines[i].timer <= 0)
			return i;

		if (gamelines[i].y < lowesty)
		{
			lowesty = gamelines[i].y;
			firstone = i;
		}

	}

	// Nothing available? Kill out the first block in the
	// array and lets run with that
	gamelines[firstone].timer = 0;
	return firstone;
}


void	G_Printf(char	*text, int tankfrom, int color)
{
	int		gl;
	int		i;
	int		fontheight = text_height((FONT *)maindata[VER10].dat);

	gl = FindGameLineSpot();

	// Move all the existing text up
	for(i=0; i<MAX_GAMELINES; i++)
		{
		if(gamelines[i].timer <= 0)
			continue;

		gamelines[i].y -= fontheight+2;
		}

	// Fill in some initial varibles
	gamelines[gl].x = 10;
	gamelines[gl].y = 570;
	gamelines[gl].color = color;
	gamelines[gl].timer = 6*TICRATE;
	gamelines[gl].tankfrom = tankfrom;
	gamelines[gl].opacity = 255;
	
	// Finally, copy the text
	ustrcpy(gamelines[gl].text, text);

	Printf(gamelines[gl].text);
	Printf("\n"); 
}


void	GameLinesThinker(void)
{
	int		i;

	for (i=0; i<MAX_GAMELINES; i++)
	{
		if (gamelines[i].timer <= 0)
			continue;
		
		gamelines[i].timer--;

		if (gamelines[i].timer < 1*TICRATE)
			gamelines[i].opacity -= 5;
	}
}


void	GameLinesDrawer(BITMAP *b)
{
	FONT	*f = (FONT*)maindata[VER10].dat;
	int		i;
	int		gamelength=0;
	int		blitx, blity;
	char	str[64];

	for (i=0; i<MAX_GAMELINES; i++)
	{
		if (gamelines[i].timer <= 0)
			continue;

		/*if (gamelines[i].opacity < 255)
		{
			drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
			set_trans_blender(0,0,0, gamelines[i].opacity);
		}*/
		

		// Add a dirty rectangle before doing anything
		AddRectangle(b, gamelines[i].x, 
			gamelines[i].y, 
			text_length(f, gamelines[i].text)+5,
			text_height(f)+1);

		blitx = gamelines[i].x;
		blity = gamelines[i].y;

		// Coming from a tank?
		if  (gamelines[i].tankfrom >= 0)
		{
			sprintf(str, "%s:", ta[gamelines[i].tankfrom].name);
			//ustrcat(blitstring, str);

			// Add a dirty rectangle before doing anything
			AddRectangle(b, blitx, 
				blity, 
				text_length(f, str)+5,
				text_height(f)+1);

			// Draw the shadow first
			textout(b, f, str, blitx+1, blity+1, 0);
			textout(b, f, str, blitx, blity, makecol(ta[gamelines[i].tankfrom].colorr,
				ta[gamelines[i].tankfrom].colorg,
				ta[gamelines[i].tankfrom].colorb));

			blitx += text_length(font, str)+15;
		}

		// Add a dirty rectangle before doing anything
		AddRectangle(b, blitx, 
			blity, 
			text_length(f, gamelines[i].text)+5,
			text_height(f)+1);

		// Draw the shadow first
		textout(b, f, gamelines[i].text, blitx+1, blity+1, 0);
		textout(b, f, gamelines[i].text, blitx, blity, gamelines[i].color);

		// Back into solid mode
		//if (gamelines[i].opacity < 255)
		//	solid_mode();
	}
}

