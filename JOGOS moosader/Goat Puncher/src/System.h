#pragma once
#include <allegro.h>
#include <string>
#include <iostream>
using namespace std;

enum GameState { sMENU, sINGAME, sGAMEOVER, sQUIT };

class System
{
    private:
        int scrWidth, scrHeight;
        GameState state;
        bool fullscreen;
        
    public:
        BITMAP *buffer;
        
        System();
        
        int ScrWidth() { return scrWidth; } 
        int ScrHeight() { return scrHeight; }
        GameState State() { return state; }
        void State( GameState state ) { this->state = state; }
        
        void ToggleFullscreen();
        void Setup();
        void DrawToScreen( BITMAP *buffer );
};


