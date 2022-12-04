#include <stdlib.h>
#include <string.h>
#include "allegro.h"
#include "mapa.h"

/**************************
 * Funções para o tilemap *
 **************************/

/**********************************************************************************************
	Retorna um tilemap criado

	ENTRADA:	w, h - tamanho do tilemap, em tiles
				tw, th - tamanho dos tiles, em pixels
				isom - usar perspectiva isométrica?
	SAÍDA:	ponteiro para o tilemap, NULL se ocorreu um erro
**********************************************************************************************/
TILEMAP *tilemap_cria(int w, int h, int tw, int th, unsigned char isom)
{
   TILEMAP *t;
   /* aloca memória para o mapa */
   t = malloc(sizeof(TILEMAP));
   if(t == NULL)
      return NULL;
   t->w = w;
   t->h = h;
   t->tw = tw;
   t->th = th;
   t->isom = isom;
   t->ntani = 0;
   t->ntposani = 0;
   t->tiles_ani = NULL;
   t->tiles_pani = NULL;
   /* aloca memória para os tiles */
   t->tiles = malloc(sizeof(TILE)*w*h);
   if(t->tiles == NULL) {
      free(t);
      return NULL;
   }
   memset(t->tiles, 0, sizeof(TILE)*w*h);
	return t;
}
/**********************************************************************************************
	Libera memória usada pelo tilemap

	ENTRADA:	t - tilemap que será apagado
	SAÍDA:	nada
**********************************************************************************************/
void tilemap_destroi(TILEMAP *t)
{
   TILE_ANI *a, *ta;
   TILE_PANI *pa, *tpa;
	if(t == NULL)
		return;
   free(t->tiles);
   for(ta = t->tiles_ani; ta; ) {
      a = ta->prox;
      tile_ani_destroi(ta);
		ta = a;
   }
   for(tpa = t->tiles_pani; tpa; ) {
      pa = tpa->prox;
      tile_pani_destroi(tpa);
		tpa = pa;
   }
   free(t);
   t = NULL;
}

/**********************************************************************************************
	Retorna o tile da posição x, y

	ENTRADA:	t - tilemap onde está o tile
				x, y - posição do tile
	SAÍDA:	ponteiro para o tile, NULL se não tem
**********************************************************************************************/
TILE *tilemap_pega_tile(TILEMAP *t, int x, int y)
{
   if( (x<0) || (x>=t->w) || (y<0) || (y>=t->h) )
      return NULL;
   return &t->tiles[y*t->w+x];
}

/**********************************************************************************************
	Põe o tile na posição x, y

	ENTRADA:	t - tilemap onde será colocado o tile
				x, y - posição do tile
				ntile - novo tile que será copiado para a posição
	SAÍDA:	0 se OK, 1 se erro
**********************************************************************************************/
int tilemap_poe_tile(TILEMAP *t, int x, int y, TILE *ntile)
{
   if( (x<0) || (x>=t->w) || (y<0) || (y>=t->h) )
      return 1;
   memcpy(tilemap_pega_tile(t, x, y), ntile, sizeof(TILE));
   return 0;
}

/**********************************************************************************************
	Cria um tile animado e põe na lista para poder usar

	ENTRADA:	t - ponteiro para o tilemap
				id - id do tile animado
				quadros - nº de quadros de animação
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int tilemap_poe_tani(TILEMAP *t, short int id, short int quadros)
{
	TILE_ANI *ta;
	if(tile_ani_procura_id(&t->tiles_ani, id))
		return 1;
	ta = tile_ani_cria(id, quadros);
	if(!ta)
		return 1;
	tile_ani_adiciona(ta, &t->tiles_ani);
	t->ntani++;
	return 0;
}

/**********************************************************************************************
	Retorna um ponteiro para o tile animado procurado

	ENTRADA:	t - ponteiro para o tilemap
				id - nº de id do tile animado procurado
	SAÍDA:	ponteiro para o tile animado, ou NULL se não encontrou
**********************************************************************************************/
TILE_ANI *tilemap_pega_tani(TILEMAP *t, short int id)
{
	return tile_ani_procura_id(&t->tiles_ani, id);
}

