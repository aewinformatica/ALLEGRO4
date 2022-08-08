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

Sprite personagem;
Sprite karate;
Sprite ryu;

Logica timer;

int main()
{
init(); 
BITMAP *pBG;
        pBG = load_bitmap("gfx/BG.png",NULL);
                                        
personagem.carregarSprite("gfx/dados.dat",personagem.tipo=DATASHEET) ;
personagem.setFrame(0,8);

//ryu.carregarSprite("gfx/ryu.dat",ryu.tipo=RLESHEET,soco) ;
//ryu.carregarSprite("gfx/ryu.dat",ryu.tipo=RLESHEET,woody2) ;
//ryu.carregarSprite("gfx/dados.dat",ryu.tipo=RLESHEET,RYU) ;
//ryu.setFrame(0,5);

karate.carregarSprite("gfx/karate.bmp");
karate.setFrame(0,5);

                                                          
 while(!key[KEY_ESC]){ 
       
	   clear(pBuffer);
       //timer.Conta_Tempo(pBuffer);    
	    
      while(velocidade>0){
	  	
          //Desenhando PNG com Transparencia
	  	draw_trans_sprite(pBuffer, pBG,0, 0); 
	  	
	  	//ryu.desenhar(pBuffer,ryu.tipo=RLESHEET);
            
        personagem.desenhar(pBuffer);
        //personagem.controle();
	  	
        //karate.desenhar(pBuffer,karate.tipo=SPRITESHEET);//depende do setFrame
        //karate.desenhar(pBuffer,karate.AnimArray=SETARRAY);//depende do atualizar
        //karate.Atualizar();
        //karate.controle();
    	        
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
