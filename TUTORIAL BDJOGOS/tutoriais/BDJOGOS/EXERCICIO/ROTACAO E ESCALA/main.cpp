#include <allegro.h>

void init();
void deinit();
void desenha();
void rotacao();
void escala();

BITMAP *bdjogos = NULL;
BITMAP *mouse = NULL;
BITMAP *BUFFER = NULL;

int main() {
	init();

desenha();
rotacao();
 
	while (!key[KEY_ESC]) {
          /*clear_bitmap(BUFFER);
          */
	show_mouse(BUFFER);
	
	blit(BUFFER,screen,0,0,0,0,SCREEN_H,SCREEN_W);
	
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
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
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
void desenha(){
     
bdjogos = create_bitmap(215,76);
bdjogos = load_bitmap("bdjogos.bmp",NULL);

mouse = create_bitmap(16,16);
mouse = load_bitmap("mouse.bmp",NULL);

BUFFER =create_bitmap(800,600);

set_mouse_sprite(mouse);
     }
void rotacao(){
   rotate_sprite(BUFFER, bdjogos, 0, 0, itofix(0));
   rotate_sprite(BUFFER, bdjogos, 0, 155, itofix(64));
   rotate_sprite(BUFFER, bdjogos, 0, 310, itofix(128));
   rotate_sprite(BUFFER, bdjogos, 0, 400, itofix(256));
   
   rotate_sprite(BUFFER, bdjogos, 200, 155, itofix(32));

   rotate_sprite_v_flip(BUFFER, bdjogos, 300, 0, itofix(0));
   rotate_sprite_v_flip(BUFFER, bdjogos, 300, 155, itofix(64));
   rotate_sprite_v_flip(BUFFER, bdjogos, 300, 310, itofix(128));
   rotate_sprite_v_flip(BUFFER, bdjogos, 300, 400, itofix(256));

     }
     
     void escala(){
          rotate_scaled_sprite(BUFFER, bdjogos, 0, 0, itofix(0), itofix(2));
          
          rotate_scaled_sprite_v_flip(BUFFER, bdjogos, 0, 0, itofix(0), itofix(2));
          }
