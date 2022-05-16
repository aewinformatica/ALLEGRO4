#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro.h>

#include "mario.h"
#include "imagens.h"
#include "scroll.h"
#include "sprites.h"
#include "fase.h"
#include "colisoes.h"
#include "entrada.h"

void cria_sprite(int x, int y, int quadro, int delay)
{
   sprite[fase_sprites].id = fase_sprites;
   sprite[fase_sprites].familia = quadro;
   sprite[fase_sprites].quadro_atual = quadro;
   sprite[fase_sprites].x = sprite[fase_sprites].x_ant = x;
   sprite[fase_sprites].y = sprite[fase_sprites].y_ant = y;
   sprite[fase_sprites].direcao = DIRECAO_DIREITA;
   sprite[fase_sprites].delay = delay;
   sprite[fase_sprites].contador = 0;
   sprite[fase_sprites].fsm = 0;

   /* para sprites mais altos do que os normais */
   if (quadro > MAX_TILE) sprite[fase_sprites].y -= TILE_SIZE_EXTRA - TILE_SIZE;

   if (SPRITE_MOEDA(quadro))
      sprite[fase_sprites].estado = ESTADO_ESPIRITO;
   else if (SPRITE_BOLA_DE_FOGO(quadro) || SPRITE_PEIXE(quadro))
   {
      sprite[fase_sprites].estado = ESTADO_INVISIVEL;
      sprite[fase_sprites].contador = rand() % MAX_ESPERA_FOGO_PEIXE;
   }
   else sprite[fase_sprites].estado = ESTADO_PARADO;

   fase_sprites++;
}

void pega_cogumelo(SPRITE *cogumelo)
{
   cogumelo->estado = ESTADO_MORTO;
   if ((mario->fsm & 0x00FF) == MARIO_PEQUENO)
   {
      mario->y -= TILE_SIZE_EXTRA - TILE_SIZE;
      mario->fsm = MARIO_GRANDE_INVENCIVEL;
      if (mario->estado == ESTADO_ANDANDO) mario_anda();
      else if (mario->estado == ESTADO_PARADO) mario_para();
      else if (mario->estado == ESTADO_PULANDO) mario_pula();
      play_sample(wav_cresce,255,128,1000,FALSE);
   }
}

void pega_moeda(SPRITE *moeda)
{
   moeda->familia = FUMACA_01;
   moeda->quadro_atual = FUMACA_01;
   moeda->contador = 20;
   moeda->estado = ESTADO_MORRENDO;
   play_sample(wav_moeda,255,128,1000,FALSE);

   mario_moedas++;
   if (mario_moedas == 100)
   {
      mario_moedas = 0;
      if(mario_vidas <98){
      mario_vidas++;
      play_sample(wav_1up,255,128,1000,FALSE);	  
	  }
	  
   }
}

void move_moeda(SPRITE *moeda)
{
   switch(moeda->fsm)
   {
      case FSM_MOEDA_PARADA:
      break;

      case FSM_MOEDA_SUBINDO:
         moeda->y -= pulo[moeda->contador++];
         if (moeda->contador >= DURACAO_PULO) pega_moeda(moeda);
      break;
   }
}

void cabecada_interrogacao(int i, int j, int tipo)
{
   if (SPRITE_COGUMELO(tipo))
   {
      cria_sprite(i*TILE_SIZE,j*TILE_SIZE-1,COGUMELO,4);
      sprite[fase_sprites-1].estado = ESTADO_ESPIRITO;
      play_sample(wav_cogumelo,255,128,1000,FALSE);
   }
   else if (SPRITE_MOEDA(tipo))
   {
      cria_sprite(i*TILE_SIZE,j*TILE_SIZE,MOEDA_01,4);
      sprite[fase_sprites-1].estado = ESTADO_PARABOLA;
      sprite[fase_sprites-1].fsm = FSM_MOEDA_SUBINDO;
      sprite[fase_sprites-1].contador = 2;
   }
}

void pisa_goomba(SPRITE *goomba)
{
   goomba->familia = GOOMBA_MORTO;
   goomba->quadro_atual = GOOMBA_MORTO;
   goomba->contador = 10;
   goomba->estado = ESTADO_MORRENDO;
   mario->contador = 0;
   mario->estado = ESTADO_PULANDO;
   min_alcance_pulo = DURACAO_PULO;
   play_sample(wav_pulo,255,128,1000,FALSE);
}

