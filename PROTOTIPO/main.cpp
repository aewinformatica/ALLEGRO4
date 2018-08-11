#include <allegro.h> 
#include <alpng.h>
#include "sprite.h"
#include "sistema.h"


//Gerenciar FPS
volatile int fps =0;
void conta_fps(void);

Sprite personagem;

Sistema Jogo;
int main()
{


LOCK_VARIABLE(fps);    
LOCK_FUNCTION(conta_fps);                                                                            
     
 install_int(conta_fps,40); 

BITMAP *pBG;
        pBG = load_bitmap("gfx/BG.png",NULL);

//personagem.carregarSprite("gfx/karate.bmp"); //ARQUIVO EXTERNO
 
//personagem.carregarSprite("gfx/dados.dat") ;//ARUIVOS DAT SEPARADOS                                       

//personagem.carregarSprite("gfx/ryu.dat",soco) ;
///personagem.carregarSprite("gfx/ryu.dat",woody2) ;//OK
//personagem.carregarSprite("gfx/zero.dat",entrada) ;//OK
//personagem.carregarSprite("gfx/zero.dat",parado) ;//OK
//personagem.carregarSprite("gfx/zero.dat",saltando) ;//OK
//personagem.carregarSprite("gfx/zero.dat",andando) ;//OK
personagem.carregarSprite("gfx/woody.dat",abaixando) ;//OK

                                            
while(!Jogo.Fim()){ 
       
	   Jogo.Limpar_Tela();

        if ( key[KEY_ESC] || key[KEY_F4] ) { Jogo.Sair(); }
        if ( key[KEY_F5] ) { Jogo.Mudar_Tela(); }    
	    
      while(fps>0){
	  	
          //Desenhando PNG com Transparencia
	  	draw_trans_sprite(Jogo.pBuffer, pBG,0, 0); 
        
        personagem.desenhar(Jogo.pBuffer);//OK
        personagem.Atualizar();//OK
        personagem.controle();//OK
        
        Jogo.Desenhar();        

		fps--; 
       }				
  }
  
 allegro_exit();                                                                              
 return 0;                                     
}
END_OF_MAIN()
void conta_fps(void)
{
 	 fps++;
 }
