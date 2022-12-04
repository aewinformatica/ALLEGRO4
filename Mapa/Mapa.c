#include <stdlib.h>
#include <string.h>
#include "allegro.h"
#include	"mapa.h"

/***********************
 * Funções para o mapa *
 ***********************/


/**********************************************************************************************
	Aloca memória para um novo mapa e seta o tamanho do campo de visão

	ENTRADA:	vw, vh - tamanho do campo de visão
				w, h - tamanho total do mapa
	SAíDA:	ponteiro para um novo mapa, NULL em caso de erro
**********************************************************************************************/
MAPA *mapa_cria(int w, int h, int vw, int vh)
{
	MAPA *m;
	m = malloc(sizeof(MAPA));
	if(!m)
		return NULL;
	m->w = w;
	m->h = h;
	m->x1 = 0;
	m->y1 = 0;
	m->vw = vw;
	m->vh = vh;
	m->nlayers = 0;
	m->layers = NULL;
	m->df_carregado = 0;
	return m;
}

/**********************************************************************************************
	Libera memória usada pelo mapa

	ENTRADA:	m - ponteiro para o mapa
	SAÍDA:	nada
**********************************************************************************************/
void mapa_destroi(MAPA *m)
{
	LAYER *p, **l;
	if(m->df_carregado)
		layer_descarrega_lista_df(&m->layers);
	for(*l = m->layers; *l != NULL; ) {
		p = (*l)->prox;
		layer_destroi(*l);
		*l = p;
	}
	free(m);
	m = NULL;
}

/**********************************************************************************************
	Adiciona um layer ao mapa

	ENTRADA:	m - ponteiro para o mapa
				id - nº de identificação do layer. Se houver outro com o mesmo id, não coloca
				tipo - tipo do layer (tile, sprite, etc)
				prioridade - usada para definir qual layer desenhar primeiro
	SAÍDA:	0 se tudo bem, 1 se erro
**********************************************************************************************/
int mapa_poe_layer(MAPA *m, short int id, unsigned char tipo, short int prioridade, short int ndf)
{
	LAYER *l;
	/* se o id já existe, retorna 1 */
	if(layer_procura_id(&m->layers, id) != NULL)
		return 1;
	/* se for do tipo obstmap e já existe um, retorna 1 */
	if( tipo == L_T_OBSTMAP && mapa_existe_obstmap(m) )
		return 1;
	/* cria o layer */
	l = layer_cria(id, tipo, prioridade, m->w, m->h, ndf);
	/* se não conseguiu criar o layer, retorna 1 */
	if(l == NULL)
		return 1;
	/* põe o layer na lista */
	layer_adiciona(l, &m->layers);
	m->nlayers++;
	return 0;
}

/**********************************************************************************************
	Retorna um ponteiro para um layer com o id escolhido

	ENTRADA:	m - ponteiro para o mapa
				id - id do layer que será procurado
	SAÍDA:	ponteiro para o layer se ele encontrou o id, NULL em caso de não ter
				encontrado
**********************************************************************************************/
LAYER	*mapa_pega_layer(MAPA *m, short int id)
{
	return layer_procura_id(&m->layers, id);
}

/**********************************************************************************************
	Aloca memória para os dados do layer indicado

	ENTRADA:	m - ponteiro para o mapa onde está o layer
				id - nº de id do layer
				tw, th - largura e altura dos tiles em pixels, se for o caso
				v - usa perspectiva isométrica (tilemap, não implementado) ou
				valor padrão (obstmap)
	SAÍDA:	0 se OK, 1 se erro
**********************************************************************************************/
int mapa_cria_dados(MAPA *m, short int id, short int tw, short int th, unsigned char v)
{
	LAYER *l;
	l = mapa_pega_layer(m, id);
	if(!l)		/* Layer não encontrado */
		return 1;
	return layer_cria_dados(l, tw, th, v);
}

/**********************************************************************************************
	Põe dados novos no layer (deve ser do mesmo tipo do layer)

	ENTRADA:	m - ponteiro para o mapa
				id - nº de id do layer
				d - ponteiro para os dados a serem colocados
	SAÍDA:	0 se OK, 1 se erro
**********************************************************************************************/
int mapa_poe_dados(MAPA *m, short int id, void *d)
{
	LAYER *l;
	l = mapa_pega_layer(m, id);
	if(!l)		/* Layer não encontrado */
		return 1;
	layer_poe_dados(l, d);
	return 0;
}

