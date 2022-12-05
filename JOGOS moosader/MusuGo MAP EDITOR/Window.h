#pragma once
#include "Gui.h"
#include "Color.h"
#include "Button.h"

class Window : public Gui
{
    private:
        Color color[4];
        Button button[15];
        int buttonsUsed;
    public:
        Window();
        Window( int tindex, string tid );
        void Draw( BITMAP *buffer, BITMAP *image );
        void Setup( int tindex, string tid );
        void ResetButtons();
        void HighlightButtons();
        string CheckClick();
        void SetButtonEnable( string name, bool value );
};







