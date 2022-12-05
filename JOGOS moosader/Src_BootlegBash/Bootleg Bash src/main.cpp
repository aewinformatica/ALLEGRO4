#include <allegro.h>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Tiles.h"
#include "Level.h"
#include "SFX.h"
#include "Global.h"
#include "MenuButton.h"
#include "Item.h"
/*
Bootleg Bash
08/01/08 to 08/29/08
Made for the TIGsource.com "Bootleg Demakes" competition
Source is a huge mess -_-
Programming, graphics, and music by Rachel J. Morris
Sound effects generated with SFXR
*/

const int SCR_W = 512;
const int SCR_H = 480;

const int RES_W = 256;
const int RES_H = 240;

volatile long counter = 0;

void IncrementCounter();
void InitCrap(bool);
void ToggleFullscreen( bool *fullscreen );
void DrawFacepic( BITMAP *image, BITMAP *buffer, int x, int y, int image );
bool IsCollision( Player *player, Item *item );

enum { MAINMENU = 0, VSMENU = 1, LVLMENU = 2, INGAME=3 };

class VsMenuCursor
{
    public:
        int x, y, index;
        void Setup( int nX, int nY, int nIndex )
        {
            x = nX;     y = nY;     index = nIndex;
        }
        void Draw( BITMAP *image, BITMAP *buffer )
        {
            masked_blit( image, buffer, 0, index*16 + 128, x, y, 16, 16 );
        }
};

bool CheckCollision( Player player[], Level *level, SAMPLE *p0, SAMPLE *p1, SAMPLE *p2, SAMPLE *p3, float *soundTimer, SFX sfx[] );

int main(int argc, char *argv[])
{
    bool fullscreen = false;
    bool smallScreen = false;
    bool done = false;
    InitCrap( fullscreen );

    set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0); fullscreen = true;

    float marqueeX = 256;
    float itemCounter = 0;

    int activeMenu = MAINMENU;
    int levelMap = 0, highlightMap = -1;
    int random;

    srand( (unsigned)time( NULL ) );
//
    BITMAP *buffer = create_bitmap( RES_W,RES_H );
    BITMAP *resizeBuffer = create_bitmap( SCR_W, SCR_H );
    BITMAP *imgMenu = load_bitmap( "menugraphics.bmp", NULL );
    BITMAP *imgtitle = load_bitmap( "titleimage.bmp", NULL );
    BITMAP *imgtiles = load_bitmap( "tileset.bmp", NULL );
    BITMAP *effects = load_bitmap( "effects.bmp", NULL );
    BITMAP *imgitems = load_bitmap( "items.bmp", NULL );
    BITMAP *charimg[11];
    charimg[0] = load_bitmap( "charplum.bmp", NULL );
    charimg[1] = load_bitmap( "charrank.bmp", NULL );
    charimg[2] = load_bitmap( "charmarxio.bmp", NULL );
    charimg[3] = load_bitmap( "charpookie.bmp", NULL );
    charimg[4] = load_bitmap( "charname.bmp", NULL );
    charimg[5] = load_bitmap( "charmars.bmp", NULL );
    charimg[6] = load_bitmap( "charwonky.bmp", NULL );
    charimg[7] = load_bitmap( "charphonics.bmp", NULL );
    float keyTimer = 0, soundTimer[5];
    for ( int i=0; i<5; i++ )
        soundTimer[i] = 0;

    SFX sfx[8]; //up to 8 at a time

    SAMPLE *jumpSound[4];
    jumpSound[0] = load_sample( "jump0.wav" );
    jumpSound[1] = load_sample( "jump1.wav" );
    jumpSound[2] = load_sample( "jump2.wav" );
    jumpSound[3] = load_sample( "jump3.wav" );
    SAMPLE *attackSound[4];
    attackSound[0] = load_sample( "hit0.wav" );
    attackSound[1] = load_sample( "hit1.wav" );
    attackSound[2] = load_sample( "hit2.wav" );
    attackSound[3] = load_sample( "hit3.wav" );
    SAMPLE *confirm = load_sample( "confirm.wav" );
    SAMPLE *heal = load_sample( "heal.wav" );
    SAMPLE *confirm2 = load_sample( "confirma.wav" );
    SAMPLE *deny = load_sample( "deny.wav" );
    SAMPLE *explosion = load_sample( "explosion.wav" );
    SAMPLE *drop = load_sample( "drop.wav" );
    SAMPLE *song[4];
    song[0] = load_sample( "song0.wav" );
    song[1] = load_sample( "song1.wav" );
    song[2] = load_sample( "song2.wav" );
    song[3] = load_sample( "song3.wav" );

    VsMenuCursor vsCursor[4];
    vsCursor[0].Setup( 32,     152,     0 );
    vsCursor[1].Setup( 96,      152,    1 );
    vsCursor[2].Setup( 160,      152,    2 );
    vsCursor[3].Setup( 224,    152,    3 );

    Level level[6];
    level[0].LoadMap( "map0.map" );
    level[1].LoadMap( "map1.map" );
    level[2].LoadMap( "map2.map" );
    level[3].LoadMap("map3.map");
    level[4].LoadMap("map4.map");
    level[5].LoadMap("map5.map");

    Player player[4];
    player[0].Setup( level[levelMap].StartX(0), level[levelMap].StartY(0), 0, WASD );
    player[1].Setup( level[levelMap].StartX(1), level[levelMap].StartY(1), 1, IJKL );
    player[2].Setup( level[levelMap].StartX(2), level[levelMap].StartY(2), 2, ARROWS );
    player[3].Setup( level[levelMap].StartX(3), level[levelMap].StartY(3), 3, NUMPAD );

    Item item[3];  //three on a map at a time

    int alivePlayers = 4;
    int winner = -1;

    MenuButton menuButton[9];
    for ( int i=0; i<9; i++ )
        menuButton[i].Setup(i);

    play_sample( song[0], 255, 128, 1000, true );
    while (!done)
    {
        while (counter > 0)
        {
            if (key[KEY_F5])
            {
                if ( fullscreen ) { set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCR_W, SCR_H, 0, 0); fullscreen = false; }
                else { set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0); fullscreen = true; }
            }
            if ( key[KEY_F6] )
            {
                if ( smallScreen )
                {
                    smallScreen = false;
                    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCR_W, SCR_H, 0, 0);
                    fullscreen = false;
                }
                else
                {
                    fullscreen = false;
                    smallScreen = true;
                    set_gfx_mode( GFX_AUTODETECT, RES_W, RES_H, 0, 0 );
                }
            }
            if (key[KEY_F4])
            {
                done = true;
            }