/**********************************************************************************************
	Retorna o conteúdo do layer

	ENTRADA:	m - ponteiro para o mapa
				id - nº de id do layer
	SAÍDA:	ponteiro para os dados se OK, NULL se erro
**********************************************************************************************/
void *mapa_pega_dados(MAPA *m, short int id)
{
	LAYER *l;
	l = mapa_pega_layer(m, id);
	if(!l)		/* Layer não encontrado */
		return NULL;
	return layer_pega_dados(l);
}

/**********************************************************************************************
	Muda a prioridade de um dos layer do mapa

	ENTRADA:	m - ponteiro para o mapa
				id - id do layer
				np - novo valor da prioridade
	SAÍDA:	nada
**********************************************************************************************/
void mapa_muda_prioridade_layer(MAPA *m, short int id, short int np)
{
	LAYER *l;
	l = layer_procura_id(&m->layers, id);
	if(l == NULL)
		return;
	layer_muda_prioridade(&m->layers, l, np);
}

/**********************************************************************************************
	Apaga um layer da lista e libera a memória

	ENTRADA:	m - ponteiro para o mapa
				id - id do layer a ser apagado
	SAÍDA:	nada
**********************************************************************************************/
void mapa_destroi_layer(MAPA *m, short int id)
{
	LAYER *l;
	l = mapa_pega_layer(m, id);
	if(l == NULL)
		return;
	layer_deleta(l, &m->layers);
	m->nlayers--;
}

/**********************************************************************************************
	Salva o mapa em um arquivo

	ENTRADA:	m - ponteiro para o mapa
				arquivo - nome do arquivo
	SAÍDA:	0 se OK, 1 se deu erro
**********************************************************************************************/
int mapa_salva(MAPA *m, char *arquivo)
{
	PACKFILE *f;
	LAYER *l;
	f = pack_fopen(arquivo, F_WRITE_PACKED);
	if(!f) {
		return 1;
	}
	pack_iputl(AL_ID('M', 'A', 'P', 'A'), f);
	pack_fwrite(m, sizeof(MAPA), f);
	/* salva os layers */
	for(l = m->layers; l; l = l->prox) {
		if(layer_salva(l, f)) {
			pack_fclose(f);
			return 1;
		}
	}
	pack_fclose(f);
	return 0;
}

/**********************************************************************************************
	Abre o arquivo salvo

	ENTRADA:	arquivo - nome do arquivo
	SAÍDA:	ponteiro para o mapa, ou NULL em caso de erro
**********************************************************************************************/
MAPA *mapa_abre(char *arquivo)
{
	PACKFILE *f;
	MAPA *m, t;
	LAYER *l;
	f = pack_fopen(arquivo, F_READ_PACKED);
	if(!f) {
		return NULL;
	}
	if(pack_igetl(f) != AL_ID('M', 'A', 'P', 'A')) {
		pack_fclose(f);
		return NULL;
	}
	pack_fread(&t, sizeof(MAPA), f);
	/* cria o novo mapa */
	m = mapa_cria(t.w, t.h, t.vw, t.vh);
	if(!m) {
		pack_fclose(f);
		return NULL;
	}
	/* move para ponto onde foi salvo o mapa */
	mapa_move_visao(m, t.x1, t.y1);
	/* lê os layers */
	for(; t.nlayers; t.nlayers--) {
		l = layer_abre(f);
		if(!l) {
			mapa_destroi(m);
			pack_fclose(f);
			return NULL;
		}
		layer_adiciona(l, &m->layers);
		m->nlayers++;
	}
	pack_fclose(f);
	return m;
}

/**********************************************************************************************
	Atualiza as animações do mapa inteiro

	ENTRADA:	m - ponteiro para o mapa
				tempo - tempo decorrido desde a última chamada, em quadros (se o seu programa
				usa 30 quadros por segundo, passando 15 indica 0.5 segundos)
	SAÍDA:	nada
**********************************************************************************************/
void mapa_atualiza(MAPA *m, int tempo)
{
	LAYER *p;
	p = m->layers;
	while(p) {
		layer_atualiza(p, tempo);
		p = p->prox;
	}
}

