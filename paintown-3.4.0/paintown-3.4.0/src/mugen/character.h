#ifndef paintown_mugen_character_h
#define paintown_mugen_character_h

#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "exception.h"

// Implement object_attack
#include "object/object_attack.h"
#include "network/network.h"
#include "util/pointer.h"
#include "input/input-map.h"
#include "animation.h"
#include "util.h"
#include "compiler.h"
#include "definitions.h"
#include "util/bitmap.h"

namespace Ast{
    class KeyList;
    class Key;
    class Value;
    class Section;
}

/*
namespace Mugen{
namespace Compiler{
    class Value;
}
}
*/

namespace PaintownUtil = Util;

class Bitmap;
class MugenItemContent;
class MugenSprite;
class MugenSound;
class MugenAnimation;
class MugenStage;

namespace Mugen{

class Behavior;

struct Constant{
    enum ConstantType{
        None,
        Double,
        ListOfDouble,
    };

    Constant():
    type(None){
    }

    Constant(double value):
    type(Double),
    double_(value){
    }

    Constant(std::vector<double> doubles):
    type(ListOfDouble),
    doubles(doubles){
    }

    ConstantType type;

    double double_;
    std::vector<double> doubles;
};

namespace Physics{

enum Type{
    None, /* N */
    Air, /* A */
    Stand, /* S */
    Crouch, /* C */
};

}

namespace StateType{
    extern std::string Stand;
    extern std::string Crouch;
    extern std::string Air;
    extern std::string LyingDown;
}

namespace Move{
    extern std::string Attack;
    extern std::string Idle;
    extern std::string Hit;
}

namespace AttackType{
    extern std::string Normal;
    extern std::string Special;
    extern std::string Hyper;

    enum Animation{
        NoAnimation = -1,
        Light = 0,
        Medium = 1,
        Hard = 2,
        Back = 3,
        Up = 4,
        DiagonalUp = 5
    };

    /* Badly named.. */
    enum Ground{
        None = 0,
        High,
        Low,
        Trip
    };
}

struct WinGame{
    /* TODO: add an explanation for each win type that describes how to
     * achieve that state.
     */
    enum WinType{
        Normal,
        Special,
        Hyper,
        NormalThrow,
        Cheese,
        TimeOver,
        Suicide,
        Teammate,
        /* Overlayed */
        /* is this needed now that the `perfect' bool exists? */
        Perfect,
    };

    WinGame():
    type(Normal),
    perfect(false){
    }

    WinType type;
    bool perfect;

};

namespace PhysicalAttack{
    extern std::string Normal;
    extern std::string Throw;
    extern std::string Projectile;
}

class Character;

struct HitDefinition{
    HitDefinition():
    alive(false),
    hitFlag("MAF"),
    animationType(AttackType::Light),
    animationTypeAir(AttackType::NoAnimation),
    animationTypeFall(AttackType::NoAnimation),
    spark(-1),
    guardSpark(-1),
    groundType(AttackType::None),
    airType(AttackType::None),
    groundHitTime(0),
    airHitTime(20),
    yAcceleration(0.35),
    guardVelocity(0),
    airJuggle(0)
    {}

    bool alive;

    void enable(){
        alive = true;
    }

    bool isEnabled() const {
        return alive;
    }

    void disable(){
        alive = false;
    }

    struct Attribute{
        /* StateType */
        std::string state;
        /* AttackType */
        std::string attackType;
        /* PhysicalAttack */
        std::string physics;
    } attribute;

    std::string hitFlag;
    std::string guardFlag;
    AttackType::Animation animationType;
    AttackType::Animation animationTypeAir;
    AttackType::Animation animationTypeFall;

    struct Priority{
        int hit;
        std::string type;
    } priority;

    struct Damage{
        Damage(){
        }

        int damage;
        int guardDamage;
    } damage;

    struct PauseTime{
        PauseTime():
            player1(0), player2(0){}
        int player1;
        int player2;
    } pause;

    struct GuardPauseTime{
        int player1;
        int player2;
    } guardPause;

    int spark;
    int guardSpark;
    
    struct SparkPosition{
        int x, y;
    } sparkPosition;

    struct HitSound{
        bool own;
        int group;
        int item;
    } hitSound;

    struct GetPower{
        GetPower():
            hit(0),
            guarded(0){
            }
        int hit;
        int guarded;
    } getPower;

    struct GuardHitSound{
        GuardHitSound():
            own(false),
            group(0),
            item(0){
            }

        bool own;
        int group;
        int item;
    } guardHitSound;

    AttackType::Ground groundType;
    AttackType::Ground airType;
    int groundSlideTime;
    int guardSlideTime;
    int groundHitTime;
    int guardGroundHitTime;
    int airHitTime;
    int guardControlTime;
    int guardDistance;
    double yAcceleration;

