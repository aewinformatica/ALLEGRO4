#include <allegro.h> 
#include <alpng.h>
#include "sprite.h"
#include "logica.h" 
#include "ryuindex.h"
#include "dadosindex.h"   


                       

int velocidade = 0;

void init();
void conta_velocidade_animacao();

BITMAP *pBuffer;

//Sprite personagem;
Sprite karate;
//Sprite ryu;

Logica timer;

int main()
{
init(); 
BITMAP *pBG;
        pBG = load_bitmap("gfx/BG.png",NULL);
karate.carregarSprite("gfx/karate.bmp"); //ARQUIVO EXTERNO 
//personagem.carregarSprite("gfx/dados.dat") ;//ARUIVOS DAT SEPARADOS                                       

//ryu.carregarSprite("gfx/ryu.dat",soco) ;
//ryu.carregarSprite("gfx/ryu.dat",woody) ;//OK
                                            
 while(!key[KEY_ESC]){ 
       
	   clear(pBuffer);
       //timer.Conta_Tempo(pBuffer);    
	    
      while(velocidade>0){
	  	
          //Desenhando PNG com Transparencia
	  	draw_trans_sprite(pBuffer, pBG,0, 0); 
	  	/*
	  	ryu.desenhar(pBuffer);//OK
	  	ryu.Atualizar();//OK
	  	ryu.controle();//OK
          */
        /*
        personagem.desenhar(pBuffer); //OK
        personagem.Atualizar();//OK
        personagem.controle();//OK
	  	*/
        
        karate.desenhar(pBuffer);//OK
        karate.Atualizar();//OK
        karate.controle();//OK
        
    	        
		blit (pBuffer, screen, 0, 0, 0, 0, pBuffer->w, pBuffer->h);
		velocidade--; 
		}
				
	   }
  
 allegro_exit();                                                                              
 return 0;                                     
}
END_OF_MAIN()
void init()
{                                          
      allegro_init();
	  alpng_init(); 
      install_keyboard();
	  install_timer();
	    
      LOCK_VARIABLE(velocidade);    
      LOCK_FUNCTION(conta_animacao);                                                                            
      
	  set_color_depth(32);
	  set_alpha_blender(); 
      set_uformat(U_ASCII);                         

//Autodetecta a placa  de video e usa resolucao de 640x480 
 set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);
 
 install_int(conta_velocidade_animacao,60);                              
     

if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0) != 0) {
 exit(-1);
}   

pBuffer = create_bitmap(SCREEN_W,SCREEN_H);
}
void conta_velocidade_animacao()
{
 	 velocidade++;
 }
