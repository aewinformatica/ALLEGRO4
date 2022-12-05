//#define __EGL_DEBUG_MODE

#include <iostream>
#include <string>
#include <sstream>
#include "..//..//engcomp_glib.h"

using namespace std;

int main(void)
{
	egl_inicializar(800,600);
    fundo neve;
    neve.carregar("sprites//neve.bmp");
    	
	imagem taz;
	taz.carregar("sprites//taz.bmp");
	//taz.carregar("sprites//billy.bmp",12,137,59,118);
 //   taz.carregar("sprites//billy.bmp",85,137,59,118);
 //   taz.carregar("sprites//billy.bmp",157,137,59,118);
 //   taz.carregar("sprites//billy.bmp",231,137,59,118);
 //   taz.carregar("sprites//billy.bmp",157,137,59,118);
 //   taz.carregar("sprites//billy.bmp",85,137,59,118);
    
    taz.setar_tempo_animacao(20);

	int x,y;
	
	x = 100;
	y = 100;
	//string buffer;
	//egl_ler_string_teclado(buffer, 40, 330, 340);
	while(!key[KEY_ESC])
	{
        neve.desenha(0,0);
        
 
        if(key[KEY_UP]) y--;
        if(key[KEY_DOWN]) y++;
        
        if(key[KEY_LEFT]) x-=3;
        if(key[KEY_RIGHT]) x+=3;
        
        
        
        taz.desenha(x,y);                
 
		egl_desenha_frame();
	}

	egl_finalizar();

	return 0;
}
END_OF_MAIN();