    struct GroundVelocity{
        GroundVelocity():
            x(0), y(0){}
        double x, y;
    } groundVelocity;

    double guardVelocity;

    struct AirVelocity{
        AirVelocity():
            x(0), y(0){}

        double x, y;
    } airVelocity;

    struct AirGuardVelocity{
        AirGuardVelocity():
            x(0), y(0){}

        double x, y;
    } airGuardVelocity;

    double groundCornerPushoff;
    double airCornerPushoff;
    double downCornerPushoff;
    double guardCornerPushoff;
    double airGuardCornerPushoff;
    int airGuardControlTime;
    int airJuggle;

    struct Distance{
        int x, y;
    };

    Distance minimum, maximum;
    Distance snap;
    int player1SpritePriority;
    int player2SpritePriority;
    int player1Facing;
    int player1GetPlayer2Facing;
    int player2Facing;
    int player1State;
    int player2State;
    int player2GetPlayer1State;
    int forceStand;

    struct Fall{
        Fall():
            yVelocity(0){
            }

        struct Shake{
            int time;
        } envShake;

        int fall;
        double xVelocity;
        double yVelocity;
        bool recover;
        int recoverTime;
        int damage;
        int airFall;
        int forceNoFall;
    } fall;
};

/* subset of HitDefinition stuff, just for reversals */
struct ReversalData{
    ReversalData():
        spark(-1),
        player1State(-1),
        player2State(-1),
        standing(true),
        crouching(true),
        aerial(true){
        }

    struct PauseTime{
        PauseTime():
            player1(0),
            player2(0){
            }

        int player1;
        int player2;
    } pause;

    int spark;

    struct HitSound{
        HitSound():
            own(false),
            group(-1),
            item(-1){
            }

        bool own;
        int group;
        int item;
    } hitSound;

    int player1State;
    int player2State;

    bool standing;
    bool crouching;
    bool aerial;
    std::vector<AttackType::Attribute> attributes;
};

struct HitState{
    HitState():
        shakeTime(0),
        hitTime(-1),
        hits(0),
        slideTime(0),
        returnControlTime(0),
        yAcceleration(0),
        yVelocity(0),
        xVelocity(0),
        guarded(false),
        damage(0){
        }

    void update(MugenStage & stage, const Character & guy, bool inAir, const HitDefinition & hit);
    int shakeTime;
    int hitTime;
    
    /* FIXME: handle hits somehow. corresponds to hitcount
     * hitcount: Returns the number of hits taken by the player in current combo. (int)
     */
    int hits;
    int slideTime;
    int returnControlTime;
    double yAcceleration;
    double yVelocity;
    double xVelocity;
    AttackType::Animation animationType;
    AttackType::Ground airType;
    AttackType::Ground groundType;
    AttackType::Ground hitType;
    bool guarded;
    int damage;

    struct Fall{
        Fall():
            fall(false),
            recover(true),
            recoverTime(0),
            xVelocity(0),
            yVelocity(0),
            damage(0){
            }

        struct Shake{
            int time;
        } envShake;

        bool fall;
        bool recover;
        int recoverTime;
        double xVelocity;
        double yVelocity;
        double damage;
    } fall;
};

class StateController;

/* comes from a StateDef */
class State{
public:
    State(int id);

    enum Type{
        Standing,
        Crouching,
        Air,
        LyingDown,
        Unchanged,
    };

    virtual inline void setType(Type t){
        type = t;
    }

    virtual inline void setAnimation(Compiler::Value * animation){
        this->animation = animation;
    }

    virtual inline int getState() const {
        return id;
    }

    virtual State * deepCopy() const;

    virtual inline void setControl(Compiler::Value * control){
        changeControl = true;
        this->control = control;
    }

    virtual void setJuggle(Compiler::Value * juggle);

    virtual void setVelocity(Compiler::Value * x, Compiler::Value * y);
    virtual void setPhysics(Physics::Type p);
    virtual void setPower(Compiler::Value * power);

    virtual inline Compiler::Value * getPower() const {
        return powerAdd;
    }

    virtual inline bool powerChanged() const {
        return changePower;
    }

    virtual void setMoveType(const std::string & type);
                    
    virtual inline void setHitDefPersist(bool what){
        hitDefPersist = what;
    }
    
    virtual inline bool doesHitDefPersist() const {
        return hitDefPersist;
    }

    virtual inline const std::vector<StateController*> & getControllers() const {
        return controllers;
    }

    virtual void addController(StateController * controller);
    virtual void addControllerFront(StateController * controller);

    virtual void transitionTo(const MugenStage & stage, Character & who);

