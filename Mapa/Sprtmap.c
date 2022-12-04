#include <stdlib.h>
#include <string.h>
#include "allegro.h"
#include "mapa.h"

/**********************************************************************************************
	Cria um sprite w x h pixels

	ENTRADA:	w, h - tamanho do mapa, em pixels
	SAÍDA:	ponteiro para o spritemap criado, ou NULL se erro
**********************************************************************************************/
SPRITEMAP *spritemap_cria(int w, int h)
{
	SPRITEMAP *sm;
	sm = malloc(sizeof(SPRITEMAP));
	if(!sm)
		return NULL;
	sm->w = w;
	sm->h = h;
	sm->n_sprites = 0;
	sm->lista = NULL;
	return sm;
}

/**********************************************************************************************
	Libera a memória alocada pelo spritemap

	ENTRADA:	sm - spritemap a ser destruído
	SAÍDA:	nada
**********************************************************************************************/
void spritemap_destroi(SPRITEMAP *sm)
{
	SPRITE *s, *p;
	if(!sm)
		return;
	for(s = sm->lista; s; ) {
		p = s->prox;
		sprite_destroi(s);
		s = p;
	}
	free(sm);
	sm = NULL;
}

/**********************************************************************************************
	Cria um sprite e põe na lista

	ENTRADA: sm - spritemap
				id - id do sprite
				x, y - posição inicial
				w, h - tamanho do sprite
				n - número de quadros
				ndf - índice do df que contém os quadros do sprite
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int spritemap_poe_sprite(SPRITEMAP *sm, short int id, short int x, short int y,
								 short int w, short int h, short int n, short int ndf)
{
	SPRITE *s;
	if(sprite_procura_id(&sm->lista, id))
		return 1;
	s = sprite_cria(id, x, y, w, h, n, ndf);
	if(!s)
		return 1;
	sprite_adiciona(s, &sm->lista);
	sm->n_sprites++;
	return 0;
}

/**********************************************************************************************
	Retorna o sprite com o id especificado

	ENTRADA:	sm - spritemap
				id - id do sprite procurado
	SAÍDA:	ponteiro para o sprite se encontrou, NULL se não achou
**********************************************************************************************/
SPRITE *spritemap_pega_sprite(SPRITEMAP *sm, short int id)
{
	return sprite_procura_id(&sm->lista, id);
}

/**********************************************************************************************
	Tira o sprite da lista e libera a memória

	ENTRADA:	sm - spritemap
				id - id do sprite a ser deletado
	SAÍDA:	nada
**********************************************************************************************/
void spritemap_destroi_sprite(SPRITEMAP *sm, short int id)
{
	SPRITE *s;
	s = sprite_procura_id(&sm->lista, id);
	if(s) {
		sprite_deleta(s, &sm->lista);
		sm->n_sprites--;
	}
}

/**********************************************************************************************
	Atualiza as animações

	ENTRADA:	sm - spritemap
				tempo - tempo decorrido desde a última chamada
	SAÍDA:	nada
**********************************************************************************************/
void spritemap_atualiza(SPRITEMAP *sm, int tempo)
{
	short int d, tp, qatual;
	SPRITE *s;
	if(tempo <= 0)
		return;
	for(s = sm->lista; s; s = s->prox) {
		qatual = s->q_atual;
		tp = s->tempo+tempo;
		d = s->delays[qatual];
		while(tp >= d && d>0) {
			tp -= d;
			++qatual;
			if(qatual >= s->n_quadros)
				qatual = 0;
			d = s->delays[qatual];
		}
		s->tempo = tp;
		s->q_atual = qatual;
	}
}

/**********************************************************************************************
	Muda um dos quadros do sprite, alterando o quadro e o delay

	ENTRADA:	sm - spritemap
				id - id do sprite a ser alterado
				quadro - número do quadro
				nq, nd, ne, nc, na - novos quadro, delay efeito, cor e angulo
	SAÍDA:	nada
**********************************************************************************************/
void spritemap_muda_quadro_sprite(SPRITEMAP *sm,  short int id, short int quadro,
											 short int nq, short int nd, unsigned char ne, int nc, fixed na)
{
	SPRITE *s;
	s = sprite_procura_id(&sm->lista, id);
	if(s)
		sprite_muda_quadro(s, quadro, nq, nd, ne, nc, na);
}

