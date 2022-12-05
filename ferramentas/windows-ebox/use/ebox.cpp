/*
 *	EBox.cpp
 *	Author Jon Rafkind
 *	Date 10/24/02
 *
 *	This code is freely distributed and modifiable with yoru own code. I am not resoponsible for any
 *	misfortune it may cause.
 */


#include "ebox.h"
#include "allegro.h"

bool prect( int px1, int py1, int x1, int y1, int x2, int y2 ){

	if ( px1 >= x1 && px1 <= x2 && py1 >= y1 && py1 <= y2 )
		return true;
	return false;
}

EBox::EBox( int ax1, int ay1, int ax2, int ay2 ):
	x1( ax1 ),
	y1( ay1 ),
	x2( ax2 ),
	y2( ay2 ){
		next = NULL;
	}

void EBox::add( EBox * d ){
	next = d;
}

void EBox::remove(){
	next = NULL;
}

bool EBox::collide( EBox * hit, int mx, int my, int ax, int ay ){
	if ( prect( x1+mx, y1+my, hit->x1+ax, hit->y1+ay, hit->x2+ax, hit->y2+ay ) ||
	     prect( x1+mx, y2+my, hit->x1+ax, hit->y1+ay, hit->x2+ax, hit->y2+ay ) ||
	     prect( x2+mx, y1+my, hit->x1+ax, hit->y1+ay, hit->x2+ax, hit->y2+ay ) ||
	     prect( x2+mx, y2+my, hit->x1+ax, hit->y1+ay, hit->x2+ax, hit->y2+ay ) ) return true;

	int zx1 = x1+mx;
	int zy1 = y1+my;
	int zx2 = x2+mx;
	int zy2 = y2+my;
	
	int zx3 = hit->x1+ax;
	int zy3 = hit->y1+ay;
	int zx4 = hit->x2+ax;
	int zy4 = hit->y2+ay;

	if ( ( (zx1 >= zx3 && zx1<= zx4) || ( zx2 >= zx3 && zx2 <= zx4) ) &&
		( (zy3 >= zy1 && zy3 <= zy2 ) || (zy4 >= zy1 && zy4 <= zy2 ) ) ) return true;
	if ( ( (zx3 >= zx1 && zx3 <= zx2) || ( zx4 >= zx1 && zx4 <= zx2 ) ) &&
		( (zy1 >= zy3 && zy1 <= zy4 ) || (zy2 >= zy3 && zy2 <= zy4 ) ) ) return true; 

	
	return false;
}

void ECollide::do_rect( int ** map, int x, int y, int MX, int MY, int sq, int size ){

	if ( x + sq >= MX ) return;
	if ( y + sq >= MY ) return;
	
	int xs = sq;
	if ( x+xs >= MX ) xs = MX-x-1;
	int ys = sq;
	if ( y+ys >= MY ) ys = MY-y-1;

	bool total = true;
	for ( int ay = y; ay < y + ys; ay++ )
		for ( int ax = x; ax < x + xs; ax++ )
			if ( map[ay][ax] != 1 ) return;
	if ( total ){
		for ( int ay = y; ay < y + ys; ay++ )
			for ( int ax = x; ax < x + xs; ax++ )
				map[ay][ax] = 2;

		add( x*size,y*size,(x+xs)*size,(y+ys)*size );
	}
	
}

void ECollide::mapBox_hori( int ** map, int MX, int MY, int x, int y, int size, bool who ){

	int qx = x;
	while ( qx < MX && map[y][qx] == 1 ) qx++;
	if ( qx-x < 2 && who ) return;
	for ( int g = x; g < qx; g++ )
		map[y][g] = 2;
	
	add( x*size, y*size,qx*size,y*size+size);
	return;
	
}

void ECollide::mapBox_vert( int ** map, int MX, int MY, int x, int y, int size, bool who ){

	int qy = y;
	while ( qy < MY && map[qy][x] == 1 ) qy++;
	if ( qy-y < 2 && who ) return;
	for ( int g = y; g < qy; g++ )
		map[g][x] = 2;
	
	add( x*size, y*size,x*size+size,qy*size);
	return;
	
}

ECollide::ECollide():
	xs( 0 ),
	ys( 0 ),
	ul_x( -1 ),
	ul_y( -1 ),
	lr_x( -1 ),
	lr_y( -1 ){
	head = new EBox( 0, 0, 0, 0 );
	current = head;
}

