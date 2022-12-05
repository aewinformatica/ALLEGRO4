#ifdef HAVE_NETWORKING

#include "util/bitmap.h"
#include "util/trans-bitmap.h"
#include "client.h"
#include "input/keyboard.h"
#include "input/input-map.h"
#include "input/input-manager.h"
#include "input/text-input.h"
#include "init.h"
#include "globals.h"
#include "util/funcs.h"
#include "util/font.h"
#include "util/file-system.h"
#include "level/utils.h"
#include "factory/font_render.h"
#include "factory/object_factory.h"
#include "factory/heart_factory.h"
#include "game/world.h"
#include "music.h"
#include "object/character.h"
#include "object/player.h"
#include "object/network_character.h"
#include "object/network_player.h"
#include "network_world_client.h"
#include "game/game.h"
#include "game/mod.h"
#include "exceptions/exception.h"
#include "loading.h"
#include "chat_client.h"
#include "network.h"
#include "configuration.h"
#include "util/system.h"
#include <string>
#include <sstream>

using namespace std;

namespace Network{

static bool uniqueId( const vector< Paintown::Object * > & objs, Paintown::Object::networkid_t id ){
    for ( vector< Paintown::Object * >::const_iterator it = objs.begin(); it != objs.end(); it++ ){
        Paintown::Object * o = *it;
        if ( o->getId() == id ){
            return false;
        }
    }
    return true;
}

/* send ok to server, get an ok back */
static void waitForServer(Socket socket){
    Message ok;
    ok << World::OK;
    ok.send( socket );
    Global::debug( 1 ) << "Sent ok " << endl;
    Message ok_ack(socket);
    int type;
    ok_ack >> type;
    if (type != World::OK){
        Global::debug(0) << "Did not receive ok from the server, instead received " << type << ". Badness may ensure" << endl;
    }
    Global::debug( 1 ) << "Received ok " << endl;
}

static void sendDummy(Socket socket){
    Message dummy;
    dummy.id = 0;
    dummy << World::IGNORE_MESSAGE;
    dummy.send( socket );
}

static void sendQuit(Socket socket){
    Message quit;
    quit.id = 0;
    quit << World::QUIT;
    quit.send(socket);
}

/* select a player through the usual select screen then tell the server
 * about our existence and get back an id for the player
 */
static Paintown::Player * createNetworkPlayer(Socket socket){
    class Context: public Loader::LoadingContext {
    public:
        Context(Socket socket, Level::LevelInfo & info, const Filesystem::AbsolutePath & playerPath, int remap):
        socket(socket),
        info(info),
        playerPath(playerPath),
        remap(remap),
        player(NULL){
        }

        Socket socket;
        Level::LevelInfo & info;
        const Filesystem::AbsolutePath & playerPath;
        int remap;
        Paintown::Player * player;

        virtual void load(){
            Global::info("Create player " + playerPath.path());
            player = new Paintown::Player(playerPath);
            player->setMap(remap);
            player->ignoreLives();
            Filesystem::RelativePath cleanPath = Filesystem::cleanse(playerPath);

            Global::info("Notify server");
            /* send the path of the chosen player */
            Message create;
            create << World::CREATE_CHARACTER;
            create.path = cleanPath.path();
            create.send(socket);

            Global::info("Waiting for id from server");
            /* get the id from the server */
            Message myid(socket);
            int type;
            int alliance;
            myid >> type;
            Paintown::Object::networkid_t client_id = (Paintown::Object::networkid_t) -1;
            if (type == World::SET_ID){
                myid >> client_id >> alliance;
                player->setId(client_id);
                player->setAlliance(alliance);
                Global::debug(1) << "Client id is " << client_id << endl;
            } else {
                Global::debug(0) << "Bogus message, expected SET_ID(" << World::SET_ID << ") got " << type << endl;
            }
            Global::info("Received an id");
        }
    };

    /* TODO: get the info from the server */
    Level::LevelInfo info;

    /* remap will be modified by the selectPlayer method */
    int remap = 0;
    Filesystem::AbsolutePath playerPath = Paintown::Mod::getCurrentMod()->selectPlayer("Pick a player", info, remap);

    Context context(socket, info, playerPath, remap);
    Loader::loadScreen(context, info);
    return context.player;
}

/* TODO: simplify this code */
static void playGame(Socket socket){
    // Util::Thread::Id loadingThread;
    try{
        Paintown::Player * player = createNetworkPlayer(socket);
        vector<Paintown::Object *> players;
        players.push_back(player);

        map<Paintown::Object::networkid_t, string> clientNames;

        bool done = false;
        while (! done){
            class Dispatcher: public Loader::LoadingContext {
            public:
                Dispatcher(vector<Paintown::Object*> & players,
                           Paintown::Player * player,
                           map<Paintown::Object::networkid_t, string> & clientNames,
                           bool & done,
                           Socket socket):
                players(players),
                player(player),
                clientNames(clientNames),
                done(done),
                socket(socket),
                play(false),
                world(NULL){
                }

                vector<Paintown::Object*> & players;
                Paintown::Player * player;
                map<Paintown::Object::networkid_t, string> & clientNames;
                bool & done;
                Socket socket;
                bool play;

                NetworkWorldClient * world;

                virtual ~Dispatcher(){
                    delete world;
                }

                void createCharacter(Message & next){
                    Paintown::Object::networkid_t id;
                    int alliance;
                    next >> id >> alliance;
                    if (uniqueId(players, id)){
                        Global::debug(1) << "Create a new network player id " << id << " alliance " << alliance << endl;
                        Global::info("Create character " + next.path);
                        Paintown::Character * c = new Paintown::NetworkPlayer(Filesystem::find(Filesystem::RelativePath(next.path)), alliance);
                        c->setId(id);
                        ((Paintown::NetworkCharacter *)c)->alwaysShowName();
                        players.push_back(c);
                    }

                }

                void clientInfo(Message & next){
                    Paintown::Object::networkid_t id;
                    next >> id;
                    string name = next.path;
                    clientNames[id] = name;
                    Global::info("Player " + name);
                }

                void loadLevel(Message & next){
                    Filesystem::AbsolutePath level = Filesystem::find(Filesystem::RelativePath(next.path));
                    /* TODO: handle LoadException here */
                    this->world = new NetworkWorldClient(socket, players, level, player->getId(), clientNames);
                    Music::changeSong();

                    Global::info("Waiting for ok from server");
                    waitForServer(socket);

                    this->world->startMessageHandler();
                    play = true;
                }

                void gameOver(){
                    done = true;
                }

                virtual void load(){
                    while (!done && !play){
                        Message next(socket);
                        int type;
                        next >> type;
                        switch (type){
                            case World::CREATE_CHARACTER : {
                                createCharacter(next);
                                break;
                            }
                            case World::CLIENT_INFO : {
                                clientInfo(next);
                                break;
                            }
                            case World::LOAD_LEVEL : {
                                loadLevel(next);
                                break;
                            }
                            /* thats all folks! */
                            case World::GAME_OVER : {
                                gameOver();
                                break;
                            }
                        }
                    }
                }
            };

            Dispatcher dispatch(players, player, clientNames, done, socket);
            Level::LevelInfo info;
            Loader::loadScreen(dispatch, info);

            if (dispatch.play){
                NetworkWorldClient & world = *dispatch.world;
                try{
                    vector<Paintown::Object*> xplayers;
                    bool forceQuit = ! Game::playLevel(world, xplayers);

                    ObjectFactory::destroy();
                    HeartFactory::destroy();

                    if (forceQuit){
                        Global::debug(1, __FILE__) << "Force quit" << endl;
                        sendQuit(socket);
                        /* After quit is sent the socket will be closed
                         * by someone later on. The input handler in the client
                         * world will throw a network exception and then
                         * the thread will die.
                         */
                        done = true;
                    } else {
                        Global::debug(1) << "Stop running client world" << endl;
                        /* this dummy lets the server message handler
                         * stop running. its currently blocked waiting for
                         * a message to come through.
                         */
                        sendDummy(socket);
                        world.stopRunning();
                        Global::debug(1) << "Wait for server " << endl;
                        /* then wait for a barrier */
                        waitForServer(socket);
                    }
                } catch ( const Exception::Return & e ){
                    /* do we need to close the socket here?
                     * when this function returns the socket will be
                     * close anyway.
                     */
                    Network::close(socket);
                }
            }
        }

        for (vector<Paintown::Object*>::iterator it = players.begin(); it != players.end(); it++){
            delete *it;
        }
    } catch ( const LoadException & le ){
        Global::debug(0, "client") << "Load exception: " + le.getTrace() << endl;
    }
}

static void drawBox( const Bitmap & area, const Bitmap & copy, const string & str, const Font & font, bool hasFocus ){
    copy.Blit( area );
    Bitmap::transBlender( 0, 0, 0, 192 );
    area.drawingMode( Bitmap::MODE_TRANS );
    area.rectangleFill( 0, 0, area.getWidth(), area.getHeight(), Bitmap::makeColor( 0, 0, 0 ) );
    area.drawingMode( Bitmap::MODE_SOLID );
    int color = Bitmap::makeColor( 255, 255, 255 );
    if (hasFocus){
        color = Bitmap::makeColor( 255, 255, 0 );
    }
    area.rectangle( 0, 0, area.getWidth() - 1, area.getHeight() - 1, color );
    font.printf( 1, 0, Bitmap::makeColor( 255, 255, 255 ), area, str, 0 );
}

static char lowerCase( const char * x ){
	if ( x[0] >= 'A' && x[0] <= 'Z' ){
		return x[0] - 'A' + 'a';
	}
	return x[0];
}

/*
static bool handleNameInput( string & str, const vector< int > & keys ){
	bool cy = false;
	for ( vector< int >::const_iterator it = keys.begin(); it != keys.end(); it++ ){
		const int & key = *it;
		if ( str.length() < 16 && Keyboard::isAlpha( key ) ){
			str += lowerCase( Keyboard::keyToName( key ) );
			cy = true;
		} else if ( key == Keyboard::Key_BACKSPACE ){
			if ( str != "" ){
				str = str.substr( 0, str.length() - 1 );
				cy = true;
			}
		}
	}
	return cy;
}
*/

/*
static bool handleHostInput( string & str, const vector< int > & keys ){
	bool cy = false;
	for ( vector< int >::const_iterator it = keys.begin(); it != keys.end(); it++ ){
		const int & key = *it;
		if ( Keyboard::isAlpha( key ) || key == Keyboard::Key_STOP ){
			str += lowerCase( Keyboard::keyToName( key ) );
			cy = true;
		} else if ( key == Keyboard::Key_BACKSPACE ){
			if ( str != "" ){
				str = str.substr( 0, str.length() - 1 );
				cy = true;
			}
		}
	}
	return cy;
}
*/

/*
static bool handlePortInput( string & str, const vector< int > & keys ){
	bool cy = false;
	for ( vector< int >::const_iterator it = keys.begin(); it != keys.end(); it++ ){
		const int & key = *it;
		if ( Keyboard::isNumber( key ) || key == Keyboard::Key_STOP ){
			str += Keyboard::keyToName( key );
			cy = true;
		} else if ( key == Keyboard::Key_BACKSPACE ){
			if ( str != "" ){
				str = str.substr( 0, str.length() - 1 );
			cy = true;
			}
		}
	}
	return cy;
}
*/

static void popup( Bitmap & work, const Font & font, const string & message ){
    int length = font.textLength( message.c_str() ) + 20; 
    // Bitmap area( *Bitmap::Screen, GFX_X / 2 - length / 2, 220, length, font.getHeight() * 3 );
    Bitmap area( work, GFX_X / 2 - length / 2, 220, length, font.getHeight() * 3 );
    Bitmap::transBlender( 0, 0, 0, 128 );
    // area.drawingMode( Bitmap::MODE_TRANS );
    area.translucent().rectangleFill( 0, 0, area.getWidth(), area.getHeight(), Bitmap::makeColor( 64, 0, 0 ) );
    // area.drawingMode( Bitmap::MODE_SOLID );
    int color = Bitmap::makeColor( 255, 255, 255 );
    area.rectangle( 0, 0, area.getWidth() - 1, area.getHeight() - 1, color );
    font.printf( 10, area.getHeight() / 2, Bitmap::makeColor( 255, 255, 255 ), area, message, 0 );
    work.BlitToScreen();
}

static const char * getANumber(){
    switch (Util::rnd(10)){
        case 0 : return "0";
        case 1 : return "1";
        case 2 : return "2";
        case 3 : return "3";
        case 4 : return "4";
        case 5 : return "5";
        case 6 : return "6";
        case 7 : return "7";
        case 8 : return "8";
        case 9 : return "9";
        default : return "0";
    }
}

static const char * propertyLastClientName = "network:last-client-name";
static const char * propertyLastClientHost = "network:last-client-host";
static const char * propertyLastClientPort = "network:last-client-port";

enum ClientActions{
    Next,
    Back,
    Quit,
    Action
};

static void runClient(const string & name, const string & host, const string & port, InputMap<ClientActions> & input){
    Configuration::setStringProperty(propertyLastClientName, name);
    Configuration::setStringProperty(propertyLastClientHost, host);
    Configuration::getStringProperty(propertyLastClientPort, port);
    istringstream stream(port);
    int portNumber;
    stream >> portNumber;
    Network::Socket socket = Network::connect(host, portNumber);
    ChatClient chat(socket, name);
    InputManager::waitForRelease(input, Action);
    chat.run();
    if (chat.isFinished()){
        playGame(socket);
    }
    Network::close(socket);
}

enum Focus{
    Name, Host, Port, Connect, FocusBack
};

struct FocusBundle{
    FocusBundle(Focus & focus, TextInput & name, TextInput & port, TextInput & host):
    focus(focus),
    name(name),
    port(port),
    host(host){
    }

