#ifndef _paintown_versus_player_h
#define _paintown_versus_player_h

#include "player.h"
#include <string>
#include "util/load_exception.h"

class World;
class Bitmap;

namespace Paintown{

class Object;

class VersusPlayer: public Player {
public:
	VersusPlayer( const Filesystem::AbsolutePath & str ) throw( LoadException );
	VersusPlayer( const Player & player ) throw( LoadException );
	
	virtual void act( std::vector< Object * > * others, World * world, std::vector< Object * > * add );
	
	virtual void draw( Bitmap * work, int rel_x, int rel_y );
	virtual bool canGrab( Object * enemy );

	virtual ~VersusPlayer();
};

}

#endif
