#pragma once
#include <iostream>
using namespace std;

class Tile
{
    private:
        int x, y, w, h, fx, fy, rLeft, rRight, rTop, rBottom;
        bool deadly;
    public:
        Tile();
        void Setup( int tehX, int tehY, int tehW, int tehH, int tehFilmstripX, int tehFilmstripY, int tehLeftRegion, int tehRightRegion, int tehTopRegion, int tehBottomRegion );
        void SetFX( int val ) { fx = val; }
        void SetFY( int val ) { fy = val; }
        void SetX( int val ) { x = val; }
        void SetY( int val ) { y = val; }
        void SetW( int val ) { w = val; }
        void SetH( int val ) { h = val; }
        void SetRegions( int L, int R, int T, int B ) { rLeft = L; rRight = R; rTop = T; rBottom = B; }
        void SetDeadliness( bool val ) { deadly = val; }
        int X() { return x; }
        int Y() { return y; }
        int W() { return w; }
        int H() { return h; }
        int FX() { return fx; }
        int FY() { return fy; }
        int Left() { return rLeft; }
        int Right() { return rRight; }
        int Top() { return rTop; }
        int Bottom() { return rBottom; }
        bool Deadly() { return deadly; }
};

Tile::Tile()
{
    w = h = 16;
    x = y = 0;
    fx = fy = 0;
    deadly = false;
    rLeft = rRight = rTop = rBottom = -1;
}

void Tile::Setup( int tehX, int tehY, int tehW, int tehH, int tehFilmstripX, int tehFilmstripY, int tehLeftRegion, int tehRightRegion, int tehTopRegion, int tehBottomRegion )
{
    x = tehX;
    y = tehY;
    w = tehW;
    h = tehH;
    fx = tehFilmstripX;
    fy = tehFilmstripY;
    rLeft = tehLeftRegion;
    rRight = tehRightRegion;
    rTop = tehTopRegion;
    rBottom = tehBottomRegion;
}

