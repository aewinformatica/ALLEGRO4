#include "ImageManager.h"

ImageManager::ImageManager()
{
    buffer = create_bitmap( 640, 480 );
    SPRITE[0].Setup( "background" );
    SPRITE[1].Setup( "jitensha" );
    SPRITE[2].Setup( "nomi" );
    SPRITE[3].Setup( "sakana" );
    SPRITE[4].Setup( "toire" );
    SPRITE[5].Setup( "gptitle" );
    SPRITE[6].Setup( "goat" );
}

ImageManager::~ImageManager()
{
    if ( buffer != NULL )
        destroy_bitmap( buffer );
        
    for ( int i=0; i<QTD; i++ )
    {
        if ( SPRITE[i].img != NULL )
            destroy_bitmap( SPRITE[i].img );
    }
}

void ImageManager::Draw( string name, int x, int y, int w, int h, int fx, int fy )
{
    bool success = false;
    for ( int i=0; i<QTD; i++ )
    {
        if ( SPRITE[i].description == name )
        {
            Draw( i, x, y, w, h, fx, fy );
            success = true;
        }
    }
    if ( !success ) { cerr<<"Error drawing "<<name<<endl; }
}

void ImageManager::Draw( int index, int x, int y, int w, int h, int fx, int fy )
{
    masked_blit( SPRITE[index].img, buffer, fx, fy, x, y, w, h );
}


