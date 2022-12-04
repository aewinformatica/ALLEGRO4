#pragma once

#include "MenuItem.h"

/*
=======
Menu

Classe que gerencia os menus
=======
*/
class Menu {
private:
    vector<MenuItem> items; // representa os itens do menu

public:
    Menu();

    // verifica a logica para todos os itens do menu
    void atualiza();

    // desenha os itens de menu
    void desenha(BITMAP *bmp);

    // adiciona um item ao menu
    void adicionaItem(MenuItem menuItem);

    ~Menu();
};
