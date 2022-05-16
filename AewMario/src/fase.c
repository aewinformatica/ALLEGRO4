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

void fim_de_jogo(void)
{
   stop_midi();
   play_sample(wav_fim,255,128,1000,FALSE);
   counter = 0;
   while(counter < 150);
   counter = 0;

   clear_to_color(screen,0);
   #ifdef VERSAO_640_480
      stretch_blit(fim,screen,0,0,fim->w,fim->h,0,176,2*fim->w,2*fim->h);
   #else
      blit(fim,screen,0,0,0,88,fim->w,fim->h);
   #endif
   while(!key[KEY_ENTER]);
   le_fase(arq_fase[1]);
}

void inicia_fase(void)
{
   counter = 0;
   while(counter < 45);
   counter = 0;

   clear_to_color(screen,0);

   #ifdef VERSAO_640_480
      if (mario_vidas > -1)
         stretch_blit(hiper,screen,0,0,hiper->w,hiper->h,0,176,2*hiper->w,2*hiper->h);
      else stretch_blit(gameover,screen,0,0,hiper->w,hiper->h,0,176,gameover->w*2,gameover->h*2);
   #else
   /*
      if (mario_vidas > -1)
         blit(hiper,screen,0,0,0,88,hiper->w,hiper->h);
      else blit(gameover,screen,0,0,0,88,gameover->w,gameover->h);*/
            if (mario_vidas > -1)
         blit(hiper,screen,0,0,33,0,hiper->w,hiper->h);
      else blit(gameover,screen,0,0,33,0,gameover->w,gameover->h);
   #endif

   while(!key[KEY_ENTER]);
}

void le_fase(char *arquivo_fase)
{
   register int i,j,k,temp;
   char arquivo[256];
   PALETTE lixo;
   FILE *fp = fopen(arquivo_fase,"r");

   /* se nao conseguir ler o arquivo, termina o jogo com uma mensagem de erro */
   if (!fp)
   {
      allegro_exit();
      printf("Erro ao tentar abrir o arquivo '%s'\n",arquivo_fase);
      exit(0);
   }

   inicia_fase();
   if (mario_vidas < 0) return;

   memset(sprite,0,sizeof(sprite));
   memset(mapa,0,sizeof(mapa));
   memset(cabecada,0,sizeof(cabecada));
   memset(mundo_x,0,sizeof(mundo_x));
   memset(mundo_y,0,sizeof(mundo_y));
   memset(velocidade_x,0,sizeof(velocidade_x));
   memset(velocidade_y,0,sizeof(velocidade_y));
   memset(warp_zone_x,0,sizeof(warp_zone_x));
   memset(warp_zone_y,0,sizeof(warp_zone_y));

   /* inicia as variaveis do mario (menos x e y que serao lidas do arquivo) */
   mario->id = 0;
   mario->familia = 0;
   mario->quadro_atual = MARIO_PEQUENO_PARADO;
   mario->estado = ESTADO_PARADO;
   mario->direcao = DIRECAO_DIREITA;
   mario->delay = 2;
   mario->contador = 0;
   mario->fsm = MARIO_PEQUENO_INVENCIVEL;

   /* trava de pulo (para evitar pulos seguidos) */
   t.trava_pulo = FALSE;

   /* inicia variaveis de fase */
   topo_blocos_partidos = 0;
   scroll_x = scroll_y = 0;
   mult_pulo = 1;
   plataforma_subida = FALSE;
   passou_de_fase = FALSE;
   fase_sprites = 1;

   /* le dados do arquivo */

   /* nome da fase */
   fgets(nome_fase,256,fp);
   /* tira o \n final */
   nome_fase[strlen(nome_fase)-1] = 0;
   textout_centre_ex(status,font,nome_fase,208,5,-1,-1);
   printf("NOME DO MAPA: %s \n",nome_fase);
   printf("VIDAS: %d \n",mario_vidas);
   /* musica midi */
   fgets(arquivo,256,fp);
   /* tira o \n final */
   arquivo[strlen(arquivo)-1] = 0;
   musica = load_midi(arquivo);
   if (!musica)
   {
      allegro_exit();
      printf("Erro ao tentar carregar o arquivo '%s'\n",arquivo);
      exit(0);
   }

   /* bitmap de fundo */
   fgets(arquivo,256,fp);
   /* tira o \n final */
   arquivo[strlen(arquivo)-1] = 0;
   fundo = load_bitmap(arquivo,lixo);
   if (!fundo)
   {
      allegro_exit();
      printf("Erro ao tentar carregar o arquivo '%s'\n",arquivo);
      exit(0);
   }

   fscanf(fp,"%d",&fase_camada);
   fscanf(fp,"%d %d",&fase_mapa_x,&fase_mapa_y);

   for(i = 0; i < fase_camada; i++)
      fscanf(fp,"%d",tipo_camada + i);

   for(i = 0; i < fase_camada; i++)
      fscanf(fp,"%d %d",velocidade_x + i,velocidade_y + i);
   for(i = 0; i < fase_camada; i++)
      if (tipo_camada[i] != CAMADA_CENARIO)
         velocidade_x[i] = velocidade_y[i] = 2;

   fscanf(fp,"%d %d",&mario->x,&mario->y);

   for(i = 0; i < fase_camada; i++)
      for(j = 0; j < fase_mapa_y; j++)
         for(k = 0; k < fase_mapa_x; k++)
         {
            fscanf(fp,"%hd",&mapa[i][j][k]);

            /* verifica se eh uma warp zone */
            /* bits: 0123456789ABCDEF */
            /* 0123 = origem, 4567 = destino, 89ABCDEF = identificador de cano */
            temp = mapa[i][j][k] & 0x00FF;
            if (TILE_CANO_TOPO(temp))
            {
               temp = mapa[i][j][k] >> 12;
               if (temp > 0 && temp < 16)
               {
                  warp_zone_x[temp] = k;
                  warp_zone_y[temp] = j;
               }
            }
         }

   for(k = 0; k < fase_camada; k++)
      if (tipo_camada[k] == CAMADA_SPRITES)
         for(i = 0; i < fase_mapa_x; i++)
            for(j = 0; j < fase_mapa_y; j++)
               if (mapa[k][j][i])
                  cria_sprite(i * TILE_SIZE,j * TILE_SIZE,mapa[k][j][i],4);

   fclose(fp);
   play_midi(musica,TRUE);
}