//MAIN MENU
            if ( activeMenu == MAINMENU )
            {
                if ( key[KEY_ENTER] && keyTimer <= 0)
                {
                    activeMenu = VSMENU;
                    keyTimer = 10;
                    play_sample( confirm, 255, 128, 900, false );
                }
            }
//VS MENU
            else if ( activeMenu == VSMENU )
            {
                if ( key[KEY_W] ) { vsCursor[0].y--; }
                else if ( key[KEY_S] ) { vsCursor[0].y++; }
                if ( key[KEY_A] ) { vsCursor[0].x--; }
                else if ( key[KEY_D] ) { vsCursor[0].x++; }
                if ( key[KEY_Q] || key[KEY_E] )
                {
                    //Char select
                    for ( int i=0; i<9; i++ )
                    {
                        int temp = menuButton[i].IsCollision( vsCursor[0].x, vsCursor[0].y );
                        if ( temp > -1 )
                        {
                            if ( soundTimer[0] < 0 )
                            {
                                soundTimer[0] = 10;
                                play_sample( confirm2, 255, 128, 1000, false );
                                if ( temp == 8 )
                                {
                                    temp = rand() % 8;
                                }
                                player[0].Img( temp );
                            }
                        }
                    }
                }

                if ( key[KEY_I] ) { vsCursor[1].y--; }
                else if ( key[KEY_K] ) { vsCursor[1].y++; }
                if ( key[KEY_J] ) { vsCursor[1].x--; }
                else if ( key[KEY_L] ) { vsCursor[1].x++; }
                if ( key[KEY_U] || key[KEY_O] )
                {
                    //Char select
                    for ( int i=0; i<9; i++ )
                    {
                        int temp = menuButton[i].IsCollision( vsCursor[1].x, vsCursor[1].y );
                        if ( temp > -1 )
                        {
                            if ( soundTimer[1] < 0 )
                            {
                                play_sample( confirm2, 255, 128, 1100, false );
                                soundTimer[1] = 10;
                                if ( temp == 8 )
                                {
                                    temp = rand() % 8;
                                }
                                player[1].Img( temp );
                            }
                        }
                    }
                }

                if ( key[KEY_UP] ) { vsCursor[2].y--; }
                else if ( key[KEY_DOWN] ) { vsCursor[2].y++; }
                if ( key[KEY_LEFT] ) { vsCursor[2].x--; }
                else if ( key[KEY_RIGHT] ) { vsCursor[2].x++; }
                if ( key[KEY_RCONTROL] || key[KEY_RSHIFT] )
                {
                    //Char select
                    for ( int i=0; i<9; i++ )
                    {
                        int temp = menuButton[i].IsCollision( vsCursor[2].x, vsCursor[2].y );
                        if ( temp > -1 )
                        {
                            if ( soundTimer[2] < 0 )
                            {
                                play_sample( confirm2, 255, 128, 900, false );
                                soundTimer[2] = 10;
                                if ( temp == 8 )
                                {
                                    temp = rand() % 8;
                                }
                                player[2].Img( temp );
                            }
                        }
                    }
                }

                if ( key[KEY_8_PAD] ) { vsCursor[3].y--; }
                else if ( key[KEY_2_PAD] ) { vsCursor[3].y++; }
                if ( key[KEY_4_PAD] ) { vsCursor[3].x--; }
                else if ( key[KEY_6_PAD] ) { vsCursor[3].x++; }
                if ( key[KEY_0_PAD] || key[KEY_ENTER_PAD] )
                {
                    //Char select
                    for ( int i=0; i<9; i++ )
                    {
                        int temp = menuButton[i].IsCollision( vsCursor[3].x, vsCursor[3].y );
                        if ( temp > -1 )
                        {
                            if ( soundTimer[3] < 0 )
                            {
                                play_sample( confirm2, 255, 128, 1200, false );
                                soundTimer[3] = 10;
                                if ( temp == 8 )
                                {
                                    temp = rand() % 8;
                                }
                                player[3].Img( temp );
                            }
                        }
                    }
                }
                marqueeX -= 0.5;
                if ( marqueeX < -500 )
                    marqueeX = 256;

                if ( key[KEY_ENTER] && keyTimer <= 0 )
                {
                    activeMenu = LVLMENU;
                    keyTimer = 10;
                    play_sample( confirm, 255, 128, 900, false );
                }
            }
