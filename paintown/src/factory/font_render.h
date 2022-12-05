#ifndef _font_render_h
#define _font_render_h

#include <string>
#include <vector>

#include "util/font.h"

using namespace std;

class Bitmap;

struct render_message{
	render_message( const Font & f, int x, int y, int fg, int bg, const string & str ):r_font( f ){
		this->x = x;
		this->y = y;
		this->fg = fg;
		this->bg = bg;
		this->str = str;
	}

	render_message( const render_message & c ):
		r_font( c.r_font ){
		x = c.x;
		y = c.y;
		fg = c.fg;
		bg = c.bg;
		str = c.str;
	}

	render_message & operator=( const render_message & rhs ){
		if ( this == &rhs ) return *this;

		/*
		this->r_font = rhs.r_font;
		this->x = rhs.x;
		this->y = rhs.y;
		this->fg = rhs.fg;
		this->bg = rhs.bg;
		this->str = rhs.str;
		*/
		return *this;
	}

	const Font & r_font;
	int x;
	int y;
	int fg;
	int bg;
	string str;
};

/* factory class to render strings later on */
class FontRender{
public:

	static FontRender * getInstance();
	static void destroy();

	void addMessage( const Font & f, int x, int y, int fg, int bg, const string & str );
	void addMessage( const Font & f, int x, int y, int fg, int bg, const char * str, ... );
	void addMessage( const char * font_name, int x, int y, int fg, int bg, const string & str );
	void render( Bitmap * work );

private:

	static FontRender * my_render;

	vector< render_message > messages;

};

#endif