    virtual ~State();

protected:
    int id;
    Type type;
    Compiler::Value * animation;
    bool changeControl;
    Compiler::Value * control;
    std::vector<StateController*> controllers;
    bool changeVelocity;
    Compiler::Value * velocity_x, * velocity_y;
    bool changePhysics;
    Physics::Type physics;
    bool changePower;
    Compiler::Value * powerAdd;
    std::string moveType;
    Compiler::Value * juggle;
    bool hitDefPersist;
};

class Command;

class Object: public Paintown::ObjectAttack {
public:
    Object(int alliance);
    Object(const int x, const int y, int alliance);
    Object(const Object & copy);

    virtual ~Object();
        
    /* paused from an attack */
    virtual bool isPaused() = 0;
    virtual Physics::Type getCurrentPhysics() const = 0;
    virtual double getGravity() const = 0;
    virtual void setYVelocity(double y) = 0;
    virtual double getYVelocity() const = 0;
    virtual void changeState(MugenStage & stage, int state, const std::vector<std::string> & inputs) = 0;
    virtual double getXVelocity() const = 0;
    virtual void setXVelocity(double x) = 0;
    virtual bool canTurn() const = 0;
    virtual void doTurn(MugenStage & stage) = 0;
    virtual double getStandingFriction() const = 0;
    virtual const std::string & getMoveType() const = 0;
    virtual HitDefinition & getHit() = 0;
    virtual const HitDefinition & getHit() const = 0;
    virtual int getCurrentJuggle() const = 0;
    virtual const std::vector<MugenArea> getAttackBoxes() const = 0;
    virtual const std::vector<MugenArea> getDefenseBoxes() const = 0;
    virtual int getDefaultSpark() const = 0;
    virtual int getDefaultGuardSpark() const = 0;
    virtual int getAttackDistance() const = 0;
    virtual void guarded(Object * enemy, const HitDefinition & hit) = 0;
    virtual void addPower(double d) = 0;
    virtual void wasHit(MugenStage & stage, Object * enemy, const HitDefinition & hit) = 0;
    virtual void didHit(Object * enemy, MugenStage & stage) = 0;
};

class Character: public Object {
public:
	// Location at dataPath() + "mugen/chars/"
	Character(const Filesystem::AbsolutePath & s );
	// Character(const char * location );
	Character(const Filesystem::AbsolutePath & s, int alliance );
	Character(const Filesystem::AbsolutePath & s, const int x, const int y, int alliance );
	Character(const Character &copy );

	virtual ~Character();

        enum Specials{
            Intro,
            Invisible,
            RoundNotOver,
            NoBarDisplay,
            NoBG,
            NoFG,
            NoStandGuard,
            NoCrouchGuard,
            NoAirGuard,
            NoAutoTurn,
            NoJuggleCheck,
            NoKOSnd,
            NoKOSlow,
            NoShadow,
            GlobalNoShadow,
            NoMusic,
            NoWalk,
            TimerFreeze,
            UnGuardable
        };
	
	// Convert to paintown character or whatever
	// Do code
	
	virtual void load(int useAct = 1);
	
	virtual void renderSprite(const int x, const int y, const unsigned int group, const unsigned int image, Bitmap *bmp, const int flip=1, const double scalex = 1, const double scaley = 1);
			   
	// Change palettes
	virtual void nextPalette();
	virtual void priorPalette();
	
	virtual inline const std::string & getName() const {
            return name;
        }

        virtual const Filesystem::AbsolutePath & getLocation() const {
            return location;
        }

        virtual inline const std::string & getAuthor() const {
            return author;
        }
	
	virtual inline const std::string & getDisplayName() const {
            return displayName;
        }
	
	virtual inline unsigned int getCurrentPalette() const {
            return currentPalette;
        }

        virtual inline const std::map<int, MugenAnimation*> & getAnimations() const {
            return animations;
        }

        virtual MugenAnimation * getAnimation(int id) const;

        /*
        virtual inline void setInput(const InputMap<Mugen::Keys> & inputLeft, const InputMap<Mugen::Keys> & inputRight){
            this->inputLeft = inputLeft;
            this->inputRight = inputRight;
        }
        */
	
	virtual inline const std::map<unsigned int, std::map<unsigned int, MugenSound *> >& getSounds() const {
            return sounds;
        }

	virtual inline const std::map<unsigned int, std::map<unsigned int, MugenSound *> >* getCommonSounds() const {
            return commonSounds;
        }

        virtual inline MugenSprite * getSprite(int group, int image){
            return this->sprites[group][image];
        }

        virtual const Bitmap * getCurrentFrame() const;
        MugenAnimation * getCurrentAnimation() const;

