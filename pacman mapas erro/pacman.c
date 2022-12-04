/*
 * Projeto Pac-Man
 * Autores: Carlos Henrique Pinto Rodriguez
 *          Fabio Cunha Pinto Coelho
 */

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX_X		 640
#define MAX_Y		 480
#define COLOR_BITS	 16
#define PAC_TAM      32
#define DIREITA      0
#define BAIXO        1
#define ESQUERDA     2
#define CIMA         3
#define PARADO       -1
#define FPS          55
#define PASSO        2
#define VERTS        126
#define HORIZS       140
#define FANTASMAS    4
#define IMAGENS      88
#define PILULASX     17
#define PILULASY     13
#define EXISTE       1
#define COMIDA       0
#define MUSICAS      10
#define SONS         5
#define DESATIVADO   0
#define ATIVADO      1
#define FASES        9
#define MORRENDO     2
#define SIM          1
#define NAO          0
#define VIDAS        5

int  inicia    (void);
void principal (void);
void finaliza  (void);

void leRecorde (void);
int  colisao   (int x, int y);
int bonusAleatorio (void);
void carregando (void);
void desenharMapa (void);
void movimentandoPac (void);
void desenhandoSprites (int bola);
void mudandoFase(void);
void iniciarPosicoes (void);
void pacMorrendo (void);
void parada (int centesimos);
void salvaRecorde (void);
void menuInicial(void);
void menuInicial2(int mov);
void manual(void);
void heroi(void);
void mudaHeroi(void);
void inc_novo_frame (void);

typedef struct _SPRITE
{
	int	x;
	int	y;
	int xinic;
	int yinic;
	int movv;
	int movh;
	int	estado;
	int sentido;
	int colidiu;
	int morto;
	int parado;
	BITMAP *sprite[9][2];

} SPRITE;

int colisaoSprites (SPRITE *s1, SPRITE *s2);
void movFantSeguindo(SPRITE *fantasma);
void movFantAleatorio(SPRITE *fantasma);
void movFantMorto (SPRITE *fantasma);

typedef struct _ITEM
{
    int x,
        y;
    int estado;
    BITMAP *sprite[2];
    
}ITEM;

/* ------------------------ VARIÁVEIS GLOBAIS ------------------------ */

    int novo_frame = 0;        /* Controla o timer dos frames */
    int estadoTimer = 0;       /* Controla o timer do estado dos personagens */
    int pontos = 0;            /* Pontos Ganhos */
    int maxpontos;             /* Recorde de Pontos */
    int maxpontosAtual = 0;    /* Recorde de Pontos na atual execução */
    int vidas = VIDAS;         /* Quantidade de Vidas do Pac-Man */
    int fase = 0;              /* Fase Atual */
    int modoAzul = DESATIVADO; /* Ativa e Desativa o modo Azul */
    int azulContador = 0;      /* Controla o timer do modo Azul */
    int mudarFase = NAO;       /* Controla a mudança de fase */
    int fps = FPS;             /* Controla as páginas por segundo do jogo */
    int dificuldade = 0;       /* Controla o aumento da dificuldade do jogo */
    int fantMortos = 0;        /* Conta quantos fantasmas morreram no modoAzul*/
    int timerBonus = 0;        /* Controla o tempo que o bonus fica na tela */
    int terminarJogo = NAO;    /* Controla o término do jogo */
    int heroiatual = 0;        /* Aponto qual o atual skin do herói */
    int removerColisao = NAO;  /* Controla Passos Aleatórios ao seguir */
    int removerColisaoTimer=0; 


    
    char msg[30];              /* Contém as mensagens de textout */
    char jogador[10];          /* Nome do jogador recordista */
    
    BITMAP *pacmorrendo[11];   /* Bitmaps do Pac-Man morrendo */
    BITMAP *mapa;              /* Onde são impressas as paredes */
    BITMAP *bonecos;           /* Onde são impressos os personagens */
    BITMAP *imagens[IMAGENS];  /* Carrega várias imagens do sprite.bmp */
    BITMAP *background[FASES]; /* Fundos de Tela (Um para cada fase) */
    BITMAP *menu[9];           /* Bitmaps do Menu */
    BITMAP *faseEntrada[FASES];/* Mensagens iniciais de cada fase */
    BITMAP *faseCompleta;      /* Mensagem de que a fase foi ganha */
    BITMAP *gameover;          /* Mensagem de Fim do Jogo */
    BITMAP *pause;             /* Mensagem ao pausar o jogo */

    SAMPLE *som[SONS];         /* Alguns sons do jogo */

    MIDI *musica[MUSICAS];     /* Musicas do jogo */
    
    SPRITE pacman;         
    SPRITE fant[FANTASMAS];

    ITEM pilula[PILULASY][PILULASX];
    ITEM bonus[FASES];
    
    int horiz[HORIZS][3];  /* Retas Horizontais dos mapas */
    int vert[VERTS][3];    /* Retas Verticais dos mapas */
    
                          
/* --------------------- FIM DAS VARIÁVEIS GLOBAIS --------------------- */

int main (void)
{   
    leRecorde();

    if(!inicia())
    {
        finaliza();
      	return -1;
    } 
    else while(!terminarJogo)
    {
        principal();
    }    
    
    finaliza();

    return 0;
}
END_OF_MAIN();


/*
 * Lê o jogador que obteve o maior record a partir do arquivo pace.dat
 */
void leRecorde (void)
{
    FILE *pa;
    int i;

    if(!(pa = fopen("pace.dat", "r")))
    {
        /* Caso pace.dat não exista, ele é criado */
        pa = fopen("pace.dat", "w");
        fprintf(pa, "%s", "Pac-Man         0");
        fclose(pa);
        /* Após pace.dat ser criado, ele executa esta função novamente */
        leRecorde();

    }
    
    for(i=0 ; i<10 ; i++)
    {
        fscanf(pa, "%c", &jogador[i]);
    }
    for(i=0 ; i<10 ; i++)
    {
        if(jogador[9-i] == 32)
        {
            jogador[9-i] = '\0';
        }    
        else break;
    }    
    fscanf(pa, "%d", &maxpontos);
    fclose(pa);

}

/* 
 * Iniciando o Allegro
 */
int inicia (void)
{
	allegro_init();

	install_keyboard();

	set_color_depth(COLOR_BITS);
	if(set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, 0, 0) < 0)
	{
		printf("Erro ao inicializar o modo grafico.\n");
		return (FALSE);
	}
	
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
	{
	printf("Erro ao inicializar o som");
	return (FALSE);
	}
	
	return (TRUE);
}

/*
 * Inicia o processo do jogo
 */
