/*		Este tem que sair.........
		Esta biblioteca dever�, se tudo correr bem, de fornecer rotinas para manipular um mapa de
	jogo, podendo ser de tiles ou de sprites, e ter� que detectar colis�es tamb�m, que ser� usado
	para isso um outro mapa.
		Ser� poss�vel tamb�m aplicar alguns efeitos em tiles/sprites isoladamente ou no layer
	inteiro.
		Para isso usarei uma estrutura principal, chamada MAPA, e todas as outras fun��es ir�o
	receb�-la e manipul�-la. Dentro dela, haver� outra estrutura, LAYER, que ir� servir para um
	mapa de m�ltiplas camadas. Cada layer poder� ser de tr�s tipos, TILES, SPRITES ou OBST�CULOS,
	usados para definir como desenhar o layer e outras coisas.
		Espero que desta vez eu consiga acabar isso, e que as rotinas de salvamento e carregamento
	funcionem como eu espero.
*/

#ifndef MAPA_H
#define MAPA_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************
 * Defini��o de constantes, tipos, etc... *
 ******************************************/

struct MAPA;

struct LAYER;
#define DFNOME_TAM			100

struct TILEMAP;
struct TILE;
struct TILE_ANI;
struct TILE_PANI;

struct SPRITEMAP;
struct SPRITE;

struct OBSTMAP;
struct OBSTACULO;

/***********************/
/*	Estrutura principal */

/*
	Este � o dado onde todos os outros estar�o e o programador s� precisar�
	trabalhar com este (normalmente :-)) para apresentar os gr�ficos.
	Pode ser salvo em arquivo, criado em tempo real, posicionar a c�mera, etc.
*/
typedef struct MAPA {
	short int nlayers;			/* n� de layers */
	struct LAYER *layers;		/* lista encadeada de layers */
	int x1, y1, vw, vh;			/* tamanho do campo de vis�o da c�mera */
	int w, h;						/* tamanho do mapa */
	unsigned char df_carregado;/* indica se os df's dos layers est�o carregados */
} MAPA;

/***********************/


/**************************/
/*	Estrutura para o layer */

/*
	Este tipo de dado apenas define a forma como o dado real deve ser manipulado
	e dizendo quando. Serve para separar a imagem em camadas distintas (fundo,
	pessoas, constru��es, efeitos e texto).
	N�o se pode misturar tiles com sprites ou obst�culos num mesmo layer, pois
	esta habilidade n�o � suportada.
*/
typedef struct LAYER {
	short int id;					/* n�mero de identifica��o do layer */
	int w, h;						/* tamanho do layer */
	short int prioridade;		/* prioridade do layer */
	unsigned char tipo;			/* tipo do layer */
	unsigned char status;		/* status atual do layer */

	DATAFILE **df;					/* vetor de datafiles usado pelo layer */
	char **dfnome;					/* nomes dos arquivos dos datafiles */
	short int ndf, prox_df;		/* n� de datafiles usado e posi��o do pr�ximo a ser colocado*/

	union {
		struct TILEMAP *dat_t;
		struct SPRITEMAP *dat_s;
		struct OBSTMAP *dat_o;
	} dat;							/* diferentes tipos de dados poss�veis */
	struct LAYER *prox, *ant;
} LAYER;

/*	Os tipos de layers poss�veis */
#define L_T_TILEMAP			0
#define L_T_SPRITEMAP		1
#define L_T_OBSTMAP			2

/*	Os status de layers poss�veis */
#define L_S_DESATIVADO		1		/* o layer deve ser ignorado */

/**************************/


/****************************/
/*	Estrutura para o tilemap */

