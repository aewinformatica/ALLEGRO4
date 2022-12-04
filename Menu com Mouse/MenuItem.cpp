//#pragma once
#include "MenuItem.h"

// esta funcao verifica se o cursor do mouse esta em cima (dentro) deste item de menu
    // se estiver, retorna true, caso contrario retorna false
bool MenuItem::mouseDentro() {
        int x1 = x;
        int y1 = y;
        int x2 = x1 + pic->w;
        int y2 = y1 + pic->h;

        if (mouse_x > x1 && mouse_x < x2 && mouse_y > y1 && mouse_y < y2)
            return true;
        else
            return false;
}


MenuItem::MenuItem(int posX, int posY, string imagem, string imagemDentro, void (*pFuncao)()) {
        isMouseDentro = false;
        
        // define a posicao
        x = posX;
        y = posY;

        // carrega as figuras do menu
        pic = load_bitmap(imagem.c_str(), NULL);
        picMouseDentro = load_bitmap(imagemDentro.c_str(), NULL);
    
        // se especificou uma funcao, passa ela
        if (pFuncao)
            funcao = pFuncao;
}
    
    // acao que este item de menu ira realizar
void MenuItem::realizaAcao() {
        if (funcao != NULL)
            (funcao)(); // chama a funcao
}
// atualiza este item de menu
void MenuItem::atualiza() {
        // se o mouse passar em cima, executa a acao deste menu
        if (mouseDentro()) {
            isMouseDentro = true;
            
            // se clicar realiza a acao
            if (mouse_b & 1) realizaAcao();

        } else {
            isMouseDentro = false;
        }
}
    
    // desenha este item de menu na tela
void MenuItem::desenha(BITMAP *bmp) {
        if (isMouseDentro)
            draw_sprite(bmp, picMouseDentro, x, y);
        else
            draw_sprite(bmp, pic, x, y);
}

MenuItem::~MenuItem() {
}
