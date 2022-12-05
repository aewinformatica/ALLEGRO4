#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"

drect		*drectangles;
int			numdrects;

//
// AddRectangle - Adds a dirty rectangle to the stack
// 

drect* AddRectangle(BITMAP *sub, int x, int y, int w, int h, int color)
{
#ifdef	DEDICATED_SERVER
	return NULL;
#else

	drect	*d = new drect;

	if (!d)
		return NULL;		// Big problems

	d->x = x;
	d->y = y;
	d->w = w;
	d->h = h;
	d->color = -1;
	d->singlepixel = -1;

	if (color >= 0)
	{
		d->color = color;
		d->singlepixel = 10;
	}

	if (d->singlepixel <= 0)
	{
		// Some standard clipping
		if (d->x < 0) { d->x = 0; }
		if (d->y < 0) { d->y = 0; }
		if (d->x+d->w > SCREENWIDTH)
		{ d->w = SCREENWIDTH-d->x; }
		if (d->y+d->h > SCREENHEIGHT)
		{ d->h = SCREENHEIGHT-d->y; }
	
		//d->buf = create_video_bitmap(w,h);
		d->buf = create_bitmap(w,h);
		if (!d->buf)
			return NULL;

		blit(sub, d->buf, d->x, d->y, 0, 0, d->w,d->h);
	}
	

	d->needstodie = 0;
	
	d->next = drectangles;
	drectangles = d;

	numdrects++;

	return d;
#endif
}
	


//
// DirtyDrawer - Its not what it sounds! This draws all of our dirty rectangles to the screen =)
//

void DirtyDrawer(void)
{
	drect	*d = drectangles;

	while (d)
	{
		blit(d->buf, buffer, 0, 0, d->x, d->y, (d->x+d->w), (d->y+d->h));
		d = d->next;
	}
}


//
// RestoreScreen - Restores the buffer to its previous state (before we started drawing) and
// deletes all the dirty rectangles
//

void RestoreScreen(BITMAP *b)
{
	drect	*d;

	while (drectangles)
	{
		d = drectangles;

		// Single pixel?
		if (d->singlepixel > 0)
		{
			if (OnScreen(b, d->x, d->y) > 0)
				FAST_PUTPIXEL(b, d->x, d->y, d->color);
		}
		else
		{

			blit(drectangles->buf, b, 0, 0, d->x, d->y, (d->x+d->w), (d->y+d->h));	
	
			// Debug. (Visually shows the dirty rectangles)
			//rect(b, d->x, d->y, d->x+d->w, d->y+d->h, makecol(255,0,0));
			
			// Free stored memory
			destroy_bitmap(drectangles->buf);
		}
	
		drectangles = d->next;
		delete d;
	}

	numdrects = 0;
}

