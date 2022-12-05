#ifndef mugen_stage_h
#define mugen_stage_h

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "exception.h"
#include "util.h"

#include "game/world.h"

class Bitmap;
namespace Console{
    class Console;
}

namespace Mugen{
    class Character;
    class Helper;
    class Object;
    class Background;
    class Spark;
    class Effect;
    class GameInfo;

class Effect{
public:
    Effect(const Character * owner, MugenAnimation * animation, int id, int x, int y);
    
    virtual void draw(const Bitmap & work, int cameraX, int cameraY);
    virtual void logic();
    virtual bool isDead();

    virtual inline const Character * getOwner() const {
        return owner;
    }

    virtual inline double getX() const {
        return x;
    }

    virtual inline double getY() const {
        return y;
    }

    virtual void setX(double x){
        this->x = x;
    }

    virtual void setY(double y){
        this->y = y;
    }

    virtual inline int getId() const {
        return id;
    }

    virtual ~Effect();
protected:
    const Character * owner;
    MugenAnimation * animation;
    int id;
    double x;
    double y;
};

}

namespace Ast{
    class Section;
}

class MugenItemContent;
class MugenSprite;
class MugenSound;
class MugenAnimation;
// class MugenBackgroundManager;

struct PlayerData {
    double oldx;
    double oldy;
    bool leftTension;
    bool rightTension;
    bool leftSide;
    bool rightSide;
    bool above;
    bool jumped;
};

class MugenStage: public World {
public:
    // Location at dataPath() + "mugen/stages/"
    MugenStage(const Filesystem::AbsolutePath & s);
    // MugenStage( const char * location );

    virtual ~MugenStage();

    void load();

    inline const std::string &getName() const {
        return name;
    }
    /*
       inline const std::map<int, MugenAnimation*> & getAnimations() const {
       return animations;
       }*/

    inline double getCameraX() const { return camerax; }
    inline double getCameraY() const { return cameray; }
    inline int getTension() const { return tension; }
    inline int getFloorTension() const { return floortension; }
    inline int getZ() const { return zoffset; }
    /* get position of the floor */
    int getFloor() const;

    inline unsigned long int getTicks(){ return ticker; }

    void setCamera( const double x, const double y );
    void moveCamera( const double x, const double y );

    // Get background by ID
    //MugenBackground *getBackground( int ID );

    // This will update ticks and controllers. When ready pass the player objects to properly allow camera to follow
    // For now use view port, x and y screen size will be used to determine width/height
    void logic();

    // Render the backgrounds appropriately
    void render( Bitmap *work );

    // Reset scenario
    void reset();

    // Add player1 people
    virtual void addPlayer1( Paintown::Object * o );

    // Add player2 people
    virtual void addPlayer2( Paintown::Object * o );

    // Set player health
    virtual void setPlayerHealth(int health);

    //! Set match
    virtual void setMatchOver(bool over){
        this->gameOver = over;
    }

    //! Check match
    virtual bool isMatchOver() const {
        return this->gameOver;
    }

    virtual Mugen::GameInfo * getGameInfo() const {
        return gameHUD;
    }

    //! Do continue screen return true to continue playing, false to end
    virtual bool doContinue(const Mugen::PlayerType & type, InputMap<Mugen::Keys> &,  const Bitmap &);

    /* 1 is normal, 0<rate<1 is slower, 1<rate is faster */
    virtual void setGameRate(double rate);

    virtual int currentZOffset() const;

    virtual void pause();
    virtual void unpause();
    virtual void changePause();
    virtual bool isPaused();

    // Console
    virtual void toggleConsole();

    // Render debug stuff like lines and crap
    virtual void toggleDebug();

