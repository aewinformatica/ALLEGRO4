/*
 *	EBox.cpp
 *	Author Jon Rafkind
 *	Date 10/24/02
 *
 *	This code is freely distributed and modifiable with yoru own code. I am not resoponsible for any
 *	misfortune it may cause.
 */


#ifndef _estimated_collision_boxes_h_
#define _estimated_collision_boxes_h_

#include "allegro.h"

class EBox{
public:

	EBox( int ax1, int ay1, int ax2, int ay2 );
	void add( EBox * d );
	void remove();
	bool collide( EBox * hit, int mx, int my, int ax, int ay );
	
	int x1, y1, x2, y2;
	EBox * next;
	
};

class ECollide{
public:

	ECollide();
	ECollide( BITMAP * work, int size, int mask, double enough );
	bool Collision( ECollide * hit, int mx, int my, int ax, int ay );
	void display( BITMAP * buf, int x, int y );
	
	void add( int x1, int y1, int x2, int y2 );
	void remove();
	void finalize();
	bool Single( int mx, int my, int ax, int ay );

protected:
	void reset();

	int CX( int x );
	int CY( int y );
	void Next();
	void mapBox_hori( int ** map, int MX, int MY, int x, int y, int size, bool who );
	void mapBox_vert( int ** map, int MX, int MY, int x, int y, int size, bool who );
	void do_rect( int ** map, int x, int y, int MX, int MY, int sq, int size );
	EBox * Box();
	EBox * head;
	EBox * current;

	int xs, ys; //length and width of bitmap
	int ul_x, ul_y, lr_x, lr_y;
	
};

#endif

