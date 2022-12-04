//main.cpp

#include <allegro.h>
#include "Character.h"

using namespace std;

BITMAP* buffer;
Character myChar;

void Setup(){

     buffer= create_bitmap(640, 480);
     myChar.SetImagesLeft( "left1.bmp", "left2.bmp", "left3.bmp");
     myChar.SetImagesRight( "right1.bmp", "right2.bmp", "right3.bmp");
     myChar.SetImagesUp( "up1.bmp", "up2.bmp", "up3.bmp");
     myChar.SetImagesDown( "down1.bmp", "down2.bmp", "down3.bmp");
     
     myChar.Show( screen);

}

void movePlayer(){

    if( key[KEY_LEFT]){
        myChar.MoveOnX( LEFT, buffer, screen);
    } else if( key[KEY_RIGHT]){
        myChar.MoveOnX( RIGHT, buffer, screen);
    } else if( key[KEY_UP]){
        myChar.MoveOnY( UP, buffer, screen);
    } else if( key[KEY_DOWN]){
        myChar.MoveOnY( DOWN, buffer, screen);
    }

}

int main(int argc, char *argv[])
{
    allegro_init(); 
    install_mouse(); 
    install_keyboard(); 
    set_color_depth(16); 
    
    set_gfx_mode( GFX_AUTODETECT, 640, 480, 0, 0); 
    
    Setup();
    
    while( !key[KEY_ESC]){
        movePlayer();
    }
    
    return 0;
}
END_OF_MAIN();