/*
	Este � o dado que ser� usado para se fazer tilemaps, normalmente para ser usado
	como fundo, j� que os tiles n�o podem mudar de posi��o. Pode-se, entretanto,
	obter-se anima��o e aplicar alguns efeitos individualmente.
	As anima��es s�o armazenadas separadamente e uma outra estrutura de dados �
	usada para saber qual a posi��o das anima��es, repetindo-as sem gastar mais
	mem�ria.
*/
typedef struct TILEMAP {
	short int w, h;					/* largura e altura, em n� de tiles */
	short int tw, th;					/* largura e altura dos tiles, em pixels */
	struct TILE *tiles;				/* vetor para armazenar os tiles em sequ�ncia */
	int ntani;							/* n� de tiles animados */
	struct TILE_ANI *tiles_ani;	/* tiles animados */
	int ntposani;						/* n� de tiles do mapa que s�o animados */
	struct TILE_PANI *tiles_pani;	/* tiles do mapa que s�o animados */
	unsigned char isom;				/* diz se � um mapa com perspectiva isom�trica ou n�o */
} TILEMAP;

/*	Estrutura para cada tile */
typedef struct TILE {
	short int i;						/* �ndice do tile no datafile do layer */
	unsigned char efeito;			/* para os efeitos especiais */
	int c;								/* para ser usado no efeito de pintura */
} TILE;

/*	Efeitos que os tiles podem ter */
#define T_E_VERT			1			/* efeito espelho vertical */
#define T_E_HOR			2			/* efeito espelho horizontal */
#define T_E_DIAG			3			/* efeito espelho diagonal */
#define T_E_TRANS			4			/* efeito de transpar�ncia (50%) */
#define T_E_PINTA			5			/* efeito de pintar o sprite com outra cor */

/*	Estrutura de cada tile animado */
typedef struct TILE_ANI {
	short int id;						/* identifica��o do tile animado */
	short int n;						/* n�mero de quadros do tile animado */
	short int *delays;				/* espera entre os quadros */
	struct TILE *tiles;				/* cada quadro da anima��o */
	struct TILE_ANI *ant, *prox;	/* usado para a lista */
} TILE_ANI;

/*	Estrutura para as posi��es dos tiles animados */
typedef struct TILE_PANI {
	short int x, y;					/* posi��o x - y do tile animado */
	short int tatual;					/* tile atual */
	short int tempo;					/* tempo decorrido */
	short int id_ani;					/* identifica��o do tile animado usado nesta posi��o */
	struct TILE_PANI *ant, *prox;	/* usado na lista */
} TILE_PANI;

/****************************/


/*******************************/
/*	Estruturas para o spritemap */

/*
	Este dado armazena um mapa de sprites, eles podem ser movidos para qualquer
	ponto da tela, diferente dos tiles, que possuem posi��es fixas, e podem ser
	aplicados diversos efeitos.
	Tamb�m podem ter anima��es, mas eles n�o s�o armazenados separadamente, e
	podem usar mais de um datafile (um para cada personagem do jogo, por exemplo).
*/
typedef struct SPRITEMAP {
	int w, h;							/* tamanho do mapa, em pixels */
	short int n_sprites;				/* n�mero de sprites */
	struct SPRITE *lista;			/* lista contendo os sprites */
} SPRITEMAP;

/*	Estrutura para cada sprite */
typedef struct SPRITE {
	short int id;						/* id do sprite, n�o pode repetir */
	short int xp, yp;					/* posi��o nos eixos x e y */
	short int w, h;					/* tamanho do sprite */
	short int n_quadros;				/* n�meros de quadros */
	short int *quadros;				/* vetor contendo os �ndices dos quadros */
	short int *delays;				/* vetor contendo o tempo de espera entre cada quadro */
	short int q_atual;				/* quadro atual */
	short int tempo;					/* tempo decorrido */
	unsigned char *efeito;			/* para os efeitos especiais */
	int *c;								/* para os efeitos de pintura */
	fixed *angulo;						/* para o efeito de rota��o */
	unsigned char status;			/* para saber se est� vis�vel ou n�o */
	short int ndf;						/* �ndice do df que cont�m os quadros */
	struct SPRITE *prox, *ant;		/* para a lista duplamente encadeada */
} SPRITE;

