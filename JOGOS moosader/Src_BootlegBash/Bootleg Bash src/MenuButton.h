#pragma once
#include <allegro.h>
#include <string.h>
#include "Global.h"
using namespace std;

class MenuButton
{
    public:
        int X() { return x; }
        void X( int val ) { x = val; }
        int Y() { return y; }
        void Y( int val ) { y = val; }
        int W() { return w; }
        void W( int val ) { w = val; }
        int H() { return h; }
        void H( int val ) { h = val; }
        int FX() { return fx; }
        void FX( int val ) { fx = val; }
        int FY() { return fy; }
        void FY( int val ) { fy = val; }
        int Value() { return value; }
        string Caption() { return caption; }
        string Caption( string val ) { caption = val; }
        void Value( int val ) { value = val; }
        void Setup( int, int, int, int, int, int, string );
        void Setup( int );
        void Draw( BITMAP *image, BITMAP *buffer );
        int IsCollision( int cx, int cy );
        MenuButton() { w = h = 40; x = y = fx = fy = 0; caption = "none"; }
    private:
        int x, y, w, h, fx, fy, value;      //value is what it returns when hit
        string caption;
};

void MenuButton::Setup( int tx, int ty, int tw, int th, int tfx, int tfy, string tcapt )
{
    x = tx;
    y = ty;
    w = tw;
    h = th;
    fx = tfx;
    fy = tfy;
    caption = tcapt;
}

void MenuButton::Setup( int index )
{
    w = h = 40;
    value = index;
    switch ( index )
    {
        case 0:     //Plum
            x = 8;
            y = 32;
            fx = 0;
            fy = 192;
            caption = "P. Plum";
        break;

        case 1:     //Rank
            x = 72;
            y = 32;
            fx = 40;
            fy = 192;
            caption = "Rank";
        break;

        case 2:     //Marxio
            x = 136;
            y = 32;
            fx = 80;
            fy = 192;
            caption = "Marxio";
        break;

        case 3:     //Pookie
            x = 200;
            y = 32;
            fx = 120;
            fy = 192;
            caption = "Pookie";
        break;

        case 4:     //Name
            x = 0;
            y = 80;
            fx = 0;
            fy = 232;
            caption = "Name";
        break;

        case 5:     //Mars
            x = 52;
            y = 80;
            fx = 40;
            fy = 232;
            caption = "Mars";
        break;

        case 6:     //Wonky
            x = 104;
            y = 80;
            fx = 80;
            fy = 232;
            caption = "Wonky";
        break;

        case 7:     //Phonics
            x = 156;
            y = 80;
            fx = 120;
            fy = 232;
            caption = "Phonics";
        break;

        case 9:     //Brandon
            x = -100;
            y = -100;
            fx = 160;
            fy = 192;
            caption = "Chaotis";
        break;

        case 10:     //Rachel
            x = -100;
            y = -100;
            fx = 200;
            fy = 192;
            caption = "Lusikka";
        break;

        case 8:     //Random
            x = 208;
            y = 80;
            fx = 160;
            fy = 232;
            caption = "Random";
        break;
    }
}

void MenuButton::Draw( BITMAP *img, BITMAP *buffer )
{
    masked_blit( img, buffer, 112, 128, x, y, 48, 48 );
    masked_blit( img, buffer, fx, fy, x+4, y+4, w, h );
}

int MenuButton::IsCollision( int cx, int cy )
{
    //is a cursor hitting the button?  Also, return appropriate Value.
//    cout<<"\nPlayer ("<<cx<<", "<<cy<<")";
//    cout<<"\tX REGION "<<x<<" to "<<x+w;
//    cout<<"\tY REGION "<<y<<" to "<<y+h;
//    cout<<"\tCaption "<<caption<<endl;

    if ( (cx >= x && cx <= x + w) && (cy >= y && cy <= y+h ) )
    {
        return value;
    }
    return -1;
}



