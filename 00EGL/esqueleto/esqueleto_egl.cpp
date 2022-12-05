#include <iostream>
#include <string>
#include <sstream>
#include "allegro.h"
#include "engcomp_glib.h"

using namespace std;

int main(void)
{
	egl_inicializar(1024,768);
	
	while(!key[KEY_ESC])
	{
	


		egl_desenha_frame();
	}

	egl_finalizar();

	return 0;
}
END_OF_MAIN();