void pisa_peixe(SPRITE *peixe)
{
   morre_peixe(peixe);
   mario->contador = 0;
   mario->estado = ESTADO_PULANDO;
   min_alcance_pulo = DURACAO_PULO;
   play_sample(wav_pulo,255,128,1000,FALSE);
}

void chuta_koopa(SPRITE *koopa)
{
   int meio_mario,meio_koopa;

   /* usa o meio do eixo de cada pra decidir pra que lado vai o koopa */
   meio_mario = mario->x + (imagem[mario->quadro_atual]->w / 2);
   meio_koopa = koopa->x + (imagem[koopa->quadro_atual]->w / 2);

   koopa->fsm = (meio_mario > meio_koopa) ? FSM_KOOPA_CHUTADO_ESQUERDA : FSM_KOOPA_CHUTADO_DIREITA;
}

void pisa_koopa(SPRITE *koopa)
{
   if (koopa->fsm != FSM_KOOPA_PISADO)
   {
      if (koopa->fsm < FSM_KOOPA_PISADO) koopa->y += TILE_SIZE_EXTRA - TILE_SIZE;
      koopa->quadro_atual = KOOPA_CHUTADO_01;
      koopa->fsm = FSM_KOOPA_PISADO;
      koopa->estado = ESTADO_PARADO;
   }
   else chuta_koopa(koopa);

   mario->contador = 0;
   mario->estado = ESTADO_PULANDO;
   min_alcance_pulo = DURACAO_PULO;
   play_sample(wav_pulo,255,128,1000,FALSE);
}

void desenha_sprite(SPRITE *_sprite)
{
   int w,h,x,y;


   if (fora_da_tela(_sprite))
   {
      if (_sprite->quadro_atual > -1)
         if ((lacos % _sprite->delay) == 0 && mario->estado < ESTADO_MORTO)
            _sprite->quadro_atual = proximo_quadro[_sprite->quadro_atual];
      return;
   }

   x = X_RELATIVO(_sprite->x);
   y = Y_RELATIVO(_sprite->y);

   /* usa imagem[MAX_IMAGEM-1] como temporario */
   if (modo_16_bits)
      blit(imagem[MAX_IMAGEM-2],imagem[MAX_IMAGEM-1],0,0,0,0,imagem[MAX_IMAGEM-1]->w,imagem[MAX_IMAGEM-1]->h);
   else clear(imagem[MAX_IMAGEM-1]);

   if (_sprite->quadro_atual > -1)
   {
      w = imagem[_sprite->quadro_atual]->w;
      h = imagem[_sprite->quadro_atual]->h;

      if (_sprite->estado == ESTADO_PARABOLA && _sprite->familia != 0)
         draw_sprite_v_flip(imagem[MAX_IMAGEM-1],imagem[_sprite->quadro_atual],0,0);
      else if (_sprite->direcao == DIRECAO_DIREITA)
         draw_sprite(imagem[MAX_IMAGEM-1],imagem[_sprite->quadro_atual],0,0);
      else
         draw_sprite_h_flip(imagem[MAX_IMAGEM-1],imagem[_sprite->quadro_atual],0,0);    

      /* testa delay para animar sprite */
      if ((lacos % _sprite->delay) == 0 && mario->estado < ESTADO_MORTO)
         _sprite->quadro_atual = proximo_quadro[_sprite->quadro_atual];
   }
   else
   {
      w = h = TILE_SIZE / 2;
      draw_sprite(imagem[MAX_IMAGEM-1],blocos_partidos[_sprite->delay],0,0);
   }
   #ifdef VERSAO_640_480
      masked_stretch_blit(imagem[MAX_IMAGEM-1],tela,0,0,w,h,x*2,y*2,w*2,h*2);
   #else
      draw_sprite(tela,imagem[MAX_IMAGEM-1],x,y);
   #endif
}

