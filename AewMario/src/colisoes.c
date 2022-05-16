#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include <winalleg.h>

#include "mario.h"
#include "imagens.h"
#include "scroll.h"
#include "sprites.h"
#include "fase.h"
#include "colisoes.h"
#include "entrada.h"

/* calcula a direcao da colisao */
void calcula_direcao(int i,int j,int i1,int j1,int i2,int j2,SPRITE *_sprite)
{
   int dx,dy;

   dx = _sprite->x - _sprite->x_ant;
   dy = _sprite->y - _sprite->y_ant;

   if (dy > 0 && j > j1) _sprite->colisao[DIRECAO_BAIXO] = TRUE;
   if (dy < 0 && j < j2) _sprite->colisao[DIRECAO_CIMA] = TRUE;
   if (dx > 0 && i > i1) _sprite->colisao[DIRECAO_DIREITA] = TRUE;
   if (dx < 0 && i < i2) _sprite->colisao[DIRECAO_ESQUERDA] = TRUE;
}

/* colisao de sprite(i,dx,dy) com tile(j) com direcao (dir) e passo (conta) */
/* TRUE == cabecada no tile! */
int trata_colisao_tile(int i, int j, int dx, int dy, int conta)
{
   int eixo_y = 1;

   if (sprite[i].x != sprite[i].x_ant)
   {
      if (sprite[i].x > sprite[i].x_ant)
      {
         sprite[i].x -= (sprite[i].x % TILE_SIZE);
         if (sprite[i].x < sprite[i].x_ant) sprite[i].x = sprite[i].x_ant;
      }
      else
      {
         sprite[i].x += (sprite[i].x % TILE_SIZE);
         if (sprite[i].x > sprite[i].x_ant) sprite[i].x = sprite[i].x_ant;
      }
      eixo_y = 0;
   }
   else if (dy > 0)
      sprite[i].y = sprite[i].y_ant + dy - conta;
   else if (dy < 0)
      sprite[i].y = sprite[i].y_ant + dy + conta;

   /* i == 0 <=> mario */
   if (i == 0)
   {
      if (TILE_SAIDA(j)) passou_de_fase = TRUE;
      if (TILE_ESTACA(j) || TILE_LAVA(j))
      {
         mario_morre(TRUE);
         return(FALSE);
      }
      if (sprite[i].colisao[DIRECAO_ESQUERDA])
      {
      }
      if (sprite[i].colisao[DIRECAO_DIREITA])
      {
      }
      if (sprite[i].colisao[DIRECAO_BAIXO] && eixo_y)
      {
         if (mario->estado == ESTADO_CAINDO)
         {
            mario->contador = 0;
            mario->estado = ESTADO_PARADO;
            mario_para();
         }
         if (TILE_NOTA(j))
         {
            mario->contador = 0;
            mario->estado = ESTADO_PULANDO;
            min_alcance_pulo = DURACAO_PULO;
            mult_pulo = 2;
            play_sample(wav_pulo,255,128,1000,FALSE);
            return(TRUE);
         }
      }
      if (sprite[i].colisao[DIRECAO_CIMA] && eixo_y)
      {
         if (mario->estado == ESTADO_PULANDO)
            mario->estado = ESTADO_CAINDO;
         if (TILE_NOTA(j)) return(FALSE);
         return(TRUE);
      }

      if (eixo_y && dy == 0) return(TRUE);
   }
   else if (SPRITE_MOEDA(sprite[i].familia))
   {
      pega_moeda(sprite + i);
   }
   else
   {
      if (SPRITE_GOOMBA(sprite[i].familia))
      {
         if (TILE_ESTACA(j) || TILE_LAVA(j))
         {
            morre_goomba(sprite+i);
            return(FALSE);
         }
         sprite[i].fsm = (sprite[i].fsm == FSM_GOOMBA_ESQUERDA) ? 
            FSM_GOOMBA_DIREITA : FSM_GOOMBA_ESQUERDA;
         sprite[i].estado = ESTADO_PARADO;
      }
      else if (SPRITE_SPINY(sprite[i].familia))
      {
         if (TILE_ESTACA(j) || TILE_LAVA(j))
         {
            morre_spiny(sprite+i);
            return(FALSE);
         }
         sprite[i].fsm = (sprite[i].fsm == FSM_SPINY_ESQUERDA) ? 
            FSM_SPINY_DIREITA : FSM_SPINY_ESQUERDA;
         sprite[i].estado = ESTADO_PARADO;
      }
      else if (SPRITE_KOOPA(sprite[i].familia))
      {
         if (TILE_ESTACA(j) || TILE_LAVA(j))
         {
            morre_koopa(sprite+i);
            return(FALSE);
         }

         sprite[i].estado = ESTADO_PARADO;

         if (sprite[i].fsm < FSM_KOOPA_PISADO)
         {
            sprite[i].fsm = (sprite[i].fsm == FSM_KOOPA_ESQUERDA) ?
               FSM_KOOPA_DIREITA : FSM_KOOPA_ESQUERDA;
         }
         else 
         {
            if (dy == 0)
            {
               sprite[i].fsm = (sprite[i].fsm == FSM_KOOPA_CHUTADO_ESQUERDA) ?
                  FSM_KOOPA_CHUTADO_DIREITA : FSM_KOOPA_CHUTADO_ESQUERDA;
               return(TRUE);
            }
         }
      }
      else if (SPRITE_COGUMELO(sprite[i].familia))
      {
         if (!sprite[i].colisao[DIRECAO_BAIXO])
            sprite[i].fsm = (sprite[i].fsm == FSM_COGUMELO_ESQUERDA) ?
               FSM_COGUMELO_DIREITA : FSM_COGUMELO_ESQUERDA;
         sprite[i].estado = ESTADO_PARADO;
      }
   }

   return(FALSE);
}

