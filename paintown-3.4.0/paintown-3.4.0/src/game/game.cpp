#include "util/bitmap.h"
#include "util/trans-bitmap.h"

#include "game.h"
#include "music.h"
#include "util/funcs.h"
#include "util/font.h"
#include "menu/menu.h"
#include "configuration.h"
#include "object/object.h"
#include "object/character.h"
#include "object/player.h"
#include "factory/object_factory.h"
#include "factory/heart_factory.h"
#include "level/utils.h"
#include "factory/font_render.h"
#include "util/token.h"
#include "util/tokenreader.h"
#include "input/keyboard.h"
#include "globals.h"
#include "script/script.h"
#include "exceptions/shutdown_exception.h"
#include "exceptions/exception.h"
#include "util/file-system.h"
#include "util/system.h"
#include "util/events.h"
#include "loading.h"
#include "network/network.h"
#include "world.h"
#include "adventure_world.h"
#include "console.h"
#include "input/input-manager.h"
#include "input/input-map.h"
#include "versus_world.h"
#include "init.h"
#include "util/thread.h"
#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;

// static int LAZY_KEY_DELAY = 300;
static bool show_loading_screen = true;

namespace Game{

struct Background{
    string path;
    int z;

    Background():z(0){}
};
	
static double startingGameSpeed(){
    return 1.0;
}

/*
static void stopLoading(Util::Thread::Id thread){
    if (show_loading_screen){
        Loader::stopLoading(thread);
    }
}

static void startLoading(Util::Thread::Id * thread, const Level::LevelInfo & info ){
    if (show_loading_screen){
        Loader::startLoading(thread, (void*) &info);
    }
}
*/

static Network::Message removeMessage(int id){
    Network::Message message;

    message.id = 0;
    message << World::REMOVE;
    message << id;

    return message;
}

static vector<Background> readBackgrounds( const Filesystem::AbsolutePath & path ){
    vector<Background> backgrounds;

    try{
        TokenReader reader( path.path() + "/bgs.txt" );
        Token * head = reader.readToken();

        if ( *head == "backgrounds" ){
            TokenView view = head->view();
            while (view.hasMore()){
                const Token * background;
                view >> background;
                if ( *background == "background" ){
                    Background b;
                    TokenView backgroundView = background->view();
                    for ( int i = 0; i < 2; i++ ){
                        const Token * next;
                        backgroundView >> next;
                        if ( *next == "path" ){
                            next->view() >> b.path;
                        } else if ( *next == "z" ){
                            next->view() >> b.z;
                        }
                    }
                    backgrounds.push_back(b);
                }
            }
        }

    } catch ( const TokenException & ex ){
        Global::debug( 0 ) << "Could not load " + path.path() + "/bgs.txt because " << ex.getTrace() << endl;
    }

    /*
       Background b1;
       b1.path = path + "/versus/bg1.png";
       b1.z = 420;
       backgrounds.push_back( b1 );
       */

    return backgrounds;
}

static string findNextFile( const char * name ){
    char buf[ 128 ];
    const char * extension = strchr( name, '.' );
    char first[ 128 ];
    strncpy( first, name, extension - name );
    first[ extension - name ] = '\0';
    unsigned int num = 0;
    sprintf( buf, "%s%u%s", first, num, extension );
    do{
        num += 1;
        sprintf( buf, "%s%u%s", first, num, extension );
        /* num != 0 prevents an infinite loop in the extremely
         * remote case that the user has 2^32 files in the directory
         */
    } while (num != 0 && Util::exists(buf));
    return string(buf);
}

static void drawHelp( const Font & font, int x, int y, int color, const Bitmap & buffer ){
    font.printf( x, y, color, buffer, "Controls", 0 );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Up: %s", 0,  Keyboard::keyToName( Configuration::config( 0 ).getUp() ) );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Down: %s", 0,  Keyboard::keyToName( Configuration::config( 0 ).getDown() ) );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Left: %s", 0,  Keyboard::keyToName( Configuration::config( 0 ).getLeft() ) );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Right: %s", 0,  Keyboard::keyToName( Configuration::config( 0 ).getRight() ) );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Jump: %s", 0,  Keyboard::keyToName( Configuration::config( 0 ).getJump() ) );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Attack1: %s", 0,  Keyboard::keyToName( Configuration::config( 0 ).getAttack1() ) );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Attack2: %s", 0,  Keyboard::keyToName( Configuration::config( 0 ).getAttack2() ) );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Attack3: %s", 0,  Keyboard::keyToName( Configuration::config( 0 ).getAttack3() ) );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Tab to hide/show minimap", 0 );
    y += font.getHeight() + 1;
    font.printf( x, y, color, buffer, "Press F1 to view this help", 0 );
}

namespace Game{
    enum Input{
        Screenshot,
        Slowdown,
        Speedup,
        NormalSpeed,
        ReloadLevel,
        KillAllHumans,
        Pause,
        MiniMaps,
        ShowHelp,
        ShowFps,
        Quit,
        Console,
    };
}

static void doTakeScreenshot(const Bitmap & work){
    string file = findNextFile("paintown-screenshot.bmp");
    Global::debug(2) << "Saved screenshot to " << file << endl;
    work.save(file);
}