/**********************************************************************************************
	Desenha o mapa inteiro no bitmap especificado

	ENTRADA:	m - ponteiro para o mapa
				bmp - bitmap onde será desenhado o mapa
				xoff, yoff - ponto onde será iniciado o desenho
	SAÍDA:	nada
**********************************************************************************************/
void mapa_desenha(MAPA *m, BITMAP *bmp, int xoff, int yoff)
{
	LAYER *p;
	int cl, cr, ct, cb;
	cl = bmp->cl;
	cr = bmp->cr;
	ct = bmp->ct;
	cb = bmp->cb;
	/* limpa a área do bitmap que será desenhado */
	rectfill(bmp, xoff, yoff, xoff+m->vw, yoff+m->vh, 0);
	/* p aponta para o primeiro layer */
	p = m->layers;
	while(p) {
		/* desenha p */
		layer_desenha(p, bmp, xoff, yoff, m->x1, m->y1, m->x1+m->vw, m->y1+m->vh);
		/* p aponta para o próximo layer */
		p = p->prox;
	}
	bmp->cl = cl;
	bmp->cr = cr;
	bmp->ct = ct;
	bmp->cb = cb;
}

/**********************************************************************************************
	Altera a janela de visualização do mapa

	ENTRADA:	m - ponteiro para o mapa
				nw, nh - nova largura, altura da janela de visualização
	SAÍDA:	nada
**********************************************************************************************/
void mapa_altera_campo_visao(MAPA *m, int nw, int nh)
{
	if(nw<=0 || nh <=0)
		return;
	m->vw = nw;
	m->vh = nh;
}

/**********************************************************************************************
	Muda o campo de visão do mapa

	ENTRADA:	x1, y1 - novo ponto do canto de cima esquerda do mapa
	SAÍDA:	nada
**********************************************************************************************/
void mapa_muda_visao(MAPA *m, int x1, int y1)
{
	m->x1 = x1;
	m->y1 = y1;
}

/**********************************************************************************************
	Move o campo de visão do mapa

	ENTRADA:	x1, y1 - quantos pontos irá se mover no eixo x e y (negativos = esquerda/cima
				positivos = direita/baixo)
	SAÍDA:	nada
**********************************************************************************************/
void mapa_move_visao(MAPA *m, int x1, int y1)
{
	m->x1 += x1;
	m->y1 += y1;
}

/**********************************************************************************************
	Carrega todos os datafiles dos layers do mapa

	ENTRADA:	m - ponteiro para o mapa
	SAÍDA:	0 se OK, 1 se erro
**********************************************************************************************/
int mapa_carrega_lista_df(MAPA *m)
{
	if(m->df_carregado)
		return 0;
	if(layer_carrega_lista_df(&m->layers)) {
		return 1;
	}
	else {
		m->df_carregado = 1;
		return 0;
	}
}

/**********************************************************************************************
	Põe o nome do arquivo datafile no layer indicado pelo id

	ENTRADA:	m - ponteiro para o mapa
				id - nº de id do layer
				arq - nome do arquivo
	SAÍDA:	0 OK, 1 se não foi possível por não haver mais espaço no vetor de
				nomes de datafiles
**********************************************************************************************/
int mapa_poe_dfnome(MAPA *m, short int id, char *arq)
{
	LAYER *l;
	l = mapa_pega_layer(m, id);
	if(!l)
		return 1;
	return layer_poe_dfnome(l, arq);
}

/**********************************************************************************************
	Retorna o datafile do layer indicado pelo id

	ENTRADA:	m - ponteiro para o mapa
				id - nº de id do layer
				ndf - nº do datafile
	SAÍDA:	ponteiro para o df, NULL se erro
**********************************************************************************************/
DATAFILE	*mapa_pega_df(MAPA *m, short int id, short int ndf)
{
	LAYER *l;
	l = mapa_pega_layer(m, id);
	if(!l)
		return NULL;
	return layer_pega_datafile(l, ndf);
}

/**********************************************************************************************
	Diz se já existe um mapa de obstáculos (só pode ter um)

	ENTRADA:	m - mapa
	SAÍDA:	0 se não, 1 se existe
**********************************************************************************************/
int mapa_existe_obstmap(MAPA *m)
{
	LAYER *l;
	for(l = m->layers; l; l = l->prox)
		if(layer_pega_tipo(l) == L_T_OBSTMAP)
			return 1;
	return 0;
}

