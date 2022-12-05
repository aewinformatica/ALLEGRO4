#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"
#include "particles.h"

// For shaking the screen
int		shakefactor;
int		shaketimer;

void	SetShakeTimer(int	tics, int factor)
{
	shakefactor = factor;
	shaketimer = tics;
}

//
// LoopDrawer - Main drawing routines (only buffer blitting)
//

void LoopDrawer(BITMAP	*bmp)
{
	int		i;
	
	// Draw the background & foreground as needed
	ground.DrawScene(bmp);

	// Draw all tanks
	for(i=0;i<MAX_TANKS;i++) 
		{
		if(ta[i].exist <= 0)
			continue;

		if(ta[i].health > 0 || ta[i].needstodie)
			{ ta[i].draw(bmp); }
		}

	// The idea is to have dirt fly up behind the explosion graphics
	DirtDrawer(bmp);
			
	// Draw all the current shots in the air
	for(i=0;i<N_SHOTS;i++) 
		{
		if(sh[i].exist > 0)
			{
			sh[i].draw(bmp);
			}
		}

	// Draw all current sprites
	G_SpriteDrawer(bmp);

	// Process sprites
	G_SpriteTicker();

	// Draw us some der particles
	ParticleDrawer(bmp);

#ifndef	DEDICATED_SERVER

	// Process dynamic text
	GameTextThinker();

	// Draw dynamic text
	DrawGameText(bmp);

	// Draw the status bar for the current tank
	if (numalivetanks() > 0)
		{
		if(hideHUD->value > 0)	//	the player wants to hide the HUD
			{					//	turn off if tank has mde a shot
			if((ta[tankturn].specialflags & TANKSPECIAL_MADESHOT) == 0)
				ta[tankturn].DrawHUD(bmp);
			}
		else
			{
			ta[tankturn].DrawHUD(bmp);	//	just draw it
			}
		}

	// Gamelines thinker & drawer
	GameLinesThinker();
	GameLinesDrawer(bmp);

	// Draw the stats displaying hitpoints/energy
	TankStatDrawer(bmp);

	DrawTankScoreBoard(bmp);

	if (numalivetanks() > 0)
		{
		if(hideHUD->value > 0)	//	the player wants to hide the HUD
			{					//	turn off if tank has mde a shot
			if((ta[tankturn].specialflags & TANKSPECIAL_MADESHOT) == 0)
				Tank_ShotClockDrawer(bmp);
			}
		else
			{
			Tank_ShotClockDrawer(bmp);	//	just draw it
			}
		}
		

	// Drawer for misc. tank stuff
	CommonPlayerInputDrawer(bmp);

	// debug
#ifdef	_DEBUG
	if(!IsKeyUp(KEY_F1))	// don't show on screenshot
		{
		AddRectangle(bmp, 0, SCREENHEIGHT-40, 250, 20);
		textprintf(bmp, font, 0+1, (SCREENHEIGHT-40)+1, 0,
			"checktanktimer= %d", checktanktimer);
		textprintf(bmp, font, 0, SCREENHEIGHT-40, makecol(255,255,255),
			"checktanktimer= %d", checktanktimer);
		}
#endif

	DrawHEBStats(bmp);

	// Active menu?
	if (menuactive)
		GUI_Drawer(buffer);

	if (IsTankMessage() > 0)
		TankMessage_Drawer(bmp);
#endif
}


void	Blit_Mouse(BITMAP *buf)
{
	int		mx = mouse_x;
	int		my = mouse_y;

	AddRectangle(buf, mx, my, mouse_sprite->w, mouse_sprite->h);
	masked_blit(mouse_sprite, buf, 0,0, mx, my, mouse_sprite->w, mouse_sprite->h);
}


void	Screen_Blit(bool blitmouse)
{
	int		x=0;
	int		y=0;
#ifdef	DEDICATED_SERVER
		return;
#endif

	if (blitmouse)
		Blit_Mouse(buffer);

	if (IsKeyUp(KEY_F1))
		ScreenShot();

	// Shaking the screen?
	if (shaketimer > 0)
	{
		if (rand()%2 == 1)
		{
			x = rand()%shakefactor;
			y = rand()%shakefactor;
		}
		else
		{
			x = -(rand()%shakefactor);
			y = -(rand()%shakefactor);
		}

		shaketimer --;
	}

	acquire_screen();
	blit(buffer,screen,0,0,x,y,SCREENWIDTH,SCREENHEIGHT);
	release_screen();

	// Restore the buffer to its orginal state
	RestoreScreen(buffer);
}
