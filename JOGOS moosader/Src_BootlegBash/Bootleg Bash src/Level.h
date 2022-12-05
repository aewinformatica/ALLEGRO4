#pragma once
#include "Tiles.h"
#include <allegro.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

const int MAXTILEX = 16;
const int MAXTILEY = 15;
const int TILEWIDTH = 16;

enum { WATERA  = 38, WATERB = 39 };

class Level
{
    private:
        int startX[4], startY[4];
        int bgcolor;
    public:
        void LoadMap( string filename );
        void DrawBottom( BITMAP *buffer, BITMAP *tileset );
        void DrawTop( BITMAP *buffer, BITMAP *tileset );
        int StartX( int index ) { return startX[index]; }
        int StartY( int index ) { return startY[index]; }
        Tile tile[3][MAXTILEX][MAXTILEY];       //layer, x, y
};

void Level::DrawBottom( BITMAP *buffer, BITMAP *tileset )
{
    //draw background
    rectfill( buffer, 0, 0, MAXTILEX*16, MAXTILEY*16, bgcolor );
    for ( int i=0; i<MAXTILEX; i++ )
    {
        for ( int j=0; j<MAXTILEY; j++ )
        {
            if ( tile[0][i][j].FX() != 0 )
            {
                masked_blit( tileset, buffer, tile[0][i][j].FX(), tile[0][i][j].FY(), tile[0][i][j].X(), tile[0][i][j].Y(), tile[0][i][j].W(), tile[0][i][j].H() );
////TEMP        (bounding boxes for debug)
//           if ( tile[0][i][j].Left() > -1 )
//                {
//                    int x = tile[0][i][j].X();
//                    int y = tile[0][i][j].Y();
//                    rect( buffer, x+tile[0][i][j].Left(), y+tile[0][i][j].Top(), x+tile[0][i][j].Right(), y+tile[0][i][j].Bottom(), makecol( 255, 0, 0 ) );
//                }
//END TEMP
            }
            if ( tile[1][i][j].FX() != 0 )
            {
                masked_blit( tileset, buffer, tile[1][i][j].FX(), tile[1][i][j].FY(), tile[1][i][j].X(), tile[1][i][j].Y(), tile[1][i][j].W(), tile[1][i][j].H() );
////TEMP
//                if ( tile[1][i][j].Left() > -1 )
//                {
//                    int x = tile[1][i][j].X();
//                    int y = tile[1][i][j].Y();
//                    rect( buffer, x+tile[1][i][j].Left(), y+tile[1][i][j].Top(), x+tile[1][i][j].Right(), y+tile[1][i][j].Bottom(), makecol( 0, 255, 200 ) );
//                }
////END TEMP
            }
        }
    }
}

void Level::DrawTop( BITMAP *buffer, BITMAP *tileset )
{
    for ( int i=0; i<MAXTILEX; i++ )
    {
        for ( int j=0; j<MAXTILEY; j++ )
        {
            if ( tile[2][i][j].FX() != 0 )
            {
                masked_blit( tileset, buffer, tile[2][i][j].FX(), tile[2][i][j].FY(), tile[2][i][j].X(), tile[2][i][j].Y(), tile[2][i][j].W(), tile[2][i][j].H() );
            }
        }
    }
}

