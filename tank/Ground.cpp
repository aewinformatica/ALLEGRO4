///	ground.cpp

#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"
#include "fblend.h"
#include "editor/editor.h"

//
// DrawBitmapGroundMask - Draws a bitmap onto the buffer bmp
// while not only masking out transparent pixels, but also masking
// out the pixels if the ground.bmp dosen't have a pixel there either.
//

void DrawBitmapGroundMask( BITMAP *from, BITMAP *bmp, int x, int y, int trans, bool usedirty )
	{
	int w, h, color, bmpcolor;

	// Dirty Rectangles
	if( usedirty )
		AddRectangle( bmp, x, y, from->w, from->h );

	if( trans > 0 )
		{
		drawing_mode( DRAW_MODE_TRANS, NULL, 0, 0 );
		set_trans_blender( 0, 0, 0, trans );
		}

	for( h = 0; h < from->h; h++ )
		{
		for( w = 0; w < from->w; w++ )
			{
			color = _getpixel16( from, w, h );
			bmpcolor = getpixel( ground.bmp, x + w, y + h );
			if( bmpcolor ==  - 1 )
				continue;

			if( color != TRANSCOLOR && bmpcolor != TRANSCOLOR )
				putpixel( bmp, x + w, y + h, color );
			}
		}

	if( trans > 0 )
		solid_mode();
	}



//
// DrawGroundAddition - Ingoring the pixels that are already there, this will
// draw the sprite anywhere on ground.bmp that has transparency area
// Mainly used for dirt weapons
//

void DrawGroundAddition( BITMAP *from, BITMAP *bmp, int x, int y )
	{
	int w, h, color, bmpcolor;

	for( h = 0; h < from->h; h++ )
		{
		for( w = 0; w < from->w; w++ )
			{
			color = _getpixel16( from, w, h );
			bmpcolor = getpixel( ground.bmp, x + w, y + h );
			if( bmpcolor ==  - 1 )
				continue;

			if( color != TRANSCOLOR && bmpcolor == TRANSCOLOR )
				putpixel( bmp, x + w, y + h, color );
			}
		}
	}


//
// DrawGroundSubtract - Transparent areas of the bitmap are ingored while 
// other non-transparent pixels will put a hole into the ground
//

void DrawGroundSubtract( BITMAP *from, BITMAP *bmp, int x, int y )
	{
	int w, h, color, bmpcolor;
	int tc = TRANSCOLOR;

	for( h = 0; h < from->h; h++ )
		{
		for( w = 0; w < from->w; w++ )
			{
			color = _getpixel16( from, w, h );
			bmpcolor = getpixel( ground.bmp, x + w, y + h );

			if( bmpcolor ==  - 1 )
				continue;

			if( color != tc && bmpcolor != tc )
				putpixel( bmp, x + w, y + h, tc );
			}
		}
	}


//    either
//  ground.restartcheck(0,0,0,0);
//  ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);

void GROUND::restartcheck( int x1, int y1, int x2, int y2 )
	{
	// Make the ground fall?
	if( !( currentlevel.gravityflags &NOGRAVITY_GROUND ))
		{
		checkit = 1;
		xstart = 0;
		if( y1 < ystart )
			ystart = y1;
		if( y2 > ystop )
			ystop = y2;
		if( x1 < xstart )
			xstart = x1;
		if( x2 > xstop )
			xstop = x2;

		falling = true;
		fallenpixels = 0;
		}


	// Is the water falling?
	if( !( currentlevel.gravityflags &NOGRAVITY_WATER ))
		{
		pwater->checkit = 1;
		pwater->flags |= PSYSTEM_RESETWATERLIFE;
		// Reset all particle water life to 0 apon next thinker
		}
};

//
// DrawScene - Draws the foreground & background onto the buffer with added
// effects
//

void GROUND::DrawScene( BITMAP *b )
	{
	if( ground.needsupdate )
		{
		if( ground.specialtics > 0 )  // used by Black_Hole_Bomb
			{
			if( ground.specialopacity < 0 )
				ground.specialopacity = 0;

			if( ground.specialopacity <= 254 )
				{
				blit( background, b, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT );
				fblend_trans( ground.specialbmp, b, 0, 0, ground.specialopacity );
				}

			if( ground.specialopacity == 255 )
				blit( ground.specialbmp, b, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT );


			ground.specialtics--;
			}
		else
			{
			if(( currentlevel.background_lightlevel ) < 255 )
				{
				if(( currentlevel.background_lightlevel ) < 0 )
					currentlevel.background_lightlevel = 0;

				// Fblend comes through again for FAST color blending!
				fblend_fade_to_color( background, b, 0, 0, makecol( 0, 0, 0 ), ( currentlevel.background_lightlevel ));
				}
			else
				blit( background, b, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT );
			}


		if(( currentlevel.foreground_lightlevel ) < 255 )
			{
			if(( currentlevel.foreground_lightlevel ) < 0 )
				currentlevel.foreground_lightlevel = 0;

			set_trans_blender( 0, 0, 0, 0 );

			draw_lit_sprite( b, bmp, 0, 0, ( 255-currentlevel.foreground_lightlevel ));
			}
		else
			masked_blit( bmp, b, 0, 0, 0, 0, SCREENWIDTH, SCREENHEIGHT );

		needsupdate = false;
		}


	// Simulated lightning effect...whatever could we use this for?
	if( currentlevel.whiteflashtics > 0 )
		{
		rectfill( b, 0, 0, SCREENWIDTH, SCREENHEIGHT, makecol( 255, 255, 255 ));

		currentlevel.whiteflashtics--;
		if( currentlevel.whiteflashtics == 0 )
			needsupdate = true;
		}

#ifndef DEDICATED_SERVER

	int wcolor; // Wall type color

	// If there are special wall settings, draw two bars on 
	// either side of the screen to indicate this
	if( currentlevel.walltype == WALLS_CEMENT )
		wcolor = makecol( 255, 255, 255 );
	if( currentlevel.walltype == WALLS_RUBBER )
		wcolor = makecol( 250, 0, 250 );
	if( currentlevel.walltype == WALLS_WRAP )
		wcolor = makecol( 255, 255, 0 );

	if( currentlevel.walltype != WALLS_NORMAL )
		{
		line( b, 1, 0, 1, SCREENHEIGHT, wcolor );
		line( b, SCREENWIDTH - 1, 0, SCREENWIDTH - 1, SCREENHEIGHT, wcolor );
		}
#endif 

	}

