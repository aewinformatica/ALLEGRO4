#include "util/bitmap.h"
#include "util/trans-bitmap.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <cstring>
#include <vector>
#include <ostream>
#include <sstream>
#include <iostream>

// To aid in filesize
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util/funcs.h"
#include "util/font.h"
#include "util/file-system.h"
#include "factory/font_render.h"

#include "animation.h"
#include "item.h"
#include "item-content.h"
#include "section.h"
#include "character.h"
#include "sound.h"
#include "reader.h"
#include "sprite.h"
#include "util.h"
#include "stage.h"
#include "globals.h"
#include "state.h"
#include "evaluator.h"
#include "compiler.h"
#include "command.h"
#include "behavior.h"
#include "state-controller.h"

#include "input/input-map.h"
#include "input/input-manager.h"

#include "parse-cache.h"
#include "parser/all.h"
#include "ast/all.h"

using namespace std;

static const int REGENERATE_TIME = 40;

namespace Mugen{

namespace StateType{

std::string Stand = "S";
std::string Crouch = "C";
std::string Air = "A";
std::string LyingDown = "L";

}

namespace Move{

std::string Attack = "A";
std::string Idle = "I";
std::string Hit = "H";

}

namespace AttackType{
    std::string Normal = "N";
    std::string Special = "S";
    std::string Hyper = "H";
}

namespace PhysicalAttack{
    std::string Normal = "A";
    std::string Throw = "T";
    std::string Projectile = "P";
}

namespace PaintownUtil = ::Util;

static string sourceLocation(const Ast::AttributeSimple & simple, const Filesystem::AbsolutePath & path){
    ostringstream out;
    out << "in " << path.path() << " at line " << simple.getLine() << " column " << simple.getColumn();
    return out.str();
}

State::State(int id):
id(id),
type(Unchanged),
animation(NULL),
changeControl(false),
control(NULL),
changeVelocity(false),
velocity_x(NULL),
velocity_y(NULL),
changePhysics(false),
changePower(false),
powerAdd(NULL),
moveType(Move::Idle),
juggle(0),
hitDefPersist(false){
}
    
State * State::deepCopy() const {
    State * state = new State(id);
    state->type = this->type;
    state->animation = Compiler::copy(this->animation);
    state->changeControl = this->changeControl;
    state->control = Compiler::copy(this->control);
    state->changeVelocity = this->changeVelocity;
    state->velocity_x = Compiler::copy(this->velocity_x);
    state->velocity_y = Compiler::copy(this->velocity_y);
    state->changePhysics = this->changePhysics;
    state->changePower = this->changePower;
    state->powerAdd = Compiler::copy(this->powerAdd);
    state->moveType = this->moveType;
    state->juggle = Compiler::copy(this->juggle);
    state->hitDefPersist = this->hitDefPersist;
    for (vector<StateController*>::const_iterator it = controllers.begin(); it != controllers.end(); it++){
        StateController * controller = *it;
        state->addController(controller->deepCopy());
    }
    return state;
}

void State::addController(StateController * controller){
#if 0
    for (vector<StateController*>::iterator it = controllers.begin(); it != controllers.end(); /**/ ){
        StateController * mine = *it;

        /* controllers must have unique names */
        if (mine->getName() == controller->getName()){
            delete mine;
            it = controllers.erase(it);
        } else {
            it++;
        }
    }
#endif

    controllers.push_back(controller);
}

void State::addControllerFront(StateController * controller){
#if 0
    for (vector<StateController*>::iterator it = controllers.begin(); it != controllers.end(); /**/ ){
        StateController * mine = *it;
        /* controllers must have unique names */
        if (mine->getName() == controller->getName()){
            delete mine;
            it = controllers.erase(it);
        } else {
            it++;
        }
    }
#endif

    controllers.insert(controllers.begin(), controller);
}

void State::setJuggle(Compiler::Value * juggle){
    this->juggle = juggle;
}

void State::setVelocity(Compiler::Value * x, Compiler::Value * y){
    changeVelocity = true;
    velocity_x = x;
    velocity_y = y;
}

void State::setPower(Compiler::Value * power){
    powerAdd = power;
    changePower = true;
}
    
void State::setMoveType(const std::string & type){
    moveType = type;
}
    
void State::setPhysics(Physics::Type p){
    changePhysics = true;
    physics = p;
}

void State::transitionTo(const MugenStage & stage, Character & who){
    if (animation != NULL){
        who.setAnimation((int) animation->evaluate(FullEnvironment(stage, who)).toNumber());
    }

    if (changeControl){
        who.setControl(control->evaluate(FullEnvironment(stage, who)).toBool());
    }

    if (juggle != NULL){
        who.setCurrentJuggle((int) juggle->evaluate(FullEnvironment(stage, who)).toNumber());
    }

    who.setMoveType(moveType);

    /* I don't think a hit definition should persist across state changes, unless
     * hitdefpersist is set to 1. Anyway this is needed because a state could try
     * to repeatedly set the same hitdef, like:
     *   state 200
     *   type = hitdef
     *   trigger1 = animelem = 3
     * But since the animelem won't change every game tick, this trigger will be
     * activated a few times (like 3-4).
     * Resetting the hitdef to NULL allows the hitdef controller to ensure only
     * unique hitdef's get set as well as repeat the same hitdef in sequence.
     *
     * Update: 7/28/2010: I don't think this is true anymore because `animelem'
     * is only true for the first tick of an animation. `animelem = 3' won't be true
     * for as long as the animation is 3, just the first tick of the game that its 3.
     */
    if (!doesHitDefPersist()){
        who.disableHit();
        /*
        who.setHitDef(NULL);
        */
    }

    if (changeVelocity){
        who.setXVelocity(velocity_x->evaluate(FullEnvironment(stage, who)).toNumber());
        who.setYVelocity(velocity_y->evaluate(FullEnvironment(stage, who)).toNumber());
        // Global::debug(0) << "Velocity set to " << velocity_x->evaluate(FullEnvironment(stage, who)).toNumber() << ", " << velocity_y->evaluate(FullEnvironment(stage, who)).toNumber() << endl;
    }

    if (changePhysics){
        who.setCurrentPhysics(physics);
    }

    switch (type){
        case Standing : {
            who.setStateType(StateType::Stand);
            break;
        }
        case Crouching : {
            who.setStateType(StateType::Crouch);
            break;
        }
        case Air : {
            who.setStateType(StateType::Air);
            break;
        }
        case LyingDown : {
            who.setStateType(StateType::LyingDown);
            break;
        }
        case Unchanged : {
            break;
        }
    }
    
    for (vector<StateController*>::iterator it = controllers.begin(); it != controllers.end(); it++){
        StateController * controller = *it;
        controller->resetPersistent();
    }
}

State::~State(){
    for (vector<StateController*>::iterator it = controllers.begin(); it != controllers.end(); it++){
        delete (*it);
    }

    delete powerAdd;
    delete animation;
    delete control;
    delete juggle;
    delete velocity_x;
    delete velocity_y;
}

/* Called when the player was hit */
void HitState::update(MugenStage & stage, const Character & guy, bool inAir, const HitDefinition & hit){
    /* FIXME: choose the proper ground/air/guard types */

    guarded = false;
    shakeTime = hit.pause.player2;
    yAcceleration = hit.yAcceleration;
    airType = hit.airType;
    groundType = hit.groundType;

    /* FIXME: set damage */
    
    /* if in the air */
    if (inAir){
        hitType = hit.airType;
        if (fall.fall){
            if (hit.animationTypeFall == AttackType::NoAnimation){
                if (hit.animationTypeAir == AttackType::Up){
                    animationType = AttackType::Up;
                } else {
                    animationType = AttackType::Back;
                }
            } else {
                animationType = hit.animationTypeFall;
            }

            hitTime = 0;
        } else {
            if (hit.animationTypeAir != AttackType::NoAnimation){
                animationType = hit.animationTypeAir;
            } else {
                animationType = hit.animationType;
            }

            hitTime = hit.airHitTime;
        }
        
        xVelocity = hit.airVelocity.x;
        yVelocity = hit.airVelocity.y;

        fall.fall |= hit.fall.fall;
        fall.yVelocity = hit.fall.yVelocity;
        int groundSlideTime = 0;
        groundSlideTime = (int) hit.groundSlideTime;
        returnControlTime = hit.airGuardControlTime;
    } else {
        hitType = hit.groundType;
        int groundSlideTime = 0;
        groundSlideTime = (int) hit.groundSlideTime;
        animationType = hit.animationType;
        returnControlTime = hit.guardControlTime;
        hitTime = hit.groundHitTime;
        slideTime = groundSlideTime;
        xVelocity = hit.groundVelocity.x;
        yVelocity = hit.groundVelocity.y;
        fall.fall = hit.fall.fall;
        fall.yVelocity = hit.fall.yVelocity;
    }

    fall.envShake.time = hit.fall.envShake.time;

    // Global::debug(0) << "Hit definition: shake time " << shakeTime << " hit time " << hitTime << endl;
}

Character::Character(const Filesystem::AbsolutePath & s ):
Object(0),
commonSounds(NULL){
    this->location = s;
    initialize();
}

/*
Character::Character( const char * location ):
ObjectAttack(0),
commonSounds(NULL),
hit(NULL){
    this->location = std::string(location);
    initialize();
}
*/

Object::Object(int alliance):
Paintown::ObjectAttack(alliance){
}

Object::Object(const int x, const int y, int alliance):
Paintown::ObjectAttack(x, y, alliance){
}


Object::Object(const Object & copy):
ObjectAttack(copy){
}
    
Object::~Object(){
}

Character::Character( const Filesystem::AbsolutePath & s, int alliance ):
Object(alliance),
commonSounds(NULL){
    this->location = s;
    initialize();
}

Character::Character( const Filesystem::AbsolutePath & s, const int x, const int y, int alliance ):
Object(x,y,alliance),
commonSounds(NULL){
    this->location = s;
    initialize();
}

Character::Character( const Character & copy ):
Object(copy),
xscale(copy.xscale),
yscale(copy.yscale),
commonSounds(NULL),
currentState(copy.currentState),
previousState(copy.previousState),
currentAnimation(copy.currentAnimation),
debug(false),
needToGuard(false)
{
}

Character::~Character(){
     // Get rid of sprites
    for (std::map< unsigned int, std::map< unsigned int, MugenSprite * > >::iterator i = sprites.begin() ; i != sprites.end() ; ++i ){
      for( std::map< unsigned int, MugenSprite * >::iterator j = i->second.begin() ; j != i->second.end() ; ++j ){
	  if( j->second )delete j->second;
      }
    }
    
     // Get rid of bitmaps
    for( std::map< unsigned int, std::map< unsigned int, Bitmap * > >::iterator i = bitmaps.begin() ; i != bitmaps.end() ; ++i ){
      for( std::map< unsigned int, Bitmap * >::iterator j = i->second.begin() ; j != i->second.end() ; ++j ){
	  if( j->second )delete j->second;
      }
    }
    
    // Get rid of animation lists;
    for( std::map< int, MugenAnimation * >::iterator i = animations.begin() ; i != animations.end() ; ++i ){
	if( i->second )delete i->second;
    }
    
    // Get rid of sounds
    for( std::map< unsigned int, std::map< unsigned int, MugenSound * > >::iterator i = sounds.begin() ; i != sounds.end() ; ++i ){
      for( std::map< unsigned int, MugenSound * >::iterator j = i->second.begin() ; j != i->second.end() ; ++j ){
	  if( j->second )delete j->second;
      }
    }

    for (vector<Command*>::iterator it = commands.begin(); it != commands.end(); it++){
        delete (*it);
    }
        
    for (map<int, State*>::iterator it = states.begin(); it != states.end(); it++){
        State * state = (*it).second;
        delete state;
    }

    // delete internalJumpNumber;
}

void Character::initialize(){
    currentState = Standing;
    currentPhysics = Physics::Stand;
    moveType = Move::Idle;
    frozen = false;
    reversalActive = false;
    previousState = currentState;
    stateType = StateType::Stand;
    currentAnimation = Standing;
    juggleRemaining = 0;
    defenseMultiplier = 1;
    attackMultiplier = 1;
    lieDownTime = 0;
    debug = false;
    has_control = true;
    airjumpnum = 0;
    airjumpheight = 35;
    blocking = false;
    guarding = false;
    behavior = NULL;

    sparkno = 0;
    guardsparkno = 0;

    needToGuard = false;

    matchWins = 0;

    combo = 1;
    // nextCombo = 0;

    lastTicket = 0;

    internalJumpNumber = NULL;
    
    /* Load up info for the select screen */
    //loadSelectData();

    /* provide sensible defaults */
    walkfwd = 0;
    walkback = 0;
    runbackx = 0;
    runbacky = 0;
    runforwardx = 0;
    runforwardy = 0;
    power = 0;

    velocity_x = 0;
    velocity_y = 0;

    gravity = 0.1;
    standFriction = 1;

    stateTime = 0;

    /* Regeneration */
    regenerateHealth = false;
    regenerating = false;
    regenerateTime = REGENERATE_TIME;
    regenerateHealthDifference = 0;
}

void Character::loadSelectData(){
    /* Load up info for the select screen */
    try{
        Filesystem::AbsolutePath baseDir = location.getDirectory();
	Global::debug(1) << baseDir.path() << endl;
        Filesystem::RelativePath str = Filesystem::RelativePath(location.getFilename().path());
	const Filesystem::AbsolutePath ourDefFile = Util::fixFileName(baseDir, str.path() + ".def");
	
	if (ourDefFile.isEmpty()){
	    Global::debug(1) << "Cannot locate player definition file for: " << location.path() << endl;
	}
	
        Ast::AstParse parsed(Util::parseDef(ourDefFile.path()));
	// Set name of character
	this->name = Mugen::Util::probeDef(parsed, "info", "name");
	this->displayName = Mugen::Util::probeDef(parsed, "info", "displayname");
	this->sffFile = Mugen::Util::probeDef(parsed, "files", "sprite");
	// Get necessary sprites 9000 & 9001 for select screen
        /* FIXME: replace 9000 with some readable constant */
	this->sprites[9000][0] = Mugen::Util::probeSff(Util::fixFileName(baseDir, this->sffFile), 9000, 0, true);
	this->sprites[9000][1] = Mugen::Util::probeSff(Util::fixFileName(baseDir, this->sffFile), 9000, 1, true);
	
    } catch (const MugenException &ex){
	Global::debug(1) << "Couldn't grab details for character!" << endl;
	Global::debug(1) << "Error was: " << ex.getReason() << endl;
    }
}
    
void Character::addCommand(Command * command){
    commands.push_back(command);
}

void Character::setAnimation(int animation){
    currentAnimation = animation;
    if (getCurrentAnimation() != NULL){
        getCurrentAnimation()->reset();
    }
}

void Character::loadCmdFile(const Filesystem::RelativePath & path){
    Filesystem::AbsolutePath full = baseDir.join(path);
    try{
        int defaultTime = 15;
        int defaultBufferTime = 1;

        Ast::AstParse parsed((list<Ast::Section*>*) Util::parseCmd(full.path()));
        State * currentState = NULL;
        for (Ast::AstParse::section_iterator section_it = parsed.getSections()->begin(); section_it != parsed.getSections()->end(); section_it++){
            Ast::Section * section = *section_it;
            std::string head = section->getName();
            head = Util::fixCase(head);

            if (head == "command"){
                class CommandWalker: public Ast::Walker {
                public:
                    CommandWalker(Character & self, const int defaultTime, const int defaultBufferTime, const Filesystem::AbsolutePath & full):
                        self(self),
                        time(defaultTime),
                        bufferTime(defaultBufferTime),
                        key(NULL),
                        path(full){
                        }

                    Character & self;
                    int time;
                    int bufferTime;
                    string name;
                    Ast::Key * key;
                    const Filesystem::AbsolutePath & path;

                    virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                        if (simple == "name"){
                            simple >> name;
                        } else if (simple == "command"){
                            key = (Ast::Key*) simple.getValue()->copy();
                        } else if (simple == "time"){
                            simple >> time;
                        } else if (simple == "buffer.time"){
                            simple >> bufferTime;
                            /* Time that the command will be buffered for. If the command is done
                             * successfully, then it will be valid for this time. The simplest
                             * case is to set this to 1. That means that the command is valid
                             * only in the same tick it is performed. With a higher value, such
                             * as 3 or 4, you can get a "looser" feel to the command. The result
                             * is that combos can become easier to do because you can perform
                             * the command early.
                             */
                        }
                    }

                    virtual void onSectionFinished(const Ast::Section & section){
                        if (name == ""){
                            throw MugenException("No name given for command");
                        }

                        if (key == NULL){
                            throw MugenException("No key sequence given for command");
                        }

                        /* parser guarantees the key will be a KeyList */
                        self.addCommand(new Command(name, (Ast::KeyList*) key, time, bufferTime));
                    }
                };

                CommandWalker walker(*this, defaultTime, defaultBufferTime, full);
                section->walk(walker);
            } else if (head == "defaults"){
                class DefaultWalker: public Ast::Walker {
                public:
                    DefaultWalker(int & time, int & buffer):
                        time(time),
                        buffer(buffer){
                        }

                    int & time;
                    int & buffer;

                    virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                        if (simple == "command.time"){
                            simple >> time;
                        } else if (simple == "command.buffer.time"){
                            simple >> buffer;
                        }
                    }
                };

                DefaultWalker walker(defaultTime, defaultBufferTime);
                section->walk(walker);
            } else if (PaintownUtil::matchRegex(head, "statedef")){
                currentState = parseStateDefinition(section, full);
            } else if (PaintownUtil::matchRegex(head, "state ")){
                if (currentState != NULL){
                    currentState->addController(parseState(section));
                } else {
                    /* not all states in the .cmd file will have a statedef */
                    StateController * controller = parseState(section);
                    if (getState(controller->getState()) != NULL){
                        getState(controller->getState())->addController(controller);
                    } else {
                        delete controller;
                    }
                }
            }

            /* [Defaults]
             * ; Default value for the "time" parameter of a Command. Minimum 1.
             * command.time = 15
             *
             * ; Default value for the "buffer.time" parameter of a Command. Minimum 1,
             * ; maximum 30.
             * command.buffer.time = 1
             */
        }
    } catch (const Mugen::Cmd::ParseException & e){
        /*
        Global::debug(0) << "Could not parse " << path << endl;
        Global::debug(0) << e.getReason() << endl;
        */
        ostringstream out;
        out << "Could not parse " << path.path() << ": " << e.getReason();
        throw MugenException(out.str());
    }
}

