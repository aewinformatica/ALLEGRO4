#ifndef MARIO_H
#define MARIO_H
#pragma once
/* versao compilada com fullscreen a 640x480 (quando nao consegue com 320x240) */
//#define VERSAO_640_480

/* resolucao da tela */
#ifdef VERSAO_640_480
#define RES_X 640
#define RES_Y 480
#else
#define RES_X 320
#define RES_Y 240
#endif

/* frame rate (em frames por segundo) e velocidade de animacao (para tiles) */
#define FRAME_RATE      30
#define DELAY_ANIMACAO  4

/* bitmaps para buffer duplo, linha de status etc. */
BITMAP *tela,*status,*gameover,*hiper,*fim,*fundo;

/* para manter o frame rate constante */
volatile int counter;

/* variavel contadora de lacos do jogo */
int lacos;

/* opcoes de linha de comando */
char modo_16_bits, arq_fase[2][256], vol_musica, vol_som;

/* totais de jogo do mario */
int mario_vidas,mario_moedas,fase_atual;
char passou_de_fase;

/* wavs para efeitos sonoros de jogo */
SAMPLE *wav_moeda,*wav_pulo,*wav_quebra,*wav_bump,*wav_cresce,*wav_diminui;
SAMPLE *wav_morreu,*wav_cogumelo,*wav_cano,*wav_parabola,*wav_1up,*wav_fim;


#endif
