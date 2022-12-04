/* Teste #7
		Talvez o teste final, tudo junto agora (finalmente):-)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "allegro.h"
#include "mapa.h"

#define FPS       30				/* frames por segundo */
#define SX			320			/* resolução da tela (SX x SY) */
#define SY			200
#define MX			640			/* tamanho do mapa (MX x MY) */
#define MY			480
#define VX			318			/* campo de visão (VX x VY) */
#define VY			140
#define T007NOME	"Teste007.map" /* nome do arquivo que será criado com o mapa */

#define OBST_ID	0				/* id do layer de obstáculos */
#define PADRAO_V	0x00
#define PAREDE_ID	0
#define PAREDE_V	0xFF

#define TILE1_ID	1				/* id do layer de tiles (BG1) */
#define TILE2_ID	2				/* id do layer de tiles (BG2) */
#define TW        16				/* tamanho dos tiles (TW x TH) */
#define TH        16
#define TCAMPO		1				/* tiles do campo, bordas e águas */
#define TBORDA_C	7
#define TBORDA_CE	6
#define TBORDA_CD	9
#define TBORDA_E	14
#define TBORDA_D	16
#define TBORDA_B	23
#define TBORDA_BE	22
#define TBORDA_BD	25
#define TAGUA1		2
#define TAGUA2		3
#define TAGUA3		4
#define TAGUA4		5
#define TAGUAD		3				/* delay entre as águas da animação */

#define SPR_ID		3				/* id do layer de sprites (SPRITES) */
#define HEROIW		16				/* tamanho do sprite do herói */
#define HEROIH		16
#define CIMA		1				/* direções que o personagem pode estar virado */
#define ESQUERDA	2
#define DIREITA	4
#define BAIXO		8
#define MOVENDO	16				/* indica se está se movendo */
#define HEROI_ID	0				/* id do sprite */
#define SB1			1				/* direção cima (parado e 1º frame da animação) */
#define SB2			2				/* 2º frame */
#define SC1			3				/* parado e 1º frame */
#define SC2			4				/* 2º frame */
#define SE1			5				/* parado e 1º frame */
#define SE2			6				/* 2º frame */
#define SD1			7				/* parado e 1º frame */
#define SD2			8				/* 2º frame */

/* funções para controlar o tempo do jogo */
volatile	int cont=0, fps=0, frame=0;
void inc_cont(void);
void fps_cont(void);

void inc_cont(void) { cont++; } END_OF_FUNCTION(inc_cont);
void fps_cont(void) { fps = frame; frame = 0; } END_OF_FUNCTION(fps_cont);

/* funções para a criação do mapa */

MAPA *teste007_mapa(void);
void teste007_cria_bg1(MAPA *m);
void teste007_cria_bg2(MAPA *m);
void teste007_cria_sprites(MAPA *m);

/* funções para salvar, carregar e tirar foto durante o jogo */

void teste007_salva(MAPA *m);
void teste007_abre(MAPA *m);
void teste007_foto(void);

/* função e variáveis para mostrar mensagens na tela */

char msg_str[80];
int msg_vida;
void enviar_msg(char *str, int vida);
void atualizar_msg(int tempo);
void mostrar_msg(BITMAP *bmp, int x, int y, int cor, int centro);
void erro(int n, char *desc);

/* funções e variáveis para o herói */

int x, y, dir, dir_ant, sprite;
void atualiza_heroi(MAPA *m, int tempo);

/* função principal */

int main(void);