static bool isStateDefSection(string name){
    name = Util::fixCase(name);
    return PaintownUtil::matchRegex(name, "state ") ||
           PaintownUtil::matchRegex(name, "statedef ");
}

bool Character::canBeHit(Object * enemy){
    /* FIXME: handle the hit flags here */
    return (moveType != Move::Hit && lastTicket < enemy->getTicket()) ||
           (moveType == Move::Hit && lastTicket < enemy->getTicket() &&
            juggleRemaining >= enemy->getCurrentJuggle());
}
    
void Character::setConstant(std::string name, const vector<double> & values){
    constants[name] = Constant(values);
}

void Character::setConstant(std::string name, double value){
    constants[name] = Constant(value);
}

void Character::setFloatVariable(int index, const RuntimeValue & value){
    floatVariables[index] = value;
}

void Character::setVariable(int index, const RuntimeValue & value){
    variables[index] = value;
}

static RuntimeValue extractVariable(const map<int, RuntimeValue> & stuff, int index){
    map<int, RuntimeValue>::const_iterator found = stuff.find(index);
    if (found != stuff.end()){
        return (*found).second;
    }
    return RuntimeValue(0);
}

RuntimeValue Character::getVariable(int index) const {
    return extractVariable(variables, index);
    /*
    map<int, RuntimeValue>::const_iterator found = variables.find(index);
    if (found != variables.end()){
        return (*found).second;
    }
    return RuntimeValue(0);
    */
}

RuntimeValue Character::getFloatVariable(int index) const {
    return extractVariable(floatVariables, index);

    /*
    map<int, RuntimeValue>::const_iterator found = floatVariables.find(index);
    if (found != floatVariables.end()){
        return (*found).second;
    }
    return RuntimeValue(0);
    */
}
        
void Character::setSystemVariable(int index, const RuntimeValue & value){
    systemVariables[index] = value;
}

RuntimeValue Character::getSystemVariable(int index) const {
    return extractVariable(systemVariables, index);

    /*
    map<int, RuntimeValue>::const_iterator found = systemVariables.find(index);
    if (found != systemVariables.end()){
        return (*found).second;
    }
    return RuntimeValue(0);
    */
}
        
void Character::resetStateTime(){
    stateTime = 0;
}
        
void Character::changeState(MugenStage & stage, int stateNumber, const vector<string> & inputs){
    /* dont let after images carry over to the next state
     * UPDATE: mugen actually allows this
     */
    // afterImage.show = false;

    /* reset juggle points once the player gets up */
    if (stateNumber == GetUpFromLiedown){
        juggleRemaining = getJugglePoints();
    }

    /* reset hit count */
    hitCount = 0;

    Global::debug(1) << "Change to state " << stateNumber << endl;
    previousState = currentState;
    currentState = stateNumber;
    resetStateTime();
    if (getState(currentState) != NULL){
        State * state = getState(currentState);
        state->transitionTo(stage, *this);
        doStates(stage, inputs, currentState);
    } else {
        Global::debug(0) << "Unknown state " << currentState << endl;
    }
}

void Character::changeOwnState(MugenStage & stage, int state, const std::vector<std::string> & inputs){
    /* FIXME: change to states in the characters own cns file */
    changeState(stage, state, inputs);
}