void principal (void)
{
    int ganharVida = 1;
    int i,j;
    float distancia;
    
    /* Zerando condições dos fantasmas */
    for(i=0 ; i<FANTASMAS ; i++)
    {
        fant[i].morto = NAO;
        fant[i].colidiu = NAO;
        fant[i].parado = NAO;
    }    

    /* Zerando o aparecimento dos bonus */
    for(i=0 ; i<FASES ; i++)
    {
        bonus[i].estado = 0;
    }    
    
    srand(time(NULL));
    
    /* Funções que controlam o timer do allegro */
   	novo_frame = 0;
	LOCK_VARIABLE(novo_frame);
	LOCK_FUNCTION(inc_novo_frame);
	install_int(inc_novo_frame, 1000 / FPS);
	
    carregando();
      
    menuInicial();

    while(!key[KEY_ESC] && terminarJogo == NAO)
    {
        if(vidas == -1)
        {
            clear(screen);
            draw_sprite(screen, gameover, 103, 195);
            parada(200);
            break;
        }    
        /* A cada 10000 pontos, ganha-se 1 vida */
        if(pontos > 10000 * ganharVida)
        {
            vidas++;
            play_sample(som[2], 200, 127, 1000, 0);
            ganharVida++;
        }

        /* Se nenhuma pilula existir, a fase será mudada */        
        mudarFase = SIM;
        if(fase != 0)
        {
            for(i=0 ; i<PILULASY ; i++)
            {
                for(j=0 ; j<PILULASX ; j++)
                {
                    if(pilula[i][j].estado == 1 || pilula[i][j].estado == 2)
                    {
                        mudarFase = NAO;
                    }    
                }    
            }    
        }    
        
        if(mudarFase)
        {
            mudandoFase();

            desenhandoSprites(SIM);
        }    

        if (novo_frame)
        {

            movimentandoPac ();
            
            for(i=0 ; i<FANTASMAS ; i++)
            {
                /* Caso o Modo Azul acabe antes do fantasma chegar na posição
                inicial, esse if fará ele voltar ao seu movimento normal */
                if(fant[i].x == fant[i].xinic && fant[i].y == fant[i].yinic &&
                   modoAzul == DESATIVADO)
                {
                    fant[i].morto = NAO;
                    fant[i].parado = NAO;
                }    
            
                /* Movimentos dos Fantasmas */
                if(fant[i].parado == NAO)
                {
                    /* Calcula a distância entre o fantasma e o Pac-Man */
                    distancia=sqrt((pacman.x - fant[i].x)*
                                   (pacman.x - fant[i].x)+
                                   (pacman.y - fant[i].y)*
                                   (pacman.y - fant[i].y));
                                   
                    if(modoAzul == ATIVADO && fant[i].morto == NAO)
                    {
                        movFantAleatorio(&fant[i]);
                    }    
                    else if(fant[i].morto == SIM)
                    {
                        movFantMorto(&fant[i]);             
                    }    
                    else if(distancia <= 250.0)
                    {
                        movFantSeguindo(&fant[i]);
                    }    
                    else movFantAleatorio(&fant[i]);
                }    
            }    
           
            desenhandoSprites(NAO);
        }   
        
        /* Pausando o jogo */ 
        if(key[KEY_SPACE])
        {
            draw_sprite(screen, pause, 195, 170);
            play_sample(som[2], 200, 127, 1000, 0);
            parada(50);
            while(!key[KEY_ENTER]);
            play_sample(som[2], 200, 127, 1000, 0);
            parada(50);
        }    
    }
    
    stop_midi();

	remove_int(inc_novo_frame);
}

/*
 * Finaliza o Allegro e salva um novo record, caso exista
 */
void finaliza (void)
{
	allegro_exit();
    salvaRecorde();   
}

/*
 * Testa colisão entre o fantasma e a parede ou o pac-man e a parede
 */
int colisao (int x, int y) 
{
    int j;
    for(j=0 ; j<VERTS ; j++)
    {
        if( x>vert[j][0]-(PAC_TAM+1) &&
            y>vert[j][1]-(PAC_TAM+1)
            && x<vert[j][0]+1
            && y<vert[j][2]+1)   return 1;
    }
    for(j=0 ; j<HORIZS ; j++)
    {   
        if( x>horiz[j][0]-(PAC_TAM+1) &&
            y>horiz[j][1]-(PAC_TAM+1)
            && x<horiz[j][2]+1
            && y<horiz[j][1]+1)  return 1;
    }   
    return 0;
}

/*
 * Carregando imagens e sons
 */ 
