#ifndef SCROLL_H
#define SCROLL_H
#pragma once
/* variaveis para controlar o scroll da tela */
int scroll_x,scroll_y;

/* retorna os valores relativos aa tela */
#define X_RELATIVO(x_absoluto) (x_absoluto - scroll_x)
#define Y_RELATIVO(y_absoluto) (y_absoluto - scroll_y)

void blit_scrolled(void);
void calcula_scroll(void);

#endif