/* abre o mapa salvo anteriormente ou cria um novo */
MAPA *teste007_mapa(void)
{
	MAPA *m;
	m = mapa_abre(T007NOME);
	if(m) {
		x = m->x1+(VX+HEROIW)/2;
		y = m->y1+(VY+HEROIH)/2;
		dir_ant = BAIXO;
		sprite = SB1;
		dir = BAIXO;
		if(mapa_carrega_lista_df(m)) {
			mapa_destroi(m);
			erro(15, "Não foi possível carregar a lista de datafiles\n");
		}
		return m;
	}
	else {
		m = mapa_cria(MX, MY, VX, VY);
		/* cria o layer de obstáculos */
		if(mapa_poe_layer(m, OBST_ID, L_T_OBSTMAP, 0, 0)) {
			mapa_destroi(m);
			erro(1, "Não foi possível criar o layer de obstáculos\n");
		}
		if(mapa_cria_dados(m, OBST_ID, 0, 0, PADRAO_V)) {
			mapa_destroi(m);
			erro(2, "Não foi possível criar os dados dos obstáculos\n");
		}
		((LAYER *)mapa_pega_layer(m, OBST_ID))->status |= L_S_DESATIVADO;
		teste007_cria_bg1(m);
		teste007_cria_bg2(m);
		teste007_cria_sprites(m);
		if(mapa_salva(m, T007NOME)) {
			mapa_destroi(m);
			erro(13, "Não foi possível salvar o mapa\n");
		}
		if(mapa_carrega_lista_df(m)) {
			mapa_destroi(m);
			erro(14, "Não foi possível carregar a lista de datafiles\n");
		}
		return m;
	}
}

/* cria o layer mais ao fundo (águas e terreno) */
void teste007_cria_bg1(MAPA *m)
{
	TILEMAP *t;
	int i, j;
	TILE agua_t[4] = {
		{ TAGUA1, 0, 0 },
		{ TAGUA2, 0, 0 },
		{ TAGUA3, 0, 0 },
		{ TAGUA4, 0, 0 }
	};
	short int agua_d[4] = {
		TAGUAD, TAGUAD, TAGUAD, TAGUAD
	};
	TILE tile;
	OBSTMAP *o;

	/* cria o layer */
	if(mapa_poe_layer(m, TILE1_ID, L_T_TILEMAP, -5, 1)) {
		mapa_destroi(m);
		erro(3, "Não foi possível criar o layer BG1\n");
	}
	/* cria os dados */
	if(mapa_cria_dados(m, TILE1_ID, TW, TH, 0)) {
		mapa_destroi(m);
		erro(4, "Não foi possível criar os dados de BG1\n");
	}
	/* colocando o nome do datafile */
	mapa_poe_dfnome(m, TILE1_ID, "teste.dat");
	/* colocando o tile animado */
	t = (TILEMAP *)mapa_pega_dados(m, TILE1_ID);
	if(tilemap_poe_tani(t, 1, 4)) {
		mapa_destroi(m);
		erro(5, "Não foi possível criar a animação da água\n");
	}
	tilemap_muda_tani(t, 1, 4, agua_d, agua_t);
	/* desliga os efeitos */
	tile_poe_efeito(&tile, 0, 0);
	/* criando o mapa */
	for(i=0; i<t->h; i++) {
		for(j=0; j<t->w; j++) {
			if(i==0 || i==t->h-1 || j==0 || j==t->w-1) {
				tile.i = TAGUA1;
				tilemap_poe_tpani(t, j, i, 1, 0, 0);
			}
			else {
				tile.i = TCAMPO;
			}
			tilemap_poe_tile(t, j, i, &tile);
		}
	}
	/* criando os obstáculos */
	o = (OBSTMAP *)mapa_pega_dados(m, OBST_ID);
	obstmap_poe_obstaculo(o, PAREDE_ID, 0, 0, t->w*TW, TH, PAREDE_V);
	obstmap_poe_obstaculo(o, PAREDE_ID, 0, 0, TW, t->h*TH, PAREDE_V);
	obstmap_poe_obstaculo(o, PAREDE_ID, (t->w*TW)-TW, 0, TW, t->h*TH, PAREDE_V);
	obstmap_poe_obstaculo(o, PAREDE_ID, 0, (t->h*TH)-TH, t->w*TW, TH, PAREDE_V);
}

