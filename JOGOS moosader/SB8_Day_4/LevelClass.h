//LevelClass.h
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <allegro.h>
#include "TileClass.h"
using namespace std;

const int TILEX = 160;
const int TILEY = 60;
const int LAYERS = 3;

class LevelClass
{
    public:
        TileClass tile[LAYERS][TILEX][TILEY];
        BITMAP *background;
        void LoadMap(string filename);
        void DrawTop(BITMAP *buffer, BITMAP *tileset, float, float);
        void DrawBottom(BITMAP *buffer, BITMAP *tileset, float, float);
};