void Character::loadCnsFile(const Filesystem::RelativePath & path){
    Filesystem::AbsolutePath full = baseDir.join(path);
    try{
        /* cns can use the Cmd parser */
        Ast::AstParse parsed((list<Ast::Section*>*) Util::parseCmd(full.path()));
        for (Ast::AstParse::section_iterator section_it = parsed.getSections()->begin(); section_it != parsed.getSections()->end(); section_it++){
            Ast::Section * section = *section_it;
            std::string head = section->getName();
            head = Util::fixCase(head);
            if (false && !isStateDefSection(head)){
                /* I dont think this is the right way to do it */
                class AttributeWalker: public Ast::Walker {
                public:
                    AttributeWalker(Character & who):
                    self(who){
                    }

                    Character & self;

                    virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                        string name = simple.idString();
                        if (simple.getValue() != 0 && simple.getValue()->hasMultiple()){
                            vector<double> values;
                            simple >> values;
                            self.setConstant(name, values);
                        } else {
                            double value;
                            simple >> value;
                            self.setConstant(name, value);
                        }
                    }
                };

                AttributeWalker walker(*this);
                section->walk(walker);
            } else if (head == "velocity"){
                class VelocityWalker: public Ast::Walker {
                public:
                    VelocityWalker(Character & who):
                    self(who){
                    }

                    Character & self;

                    virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                        if (simple == "walk.fwd"){
                            double speed;
                            simple >> speed;
                            self.setWalkForwardX(speed);
                        } else if (simple == "walk.back"){
                            double speed;
                            simple >> speed;
                            self.setWalkBackX(speed);
                        } else if (simple == "run.fwd"){
                            double x, y;
                            simple >> x >> y;
                            self.setRunForwardX(x);
                            self.setRunForwardY(y);
                        } else if (simple == "run.back"){
                            double x, y;
                            simple >> x >> y;
                            self.setRunBackX(x);
                            self.setRunBackY(y);
                        } else if (simple == "jump.neu"){
                            double x, y;
                            simple >> x >> y;
                            self.setNeutralJumpingX(x);
                            self.setNeutralJumpingY(y);
                        } else if (simple == "jump.back"){
                            double speed;
                            simple >> speed;
                            self.setJumpBack(speed);
                        } else if (simple == "jump.fwd"){
                            double speed;
                            simple >> speed;
                            self.setJumpForward(speed);
                        } else if (simple == "runjump.back"){
                            double speed;
                            simple >> speed;
                            self.setRunJumpBack(speed);
                        } else if (simple == "runjump.fwd"){
                            double speed;
                            simple >> speed;
                            self.setRunJumpForward(speed);
                        } else if (simple == "airjump.neu"){
                            double x, y;
                            simple >> x >> y;
                            self.setAirJumpNeutralX(x);
                            self.setAirJumpNeutralY(y);
                        } else if (simple == "airjump.back"){
                            double speed;
                            simple >> speed;
                            self.setAirJumpBack(speed);
                        } else if (simple == "airjump.fwd"){
                            double speed;
                            simple >> speed;
                            self.setAirJumpForward(speed);
                        }
                    }
                };

                VelocityWalker walker(*this);
                section->walk(walker);
            } else if (head == "data"){
                class DataWalker: public Ast::Walker {
                public:
                    DataWalker(Character & who):
                    self(who){
                    }
                    
                    Character & self;

                    virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                        if (simple == "liedown.time"){
                            int x;
                            simple >> x;
                            self.setLieDownTime(x);
                        } else if (simple == "airjuggle"){
                            int x;
                            simple >> x;
                            self.setJugglePoints(x);
                        } else if (simple == "life"){
                            int x;
                            simple >> x;
                            self.setMaxHealth(x);
                            self.setHealth(x);
                        } else if (simple == "sparkno"){
                            string spark;
                            simple >> spark;
                            spark = PaintownUtil::lowerCaseAll(spark);
                            if (PaintownUtil::matchRegex(spark, "s[0-9]+")){
                                /* FIXME: handle S */
                            } else {
                                self.setDefaultSpark(atoi(spark.c_str()));
                            }
                        } else if (simple == "guard.sparkno"){
                            string spark;
                            simple >> spark;
                            if (PaintownUtil::matchRegex(spark, "s[0-9]+")){
                                /* FIXME: handle S */
                            } else {
                                self.setDefaultGuardSpark(atoi(spark.c_str()));
                            }
                        }
                    }

                };
                
                DataWalker walker(*this);
                section->walk(walker);
            } else if (head == "size"){
                class SizeWalker: public Ast::Walker {
                public:
                    SizeWalker(Character & self):
                        self(self){
                        }

                    Character & self;

                    virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                        if (simple == "height"){
                            int x;
                            simple >> x;
                            self.setHeight(x);
                        } else if (simple == "xscale"){
			    simple >> self.xscale;
			} else if (simple == "yscale"){
			    simple >> self.yscale;
			} else if (simple == "ground.back"){
			    simple >> self.groundback;
			} else if (simple == "ground.front"){
			    simple >> self.groundfront;
			} else if (simple == "air.back"){
			    simple >> self.airback;
			} else if (simple == "air.front"){
			    simple >> self.airfront;
			} else if (simple == "attack.dist"){
			    simple >> self.attackdist;
			} else if (simple == "proj.attack.dist"){
			    simple >> self.projattackdist;
			} else if (simple == "proj.doscale"){
			    simple >> self.projdoscale;
			} else if (simple == "head.pos"){
			    int x=0,y=0;
			    try{
				simple >> self.headPosition.x >> self.headPosition.y;
			    } catch (const Ast::Exception & e){
			    }
			} else if (simple == "mid.pos"){
			    int x=0,y=0;
			    try{
				simple >> self.midPosition.x >> self.midPosition.y;
			    } catch (const Ast::Exception & e){
			    }
			} else if (simple == "shadowoffset"){
			    simple >> self.shadowoffset;
			} else if (simple == "draw.offset"){
			    int x=0,y=0;
			    try{
				simple >> self.drawOffset.x >> self.drawOffset.y;
			    } catch (const Ast::Exception & e){
			    }
			}
                    }
                };
                
                SizeWalker walker(*this);
                section->walk(walker);

            } else if (head == "movement"){
                class MovementWalker: public Ast::Walker {
                public:
                    MovementWalker(Character & who):
                    self(who){
                    }

                    Character & self;

                    virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                        if (simple == "yaccel"){
                            double n;
                            simple >> n;
                            self.setGravity(n);
                        } else if (simple == "stand.friction"){
                            double n;
                            simple >> n;
                            self.setStandingFriction(n);
                        } else if (simple == "airjump.num"){
                            int x;
                            simple >> x;
                            self.setExtraJumps(x);
                        } else if (simple == "airjump.height"){
                            double x;
                            simple >> x;
                            self.setAirJumpHeight(x);
                        }
                    }
                };

                MovementWalker walker(*this);
                section->walk(walker);
            }
        }
    } catch (const Mugen::Cmd::ParseException & e){
        ostringstream out;
        out << "Could not parse " << path.path() << ": " << e.getReason();
        throw MugenException(out.str());
    } catch (const Ast::Exception & e){
        ostringstream out;
        out << "Could not parse " << path.path() << ": " << e.getReason();
        throw MugenException(out.str());
    }
}

State * Character::parseStateDefinition(Ast::Section * section, const Filesystem::AbsolutePath & path){
    std::string head = section->getName();
    /* this should really be head = Mugen::Util::fixCase(head) */
    head = Util::fixCase(head);

    int state = atoi(PaintownUtil::captureRegex(head, "statedef *(-?[0-9]+)", 0).c_str());
    class StateWalker: public Ast::Walker {
        public:
            StateWalker(State * definition, const Filesystem::AbsolutePath & path):
            definition(definition),
            path(path){
            }

            State * definition;
            const Filesystem::AbsolutePath & path;

            virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                if (simple == "type"){
                    string type;
                    simple >> type;
                    type = PaintownUtil::lowerCaseAll(type);
                    if (type == "s"){
                        definition->setType(State::Standing);
                    } else if (type == "c"){
                        definition->setType(State::Crouching);
                    } else if (type == "a"){
                        definition->setType(State::Air);
                    } else if (type == "l"){
                        definition->setType(State::LyingDown);
                    } else if (type == "u"){
                        definition->setType(State::Unchanged);
                    } else {
                        ostringstream out;
                        out << "Unknown statedef type: '" << type << "'";
                        throw MugenException(out.str());
                    }
                } else if (simple == "movetype"){
                    string type;
                    simple >> type;
                    definition->setMoveType(type);
                } else if (simple == "physics"){
                    string type;
                    simple >> type;
                    if (type == "S"){
                        definition->setPhysics(Physics::Stand);
                    } else if (type == "N"){
                        definition->setPhysics(Physics::None);
                    } else if (type == "C"){
                        definition->setPhysics(Physics::Crouch);
                    } else if (type == "A"){
                        definition->setPhysics(Physics::Air);
                    }
                    /* if physics is U (unchanged) then dont set the state physics
                     * and then the character's physics won't change during
                     * a state transition.
                     */
                } else if (simple == "anim"){
                    definition->setAnimation(Compiler::compile(simple.getValue()));
                } else if (simple == "velset"){
                    const Ast::Value * x;
                    const Ast::Value * y;
                    simple >> x >> y;
                    definition->setVelocity(Compiler::compile(x), Compiler::compile(y));
                } else if (simple == "ctrl"){
                    definition->setControl(Compiler::compile(simple.getValue()));
                } else if (simple == "poweradd"){
                    definition->setPower(Compiler::compile(simple.getValue()));
                } else if (simple == "juggle"){
                    definition->setJuggle(Compiler::compile(simple.getValue()));
                } else if (simple == "facep2"){
                } else if (simple == "hitdefpersist"){
                    bool what;
                    simple >> what;
                    definition->setHitDefPersist(what);
                } else if (simple == "movehitpersist"){
                } else if (simple == "hitcountpersist"){
                } else if (simple == "sprpriority"){
                } else {
                    Global::debug(0) << "Unhandled statedef attribute: " << simple.toString() << " " << sourceLocation(simple, path) << endl;
                }
            }
    };

    State * definition = new State(state);
    StateWalker walker(definition, path);
    section->walk(walker);
    if (states[state] != NULL){
        Global::debug(1) << "Overriding state " << state << endl;
        delete states[state];
    }
    Global::debug(2) << "Adding state definition " << state << endl;
    states[state] = definition;

    return definition;
}

