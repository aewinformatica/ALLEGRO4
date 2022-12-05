#include <allegro.h>

void init();
void deinit();
void texto();

int main() {
	init();
     texto();
	while (!key[KEY_ESC]) {
		/* put your code here */
	}

	deinit();
	return 0;
}
END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}
void texto(){
     int iPontos = 100;
     //esquerda
   textprintf_ex(screen, font, 0, 0,makecol(255, 255, 0), -1,"Pontuacao.: %d",iPontos);
     //centro
  textprintf_centre_ex(screen, font, SCREEN_W/2, 0, makecol(255, 255, 255), makecol(0, 0, 0),"Pontuacao.: 1100",iPontos);

 //direita
 textprintf_right_ex(screen, font,SCREEN_W, 0, makecol(255,255,0), -1,"Pontuacao.: %d",iPontos);
 

readkey(); 
       }

