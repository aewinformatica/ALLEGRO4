#include <stdlib.h>
#include <string.h>
#include "allegro.h"
#include "mapa.h"

/**********************************************************************************************
	Aloca a memória usada pelo mapa de obstáculos

	ENTRADA:	x, y - tamanho do mapa
				valor_padrao - valor do ponto se nenhum obstáculo colidiu com ele
	SAÍDA:	ponteiro para o obstmap se ok, NULL se erro
**********************************************************************************************/
OBSTMAP *obstmap_cria(int x, int y, unsigned char valor_padrao)
{
	OBSTMAP *o;
	o = (OBSTMAP *)malloc(sizeof(OBSTMAP));
	if(!o)
		return NULL;
	o->w = x;
	o->h = y;
	o->valor = valor_padrao;
	o->no = 0;
	o->lista = (OBSTACULO *)NULL;
	return o;
}

/**********************************************************************************************
	Libera a memória usada pelo mapa e por seus obstáculos

	ENTRADA:	o - ponteiro para o obstmap
	SAÍDA:	nada
**********************************************************************************************/
void obstmap_destroi(OBSTMAP *o)
{
	OBSTACULO *ob, *p;
	for(ob = o->lista; ob; ob = p) {
		p = ob->prox;
		obstaculo_destroi(ob);
	}
	free(o);
	o = NULL;
}

/**********************************************************************************************
	Cria um obstáculo com a área, o id e o valor passado (o id pode ser repetido)

	ENTRADA:	o - mapa de obstáculos
				id - id do obstáculo (pode repetir)
				x, y, w, h - área do obstáculo
				d - valor daquela área
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int obstmap_poe_obstaculo(OBSTMAP *o, short int id, short int x, short int y, short int w, short int h, unsigned char d)
{
	OBSTACULO *ob;
	ob = obstaculo_cria(id, x, y, w, h, d);
	if(!ob)
		return 1;
	obstaculo_adiciona(ob, &o->lista);
	o->no++;
	return 0;
}

/**********************************************************************************************
	Retorna os obstáculos com o id especificado.
	ATENÇÃO: a memória usada pelo vetor de ponteiros deve ser liberada manualmente
	usando-se free(nome_da_var);, onde nome_da_var é OBSTACULO **. Isto acontece
	porque pode ter vários obstáculos com o mesmo id.

	ENTRADA:	o - mapa de obstáculos
				id - id dos obstáculos
				no - número de obstáculos retornados
	SAÍDA:	NULL se não encontrou ou vetor de ponteiros de tamanho no
**********************************************************************************************/
OBSTACULO **obstmap_pega_obstaculo(OBSTMAP *o, short int id, short int *no)
{
	OBSTACULO *ob, *t, **r;
	/* primeiro, conta quantos obstáculos existem */
	*no = 0;
	ob = o->lista;
	while(ob) {
		t = obstaculo_procura_id(&ob, id);
		ob = t;
		if(t)
			(*no)++;
	}
	if(*no == 0)
		return NULL;
	/* depois, aloca a memória para o vetor */
	r = (OBSTACULO **)malloc(sizeof(OBSTACULO *)*(*no));
	/* por último, coloca os obstáculos no vetor e retorna */
	*no = 0;
	ob = o->lista;
	while(ob) {
		t = obstaculo_procura_id(&ob, id);
		ob = t;
		if(t) {
			r[*no] = t;
			(*no)++;
		}
	}
	return r;
}

/**********************************************************************************************
	Libera a memória alocada pelos obstáculos com valor de id igual ao passado para
	a função.

		ENTRADA:	o - mapa de obstáculos
					id - id dos obstáculos a serem apagados
		SAÍDA:	nada
**********************************************************************************************/
void obstmap_destroi_obstaculo(OBSTMAP *o, short int id)
{
	OBSTACULO *ob;
	for(ob = obstaculo_procura_id(&o->lista, id); ob; ob = obstaculo_procura_id(&o->lista, id)) {
		obstaculo_deleta(ob, &o->lista);
		o->no--;
	}
}