/* returns false if players cannot be respawned due to running out of lives */
static bool respawnPlayers(const vector<Paintown::Object*> & players, World & world){
    for ( vector< Paintown::Object * >::const_iterator it = players.begin(); it != players.end(); it++ ){
        Paintown::Character * player = (Paintown::Character *) *it;
        if ( player->getHealth() <= 0 ){
            if ( player->spawnTime() == 0 ){
                player->deathReset();
                if ( player->getLives() == 0 ){
                    return false;
                }
                world.addMessage( removeMessage( player->getId() ) );
                world.addObject( player );
                world.addMessage( player->getCreateMessage() );
                world.addMessage( player->movedMessage() );
                world.addMessage( player->animationMessage() );
            }
        }
    }
    return true;
}

/* in-game menu */
static bool doMenu(const Bitmap & screen_buffer, const Token * data){
    // Menu::Menu menu(Filesystem::find(Filesystem::RelativePath("menu/in-game.txt")));
    Menu::Menu menu(data);
    Menu::Context context;
    /* use the current screen as the background */
    context.addBackground(screen_buffer);
    try{
        menu.run(context);
        /* im pretty sure there is no way to get the menu to return normally,
         * it should always throw an exception either because ESC was pressed
         * or because an option threw something
         */
        return false;
    } catch (const Exception::Quit & forceQuit){
        return true;
    } catch (const Exception::Return & back){
        return false;
    }
}