/**********************************************************************************************
	Verifica se o ponto colidi com um obstáculo

	ENTRADA:	m - mapa
				x, y - ponto
	SAÍDA:	valor do local do ponto ou 0 se não existe mapa de obstáculos
**********************************************************************************************/
unsigned char mapa_colisao_ponto(MAPA *m, short int x, short int y)
{
	LAYER *l;
	OBSTMAP *o;
	unsigned char r;
	for(l = m->layers; l; l = l->prox) {
		if(layer_pega_tipo(l) == L_T_OBSTMAP) {
			o = (OBSTMAP *)layer_pega_dados(l);
			obstmap_colisao_ponto(o, x, y, &r);
			return r;
		}
	}
	return 0;
}

/**********************************************************************************************
	Verifica se a área colidi com um obstáculo

	ENTRADA:	m - mapa
				x, y, w, h - ponto
	SAÍDA:	valor do local do ponto ou 0 se não existe mapa de obstáculos
**********************************************************************************************/
unsigned char mapa_colisao_area(MAPA *m, short int x, short int y, short int w, short int h)
{
	LAYER *l;
	OBSTMAP *o;
	unsigned char r;
	for(l = m->layers; l; l = l->prox) {
		if(layer_pega_tipo(l) == L_T_OBSTMAP) {
			o = (OBSTMAP *)layer_pega_dados(l);
			obstmap_colisao_area(o, x, y, w, h, &r);
			return r;
		}
	}
	return 0;
}

/************************
 * Funções para o layer *
 ************************/

/**********************************************************************************************
	Aloca memória para um layer

	ENTRADA:	id - id do novo layer
				tipo - tipo do layer
				prioridade - usada para estabelecer a ordem para desenhar
				w, h - tamanho do layer
				ndf - nº de datafiles usado pelo layer (deve ser sempre 1 se for tilemap)
	SAÍDA:	ponteiro para o novo layer, NULL em caso de falta de memória
**********************************************************************************************/
LAYER	*layer_cria(short int id, unsigned char tipo, short int prioridade, int w, int h, short int ndf)
{
	LAYER *l;
	int i;
	/* aloca a memória */
	l = malloc(sizeof(LAYER));
	if(l == NULL)
		return NULL;
	l->id = id;
	l->tipo = tipo;
	l->prioridade = prioridade;
	l->w = w;
	l->h = h;
	l->status = 0;
	switch(tipo) {
		case L_T_TILEMAP :
			l->dat.dat_t = NULL;
			break;
		case L_T_SPRITEMAP :
			l->dat.dat_s = NULL;
			break;
		case L_T_OBSTMAP :
			l->dat.dat_o = NULL;
		default :
			l->dat.dat_t = NULL;
			break;
	}
	l->df = malloc(ndf * sizeof(DATAFILE *));
	if(!l->df) {
		free(l);
		return NULL;
	}
	l->dfnome = malloc(ndf * sizeof(char *));
	if(!l->dfnome) {
		free(l->df);
		free(l->dfnome);
		return NULL;
	}
	for(i=0; i<ndf; i++) {
		l->dfnome[i] = malloc(sizeof(char)*DFNOME_TAM);
		if(!l->dfnome[i]) {
			for(--i; i>=0; i--) {
				free(l->dfnome[i]);
			}
			free(l->dfnome);
			free(l->df);
			free(l);
			return NULL;
		}
		l->df[i] = NULL;
	}
	l->ndf = ndf;
	l->prox_df = 0;
	l->prox = l->ant = NULL;
	/* retorna o layer criado */
	return l;
}

/**********************************************************************************************
	Libera a memória usada pelo layer e seu conteúdo

	ENTRADA:	l - layer a ser destruído
	SAÍDA:	nada
**********************************************************************************************/
void layer_destroi(LAYER *l)
{
	short int i;
	if(l == NULL)
		return;
	switch(layer_pega_tipo(l)) {
		case L_T_TILEMAP : 
			if(l->dat.dat_t)
				tilemap_destroi(l->dat.dat_t);
			break;
		case L_T_SPRITEMAP :
			if(l->dat.dat_s)
				spritemap_destroi(l->dat.dat_s);
			break;
		case L_T_OBSTMAP :
			if(l->dat.dat_o)
				obstmap_destroi(l->dat.dat_o);
			break;
		default : break;
	}
	free(l);
	free(l->df);
	for(i=0; i<l->ndf; i++)
		free(l->dfnome[i]);
	free(l->dfnome);
	l = NULL;
}

