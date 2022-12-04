#include "Cenario.h"

Cenario::Cenario()
{                 
                      DATAFILE *Imagens  = NULL;
                                Imagens  = load_datafile("Cenario/Cachoeira.dat");
                   
                   Imagem[0] = (BITMAP *)Imagens[0].dat;
                   Imagem[1] = (BITMAP *)Imagens[1].dat;
                   Imagem[2] = (BITMAP *)Imagens[2].dat;
                   Imagem[3] = (BITMAP *)Imagens[3].dat;
                   Imagem[4] = (BITMAP *)Imagens[4].dat;
                   Imagem[5] = (BITMAP *)Imagens[5].dat;
                  
                   }
void Cenario::Desenhar(BITMAP *Buffer, BITMAP *Imagem,int x,int y)                   
{                    
             
             /* stretch_sprite(Buffer, Imagem, 0, 0, SCREEN_W, SCREEN_H);*/
             masked_blit(Imagem,Buffer,0,0,x,y,SCREEN_W,SCREEN_H);
             
 }
