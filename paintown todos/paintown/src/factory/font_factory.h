#ifndef _font_factory_h
#define _font_factory_h

#include <string>
#include <map>

struct DATAFILE;
class Font;

using namespace std;

class FontFactory{
public:
	static Font * getFont( const string & str, const int x, const int y );
	static void destroy();

private:
	FontFactory();
	~FontFactory();

	Font * getRealFont( const string & str, const int x, const int y );

private:
	static FontFactory * my_factory;

	DATAFILE * my_data;
	map< string, Font * > font_mapper;

};

#endif
