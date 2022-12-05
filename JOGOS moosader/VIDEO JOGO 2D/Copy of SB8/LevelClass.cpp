//LevelClass.cpp
#include "LevelClass.h"

void LevelClass::DrawBottom(BITMAP *buffer, BITMAP *tileset, float offsetX, float offsetY)
{
    //Background
    masked_blit(background, buffer, 0, 0, 0, 0, 640, 480);
    //Bottom and middlelayer
    for (int i=0; i<TILEX; i++)
    {
        for (int j=0; j<TILEY; j++)
        {
            if ( tile[0][i][j].cx != 0 )
                masked_blit(tileset, buffer, tile[0][i][j].cx, tile[0][i][j].cy, tile[0][i][j].x - offsetX, tile[0][i][j].y - offsetY, tile[0][i][j].w, tile[0][i][j].h);
            if ( tile[1][i][j].cx != 0 )
                masked_blit(tileset, buffer, tile[1][i][j].cx, tile[1][i][j].cy, tile[1][i][j].x - offsetX, tile[1][i][j].y - offsetY, tile[1][i][j].w, tile[1][i][j].h);
        }
    }
}

void LevelClass::DrawTop(BITMAP *buffer, BITMAP *tileset, float offsetX, float offsetY)
{
    //Top layer
    for (int i=0; i<TILEX; i++)
    {
        for (int j=0; j<TILEY; j++)
        {
            if ( tile[2][i][j].cx != 0 )        //if the tile isn't blank then draw it.
                masked_blit(tileset, buffer, tile[2][i][j].cx, tile[2][i][j].cy, tile[2][i][j].x - offsetX, tile[2][i][j].y - offsetY, tile[2][i][j].w, tile[2][i][j].h);
        }
    }
}

void LevelClass::LoadMap(string filename)
{
    ifstream infile;
    infile.open(filename.c_str());
    string temp;
    infile>>temp;       //x
    infile>>temp;       //160
    infile>>temp;       //y
    infile>>temp;       //60
    infile>>temp;       //bgimage
    infile>>temp;       //background
    background = load_bitmap(temp.c_str(), NULL);

    //bottom layer
    for (int j=0; j<TILEY; j++)
    {
        for(int i=0; i<TILEX; i++)
        {
            infile>>temp;
            tile[0][i][j].cx = atoi ( temp.c_str() ) * 32;
            tile[0][i][j].x = i * 32;
            tile[0][i][j].y = j * 32;
            if ( tile[0][i][j].cx >= 1*32 && tile[0][i][j].cx <= 6*32 )
                tile[0][i][j].solid = true;     //ground tile
            if ( tile[0][i][j].cx >= 7 * 32 && tile[0][i][j].cx <= 13*32)
                tile[0][i][j].halfsolid = true;
            if ( tile[0][i][j].cx == 14 || tile[0][i][j].cx == 15 )
                tile[0][i][j].deadly = true;
        }
    }
    //middle layer
    for (int j=0; j<TILEY; j++)
    {
        for(int i=0; i<TILEX; i++)
        {
            infile>>temp;
            tile[1][i][j].cx = atoi ( temp.c_str() ) * 32;
            tile[1][i][j].x = i * 32;
            tile[1][i][j].y = j * 32;
            if ( tile[1][i][j].cx >= 1*32 && tile[1][i][j].cx <= 6*32 )
                tile[1][i][j].solid = true;     //ground tile
            if ( tile[1][i][j].cx >= 7 * 32 && tile[1][i][j].cx <= 13*32)
                tile[1][i][j].halfsolid = true;
            if ( tile[1][i][j].cx == 14 || tile[1][i][j].cx == 15 )
                tile[1][i][j].deadly = true;
        }
    }

    //top layer
    for (int j=0; j<TILEY; j++)
    {
        for(int i=0; i<TILEX; i++)
        {
            infile>>temp;
            tile[2][i][j].cx = atoi ( temp.c_str() ) * 32;
            tile[2][i][j].x = i * 32;
            tile[2][i][j].y = j * 32;
        }
    }
}