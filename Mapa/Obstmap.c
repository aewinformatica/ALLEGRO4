#include <stdlib.h>
#include <string.h>
#include "allegro.h"
#include "mapa.h"

/**********************************************************************************************
	Aloca a mem�ria usada pelo mapa de obst�culos

	ENTRADA:	x, y - tamanho do mapa
				valor_padrao - valor do ponto se nenhum obst�culo colidiu com ele
	SA�DA:	ponteiro para o obstmap se ok, NULL se erro
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
	Libera a mem�ria usada pelo mapa e por seus obst�culos

	ENTRADA:	o - ponteiro para o obstmap
	SA�DA:	nada
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
	Cria um obst�culo com a �rea, o id e o valor passado (o id pode ser repetido)

	ENTRADA:	o - mapa de obst�culos
				id - id do obst�culo (pode repetir)
				x, y, w, h - �rea do obst�culo
				d - valor daquela �rea
	SA�DA:	0 se ok, 1 se erro
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
	Retorna os obst�culos com o id especificado.
	ATEN��O: a mem�ria usada pelo vetor de ponteiros deve ser liberada manualmente
	usando-se free(nome_da_var);, onde nome_da_var � OBSTACULO **. Isto acontece
	porque pode ter v�rios obst�culos com o mesmo id.

	ENTRADA:	o - mapa de obst�culos
				id - id dos obst�culos
				no - n�mero de obst�culos retornados
	SA�DA:	NULL se n�o encontrou ou vetor de ponteiros de tamanho no
**********************************************************************************************/
OBSTACULO **obstmap_pega_obstaculo(OBSTMAP *o, short int id, short int *no)
{
	OBSTACULO *ob, *t, **r;
	/* primeiro, conta quantos obst�culos existem */
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
	/* depois, aloca a mem�ria para o vetor */
	r = (OBSTACULO **)malloc(sizeof(OBSTACULO *)*(*no));
	/* por �ltimo, coloca os obst�culos no vetor e retorna */
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
	Libera a mem�ria alocada pelos obst�culos com valor de id igual ao passado para
	a fun��o.

		ENTRADA:	o - mapa de obst�culos
					id - id dos obst�culos a serem apagados
		SA�DA:	nada
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
	Altera o valor dos obst�culos

	ENTRADA:	o - mapa de obst�culos
				id - id dos obst�culos
				nv - novo valor
	SA�DA:	nada
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
	/* liberando a mem�ria manualmente */
	free(r);
}

/**********************************************************************************************
	Move os obst�culos com o id passado x pontos na horizontal e y na vertical

	ENTRADA:	o - mapa de obst�culos
				id - id dos obst�culos
				x, y - quantidade de pontos para se movimentar
	SA�DA:	nada
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
	/* liberando a mem�ria manualmente */
	free(r);
}

/**********************************************************************************************
	Percorre todos os obst�culos at� encontrar um que o ponto esteja dentro

	ENTRADA:	o - mapa de obst�culos
				x, y - coordenadas do ponto
				v - valor retornado
	SA�DA:	0 se n�o colidiu e v � colocado no valor padr�o, ou 1 se colidiu
				e v � colocado no valor do obst�culo
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
	Percorre todos os obst�culos at� encontrar um em que o ret�ngulo delimitado por
	x, y, w, h colida com um dos obst�culos

	ENTRADA:	o - mapa de obst�culos
				x, y, w, h - ret�ngulo delimitador da �rea
				v - valor de retorno
	SA�DA:	0 se n�o colidiu e v recebe o valor padr�o, ou 1 se colidiu e v recebe
				o valor do obst�culo
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
	Salva o mapa de obst�culos no arquivo

	ENTRADA:	o - mapa de obst�culos
				f - arquivo no ponto onde dever� ser salvo o mapa
	SA�DA:	0 se ok, 1 se erro
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
	Abre o mapa de obst�culos do arquivo

	ENTRADA:	f - arquivo no ponto onde dever� ser aberto o mapa
	SA�DA:	ponteiro para o mapa se ok, NULL se erro
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
	Desenha o mapa de obst�culos (serve apenas para saber onde eles est�o, pois
	n�o ser�o desenhados normalmente)

	ENTRADA:	o - mapa
				b - bitmap onde ser� desenhado
				xoff, yoff - ponto onde se come�ar� a desenhar
				x1, y1, x2, y2 - �rea que deve ser desenhada
	SA�DA:	nada
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
	Reserva a mem�ria para um obst�culo

	ENTRADA:	id - id do obst�culo
				x, y, w, h - �rea reservada para ele
				v - valor daquela �rea
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
	Libera a mem�ria deste obst�culo

	ENTRADA:	ob - obst�culo
	SA�DA:	nada
**********************************************************************************************/
void obstaculo_destroi(OBSTACULO *ob)
{
	if(ob != NULL) {
		free(ob);
		ob = NULL;
	}
}

/**********************************************************************************************
	Coloca o obst�culo na lista

	ENTRADA:	ob - obst�culo
				lista - in�cio da lista de obst�culos
	SA�DA:	nada
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
	/* come�a no topo da lista */
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
	Retira da lista um obst�culo e libera a mem�ria

	ENTRADA:	ob - obst�culo
				lista - in�cio da lista de obst�culos
	SA�DA:	nada
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
	Retorna o id do obst�culo

	ENTRADA:	ob - obst�culo
	SA�DA:	id do obst�culo
**********************************************************************************************/
short int obstaculo_pega_id(OBSTACULO *ob)
{
	return ob->id;
}

/**********************************************************************************************
	Procura na lista um obst�culo com id igual ao passado

	ENTRADA:	lista - in�cio da lista de obst�culos
				id - id a ser procurado
	SA�DA:	ponteiro para o obst�culo se encontrou igual, NULL se n�o
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
	Verifica se o ponto colidi com o obst�culo

	ENTRADA:	ob - obst�culo
				x, y - ponto
				v - valor retornado
	SA�DA:	0 se n�o colidi e v n�o � alterado, ou 1 se colidi e v recebe o valor
				do obst�culo
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
	Verifica se a �rea colidi com o obst�culo

	ENTRADA:	ob - obst�culo
				x, y, w, h - �rea
				v - valor retornado
	SA�DA:	0 se n�o colidi e v n�o � alterado, ou 1 se colidi e v recebe o valor
				do obst�culo
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
 diz se o n�mero i est� entre i1 e i2
**********************************************************************************************/
int _entre(short int i, short int i1, short int i2)
{
	return ( i>=i1 && i<=i2 );
}


/**********************************************************************************************
	Salva o obst�culo no arquivo

	ENTRADA:	ob - obst�culo
				f - arquivo no ponto onde dever� ser salvo
	SA�DA:	0 se ok, 1 se erro
**********************************************************************************************/
int obstaculo_salva(OBSTACULO *ob, PACKFILE *f)
{
	return (pack_fwrite((void *)ob, sizeof(OBSTACULO), f) != sizeof(OBSTACULO));
}

/**********************************************************************************************
	Abre o obst�culo do arquivo

	ENTRADA:	f - arquivo no ponto onde dever� ser aberto
	SA�DA:	ponteiro para o obst�culo se ok, NULL se erro
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