void desenha(void)
{
   register int i,j,k,x,y,z,zz,p,soma_x,soma_y;

   /* primeiro desenha o fundo */
   #ifdef VERSAO_640_480
      for(j = 0; j < RES_Y; j += fundo->h)
         for(i = 0; i < RES_X; i += fundo->w)
            stretch_blit(fundo,tela,0,0,fundo->w,fundo->h,2*i,2*j,2*fundo->w,2*fundo->h);
   #else
      for(j = 0; j < RES_Y; j += fundo->h)
         for(i = 0; i < RES_X; i += fundo->w)
            blit(fundo,tela,0,0,i,j,fundo->w,fundo->h);
   #endif


   /* depois o resto */
   for(k = 0; k < fase_camada; k++)
      if (tipo_camada[k] == CAMADA_SPRITES)
         desenha_sprites();
      else
      {
         /* parallax */
         soma_x = (velocidade_x[k] * (scroll_x / MARIO_VELOCIDADE)) % TILE_SIZE;
         soma_y = ((velocidade_y[k] / MARIO_VELOCIDADE) * scroll_y) % TILE_SIZE;

         for(i = mundo_x[k] - 1; i < mundo_x[k] + RES_TILE_X + 1; i++) 
            if (i >= 0 && i < fase_mapa_x)
               for(j = mundo_y[k] - 1; j < mundo_y[k] + RES_TILE_Y + 1; j++)
                  if (j >= 0 && j < fase_mapa_y)
                  {
                     if (j-mundo_y[k] >= 0 && i-mundo_x[k] >= 0)
                        p = cabecada[k][j-mundo_y[k]][i-mundo_x[k]];
                     else p = 0;

                     if (mapa[k][j][i])
                     {
                        x = (i - mundo_x[k]) * TILE_SIZE;

                        z = mapa[k][j][i] & 0x00FF;
                        zz = mapa[k][j][i] & 0xFF00;

                        if (p > 0)
                        {
                           if (TILE_NOTA(z))
                              y = (j - mundo_y[k]) * TILE_SIZE + p;
                           else y = (j - mundo_y[k]) * TILE_SIZE - p;
                        }
                        else
                        {
                           if (TILE_NOTA(z))
                              y = (j - mundo_y[k]) * TILE_SIZE - p;
                           else y = (j - mundo_y[k]) * TILE_SIZE + p;
                        }

                        #ifdef VERSAO_640_480
                           masked_stretch_blit(imagem[z],tela,0,0,imagem[z]->w,imagem[z]->h,
                              (x-soma_x)*2,(y-soma_y)*2,imagem[z]->w*2,imagem[z]->h*2);
                        #else
                           draw_sprite(tela,imagem[z],x-soma_x,y-soma_y);
                        #endif

                        /* testa animacao da cabecada */
                        if (p)
                        {
                           p--;
                           if (p <= -MAX_CABECADA) p = MAX_CABECADA;
                           cabecada[k][j-mundo_y[k]][i-mundo_x[k]] = p;
                        }

                        /* testa delay para animar tile */
                        if ((lacos % DELAY_ANIMACAO) == 0 && mario->estado < ESTADO_MORTO)
                           mapa[k][j][i] = zz + proximo_quadro[z];
                     }
                     else if (p) cabecada[k][j-mundo_y[k]][i-mundo_x[k]] = 0;
                  }
      }

   desenha_sprites_frente();
}