void desenha_sprites(void)
{
   register int i;

   for(i = 1; i < fase_sprites; i++)
      if (sprite[i].estado < ESTADO_MORTO && sprite[i].estado != ESTADO_INVISIVEL)
         desenha_sprite(sprite+i);

   if (mario->fsm >= MARIO_PEQUENO_INVENCIVEL && mario->estado != ESTADO_ESPIRITO)
   {
      if (((mario->fsm >> 8) % (2 * mario->delay)) != 0)
         desenha_sprite(mario);
   }
   else if (mario->estado < ESTADO_MORTO)
      desenha_sprite(mario);
}

/* desenha os sprites na frente de tudo (quando estao no estado parabola/morrendo) */
void desenha_sprites_frente(void)
{
   register int i;

   for(i = 0; i < fase_sprites; i++)
      if (sprite[i].estado > ESTADO_MORTO)
         desenha_sprite(sprite+i);
}

void mario_anda(void)
{
   if ((mario->fsm & 0x00FF) == MARIO_PEQUENO)
   {
      if (!SPRITE_MARIO_PEQUENO_ANDANDO(mario->quadro_atual))
         mario->quadro_atual = MARIO_PEQUENO_02;
   }
   else if ((mario->fsm & 0x00FF) == MARIO_GRANDE)
   {
      if (!SPRITE_MARIO_GRANDE_ANDANDO(mario->quadro_atual))
         mario->quadro_atual = MARIO_GRANDE_02;
   }
}

void mario_para(void)
{
   if ((mario->fsm & 0x00FF) == MARIO_PEQUENO)
      mario->quadro_atual = MARIO_PEQUENO_PARADO;
   else if ((mario->fsm & 0x00FF) == MARIO_GRANDE)
      mario->quadro_atual = MARIO_GRANDE_PARADO;
}

void mario_pula(void)
{
   if ((mario->fsm & 0x00FF) == MARIO_PEQUENO)
      mario->quadro_atual = MARIO_PEQUENO_PULANDO;
   else if ((mario->fsm & 0x00FF) == MARIO_GRANDE)
      mario->quadro_atual = MARIO_GRANDE_PULANDO;
}

/* morre_direto == TRUE <=> mario morre (independente de estar grande ou nao) */
void mario_morre(int morre_direto)
{
   if (mario->fsm >= MARIO_PEQUENO_INVENCIVEL && !morre_direto)
      return;

   if ((mario->fsm & 0x00FF) == MARIO_PEQUENO || morre_direto)
   {
      mario->quadro_atual = MARIO_PEQUENO_MORTO;
      mario->estado = ESTADO_PARABOLA;
      mario->contador = 0;
      stop_midi();
      play_sample(wav_morreu,255,128,1000,FALSE);
      mario_vidas--;
   }
   else
   {
      mario->y += TILE_SIZE_EXTRA - TILE_SIZE;
      mario->fsm = MARIO_PEQUENO_INVENCIVEL;
      if (mario->estado == ESTADO_ANDANDO) mario_anda();
      else if (mario->estado == ESTADO_PARADO) mario_para();
      else if (mario->estado == ESTADO_PULANDO) mario_pula();
      play_sample(wav_diminui,255,128,1000,FALSE);
   }
}