        virtual void drawReflection(Bitmap * work, int rel_x, int rel_y, int intensity);

    /*! This all the inherited members */
    virtual void act(std::vector<Paintown::Object*, std::allocator<Paintown::Object*> >*, World*, std::vector<Paintown::Object*, std::allocator<Paintown::Object*> >*);                       
    virtual void draw(Bitmap*, int cameraX, int cameraY);
    virtual void grabbed(Paintown::Object*);
    virtual void unGrab();
    virtual bool isGrabbed();
    virtual Paintown::Object* copy();
    virtual const std::string& getAttackName();
    virtual bool collision(Paintown::ObjectAttack*);
    virtual int getDamage() const;
    virtual bool isCollidable(Paintown::Object*);
    virtual bool isGettable();
    virtual bool isGrabbable(Paintown::Object*);
    virtual bool isAttacking();
    virtual int getWidth() const;
    virtual int getBackWidth() const;

    /* absolute X coordinate of the back of the character */
    virtual int getBackX() const;
    /* same thing for the front */
    virtual int getFrontX() const;
    virtual Network::Message getCreateMessage();
    virtual void getAttackCoords(int&, int&);
    virtual double minZDistance() const;
    virtual void attacked(World*, Paintown::Object*, std::vector<Paintown::Object*, std::allocator<Paintown::Object*> >&);

    virtual void changeState(MugenStage & stage, int state, const std::vector<std::string> & inputs);
    /* change back to states in the players own cns file */
    virtual void changeOwnState(MugenStage & stage, int state, const std::vector<std::string> & inputs);
    
    virtual void setAnimation(int animation);
    
    /* true if enemy can hit this */
    virtual bool canBeHit(Object * enemy);
    
    virtual inline int getAnimation() const {
        return this->currentAnimation;
    }
    
    virtual inline int getCurrentState() const {
        return this->currentState;
    }

    virtual State * getState(int id) const;
    virtual void setState(int id, State * what);

        virtual inline std::string getStateType() const {
            return stateType;
        }

        virtual inline void setStateType(const std::string & str){
            stateType = str;
        }

        virtual inline void setXVelocity(double x){
            this->velocity_x = x;
        }

        virtual inline double getXVelocity() const {
            return this->velocity_x;
        }
        
        virtual inline void setYVelocity(double y){
            this->velocity_y = y;
        }
        
        virtual inline double getYVelocity() const {
            return this->velocity_y;
        }

        virtual inline double getWalkBackX() const {
            return walkback;
        }
        
        virtual inline double getWalkForwardX() const {
            return walkfwd;
        }

        virtual inline void setWalkBackX(double x){
            walkback = x;
        }
        
        virtual inline void setWalkForwardX(double x){
            walkfwd = x;
        }
        
        virtual inline double getRunBackX() const {
            return runbackx;
        }

        virtual inline void setRunBackX(double x){
            runbackx = x;
        }

        virtual inline void setRunBackY(double x){
            runbacky = x;
        }

        virtual inline double getRunBackY() const {
            return runbacky;
        }
        
        virtual inline double getRunForwardX() const {
            return runforwardx;
        }

        virtual inline void setRunForwardX(double x){
            runforwardx = x;
        }
        
        virtual inline double getRunForwardY() const {
            return runforwardy;
        }
        
        virtual inline void setRunForwardY(double x){
            runforwardy = x;
        }

        virtual inline double getNeutralJumpingX() const {
            return jumpneux;
        }

        virtual inline void setNeutralJumpingX(double x){
            jumpneux = x;
        }
        
        virtual inline double getNeutralJumpingY() const {
            return jumpneuy;
        }
        
        virtual inline void setNeutralJumpingY(double x){
            jumpneuy = x;
        }

        virtual inline double getYPosition() const {
            return getY();
        }
        
        virtual inline double getPower() const {
            return power;
        }

        virtual inline void setPower(double p){
            power = p;
        }

        virtual void addPower(double d);

        virtual inline bool hasControl() const {
            return has_control;
        }

        virtual inline void setControl(bool b){
            has_control = b;
        }

        virtual inline void setJumpBack(double x){
            jumpback = x;
        }

        virtual inline double getJumpBack() const {
            return jumpback;
        }

        virtual inline void setJumpForward(double x){
            jumpfwd = x;
        }

        virtual inline double getJumpForward() const {
            return jumpfwd;
        }
        
        virtual inline void setRunJumpBack(double x){
            runjumpback = x;
        }

        virtual inline int getRunJumpBack() const {
            return runjumpback;
        }

        virtual inline void setRunJumpForward(double x){
            runjumpfwd = x;
        }

        virtual inline double getRunJumpForward() const {
            return runjumpfwd;
        }

