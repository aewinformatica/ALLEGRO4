#ifndef _paintown_animation_event_h
#define _paintown_animation_event_h

#include "attack.h"
#include "util/load_exception.h"

class Token;

namespace Paintown{

class Projectile;
class Animation;

class AnimationEvent{
public:
    AnimationEvent();
    virtual void Interact( Animation * animation );
    virtual ~AnimationEvent();
};

class Animation;

class AnimationEventAttack: public AnimationEvent{
public:
	AnimationEventAttack( const Attack & a );
	// AnimationEventAttack( int _x1, int _y1, int _x2, int _y2, int _damage, int _force );

	virtual void Interact( Animation * animation ); 

protected:
        Attack attack;
};

class AnimationEventBBox: public AnimationEvent {
public:
	AnimationEventBBox( int _x1, int _y1, int _x2, int _y2 );

	virtual void Interact( Animation * animation ); 

protected:
	int x1, y1, x2, y2;

};

class AnimationEventCoords: public AnimationEvent{
public:
	AnimationEventCoords( int _x, int _y, int _z );

	virtual void Interact( Animation * animation ); 

protected:
	int x, y, z;

};

class AnimationEventDelay: public AnimationEvent{
public:
	AnimationEventDelay( int _delay );

	virtual void Interact( Animation * animation );

protected:
	int delay;
};

class AnimationEventFace: public AnimationEvent{
public:
	AnimationEventFace( int direction );

	virtual void Interact( Animation * animation ); 

protected:
	int direction;

};

class AnimationEventFrame: public AnimationEvent{
public:
	AnimationEventFrame( const std::string & _path );

	virtual void Interact( Animation * animation ); 

	virtual ~AnimationEventFrame();

protected:
        std::string path;
};

class AnimationEventJump: public AnimationEvent{
public:
	AnimationEventJump( double vx, double vy, double vz );

	virtual void Interact( Animation * animation ); 

protected:
	double vx, vy, vz;

};

class AnimationEventMove: public AnimationEvent{
public:
	AnimationEventMove( int _x, int _y, int _z );

	virtual void Interact( Animation * animation ); 

protected:
	int x, y, z;

};

class AnimationEventNOP: public AnimationEvent{
public:
	AnimationEventNOP();

	virtual void Interact( Animation * animation ); 

};

class AnimationEventOffset: public AnimationEvent{
public:
	AnimationEventOffset( int _x, int _y );

	virtual void Interact( Animation * animation ); 

protected:
	int x, y;

};

class AnimationEventProjectile: public AnimationEvent {
public:
	AnimationEventProjectile(const Token * token ) throw( LoadException );

	virtual void Interact( Animation * animation ); 

	virtual ~AnimationEventProjectile();

protected:
	inline void setX( int x ){
		this->x = x;
	}

	inline void setY( int y ){
		this->y = y;
	}

	inline int getX() const {
		return x;
	}

	inline int getY() const {
		return y;
	}

	inline void setDX( double dx ){
		this->dx = dx;
	}

	inline void setDY( double dy ){
		this->dy = dy;
	}

	inline double getDX() const {
		return this->dx;
	}
	
	inline double getDY() const {
		return this->dy;
	}

	inline void setLife( int life ){
		this->life = life;
	}

	inline int getLife() const {
		return life;
	}

	Projectile * projectile;
	int x, y;
	double dx, dy;
	int life;
};

class AnimationEventShadow: public AnimationEvent {
public:
	AnimationEventShadow( int x, int y );

	virtual void Interact( Animation * animation ); 

protected:

	int x, y;
};

class AnimationEventSound: public AnimationEvent {
public:
	AnimationEventSound( const std::string & path );

	virtual void Interact( Animation * animation ); 

	virtual ~AnimationEventSound();

protected:

        std::string path;

};

class AnimationEventStatus: public AnimationEvent{
public:
	AnimationEventStatus( int _stat );

	virtual void Interact( Animation * animation ); 

protected:
	int status;

};

class AnimationEventTicket: public AnimationEvent{
public:
	AnimationEventTicket();

	virtual void Interact( Animation * animation ); 

};

class AnimationEventTrail: public AnimationEvent {
public:
    AnimationEventTrail(const int produce, const int life);
    virtual void Interact( Animation * animation ); 

protected:
	const int produce;
        const int life;
};

class AnimationEventZDistance: public AnimationEvent {
public:
	AnimationEventZDistance( const double d );

	virtual void Interact( Animation * animation ); 

protected:
	double d;
};

}

#endif
