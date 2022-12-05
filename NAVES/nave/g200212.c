#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

#define MENH 20		/*altura do menu*/
#define MAXW 800	/*tamanho da tela*/
#define MAXH 580	/*tamanho do fundo*/
#define SCRH 600	/*tamanho da tela*/

#define MAXT 100	/*máximo de tiro*/
#define MAXE 75		/*máximo de escudo*/
#define MAXF 50000	/*máximo de combustível*/

#define ETEMP 30	/*temporizador de escudo*/
#define FTEMP 10	/*temporizador de fundo*/
#define STEP 12		/*passo da nave*/
#define NUMSTARS 300/*numero de estrelas*/

#define TXTMD -1	/*modo de texto*/

#define CDEPTH 8 /*8 bits de cor*/

#define BOTW 300	/*largura do botão*/
#define BOTH 50		/*altura do botão*/

#define SIM 1
#define NAO 0

volatile int rel;

void incRelogio(void)		/*função que incrementa os contadores*/
{
rel++;
}
END_OF_FUNCTION(incRelogio);
								/*funcóes do jogo*/
int inicia(void); 			/*verifica se som e vídeo estão ok */
void principal(int);		/* função principal*/
void finaliza(void);		/* finaliza o ALLEGRO*/
void mstart(int);			/* mstart é a função de menu */
void incRelogio(void);
void mudaPaleta(int, int, int, int, PALETTE);		/*configura a paleta*/
int confirmarSair(int); /*mostra o vencedor*/

int main(void)
{
	if (!inicia()) /*se a função int inicia não iniciar corretamente o jogo não irá começar*/
	{
    		finaliza();
    		exit(-1);
	}

	principal(SIM);

	finaliza();
	exit(0);
}
END_OF_MAIN();

int inicia(void) /* função que verifica se o vídeo e o som foram carregados corretamente*/
{

    allegro_init(); /*inicia o allegro*/

    install_keyboard();
    install_timer();

    set_color_depth(CDEPTH);
    if(set_gfx_mode(GFX_AUTODETECT, MAXW, SCRH, 0, 0)!=0) /*reconhecer placa de vídeo*/
    {
     printf("NÆo foi possivel carregar o video\n");
     readkey();
     return(FALSE);
    }
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)!=0) /*reconhecer som*/
    {
     printf("NÆo foi possivel detectar o driver de som\n");
     readkey();
     return(FALSE);
    }

    LOCK_VARIABLE(rel);
    LOCK_FUNCTION(incRelogio);
    return(TRUE);
}

