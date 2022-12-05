#ifndef paintown_mugen_game_h
#define paintown_mugen_game_h

#include <string>

#include "util.h"

class Bitmap;
class MugenStage;

namespace Mugen{

class CharacterSelect;
    
/* Our game definition, this is to facilitate running a game */
class Game {
    public:
        Game(const PlayerType &, const GameType &, const Filesystem::AbsolutePath & systemFile);
        virtual ~Game();

        //! Runs everything
        virtual void run();

        /* just start a training match */
        static void startTraining(const std::string & player1, const std::string & player2, const std::string & stage);
    private:
        void doArcade(const Bitmap &);
        void doTraining(const Bitmap & bmp);
        void doVersus(const Bitmap &);
        void doWatch(const Bitmap &);

        //! Player type
        PlayerType playerType;
        //! Game type
        GameType gameType;
        //! System file
        Filesystem::AbsolutePath systemFile;
        //! Motif Base Directory
        Filesystem::AbsolutePath motifDirectory;
};

}

#endif

