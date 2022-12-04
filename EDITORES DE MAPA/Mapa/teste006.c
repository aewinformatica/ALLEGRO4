/* Teste #6
		Testa o obstmap
*/

#include <stdio.h>
#include <conio.h>
#include "allegro.h"
#include "mapa.h"

#define FPS       30				/* frames por segundo */
#define SX			320			/* resolução da tela (SX x SY) */
#define SY			200
#define MX			640			/* tamanho do mapa (MX x MY) */
#define MY			480
#define VX			320			/* campo de visão (VX x VY) */
#define VY			140
#define OBST_ID	0				/* id do layer de obstáculos */
#define PAREDE_ID	0
#define PAREDE_V	0xEF
#define SALVAR_ID	1
#define SALVAR_V	0x80

void salvar(MAPA *m, char *nome);
void carregar(MAPA *m, char *nome);

volatile	int cont=0, fps=0, frame=0;
void inc_cont(void);
void fps_cont(void);

void inc_cont(void) { cont++; } END_OF_FUNCTION(inc_cont);
void fps_cont(void)
{
	fps = frame;
	frame = 0;
}
END_OF_FUNCTION(fps_cont);

int main(void)
{
	MAPA *m;
	OBSTMAP *o;
	PALETTE pal;
	BITMAP *buf;

	/* posição do ponto e sua cor */
	int x, y, w=10, h=10;
	int cor;

	allegro_init();
	install_keyboard();

	clrscr();
	printf("Criando mapa %dx%d...", MX, MY);
	m = mapa_cria(MX, MY, VX, VY);
	if(mapa_poe_layer(m, OBST_ID, L_T_OBSTMAP, 0, 0)) {
		printf("ERRO 1\n");
		fflush(stdout);
		mapa_destroi(m);
		readkey();
		clrscr();
		return 1;
	}
	printf("OK\n");

	printf("Colocando o mapa de obstáculos...");
	fflush(stdout);
	if(mapa_poe_layer(m, OBST_ID+1, L_T_OBSTMAP, 0, 0)) {
		printf("Isto era esperado...");
	}
	else {
		printf("ERRO 2\n");
		fflush(stdout);
		mapa_destroi(m);
		readkey();
		clrscr();
		return 2;
	}
	if(mapa_cria_dados(m, OBST_ID, 0, 0, 0)) {
		printf("ERRO 3\n");
		fflush(stdout);
		mapa_destroi(m);
		readkey();
		clrscr();
		return 3;
	}
	printf("OK\n");

	printf("Criando obstáculos...");
	fflush(stdout);
	o = (OBSTMAP *)mapa_pega_dados(m, OBST_ID);
	/* criando uma borda intransponível */
	obstmap_poe_obstaculo(o, PAREDE_ID, 0, 0, 640, 10, PAREDE_V);
	obstmap_poe_obstaculo(o, PAREDE_ID, 0, 0, 10, 480, PAREDE_V);
	obstmap_poe_obstaculo(o, PAREDE_ID, 630, 0, 10, 480, PAREDE_V);
	obstmap_poe_obstaculo(o, PAREDE_ID, 0, 470, 640, 10, PAREDE_V);
	/* criando um local onde seria permitido salvar o jogo */
	obstmap_poe_obstaculo(o, SALVAR_ID, 200, 200, 16, 16, SALVAR_V);
	printf("OK\n");

	printf("Salvando o mapa e recarregando...");
	fflush(stdout);
	if(mapa_salva(m, "Teste006.map")) {
		printf("ERRO 4\n");
		fflush(stdout);
		mapa_destroi(m);
		readkey();
		clrscr();
		return 4;
	}
	mapa_destroi(m);
	m = mapa_abre("Teste006.map");
	if(m == NULL) {
		printf("ERRO 5\n");
		fflush(stdout);
		readkey();
		clrscr();
		return 5;
	}
	printf("OK\n");
/*
	printf("Carregando Datafiles...");
	fflush(stdout);
	if(mapa_carrega_lista_df(m)) {
		printf("ERRO 6\n");
		fflush(stdout);
		mapa_destroi(m);
		readkey();
		clrscr();
		return 6;
	}
	printf("OK\n");
*/

	printf("\n*** Aperte qualquer tecla para ver o mapa ***");
	fflush(stdout);
	readkey();
	install_timer();
	set_gfx_mode(GFX_AUTODETECT, SX, SY, 0, 0);
	/* a paleta está no arquivo land.pcx */
	buf = load_bitmap("land.bmp", pal);
	if(buf == NULL) {
		set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
		printf("ERRO 7\n");
		fflush(stdout);
		mapa_destroi(m);
		readkey();
		clrscr();
		return 7;
	}
	set_palette(pal);
	destroy_bitmap(buf);
	buf = create_bitmap(SX, SY);
	clear(buf);
	LOCK_VARIABLE(cont);
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(frame);
	LOCK_FUNCTION(inc_cont);
	LOCK_FUNCTION(fps_cont);
	install_int_ex(inc_cont, BPS_TO_TIMER(FPS));
	install_int_ex(fps_cont, BPS_TO_TIMER(1));

	hline(buf, 0, 29, 319, makecol(0, 0, 255));
	hline(buf, 0, 171, 319, makecol(0, 0, 255));

	/* inicializando a posição do ponto */
	x = 160;
	y = 70;
	cor = makecol(255, 255, 255);
	cont=0;
	while(!key[KEY_ESC]) {
		if(cont) {
			for(; cont; cont--) {
				if(key[KEY_UP]) {
					y--;
					if(mapa_colisao_area(m, x, y, w, h) == PAREDE_V) {
						y++;
					}
					else {
						mapa_muda_visao(m, x-(m->vw/2), y-(m->vh/2));
					}
				}
				if(key[KEY_DOWN]) {
					y++;
					if(mapa_colisao_area(m, x, y, w, h) == PAREDE_V) {
						y--;
					}
					else {
						mapa_muda_visao(m, x-(m->vw/2), y-(m->vh/2));
					}
				}
				if(key[KEY_LEFT]) {
					x--;
					if(mapa_colisao_area(m, x, y, w, h) == PAREDE_V) {
						x++;
					}
					else {
						mapa_muda_visao(m, x-(m->vw/2), y-(m->vh/2));
					}
				}
				if(key[KEY_RIGHT]) {
					x++;
					if(mapa_colisao_area(m, x, y, w, h) == PAREDE_V) {
						x--;
					}
					else {
						mapa_muda_visao(m, x-(m->vw/2), y-(m->vh/2));
					}
				}
			}
			if(key[KEY_F2])
				salvar(m, "t006.map");
			if(key[KEY_F3])
				carregar(m, "t006.map");
			cont = 0;
			mapa_desenha(m, buf, 0, 30);
			textprintf(buf, font, 0, 0, cor, "%2d/%2d", fps, FPS);
			textprintf(buf, font, 0, 190, cor, "X: %3d Y: %3d V: %02X", x, y, mapa_colisao_area(m, x, y, w, h));
			rect(buf, x-m->x1, 30+y-m->y1, x-m->x1+w, 30+y-m->y1+h, cor);
			blit(buf, screen, 0, 0, 0, 0, SX, SY);
			frame++;
		}
	}
	destroy_bitmap(buf);
	allegro_exit();
	mapa_destroi(m);

	printf("X: %d\nY: %d\n", x, y);
	
	return 0;
}

void salvar(MAPA *m, char *nome)
{
	int i;
	i = cont;
	mapa_salva(m, nome);
	cont = i;
}

void carregar(MAPA *m, char *nome)
{
	int i;
	i = cont;
	if(m)
		mapa_destroi(m);
	m = mapa_abre(nome);
	cont = i;
}