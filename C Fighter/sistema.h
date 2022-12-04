#pragma once
#include <allegro.h>

class Sistema {
      
      private:
              int tela_alt, tela_larg;
              bool tela_cheia, fim;
              
      public:
             Sistema();
          int Tel_Alt(){ return tela_alt;}
          int Tel_Larg(){ return tela_larg;}
         volatile long spdCounter;
          bool Fim(){return fim;}
          void Sair();
          void Iniciar();
          void Mudar_Tela();
          void Desenhar(BITMAP *Buffer);
          void IncrementCounter();
          
             

      };


