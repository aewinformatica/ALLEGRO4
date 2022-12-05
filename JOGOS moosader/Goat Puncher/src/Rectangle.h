#pragma once

struct Rectangle
{
    int x, y, w, h;
    
    void Setup( int x, int y, int w, int h )
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    
    void CompletelySolid() { x = y = 0; w = h = 32; }
    void NotSolid() { x = y = w = h = -1; }
    
    int L() { return x; }
    int R() { return x+w; }
    int T() { return y; }
    int B() { return y+h; }
};
