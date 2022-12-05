#pragma once

class Color
{
    public:
        int r, g, b;
        Color();
        Color( int tr, int tg, int tb );
        int R() { return r; }
        int G() { return g; }
        int B() { return b; }
        void SetRGB( int tr, int tg, int tb );
        void Lighten();
        void Darken();
};


