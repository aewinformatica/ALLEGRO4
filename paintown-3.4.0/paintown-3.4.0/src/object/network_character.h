#ifndef _paintown_network_character_h
#define _paintown_network_character_h

#include <string>
#include <vector>
#include <map>
#include "util/load_exception.h"
#include "character.h"

namespace Paintown{

class NetworkCharacter: public Character {
public:

	/* constructors and destructor */
	NetworkCharacter( int alliance ); 
	NetworkCharacter( const char * filename, int alliance ) throw( LoadException );
	NetworkCharacter( const Filesystem::AbsolutePath & filename, int alliance ) throw ( LoadException );
	NetworkCharacter( const Character & chr ) throw( LoadException );
	
	virtual ~NetworkCharacter();
	
	virtual Object * copy();

public:
	virtual Network::Message grabMessage( unsigned int from, unsigned int who );
	virtual void drawFront(Bitmap * work, int rel_x);

	virtual void setNameTime( int d );
	virtual void alwaysShowName();
	virtual void interpretMessage(World * world, Network::Message & m );

	/* logic */
	virtual void act( std::vector< Object * > * others, World * world, std::vector< Object * > * add );
	virtual void landed( World * world );
	virtual void deathReset();
protected:
	int name_id;
	int show_name_time;
};

}

#endif