/**********************************************************************************************
	Altera o status de um sprite

	ENTRADA:	sm - spritemap
				id - id do sprite
				ns - novo valor do status
	SAIDA:	nada
**********************************************************************************************/
void spritemap_muda_status_sprite(SPRITEMAP *sm, short int id, unsigned char ns)
{
	SPRITE *s;
	s = sprite_procura_id(&sm->lista, id);
	if(s)
		sprite_muda_status(s, ns);
}

/**********************************************************************************************
	Retorna o status de um sprite

	ENTRADA:	sm - spritemap
				id - id do sprite
	SAIDA:	valor do status do sprite, ou 0xFF se nao encontrou o sprite
**********************************************************************************************/
unsigned char spritemap_pega_status_sprite(SPRITEMAP *sm, short int id)
{
	SPRITE *s;
	s = sprite_procura_id(&sm->lista, id);
	if(s)
		return sprite_pega_status(s);
	return 0xFF;
}

/**********************************************************************************************
	Desenha o spritemap no bitmap especificado

	ENTRADA:	sm - spritemap
				b - bitmap onde será desenhado
				xoff, yoff - ponto indicando o canto superior esquerdo do desenho
				x1, y1, x2, y2 - área que deve ser desenhada
				dfs - vetor de datafiles que será usado
	SAÍDA:	nada
**********************************************************************************************/
void spritemap_desenha(SPRITEMAP *sm, BITMAP *b, int xoff, int yoff,
							  int x1, int y1, int x2, int y2, DATAFILE **dfs)
{
	SPRITE *s;
	set_clip(b, xoff, yoff, xoff+x2-x1, yoff+y2-y1);
	for(s = sm->lista; s; s = s->prox) {
		/* verifica se deve desenhar o sprite */
		if( !((s->status & S_S_DESATIVADO) || (s->xp+s->w < x1) || (s->yp+s->h < y1)
			|| (s->xp > x2) || (s->yp > y2)) ){
			sprite_desenha(s, b, xoff+s->xp-x1, yoff+s->yp-y1, dfs[s->ndf]);
		}
	}
}

/**********************************************************************************************
	Salva o spritemap em um arquivo ja aberto

	ENTRADA:	sm - spritemap
				f - arquivo ja aberto onde sera salvo
	SAIDA:	0 se ok, 1 se erro
**********************************************************************************************/
int spritemap_salva(SPRITEMAP *sm, PACKFILE *f)
{
	SPRITE *s;
	pack_fwrite(sm, sizeof(SPRITEMAP), f);
	for(s = sm->lista; s; s = s->prox) {
		if(sprite_salva(s, f)) {
			return 1;
		}
	}
	return 0;
}

/**********************************************************************************************
	Carrega do arquivo o spritemap

	ENTRADA:	f - arquivo ja aberto
	SAIDA:	ponteiro para o spritemap se ok, NULL se erro
**********************************************************************************************/
SPRITEMAP *spritemap_abre(PACKFILE *f)
{
	SPRITEMAP *sm, tsm;
	SPRITE *s;
	int i;
	pack_fread(&tsm, sizeof(SPRITEMAP), f);
	sm = spritemap_cria(tsm.w, tsm.h);
	if(!sm)
   	return NULL;
	for(i=0; i<tsm.n_sprites; i++) {
		s = sprite_abre(f);
		if(!s) {
			spritemap_destroi(sm);
			return NULL;
		}
		sprite_adiciona(s, &sm->lista);
		sm->n_sprites++;
	}
	return sm;
}

/**********************************************************************************************
	Cria um sprite e retorna um ponteiro para ele

	ENTRADA:	id - id do sprite
				x, y - posiçao do sprite no mapa
				w, h - tamanho do sprite
				n - numero de quadros
				ndf - numero do datafile que estao os quadros
**********************************************************************************************/
SPRITE *sprite_cria(short int id, short int x, short int y, short int w,
						  short int h, short int n, short int ndf)
{
	SPRITE *s;
	s = malloc(sizeof(SPRITE));
	if(!s)
		return NULL;
	s->quadros = malloc(sizeof(short int)*n);
	if(!s->quadros) {
		free(s);
		return NULL;
	}
	s->delays = malloc(sizeof(short int)*n);
	if(!s->delays) {
		free(s->quadros);
		free(s);
		return NULL;
	}
	s->efeito = malloc(sizeof(unsigned char)*n);
	if(!s->efeito) {
		free(s->delays);
		free(s->quadros);
		free(s);
		return NULL;
	}
	s->c = malloc(sizeof(int)*n);
	if(!s->c) {
		free(s->efeito);
		free(s->delays);
		free(s->quadros);
		free(s);
		return NULL;
	}
	s->angulo = malloc(sizeof(fixed)*n);
	if(!s->angulo) {
		free(s->c);
		free(s->efeito);
		free(s->delays);
		free(s->quadros);
		free(s);
		return NULL;
	}
	memset(s->angulo, 0, sizeof(fixed)*n);
	memset(s->c, 0, sizeof(int)*n);
	memset(s->efeito, 0, sizeof(unsigned char)*n);
	memset(s->delays, 0, sizeof(short int)*n);
	memset(s->quadros, 0, sizeof(short int)*n);
	s->status = S_S_NORMAL;
	s->id = id;
	s->xp = x;
	s->yp = y;
	s->w = w;
	s->h = h;
	s->n_quadros = n;
	s->ndf = ndf;
	s->q_atual = 0;
	s->tempo = 0;
	s->prox = s->ant = NULL;
	return s;
}