//LVL MENU
            else if ( activeMenu == LVLMENU )
            {
                if ( vsCursor[2].x >= 0 && vsCursor[2].x < 75 && vsCursor[2].y >= 0 && vsCursor[2].y < 70 )
                {
                    highlightMap = 0;
                }
                else if ( vsCursor[2].x >= 90 && vsCursor[2].x < 165 && vsCursor[2].y >= 0 && vsCursor[2].y < 70 )
                {
                    highlightMap = 1;
                }
                else if ( vsCursor[2].x >= 181 && vsCursor[2].x < 256 && vsCursor[2].y >= 0 && vsCursor[2].y < 70 )
                {
                    highlightMap = 2;
                }
                else if ( vsCursor[2].x >= 0 && vsCursor[2].x < 75 && vsCursor[2].y >= 75 && vsCursor[2].y < 145 )
                {
                    highlightMap = 3;
                }
                else if ( vsCursor[2].x >= 90 && vsCursor[2].x < 165 && vsCursor[2].y >= 75 && vsCursor[2].y < 145 )
                {
                    highlightMap = 4;
                }
                else if ( vsCursor[2].x >= 181 && vsCursor[2].x < 256 && vsCursor[2].y >= 75 && vsCursor[2].y < 145 )
                {
                    highlightMap = 5;
                }

                if ( key[KEY_ENTER] && keyTimer <= 0 )
                {
                    alivePlayers = 4;
                    winner = -1;
                    if ( highlightMap == 0 )
                    {
                        levelMap = highlightMap;
                        activeMenu = INGAME;
                        stop_sample(song[0]);
                        stop_sample(song[1]);
                        stop_sample(song[2]);
                        stop_sample(song[3]);
                        play_sample( song[1], 255, 128, 1000, true );
                        play_sample( confirm, 255, 128, 900, false );
                    }
                   else if ( highlightMap == 1 )
                    {
                        levelMap = highlightMap;
                        activeMenu = INGAME;
                        stop_sample(song[0]);
                        stop_sample(song[1]);
                        stop_sample(song[2]);
                        stop_sample(song[3]);
                        play_sample( song[2], 255, 128, 1000, true );
                        play_sample( confirm, 255, 128, 900, false );
                    }
                    else if ( highlightMap == 2 )
                    {
                        levelMap = highlightMap;
                        activeMenu = INGAME;
                        stop_sample(song[0]);
                        stop_sample(song[1]);
                        stop_sample(song[2]);
                        stop_sample(song[3]);
                        play_sample( song[3], 255, 128, 1000, true );
                        play_sample( confirm, 255, 128, 900, false );
                    }
                    else if ( highlightMap == 3 )
                    {
                        levelMap = highlightMap;
                        activeMenu = INGAME;
                        stop_sample(song[0]);
                        stop_sample(song[1]);
                        stop_sample(song[2]);
                        stop_sample(song[3]);
                        play_sample( song[1], 255, 128, 1000, true );
                        play_sample( confirm, 255, 128, 900, false );
                    }
                   else if ( highlightMap == 4 )
                    {
                        levelMap = highlightMap;
                        activeMenu = INGAME;
                        stop_sample(song[0]);
                        stop_sample(song[1]);
                        stop_sample(song[2]);
                        stop_sample(song[3]);
                        play_sample( song[2], 255, 128, 1000, true );
                        play_sample( confirm, 255, 128, 900, false );
                    }
                    else if ( highlightMap == 5 )
                    {
                        levelMap = highlightMap;
                        activeMenu = INGAME;
                        stop_sample(song[0]);
                        stop_sample(song[1]);
                        stop_sample(song[2]);
                        stop_sample(song[3]);
                        play_sample( song[3], 255, 128, 1000, true );
                        play_sample( confirm, 255, 128, 900, false );
                    }
                    else    //nothing
                    {
                        play_sample( deny, 255, 128, 900, false );
                    }
                    player[0].Setup( level[levelMap].StartX(0), level[levelMap].StartY(0), player[0].Img(), WASD );
                    player[1].Setup( level[levelMap].StartX(1), level[levelMap].StartY(1), player[1].Img(), IJKL );
                    player[2].Setup( level[levelMap].StartX(2), level[levelMap].StartY(2), player[2].Img(), ARROWS );
                    player[3].Setup( level[levelMap].StartX(3), level[levelMap].StartY(3), player[3].Img(), NUMPAD );
                }
                if ( key[KEY_UP] ) { vsCursor[2].y--; }
                else if ( key[KEY_DOWN] ) { vsCursor[2].y++; }
                if ( key[KEY_LEFT] ) { vsCursor[2].x--; }
                else if ( key[KEY_RIGHT] ) { vsCursor[2].x++; }

                marqueeX -= 0.5;
                if ( marqueeX < -500 )
                    marqueeX = 256;

                if ( key[KEY_ESC] && keyTimer <= 0 )
                {
                    play_sample( deny, 255, 128, 900, false );
                    activeMenu = VSMENU;
                    keyTimer = 10;
                }
            }