/*	Efeitos que os sprites podem ter */
#define S_E_VERT			0x01		/* efeito espelho vertical */
#define S_E_HOR			0x02		/* efeito espelho horizontal */
#define S_E_DIAG			0x04		/* efeito espelho diagonal */
#define S_E_TRANS			0x08		/* efeito de transpar�ncia */
#define S_E_PINTA			0x10		/* efeito de pintar o sprite com outra cor */
#define S_E_ROTACAO		0x20		/* efeito de rota��o */
/*	Status dos sprites */
#define S_S_NORMAL		0			/* normal */
#define S_S_DESATIVADO	1			/* desativado */

/*******************************/

/****************************************/
/*	Estruturas para o mapa de obst�culos */

/*
	Este dado serve para saber, por exemplo, se no ponto onde o her�i est� pode
	ser gravado, ou � uma parede intranspon�vel ou precisa saber nadar para
	atravess�-lo, entre outras coisas.
	S� pode existir um obstmap desses por mapa.
*/

typedef struct OBSTMAP {
	short int w, h;					/* tamanho do mapa em pixels*/
	unsigned char valor;				/* se o ponto est� fora de todos os obst�culos */
											/* este ser� o valor daquele ponto */
	short int no;						/* armazena o n�mero de obst�culos */
	struct OBSTACULO *lista;		/* in�cio da lista de obst�culos */
} OBSTMAP;

typedef struct OBSTACULO {
	short int id;						/* id do obst�culo, pode repetir */
	unsigned char valor;				/* valor dos pontos dentro da �rea */
	short int x, y, w, h;			/* �rea do obst�culo */
	struct OBSTACULO *prox, *ant;	/* pr�ximo e anterior na lista */
} OBSTACULO;
/****************************************/


/***********************
 * Fun��es para o mapa *
 ***********************/

MAPA *mapa_cria(int w, int h, int vw, int vh);
void mapa_destroi(MAPA *m);
int mapa_poe_layer(MAPA *m, short int id, unsigned char tipo, short int prioridade, short int ndf);
LAYER	*mapa_pega_layer(MAPA *m, short int id);
int mapa_cria_dados(MAPA *m, short int id, short int tw, short int th, unsigned char v);
int mapa_poe_dados(MAPA *m, short int id, void *d);
void *mapa_pega_dados(MAPA *m, short int id);
void mapa_muda_prioridade_layer(MAPA *m, short int id, short int np);
void mapa_destroi_layer(MAPA *m, short int id);
int mapa_salva(MAPA *m, char *arquivo);
MAPA *mapa_abre(char *arquivo);
void mapa_atualiza(MAPA *m, int tempo);
void mapa_desenha(MAPA *m, BITMAP *bmp, int xoff, int yoff);
void mapa_altera_campo_visao(MAPA *m, int nw, int nh);
void mapa_muda_visao(MAPA *m, int x1, int y1);
void mapa_move_visao(MAPA *m, int x1, int y1);
int mapa_carrega_lista_df(MAPA *m);
int mapa_poe_dfnome(MAPA *m, short int id, char *arq);
DATAFILE	*mapa_pega_df(MAPA *m, short int id, short int ndf);
int mapa_existe_obstmap(MAPA *m);
unsigned char mapa_colisao_ponto(MAPA *m, short int x, short int y);
unsigned char mapa_colisao_area(MAPA *m, short int x, short int y, short int w, short int h);


/************************
 * Fun��es para o layer *
 ************************/

LAYER	*layer_cria(short int id, unsigned char tipo, short int prioridade, int w, int h, short int ndf);
void layer_destroi(LAYER *l);
void layer_adiciona(LAYER *l, LAYER **lini);
void layer_deleta(LAYER *l, LAYER **lini);

int layer_cria_dados(LAYER *l, int tw, int th, unsigned char v);
void layer_poe_dados(LAYER *l, void *d);
void *layer_pega_dados(LAYER *l);
void layer_destroi_dados(LAYER *l);

void layer_muda_prioridade(LAYER **lini, LAYER *l, short int p);
short	int layer_pega_prioridade(LAYER *l);

short	int layer_pega_id(LAYER *l);
LAYER	*layer_procura_id(LAYER **lini, short int id);