StateController * Character::parseState(Ast::Section * section){
    std::string head = section->getName();
    head = Util::fixCase(head);

    string name = PaintownUtil::captureRegex(head, "state *-?[0-9]+ *, *(.*)", 0);
    int state = atoi(PaintownUtil::captureRegex(head, "state *(-?[0-9]+)", 0).c_str());

    class StateControllerWalker: public Ast::Walker {
    public:
        StateControllerWalker():
        type(StateController::Unknown){
            types["afterimage"] = StateController::AfterImage;
            types["afterimagetime"] = StateController::AfterImageTime;
            types["allpalfx"] = StateController::AllPalFX;
            types["angleadd"] = StateController::AngleAdd;
            types["angledraw"] = StateController::AngleDraw;
            types["anglemul"] = StateController::AngleMul;
            types["angleset"] = StateController::AngleSet;
            types["appendtoclipboard"] = StateController::AppendToClipboard;
            types["assertspecial"] = StateController::AssertSpecial;
            types["attackdist"] = StateController::AttackDist;
            types["attackmulset"] = StateController::AttackMulSet;
            types["bgpalfx"] = StateController::BGPalFX;
            types["bindtoparent"] = StateController::BindToParent;
            types["bindtoroot"] = StateController::BindToRoot;
            types["bindtotarget"] = StateController::BindToTarget;
            types["changeanim"] = StateController::ChangeAnim;
            types["changeanim2"] = StateController::ChangeAnim2;
            types["changestate"] = StateController::ChangeState;
            types["clearclipboard"] = StateController::ClearClipboard;
            types["ctrlset"] = StateController::CtrlSet;
            types["defencemulset"] = StateController::DefenceMulSet;
            types["destroyself"] = StateController::DestroySelf;
            types["displaytoclipboard"] = StateController::DisplayToClipboard;
            types["envcolor"] = StateController::EnvColor;
            types["envshake"] = StateController::EnvShake;
            types["explod"] = StateController::Explod;
            types["explodbindtime"] = StateController::ExplodBindTime;
            types["forcefeedback"] = StateController::ForceFeedback;
            types["fallenvshake"] = StateController::FallEnvShake;
            types["gamemakeanim"] = StateController::GameMakeAnim;
            types["gravity"] = StateController::Gravity;
            types["helper"] = StateController::Helper;
            types["hitadd"] = StateController::HitAdd;
            types["hitby"] = StateController::HitBy;
            types["hitdef"] = StateController::HitDef;
            types["hitfalldamage"] = StateController::HitFallDamage;
            types["hitfallset"] = StateController::HitFallSet;
            types["hitfallvel"] = StateController::HitFallVel;
            types["hitoverride"] = StateController::HitOverride;
            types["hitvelset"] = StateController::HitVelSet;
            types["lifeadd"] = StateController::LifeAdd;
            types["lifeset"] = StateController::LifeSet;
            types["makedust"] = StateController::MakeDust;
            types["modifyexplod"] = StateController::ModifyExplod;
            types["movehitreset"] = StateController::MoveHitReset;
            types["nothitby"] = StateController::NotHitBy;
            types["null"] = StateController::Null;
            types["offset"] = StateController::Offset;
            types["palfx"] = StateController::PalFX;
            types["parentvaradd"] = StateController::ParentVarAdd;
            types["parentvarset"] = StateController::ParentVarSet;
            types["pause"] = StateController::Pause;
            types["playerpush"] = StateController::PlayerPush;
            types["playsnd"] = StateController::PlaySnd;
            types["posadd"] = StateController::PosAdd;
            types["posfreeze"] = StateController::PosFreeze;
            types["posset"] = StateController::PosSet;
            types["poweradd"] = StateController::PowerAdd;
            types["powerset"] = StateController::PowerSet;
            types["projectile"] = StateController::Projectile;
            types["removeexplod"] = StateController::RemoveExplod;
            types["reversaldef"] = StateController::ReversalDef;
            types["screenbound"] = StateController::ScreenBound;
            types["selfstate"] = StateController::SelfState;
            types["sprpriority"] = StateController::SprPriority;
            types["statetypeset"] = StateController::StateTypeSet;
            types["sndpan"] = StateController::SndPan;
            types["stopsnd"] = StateController::StopSnd;
            types["superpause"] = StateController::SuperPause;
            types["targetbind"] = StateController::TargetBind;
            types["targetdrop"] = StateController::TargetDrop;
            types["targetfacing"] = StateController::TargetFacing;
            types["targetlifeadd"] = StateController::TargetLifeAdd;
            types["targetpoweradd"] = StateController::TargetPowerAdd;
            types["targetstate"] = StateController::TargetState;
            types["targetveladd"] = StateController::TargetVelAdd;
            types["targetvelset"] = StateController::TargetVelSet;
            types["trans"] = StateController::Trans;
            types["turn"] = StateController::Turn;
            types["varadd"] = StateController::VarAdd;
            types["varrandom"] = StateController::VarRandom;
            types["varrangeset"] = StateController::VarRangeSet;
            types["varset"] = StateController::VarSet;
            types["veladd"] = StateController::VelAdd;
            types["velmul"] = StateController::VelMul;
            types["velset"] = StateController::VelSet;
            types["width"] = StateController::Width;

        }

        StateController::Type type;
        map<string, StateController::Type> types;

        virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
            if (simple == "type"){
                string type;
                simple >> type;
                type = Mugen::Util::fixCase(type);
                
                if (types.find(type) != types.end()){
                    map<string, StateController::Type>::iterator what = types.find(type);
                    this->type = (*what).second;
                } else {
                    Global::debug(0) << "Unknown state controller type " << type << endl;
                }
            }
        }
    };

    // StateController * controller = new StateController(name);
    StateControllerWalker walker;
    section->walk(walker);
    StateController::Type type = walker.type;
    if (type == StateController::Unknown){
        Global::debug(0) << "Warning: no type given for controller " << section->getName() << endl;
        return NULL;
    } else {
        StateController * controller = StateController::compile(section, name, state, type);
        return controller;
    }
}

static Filesystem::AbsolutePath findStateFile(const Filesystem::AbsolutePath & base, const string & path){
    if (PaintownUtil::exists(base.join(Filesystem::RelativePath(path)).path())){
        return base.join(Filesystem::RelativePath(path));
    } else {
        return Filesystem::find(Filesystem::RelativePath("mugen/data/" + path));
    }

#if 0
    try{
        /* first look in the character's directory */
        // return Filesystem::find(base.join(Filesystem::RelativePath(path)));
        
    } catch (const Filesystem::NotFound & f){
        /* then look for it in the common data directory.
         * this is where common1.cns lives
         */
        return Filesystem::find(Filesystem::RelativePath("mugen/data/" + path));
    }
#endif
}
        
void Character::loadStateFile(const Filesystem::AbsolutePath & base, const string & path){
    Filesystem::AbsolutePath full = findStateFile(base, path);
    // string full = Filesystem::find(base + "/" + PaintownUtil::trim(path));
    /* st can use the Cmd parser */
    Ast::AstParse parsed((list<Ast::Section*>*) Util::parseCmd(full.path()));
    State * currentState = NULL;
    for (Ast::AstParse::section_iterator section_it = parsed.getSections()->begin(); section_it != parsed.getSections()->end(); section_it++){
        Ast::Section * section = *section_it;
        std::string head = section->getName();
        head = Util::fixCase(head);

        if (PaintownUtil::matchRegex(head, "statedef")){
            currentState = parseStateDefinition(section, full);
        } else if (PaintownUtil::matchRegex(head, "state ")){
            StateController * controller = parseState(section);
            if (controller != NULL){
                if (controller->getState() != currentState->getState()){
                    Global::debug(1) << "Warning: controller '" << controller->getName() << "' specified state " << controller->getState() << " which does not match the most recent state definition " << currentState->getState() << " in file " << full.path() << endl;
                }
                currentState->addController(controller);
            }
        }
    }
}

/* a container for a directory and a file */
struct Location{
    Location(Filesystem::AbsolutePath base, string file):
        base(base), file(file){
        }

    Filesystem::AbsolutePath base;
    string file;
};

void Character::load(int useAct){
#if 0
    // Lets look for our def since some people think that all file systems are case insensitive
    baseDir = Filesystem::find("mugen/chars/" + location + "/");
    Global::debug(1) << baseDir << endl;
    std::string realstr = Mugen::Util::stripDir(location);
    const std::string ourDefFile = Mugen::Util::fixFileName(baseDir, std::string(realstr + ".def"));
    
    if (ourDefFile.empty()){
        throw MugenException( "Cannot locate player definition file for: " + location );
    }
#endif
    
    // baseDir = Filesystem::cleanse(Mugen::Util::getFileDir(location));
    baseDir = location.getDirectory();
    // const std::string ourDefFile = location;
     
    Ast::AstParse parsed(Util::parseDef(location.path()));
    try{
        /* Extract info for our first section of our stage */
        for (Ast::AstParse::section_iterator section_it = parsed.getSections()->begin(); section_it != parsed.getSections()->end(); section_it++){
            Ast::Section * section = *section_it;
            std::string head = section->getName();
            /* this should really be head = Mugen::Util::fixCase(head) */
            head = Mugen::Util::fixCase(head);

            if (head == "info"){
                class InfoWalker: public Ast::Walker {
                    public:
                        InfoWalker(Character & who):
                            self(who){
                            }

                        Character & self;

                        virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                            if (simple == "name"){
                                simple >> self.name;
                            } else if (simple == "displayname"){
                                simple >> self.displayName;
                            } else if (simple == "versiondate"){
                                simple >> self.versionDate;
                            } else if (simple == "mugenversion"){
                                simple >> self.mugenVersion;
                            } else if (simple == "author"){
                                simple >> self.author;
                            } else if (simple == "pal.defaults"){
                                vector<int> numbers;
                                simple >> numbers;
                                for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); it++){
                                    self.palDefaults.push_back((*it) - 1);
                                }
                                // Global::debug(1) << "Pal" << self.palDefaults.size() << ": " << num << endl;
                            } else throw MugenException("Unhandled option in Info Section: " + simple.toString());
                        }
                };

                InfoWalker walker(*this);
                Ast::Section * section = *section_it;
                section->walk(walker);
            } else if (head == "files"){
                class FilesWalker: public Ast::Walker {
                    public:
                        FilesWalker(Character & self, const Filesystem::AbsolutePath & location):
                            location(location),
                            self(self){
                            }

                        vector<Location> stateFiles;
                        const Filesystem::AbsolutePath & location;

                        Character & self;
                        virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                            if (simple == "cmd"){
                                string file;
                                simple >> file;
                                self.cmdFile = Filesystem::RelativePath(file);
                                /* loaded later after the state files */
                            } else if (simple == "cns"){
                                string file;
                                simple >> file;
                                /* just loads the constants */
                                self.loadCnsFile(Filesystem::RelativePath(file));
                            } else if (PaintownUtil::matchRegex(simple.idString(), "st[0-9]+")){
                                int num = atoi(PaintownUtil::captureRegex(simple.idString(), "st([0-9]+)", 0).c_str());
                                if (num >= 0 && num <= 12){
                                    string path;
                                    simple >> path;
                                    stateFiles.push_back(Location(self.baseDir, path));
                                    // simple >> self.stFile[num];
                                }
                            } else if (simple == "stcommon"){
                                string path;
                                simple >> path;
                                stateFiles.insert(stateFiles.begin(), Location(self.baseDir, path));
                            } else if (simple == "st"){
                                string path;
                                simple >> path;
                                stateFiles.push_back(Location(self.baseDir, path));
                            } else if (simple == "sprite"){
                                simple >> self.sffFile;
                            } else if (simple == "anim"){
                                simple >> self.airFile;
                            } else if (simple == "sound"){
                                simple >> self.sndFile;
                                // Mugen::Util::readSounds(Mugen::Util::fixFileName(self.baseDir, self.sndFile), self.sounds);
                                Util::readSounds(self.baseDir.join(Filesystem::RelativePath(self.sndFile)), self.sounds);
                            } else if (PaintownUtil::matchRegex(simple.idString(), "pal[0-9]+")){
                                int num = atoi(PaintownUtil::captureRegex(simple.idString(), "pal([0-9]+)", 0).c_str());
                                string what;
                                simple >> what;
                                self.palFile[num] = what;
                            } else {
                                Global::debug(0) << "Unhandled option in Files Section: " + simple.toString() << endl;
                            }
                        }
                };

                FilesWalker walker(*this, location);
                Ast::Section * section = *section_it;
                section->walk(walker);

                /* state controllers with no obvious parent. after parsing all
                 * the state files, go through this list and try to find the right parent
                 */
                for (vector<Location>::iterator it = walker.stateFiles.begin(); it != walker.stateFiles.end(); it++){
                    Location & where = *it;
                    try{
                        /* load definitions first */
                        loadStateFile(where.base, where.file);
                    } catch (const MugenException & e){
                        ostringstream out;
                        out << "Problem loading state file " << where.file << ": " << e.getReason();
                        throw MugenException(out.str());
                    } catch (const Mugen::Cmd::ParseException & e){
                        ostringstream out;
                        out << "Problem loading state file " << where.file << ": " << e.getReason();
                        throw MugenException(out.str());
                    }

                }

                loadCmdFile(cmdFile);