void destroi_bloco(int i, int j, int k)
{
   int w = TILE_SIZE/2, h = TILE_SIZE/2;

   blocos_partidos[topo_blocos_partidos] = create_bitmap(w,h);
   blit(imagem[mapa[k][j][i]],blocos_partidos[topo_blocos_partidos],0,0,0,0,w,h);
   cria_sprite(i*TILE_SIZE,j*TILE_SIZE,-1,topo_blocos_partidos++);
   sprite[fase_sprites-1].estado = ESTADO_PARABOLA;
   sprite[fase_sprites-1].contador = DURACAO_PULO/4; /* cima */
   sprite[fase_sprites-1].fsm = 2; /* esquerda */
   if (topo_blocos_partidos == MAX_PILHA_PARTIDOS) topo_blocos_partidos = 0;

   blocos_partidos[topo_blocos_partidos] = create_bitmap(w,h);
   blit(imagem[mapa[k][j][i]],blocos_partidos[topo_blocos_partidos],w,0,0,0,w,h);
   cria_sprite(i*TILE_SIZE+w,j*TILE_SIZE,-1,topo_blocos_partidos++);
   sprite[fase_sprites-1].estado = ESTADO_PARABOLA;
   sprite[fase_sprites-1].contador = DURACAO_PULO/4; /* cima */
   sprite[fase_sprites-1].fsm = 0; /* direita */
   if (topo_blocos_partidos == MAX_PILHA_PARTIDOS) topo_blocos_partidos = 0;

   blocos_partidos[topo_blocos_partidos] = create_bitmap(w,h);
   blit(imagem[mapa[k][j][i]],blocos_partidos[topo_blocos_partidos],0,h,0,0,w,h);
   cria_sprite(i*TILE_SIZE,j*TILE_SIZE+h,-1,topo_blocos_partidos++);
   sprite[fase_sprites-1].estado = ESTADO_PARABOLA;
   sprite[fase_sprites-1].contador = DURACAO_PULO/2; /* baixo */
   sprite[fase_sprites-1].fsm = 2; /* esquerda */
   if (topo_blocos_partidos == MAX_PILHA_PARTIDOS) topo_blocos_partidos = 0;

   blocos_partidos[topo_blocos_partidos] = create_bitmap(w,h);
   blit(imagem[mapa[k][j][i]],blocos_partidos[topo_blocos_partidos],w,h,0,0,w,h);
   cria_sprite(i*TILE_SIZE+w,j*TILE_SIZE+h,-1,topo_blocos_partidos++);
   sprite[fase_sprites-1].estado = ESTADO_PARABOLA;
   sprite[fase_sprites-1].contador = DURACAO_PULO/2; /* baixo */
   sprite[fase_sprites-1].fsm = 0; /* direita */
   if (topo_blocos_partidos == MAX_PILHA_PARTIDOS) topo_blocos_partidos = 0;

   mapa[k][j][i] = 0;
   play_sample(wav_quebra,255,128,1000,FALSE);
}

void morre_goomba(SPRITE *goomba)
{
   goomba->contador = DURACAO_PULO / 2;
   goomba->fsm = FSM_GOOMBA_SUBINDO;
   goomba->estado = ESTADO_PARABOLA;
   if (!fora_da_tela(goomba)) play_sample(wav_parabola,255,128,1000,FALSE);
}

void morre_koopa(SPRITE *koopa)
{
   koopa->contador = DURACAO_PULO / 2;
   koopa->fsm = FSM_KOOPA_SUBINDO;
   koopa->estado = ESTADO_PARABOLA;
   if (!fora_da_tela(koopa)) play_sample(wav_parabola,255,128,1000,FALSE);
}

void morre_spiny(SPRITE *spiny)
{
   spiny->contador = DURACAO_PULO / 2;
   spiny->fsm = FSM_SPINY_SUBINDO;
   spiny->estado = ESTADO_PARABOLA;
   if (!fora_da_tela(spiny)) play_sample(wav_parabola,255,128,1000,FALSE);
}

void morre_peixe(SPRITE *peixe)
{
   peixe->contador = DURACAO_PULO / 2;
   peixe->fsm = FSM_PEIXE_MORTO_SUBINDO;
   peixe->estado = ESTADO_PARABOLA;
   if (!fora_da_tela(peixe)) play_sample(wav_parabola,255,128,1000,FALSE);
}

void move_cogumelo(SPRITE *cogumelo)
{
   switch(cogumelo->fsm)
   {
      /* cogumelo subindo */
      case FSM_COGUMELO_SUBINDO:
         cogumelo->y--;
         if (cogumelo->y % TILE_SIZE == 0)
         {
            cogumelo->fsm = FSM_COGUMELO_DIREITA;
            cogumelo->estado = ESTADO_ANDANDO;
         }
      break;

      /* cogumelo andando pra direita */
      case FSM_COGUMELO_DIREITA:
         cogumelo->x += VELOCIDADE_COGUMELO;
         if (cogumelo->estado == ESTADO_PARADO)
            cogumelo->estado = ESTADO_ANDANDO;
      break;

      /* cogumelo andando pra esquerda */
      case FSM_COGUMELO_ESQUERDA:
         cogumelo->x -= VELOCIDADE_COGUMELO;
         if (cogumelo->estado == ESTADO_PARADO)
            cogumelo->estado = ESTADO_ANDANDO;
      break;
   }
}