/* trata colisao entre sprite(i) e bloco(j) de baixo que levou uma cabecada do mario */
void trata_colisao_cabecada(int i, int j)
{
   if (TILE_NOTA(j)) return;

   if (SPRITE_MOEDA(sprite[i].familia))
   {
      sprite[i].estado = ESTADO_PARABOLA;
      sprite[i].fsm = FSM_MOEDA_SUBINDO;
      sprite[i].contador = 2;
   }
   else if (SPRITE_COGUMELO(sprite[i].familia))
   {
      sprite[i].y -= 4;
      sprite[i].estado = ESTADO_PARADO;
   }
   else if (SPRITE_GOOMBA(sprite[i].familia))
   {
      morre_goomba(sprite+i);
   }
   else if (SPRITE_KOOPA(sprite[i].familia))
   {
      if ((mario->fsm & 0x00FF) != MARIO_PEQUENO)
      {
         morre_koopa(sprite+i);
      }
      else
      {
         if (sprite[i].fsm < FSM_KOOPA_PISADO)
         {
            sprite[i].y += TILE_SIZE_EXTRA - TILE_SIZE;
            play_sample(wav_parabola,255,128,1000,FALSE);
         }
         sprite[i].quadro_atual = KOOPA_CHUTADO_01;
         sprite[i].fsm = FSM_KOOPA_PISADO;
         sprite[i].estado = ESTADO_PARADO;
      }
   }
}

/* colisao entre sprite(i) e sprite(j) */
void trata_colisao_sprite(int i, int j)
{
   int dx,dy;
   SPRITE *_sprite,*koopa;

   /* i == 0 <=> mario */
   if (i == 0)
   {
      dx = mario->x - mario->x_ant;
      dy = mario->y - mario->y_ant;

      if (SPRITE_MOEDA(sprite[j].familia))
      {
         pega_moeda(sprite + j);
      }
      else if (SPRITE_COGUMELO(sprite[j].familia) && sprite[j].estado != ESTADO_ESPIRITO)
      {
         pega_cogumelo(sprite + j);
      }
      else if (SPRITE_GOOMBA(sprite[j].familia))
      {
         if (dy > 0) pisa_goomba(sprite + j);
         else mario_morre(FALSE);
      }
      else if (SPRITE_PEIXE(sprite[j].familia))
      {
         if ((dy >= 0 && sprite[j].fsm == FSM_PEIXE_SUBINDO) ||
            (dy < 0 && sprite[j].fsm == FSM_PEIXE_SUBINDO && mario->y < sprite[j].y))
            pisa_peixe(sprite + j);
         else mario_morre(FALSE);
      }
      else if (SPRITE_KOOPA(sprite[j].familia))
      {
         if (dy > 0)
         {
            pisa_koopa(sprite + j);
            mario->y = sprite[j].y - imagem[mario->quadro_atual]->h;
         }
         else if (sprite[j].fsm == FSM_KOOPA_PISADO) chuta_koopa(sprite + j);
         else mario_morre(FALSE);
      }
      else if (SPRITE_BOLA_DE_FOGO(sprite[j].familia) || SPRITE_SPINY(sprite[j].familia))
      {
         mario_morre(FALSE);
      }
   }
   else if (SPRITE_KOOPA(sprite[i].familia) || SPRITE_KOOPA(sprite[j].familia))
   {
      if (SPRITE_KOOPA(sprite[i].familia))
      {
         koopa = &sprite[i];
         _sprite = &sprite[j];
      }
      else
      {
         koopa = &sprite[j];
         _sprite = &sprite[i];
      }

      /* koopa chutado matando goomba */
      if (koopa->fsm > FSM_KOOPA_PISADO && SPRITE_GOOMBA(_sprite->familia))
      {
         morre_goomba(_sprite);
         return;
      }
      /* koopa chutado matando spiny */
      else if (koopa->fsm > FSM_KOOPA_PISADO && SPRITE_SPINY(_sprite->familia))
      {
         morre_spiny(_sprite);
         return;
      }
      /* koopa chutado matando peixe */
      else if (koopa->fsm > FSM_KOOPA_PISADO && SPRITE_PEIXE(_sprite->familia))
      {
         morre_peixe(_sprite);
         return;
      }

      /* colisao de koopas */
      if (SPRITE_KOOPA(_sprite->familia))
      {
         /* guarda valores */
         dx = koopa->fsm;
         dy = _sprite->fsm;
   
         /* e ve quem matou quem (ou ambos morrem) */
         if (dx > FSM_KOOPA_PISADO) morre_koopa(_sprite);
         if (dy > FSM_KOOPA_PISADO) morre_koopa(koopa);
      }
   }
}

