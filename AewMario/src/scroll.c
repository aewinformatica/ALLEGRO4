#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>

#include "mario.h"
#include "imagens.h"
#include "scroll.h"
#include "sprites.h"
#include "fase.h"
#include "colisoes.h"
#include "entrada.h"

void blit_scrolled(void)
{
   char msg[256];

   #ifdef VERSAO_640_480
      stretch_blit(status,tela,0,0,status->w,status->h,0,
                   RES_TILE_Y*TILE_SIZE*2,RES_TILE_X*TILE_SIZE*2,TILE_SIZE*2);
      sprintf(msg,"%d",mario_vidas);
      textout(tela,font,msg,60,461,-1);
      sprintf(msg,"%02d",mario_moedas);
      textout(tela,font,msg,160,461,-1);
      masked_blit(tela,screen,0,0,0,0,RES_TILE_X*TILE_SIZE*2,(RES_TILE_Y+1)*TILE_SIZE*2);
   #else
      blit(status,tela,0,0,0,RES_TILE_Y*TILE_SIZE,RES_TILE_X*TILE_SIZE,TILE_SIZE);
      sprintf(msg,"%d",mario_vidas);
      textout_ex(tela,font,msg,30,229,-1,-1);
      sprintf(msg,"%02d",mario_moedas);
      textout_ex(tela,font,msg,80,229,-1,-1);
      masked_blit(tela,screen,0,0,0,0,RES_TILE_X*TILE_SIZE,(RES_TILE_Y+1)*TILE_SIZE);
   #endif
}

void calcula_scroll(void)
{
   register int i,j,temp;
   int x = mario->x / TILE_SIZE;
   int y = mario->y / TILE_SIZE;

   /* scroll meia tela */
   if (x >= RES_TILE_X / 2)
   {
      if (x < fase_mapa_x - (RES_TILE_X / 2))
      {
         scroll_x = mario->x - (TILE_SIZE * RES_TILE_X / 2);
      }
      else scroll_x = TILE_SIZE * ((fase_mapa_x - (RES_TILE_X / 2)) - (RES_TILE_X / 2));
   }
   else scroll_x = 0;

   for(i = 0; i < fase_camada; i++)
   {
      temp = mundo_x[i];
      mundo_x[i] = (velocidade_x[i] * (scroll_x / MARIO_VELOCIDADE)) / TILE_SIZE;

      /* recalcula vetor de cabecadas */
      if (tipo_camada[i] == CAMADA_TILES)
      {
         if (mundo_x[i] > temp)
            for(j = 0; j < RES_TILE_Y; j++)
            {
               memcpy((void *)&cabecada[i][j][0],(void *)&cabecada[i][j][1],sizeof(cabecada[i][j]));
               cabecada[i][j][RES_TILE_X-1] = 0;
            }
         else if (mundo_x[i] < temp)        
            for(j = 0; j < RES_TILE_Y; j++)
            {
               memcpy((void *)&cabecada[i][j][1],(void *)&cabecada[i][j][0],sizeof(cabecada[i][j]));
               cabecada[i][j][0] = 0;
            }
      }
   }

   /* scroll meia tela */
   if (y >= RES_TILE_Y / 2)
   {
      if (y < fase_mapa_y - (RES_TILE_Y / 2))
      {
         scroll_y = mario->y - (TILE_SIZE * (RES_TILE_Y / 2));
      }
      else scroll_y = TILE_SIZE * ((fase_mapa_y - (RES_TILE_Y / 2)) - (RES_TILE_Y / 2));
   }
   else scroll_y = 0;

   for(i = 0; i < fase_camada; i++)
   {
      temp = mundo_y[i];
      mundo_y[i] = ((velocidade_y[i] / MARIO_VELOCIDADE) * scroll_y) / TILE_SIZE;

      /* recalcula vetor de cabecadas */
      if (tipo_camada[i] == CAMADA_TILES)
      {
         if (mundo_y[i] > temp)
         {
            for(j = 0; j < RES_TILE_Y - 1; j++)
               memcpy((void *)&cabecada[i][j][0],(void *)&cabecada[i][j+1][0],sizeof(cabecada[i][j]));
            memset((void *)cabecada[i][RES_TILE_Y-1],0,sizeof(cabecada[i][RES_TILE_Y-1]));
         }
         else if (mundo_y[i] < temp)
         {
            for(j = RES_TILE_Y-1; j > 0; j--)
               memcpy((void *)&cabecada[i][j][0],(void *)&cabecada[i][j-1][0],sizeof(cabecada[i][j]));
            memset((void *)cabecada[i][0],0,sizeof(cabecada[i][0]));
         }
      }
   }
}
