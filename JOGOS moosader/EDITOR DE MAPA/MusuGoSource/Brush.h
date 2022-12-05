#pragma once
#include <allegro.h>
#include "Color.h"

class Brush
{
    private:
        int fx, layer, size;
        bool click;
        Color color[2];
    public:
        Brush();
        void Draw( BITMAP *buffer );
        void SetClick( bool val ) { click = val; }
        void SetLayer( int val ) { layer = val; }
        void SetSize( int val ) { size = val; }
        void SetFx( int val ) { fx = val; }
        int Fx() { return fx; }
        int Layer() { return layer; }
        int Size() { return size; }
        bool Click() { return click; }
};
