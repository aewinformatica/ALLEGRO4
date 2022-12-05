#include <iostream>
#include <string>
#include <sstream>
#include "engcomp_glib.h"

using namespace std;

int main(void)
{
	egl_inicializar(800, 600);
		// egl_inicializar(800, 600, true);
	int posicaoX = 100;
	int posicaoY = 100;
	//http://sdb.drshnaps.com/SpritesPC.htm
	imagem caoEsquerda, caoDireita, caoParado;
	fundo fundo2;
	caoEsquerda.carregar("Sprites//Wolf3D-AttackDog.bmp",3,105,55,39);
	caoEsquerda.carregar("Sprites//Wolf3D-AttackDog.bmp",68,105,55,39);
	caoEsquerda.carregar("Sprites//Wolf3D-AttackDog.bmp",133,105,56,39);
	caoEsquerda.carregar("Sprites//Wolf3D-AttackDog.bmp",199,105,56,39);
	caoDireita.carregar("Sprites//Wolf3D-AttackDog.bmp",3,298,55,39);
	caoDireita.carregar("Sprites//Wolf3D-AttackDog.bmp",71,298,55,39);
	caoDireita.carregar("Sprites//Wolf3D-AttackDog.bmp",142,298,56,39);
	caoDireita.carregar("Sprites//Wolf3D-AttackDog.bmp",208,298,56,39);
	caoEsquerda.setar_tempo_animacao(30);
	caoDireita.setar_tempo_animacao(30);
	caoParado.carregar("Sprites//Wolf3D-AttackDog.bmp",89,15,38,32);
	fundo2.carregar("Sprites//neve.bmp");
	while(!key[KEY_ESC])
	{
		fundo2.desenha(0,0);
		
        if(key[KEY_UP]) 
		{
			posicaoY--;
		}
        if(key[KEY_DOWN]) 
		{
			posicaoY++;
		}
        
        if(key[KEY_LEFT]) 
		{
			posicaoX-=1;
			caoEsquerda.desenha(posicaoX, posicaoY);
		}
		else if(key[KEY_RIGHT]) 
		{
			posicaoX+=1;
			caoDireita.desenha(posicaoX, posicaoY);
		}
		else
		{
			caoParado.desenha(posicaoX, posicaoY);
		}
		egl_desenha_frame();
	}
	egl_finalizar();
return 0;
}
END_OF_MAIN();