/**********************************************************************************************
	Coloca o layer na lista

	ENTRADA:	l - layer a ser apagado
				lini - início da lista de layers
	SAÍDA:	nada
**********************************************************************************************/
void layer_adiciona(LAYER *l, LAYER **lini)
{
	LAYER *p, *old;
	/* primeiro elemento da lista */
	if(*lini == NULL) {
		l->ant = NULL;
		l->prox = NULL;
		*lini = l;
		return;
	}
	/* começa no topo da lista */
	p = *lini;
	old = NULL;
	while(p) {
		if(p->prioridade < l->prioridade) {
			old = p;
			p = p->prox;
		}
		else {
			if(p->ant) {
				p->ant->prox = l;
				l->prox = p;
				l->ant = p->ant;
				p->ant = l;
				return;
			}
			/* novo primeiro elemento */
			l->prox = p;
			l->ant = NULL;
			p->ant = l;
			*lini = l;
			return;
		}
	}
	/* coloca no final da lista */
	old->prox = l;
	l->ant = old;
	l->prox = NULL;
}

/**********************************************************************************************
	Retira o layer da lista e libera a memória alocada para ele

	ENTRADA:	l - layer a ser apagado
				lini - início da lista
	SAÍDA:	nada
**********************************************************************************************/
void layer_deleta(LAYER *l, LAYER **lini)
{
	if(l) {
		if(*lini == l) {
			*lini = l->prox;
			if(*lini)
				(*lini)->ant = NULL;
		}
		else {
			l->ant->prox = l->prox;
			if(l->prox)
				l->prox->ant = l->ant;
		}
		layer_destroi(l);
	}
}

/**********************************************************************************************
	Cria os dados do layer

	ENTRADA:	l - layer onde serão colocados os dados
				tw, th - tamanho dos tiles, se for o caso
				v - usa perspectiva isométrica (tilemap, não implementado) ou
				valor padrão (obstmap)
	SAÍDA:	0 se OK, 1 se erro
**********************************************************************************************/
int layer_cria_dados(LAYER *l, int tw, int th, unsigned char v)
{
	switch(layer_pega_tipo(l)) {
		case L_T_TILEMAP :
			l->dat.dat_t = tilemap_cria(l->w/tw, l->h/th, tw, th, v);
			if(l->dat.dat_t == NULL)
				return 1;
			break;
		case L_T_SPRITEMAP :
			l->dat.dat_s = spritemap_cria(l->w, l->h);
			if(l->dat.dat_s == NULL)
				return 1;
			break;
		case L_T_OBSTMAP :
			l->dat.dat_o = obstmap_cria(l->w, l->h, v);
			if(l->dat.dat_o == NULL)
				return 1;
			break;
		default : break;
	}
	return 0;
}

/**********************************************************************************************
	Coloca dados no layer

	ENTRADA:	l - layer onde serão colocados os dados
				d - ponteiro para os dados
	SAÍDA:	nada
**********************************************************************************************/
void layer_poe_dados(LAYER *l, void *d)
{
	switch(layer_pega_tipo(l)) {
		case L_T_TILEMAP :
			if(l->dat.dat_t)
				tilemap_destroi(l->dat.dat_t);
			l->dat.dat_t = (TILEMAP *)d;
			break;
		case L_T_SPRITEMAP :
			if(l->dat.dat_s)
				spritemap_destroi(l->dat.dat_s);
			l->dat.dat_s = (SPRITEMAP *)d;
			break;
		case L_T_OBSTMAP :
			if(l->dat.dat_o)
				obstmap_destroi(l->dat.dat_o);
			l->dat.dat_o = (OBSTMAP *)d;
			break;
		default :
			break;
	}
}

/**********************************************************************************************
	Pega os dados do layer

	ENTRADA:	l - layer do qual se quer os dados
	SAÍDA:	ponteiro para os dados
**********************************************************************************************/
void *layer_pega_dados(LAYER *l)
{
	switch(layer_pega_tipo(l)) {
		case L_T_TILEMAP :
			return (void *)l->dat.dat_t;
		case L_T_SPRITEMAP :
			return (void *)l->dat.dat_s;
		case L_T_OBSTMAP :
			return (void *)l->dat.dat_o;
		default :
			return NULL;
	}
}

