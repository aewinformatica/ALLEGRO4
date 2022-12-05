#pragma once
#include <string>
#include <allegro.h>
#include "Gui.h"
#include "Color.h"
using namespace std;

class Button : public Gui
{
    private:
        Color color[7]; //0-light, 1-base, 2-dark, 3-text, 4-enabled, 5-clicked, 6-highlighted
        string caption;
        bool clicked, enabled, highlight;
        int btnImgX, btnImgY;
    public:
        Button();
        Button( int tindex, string tid );
        void Setup( int tindex, string tid );
        void Setup( int tindex );
        void Draw( BITMAP *buffer, BITMAP *image, int winX, int winY );
        void SetClicked( bool val ) { clicked = val; }
        void SetHighlight( bool val ) { highlight = val; }
        string Id() { return id; }
        int Index() { return index; }
        string Caption() { return caption; }
        void SetEnable( bool val ) { enabled = val; }
};






