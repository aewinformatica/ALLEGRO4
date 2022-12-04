#include "Menu.h"
/*
=======
Menu

Classe que gerencia os menus
=======
*/

Menu::Menu() {

}

    // verifica a logica para todos os itens do menu
void Menu::atualiza() {
        int totalItems = items.size();
        for (int i = 0; i < totalItems; i++) {
            items[i].atualiza();
        }
}
    
    // desenha os itens de menu
void Menu::desenha(BITMAP *bmp) {
        int totalItems = items.size();
        for (int i = 0; i < totalItems; i++) {
            items[i].desenha(bmp);
        }
}

    // adiciona um item ao menu
void Menu::adicionaItem(MenuItem menuItem) {
        items.push_back(menuItem);
}

Menu::~Menu() {
        int totalItems = items.size();
        for (int i = 0; i < totalItems; i++) {
            // desaloca a memoria alocada para este item
            destroy_bitmap(items[i].pic);
            destroy_bitmap(items[i].picMouseDentro);
        }
}
