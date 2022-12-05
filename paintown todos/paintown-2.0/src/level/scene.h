#ifndef _scene_h
#define _scene_h

#include <string>
#include <map>
#include <deque>
#include "util/load_exception.h"

using namespace std;

class Bitmap;
class Block;
class Heart;
class Object;

struct Panel{
	Bitmap * pic;
	Bitmap * neon;
	Bitmap * screen_overlay;

	Panel( Bitmap * _pic = NULL, Bitmap * _neon = NULL, Bitmap * my_screen = NULL );
	~Panel();
};

class Scene{
public:
	Scene( const char * filename ) throw( LoadException );

	// void Draw( int x, Bitmap * work );
	void drawFront( int x, Bitmap * work );
	void drawBack( int x, Bitmap * work );

	const int getFinished() const;

	inline const int getMinimumZ() const {
		return minimum_z;
	}

	inline const int getMaximumZ() const {
		return maximum_z;
	}

	int getLimit();
	void act( int min_x, int max_x, vector< Object * > * objects );

	~Scene();

protected:

	inline const double getBackgroundParallax() const {
		return backgroundParallax;
	}

	inline void setBackgroundParallax( double b ){
		backgroundParallax = b;
	}

	inline const double getForegroundParallax() const {
		return foregroundParallax;
	}

	inline void setForegroundParallax( double b ){
		foregroundParallax = b;
	}

	inline void setMinimumZ( const int z ){
		minimum_z = z;
	}

	inline void setMaximumZ( const int z ){
		maximum_z = z;
	}

	void calculateLength();

	/* erase dead hearts */
	void clearHearts();

protected:

	string music;
	Bitmap * background;
	Bitmap * arrow;
	int arrow_blink;
	vector< int > order;
	// vector< Panel * > front_panels;
	// vector< Bitmap * > back_panels;
	vector< Bitmap * > front_panels;
	map< int, Panel * > panels;

	deque< Block * > level_blocks;

	vector< Heart * > hearts;

	int scene_length;

	/* how far the scene has progressed */
	int block_length;

	/* minimum/maximum values characters can move within the z-plane */
	int minimum_z, maximum_z;

	Block * current_block;

	/* count of how many blocks have gone by */
	int blockNumber;

	double backgroundParallax;
	double foregroundParallax;

};

#endif