/**********************************************************************************************
	Libera a memoria alocada pelo sprite

	ENTRADA:	s - sprite
	SAIDA:	nada
**********************************************************************************************/
void sprite_destroi(SPRITE *s)
{
	if(!s)
		return;
	free(s->angulo);
	free(s->c);
	free(s->efeito);
	free(s->delays);
	free(s->quadros);
	free(s);
	s = NULL;
}

/**********************************************************************************************
	Coloca um sprite na lista, ordenando pelo id, que e unico

	ENTRADA:	s - sprite
				lista - lista de sprites onde sera colocado
**********************************************************************************************/
void sprite_adiciona(SPRITE *s, SPRITE **lista)
{
	SPRITE *p, *old;
	/* primeiro elemento da lista */
	if(*lista == NULL) {
		s->ant = NULL;
		s->prox = NULL;
		*lista = s;
		return;
	}
	/* começa no topo da lista */
	p = *lista;
	old = NULL;
	while(p) {
		if(p->id < s->id) {
			old = p;
			p = p->prox;
		}
		else {
			if(p->ant) {
				p->ant->prox = s;
				s->prox = p;
				s->ant = p->ant;
				p->ant = s;
				return;
			}
			/* novo primeiro elemento */
			s->prox = p;
			s->ant = NULL;
			p->ant = s;
			*lista = s;
			return;
		}
	}
	/* coloca no final da lista */
	old->prox = s;
	s->ant = old;
	s->prox = NULL;
}

/**********************************************************************************************
	Retira o sprite da lista e libera a memoria

	ENTRADA:	s - sprite
				lista - lista de sprites de onde sera retirado
	SAIDA:	nada
**********************************************************************************************/
void sprite_deleta(SPRITE *s, SPRITE **lista)
{
	if(s) {
		if(*lista == s) {
			*lista = s->prox;
			if(*lista)
				(*lista)->ant = NULL;
		}
		else {
			s->ant->prox = s->prox;
			if(s->prox)
				s->prox->ant = s->ant;
		}
		sprite_destroi(s);
	}
}

/**********************************************************************************************
	Altera no sprite um dos quadros

	ENTRADA:	s - sprite
				quadro - nº do quadro a ser alterado
				nq - novo quadro
				nd - novo delay
				ne - novo efeito
				nc - nova cor para efeito pintura
				na - novo angulo para efeito rotaçao
	SAIDA:	nada
**********************************************************************************************/
void sprite_muda_quadro(SPRITE *s, short int quadro, short int nq, short int nd,
								unsigned char ne, int nc, fixed na)
{
	if( (quadro<0) || (quadro>=s->n_quadros) )
		return;
	s->quadros[quadro] = nq;
	s->delays[quadro] = nd;
	s->efeito[quadro] = ne;
	s->c[quadro] = nc;
   s->angulo[quadro] = na;
}

/**********************************************************************************************
	Retorna o valor de id do sprite

	ENTRADA:	s - sprite
	SAIDA:	valor de id do sprite
**********************************************************************************************/
short int sprite_pega_id(SPRITE *s)
{
	return s->id;
}

/**********************************************************************************************
	Procura por um sprite na lista com um valor de id igual ao procurado

	ENTRADA:	lista - lista de sprites
				id - valor do id procurado
	SAIDA:	ponteiro para o sprite se encontrou, NULL se nao encontrou
**********************************************************************************************/
SPRITE *sprite_procura_id(SPRITE **lista, short int id)
{
	SPRITE *s;
	for(s = *lista; s; s = s->prox) {
		if(id == sprite_pega_id(s)) {
			return s;
		}
	}
	return NULL;
}