/* cria o 2º layer (bordas da ilha) */
void teste007_cria_bg2(MAPA *m)
{
	TILEMAP *t;
	int i, j;
	TILE tile;
	
	/* criando layer e dados */
	if(mapa_poe_layer(m, TILE2_ID, L_T_TILEMAP, -4, 1)) {
		mapa_destroi(m);
		erro(6, "Não foi possível criar o layer BG2\n");
	}
	if(mapa_cria_dados(m, TILE2_ID, TW, TH, 0)) {
		mapa_destroi(m);
		erro(7, "Não foi possível criar o tilemap do BG2\n");
	}
	/* colocando o nome do datafile */
	mapa_poe_dfnome(m, TILE2_ID, "teste.dat");
	t = (TILEMAP *)mapa_pega_dados(m, TILE2_ID);
	tile_poe_efeito(&tile, 0, 0);
	/* cria a borda do mapa */
	for(i=0; i<t->h; i++) {
		for(j=0; j<t->w; j++) {
			if(i==0 && j==0)
				tile.i = TBORDA_CE;
			else if(i==0 && j==t->w-1)
				tile.i = TBORDA_CD;
			else if(i==0)
				tile.i = TBORDA_C;
			else if(i==t->h-1 && j==0)
				tile.i = TBORDA_BE;
			else if(i==t->h-1 && j==t->w-1)
				tile.i = TBORDA_BD;
			else if(i==t->h-1)
				tile.i = TBORDA_B;
			else if(j==0)
				tile.i = TBORDA_E;
			else if(j==t->w-1)
				tile.i = TBORDA_D;
			else
				tile.i = -1;
			tilemap_poe_tile(t, j, i, &tile);
		}
	}
}

/* cria o layer dos sprites (personagem) */
void teste007_cria_sprites(MAPA *m)
{
	SPRITEMAP *sm;

	/* criando layer e dados */
	if(mapa_poe_layer(m, SPR_ID, L_T_SPRITEMAP, -3, 1)) {
		mapa_destroi(m);
		erro(8, "Não foi possível criar o layer dos sprites\n");
	}
	if(mapa_cria_dados(m, SPR_ID, 0, 0, 0)) {
		mapa_destroi(m);
		erro(9, "Não foi possível criar os sprites\n");
	}
	/* colocando o nome do datafile */
	mapa_poe_dfnome(m, SPR_ID, "t007spr.dat");
	/* cria o sprite para o personagem */
	sm = (SPRITEMAP *)mapa_pega_dados(m, SPR_ID);
	x = 50;
	y = 50;
	dir_ant = BAIXO;
	dir = BAIXO;
	sprite = SB1;
	spritemap_poe_sprite(sm, HEROI_ID, x, y, HEROIW, HEROIH, 2, 0);
	spritemap_muda_quadro_sprite(sm, HEROI_ID, 0, SB1, 0, 0, 0, 0);
	spritemap_muda_quadro_sprite(sm, HEROI_ID, 1, SB2, 5, 0, 0, 0);
	/* centraliza a visão no herói */
	mapa_muda_visao(m, x-(VX+HEROIW)/2, y-(VY+HEROIH)/2);
}

/* salva o jogo durante a demonstração */
void teste007_salva(MAPA *m)
{
	int t;
	t = cont;
	if(mapa_salva(m, T007NOME)) {
		mapa_destroi(m);
		erro(10, "Não foi possível salvar o mapa\n");
	}
	enviar_msg("Jogo Salvo", 60);
	cont = t;
}

/* abre durante a demonstração */
void teste007_abre(MAPA *m)
{
	int t;
	t = cont;
	if(m)
		mapa_destroi(m);
	m = mapa_abre(T007NOME);
	if(m == NULL) {
		erro(11, "Não foi possível abrir o mapa\n");
	}
	if(mapa_carrega_lista_df(m)) {
		mapa_destroi(m);
		erro(12, "Não foi possível carregar a lista de datafiles\n");
	}
	x = m->x1+(VX+HEROIW)/2;
	y = m->y1+(VY+HEROIH)/2;
	dir_ant = BAIXO;
	sprite = SB1;
	dir = BAIXO;
	enviar_msg("Jogo Carregado", 60);
	cont = t;
}