        virtual inline void setAirJumpNeutralX(double x){
            airjumpneux = x;
        }

        virtual inline double getAirJumpNeutralX() const {
            return airjumpneux;
        }
        
        virtual inline void setAirJumpNeutralY(double y){
            airjumpneuy = y;
        }

        virtual inline double getAirJumpNeutralY() const {
            return airjumpneuy;
        }

        virtual inline void setAirJumpBack(double x){
            airjumpback = x;
        }

        virtual inline double getAirJumpBack() const {
            return airjumpback;
        }

        virtual inline void setAirJumpForward(double x){
            airjumpfwd = x;
        }

        virtual inline double getAirJumpForward() const {
            return airjumpfwd;
        }

        virtual inline int getStateTime() const {
            return stateTime;
        }

        virtual inline int getPreviousState() const {
            return previousState;
        }

        virtual inline const std::string & getMoveType() const {
            return moveType;
        }

        virtual inline void setMoveType(const std::string & str){
            moveType = str;
        }

        virtual void resetStateTime();

        virtual void setVariable(int index, const RuntimeValue & value);
        virtual void setFloatVariable(int index, const RuntimeValue & value);
        virtual void setSystemVariable(int index, const RuntimeValue & value);
        virtual RuntimeValue getVariable(int index) const;
        virtual RuntimeValue getFloatVariable(int index) const;
        virtual RuntimeValue getSystemVariable(int index) const;

        virtual inline Physics::Type getCurrentPhysics() const {
            return currentPhysics;
        }

        virtual void setCurrentPhysics(Physics::Type p){
            currentPhysics = p;
        }

        virtual void setGravity(double n){
            gravity = n;
        }

        virtual double getGravity() const {
            return gravity;
        }

        virtual void setLieDownTime(int x){
            lieDownTime = x;
        }

        virtual int getLieDownTime() const {
            return lieDownTime;
        }

        virtual void setStandingFriction(double n){
            standFriction = n;
        }

        virtual double getStandingFriction() const {
            return standFriction;
        }

        virtual bool hasAnimation(int index) const;

        virtual inline void toggleDebug(){
            debug = !debug;
        }

        virtual HitDefinition & getHit(){
            return this->hit;
        }
        
        virtual const HitDefinition & getHit() const {
            return this->hit;
        }
        
        void enableHit();
        void disableHit();

        virtual inline int getHeight() const {
            return height;
        }

        virtual inline void setHeight(int h){
            height = h;
        }

        /* `this' hit `enemy' */
        void didHit(Object * enemy, MugenStage & stage);

        /* `enemy' hit `this' with hitdef `hit' */
        void wasHit(MugenStage & stage, Object * enemy, const HitDefinition & hit);

        /* `this' character guarded `enemy' */
        void guarded(Object * enemy, const HitDefinition & hit);

        /* true if the player is holding back */
        bool isBlocking(const HitDefinition & hit);
        /* true if the player was attacked and blocked it */
        bool isGuarding() const;

        virtual const HitState & getHitState() const {
            return hitState;
        }

        virtual HitState & getHitState(){
            return hitState;
        }

        const std::vector<MugenArea> getAttackBoxes() const;
        const std::vector<MugenArea> getDefenseBoxes() const;

        /* paused from an attack */
        virtual bool isPaused();
        /* time left in the hitpause */
        virtual int pauseTime() const;

        /* prevent character from being hit, like after a KO */
        virtual void setUnhurtable();
	
        using Paintown::Object::takeDamage;
        /* if kill is true then the damage can reduce health below 1, otherwise
         * health cannot go below 1
         * if defense is true then the defenseMultiplier is taken into account
         */
        virtual void takeDamage(World & world, Paintown::ObjectAttack * obj, int amount, bool kill, bool defense);
        /* passes true for kill and defense */
        virtual void takeDamage(World & world, Paintown::ObjectAttack * obj, int amount);

        /* character can be hit */
        virtual void setHurtable();

        bool canTurn() const;
        void doTurn(MugenStage & stage);

        /* recover after falling */
        virtual bool canRecover() const;

        virtual MugenSound * getSound(int group, int item) const;
        virtual MugenSound * getCommonSound(int group, int item) const;

        virtual inline void setJugglePoints(int x){
            airjuggle = x;
        }

        virtual inline int getJugglePoints() const {
            return airjuggle;
        }

        virtual inline void setCurrentJuggle(int j){
            currentJuggle = j;
        }

        virtual inline int getCurrentJuggle() const {
            return currentJuggle;
        }

        virtual inline void setCommonSounds(const std::map< unsigned int, std::map< unsigned int, MugenSound * > > * sounds){
            this->commonSounds = sounds;
        }

