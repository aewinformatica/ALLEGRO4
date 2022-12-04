#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	<allegro.h>
#include	"mapa.h"
#define randomize()     srand((unsigned)time(NULL))

#define TW        16
#define TH        16
#define ISOM      0
#define FPS       30
#define L1        1
#define L2        2
#define TA1_ID    1
#define TA1_N     4

int SX=320, SY=240, GFX=0;
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


int main()
{
	MAPA *m, *m2;
	TILEMAP *t;
	TILE tile, *ptile;
	short int delays[TA1_N] = {
		3, 3, 3, 3
	};
	TILE tiles[TA1_N] = {
		{ 2, 0, 0 },
		{ 3, 0, 0 },
		{ 4, 0, 0 },
		{ 5, 0, 0 }
	};
	BITMAP *buf;
	PALETTE *pal;
   RGB_MAP rgb_table;
   COLOR_MAP trans_table;
	int i, j;
	char arq[] = "teste.dat";

	randomize();
	allegro_init();
	install_keyboard();

	printf("\n\nAlocando mem¢ria para o mapa...");
	fflush(stdout);
	m = mapa_cria(640, 480, SX, SY);
	if(!m) {
		puts("\nNÆo foi poss¡vel criar o mapa\n");
		return 1;
	}
	printf("OK\n");

	printf("Criando Layers...");
	if(mapa_poe_layer(m, L1, L_T_TILEMAP, 0, 1)) {
		mapa_destroi(m);
		puts("\nERRO! Não foi possível alocar memória para o layer.\n");
		return 1;
	}
	if(mapa_poe_layer(m, L2, L_T_TILEMAP, 1, 1)) {
		mapa_destroi(m);
		puts("\nERRO! Não foi possível alocar memória para o layer.\n");
		return 1;
	}
	printf("OK\n");
	fflush(stdout);

	printf("Alocando memória para o tilemap...");
	if(mapa_cria_dados(m, L1, TW, TH, ISOM)) {
		mapa_destroi(m);
		puts("ERRO! Não foi possível alocar memória para o tilemap.\n");
		return 1;
	}
	if(mapa_cria_dados(m, L2, TW, TH, ISOM)) {
		mapa_destroi(m);
		puts("ERRO! Não foi possível alocar memória para o tilemap.\n");
		return 1;
	}
	printf("OK\n");
	fflush(stdout);

	printf("Colocando o nome do arquivo de dados %s...", arq);
	mapa_poe_dfnome(m, L1, arq);
	mapa_poe_dfnome(m, L2, arq);
	printf("OK\n");
	fflush(stdout);

	printf("Criando mapa...");
	tile_poe_efeito(&tile, 0, 0);
	ptile = &tile;
	t = (TILEMAP *)mapa_pega_dados(m, L1);
	if(tilemap_poe_tani(t, TA1_ID, TA1_N)) {
		mapa_destroi(m);
		puts("\nERRO! Não foi possível colocar a animação\n");
		return 1;
	}
	tilemap_muda_tani(t, TA1_ID, TA1_N, delays, tiles);
	for(i=0; i<t->h; i++) {
		for(j=0; j<t->w; j++) {
			tile.i = 2;
			tilemap_poe_tile(t, j, i, ptile);
			if(tilemap_poe_tpani(t, j, i, TA1_ID, j%4, 0)) {
				mapa_destroi(m);
				puts("\nNão foi possível colocar a posição da animação\n");
				return 1;
			}
		}
	}
	t = (TILEMAP *)mapa_pega_dados(m, L2);
	for(i=0; i<t->h; i++) {
		for(j=0; j<t->w; j++) {
			if(i==0 && j==0)
				tile.i = 20;
			else if(i==0 && j==t->w-1)
				tile.i = 19;
			else if(i==0)
				tile.i = 23;
			else if(i==t->h-1 && j==0)
				tile.i = 12;
			else if(i==t->h-1 && j==t->w-1)
				tile.i = 10;
			else if(i==t->h-1)
				tile.i = 8;
			else if(j==0)
				tile.i = 17;
			else if(j==t->w-1)
				tile.i = 14;
			else if(i==3 && j==3) {
				tile.i = 6;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}
			else if(i==3 && j==13) {
				tile.i = 9;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}
			else if(i==3 && j>3  && j<13) {
				tile.i = 7;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}
			else if(i==4 && j==3) {
				tile.i = 14;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}
			else if(i==4 && j==13) {
				tile.i = 16;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}
			else if(i==4 && j>3 && j<13) {
				tile.i = 39;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}
			else if(i==5 && j==3) {
				tile.i = 22;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}
			else if(i==5 && j==13) {
				tile.i = 25;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}
			else if(i==5 && j>3 && j<13) {
				tile.i = 23;
				tile_poe_efeito(ptile, T_E_TRANS, 0);
			}

			else if(i==6 && j==3) {
				tile.i = 6;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==6 && j==13) {
				tile.i = 9;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==6 && j>3  && j<13) {
				tile.i = 7;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==7 && j==3) {
				tile.i = 14;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==7 && j==13) {
				tile.i = 16;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==7 && j>3 && j<13) {
				tile.i = 39;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==8 && j==3) {
				tile.i = 22;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==8 && j==13) {
				tile.i = 25;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==8 && j>3 && j<13) {
				tile.i = 23;
				tile_poe_efeito(ptile, T_E_PINTA, 26);
			}
			else if(i==9 && j==3) {
				tile.i = 6;
			}
			else if(i==9 && j==13) {
				tile.i = 9;
			}
			else if(i==9 && j>3  && j<13) {
				tile.i = 7;
			}
			else if(i==10 && j==3) {
				tile.i = 14;
			}
			else if(i==10 && j==13) {
				tile.i = 16;
			}
			else if(i==10 && j>3 && j<13) {
				tile.i = 39;
			}
			else if(i==11 && j==3) {
				tile.i = 22;
			}
			else if(i==11 && j==13) {
				tile.i = 25;
			}
			else if(i==11 && j>3 && j<13) {
				tile.i = 23;
			}
			else
				tile.i = 0;
			tilemap_poe_tile(t, j, i, ptile);
			tile_poe_efeito(ptile, 0, 0);
		}
	}
	printf("OK\n");
	printf("Salvando o mapa...");
	fflush(stdout);
	if(mapa_salva(m, "teste004.map")) {
		mapa_destroi(m);
		puts("\nERRO! Não foi possível salvar o mapa.\n");
		return 1;
	}
	printf("OK\n");
	mapa_destroi(m);
	printf("Abrindo o mapa salvo em outra variável...");
	fflush(stdout);
	m2 = mapa_abre("teste004.map");
	if(!m2) {
		puts("\nERRO! Não consegui abrir o arquivo teste004.map\n");
		return 1;
	}
	printf("OK\n");
	printf("Carregando os datafiles...");
	if(mapa_carrega_lista_df(m2)) {
		puts("\nERRO! Não foi possível abrir o df.\n");
		mapa_destroi(m2);
		return 1;
	}
	printf("\nAperte qualquer tecla para ver o mapa.\n");
	fflush(stdout);
	readkey();

	install_timer();
	set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
	set_palette(desktop_palette);
	if(!gfx_mode_select(&GFX, &SX, &SY)) {
		mapa_destroi(m2);
		return 1;
	}
	set_gfx_mode(GFX, SX, SY, 0, 0);
	mapa_altera_campo_visao(m2, SX, SY);
	buf = create_bitmap(SX, SY);
	clear(buf);
	/* a paleta está no primeiro item do datafile */
	pal = ((DATAFILE *)mapa_pega_df(m2, L1, 0))[0].dat;
	set_palette(*pal);
   create_rgb_table(&rgb_table, *pal, NULL);
   rgb_map = &rgb_table;
   create_trans_table(&trans_table, *pal, 128, 128, 128, NULL);
   color_map = &trans_table;
	LOCK_VARIABLE(cont);
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(frame);
	LOCK_FUNCTION(inc_cont);
	LOCK_FUNCTION(fps_cont);
	install_int_ex(inc_cont, BPS_TO_TIMER(FPS));
	install_int_ex(fps_cont, BPS_TO_TIMER(1));

	while(!key[KEY_ESC]) {
		if(cont) {
			mapa_atualiza(m2, cont);
			if(key[KEY_UP])
				mapa_move_visao(m2, 0, -cont<<1);
			if(key[KEY_DOWN])
				mapa_move_visao(m2, 0, cont<<1);
			if(key[KEY_LEFT])
				mapa_move_visao(m2, -cont<<1, 0);
			if(key[KEY_RIGHT])
				mapa_move_visao(m2, cont<<1, 0);
			cont = 0;
			mapa_desenha(m2, buf, 0, 0);
			textprintf(buf, font, 0, 0, 255, "%2d/%2d", fps, FPS);
			blit(buf, screen, 0, 0, 0, 0, SX, SY);
			frame++;
		}
	}
	destroy_bitmap(buf);
	allegro_exit();
	mapa_destroi(m2);
	return 0;
}