//IN GAME
            else if ( activeMenu == INGAME )
            {
                if ( winner == -1 )
                {
                    alivePlayers = 4;
                    for ( int i=0; i<4; i++ )
                    {
                        if ( player[i].Dead() == true )
                        {
                            alivePlayers -= 1;
                        }
                        for ( int j=0; j<3; j++ )
                        {
                            if ( IsCollision( &player[i], &item[j] ) && item[j].Carrier() == -1 && item[j].Exists() == true )
                            {
    //                            if ( item[j].Use() == THROW )
    //                                item[j].Carrier( i );
                                if ( item[j].Use() == HEAL )
                                {
                                    item[j].Exists( false );
                                    player[i].AddHealth(20);
                                    play_sample( heal, 255, 128, 1000, false );
                                    player[i].IncFoodEaten();
                                }
    //                            else if ( item[j].Use() == SUMMON )
    //                            {
    //                                item[j].Carrier( i );
    //                            }
                            }
                        }
                    }
                    if ( alivePlayers == 1 )
                    {
                        if ( player[0].Dead() == false ) { winner = 0; }
                        else if ( player[1].Dead() == false ) { winner = 1; }
                        else if ( player[2].Dead() == false ) { winner = 2; }
                        else if ( player[3].Dead() == false ) { winner = 3; }
                    }
        int tx, ty;
                    if ( itemCounter == 0 )
                    {
                        random = rand()%15;
                        if ( random >= 9 ) { ; }   //nothing
                        else
                        {
                            tx = rand()%224+16;
                            ty = rand()%208;
                            if ( levelMap == 0 )
                            {
                                ty = rand()%64;
                            }
                            else if ( levelMap == 4 )
                            {
                                ty = rand() % 128;
                            }
                            else if ( levelMap == 5 )
                            {
                                ty = rand() % 176;
                            }
                            if ( item[0].Exists() == false )
                            {
                                item[0].Setup( tx, ty, 16, 16, random );
                                play_sample( drop, 255, 128, 500, false );
                            }
                            else if ( item[1].Exists() == false )
                            {
                                item[1].Setup( tx, ty, 16, 16, random );
                                play_sample( drop, 255, 128, 1000, false );
                            }
                            else if ( item[2].Exists() == false )
                            {
                                item[2].Setup( tx,ty, 16, 16, random );
                                play_sample( drop, 255, 128, 1500, false );
                            }
                        }
                    }
                    for ( int i=0; i<3; i++ )
                    {
                        if ( item[i].Carrier() != -1 )
                        {
                            item[i].Update( player[item[i].Carrier()].X(), player[item[i].Carrier()].Y() );
                        }
                        else
                        {
                            item[i].Update( &level[ levelMap ] );
                        }
                    }
                }//if winner == -1
                if ( key[KEY_ESC] && keyTimer <= 0 )
                {
                    play_sample( deny, 255, 128, 900, false );
                    activeMenu = LVLMENU;
                    keyTimer = 10;
                    stop_sample( song[0] );
                    stop_sample( song[1] );
                    stop_sample( song[2] );
                    stop_sample( song[3] );
                    play_sample( song[0], 255, 128, 1000, true );
                    item[0].Exists( false );
                    item[1].Exists( false );
                    item[2].Exists( false );
                    alivePlayers = 4;
                    winner = -1;
                    for ( int i=0; i<4; i++ )
                    {
                        player[i].Frame( 0 );
                    }
                }
                if ( winner > -1 )
                {
                    marqueeX -= 0.5;
                    if ( marqueeX < -600 )
                        marqueeX = 256;
                }
                else if ( winner == -1 )
                {
                    //WASD KEYS
                    if ( key[KEY_A] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == WASD )
                                player[i].Move( LEFT, &level[ levelMap] );
                        }
                    }
                    else if ( key[KEY_D] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == WASD )
                                player[i].Move( RIGHT, &level[ levelMap] );
                        }
                    }
                    if ( key[KEY_W] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == WASD )
                            {
                                if ( player[i].Jump( jumpSound[2] ) )
                                {
                                    //effect
                                    sfx[i].Setup( (int)player[i].X(), (int)player[i].Y(), 0 );
                                }
                            }
                        }
                    }
                    else if ( key[KEY_S] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == WASD )
                                player[i].Action( DEFEND );
                        }
                    }
                    if ( key[KEY_Q] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == WASD )
                                player[i].Action( ATK1 );
                        }
                    }
                    else if ( key[KEY_E] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == WASD )
                                player[i].Action( ATK2 );
                        }
                    }
                    //IJKL KEYS
                    if ( key[KEY_J] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == IJKL )
                                player[i].Move( LEFT, &level[ levelMap] );
                        }
                    }
                    else if ( key[KEY_L] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == IJKL )
                                player[i].Move( RIGHT, &level[ levelMap] );
                        }
                    }
                    if ( key[KEY_I] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == IJKL )
                            {
                                if ( player[i].Jump( jumpSound[1] ) )
                                {
                                    //effect
                                    sfx[i].Setup( (int)player[i].X(), (int)player[i].Y(), 0 );
                                }
                            }
                        }
                    }
                    else if ( key[KEY_K] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == IJKL )
                                player[i].Action( DEFEND );
                        }
                    }
                    if ( key[KEY_U] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == IJKL )
                                player[i].Action( ATK1 );
                        }
                    }
                    else if ( key[KEY_O] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == IJKL )
                                player[i].Action( ATK2 );
                        }
                    }
                    //ARROW KEYS
                    if ( key[KEY_LEFT] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == ARROWS )
                                player[i].Move( LEFT, &level[ levelMap] );
                        }
                    }
                    else if ( key[KEY_RIGHT] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == ARROWS )
                                player[i].Move( RIGHT, &level[ levelMap] );
                        }
                    }
                    if ( key[KEY_B] ) { player[3].Y( player[3].Y() - 3 ); }
                    if ( key[KEY_V] ) { player[3].Y( player[3].Y() + 1 ); }
                    if ( key[KEY_UP] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == ARROWS )
                            {
                                if ( player[i].Jump( jumpSound[3] ) )
                                {
                                    //effect
                                    sfx[i].Setup( (int)player[i].X(), (int)player[i].Y(), 0 );
                                }
                            }
                        }
                    }
                    else if ( key[KEY_DOWN] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == ARROWS )
                                player[i].Action( DEFEND );
                        }
                    }
                    if ( key[KEY_RCONTROL] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == ARROWS )
                                player[i].Action( ATK1 );
                        }
                    }
                    else if ( key[KEY_RSHIFT] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == ARROWS )
                                player[i].Action( ATK2 );
                        }
                    }
                    //NUMPAD KEYS
                    if ( key[KEY_4_PAD] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == NUMPAD )
                                player[i].Move( LEFT, &level[ levelMap] );
                        }
                    }
                    else if ( key[KEY_6_PAD] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == NUMPAD )
                                player[i].Move( RIGHT, &level[ levelMap] );
                        }
                    }
                    if ( key[KEY_8_PAD] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == NUMPAD )
                            {
                                if ( player[i].Jump( jumpSound[0] ) )
                                {
                                    //effect
                                    sfx[i].Setup( (int)player[i].X(), (int)player[i].Y(), 0 );
                                }
                            }
                        }
                    }
                    else if ( key[KEY_2_PAD] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == NUMPAD )
                                player[i].Action( DEFEND );
                        }
                    }
                    if ( key[KEY_ENTER_PAD] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == NUMPAD )
                                player[i].Action( ATK1 );
                        }
                    }
                    else if ( key[KEY_0_PAD] )
                    {
                        for ( int i=0; i<4; i++)
                        {
                            if ( player[i].CtrlScheme() == NUMPAD )
                                player[i].Action( ATK2 );
                        }
                    }

    //                if ( key[KEY_MINUS] )
    //                {
    //                    modifier -= 0.5;
    //                }
    //                else if (key[KEY_EQUALS] )
    //                {
    //                    modifier += 0.5;
    //                }
                }//if winner == -1
            for ( int i=0; i<4; i++ )
            {
                player[i].Update( &level[ levelMap ] );     //take care of gravity and other things

                if ( (player[i].X() < -16 || player[i].X() > 256 || player[i].Damage() <= 0) && player[i].Dead() == false )
                {
                    player[i].Kill();
                    play_sample( explosion, 255, 128, 1000, false );
                }
            }

            }//else if ( activeMenu == INGAME )

            for ( int i=0; i<8; i++ )
            {
                if ( sfx[i].exists )
                {
                    sfx[i].IncrementFrame();
                }
            }
            if ( soundTimer > 0 )
                soundTimer[4] -= 2.0;
            if ( soundTimer[0] >= 0 )
                soundTimer[0] -= 1.0;
            if ( soundTimer[1] >= 0 )
                soundTimer[1] -= 1.0;
            if ( soundTimer[2] >= 0 )
                soundTimer[2] -= 1.0;
            if ( soundTimer[3] >= 0 )
                soundTimer[3] -= 1.0;
            CheckCollision( player, &level[ levelMap], attackSound[0], attackSound[1], attackSound[2], attackSound[3], &soundTimer[4], sfx );
            keyTimer -= 0.5;
            itemCounter += 0.07;
            if ( itemCounter >= 10 )
                itemCounter = 0;
            counter--;
        }//while (speed_counter > 0)

        text_mode(-1);
        vsync();
        acquire_screen();

        if ( activeMenu == MAINMENU )
        {
            draw_sprite( buffer, imgtitle, 0, 0 );
        }
        else if ( activeMenu == VSMENU )
        {
            masked_blit( imgMenu, buffer, 64, 90, 0, 0, 256, 16 );
            textprintf_centre_ex( buffer, font, 128, 2, makecol( 0, 0, 0 ), -1, " VERSUS " );

            int rowY = 32;

            //First row
            masked_blit( imgMenu, buffer, 112, 128, 8, rowY, 48, 48 );
            masked_blit( imgMenu, buffer, 112, 128, 72, rowY, 48, 48 );
            masked_blit( imgMenu, buffer, 112, 128, 136, rowY, 48, 48 );
            masked_blit( imgMenu, buffer, 112, 128, 200, rowY, 48, 48 );
            //Second row
            masked_blit( imgMenu, buffer, 112, 128, 0, rowY+48, 48, 48 );
            masked_blit( imgMenu, buffer, 112, 128, 48+4, rowY+48, 48, 48 );
            masked_blit( imgMenu, buffer, 112, 128, 104, rowY+48, 48, 48 );
            masked_blit( imgMenu, buffer, 112, 128, 160-4, rowY+48, 48, 48 );
            masked_blit( imgMenu, buffer, 112, 128, 208, rowY+48, 48, 48 );

            //Images
            masked_blit( imgMenu, buffer, 0,    192, 8+4,   rowY+4, 40, 40 );
            masked_blit( imgMenu, buffer, 40,  192, 72+4, rowY+4, 40, 40 );
            masked_blit( imgMenu, buffer, 80,  192, 136+4,    rowY+4, 40, 40 );
            masked_blit( imgMenu, buffer, 120,  192, 200+4,    rowY+4, 40, 40 );

            masked_blit( imgMenu, buffer, 0,      232,       0+4,     rowY+48+4, 40, 40 );
            masked_blit( imgMenu, buffer, 40,    232,       48+8,   rowY+48+4, 40, 40 );
            masked_blit( imgMenu, buffer, 80,    232,       104+4,   rowY+48+4, 40, 40 );
            masked_blit( imgMenu, buffer, 120,  232,      160, rowY+48+4, 40, 40 );
            masked_blit( imgMenu, buffer, 160,  232,       208+4, rowY+48+4, 40, 40 );

            //Bottom
            masked_blit( imgMenu, buffer, 48, 128, 0, 176, 64, 64 );
            masked_blit( imgMenu, buffer, 48, 128, 64, 176, 64, 64 );
            masked_blit( imgMenu, buffer, 48, 128, 128, 176, 64, 64 );
            masked_blit( imgMenu, buffer, 48, 128, 192, 176, 64, 64 );

            DrawFacepic( imgMenu, buffer, 0+8, 176+8, player[0].Img() );
            DrawFacepic( imgMenu, buffer, 64+8, 176+8, player[1].Img() );
            DrawFacepic( imgMenu, buffer, 128+12, 176+8, player[2].Img() );
            DrawFacepic( imgMenu, buffer, 192+8, 176+8, player[3].Img() );

            textprintf_centre_ex( buffer, font, 32, 228, makecol( 0, 0, 0 ), -1, menuButton[player[0].Img()].Caption().c_str() );
            textprintf_centre_ex( buffer, font, 96, 228, makecol( 0, 0, 0 ), -1, menuButton[player[1].Img()].Caption().c_str() );
            textprintf_centre_ex( buffer, font, 160, 228, makecol( 0, 0, 0 ), -1, menuButton[player[2].Img()].Caption().c_str() );
            textprintf_centre_ex( buffer, font, 224, 228, makecol( 0, 0, 0 ), -1, menuButton[player[3].Img()].Caption().c_str() );

            for ( int i=0; i<8; i++ )
            {
                menuButton[i].Draw( imgMenu, buffer );
            }

//            textprintf_ex( buffer, font, 2, 18, makecol( 255, 255, 255 ), -1, "<-- ESC" );
//            textprintf_ex( buffer, font, 184, 18, makecol( 255, 255, 255 ), -1, "ENTER -->" );

            textprintf_ex( buffer, font, (int)marqueeX, 18, makecol( 255, 255, 255 ), -1, "Select your characters and hit ENTER to continue!" );
            masked_blit( imgMenu, buffer, 64, 0, 8, 140, 48, 32 );
            masked_blit( imgMenu, buffer, 112, 0, 72, 140, 48, 32 );
            masked_blit( imgMenu, buffer, 160, 0, 136, 140, 48, 32 );
            masked_blit( imgMenu, buffer, 208, 0, 200, 140, 48, 32 );
            for ( int i=0; i<4; i++ )
            {
                vsCursor[i].Draw( imgMenu, buffer );
            }
        } //else if ( activeMenu == VSMENU )
        else if ( activeMenu == LVLMENU )
        {
            masked_blit( imgMenu, buffer, 0, 272, 0, 0, 75, 70 );
            masked_blit( imgMenu, buffer, 75, 272, 90, 0, 75, 70 );
            masked_blit( imgMenu, buffer, 150, 272, 181, 0, 75, 70 );

            masked_blit( imgMenu, buffer, 0, 342, 0, 75, 75, 70 );
            masked_blit( imgMenu, buffer, 75, 342, 90, 75, 75, 70 );
            masked_blit( imgMenu, buffer, 150, 342, 181, 75, 75, 70 );

            if ( highlightMap == 0 )    //Blademaster Bobo's name for level
                textprintf_centre_ex( buffer, font, 128, 210, makecol( 255, 255, 255 ), -1, "Hanging Balls" );
            else if ( highlightMap == 1 )
                textprintf_centre_ex( buffer, font, 128, 210, makecol( 255, 255, 255 ), -1, "Plum's Castle" );
            else if ( highlightMap == 2 )
                textprintf_centre_ex( buffer, font, 128, 210, makecol( 255, 255, 255 ), -1, "Ruin Gardens" );
            else if ( highlightMap == 3 )
                textprintf_centre_ex( buffer, font, 128, 210, makecol( 255, 255, 255 ), -1, "Hat Lantis" );
            else if ( highlightMap == 4 )
                textprintf_centre_ex( buffer, font, 128, 210, makecol( 255, 255, 255 ), -1, "Wonky Forest" );
            else if ( highlightMap == 5 )
                textprintf_centre_ex( buffer, font, 128, 210, makecol( 255, 255, 255 ), -1, "Ramen Ranch" );
            else
                ;

            textprintf_ex( buffer, font, marqueeX, 230, makecol( 255, 255, 255 ), -1, "Choose a map with the ARROW KEYS and hit ENTER to start!" );

            vsCursor[2].Draw( imgMenu, buffer );
        } //else if ( activeMenu == LVLMENU )
        else if ( activeMenu == INGAME )
        {
            level[ levelMap ].DrawBottom( buffer, imgtiles );
            for ( int i=0; i<4; i++ )
            {
                player[i].Draw( charimg[ player[i].Img() ], buffer, i );
                if ( i <= 3 )
                    item[i].Draw( imgitems, buffer );
            }
            level[ levelMap ].DrawTop( buffer, imgtiles );
            //Damage meters
            textprintf_centre_ex( buffer, font, 32-1, 230-1, makecol( 0, 0, 0 ), -1, "%i", (int)player[0].Damage() );
            textprintf_centre_ex( buffer, font, 96-1, 230-1, makecol( 0, 0, 0 ), -1, "%i", (int)player[1].Damage() );
            textprintf_centre_ex( buffer, font, 160-1, 230-1, makecol( 0, 0, 0 ), -1, "%i", (int)player[2].Damage() );
            textprintf_centre_ex( buffer, font, 224-1, 230-1, makecol( 0, 0, 0 ), -1, "%i", (int)player[3].Damage() );

            textprintf_centre_ex( buffer, font, 32, 230, makecol( 255, 255, 255 ), -1, "%i", (int)player[0].Damage() );
            textprintf_centre_ex( buffer, font, 96, 230, makecol( 255, 255, 255 ), -1, "%i", (int)player[1].Damage() );
            textprintf_centre_ex( buffer, font, 160, 230, makecol( 255, 255, 255 ), -1, "%i", (int)player[2].Damage() );
            textprintf_centre_ex( buffer, font, 224, 230, makecol( 255, 255, 255 ), -1, "%i", (int)player[3].Damage() );

            masked_blit( charimg[ player[0].Img() ], buffer, 48, 0, 32-8, 210, 16, 16 );
            masked_blit( charimg[ player[1].Img() ], buffer, 48, 0, 96-8, 210, 16, 16 );
            masked_blit( charimg[ player[2].Img() ], buffer, 48, 0, 160-8, 210, 16, 16 );
            masked_blit( charimg[ player[3].Img() ], buffer, 48, 0, 224-8, 210, 16, 16 );

            for ( int i=0; i<8; i++ )
            {
                if ( sfx[i].exists )
                {
                    masked_blit( effects, buffer, (int)sfx[i].frame*32, sfx[i].img*16, sfx[i].x, sfx[i].y, 32, 16 );
                }
            }
            int dispX = 256/2 - 180/2 - 10, dispY = 240/2-130/2;
            if ( winner != -1 )
            {
                masked_blit( imgMenu, buffer, 0, 412, dispX, dispY, 179, 130 );
                dispY += 10;
                dispX += 150;
                if ( winner == 0 )
                {
                    masked_blit( imgMenu, buffer, 179, 412, dispX, dispY, 50, 50 );
                }
                else if ( winner == 1 )
                {
                    masked_blit( imgMenu, buffer, 229, 412, dispX, dispY, 50, 50 );
                }
                else if ( winner == 2 )
                {
                    masked_blit( imgMenu, buffer, 179, 462, dispX, dispY, 50, 50 );
                }
                else if ( winner == 3 )
                {
                    masked_blit( imgMenu, buffer, 229, 462, dispX, dispY, 50, 50 );
                }

                //Stats
                textprintf_ex( buffer, font, marqueeX, 200, makecol( 0, 0, 0 ), -1, "Food eaten:  Player 1 - %i, Player 2 - %i, Player 3 - %i, Player 4 - %i",
                        player[0].FoodEaten(), player[1].FoodEaten(), player[2].FoodEaten(), player[3].FoodEaten() );
                    textprintf_ex( buffer, font, marqueeX+1, 200+1, makecol( 255, 255, 255 ), -1, "Food eaten:  Player 1 - %i, Player 2 - %i, Player 3 - %i, Player 4 - %i",
                        player[0].FoodEaten(), player[1].FoodEaten(), player[2].FoodEaten(), player[3].FoodEaten() );
            }
        }

        stretch_sprite( resizeBuffer, buffer, 0, 0, SCR_W, SCR_H );
        if ( fullscreen )
            blit(resizeBuffer, screen, 0, 0, 64, 0, SCR_W, SCR_H);
        else if ( !smallScreen && !fullscreen )
            blit(resizeBuffer, screen, 0, 0, 0, 0, SCR_W, SCR_H);
        else if ( smallScreen )
            blit( buffer, screen, 0, 0, 0, 0, RES_W, RES_H );
        clear_bitmap(buffer);
        clear_bitmap(resizeBuffer);
        release_screen();
    }//while (!key[KEY_ESC])