    // Inherited world actions
    virtual void act();
    virtual void draw( Bitmap * work );
    virtual void addObject(Paintown::Object * o);
    virtual bool finished() const;
    virtual void reloadLevel() throw( LoadException );
    virtual Script::Engine * getEngine() const;
    /* upper left hand corner of the screen */
    virtual int getX();
    virtual int getY();
    /* this shouldn't be here */
    // I guess ignore this one
    virtual const std::deque<Bitmap*> & getScreenshots();
    virtual int levelLength() const;
    // Since this isn't a paintown level, I guess block wouldn't apply
    virtual const Block * currentBlock() const;
    /* bleh.. */
    virtual void addEnemy(Paintown::Enemy * obj);
    virtual Paintown::Object * findObject(int id);
    virtual int getMaximumZ();
    virtual int getMinimumZ();
    virtual void drawMiniMaps( bool b );
    virtual bool shouldDrawMiniMaps();
    virtual void killAllHumans( Paintown::Object * player );
    virtual void addMessage(Network::Message m, Network::Socket from = 0, Network::Socket to = 0);
    virtual Network::Message createBangMessage( int x, int y, int z );

    static const std::string getStageName(const std::string &filename) throw (MugenException);

    virtual Mugen::Character * getEnemy(const Mugen::Character * who) const;
    virtual std::vector<Mugen::Character *> getTargets(int id, const Mugen::Character * from) const;

    virtual int getGameTime() const;

    /* pause for dramatic effect */
    virtual void doSuperPause(int time, int animation, int positionX, int positionY);
    virtual void doPause(int time, int buffer, int moveAllowed, bool pauseBackground);

    /* make dust effects */
    virtual void createDust(int x, int y);

    /* get an animation from fightfx.sff */
    virtual MugenAnimation * getFightAnimation(int id);

    virtual void addEffect(Mugen::Effect * effect);
    virtual void removeEffects(const Mugen::Character * owner, int id);
    virtual int countMyEffects(const Mugen::Character * owner) const;
    virtual int countMyHelpers(const Mugen::Character * owner) const;
    virtual const Mugen::Character & findRoot(const Mugen::Character & who) const;
    virtual std::vector<Mugen::Helper*> findHelpers(const Mugen::Character * owner, int id) const;
    virtual Mugen::Effect * findEffect(const Mugen::Character * owner, int id);
    virtual std::vector<Mugen::Effect *> findEffects(const Mugen::Character * owner, int id);

    virtual void setEnvironmentColor(int color, int time, bool under); 

    virtual void removeHelper(Mugen::Character * who);

    // Alliance setting
    enum teams{
        Player1Side = 5,
        Player2Side
    };

    int maximumRight() const;
    int maximumLeft() const;
protected:
    struct cymk_holder{
        cymk_holder():c(0),m(0),y(0),k(0){}
        int c, m, y, k;
    };

    /* section loaders */
    void loadSectionInfo(Ast::Section * section);
    void loadSectionCamera(Ast::Section * section);
    void loadSectionPlayerInfo(Ast::Section * section);
    void loadSectionBound(Ast::Section * section);
    void loadSectionStageInfo(Ast::Section * section);
    void loadSectionShadow(Ast::Section * section, cymk_holder & shadow);
    void loadSectionReflection(Ast::Section * section);

    void updatePlayer(Paintown::Object *o);
    void physics(Paintown::Object * o);
    bool doBlockingDetection(Mugen::Object * obj1, Mugen::Object * obj2);
    bool doCollisionDetection(Mugen::Object * obj1, Mugen::Object * obj2);
    void destroyRaw(const std::map< unsigned int, std::map< unsigned int, MugenSprite * > > & sprites);

protected:

    void addSpark(int x, int y, int sparkNumber);
    void playSound(int group, int item, bool own);

    std::vector<Paintown::Object*> getOpponents(Paintown::Object * who);

    /* Location is the directory passed in ctor
       This is where the def is loaded and all the relevant files
       are loaded from
       */
    Filesystem::AbsolutePath location;

    Filesystem::AbsolutePath baseDir;

    /* These items are taken from stage.def file */

    /* Base definitions */

    // Name of Stage
    std::string name;

    /*;Camera starting position: Usually 0 for both */
    int startx;
    int starty;

    /*;Left and right bound of camera */

    int boundleft;
    int boundright;

    /*;High and low bound of camera 
      ;High is a negative number. Make is more negative if you want to
      ;camera to be able to move higher.
      ;Low should usually be 0.
      ;If omitted, defaults to -25 and 0 respectively */
    int boundhigh;
    int boundlow;