/**********************************************************************************************
	Muda o t.a.

	ENTRADA:	t - ponteiro para o tilemap
				id - id do t.a. a ser mudado
				n, delays, tiles - novas características do t.a.
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int tilemap_muda_tani(TILEMAP *t, short int id, short int n, short int *delays,
							 TILE *tiles)
{
	TILE_ANI *ta;
	ta = tilemap_pega_tani(t, id);
	if(!ta)
		return 1;
	return tile_ani_muda(ta, n, delays, tiles);
}

/**********************************************************************************************
	Apaga o ta da lista

	ENTRADA:	t - ponteiro para o tilemap
				id - id do ta a ser apagado
	SAÍDA:	nada
**********************************************************************************************/
void tilemap_destroi_tani(TILEMAP *t, short int id)
{
	TILE_ANI *ta;
	ta = tile_ani_procura_id(&t->tiles_ani, id);
	if(ta) {
		tile_ani_deleta(ta, &t->tiles_ani);
		t->ntani--;
	}
}

/**********************************************************************************************
	Cria um tpa e põe na lista

	ENTRADA:	t - ponteiro para o tilemap
				x, y - posição do tpa
				id - nº de id do t.a.
				tatual - índice do tile atual na animação
				tempo - tempo decorrido
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int tilemap_poe_tpani(TILEMAP *t, short int x, short int y, short int id,
							 short int tatual, short int tempo)
{
	TILE_PANI *tpa;
	if(tile_pani_procura_xy(&t->tiles_pani, x, y))
		return 1;
	tpa = tile_pani_cria(x, y, id, tatual, tempo);
	if(!tpa)
		return 1;
	tile_pani_adiciona(tpa, &t->tiles_pani);
	t->ntposani++;
	return 0;
}

/**********************************************************************************************
	Retorna um ponteiro para o tpa da posição x, y

	ENTRADA:	t - ponteiro para o tilemap
				x, y - posição do tpa
	SAÍDA:	ponteiro para o tpa, NULL se não encontrou
**********************************************************************************************/
TILE_PANI *tilemap_pega_tpani(TILEMAP *t, short int x, short int y)
{
	return tile_pani_procura_xy(&t->tiles_pani, x, y);
}

/**********************************************************************************************
	Apaga o tpa da posição x, y

	ENTRADA:	t - ponteiro para o tilemap
				x, y - posição do tpa
	SAÍDA:	nada
**********************************************************************************************/
void tilemap_destroi_tpani(TILEMAP *t, short int x, short int y)
{
	TILE_PANI *tpa;
	tpa = tile_pani_procura_xy(&t->tiles_pani, x, y);
	if(tpa) {
		tile_pani_deleta(tpa, &t->tiles_pani);
		t->ntposani--;
	}
}

/**********************************************************************************************
	Atualiza as animações do tilemap inteiro

	ENTRADA:	t - ponteiro para o tilemap
				tempo - tempo decorrido desde a última chamada, em quadros (se o seu programa
				usa 30 quadros por segundo, passando 15 indica 0.5 segundos)
	SAÍDA:	nada
**********************************************************************************************/
void tilemap_atualiza(TILEMAP *t, int tempo)
{
   int d, tp;
	short int tatual;
   TILE_ANI *tani;
   TILE_PANI *tpani;
	unsigned char mudou;
   if(tempo <= 0)
      return;
   for(tpani = t->tiles_pani, tani = t->tiles_ani; tpani; tpani = tpani->prox) {
		mudou = 0;
		/* se o id não mudou do anterior, não precisa procurar de novo a mesma
			animação */
		if(tani->id != tpani->id_ani)
	      tani = tilemap_pega_tani(t, tpani->id_ani);
      tatual = tpani->tatual;
      tp = tpani->tempo + tempo;
      d = tani->delays[tatual];
      while(tp >= d && d > 0) {
         tp -= d;
         ++tatual;
         if(tatual>=tani->n)
            tatual = 0;
         d = tani->delays[tatual];
			/* se mudou, atualiza o tile atual*/
			mudou = 1;
      }
      tpani->tempo = tp;
		if(mudou) {
	      tpani->tatual = tatual;
		   tilemap_poe_tile(t, tpani->x, tpani->y, &tani->tiles[tatual]);
		}
   }
}