bool playLevel( World & world, const vector< Paintown::Object * > & players){
    Bitmap screen_buffer(GFX_X, GFX_Y);

    /* 150 pixel tall console */
    Console::Console console(150);
    {
        class CommandQuit: public Console::Command{
        public:
            CommandQuit(){
            }

            string act(){
                throw ShutdownException();
            }
        };

        class CommandMemory: public Console::Command {
        public:
            CommandMemory(){
            }

            string act(){
                ostringstream out;
                out << "Memory usage: " << Util::niceSize(System::memoryUsage()) << "\n";
                return out.str();
            }
        };

        class CommandHelp: public Console::Command{
        public:
            CommandHelp(){
            }

            string act(){
                ostringstream out;
                out << "quit - quit the game entirely" << "\n";
                out << "memory - current memory usage" << "\n";
                out << "help - this help menu";
                return out.str();
            }
        };

        console.addCommand("quit", new CommandQuit());
        console.addCommand("help", new CommandHelp());
        console.addCommand("memory", new CommandMemory());
    }
    // bool toggleConsole = false;
    // const int consoleKey = Keyboard::Key_TILDE;

    world.getEngine()->createWorld(world);

    // int game_time = 100;
    // Global::second_counter = 0;

    /* thrown when the player runs out of lives */
    class LoseException: Exception::Base {
    public:
        LoseException():
        Exception::Base(__FILE__, __LINE__){
        }
    };

    struct GameState{
        GameState():
            force_quit(false),
            helpTime(0),
            pressed(0),
            done(false),
            show_fps(false){
            }

        bool force_quit;
        double helpTime;
        int pressed;
        bool done;
        bool show_fps;
    };

    class Logic{
    public:
        Logic(const vector<Paintown::Object*> & players, World & world, Console::Console & console):
        runCounter(0),
        gameSpeed(startingGameSpeed()),
        players(players),
        helped(false),
        world(world),
        console(console){
            if (Global::getDebug() > 0){
                input.set(Keyboard::Key_MINUS_PAD, 2, false, Game::Slowdown);
                input.set(Keyboard::Key_PLUS_PAD, 2, false, Game::Speedup);
                input.set(Keyboard::Key_F4, 0, true, Game::ReloadLevel);
                input.set(Keyboard::Key_F8, 200, false, Game::KillAllHumans);
            }

            input.set(Keyboard::Key_P, 10, false, Game::Pause);
            input.set(Keyboard::Key_TAB, 10, false, Game::MiniMaps);
            input.set(Keyboard::Key_TILDE, 25, false, Game::Console);
            input.set(Keyboard::Key_ESC, 0, false, Game::Quit);
            input.set(Joystick::Quit, 0, false, Game::Quit);
            input.set(Keyboard::Key_F1, 0, false, Game::ShowHelp);
            input.set(Keyboard::Key_F9, 20, false, Game::ShowFps);
            input.set(Keyboard::Key_F12, 10, false, Game::Screenshot);
        }

        InputMap<Game::Input> input;
        double runCounter;
        double gameSpeed;
        const vector<Paintown::Object*> & players;
        bool helped;
        World & world;
        Util::EventManager eventManager;
        Console::Console & console;

        void doInput(GameState & state, bool & takeScreenshot, bool & draw){
            vector<InputMap<Game::Input>::InputEvent> events = InputManager::getEvents(input);

            bool pressed = false;
            for (vector<InputMap<Game::Input>::InputEvent>::iterator it = events.begin(); it != events.end(); it++){
                const InputMap<Game::Input>::InputEvent & event = *it;
                if (!event.enabled){
                    continue;
                }

                if (event[Game::ShowHelp]){
                    helped = true;
                    state.helpTime = state.helpTime < 260 ? 260 : state.helpTime;
                }

                if (event[Game::ShowFps]){
                    state.show_fps = ! state.show_fps;
                }

                if (event[Game::Console]){
                    console.toggle();
                }

                takeScreenshot = event[Game::Screenshot];

                if (event[Game::Pause]){
                    /*
                       paused = ! paused;
                       world.addMessage(paused ? pausedMessage() : unpausedMessage());
                       draw = true;
                     */
                    world.changePause();
                }

                if (event[Game::MiniMaps]){
                    world.drawMiniMaps( ! world.shouldDrawMiniMaps() );
                }

                /*
                   if ( key[ Keyboard::Key_F8 ] ){
                   world.killAllHumans( player );
                   }
                 */

                if ( Global::getDebug() > 0 ){
                    const double SPEED_INC = 0.02;
                    if (event[Game::Speedup]){
                        gameSpeed += SPEED_INC;
                        Global::debug( 3 ) << "Game speed " << gameSpeed << endl;
                    }

                    if (event[Game::Slowdown]){
                        gameSpeed -= SPEED_INC;
                        if ( gameSpeed < SPEED_INC ){
                            gameSpeed = SPEED_INC;
                        }
                        Global::debug( 3 ) << "Game speed " << gameSpeed << endl;
                    }

                    if (event[Game::NormalSpeed]){
                        gameSpeed = 1;
                        Global::debug( 3 ) << "Game speed " << gameSpeed << endl;
                    }

                    if (event[Game::ReloadLevel]){
                        try{
                            world.reloadLevel();
                            draw = true;
                        } catch ( const LoadException & le ){
                            Global::debug( 0 ) << "Could not reload world: " << le.getTrace() << endl;
                        }
                    }
                }

                state.force_quit |= event[Game::Quit];
            }

            if (!pressed){
                state.pressed = 0;
            } else {
                state.pressed += 1;
                if (state.pressed > 100){
                    state.pressed = 100;
                    state.helpTime = 260;
                }
            }

            try{
                console.doInput();
            } catch (const Exception::Return & r){
                state.force_quit = true;
            }
        }

        bool run(bool & takeScreenshot, GameState & state){
            bool draw = false;
            if (Global::speed_counter > 0){
                runCounter += world.ticks(Global::speed_counter * gameSpeed * Global::LOGIC_MULTIPLIER);
                Global::speed_counter = 0;

                InputManager::poll();
                while (runCounter >= 1.0){
                    // eventManager.run();
                    draw = true;
                    world.act();
                    console.act();
                    runCounter -= 1.0;

                    if (!respawnPlayers(players, world)){
                        throw LoseException();
                    }

                    if (state.helpTime > 0){
                        if (helped){
                            state.helpTime -= 2;
                        } else {
                            state.helpTime -= 0.5;
                        }
                    }
                }

                doInput(state, takeScreenshot, draw);
                
                state.done |= world.finished();
            }

            return draw;
        }

        void rest(){
            Util::rest(1);
            // InputManager::poll();
            // eventManager.run();
        }

        void waitForQuit(){
            InputManager::waitForRelease(input, Game::Quit);
        }
    };

    class Draw{
    public:
        Draw(Console::Console & console, World & world):
        console(console),
        world(world),
        /* the game graphics are meant for 320x240 and will be stretched
         * to fit the screen
         */
        work(320, 240),
        frames(0),
        second_counter(Global::second_counter),
        fps(Global::TICS_PER_SECOND){
        }

        Console::Console & console;
        World & world;
        Bitmap work;
        int frames;
        unsigned int second_counter;
        double fps;

        void updateFrames(){
            if (second_counter != Global::second_counter){
                int difference = Global::second_counter - second_counter;
                double alpha = 0.2;
                /* unlikely, but just in case */
                if (difference == 0){
                    difference = 1;
                }
                fps = (alpha * fps) + ((1 - alpha) * (double) frames / difference);
                // fps[fps_index] = (double) frames / (double) difference;
                // fps_index = (fps_index+1) % max_fps_index;
                second_counter = Global::second_counter;
                frames = 0;
            }

            frames += 1;
        }

        void run(const Bitmap & screen_buffer, const GameState & state, bool takeScreenshot){
            updateFrames();

            world.draw( &work );

            work.Stretch(screen_buffer);
            FontRender * render = FontRender::getInstance();
            render->render( &screen_buffer );

            const Font & font = Font::getFont(Global::DEFAULT_FONT, 20, 20 );

            if (state.helpTime > 0){
                int x = 100;
                int y = screen_buffer.getHeight() / 5;
                int color = Bitmap::makeColor( 255, 255, 255 );
                Bitmap::transBlender( 0, 0, 0, (int)(state.helpTime > 255 ? 255 : state.helpTime));
                drawHelp( font, x, y, color, screen_buffer.translucent());
            }

            if (state.show_fps){
                font.printf( screen_buffer.getWidth() - 120, 10, Bitmap::makeColor(255,255,255), screen_buffer, "FPS: %0.2f", 0, fps );
            }
            console.draw(screen_buffer);

            /* getX/Y move when the world is quaking */
            screen_buffer.BlitToScreen(world.getX(), world.getY());

            if (takeScreenshot){
                doTakeScreenshot(work);
            }

            work.clear();
        }

        void showScreenshots(const Bitmap & screen_buffer){
            work.clear();
            Sound snapshot(Filesystem::find(Filesystem::RelativePath("sounds/snapshot.wav")).path());
            for (deque<Bitmap*>::const_iterator it = world.getScreenshots().begin(); it != world.getScreenshots().end(); it++){
                Bitmap * shot = *it;
                int angle = Util::rnd(-6, 6);

                /*
                   int gap = 4;
                   int x = Util::rnd(work.getWidth() - 2 * work.getWidth() / gap) + work.getWidth() / gap;
                   int y = Util::rnd(work.getHeight() - 2 * work.getHeight() / gap) + work.getHeight() / gap;
                   double scale = 1.0 - log(world.getScreenshots().size()+1) / 9.0;
                   shot->greyScale().drawPivot(shot->getWidth() / 2, shot->getHeight() / 2, x, y, angle, scale, work);
                   */

                int x = work.getWidth() / 2;
                int y = work.getHeight() / 2;
                double scale = 0.9;
                shot->border(0, 1, Bitmap::makeColor(64,64,64));
                shot->greyScale().drawPivot(shot->getWidth() / 2, shot->getHeight() / 2, x, y, angle, scale, work);
                work.Stretch(screen_buffer);
                screen_buffer.BlitToScreen();
                snapshot.play();
                Util::restSeconds(1.5);
            }
            if (world.getScreenshots().size() > 0){
                Util::restSeconds(2);
            }

        }
    };
    
    TokenReader reader;
    Token * menuData = reader.readToken(Filesystem::find(Filesystem::RelativePath("menu/in-game.txt")).path());

    bool finish = true;
    Logic logic(players, world, console);
    Draw drawer(console, world);
    GameState state;
    // state.helpTime = helpTime;

    /* don't put anything after these variables and before the while loop */
    Global::speed_counter = 0;

    try{
        /* Main Loop! */
        while (!state.done && !state.force_quit){
            bool draw = false;
            bool takeScreenshot = false;
            draw = logic.run(takeScreenshot, state);

            if (draw){
                drawer.run(screen_buffer, state, takeScreenshot);
            }

            if (state.force_quit){
                state.force_quit = doMenu(screen_buffer, menuData);
            }

            while (Global::speed_counter < 1){
                logic.rest();
            }
        }

        if (!state.force_quit){
            drawer.showScreenshots(screen_buffer);
        }
    } catch (const LoseException & lose){
        fadeOut(screen_buffer, "You lose");
        finish = false;
    }

    world.getEngine()->destroyWorld(world);

    if (state.force_quit){
        logic.waitForQuit();
        finish = false;
    }

    return finish;
}