//    //Clean up!!
    destroy_bitmap( resizeBuffer );
    destroy_bitmap( buffer );
    destroy_bitmap( imgMenu );
    destroy_bitmap( imgtitle );
    destroy_bitmap( imgtiles );
    destroy_bitmap( charimg[0] );
    destroy_bitmap( charimg[1] );
    destroy_bitmap( charimg[2] );
    destroy_bitmap( charimg[3] );
    destroy_bitmap( charimg[4] );
    destroy_bitmap( charimg[5] );
    destroy_bitmap( charimg[6] );
    destroy_bitmap( charimg[7] );
    destroy_bitmap( effects );
    destroy_sample( jumpSound[0] );
    destroy_sample( jumpSound[1] );
    destroy_sample( jumpSound[2] );
    destroy_sample( jumpSound[3] );
    destroy_sample( attackSound[0] );
    destroy_sample( attackSound[1] );
    destroy_sample( attackSound[2] );
    destroy_sample( attackSound[3] );
    destroy_sample( drop );
    destroy_sample( heal );
    destroy_sample( explosion );
    destroy_sample( song[0] );
    destroy_sample( song[1] );
    destroy_sample( song[2] );
    destroy_sample( song[3] );
    destroy_sample( confirm );
    destroy_sample( deny );

    return 0;
}
END_OF_MAIN();