/**********************************************************************************************
	Desenha o tilemap no bitmap especificado, usando o datafile correspondente

	ENTRADA:	t - ponteiro para o tilemap
				bmp - bitmap onde será desenhado o mapa
				xoff, yoff - ponto onde será iniciado o desenho
				modo - 0 = sem transparência, 1 com transparência
				x1, y1, x2, y2 - área que deve ser desenhada
				df - ponteiro para o datafile
	SAÍDA:	nada
**********************************************************************************************/
void tilemap_desenha(TILEMAP *t, BITMAP *b, int xoff, int yoff, int x1, int y1,
							int x2, int y2, DATAFILE *df)
{
   int i, j, x, y;
	x = x2/t->tw;
	y = y2/t->th;
	set_clip(b, xoff, yoff, xoff+x2-x1, yoff+y2-y1);
   for(i=y1/t->th; i<=y; i++) {
      for(j=x1/t->tw; j<=x; j++) {
         tile_desenha(tilemap_pega_tile(t, j, i), b, xoff+(j*t->tw)-x1, yoff+(i*t->th)-y1,
							 t->tw, t->th, df);
      }
   }
}

/**********************************************************************************************
	Salva o tilemap em disco

	ENTRADA: t - tilemap que será salvo
				f - arquivo já aberto onde será salvo
	SAÍDA:	0 se OK
**********************************************************************************************/
int tilemap_salva(TILEMAP *t, PACKFILE *f)
{
	TILE_ANI *ta;
	TILE_PANI *tpa;
	int i;
   pack_fwrite(t, sizeof(TILEMAP), f);
   pack_fwrite(t->tiles, sizeof(TILE)*t->w*t->h, f);
	for(i=0, ta=t->tiles_ani; i<t->ntani; i++, ta=ta->prox) {
      if(tile_ani_salva(ta, f)) {
			return 1;
		}
	}for(i=0, tpa=t->tiles_pani; i<t->ntposani; i++, tpa = tpa->prox) {
		if(tile_pani_salva(tpa, f)) {
			return 1;
		}
	}
   return 0;
}

/**********************************************************************************************
	Carrega o tilemap do disco

	ENTRADA: f - arquivo já aberto onde se encontra o tilemap
	SAÍDA:	ponteiro para o tilemap se OK, NULL se deu erro
**********************************************************************************************/
TILEMAP *tilemap_abre(PACKFILE *f)
{
	TILEMAP *r, t;
	TILE_ANI *ta;
	TILE_PANI *tpa;
	int i;
   pack_fread(&t, sizeof(TILEMAP), f);
	r = tilemap_cria(t.w, t.h, t.tw, t.th, t.isom);
	if(!r)
		return NULL;
	pack_fread(r->tiles, sizeof(TILE)*t.w*t.h, f);
	for(i=0; i<t.ntani; i++) {
		ta = tile_ani_abre(f);
		tile_ani_adiciona(ta, &r->tiles_ani);
	}
	for(i=0; i<t.ntposani; i++) {
		tpa = tile_pani_abre(f);
		tile_pani_adiciona(tpa, &r->tiles_pani);
	}
	return r;
}

/**********************************************************************************************
	Aplica um efeito desejado em um tile

	ENTRADA:	t - ponteiro para o tile
				efeito - efeito desejado
				x, y, z - variáveis usadas nos efeitos
	SAÍDA:	nada
**********************************************************************************************/
void tile_poe_efeito(TILE *t, unsigned char efeito, int c)
{
	t->efeito = efeito;
	t->c = c;
}