/* salva a tela num arquivo */
void teste007_foto()
{
	BITMAP *bmp;
	PALETTE pal;
	int t;
	t = cont;
	get_palette(pal);
	bmp = create_sub_bitmap(screen, 0, 0, SCREEN_W, SCREEN_H);
	save_bitmap("Teste007.pcx", bmp, pal);
	destroy_bitmap(bmp);
	enviar_msg("Tela salva em Teste007.pcx", 60);
	cont = t;
}

char msg_str[80];
int msg_vida;
/* copia a mensagem para a variável global para mostrá-la depois */
void enviar_msg(char *str, int vida)
{
	strcpy(msg_str, str);
	msg_vida = vida;
}

/* atualiza a mensagem (diminui seu tempo de vida) */
void atualizar_msg(int tempo)
{
	if(msg_vida > 0)
		msg_vida -= tempo;
}

/* mostra a mensagem se ela tem tempo de vida ainda */
void mostrar_msg(BITMAP *bmp, int x, int y, int cor, int centro)
{
	if(msg_vida > 0) {
		if(centro)
			textout_centre(bmp, font, msg_str, x, y, cor);
		else
			textout(bmp, font, msg_str, x, y, cor);
	}
}

/* sai para o modo texto, mostra a mensagem e sai do programa */
void erro(int n, char *desc)
{
	allegro_exit();
	clrscr();
	printf("ERRO%d!\n%s", n, desc);
	exit(n);
}

/* atualiza o sprite do herói e sua posição */
void atualiza_heroi(MAPA *m, int tempo)
{
	SPRITEMAP *sm;
	SPRITE *s;
	int w = HEROIW, h = HEROIH;
	int i;
	if(tempo <= 0)
		return;
	for(i=0, dir=0; i<tempo; i++) {
		if(key[KEY_UP]) {
			y--;
			if(mapa_colisao_area(m, x, y, w, h) == PAREDE_V)
				y++;
			else
				dir |= CIMA;
		}
		if(key[KEY_DOWN]) {
			y++;
			if(mapa_colisao_area(m, x, y, w, h) == PAREDE_V)
				y--;
			else
				dir |= BAIXO;
		}
		if(key[KEY_LEFT]) {
			x--;
			if(mapa_colisao_area(m, x, y, w, h) == PAREDE_V)
				x++;
			else
				dir |= ESQUERDA;
		}
		if(key[KEY_RIGHT]) {
			x++;
			if(mapa_colisao_area(m, x, y, w, h) == PAREDE_V)
				x--;
			else
				dir |= DIREITA;
		}
	}
	/* se cima e baixo ou esquerda e direita são apertados ao mesmo tempo, não move */
	if( (dir&CIMA) && (dir&BAIXO) )
		dir ^= (CIMA | BAIXO);
	if( (dir&ESQUERDA) && (dir&DIREITA) )
		dir ^= (ESQUERDA | DIREITA);
	if(dir)
		dir |= MOVENDO;
	/* põe a câmera em cima do herói */
	mapa_muda_visao(m, x-(VX+HEROIW)/2, y-(VY+HEROIH)/2);
	sm = mapa_pega_dados(m, SPR_ID);
	s = spritemap_pega_sprite(sm, HEROI_ID);
	s->xp = x;
	s->yp = y;

	/* se não move para lugar nenhum fica na mesma direção e parado */
	if(dir == 0) {
		if(dir_ant & MOVENDO) {
			/* para a animação, mantendo o sprite e como o outro sprite é sprite+1
			   basta colocar como no exemplo abaixo */
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 0, sprite, 0, 0, 0, 0);
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 1, sprite+1, 0, 0, 0, 0);
			dir_ant ^= MOVENDO;
		}
		return;
	}
	/* se move e direção é diferente da anterior, vê o sprite para ser colocado */
	else if(dir != dir_ant) {
		if(dir & ESQUERDA) {
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 0, SE1, 5, 0, 0, 0);
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 1, SE2, 5, 0, 0, 0);
			sprite = SE1;
		}
		else if(dir & DIREITA) {
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 0, SD1, 5, 0, 0, 0);
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 1, SD2, 5, 0, 0, 0);
			sprite = SD1;
		}
		if(dir & CIMA) {
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 0, SC1, 5, 0, 0, 0);
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 1, SC2, 5, 0, 0, 0);
			sprite = SC1;
		}
		else if(dir & BAIXO) {
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 0, SB1, 5, 0, 0, 0);
			spritemap_muda_quadro_sprite(sm, HEROI_ID, 1, SB2, 5, 0, 0, 0);
			sprite = SB1;
		}
		dir_ant = dir;
	}
}