        virtual inline void setExtraJumps(int a){
            airjumpnum = a;
        }

        virtual inline int getExtraJumps() const {
            return airjumpnum;
        }

        virtual inline double getAirJumpHeight() const {
            return airjumpheight;
        }

        virtual inline void setAirJumpHeight(double f){
            airjumpheight = f;
        }

        virtual int getCurrentCombo() const;

        virtual inline int getHitCount() const {
            return hitCount;
        }

        virtual inline const std::vector<WinGame> & getWins() const {
            return wins;
        }

        virtual inline void clearWins(){
            wins.clear();
        }

        virtual void addWin(WinGame win);

        virtual inline int getMatchWins() const {
            return matchWins;
        }

        virtual void addMatchWin();

        virtual void resetPlayer();

        virtual inline void setBehavior(Behavior * b){
            behavior = b;
        }

        virtual inline Behavior * getBehavior(){
            return this->behavior;
        }

        virtual inline void setDefaultSpark(int s){
            sparkno = s;
        }

        virtual inline void setDefaultGuardSpark(int s){
            guardsparkno = s;
        }

        virtual inline int getDefaultSpark() const {
            return sparkno;
        }

        virtual inline int getDefaultGuardSpark() const {
            return guardsparkno;
        }

        virtual inline void setRegeneration(bool r){
            this->regenerateHealth = r;
        }
        
        virtual inline int getAttackDistance() const {
	    return this->attackdist;
	}

        virtual void setDrawOffset(double x, double y);

        virtual void setAfterImageTime(int time);

        virtual void updateAngleEffect(double angle);
        virtual double getAngleEffect() const;
        virtual void drawAngleEffect(double angle, bool setAngle, double scaleX, double scaleY);

        virtual void assertSpecial(Specials special);

        virtual void setWidthOverride(int edgeFront, int edgeBack, int playerFront, int playerBack);
        virtual void setHitByOverride(int slot, int time, bool standing, bool crouching, bool aerial, const std::vector<AttackType::Attribute> & attributes);
        virtual void setHitOverride(int slot, const std::string & attribute, int state, int time, bool air);

        virtual void setDefenseMultiplier(double defense);
        virtual void setAttackMultiplier(double attack);

        virtual void doFreeze();
	
        virtual void moveX( const int x );
        virtual void moveYNoCheck(double y);

        virtual void setSpritePriority(int priority);
        
        virtual void setTemporaryAnimation(MugenAnimation * animation);

        virtual bool isHelper() const;

        /* increase combo count */
        virtual void addCombo(int combo);

        virtual void disablePushCheck();

        virtual void setReversalActive();
        ReversalData & getReversal();

        virtual void setTransOverride(TransType type, int alphaFrom, int alphaTo);

protected:
    void initialize();

    virtual inline void setCurrentState(int state){
        this->currentState = state;
    }
    
    virtual void loadSelectData();

    virtual void loadCmdFile(const Filesystem::RelativePath & path);
    virtual void loadCnsFile(const Filesystem::RelativePath & path);
    virtual void loadStateFile(const Filesystem::AbsolutePath & base, const std::string & path);

    virtual void addCommand(Command * command);

    virtual void setConstant(std::string name, const std::vector<double> & values);
    virtual void setConstant(std::string name, double value);

    virtual std::vector<std::string> doInput(const MugenStage & stage);
    virtual bool doStates(MugenStage & stage, const std::vector<std::string> & active, int state);

    void destroyRaw(const std::map< unsigned int, std::map< unsigned int, MugenSprite * > > & sprites);

    void resetJump(MugenStage & stage, const std::vector<std::string> & inputs);
    void doubleJump(MugenStage & stage, const std::vector<std::string> & inputs);
    void stopGuarding(MugenStage & stage, const std::vector<std::string> & inputs);

    /*
    internalCommand_t resetJump;
    internalCommand_t doubleJump;
    internalCommand_t stopGuarding;
    */

    virtual void fixAssumptions();
    virtual StateController * parseState(Ast::Section * section);
    virtual State * parseStateDefinition(Ast::Section * section, const Filesystem::AbsolutePath & path);

    // InputMap<Mugen::Keys> & getInput();

protected:

	/* Location is the directory passed in ctor
	This is where the def is loaded and all the relevant files
	are loaded from
	*/
    Filesystem::AbsolutePath location;
	
        Filesystem::AbsolutePath baseDir;
	
	/* These items are taken from character.def file */
	
	/* Base definitions */
	
	// Name of Character
	std::string name;
	// Name of Character to Display why there is two is beyond me, retards
	std::string displayName;
	// Version date (unused)
	std::string versionDate;
	// Version that works with mugen (this isn't mugen)
	std::string mugenVersion;
	// Author 
	std::string author;
	// Palette defaults
	std::vector< unsigned int> palDefaults;
	unsigned int currentPalette;
	