/**********************************************************************************************
	Destroi os dados do layer e libera a memória

	ENTRADA:	l - layer onde estão os dados
	SAÍDA:	nada
**********************************************************************************************/
void layer_destroi_dados(LAYER *l)
{
	switch(layer_pega_tipo(l)) {
		case L_T_TILEMAP : 
			tilemap_destroi(l->dat.dat_t);
			break;
		case L_T_SPRITEMAP :
			spritemap_destroi(l->dat.dat_s);
			break;
		case L_T_OBSTMAP :
			obstmap_destroi(l->dat.dat_o);
			break;
		default : break;
	}
}

/**********************************************************************************************
	Altera a prioridade do layer e coloca na posição correta

	ENTRADA:	lini - início da lista de layers
				l - layer a ser alterado a prioridade
				p - novo valor da prioridade
	SAÍDA:	nada
**********************************************************************************************/
void layer_muda_prioridade(LAYER **lini, LAYER *l, short int p)
{
	LAYER *i;
	/* procura pelo layer na lista */
	i = *lini;
	while(i && i != l) {
		i = i->prox;
	}
	/* se o layer não está na lista retorna */
	if(i == NULL)
		return;
	/* altera a prioridade e coloca na posição correspondente */
	l->prioridade = p;
	while( (p < layer_pega_prioridade(l->ant)) && (l->ant != NULL) ) {
		i = l->ant;
		/* se for o primeiro, atualiza a posição */
		if(*lini == i) {
			*lini = l;
		}
		if(i->ant != NULL)
			i->ant->prox = l;
		l->ant = i->ant;
		i->ant = l;
		if(l->prox != NULL)
			l->prox->ant = i;
		i->prox = l->prox;
		l->prox = i;
	}
	while( (p > layer_pega_prioridade(l->prox)) && (l->prox != NULL) ) {
		/* se for o primeiro, atualiza posição */
		if(*lini == l) {
			*lini = i;
		}
		i = l->prox;
		if(i->prox != NULL)
			i->prox->ant = l;
		l->prox = i->prox;
		i->prox = l;
		if(l->ant != NULL)
			l->ant->prox = i;
		i->ant = l->ant;
		l->ant = i;
	}
}

/**********************************************************************************************
	Retorna o valor da prioridade do layer

	ENTRADA:	layer do qual se quer a prioridade
	SAÍDA:	valor da prioridade
**********************************************************************************************/
short	int layer_pega_prioridade(LAYER *l)
{
	return l->prioridade; 
}

/**********************************************************************************************
	Retorna o id do layer

	ENTRADA:	layer do qual se quer o id
	SAÍDA:	valor do id
**********************************************************************************************/
short	int layer_pega_id(LAYER *l)
{
	return l->id;
}

/**********************************************************************************************
	Retorna o layer com o id especificado

	ENTRADA:	lini - lista de layers
				id - id a ser procurado
	SAÍDA:	ponteiro para o layer, ou NULL se não encontrou
**********************************************************************************************/
LAYER	*layer_procura_id(LAYER **lini, short int id)
{
	LAYER *p;
	p = *lini;
	while(p) {
		if( layer_pega_id(p) == id )
			return p;
		p = p->prox;
	}
	return NULL;
}

/**********************************************************************************************
	Retorna o tipo do layer

	ENTRADA:	l - layer do qual se quer o tipo
	SAÍDA:	o tipo do layer
**********************************************************************************************/
unsigned	char layer_pega_tipo(LAYER *l)
{
	return l->tipo;
}

/**********************************************************************************************
	Altera o tipo do layer e apaga as informações anteriores

	ENTRADA:	l - layer que se quer alterar o tipo
				tipo - novo tipo
	SAÍDA:	nada
**********************************************************************************************/
void layer_muda_tipo(LAYER *l, unsigned char tipo)
{
	/* se os tipos são iguais, não faz nada */
	if(layer_pega_tipo(l) != tipo) {
		switch(layer_pega_tipo(l)) {
			case L_T_TILEMAP :
				tilemap_destroi(l->dat.dat_t);
				break;
			case L_T_SPRITEMAP :
				spritemap_destroi(l->dat.dat_s);
				break;
			case L_T_OBSTMAP :
				obstmap_destroi(l->dat.dat_o);
				break;
			default :
				break;
		}
		l->tipo = tipo;
	}
}