    /*
       ;This is how much the camera will move vertically towards the
       ;highest player. Valid values are from 0 to 1. A value of 0 will mean
       ;the camera does not move up at all. A value of 1 will makes the camera
       ;follow the highest player.
       ;Typically .2 for normal-sized backgrounds. You may need to pull this
       ;value up for taller backgrounds.*/
    double verticalfollow;

    /*
       ;Minimum vertical distance the highest player has to be from the floor,
       ;before the camera starts to move up to follow him. For extremely
       ;tall stages, a floor tension of about 20-30 coupled with a
       ;vertical-follow of .8 allows the camera to follow the action. */
    int floortension;

    /*;Horizontal distance player is from edge before camera starts to move
      ;left or right. Typically 50 or 60.*/
    int tension;

    /*
       ;--------------------------------------------------------
       [PlayerInfo]
       ;--- Player 1 ---
       ;Player 1 starting coordinates.
       ;p1startx is typically -70 and p2startx is 70.
       ;p1starty and p1startz should be 0.*/
    int p1startx; //            ;Starting x coordinates
    int p1starty; //            ;Starting y coordinates
    int p1startz; //            ;Starting z coordinates
    int p1facing; //            ;Direction player faces: 1=right, -1=left

    /*;--- Player 2 --- */
    int p2startx;
    int p2starty;
    int p2startz;
    int p2facing;

    /*;--- Common ---
      ;Don't change these values.*/
    int leftbound; // ;Left bound (x-movement)
    int rightbound; // ;Right bound
    int topbound; //     ;Top bound (z-movement)
    int botbound; //     ;Bottom bound

    /*
       ;--------------------------------------------------------
       [Scaling]
       ;No need to change these values
       int topz;	//     = 0       ;Top z-coordinate for scaling
       int botz; //   ;Bottom z-coordinate for scaling
       double topscale; //      ;Scale to use at top
       double botscale; //     ;Scale to use at bottom*/

    /*
       ;--------------------------------------------------------
       [Bound]
       ;Distance from left/right edge of screen that player can move to
       ;Typically 15 */
    int screenleft; //    ;Dist from left of screen that player can move to
    int screenright; //   ;Right edge

    /*;--------------------------------------------------------
      [StageInfo]
      ;Z offset for drawing
      ;Adjust this value to move the ground level up/down in the screen.
      ;It's the position where the players stand at.
      ;Up - smaller, Down - larger
      ;*** */
    int zoffset;
    /*
       zoffsetlink = BG_ID ;(optional)
       If this parameter is specified, it links the zoffset value to the y-position of a background element with ID number BG_ID. 
       For example, you can link the value to a dummy element (refer to "Other background element types" in section III: 
       Description of background elements) with a sin.y parameter to make the characters bob up and down in a sinusoidal fashion.
       */
    int zoffsetlink;

    //;Leave this at 1. It makes the players face each other
    bool autoturn;

    /*;Set the following to 1 to have the background be reset between
      ;rounds.*/
    bool resetBG;

    /*;--------------------------------------------------------
      [Shadow]
      ;This is the shadow darkening intensity. Valid values range from
      ;0 (lightest) to 256 (darkest). Defaults to 128 if omitted.*/
    int shadowIntensity;

    // Reflection aparently newer versions have it under shadow
    bool reflect;

    /*;This is the shadow color given in r,g,b. Valid values for each
      ;range from 0 (lightest) to 255 (darkest).
      ;Defaults to 0,0,0 if omitted.
      ;intensity and color's effects add up to give the final shadow
      ;result.*/
    int shadowColor;  // this = 0,0,0 gets converted by Bitmap::makeColor

    /*;This is the scale factor of the shadow. Use a big scale factor
      ;to make the shadow longer. You can use a NEGATIVE scale factor
      ;to make the shadow fall INTO the screen.
      ;Defaults to 0.4 if omitted.*/
    double shadowYscale;

    /*;This parameter lets you set the range over which the shadow is
      ;visible. The first value is the high level, and the second is
      ;the middle level. Both represent y-coordinates of the player.
      ;A shadow is invisible if the player is above the high level,
      ;and fully visible if below the middle level. The shadow is
      ;faded in between the two levels. This gives an effect of the
      ;shadow fading away as the player gets farther away from the
      ;ground. If omitted, defaults to no level effects (shadow is
      ;always fully visible).*/
    //fade.range = -400,-100
    int shadowFadeRangeHigh;
    int shadowFadeRangeMid;