void move_goomba(SPRITE *goomba)
{
   switch(goomba->fsm)
   {
      /* goomba andando pra esquerda */
      case FSM_GOOMBA_ESQUERDA:
         goomba->x -= VELOCIDADE_GOOMBA;
         goomba->direcao = DIRECAO_ESQUERDA;
         if (goomba->estado == ESTADO_PARADO)
            goomba->estado = ESTADO_ANDANDO;
      break;

      /* goomba andando pra direita */
      case FSM_GOOMBA_DIREITA:
         goomba->x += VELOCIDADE_GOOMBA;
         goomba->direcao = DIRECAO_DIREITA;
         if (goomba->estado == ESTADO_PARADO)
            goomba->estado = ESTADO_ANDANDO;
      break;

      /* goomba morto subindo */
      case FSM_GOOMBA_SUBINDO:
         goomba->y -= pulo[goomba->contador++];
         goomba->x -= VELOCIDADE_EIXO_X;
         if (goomba->contador >= DURACAO_PULO)
         {
            goomba->fsm = FSM_GOOMBA_CAINDO;
            goomba->contador--;
         }
      break;

      /* goomba morto caindo */
      case FSM_GOOMBA_CAINDO:
         goomba->y += pulo[goomba->contador--];
         goomba->x -= VELOCIDADE_EIXO_X;
         if (goomba->contador < 0) goomba->contador = 0;
         if (fora_da_tela(goomba)) goomba->estado = ESTADO_MORTO;
      break;
   }
}

void move_spiny(SPRITE *spiny)
{
   switch(spiny->fsm)
   {
      /* spiny andando pra esquerda */
      case FSM_SPINY_ESQUERDA:
         spiny->x -= VELOCIDADE_SPINY;
         spiny->direcao = DIRECAO_ESQUERDA;
         if (spiny->estado == ESTADO_PARADO)
            spiny->estado = ESTADO_ANDANDO;
      break;

      /* spiny andando pra direita */
      case FSM_SPINY_DIREITA:
         spiny->x += VELOCIDADE_SPINY;
         spiny->direcao = DIRECAO_DIREITA;
         if (spiny->estado == ESTADO_PARADO)
            spiny->estado = ESTADO_ANDANDO;
      break;

      /* spiny morto subindo */
      case FSM_SPINY_SUBINDO:
         spiny->y -= pulo[spiny->contador++];
         spiny->x -= VELOCIDADE_EIXO_X;
         if (spiny->contador >= DURACAO_PULO)
         {
            spiny->fsm = FSM_SPINY_CAINDO;
            spiny->contador--;
         }
      break;

      /* spiny morto caindo */
      case FSM_SPINY_CAINDO:
         spiny->y += pulo[spiny->contador--];
         spiny->x -= VELOCIDADE_EIXO_X;
         if (spiny->contador < 0) spiny->contador = 0;
         if (fora_da_tela(spiny)) spiny->estado = ESTADO_MORTO;
      break;
   }
}

void move_koopa(SPRITE *koopa)
{
   switch(koopa->fsm)
   {
      /* koopa andando pra esquerda */
      case FSM_KOOPA_ESQUERDA:
         koopa->x -= VELOCIDADE_KOOPA;
         koopa->direcao = DIRECAO_ESQUERDA;
         if (koopa->estado == ESTADO_PARADO)
            koopa->estado = ESTADO_ANDANDO;
      break;

      /* koopa andando pra direita */
      case FSM_KOOPA_DIREITA:
         koopa->x += VELOCIDADE_KOOPA;
         koopa->direcao = DIRECAO_DIREITA;
         if (koopa->estado == ESTADO_PARADO)
            koopa->estado = ESTADO_ANDANDO;
      break;

      /* koopa pisado */
      case FSM_KOOPA_PISADO:
         koopa->quadro_atual = KOOPA_CHUTADO_01;
         koopa->estado = ESTADO_PARADO;
      break;

      /* koopa chutado pra esquerda */
      case FSM_KOOPA_CHUTADO_ESQUERDA:
         koopa->x -= VELOCIDADE_KOOPA_CHUTADO;
         koopa->direcao = DIRECAO_ESQUERDA;
         if (koopa->estado == ESTADO_PARADO)
            koopa->estado = ESTADO_ANDANDO;
      break;

      /* koopa chutado pra direita */
      case FSM_KOOPA_CHUTADO_DIREITA:
         koopa->x += VELOCIDADE_KOOPA_CHUTADO;
         koopa->direcao = DIRECAO_DIREITA;
         if (koopa->estado == ESTADO_PARADO)
            koopa->estado = ESTADO_ANDANDO;
      break;

      /* koopa morto subindo */
      case FSM_KOOPA_SUBINDO:
         koopa->y -= pulo[koopa->contador++];
         koopa->x -= VELOCIDADE_EIXO_X;
         if (koopa->contador >= DURACAO_PULO)
         {
            koopa->fsm = FSM_KOOPA_CAINDO;
            koopa->contador--;
         }
      break;

      /* koopa morto caindo */
      case FSM_KOOPA_CAINDO:
         koopa->y += pulo[koopa->contador--];
         koopa->x -= VELOCIDADE_EIXO_X;
         if (koopa->contador < 0) koopa->contador = 0;
         if (fora_da_tela(koopa)) koopa->estado = ESTADO_MORTO;
      break;
   }
}

