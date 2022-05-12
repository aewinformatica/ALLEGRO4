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

void configura_teclas(void)
{
   cima = KEY_W;
   baixo = KEY_S;
   esquerda = KEY_A;
   direita = KEY_D;
   pula = KEY_SPACE;
   corre = KEY_LCONTROL;
}

void le_entrada(void)
{   
   register int i,j,k,temp;

   t.cima = key[cima];
   t.baixo = key[baixo];
   t.direita = key[direita];
   t.esquerda = key[esquerda];
   t.pula = key[pula];

   if (mario->estado == ESTADO_PULANDO || mario->estado == ESTADO_CAINDO)
      t.corre = t.corre & key[corre];
   else t.corre = key[corre];

   /* verifica se entrou numa warp zone */
   if (mario->estado == ESTADO_PARADO && t.baixo)
   {
      for(i = 0; i < fase_camada; i++)
         if (tipo_camada[i] == CAMADA_TILES)
         {
            if ((mario->fsm & 0x00FF) == MARIO_PEQUENO)
               j = mario->y / TILE_SIZE;
            else j = (mario->y + (TILE_SIZE_EXTRA - TILE_SIZE)) / TILE_SIZE;
            k = mario->x / TILE_SIZE;

            temp = mapa[i][j+1][k] & 0x00FF;
            if (TILE_CANO_TOPO(temp))
            {
               temp = mapa[i][j+1][k+1] & 0x00FF;
               if (TILE_CANO_TOPO(temp))
               {
                  /* verifica qual eh o destino */
                  temp = (mapa[i][j+1][k] & 0x0F00) >> 8;
                  if (temp > 0 && temp < 16)
                  {
                     mario_warp_x = (warp_zone_x[temp] * TILE_SIZE) - (TILE_SIZE / 2);
                     mario_warp_y = warp_zone_y[temp] * TILE_SIZE;
                     mario->estado = ESTADO_ESPIRITO;
                     mario->contador = -1;
                     play_sample(wav_cano,255,128,1000,FALSE);
                  }
               }
            }
         }
   }

   if (t.esquerda)
   {
      mario->x -= MARIO_VELOCIDADE;
      if (t.corre) mario->x -= MARIO_VELOCIDADE;
      if (mario->estado == ESTADO_PARADO) mario->estado = ESTADO_ANDANDO;
      mario_anda();
      mario->direcao = DIRECAO_ESQUERDA;
   }
   else if (t.direita)
   {
      mario->x += MARIO_VELOCIDADE;
      if (t.corre) mario->x += MARIO_VELOCIDADE;
      if (mario->estado == ESTADO_PARADO) mario->estado = ESTADO_ANDANDO;
      mario_anda();
      mario->direcao = DIRECAO_DIREITA;
   }
   else if (mario->estado == ESTADO_ANDANDO)
   {
      mario->estado = ESTADO_PARADO;
      mario_para();
   }

   if (t.pula && mario->contador == 0 && mario->estado != ESTADO_CAINDO && !t.trava_pulo)
   {
      mario->estado = ESTADO_PULANDO;
      mario->contador++;
      min_alcance_pulo = 0;
      t.trava_pulo = TRUE;

      play_sample(wav_pulo,255,128,1000,FALSE);
   }
   else if (!t.pula && mario->estado == ESTADO_PULANDO && mario->contador > min_alcance_pulo)
   {
      mario->contador = DURACAO_PULO - 2;
      min_alcance_pulo = DURACAO_PULO;
   }

   if (!t.pula && mario->estado != ESTADO_PULANDO && mario->estado != ESTADO_CAINDO)
      t.trava_pulo = FALSE;
}

void move_mario(void)
{
   if (mario->estado >= ESTADO_MORTO)
   {
      if (mario->estado == ESTADO_PARABOLA)
      {
         if (mario->contador < DURACAO_PULO)
            mario->y -= pulo[mario->contador++];
         else
         {
            mario->y += pulo[DURACAO_PULO - (mario->contador++ - DURACAO_PULO)];
            if (mario->contador > 2 * DURACAO_PULO) 
               mario->contador = 2 * DURACAO_PULO;
         }
         if (fora_da_fase(mario)) mario->estado = ESTADO_MORTO;
      }
      return;
   }

   /* entrando pelo cano */
   if (mario->estado == ESTADO_ESPIRITO)
   {
      if (mario->contador < 0)
      {
         mario->y++;
         if ((mario->contador == -TILE_SIZE && (mario->fsm & 0x00FF) == MARIO_PEQUENO) ||
            mario->contador == -TILE_SIZE_EXTRA)
         {
            mario->contador = 1;
            mario->x = mario_warp_x;
            mario->y = mario_warp_y;
            play_sample(wav_cano,255,128,1000,FALSE);
         }
         else mario->contador--;
      }
      else
      {
         mario->y--;
         if ((mario->contador == TILE_SIZE && (mario->fsm & 0x00FF) == MARIO_PEQUENO) ||
            mario->contador == TILE_SIZE_EXTRA)
         {
            mario->estado = ESTADO_PARADO;
            mario->contador = 0;
         }
         else mario->contador++;
      }
      return;
   }

   mario->x_ant = mario->x;
   mario->y_ant = mario->y;

   if (mario->estado == ESTADO_PULANDO)
   {
      mario->contador++;
      if (mario->contador >= DURACAO_PULO)
      {
         mario->estado = ESTADO_CAINDO;
         mario->contador--;
      }
   }

   if (mario->fsm >= MARIO_PEQUENO_INVENCIVEL)
   {
      mario->fsm += 0x0100;
      if ((mario->fsm >> 8) >= DURACAO_INVENCIVEL)
         mario->fsm &= 0x00FF;
   }

   le_entrada();

   if (mario->estado == ESTADO_CAINDO || mario->estado == ESTADO_PULANDO)
      mario_pula();

   if (mario->estado == ESTADO_PULANDO)
      mario->y -= mult_pulo * pulo[mario->contador];
   if (mario->estado == ESTADO_CAINDO)
   {
      mario->y += pulo[mario->contador--];
      mult_pulo = 1;
   }
   if (mario->contador < 0 && mario->estado != ESTADO_ESPIRITO) mario->contador = 0;

   /* ve se o mario morreu caindo num buraco */
   if (mario->y > fase_mapa_y * TILE_SIZE) mario_morre(TRUE);

   if (mario->x < 0) mario->x = 0;
   if (mario->x > (fase_mapa_x * TILE_SIZE) - imagem[mario->quadro_atual]->w)
      mario->x = (fase_mapa_x * TILE_SIZE) - imagem[mario->quadro_atual]->w;
}