static string funnyGo(){
    switch (Util::rnd(10)){
        case 0 : return "Robofrance 99";
        case 1 : return "Is that a weasel?";
        case 2 : return "You are the insult master!";
        case 3 : return "Get to the choppaa!!!";
        case 4 : return "Ride or die";
        case 5 : return "Let the Dragon ride on the winds of time";
        case 6 : return "I LIKE TURTLES";
        default : return "Go!";
    }
}

static void initializePlayers(const vector<Paintown::Object*> & players){
    for (vector<Paintown::Object *>::const_iterator it = players.begin(); it != players.end(); it++){
        Paintown::Player * playerX = (Paintown::Player *) *it;
        playerX->setTrails(0, 0);
        playerX->setY(200);
        /* setMoving(false) sets all velocities to 0 */
        playerX->setMoving(false);
        /* but the player is falling so set it back to true */
        playerX->setMoving(true);

        playerX->setStatus(Paintown::Status_Falling);
    }
}

static void realGame(const vector<Util::Future<Paintown::Object*> * > & futurePlayers, const Level::LevelInfo & levelInfo, const string & level){
    class GameContext: public Loader::LoadingContext {
    public:
        GameContext(const vector<Util::Future<Paintown::Object*> * > & futurePlayers, const Filesystem::RelativePath & path):
        data(NULL),
        futurePlayers(futurePlayers),
        path(path),
        failed(NULL){
        }

        ~GameContext(){
            /* who will delete the players contained in the data? the futures
             * passed in as `futurePlayers'
             */
            delete data;
            delete failed;
        }

        virtual void failure(){
            if (failed){
                throw LoadException(*failed);
            }
        }

        virtual void load(){
            try{
                vector<Paintown::Object*> players;
                for (vector<Util::Future<Paintown::Object*>*>::const_iterator fit = futurePlayers.begin(); fit != futurePlayers.end(); fit++){
                    Util::Future<Paintown::Object*> * future = *fit;
                    players.push_back(future->get());
                }
                data = new Data(players, Filesystem::find(path));
            } catch (const LoadException & exception){
                failed = new LoadException(exception);
            }
        }

        World & getWorld(){
            if (data == NULL){
                throw LoadException(__FILE__, __LINE__, "World didn't load");
            }
            return data->world;
        }

        vector<Paintown::Object*> & getPlayers(){
            if (data == NULL){
                throw LoadException(__FILE__, __LINE__, "Players didn't load");
            }
            return data->players;
        }

        struct Data{
            Data(vector<Paintown::Object*> players, const Filesystem::AbsolutePath & path):
            world(players, path),
            players(players){
            }

            AdventureWorld world;
            vector<Paintown::Object*> players;
        };

        Data * data;
        vector<Util::Future<Paintown::Object*> * > futurePlayers;
        Filesystem::RelativePath path;
        LoadException * failed;
    };

    Global::info("Setting up world");
    GameContext context(futurePlayers, Filesystem::RelativePath(level));
    Loader::loadScreen(context, levelInfo);
    Global::info("World setup");
    Global::info(funnyGo());

    Music::changeSong();

    initializePlayers(context.getPlayers());

    bool gameState = playLevel(context.getWorld(), context.getPlayers());
    ObjectFactory::destroy();
    HeartFactory::destroy();

    if (! gameState){
        throw Exception::Return(__FILE__, __LINE__);
    }
}

