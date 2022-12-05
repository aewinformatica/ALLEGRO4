#include "Tile.h"

Tile::Tile()
{
    x = y = 0;
    w = h = 32;
}

void Tile::Setup( int tx, int ty, int tw, int th, int tfx, bool tsolid )
{
    x = tx; y = ty; w = tw; h = th; fx = tfx;
    solid = tsolid;
}

void Tile::Draw( BITMAP *buffer, BITMAP *tileset, int xOff, int yOff )
{
    if ( fx != 0 && fx != 1 )
    {
        masked_blit( tileset, buffer, fx, 0, x-xOff, y-yOff, w, h );
    }
    if ( fx == 1 )
    {
        line( buffer, x-xOff, y-yOff, x-xOff+w, y-yOff+h, makecol( 255, 0, 0 ) );
        line( buffer, x-xOff, y-yOff+h, x-xOff+w, y-yOff, makecol( 255, 0, 0 ) );
    }
}