#if 0
                for (vector<Location>::iterator it = walker.stateFiles.begin(); it != walker.stateFiles.end(); it++){
                    Location & where = *it;
                    try{
                        /* load definitions first */
                        loadStateFile(where.base, where.file, true, false);
                    } catch (const MugenException & e){
                        ostringstream out;
                        out << "Problem loading state file " << where.file << ": " << e.getReason();
                        throw MugenException(out.str());
                    } catch (const Mugen::Cmd::ParseException & e){
                        ostringstream out;
                        out << "Problem loading state file " << where.file << ": " << e.getReason();
                        throw MugenException(out.str());
                    }
                }

                for (vector<Location>::iterator it = walker.stateFiles.begin(); it != walker.stateFiles.end(); it++){
                    Location & where = *it;
                    try{
                        /* then load controllers */
                        loadStateFile(where.base, where.file, false, true);
                    } catch (const MugenException & e){
                        ostringstream out;
                        out << "Problem loading state file " << where.file << ": " << e.getReason();
                        throw MugenException(out.str());
                    } catch (const Mugen::Cmd::ParseException & e){
                        ostringstream out;
                        out << "Problem loading state file " << where.file << ": " << e.getReason();
                        throw MugenException(out.str());
                    }
                }
#endif


#if 0
                /* now just load the state controllers */
                for (vector<Location>::iterator it = walker.stateFiles.begin(); it != walker.stateFiles.end(); it++){
                    Location & where = *it;
                    try{
                        loadStateFile(where.base, where.file, false, true);
                    } catch (const MugenException & e){
                        ostringstream out;
                        out << "Problem loading state file " << where.file << ": " << e.getReason();
                        throw MugenException(out.str());
                    }
                }
#endif

                /*
                   if (commonStateFile != ""){
                   loadStateFile("mugen/data/", commonStateFile);
                   }
                   if (stateFile != ""){
                   loadStateFile("mugen/chars/" + location, stateFile);
                   }
                   if (
                   */

            } else if (head == "arcade"){
                class ArcadeWalker: public Ast::Walker {
                    public:
                        ArcadeWalker(Character & self):
                            self(self){
                            }

                        Character & self;

                        virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                            if (simple == "intro.storyboard"){
                                simple >> self.introFile;
                            } else if (simple == "ending.storyboard"){
                                simple >> self.endingFile;
                            } else {
                                throw MugenException("Unhandled option in Arcade Section: " + simple.toString());
                            }
                        }
                };

                ArcadeWalker walker(*this);
                Ast::Section * section = *section_it;
                section->walk(walker);
            }
        }
    } catch (const Ast::Exception & e){
        ostringstream out;
        out << "Could not load " << location.path() << ": " << e.getReason();
        throw MugenException(out.str());
    }

    /* Is this just for testing? */
    if (getMaxHealth() == 0 || getHealth() == 0){
        setHealth(1000);
        setMaxHealth(1000);
    }

    // Current palette
    if (palDefaults.empty()){
	// Correct the palette defaults
	for (unsigned int i = 0; i < palFile.size(); ++i){
	    palDefaults.push_back(i);
	}
    }
    /*
    if (palDefaults.size() < palFile.size()){
	bool setPals[palFile.size()];
	for( unsigned int i =0;i<palFile.size();++i){
	    setPals[i] = false;
	}
	// Set the ones already set
	for (unsigned int i = 0; i < palDefaults.size(); ++i){
	    setPals[palDefaults[i]] = true;
	}
	// now add the others
	for( unsigned int i =0;i<palFile.size();++i){
	    if(!setPals[i]){
		palDefaults.push_back(i);
	    }
	}
    }
    */
    std::string paletteFile = "";
    currentPalette = useAct;
    if (palFile.find(currentPalette) == palFile.end()){
        /* FIXME: choose a default. its not just palette 1 because that palette
         * might not exist
         */
	Global::debug(1) << "Couldn't find palette: " << currentPalette << " in palette collection. Defaulting to internal palette if available." << endl;
        paletteFile = palFile.begin()->second;
    } else {
	paletteFile = palFile[currentPalette];
	Global::debug(2) << "Current pal: " << currentPalette << " | Palette File: " << paletteFile << endl;
    }
    /*
    if (currentPalette > palFile.size() - 1){
        currentPalette = 1;
    }
    */
    Global::debug(2) << "Reading Sff (sprite) Data..." << endl; 
    /* Sprites */
    // Mugen::Util::readSprites( Mugen::Util::fixFileName(baseDir, sffFile), Mugen::Util::fixFileName(baseDir, paletteFile), sprites);
    Util::readSprites(baseDir.join(Filesystem::RelativePath(sffFile)), baseDir.join(Filesystem::RelativePath(paletteFile)), sprites, true);
    destroyRaw(sprites);
    Global::debug(2) << "Reading Air (animation) Data..." << endl;
    /* Animations */
    // animations = Mugen::Util::loadAnimations(Mugen::Util::fixFileName(baseDir, airFile), sprites);
    animations = Util::loadAnimations(baseDir.join(Filesystem::RelativePath(airFile)), sprites, true);

    fixAssumptions();

    /*
    State * state = states[-1];
    for (vector<StateController*>::const_iterator it = state->getControllers().begin(); it != state->getControllers().end(); it++){
        Global::debug(0) << "State -1: '" << (*it)->getName() << "'" << endl;
    }
    */
}

void Character::destroyRaw(const map< unsigned int, std::map< unsigned int, MugenSprite * > > & sprites){
    for (map< unsigned int, std::map< unsigned int, MugenSprite * > >::const_iterator i = sprites.begin() ; i != sprites.end() ; ++i ){
        for(map< unsigned int, MugenSprite * >::const_iterator j = i->second.begin() ; j != i->second.end() ; ++j ){
            MugenSprite * sprite = j->second;
            sprite->unloadRaw();
        }
    }
}
        
bool Character::hasAnimation(int index) const {
    return getAnimation(index) != NULL;
}

/* completely arbitrary number, just has to be unique and unlikely to
 * be used by the system. maybe negative numbers are better?
 */
static const int JumpIndex = 234823;

class MutableCompiledInteger: public Compiler::Value {
public:
    MutableCompiledInteger(int value):
        value(value){
        }

    int value;

    virtual RuntimeValue evaluate(const Environment & environment) const {
        return RuntimeValue(value);
    }

    virtual void set(int value){
        this->value = value;
    }

    virtual int get() const {
        return this->value;
    }

    virtual ~MutableCompiledInteger(){
    }
};

void Character::resetJump(MugenStage & stage, const vector<string> & inputs){
    setSystemVariable(JumpIndex, RuntimeValue(0));
    /*
    RuntimeValue number = getSystemVariable(JumpIndex);
    number->set(0);
    */
    changeState(stage, JumpStart, inputs);
}

void Character::doubleJump(MugenStage & stage, const vector<string> & inputs){
    /*
    MutableCompiledInteger * number = (MutableCompiledInteger*) getSystemVariable(JumpIndex);
    number->set(number->get() + 1);
    */
    setSystemVariable(JumpIndex, RuntimeValue(getSystemVariable(JumpIndex).toNumber() + 1));
    changeState(stage, AirJumpStart, inputs);
}

void Character::stopGuarding(MugenStage & stage, const vector<string> & inputs){
    guarding = false;
    if (stateType == StateType::Crouch){
        changeState(stage, Crouching, inputs);
    } else if (stateType == StateType::Air){
        changeState(stage, 52, inputs);
    } else {
        changeState(stage, Standing, inputs);
    }
}

static StateController * parseController(const string & input, const string & name, int state, StateController::Type type){
    try{
        Ast::AstParse sections((list<Ast::Section*>*) Mugen::Cmd::parse(input.c_str()));
        if (sections.getSections()->size() == 0){
            ostringstream out;
            out << "Could not parse controller: " << input;
            throw MugenException(out.str());
        }
        Ast::Section * first = sections.getSections()->front();
        return StateController::compile(first, name, state, type);
    } catch (const Ast::Exception & e){
        throw MugenException(e.getReason());
    } catch (const Mugen::Cmd::ParseException & e){
        ostringstream out;
        out << "Could not parse " << input << " because " << e.getReason();
        throw MugenException(out.str());
    }
}