void move_bloco(SPRITE *bloco)
{
   bloco->x += (bloco->fsm < 2) ? VELOCIDADE_EIXO_X : -VELOCIDADE_EIXO_X;

   switch(bloco->fsm)
   {
      case 0:
      case 2:
         bloco->y -= pulo[bloco->contador++];
         if (bloco->contador >= DURACAO_PULO)
         {
            bloco->fsm++;
            bloco->contador--;
         }
      break;

      case 1:
      case 3:
         bloco->y += pulo[bloco->contador--];
         if (bloco->contador < 0) bloco->contador = 0;
         if (fora_da_tela(bloco)) bloco->estado = ESTADO_MORTO;
      break;
   }
}

void move_bola_de_fogo(SPRITE *bola)
{
   switch(bola->fsm)
   {
      case FSM_BOLA_DE_FOGO_ESPERA:
         bola->contador++;
         bola->estado = ESTADO_INVISIVEL;
         if (bola->contador >= MAX_ESPERA_FOGO_PEIXE)
         {
            bola->contador = 0;
            bola->fsm = FSM_BOLA_DE_FOGO_SUBINDO;
            bola->estado = ESTADO_ESPIRITO;
         }
      break;

      case FSM_BOLA_DE_FOGO_SUBINDO:
         bola->y -= super_pulo[bola->contador++];
         if (bola->contador >= DURACAO_SUPER_PULO)
         {
            bola->fsm = FSM_BOLA_DE_FOGO_CAINDO;
            bola->contador = DURACAO_SUPER_PULO - 1;
         }
      break;

      case FSM_BOLA_DE_FOGO_CAINDO:
         bola->y += super_pulo[bola->contador--];
         if (bola->contador < 0)
         {
            bola->fsm = FSM_BOLA_DE_FOGO_ESPERA;
            bola->contador = 0;
            bola->estado = ESTADO_INVISIVEL;
         }
      break;
   }
}

void move_peixe(SPRITE *peixe)
{
   switch(peixe->fsm)
   {
      case FSM_PEIXE_ESPERA:
         peixe->contador++;
         peixe->estado = ESTADO_INVISIVEL;
         if (peixe->contador >= MAX_ESPERA_FOGO_PEIXE)
         {
            peixe->contador = 0;
            peixe->fsm = FSM_PEIXE_SUBINDO;
            peixe->estado = ESTADO_ESPIRITO;
         }
      break;

      case FSM_PEIXE_SUBINDO:
         peixe->y -= super_pulo[peixe->contador++];
         peixe->direcao = DIRECAO_DIREITA;
         if (peixe->contador >= DURACAO_SUPER_PULO)
         {
            peixe->fsm = FSM_PEIXE_CAINDO;
            peixe->contador = DURACAO_SUPER_PULO - 1;
         }
      break;

      case FSM_PEIXE_CAINDO:
         peixe->y += super_pulo[peixe->contador--];
         peixe->direcao = DIRECAO_ESQUERDA;
         if (peixe->contador < 0)
         {
            peixe->fsm = FSM_PEIXE_ESPERA;
            peixe->contador = 0;
            peixe->estado = ESTADO_INVISIVEL;
         }
      break;

      /* peixe morto subindo */
      case FSM_PEIXE_MORTO_SUBINDO:
         peixe->y -= pulo[peixe->contador++];
         peixe->x -= VELOCIDADE_EIXO_X;
         if (peixe->contador >= DURACAO_PULO)
         {
            peixe->fsm = FSM_PEIXE_MORTO_CAINDO;
            peixe->contador--;
         }
      break;

      /* peixe morto caindo */
      case FSM_PEIXE_MORTO_CAINDO:
         peixe->y += pulo[peixe->contador--];
         peixe->x -= VELOCIDADE_EIXO_X;
         if (peixe->contador < 0) peixe->contador = 0;
         if (fora_da_tela(peixe)) peixe->estado = ESTADO_MORTO;
      break;
   }
}