ECollide::ECollide( BITMAP * work, int size, int mask, double enough ){

	head = new EBox( 0, 0, 0, 0 );
	current = head;
	xs = work->w/2;
	ys = work->h/2;

	if ( enough < 0 ) enough = 50;
	if ( enough > 100 ) enough = 100;

	int map_x = work->w/size+1;
	int map_y = work->h/size+1;
	
	int ** map = new int*[ map_y ];
	for ( int y = 0; y < map_y; y++ )
		map[y] = new int[ map_x ];
	
	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			map[y][x] = 0;

	int mx = 0, my = 0;
	for ( int y = 0, my=0; y < work->h; y += size, my++ ){
		for ( int x = 0, mx=0; x < work->w; x += size, mx++ ){
			BITMAP * sub = create_sub_bitmap( work, x, y, size, size ); 
			int total = 0;
			for ( int qx = 0; qx < sub->w; qx++ )
				for ( int qy = 0; qy < sub->h; qy++ )
					if ( getpixel(sub,qx,qy) != mask )
						total++;
			double percent = 100.0 * (double)total / (double)(sub->w*sub->h);
			if ( percent > enough )
				map[my][mx] = 1;
			destroy_bitmap( sub );
		}
	}

	for ( int sq = map_x>map_y?map_x:map_y; sq > 2; sq-- )
		for ( int y = 0; y < map_y; y++ )
			for ( int x = 0; x < map_x; x++ )
				do_rect( map, x, y, map_x, map_y, sq, size );
	
	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			if ( map[y][x] == 1 )
				mapBox_hori( map, map_x, map_y, x, y, size, true );
	
	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			if ( map[y][x] == 1 )
				mapBox_vert( map, map_x, map_y, x, y, size, true );
	
	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			if ( map[y][x] == 1 )
				mapBox_hori( map, map_x, map_y, x, y, size, false );
			
	for ( int x = 0; x < map_y; x++ )
		delete[] map[x];
	delete[] map;

	finalize();
	
}

void ECollide::finalize(){
	int gx = 0;
	int gy = 0;
	int lx = 0;
	int ly = 0;
	reset();
	if ( current != NULL ){
		gx = current->x1;
		gy = current->y1;
	}
	while ( current != NULL ){

		if ( current->x1 < gx ) gx = current->x1;
		if ( current->y1 < gy ) gy = current->y1;
		if ( current->x2 > lx ) lx = current->x2;
		if ( current->y2 > ly ) ly = current->y2;
		Next();
		
	}
	ul_x = gx;
	ul_y = gy;
	lr_x = lx;
	lr_y = ly;
}

void ECollide::display( BITMAP * buf, int x, int y ){
	reset();
	while( current != NULL ){
		rect( buf, CX(current->x1+x), CY(current->y1+y), CX(current->x2+x), CY(current->y2+y), makecol(255,255,255) );
		Next();
	}
}

void ECollide::add( int x1, int y1, int x2, int y2 ){
	EBox * junk = new EBox( x1, y1, x2, y2 );
	current = head;
	while ( current->next != NULL ) current=current->next;
	current->add( junk );
}

void ECollide::remove(){
	current = head;
	EBox * junk = current;
	while ( current->next != NULL ) current=current->next;
	while ( junk->next != current ) junk=junk->next;
	delete current;
	junk->remove();
}

int ECollide::CX( int x ){
	return x-xs;
}

int ECollide::CY( int y ){
	return y-ys;
}

bool ECollide::Single( int mx, int my, int ax, int ay ){

	if ( ul_x != -1 ){
		if ( !prect( ax, ay, CX(ul_x+mx), CY(ul_y+my), CX(lr_x+mx), CY(lr_y+my) ) ) return false;
	}
	
	reset();
	while ( current != NULL ){

		if ( prect( ax, ay, CX(current->x1+mx), CY(current->y1+my),
					CX(current->x2+mx), CY(current->y2+my) ) )
			return true;
		
		Next();
	}
	return false;
	
}

bool ECollide::Collision( ECollide * hit, int mx, int my, int ax, int ay ){

	if ( ul_x != -1 && hit->ul_x != -1 ){
		int zx1 = CX(ul_x+mx);
		int zy1 = CY(ul_y+my);
		int zx2 = CX(lr_x+mx);
		int zy2 = CY(lr_y+my);

		int zx3 = hit->CX(hit->ul_x+ax);
		int zy3 = hit->CY(hit->ul_y+ay);
		int zx4 = hit->CX(hit->lr_x+ax);
		int zy4 = hit->CY(hit->lr_y+ay);

		bool cy = ( prect( zx1, zy1, zx3, zy3, zx4, zy4 ) ||
		            prect( zx2, zy1, zx3, zy3, zx4, zy4 ) ||
		            prect( zx1, zy2, zx3, zy3, zx4, zy4 ) ||
		            prect( zx2, zy2, zx3, zy3, zx4, zy4 ) );
		
		bool s1 = ( ( (zx1 >= zx3 && zx1<= zx4) || ( zx2 >= zx3 && zx2 <= zx4) ) &&
			( (zy3 >= zy1 && zy3 <= zy2 ) || (zy4 >= zy1 && zy4 <= zy2 ) ) );
		bool s2 = ( ( (zx3 >= zx1 && zx3 <= zx2) || ( zx4 >= zx1 && zx4 <= zx2 ) ) &&
			( (zy1 >= zy3 && zy1 <= zy4 ) || (zy2 >= zy3 && zy2 <= zy4 ) ) ); 

		if ( !cy && !s1 && !s2 ) return false;

	}
	
	reset();
	while ( current != NULL ){
		hit->reset();
		while ( hit->Box() != NULL ){
			if ( current->collide( hit->Box(), CX(mx), CY(my), hit->CX(ax), hit->CY(ay)  ) || hit->Box()->collide( current, hit->CX(ax), hit->CY(ay), CX(mx), CY(my) ) ) return true;
			hit->Next();
		}
		Next();
	}
	return false;
	
}

EBox * ECollide::Box(){
	return current;
}

void ECollide::Next(){
	current = current->next;
}

void ECollide::reset(){
	current = head->next;
}
