//LevelClass.h
#pragma once
#include "TileClass.h"
#include <allegro.h>

class LevelClass{

public:
       LevelClass();
       TileClass tile;
       int iTilesX;      
       int iTilesY;
       int iQtdeFases;
       int Mapa[3][20][15];
       char *ArqFases[3]; 
       void CarregaMatriz();
       void DesenhaCenario(BITMAP *, int );
       ~LevelClass();
};
