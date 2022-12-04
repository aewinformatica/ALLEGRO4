#include <allegro.h>
#include "bitmap.h"
#include <stdarg.h>
#include <iostream>
/*
#include "scale.h"
#include <fblend.h>
*/

using namespace std;

#ifndef debug
#define debug cout<<"File: "<<__FILE__<<" Line: "<<__LINE__<<endl;
#endif

Bitmap::Bitmap( int x, int y ):
error( false ){
	my_bitmap = create_bitmap( x, y );
	// clear( my_bitmap );
	clear();
	own = true;
}

Bitmap::Bitmap( BITMAP * who ):
error( false ){
	my_bitmap = who;
	own = false;
}

Bitmap::Bitmap( const char * load_file ):
error( false ){
	internalLoadFile( load_file );
	/*
	my_bitmap = load_bitmap( load_file, NULL );
	if ( !my_bitmap ){
		my_bitmap = create_bitmap( 100, 100 );
		clear( my_bitmap );
		cout<<"Could not load "<<load_file<<endl;
		error = true;
	}
	own = true;
	*/
}

Bitmap::Bitmap( const string & load_file ):
error( false ){
	internalLoadFile( load_file.c_str() );
}

Bitmap::Bitmap( const char * load_file, int sx, int sy ):
error( false ){
	path = load_file;
	BITMAP * temp = load_bitmap( load_file, NULL );
	// my_bitmap = load_bitmap( load_file, NULL );
	my_bitmap = create_bitmap( sx, sy );
	// clear( my_bitmap );
	clear();
	if ( !temp ){
		cout<<"Could not load "<<load_file<<endl;
		error = true;
	} else {
		stretch_blit( temp, my_bitmap, 0, 0, temp->w, temp->h, 0, 0, my_bitmap->w, my_bitmap->h );
		destroy_bitmap( temp );
	}
	own = true;
}

Bitmap::Bitmap( const char * load_file, int sx, int sy, double accuracy ):
error( false ){
	path = load_file;
	BITMAP * temp = load_bitmap( load_file, NULL );
	if ( !temp ){
		cout<<"Could not load "<<load_file<<endl;
		my_bitmap = create_bitmap( sx, sy );
		// clear( my_bitmap );
		clear();
		error = true;
	} else {
		if ( temp->w > sx || temp->h > sy ){
			double bx = temp->w / sx;
			double by = temp->h / sy;
			double use;
			use = bx > by ? bx : by;
			int fx = (int)(sx / use);
			int fy = (int)(sy / use);
			my_bitmap = create_bitmap( fx, fy );
			
			stretch_blit( temp, my_bitmap, 0, 0, temp->w, temp->h, 0, 0, my_bitmap->w, my_bitmap->h );
			
			destroy_bitmap( temp );
		} else my_bitmap = temp;
	}
	own = true;
}

Bitmap::Bitmap( const Bitmap & copy, int sx, int sy ):
error( false ){
	path = copy.getPath();
	BITMAP * temp = copy.my_bitmap;
	my_bitmap = create_bitmap( sx, sy );
	// clear( my_bitmap );
	clear();
	stretch_blit( temp, my_bitmap, 0, 0, temp->w, temp->h, 0, 0, my_bitmap->w, my_bitmap->h );
	own = true;
}

Bitmap::Bitmap( const Bitmap & copy, int sx, int sy, double accuracy ):
error( false ){
	path = copy.getPath();
	BITMAP * temp = copy.my_bitmap;
	if ( temp->w > sx || temp->h > sy ){
		double bx = (double)temp->w / (double)sx;
		double by = (double)temp->h / (double)sy;
		double use;
		use = bx > by ? bx : by;
		int fx = (int)(temp->w / use);
		int fy = (int)(temp->h / use);
		my_bitmap = create_bitmap( fx, fy );
		own = true;
		if ( !my_bitmap ){
			allegro_message("Could not create bitmap\n");
			own = false;
			error = true;
			return;
		}
	
		stretch_blit( temp, my_bitmap, 0, 0, temp->w, temp->h, 0, 0, my_bitmap->w, my_bitmap->h );
		// destroy_bitmap( temp );
	} else {
		my_bitmap = create_bitmap( temp->w, temp->h );
		blit( temp, my_bitmap, 0, 0, 0, 0, temp->w, temp->h );
		own = true;
	}
}