/* chama as outras funções e anima a tela */
int main(void)
{
	MAPA *m;
	int t;
	BITMAP *buf;
	PALETTE *pal;

	allegro_init();
	install_keyboard();
	/* mostra instruções */
	clrscr();
	printf("Instruções:\n\n");
	printf("  Setas direcionais: movimenta o personagem\n");
	printf("  F2: salva no local atual\n");
	printf("  F3: carrega o jogo salvo anteriormente\n");
	printf("  F12: tira uma foto da tela e salva em arquivo\n");
	printf("  ESC: sair da demonstração\n");
	fflush(stdout);
	readkey();
	install_timer();
	set_gfx_mode(GFX_AUTODETECT, SX, SY, 0, 0);
	buf = create_bitmap(SX, SY);
	clear(buf);
	/* é chamado depois para os tiles e os sprites serem carregados corretamente */
	m = teste007_mapa();
	/* a paleta está no primeiro item do datafile dos tiles e dos sprites */
	pal = (PALETTE *)((DATAFILE *)mapa_pega_df(m, TILE1_ID, 0))[0].dat;
	set_palette(*pal);
	LOCK_VARIABLE(cont);
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(frame);
	LOCK_FUNCTION(inc_cont);
	LOCK_FUNCTION(fps_cont);
	install_int_ex(inc_cont, BPS_TO_TIMER(FPS));
	install_int_ex(fps_cont, BPS_TO_TIMER(1));
	enviar_msg("", 0);

	mapa_desenha(m, buf, (SX-VX)>>1, (SY-VY)>>1);
	blit(buf, screen, 0, 0, 0, 0, SX, SY);
	fps = 0;
	cont = 0;
	while(!key[KEY_ESC]) {
		if(cont) {
			t = cont;
			mapa_atualiza(m, t);
			atualiza_heroi(m, t<<1);	/* t*2 */
			atualizar_msg(t);
			/* verifica se deve salvar ou carregar o jogo */
			if(key[KEY_F2])
				teste007_salva(m);
			if(key[KEY_F3])
				teste007_abre(m);
			if(key[KEY_F12])
				teste007_foto();
			cont = 0;
			mapa_desenha(m, buf, (SX-VX)>>1, (SY-VY)>>1);	/* n/2 */
			mostrar_msg(buf, SX>>1, 5, 255, 1);
			/* mostra a taxa de frames por segundo */
			textprintf(buf, font, 0, 0, 255, "%2d/%2d", fps, FPS);
			blit(buf, screen, 0, 0, 0, 0, SX, SY);
			/* limpa a mensagem anterior */
			rectfill(buf, 0, 5, SX, 13, 0);
			frame++;
		}
	}
	destroy_bitmap(buf);
	mapa_destroi(m);
	allegro_exit();
	return 0;
}