	/* Relevant files */
	
	// Command set file
        Filesystem::RelativePath cmdFile;
	// Constants
	std::string constantsFile;
        /*
	// States
	std::string stateFile;
	// Common States
	std::string commonStateFile;
	// Other state files? I can't find documentation on this, in the meantime we'll wing it
	std::string stFile[12];
        */
	// Sprites
	std::string sffFile;
	// animations
	std::string airFile;
	// Sounds
	std::string sndFile;
	// Palettes max 12
        std::map<int, std::string> palFile;
	
	// Arcade mode ( I don't think we will be using this anytime soon )
	std::string introFile;
	std::string endingFile;
	
	/* Now on to the nitty gritty */
	
	/* Player Data and constants comes from cns file */
	
	/* Section [Data] */
	
	// Life
	int life;
	// Attack
	int attack;
	// Defence
	int defence;
	// How much to bring up defese on fall
	int falldefenseup;
	// How long to lie down when fall
	int lieDownTime;
        /* starting air juggle points */
	int airjuggle;

        /* number of juggle points left */
        int juggleRemaining;

        /* number of juggle points the current move will take */
        int currentJuggle;

	// Default Hit Spark Number for hitdefs ???
	int sparkno;
	// Default guard spark number
	int guardsparkno;
	// Echo on KO (I guess is for the death sound)
	bool koecho;
	// Volume offset on characters sounds
	int volumeoffset;
	// Maybe used in VS mode later
	/* According to the definition: 
	Variables with this index and above will not have their values
	reset to 0 between rounds or matches. There are 60 int variables,
	indexed from 0 to 59, and 40 float variables, indexed from 0 to 39.
	If omitted, then it defaults to 60 and 40 for integer and float
	variables repectively, meaning that none are persistent, i.e. all
	are reset. If you want your variables to persist between matches,
	you need to override state 5900 from common1.cns.
	*/
	int intpersistindex;
	int floatpersistindex;
	
	/* Section [Size] */
	
	// Horizontal Scaling Factor
	int xscale;
	//Vertical scaling factor.
	int yscale;
	//      ;Player width (back, ground)
	int groundback;
	//   ;Player width (front, ground)
	int groundfront;
	//      ;Player width (back, air)
	int airback;
	//     ;Player width (front, air)
	int airfront;
	//  = 60          ;Height of player (for opponent to jump over)
	int height;
	// = 160    ;Default attack distance
	int attackdist;
	//  = 90 ;Default attack distance for projectiles
	int projattackdist;
	//  = 0     ;Set to 1 to scale projectiles too
	bool projdoscale;
	// = -5, -90   ;Approximate position of head
	Mugen::Point headPosition;
	//  = -5, -60    ;Approximate position of midsection
	Mugen::Point midPosition;
	//  = 0     ;Number of pixels to vertically offset the shadow
	int shadowoffset;
	// = 0,0    ;Player drawing offset in pixels (x, y)
	Mugen::Point drawOffset;
	
	/* Section [Velocity] */
	
	//   = 2.4      ;Walk forward
	double walkfwd;
	// = -2.2     ;Walk backward
	double walkback;
	//  = 4.6, 0    ;Run forward (x, y)
	double runforwardx;
	double runforwardy;
	// = -4.5,-3.8 ;Hop backward (x, y)
	double runbackx;
	double runbacky;
	// = 0,-8.4    ;Neutral jumping velocity (x, y)
	double jumpneux;
	double jumpneuy;
	// = -2.55    ;Jump back Speed (x, y)
	double jumpback;
	// = 2.5       ;Jump forward Speed (x, y)
	double jumpfwd;
	// = -2.55,-8.1 ;Running jump speeds (opt)
	double runjumpback;
	// = 4,-8.1      ;.
	double runjumpfwd;
	// = 0,-8.1      ;.
	double airjumpneux;
	double airjumpneuy;
	// Air jump speeds (opt)
	double airjumpback;
	double airjumpfwd;

        double power;
	
	/* Movement */
	
	//  = 1      ;Number of air jumps allowed (opt)
	int airjumpnum;
	//  = 35  ;Minimum distance from ground before you can air jump (opt)
	double airjumpheight;
	// = .44         ;Vertical acceleration
	double yaccel;
	//  = .85  ;Friction coefficient when standing
	double standfriction;
	//  = .82 ;Friction coefficient when crouching
	double crouchfriction;

	/* Sprites */
	std::map< unsigned int, std::map< unsigned int, MugenSprite * > > sprites;
	// Bitmaps of those sprites
	std::map< unsigned int, std::map< unsigned int, Bitmap * > > bitmaps;
	