void InitCrap(bool fullscreen)
{
    allegro_init();
    install_keyboard();
    install_timer();
    install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
    install_joystick(JOY_TYPE_AUTODETECT);
    LOCK_VARIABLE( counter );
    LOCK_FUNCTION( IncrementCounter );
    install_int_ex( IncrementCounter,BPS_TO_TIMER(90) );
    set_color_depth(16);
    if ( fullscreen ) { set_gfx_mode(GFX_AUTODETECT, SCR_W, SCR_H, 0, 0); }
    else { set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCR_W, SCR_H, 0, 0); }
}

void IncrementCounter()
{
    counter++;
}

void ToggleFullscreen( bool *fullscreen )
{
    if ( *fullscreen )
    {
        *fullscreen = false;
        set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCR_W, SCR_H, 0, 0);
    }
    else
    {
        *fullscreen = true;
        set_gfx_mode(GFX_AUTODETECT, SCR_W, SCR_H, 0, 0);
    }
}

bool CheckCollision( Player player[], Level *level, SAMPLE *p0, SAMPLE *p1, SAMPLE *p2, SAMPLE *p3, float *soundTimer, SFX sfx[] )
{
    int random = (int)rand() % 4;
    //Check collision between players, see if any are attacking each other.
    for ( int i=0; i<4; i++ )
    {
        for ( int j=0; j<4; j++ )
        {
            if ( i != j && player[i].Dead() == false && player[j].Dead() == false )
            {
                //Collision?
                if (    ( player[i].X() + player[i].Left()         <       player[j].X() + player[j].Right()  ) &&
                        ( player[i].X() + player[i].Right()       >       player[j].X() + player[j].Left()  ) &&
                        ( player[i].Y() + player[i].Top()         <       player[j].Y() + player[j].Bottom() ) &&
                        ( player[i].Y() + player[i].Bottom()   >       player[j].Y() + player[j].Top() ) )
                {
                    int hDir, hDir2, vDir;
                    //no move :(
                    if ( player[i].Action() == ATK1 || player[i].Action() == ATK2 )
                    {
                        if ( player[i].X() < player[j].X() )        //Player i is to the left of player j
                        {
                            hDir = LEFT;
                            hDir2 = RIGHT;
                        }
                        else
                        {
                            hDir = RIGHT;
                            hDir2 = LEFT;
                        }
                        if ( player[i].Y() < player[j].Y() )
                            vDir = UP;
                        else
                            vDir = DOWN;
                        player[i].Push( hDir2, level );
                        player[j].Hurt( player[i].Atk( player[i].Action() ), hDir, vDir, level );     //Player j is hurt by player i
                        if ( hDir == LEFT )
                            sfx[j+4].Setup( player[j].X(), player[j].Y(), 2 );
                        else if ( hDir == RIGHT )
                            sfx[j+4].Setup( player[j].X(), player[j].Y(), 1 );
                        if ( *soundTimer <= 0 )
                        {
                            if ( random == 0 )
                                play_sample( p0, 255, 128, 1000, false );
                            else if ( random == 1 )
                                play_sample( p1, 255, 128, 1000, false );
                            else if ( random == 2 )
                                play_sample( p2, 255, 128, 1000, false );
                            else if ( random == 3 )
                                play_sample( p3, 255, 128, 1000, false );

                            *soundTimer = 10;
                        }
                    }
                    else if ( player[j].Action() == ATK1 || player[j].Action() == ATK2 )
                    {
                        if ( player[j].X() < player[i].X() )
                        {
                            hDir = LEFT;
                            hDir2 = RIGHT;
                        }
                        else
                        {
                            hDir = RIGHT;
                            hDir2 = LEFT;
                        }
                        if ( player[j].Y() < player[i].Y() )
                            vDir = UP;
                        else
                            vDir = DOWN;
                        player[i].Hurt( player[j].Atk( player[j].Action() ), hDir, vDir, level );
                        player[j].Push( hDir2, level );
                        if ( hDir == LEFT )
                            sfx[i+4].Setup( player[i].X(), player[i].Y(), 2 );
                        else if ( hDir == RIGHT )
                            sfx[i+4].Setup( player[i].X(), player[i].Y(), 1 );
                        if ( *soundTimer <= 0 )
                        {
                            if ( random == 0 )
                                play_sample( p0, 255, 128, 1000, false );
                            else if ( random == 1 )
                                play_sample( p1, 255, 128, 1000, false );
                            else if ( random == 2 )
                                play_sample( p2, 255, 128, 1000, false );
                            else if ( random == 3 )
                                play_sample( p3, 255, 128, 1000, false );

                            *soundTimer = 10;
                        }
                    }
                }
            }
        }
    }
}

