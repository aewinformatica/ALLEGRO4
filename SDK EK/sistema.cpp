#include <allegro.h>
#include <cstdlib>
#include <ctime>
#include "sistema.h"

#include <almp3.h>
#include <alpng.h>
#include <loadpng.h>  

#define BRANCA makecol(255,255,255)
#define TRANSPARENTE -1



Sistema::Sistema()
{
                  
                  Iniciar();
                                 
}
Sistema::~Sistema()
{
                   clear_keybuf();
                   }
void Sistema::Iniciar(){
     
     tela_larg = 800;
     tela_alt = 600;
     
     
     tela_cheia = false;
     fim = false;
     
    allegro_init();
    loadpng_init(); 
    install_keyboard();
    install_timer();
    install_mouse();
    show_mouse(screen);
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
    install_joystick(JOY_TYPE_AUTODETECT);
    
    LOCK_FUNCTION(BotaoFechar); 

    
    
    LOCK_VARIABLE( spdCounter );
    LOCK_FUNCTION( IncrementCounter );
    
    set_color_depth( 32 );
    set_alpha_blender();
    
    set_volume_per_voice(0);
    /*
    set_display_switch_mode(SWITCH_BACKGROUND);
     */
    if(tela_cheia){
                    set_gfx_mode(GFX_AUTODETECT,tela_larg,tela_alt,0,0);
                    
                    }
                    else
                    {
                    set_gfx_mode(GFX_AUTODETECT_WINDOWED,tela_larg,tela_alt,0,0);
                    
                    }
     }
void Sistema::Menu(BITMAP *Buffer,BITMAP *pFundo,BITMAP pItens[1]){
     masked_blit(pFundo,Buffer,0,0,0,0,800,600);
     //masked_blit(pItens[0],Buffer,0,0,50,150,640,480);
    // masked_blit(pItens[1],Buffer,0,0,50,200,640,480);
    // masked_blit(pItens[1],Buffer,0,0,50,150,640,480);
     
     
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
     
bool Sistema::Sair(){
     
    return fim = true;
    
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
     
play_sample(somFundo,255,128,1000,FALSE);
 draw_trans_sprite(screen, Imagem, 150, 0); 
 rest( 2400 );
 draw_trans_sprite(screen, Imagem2, 100, 200);
 rest( 1000 );
textout_centre_ex(screen,font,"Personagens Originais da ",250,175,BRANCA,TRANSPARENTE);
textout_centre_ex(screen,font,"Jogo Feito por Wagner Goncalves de Jesus ",280,320,BRANCA,TRANSPARENTE);
 
    


	// espera 3 segundos
	rest( 4000 );
	/*
	highcolor_fade_out(1);
*/
    stop_sample(somFundo);

      
	clear_keybuf();
   
}
void Sistema::MenuSelecionar(BITMAP *Buffer, BITMAP *Imagem[],BITMAP *Imagem2,BITMAP *Imagem3,int x,int y)
{
     int i= 0;
     clear(Buffer);
     
     BITMAP *aux = NULL;
            aux = create_bitmap(640,480);
     for( i =0;i<5;i++)
     {       
if(i>=5)
{
        i=0;
        }
        
     }     
    draw_trans_sprite(aux, Imagem[0], 150, 150); 
     draw_trans_sprite(aux, Imagem2, 190,150);
     draw_trans_sprite(aux, Imagem3, 220,130);
     
   textprintf_centre_ex( aux, font, 300, 120, makecol( 255, 255, 255 ), -1, " ARCADE " );
   
     masked_blit(aux,Buffer,0,0,0,0,640,480); 
     
     }
void Sistema::Face( BITMAP *img, BITMAP *Buffer, int x, int y, int Imagem )
{ 
     int tfx, tfy;
     
     tfx = 0;
     tfx = 0;
  /*  
  switch ( Imagem )
    {
        case 0:     //Plum
            tfx = 0;
            tfy = 192;
        break;

        case 1:     //Rank
            tfx = 40;
            tfy = 192;
        break;

        case 2:     //Marxio
            tfx = 80;
            tfy = 192;
        break;

        case 3:     //Pookie
            tfx = 120;
            tfy = 192;
        break;

        case 4:     //Name
            tfx = 0;
            tfy = 232;
        break;

        case 5:     //Mars
            tfx = 40;
            tfy = 232;
        break;

        case 6:     //Wonky
            tfx = 80;
            tfy = 232;
        break;

        case 7:     //Phonics
            tfx = 120;
            tfy = 232;
        break;

        case 8:     //Random
            tfx = 160;
            tfy = 232;
        break;

        case 9:     //Dono
            tfx = 160;
            tfy = 192;
        break;

        case 10:    //Me
            tfx = 200;
            tfy = 192;
        break;

        default:
            cerr<<"no image"<<endl;
        break;
    } */
    masked_blit( img, Buffer, tfx, tfy, x, y, 640, 480 );
    
     }