void Character::fixAssumptions(){
    /* need a -1 state controller that changes to state 20 if holdfwd
     * or holdback is pressed
     */

    if (states[-1] != 0){
        /* walk */
        {
            ostringstream raw;
            raw << "[State -1, paintown-internal-walk]\n";
            raw << "triggerall = stateno = 0\n";
            raw << "trigger1 = command = \"holdfwd\"\n";
            raw << "trigger2 = command = \"holdback\"\n";
            raw << "value = " << WalkingForwards << "\n";
            states[-1]->addController(parseController(raw.str(), "paintown internal walk", -1, StateController::ChangeState));
        }


        /* crouch */
        {
            ostringstream raw;
            raw << "[State -1, paintown-internal-crouch]\n";
            raw << "value = " << StandToCrouch << "\n";
            raw << "triggerall = ctrl\n";
            raw << "trigger1 = stateno = 0\n";
            raw << "trigger2 = stateno = " << WalkingForwards << "\n";
            raw << "triggerall = command = \"holddown\"\n";

            states[-1]->addControllerFront(parseController(raw.str(), "paintown internal crouch", -1, StateController::ChangeState));
        }

        /* jump */
        {
            class InternalJumpController: public StateController {
            public:
                InternalJumpController():
                StateController("jump", -1){
                }

                virtual void activate(MugenStage & stage, Character & guy, const vector<string> & commands) const {
                    guy.resetJump(stage, commands);
                }

                virtual StateController * deepCopy() const {
                    return new InternalJumpController();
                }
            };

            InternalJumpController * controller = new InternalJumpController();
            controller->addTriggerAll(Compiler::compileAndDelete(new Ast::SimpleIdentifier("ctrl")));
            controller->addTriggerAll(Compiler::compileAndDelete(new Ast::ExpressionInfix(Ast::ExpressionInfix::Equals,
                        new Ast::SimpleIdentifier("statetype"),
                        new Ast::SimpleIdentifier("S"))));
            controller->addTrigger(1, Compiler::compileAndDelete(new Ast::ExpressionInfix(Ast::ExpressionInfix::Equals,
                        new Ast::SimpleIdentifier("command"),
                        new Ast::String(new string("holdup")))));
            states[-1]->addController(controller);
        }

        /* double jump */
        {
            string jumpCommand = "internal:double-jump-command";
            vector<Ast::Key*> keys;
            keys.push_back(new Ast::KeyModifier(Ast::KeyModifier::Release, new Ast::KeySingle("U")));
            keys.push_back(new Ast::KeySingle("U"));
            Command * doubleJumpCommand = new Command(jumpCommand, new Ast::KeyList(keys), 5, 0);
            addCommand(doubleJumpCommand);

            setSystemVariable(JumpIndex, RuntimeValue(0));

            class InternalDoubleJumpController: public StateController {
            public:
                InternalDoubleJumpController():
                StateController("double jump", -1){
                }

                virtual void activate(MugenStage & stage, Character & guy, const vector<string> & commands) const {
                    guy.doubleJump(stage, commands);
                }

                StateController * deepCopy() const {
                    return new InternalDoubleJumpController();
                }
            };

            InternalDoubleJumpController * controller = new InternalDoubleJumpController();
            controller->addTriggerAll(Compiler::compileAndDelete(new Ast::SimpleIdentifier("ctrl")));
            controller->addTriggerAll(Compiler::compileAndDelete(new Ast::ExpressionInfix(Ast::ExpressionInfix::Equals,
                        new Ast::SimpleIdentifier("statetype"),
                        new Ast::SimpleIdentifier("A"))));
            controller->addTriggerAll(Compiler::compileAndDelete(new Ast::ExpressionInfix(Ast::ExpressionInfix::GreaterThan,
                                                                                          new Ast::ExpressionUnary(Ast::ExpressionUnary::Minus,
                                                                                                                   new Ast::Keyword("pos y")),
                                                                                          new Ast::SimpleIdentifier("internal:airjump-height"))));
            controller->addTriggerAll(Compiler::compileAndDelete(new Ast::ExpressionInfix(Ast::ExpressionInfix::LessThan,
                        new Ast::Function("sysvar", new Ast::ValueList(new Ast::Number(JumpIndex))),
                        new Ast::SimpleIdentifier("internal:extra-jumps"))));
            controller->addTrigger(1, Compiler::compileAndDelete(new Ast::ExpressionInfix(Ast::ExpressionInfix::Equals,
                        new Ast::SimpleIdentifier("command"),
                        // new Ast::String(new string("holdup")
                        new Ast::String(new string(jumpCommand)
                            ))));
            states[-1]->addController(controller);
        }
    }

    {
        if (states[StopGuardStand] != 0){
            class StopGuardStandController: public StateController {
            public:
                StopGuardStandController():
                StateController("stop guarding", StopGuardStand){
                }

                virtual void activate(MugenStage & stage, Character & guy, const vector<string> & commands) const {
                    guy.stopGuarding(stage, commands);
                }

                StateController * deepCopy() const {
                    return new StopGuardStandController();
                }
            };

            StopGuardStandController * controller = new StopGuardStandController();
            controller->addTrigger(1, Compiler::compileAndDelete(new Ast::ExpressionInfix(Ast::ExpressionInfix::Equals,
                    new Ast::SimpleIdentifier("animtime"),
                    new Ast::Number(0))));
            states[StopGuardStand]->addController(controller);
        }
    }

    /* need a 20 state controller that changes to state 0 if holdfwd
     * or holdback is not pressed
     */
    if (states[20] != 0){
        ostringstream raw;
        raw << "[State 20, paintown-internal-stop-walking]\n";
        raw << "value = " << Standing << "\n";
        raw << "trigger1 = command != \"holdfwd\"\n";
        raw << "trigger1 = command != \"holdback\"\n";

        states[20]->addController(parseController(raw.str(), "paintown internal stop walking", 20, StateController::ChangeState));
    }

    if (states[Standing] != 0){
        states[Standing]->setControl(Compiler::compile(1));
    }

    if (states[StandToCrouch] != NULL){
        ostringstream raw;
        raw << "[State " << StandToCrouch << ", paintown-internal-stand-while-crouching]\n";
        raw << "value = " << CrouchToStand << "\n";
        raw << "trigger1 = command != \"holddown\"\n";

        states[StandToCrouch]->addController(parseController(raw.str(), "stand while crouching", StandToCrouch, StateController::ChangeState));

    }

    /* stand after crouching */
    if (states[11] != 0){
        ostringstream raw;
        raw << "[State 11, paintown-internal-stand-after-crouching]\n";
        raw << "value = " << CrouchToStand << "\n";
        raw << "trigger1 = command != \"holddown\"\n";

        states[11]->addController(parseController(raw.str(), "stand after crouching", 11, StateController::ChangeState));
    }

    /* get up kids */
    if (states[Liedown] != 0){
        ostringstream raw;
        raw << "[State " << Liedown << ", paintown-internal-get-up]\n";
        raw << "value = " << GetUpFromLiedown << "\n";
        raw << "trigger1 = time >= " << getLieDownTime() << "\n";

        states[Liedown]->addController(parseController(raw.str(), "get up", Liedown, StateController::ChangeState));
    }

    /* standing turn state */
    {
        State * turn = new State(5);
        turn->setType(State::Unchanged);
        turn->setAnimation(Compiler::compile(5));
        states[5] = turn;

        ostringstream raw;
        raw << "[State 5, paintown-internal-turn]\n";
        raw << "value = " << Standing << "\n";
        raw << "trigger1 = animtime = 0\n";

        turn->addController(parseController(raw.str(), "turn", 5, StateController::ChangeState));
    }

    /* if y reaches 0 then auto-transition to state 52.
     * probably just add a trigger to state 50
     */
    if (states[50] != 0){
        /*
        ostringstream raw;
        raw << "[State 50, paintown-internal-land]\n";
        raw << "value = 52\n";
        raw << "trigger1 = pos y >= 0\n";
        raw << "trigger1 = vel y >= 0\n";

        states[50]->addController(parseController(raw.str(), "land", 50, StateController::ChangeState));
        */

        /*
        StateController * controller = new StateController("jump land");
        controller->setType(StateController::ChangeState);
        controller->setValue1(new Ast::Number(52));
        controller->addTrigger(1, new Ast::ExpressionInfix(Ast::ExpressionInfix::GreaterThanEquals,
                    new Ast::Keyword("pos y"),
                    new Ast::Number(0)));
        controller->addTrigger(1, new Ast::ExpressionInfix(Ast::ExpressionInfix::GreaterThan,
                    new Ast::Keyword("vel y"),
                    new Ast::Number(0)));
        states[50]->addController(controller);
        */

    }
}

// Render sprite
void Character::renderSprite(const int x, const int y, const unsigned int group, const unsigned int image, Bitmap *bmp , const int flip, const double scalex, const double scaley ){
    MugenSprite *sprite = sprites[group][image];
    if (sprite){
	Bitmap *bitmap = sprite->getBitmap(true); //bitmaps[group][image];
	/*if (!bitmap){
	    bitmap = new Bitmap(Bitmap::memoryPCX((unsigned char*) sprite->pcx, sprite->newlength));
	    bitmaps[group][image] = bitmap;
	}*/
	const int width = (int)(bitmap->getWidth() * scalex);
	const int height =(int)(bitmap->getHeight() * scaley);
	if (flip == 1){
	    bitmap->drawStretched(x,y, width, height, *bmp);
	} else if (flip == -1){
	    // temp bitmap to flip and crap
	    Bitmap temp = Bitmap::temporaryBitmap(bitmap->getWidth(), bitmap->getHeight());
	    temp.fill(Bitmap::MaskColor());
	    bitmap->drawHFlip(0,0,temp);
	    temp.drawStretched(x-width,y, width, height, *bmp);
	}
    }
}
        
bool Character::canRecover() const {
    /* FIXME */
    return true;
    // return getY() == 0;
}

void Character::nextPalette(){
    if (currentPalette < palDefaults.size()-1){
	currentPalette++;
    } else currentPalette = 0;
    Global::debug(1) << "Current pal: " << currentPalette << " | Location: " << palDefaults[currentPalette] << " | Palette File: " << palFile[palDefaults[currentPalette]] << endl;
}

void Character::priorPalette(){
    if (currentPalette > 0){
	currentPalette--;
    } else currentPalette = palDefaults.size() -1;
    Global::debug(1) << "Current pal: " << currentPalette << " | Palette File: " << palFile[palDefaults[currentPalette]] << endl;
}

const Bitmap * Character::getCurrentFrame() const {
    return getCurrentAnimation()->getCurrentFrame()->getSprite()->getBitmap(true);
}

void Character::drawReflection(Bitmap * work, int rel_x, int rel_y, int intensity){
    getCurrentAnimation()->renderReflection(getFacing() == Object::FACING_LEFT, true, intensity, getRX() - rel_x, (int)(getZ() + getY() - rel_y), *work);
}

MugenAnimation * Character::getCurrentAnimation() const {
    return getAnimation(currentAnimation);
    /*
    typedef std::map< int, MugenAnimation * > Animations;
    Animations::const_iterator it = getAnimations().find(currentAnimation);
    if (it != getAnimations().end()){
        MugenAnimation * animation = (*it).second;
        return animation;
    }
    return NULL;
    */
}

/* returns all the commands that are currently active */
vector<string> Character::doInput(const MugenStage & stage){
    if (behavior == NULL){
        throw MugenException("Internal error: No behavior specified");
    }

    return behavior->currentCommands(stage, this, commands, getFacing() == Object::FACING_RIGHT);
}

bool Character::isPaused(){
    return hitState.shakeTime > 0;
}

int Character::pauseTime() const {
    return hitState.shakeTime;
}

/*
InputMap<Mugen::Keys> & Character::getInput(){
    if (getFacing() == Object::FACING_RIGHT){
        return inputLeft;
    }
    return inputRight;
}
*/

static bool holdingBlock(const vector<string> & commands){
    for (vector<string>::const_iterator it = commands.begin(); it != commands.end(); it++){
        if (*it == "holdback"){
            return true;
        }
    }

    return false;
}

