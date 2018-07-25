#ifndef ENTRADA_H
#define ENTRADA_H
#pragma once
typedef struct TECLA
{
	char cima,baixo,esquerda,direita,pula,trava_pulo,corre;
} TECLA;

TECLA t;
int cima,baixo,esquerda,direita,pula,corre;
int mario_warp_x,mario_warp_y;

void configura_teclas(void);
void le_entrada(void);
void move_mario(void);

#endif
