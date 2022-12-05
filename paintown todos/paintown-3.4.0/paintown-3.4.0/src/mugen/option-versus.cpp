#include "util/bitmap.h"
#include "options.h"
#include "game.h"
#include "menu.h"
#include "config.h"

using namespace std;
using namespace Mugen;

OptionVersus::OptionVersus( const std::string &name ) throw( LoadException ){
    if (name.empty()){
	throw LoadException(__FILE__, __LINE__, "No name given to versus");
    }
    this->setText(name);
}

OptionVersus::~OptionVersus(){
	// Nothing
}

void OptionVersus::executeOption(const Mugen::PlayerType & player, bool &endGame){
    /* Get motif system.def */
    Mugen::Game versus(player, Mugen::Versus, Mugen::Data::getInstance().getFileFromMotif(Mugen::Data::getInstance().getMotif()));;
    versus.run();
}