void principal(int comeco) /* esta é a função principal do jogo onde estão a maioria dos comandos etc...*/
{

int posFundo=0;
int xp1, yp1, xp2, yp2, ye1, ye2, escudo1, escudo2, i, j;		 /*declaração de variáveis*/
int xm1[5], ym1[5], ym2[5], xm2[5], atirou1=0, atirou2=0;
int fimTiro1=1, fimTiro2=1;
int tiros1=MAXT, tiros2=MAXT, cesc1=MAXE, cesc2=MAXE, fuel1=MAXF, 
fuel2=MAXF;
long startE1, timeE1, startE2, timeE2, startFundo, timeFundo=0;
int x, y;
				/*decalrações de bitmap, paleta, e som*/
BITMAP *fundo;
BITMAP *nave;
BITMAP *nave2;
BITMAP *tiro;
BITMAP *escudo;
BITMAP *menu;
BITMAP *vscreen;
PALLETE pal;
SAMPLE *somTiro;

rel = 1;
install_int(incRelogio, 10);

vscreen = create_bitmap(MAXW, SCRH);
menu = create_bitmap(MAXW, MENH);
fundo = create_bitmap(MAXW, MAXH);
nave = load_bitmap("nave.bmp", pal);
tiro = load_bitmap("tiro.bmp", NULL);
nave2 = load_bitmap("nave2.bmp", NULL);
escudo = load_bitmap("escudo.bmp", NULL);
somTiro = load_sample("tiro.wav");
							/*definição de uma paleta própria*/
mudaPaleta(0, 0, 0, 0, pal); /*chama a função mudaPaleta que a configura*/
mudaPaleta(255, 255, 255, 1, pal);
mudaPaleta(255, 255, 0, 3, pal);
mudaPaleta(255, 0, 0, 4, pal);

set_palette(pal);				/*posição inicial dos objetos no jogo*/
							
xp1 = (MAXW/2)-(nave->w/2);/* posição x do jogador 1 na tela para o início do jogo */
yp1 = MAXH-nave->h; /* posição y do jogador 1 na tela para o início do jogo */
xp2 = (MAXW/2)-(nave->w/2);/* posição x do jogador 2 na tela para o início do jogo */
yp2 = 0; /* posição y do jogador 1 na tela para o início do jogo */

ym1[atirou1] = MAXH;	/* posição do tiro do jogador 1 */
ym2[atirou2] = 0;		/* posição do tiro do jogador 2 */

ye1 = yp1-escudo->h-3;  /* posição do escudo do jogador 1*/
ye2 = yp2+nave->h+3;	/* posição do escudo do jogador 2*/

clear(screen);		/*limpa a tela*/
clear(vscreen);		/*limpa a tela virtual*/
clear(menu);		/*limpa o menu*/
clear(fundo);		/*limpa o fundo*/

set_volume(255, 255); /*definições de volume*/

srand(time(0));
						/*esta parte cria os ponto no fundo para dar a impressão de movimento*/
for(i=1;i<=NUMSTARS;i++)
{
x = rand() % fundo->w;
y = rand() % fundo->h;
putpixel(fundo, x, y, 1);
}
						
text_mode(TXTMD);
 
if(comeco)
mstart(NAO); 

while(1) 
{
if (!timeFundo) 
    startFundo = rel;			/*verificação de tempo de rolamento do fundo*/
timeFundo = rel;
if ((timeFundo-startFundo)>=FTEMP)
{
    timeFundo=0;
    posFundo+=STEP*2;
    if (posFundo>fundo->h)
       posFundo = 0;
}

if ((!fimTiro1 && !atirou1) && (!fimTiro2 && !atirou2)) /*verifica se acabou os tiros ou o adversário for atingido*/
{																			
  if (confirmarSair(3))
    principal(NAO);
  return;
}
if (key[KEY_ESC]) /* se o ESC for apertado a função de menu é chamada */
    mstart(SIM);
if (key[KEY_LEFT] && (xp1-STEP)>0 && (!key[KEY_RIGHT]))
{
    xp1-=STEP;		/*faz a movimentação do jogador 1 para esquerda e diminui o combustível*/
    fuel1--;
}
if (key[KEY_RIGHT] && (xp1+STEP)<MAXW-nave->w && (!key[KEY_LEFT]))
{
    xp1+=STEP;		/*faz a movimentação do jogador 1 para direita e diminui o combustível*/
    fuel1--;
}
if (key[KEY_J] && (xp2-STEP)>0 && (!key[KEY_K]))
{
    xp2-=STEP;		/*faz a movimentação do jogador 2 para esquerda e diminui o combustível*/
    fuel2--;
}
if (key[KEY_K] && (xp2+STEP)<MAXW-nave->w && (!key[KEY_J]))
{
    xp2+=STEP;		/*faz a movimentação do jogador 1 para direita e diminui o combustível*/
    fuel2--;
}
if (key[KEY_INSERT] && (!atirou1 || (ym1[atirou1]<(3*((MAXH-nave->h)/4)))) 
&& (tiros1>0))		/* se o jogador 1 atirar: */
{
    play_sample(somTiro, 255, 128, 1000, FALSE); /*som do tiro */
    atirou1++; /*aumenta a quantidade de tiros na tela*/
    tiros1--; /*diminui a quantidade dos tiros*/
    xm1[atirou1] = (xp1+(nave->w/2))-(tiro->w/2);
    ym1[atirou1] = yp1-tiro->h; 
}
if (key[KEY_A] && (!atirou2 || (ym2[atirou2]>((MAXH-nave->h)/4)+nave->h)) && 
(tiros2>0))			/*se o jogador 2  atirar:*/
{
    play_sample(somTiro, 255, 128, 1000, FALSE); /*som do tiro*/
    atirou2++; /*aumenta a quantidade de tiros*/
    tiros2--; /*diminui a quantidade dos tiros */
    xm2[atirou2] = (xp2+(nave->w/2))-(tiro->w/2); 
    ym2[atirou2] = yp2+nave->h; 
}
if (key[KEY_UP] && cesc1>0) /* verifica se ainda resta escudo e o ativa */
{
    if (!escudo1)
    {
       startE1 = rel;
       cesc1--; /* diminui a quantidade de escudo do jogador 1*/
    }
    escudo1 = 1;
    timeE1 = rel;
    if ((timeE1-startE1)>=ETEMP)
    {
       cesc1--;
       startE1 = rel;
    }
if (cesc1==0) /* caso a quantidade de escudo acabe o escudo não é amis ativado*/
    escudo1 = 0;
}

if (!key[KEY_UP])
    escudo1 = 0;
if (key[KEY_Z] && cesc2>0)	/* verifica se ainda resta escudo e o ativa */
{
    if (!escudo2)
    {
       startE2 = rel;
       cesc2--;			/* diminui a quantidade de escudo do jogador 2*/
    }
    escudo2 = 1;
    timeE2 = rel;
    if ((timeE2-startE2)>=ETEMP)
    {
       cesc2--;
       startE2 = rel;
    }
    if (cesc2==0)/* caso a quantidade de escudo acabe o escudo não é amis ativado*/
       escudo2 = 0;
}

if (!key[KEY_Z])
    escudo2 = 0;

clear(vscreen);
clear(menu);

blit(fundo, vscreen, 0, 0, 0, posFundo-fundo->h, fundo->w, fundo->h);
blit(fundo, vscreen, 0, 0, 0, posFundo, fundo->w, fundo->h);

if (escudo1)	/*desenha o escudo do jogador 1*/
    blit(escudo, vscreen, 0, 0, xp1, ye1, escudo->w, escudo->h);
if (escudo2)	/*desenha o escudo do jogador 2*/
    blit(escudo, vscreen, 0, 0, xp2, ye2, escudo->w, escudo->h);
  
if (atirou1)	/*verifica se atirou*/
{
    for(i=1;i<=atirou1;i++)
    {
     if (escudo2)
        if (((ym1[i]<=ye2+escudo->h) && (ym1[i]>yp2+nave->h)) && 
(((xm1[i]>=xp2)&&(xm1[i]<=xp2+nave->w))||((xm1[i]+tiro->w>=xp2)&&(xm1[i]+tiro->w<=xp2+nave->w))))
           ym1[i] = 0;
     if (((ym1[i]<=yp2+nave->h) && (ym1[i]>0)) &&  
(((xm1[i]>=xp2)&&(xm1[i]<=xp2+nave->w))||((xm1[i]+tiro->w>=xp2)&&(xm1[i]+tiro->w<=xp2+nave->w))))
     {		/*verifica se acertou o adversário*/
     if (confirmarSair(1))
        principal(NAO);
     return;
     }

     if (ym1[i]>0) /*verifica se o tiro saiu da tela*/
     {
        ym1[i]-=STEP;
        blit(tiro, vscreen, 0, 0, xm1[i], ym1[i], tiro->w, tiro->h);
     }
     else
     {
        for(j=1;j<atirou1;j++)
        {
           ym1[j] = ym1[j+1];
           xm1[j] = xm1[j+1];
        }
        ym1[atirou1] = yp1;
        atirou1--;
        if (tiros1==0)
           fimTiro1=0;
     }
    }
}

if (atirou2)
{
    for(i=1;i<=atirou2;i++)
    {
     if (escudo1)
        if (((ym2[i]+tiro->h>=ye1) && (ym2[i]+tiro->h<yp1)) && 
(((xm2[i]>=xp1)&&(xm2[i]<=xp1+nave->w))||((xm2[i]+tiro->w>=xp1)&&(xm2[i]+tiro->w<=xp1+nave->w))))
           ym2[i] = MAXH;
     if (((ym2[i]+tiro->h>=yp1) && (ym2[i]+tiro->h<MAXH)) && 
(((xm2[i]>=xp1)&&(xm2[i]<=xp1+nave->w))||((xm2[i]+tiro->w>=xp1)&&(xm2[i]+tiro->w<=xp1+nave->w))))
     {
     if (confirmarSair(2))
        principal(NAO);
     return;
     }

     if (ym2[i]+tiro->h<MAXH)
     {
        ym2[i]+=STEP;
        blit(tiro, vscreen, 0, 0, xm2[i], ym2[i], tiro->w, tiro->h);
     }
     else
     {
        for(j=1;j<atirou2;j++)
        {
          ym2[j] = ym2[j+1];
          xm2[j] = xm2[j+1];
        }
        ym2[atirou2] = yp2+nave->h;
        atirou2--;
        if (tiros2==0)
           fimTiro2=0;
     }
    }
}

rectfill(menu, 0, 0, MAXW, MENH, 10);
hline(menu, 0, 0, MAXW, 2);
textout(menu, font, "PLAYER 1", 5, 5, 3);
textprintf(menu, font, 100, 5, 3, "TIROS %d", tiros1);
textprintf(menu, font, 200, 5, 3, "ESCUDO %d", cesc1);
textprintf(menu, font, 300, 5, 3, "FUEL %d", fuel1);
textout(menu, font, "PLAYER 2", 5+menu->w/2, 5, 4);
textprintf(menu, font, 105+menu->w/2, 5, 4, "TIROS %d", tiros2);
textprintf(menu, font, 205+menu->w/2, 5, 4, "ESCUDO %d", cesc2);
textprintf(menu, font, 305+menu->w/2, 5, 4, "FUEL %d", fuel2);
blit(menu, vscreen, 0, 0, 0, MAXH, menu->w, menu->h);
blit(nave, vscreen, 0, 0, xp1, yp1, nave->w, nave->h);
blit(nave2, vscreen, 0, 0, xp2, yp2, nave->w, nave->h);
blit(vscreen, screen, 0, 0, 0, 0, MAXW, SCRH);
}


remove_int(incRelogio);

}