unsigned	char layer_pega_tipo(LAYER *l);
void layer_muda_tipo(LAYER *l, unsigned char tipo);

void layer_atualiza(LAYER *l, int tempo);
void layer_desenha(LAYER *l, BITMAP *bmp, int xoff, int yoff, int x1, int y1,
						 int x2, int y2);

int layer_salva(LAYER *l, PACKFILE *f);
LAYER	*layer_abre(PACKFILE *f);

int layer_carrega_lista_df(LAYER **lini);
void layer_descarrega_lista_df(LAYER **lini);
int layer_poe_dfnome(LAYER *l, char *arq);
char *layer_pega_dfnome(LAYER *l, short int ndf);
int layer_compara_dfnome(LAYER *l, char *arq);
DATAFILE	*layer_pega_datafile(LAYER *l, short int ndf);


/**************************
 * Fun��es para o tilemap *
 **************************/

TILEMAP *tilemap_cria(int w, int h, int tw, int th, unsigned char isom);
void tilemap_destroi(TILEMAP *t);

TILE *tilemap_pega_tile(TILEMAP *t, int x, int y);
int tilemap_poe_tile(TILEMAP *t, int x, int y, TILE *ntile);
int tilemap_poe_tani(TILEMAP *t, short int id, short int quadros);
TILE_ANI	*tilemap_pega_tani(TILEMAP *t, short int id);
int tilemap_muda_tani(TILEMAP *t, short int id, short int n, short int *delays,
							 TILE *tiles);
void tilemap_destroi_tani(TILEMAP *t, short int id);
int tilemap_poe_tpani(TILEMAP *t, short int x, short int y, short int id,
							 short int tatual, short int tempo);
TILE_PANI *tilemap_pega_tpani(TILEMAP *t, short int x, short int y);
void tilemap_destroi_tpani(TILEMAP *t, short int x, short int y);

void tilemap_atualiza(TILEMAP *t, int tempo);
void tilemap_desenha(TILEMAP *t, BITMAP *b, int xoff, int yoff, int x1, int y1,
							int x2, int y2, DATAFILE *df);
int tilemap_salva(TILEMAP *t, PACKFILE *f);
TILEMAP *tilemap_abre(PACKFILE *f);

void tile_poe_efeito(TILE *t, unsigned char efeito, int c);
void tile_desenha(TILE *t, BITMAP *bmp, int xoff, int yoff, int tw, int th, DATAFILE *df);

TILE_ANI	*tile_ani_cria(short int id, short int quadros);
void tile_ani_destroi(TILE_ANI *t);
void tile_ani_adiciona(TILE_ANI *t, TILE_ANI **taini);
void tile_ani_deleta(TILE_ANI *t, TILE_ANI **taini);
TILE_ANI	*tile_ani_procura_id(TILE_ANI **taini, short int id);
int tile_ani_muda(TILE_ANI *t, short int n, short int *delays, TILE *tiles);
int tile_ani_salva(TILE_ANI *t, PACKFILE *f);
TILE_ANI	*tile_ani_abre(PACKFILE *f);

TILE_PANI *tile_pani_cria(short int x, short int y, short int id,
								  short int tatual, short int tempo);
void tile_pani_destroi(TILE_PANI *t);
void tile_pani_adiciona(TILE_PANI *t, TILE_PANI **tpaini);
void tile_pani_deleta(TILE_PANI *t, TILE_PANI **tpaini);
TILE_PANI *tile_pani_procura_xy(TILE_PANI **tpaini, short int x, short int y);
int tile_pani_salva(TILE_PANI *t, PACKFILE *f);
TILE_PANI *tile_pani_abre(PACKFILE *f);


/****************************
 * Fun��es para o spritemap *
 ****************************/

SPRITEMAP *spritemap_cria(int w, int h);
void spritemap_destroi(SPRITEMAP *sm);
int spritemap_poe_sprite(SPRITEMAP *sm, short int id, short int x, short int y,
								 short int w, short int h, short int n, short int ndf);
