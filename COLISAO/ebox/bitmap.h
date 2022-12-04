#ifndef _bitmap_h_
#define _bitmap_h_

#include <allegro.h>
#include <string>

using namespace std;

class Bitmap{
public:

	Bitmap( int x, int y );
	Bitmap( BITMAP * who );
	Bitmap( const char * load_file );
	Bitmap( const string & load_file );
	Bitmap( const char * load_file, int sx, int sy );
	Bitmap( const char * load_file, int sx, int sy, double accuracy );
	Bitmap( const Bitmap & copy );
	Bitmap( const Bitmap & copy, int sx, int sy );
	Bitmap( const Bitmap & copy, int sx, int sy, double accuracy );
	Bitmap( const Bitmap & copy, int x, int y, int width, int height );
	virtual ~Bitmap();

	Bitmap & operator=( const Bitmap & );

	inline const int getWidth() const{
		return my_bitmap->w;
	}

	inline const int getHeight() const{
		return my_bitmap->h;
	}

	void acquire();
	void release();
	
	void printf( int x, int y, int color, FONT * f, const char * str, ... );
	void rectangle( int x1, int y1, int x2, int y2, int color );
	void rectangleFill( int x1, int y1, int x2, int y2, int color );
	void circleFill( int x, int y, int radius, int color );
	void circle( int x, int y, int radius, int color );
	void line( const int x1, const int y1, const int x2, const int y2, const int color );
		
	void hLine( const int x1, const int y, const int x2, const int color );
	void vLine( const int y1, const int x, const int y2, const int color );
	void polygon( const int * verts, const int nverts, const int color );

	void draw( const int x, const int y, const Bitmap & where );
	void drawLit( const int x, const int y, const int level, const Bitmap & where );
	void drawHFlip( const int x, const int y, const Bitmap & where );
	void drawTrans( const int x, const int y, const Bitmap & where );
	void drawMask( const int x, const int y, const Bitmap & where );
	void drawStretched( const int x, const int y, const int new_width, const int new_height, const Bitmap & who );

	void Stretch( const Bitmap & where );
	void StretchBy2( const Bitmap & where );
	void StretchBy4( const Bitmap & where );
	void Blit( const int x, const int y, const Bitmap & where );
	void Blit( const int mx, const int my, const int wx, const int wy, const Bitmap & where );
	void Blit( const int mx, const int my, const int width, const int height, const int wx, const int wy, Bitmap & where );
	void fill( int color );
	void clear();

	bool getError();

	inline BITMAP * getBitmap() const{
		return my_bitmap;
	}

	inline int getPixel( int x, int y ) const{
		if ( x >= 0 && x < my_bitmap->w && y >= 0 && y <= my_bitmap->h )
			return _getpixel16( my_bitmap, x, y );
		return -1;
	}
	
	inline void putPixel( int x, int y, int col ) const{
		if ( x >= 0 && x < my_bitmap->w && y >= 0 && y <= my_bitmap->h )
			_putpixel16( my_bitmap, x, y, col );
	}

	inline const string & getPath() const{
		return path;
	}

	inline static int makeColor( int r, int g, int b ){
		return makecol16( r, g, b );
	}

	static const int MaskColor = MASK_COLOR_16;

protected:

	void internalLoadFile( const char * load_file );

	BITMAP * my_bitmap;
	bool own;
	bool error;
	string path;

};

#endif