Bitmap::Bitmap( const Bitmap & copy ):
error( false ){
	path = copy.getPath();
	/*
	BITMAP * his = copy.getBitmap();
	my_bitmap = create_bitmap( his->w, his->h );
	::blit( his, my_bitmap, 0, 0, 0, 0, his->w, his->h );
	own = true;
	*/
	my_bitmap = copy.getBitmap();
	own = false;
}

Bitmap::Bitmap( const Bitmap & copy, int x, int y, int width, int height ):
error( false ){
	path = copy.getPath();
	BITMAP * his = copy.getBitmap();
	my_bitmap = create_sub_bitmap( his, x, y, width, height );
	if ( !my_bitmap ){
		cout<<"Could not create sub-bitmap"<<endl;
		my_bitmap = create_bitmap( 10, 10 );
		// clear( my_bitmap );
		clear();
	}
	own = true;
}
	
void Bitmap::internalLoadFile( const char * load_file ){
	path = load_file;
	my_bitmap = load_bitmap( load_file, NULL );
	if ( !my_bitmap ){
		cout<<"Could not load "<<load_file<<". Using default"<<endl;
		my_bitmap = create_bitmap( 100, 100 );
		if ( !my_bitmap ){
			cout<<"Out of memory or Allegro not initialized"<<endl;
			error = true;
			return;
		}
		// clear( my_bitmap );
		clear();
		cout<<"Could not load "<<load_file<<endl;
		error = true;
	}
	own = true;
}

/*
const string & Bitmap::getPath() const{
	return path;
}
*/
	
Bitmap & Bitmap::operator=( const Bitmap & copy ){
	
	if ( own ){
		destroy_bitmap(my_bitmap);
	}

	path = copy.getPath();
	my_bitmap = copy.getBitmap();
	own = false;
	return *this;
}

Bitmap::~Bitmap(){
	if ( own )
		destroy_bitmap( my_bitmap );
}

/*
BITMAP * Bitmap::getBitmap() const{
	return my_bitmap;
}
*/

bool Bitmap::getError(){
	return error;
}

/*
int Bitmap::getWidth(){
	return my_bitmap->w;
}

int Bitmap::getHeight(){
	return my_bitmap->h;
}
*/
	
void Bitmap::acquire(){
	acquire_bitmap( my_bitmap );
}

void Bitmap::release(){
	acquire_bitmap( my_bitmap );
}

void Bitmap::circleFill( int x, int y, int radius, int color ){
	::circlefill( my_bitmap, x, y, radius, color );
}
	
void Bitmap::circle( int x, int y, int radius, int color ){
	::circle( my_bitmap, x, y, radius, color );
}
	
void Bitmap::line( const int x1, const int y1, const int x2, const int y2, const int color ){

	::line( my_bitmap, x1, y1, x2, y2, color );
	
}
	
void Bitmap::printf( int x, int y, int color, FONT * f, const char * str, ... ){

	char buf[512];
	va_list ap;

	va_start(ap, str);
	uvszprintf(buf, sizeof(buf), str, ap);
	va_end(ap);

	textout_ex( my_bitmap, f, buf, x, y, color, -1);
}

void Bitmap::rectangle( int x1, int y1, int x2, int y2, int color ){
	rect( my_bitmap, x1, y1, x2, y2, color );
}

void Bitmap::rectangleFill( int x1, int y1, int x2, int y2, int color ){
	rectfill( my_bitmap, x1, y1, x2, y2, color );
}

/*
int Bitmap::getPixel( int x, int y ){
	if ( x >= 0 && x < my_bitmap->w && y >= 0 && y <= my_bitmap->h )
		return _getpixel16( my_bitmap, x, y );
	return -1;
}
*/

/*
int Bitmap::makeColor( int r, int g, int b ){
	return makecol16( r, g, b );
}
*/
	
void Bitmap::hLine( const int x1, const int y, const int x2, const int color){
	hline( my_bitmap, x1, y, x2, color );
}

void Bitmap::vLine( const int y1, const int x, const int y2, const int color ){
	vline( my_bitmap, x, y1, y2, color );
}
	
