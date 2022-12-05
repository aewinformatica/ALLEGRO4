#include "sistema.h"

Sistema::Sistema()
{
                  Iniciar();
                                 
}
void Sistema::Iniciar(){
     
     tela_alt = 480;
     tela_larg = 640;
     
     tela_cheia = false;
     fim = false;
     
    allegro_init();
    install_keyboard();
    install_timer();
    install_mouse();
    install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, 0 );
    set_color_depth( 32 );

     
    if(tela_cheia){
                    set_gfx_mode(GFX_AUTODETECT,tela_larg,tela_alt,0,0);
                    
                    }
                    else
                    {
                    set_gfx_mode(GFX_AUTODETECT_WINDOWED,tela_larg,tela_alt,0,0);
                    
                    }
     }
          
void Sistema::Mudar_Tela(){
               
              if(tela_cheia){
                             
                           tela_cheia = false;
                           
                    set_gfx_mode(GFX_AUTODETECT_WINDOWED,tela_larg,tela_alt,0,0);
                    
                    }
                    else
                    {
                           tela_cheia = true;
                        
                    set_gfx_mode(GFX_AUTODETECT,tela_larg,tela_alt,0,0);
                    
                    }
     
     }
     
void Sistema::Sair(){
     
     fim = true;
    
     }
void Sistema::Desenhar(BITMAP *Buffer){
     
         blit( Buffer, screen, 0, 0, 0, 0, tela_larg,tela_alt );
    release_screen();
    clear_bitmap( Buffer );
     }
