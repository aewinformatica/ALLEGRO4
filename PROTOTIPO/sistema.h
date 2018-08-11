#pragma once
#include <allegro.h>
#include <time.h>

class Sistema {
      
      private:
              int tela_alt, tela_larg;
              bool tela_cheia, fim;
              
      public:
             Sistema();
          int Tel_Alt(){ return tela_alt;}
          int Tel_Larg(){ return tela_larg;}
          
          BITMAP *pBuffer;
          volatile long spdCounter;
          bool Fim(){return fim;}
          void Sair();
          void Iniciar();
          void Mudar_Tela();
          void Limpar_Tela(){clear(pBuffer);}
          void Desenhar();
          void IncrementCounter();

          
             

      };


