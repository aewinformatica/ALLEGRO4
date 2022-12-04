/*
======================================
Exemplo da implementacao de um Game Loop em Allegro

por Vitor Almeida da Silva

10/2007

Este codigo pode ser utilizado somente para fins educacionais.
======================================
*/

// inclui o header da biblioteca Allegro
#include <allegro.h>
#include <string>  
#include <vector>
#include "Menu.h"  
using namespace std;  

// prototipos das funcoes utilizadas
void Inicializa();
void Finaliza();
bool fimJogo = false; // flag que indica se o jogo foi ou nao finalizado
int ticks = 0;  // esta variavel ira ser um contador de ticks do clock principal
                // do jogo
                // cada tick a mais significa que um ciclo de logica do jogo
                // foi processado

// a funcao do timer do tick apenas incrementa o contador
void ticker() {
    ticks++;
}END_OF_FUNCTION(ticker);
int segundos = 0;   // esta variavel ira ser um contador de segundos
                    // a cada segundo passado este contador ira ser incrementado
                    // basicamente eh a chave para fazer um contador de fps
                    // pois se tivermos a quantidade de frames desenhados
                    // e a quantidade de segundos podemos calcular os frames
                    // por segundo

// a funcao do timer do clock apenas incrementa o contador
void clocka() {
    segundos++;
} END_OF_FUNCTION(clocka);// a partir daqui definimos algumas outras variaveis auxiliares

// dimensoes do video
int vid_largura = 640;      // quantidade de pixels de largura do video
int vid_altura = 480;       // quantidade de pixels de altura do video
int vid_profundidade = 32;  // quantidade de bits de profundidade (numero de bits que cada pixel tera)
int frames = 0; // contador da quantidade de frames desenhados na tela
int fps = 0;    // contador de fps (frames por segundo)
BITMAP *bmp;    // bitmap que ira guardar tudo o que desenharmos (para depois ser "jogado" na tela")
                // este bitmap eh de suma importancia pois cada funcao de desenho
                // que desenha em um bitmap provavelmente devera ser apontada
                // para este bitmap






// ===================
// definicao das funcoes dos menus
// ===================

void mnuNewGame() {
}
void mnuOptions() {
}
void mnuHighScores() {
}
void mnuExit() {
    fimJogo = true;
}

// ===================
// fim das definicoes das funcoes dos menus
// ===================