/* Inherited members */
void Character::act(vector<Paintown::Object*>* others, World* world, vector<Paintown::Object*>* add){
    
    reversalActive = false;

    special.reset();

    if (frozen){
        frozen = false;
    }

    /* reset some stuff */
    blocking = false;
    widthOverride.enabled = false;
    transOverride.enabled = false;

    if (afterImage.lifetime > 0){
        afterImage.lifetime -= 1;
        afterImage.currentTime += 1;

        int x = getRX();
        int y = getRY();

        while (afterImage.currentTime >= afterImage.timegap){
            int life = 200;
            MugenAnimation * animation = getCurrentAnimation();
            afterImage.currentTime -= afterImage.timegap;
            MugenFrame * currentSprite = animation->getCurrentFrame();
            afterImage.frames.push_front(AfterImage::Frame(currentSprite, animation->getCurrentEffects(getFacing() == Object::FACING_LEFT, false, xscale, yscale), life, x, y));
            if (afterImage.frames.size() > afterImage.length){
                afterImage.frames.resize(afterImage.length);
            }
        }
    }

    for (deque<AfterImage::Frame>::iterator it = afterImage.frames.begin(); it != afterImage.frames.end(); /**/ ){
        AfterImage::Frame & frame = *it;
        frame.life -= 1;
        /* negative lifetimes mean indefinite frames */
        if (frame.life == 0){
            it = afterImage.frames.erase(it);
        } else {
            it++;
        }
    }

    for (int slot = 0; slot < 2; slot++){
        if (hitByOverride[slot].time > 0){
            hitByOverride[slot].time -= 1;
        }
    }

    // if (hitState.shakeTime > 0 && moveType != Move::Hit){
    if (hitState.shakeTime > 0){
        hitState.shakeTime -= 1;
        return;
    }

    /*
    if (nextCombo > 0){
        nextCombo -= 1;
        if (nextCombo <= 0){
            combo = 0;
        }
    }
    */

    MugenAnimation * animation = getCurrentAnimation();
    if (animation != 0){
	/* Check debug state */
	if (debug){
	    if (!animation->showingDefense()){
		animation->toggleDefense();
	    }
	    if (!animation->showingOffense()){
		animation->toggleOffense();
	    }
	} else {
	    if (animation->showingDefense()){
		animation->toggleDefense();
	    }
	    if (animation->showingOffense()){
		animation->toggleOffense();
	    }
	}
        animation->logic();
    }

    /* redundant for now */
    if (hitState.shakeTime > 0){
        hitState.shakeTime -= 1;
    } else if (hitState.hitTime > -1){
        hitState.hitTime -= 1;
    }

    /* if shakeTime is non-zero should we update stateTime? */
    stateTime += 1;
    
    /* hack! */
    MugenStage & stage = *(MugenStage*) world;

    /* active is the current set of commands */
    vector<string> active = doInput(stage);
    /* always run through the negative states */

    blocking = holdingBlock(active);

    if (needToGuard){
        needToGuard = false;
        /* misnamed state, but this is the first guard state and will
         * eventually transition to stand/crouch/air guard
         */
        guarding = true;
        changeState(stage, Mugen::StartGuardStand, active);
    }

    doStates(stage, active, -3);
    doStates(stage, active, -2);
    doStates(stage, active, -1);
    doStates(stage, active, currentState);

    /*
    while (doStates(active, currentState)){
        / * empty * /
    }
    */

    /*! do regeneration if set */
    if (regenerateHealth){
        if (getHealth() < 5){
            setHealth(5);
            regenerateTime = REGENERATE_TIME;
        }

        if (regenerating){

            /* avoid rounding errors */
            if (getHealth() >= getMaxHealth() - 2){
                setHealth(getMaxHealth());
            } else {
                setHealth((int) ((getMaxHealth() + getHealth()) / 2.0));
            }

            if (getHealth() == getMaxHealth()){
                regenerating = false;
            }
            regenerateTime = REGENERATE_TIME;
        } else if (getHealth() < getMaxHealth() && regenerateTime == REGENERATE_TIME){
            regenerateTime -= 1;
        } else if (regenerateTime <= 0){
            regenerating = true;
        } else {
            regenerateTime -= 1;
        }

        /*
        if (getHealth() < getMaxHealth()){
            regenerateTime = REGENERATE_TIME;
        } else {
            if (regenerateTime <= 0){
                setHealth((getMaxHealth() + getHealth())/2);
                regenerateHealthDifference = getHealth();
            } else {
                regenerateTime -= 1;
            }
        }
        */
    }
}
        
void Character::addPower(double d){
    power += d;
    /* max power is 3000. is that specified somewhere or just hard coded
     * in the engine?
     */
    if (power > 3000){
        power = 3000;
    }

    if (power < 0){
        power = 0;
    }
}

void Character::addCombo(int combo){
    hitCount += combo;
    if (hitCount < 0){
        hitCount = 0;
    }
}
        
void Character::didHit(Object * enemy, MugenStage & stage){
    hitState.shakeTime = getHit().pause.player1;
    addPower(getHit().getPower.hit);

    if (getState(getCurrentState())->powerChanged()){
        addPower(getState(getCurrentState())->getPower()->evaluate(FullEnvironment(stage, *this)).toNumber());
    }

    /* if he is already in a Hit state then increase combo */
    if (enemy->getMoveType() == Move::Hit){
        combo += 1;
    } else {
        combo = 1;
    }

    // nextCombo = 15;

    hitCount += 1;

    if (behavior != NULL){
        behavior->hit(enemy);
    }
}
        
void Character::takeDamage(World & world, ObjectAttack * obj, int amount, bool kill, bool defense){
    if (defense){
        Object::takeDamage(world, obj, (int)(amount / defenseMultiplier));
    } else {
        Object::takeDamage(world, obj, amount);
    }
    if (!kill){
        if (getHealth() < 1){
            setHealth(1);
        }
    }
}
        
void Character::takeDamage(World & world, ObjectAttack * obj, int amount){
    takeDamage(world, obj, amount, true, true);
}

void Character::wasHit(MugenStage & stage, Object * enemy, const HitDefinition & hisHit){
    hitState.update(stage, *this, getY() > 0, hisHit);
    setXVelocity(hitState.xVelocity);
    setYVelocity(hitState.yVelocity);
    lastTicket = enemy->getTicket();

    if (hisHit.damage.damage != 0){
        takeDamage(stage, enemy, hisHit.damage.damage, true, true);
    }

    /*
    if (getHealth() <= 0){
        hitState.fall.fall = true;
    }
    */

    juggleRemaining -= enemy->getCurrentJuggle() + hisHit.airJuggle;
    
    vector<string> active;
    /* FIXME: replace 5000 with some constant */
    changeState(stage, 5000, active);

    /*
    vector<string> active;
    while (doStates(active, currentState)){
    }
    */
}

