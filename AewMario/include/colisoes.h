#ifndef COLISOES_H
#define COLISOES_H
#pragma once

#define colide(x1,y1,w1,h1,x2,y2,w2,h2) (\
x1+w1-1 >= x2 &&\
x1 <= x2+w2-1 &&\
y1+h1-1 >= y2 &&\
y1 <= y2+h2-1)

#define CALCULA_LIMITES {\
   i1 = sprite[l].x / TILE_SIZE;\
   j1 = sprite[l].y / TILE_SIZE;\
   i2 = (sprite[l].x + imagem[sprite[l].quadro_atual]->w - 1) / TILE_SIZE;\
   j2 = (sprite[l].y + imagem[sprite[l].quadro_atual]->h - 1) / TILE_SIZE;\
}

/* plataforma especial (so colide na subida) */
char plataforma_subida;

void calcula_direcao(int i,int j,int i1,int j1,int i2,int j2,SPRITE *_sprite);
int trata_colisao_tile(int i, int j, int dx, int dy, int conta);
void trata_colisao_cabecada(int i, int j);
void trata_colisao_sprite(int i, int j);
void detecta_colisoes(void);

#endif