/**********************************************************************************************
	Desenha um tile no bitmap especificado

	ENTRADA:	t - ponteiro para o tile
				bmp - bitmap onde será desenhado
				xoff, yoff - ponto inicial do desenho
				modo - 0 = sem transparência, 1 = com transparência
				maxx, maxy - até onde o desenho pode ir
				df - datafile contendo os tiles
**********************************************************************************************/
void tile_desenha(TILE *t, BITMAP *bmp, int xoff, int yoff, int tw, int th, DATAFILE *df)
{
   int td;
   td = t->i;
   switch(df[td].type) {
      case DAT_BITMAP :
			switch(t->efeito) {
				case T_E_VERT :
					draw_sprite_v_flip(bmp, (BITMAP *)df[td].dat, xoff, yoff);
					break;
				case T_E_HOR :
					draw_sprite_h_flip(bmp, (BITMAP *)df[td].dat, xoff, yoff);
					break;
				case T_E_DIAG :
					draw_sprite_vh_flip(bmp, (BITMAP *)df[td].dat, xoff, yoff);
					break;
				case T_E_TRANS :
					draw_trans_sprite(bmp, (BITMAP *)df[td].dat, xoff, yoff);
					break;
				case T_E_PINTA :
					draw_lit_sprite(bmp, (BITMAP *)df[td].dat, xoff, yoff, t->c);
					break;
				default :
					draw_sprite(bmp, (BITMAP *)df[td].dat, xoff, yoff);
					break;
			}
			break;
      case DAT_RLE_SPRITE :
			switch(t->efeito) {
				case T_E_TRANS :
					draw_trans_rle_sprite(bmp, (RLE_SPRITE *)df[td].dat, xoff, yoff);
					break;
				case T_E_PINTA :
					draw_lit_rle_sprite(bmp, (RLE_SPRITE *)df[td].dat, xoff, yoff, t->c);
					break;
				default :
					draw_rle_sprite(bmp, (RLE_SPRITE *)df[td].dat, xoff, yoff);
					break;
			}
         break;
      default :
			break;
   }
}

/**********************************************************************************************
	Aloca memória para um tile animado

	ENTRADA:	id - nº de id do novo tile animado
				quadros - nº de quadros do tile animado
	SAÍDA:	ponteiro para o tile animado se ok, NULL se erro
**********************************************************************************************/
TILE_ANI *tile_ani_cria(short int id, short int quadros)
{
	TILE_ANI *ta;
	ta = malloc(sizeof(TILE_ANI));
	if(!ta)
		return NULL;
	ta->id = id;
	ta->n = quadros;
	ta->delays = malloc(quadros*sizeof(short int));
	if(!ta->delays) {
		free(ta);
		return NULL;
	}
	ta->tiles = malloc(quadros*sizeof(TILE));
	if(!ta->tiles) {
		free(ta->delays);
		free(ta);
		return NULL;
	}
	ta->prox = ta->ant = NULL;
	memset(ta->delays, 0, quadros*sizeof(short int));
	memset(ta->tiles, 0, quadros*sizeof(TILE));
	return ta;
}

/**********************************************************************************************
	Libera memória usada pelo tile animado

	ENTRADA:	t - ponteiro para o tile animado
	SAÍDA:	nada
**********************************************************************************************/
void tile_ani_destroi(TILE_ANI *t)
{
	if(!t)
		return;
	free(t->tiles);
	free(t->delays);
	free(t);
	t = NULL;
}

/**********************************************************************************************
	Coloca o t.a. na lista

	ENTRADA:	t - t.a. que será colocado na lista
				taini - início da lista de t.a.
	SAÍDA:	nada
**********************************************************************************************/
void tile_ani_adiciona(TILE_ANI *t, TILE_ANI **taini)
{
	TILE_ANI *p, *old;
	/* primeiro elemento da lista */
	if(*taini == NULL) {
		t->ant = NULL;
		t->prox = NULL;
		*taini = t;
		return;
	}
	/* começa no topo da lista */
	p = *taini;
	old = NULL;
	while(p) {
		if(p->id < t->id) {
			old = p;
			p = p->prox;
		}
		else {
			if(p->ant) {
				p->ant->prox = t;
				t->prox = p;
				t->ant = p->ant;
				p->ant = t;
				return;
			}
			/* novo primeiro elemento */
			t->prox = p;
			t->ant = NULL;
			p->ant = t;
			*taini = t;
			return;
		}
	}
	/* coloca no final da lista */
	old->prox = t;
	t->ant = old;
	t->prox = NULL;
}

