/*
  Name: Speedhack 2007
  Copyright: 2007 Lune Knight Software
  Author: Jeff Bernard
  Date: 09/14/07 08:56
*/
#include <allegro.h>
#include <cstdio>
#include "dialog.h"

volatile int speed = 0;

void increment_timer()
{
   speed++;
}
END_OF_FUNCTION(increment_timer)

void init();

bool prologue(int* place)
{
   dialog& dlg = dialog::instance();

   switch (*place)
   {
      case 0: if (dlg.input().size())
              {
                  if (dlg.input() == "SKIP")
                  {
                     
                     dlg.show(false);
                     return false;
                  }
                  dlg.setMsg("Pai: Em nosso mundo de figuras da vara, voce pode dizer que está mal, porque eles sao vermelhos. Com certeza e um recurso pratico da vida. Qual a cor que voce estava outra vez, rapaz?");
                  dlg.setInput(true);
                  *place = 1;
              }
              break;
      case 1: if (dlg.input().size())
              {
                 dlg.setMsg("Mother: Well, it doesn't really matter what you say, because you're red. So, let's throw the kid into a dumpster.");
                 dlg.setInput(true);
                 *place = 2;
              }
              break;
      case 2: if (dlg.input().size())
              {
                 dlg.setMsg("Television: Breaking news alert! Goodzilla has just arrived and is wreaking peace all throughout our fair city. Reds beware, because Goodzilla is cool and green. =)");
                 dlg.setInput(true);
                 *place = 3;
              }
              break;
      case 3: if (dlg.input().size())
              {
                 dlg.setMsg("Pai: Vamos deixar a criança com uma arma tão Goodzilla contará com ele para nós. Poupa-nos tempo de ir para o lixo. Que arma, embora?");
                 dlg.setInput(true);
                 *place = 4;
              }
              break;
      case 4: if (dlg.input().size())
              {
                 if (dlg.input() == "SWORD")
                 {
                    dlg.show(false);
                    return false;
                 }
                 if (dlg.input() == "SPOON")
                 {
                    dlg.show(false);
                    return false;
                 }
                 if (dlg.input() == "GUN")
                 {
                    dlg.show(false); 
                    return false;
                 }
                 if (dlg.input() == "POGO STICK")
                 {
                    dlg.show(false);
                    return false;
                 }
                 if (dlg.input() == "NOTHING")
                 {
                    dlg.show(false);
                    
                    return false;
                 }
                 dlg.setMsg("  SWORD | SPOON | GUN | POGO STICK | NOTHING");
                 dlg.setInput(true);
              }
   }
   return true;
}

bool epilogue(int* place)   
{
   dialog& dlg = dialog::instance();

   switch (*place)
   {
      case 0: if (dlg.input().find("NEW GAME") != std::string::npos)
              {
                 dlg.show(false);

                 return false;
              }
              else if (dlg.input().size())
              {
                 *place = 1000;
                 return true;
              }
              break;
   }
   return true;
}
     
int main()
{
   init();
   
   BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
   clear_to_color(buffer, makecol(255,255,255));
   dialog& dlg = dialog::instance();
   dlg.show(true);
   dlg.setMsg("           Aperte Enter Para  Iniciar            DIGITE 'SKIP' para pular a introducao.");
   dlg.setInput(true);
   bool intro = true, gameOver = false;
   int place = 0;
      
   LOCK_VARIABLE(speed);
   LOCK_FUNCTION(increment_timer);
   install_int_ex(increment_timer, BPS_TO_TIMER(240));

   while (!key[KEY_ESC] && place < 999)
   {
      
      while (speed > 0)
      {
         if (intro)
         {
            dlg.update();
            intro = prologue(&place);
         }
         else if (gameOver)
         {
            dlg.update();
            gameOver = epilogue(&place);
         }

         speed--;
      }
      
      if (intro || gameOver)
      {
        dlg.draw(buffer);
        }

      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      clear_to_color(buffer, makecol(255,255,255));
     
   }
   
   destroy_bitmap(buffer);
   return 0;
}
END_OF_MAIN()

void init()
{
   int depth, res;
   allegro_init();
   depth = desktop_color_depth();
   if (depth == 0) depth = 32;
   set_color_depth(depth);
   int width = 640, height = 480; // i wish...
   get_desktop_resolution(&width, &height);
   if (width*3 != height*4) // go widescreen...
   {
      width = 640;
      height = 400; // lulz, cut off the bottom of the screen
   }
   else // we can go 4:3
   {
      width = 640;
      height = 480;
   }    
   res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
   if (res != 0)
   {
      char c[52];
      usprintf(c, "%dx%d", width, height);
      allegro_message(c);
      exit(-1);
   }
   install_timer();
   install_keyboard();
   install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
   
   drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
   set_trans_blender(0,0,0,255);
}