void move_sprites(void)
{
   register int i;

   for(i = 1; i < fase_sprites; i++)
      if (sprite[i].estado != ESTADO_MORTO)
      {
         sprite[i].x_ant = sprite[i].x;
         sprite[i].y_ant = sprite[i].y;

         if (sprite[i].estado == ESTADO_CAINDO)
         {
            sprite[i].y += pulo[sprite[i].contador--];
            if (sprite[i].contador < 0) sprite[i].contador = 0;
         }

         if (SPRITE_BLOCO_PARTIDO(sprite[i].familia))
            move_bloco(sprite + i);
         else if (SPRITE_GOOMBA(sprite[i].familia))
            move_goomba(sprite + i);
         else if (SPRITE_SPINY(sprite[i].familia))
            move_spiny(sprite + i);
         else if (SPRITE_KOOPA(sprite[i].familia))
            move_koopa(sprite + i);
         else if (SPRITE_BOLA_DE_FOGO(sprite[i].familia))
            move_bola_de_fogo(sprite + i);
         else if (SPRITE_PEIXE(sprite[i].familia))
            move_peixe(sprite + i);
         else if (SPRITE_COGUMELO(sprite[i].familia))
            move_cogumelo(sprite + i);
         else if (SPRITE_MOEDA(sprite[i].familia))
            move_moeda(sprite + i);

         if (fora_da_fase(sprite + i)) sprite[i].estado = ESTADO_MORTO;

         if (sprite[i].estado != ESTADO_PARABOLA && 
             sprite[i].estado != ESTADO_ESPIRITO &&
             sprite[i].estado != ESTADO_INVISIVEL)
         {
            if (sprite[i].contador > 0) sprite[i].contador--;
            if (sprite[i].contador == 0 && sprite[i].estado == ESTADO_MORRENDO)
               sprite[i].estado = ESTADO_MORTO;
         }
      }
}

int fora_da_tela(SPRITE *_sprite)
{
   int x,y,w,h;

   x = X_RELATIVO(_sprite->x);
   y = Y_RELATIVO(_sprite->y);
   w = (_sprite->quadro_atual >= 0) ? imagem[_sprite->quadro_atual]->w : blocos_partidos[0]->w;
   h = (_sprite->quadro_atual >= 0) ? imagem[_sprite->quadro_atual]->h : blocos_partidos[0]->h;

   #ifdef VERSAO_640_480
      if (x*2 >= tela->w || (x+w)*2 < 0) return(TRUE);
      if (y*2 >= tela->h || (y+h)*2 < 0) return(TRUE);
   #else
      if (x >= tela->w || (x+w) < 0) return(TRUE);
      if (y >= tela->h || (y+h) < 0) return(TRUE);
   #endif

   return(FALSE);
}

int fora_da_fase(SPRITE *_sprite)
{
   int x,y,w,h;

   x = _sprite->x;
   y = _sprite->y;
   w = (_sprite->quadro_atual >= 0) ? imagem[_sprite->quadro_atual]->w : blocos_partidos[0]->w;
   h = (_sprite->quadro_atual >= 0) ? imagem[_sprite->quadro_atual]->h : blocos_partidos[0]->h;

   #ifdef VERSAO_640_480
      if (x >= fase_mapa_x * TILE_SIZE || (x+w)*2 < 0) return(TRUE);
      if (y >= fase_mapa_y * TILE_SIZE || (y+h)*2 < 0) return(TRUE);
   #else
      if (x >= fase_mapa_x * TILE_SIZE || (x+w) < 0) return(TRUE);
      if (y >= fase_mapa_y * TILE_SIZE || (y+h) < 0) return(TRUE);
   #endif

   return(FALSE);
}

