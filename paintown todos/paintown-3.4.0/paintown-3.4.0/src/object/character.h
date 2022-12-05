#ifndef _character_h
#define _character_h

#include <string>
#include <vector>
#include <map>
#include "object_attack.h"
#include "util/load_exception.h"
#include "util/file-system.h"
#include "network/network.h"

class Bitmap;
class Stimulation;
class World;
class Sound;

namespace Script{
    class Character;
}

namespace Paintown{

class Animation;
class AnimationTrail;
class ObjectAttack;
class Projectile;
class DrawEffect;

/* used for gibs */
struct BodyPart{
	BodyPart( int x, int y, Bitmap * image ):x(x), y(y), image(image){}
	/* relative to getX()/getY() */
	int x, y;
	Bitmap * image;
};

/* these should be self explanatory.. */
const int Status_Ground = 0; /* normal status, on the ground */
const int Status_Jumping = 1; /* jumping */
const int Status_Grab = 2; /* grabbing another character */
const int Status_Grabbed = 3; /* this character is being grabbed */
const int Status_Hurt = 4; /* just got hurt */
const int Status_Rise = 5; /* getting up from ground */
const int Status_Fell = 6; /* fell down due to damage */
const int Status_Dead = 7; /* dying */
const int Status_Get = 8; /* getting an object */
const int Status_Falling = 9; /* falling due to lack of ground beneath them */

class Character;

/* Handles palette swaps */
class Remap{
public:
    Remap();
    Remap(const Filesystem::RelativePath & from, const Filesystem::RelativePath & to, Remap * original);
    Remap(const Remap & copy, Character * parent);

    virtual ~Remap();

    virtual void setAnimation(const std::string & name, Animation * animation);
    virtual Animation * getAnimation(const std::string & name);
    virtual const inline std::map<std::string, Animation*> & getAnimations() const {
        return mapper;
    }

    virtual inline const Filesystem::RelativePath & getFrom() const {
        return remapFrom;
    }
    
    virtual inline const Filesystem::RelativePath & getTo() const {
        return remapTo;
    }

    virtual void use(Character * from);
protected:
    virtual void doRemap(Character * from);

    std::map<std::string, Animation*> mapper;
    bool needRemap;
    Remap * original;
    Filesystem::RelativePath remapFrom;
    Filesystem::RelativePath remapTo;
};

class Character: public ObjectAttack {
public:

	/* constructors and destructor */
	Character( int alliance ); 
	Character( const char * filename, int alliance ) throw( LoadException );
	Character(const Filesystem::AbsolutePath & filename, int alliance ) throw ( LoadException );
	Character( const Character & chr ) throw( LoadException );
	
	virtual ~Character();

public:

	virtual void createProjectile( Projectile * projectile );

	/* logic */
	virtual void act( std::vector< Object * > * others, World * world, std::vector< Object * > * add );

	/* drawing */
	virtual void draw( Bitmap * work, int rel_x, int rel_y );
	
	virtual void drawReflection(Bitmap * work, int rel_x, int rel_y, int intensity);
	virtual void drawShade(Bitmap * work, int rel_x, int intensity, int color, double scale, int fademid, int fadehigh);

        virtual const Bitmap * getCurrentFrame() const;
	
	// virtual bool collision( Object * obj );

	/* get a mirror image of this character */
	virtual Object * copy();

	/* make the character jump with an x velocity of x_ */
	virtual void doJump( double x_, double z_ );
	
        /* implements the visitor OOP pattern:
         * stimulation calls stimulate on character, then character
         * calls stimulate on stimulation with the proper type
         */
	virtual void stimulate( const Stimulation & stim );
        
        virtual bool touchPoint(int x, int y);

	/* inherited from object */
	virtual void thrown();

	/* you have collided with obj */
	virtual void collided(World * world, ObjectAttack * obj, std::vector< Object * > & objects );
	
	inline virtual void setFacing( const int f ){
		if ( getStatus() == Status_Ground || getStatus() == Status_Grabbed ){
			Object::setFacing( f );
		}
	}
	
	virtual bool isGrabbed();

	virtual void grabbed( Object * obj );
	virtual void unGrab();
	
	virtual void attacked( World * world, Object * something, std::vector< Object * > & objects );

	/* obj hurt you, take some damage */
	virtual void takeDamage( World & world, ObjectAttack * obj, int x );

