#include <allegro.h>
#include <stdio.h>
#include "pmask.h"

#define res_type  GFX_AUTODETECT_FULLSCREEN

#define delay_time 25
#define move_speed 3
int velocidade = 0;
void controlador();
void init();
int main (int argc, char **argv) {
	int i, j, x[2], y[2], mode[2];
	init();
		BITMAP *Buffer = NULL;
            Buffer = create_bitmap(640,480);
DATAFILE *data;
          data = load_datafile("dados.dat");
SAMPLE *Hadoken = NULL;
        Hadoken = (SAMPLE *)data[5].dat;

	PMASK pd;
	PMASK pc;
	
	//Ryu 
	BITMAP *Ryu_sprite[6];
    PMASK *Ryu_mask[6];
    
    RLE_SPRITE *Ryutmp;
            Ryutmp = (RLE_SPRITE*) data[0].dat;
            BITMAP *tmpbmp1;
                    tmpbmp1 = create_bitmap_ex(Ryutmp->color_depth, Ryutmp->w, Ryutmp->h);	

                    clear_to_color(tmpbmp1, bitmap_mask_color(tmpbmp1));
                    draw_rle_sprite(tmpbmp1, Ryutmp, 0, 0);   
                    Ryutmp = NULL;
//----------------------------------------------------------------------------------------------------------------
    //Asteroid
    BITMAP *Asteroid_sprite[64];
    PMASK *Asteroid_mask[64];        
    RLE_SPRITE *Asteroidtmp;
                Asteroidtmp = (RLE_SPRITE*) data[1].dat;
                  BITMAP *tmpbmp2;
                          tmpbmp2 = create_bitmap_ex(Asteroidtmp->color_depth, Asteroidtmp->w, Asteroidtmp->h);
                          clear_to_color(tmpbmp2, bitmap_mask_color(tmpbmp2));
                          draw_rle_sprite(tmpbmp2, Asteroidtmp, 0, 0);   
                          Asteroidtmp = NULL;

	//install_pmask();
//----------------------------------------------------------------------------------------------------------------	
int QdtH = 6;
int QdtV = 1;

int sprite_w = tmpbmp1->w / QdtH;
int sprite_h = tmpbmp1->h / QdtV;

for (int i = 0; i < 6; i += 1){
Ryu_sprite[i] = create_sub_bitmap(tmpbmp1, (i%6)*  tmpbmp1->w / QdtH, (i/6) *  tmpbmp1->h * QdtV, tmpbmp1->w / 6, tmpbmp1->h /*/ 1*/);
Ryu_mask[i] = create_allegro_pmask(Ryu_sprite[i]);
}
//init_allegro_pmask(&pd, Ryu_sprite[0]);	

for (int i = 0; i < 64; i += 1) 
{
Asteroid_sprite[i] = create_sub_bitmap(tmpbmp2, (i%8) * tmpbmp2->w / 8, (i/8) * tmpbmp2->h / 8, tmpbmp2->w / 8, tmpbmp2->h / 8);
Asteroid_mask[i] = create_allegro_pmask(Asteroid_sprite[i]);
}	
init_allegro_pmask(&pc, Asteroid_sprite[0]);



	x[0] = 0;
	y[0] = 0;
	mode[0] = 0;

	x[1] = 0;
	y[1] = 0;
	mode[1] = 0;

	j = 0;


	while (!key[KEY_ESC]) {

        
        
        int colliding;

		colliding = check_pmask_collision (&pd, Ryu_mask[1] , x[0], y[0], x[1], y[1]);
		colliding = check_pmask_collision (&pc, Asteroid_mask[0] , x[0], y[0], x[1], y[1]);

	while(velocidade <0)
		clear(Buffer);
		

	
    if (key[KEY_RIGHT] || key[KEY_6_PAD]) {
			x[j] += move_speed;
			if (x[j] > 640) x[j] = 640-119;
		}
		if (key[KEY_LEFT] || key[KEY_4_PAD]) {
			x[j] -= move_speed;
			if (x[j] < 0) x[j] = 0;
		}
		if (key[KEY_DOWN] || key[KEY_2_PAD]) {
			y[j] += move_speed;
			if (y[j] > 480) y[j] = 480-97;
		}
		if (key[KEY_UP] || key[KEY_8_PAD]) {
			y[j] -= move_speed;
			if (y[j] < 0) y[j] = 0;
		}
		
		for (i = 0; i < 5; i ++) {

				
            //RYU	
             draw_allegro_pmask(Ryu_mask[i], Buffer, x[i], y[i], makecol(0,0,144));	
             masked_blit(Ryu_sprite[i], Buffer, 0, 0, x[i], y[i], Ryu_sprite[i]->w, Ryu_sprite[i]->h);
             
            //ASTEROIDE
            draw_allegro_pmask(Asteroid_mask[i], Buffer, 200, 200, makecol(0,0,144));
            masked_blit(Asteroid_sprite[i], Buffer, 0, 0, 200, 200, Asteroid_sprite[i]->w, Asteroid_sprite[i]->h);    
        }
        

        		
		if (colliding) {
			textprintf_ex(Buffer, font, SCREEN_W/2, 240, palette_color[12],makecol(255,255,255), "Colidiu!");
			//x[0]=x[1];
            //MOVIMENTAVA
            x[1]=x[1]+20;
			play_sample(Hadoken, 255, 128, 1000, FALSE ); 
		}
		
		if (keypressed()) {
			int i = readkey();
			if ((i >> 8) == KEY_TAB) j ^= 1;
			if ((i >> 8) == KEY_SPACE) mode[j] ^= 1;
		}
		
		velocidade --;
		textprintf_ex(Buffer, font, SCREEN_W/3, 0, palette_color[12],-1, "Aperte TAB para alternar entre os personagens");
		blit(Buffer,screen,0,0,0,0,640,480);
	}
deinit_pmask(&pd);
destroy_pmask ( Ryu_mask[i]);
destroy_pmask ( Asteroid_mask[i] );

	return 0;
}
END_OF_MAIN()
void controlador()
{
 velocidade++;    
}
void init(){
allegro_init();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) ;
	install_timer();
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,0);
	install_int(controlador,30);
	install_pmask();

}

