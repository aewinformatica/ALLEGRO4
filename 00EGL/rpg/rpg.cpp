#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "allegro.h"
#include "engcomp_glib.h"

using namespace std;

#define PARA_CIMA 1
#define PARA_BAIXO 2
#define PARA_DIR 3
#define PARA_ESQ 4

vector<vector<unsigned char> > carrega_nivel(string nome_arquivo, int tamx, int tamy)
{
	vector<vector<unsigned char> > mat;
	
	// abre o arquivo e testa se não aconteceu erros
	ifstream arq(nome_arquivo.c_str());
	if(!arq.is_open()) return mat;

	// aloca espaço na matriz
	mat.resize(tamx);
	for(int i=0; i < tamx;i++) mat[i].resize(tamy);

	// ler do arquivo
	for(int y = 0; y < tamy; y++)
	{
		for(int x = 0; x < tamx; x++)
		{
			arq >> mat[x][y];
		}
	}

	return mat;
}

void desenha_nivel(vector<vector<unsigned char> > nivel, imagem* tiles)
{
	int x,y,tx,ty;
	unsigned char tile;
	int tamx = nivel.size();
	int tamy = nivel[0].size();

	for(y = 0,ty=0; y < tamy; y++,ty+=64)
	{
		for(x = 0,tx=0; x < tamx; x++,tx+=64)
		{
			tile = nivel[x][y];
			switch(tile)
			{
				case '.': 
					tiles[2].desenha(tx,ty); 
					break;
				case '#': 
					tiles[0].desenha(tx,ty); 
					break;
				case '(': 
					tiles[4].desenha(tx,ty); 
					break;
				case ')': 
					tiles[3].desenha(tx,ty); 
					break;
				case '%': 
					tiles[1].desenha(tx,ty); 
					break;
				case 't': 
					tiles[5].desenha(tx,ty); 
					break;
				case 'c': 
					tiles[6].desenha(tx,ty); 
					break;
			}
		}
	}
}

bool colidiu(int x, int y, int w, int h, vector<vector<unsigned char> > nivel)
{
	int tx,ty;
	int maxx,maxy;
	maxx = nivel.size()-1;
	maxy = nivel[0].size()-1;

	tx = x/64;
	ty = y/64;
	if( (tx <= maxx) && (ty <= maxy) )
		if( (nivel[tx][ty] == '#') || (nivel[tx][ty] == 't')  ) return true;

	ty = (y+h)/64;
	if( (tx <= maxx) && (ty <= maxy) )
		if( (nivel[tx][ty] == '#') || (nivel[tx][ty] == 't')  ) return true;

	tx = (x+w)/64;
	ty = y/64;
	if( (tx <= maxx) && (ty <= maxy) )
		if( (nivel[tx][ty] == '#') || (nivel[tx][ty] == 't')  ) return true;

	ty = (y+h)/64;
	if( (tx <= maxx) && (ty <= maxy) )
		if( (nivel[tx][ty] == '#') || (nivel[tx][ty] == 't')  ) return true;

	return false;
}