SPRITE *spritemap_pega_sprite(SPRITEMAP *sm, short int id);
void spritemap_destroi_sprite(SPRITEMAP *sm, short int id);
void spritemap_atualiza(SPRITEMAP *sm, int tempo);
void spritemap_muda_quadro_sprite(SPRITEMAP *sm,  short int id, short int quadro, short int nq,
											 short int nd, unsigned char ne, int nc, fixed na);
void spritemap_muda_status_sprite(SPRITEMAP *sm, short int id, unsigned char ns);
unsigned	char spritemap_pega_status_sprite(SPRITEMAP *sm, short int id);
void spritemap_desenha(SPRITEMAP *sm, BITMAP *b, int xoff, int yoff,
							  int x1, int y1, int x2, int y2, DATAFILE **dfs);
int spritemap_salva(SPRITEMAP *sm, PACKFILE *f);
SPRITEMAP *spritemap_abre(PACKFILE *f);

SPRITE *sprite_cria(short int id, short int x, short int y, short int w,
						  short int h, short int n, short int ndf);
void sprite_destroi(SPRITE *s);
void sprite_adiciona(SPRITE *s, SPRITE **lista);
void sprite_deleta(SPRITE *s, SPRITE **lista);
void sprite_muda_quadro(SPRITE *s, short int quadro, short int nq, short int nd,
								unsigned char ne, int nc, fixed na);
short	int sprite_pega_id(SPRITE *s);
SPRITE *sprite_procura_id(SPRITE **lista, short int id);
void sprite_muda_status(SPRITE *s, unsigned char ns);
unsigned	char sprite_pega_status(SPRITE *s);
void sprite_desenha(SPRITE *s, BITMAP *b, int xoff, int yoff, DATAFILE *df);
int sprite_salva(SPRITE *s, PACKFILE *f);
SPRITE *sprite_abre(PACKFILE *f);


/**************************
 *	Fun��es para o Obstmap *
 **************************/

OBSTMAP *obstmap_cria(int x, int y, unsigned char valor_padrao);
void obstmap_destroi(OBSTMAP *o);
int obstmap_poe_obstaculo(OBSTMAP *o, short int id, short int x, short int y, short int w, short int h, unsigned char d);
OBSTACULO **obstmap_pega_obstaculo(OBSTMAP *o, short int id, short int *no);
void obstmap_destroi_obstaculo(OBSTMAP *o, short int id);
void obstmap_muda_obstaculo(OBSTMAP *o, short int id, unsigned char nv);
void obstmap_move_obstaculo(OBSTMAP *o, short int id, short int x, short int y);
int obstmap_colisao_ponto(OBSTMAP *o, short int x, short int y, unsigned char *v);
int obstmap_colisao_area(OBSTMAP *o, short int x, short int y, short int w, short int h, unsigned char *v);
int obstmap_salva(OBSTMAP *o, PACKFILE *f);
OBSTMAP *obstmap_abre(PACKFILE *f);
void obstmap_desenha(OBSTMAP *o, BITMAP *b, int xoff, int yoff,
							int x1, int y1, int x2, int y2);
OBSTACULO *obstaculo_cria(short int id, short int x, short int y, short int w, short int h, unsigned char v);
void obstaculo_destroi(OBSTACULO *ob);
void obstaculo_adiciona(OBSTACULO *ob, OBSTACULO **lista);
void obstaculo_deleta(OBSTACULO *ob, OBSTACULO **lista);
short int obstaculo_pega_id(OBSTACULO *ob);
OBSTACULO *obstaculo_procura_id(OBSTACULO **lista, short int id);
int obstaculo_colisao_ponto(OBSTACULO *ob, short int x, short int y, unsigned char *v);
int obstaculo_colisao_area(OBSTACULO *ob, short int x, short int y, short int w, short int h, unsigned char *v);
int _entre(short int i, short int i1, short int i2);
int obstaculo_salva(OBSTACULO *ob, PACKFILE *f);
OBSTACULO *obstaculo_abre(PACKFILE *f);


#ifdef __cplusplus
}
#endif

#endif /* ifndef MAPA_H */