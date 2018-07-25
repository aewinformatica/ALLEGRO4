#ifndef FASE_H
#define FASE_H
#pragma once
/* dimensoes do mapa */
#define MAX_CAMADA               6
#define MAX_MAPA_X               1000
#define MAX_MAPA_Y               100

/* dimensoes da tela em tiles */
#define RES_TILE_X               20
#define RES_TILE_Y               14

/* tipos de camada */
#define CAMADA_CENARIO           0
#define CAMADA_TILES             1
#define CAMADA_SPRITES           2

/* numero maximo de "warp zones" */
#define MAX_WARP_ZONE            16

/* string que identifica a fase */
char nome_fase[256];

/* array do mapa da fase */
short mapa[MAX_CAMADA][MAX_MAPA_Y][MAX_MAPA_X];

/* para fazer os blocos "pularem" com a cabecada do mario */
char cabecada[MAX_CAMADA][RES_TILE_Y][RES_TILE_X];

/* duracao da cabecada */
#define MAX_CABECADA 3

/* variaveis de limite da fase atual */
int fase_camada,fase_mapa_x,fase_mapa_y,fase_sprites;

/* indica qual tipo da camada */
int tipo_camada[MAX_CAMADA];

/* coordenadas do mundo para o scroll por tiles */
int mundo_x[MAX_CAMADA],mundo_y[MAX_CAMADA],velocidade_x[MAX_CAMADA],velocidade_y[MAX_CAMADA];

/* localizacao das warp zones (canos) */
int warp_zone_x[MAX_WARP_ZONE],warp_zone_y[MAX_WARP_ZONE];

/* musica do mundo */
MIDI *musica;

void fim_de_jogo(void);
void inicia_fase(void);
void le_fase(char *arquivo_fase);
void desenha(void);

#endif