/**********************************************************************************************
	Altera o valor dos obstáculos

	ENTRADA:	o - mapa de obstáculos
				id - id dos obstáculos
				nv - novo valor
	SAÍDA:	nada
**********************************************************************************************/
void obstmap_muda_obstaculo(OBSTMAP *o, short int id, unsigned char nv)
{
	OBSTACULO **r;
	short int no, i;
	r = obstmap_pega_obstaculo(o, id, &no);
	if(no == 0)
		return;
	for(i=0; i<no; i++) {
		r[i]->valor = nv;
	}
	/* liberando a memória manualmente */
	free(r);
}

/**********************************************************************************************
	Move os obstáculos com o id passado x pontos na horizontal e y na vertical

	ENTRADA:	o - mapa de obstáculos
				id - id dos obstáculos
				x, y - quantidade de pontos para se movimentar
	SAÍDA:	nada
**********************************************************************************************/
void obstmap_move_obstaculo(OBSTMAP *o, short int id, short int x, short int y)
{
	OBSTACULO **ob;
	short int no, i;
	r = obstmap_pega_obstaculo(o, id, &no);
	if(no == 0)
		return;
	for(i=0; i<no; i++) {
		r[i]->x += x;
		r[i]->y += y;
	}
	/* liberando a memória manualmente */
	free(r);
}

/**********************************************************************************************
	Percorre todos os obstáculos até encontrar um que o ponto esteja dentro

	ENTRADA:	o - mapa de obstáculos
				x, y - coordenadas do ponto
				v - valor retornado
	SAÍDA:	0 se não colidiu e v é colocado no valor padrão, ou 1 se colidiu
				e v é colocado no valor do obstáculo
**********************************************************************************************/
int obstmap_colisao_ponto(OBSTMAP *o, short int x, short int y, unsigned char *v)
{
	OBSTACULO *ob;
	*v = o->valor;
	for(ob = o->lista; ob; ob = ob->prox)
		if(obstaculo_colisao_ponto(ob, x, y, v))
			return 1;
	return 0;
}

/**********************************************************************************************
	Percorre todos os obstáculos até encontrar um em que o retângulo delimitado por
	x, y, w, h colida com um dos obstáculos

	ENTRADA:	o - mapa de obstáculos
				x, y, w, h - retângulo delimitador da área
				v - valor de retorno
	SAÍDA:	0 se não colidiu e v recebe o valor padrão, ou 1 se colidiu e v recebe
				o valor do obstáculo
**********************************************************************************************/
int obstmap_colisao_area(OBSTMAP *o, short int x, short int y, short int w, short int h, unsigned char *v)
{
	OBSTACULO *ob;
	*v = o->valor;
	for(ob = o->lista; ob; ob = ob->prox)
		if(obstaculo_colisao_area(ob, x, y, w, h, v))
			return 1;
	return 0;
}

/**********************************************************************************************
	Salva o mapa de obstáculos no arquivo

	ENTRADA:	o - mapa de obstáculos
				f - arquivo no ponto onde deverá ser salvo o mapa
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int obstmap_salva(OBSTMAP *o, PACKFILE *f)
{
	OBSTACULO *ob;
	if(pack_fwrite((void *)o, sizeof(OBSTMAP), f) != sizeof(OBSTMAP))
		return 1;
	for(ob = o->lista; ob; ob = ob->prox)
		if(obstaculo_salva(ob, f))
			return 1;
	return 0;
}

/**********************************************************************************************
	Abre o mapa de obstáculos do arquivo

	ENTRADA:	f - arquivo no ponto onde deverá ser aberto o mapa
	SAÍDA:	ponteiro para o mapa se ok, NULL se erro
**********************************************************************************************/
OBSTMAP *obstmap_abre(PACKFILE *f)
{
	OBSTMAP *o, t;
	OBSTACULO *ob;
	int i;
	if(pack_fread(&t, sizeof(OBSTMAP), f) != sizeof(OBSTMAP))
		return NULL;
	o = obstmap_cria(t.w, t.h, t.valor);
	if(o == NULL)
		return NULL;
	for(i=0; i<t.no; i++) {
		ob = obstaculo_abre(f);
		if(ob == NULL) {
			obstmap_destroi(o);
			return NULL;
		}
		obstaculo_adiciona(ob, &o->lista);
		o->no++;
	}
	return o;
}