//-------------------------------------------------------------------------

void GROUND::fall()
	{
	int x, y;
	int a; //,b;
	int c;
	int pixels;

	if( !falling )
		return ;
	if( checkit == 0 )
		return ;

	for( pixels = 0; pixels < 6 *2; pixels++ )
	// test 12 lines for this frame
		{
		fallenpixels++;
		y = ystart;

		for( x = 0; x < SCREENWIDTH; x++ )
			{
			a = 0;

			// Error checking
			// cannot see the use of this as x is only modified by the for loop
			//if (x > SCREENWIDTH || x < 0) continue;

			if( y > SCREENHEIGHT - 2 || y < 0 )
				continue;
			//if (y+1 > SCREENHEIGHT || y+1 < 0) continue;

			a = FAST_GETPIXEL( bmp, x, y );
			//  Check if pixel is concrete and thus does not fall under gravity 
			//  then skip over it.
			if( a == CONCRETE )
				continue;

			if( a != TRANSCOLOR && FAST_GETPIXEL( bmp, x, y + 1 ) == TRANSCOLOR )
				{
				c = y;
				// drop pixel (y) into clear spot below (y+1)
				// then loop upwards moving clear spot up until 
				// another clear spot or top of screen.
				while( FAST_GETPIXEL( bmp, x, c ) != TRANSCOLOR )
					{
					// if concrete then don't move it, just skip to next x value
					if( FAST_GETPIXEL( bmp, x, c ) == CONCRETE )
						break;

					//  else swap the two pixels vertically
					FAST_PUTPIXEL( bmp, x, c + 1, FAST_GETPIXEL( bmp, x, c ));
					FAST_PUTPIXEL( bmp, x, c, TRANSCOLOR );

					c--;

					//if (x >	SCREENWIDTH    || x < 0) break; // why need ??????
					if( c > SCREENHEIGHT - 2 || c < 0 )
						break;
					//if (c+1 > SCREENHEIGHT || c+1 < 0) break;
					}
				this->needsupdate = true;
				}
			}

		ystart++;
		// don't bother with the bottom edge of the screen
		if( ystart > SCREENHEIGHT - 2 )
			{
			checkit = 0;
			falling = false;
			fallenpixels = 0;
			// patch to hopefully fix the non-falling water
			if( pwater->numparticles > 0 )	// Are there water particles to move around
				if( !( currentlevel.gravityflags & NOGRAVITY_WATER ))
					{
					pwater->checkit = 1;
					pwater->flags |= PSYSTEM_RESETWATERLIFE;
					// Reset all particle water life to 0 apon next thinker
					}
			}
		}

	///		Check if any treasure has been uncovered.
	// Server will do this in multiplayer ##
	if(multiplayer && multiplayermode == MP_CLIENT)
		return;


	if( GOLD )
	if( FAST_GETPIXEL( bmp, goldX, goldY ) == TRANSCOLOR )
		{						//		treasure pixel is uncovered so show treasure
		GOLD = 0;		//		and clear GOLD so it doesn't show again.
		int prize = rand()%num_prizes;
		

#ifdef	_DEBUG
		Printf( "Gold Found under Ground - X:%d  Y:%d    ", goldX, goldY);  //  Degauss debug
		Printf( "Prize[%s]\n",prize_list[prize] );
#endif

		sprite_t *s;
		s = G_SpawnWO( WO_TREASURECHEST, goldX, goldY );
		if( s )
			{			
			s->tankfrom	= -1; // belongs to system not a tank
			s->misc2		= prize;  /// atoi(prize_list[prize] ); // index into prize_list

			// Send to all clients if we're the server
			if(multiplayer && multiplayermode == MP_SERVER)
				{
				SV_SpawnSprite(s);
				}
			}
		else
			{
			Printf( "Gold Sprite failed\n" );
			// don't send anythig when spawn fails
			}
		}
	return ;
	}

char	ServerClient = '*';	// Flag to indicate a server or client call

void	DestroyDirt(int x, int y, int rad)
{
	if(!multiplayer)
		circlefill(ground.bmp,x,y,rad,TRANSCOLOR);							//	single player
	else
	Printf("DestroyDirt(%d,%d)[%d]%c\n",(int)x,(int)y,rad, ServerClient );	// Degauss SetBlowUp
	if(multiplayer && multiplayermode == MP_SERVER)
		{
		circlefill(ground.bmp,(int)x,(int)y,rad,TRANSCOLOR);		//	server explode
		BCast_WriteByte(STOC_BLOWUP);														//	pass to client here
		BCast_WriteShort((int)x);
		BCast_WriteShort((int)y);
		BCast_WriteShort(rad);
		SV_SendPackets();
		}
	else
		{
		circlefill(ground.bmp,(int)x,(int)y,rad,TRANSCOLOR);							//	client explode
		}
	Printf("DestroyDirt(%d,%d)[%d]%c\n",(int)x,(int)y,rad, ServerClient );	// Degauss SetBlowUp
	ServerClient = '*';
}