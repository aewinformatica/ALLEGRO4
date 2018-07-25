#include <allegro.h>
#include <iostream>
#include <fstream>

#include "LevelClass.h"

using namespace std;

int fps = 0;
int fps_antigo = 0;
int fps_speed = 0;

// prototipo do contador de frames
void frame_rate();
// prototipo do contador de velocidade 
void incrementa_speed();
void Teclado();
void status(BITMAP * Buffer);

const int iQtdeFases = 3;    // Jogo com 3 fases

int iFaseAtual = 0; // Guarda o index da fase atual.
   
LevelClass level;

int main()
{
   allegro_init();
   install_keyboard();
   set_color_depth(32);
   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
   set_window_title( "Fases em arquivos textos" );

   install_timer();
   install_int( frame_rate, 1000 );
   install_int_ex( incrementa_speed, BPS_TO_TIMER(60) );

   //Habilita Acentuação
   set_uformat(U_ASCII);

   BITMAP *Buffer = NULL;
   Buffer = create_bitmap(SCREEN_W, SCREEN_H);
 



   while (!key[KEY_ESC])
   {
      while ( ::fps_speed > 0 ) 
      { 
         clear_bitmap(Buffer);
         
         level.DesenhaCenario(Buffer,iFaseAtual);
         Teclado();
         status(Buffer);

         ::fps_speed--;
         ::fps++;
      }
      blit (Buffer,screen, 0, 0, 0, 0,SCREEN_W, SCREEN_H);
      //vsync();
   }
   //Desaloca as imagens da memória
   destroy_bitmap(Buffer);
   allegro_exit();

   return 0;
}
void Teclado()
{
   /* 
   Quando declaramos uma variável como sendo static quer dizer que mesmo saindo
   da função ela não vai perder o valor dela. Dessa forma, utilizamos a variável 
   abaixo para controlar o tempo de uma tecla a outra.
   */
   static int Buffer_Teclado = 0;

   //Tudo que estiver dentro o IF abaixo será executado a cada 30 ciclos de CPU
   if (Buffer_Teclado == 0)
   {
      // Muda o Cenário caso aperte a tecla espaço
      if (key[KEY_SPACE])
      {
         if (iFaseAtual < (iQtdeFases-1))
            iFaseAtual++;
         else
            iFaseAtual = 0;
         
         Buffer_Teclado = 30;
      }
   } else Buffer_Teclado--;
}

void frame_rate()
{
   ::fps_antigo = ::fps;
   ::fps = 0;
}
   
void incrementa_speed()
{
   ::fps_speed++; 
}
END_OF_MAIN();
void status(BITMAP * Buffer){
              textprintf_ex( Buffer, font, 8, 10, makecol(255,255,0), -1, "FPS: %d", ::fps_antigo );
                     textprintf_centre_ex(Buffer, font, SCREEN_W/2, (SCREEN_H/2)-20, makecol(255, 255, 0),
         -1, "ARQUIVO: %s", level.ArqFases[iFaseAtual]);
         
         textprintf_centre_ex(Buffer, font, SCREEN_W/2, (SCREEN_H/2)-10, makecol(255, 255, 0),
         -1, "FASE: %d", iFaseAtual+1);
      
         textprintf_centre_ex(Buffer, font, SCREEN_W/2, SCREEN_H/2, makecol(255, 255, 0), -1, 
         "Pressione espaço para mudar de fase.");
}
