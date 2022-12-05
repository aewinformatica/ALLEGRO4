#include <iostream>
#include <string>
#include <fstream>
#include <allegro.h>
#include "System.h"
#include "Button.h"
#include "Window.h"
#include "Brush.h"
#include "Tile.h"
#include "Level.h"
using namespace std;

volatile long spdCounter = 0;
void IncrementCounter();
void HandleButton( Brush*, Level*, Window*, int*, int, string, string, System*, int, int );
void LoadConfig( string *mapSave, string *mapLoad, string *tilesetFilename );

const int TILE_AMT = 250;
const int LAYERAMT = 4;

int MAX_X = 75;       
int MAX_Y = 56;       

int main()
{
    cout<<"-------------------------------------------------"<<endl;
    cout<<"|  MusuGo v0.4.5  |"<<endl;
    cout<<"|  Press C to access collision layer "<<endl;
    cout<<"| to load in old maps, add 'v 0' after the w/x and h/y coordinates"<<endl;

    System system;
    LOCK_VARIABLE( counter );
    LOCK_FUNCTION( IncrementCounter );
    install_int_ex( IncrementCounter, BPS_TO_TIMER( 90 ) );

    int tileScroll = 0, levelIndex = 0, toggle = 0;;
    string mapSave, mapLoad, tilesetFilename;
    LoadConfig( &mapSave, &mapLoad, &tilesetFilename );

    BITMAP *buffer = create_bitmap( system.ScrW(), system.ScrH() ) ;
    BITMAP *imgButtonStrip = load_bitmap( "musugo_buttons.bmp", NULL );
    BITMAP *imgTileset = load_bitmap( tilesetFilename.c_str(), NULL );

    float keybTimer = 0;

    Level level[1];

    level[levelIndex].AllocateTiles( MAX_X, MAX_Y, LAYERAMT );

    Tile tileset[TILE_AMT];
    for ( int i=0; i<TILE_AMT; i++ )
    {
        tileset[i].Setup( i*32, 0, 32, 32, i*32, false );
    }

    level[0].Setup( MAX_X, MAX_Y );

    Window winSub( 0, "sub" );
    Window winTile( 1, "tile" );

    Brush cursor;

    while ( !system.Done() )
    {
        while ( spdCounter > 0 )
        {
            if ( key[KEY_ESC] || key[KEY_F4] ) { system.Quit(); }
            if ( key[KEY_F5] ) { system.ToggleFullscreen(); }

            if ( key[KEY_UP] && key[KEY_RSHIFT] ) { level[levelIndex].MoveOffsetY( -20 ); }
            else if ( key[KEY_UP] ) { level[levelIndex].MoveOffsetY( -5 ); }
            if ( key[KEY_DOWN] && key[KEY_RSHIFT] ) { level[levelIndex].MoveOffsetY( 20 ); }
            else if ( key[KEY_DOWN] ) { level[levelIndex].MoveOffsetY( 5 ); }
            if ( key[KEY_LEFT] && key[KEY_RSHIFT] ) { level[levelIndex].MoveOffsetX( -20 ); }
            else if ( key[KEY_LEFT] ) { level[levelIndex].MoveOffsetX( -5 ); }
            if ( key[KEY_RIGHT] && key[KEY_RSHIFT] ) { level[levelIndex].MoveOffsetX( 20 ); }
            else if ( key[KEY_RIGHT] ) { level[levelIndex].MoveOffsetX( 5 ); }

            if ( key[KEY_HOME] ) { level[levelIndex].OffsetX( 0 ); }
            else if ( key[KEY_END] ) { level[levelIndex].OffsetX( MAX_X*32 - 1024 ); }
            if ( key[KEY_PGUP] ) { level[levelIndex].OffsetY( 0 ); }
            else if ( key[KEY_PGDN] ) { level[levelIndex].OffsetY( MAX_Y*32 - 768 ); }
            
            if ( key[KEY_MINUS] && keybTimer <= 0 )
            {
                level[levelIndex].ScaleMiniMap( 1 );
                keybTimer = 10;
            }
            else if ( key[KEY_EQUALS] && keybTimer <= 0 )
            {
                level[levelIndex].ScaleMiniMap( -1 );
                keybTimer = 10;
            }

            if ( key[KEY_C] && keybTimer <= 0 ) 
            { 
                level[levelIndex].ToggleCollisionLayer(); 
                keybTimer = 10; 
                cout<<"layer "<<level[levelIndex].IsEditingCollisionLayer()<<endl;
            }
            
            if ( key[KEY_M] && keybTimer <= 0 ) { level[levelIndex].ToggleMiniMap(); keybTimer = 10; }

            if ( key[KEY_A] ) { if ( tileScroll - 5 > 0 ) { tileScroll -= 5; } }
            else if ( key[KEY_D] ) { tileScroll += 5; }
            if ( key[KEY_S] && keybTimer <= 0 )
            {
                level[levelIndex].SaveMap( mapSave, MAX_X, MAX_Y );
                keybTimer = 10;
            }
            else if ( key[KEY_L] && keybTimer <= 0 )
            {
                level[levelIndex].LoadMap( mapLoad, &MAX_X, &MAX_Y );
                keybTimer = 10;
            }

            if ( key[KEY_G] && toggle == 0 ) { level[levelIndex].ToggleGrid(); toggle = 10; }

            if ( mouse_b & 1 )  //left mouse button
            {
                cursor.SetClick( true );
                if ( mouse_y < 32 ) //choosing tile
                {
                    cursor.SetFx( (( mouse_x + tileScroll ) / 32 ) * 32 );
                }
                else if ( mouse_y < 60 && mouse_x < 138)
                {
                    HandleButton( &cursor, &level[levelIndex], &winTile, &tileScroll, imgTileset->w, mapSave, mapLoad, &system, MAX_X, MAX_Y );
                }
                else if ( mouse_y >= winSub.Y1() )  //on bottom menu
                {
                    cursor.SetClick( true );
                }
                else    //on level
                {
                    level[levelIndex].CheckClick( &cursor, 1, MAX_X, MAX_Y );
                }
   
            }
            else if ( mouse_b & 2 ) //right mouse button
            {
                cursor.SetClick( true );
                level[levelIndex].CheckClick( &cursor, 2, MAX_X, MAX_Y );
            }
            else if ( !mouse_b )    //no mouse buttons clicked
            {
                if ( cursor.Click() == true )
                {
                    cursor.SetClick( false );
                    HandleButton( &cursor, &level[levelIndex], &winSub, &tileScroll, imgTileset->w, mapSave, mapLoad, &system, MAX_X, MAX_Y );
                }
                winSub.ResetButtons();
                winTile.ResetButtons();
            }

            winSub.HighlightButtons();
            winTile.HighlightButtons();

            if ( toggle > 0 )
                toggle -= 0.1;

        if ( keybTimer > 0 ) { keybTimer -= 0.5; }
        spdCounter--;
        }
        vsync();
        acquire_screen();
        //Draw
        rectfill( buffer, 0, 0, system.ScrW(), system.ScrH(), makecol( 125, 100, 150 ) );

        //Draw level
        level[levelIndex].Draw( buffer, imgTileset, MAX_X, MAX_Y );

        //Draw tileset
        rectfill( buffer, 0, 0, 1024, 32, makecol( 0, 0, 0 ) );
        for ( int i=0; i<TILE_AMT; i++ )
        {
            tileset[i].Draw( buffer, imgTileset, tileScroll, 0 );
        }
        rect( buffer, 0, 0, 1024, 33, makecol( 0, 0, 0 ) );

        //Draw menus
        winSub.Draw( buffer, imgButtonStrip );
        winTile.Draw( buffer, imgButtonStrip );

        //Draw current brush
        masked_blit( imgTileset, buffer, cursor.Fx(), 0, 305, 685, 32, 32 );
        rect( buffer, 305, 685, 305+32, 685+32, makecol( 0, 0, 0 ) );

        //Text
        
        if ( level[levelIndex].IsEditingCollisionLayer() == true )
        {
            textprintf_ex( buffer, font, 4, 100, makecol( 255, 255, 255 ),
                -1, "EDITING COLLISION LAYER" );
        }
        
        textprintf_ex( buffer, font, 600, 750, makecol( 0, 0, 0 ),
                -1, "Map Dimension %ix%i", MAX_X, MAX_Y );        
        
        //draw mini map
        level[levelIndex].DrawMiniMap( buffer, imgTileset, MAX_X, MAX_Y, &system );
        textprintf_ex( buffer, font, 370, 670-12, makecol( 255, 255, 255 ), -1,
            "Minimap - M to toggle, -/+ scale" );

        //Draw mouse
        cursor.Draw( buffer );

        //Draw to screen
        blit( buffer, screen, 0, 0, 0, 0, system.ScrW(), system.ScrH() );
        //Clear buffer for next draw
        clear_bitmap( buffer );
        release_screen();
    }

    return 0;
}
END_OF_MAIN();