    Focus & focus;
    TextInput & name;
    TextInput & port;
    TextInput & host;
};

static void nextFocus(void * stuff){
    FocusBundle * focus = (FocusBundle*) stuff;
    focus->name.disable();
    focus->port.disable();
    focus->host.disable();
    switch (focus->focus){
        case Name: focus->focus = Host; focus->host.enable(); break;
        case Host: focus->focus = Port; focus->port.enable(); break;
        case Port: focus->focus = Connect; break;
        case Connect: focus->focus = FocusBack; break;
        case FocusBack: focus->focus = Name; focus->name.enable(); break;
        default: focus->focus = Host; focus->host.enable(); break;
    }
}

static void previousFocus(void * stuff){
    FocusBundle * focus = (FocusBundle*) stuff;
    focus->name.disable();
    focus->host.disable();
    focus->port.disable();
    switch (focus->focus){
        case Name: focus->focus = FocusBack; break;
        case Host: focus->focus = Name; focus->name.enable(); break;
        case Port: focus->focus = Host; focus->host.enable(); break;
        case Connect: focus->focus = Port; focus->port.enable(); break;
        case FocusBack: focus->focus = Connect; break;
        default: focus->focus = Host; focus->host.enable(); break;
    }
}

static void doQuit(void * stuff){
    throw Exception::Return(__FILE__, __LINE__);
}

void networkClient(){
    Bitmap background(Global::titleScreen().path());
    Global::speed_counter = 0;
    
    InputMap<ClientActions> input;
    input.set(Keyboard::Key_TAB, 0, true, Next);
    input.set(Keyboard::Key_DOWN, 0, true, Next);
    input.set(Keyboard::Key_UP, 0, true, Back);
    input.set(Keyboard::Key_ESC, 0, true, Quit);
    input.set(Keyboard::Key_ENTER, 0, true, Action);

    TextInput nameInput(Configuration::getStringProperty(propertyLastClientName, string("player") + getANumber() + getANumber()));
    TextInput hostInput(Configuration::getStringProperty(propertyLastClientHost, "localhost"));
    TextInput portInput(Configuration::getStringProperty(propertyLastClientPort, "7887"));

    const Font & font = Font::getFont(Global::DEFAULT_FONT, 20, 20 );

    Bitmap work(GFX_X, GFX_Y);

    Focus focus = Name;
    nameInput.enable();

    FocusBundle bundle(focus, nameInput, portInput, hostInput);

    nameInput.addBlockingHandle(Keyboard::Key_TAB, nextFocus, &bundle);
    portInput.addBlockingHandle(Keyboard::Key_TAB, nextFocus, &bundle);
    hostInput.addBlockingHandle(Keyboard::Key_TAB, nextFocus, &bundle);
    
    nameInput.addBlockingHandle(Keyboard::Key_DOWN, nextFocus, &bundle);
    portInput.addBlockingHandle(Keyboard::Key_DOWN, nextFocus, &bundle);
    hostInput.addBlockingHandle(Keyboard::Key_DOWN, nextFocus, &bundle);
    
    nameInput.addBlockingHandle(Keyboard::Key_UP, previousFocus, &bundle);
    portInput.addBlockingHandle(Keyboard::Key_UP, previousFocus, &bundle);
    hostInput.addBlockingHandle(Keyboard::Key_UP, previousFocus, &bundle);
    
    nameInput.addBlockingHandle(Keyboard::Key_ESC, doQuit, NULL);
    portInput.addBlockingHandle(Keyboard::Key_ESC, doQuit, NULL);
    hostInput.addBlockingHandle(Keyboard::Key_ESC, doQuit, NULL);

    bool done = false;
    bool draw = true;
    while ( ! done ){
        int think = Global::speed_counter;
        Global::speed_counter = 0;
        Focus oldFocus = focus;
        while (think > 0){
            InputManager::poll();
            think -= 1;

            vector<InputMap<ClientActions>::InputEvent> events = InputManager::getEvents(input);

            switch (focus){
                case Name: draw = draw || nameInput.doInput(); break;
                case Port: draw = draw || portInput.doInput(); break;
                case Host: draw = draw || hostInput.doInput(); break;
                default: break;
            }

            // draw = draw || nameInput.doInput() || portInput.doInput() || hostInput.doInput();

            for (vector<InputMap<ClientActions>::InputEvent>::iterator it = events.begin(); it != events.end() && !done; it++){
                const InputMap<ClientActions>::InputEvent & event = *it;
                if (!event.enabled){
                    continue;
                }

                if (event[Next]){
                    nextFocus(&bundle);
                }

                if (event[Back]){
                    previousFocus(&bundle);
                }

                if (event[Quit]){
                    throw Exception::Return(__FILE__, __LINE__);
                }

                if (event[Action]){
                    switch (focus){
                        case Name:
                        case Host:
                        case Port: break;
                        case Connect: {
                            done = true;
                            try{
                                runClient(nameInput.getText(), hostInput.getText(), portInput.getText(), input);
                            } catch ( const NetworkException & e ){
                                popup( work, font, e.getMessage() );
                                InputManager::waitForRelease(input, Action);
                                InputManager::waitForPress(input, Action);
                                InputManager::waitForRelease(input, Action);
                                /*
                                   keyboard.wait();
                                   keyboard.readKey();
                                   */
                                /*
                                   Global::showTitleScreen();
                                   font.printf( 20, min_y - font.getHeight() * 3 - 1, Bitmap::makeColor( 255, 255, 255 ), *Bitmap::Screen, "Name", 0 );
                                   font.printf( 20, min_y - font.getHeight() - 1, Bitmap::makeColor( 255, 255, 255 ), *Bitmap::Screen, "Host", 0 );
                                   font.printf( 20, min_y + font.getHeight() * 2 - font.getHeight() - 1, Bitmap::makeColor( 255, 255, 255 ), *Bitmap::Screen, "Port", 0 );
                                   font.printf( 20, 20, Bitmap::makeColor( 255, 255, 255 ), *Bitmap::Screen, "Press TAB to cycle the next input", 0 );
                                   */
                                done = false;
                                draw = true;
                                think = 0;
                            }
                            break;
                        }
                        case FocusBack : done = true; break;
                    }
                }

            }

            draw = draw || oldFocus != focus;
        }

        if (draw){
            draw = false;

            background.Blit(work);

            const int inputBoxLength = font.textLength( "a" ) * 40;
            const int min_y = 140;

            font.printf( 20, min_y - font.getHeight() * 3 - 1, Bitmap::makeColor( 255, 255, 255 ), work, "Your name", 0 );
            Bitmap nameBox( work, 20, min_y - font.getHeight() * 2, inputBoxLength, font.getHeight() );
            Bitmap copyNameBox( nameBox.getWidth(), nameBox.getHeight() );
            nameBox.Blit(copyNameBox);

            font.printf( 20, min_y - font.getHeight() - 1, Bitmap::makeColor( 255, 255, 255 ), work, "Host (IP address or name)", 0 );
            Bitmap hostBox(work, 20, min_y, inputBoxLength, font.getHeight());
            Bitmap copyHostBox( hostBox.getWidth(), hostBox.getHeight() );
            hostBox.Blit(copyHostBox);

            font.printf( 20, min_y + font.getHeight() * 2 - font.getHeight() - 1, Bitmap::makeColor( 255, 255, 255 ), work, "Network Host Port", 0 );
            Bitmap portBox(work, 20, min_y + font.getHeight() * 2, inputBoxLength, font.getHeight());
            Bitmap copyPortBox( portBox.getWidth(), portBox.getHeight() );
            portBox.Blit(copyPortBox);

            font.printf( 20, 20, Bitmap::makeColor( 255, 255, 255 ), work, "Press TAB to cycle the next input", 0 );

            int focusColor = Bitmap::makeColor( 255, 255, 0 );
            int unFocusColor = Bitmap::makeColor( 255, 255, 255 );

            drawBox(nameBox, copyNameBox, nameInput.getText(), font, focus == Name);
            drawBox(hostBox, copyHostBox, hostInput.getText(), font, focus == Host);
            drawBox(portBox, copyPortBox, portInput.getText(), font, focus == Port);
            font.printf( 20, min_y + font.getHeight() * 5, focus == Connect ? focusColor : unFocusColor, work, "Connect", 0 );
            font.printf( 20, min_y + font.getHeight() * 6 + 5, focus == FocusBack ? focusColor : unFocusColor, work, "Back", 0 );

            work.BlitToScreen();
        }

        while (Global::speed_counter == 0){
            Util::rest(1);
        }
    }
}

}

#endif
