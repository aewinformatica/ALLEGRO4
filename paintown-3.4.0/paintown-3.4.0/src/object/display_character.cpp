#include "globals.h"
#include "character.h"
#include "display_character.h"
#include "draw-effect.h"
#include "util/thread.h"
#include "util/load_exception.h"
#include "util/token.h"
#include "util/token_exception.h"
#include "util/tokenreader.h"
#include "util/funcs.h"
#include "util/file-system.h"
#include "animation.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

namespace Paintown{

DisplayCharacter::DisplayCharacter(const string & path):
Character(ALLIANCE_NONE),
path(path),
loaded(false){
    setName(Filesystem::removeExtension(Filesystem::stripDir(path)));
    /* throws load-exception if the file can't be read */
    Util::Thread::initializeLock(&load_lock);
    // TokenReader reader(path);
}

void DisplayCharacter::load(){
    Global::debug(1) << "Loading " << path << endl;
    TokenReader reader( path );
    try{
        Token * head = reader.readToken();
        if ( *head != "character" ){
            throw LoadException(__FILE__, __LINE__, "First token is not 'character' in " + path );
        }

        // map<string, Filesystem::AbsolutePath> remaps;

        TokenView view = head->view();
        const Token * current;
        while (view.hasMore()){
            view >> current;
            if ( *current == "name" ){
                string n;
                current->view() >> n;
                setName( n );
            } else if ( *current == "anim" ){

                const Token * name = current->findToken("_/name");
                if (name == NULL){
                    /* screw it */
                    continue;
                }
                // current->resetToken();
                string xname;
                name->view() >> xname;
                /* only care about the 'idle' animation */
                if ( xname == "idle" ){
                    // name->resetToken();
                    Animation * ani = new Animation(current, this);

                    if ( getMovement( "idle" ) != NULL ){
                        delete getMovement( "idle" );
                    }

                    setMovement( ani, "idle" );
                }
            } else if ( *current == "remap" ){
                string first;
                string second;
                current->view() >> first >> second;
                if (newRemap(first, second)){
                    addRemap(new Remap(Filesystem::RelativePath(first), Filesystem::RelativePath(second), mapper[0]));
                }
                // remaps[Filesystem::find(Filesystem::RelativePath(second)).path()] = Filesystem::find(Filesystem::RelativePath(first));
            }
        }

        /*
        for ( map<string, Filesystem::AbsolutePath>::iterator it = remaps.begin(); it != remaps.end(); it++ ){
            const string & x1 = (*it).first;
            const Filesystem::AbsolutePath & alter = (*it).second;
            reMap(alter.path(), x1, getMapper().size() );
        }
        */
    } catch (const TokenException & ex){
        // Global::debug(0) << "Could not read " << path << " : " << ex.getReason() << endl;
        throw LoadException(__FILE__, __LINE__, ex, "Could not open character file: " + path );
    } catch (const Filesystem::NotFound & ex){
        throw LoadException(__FILE__, __LINE__, ex, "Could not load character " + path);
    }

    if ( getMovement( "idle" ) == NULL ){
        throw LoadException(__FILE__, __LINE__, "No 'idle' animation given for " + path );
    }

    animation_current = getMovement( "idle" );
    animation_current->Act();

    effects.push_back(new DrawNormalEffect(this));
}

void DisplayCharacter::loadDone(){
    Util::Thread::acquireLock(&load_lock);
    loaded = true;
    Util::Thread::releaseLock(&load_lock);
}

bool DisplayCharacter::isLoaded(){
    bool ok = false;
    Util::Thread::acquireLock(&load_lock);
    ok = loaded;
    Util::Thread::releaseLock(&load_lock);
    return ok;
}

DisplayCharacter::~DisplayCharacter(){
}

DisplayCharacterLoader::DisplayCharacterLoader(const vector<DisplayCharacter*> & characters):
forceQuit(false){
    this->characters = characters;
    Util::Thread::initializeLock(&data_lock);
}

void DisplayCharacterLoader::load(){
    Global::debug(1) << "Starting display character loader" << endl;
    while (!done()){
        DisplayCharacter * character = nextCharacter();
        try{
            character->load();
            character->loadDone();
        } catch (const LoadException & le){
            Global::debug(0) << "Problem loading character: " << le.getTrace() << endl;
        }
    }
    Global::debug(1) << "Character display loader done" << endl;
}

bool DisplayCharacterLoader::done(){
    bool result = false;
    Util::Thread::acquireLock(&data_lock);
    result = forceQuit || (characters.size() == 0);
    Util::Thread::releaseLock(&data_lock);
    return result;
}

void DisplayCharacterLoader::stop(){
    Util::Thread::acquireLock(&data_lock);
    forceQuit = true;
    Util::Thread::releaseLock(&data_lock);
}

DisplayCharacter * DisplayCharacterLoader::nextCharacter(){
    DisplayCharacter * result = NULL;
    Util::Thread::acquireLock(&data_lock);
    result = characters.front();
    characters.erase(characters.begin());
    Util::Thread::releaseLock(&data_lock);
    return result;
}

/* put the character in the front of the list if it has yet to be loaded */
void DisplayCharacterLoader::update(DisplayCharacter* character){
    Util::Thread::acquireLock(&data_lock);
    bool found = false;
    for (vector<DisplayCharacter*>::iterator it = characters.begin(); it != characters.end(); ){
        DisplayCharacter * who = *it;
        if (who == character){
            found = true;
            it = characters.erase(it);
            break;
        } else {
            it++;
        }
    }
    if (found){
        characters.insert(characters.begin(), character);
    }
    Util::Thread::releaseLock(&data_lock);
}

DisplayCharacterLoader::~DisplayCharacterLoader(){
}

}
