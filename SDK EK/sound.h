#pragma once

#include <allegro.h>
#include <string>
#include "load_exception.h"

using namespace std;

/* a sound! */
class Sound{
public:
	Sound( const string & path ) throw( LoadException );
	Sound( const Sound & copy );

	void play();

	virtual ~Sound();

protected:

	SAMPLE *my_sound;
	int * own;

};

