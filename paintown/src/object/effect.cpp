#include <iostream>

#include "animation.h"
#include "util/bitmap.h"
#include "effect.h"
#include "globals.h"
#include "util/load_exception.h"
#include "object.h"
#include "object_nonattack.h"
#include "util/token.h"
#include "util/token_exception.h"
#include "util/tokenreader.h"
#include "world.h"

using namespace std;

Effect::Effect( int alliance ):
ObjectNonAttack( alliance ),
image( NULL ){
	image = NULL;
}

Effect::Effect( const Effect & ef ):
ObjectNonAttack( ef ){
	image = new Animation( *(ef.image), NULL );
}

Effect::Effect( const char * _filename, int alliance ) throw( LoadException ):
ObjectNonAttack( alliance ),
image( NULL ){

	TokenReader tr( _filename );

	setMaxHealth( 100 );

	Token * head;
	try{ 
		head = tr.readToken();
	} catch ( const TokenException * te ){
		throw LoadException("Could not load effect");
	}

	if ( *head != "effect" ){
		cout<<_filename<< " is not an effect"<<endl;
		// delete head;
		throw LoadException("Not an effect");
	}
	
	while ( head->hasTokens() ) {
		Token * n = NULL;

		try{

			*head >> n;

			// for ( int q = 0; q < head->numTokens(); q++ ){
				// n = head->getToken( q );

			if ( *n == "anim" ){

				if ( image ){
					cout<<"Mulitple animations specified"<<endl;
					throw LoadException("Multiple animations specified");
				}

				image = new Animation( n, NULL );

			} else {
				cout<<"Unhandled effect attribute: "<<endl;
				n->print(" ");
			}

			// }

		} catch( const exception & ex ){
			// delete head;
			n->print(" ");
			/*
			if ( n ){
				cout<<"Error with: "<<n<<endl;
			} else 
				cout<<"Something bad happened in character"<<endl;
			throw ex;
			*/
			throw LoadException("Effect parse error");
		}

	}

	// delete head;

}

Object * Effect::copy(){
	return new Effect( *this );
}
	
bool Effect::isGettable(){
	return false;
}
	
const int Effect::getWidth() const{
	if ( image )
		return image->getWidth();
	return 0;
}
	
bool Effect::isCollidable( Object * obj ){
	return false;
}

const int Effect::getHeight() const{
	if ( image )
		return image->getHeight();
	return 0;
}

void Effect::act( vector< Object * > * others, World * world, vector< Object * > * add ){
	if ( image ){
		if ( image->Act() ){
			setHealth( -1 );
			// image->reset();
		}
	}
}
	
void Effect::draw( Bitmap * work, int rel_x ){
	if ( image ){
		image->Draw( getRX() - rel_x, getRY(), work );
	}
}

Effect::~Effect(){
	if ( image )
		delete image;
}