/**********************************************************************************************
	Atualiza as informações das animações em seu layer

	ENTRADA:	l - layer com as animações
				tempo - tempo decorrido desde a última chamada, em quadros (se o seu programa
				usa 30 quadros por segundo, passando 15 indica 0.5 segundos)
	SAÍDA:	nada
**********************************************************************************************/
void layer_atualiza(LAYER *l, int tempo)
{
	switch(layer_pega_tipo(l)) {
		case L_T_TILEMAP :
			tilemap_atualiza(l->dat.dat_t, tempo);
			break;
		case L_T_SPRITEMAP :
			spritemap_atualiza(l->dat.dat_s, tempo);
			break;
		default :
			break;
	}
}

/**********************************************************************************************
	Desenha o conteúdo do layer no bitmap

	ENTRADA:	l - layer a ser desenhado
				bmp - bitmap onde será desenhado
				xoff, yoff - ponto onde se iniciará o desenho
				modo - com ou sem transparência
				x1, y1, x2, y2 - área do layer que deve ser desenhada
	SAÍDA:	nada
**********************************************************************************************/
void layer_desenha(LAYER *l, BITMAP *bmp, int xoff, int yoff, int x1, int y1,
						 int x2, int y2)
{
	/* o layer está desativado, portanto não precisa desenhar */
	if(l->status & L_S_DESATIVADO)
		return;
	switch(layer_pega_tipo(l)) {
		/* os tilemaps só podem ter um datafile associado a ele */
		case L_T_TILEMAP :
			tilemap_desenha(l->dat.dat_t, bmp, xoff, yoff, x1, y1,
								 x2, y2, layer_pega_datafile(l, 0));
			break;
		/* os spritemaps podem ter vários datafiles em cada layer */
		case L_T_SPRITEMAP :
			spritemap_desenha(l->dat.dat_s, bmp, xoff, yoff, x1, y1, x2, y2, l->df);
			break;
		case L_T_OBSTMAP :
			obstmap_desenha(l->dat.dat_o, bmp, xoff, yoff, x1, y1, x2, y2);
		default : break;
	}
}

/**********************************************************************************************
	Salva o layer em disco

	ENTRADA:	l - layer a ser salvo
				f - arquivo já aberto onde será salvo
	SAÍDA:	0 se OK, 1 se deu erro
**********************************************************************************************/
int layer_salva(LAYER *l, PACKFILE *f)
{
	short int i;
	pack_fwrite(l, sizeof(LAYER), f);
	for(i=0; i<l->prox_df; i++) {
		pack_fwrite(l->dfnome[i], sizeof(char)*DFNOME_TAM, f);
	}
	switch(layer_pega_tipo(l)) {
		case L_T_TILEMAP :
			return tilemap_salva(l->dat.dat_t, f);
		case L_T_SPRITEMAP :
			return spritemap_salva(l->dat.dat_s, f);
		case L_T_OBSTMAP :
			return obstmap_salva(l->dat.dat_o, f);
		default :
			break;
	}
	return 0;
}

/**********************************************************************************************
	Abre o layer do disco

	ENTRADA:	f - arquivo já aberto onde se encontra o layer
	SAÍDA:	ponteiro para o layer, ou NULL em caso de erro
**********************************************************************************************/
LAYER	*layer_abre(PACKFILE *f)
{
	char s[DFNOME_TAM];
	short int i;
	LAYER *l, t;
	pack_fread(&t, sizeof(LAYER), f);
	l = layer_cria(t.id, t.tipo, t.prioridade, t.w, t.h, t.ndf);
	if(!l) {
		return NULL;
	}
	for(i=0; i<t.prox_df; i++) {
		pack_fread(s, sizeof(char)*DFNOME_TAM, f);
		layer_poe_dfnome(l, s);
	}
	switch(layer_pega_tipo(&t)) {
		case L_T_TILEMAP :
			l->dat.dat_t = tilemap_abre(f);
			if(!l->dat.dat_t) {
				layer_destroi(l);
				return NULL;
			}
			break;
		case L_T_SPRITEMAP :
			l->dat.dat_s = spritemap_abre(f);
			if(!l->dat.dat_s) {
				layer_destroi(l);
				return NULL;
			}
			break;
		case L_T_OBSTMAP :
			l->dat.dat_o = obstmap_abre(f);
			if(!l->dat.dat_o) {
				layer_destroi(l);
				return NULL;
			}
			break;
		default :
			break;
	}
	l->status = t.status;
	return l;
}

