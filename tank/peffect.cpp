#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <allegro.h>
#include "scorch.h"
#include "peffect.h"

#define MAX_BALLS	50


BITMAP		*pbuffer, *pbuffer2;

PALLETE pallete;

bool Initialise(void);

class CParticle
{
	public:
		float x, y;				// x and y co-ordinates
		float dx, dy;			// speed in x and y directions

		int colour;				// colour of particle
};

class CBalls: public CParticle
{
	public:
		CBalls(void)
		{
			x = rand()%SCREENWIDTH;
			y = rand()%SCREENHEIGHT;
			dx = (rand()%3 - 1)/2;
			dy = (rand()%3 - 1)/2;
			colour = rand()%100 + 156;
		}


		void Move(void)			// Movement routine
		{
			x += dx;			// Change the x co-ordinate by dx
			y += dy;			// Change the y co-ordinate by dy
			colour--;			// Fade the colour


			// 'Suck' the balls towards the centre of the screen

			if (x < SCREENWIDTH/2) dx += 0.1;
				else dx -= 0.1;
			if (y < SCREENHEIGHT/2) dy += 0.1;
				else dy -= 0.1;


			// Make the balls bounce off the edge of the screen
				
			if ((y > SCREENHEIGHT) || (y < 0)) dy = -dy;
			if ((x > SCREENWIDTH) || (x < 0)) dx = -dx;


			// Particles life ends when the colour is zero

			if (colour < 1)
			{
				y = rand()%SCREENHEIGHT;
				x = rand()%SCREENWIDTH;
				dx = (rand()%3 - 1)/2;
				dy = (rand()%3 - 1)/2;
				colour = rand()%55 + 200;
			}

				
		}

		void Draw(BITMAP *pic, int colour)			// Draw the particle as a circle
		{
			circlefill(pic, x, y, 2, colour);
		}
};


CBalls balls[MAX_BALLS];

void Initialise_PEffect(void)
{
	int		i;
	pbuffer = create_bitmap_ex(8, SCREENWIDTH, SCREENHEIGHT);
	pbuffer2 = create_bitmap_ex(8, SCREENWIDTH, SCREENHEIGHT);

	for (i=0; i<20; i++)
	{ RunGUI_PEffect(NULL); }

}

bool ChangePalette(float r, float g, float b)
{

	int i;
	int		realr, realg, realb;

	realr = (r/4);
	realg = (g/4);
	realb = (b/4);
	
	
	for (i=128; i>=0; i-=2)
	{
		pallete[i].r = realr;
		pallete[i].g = realg;
		pallete[i].b = realb;

		if (i-1 >= 0)
		{
			pallete[i-1].r = realr;
			pallete[i-1].g = realg;
			pallete[i-1].b = realb;
		}


		realr--;
		realg--;
		realb--;

		if (realr < 0) { realr=0; }
		if (realg < 0) { realg=0; }
		if (realb < 0) { realb=0; }
	}

	set_pallete(pallete);

	return true;
}


void	Shutdown_PEffect(void)
{
	destroy_bitmap(pbuffer);
	destroy_bitmap(pbuffer2);
}



void	RunGUI_PEffect(BITMAP *buf)
{
	// The standard fade routine. Average the colours of 4 pixels around 
		// each pixel for the next colour
		int		i;
		int		j;
	
		for (i = 1; i < SCREENWIDTH - 1; i++)
		{
			for (j = 1; j < SCREENHEIGHT - 1; j++)
			{
				pbuffer->line[j][i] = (pbuffer->line[j + 1][i] + pbuffer->line[j - 1][i] +
					pbuffer->line[j][i + 1] + pbuffer->line[j][i - 1])/4;
			}
		}

		// Stretch the pbuffer for the zooming effect
		stretch_blit(pbuffer, pbuffer2, 10, 10, SCREENWIDTH - 20, SCREENHEIGHT - 20, 0, 0, SCREENWIDTH, SCREENHEIGHT);


		// Move and Draw balls
		for (i = 0; i < MAX_BALLS; i++)
		{
			balls[i].Draw(pbuffer2, balls[i].colour);
			balls[i].Move();
		}

		// Blit
		blit(pbuffer2, pbuffer, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT);
		if (buf)
			blit(pbuffer2, buf, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT);
}