/**********************************************************************************************
	Desenha o mapa de obstáculos (serve apenas para saber onde eles estão, pois
	não serão desenhados normalmente)

	ENTRADA:	o - mapa
				b - bitmap onde será desenhado
				xoff, yoff - ponto onde se começará a desenhar
				x1, y1, x2, y2 - área que deve ser desenhada
	SAÍDA:	nada
**********************************************************************************************/
void obstmap_desenha(OBSTMAP *o, BITMAP *b, int xoff, int yoff,
							int x1, int y1, int x2, int y2)
{
	OBSTACULO *ob;
	unsigned char *v;
	int cor = makecol(255, 255, 0);
	set_clip(b, xoff, yoff, xoff+x2-x1, yoff+y2-y1);
	for(ob = o->lista; ob; ob = ob->prox) {
		if(obstaculo_colisao_area(ob, x1, y1, x2-x1, y2-y1, v))
			rect(b, xoff-x1+ob->x, yoff-y1+ob->y, xoff-x1+ob->x+ob->w, yoff-y1+ob->y+ob->h, cor);
	}
}

/**********************************************************************************************
	Reserva a memória para um obstáculo

	ENTRADA:	id - id do obstáculo
				x, y, w, h - área reservada para ele
				v - valor daquela área
**********************************************************************************************/
OBSTACULO *obstaculo_cria(short int id, short int x, short int y, short int w, short int h, unsigned char v)
{
	OBSTACULO *ob;
	ob = malloc(sizeof(OBSTACULO));
	if(ob == NULL)
		return NULL;
	ob->id = id;
	ob->x = x;
	ob->y = y;
	ob->w = w;
	ob->h = h;
	ob->valor = v;
	ob->prox = ob->ant = NULL;
	return ob;
}

/*********************************************************************************************
	Libera a memória deste obstáculo

	ENTRADA:	ob - obstáculo
	SAÍDA:	nada
**********************************************************************************************/
void obstaculo_destroi(OBSTACULO *ob)
{
	if(ob != NULL) {
		free(ob);
		ob = NULL;
	}
}

/**********************************************************************************************
	Coloca o obstáculo na lista

	ENTRADA:	ob - obstáculo
				lista - início da lista de obstáculos
	SAÍDA:	nada
**********************************************************************************************/
void obstaculo_adiciona(OBSTACULO *ob, OBSTACULO **lista)
{
	OBSTACULO *p, *old;
	/* primeiro elemento da lista */
	if(*lista == NULL) {
		ob->ant = NULL;
		ob->prox = NULL;
		*lista = ob;
		return;
	}
	/* começa no topo da lista */
	p = *lista;
	old = NULL;
	while(p) {
		if(p->id < ob->id) {
			old = p;
			p = p->prox;
		}
		else {
			if(p->ant) {
				p->ant->prox = ob;
				ob->prox = p;
				ob->ant = p->ant;
				p->ant = ob;
				return;
			}
			/* novo primeiro elemento */
			ob->prox = p;
			ob->ant = NULL;
			p->ant = ob;
			*lista = ob;
			return;
		}
	}
	/* coloca no final da lista */
	old->prox = ob;
	ob->ant = old;
	ob->prox = NULL;
}

/**********************************************************************************************
	Retira da lista um obstáculo e libera a memória

	ENTRADA:	ob - obstáculo
				lista - início da lista de obstáculos
	SAÍDA:	nada
**********************************************************************************************/
void obstaculo_deleta(OBSTACULO *ob, OBSTACULO **lista)
{
	if(ob) {
		if(*lista == ob) {
			*lista = ob->prox;
			if(*lista)
				(*lista)->ant = NULL;
		}
		else {
			ob->ant->prox = ob->prox;
			if(ob->prox)
				ob->prox->ant = ob->ant;
		}
		obstaculo_destroi(ob);
	}
}

