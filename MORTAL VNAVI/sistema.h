#pragma once
#include <allegro.h>
#include <alpng.h>  

class Sistema {
      
      private:
              int tela_alt, tela_larg;
              int cursor_x, cursor_y;
              bool tela_cheia, fim;
              
      public:
             Sistema();
             ~Sistema();
          int Tel_Alt(){ return tela_alt;}
          int Tel_Larg(){ return tela_larg;}
         volatile long spdCounter;
          bool Fim(){return fim;}
          void Sair();
          void Iniciar();
          void Mudar_Tela();
          void Desenhar(BITMAP *Buffer);
          void IncrementCounter();
          void Abertura(BITMAP *Imagem,BITMAP *Imagem2,SAMPLE *somFundo);
          void highcolor_fade_in(BITMAP *bmp_orig, int speed);
          void highcolor_fade_out(int speed);
          int Mouse_Info();
          void Sistema::Desenha_Mouse(BITMAP *Buffer);
          
             

      };


