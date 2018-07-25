//TileClass.h
#pragma once
#include <allegro.h>

class TileClass{
public:
       TileClass();
       ~TileClass();
       int x,y,w,h,cx,cy;
       int qtdTextura;
       BITMAP *Textura[3];
       void CarregaTexturas();
       
            
};
