#include "ImagemManager.h"

ImageManager::ImageManager()
{
    Buffer = create_bitmap( 640, 480 );
    SPRITE[0].Carregar( "Player" );
    SPRITE[1].Carregar( "Mapa" );
    SPRITE[2].Carregar( "bg1" );
    SPRITE[3].Carregar( "Cenario-02" );
    SPRITE[4].Carregar( "Cenario-03" );
    SPRITE[5].Carregar( "Efeito-01" );
    SPRITE[6].Carregar( "Icones-01" );
    SPRITE[7].Carregar( "Plataforma-01" );
    SPRITE[8].Carregar( "Plataforma-02" );
    
}

ImageManager::~ImageManager()
{
    if ( Buffer != NULL )
        destroy_bitmap( Buffer );
        
    for ( int i=0; i<QTD; i++ )
    {
        if ( SPRITE[i].img != NULL )
            destroy_bitmap( SPRITE[i].img );
    }
}

void ImageManager::Draw( string nome, int x, int y, int w, int h, int fx, int fy )
{
    bool success = false;
    for ( int i=0; i<QTD; i++ )
    {
        if ( SPRITE[i].descricao == nome )
        {
            Draw( i, x, y, w, h, fx, fy );
            success = true;
        }
    }
    if ( !success ) { cerr<<"Error drawing "<<nome<<endl; }
}

void ImageManager::Draw( int index, int x, int y, int w, int h, int fx, int fy )
{
    masked_blit( SPRITE[index].img, Buffer, fx, fy, x, y, w, h );
}


