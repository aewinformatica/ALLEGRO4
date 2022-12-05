#include "Color.h"

Color::Color()
{
    r = g = b = 150;
}

Color::Color( int tr, int tg, int tb )
{
    SetRGB( tr, tg, tb );
}

void Color::SetRGB( int tr, int tg, int tb )
{
    r = tr;
    g = tg;
    b = tb;
}

void Color::Lighten()
{
    if ( r+25 <= 255 )
        r += 25;
    if ( g+25 <= 255 )
        g += 25;
    if ( b+25 <= 255 )
        b += 25;
}

void Color::Darken()
{
    if ( r <= 255 )
        r -= 25;
    if ( g <= 255 )
        g -= 25;
    if ( b <= 255 )
        b -= 25;
}