    /*;--------------------------------------------------------
      [Reflection]
      ;Intensity of reflection (from 0 to 256). Set to 0 to have no
      ;reflection. Defaults to 0.*/
    int reflectionIntensity;

    /*;--------------------------------------------------------
      [Music]
      ;Put a filename for a MOD, MP3 or MIDI here, or just leave it
      ;blank if you don't want music. If an invalid filename is
      ;given, then no music will play. To play CD audio, put
      ;the track number followed by ".da". Using a track number of
      ;0 will play a random audio track. For example, to play
      ;track 3 from a music CD, use:
      ;  bgmusic = 3.da 
      This option is ignored in paintown, no support for those formats*/
    //int bgmusic;

    /*;Adjust the volume. 0 is normal, negative for softer, and
      ;positive for louder (only for mp3, mods and CDA)
      Probably won't be used unless support for those items above get added*/
    //int bgvolume;

    /* Main background definitions.. ie [BGdef] */

    // Sprite file
    //std::string sffFile;
    // This is debug for backgrounds probably won't use it, clears to magenta before drawing first layer 
    //bool debugbg;

    /* Sprites */
    //std::map< unsigned int, std::map< unsigned int, MugenSprite * > > sprites;

    /* Animation Lists stored by action number, ie [Begin Action 500] */
    //std::map< int, MugenAnimation * > animations;

    /* Backgrounds */
    //std::vector< MugenBackground * > backgrounds;

    /* Foregrounds */
    //std::vector< MugenBackground * > foregrounds;

    /* Sounds */
    std::map< unsigned int, std::map< unsigned int, MugenSound * > > sounds;

    // MugenBackgroundManager *background;
    Mugen::Background * background;

    /* Commands, Triggers or whatever else we come up with */

    // Our working bitmap
    Bitmap *board;

    // Our real axis givin that we have to use negatives
    int xaxis;
    int yaxis;

    // Camera location on stage
    double camerax;
    double cameray;

    // Start the stage sometime
    bool stageStart;

    // Our ticker/frames during duration of the entire stage lifetime
    unsigned long int ticker;

    // Total rounds set to -1 for infinite rounds
    int totalRounds;

    // Current round
    int round;

    // Total Time per round -1 for infinite time
    int totalTime;

    // Current time
    int time;

    // p1 round points (this also goes for team players ie 2v1 etc)
    int p1points;

    // p2 round points
    int p2points;

private:

    // Our output Console
    Console::Console *console;

    // Debug enabled?
    bool debugMode;

    void cleanup();
    bool isaPlayer(Paintown::Object * o) const;

    // player list so we can distinguish
    std::vector<Paintown::Object *> players;

    /* FIXME: sort of a hack which lets other classes with a reference to the
     * stage call stage.addObject(blah). really a vector should be passed
     * to the calling class that the class can add to and the stage will
     * insert all the objects from that vector into the `objects' vector
     * at some later point.
     */
    std::vector<Paintown::Object*> addedObjects;
    // Hold information for players
    std::map<void *, PlayerData> playerInfo;

    // This is for when characters are inside the tension area
    int inleft;
    int inright;
    int onLeftSide;
    int onRightSide;
    int inabove;

    bool loaded;

    // Controllers
    //std::vector<MugenBackgroundController *> controllers;

    // This is for controllers as sometimes backgrounds share IDs for this purpose
    //void getBackgrounds( std::vector<MugenBackground *> &bgs, int ID );

    // What garbage
    std::deque<Bitmap*> garbage;

    void initializeName();

    Mugen::SpriteMap effects;
    std::map<int, MugenAnimation*> sparks;
    std::vector<Mugen::Effect*> showSparks;

    // Character huds
    Mugen::GameInfo *gameHUD;

    bool gameOver;

    /* frequency of logic updates. 1 is normal, lower is slower */
    double gameRate;
    int cycles;

    struct SuperPause{
        SuperPause():time(0){
        }

        int time;
        int positionX, positionY;
        int soundGroup, soundItem;
    } superPause;
};

#endif
