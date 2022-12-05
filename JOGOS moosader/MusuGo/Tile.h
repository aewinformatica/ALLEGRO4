#pragma once
#include <allegro.h>
#include <iostream>
using namespace std;

class Tile
{
    private:
        int x, y, w, h, fx;
        bool solid;
    public:
        Tile();
        void Setup( int tx, int ty, int tw, int th, int tfx, bool tsolid );
        void Draw( BITMAP *buffer, BITMAP *tileset, int xOff, int yOff );
        int X() { return x; }
        int Y() { return y; }
        void X( int val ) { x = val; }
        void Y( int val ) { y = val; }
        void FX( int val ) { fx = val; }
        int Fx() { return fx; }
};