void carregando (void)
{
	BITMAP	*imgs;
	int	i, j, k;
	char arquivo[20] = "sprites/fase0.bmp";

	som[0] = load_sample("musicas/iniciando.wav");
	som[1] = load_sample("musicas/comendo.wav");
	som[2] = load_sample("musicas/modoAzul1.wav");
	som[3] = load_sample("musicas/modoAzul2.wav");
	som[4] = load_sample("musicas/morrendo.wav");	

	/* musica[0] é diferente para cada herói */ 
    musica[1] = load_midi("musicas/sonic.midi");
    musica[2] = load_midi("musicas/ctrobo.midi");
    musica[3] = load_midi("musicas/gradius.midi");
    musica[4] = load_midi("musicas/ct1.midi");
    musica[5] = load_midi("musicas/castlevania2.midi");
    musica[6] = load_midi("musicas/dk3.midi");                        
    musica[7] = load_midi("musicas/ct3.midi");    
    musica[8] = load_midi("musicas/castlevania1.midi");
    musica[9] = load_midi("musicas/kirby.midi");    		
	
	/* Carregando várias imagens de um mesmo arquivo */
	imgs = load_bitmap("sprites/sprites.bmp", NULL);
	i = j = k = 0;
	while (i < IMAGENS)
	{
		imagens[i] = create_bitmap(PAC_TAM, PAC_TAM);
		blit(imgs, imagens[i], j * PAC_TAM, k * PAC_TAM, 0, 0, PAC_TAM, PAC_TAM);
		j++;
		if (j == 8)
		{
			j = 0;
			k++;
		}
		i++;
	}
	destroy_bitmap(imgs);

    mapa = create_bitmap(MAX_X ,MAX_Y); /* Conterá o mapa da fase */
    bonecos = create_bitmap(MAX_X ,MAX_Y); /* Conterá os personagens e itens */
    
    pacman.sprite[0][0] = imagens[4];
    pacman.sprite[0][1] = imagens[5];
    pacman.sprite[1][0] = imagens[2];
    pacman.sprite[1][1] = imagens[3];
    pacman.sprite[2][0] = imagens[6];
    pacman.sprite[2][1] = imagens[7];
    pacman.sprite[3][0] = imagens[0];
    pacman.sprite[3][1] = imagens[1];
    pacman.sprite[4][0] = imagens[8];
    
    fant[0].sprite[0][0] = imagens[20];
    fant[0].sprite[0][1] = imagens[21];
    fant[0].sprite[1][0] = imagens[18];
    fant[0].sprite[1][1] = imagens[19];
    fant[0].sprite[2][0] = imagens[22];
    fant[0].sprite[2][1] = imagens[23];
    fant[0].sprite[3][0] = imagens[16];
    fant[0].sprite[3][1] = imagens[17];
    fant[0].sprite[4][0] = imagens[54];
    fant[0].sprite[5][0] = imagens[53];
    fant[0].sprite[6][0] = imagens[55];
    fant[0].sprite[7][0] = imagens[52];
    fant[0].sprite[8][0] = imagens[48];
    fant[0].sprite[8][1] = imagens[49];
    
    fant[1].sprite[0][0] = imagens[28];
    fant[1].sprite[0][1] = imagens[29];
    fant[1].sprite[1][0] = imagens[26];
    fant[1].sprite[1][1] = imagens[27];
    fant[1].sprite[2][0] = imagens[30];
    fant[1].sprite[2][1] = imagens[31];
    fant[1].sprite[3][0] = imagens[24];
    fant[1].sprite[3][1] = imagens[25];
    fant[1].sprite[4][0] = imagens[54];
    fant[1].sprite[5][0] = imagens[53];
    fant[1].sprite[6][0] = imagens[55];
    fant[1].sprite[7][0] = imagens[52];
    fant[1].sprite[8][0] = imagens[48];
    fant[1].sprite[8][1] = imagens[49];    

    fant[2].sprite[0][0] = imagens[36];
    fant[2].sprite[0][1] = imagens[37];
    fant[2].sprite[1][0] = imagens[34];
    fant[2].sprite[1][1] = imagens[35];
    fant[2].sprite[2][0] = imagens[38];
    fant[2].sprite[2][1] = imagens[39];
    fant[2].sprite[3][0] = imagens[32];
    fant[2].sprite[3][1] = imagens[33];
    fant[2].sprite[4][0] = imagens[54];
    fant[2].sprite[5][0] = imagens[53];
    fant[2].sprite[6][0] = imagens[55];
    fant[2].sprite[7][0] = imagens[52];
    fant[2].sprite[8][0] = imagens[50];
    fant[2].sprite[8][1] = imagens[51];       
    
    fant[3].sprite[0][0] = imagens[44];
    fant[3].sprite[0][1] = imagens[45];
    fant[3].sprite[1][0] = imagens[42];
    fant[3].sprite[1][1] = imagens[43];
    fant[3].sprite[2][0] = imagens[46];
    fant[3].sprite[2][1] = imagens[47];
    fant[3].sprite[3][0] = imagens[40];
    fant[3].sprite[3][1] = imagens[41];
    fant[3].sprite[4][0] = imagens[54];
    fant[3].sprite[5][0] = imagens[53];
    fant[3].sprite[6][0] = imagens[55];
    fant[3].sprite[7][0] = imagens[52];
    fant[3].sprite[8][0] = imagens[50];
    fant[3].sprite[8][1] = imagens[51]; 
    
    bonus[0].sprite[0] = imagens[rand() % 7 + 9];
    bonus[1].sprite[0] = imagens[9];
    bonus[2].sprite[0] = imagens[10];
    bonus[3].sprite[0] = imagens[11];
    bonus[4].sprite[0] = imagens[12];
    bonus[5].sprite[0] = imagens[13];                    
    bonus[6].sprite[0] = imagens[rand() % 7 + 9];
    bonus[7].sprite[0] = imagens[14];
    bonus[8].sprite[0] = imagens[15];  
    
    menu[0] = load_bitmap("sprites/menu.bmp", NULL);
    menu[1] = load_bitmap("sprites/enteresc.bmp", NULL);
	menu[2] = load_bitmap("sprites/menu2.bmp", NULL); 
	menu[3] = load_bitmap("sprites/caminho.bmp", NULL);    
	menu[4] = load_bitmap("sprites/instrucoes.bmp", NULL);	
    menu[5] = load_bitmap("sprites/heroi.bmp", NULL);	
    menu[6] = load_bitmap("sprites/heroi0.bmp", NULL);
    menu[7] = load_bitmap("sprites/heroi1.bmp", NULL);
    menu[8] = load_bitmap("sprites/heroi2.bmp", NULL);      

    for(i=0 ; i<11 ; i++)
    {
        pacmorrendo[i] = imagens[56 + i];
    }    

    for(i=0 ; i<PILULASY ; i++ )
    {
        for(j=0 ; j<PILULASX ; j++)
        {
            pilula[i][j].sprite[0] = imagens[69];
            pilula[i][j].sprite[1] = imagens[68];
            /* Posiciona pílulas ao longo de todo o mapa, mesmo que elas
            não existam. O fato de ela existir ou não, será decidido em outra
            função */
            pilula[i][j].x = 31 + 34*j;
            pilula[i][j].y = 29 + 34*i;
        }
    }    
    
    /* Carrega o Bitmap-Introdução de cada fase */
    for(i=0 ; i<FASES ; i++)
    {
        arquivo[12] = 49 + i;
        faseEntrada[i] = load_bitmap(arquivo, NULL);
    }    
    
    /* Fundos de tela */
    background[0] = load_bitmap("backgrounds/singularity.bmp", NULL);
    background[1] = load_bitmap("backgrounds/skysong2.bmp", NULL);
    background[2] = load_bitmap("backgrounds/trinity.bmp", NULL);    
    background[3] = load_bitmap("backgrounds/thetismoon.bmp", NULL);
    background[4] = load_bitmap("backgrounds/arboreal.bmp", NULL);
    background[5] = load_bitmap("backgrounds/once.bmp", NULL);
    background[6] = load_bitmap("backgrounds/haiku.bmp", NULL); 
    background[7] = load_bitmap("backgrounds/cancer.bmp", NULL);     
    background[8] = load_bitmap("backgrounds/tendrils.bmp", NULL);
    
    pause = load_bitmap("sprites/pause.bmp", NULL);
    gameover = load_bitmap("sprites/gameover.bmp", NULL);                       
    faseCompleta = load_bitmap("sprites/fasecompleta.bmp", NULL);
}

/*
 * Função que desenha o mapa no bitmap "mapa"
 */
void desenharMapa (void)
{
    int i;

    for (i=0 ; i<HORIZS ; i++)
    {
        hline(mapa, horiz[i][0], horiz[i][1],
              horiz[i][2], makecol(0,255,255));
    }    
    for (i=0 ; i<VERTS ; i++)
    {
        vline(mapa, vert[i][0], vert[i][1],
              vert[i][2] , makecol(0,255,255));
    }
}   

