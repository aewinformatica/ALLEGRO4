#include "Brush.h"


Brush::Brush()
{
    click = false;
    layer = 0;
    fx = 0;
    size = 1;
    color[0].SetRGB( 255, 255, 255 );
    color[1].SetRGB( 100, 100, 255 );
}

void Brush::Draw( BITMAP *buffer )
{
    int i;
    if ( click )
        i = 1;
    else
        i = 0;

    circlefill( buffer, mouse_x, mouse_y, 3, makecol( color[i].r-100, color[i].g-100, color[i].b-100 ) );
    circlefill( buffer, mouse_x+6, mouse_y, 3, makecol( color[i].r-100, color[i].g-100, color[i].b-100 ) );
    circlefill( buffer, mouse_x+12, mouse_y, 3, makecol( color[i].r-100, color[i].g-100, color[i].b-100 ) );
    circlefill( buffer, mouse_x, mouse_y+6, 3, makecol( color[i].r-100, color[i].g-100, color[i].b-100 ) );
    circlefill( buffer, mouse_x, mouse_y+12, 3, makecol( color[i].r-100, color[i].g-100, color[i].b-100 ) );
    circlefill( buffer, mouse_x+6, mouse_y+6, 3, makecol( color[i].r-100, color[i].g-100, color[i].b-100 ) );
    circlefill( buffer, mouse_x+12, mouse_y+12, 3, makecol( color[i].r-100, color[i].g-100, color[i].b-100 ) );

    circlefill( buffer, mouse_x, mouse_y, 1, makecol( color[i].r, color[i].g, color[i].b ) );
    circlefill( buffer, mouse_x+6, mouse_y, 1, makecol( color[i].r, color[i].g, color[i].b ) );
    circlefill( buffer, mouse_x+12, mouse_y, 1, makecol( color[i].r, color[i].g, color[i].b ) );
    circlefill( buffer, mouse_x, mouse_y+6, 1, makecol( color[i].r, color[i].g, color[i].b ) );
    circlefill( buffer, mouse_x, mouse_y+12, 1, makecol( color[i].r, color[i].g, color[i].b ) );
    circlefill( buffer, mouse_x+6, mouse_y+6, 1, makecol( color[i].r, color[i].g, color[i].b ) );
    circlefill( buffer, mouse_x+12, mouse_y+12, 1, makecol( color[i].r, color[i].g, color[i].b ) );
}
