#include <allegro.h>
#include "sistema.h"
#include <loadpng.h>
#include <alpng.h>  



Sistema::Sistema()
{
                  Iniciar();
                                 
}
Sistema::~Sistema()
{
                   clear_keybuf();
                   }
void Sistema::Iniciar(){
     
     tela_alt = 600;
     tela_larg = 800;
     
     tela_cheia = false;
     fim = false;
     
    allegro_init();
    alpng_init(); 
    install_keyboard();
    install_timer();
    install_mouse();
    show_mouse(screen);
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
    install_joystick(JOY_TYPE_AUTODETECT);
    LOCK_VARIABLE( spdCounter );
    LOCK_FUNCTION( IncrementCounter );
    
    
    /*install_int_ex(IncrementCounter,BPS_TO_TIMER(90));
    */
    
    set_color_depth( 32 );
    set_color_conversion(COLORCONV_NONE);
	register_png_file_type();
    set_alpha_blender();
     
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

     
void Sistema::highcolor_fade_in(BITMAP *bmp_orig, int speed)
{
   BITMAP *bmp_buff;

    if ((bmp_buff = create_bitmap(SCREEN_W, SCREEN_H)))
    {
        int a;
        if (speed <= 0) speed = 16;
        
        for (a = 0; a < 256; a+=speed)
        {
            clear(bmp_buff);
            set_trans_blender(0,0,0,a);
            draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
            vsync();
            blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
        }
        destroy_bitmap(bmp_buff);
    } 

    blit(bmp_orig, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
}



void Sistema::highcolor_fade_out(int speed)
{
    BITMAP *bmp_orig, *bmp_buff;

    if ((bmp_orig = create_bitmap(SCREEN_W, SCREEN_H)))
    {
        if ((bmp_buff = create_bitmap(SCREEN_W, SCREEN_H)))
        {
            int a;
            blit(screen, bmp_orig, 0,0, 0,0, SCREEN_W, SCREEN_H);
            if (speed <= 0) speed = 16;
        
            for (a = 255-speed; a > 0; a-=speed)
            {
                clear(bmp_buff);
                set_trans_blender(0,0,0,a);
                draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
                vsync();
                blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
            }
            destroy_bitmap(bmp_buff);
        }
        destroy_bitmap(bmp_orig);
    }

    rectfill(screen, 0,0, SCREEN_W,SCREEN_H, makecol(0,0,0));
}
int Sistema::Mouse_Info()
{
    {
     if(mouse_b & 1){
                  cursor_x = mouse_x;
                  cursor_y = mouse_y;
      return 1;
     }
  return 0;
}
}

void Sistema::Desenha_Mouse(BITMAP *Buffer){
 
     show_mouse(NULL);
     circlefill ( Buffer, cursor_x, cursor_y, 5, makecol( 0, 0, 255));
     draw_sprite( screen, Buffer, 0, 0);  
}
void Sistema::IncrementCounter()
{
    spdCounter++;
}
void Sistema::Abertura(BITMAP *Imagem,BITMAP *Imagem2,SAMPLE *somFundo)
{
     

 draw_trans_sprite(screen, Imagem, 100, 0); 
 draw_trans_sprite(screen, Imagem2, 100, 0);
 

 
    play_sample(somFundo,255,128,1000,FALSE);


	// espera 3 segundos
	rest( 12000 );
	highcolor_fade_out(6);
	
    stop_sample(somFundo);

      
	clear_keybuf();

}