void movimentandoPac (void)
{
    int i, j;

    /* Escolhendo o Sentido.
     * Esses 4 if's só escolhem para onde o Pac-Man tentará ir.
     * Seu sentido só será realmente mudado se os if's seguintes não 
     * apontarem colisão.
     */
    if(key[KEY_RIGHT])
    {
        pacman.movh = DIREITA;
    }    
    if(key[KEY_DOWN])
    {
        pacman.movv = BAIXO;
    }    
    if(key[KEY_LEFT])
    {
        pacman.movh = ESQUERDA;
    }    
    if(key[KEY_UP])
    {
        pacman.movv = CIMA;
    }  


    /* Cada if(pacman.mov == Direção) fará o Pac-Man se movimentar.
       Se colidir com a parede, ele ficará parado.
       Se não colidir, seu sentido passará a ser o que foi escolhido.  */
    if (pacman.movh == DIREITA)
    {
        pacman.x += PASSO;
        if(colisao(pacman.x,pacman.y))
        {
            pacman.x -= PASSO;            
            pacman.movh = PARADO;
        }    
        else pacman.sentido = DIREITA;
    }   
    if (pacman.movh == ESQUERDA)
    {
        pacman.x -= PASSO;
        if(colisao(pacman.x,pacman.y))
        {
            pacman.x += PASSO;            
            pacman.movh = PARADO;
        }
        else pacman.sentido = ESQUERDA;
    }   
    
    
    /* Comendo Pílulas */
    for(i=0 ; i<PILULASY ; i++)
    {
        for(j=0 ; j<PILULASX ; j++)
        {
            if(pacman.x == pilula[i][j].x && pacman.y == pilula[i][j].y &&
               pilula[i][j].estado != COMIDA && pilula[i][j].estado != 3)
            {
                if(pilula[i][j].estado == 2)
                {
                    /* Se entrar nesse if, é porque a pílula
                       comida era uma vitamina */
                    pontos += 40;
                    modoAzul = ATIVADO;
                    azulContador = 0;
                    play_sample(som[2], 200, 127, 1000, 0);
                }    
                pilula[i][j].estado = COMIDA;
                pontos += 10;
                play_sample(som[1], 200, 127, 1000, 0);
            }    
        }    
    }   
    if (pacman.movv == BAIXO)
    {
        pacman.y += PASSO;
        if(colisao(pacman.x,pacman.y))
        {
            pacman.y -= PASSO;            
            pacman.movv = PARADO;
        }    
        else pacman.sentido = BAIXO;
    }            
 
    if (pacman.movv == CIMA)
    {
        pacman.y -= PASSO;
        if(colisao(pacman.x,pacman.y))
        {
            pacman.y += PASSO;
            pacman.movv = PARADO;
        }    
        else pacman.sentido = CIMA;
     }
     
     /* Comendo Pílulas sendo executada novamente porque o Pac-Man, ao fazer
      * uma curva, anda 2 pixels de uma vez (1 na horizonal e 1 na vertical).
      * Portanto, o teste tem que ser feito 1 vez para cada direção. */
    for(i=0 ; i<PILULASY ; i++)
    {
        for(j=0 ; j<PILULASX ; j++)
        {
            if(pacman.x == pilula[i][j].x && pacman.y == pilula[i][j].y &&
               pilula[i][j].estado != COMIDA && pilula[i][j].estado != 3)
            {
                if(pilula[i][j].estado == 2)
                {
                    /* Se entrar nesse if, é porque a pílula
                       comida era uma vitamina */
                    pontos += 40;
                    /* Como a pílula era uma vitamina, o jogo
                       entra no modo Azul */
                    modoAzul = ATIVADO;    
                    azulContador = 0;                
                    play_sample(som[2], 200, 127, 1000, 0);                    
                }    
                pilula[i][j].estado = COMIDA;
                pontos += 10;
                play_sample(som[1], 200, 127, 1000, 0);
            }    
        }    
    }   
     for(i=0 ; i<FANTASMAS ; i++)
     {
         if(modoAzul)
         {
             if(colisaoSprites(&pacman, &fant[i]) && fant[i].morto == NAO)
             {
                 if(fantMortos == 0) pontos += 200;
                 if(fantMortos == 1) pontos += 400;
                 if(fantMortos == 2) pontos += 800;
                 if(fantMortos == 3) pontos += 1600;
                 fantMortos++;
                 fant[i].morto = SIM;
             }    
         }
         
         else if (colisaoSprites(&pacman, &fant[i]) && modoAzul == DESATIVADO)
         {
             vidas--;
             pacMorrendo();
         }    
     }    
     
     /* Impede o Pac-Man de sair do mapa, fazendo-o voltar no lado oposto
        ao qual saiu. */
     if(pacman.x > MAX_X) pacman.x = -2*PAC_TAM+1;
     if(pacman.x < -2*PAC_TAM) pacman.x = MAX_X-1;      
     if(pacman.y > MAX_Y) pacman.y = -2*PAC_TAM+1 ;      
     if(pacman.y < -2*PAC_TAM) pacman.y = MAX_Y-1;   
     
     /* Controla o tempo que o Modo Azul se mantém ativado */
     if(modoAzul == ATIVADO) azulContador++;

     if(azulContador % 40 == 0 && modoAzul == ATIVADO)
     {
         play_sample(som[3], 200, 127, 1000, 0);
     }    

     /* Saindo do modo Azul */
     if(azulContador == 350)
     {
         modoAzul = DESATIVADO;
         azulContador = 0;
         fantMortos = 0;
         
         /* Resetando os Fantasmas */
         for(i=0 ; i<FANTASMAS ; i++)
         {
             if(fant[i].x == fant[i].xinic && fant[i].y == fant[i].yinic)
             {
                 fant[i].morto = NAO;
                 fant[i].parado = NAO;
             }    
         }    
     }  

    /* Comendo os itens bonus */
    if(pacman.x == bonus[fase - 1].x && pacman.y == bonus[fase - 1].y &&
       bonus[fase - 1].estado == 1)
    {
        pontos += (fase + (dificuldade - 1) * FASES) * 100;
        bonus[fase - 1].estado = 3;
        play_sample(som[2], 200, 127, 1000, 0);
    } 
    
    /* Controlando o tempo que o bonus leva para sumir e não aparecer mais */   
    if(bonus[fase - 1].estado == 1) timerBonus++;
    if(timerBonus == 7 * fps) bonus[fase - 1].estado = 3;
}    

/*
 * Função que movimenta os fantasmas aleatoriamente pelo mapa.
 */  
void movFantAleatorio(SPRITE *fantasma)
{
    int i;
    
    
    /*
     * Ao colidir, o fantasma sempre mudará de direção e,
     * conseqüentemente de sentido.  
     */
    if(fantasma->sentido == DIREITA)
    {
        fantasma->x += PASSO;
        
        if (colisao(fantasma->x, fantasma->y))
        {
            fantasma->x -= PASSO;
            while(fantasma->sentido == DIREITA || fantasma->sentido == ESQUERDA)
            {
                fantasma->sentido = rand() % 4;
            }  
        }

        i = rand() % 3;
        
        if(i == 0 && !colisao(fantasma->x, fantasma->y+1))
        {
            fantasma->sentido = BAIXO;
        }    

        if(i == 1 && !colisao(fantasma->x, fantasma->y-1))
        {
            fantasma->sentido = CIMA;
        }
    }    
   
    else if(fantasma->sentido == ESQUERDA)    
    {
        fantasma->x -= PASSO;
        if(colisao(fantasma->x, fantasma->y))
        {
            fantasma->x += PASSO;
            while(fantasma->sentido == DIREITA || fantasma->sentido == ESQUERDA)
            {
                fantasma->sentido = rand() % 4;
            }  
        } 
        
        i = rand() % 3;
        
        if(i == 0 && !colisao(fantasma->x, fantasma->y+1))
        {
            fantasma->sentido = BAIXO;
        }    

        if(i == 1 && !colisao(fantasma->x, fantasma->y-1))
        {
            fantasma->sentido = CIMA;
        }         
    }    
  
    else if(fantasma->sentido == BAIXO)    
    {
        fantasma->y += PASSO;        
        if(colisao(fantasma->x, fantasma->y))
        {
            fantasma->y -= PASSO;
            while(fantasma->sentido == BAIXO || fantasma->sentido == CIMA)
            {
                fantasma->sentido = rand() % 4;
            }   
        }    

        i = rand() % 3;

        if(i == 0 && !colisao(fantasma->x+1, fantasma->y))
        {
            fantasma->sentido = DIREITA;
        }    

        if(i == 1 && !colisao(fantasma->x-1, fantasma->y))
        {
            fantasma->sentido = ESQUERDA;
        }
    }        
    
    else if(fantasma->sentido == CIMA)    
    {
        fantasma->y -= PASSO;
        if(colisao(fantasma->x, fantasma->y))
        {
            fantasma->y += PASSO;
            while(fantasma->sentido == BAIXO || fantasma->sentido == CIMA)
            {
                fantasma->sentido = rand() % 4;
            }  
        }  
         

        i = rand() % 3;

        if(i == 0 && !colisao(fantasma->x+1, fantasma->y))
        {
            fantasma->sentido = DIREITA;
        }    

        if(i == 1 && !colisao(fantasma->x-1, fantasma->y))
        {
            fantasma->sentido = ESQUERDA;
        }     
    } 

     /* Impede os fantasmas de saírem do mapa, fazendo-os voltar no lado oposto
        ao qual saíram. */
    if(fantasma->x > MAX_X) fantasma->x = -2*PAC_TAM+1;
    if(fantasma->x < -2*PAC_TAM) fantasma->x = MAX_X-1;      
    if(fantasma->y > MAX_Y) fantasma->y = -2*PAC_TAM+1;      
    if(fantasma->y < -2*PAC_TAM) fantasma->y = MAX_Y-1;    
}


/*
 * Função que faz os fantasmas seguirem o Pac-Man.
 * Ela funciona da seguinte forma:
 * O sentido do fantasma é apontado para o do Pac-Man, e se não houver forma
 * do fantasma se mexer para ir na direção dele, ele sofre uma mudança de
 * direção aleatória(Somente uma).
 * Logo em seguida, ele torna a seguir o Pac-Man novamente.
 */
