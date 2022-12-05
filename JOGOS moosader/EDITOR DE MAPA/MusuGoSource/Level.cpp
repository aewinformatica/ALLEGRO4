#include "Level.h"

void Level::Setup()
{
    collisionLayer = false;
    xOff = -64;
    yOff = -96;
    confirmSound = load_sample( "confirm.wav" );
    if ( !confirmSound ) { cerr<<"error loading confirm.wav"<<endl; }
    drawMiniMap = true;
    miniMapScale = 6;
}

void Level::ScaleMiniMap( int amt ) 
{ 
    if ( miniMapScale + amt > 1 )
        miniMapScale += amt; 
}

void Level::FreeTiles()
{
    for ( int i=0; i<3; i++ )
    {
        for ( int j=0; j<sizeof( tile[i] ); j++ )
        {
            for ( int k=0; k<sizeof( tile[i][j] ); k++ )
            {
                free( tile[i][j] );
                tile[i][j] = NULL;
            }
        }
    }
    delete [] tile;
}

void Level::AllocateTiles( int MAX_X, int MAX_Y, int LAYERAMT )
{
    tile = new Tile**[LAYERAMT];
    for ( int i=0; i<LAYERAMT; i++ )
    {
        tile[i]= new Tile*[MAX_X];
        for ( int j=0; j<MAX_X; j++ )
        {
            tile[i][j] = new Tile[MAX_Y];
        }
    }
}

void Level::Setup( int MAX_X, int MAX_Y )
{
    drawMiniMap = true;
    miniMapScale = 6;
    collisionLayer = false;
    confirmSound = load_sample( "confirm.wav" );
    if ( !confirmSound ) { cerr<<"error loading confirm.wav"<<endl; }
    xOff = -64;
    yOff = -96;
    //xOff = yOff = 0;
    grid = true;
    for ( int i=0; i<MAX_X; i++ )
    {
        for ( int j=0; j<MAX_Y; j++ )
        {
            //x, y, w, h, filmstrip x, solidity
            tile[0][i][j].Setup( i*32, j*32, 32, 32, 0, false );
            tile[1][i][j].Setup( i*32, j*32, 32, 32, 0, false );
            tile[2][i][j].Setup( i*32, j*32, 32, 32, 0, false );
            tile[3][i][j].Setup( i*32, j*32, 32, 32, 0, false );
        }
    }
}

void Level::ToggleGrid()
{
    if ( grid )
        grid = false;
    else
        grid = true;
}

void Level::Draw( BITMAP *buffer, BITMAP *tileset, int MAX_X, int MAX_Y )
{
    for ( int i=0; i<MAX_X; i++ )
    {
        for ( int j=0; j<MAX_Y; j++ )
        {
            if ( tile[0][i][j].X() >= xOff-32 && tile[0][i][j].X() <= xOff+1024 &&
                    tile[0][i][j].Y() >= yOff-32 && tile[0][i][j].Y() <= yOff+768 )
            {
                //x, y, w, h, filmstrip x, solidity
                tile[0][i][j].Draw( buffer, tileset, xOff, yOff );
                tile[1][i][j].Draw( buffer, tileset, xOff, yOff );
                tile[2][i][j].Draw( buffer, tileset, xOff, yOff );
                tile[3][i][j].Draw( buffer, tileset, xOff, yOff );
                
                if ( grid )
                    rect( buffer, i*32-xOff, j*32-yOff, i*32+32-xOff, j*32+32-yOff, makecol( 100, 100, 100 ) );

                textprintf_centre_ex( buffer, font, i*32+16-xOff, 64, makecol( 255, 255, 255 ), -1, "%i", i );
                textprintf_ex( buffer, font, 48, 16+j*32-yOff, makecol( 255, 255, 255 ), -1, "%i", j );
            }
        }
    }
}

void Level::ToggleMiniMap()
{
    if ( drawMiniMap )
        drawMiniMap = false;
    else
        drawMiniMap = true;
}

void Level::DrawMiniMap( BITMAP *buffer, BITMAP *tileset, int MAX_X, int MAX_Y, System *system )
{
    if ( drawMiniMap )
    {
        int x = 370, y = 670;
        int w = MAX_X*32, h = MAX_Y*32;
        int scale = miniMapScale;
    
        BITMAP *temp = create_bitmap( w, h );
        
        for ( int i=0; i<MAX_X; i++ )
        {
            for ( int j=0; j<MAX_Y; j++ )
            {
                for ( int k=0; k<3; k++ )
                {
                    tile[0][i][j].Draw( temp, tileset, 0, 0 );
                    tile[1][i][j].Draw( temp, tileset, 0, 0 );
                    tile[2][i][j].Draw( temp, tileset, 0, 0 );                       
                }
            }
        }
        
        //stretch_blit(src, dest, xsrc, ysrc, srcw, srch, xdest, ydest, destw, desth)
            stretch_blit( temp, buffer, 
                0, 0, w, h, x, y, w/scale, h/scale );
        
        destroy_bitmap( temp );
    
    }
}

