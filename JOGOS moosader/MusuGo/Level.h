#pragma once
#include <iostream>
#include <fstream>
#include <allegro.h>
#include "Tile.h"
#include "Brush.h"
#include "global.h"
#include "System.h"
using namespace std;

class Level
{
    private:
        int xOff, yOff;
        bool grid;
        SAMPLE *confirmSound;
        bool collisionLayer, drawMiniMap;
        int miniMapScale;
    public:
        Tile ***tile;
//        Level() { Setup(); }
        void Setup();
        void Setup( int, int );
        void Draw( BITMAP*, BITMAP*, int, int );
        void DrawMiniMap( BITMAP *buffer, BITMAP *tileset, int MAX_X, int MAX_Y, System *system );
        void ToggleGrid();
        void MoveOffsetX( int amt ) { xOff += amt; }
        void MoveOffsetY( int amt ) { yOff += amt; }
        void ScaleMiniMap( int amt );
        void ToggleMiniMap();
        void ToggleCollisionLayer() 
        { 
            if ( collisionLayer == true )
                collisionLayer = false;
            else
                collisionLayer = true;
        }
        bool IsEditingCollisionLayer() { return collisionLayer; }
        int OffsetX() { return xOff; }
        int OffsetY() { return yOff; }
        void OffsetX( int val ) { xOff = val; }
        void OffsetY( int val ) { yOff = val; }
        void CheckClick( Brush*, int, int, int );
        void FillLayer( int, int, int, int );
        void ClearLayer( int, int, int );
        void LoadMap( string, int*, int* );
        void SaveMap( string, int, int );
        void FreeTiles();
        void AllocateTiles( int, int, int );
        ~Level()
        {
            FreeTiles();
        }
};