void movFantSeguindo(SPRITE *fantasma)
{
    int x = fantasma->x;
    int y = fantasma->y;
    int aleatorio = rand() % 2;
    
      
    /* 1/2 das vezes ele move na horizontal primeiro
       e 1/2 das vezes na vertical */
    if(aleatorio)
    {
        if(fantasma->x < pacman.x && fantasma->colidiu == NAO &&
           fantasma->sentido != ESQUERDA)
        {
            fantasma->x += PASSO;
            if(colisao(fantasma->x, fantasma->y))
            {
                fantasma->x -= PASSO;
            }   
            else fantasma->sentido = DIREITA; 
        }
        else if(fantasma->x > pacman.x && fantasma->colidiu == NAO &&
           fantasma->sentido != DIREITA)
        {
            fantasma->x -= PASSO;
            if(colisao(fantasma->x, fantasma->y))
            {
                fantasma->x += PASSO;
            }    
            else fantasma->sentido = ESQUERDA;
        }    
        if(fantasma->y > pacman.y && fantasma->colidiu == NAO &&
           fantasma->sentido != BAIXO)
        {
            fantasma->y -= PASSO;
            if(colisao(fantasma->x, fantasma->y))
            {
                fantasma->y += PASSO;
            }    
            else fantasma->sentido = CIMA;
        }    
        else if(fantasma->y < pacman.y && fantasma->colidiu == NAO &&
           fantasma->sentido != CIMA)
        {
            fantasma->y += PASSO;
            if(colisao(fantasma->x, fantasma->y))
            {
                fantasma->y -= PASSO;
            }    
            else fantasma->sentido = BAIXO;
        }  
    }   
    else 
    {
        if(fantasma->y > pacman.y && fantasma->colidiu == NAO &&
           fantasma->sentido != BAIXO)
        {
            fantasma->y -= PASSO;
            if(colisao(fantasma->x, fantasma->y))
            {
                fantasma->y += PASSO;
            }    
            else fantasma->sentido = CIMA;
        }    
        else if(fantasma->y < pacman.y && fantasma->colidiu == NAO &&
           fantasma->sentido != CIMA)
        {
            fantasma->y += PASSO;
            if(colisao(fantasma->x, fantasma->y))
            {
                fantasma->y -= PASSO;
            }    
            else fantasma->sentido = BAIXO;
        }   
        if(fantasma->x < pacman.x && fantasma->colidiu == NAO &&
           fantasma->sentido != ESQUERDA)
        {
            fantasma->x += PASSO;
            if(colisao(fantasma->x, fantasma->y))
            {
                fantasma->x -= PASSO;
            }   
            else fantasma->sentido = DIREITA; 
        }
        else if(fantasma->x > pacman.x && fantasma->colidiu == NAO &&
           fantasma->sentido != DIREITA)
        {
            fantasma->x -= PASSO;
            if(colisao(fantasma->x, fantasma->y))
            {
                fantasma->x += PASSO;
            }    
            else fantasma->sentido = ESQUERDA;
        }    
    }    
    
    if(fantasma->colidiu == SIM)
    {
        if(removerColisao == SIM)
        {
            fantasma->colidiu = NAO;
            removerColisao = NAO;
            removerColisaoTimer = 0;
        }    
        movFantAleatorio(fantasma);
    }    
    else if(fantasma->x == x && fantasma->y == y)
    {
        fantasma->colidiu = SIM;
        removerColisao = NAO;                    
        removerColisaoTimer = 0;  
    }

    
}   

/*
 * Função que faz os fantasmas mortos voltarem para suas posições iniciais
 * após serem mortos pelo Pac-Man no modo Azul.
 */
void movFantMorto (SPRITE *fantasma)
{
    /* Voltando para a posicao inicial */
    if(fantasma->x < fantasma->xinic) fantasma->x += PASSO;
    if(fantasma->y > fantasma->yinic) fantasma->y -= PASSO;
    if(fantasma->x > fantasma->xinic) fantasma->x -= PASSO;
    if(fantasma->y < fantasma->yinic) fantasma->y += PASSO;

    /* Para ao chegar a posicao inicial */
    if(fantasma->x == fantasma->xinic && fantasma->y == fantasma->yinic)
    {
        fantasma->parado = SIM;
    }    
}    

/*
 * Função que desenha todas as imagens na tela.
 */
void desenhandoSprites (int bola)