/**********************************************************************************************
	Retira o tile animado da lista e libera a memória usada

	ENTRADA:	t - tile animado a ser apagado
				taini - início da lista de tiles animados onde o t.a. se encontra
	SAÍDA:	nada
**********************************************************************************************/
void tile_ani_deleta(TILE_ANI *t, TILE_ANI **taini)
{
	if(t) {
		if(*taini == t) {
			*taini = t->prox;
			if(*taini)
				(*taini)->ant = NULL;
		}
		else {
			t->ant->prox = t->prox;
			if(t->prox)
				t->prox->ant = t->ant;
		}
		tile_ani_destroi(t);
	}
}

/**********************************************************************************************
	Procura o t.a. com o id correspondente

	ENTRADA:	taini - lista de t.a.
				id - nº do id a ser procurado
	SAÍDA:	ponteiro para o t.a., NULL se não encontrou
**********************************************************************************************/
TILE_ANI *tile_ani_procura_id(TILE_ANI **taini, short int id)
{
	TILE_ANI *p;
	p = *taini;
	while(p) {
		if(p->id == id)
			return p;
		p = p->prox;
	}
	return NULL;
}

/**********************************************************************************************
	Altera o t.a. passado

	ENTRADA:	t - ponteiro para o t.a.
				n - nº de quadros
				delays - espera entre cada quadro, em nº de quadros
				tiles - vetor com os tiles da animação
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int tile_ani_muda(TILE_ANI *t, short int n, short int *delays, TILE *tiles)
{
	if(n<=1)
		return 1;
	if(n != t->n) {
		free(t->delays);
		free(t->tiles);
		t->delays = malloc(n*sizeof(short int));
		if(!t->delays)
			return 1;
		t->tiles = malloc(n*sizeof(TILE));
		if(!t->tiles) {
			free(t->delays);
			return 1;
		}
	}
	memcpy(t->delays, delays, n*sizeof(short int));
	memcpy(t->tiles, tiles, n*sizeof(TILE));
	return 0;
}

/**********************************************************************************************
	Salva o t.a. no arquivo aberto antes

	ENTRADA:	t - tile animado a ser salvo
				f - arquivo aberto previamente
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int tile_ani_salva(TILE_ANI *t, PACKFILE *f)
{
	pack_fwrite(t, sizeof(TILE_ANI), f);
	pack_fwrite(t->delays, t->n*sizeof(short int), f);
	pack_fwrite(t->tiles, t->n*sizeof(TILE), f);
	return 0;
}

/**********************************************************************************************
	Abre o t.a. do arquivo

	ENTRADA:	f - arquivo já aberto
	SAÍDA:	ponteiro para o t.a., NULL se erro
**********************************************************************************************/
TILE_ANI *tile_ani_abre(PACKFILE *f)
{
	TILE_ANI *t, tmp;
	pack_fread(&tmp, sizeof(TILE_ANI), f);
	t = tile_ani_cria(tmp.id, tmp.n);
	if(!t)
		return NULL;
	pack_fread(t->delays, tmp.n*sizeof(short int), f);
	pack_fread(t->tiles, tmp.n*sizeof(TILE), f);
	return t;
}