void Level::LoadMap( string filename )
{
    ifstream infile;
    infile.open( filename.c_str() );
    string temp;
    infile>>temp;       //x
    infile>>temp;       //160
    infile>>temp;       //y
    infile>>temp;       //60
    infile>>temp;       //bgimage
    infile>>temp;       //background
    int t = -1;
    //load bottom layer
    for ( int j=0; j<MAXTILEY; j++ )
    {
        for ( int i=0; i<MAXTILEX; i++ )
        {
            infile>>temp;
            tile[0][i][j].SetFX ( (atoi( temp.c_str() ) * TILEWIDTH) );
            tile[0][i][j].SetFY ( 0 );
            tile[0][i][j].SetX ( i * TILEWIDTH );
            tile[0][i][j].SetY ( j * TILEWIDTH );
            tile[0][i][j].SetW( TILEWIDTH );
            tile[0][i][j].SetH( TILEWIDTH );
            tile[0][i][j].SetRegions( -1, -1, -1, -1 );

            t = tile[0][i][j].FX();// / TILEWIDTH;
            if (   t == 16 || t == 32 || t == 48 || t == 64 || t == 144 ||  t == 160 || t == 240 || t == 256 || t == 320 || t == 272 || t == 400 || t == 496 || t == 544 || t == 560 || t == 576 || t == 392 || t == 608 || t == 624 || t == 640 || t == 720 || t == 784 || t == 800 || t == 816 || t == 944 || t == 1056 || t == 1072 || t == 1088 )
            {
                //fully solid
                tile[0][i][j].SetRegions( 0, 16, 0, 16 );       // Left right top bottom
            }
            else if ( t == WATERA || t == WATERB )
            {
                tile[0][i][j].SetDeadliness( true );
                tile[0][i][j].SetRegions( 0, 16, 0, 16 );
            }
//            else if ( t == BRIDGEA || t == BRIDGEB )
//            {
//                tile[0][i][j].SetRegions( 0, 16, 0, 5 );
//            }
        }
    }

    //load middle layer
    for ( int j=0; j<MAXTILEY; j++ )
    {
        for ( int i=0; i<MAXTILEX; i++ )
        {
            infile>>temp;
            tile[1][i][j].SetFX ( (atoi( temp.c_str() ) * TILEWIDTH) );
            tile[1][i][j].SetFY ( 0 );
            tile[1][i][j].SetX ( i * TILEWIDTH );
            tile[1][i][j].SetY ( j * TILEWIDTH );
            tile[1][i][j].SetW( TILEWIDTH );
            tile[1][i][j].SetH( TILEWIDTH );
            tile[1][i][j].SetRegions( -1, -1, -1, -1 );

            t = tile[1][i][j].FX();// / TILEWIDTH;
            if (   t == 16 || t == 32 || t == 48 || t == 64 || t == 144 || t == 160 || t == 240 || t == 256 || t == 320 || t == 272 || t == 400 || t == 496 || t == 544 || t == 560 || t == 576 || t == 392 || t == 608 || t == 624 || t == 640 || t == 720 || t == 784 || t == 800 || t == 816 || t == 944 || t == 1056 || t == 1072 || t == 1088 )
            {
                //fully solid
                tile[1][i][j].SetRegions( 0, 16, 0, 16 );       // Left right top bottom
            }
            else if ( t == WATERA || t == WATERB )
            {
                //deadly
                tile[1][i][j].SetDeadliness( true );
                tile[1][i][j].SetRegions( 0, 16, 0, 16 );
            }
//            else if ( t == BRIDGEA || t == BRIDGEB )
//            {
//                tile[1][i][j].SetRegions( 0, 16, 0, 5 );
//            }
        }
    }

    //load top layer
    for ( int j=0; j<MAXTILEY; j++ )
    {
        for ( int i=0; i<MAXTILEX; i++ )
        {
            infile>>temp;
            tile[2][i][j].SetFX ( (atoi( temp.c_str() ) * TILEWIDTH) );
            tile[2][i][j].SetFY ( 0 );
            tile[2][i][j].SetX ( i * TILEWIDTH );
            tile[2][i][j].SetY ( j * TILEWIDTH );
            tile[2][i][j].SetW( TILEWIDTH );
            tile[2][i][j].SetH( TILEWIDTH );
        }
    }

    if ( filename == "map0.map" )
    {
        startX[0] = 0 * TILEWIDTH;
        startY[0] = 3 * TILEWIDTH;
        startX[1] = 6 * TILEWIDTH;
        startY[1] = 2 * TILEWIDTH;
        startX[2] = 11 * TILEWIDTH;
        startY[2] = 3 * TILEWIDTH;
        startX[3] =15 * TILEWIDTH;
        startY[3] = 3 * TILEWIDTH;
        bgcolor = makecol( 64, 40, 196 );
    }
    else if ( filename == "map1.map" )
    {
        startX[0] = 1 * TILEWIDTH;
        startY[0] = 6 * TILEWIDTH;
        startX[1] = 14 * TILEWIDTH;
        startY[1] = 6 * TILEWIDTH;
        startX[2] = 5 * TILEWIDTH;
        startY[2] = 11 * TILEWIDTH;
        startX[3] = 10 * TILEWIDTH;
        startY[3] = 11 * TILEWIDTH;
        bgcolor = makecol( 172, 0, 40 );
    }
    else if ( filename == "map2.map" )
    {
        startX[0] = 1 * TILEWIDTH;
        startY[0] = 3 * TILEWIDTH;
        startX[1] = 7 * TILEWIDTH;
        startY[1] = 3 * TILEWIDTH;
        startX[2] = 12 * TILEWIDTH;
        startY[2] = 7 * TILEWIDTH;
        startX[3] = 12 * TILEWIDTH;
        startY[3] = 11 * TILEWIDTH;
        bgcolor = makecol( 56, 192, 248 );
    }
    else if ( filename == "map3.map" )
    {
        startX[0] = 3 * TILEWIDTH;
        startY[0] = 3 * TILEWIDTH;
        startX[1] = 12 * TILEWIDTH;
        startY[1] = 7 * TILEWIDTH;
        startX[2] = 9 * TILEWIDTH;
        startY[2] = 10 * TILEWIDTH;
        startX[3] =2 * TILEWIDTH;
        startY[3] = 9 * TILEWIDTH;
        bgcolor = makecol( 0, 64, 88 );
    }
    else if ( filename == "map4.map" )
    {
        startX[0] = 0 * TILEWIDTH;
        startY[0] = 6 * TILEWIDTH;
        startX[1] = 5 * TILEWIDTH;
        startY[1] = 8 * TILEWIDTH;
        startX[2] = 11 * TILEWIDTH;
        startY[2] = 7 * TILEWIDTH;
        startX[3] =15 * TILEWIDTH;
        startY[3] = 4 * TILEWIDTH;
        bgcolor = makecol( 176, 240, 216 );
    }
    else if ( filename == "map5.map" )
    {
        startX[0] = 1 * TILEWIDTH;
        startY[0] = 6 * TILEWIDTH;
        startX[1] = 14 * TILEWIDTH;
        startY[1] = 6 * TILEWIDTH;
        startX[2] = 1 * TILEWIDTH;
        startY[2] = 11 * TILEWIDTH;
        startX[3] = 14 * TILEWIDTH;
        startY[3] = 11* TILEWIDTH;
        bgcolor = makecol( 164, 232, 252 );
    }
}