/* returns true if a state change occured */
bool Character::doStates(MugenStage & stage, const vector<string> & active, int stateNumber){
    int oldState = getCurrentState();
    if (getState(stateNumber) != 0){
        State * state = getState(stateNumber);
        for (vector<StateController*>::const_iterator it = state->getControllers().begin(); it != state->getControllers().end(); it++){
            StateController * controller = *it;
            Global::debug(2 * !controller->getDebug()) << "State " << stateNumber << " check state controller " << controller->getName() << endl;

#if 0
            /* more debugging */
            bool hasFF = false;
            for (vector<string>::const_iterator it = active.begin(); it != active.end(); it++){
                if (*it == "holdup"){
                    hasFF = true;
                }
            }
            if (stateNumber == -1 && controller->getName() == "jump" && hasFF){
            if (controller->getName() == "run fwd"){
                int x = 2;
            }
            /* for debugging
            if (stateNumber == 20 && controller->getName() == "3"){
                int x = 2;
            }
            */
#endif

            try{
                if (controller->canTrigger(stage, *this, active)){
                    /* check if the controller's persistent values allow it
                     * to be activated.
                     */
                    if (controller->persistentOk()){
                        /* activate may modify the current state */
                        controller->activate(stage, *this, active);

                        if (stateNumber >= 0 && getCurrentState() != oldState){
                            return true;
                        }
                    }
                }
            } catch (const MugenException & me){
                Global::debug(0) << "Error while processing state " << stateNumber << ", " << controller->getName() << ". Error with trigger: " << me.getReason() << endl;
            }
        }
    }
    return false;
}

void Character::draw(Bitmap * work, int cameraX, int cameraY){
    /*
    int color = Bitmap::makeColor(255,255,255);
    font.printf( x, y, color, *work, "State %d Animation %d", 0,  getCurrentState(), currentAnimation);
    font.printf( x, y + font.getHeight() + 1, color, *work, "X %f Y %f", 0, (float) getXVelocity(), (float) getYVelocity());
    font.printf( x, y + font.getHeight() * 2 + 1, color, *work, "Time %d", 0, getStateTime());
    */

    if (special.invisible){
        return;
    }

    MugenAnimation * animation = getCurrentAnimation();
    /* this should never be NULL... */
    if (animation != NULL){
        int x = getRX() - cameraX + drawOffset.x;
        int y = getRY() - cameraY + drawOffset.y;

        if (isPaused() && moveType == Move::Hit){
            x += PaintownUtil::rnd(3) - 1;
        }

        for (unsigned int index = 0; index < afterImage.frames.size(); index += afterImage.framegap){
            class AfterImageFilter: public Bitmap::Filter {
            public:
                AfterImageFilter(const AfterImage::RGB & bright, const AfterImage::RGB & contrast, const AfterImage::RGB & post, const AfterImage::RGB & extraAdd, const AfterImage::RGB & extraMultiplier, int extra):
                    bright(bright),
                    contrast(contrast),
                    post(post),
                    extraAdd(extraAdd),
                    extraMultiplier(extraMultiplier),
                    extra(extra){
                    }

                const AfterImage::RGB & bright;
                const AfterImage::RGB & contrast;
                const AfterImage::RGB & post;
                const AfterImage::RGB & extraAdd;
                const AfterImage::RGB & extraMultiplier;
                const int extra;
                mutable map<unsigned, unsigned int> cache;

                unsigned int doFilter(int red, int green, int blue) const {
                    int red_out = red;
                    int green_out = green;
                    int blue_out = blue;
                    for (int i = 0; i < extra; i++){
                        red_out += extraAdd.red;
                        red_out *= extraMultiplier.red;
                        green_out += extraAdd.green;
                        green_out *= extraMultiplier.green;
                        blue_out += extraAdd.blue;
                        blue_out *= extraMultiplier.blue;
                    }

                    red_out = (red_out + bright.red) * contrast.red / 256 + post.red;
                    green_out = (green_out + bright.green) * contrast.green / 256 + post.green;
                    blue_out = (blue_out + bright.blue) * contrast.blue / 256 + post.blue;
                    
                    if (red_out < 0){
                        red_out = 0;
                    }
                    if (red_out > 255){
                        red_out = 255;
                    }
                    if (green_out < 0){
                        green_out = 0;
                    }
                    if (green_out > 255){
                        green_out = 255;
                    }
                    if (blue_out < 0){
                        blue_out = 0;
                    }
                    if (blue_out > 255){
                        blue_out = 255;
                    }

                    int out = Bitmap::makeColor(red_out, green_out, blue_out);
                    return (unsigned int) out;
                }

                unsigned int filter(unsigned int pixel) const {
                    if (cache.find(pixel) != cache.end()){
                        return cache[pixel];
                    }

                    int red = Bitmap::getRed(pixel);
                    int green = Bitmap::getGreen(pixel);
                    int blue = Bitmap::getBlue(pixel);
                    unsigned int out = doFilter(red, green, blue);
                    cache[pixel] = out;
                    return out;
                }
            };

            AfterImage::Frame & frame = afterImage.frames[index];
            Bitmap fixed;
            if (frame.extra != index){
                /* TODO: handle afterImage.color and afterImage.invert */
                AfterImageFilter filter(afterImage.bright, afterImage.contrast, afterImage.postBright, afterImage.add, afterImage.multiply, index);

                // frame.sprite->render(frame.x - cameraX + drawOffset.x, frame.y - cameraY + drawOffset.y, *work, frame.effects + afterImage.translucent + blender);
                Bitmap original = frame.sprite->getSprite()->getFinalBitmap(frame.effects);
                fixed = Bitmap::temporaryBitmap2(original.getWidth(), original.getHeight());
                original.draw(0, 0, filter, fixed);
                frame.extra = index;
                /* trade time for space -- but allocating a bitmap is sort
                 * of expensive..
                 */
                frame.cache = Bitmap(fixed, true);
            } else {
                fixed = frame.cache;
            }
            MugenSprite::draw(fixed, frame.x - cameraX + drawOffset.x + frame.sprite->xoffset, frame.y - cameraY + drawOffset.y + frame.sprite->yoffset, frame.sprite->getSprite()->getX(), frame.sprite->getSprite()->getY(), *work, frame.effects + afterImage.translucent);
        }

        animation->render(getFacing() == Object::FACING_LEFT, false, x, y, *work, xscale, yscale);
    }

    if (debug){
        const Font & font = Font::getFont(Global::DEFAULT_FONT, 18, 18);
        int x = 1;
        if (getAlliance() == MugenStage::Player2Side){
            x = 640 - font.textLength("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") - 1;
        }
        int y = 1;
        int color = Bitmap::makeColor(255, 255, 255);
        FontRender * render = FontRender::getInstance();
        render->addMessage(font, x, y, color, -1, "State %d Animation %d", getCurrentState(), currentAnimation);
        y += font.getHeight();
        render->addMessage(font, x, y, color, -1, "Vx %f Vy %f", getXVelocity(), getYVelocity());
        y += font.getHeight();
        render->addMessage(font, x, y, color, -1, "X %f Y %f", getX(), getY());
        y += font.getHeight();
        render->addMessage(font, x, y, color, -1, "Time %d", getStateTime());
        y += font.getHeight();
        render->addMessage(font, x, y, color, -1, "State type %s", getStateType().c_str());
        y += font.getHeight();
        render->addMessage(font, x, y, color, -1, "Attack type %s", getMoveType().c_str());
        y += font.getHeight();
        render->addMessage(font, x, y, color, -1, "Hit enabled %d", hit.isEnabled());
        y += font.getHeight();
        render->addMessage(font, x, y, color, -1, "Control %d", hasControl());
        y += font.getHeight();
        if (getMoveType() == Move::Hit){
            render->addMessage(font, x, y, color, -1, "HitShake %d HitTime %d", getHitState().shakeTime, getHitState().hitTime);
            y += font.getHeight();
            render->addMessage(font, x, y, color, -1, "Hit velocity x %f y %f", getHitState().xVelocity, getHitState().yVelocity);
        }

        int wx = 1;
        int wy = 1;
        int width = work->getWidth();
        int height = 110;
        if (getAlliance() == MugenStage::Player2Side){
            wx = work->getWidth() - width - 1;
        }
        Bitmap::transBlender(0, 0, 0, 128);
        work->translucent().rectangleFill(wx, wy, wx+width, wy+height, Bitmap::makeColor(0, 0, 0));
        work->translucent().line(0, wy+height, wx+width, wy+height, Bitmap::makeColor(64, 64, 64));
    }
}

bool Character::canTurn() const {
    return getCurrentState() == Standing ||
           getCurrentState() == WalkingForwards ||
           getCurrentState() == WalkingBackwards ||
           getCurrentState() == Crouching;
}
        
void Character::setTransOverride(TransType type, int alphaFrom, int alphaTo){
    transOverride.enabled = true;
    transOverride.type = type;
    transOverride.alphaSource = alphaFrom;
    transOverride.alphaDestination = alphaTo;
}

static MugenSound * findSound(const Mugen::SoundMap & sounds, int group, int item){
    Mugen::SoundMap::const_iterator findGroup = sounds.find(group);
    if (findGroup != sounds.end()){
        const map<unsigned int, MugenSound*> & found = (*findGroup).second;
        map<unsigned int, MugenSound*>::const_iterator sound = found.find(item);
        if (sound != found.end()){
            return (*sound).second;
        }
    }
    return NULL;
}

MugenSound * Character::getCommonSound(int group, int item) const {
    if (getCommonSounds() == NULL){
        return NULL;
    }
    return findSound(*getCommonSounds(), group, item);
}
        
MugenSound * Character::getSound(int group, int item) const {
    return findSound(getSounds(), group, item);
    /*
    map<unsigned int, map<unsigned int, MugenSound*> >::const_iterator findGroup = sounds.find(group);
    if (findGroup != sounds.end()){
        const map<unsigned int, MugenSound*> & found = (*findGroup).second;
        map<unsigned int, MugenSound*>::const_iterator sound = found.find(item);
        if (sound != found.end()){
            return (*sound).second;
        }
    }
    return 0;
    */
}

void Character::doTurn(MugenStage & stage){
    vector<string> active;
    if (getCurrentState() != Mugen::Crouching){
        changeState(stage, Mugen::StandTurning, active);
    } else {
        changeState(stage, Mugen::CrouchTurning, active);
    }
    reverseFacing();
}

void Character::grabbed(Paintown::Object*){
}

void Character::unGrab(){
}

bool Character::isGrabbed(){
    return false;
}

Paintown::Object* Character::copy(){
    return this;
}

const vector<MugenArea> Character::getAttackBoxes() const {
    return getCurrentAnimation()->getAttackBoxes(getFacing() == Object::FACING_LEFT);
}

const vector<MugenArea> Character::getDefenseBoxes() const {
    return getCurrentAnimation()->getDefenseBoxes(getFacing() == Object::FACING_LEFT);
}

const std::string& Character::getAttackName(){
    return getName();
}

bool Character::collision(ObjectAttack*){
    return false;
}

int Character::getDamage() const {
    return 0;
}

bool Character::isCollidable(Paintown::Object*){
    return true;
}

bool Character::isGettable(){
    return false;
}

bool Character::isGrabbable(Paintown::Object*){
    return true;
}

bool Character::isAttacking(){
    return false;
}

int Character::getWidth() const {
    if (widthOverride.enabled){
        return widthOverride.playerFront;
    }
    return groundfront;
}

int Character::getBackWidth() const {
    if (widthOverride.enabled){
        return widthOverride.playerBack;
    }
    return groundback;
}
        
int Character::getBackX() const {
    int width = getBackWidth();
    if (widthOverride.enabled){
        width = widthOverride.edgeBack;
    }
    if (getFacing() == Object::FACING_LEFT){
        return getRX() + width;
    }
    return getRX() - width;
}

int Character::getFrontX() const {
    int width = getWidth();
    if (widthOverride.enabled){
        width = widthOverride.edgeFront;
    }
    if (getFacing() == Object::FACING_LEFT){
        return getRX() + width;
    }
    return getRX() - width;
}

Network::Message Character::getCreateMessage(){
    return Network::Message();
}

void Character::getAttackCoords(int&, int&){
}

double Character::minZDistance() const{
    return 0;
}

void Character::attacked(World*, Paintown::Object*, std::vector<Paintown::Object*, std::allocator<Paintown::Object*> >&){
}
        
int Character::getCurrentCombo() const {
    return combo;
}

/* TODO: implement these */
void Character::setUnhurtable(){
}

void Character::setHurtable(){
}
        
void Character::addWin(WinGame win){
    wins.push_back(win);
}

void Character::addMatchWin(){
    matchWins += 1;
}

void Character::resetPlayer(){
    clearWins();
    power = 0;
    setHealth(getMaxHealth());
}
        
bool Character::isBlocking(const HitDefinition & hit){
    /* FIXME: can only block if in the proper state relative to the hit def */
    return hasControl() && blocking;
}

bool Character::isGuarding() const {
    return guarding;
}
        
void Character::guarded(Object * enemy, const HitDefinition & hit){
    /* FIXME: call hitState.updateGuard */
    hitState.guarded = true;
    lastTicket = enemy->getTicket();
    enemy->addPower(hit.getPower.guarded);
    /* the character will transition to the guard state when he next acts */
    needToGuard = true;
    bool inAir = getY() > 0;
    if (inAir){
    } else {
        setXVelocity(hit.guardVelocity);
    }
}

void Character::setAfterImage(int time, int length, int timegap, int framegap, TransType translucent, int paletteColor, bool invertColor, const AfterImage::RGB & bright, const AfterImage::RGB & contrast, const AfterImage::RGB & postBright, const AfterImage::RGB & add, const AfterImage::RGB & multiply){
    afterImage.currentTime = 0;
    afterImage.timegap = timegap;
    afterImage.framegap = framegap;
    afterImage.lifetime = time;
    afterImage.length = length;
    afterImage.translucent = translucent;
    afterImage.frames.clear();
    afterImage.paletteColor = paletteColor;
    afterImage.invertColor = invertColor;
    afterImage.bright = bright;
    afterImage.contrast = contrast;
    afterImage.postBright = postBright;
    afterImage.add = add;
    afterImage.multiply = multiply;
}
        
void Character::setAfterImageTime(int time){
    afterImage.lifetime = time;
}
        
void Character::updateAngleEffect(double angle){
    /* TODO */
}

double Character::getAngleEffect() const {
    /* FIXME */
    return 0;
}
        
void Character::drawAngleEffect(double angle, bool setAngle, double scaleX, double scaleY){
    /* TODO */
}
        
void Character::assertSpecial(Specials special){
    if (special == Invisible){
        this->special.invisible = true;
    }
}

void Character::enableHit(){
    hit.enable();
}

void Character::disableHit(){
    hit.disable();
}
        
void Character::setWidthOverride(int edgeFront, int edgeBack, int playerFront, int playerBack){
    widthOverride.enabled = true;
    widthOverride.edgeFront = edgeFront;
    widthOverride.edgeBack = edgeBack;
    widthOverride.playerFront = playerFront;
    widthOverride.playerBack = playerBack;
}
        
MugenAnimation * Character::getAnimation(int id) const {
    std::map<int, MugenAnimation*>::const_iterator where = getAnimations().find(id);
    if (where != getAnimations().end()){
        return where->second;
    }
    return NULL;
}
        
void Character::setHitByOverride(int slot, int time, bool standing, bool crouching, bool aerial, const std::vector<AttackType::Attribute> & attributes){
    hitByOverride[slot].time = time;
    hitByOverride[slot].standing = standing;
    hitByOverride[slot].crouching = crouching;
    hitByOverride[slot].aerial = aerial;
    hitByOverride[slot].attributes = attributes;
}
        
void Character::setDefenseMultiplier(double defense){
    defenseMultiplier = defense;
}

void Character::setAttackMultiplier(double attack){
    attackMultiplier = attack;
}
        
void Character::doFreeze(){
    frozen = true;
}
        
void Character::moveX( const int x ){
    if (!frozen){
        ObjectAttack::moveX(x);
    }
}

void Character::moveYNoCheck(double y){
    if (!frozen){
        ObjectAttack::moveYNoCheck(y);
    }
}
        
void Character::setSpritePriority(int priority){
    /* TODO */
}
        
void Character::setTemporaryAnimation(MugenAnimation * animation){
    /* TODO */
    delete animation;
}
        
bool Character::isHelper() const {
    return false;
}
        
void Character::setReversalActive(){
    reversalActive = true;
}

ReversalData & Character::getReversal(){
    return reversal;
}
        
void Character::disablePushCheck(){
    /* TODO */
}
        
void Character::setHitOverride(int slot, const string & attribute, int state, int time, bool air){
    /* TODO */
}

void Character::setDrawOffset(double x, double y){
    drawOffset.set(x, y);
}
    
State * Character::getState(int id) const {
    if (states.find(id) != states.end()){
        return states.find(id)->second;
    }
    return NULL;
}

void Character::setState(int id, State * what){
    delete states[id];
    states[id] = what;
}
        
}