void Level::CheckClick( Brush *mouse, int click, int MAX_X, int MAX_Y )
{
    //See if a tile was clicked
    int mx = ( mouse_x + xOff ) / 32;
    int my = ( mouse_y + yOff ) / 32;
    
    int tLayer = mouse->Layer();
    int tBrush = mouse->Fx();
    
    if ( collisionLayer )
    {
        tLayer = 3;
        tBrush = 1;
    }
    
    if ( click != 1 )
        tBrush = 0;

    if ( mx < MAX_X && my < MAX_Y ) 
    {
        tile[ tLayer ][ mx ][ my ].FX ( tBrush );
    }
    
    if ( mouse->Size() == 3 )
    {
        if ( mx-1 < MAX_X && my < MAX_Y ) 
            tile[ tLayer ][ mx-1 ][ my ].FX ( tBrush );
        if ( mx+1 < MAX_X && my < MAX_Y ) 
            tile[ tLayer ][ mx+1 ][ my ].FX ( tBrush );
        if ( mx < MAX_X && my-1 < MAX_Y ) 
            tile[ tLayer ][ mx ][ my-1 ].FX ( tBrush );
        if ( mx < MAX_X && my+1 < MAX_Y ) 
            tile[ tLayer ][ mx ][ my+1 ].FX ( tBrush );
        if ( mx-1 < MAX_X && my-1 < MAX_Y ) 
            tile[ tLayer ][ mx-1 ][ my-1 ].FX ( tBrush );
        if ( mx-1 < MAX_X && my+1 < MAX_Y ) 
            tile[ tLayer ][ mx-1 ][ my+1 ].FX ( tBrush );
        if ( mx+1 < MAX_X && my-1 < MAX_Y ) 
            tile[ tLayer ][ mx+1 ][ my-1 ].FX ( tBrush );
        if ( mx+1 < MAX_X && my+1 < MAX_Y ) 
            tile[ tLayer ][ mx+1 ][ my+1 ].FX ( tBrush );
    }
}

void Level::FillLayer( int theLayer, int theTile, int MAX_X, int MAX_Y )
{
    for ( int i=0; i<MAX_X; i++ )
    {
        for ( int j=0; j<MAX_Y; j++ )
        {
            tile[ theLayer ][i][j].FX( theTile );
        }
    }
}

void Level::ClearLayer( int theLayer, int MAX_X, int MAX_Y )
{
    for ( int i=0; i<MAX_X; i++ )
    {
        for ( int j=0; j<MAX_Y; j++ )
        {
            tile[ theLayer ][i][j].FX( 0 );
        }
    }
}

void Level::SaveMap( string filename, int MAX_X, int MAX_Y )
{
    cout<<"Saving to "<<filename<<"..."<<endl;
    ofstream outfile;
    outfile.open( filename.c_str() );
    outfile<<"W: "<<MAX_X<<"\tH: "<<MAX_Y<<"\tv 1";
    outfile<<"\nbgimage none"<<endl;
    for ( int k=0; k<4; k++ )               //layer
    {
        for ( int j=0; j<MAX_Y; j++ )       //y
        {
            for ( int i=0; i<MAX_X; i++ )   //x
            {
                if ( k == 3 && tile[k][i][j].Fx() == 1 )
                {
                    outfile<<"001 ";
                }
                else if ( tile[k][i][j].Fx()/32 < 10 )
                {
                    outfile<<"000"<<tile[k][i][j].Fx()/32<<" ";
                }
                else if ( tile[k][i][j].Fx()/32 < 100 )
                {
                    outfile<<"00"<<tile[k][i][j].Fx()/32<<" ";
                }
                else if ( tile[k][i][j].Fx()/32 < 1000 )
                {
                    outfile<<"0"<<tile[k][i][j].Fx()/32<<" ";
                }
                else
                {
                    outfile<<tile[k][i][j].Fx()/32<<" ";
                }
            }
            outfile<<endl;
        }
        outfile<<endl<<endl;
    }
    outfile.close();
    cout<<"Save complete"<<endl;
    //if ( confirmSound != NULL )
        play_sample( confirmSound, 255, 128, 1000, false );
}

void Level::LoadMap( string filename, int *MAX_X, int *MAX_Y )
{
    cout<<"Loading from "<<filename<<"..."<<endl;
    ifstream infile;
    infile.open( filename.c_str() );
    string temp;
    
    int mapVer;
    
    int newMaxX, newMaxY;

    infile>>temp;   //x
    infile>>newMaxX;   //#
    infile>>temp;   //y
    infile>>newMaxY;   //#
    infile>>temp;   //v
    infile>>mapVer; //map version format
    infile>>temp;   //bg image
    infile>>temp;   //filename
    
    *MAX_X = newMaxX;
    *MAX_Y = newMaxY;  
    
    int layers;
    
    if ( mapVer == 0 )
        layers = 3;
    else if ( mapVer > 0 )
        layers = 4;
      
    FreeTiles();                    // Clear out map  
    AllocateTiles( newMaxX, newMaxY, 4 );     // Setup new map

    for ( int k=0; k<layers; k++ )
    {
        for ( int j=0; j<*MAX_Y; j++ )
        {
            for ( int i=0; i<*MAX_X; i++ )
            {
                int temp;
                bool solid = false;
                infile>>temp;
                if ( temp == 1 )                 // collision layer
                    tile[k][i][j].FX( 1 );
                else
                    tile[k][i][j].FX( temp*32 );
                
                //void Setup( int tx, int ty, int tw, int th, int tfx, bool tsolid );
                if ( k != 3 ) { solid = false; }
                else if ( k == 3 && tile[k][i][j].Fx() == 1 ) { solid = true; }
                
                tile[k][i][j].Setup( i*32, j*32, 32, 32, temp*32, solid );
                
            }
        }
    }
    infile.close();
    cout<<"Load complete"<<endl;
    //if ( confirmSound != NULL )
        play_sample( confirmSound, 255, 128, 1000, false );
}
