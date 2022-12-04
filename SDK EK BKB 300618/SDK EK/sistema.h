#pragma once
#include <allegro.h>
#include <alpng.h>
#include <loadpng.h>  

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
          void Abertura(BITMAP *Imagem,BITMAP *Imagem2,SAMPLE *somFundo);
          void Menu(BITMAP *Buffer,BITMAP *Imagem,BITMAP *Imagem2);
          void MenuSelecionar(BITMAP *Buffer, BITMAP *Imagem[],BITMAP *Imagem2,BITMAP *Imagem3,int x,int y);
          void Face( BITMAP *img, BITMAP *Buffer, int x, int y, int Imagem );
          void Mudar_Tela();
          void Desenhar(BITMAP *Buffer);
          void IncrementCounter();
          void BotaoFechar(void);
          
          void highcolor_fade_in(BITMAP *bmp_orig, int speed);
          void highcolor_fade_out(int speed);
          int Mouse_Info();
          void Desenha_Mouse(BITMAP *Buffer);
          
          
             

      };


