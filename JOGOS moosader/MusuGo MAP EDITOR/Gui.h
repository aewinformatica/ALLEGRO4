#pragma once
#include <iostream>
#include <string>
#include <allegro.h>
using namespace std;

class Gui
{
    protected:
        int x1, y1, x2, y2, index;
        bool visible;
        string id;
    public:
        Gui();
        void Draw( BITMAP* );
        int X1() { return x1; }
        int X2() { return x2; }
        int Y1() { return y1; }
        int Y2() { return y2; }
        bool Visible() { return visible; }
        void SetVisible( bool val ) { visible = val; }
};


