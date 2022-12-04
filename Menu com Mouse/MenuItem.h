#pragma once
#include <allegro.h>
#include <string>  
#include <vector>
using namespace std;
/*
=======
MenuItem

Representa o item de um menu
=======
*/
class MenuItem {
private:
    int x, y;                   // localizacao deste item de menu na tela
    // esta funcao verifica se o cursor do mouse esta em cima (dentro) deste item de menu
    // se estiver, retorna true, caso contrario retorna false
    bool mouseDentro();
public:
    BITMAP *pic;                // imagem do item do menu
    BITMAP *picMouseDentro;     // imagem do item do menu (quando o mouse estiver em cima)
    bool isMouseDentro;         // flag que indica se o mouse esta ou nao dentro do menu
    void (*funcao)();           // ponteiro para a funcao que sera executada quando o usuario clicar neste menu

    MenuItem(int posX, int posY, string imagem, string imagemDentro, void (*pFuncao)());
    // acao que este item de menu ira realizar
    void realizaAcao();
    // atualiza este item de menu
    void atualiza();
    // desenha este item de menu na tela
    void desenha(BITMAP *bmp);

    ~MenuItem();
};
