#pragma once
#include <allegro.h>

class System
{
    private:
        int screenWidth, screenHeight;
        bool fullscreen, done;
    public:
        System();
        int ScrW() { return screenWidth; }
        int ScrH() { return screenHeight; }
        bool Done() { return done; }
        void Quit();
        void Init();
        void ToggleFullscreen();
};