	/* make the character jump! */
	virtual void jump();

	virtual void died( std::vector< Object * > & objects );

public:
	/* debug methods */

	void print() const;

public:
	/* probers */

	/* am I doing an attacking move? */
	virtual bool isAttacking();

	inline bool isMoving() const{
		return moving;
	}
	
	virtual double minZDistance() const;

	/* can I be hit? */
	virtual bool isCollidable( Object * obj );

	/* have I hit something? */
	virtual bool collision( ObjectAttack * obj );
        
    virtual bool isGrabbable(Object * obj);

	virtual bool isJumping();
	
	virtual int getAlliance() const;

public:
	/* setters */
	
	virtual inline void setStatus( const int _status ){
		status = _status;
	}

	virtual int getToughness() const {
		return toughness;
	}

	virtual int getRX() const;
	virtual int getRZ() const;
	virtual int getRY() const;

	virtual inline void setShadow( int i ){
		shadow = i;
	}

	virtual void setMap( const unsigned int x );
	virtual void nextMap();
	virtual void nextMap( int x );
	virtual int getNextMap() const;
	virtual int getNextMap( unsigned int x ) const;

	inline void setLink( Object * o ){
		linked = o;
	}

	inline void setThrown( const bool b ){
		thrown_status = b;
	}

	/* should these two methods be private? probably.. */
        /*
	inline void setMaxJumpHeight( const int j ){
		max_jump_height = j;
	}
        */
	
	inline void setXVelocity( const double j ){
		moving = true;
		x_velocity = j;
	}

	inline void setZVelocity( const double j ){
		moving = true;
		z_velocity = j;
	}
	
	inline void setYVelocity( const double j ){
		moving = true;
		y_velocity = j;
	}

	inline void setMoving( const bool x ){
		if ( x == false ){
			setXVelocity( 0 );
			setYVelocity( 0 );
			setZVelocity( 0 );
		}
		moving = x;
	}

	inline void setJumpingYVelocity( const double j ){
		jumping_starting_velocity = j;
	}

	inline void initJumpingYVelocity(){
		setYVelocity( jumping_starting_velocity );
	}
		
	void decreaseYVelocity();
	void decreaseXVelocity();
	void decreaseZVelocity();

        virtual void setTrails(const int produce, const int life);

public:
	/* for testing purposes only */
	virtual bool testAnimation();
	virtual void testAnimation( unsigned int x );
	virtual void testAnimation( std::string name );
	void testReset();


public:
	/* getters */

	/* how much damage I do */
	virtual int getDamage() const;

	/* collision detection object */
	virtual ECollide * getCollide() const;

	inline Object * getLink(){
		return linked;
	}

	inline bool getThrown() const{
		return thrown_status;
	}

	inline const std::map< int, Remap*> getMapper() const {
		return mapper;
	}

	virtual bool isGettable();
	virtual Animation * getCurrentMovement() const;
	virtual void setMovement( Animation * animation, const std::string & name );
	virtual Animation * getMovement( const std::string & str );
	// virtual Animation * getMovement( const unsigned int x );
	virtual const std::map<std::string,Animation*> & getMovements();

	virtual inline int getShadow() const {
		return shadow;
	}
	virtual void getAttackCoords( int & x, int & y);
	const std::string getName() const;
	void setName( const std::string & str );

	inline void setSpeed( const double s ){
		speed = s;
	}

	inline void setDeath( int i ){
		death = i;
	}

	inline virtual int getDeath() const {
		return death;
	}

	inline virtual double getSpeed() const {
		return speed;
	}

	inline virtual unsigned int getCurrentMap() const {
		return current_map;
	}

	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual const std::string & getAttackName();
	virtual inline int getStatus() const {
		return status;
	}

        /*
	inline const double getMaxJumpHeight() const{
		return max_jump_height;
	}
        */

	inline double getMaxJumpingVelocity() const {
		return jumping_starting_velocity;
	}

	inline double getYVelocity() const {
		return y_velocity;
	}
	
	inline double getXVelocity() const {
		return x_velocity;
	}

	inline double getZVelocity() const {
		return z_velocity;
	}
	
	virtual void drawLifeBar( int x, int y, Bitmap * work );
	
	virtual inline const Filesystem::AbsolutePath & getPath() const {
            return path;
	}

