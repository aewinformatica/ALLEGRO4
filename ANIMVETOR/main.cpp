#include <iostream>
#include <allegro.h>
#include <vector>
#include "sprite.h"
#include "indexryu.h"
using namespace std;

int velocidade = 0;

void init();
void conta_velocidade_animacao();
BITMAP *pBuffer;



int main()
{
init();                                 

DATAFILE *dados = load_datafile("ryu.dat");
Sprite personagem;
personagem.getAnim(dados);
                                                          
 while(!key[KEY_ESC]){ 
       
	   clear(pBuffer);
      while(velocidade>0){
	  	
	  	//personagem.draw(pBuffer);
	  	        
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
	  
      install_keyboard();
	  install_timer();
	    
      LOCK_VARIABLE(velocidade);    
      LOCK_FUNCTION(conta_animacao);                                                                            
      
	  set_color_depth(32);
	  set_alpha_blender();                          

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
