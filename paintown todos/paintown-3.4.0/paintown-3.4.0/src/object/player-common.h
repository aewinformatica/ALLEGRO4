#ifndef _paintown_player_common_h
#define _paintown_player_common_h

#include "character.h"
#include "util/load_exception.h"

namespace Filesystem{
    class AbsolutePath;
}

namespace Paintown{

class PlayerCommon: public Character {
public:
    PlayerCommon(const Character& chr) throw (LoadException);
    PlayerCommon(const char * filename) throw (LoadException);
    PlayerCommon(const Filesystem::AbsolutePath & str) throw (LoadException);
    PlayerCommon(const PlayerCommon & pl) throw (LoadException);
    virtual ~PlayerCommon();

    virtual void increaseScore(int much);

    virtual inline unsigned int getScore() const {
        return score;
    }

    virtual inline void setScore(unsigned int score){
        this->score = score;
    }
	
private:
    unsigned int score;

};

}

#endif
