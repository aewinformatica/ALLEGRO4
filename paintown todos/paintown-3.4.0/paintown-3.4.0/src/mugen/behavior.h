#ifndef _paintown_mugen_behavior_h
#define _paintown_mugen_behavior_h

#include <vector>
#include <string>
#include "util.h"
#include "input/input-map.h"

class MugenStage;

namespace Mugen{

class Object;
class Character;
class Command;

/* handles input and tells the character what commands to invoke */
class Behavior{
public:
    Behavior();

    virtual std::vector<std::string> currentCommands(const MugenStage & stage, Character * owner, const std::vector<Command*> & commands, bool reversed) = 0;

    /* hit someone */
    virtual void hit(Object * enemy);

    virtual ~Behavior();
};

class HumanBehavior: public Behavior {
public:
    HumanBehavior(InputMap<Keys> left, InputMap<Keys> right);

    virtual std::vector<std::string> currentCommands(const MugenStage & stage, Character * owner, const std::vector<Command*> & commands, bool reversed);

    virtual ~HumanBehavior();

protected:
    InputMap<Keys> & getInput(bool facing);

protected:
    InputMap<Keys> right;
    InputMap<Keys> left;
};

/* dummy does absolutely nothing */
class DummyBehavior: public Behavior {
public:
    DummyBehavior();

    virtual std::vector<std::string> currentCommands(const MugenStage & stage, Character * owner, const std::vector<Command*> & commands, bool reversed);

    virtual ~DummyBehavior();
};

class RandomAIBehavior: public Behavior {
public:
    RandomAIBehavior();

    virtual std::vector<std::string> currentCommands(const MugenStage & stage, Character * owner, const std::vector<Command*> & commands, bool reversed);

    virtual ~RandomAIBehavior();
};

class LearningAIBehavior: public Behavior {
public:
    /* 1 is easy, 10 is high */
    LearningAIBehavior(int difficult);

    virtual std::vector<std::string> currentCommands(const MugenStage & stage, Character * owner, const std::vector<Command*> & commands, bool reversed);
    
    virtual void hit(Object * enemy);

    virtual ~LearningAIBehavior();

    /* stores information about the player when the move was executed */
    struct Move{
        Move():
        points(0),
        attempts(0),
        consecutive(0),
        minimumDistance(-1),
        maximumDistance(-1){
        }

        /* times the move succeeded */
        int points;

        /* times the move was tried */
        int attempts;

        /* how many consecutive times this move was tried */
        double consecutive;

        /* minimum known distance the move succeeded */
        int minimumDistance;

        /* maximum known distance the move succeeded */
        int maximumDistance;
    };
    
    enum Direction{
        Forward, Backward, Crouch, Stopped
    };

protected:
    std::string selectBestCommand(int distance, const std::vector<Command*> & commands);

    std::map<std::string, Move> moves;

    std::string lastCommand;
    int lastDistance;
    Direction direction;
    int difficulty;
    int dontMove;
};

}

#endif