/**********************************************************************************************
	Cria uma posição onde usa um t.a.

	ENTRADA:	x, y - posição do tile animado
				id - id do t.a.
				tatual - índice do tile que começa
				tempo - tempo que começa, em quadros
	SAÍDA:	ponteiro para o tpa, NULL se erro
**********************************************************************************************/
TILE_PANI *tile_pani_cria(short int x, short int y, short int id,
								  short int tatual, short int tempo)
{
	TILE_PANI *t;
	t = malloc(sizeof(TILE_PANI));
	if(!t)
		return NULL;
	t->x = x;
	t->y = y;
	t->id_ani = id;
	t->tatual = tatual;
	t->tempo = tempo;
	t->ant = NULL;
	t->prox = NULL;
	return t;
}

/**********************************************************************************************
	Libera a memória usada pelo tpa

	ENTRADA:	t - ponteiro para a posição do tile animado
	SAÍDA:	nada
**********************************************************************************************/
void tile_pani_destroi(TILE_PANI *t)
{
	if(!t)
		return;
	free(t);
	t = NULL;
}

/**********************************************************************************************
	Coloca na lista de posições de tiles animados

	ENTRADA:	t - tpa a ser colocado
				tpaini - início da lista
	SAÍDA:	nada
**********************************************************************************************/
void tile_pani_adiciona(TILE_PANI *t, TILE_PANI **tpaini)
{
	TILE_PANI *p, *old;
	/* primeiro elemento da lista */
	if(*tpaini == NULL) {
		t->ant = NULL;
		t->prox = NULL;
		*tpaini = t;
		return;
	}
	/* começa no topo da lista */
	p = *tpaini;
	old = NULL;
	while(p) {
		if(p->id_ani < t->id_ani) {
			old = p;
			p = p->prox;
		}
		else {
			if(p->ant) {
				p->ant->prox = t;
				t->prox = p;
				t->ant = p->ant;
				p->ant = t;
				return;
			}
			/* novo primeiro elemento */
			t->prox = p;
			t->ant = NULL;
			p->ant = t;
			*tpaini = t;
			return;
		}
	}
	/* coloca no final da lista */
	old->prox = t;
	t->ant = old;
	t->prox = NULL;
}

/**********************************************************************************************
	Retira o tpa da lista e libera a memória usada

	ENTRADA:	t - tpa a ser apagado
				tpaini - início da lista de tpa
	SAÍDA:	nada
**********************************************************************************************/
void tile_pani_deleta(TILE_PANI *t, TILE_PANI **tpaini)
{
	if(t) {
		if(*tpaini == t) {
			*tpaini = t->prox;
			if(*tpaini)
				(*tpaini)->ant = NULL;
		}
		else {
			t->ant->prox = t->prox;
			if(t->prox)
				t->prox->ant = t->ant;
		}
		tile_pani_destroi(t);
	}
}

/**********************************************************************************************
	Procura pelo tpa da posição x, y

	ENTRADA:	tpaini - início da lista de tpa
				x, y - posição do tpa
	SAÍDA:	ponteiro para o tpa, NULL se não encontrou
**********************************************************************************************/
TILE_PANI *tile_pani_procura_xy(TILE_PANI **tpaini, short int x, short int y)
{
	TILE_PANI *p;
	p = *tpaini;
	while(p) {
		if( (p->x == x) && (p->y == y) )
			return p;
		p = p->prox;
	}
	return NULL;
}

/**********************************************************************************************
	Salva o tpa em arquivo

	ENTRADA:	t - tpa a ser salvo
				f - arquivo já aberto onde será salvo
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int tile_pani_salva(TILE_PANI *t, PACKFILE *f)
{
	pack_fwrite(t, sizeof(TILE_PANI), f);
	return 0;
}

/**********************************************************************************************
	Carrega um tpa do arquivo

	ENTRADA:	f - arquivo já aberto
	SAÍDA:	ponteiro para o tpa, NULL se erro
**********************************************************************************************/
TILE_PANI *tile_pani_abre(PACKFILE *f)
{
	TILE_PANI *t, tmp;
	pack_fread(&tmp, sizeof(TILE_PANI), f);
	t = tile_pani_cria(tmp.x, tmp.y, tmp.id_ani, tmp.tatual, tmp.tempo);
	if(!t)
		return NULL;
	return t;
}