	/* Animation Lists stored by action number, ie [Begin Action 500] */
	std::map< int, MugenAnimation * > animations;
	
	/* Sounds */
	std::map< unsigned int, std::map< unsigned int, MugenSound * > > sounds;
        /* sounds from the stage */
        const std::map< unsigned int, std::map< unsigned int, MugenSound * > > * commonSounds;
	
	/* Commands, Triggers or whatever else we come up with */
        std::map<std::string, Constant> constants;

        std::vector<Command *> commands;

        std::map<int, State*> states;

        int currentState;
        int previousState;
        int currentAnimation;
	
	// Debug state
	bool debug;

        /*
        InputMap<Mugen::Keys> inputLeft;
        InputMap<Mugen::Keys> inputRight;
        */

        double velocity_x, velocity_y;

        bool has_control;

        /* how much time the player has been in the current state */
        int stateTime;
    
        /* dont delete these in the destructor, the state controller will do that */
        std::map<int, RuntimeValue> variables;
        std::map<int, RuntimeValue> floatVariables;
        std::map<int, RuntimeValue> systemVariables;
        Physics::Type currentPhysics;

        /* yaccel */
        double gravity;

        /* stand.friction */
        double standFriction;

        /* S (stand), C (crouch), A (air), L (lying down) */
        std::string stateType;
        std::string moveType;

        HitDefinition hit;

        HitState hitState;
        unsigned int lastTicket;

        int combo;
        // int nextCombo;
        
        int hitCount;

        std::vector<WinGame> wins;

        int matchWins;

        Compiler::Value * internalJumpNumber;

        Behavior * behavior;

        /* true if the player is holding the back button */
        bool blocking;

        //! regenerate health?
        bool regenerateHealth;
        bool regenerating;
        int regenerateTime;
        int regenerateHealthDifference;

        /* used to communicate the need to guard in the engine */
        bool needToGuard;

        /* true if the player is currently guarding an attack */
        bool guarding;

public:
        /* this is public so the AfterImage state-controller can easily communicate
         * RGB information
         */
        struct AfterImage{
            AfterImage():
                currentTime(0),
                timegap(0),
                framegap(0),
                lifetime(0),
                length(0),
                translucent(Default){
                }

            struct Frame{
                Frame():
                    sprite(NULL),
                    life(0){
                    }

                Frame(MugenFrame * sprite, Effects effects, int life, int x, int y):
                    sprite(sprite),
                    extra(-1),
                    effects(effects),
                    life(life),
                    x(x),
                    y(y){
                    }

                MugenFrame * sprite;
                Bitmap cache;
                unsigned int extra;
                Effects effects;
                int life;
                int x;
                int y;
            };

            struct RGB{
                double red, green, blue;
            };

            /* count ticks */
            int currentTime;
            int timegap;
            int framegap;

            /* time left for the afterimage effects will be added */
            int lifetime;

            /* maximum number of afterimage frames */
            unsigned int length;
            TransType translucent;

            int paletteColor;
            bool invertColor;

            RGB bright;
            RGB contrast;
            RGB postBright;
            RGB add;
            RGB multiply;

            std::deque<Frame> frames;
        } afterImage;
protected:

        struct WidthOverride{
            WidthOverride():
                enabled(false),
                edgeFront(0),
                edgeBack(0),
                playerFront(0),
                playerBack(0){
                }

            bool enabled;
            int edgeFront, edgeBack;
            int playerFront, playerBack;
        } widthOverride;

        struct HitByOverride{
            HitByOverride():
            time(0){
            }

            bool standing;
            bool crouching;
            bool aerial;
            int time;
            std::vector<AttackType::Attribute> attributes;
        } hitByOverride[2];

        /* reduces damage taken */
        double defenseMultiplier;
        /* increase attack damage */
        double attackMultiplier;

        /* if frozen then the player won't move */
        bool frozen;

        ReversalData reversal;
        bool reversalActive;

        struct TransOverride{
            TransOverride():
            enabled(false){
            }

            bool enabled;
            TransType type;
            int alphaSource;
            int alphaDestination;
        } transOverride;
public:
        /* this definition is down here so we can get access to the AfterImage
         * struct definition
         */
        virtual void setAfterImage(int time, int length, int timegap, int framegap, TransType effects, int paletteColor, bool invertColor, const AfterImage::RGB & bright, const AfterImage::RGB & contrast, const AfterImage::RGB & postBright, const AfterImage::RGB & add, const AfterImage::RGB & multiply);

        struct SpecialStuff{
            SpecialStuff():
            invisible(false){
            }

            void reset(){
                invisible = false;
            }

            bool invisible;
        } special;
};

}

#endif