void DrawFacepic( BITMAP *img, BITMAP *buffer, int x, int y, int image )
{
    int tfx, tfy;
    switch ( image )
    {
        case 0:     //Plum
            tfx = 0;
            tfy = 192;
        break;

        case 1:     //Rank
            tfx = 40;
            tfy = 192;
        break;

        case 2:     //Marxio
            tfx = 80;
            tfy = 192;
        break;

        case 3:     //Pookie
            tfx = 120;
            tfy = 192;
        break;

        case 4:     //Name
            tfx = 0;
            tfy = 232;
        break;

        case 5:     //Mars
            tfx = 40;
            tfy = 232;
        break;

        case 6:     //Wonky
            tfx = 80;
            tfy = 232;
        break;

        case 7:     //Phonics
            tfx = 120;
            tfy = 232;
        break;

        case 8:     //Random
            tfx = 160;
            tfy = 232;
        break;

        case 9:     //Dono
            tfx = 160;
            tfy = 192;
        break;

        case 10:    //Me
            tfx = 200;
            tfy = 192;
        break;

        default:
            cerr<<"no image"<<endl;
        break;
    }
    masked_blit( img, buffer, tfx, tfy, x, y, 40, 40 );
}

bool IsCollision( Player *player, Item *item )
{
    int top1 = player->Y() + player->Top();
    int bottom1 = player->Y() + player->Bottom();
    int left1 = player->X() + player->Left();
    int right1 = player->X() + player->Right();
    int top2 = item->Y() + item->Top();
    int bottom2 = item->Y() + item->Bottom();
    int left2 = item->X() + item->Left();
    int right2 = item->X() + item->Right();
    if ( right1 > left2 && left1 < right2 && bottom1 > top2 && top1 < bottom2 )
        return true;
    return false;
}