/**********************************************************************************************
	Retorna o id do obstáculo

	ENTRADA:	ob - obstáculo
	SAÍDA:	id do obstáculo
**********************************************************************************************/
short int obstaculo_pega_id(OBSTACULO *ob)
{
	return ob->id;
}

/**********************************************************************************************
	Procura na lista um obstáculo com id igual ao passado

	ENTRADA:	lista - início da lista de obstáculos
				id - id a ser procurado
	SAÍDA:	ponteiro para o obstáculo se encontrou igual, NULL se não
**********************************************************************************************/
OBSTACULO *obstaculo_procura_id(OBSTACULO **lista, short int id)
{
	OBSTACULO *ob;
	for(ob = *lista; ob; ob = ob->prox) {
		if(id == obstaculo_pega_id(ob))
			return ob;
		else if(id > obstaculo_pega_id(ob))
			return NULL;
	}
	return NULL;
}

/**********************************************************************************************
	Verifica se o ponto colidi com o obstáculo

	ENTRADA:	ob - obstáculo
				x, y - ponto
				v - valor retornado
	SAÍDA:	0 se não colidi e v não é alterado, ou 1 se colidi e v recebe o valor
				do obstáculo
**********************************************************************************************/
int obstaculo_colisao_ponto(OBSTACULO *ob, short int x, short int y, unsigned char *v)
{
	if( _entre(x, ob->x, ob->x+ob->w) && _entre(y, ob->y, ob->y+ob->h) ) {
		*v = ob->valor;
		return 1;
	}
	return 0;
}

/**********************************************************************************************
	Verifica se a área colidi com o obstáculo

	ENTRADA:	ob - obstáculo
				x, y, w, h - área
				v - valor retornado
	SAÍDA:	0 se não colidi e v não é alterado, ou 1 se colidi e v recebe o valor
				do obstáculo
**********************************************************************************************/
int obstaculo_colisao_area(OBSTACULO *ob, short int x, short int y, short int w, short int h, unsigned char *v)
{
	int x2, y2;
	x2 = x+w;
	y2 = y+h;
	if((_entre(ob->x, x, x2) || _entre(ob->x+ob->w, x, x2) || _entre(x, ob->x, ob->x+ob->w) || _entre(x2, ob->x, ob->x+ob->w))
		&& (_entre(ob->y, y, y2) || _entre(ob->y+ob->h, y, y2) || _entre(y, ob->y, ob->y+ob->h) || _entre(y2, ob->y, ob->y+ob->h)) ) {
		*v = ob->valor;
		return 1;
	}
	return 0;
}

/**********************************************************************************************
 diz se o número i está entre i1 e i2
**********************************************************************************************/
int _entre(short int i, short int i1, short int i2)
{
	return ( i>=i1 && i<=i2 );
}


/**********************************************************************************************
	Salva o obstáculo no arquivo

	ENTRADA:	ob - obstáculo
				f - arquivo no ponto onde deverá ser salvo
	SAÍDA:	0 se ok, 1 se erro
**********************************************************************************************/
int obstaculo_salva(OBSTACULO *ob, PACKFILE *f)
{
	return (pack_fwrite((void *)ob, sizeof(OBSTACULO), f) != sizeof(OBSTACULO));
}

/**********************************************************************************************
	Abre o obstáculo do arquivo

	ENTRADA:	f - arquivo no ponto onde deverá ser aberto
	SAÍDA:	ponteiro para o obstáculo se ok, NULL se erro
**********************************************************************************************/
OBSTACULO *obstaculo_abre(PACKFILE *f)
{
	OBSTACULO *ob, t;
	if( pack_fread((void *)&t, sizeof(OBSTACULO), f) != sizeof(OBSTACULO) )
		return NULL;
	ob = obstaculo_cria(t.id, t.x, t.y, t.w, t.h, t.valor);
	if(ob == NULL)
		return NULL;
	return ob;
}