/*
=======
main

Funcao principal
Aqui temos o ponto de entrada da aplicacao onde o Game Loop sera executado
=======
*/
int main(int argc, char *argv[]) {
    // realiza todas as inicializacoes necessarias
    Inicializa();    // a partir deste ponto temos a aplicacao pronta para iniciar o game loop

    // inicializa o gerenciador do menu
    Menu *menu = new Menu();

    // agora adiciona os itens do menu
    // new game
    MenuItem itemNewGame(187, 51,  "gfx/menu/mnuNewGame.bmp", "gfx/menu/mnuNewGameM.bmp", mnuNewGame);
    menu->adicionaItem(itemNewGame);

    // options
    MenuItem itemOptions(187, 122, "gfx/menu/mnuOptions.bmp", "gfx/menu/mnuOptionsM.bmp", mnuOptions);
    menu->adicionaItem(itemOptions);

    // highscores
    MenuItem itemHighScores(187, 199, "gfx/menu/mnuHighscores.bmp", "gfx/menu/mnuHighscoresM.bmp", mnuHighScores);
    menu->adicionaItem(itemHighScores);

    // exit
    MenuItem itemExit(187, 274, "gfx/menu/mnuExit.bmp", "gfx/menu/mnuExitM.bmp", mnuExit);
    menu->adicionaItem(itemExit);

    // inicio do game loop
    // neste caso o game loop continua executando enquanto nao pressionarmos
    // a tecla esc, mas voce pode verificar por uma outra variavel ou condicao
    while (!fimJogo) {        // inicio do processamento da logica do jogo
        while (ticks && !fimJogo) {            // aqui iniciamos o processamento da logica do jogo
            // neste ponto, voce devera inserir o codigo responsavel
            // processamento da logica do seu jogo, isto inclui: deteccao de colisao
            // inteligencia artificial, calculos diversos, etc
            // aqui esta o segredo:
            // a variavel abaixo "ticks" eh decrementada, mas lembre-se que ela
            // tambem eh incrementada pela funcao de timer, entao qual eh a logica?
            // Lembre-se que o timer do ticks eh executado em um intervalo fixo
            // entao o decremento tambem ocorrera em um intervalo fixo, isto garante
            // que a logica do jogo sera executada pelo menos N vezes por segundo
            // sendo que N eh o intervalo de tempo do timer dos ticks
            
            // se pressionou a tecla esc finaliza o jogo
            if (key[KEY_ESC]) fimJogo = true;
            
            menu->atualiza();
            
            ticks--;
        }        // fim do processamento da logica do jogo

        // limpa a tela com a cor branca
        clear_to_color(bmp, makecol(255, 255, 255));

        // quando chegarmos ateh aqui significa que a variavel ticks esta com valor
        // zerado, entao ja executamos toda a logica do jogo, agora eh hora
        // de exibir o resultado na tela
        // para exibir o resultado na tela, toda a saida deve ser desenhada
        // no bitmap bmp declarado anteriormente
        // para auxiliar, exibimos o fps, note que estamos desenhando no bmp
        // e utilizando a fonte padrao (font), e fundo transparente (-1)
        textprintf_ex(bmp, font, 0, 0, -1,-1, "FPS: %i", fps);

        // desenha o menu
        menu->desenha(bmp);

        // exibe o cursor do mouse na tela
        show_mouse(bmp);
        
        // depois que estiver como frame no bmp, copiamos o conteudo inteiro dele
        // para a tela
        // isto eh realizado pois como esta copia eh feita em memoria, eh muito
        // mais rapido
        blit(bmp, screen, 0, 0, 0, 0, vid_largura, vid_altura);

        // apos a saida concluida, incrementamos o contador de frames, pois
        // temos mais um frame desenhado na tela
        frames++;

        // agora iremos calcular o total de frames por segundo
        // a logica eh a seguinte:

        // se na aplicacao ja passou um segundo
        if (segundos) {
            // calcula o fps sendo que o fps sera a quantidade de frames desenhados
            // dividido pelo numero de segundos passados (no caso 1)
            fps = frames / segundos;
            frames = 0; // como ja calculamos o fps, pode zerar a quantidade de frames desenhados
            segundos = 0; // e zera tambem o numero de segundos, isso garante que o proximo
            // calculo de fps sera executado um segundo depois
        }
    }

    delete menu;

    // executa todas as finalizacoes necessarias
    Finaliza();

    return 0;
}END_OF_MAIN();

/*
=======
Inicializa

Realiza qualquer procedimento necessario para iniciar o jogo
Isto inclui iniciar a janela e deixa-la preparada para ser desenhada, iniciar
o estado do jogo, posicoes iniciais, nivel inicial e tudo o que for necessario
=======
*/
void Inicializa() {
    int res;
    allegro_init();
    set_color_depth(vid_profundidade);
    res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, vid_largura, vid_altura, 0, 0);

    if (res != 0) {
        allegro_message(allegro_error);
        exit(-1);
    }

    install_timer();
    install_keyboard();
    install_mouse();

    // aqui inicializamos o nosso timer de ticks
    // trancando o espaco de memoria da variavel ticks e da funcao timer
    // assim elas nao sofrem alteracoes externas
    // note que a funcao ticker sera chamada 60 vezes por segundo, isto eh,
    // a logica do nosso jogo sera executada 60 vezes em um segundo
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker,BPS_TO_TIMER(60));

    // aqui ocorre o mesmo processo para o timer dos segundos
    // note que o timer sera chamado apenas uma vez por segundo
    LOCK_VARIABLE(segundos);
    LOCK_FUNCTION(clocka);
    install_int_ex(clocka,BPS_TO_TIMER(1));

    // cria um bitmap com uma dimensao grande o suficiente para caber na tela
    bmp = create_bitmap(SCREEN_W,SCREEN_H);
    set_window_title("VSoftGames - Teste de Menus");
}

/*
=======
Finaliza

Realiza qualquer procedimento de limpeza para finalizar o jogo
=======
*/
void Finaliza() {
    clear_keybuf();
    //destroy_bitmap(bmp);

    // neste ponto voce devera adicionar qualquer tipo de finalizacao que for
    // necessario, limpeza, etc.
}
