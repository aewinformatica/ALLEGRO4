#ifndef _animation_h
#define _animation_h

#include <string>
#include <vector>
#include <map>

#include "../util/load_exception.h"
#include "attack.h"

using namespace std;

class AnimationEvent;
class Bitmap;
class ECollide;
class Token;
class Character;


const int PAIN_KEY_FORWARD = 0;
const int PAIN_KEY_BACK = 1;
const int PAIN_KEY_UP = 2;
const int PAIN_KEY_DOWN = 3;
const int PAIN_KEY_ATTACK1 = 4;
const int PAIN_KEY_ATTACK2 = 5;
const int PAIN_KEY_ATTACK3 = 6;
const int PAIN_KEY_JUMP = 7;
const int PAIN_KEY_GRAB = 8;

struct KeyPress{
	vector< int > combo;

	/*
	KeyPress & operator= ( const KeyPress & rhs ){
		this->combo = rhs.combo;
		return *this;
	}
	*/
};

/* stores a bitmap and a collision object to go with it */
struct Frame{
	Bitmap * pic;
	ECollide * collide;

	Frame( Bitmap * pic, ECollide * e);
	Frame( const Frame & f );
	~Frame();

	bool mine;
	
};

/* stores a sequence of bitmaps along with their collision
 * detection object. 
 */
class Animation{
public:
	
	Animation( Token * tok, Character * const owner ) throw( LoadException );
	Animation( const Animation & animation, Character * const owner ) throw( LoadException );

	~Animation();

	/* Act:
	 * Progress through one logic cycle
	 */
	bool Act();

	/* getName:
	 * The name of the animation
	 */
	inline const string & getName() const{
		return name;
	}

	/* returns true if the animation has finished */
	const bool empty();

	/* reset:
	 * Make the bitmap pointer point to the first bitmap in the sequence
	 */
	void reset();


	// inline const map<string,string> & getSequences() const{
	inline const vector<string> & getSequences() const {
		return sequences;
	}

	/* returns true if previous sequence is none or seq */
	bool properSequence( const string & seq );


	/* returns true if a previous sequence is seq */
	bool hasSequence( const string & seq );


	void addDecommision( const string & s );
	void addCommision( const string & s );

	const string getCurrentFramePath() const;

	inline const vector<string> & getDecommisions() const {
		return disable_animations;
	}

	inline const vector<string> & getCommisions() const {
		return enable_animations;
	}

	inline const bool isCommisioned() const {
		return commision;
	}

	inline void setCommision( const bool x ) {
		commision = x;
	}


	inline const string & getPreviousSequence() const{
		return prev_sequence;
	}

	inline const string & getNextSequence() const{	
		return next_sequence;
	}

	inline const vector<KeyPress> & getKeys() const{
		return keys;
	}

	ECollide * getCollide( int facing );
	ECollide * getNormalCollide();
	void Draw( int x, int y, Bitmap * work );
	void DrawFlipped( int x, int y, Bitmap * work );
	void DrawLit( int x, int y, Bitmap * work );
	void DrawLitFlipped( int x, int y, Bitmap * work );
	void setDelay( int _delay );

	void setFrame( const string & path );
	void setFrame( const int fr );
	void setBBox( int x1, int y1, int x2, int y2 );
	// void setAttack( int x1, int y1, int x2, int y2 );
	void Delay();

protected:
          
	void doDraw( int x, int y, const Bitmap & frame, Bitmap * work );
	void doDrawFlipped( int x, int y, const Bitmap & frame, Bitmap * work );

protected:
	string name;
	bool loop;

	Character * const parent;

	Bitmap * current_frame;
	ECollide * current_collide;
	ECollide * attack_collide;
	int delay;
	int delay_counter;

	double minZDistance;

	int bbox_x1, bbox_y1, bbox_x2, bbox_y2;
	// int attack_x1, attack_y1, attack_x2, attack_y2;

	int offset_x, offset_y;
	int shadowX, shadowY;

	/* I dont think we need range_x/range_y */
	int range_x, range_y;

	vector< AnimationEvent * > events;
	vector< AnimationEvent * >::iterator current_event;
	map< string, Frame * > frames;
	vector< KeyPress > keys;

	string next_sequence, prev_sequence;

	// map< string, string > sequences;
	vector< string > sequences;

	/* range of the attack in X direction */
	int range;

	bool own_bitmaps;
	bool own_events;
	bool is_attack;
	int status;
	int damage;

	/* can we use this animation? */
	bool commision;

	map< string, Sound * > sounds;
	Sound * contact;

	vector< string > disable_animations;
	vector< string > enable_animations;

	Attack attack;

};


#endif
