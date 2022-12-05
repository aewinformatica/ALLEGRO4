#include <allegro.h>

void init();
void deinit();

int main() {
	init();
	
	int cores[10]={50,200,130,80,140,240,30,100,222,111};
	
	int x = SCREEN_W/2;
	int y = SCREEN_H/2;
	int raio = 30;
	const int tamanho = 30;
    int i =10;
	
		  		  for(int i = 11;i>0;i--)
		{
		 		raio = ((i+ raio)/2);
		 		
		 		circlefill(screen,x,y,raio * 10,makecol(cores[i],cores[i],cores[i]));
		}
		
		x = 0;
        y = 0;

		          for(int i =0;i<tamanho;i++)
		          {
				   		  				   		  
				   		  x = x + (tamanho + i);
				   		  rectfill(screen,x,y,x+tamanho,y+tamanho,makecol(i*50,i*25,i*25));
                  }

		  


		
	while (!key[KEY_ESC]) {
		  
		  

		
		
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
