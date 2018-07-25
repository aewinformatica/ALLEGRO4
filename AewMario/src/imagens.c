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

void le_imagens(void)
{
   register int i,j,k,l,m;
   PALETTE pal;
   BITMAP *temp = (modo_16_bits) ? load_bmp("gfx/blocos.bmp",pal) : load_bmp("gfx/blocos2.bmp",pal);

   set_palette(pal);
   status = load_bmp("gfx/status.bmp",pal);
   gameover = load_bmp("gfx/gameover.bmp",pal);
   
   hiper = load_bmp("gfx/logo24.bmp",NULL);
   fim = load_bmp("gfx/fim.bmp",pal);
   
   for(j = 0, k = 1; j < 12 && k < MAX_IMAGEM; j++)
      for(i = 0; i < 20 && k < MAX_IMAGEM; i++, k++)
      {
         if (k <= MAX_TILE) /* 16x16 */
         {
            imagem[k] = create_bitmap(TILE_SIZE,TILE_SIZE);
            blit(temp,imagem[k],i*TILE_SIZE,j*TILE_SIZE,0,0,TILE_SIZE,TILE_SIZE);
         }
         else /* 16x27, sprites maiores (mario grande, koopa etc.) */
         {
            imagem[k] = create_bitmap(TILE_SIZE,TILE_SIZE_EXTRA);
            blit(temp,imagem[k],i*TILE_SIZE,j*TILE_SIZE,0,0,TILE_SIZE,TILE_SIZE_EXTRA);
         }

         /* acerta fundo e preto para modo 256 cores */
         if (!modo_16_bits)
            for(l = 0; l < imagem[k]->h; l++)
               for(m = 0; m < imagem[k]->w; m++)
               {
                  if (imagem[k]->line[l][m] == 0)
                     imagem[k]->line[l][m] = COR_PRETO;
                  else if (imagem[k]->line[l][m] == FUNDO_16_BITS)
                     imagem[k]->line[l][m] = 0;
               }
      }

   /* o zero eh vazio */
   imagem[0] = create_bitmap(TILE_SIZE,TILE_SIZE);
   if (modo_16_bits)
      blit(temp,imagem[0],temp->w-TILE_SIZE,temp->h-TILE_SIZE,0,0,TILE_SIZE,TILE_SIZE);
   else clear(imagem[0]);
}