/**********************************************************************************************
	Carrega todos os datafiles na memória

	ENTRADA:	lini - início da lista com os datafiles
	SAÍDA:	0 se OK, 1 se erro
**********************************************************************************************/
int layer_carrega_lista_df(LAYER **lini)
{
	LAYER *l, *p;
	short int ndf, i;
	unsigned char achou_igual;
	for(l = *lini; l; l = l->prox) {
		for(i=0; i<l->prox_df; i++) {
			achou_igual=0;
			for(p = l->ant; p && !achou_igual; p = p->ant) {
				ndf = layer_compara_dfnome(p, l->dfnome[i]);
				if(ndf) {
					achou_igual = 1;
					l->df[i] = layer_pega_datafile(p, ndf);
				}
			}
			if(!achou_igual) {
				l->df[i] = load_datafile(l->dfnome[i]);
			}
			/* em caso de erro, libera toda a memória dos df's e retorna */
			if(!l->df[i]) {
				for(p = l->ant; p; p = p->ant) {
					for(i=0; i<p->prox_df; i++) {
						unload_datafile(p->df[i]);
					}
				}
				return 1;
			}
		}
	}
	return 0;
}

/**********************************************************************************************
	Libera a memória alocada pelos datafiles da lista de layers

	ENTRADA:	lini - início da lista com os datafiles
	SAÍDA:	nada
**********************************************************************************************/
void layer_descarrega_lista_df(LAYER **lini)
{
	LAYER *p, *l;
	short int ndf, i;
	unsigned char achou_igual;

	for(l = *lini; l; l = l->prox) {
		for(i=0; i<l->prox_df; i++) {
			achou_igual=0;
			for(p = l->ant; p && !achou_igual; p = p->ant) {
				ndf = layer_compara_dfnome(p, l->dfnome[i]);
				/* se encontrou um df igual antes, já foi liberada a memória */
				if(ndf) {
					achou_igual = 1;
				}
			}
			/* se não encontrou, libera a memória */
			if(!achou_igual) {
				unload_datafile(l->df[i]);
			}
			l->df[i] = NULL;
		}
	}
}

/**********************************************************************************************
	Copia o nome do arquivo no layer

	ENTRADA:	l - layer onde será copiado o nome
				arq - nome do arquivo
	SAÍDA:	0 se ok, 1 se não deu
**********************************************************************************************/
int layer_poe_dfnome(LAYER *l, char *arq)
{
	if(l->prox_df == l->ndf)
		return 1;
	strcpy(l->dfnome[l->prox_df], arq);
	l->prox_df++;
	return 0;
}

/**********************************************************************************************
	Retorna o nome do datafile deste layer

	ENTRADA:	l - layer onde está o nome
				ndf - nº da posição do df no vetor
	SAÍDA:	string com o nome do arquivo
**********************************************************************************************/
char *layer_pega_dfnome(LAYER *l, short int ndf)
{
	return l->dfnome[ndf];
}

/**********************************************************************************************
	Diz se algum datafile do layer tem o mesmo nome de arq

	ENTRADA:	l - layer onde está o nome
				arq - nome para ver se é igual
	SAÍDA:	0 se diferente, índice no vetor se igual
**********************************************************************************************/
int layer_compara_dfnome(LAYER *l, char *arq)
{
	short int i;
	for(i=0; i<l->prox_df; i++) {
		if(!stricmp(layer_pega_dfnome(l, i), arq))
			return i;
	}
	return 0;
}

/**********************************************************************************************
	Retorna o endereço da memória onde está o datafile carregado

	ENTRADA:	l - layer onde está o datafile
				ndf - nº do índice do df no vetor
	SAÍDA:	ponteiro para o datafile
**********************************************************************************************/
DATAFILE	*layer_pega_datafile(LAYER *l, short int ndf)
{
	return l->df[ndf];
}