void mstart(int pausa) /* função que mostra o menu no início do jogo ou quando é chamada */ 
{

BITMAP *menustart;

menustart = create_bitmap(MAXW, SCRH);

clear(menustart);

if(pausa)
  rect(menustart, (MAXW/2)-(BOTW/2), 120,(MAXW/2)-(BOTW/2)+BOTW, 120+BOTH, 
10);
rect(menustart, (MAXW/2)-(BOTW/2), 200,(MAXW/2)-(BOTW/2)+BOTW, 200+BOTH, 
10);
rect(menustart, (MAXW/2)-(BOTW/2), 280,(MAXW/2)-(BOTW/2)+BOTW, 280+BOTH, 
10);
rect(menustart, (MAXW/2)-(BOTW/2), 360,(MAXW/2)-(BOTW/2)+BOTW, 360+BOTH, 
10);
rect(menustart, (MAXW/2)-(BOTW/2), 440,(MAXW/2)-(BOTW/2)+BOTW, 440+BOTH, 
10);

if(pausa)
  rectfill(menustart, (MAXW/2)-(BOTW/2)+2, 120+2,(MAXW/2)-(BOTW/2)+BOTW-2, 
120+BOTH-2, 10);
rectfill(menustart, (MAXW/2)-(BOTW/2)+2, 200+2,(MAXW/2)-(BOTW/2)+BOTW-2, 
200+BOTH-2, 10);
rectfill(menustart, (MAXW/2)-(BOTW/2)+2, 280+2,(MAXW/2)-(BOTW/2)+BOTW-2, 
280+BOTH-2, 10);
rectfill(menustart, (MAXW/2)-(BOTW/2)+2, 360+2,(MAXW/2)-(BOTW/2)+BOTW-2, 
360+BOTH-2, 10);
rectfill(menustart, (MAXW/2)-(BOTW/2)+2, 440+2,(MAXW/2)-(BOTW/2)+BOTW-2, 
440+BOTH-2, 10);

if(pausa)
  textout_centre(menustart, font, "C                ", (MAXW/2), 140, 4);
textout_centre(menustart, font, "N                ", (MAXW/2), 220, 4);
textout_centre(menustart, font, "I        ", (MAXW/2), 300, 4);
textout_centre(menustart, font, "R                          ", (MAXW/2), 
380, 4);
textout_centre(menustart, font, "S      ", (MAXW/2), 460, 4);

if(pausa)
  textout_centre(menustart, font, " O N T I N U A R", (MAXW/2)+1, 140, 3);
textout_centre(menustart, font, " O V O   J O G O", (MAXW/2)+1, 220, 3);
textout_centre(menustart, font, " T E N S", (MAXW/2)+1, 300, 3);
textout_centre(menustart, font, " E G R A S   D O   J O G O", (MAXW/2)+1, 
380, 3);
textout_centre(menustart, font, " A I R", (MAXW/2)+1, 460, 3);

blit(menustart, screen, 0, 0, 0, 0, MAXW, SCRH);

while(1)
{
if(key[KEY_S])
  exit(0);
if(key[KEY_C] && pausa)
  break;
if(key[KEY_N])
  principal(NAO);
}

destroy_bitmap(menustart);

}

