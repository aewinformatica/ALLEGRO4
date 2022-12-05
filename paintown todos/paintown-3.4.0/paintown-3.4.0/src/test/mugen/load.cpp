#ifdef USE_ALLEGRO
#include <allegro.h>
#endif
#ifdef USE_SDL
#include <SDL/SDL.h>
#endif

#include <iostream>
#include "util/file-system.h"
#include "mugen/character.h"
#include "mugen/exception.h"
#include "mugen/parse-cache.h"
#include "util/timedifference.h"
#include "util/bitmap.h"

/*
#include <sstream>
#include <unistd.h>
*/

using namespace std;

/*
static int getPid(){
    return getpid();
}

static void runPmap(int pid){
    ostringstream out;
    out << "pmap " << pid;
    system(out.str().c_str());
}

static void showMemory(){
    runPmap(getPid());
}
*/

static int load(const char * path){
    // showMemory();
    for (int i = 0; i < 5; i++){
        try{
            TimeDifference diff;
            diff.startTime();
            Global::debug(0) << "Loading " << path << endl;
            Mugen::Character kfm(Filesystem::find(Filesystem::RelativePath(path)));
            kfm.load();
            diff.endTime();
            Global::debug(0, "test") << diff.printTime("Success! Took") << endl;
            /*
               int * x = new int[1 << 21];
               delete[] x;
               */
        } catch (const MugenException & e){
            Global::debug(0, "test") << "Test failure!: " << e.getReason() << endl;
            return 1;
        } catch (const Filesystem::NotFound & e){
            Global::debug(0, "test") << "Test failure! Couldn't find a file: " << e.getTrace() << endl;
            return 1;
        }
    }
    return 0;
    // showMemory();
}

int main(int argc, char ** argv){
#ifdef USE_ALLEGRO
    install_allegro(SYSTEM_NONE, &errno, atexit);
    set_color_depth(16);
    set_color_conversion(COLORCONV_NONE);
#elif USE_SDL
    SDL_Init(SDL_INIT_VIDEO);
    Bitmap::setFakeGraphicsMode(640, 480);
#endif

    Global::setDebug(1);
    Mugen::ParseCache cache;

    int die = 0;
    if (argc < 2){
        die = load("mugen/chars/kfm/kfm.def");
    } else {
        die = load(argv[1]);
    }

#ifdef USE_SDL
    SDL_Quit();
#endif

    // for (int i = 0; i < 3; i++){
      // }
    return die;
}
#ifdef USE_ALLEGRO
END_OF_MAIN()
#endif