void realGame(const vector<Util::Future<Paintown::Object*> * > & futurePlayers, const Level::LevelInfo & levelInfo){
    for (vector<string>::const_iterator it = levelInfo.getLevels().begin(); it != levelInfo.getLevels().end(); it++){
        realGame(futurePlayers, levelInfo, *it);
    }
}

#if 0
void realGame(const vector<Util::Future<Paintown::Object*> * > & futurePlayers, const Level::LevelInfo & levelInfo){

    /* disables buffer input on entry, enables buffering on function exit */
    /*
    class InputBuffer{
    public:
        InputBuffer(){
            InputManager::disableBufferInput();
        }

        ~InputBuffer(){
            InputManager::enableBufferInput();
        }
    };
    */

    // InputBuffer buffer;

    // Level::LevelInfo levelInfo = Level::readLevels( levelFile );

    // global_debug = true;

    int showHelp = 800;
    for ( vector<string>::const_iterator it = levelInfo.getLevels().begin(); it != levelInfo.getLevels().end(); it++ ){
        Util::Thread::Id loading_screen_thread;
        startLoading(&loading_screen_thread, levelInfo);

        bool gameState = false;
        try {
            // vector< Object * > players;
            // players.push_back( player );

            /*
               Global::debug(0) << "Memory debug loop" << endl;
               for (int i = 0; i < 1000; i++){
               World world( players, *it );
               ObjectFactory::destroy();
               HeartFactory::destroy();
               }
               */

            Global::info("Setting up world");
            vector<Paintown::Object*> players;
            for (vector<Util::Future<Paintown::Object*>*>::const_iterator fit = futurePlayers.begin(); fit != futurePlayers.end(); fit++){
                Util::Future<Paintown::Object*> * future = *fit;
                players.push_back(future->get());
            }

            AdventureWorld world(players, Filesystem::find(Filesystem::RelativePath(*it)));
            Global::info("World setup");
            Global::info(funnyGo());

            Music::pause();
            Music::fadeIn( 0.3 );
            Music::loadSong(Filesystem::getFiles(Filesystem::find(Filesystem::RelativePath("music/")), "*"));
            Music::play();

            for ( vector< Paintown::Object * >::const_iterator it = players.begin(); it != players.end(); it++ ){
                Paintown::Player * playerX = (Paintown::Player *) *it;
                playerX->setTrails(0, 0);
                playerX->setY(200);
                /* setMoving(false) sets all velocities to 0 */
                playerX->setMoving(false);
                /* but the player is falling so set it back to true */
                playerX->setMoving(true);

                playerX->setStatus(Paintown::Status_Falling);
            }

            stopLoading(loading_screen_thread);

            gameState = playLevel(world, players, showHelp);
            showHelp = 0;
        } catch ( const LoadException & le ){
            Global::debug( 0 ) << "Could not load " << *it << " because " << le.getTrace() << endl;
            /* if the level couldn't be loaded turn off
             * the loading screen
             */
            stopLoading( loading_screen_thread );
        } catch (const ShutdownException & se){
            stopLoading(loading_screen_thread);
            throw se;
        }

        ObjectFactory::destroy();
        HeartFactory::destroy();

        if ( ! gameState ){
            return;
        }

        // fadeOut( "Next level" );
    }

    /* fix.. */
    // fadeOut( "You win!" );
}
#endif

