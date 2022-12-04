#pragma once
#include <allegro.h>
#include <string>
#include <iostream>
using namespace std;

class Cenario
{
      public:
             Cenario();
             
             BITMAP *Imagem[6];
             
             void Desenhar(BITMAP *Buffer,BITMAP *Imagem,int x ,int y);
      private:
              
      };
