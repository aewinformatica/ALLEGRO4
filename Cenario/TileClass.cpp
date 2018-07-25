//TileClass.cpp
#include <iostream>
#include <string>
#include <fstream>
#include "TileClass.h"

TileClass::TileClass(){
    this->x = this->y = this->cx = this->cy = 0;
    this->w = this->h = 32;
    this->qtdTextura = 3;
    
    for(int i=0; i < this->qtdTextura -1;i++){ 
    this->Textura[i] = NULL; 
    }
};



void TileClass::CarregaTexturas()
{
   // Define o índice 0 da textura como sendo parede
   this->Textura[0] = create_bitmap(w, h);
   Textura[0] = load_bitmap("tiles/parede.bmp", NULL);
   
   // Define o índice 1 da textura como sendo grama
   this->Textura[1] = create_bitmap(w, h);
   Textura[1] = load_bitmap("tiles/grama.bmp",NULL);

   // Define o índice 2 da textura como sendo chão
   this->Textura[2] = create_bitmap(w, h);
   Textura[2] = load_bitmap("tiles/chao.bmp",NULL);
}

TileClass::~TileClass(){

    for(int i=0; i < this->qtdTextura -1;i++){ 
     destroy_bitmap(this->Textura[i]); 
    }
    

}