/**********************************************************************************************
	Muda o status do sprite s

	ENTRADA:	s - sprite
				ns - novo valor do status
	SAIDA:	nada
**********************************************************************************************/
void sprite_muda_status(SPRITE *s, unsigned char ns)
{
	s->status = ns;
}

/**********************************************************************************************
	Retorna o valor do status do sprite s

	ENTRADA:	s - sprite
	SAIDA:	valor do status do sprite
**********************************************************************************************/
unsigned char sprite_pega_status(SPRITE *s)
{
	return s->status;
}

/**********************************************************************************************
	Desenha o sprite no bitmap e na posiçao indicada

	ENTRADA:	s - sprite
				b - bitmap onde sera desenhado
				xoff, yoff - ponto onde deve começar o desenho
				df - datafile onde estao os quadros do sprite
**********************************************************************************************/
void sprite_desenha(SPRITE *s, BITMAP *b, int xoff, int yoff, DATAFILE *df)
{
	int d;
	d = s->quadros[s->q_atual];
	switch(df[d].type) {
		case DAT_BITMAP :
			switch(s->efeito[s->q_atual]) {
				case S_E_VERT :
					draw_sprite_v_flip(b, (BITMAP *)df[d].dat, xoff, yoff);
					break;
				case S_E_HOR :
					draw_sprite_h_flip(b, (BITMAP *)df[d].dat, xoff, yoff);
					break;
				case S_E_DIAG :
					draw_sprite_vh_flip(b, (BITMAP *)df[d].dat, xoff, yoff);
					break;
				case S_E_TRANS :
					draw_trans_sprite(b, (BITMAP *)df[d].dat, xoff, yoff);
					break;
				case S_E_PINTA :
					draw_lit_sprite(b, (BITMAP *)df[d].dat, xoff, yoff, s->c[s->q_atual]);
					break;
				case S_E_ROTACAO :
					rotate_sprite(b, (BITMAP *)df[d].dat, xoff, yoff, s->angulo[s->q_atual]);
				default :
					draw_sprite(b, (BITMAP *)df[d].dat, xoff, yoff);
					break;
			}
			break;
		case DAT_RLE_SPRITE :
			switch(s->efeito[s->q_atual]) {
				case S_E_TRANS :
					draw_trans_rle_sprite(b, (RLE_SPRITE *)df[d].dat, xoff, yoff);
					break;
				case S_E_PINTA :
					draw_lit_rle_sprite(b, (RLE_SPRITE *)df[d].dat, xoff, yoff, s->c[s->q_atual]);
					break;
				default :
					draw_rle_sprite(b, (RLE_SPRITE *)df[d].dat, xoff, yoff);
					break;
			}
			break;
		default :
			break;
	}
}

/**********************************************************************************************
	Salva um sprite em arquivo

	ENTRADA:	s - sprite
				f - arquivo ja aberto
	SAIDA:	0 se ok, 1 se erro
**********************************************************************************************/
int sprite_salva(SPRITE *s, PACKFILE *f)
{
	int n;
	pack_fwrite(s, sizeof(SPRITE), f);
	n = s->n_quadros;
	pack_fwrite(s->quadros, n*sizeof(short int), f);
	pack_fwrite(s->delays, n*sizeof(short int), f);
	pack_fwrite(s->efeito, n*sizeof(unsigned char), f);
	pack_fwrite(s->c, n*sizeof(int), f);
	pack_fwrite(s->angulo, n*sizeof(fixed), f);
	return 0;
}

/**********************************************************************************************
	Carrega um sprite do disco e retorna um ponteiro para ele

	ENTRADA:	f - arquivo ja aberto
	SAIDA:	ponteiro para o sprite se ok, NULL se erro
**********************************************************************************************/
SPRITE *sprite_abre(PACKFILE *f)
{
	short int n;
	SPRITE *s, ts;
	pack_fread(&ts, sizeof(SPRITE), f);
	s = sprite_cria(ts.id, ts.xp, ts.yp, ts.w, ts.h, ts.n_quadros, ts.ndf);
	if(!s)
		return NULL;
	n = ts.n_quadros;
	pack_fread(s->quadros, n*sizeof(short int), f);
	pack_fread(s->delays, n*sizeof(short int), f);
	pack_fread(s->efeito, n*sizeof(unsigned char), f);
	pack_fread(s->c, n*sizeof(int), f);
	pack_fread(s->angulo, n*sizeof(fixed), f);
   return s;
}