#include "Button.h"

Button::Button()
{
    visible = true;
}

Button::Button( int tindex, string tid )
{
    visible = true;
    Setup( tindex, tid );
}

void Button::Draw( BITMAP *buffer, BITMAP *image, int winX, int winY )
{
    if ( visible )
    {
        if ( enabled )
        {
            rectfill( buffer, x1+winX, y1+winY, x2+winX, y2+winY, makecol( color[4].r, color[4].g, color[4].b ) );
        }
        else if ( clicked )
        {
            rectfill( buffer, x1+winX, y1+winY, x2+winX, y2+winY, makecol( color[5].r, color[5].g, color[5].b ) );
        }
        else if ( highlight )
        {
            rectfill( buffer, x1+winX, y1+winY, x2+winX, y2+winY, makecol( color[6].r, color[6].g, color[6].b ) );
        }
        else
        {
            rectfill( buffer, x1+winX, y1+winY, x2+winX, y2+winY, makecol( color[1].r, color[1].g, color[1].b ) );
        }
        rect( buffer, x1+winX, y1+winY, x2+winX, y2+winY, makecol( 255, 255, 255 ) );
        rect( buffer, x1+winX, y1+winY, x2+winX, y2+winY, makecol( color[2].r, color[2].g, color[2].b ) );
        rect( buffer, x1+winX, y1+winY, x2+winX-1, y2+winY-1, makecol( color[0].r, color[0].g, color[0].b ) );

        if ( btnImgX > -1 )
        {
            masked_blit( image, buffer, btnImgX, 0, x1+winX, y1+winY, x2-x1, y2-y1 );
        }
        else
        {
            textprintf_centre_ex( buffer, font, (x1+x2)/2+winX, (y1+y2)/2-2+winY, makecol( color[3].r, color[3].g, color[3].b ), -1, caption.c_str() );
        }
    }
}

void Button::Setup( int tindex )
{
    string tid;

    switch ( tindex )
    {
        case 0:
            tid = "save";
            break;
        case 1:
            tid = "load";
            break;
        case 2:
            tid = "exit";
            break;
        case 3:
            tid = "fullscreen";
            break;
        case 4:
            tid = "L1";
            break;
        case 5:
            tid = "L2";
            break;
        case 6:
            tid = "L3";
            break;
        case 7:
            tid = "S1";
            break;
        case 8:
            tid = "S3";
            break;
        case 9:
            tid = "fill";
            break;
        case 10:
            tid = "cls";
            break;
        case 11:
            tid = "chara editor";
            break;
        case 12:
            tid = "item editor";
            break;
        case 13:
            tid = "add warp";
            break;
    }

    Setup( tindex, tid );
}

void Button::Setup( int tindex, string tid )
{
    //0-light, 1-base, 2-dark, 3-text, 4-enabled, 5-clicked, 6-highlighted
    color[0].SetRGB( 175, 175, 255 );
    color[1].SetRGB( 125, 125, 200 );
    color[2].SetRGB( 75, 75, 175 );
    color[3].SetRGB( 255, 255, 255 );
    color[4].SetRGB( 255, 100, 255 );
    color[5].SetRGB( 100, 100, 175 );
    color[6].SetRGB( 200, 200, 255 );

    clicked = false;
    enabled = false;
    highlight = false;
    visible = true;

    id = tid;
    index = tindex;

    btnImgX = -1;
    x1 = y1 = x2 = y2 = btnImgY = 0;

    if ( id == "save" )
    {
        x1 = 10;
        y1 = 25;
        x2 = 30;
        y2 = 45;
        btnImgX = 180;
    }
    else if ( id == "load" )
    {
        x1 = 40;
        y1 = 25;
        x2 = 60;
        y2 = 45;
        btnImgX = 160;
    }
    else if ( id == "exit" )
    {
        x1 = 10;
        y1 = 55;
        x2 = 30;
        y2 = 75;
        btnImgX = 200;
    }
    else if ( id == "fullscreen" )
    {
        x1 = 40;
        y1 = 55;
        x2 = 60;
        y2 = 75;
        btnImgX = 220;
    }
    else if ( id == "L1" ) //layers
    {
        x1 = 100;
        y1 = 30;
        x2 = 120;
        y2 = 50;
        enabled = true;
        btnImgX = 80;
    }
    else if ( id == "L2" )
    {
        x1 = 130;
        y1 = 30;
        x2 = 150;
        y2 = 50;
        btnImgX = 100;
    }
    else if ( id == "L3" )
    {
        x1 = 160;
        y1 = 30;
        x2 = 180;
        y2 = 50;
        btnImgX = 120;
    }
    else if ( id == "S1" )
    {
        x1 = 100;
        y1 = 70;
        x2 = 120;
        y2 = 90;
        btnImgX = 0;
        enabled = true;
    }
    else if ( id == "S3" )
    {
        x1 = 130;
        y1 = 70;
        x2 = 150;
        y2 = 90;
        btnImgX = 20;
    }
    else if ( id == "fill" )
    {
        x1 = 160;
        y1 = 70;
        x2 = 180;
        y2 = 90;
        btnImgX = 60;
    }
    else if ( id == "cls" )
    {
        x1 = 190;
        y1 = 70;
        x2 = 210;
        y2 = 90;
        btnImgX = 40;
    }
    else if ( id == "add warp" )
    {
        x1 = 225;
        y1 = 25;
        x2 = 245;
        y2 = 45;
        btnImgX = 140;
    }
    else if ( id == "chara editor" )
    {
        x1 = 870;
        y1 = 25;
        x2 = 1014;
        y2 = 45;
    }
    else if ( id == "item editor" )
    {
        x1 = 870;
        y1 = 55;
        x2 = 1014;
        y2 = 75;
    }
    else if ( id == "<-" )
    {
        x1 = 47;
        y1 = 2;
        x2 = 67;
        y2 = 22;
        btnImgX = 320;
    }
    else if ( id == "->" )
    {
        x1 = 69;
        y1 = 2;
        x2 = 89;
        y2 = 22;
        btnImgX = 340;
    }
    else if ( id == "<<" )
    {
        x1 = 25;
        y1 = 2;
        x2 = 45;
        y2 = 22;
        btnImgX = 280;
    }
    else if ( id == ">>" )
    {
        x1 = 92;
        y1 = 2;
        x2 = 112;
        y2 = 22;
        btnImgX = 300;
    }
    else if ( id == "|<" )
    {
        x1 = 3;
        y1 = 2;
        x2 = 23;
        y2 = 22;
        btnImgX = 240;
    }
    else if ( id == ">|" )
    {
        x1 = 115;
        y1 = 2;
        x2 = 135;
        y2 = 22;
        btnImgX = 260;
    }
}