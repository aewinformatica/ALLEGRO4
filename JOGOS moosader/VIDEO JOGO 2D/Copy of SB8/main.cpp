//main.cpp
#include <iostream>
#include <allegro.h>
#include "LevelClass.h"
#include "PlayerClass.h"
using namespace std;

//every time we mention "LEFT" or another direction
//in the code, since we've defined them as numbers,
//the program will basically pass around the numbers,
//but we can reference them as these directions instead.
#define LEFT          4
#define RIGHT        6
#define DOWN        2
#define UP              8

//Screen width and height, constant variables so
//if we change the resolution we can simply change these
//instead of changing them everywhere we tell the program
//what width and height the screen are
const int SCR_W = 640;
const int SCR_H = 480;

//speedCounter and incrementCounter is to regulate the frames per second
volatile long speedCounter = 0;
void incrementCounter();
//initCrap has allegro's setup functions
void initCrap(bool);
//CheckCollision will be used to decide whether the player can move in a certain direction
bool CheckCollision(const PlayerClass *player, int direction, const LevelClass *level);

void Update(float *offsetX, float *offsetY, PlayerClass *player);

int main()
{
    int gameTimer = 0;
    bool fullscreen = false;
    bool done = false;      //when this is true, the loop keeping the program running will break and the program will quit.
    initCrap(fullscreen);   //Call the initialization function
    LevelClass level[1];    //Create an array of levels... right now we only have one.
    PlayerClass player;     //Create the player
    level[0].LoadMap("level0.txt");     //Load level0.txt as level0's map

    float offsetX, offsetY;

    //We draw everything to the buffer before we draw it to the screen.
    BITMAP *buffer = create_bitmap(SCR_W, SCR_H);
    BITMAP *tileset = load_bitmap("tilesetA.bmp", NULL);            //the graphics for the level
    //character sprites
    BITMAP *spriteMudkip = load_bitmap ("Mudkip.bmp", NULL);
    BITMAP *spritePikachu = load_bitmap ("pikachu.bmp", NULL);
    BITMAP *spriteVulpix = load_bitmap ("vulpix.bmp", NULL);
    BITMAP *spriteJigglypuff = load_bitmap("jigglypuff.bmp", NULL);

    while ( !done )     //while we're not done with the game
    {
        while ( speedCounter > 0 )      //put the main game things in here
        {
            if ( key[KEY_F5] )  //fullscreen toggle
            {
                if ( fullscreen )
                {
                    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCR_W, SCR_H, 0, 0);
                    fullscreen = false;
                }
                else
                {
                    set_gfx_mode(GFX_AUTODETECT, SCR_W, SCR_H, 0, 0);
                    fullscreen = true;
                }
            }//if ( key[KEY_F5] )
            if ( key[KEY_F4] || key[KEY_ESC] )      //quit game with F4 or ESC
            {
                done = true;
            }//if ( key[KEY_F4] || key[KEY_ESC] )

            //Player movement
            if ( key[KEY_LEFT] )
            {
                if ( CheckCollision( &player, LEFT, &level[0] ) == false )
                    player.Move( LEFT );
            }
            else if ( key[KEY_RIGHT] )
            {
                if ( CheckCollision( &player, RIGHT, &level[0] ) == false )
                    player.Move( RIGHT );
            }
            //Temporary movement
            //if ( key[KEY_DOWN] ) { player.y += 2.0; }
            if ( key[KEY_UP] ) { player.Jump(); }

            //Check to see if player is standing on anything and, if not, tell the program that they're falling.
            if ( CheckCollision( &player, DOWN, &level[0] ) == false )
            {
                player.falling = true;
            }
            else
            {
                player.falling = false;
                player.airTime = 0;
            }

            player.Update();
            speedCounter --;
            gameTimer++;
            Update(&offsetX, &offsetY, &player);
        }
        text_mode(-1);          //sets text background to be transparent
        vsync();        //to prevent screen blinking
        acquire_screen();
        //Draw graphics
        level[0].DrawBottom(buffer, tileset, offsetX, offsetY);
        player.Draw(buffer, spriteMudkip, offsetX, offsetY);
        level[0].DrawTop(buffer, tileset, offsetX, offsetY);

        blit(buffer, screen, 0, 0, 0, 0, SCR_W, SCR_H);
        clear_bitmap(buffer);       //we want to clear off the buffer after we draw it to the screen so we can draw on a clean buffer next time around.
        release_screen();
    }//while ( !done )
    return 0;
}
END_OF_MAIN();

void initCrap(bool fullscreen)
{
    allegro_init();
    install_keyboard();
    install_timer();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
    install_joystick(JOY_TYPE_AUTODETECT);
    LOCK_VARIABLE(speedCounter);
    LOCK_FUNCTION(incrementCounter);
    install_int_ex(incrementCounter, BPS_TO_TIMER(90));
    set_color_depth(16);
    if ( fullscreen )
        set_gfx_mode(GFX_AUTODETECT, SCR_W, SCR_H, 0, 0);
    else
        set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCR_W, SCR_H, 0, 0);
}

void incrementCounter()
{
    speedCounter++;
}

bool CheckCollision(const PlayerClass *player, int direction, const LevelClass *level)
{
    int tx, ty;
    if ( direction == LEFT )
    {
        tx = (player->x - player->speed) / 32;
        ty = player->y/32;
    }
    else if ( direction == RIGHT )
    {
        tx = (player->x + player->speed) / 32;
        ty = player->y/32;
    }
    else if ( direction == DOWN)
    {
        tx = player->x/32;
        ty = (player->y +(player->airTime/16)) / 32;
    }

    if ( direction == DOWN || player->falling )
    {
        if ( level->tile[0][tx][ty].solid || level->tile[0][tx+1][ty].solid )
        {
            return true;
        }
        else if ( level->tile[0][tx][ty+1].halfsolid || level->tile[0][tx+1][ty+1].halfsolid ||
                        level->tile[1][tx][ty+1].halfsolid || level->tile[1][tx+1][ty+1].halfsolid)
        {
            if (    (player->y + player->w/3 > level->tile[0][tx][ty+1].y &&
                    player->y < level->tile[0][tx][ty+1].y + 32) ||
                    (player->y + player->w/3 > level->tile[0][tx+1][ty+1].y &&
                    player->y < level->tile[0][tx+1][ty+1].y + 32))
            {
                return true;
            }
        }
    }
    else if ( direction == LEFT )
    {
        if ( level->tile[0][tx][ty].solid || level->tile[0][tx][ty+1].solid )
        {
            return true;
        }
    }
    else if ( direction == RIGHT )
    {
        if ( level->tile[0][tx+1][ty].solid || level->tile[0][tx+1][ty+1].solid )
        {
            return true;
        }
    }

    return false;
}


void Update(float *offsetX, float *offsetY, PlayerClass *player)
{
    //Thanks to Falco for help ;o

    if ( player->x >= 0.525 * SCR_W )
        *offsetX = player->x - 0.5*player->w - 0.5*SCR_W;
    if ( player->y >= 0.525 * SCR_H )
    *offsetY = player->y - 0.5*player->h - 0.5*SCR_H;
}













