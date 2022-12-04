/* Teste #5
		Testa os múltiplos datafiles e spritemap
*/

#include <stdio.h>
#include "allegro.h"
#include "mapa.h"

#define SPR_ID		1
#define FPS       30
#define SX			320
#define SY			200

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
	SPRITEMAP *sm;
	PALETTE *pal;
	BITMAP *buf;
	int i;

	allegro_init();
	install_keyboard();

	printf("Criando mapa 640x480 e associando um datafile ao layer...");
	m = mapa_cria(640, 480, 110, 100);
	mapa_poe_layer(m, SPR_ID, L_T_SPRITEMAP, 0, 1);
	mapa_cria_dados(m, SPR_ID, 0, 0, 0);
	if(mapa_poe_dfnome(m, SPR_ID, "testespr.dat")) {
		printf("ERRO 1!\n");
		mapa_destroi(m);
		return 1;
	}
	printf("OK\n");

	printf("Colocando sprites...");
	fflush(stdout);
	sm = mapa_pega_dados(m, SPR_ID);
	if(spritemap_poe_sprite(sm, 1, 10, 10, 88, 90, 3, 0)) {
		printf("ERRO 2!\n");
		mapa_destroi(m);
		return 1;
	}
	/* Mario andando */
	spritemap_muda_quadro_sprite(sm, 1, 0, 2, 8, 0, 0, 0);
	spritemap_muda_quadro_sprite(sm, 1, 1, 1, 8, 0, 0, 0);
	spritemap_muda_quadro_sprite(sm, 1, 2, 3, 8, 0, 0, 0);
	if(spritemap_poe_sprite(sm, 2, 10, 110, 88, 90, 1, 0)) {
		printf("ERRO 2!\n");
		mapa_destroi(m);
		return 1;
	}
	/* Mario de fogo */
	spritemap_muda_quadro_sprite(sm, 2, 0, 4, 0, 0, 0, 0);//itofix(100));
	printf("OK\n");

	printf("Salvando e carregando de novo o mapa...");
	if(mapa_salva(m, "Teste005.map")) {
		mapa_destroi(m);
		printf("ERRO 3!\n");
		return 1;
	}
	mapa_destroi(m);
	m = mapa_abre("Teste005.map");
	if(m == NULL) {
		printf("ERRO 4!\n");
		return 1;
	}
	printf("OK\n");

	printf("Carregando Datafiles...");
	fflush(stdout);
	if(mapa_carrega_lista_df(m)) {
		printf("ERRO 5!\n");
		mapa_destroi(m);
		return 1;
	}
	printf("OK\n");

	for(i=0; i<100; i++) {
		mapa_atualiza(m, 1);
	}

	printf("\nAperte qualquer tecla para ver o mapa.\n");
	fflush(stdout);
	readkey();
	install_timer();
	set_gfx_mode(GFX_AUTODETECT, SX, SY, 0, 0);
	mapa_altera_campo_visao(m, SX, SY);
	buf = create_bitmap(SX, SY);
	clear(buf);
	/* a paleta está no primeiro item do datafile */
	pal = ((DATAFILE *)mapa_pega_df(m, SPR_ID, 0))[0].dat;
	set_palette(*pal);
	LOCK_VARIABLE(cont);
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(frame);
	LOCK_FUNCTION(inc_cont);
	LOCK_FUNCTION(fps_cont);
	install_int_ex(inc_cont, BPS_TO_TIMER(FPS));
	install_int_ex(fps_cont, BPS_TO_TIMER(1));
	cont=0;
	while(!key[KEY_ESC]) {
		if(cont) {
			mapa_atualiza(m, cont);
			if(key[KEY_UP])
				mapa_move_visao(m, 0, -cont<<1);
			if(key[KEY_DOWN])
				mapa_move_visao(m, 0, cont<<1);
			if(key[KEY_LEFT])
				mapa_move_visao(m, -cont<<1, 0);
			if(key[KEY_RIGHT])
				mapa_move_visao(m, cont<<1, 0);
			cont = 0;
			mapa_desenha(m, buf, 0, 0);
			textprintf(buf, font, 0, 0, 255, "%2d/%2d", fps, FPS);
			blit(buf, screen, 0, 0, 0, 0, SX, SY);
			frame++;
		}
	}
	destroy_bitmap(buf);
	allegro_exit();
	mapa_destroi(m);
	
	return 0;
}