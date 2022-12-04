#include <allegro.h>
#include "sistema.h"
#include "sound.h"
#include "music.h"
#include "Menu.h"




volatile int FecharJogo = FALSE; 

void BotaoFechar(void) 
{ 
   FecharJogo = TRUE; 
} 
int main()
{
    Sistema Jogo;
    Sound som("Mortal.wav");
    BITMAP *menu = NULL;
            menu = load_bitmap("Imagens/Menu.bmp",NULL);
                BITMAP *menuItens[1];
            menuItens[0] = load_bitmap("Imagens/mnuNewGame.bmp",NULL);
    BITMAP *Buffer;
            Buffer = create_bitmap(800,600);
     set_close_button_callback(BotaoFechar);        
    while(!Jogo.Fim())
    {
		  if(key[KEY_ESC]||key[KEY_F4])	  
             Jogo.Sair();
             
     som.play();
                             //roundedRect(Buffer, 0, 0, 9, 9, 2, makecol(32,80,32));
          if(key[KEY_ENTER])
          {
             if (key_shifts & KB_ALT_FLAG) 
             Jogo.Mudar_Tela();
          }
                          
    
    Jogo.Menu(Buffer,menu,menuItens[1]);
    blit(Buffer,screen,0,0,0,0,800,600);
    }
    allegro_exit();
}
END_OF_MAIN()