#if 0
/* use MenuGlobal::doLevelMenu instead */
const Level::LevelInfo selectLevelSet( const string & base ){
    return MenuGlobals::doLevelMenu(base, NULL);

        Bitmap background( Global::titleScreen() );
	// Bitmap::Screen->Blit( Global::titleScreen() );

	// Bitmap background( Util::getDataPath() + "/paintown-title.png" );
	int fontY = 20;
	const Font & font = Font::getFont( Filesystem::find(DEFAULT_FONT), 20, fontY );
	vector<string> possible = Util::getFiles( base + "/", "*.txt" );
        for ( vector<string>::iterator it = possible.begin(); it != possible.end(); it++ ){
            string & path = *it;
            /*
            path.erase(0, Util::getDataPath().length() + 1);
            */
            path = Filesystem::cleanse(path);
        }

	if ( possible.size() == 0 ){
		return "no-files!!!";
	}

        if (possible.size() == 1){
            return Filesystem::find(possible[0]);
        }

	/*
	for ( vector< string >::iterator it = possible.begin(); it != possible.end(); it++ ){
		string & s = *it;
		s.insert( 0, base + "/" );
	}
	*/
	int choose = 0;

	font.printf( 180, (int)(200 - fontY * 1.2), Bitmap::makeColor( 255, 255, 255 ), background, "Select a set of levels to play", 0 );
	for ( unsigned int i = 0; i < possible.size(); i++ ){
		int yellow = Bitmap::makeColor( 255, 255, 0 );
		int white = Bitmap::makeColor( 255, 255, 255 );
		unsigned int color = i == (unsigned) choose ? yellow : white;
		font.printf( 200, (int)(200 + i * fontY * 1.2), color, background, possible[ i ], 0 );
	}
        background.BlitToScreen();

	Keyboard key;
	bool done = false;

	key.setDelay( Keyboard::Key_UP, LAZY_KEY_DELAY );
	key.setDelay( Keyboard::Key_DOWN, LAZY_KEY_DELAY );
	Global::speed_counter = 0;

	key.wait();

	while ( ! done ){
		
		key.poll();
		bool draw = false;
		if ( Global::speed_counter > 0 ){
			double think = Global::speed_counter;

			while ( think > 0 ){
				think--;

				if ( key[ Keyboard::Key_UP ] ){
					draw = true;
					choose = (choose - 1 + possible.size()) % possible.size();
			}

				if ( key[ Keyboard::Key_DOWN ] ){
					draw = true;
					choose = (choose + 1 + possible.size()) % possible.size();
				}

				if ( key[ Keyboard::Key_ENTER ] ){
					return Filesystem::find(possible[ choose ]);
				}

				if ( key[ Keyboard::Key_ESC ] ){
					throw ReturnException();
				}
			}

			Global::speed_counter = 0;
		}

		if ( draw ){
			for ( unsigned int i = 0; i < possible.size(); i++ ){
				int yellow = Bitmap::makeColor( 255, 255, 0 );
				int white = Bitmap::makeColor( 255, 255, 255 );
				unsigned int color = i == (unsigned) choose ? yellow : white;
				font.printf( 200, (int)(200 + i * fontY * 1.2), color, background, possible[ i ], 0 );
			}
                        background.BlitToScreen();
		}
		
		while ( Global::speed_counter == 0 ){
			Util::rest( 1 );
			key.poll();
		}
	}

	return "nothing-selected";
}
#endif

void fadeOut( Bitmap & work, const string & message ){
    Bitmap dark( GFX_X, GFX_Y );
    dark.clear();
    Bitmap::transBlender( 0, 0, 0, 128 );

    dark.drawTrans( 0, 0, work );

    const Font & f = Font::getFont(Global::DEFAULT_FONT, 50, 50 );
    f.printf( 200, 200, Bitmap::makeColor( 255, 0, 0 ), work, message, 0 );
    work.BlitToScreen();

    Util::rest( 2000 );
}

static bool closeFloat(double a, double b){
    const double epsilon = 0.0001;
    return fabs(a-b) < epsilon;
}