int confirmarSair(int playerWon) /*função que mostra o vencedor do jogo*/
{

clear(screen);

textout_centre(screen, font, "O JOGO TERMINOU!!!", (MAXW/2)-15, (SCRH/2)-60, 
3);
if ((playerWon==1) || (playerWon==2))
    textprintf_centre(screen, font, (MAXW/2)-15, (SCRH/2)-20, 3, "PLAYER %d VENCEU A BATALHA!!!", playerWon);
if (playerWon==3)
    textout_centre(screen, font, "A BATALHA TERMINOU POR FALTA DE TIROS!!!", 
(MAXW/2)-15, (SCRH/2)-20, 3);
textout_centre(screen, font, "S - Sair", (MAXW/2)-15, (SCRH/2)+20, 3);
textout_centre(screen, font, "N - Novo jogo", (MAXW/2)-15, (SCRH/2)+40, 3);

while(!key[KEY_S] || !key[KEY_N])
{
  if(key[KEY_S])
   return 0;
  if(key[KEY_N])
   return 1;
}
}

void finaliza(void)
{
allegro_exit();
}

void mudaPaleta(int r, int g, int b, int indice, PALETTE pal)/*função que define e cria a própria paleta para o jogo*/
{
pal[indice].r = r;
pal[indice].g = g;
pal[indice].b = b;
return;
}

