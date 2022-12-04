#include "personagem.h"
#include "ImagemManager.h"

Personagem::Personagem(){
      x = 50;
      y = 80;
      velocidade = 7;
      direcao = 1;
      hpmax = 800;
      hp = hpmax;
      

}
void Personagem::Desenhar(BITMAP *Buffer,BITMAP *Imagem){
     
     masked_blit(Imagem, Buffer, 0,0, x, y, 50, 50);
    
      
                            
                            }
void Personagem::Mover(int x,int y){
     
     if(x<0){
             }
     
     };

