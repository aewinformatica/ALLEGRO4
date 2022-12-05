#include <allegro.h>
#include "chao.h"

// PROGRAMA que simula subida e pulo na subida.
// Adriano Waltrick
// Arquivo: main.cpp
// Data: 06/09/2007 

// variáveis globais
int fps = 0;
int fps_antigo = 0;
int fps_speed = 0;

// prototipo do contador de frames
void frame_rate();

// prototipo do contador de velocidade
void incrementa_speed();

int main()
{
   allegro_init();
   set_color_depth(16);
   install_keyboard();
   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
   
   install_timer();
   install_int( frame_rate, 1000 );
   install_int_ex( incrementa_speed, BPS_TO_TIMER(30) ); 
   
   BITMAP *buffer = NULL;
   buffer = create_bitmap(800,800);
   
   // Inicio do programa atual 
   
   // Esse será nosso chão
   const int limite_cenario = 300;
   
   // posição do personagem
   // nosso personagem terá 10 pixels
   const int tamanho_personagem = 10;
   int x = 10;
   int y = limite_cenario - 100;
   
   // novo y para quando estiver no morro
   int novo_y = 0;
   
   // valida se o personagem já está no pulo
   bool pulou = false;
   
   // Gravidade: quanto maior, mais rápida será a queda
   const int gravidade = 2;
   
   
   // Força do pulo
   int forca = 0;
   const int forca_maxima = 30;
   
   // Variáveis do chão
   const int total_chao1   = 50;
   const int total_chao2   = 5;
   const int tamanho_chao  = 30;
   Chao *chao[total_chao1];
   Chao *chao2[total_chao2];
   
   // Criando as variaveis do morro de SUBIDA
   const int morro1_altura  = 100;
   const int morro1_largura = 100;
   
   const int morro_x  = 200;
   const int morro_x2 = 300;
   const int morro_y  = limite_cenario - tamanho_chao;
   const int morro_y2 = ( limite_cenario - tamanho_chao ) - morro1_altura;
   
   // criando um morro de descida
   const int morro2_altura  = 100;
   const int morro2_largura = 100;
   
   const int morro2_x  = 550;
   const int morro2_x2 = 450;
   const int morro2_y  = limite_cenario - tamanho_chao;
   const int morro2_y2 = ( limite_cenario- tamanho_chao ) - morro2_altura;
   
   
   // inicia o chao
   int i = 0;
   for ( i=0; i< total_chao1; i++ )
   {
      chao[ i ] = new Chao();
      chao[i]->sety( limite_cenario - tamanho_chao );
      chao[i]->setx( (i * tamanho_chao) + tamanho_chao );
      chao[i]->set_tamanho( tamanho_chao );
   }
   
   // criando o chão em cima do morro
   for ( i=0; i< total_chao2; i++ )
   {
      chao2[ i ] = new Chao();
      chao2[i]->sety( (limite_cenario - tamanho_chao) - morro2_altura );
      chao2[i]->setx( (i * tamanho_chao) + tamanho_chao + 270 );
      chao2[i]->set_tamanho( tamanho_chao );
   }
   
   
   
   
   
   // Laço principal
   while( !key[KEY_ESC] )
   {
      
      while ( ::fps_speed > 0 )
      {
         
         clear( buffer );
         
         // Caso o personagem nao esteja pulando 
         // e apertou a tecla espaço
         if ( key[KEY_SPACE] && pulou == false )
         {
            pulou = true;
            
            // força do pulo inicial
            forca = forca_maxima;
         }
      
         // movimentação do personagem
         if ( key[KEY_RIGHT] )
         {
            x+=5;
         }
         
         // movimentação do personagem
         if ( key[KEY_LEFT] )
         {
            x-=5;
         }
      
         textprintf_ex( buffer, font, 10, 10, makecol(255,0,0), -1, "FPS: %d", ::fps_antigo );
         textprintf_ex( buffer, font, 10, 30, makecol(255,0,0), -1, "Forca: %d", forca );
         textprintf_ex( buffer, font, 10, 40, makecol(255,0,0), -1, "x: %d y: %d", x, y );
      
      
      
      
      
         // Isto vai fazer com que o personagem vá voltado para o chão
         forca = forca - gravidade;
         y = y - forca;
      
         // verifica se está no morro - SUBIDA 
         if ( x+tamanho_personagem >= morro_x && x+tamanho_personagem <= morro_x2 )
         {
            novo_y = ( morro_y2 + (morro2_altura - tamanho_chao) ) - ( ( x-tamanho_personagem ) - morro_x);
            
            if ( y > novo_y )
            {
               y = novo_y;
               
               pulou = false;
               forca = 0;
            }
         }
   
         // verifica se está no morro - DESCIDA 
         if ( x >= morro2_x2 && x <= morro2_x )
         {
            novo_y = (morro2_y2) + ( (x-tamanho_personagem) - morro2_x2 );
            
            if ( y > novo_y )
            {
               y = novo_y;
               
               pulou = false;
               forca = 0;
            }
         }
   

         // imprime chao e verifica colisao
         for ( i=0; i< total_chao1; i++ )
         {
            
            rectfill( buffer, chao[i]->getx(), chao[i]->gety(), chao[i]->getx()+chao[i]->get_tamanho(), chao[i]->gety()+chao[i]->get_tamanho(), makecol(0,255,0) );
            
            if ( i < total_chao2 )
            {
               rectfill( buffer, chao2[i]->getx(), chao2[i]->gety(), chao2[i]->getx()+chao2[i]->get_tamanho(), chao2[i]->gety()+chao2[i]->get_tamanho(), makecol(0,255,0) );
            }
            
            if ( chao[i]->colide(x, y, tamanho_personagem) == true )
            {
               y = chao[i]->gety() - tamanho_personagem;
               pulou = false; 
               forca = 0; 
            }
         
            if ( i < total_chao2 )
            {
               if ( chao2[i]->colide(x, y, tamanho_personagem) == true )
               {
                     y = chao2[i]->gety() - tamanho_personagem;
                     pulou = false;
                     forca = 0;
               }
            }
            
         }
      
         // Esse if vai barrar nosso personagem de passar pelo chão ( limite )
         if (y > limite_cenario - tamanho_personagem)
         {
            y = limite_cenario - tamanho_personagem;
            
            pulou = false;
            forca = 0;
         }
      
      
         rectfill( buffer, x, y, x+tamanho_personagem, y+tamanho_personagem, makecol(255,255,0) );
         
         line(buffer, morro_x, morro_y, morro_x2, morro_y2, makecol(255, 0, 0 ));
         line(buffer, morro2_x, morro2_y, morro2_x2, morro2_y2, makecol(255, 0, 0 ));
         
         
         ::fps_speed--;
         ::fps++;
      }
      
      blit(buffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
      vsync();
      
   }
      
   destroy_bitmap( buffer );
      
      
   // destroi chão 1 e 2
   for ( i=0; i< total_chao1; i++ )
   {
      delete chao[ i ];
      
      if ( i < total_chao2 )
      {
         delete chao2[ i ];
      }
   }
      
   allegro_exit();
   return 0;
}
END_OF_MAIN();
      
void frame_rate()
{
   ::fps_antigo = ::fps;
   ::fps = 0;
}
      
void incrementa_speed()
{
   ::fps_speed++;
}
