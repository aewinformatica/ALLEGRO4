#pragma once
#include <allegro.h>

class Personagem {

      
      private:
              

      
      public:
                 
      int hp,hpmax;
      int x,y;
      float velocidade,direcao,desenho;
      float centro_x,centro_y;
      float ciclo;
      float pos;
      float area_x,area_y;
      float gravidade;
      
      Personagem();
 void Desenhar(BITMAP *, BITMAP *);
 void Mover(int x,int y);
      };