	virtual int spawnTime(){
		return 0;
	}
	
	virtual void deathReset();

	virtual inline int getLives(){
		return lives;
	}

	inline void setLives( const int x ){
		lives = x;
	}

	virtual inline bool drawShadow() const {
		return draw_shadow;
	}

	virtual inline void setDrawShadow( bool b ){
		draw_shadow = b;
	}

	virtual Network::Message getCreateMessage();
	virtual Network::Message movedMessage();
	virtual Network::Message explodeMessage();
	virtual Network::Message healthMessage();
	virtual Network::Message ungrabMessage();
	virtual Network::Message animationMessage();
	virtual Network::Message showNameMessage( int amount );
	virtual Network::Message fallMessage( double x, double y );
        virtual Network::Message grabMessage(Object::networkid_t from, Object::networkid_t who);
	
	virtual void interpretMessage(World * world, Network::Message & m );
	
	virtual void fall( double x_vel, double y_vel );
	
    virtual int getInvincibility() const;

    virtual void addEffect(DrawEffect * effect);

protected:

	virtual Network::Message jumpMessage( double x, double y );
	
	int getShadowX();
	int getShadowY();

	virtual void filterEnemies( std::vector< Object * > & mine, std::vector< Object * > * all );
	// virtual void reMap( const std::string & from, const std::string & to, int id );
        virtual void addRemap(Remap * remap);
        /* true if a mapping between `from' and `to' doesn't already exist */
        virtual bool newRemap(const std::string & from, const std::string & to);
	void loadSelf(const Filesystem::AbsolutePath & filename ) throw ( LoadException );
	bool realCollision( ObjectAttack * obj );
	
        std::vector< BodyPart > getBodyParts( Animation * animation );

	virtual void landed( World * world );

	virtual void drawLifeBar( int x, int y, int he, Bitmap * work );
	
	virtual ECollide * getNormalCollide() const;

	/* helper functions */

	// void parseObject( string object_string ) throw( exception );
	void upperCase( std::string & who );


	virtual void setInvincibility(const int x);

	virtual inline void setExplode( bool b ){
		explode = b;
	}

	virtual bool getExplode(){
		return explode;
	}


protected:
        std::string name;
	// int speed;
	int type;
	int shadow;
	Bitmap * icon;

	// int actualx, actualy;

	/* last_obj: last object to attack us */
	// Object * last_obj;

	/* last_collide: keep track of last animation ticket that hurt us */
	// unsigned long last_collide;
        std::map< Object *, unsigned long > collision_objects;

	/* map from name of animation to animation */
	// map<string,Animation *> movements;

	/* current animation */
	Animation * animation_current;

	bool own_stuff;

/* specific to character */
protected:
	
	// int max_jump_height;
	double x_velocity;
	double y_velocity;
	double z_velocity;

	/*
	double jumping_x; // velocity in the X direction
	double jumping_velocity; // velocity in the Y direction
	*/
	double jumping_starting_velocity;

	/* status: jumping or on the ground or anything else */
	int status;

	/* when health reaches 0, death is set to a number such that the character blinks
	 * for a little while before going away
	 */
	int death;

	/* can be linked to another object, in some way */
	/* Total time this object has been grabbed */
	int grab_time;

	/* being thrown or not */
	bool thrown_status;

	double speed;
	
	/* object this is linked to */
	Object * linked;

	/* object that threw this */
	Object * thrown_from;

	bool moving;

	unsigned int current_map;
	/* map from id to map of animations */
        // std::map< int, std::map<std::string, Animation*> > mapper;
        std::map<int, Remap*> mapper;
        std::vector< Object * > projectiles;
        std::vector< BodyPart > body_parts;

	Sound * die_sound;
	Sound * landed_sound;
	Sound * squish_sound;
	int invincibility;
	int toughness;
	bool explode;
	int lives;

	bool draw_shadow;

        Filesystem::AbsolutePath path;

        /* list of faded animations that act as a trail */
        std::vector<AnimationTrail*> trails;
        /* starting time before a trail is produced, 0 means don't produce any */
        int trail_generator;
        /* countdown before a trail is produced */
        int trail_counter;
        /* lifetime for trails */
        int trail_life;

        std::vector<DrawEffect*> effects;
};

}

#endif