{
    int i, j;
    
    /* Ordem de Impressão:
     *
     * Mapa -> Bonecos
     * Pílulas -> Bonecos
     * Fantasmas -> Bonecos
     * Bonus -> Bonecos
     * Pac-Man -> Bonecos
     * Mensagens -> Bonecos (vida, pontos, fase atual, recorde)
     * Bonecos -> Screen
     */
     
    clear(bonecos);
	blit(mapa, bonecos, 0, 0, 0, 0, MAX_X, MAX_Y);
    for (i=0 ; i<PILULASY ; i++)
    {
        for(j=0; j<PILULASX ; j++)
        {
            if(pilula[i][j].estado == 1)
            {
                draw_sprite(bonecos, pilula[i][j].sprite[0],
                            pilula[i][j].x, pilula[i][j].y);
            }  
            else if(pilula[i][j].estado == 2)
            {
                draw_sprite(bonecos, pilula[i][j].sprite[1],
                            pilula[i][j].x, pilula[i][j].y);
            }  
        }    
    }
    
    /* Fazendo o bônus aparecer aleatoriamente */
    if(bonus[fase - 1].estado == 0)
    {
        if(bonusAleatorio())
        {
            bonus[fase - 1].x = 31 + 34 * (rand() % 17);
            bonus[fase - 1].y = 29 + 34 * (rand() % 13);
            bonus[fase - 1].estado = 1;
            
            /* Testando se a posição do bonus é legal */
            for (i=0 ; i<PILULASY ; i++)
            {
                for(j=0 ; j<PILULASX ; j++)
                {
                    if(pilula[i][j].estado == 3 || pilula[i][j].estado == 2)
                    {
                        if(bonus[fase - 1].x == pilula[i][j].x &&
                           bonus[fase - 1].y == pilula[i][j].y)
                        {
                            bonus[fase - 1].estado = 0;
                        }    
                    }    
                }    
            }   
        }    
    }    
    
    /* Desenhando o Bonus */
    if(bonus[fase - 1].estado == 1)
    {
        draw_sprite(bonecos, bonus[fase - 1].sprite[0],
                bonus[fase - 1].x, bonus[fase - 1].y);
    }     

    for(i = 0; i<FANTASMAS; i++)
    {
        if(fant[i].morto == SIM)
        {
            draw_sprite(bonecos,
            fant[i].sprite[4*fant[i].morto + fant[i].sentido][0],
            fant[i].x,fant[i].y);
        }    
        else if(fant[i].morto == NAO && modoAzul == ATIVADO)
        {
            draw_sprite(bonecos,
            fant[i].sprite[8][fant[i].estado],
            fant[i].x,fant[i].y);
        }   
        else draw_sprite(bonecos,
                         fant[i].sprite[fant[i].sentido][fant[i].estado],
                         fant[i].x,fant[i].y); 
    }            
   /* Se estiver no início da fase, o Pac-Man terá uma forma de bola sem boca */    
    if(bola)
    {
        draw_sprite(bonecos, pacman.sprite[4][0], pacman.x, pacman.y);
    }    
    else
    {
        draw_sprite(bonecos, pacman.sprite[pacman.sentido][pacman.estado],
                pacman.x, pacman.y);
    }    
    

    sprintf(msg, "%d Pontos",pontos);
    textout(bonecos, font, msg, 4*MAX_X/7 - 12, 15, makecol(0, 255, 255));
    if (pontos > maxpontos)
    sprintf(msg, "Record %d Seu!",pontos);
    else
    sprintf(msg, "Record %d %s",maxpontos, jogador);
    textout(bonecos, font, msg, 4*MAX_X/7 - 12, 3, makecol(0, 255, 255));
    sprintf(msg, "FASE %d-%d",fase ,dificuldade);
    textout(bonecos, font, msg, MAX_X/3, 10, makecol(255, 0, 0));    
    sprintf(msg, "%d VIDAS",vidas);    
    textout(bonecos, font, msg, 4*MAX_X/7 + 100, 15, makecol(0, 255, 255));    

    blit(bonecos, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    
    novo_frame = 0;    
    
    /* Invervalo Inicial da Fase */
    if(bola && vidas != -1)
    {
        parada(100);
        play_sample(som[0], 200, 127, 1000, 0);
        parada(200);
    }    
}

/*
 * Função que carrega mapas das fases e posições e tipos das Pílulas
 */
void mudandoFase (void)
{
    FILE *fasex;
    FILE *fasey;
    FILE *pilulas;
    char nomex[20] = "mapas/mapa0x.map";
    char nomey[20] = "mapas/mapa0y.map";
    char nomepil[20] = "mapas/mapa0pil.map";
    int i, j;
    
    stop_midi();

    if(fase)
    {
        clear(screen);
        draw_sprite(screen, faseCompleta, 120, 160);
        parada(150);    
    }    
    
    /* Zerando a matriz do mapa */
    for(i=0; i<HORIZS ; i++)
    {
        for(j=0 ; j<3 ; j++)
        {
            horiz[i][j] = 0;
        }
    }
    for(i=0; i<VERTS ; i++)
    {
        for(j=0 ; j<3 ; j++)
        {
            vert[i][j] = 0;
        }
    }    

    /* Após vencer todas as fases, o jogo retorna para a primeira fase,
       porém com dificuldade aumentada  */
    if(fase % FASES == 0)
    {
        fase = 0; 
        dificuldade++;
        /* Zerando o aparecimento dos bonus */
        for(i=0 ; i<FASES ; i++)
        {
            bonus[i].estado = 0;
        }    
    }    

    fase++;

    /* A cada vez que se passa de fase, os Frames por Segundo são aumentados */
    fps += 4 + dificuldade;
	install_int(inc_novo_frame, 1000 / fps);
    
    clear(mapa);
 
    /* Desenha o fundo de tela das fases */
    blit(background[fase - 1], mapa, 0, 0, 0, 0, MAX_X, MAX_Y);  

    /* Abrindo arquivos que contém as informações sobre a fase atual */
    nomex[10] = fase + 48;
    nomey[10] = fase + 48;
    nomepil[10] = fase + 48;        
    fasex = fopen(nomex, "r");
    fasey = fopen(nomey, "r");
    pilulas = fopen(nomepil, "r");
    
    iniciarPosicoes();
    
    /* Carregando as Pílulas */
    for (i=0 ; i<PILULASY ; i++)
    {
        for(j=0 ; j<PILULASX ; j++)
        {
            fscanf(pilulas, "%d ", &pilula[i][j].estado); 
        }    
    }     
    
    /* Carregando as Paredes */                            
    for(i=0 ; i<VERTS ; i++)
    {
        fscanf(fasex, "%d", &vert[i][0]);
        fscanf(fasex, "%d", &vert[i][1]);
        fscanf(fasex, "%d", &vert[i][2]);   
        if(feof(fasex)) break;
    }
    for(i=0 ; i<HORIZS ; i++)
    {
        fscanf(fasey, "%d", &horiz[i][1]);
        fscanf(fasey, "%d", &horiz[i][0]);
        fscanf(fasey, "%d", &horiz[i][2]);                
        if(feof(fasey)) break;            
    }    
    
    modoAzul = DESATIVADO;
    
    timerBonus = 0;
    
    play_midi(musica[fase-1], TRUE);

    clear(screen);
 
    /* Desenha a introdução das fases */
    draw_sprite(screen, faseEntrada[fase - 1], 210, 115);
            
    parada(250);

    desenharMapa();   
}

/*
 * Função que carrega as posições iniciais dos personagens.
 */
void iniciarPosicoes (void)
{
    int i;
    char nomeinic[20] = "mapas/mapa0inic.map";
    FILE *faseinic;

    nomeinic[10] = fase + 48;     
    faseinic = fopen(nomeinic, "r");   
    fscanf(faseinic, "%d %d", &pacman.x, &pacman.y);        
    
    for (i = 0; i < FANTASMAS ; i++)
    {
        fscanf(faseinic, "%d %d", &fant[i].x, &fant[i].y);
        fant[i].xinic = fant[i].x;
        fant[i].yinic = fant[i].y;
        fant[i].sentido = rand() % 4;
        fant[i].estado = 1;
    }   
    fclose(faseinic);
}


/*
 * Função que testa colisão entre o Pac-Man e o Fantasma
 */
int colisaoSprites (SPRITE *pac, SPRITE *fan)
{
  	if ((fan->x >= pac->x) && (fan->x < (pac->x + PAC_TAM-8)))
	{
		if ((fan->y >= pac->y) && (fan->y < (pac->y + PAC_TAM-8))) return 1;
		if ((pac->y >= fan->y) && (pac->y < (fan->y + PAC_TAM-8))) return 1;
	}
	else if ((pac->x >= fan->x) && (pac->x < (fan->x + PAC_TAM-8)))
	{
		if ((fan->y >= pac->y) && (fan->y < (pac->y + PAC_TAM-8))) return 1;
		if ((pac->y >= fan->y) && (pac->y < (fan->y + PAC_TAM-8))) return 1;
    }
    return 0;
}  

/*
 * Função que processa a animação do Pac-Man morrendo
 */
void pacMorrendo (void)
{
    int i, /* Contador */
        j, /* Contador */
        k, /* Controla o tempo que a animação durará (junto com o novo_frame) */
        m; /* A cada 13 incrementos em k, m é incrementado e faz ocorrer mais
              1 frame do Pac-Man morrendo */
    
    /* install_int usado para coordenar o som com a animação do Pac-Man morrendo
     */
    install_int(inc_novo_frame, 1000 / FPS);
 
    for (k=0, m=0 ; k<130 ; )
    {
        if(k == 0)   play_sample(som[4], 200, 127, 1000, 0);

        if(novo_frame)
        {
            clear(bonecos);
			blit(mapa, bonecos, 0, 0, 0, 0, MAX_X, MAX_Y);
		    for (i=0 ; i<PILULASY ; i++)
		    {
		        for(j=0; j<PILULASX ; j++)
		        {
		            if(pilula[i][j].estado == 1)
		            {
		                draw_sprite(bonecos, pilula[i][j].sprite[0],
		                            pilula[i][j].x, pilula[i][j].y);
		            }    
		            else if(pilula[i][j].estado == 2)		            
		            {
		                draw_sprite(bonecos, pilula[i][j].sprite[1],
		                            pilula[i][j].x, pilula[i][j].y);
		            }    
		        }    
		    }

		    /* Desenhando o Bonus */
            if(bonus[fase - 1].estado == 1)
            {
                draw_sprite(bonecos, bonus[fase - 1].sprite[0],
                bonus[fase - 1].x, bonus[fase - 1].y);
            }     

            for(i = 0; i<FANTASMAS; i++)
            {
                if(fant[i].morto == SIM)
                {
                    draw_sprite(bonecos,
                    fant[i].sprite[4*fant[i].morto + fant[i].sentido][0],
                    fant[i].x,fant[i].y);
                }    
                else if(fant[i].morto == NAO && modoAzul == ATIVADO)
                {
                    draw_sprite(bonecos,
                    fant[i].sprite[8][fant[i].estado],
                    fant[i].x,fant[i].y);
                }   
                else draw_sprite(bonecos,
                         fant[i].sprite[fant[i].sentido][fant[i].estado],
                         fant[i].x,fant[i].y); 
            }
            if(k % 13 == 0) m++;
 		    draw_sprite(bonecos, pacmorrendo[m], pacman.x, pacman.y);
		    
            sprintf(msg, "%d Pontos",pontos);
            textout(bonecos, font, msg, 4*MAX_X/7 - 12, 15, makecol(0, 255, 255));
            if (pontos > maxpontos)
            sprintf(msg, "Record %d Seu!",pontos);
            else
            sprintf(msg, "Record %d %s",maxpontos, jogador);
            textout(bonecos, font, msg, 4*MAX_X/7 - 12, 3, makecol(0, 255, 255));
            sprintf(msg, "FASE %d-%d",fase ,dificuldade);
            textout(bonecos, font, msg, MAX_X/3, 10, makecol(255, 0, 0));    
            if(vidas != -1) sprintf(msg, "%d VIDAS",vidas);    
            else sprintf(msg, "Game Over");
            textout(bonecos, font, msg, 4*MAX_X/7 + 100, 15, makecol(0, 255, 255));    
    		                   
		    blit(bonecos, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
		    novo_frame = 0; 
            k++;   
        }    
    } 
    
    /* Caso o bonus não tenha sido comigo ainda nessa fase e ele esteja no mapa,
       ele sumirá e os testes para fazê-lo aparecer voltarão a funcionar */
    if(bonus[fase - 1].estado == 1) bonus[fase - 1].estado = 0;

    iniciarPosicoes();

    /* Reinicializando o install_int com a velocidade da fase/dificuldade atual
     */
    install_int(inc_novo_frame, 1000 / fps);    

    desenhandoSprites(SIM);
}    

/*
 * Função que, aleatoriamente, diz quando o bônus deve aparecer
 */
int bonusAleatorio (void)
{
    int i, j;
    int pilulasExistentes = 0;
    
    /* A probabilidade do bonus aparecer tem relação com a quantidade de
       pílulas existentes */
    for(i=0 ; i<PILULASY ; i++)
    {
        for(j=0 ; j<PILULASX ; j++)
        {
            if(pilula[i][j].estado == 1 || pilula[i][j].estado == 2)
            {
                pilulasExistentes++;
            }    
        }    
    }    
    /* Se numa jogada de azar, o bonus não aparecer depois de tantas tentativas
       ele será obrigado a aparecer se houverem menos de 5 pílulas */
    if(pilulasExistentes <= 6) return 1;
    else if(rand () % 2500 * pilulasExistentes == 0) return 1;
    else return 0;
}    

/*
 * Faz as animações do jogo congelarem por um número definido de segundos
 */
void parada (int centesimos)
{
    int i;
    for(i=0 ; i<fps*centesimos/100 ; )
    {
        if(novo_frame)
        {
            i++;
            novo_frame = 0;
        }    
    }    
} 

/*
 * Função que controla o Menu Inicial do jogo
 */
void menuInicial(void)
{

    int ynicial = 350;
    int xinicial[5] = {536, 504, 472, 440, 380};
    int direc = ESQUERDA;
    int mov = 0;
    int i;
    
  	estadoTimer = 0;
    
    if(pontos > maxpontosAtual)
    {
        maxpontosAtual = pontos;
    }    
    
    /* Resetando as variáveis */
    estadoTimer = 0;
    fantMortos = 0;
    pontos = 0;
    fase = 0;
    dificuldade = 0;
    fps = FPS;
    vidas = VIDAS;
    modoAzul = DESATIVADO;
    timerBonus = 0;
    clear(screen);
    
    play_midi(musica[9], TRUE);
    while (1) /* loop da tela "aperte espaço" */
 	{   
     blit(menu[0], bonecos, 0, 0, 0, 0, MAX_X, MAX_Y); 

     for(i = 0; i < FANTASMAS; i++)
     { 
         if(direc == ESQUERDA)
         {
             draw_sprite(bonecos, fant[i].sprite[direc][fant[i].estado],
                         440 + 32*i + mov,ynicial);       
             draw_sprite(bonecos, pilula[0][0].sprite[1], 80, ynicial);
         }
         else if(direc == DIREITA)
         {
             draw_sprite(bonecos, fant[i].sprite[8][fant[i].estado],
                         440 + 32*i + mov, ynicial);    
         }    
     }
     
     draw_sprite(bonecos, pacman.sprite[direc][pacman.estado],
                 380 + mov,ynicial);
     
     if(pacman.estado) draw_sprite(bonecos, menu[1], 240, 280);
     
     if(direc == ESQUERDA) mov -= 1;
     else mov += 1;

     if(mov < -300) direc = 0;
     else if(mov > 0) direc = 2;

     if(maxpontosAtual > maxpontos)
     {
         sprintf(msg, "Record %d - Atual Jogador", maxpontosAtual);
         textout(bonecos, font, msg, 70, 450, makecol(0, 255, 255));
     }  
     else if(maxpontos > 0)
     {
         sprintf(msg, "Record %d %s", maxpontos, jogador);
         textout(bonecos, font, msg, 70, 450, makecol(0, 255, 255));
     }    
     
     if(key[KEY_ESC])
     {
         terminarJogo = SIM;
         break;
     }    
     else if(key[KEY_SPACE] || key[KEY_ENTER])
     {
         menuInicial2(mov);
         break;
     }
     blit(bonecos, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
     
  } 
}    

void menuInicial2(int mov)
{
    int i;
    int selecionado = 0;

    while(1) /* bonecos indo para a esquerda */
    {
        blit(menu[0], bonecos, 0, 0, 0, 0, MAX_X, MAX_Y);
        draw_sprite(bonecos, menu[2], 50, 125);
        draw_sprite(bonecos, menu[3], 80, 250); 
        draw_sprite(bonecos, menu[3], 172, 400); 
        draw_sprite(bonecos, menu[3], 204, 400); 
        draw_sprite(bonecos, menu[3], 236, 400); 
        draw_sprite(bonecos, menu[3], 140, 400); 
    
        for(i = 0; i < FANTASMAS; i++)
        {
            draw_sprite(bonecos, fant[i].sprite[2][fant[i].estado],
                        440 + 32*i + mov,350);  
        }    
    
        draw_sprite(bonecos, pacman.sprite[2][pacman.estado],380 + mov,350);
     
        if(mov > -300.0) mov -= 1;
        else break;
    
        if(key[KEY_ESC])
        {
            terminarJogo = SIM;
            break;
        }
        blit(bonecos, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    }

    for(i = 0; i < FANTASMAS; i++)
    {
        fant[i].x = 140 + 32*i;
        fant[i].y = 350;
    }
    pacman.x = 80;
    pacman.y = 350;
        
    while(1)  /*pacman subindo, monstros descendo */
    {
        blit(menu[0], bonecos, 0, 0, 0, 0, MAX_X, MAX_Y);
        draw_sprite(bonecos, menu[2], 50, 125);
        draw_sprite(bonecos, menu[3], 80, 250); 
        draw_sprite(bonecos, menu[3], 172, 400); 
        draw_sprite(bonecos, menu[3], 204, 400); 
        draw_sprite(bonecos, menu[3], 236, 400); 
        draw_sprite(bonecos, menu[3], 140, 400); 
        
        for(i = 0; i < FANTASMAS; i++)
        {
            if(fant[i].y < 480)
            fant[i].y += 1;    
            draw_sprite(bonecos, fant[i].sprite[1][fant[i].estado],fant[i].x ,
                        fant[i].y);  
        }
        if(pacman.y > 128)
        {
            pacman.y -= 1;
            draw_sprite(bonecos, pacman.sprite[3][pacman.estado], pacman.x,
                        pacman.y);                  
        }
        else
        {
            draw_sprite(bonecos, pacman.sprite[0][pacman.estado], pacman.x,
                        pacman.y);    
            break;
        }
    
        if(key[KEY_ESC])
        {
            terminarJogo = SIM;
            break;
        }
        blit(bonecos, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    }

    mudaHeroi();

    while(1) /* Selecionando opções no menu */
    {
        blit(menu[0], bonecos, 0, 0, 0, 0, MAX_X, MAX_Y);
        draw_sprite(bonecos, menu[2], 50, 125);
        draw_sprite(bonecos, menu[3], 80, 250); 
        draw_sprite(bonecos, menu[3], 172, 400); 
        draw_sprite(bonecos, menu[3], 204, 400); 
        draw_sprite(bonecos, menu[3], 236, 400); 
        draw_sprite(bonecos, menu[3], 140, 400); 

        if(key[KEY_DOWN] && selecionado < 3)
        {
            parada(10);
            selecionado++;
            pacman.y += 32;
        }
        if(key[KEY_UP] && selecionado > 0)
        {
            parada(10);
            selecionado--;
            pacman.y -= 32;
        }
        if(key[KEY_SPACE] || key[KEY_ENTER])
        {
            play_sample(som[1], 200, 127, 1000, 0);
            if(pacman.y == 128)
            {
                parada(50);
                break;
            }
            else if(pacman.y == 160)
            {
                heroi();
            }
            else if(pacman.y == 192)
            {
                manual();    
            }
            else if(pacman.y == 224)
            {
                terminarJogo = SIM;
                break;
            }    
        }
       
        draw_sprite(bonecos, pacman.sprite[0][pacman.estado], pacman.x,
                    pacman.y);
    
        if(key[KEY_ESC])
        {
            terminarJogo = SIM;
            break;
        }
        blit(bonecos, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    }

}

void manual (void)
{
    blit(menu[0], bonecos, 0, 0, 0, 0, MAX_X, MAX_Y);
    draw_sprite(bonecos, menu[2], 50, 125);
    draw_sprite(bonecos, menu[3],80,250); 
    draw_sprite(bonecos, menu[3],172,400); 
    draw_sprite(bonecos, menu[3],204,400); 
    draw_sprite(bonecos, menu[3],236,400); 
    draw_sprite(bonecos, menu[3],140,400); 
    draw_sprite(bonecos, menu[4], 40, 330);
    draw_sprite(bonecos, pacman.sprite[0][0], pacman.x, pacman.y);
    blit(bonecos, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    parada(250);
}


/*
 * Funcao que escolhe o heroi personagem
 */
void heroi(void)
{
    while(1)
    {
        blit(menu[0], bonecos, 0, 0, 0, 0, MAX_X, MAX_Y);
        draw_sprite(bonecos, menu[2], 50, 125);
        draw_sprite(bonecos, menu[3],80,250); 
        draw_sprite(bonecos, menu[3],172,400); 
        draw_sprite(bonecos, menu[3],204,400); 
        draw_sprite(bonecos, menu[3],236,400); 
        draw_sprite(bonecos, menu[3],140,400); 
        draw_sprite(bonecos, menu[5], 40, 330);
        draw_sprite(bonecos, pacman.sprite[0][0], pacman.x, pacman.y);

        if(key[KEY_RIGHT] && heroiatual < 2)
        {
            heroiatual++;
            parada(15);
            mudaHeroi();
        }
        if(key[KEY_LEFT] && heroiatual > 0)
        {
            heroiatual--;
            parada(15);
            mudaHeroi();
        }
    
        draw_sprite(bonecos, menu[6+heroiatual], 480, 350);
    
        if(key[KEY_UP] || key[KEY_DOWN]) break;    
        
        blit(bonecos, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    }
}

/*
 * Função que muda o herói escolhido
 */
void mudaHeroi()
{
    switch (heroiatual)
        {
            case 0: /* Pac-Man clássico */
                pacman.sprite[0][0] = imagens[4];
                pacman.sprite[0][1] = imagens[5];
                pacman.sprite[1][0] = imagens[2];
                pacman.sprite[1][1] = imagens[3];
                pacman.sprite[2][0] = imagens[6];
                pacman.sprite[2][1] = imagens[7];
                pacman.sprite[3][0] = imagens[0];
                pacman.sprite[3][1] = imagens[1];
                pacman.sprite[4][0] = imagens[8];

                musica[0] = load_midi("musicas/alexstg1.midi");
    
                heroiatual = 0;
    
                break;


            case 1: /* Jackson-Man */

                pacman.sprite[0][0] = imagens[76];
                pacman.sprite[0][1] = imagens[77];
                pacman.sprite[1][0] = imagens[74];
                pacman.sprite[1][1] = imagens[75];
                pacman.sprite[2][0] = imagens[78];
                pacman.sprite[2][1] = imagens[79];
                pacman.sprite[3][0] = imagens[72];
                pacman.sprite[3][1] = imagens[73];
                pacman.sprite[4][0] = imagens[79];

                musica[0] = load_midi("musicas/jackson/jackson.midi");

                heroiatual = 1;

                break;

      
            case 2: /* Noel-Man */

                pacman.sprite[0][0] = imagens[84];
                pacman.sprite[0][1] = imagens[85];
                pacman.sprite[1][0] = imagens[82];
                pacman.sprite[1][1] = imagens[83];
                pacman.sprite[2][0] = imagens[86];
                pacman.sprite[2][1] = imagens[87];
                pacman.sprite[3][0] = imagens[80];
                pacman.sprite[3][1] = imagens[81];
                pacman.sprite[4][0] = imagens[87];
            
                background[0] = load_bitmap("backgrounds/bluexmas.bmp", NULL);
                background[8] = load_bitmap("backgrounds/theride.bmp", NULL);                

                musica[0] = load_midi("musicas/noel/jinglenatal.midi");

                heroiatual = 2;

                break;
    }
}

/*
 * Função que salva um novo recorde caso haja um
 */
void salvaRecorde (void)
{
    int i;
	char *nome = "pace.dat";
	FILE *pa;

	if (maxpontosAtual > maxpontos)
	{
    	pa = fopen(nome, "w");
    	if (!pa)
	    {
	    	puts("Erro na abertura do arquivo pace.dat");
    	}
    	printf("Novo Recorde!\nJogador: ");
    	gets(jogador);
    	jogador[9] = '\0';
    	fprintf(pa, "%-10s", jogador);
    	fprintf(pa, " %d", maxpontosAtual);
    	fclose(pa);
	}
}

/*
 * Função que controla o timer do jogo.
 * Ela é executada "fps" vezes por segundo.
 */
void inc_novo_frame (void)
{
    int i;
    
    novo_frame++;        

    /* Controla a velocidade que o estado dos personagens é mudado */
    estadoTimer++;
	if(estadoTimer == fps/3)
	{
       pacman.estado = (pacman.estado + 1) % 2;
       for(i=0 ; i<FANTASMAS ; i++)
       {
           fant[i].estado = (fant[i].estado + 1) % 2;       
       }    
       estadoTimer = 0;
    }    
    
    removerColisaoTimer++;
    if(removerColisaoTimer == 2)
    {
        removerColisao = SIM;
    }    
    
}
END_OF_FUNCTION(inc_novo_frame); 