/* TODO: deprecated */
#if 0
void playVersusMode( Paintown::Character * player1, Paintown::Character * player2, int round ){

	player1->setY( 0 );
	player2->setY( 0 );
	player1->setX( 0 );
	player2->setX( 400 );

	player1->setMaxHealth( 150 );
	player2->setMaxHealth( 150 );
	player1->setHealth( 150 );
	player2->setHealth( 150 );

	// Keyboard key;

	// key.setDelay( Keyboard::Key_P, 100 );

	bool done = false;
	bool paused = false;
	double runCounter = 0;
	double gameSpeed = startingGameSpeed();

	vector< Background > backgrounds;
	backgrounds = readBackgrounds(Filesystem::find(Filesystem::RelativePath("bgs/versus/")));

	Bitmap background( 640, 480 );
	int z = 400;
	if ( backgrounds.size() != 0 ){
		Background use = backgrounds[ Util::rnd( backgrounds.size() ) ];
		Bitmap b(Filesystem::find(Filesystem::RelativePath(use.path)).path());
		b.Stretch(background);
		z = use.z;
	}
	
	VersusWorld world( z, player1, player2 );
	
	Bitmap work( 640, 480 );
	// Bitmap work( GFX_X, GFX_Y );
	Bitmap screen_buffer( GFX_X, GFX_Y );

        Music::changeSong();
        /*
	Music::pause();
	Music::fadeIn( 0.3 );
	Music::loadSong( Util::getFiles( Filesystem::find("/music/"), "*" ) );
	Music::play();
        */

	int roundColors[ 120 ];
	int showRound = sizeof( roundColors ) / sizeof(int) - 1;
	Util::blend_palette( roundColors, 60, Bitmap::makeColor( 96, 0, 0 ), Bitmap::makeColor( 200, 0, 0 ) );
	Util::blend_palette( roundColors + 60, 60, Bitmap::makeColor( 255, 0, 0 ), Bitmap::makeColor( 96, 0, 0 ) );

	while ( ! done ){

		bool draw = false;
		// key.poll();

		if ( Global::speed_counter > 0 ){
			if ( ! paused ){
				runCounter += Global::speed_counter * gameSpeed * Global::LOGIC_MULTIPLIER;

				while ( runCounter >= 1.0 ){
					draw = true;
					world.act();
					runCounter -= 1.0;

					if ( player1->getHealth() <= 0 || player2->getHealth() <= 0 ){
						if ( player1->getHealth() <= 0 && player2->getHealth() > 0 ){
							fadeOut( screen_buffer, "Player 2 wins!" );
						} else if ( player1->getHealth() > 0 && player2->getHealth() <= 0 ){
							fadeOut( screen_buffer, "Player 1 wins!" );
						} else {
							fadeOut( screen_buffer, "Draw!" );
						}
						return;
					}
				}
			}

                        /* FIXME */
                        /*
			const double SPEED_INC = 0.02;
			if ( key[Keyboard::Key_MINUS_PAD] ){
				gameSpeed -= SPEED_INC;
				if ( gameSpeed < SPEED_INC ){
					gameSpeed = SPEED_INC;
				}
				Global::debug( 3 ) << "Game speed " << gameSpeed << endl;
			}

			if ( key[ Keyboard::Key_ESC ] ){
				throw Exception::Return(__FILE__, __LINE__);
			}

			if ( key[ Keyboard::Key_P ] ){
				paused = ! paused;
				draw = true;
			}

			if ( key[ Keyboard::Key_PLUS_PAD ] ){
				gameSpeed += SPEED_INC;
				Global::debug( 3 ) << "Game speed " << gameSpeed << endl;
			}

			if ( key[ Keyboard::Key_ENTER_PAD ] ){
				gameSpeed = 1;
				Global::debug( 3 ) << "Game speed " << gameSpeed << endl;
			}
                        */

			Global::speed_counter = 0;
		} else {
			Util::rest( 1 );
		}
		
		/*
		while ( Global::second_counter > 0 ){
			game_time--;
			Global::second_counter--;
			if ( game_time < 0 )
				game_time = 0;
		}
		*/
	
		if ( draw ){
			const Font & font = Font::getFont(Global::DEFAULT_FONT);

			background.Blit( work );
			world.draw( &work );
			
			/*

			// work.printf( 180, 1, Bitmap::makeColor(255,255,255), (FONT *)all_fonts[ JOHNHANDY_PCX ].dat, "%d", game_time );

			int min_x = (int)(player1->getX() < player2->getX() ? player1->getX() - 50 : player2->getX() - 50);
			int max_x = (int)(player1->getX() > player2->getX() ? player1->getX() + 50 : player2->getX() + 50);
			int min_y = 0;
			// int max_y = screen_buffer.getHeight();

			while ( max_x - min_x < screen_buffer.getWidth() / 2 ){
				max_x += 1;
				min_x -= 1;
			}

			if ( min_x > screen_buffer.getWidth() / 2 ){
				min_x = screen_buffer.getWidth() / 2;
			}
			if ( min_x < 0 ){
				min_x = 0;
			}
			if ( max_x < screen_buffer.getWidth() / 2 ){
				max_x = screen_buffer.getWidth() / 2;
			}
			if ( max_x > screen_buffer.getWidth() ){
				max_x = screen_buffer.getWidth();
			}
	
			/ * split is the number of pixels to show in the Y direction * /
			int split = screen_buffer.getHeight() * (max_x - min_x) / screen_buffer.getWidth();
			/ * cut the difference into two pieces, min_y and max_y * /
			min_y = (screen_buffer.getHeight() - split);
			// max_y -= (screen_buffer.getHeight() - split) / 2;

			// work.Stretch( screen_buffer, min_x, min_y, max_x - min_x, max_y - min_y, 0, 0, screen_buffer.getWidth(), screen_buffer.getHeight() );
			
			*/

			// work.Blit( screen_buffer );
			const double divider = 5;
			const double x_distance = screen_buffer.getWidth() / divider;
			double min_x_1 = player1->getX() - x_distance;
			double max_x_1 = player1->getX() + x_distance;
			double min_x_2 = player2->getX() - x_distance;
			double max_x_2 = player2->getX() + x_distance;

			if ( min_x_1 < 0 ){
				max_x_1 += 0 - min_x_1;
				min_x_1 = 0;
			}
			if ( max_x_1 > screen_buffer.getWidth() ){
				min_x_1 -= max_x_1 - screen_buffer.getWidth();
				max_x_1 = screen_buffer.getWidth();
			}
			
			if ( min_x_2 < 0 ){
				max_x_2 += 0 - min_x_2;
				min_x_2 = 0;
			}
			if ( max_x_2 > screen_buffer.getWidth() ){
				min_x_2 -= max_x_2 - screen_buffer.getWidth();
				max_x_2 = screen_buffer.getWidth();
			}

			if ( (min_x_1 < min_x_2 && max_x_1 > min_x_2) ||
			     (min_x_2 < min_x_1 && max_x_2 > min_x_1) ||
			     (closeFloat(min_x_1, min_x_2)) ){
			     /* the players are close enough together to show
			      * them in the same screen
			      */

				double space = x_distance * 4 - fabs( player1->getX() - player2->getX() );
				double p1 = player1->getX() < player2->getX() ? player1->getX() : player2->getX();
				double p2 = player1->getX() >= player2->getX() ? player1->getX() : player2->getX();
				double x1 = p1 - space / 2;
				double x2 = p2 + space / 2;

				if ( x2 > screen_buffer.getWidth() ){
					x1 -= x2 - screen_buffer.getWidth();
					x2 = screen_buffer.getWidth();
				} else if ( x1 < 0 ){
					x2 += 0 - x1;
					x1 = 0;
				}
				

				/*
				int y1 = (int)(distance / 2 - screen_buffer.getHeight() / divider);
				int y2 = plane + screen_buffer.getHeight();
				*/
				double visible = screen_buffer.getHeight() * (divider - 1) / divider;
				double y1 = world.getMinimumZ() - visible / 2;
				double y2 = world.getMinimumZ() + visible / 2;
				if ( y1 < 0 ){
					y2 += - y1;
					y1 = 0;
				} else if ( y2 > screen_buffer.getHeight() ){
					y1 -= y2 - screen_buffer.getHeight();
					y2 = screen_buffer.getHeight();
				}

				work.Stretch( screen_buffer, (int)x1, (int)y1, (int)(x2 - x1), (int)(y2 - y1), 0, 0, screen_buffer.getWidth(), screen_buffer.getHeight() );

			} else {
				/* split screen */

				/*
				int y1 = (int)(screen_buffer.getHeight() / divider);
				int y2 = screen_buffer.getHeight();
				*/

				double visible = screen_buffer.getHeight() * (divider - 1) / divider;
				double y1 = world.getMinimumZ() - visible / 2;
				double y2 = world.getMinimumZ() + visible / 2;
				if ( y1 < 0 ){
					y2 += - y1;
					y1 = 0;
				} else if ( y2 > screen_buffer.getHeight() ){
					y1 -= y2 - screen_buffer.getHeight();
					y2 = screen_buffer.getHeight();
				}
				int p = player1->getX() < player2->getX() ? 0 : screen_buffer.getWidth() / 2;
				work.Stretch( screen_buffer, (int)min_x_1, (int)y1, (int)(max_x_1 - min_x_1), (int)(y2 - y1), p, 0, screen_buffer.getWidth() / 2, screen_buffer.getHeight() );
				work.Stretch( screen_buffer, (int)min_x_2, (int)y1, (int)(max_x_2 - min_x_2), (int)(y2 - y1), screen_buffer.getWidth() / 2 - p, 0, screen_buffer.getWidth() / 2, screen_buffer.getHeight() );
			}
			     

			if ( showRound > 0 ){
				font.printf( screen_buffer.getWidth() / 2, screen_buffer.getHeight() / 2, roundColors[ showRound ], screen_buffer, "Round %d", 0, round );
				showRound -= 1;
			}

			
			font.printf( 10, 0, Bitmap::makeColor( 255, 255, 255 ), screen_buffer, "%s", 0, player1->getName().c_str() );
			player1->drawLifeBar( 10, font.getHeight(), &screen_buffer );
			font.printf( screen_buffer.getWidth() - 200, 0, Bitmap::makeColor( 255, 255, 255 ), screen_buffer, "%s", 0, player2->getName().c_str() );
			player2->drawLifeBar( screen_buffer.getWidth() - 200, font.getHeight(), &screen_buffer );

			FontRender * render = FontRender::getInstance();
			render->render( &screen_buffer );

			if ( paused ){
				screen_buffer.transBlender( 0, 0, 0, 128 );
				screen_buffer.drawingMode( Bitmap::MODE_TRANS );
				screen_buffer.rectangleFill( 0, 0, screen_buffer.getWidth(), screen_buffer.getHeight(), Bitmap::makeColor( 0, 0, 0 ) );
				screen_buffer.drawingMode( Bitmap::MODE_SOLID );
				const Font & font = Font::getFont(Global::DEFAULT_FONT);
				font.printf( screen_buffer.getWidth() / 2, screen_buffer.getHeight() / 2, Bitmap::makeColor( 255, 255, 255 ), screen_buffer, "Paused", 0 );
			}

			/* getX/Y move when the world is quaking */
			screen_buffer.BlitToScreen( world.getX(), world.getY() );

                        /*
			if ( key[ Keyboard::Key_F12 ] ){
				Global::debug( 2 ) << "Saved screenshot to scr.bmp" << endl;
				work.save( "scr.bmp" );
			}
                        */

			work.clear();
		}

		// done |= key[ Keyboard::Key_ESC ] || world.finished();
		done = world.finished();
	}
}
#endif

}