void detecta_colisoes(void)
{
   register int i,j,k,l,i1,j1,i2,j2,k1,k2,l1,l2;
   int colidiu,dx,dy,passo,tipo;
   int debug;

   /* sprites com tiles */
   for(k = 0; k < fase_camada; k++)
      if (tipo_camada[k] == CAMADA_TILES)
         for(l = 0; l < fase_sprites; l++)
            if (sprite[l].estado < ESTADO_MORTO && 
                sprite[l].estado != ESTADO_ESPIRITO &&
                sprite[l].estado != ESTADO_INVISIVEL)
            {
               dx = sprite[l].x - sprite[l].x_ant;
               dy = sprite[l].y - sprite[l].y_ant;

               debug = 0;
               for(colidiu = 1, passo = 0; colidiu; passo++)
               {
                  if (sprite[l].estado >= ESTADO_MORTO || 
                     sprite[l].estado == ESTADO_ESPIRITO ||
                     sprite[l].estado == ESTADO_INVISIVEL)
                     break;

                  if (++debug > 32) break;

                  colidiu = 0;
                  CALCULA_LIMITES;
                  for(i = 0; i < 4; i++)
                     sprite[l].colisao[i] = FALSE;

                  for(j = j2; j >= j1 && !colidiu; j--)
                     for(i = i2; i >= i1 && !colidiu; i--)
                     {
                        /* pega a parte superior do tile */
                        tipo = (mapa[k][j][i] & 0xFF00) >> 8;
                        mapa[k][j][i] &= 0x00FF;

                        if (mapa[k][j][i] || tipo || cabecada[k][j-mundo_y[k]][i-mundo_x[k]] < 0)
                        {
                           calcula_direcao(i,j,i1,j1,i2,j2,sprite+l);
                           
                           /* bloco escondido */
                           if (!sprite[l].colisao[DIRECAO_CIMA] && !mapa[k][j][i])
                           {
                              mapa[k][j][i] += (tipo << 8);
                              continue;
                           }

                           /* os outros */
                           if (trata_colisao_tile(l,mapa[k][j][i],dx,dy,passo))
                           {
                              if (!l && cabecada[k][j-mundo_y[k]][i-mundo_x[k]] == 0)
                              {
                                 if (TILE_INTERROGACAO(mapa[k][j][i]) || 
                                    TILE_INTERROGACAO_ESCONDIDO(mapa[k][j][i]))
                                 {
                                    cabecada[k][j-mundo_y[k]][i-mundo_x[k]] = -1;
                                    mapa[k][j][i] = INTERROGACAO_MORTO;
                                    cabecada_interrogacao(i,j,tipo);
                                    play_sample(wav_bump,255,128,1000,FALSE);
                                 }
                                 else if (TILE_NOTA(mapa[k][j][i]))
                                 {
                                    cabecada[k][j-mundo_y[k]][i-mundo_x[k]] = -1;
                                 }
                                 else if ((mario->fsm & 0x00FF) == MARIO_PEQUENO)
                                 {                                 
                                    if (TILE_BLOCO(mapa[k][j][i]))
                                    {
                                       cabecada[k][j-mundo_y[k]][i-mundo_x[k]] = -1;
                                    }
                                    play_sample(wav_bump,255,128,1000,FALSE);
                                 }
                                 else
                                 {
                                    if (TILE_BLOCO(mapa[k][j][i]))
                                    {
                                       cabecada[k][j-mundo_y[k]][i-mundo_x[k]] = -1;
                                       destroi_bloco(i,j,k);
                                    }
                                    else play_sample(wav_bump,255,128,1000,FALSE);
                                 }
                              }
                              else if (l)
                              {
                                 if (TILE_BLOCO(mapa[k][j][i]))
                                 {
                                    destroi_bloco(i,j,k);
                                 }
                                 else if (TILE_INTERROGACAO(mapa[k][j][i]) || 
                                    TILE_INTERROGACAO_ESCONDIDO(mapa[k][j][i]))
                                 {
                                    mapa[k][j][i] = INTERROGACAO_MORTO;
                                    cabecada_interrogacao(i,j,tipo);
                                    if (!fora_da_tela(sprite + l))
                                       play_sample(wav_bump,255,128,1000,FALSE);
                                 }
                                 else if (!fora_da_tela(sprite + l))
                                    play_sample(wav_bump,255,128,1000,FALSE);
                              }
                           }
                           colidiu++;
                        }
                        else if(l && cabecada[k][j-mundo_y[k]+1][i-mundo_x[k]] < 0)
                        {
                           /* k1 e k2 servem para verificar se realmente o bloco que levou */
                           /* uma cabecada colidiu com o sprite acima */
                           k1 = sprite[l].y + imagem[sprite[l].quadro_atual]->h - 1;
                           k2 = (j * TILE_SIZE) - abs(cabecada[k][j-mundo_y[k]+1][i-mundo_x[k]]);
                           if (k1 >= k2)
                           {
                              trata_colisao_cabecada(l,mapa[k][j+1][i]);
                              colidiu++;
                           }
                        }

                        /* coloca de volta a parte superior */
                        mapa[k][j][i] += (tipo << 8);
                     }
               }

               /* verifica se vai cair (sem chao embaixo) */
               if (sprite[l].estado == ESTADO_ANDANDO || sprite[l].estado == ESTADO_PARADO ||
                   sprite[l].estado == ESTADO_CAINDO)
               {
                  CALCULA_LIMITES;
                  j2 = (sprite[l].y + imagem[sprite[l].quadro_atual]->h) / TILE_SIZE;

                  /* ve se tem algo na linha abaixo do sprite para cada coluna do mapa */
                  for(i = i1; i <= i2; i++)
                     if ((mapa[k][j2][i] & 0xFF) > 0)
                        break;

                  /* se nao tiver, ele cai */
                  if (i > i2)
                  {
                     if (sprite[l].contador == 0 && sprite[l].estado != ESTADO_CAINDO)
                        sprite[l].contador = DURACAO_PULO-1;
                     sprite[l].estado = ESTADO_CAINDO;

                     if (!l) mario_pula();
                  }
               }
            }

   /* sprites com sprites */
   for(i = 0; i < fase_sprites; i++) 
      if (sprite[i].estado < ESTADO_MORTO)
         for(j = i+1; j < fase_sprites; j++)
            if (sprite[j].estado < ESTADO_MORTO)
            {
               i1 = sprite[i].x;
               j1 = sprite[i].y;
               i2 = imagem[sprite[i].quadro_atual]->w;
               j2 = imagem[sprite[i].quadro_atual]->h;

               k1 = sprite[j].x;
               l1 = sprite[j].y;
               k2 = imagem[sprite[j].quadro_atual]->w;
               l2 = imagem[sprite[j].quadro_atual]->h;

               if (colide(i1,j1,i2,j2,k1,l1,k2,l2))
                  trata_colisao_sprite(i,j);
            }

   /* "limpa" os sprites mortos (tirando o mario) */
   for(i = 1; i < fase_sprites; i++)
      if (sprite[i].estado == ESTADO_MORTO)
      {
         memcpy((void *)(sprite + i),(void *)(sprite + fase_sprites - 1),sizeof(sprite[0]));
         fase_sprites--;
         i--;
      }
}