void Bitmap::polygon( const int * verts, const int nverts, const int color ){
	::polygon( my_bitmap, nverts, verts, color );
}
	
void Bitmap::clear(){
	this->fill( 0 );
}

void Bitmap::fill( int color ){
	clear_to_color( my_bitmap, color );
}
	
void Bitmap::draw( const int x, const int y, const Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	draw_sprite( bmp, my_bitmap, x, y );
}
	
void Bitmap::drawHFlip( const int x, const int y, const Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	draw_sprite_h_flip( bmp, my_bitmap, x, y );
}
	
void Bitmap::drawLit( const int x, const int y, const int level, const Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	draw_lit_sprite( bmp, my_bitmap, x, y, level );
}

void Bitmap::drawTrans( const int x, const int y, const Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	draw_trans_sprite( bmp, my_bitmap, x, y );
}

void Bitmap::drawStretched( const int x, const int y, const int new_width, const int new_height, const Bitmap & who ){
	BITMAP * bmp = who.getBitmap();
	masked_stretch_blit( my_bitmap, bmp, 0, 0, my_bitmap->w, my_bitmap->h, x,y, new_height, new_width );
}

void Bitmap::drawMask( const int _x, const int _y, const Bitmap & where ){
	int mask = Bitmap::MaskColor;
	for ( int x = 0; x < getWidth(); x++ )
		for ( int y = 0; y < getHeight(); y++ ){
			if ( getPixel( x,y ) == mask )
				where.putPixel( x+_x, y+_y, mask );
			}
}

void Bitmap::StretchBy2( const Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	if ( where.getWidth() != getWidth()*2 ||
		where.getHeight() != getHeight()*2 ){
			cout<<"Wrong dimensions"<<endl;
			cout<<"My:  "<< getWidth() << " " << getHeight() << endl;
			cout<<"Him: "<<where.getWidth()<< " " << where.getHeight()<<endl;
			return;
	}
	// debug
	::stretch_blit( my_bitmap, bmp, 0, 0, my_bitmap->w, my_bitmap->h, 0, 0, bmp->w, bmp->h );
	// fblend_2x_stretch( my_bitmap, bmp, 0, 0, 0, 0, my_bitmap->w, my_bitmap->h);
	// scale2x_allegro( my_bitmap, bmp, 2 );
	// debug

}

void Bitmap::StretchBy4( const Bitmap & where ){

	BITMAP * bmp = where.getBitmap();
	if ( where.getWidth() != getWidth()*4 ||
		where.getHeight() != getHeight()*4 ){
			cout<<"Wrong dimensions"<<endl;
			cout<<"My:  "<< getWidth() << " " << getHeight() << endl;
			cout<<"Him: "<<where.getWidth()<< " " << where.getHeight()<<endl;
			cout<<"Scaled: "<<getWidth()*4<<" "<< getHeight()*4<<endl;
			return;
	}
	// fblend_2x_stretch( my_bitmap, bmp, 0, 0, 0, 0, my_bitmap->w, my_bitmap->h);
	// scale4x_allegro( my_bitmap, bmp, 2 );
	::stretch_blit( my_bitmap, bmp, 0, 0, my_bitmap->w, my_bitmap->h, 0, 0, bmp->w, bmp->h );

}

void Bitmap::Stretch( const Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	::stretch_blit( my_bitmap, bmp, 0, 0, my_bitmap->w, my_bitmap->h, 0, 0, bmp->w, bmp->h );
}

void Bitmap::Blit( const int x, const int y, const Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	/*
	acquire_bitmap( bmp );
	acquire_bitmap( my_bitmap );
	*/
	::blit( my_bitmap, bmp, 0, 0, x, y, my_bitmap->w, my_bitmap->h );
	/*
	release_bitmap( my_bitmap );
	release_bitmap( bmp );
	*/
}

void Bitmap::Blit( const int mx, const int my, const int wx, const int wy, const Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	::blit( my_bitmap, bmp, mx, my, wx, wy, my_bitmap->w, my_bitmap->h );
}

void Bitmap::Blit( const int mx, const int my, const int width, const int height, const int wx, const int wy, Bitmap & where ){
	BITMAP * bmp = where.getBitmap();
	::blit( my_bitmap, bmp, mx, my, wx, wy, width, height );
}