int main(void)
{
	int x,y,vel;
	int direcao;

	vector<vector<unsigned char> > nivel1;
	imagem tiles[7];
	imagem monstro,monstroc,monstrod,monstroe;
	imagem monstro_parado;
	imagem casa;
	imagem bob;
	
	egl_inicializar(1024,768);

	nivel1 = carrega_nivel("mapas//n1.txt",16,12);

	tiles[0].carregar("sprites//pedra.bmp");
	tiles[1].carregar("sprites//terra.bmp");
	tiles[2].carregar("sprites//grama.bmp");
	tiles[3].carregar("sprites//terra_grama.bmp");
	tiles[4].carregar("sprites//grama_terra.bmp");
	tiles[5].carregar("sprites//teto.bmp");
	tiles[6].carregar("sprites//casa.bmp");

	monstro.carregar("sprites//monstro_varios.bmp",333,184,32,45);
	monstro.carregar("sprites//monstro_varios.bmp",390,184,32,45);

	monstroc.carregar("sprites//monstro_varios.bmp",334,259,32,45);
	monstroc.carregar("sprites//monstro_varios.bmp",387,259,32,45);

	monstroe.carregar("sprites//monstro_varios.bmp",281,105,32,45);
	monstroe.carregar("sprites//monstro_varios.bmp",334,105,32,45);

	monstrod.carregar("sprites//monstro_varios.bmp",281,37,32,45);
	monstrod.carregar("sprites//monstro_varios.bmp",334,37,32,45);
	
	monstro_parado.carregar("sprites//monstro_varios.bmp",43,184,32,45);

	bob.carregar("sprites//bob.bmp",7,201,29,38);
	bob.carregar("sprites//bob.bmp",39,201,28,38);
	bob.carregar("sprites//bob.bmp",74,199,32,39);	
	bob.carregar("sprites//bob.bmp",112,195,42,42);
	bob.carregar("sprites//bob.bmp",161,195,42,43);
	bob.carregar("sprites//bob.bmp",208,195,42,43);
	bob.carregar("sprites//bob.bmp",255,194,42,45);
	bob.carregar("sprites//bob.bmp",303,194,42,45);

	x = 600;
	y = 350;
	vel = 1;

	direcao = PARA_BAIXO;

	bool dbob = false;

	while(!key[KEY_ESC])
	{
		if(key[KEY_LSHIFT]) vel = 2;
		else vel = 1;

		if(key[KEY_LEFT]  && (x > 0) && !colidiu(x-vel,y,32,45,nivel1) ) 
		{
			direcao = PARA_ESQ;
			x-=vel;
		}
		if(key[KEY_RIGHT] && (x < 1024-48) && !colidiu(x+vel,y,32,45,nivel1) ) 
		{
			direcao = PARA_DIR;
			x+=vel;
		}
		if(key[KEY_DOWN] && (y < 768-48) && !colidiu(x,y+vel,32,45,nivel1) ) 
		{
			direcao = PARA_BAIXO;
			y+=vel;
		}
		if(key[KEY_UP] && (y > 0) && !colidiu(x,y-vel,32,45,nivel1) ) 
		{
			direcao = PARA_CIMA;
			y-=vel;
		}
		
		desenha_nivel(nivel1,tiles);

		monstro_parado.desenha(100,100,true);

		if(!key[KEY_UP] && !key[KEY_DOWN] && !key[KEY_LEFT] && !key[KEY_RIGHT])
		{
			monstro_parado.desenha(x,y);
			if(monstro_parado.colide(x,y,32,45,100,100,32,45,monstro_parado)) egl_texto("colisao",800,10);
			direcao = PARA_BAIXO;
		}
		else
		{
			switch(direcao)
			{
				case PARA_BAIXO : monstro.desenha(x,y); if(monstro.colide(x,y,32,45,100,100,32,45,monstro_parado)) egl_texto("colisao",800,10); break;
				case PARA_CIMA : monstroc.desenha(x,y); if(monstroc.colide(x,y,32,45,100,100,32,45,monstro_parado)) egl_texto("colisao",800,10); break;
				case PARA_ESQ  : monstroe.desenha(x,y); if(monstroe.colide(x,y,32,45,100,100,32,45,monstro_parado)) egl_texto("colisao",800,10); break;
				case PARA_DIR  : monstrod.desenha(x,y); if(monstrod.colide(x,y,32,45,100,100,32,45,monstro_parado)) egl_texto("colisao",800,10); break;
			}
			
		}
	
		if(key[KEY_B]) dbob = true;
		if(dbob) if(!bob.desenha(10,10)) dbob = false;

		egl_desenha_frame();
	}

	egl_finalizar();

	return 0;
}
END_OF_MAIN();