void IncrementCounter()
{
    spdCounter++;
}

void HandleButton( Brush *mouse, Level *level, Window *menu, int *scroll, int max, string mapSave, string mapLoad, System *system, int MAX_X, int MAX_Y )
{
    string temp = menu->CheckClick();

    if ( temp == "exit" ) { system->Quit(); }
    else if ( temp == "fullscreen" ) { system->ToggleFullscreen(); }
    else if ( temp == "save" )
    {
        level->SaveMap( mapSave, MAX_X, MAX_Y );
    }
    else if ( temp == "load" )
    {
        level->LoadMap( mapLoad, &MAX_X, &MAX_Y );
    }
    else if ( temp == "L1" )
    {
        mouse->SetLayer( 0 );
        menu->SetButtonEnable( "L1", true );
        menu->SetButtonEnable( "L2", false );
        menu->SetButtonEnable( "L3", false );
    }
    else if ( temp == "L2" )
    {
        mouse->SetLayer( 1 );
        menu->SetButtonEnable( "L1", false );
        menu->SetButtonEnable( "L2", true );
        menu->SetButtonEnable( "L3", false );
    }
    else if ( temp == "L3" )
    {
        mouse->SetLayer( 2 );
        menu->SetButtonEnable( "L1", false );
        menu->SetButtonEnable( "L2", false );
        menu->SetButtonEnable( "L3", true );
    }
    else if ( temp == "S1" )
    {
        mouse->SetSize( 1 );
        menu->SetButtonEnable( "S1", true );
        menu->SetButtonEnable( "S3", false );
    }
    else if ( temp == "S3" )
    {
        mouse->SetSize( 3 );
        menu->SetButtonEnable( "S1", false );
        menu->SetButtonEnable( "S3", true );
    }
    else if ( temp == "fill" )
    {
        level->FillLayer( mouse->Layer(), mouse->Fx(), MAX_X, MAX_Y );
    }
    else if ( temp == "cls" )
    {
        level->ClearLayer( mouse->Layer(), MAX_X, MAX_Y );
    }
    else if ( temp == "<<" )
    {
        if ( *scroll - 20 > 0 )
            *scroll -= 20;
    }
    else if ( temp == "<-" )
    {
        if ( *scroll - 5 > 0 )
            *scroll -= 5;
    }
    else if ( temp == "->" )
    {
        *scroll += 5;
    }
    else if ( temp == ">>" )
    {
        *scroll += 20;
    }
    else if ( temp == "|<" )
    {
        *scroll = 0;
    }
    else if ( temp == ">|" )
    {
        *scroll = max - system->ScrW();
    }
}

void LoadConfig( string *mapSave, string *mapLoad, string *tilesetFilename )
{
    ifstream infile;
    infile.open( "config.txt" );
    if ( infile.fail() )
    {
        cerr<<" CONFIG FILE NOT FOUND "<<endl;
        exit(10);
    }
    string temp;
    while ( infile>>temp )
    {
        if ( temp == "Load_from:" )
            infile>>*mapLoad;
        else if ( temp == "Save_to:" )
            infile>>*mapSave;
        else if ( temp == "Tileset:")
            infile>>*tilesetFilename;
        else if ( temp == "X:" )
            infile>>MAX_X;
        else if ( temp == "Y:" )
            infile>>MAX_Y;
    }
}


