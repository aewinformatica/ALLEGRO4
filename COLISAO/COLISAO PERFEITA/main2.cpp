#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro.h>

#define MAX_X 640
#define MAX_Y 480
#define V_MAX_X 0
#define V_MAX_Y 0
#define COLOR_BITS 32
#define VIDEO_CARD GFX_AUTODETECT_FULLSCREEN
#define DIGI_CARD DIGI_AUTODETECT
#define MIDI_CARD MIDI_AUTODETECT
#define FPS 60

typedef struct ARRAY_
{
	int x;
	int y;
} ARRAY;

typedef struct F_ARRAY_
{
	float x;
	float y;
} F_ARRAY;

//-------------------------------------------------------------------------
// FUNÇÕES PRINCIPAIS
int begin(void); 
void closing(void); 
void gameloop(void); 
volatile long int counter = 0;
void game_timer(void);
int collision(ARRAY centro1, int raio1, ARRAY centro2, int raio2);
BITMAP *buffer;

int main(int argc, char **argv)
{
	if (!begin())
	{
		closing();
		return -1;
	}

	gameloop();

	closing();
	return 0;

}
END_OF_MAIN();

int begin(void)
{

	allegro_init();

	install_keyboard();
	install_mouse();

	install_timer();

	srand((unsigned)time(NULL));

	set_color_depth(COLOR_BITS);
	if (set_gfx_mode(VIDEO_CARD, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
	{
		if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
		{
			allegro_message("Erro ao inicializar o modo grafico");
			return (FALSE);
		}
	}

	if (COLOR_BITS == 32)	set_alpha_blender(); // instala o canal alpha (funciona apenas em 32 bits)

	if (install_sound(DIGI_CARD, MIDI_CARD, NULL) < 0)
	{
		if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
		{
			allegro_message("Erro ao inicializar o som");
			return (FALSE);
		}
	}


	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(game_timer);
	install_int(game_timer, 1000/FPS);

	return (TRUE);
}

void game_timer(void)
{
    counter++;
}
END_OF_FUNCTION(game_timer);

void gameloop(void)
{
	
	BITMAP *bolinha_vermelha[4], *bolinha_azul[4];
	ARRAY v_pos, v_vel, a_vel, a_pos;
	long int vel_control = 0;
	int x;
	
	v_pos.x = v_pos.y = 200;
	a_pos.x = a_pos.y = 100;
	v_vel.x = v_vel.y = 3;
	a_vel.x = a_vel.y = 4;
	
	
	buffer = create_bitmap(MAX_X,MAX_Y);
	clear(buffer);
	
	for (x = 0; x < 4;x++)
	{
		bolinha_vermelha[x] = create_bitmap(50,50);
		bolinha_azul[x] = create_bitmap(50,50);
	}
	
	for (x = 0; x < 4;x++)
	{
		clear_to_color(bolinha_vermelha[x], makecol(255,0,255));
		clear_to_color(bolinha_azul[x], makecol(255,0,255));
	}
	
	circlefill(bolinha_vermelha[0],bolinha_vermelha[0]->w/2,bolinha_vermelha[0]->h/2,15,makecol(255,0,0));
	circlefill(bolinha_vermelha[1],bolinha_vermelha[1]->w/2,bolinha_vermelha[1]->h/2,15,makecol(170,0,0));
	circlefill(bolinha_vermelha[2],bolinha_vermelha[2]->w/2,bolinha_vermelha[2]->h/2,15,makecol(90,0,0));
	circlefill(bolinha_vermelha[3],bolinha_vermelha[3]->w/2,bolinha_vermelha[3]->h/2,15,makecol(170,0,0));
	circlefill(bolinha_azul[0],bolinha_azul[0]->w/2,bolinha_azul[0]->h/2,15,makecol(0,0,255));
	circlefill(bolinha_azul[1],bolinha_azul[1]->w/2,bolinha_azul[1]->h/2,15,makecol(0,0,170));
	circlefill(bolinha_azul[2],bolinha_azul[2]->w/2,bolinha_azul[2]->h/2,15,makecol(0,0,90));
	circlefill(bolinha_azul[3],bolinha_azul[3]->w/2,bolinha_azul[3]->h/2,15,makecol(0,0,170));
	circlefill(bolinha_vermelha[0],bolinha_vermelha[0]->w/2,bolinha_vermelha[0]->h/2,10,makecol(90,100,0));
	circlefill(bolinha_vermelha[1],bolinha_vermelha[1]->w/2,bolinha_vermelha[1]->h/2,10,makecol(170,100,0));
	circlefill(bolinha_vermelha[2],bolinha_vermelha[2]->w/2,bolinha_vermelha[2]->h/2,10,makecol(255,100,0));
	circlefill(bolinha_vermelha[3],bolinha_vermelha[3]->w/2,bolinha_vermelha[3]->h/2,10,makecol(170,100,0));
	circlefill(bolinha_azul[0],bolinha_azul[0]->w/2,bolinha_azul[0]->h/2,10,makecol(0,100,90));
	circlefill(bolinha_azul[1],bolinha_azul[1]->w/2,bolinha_azul[1]->h/2,10,makecol(0,100,170));
	circlefill(bolinha_azul[2],bolinha_azul[2]->w/2,bolinha_azul[2]->h/2,10,makecol(0,100,255));
	circlefill(bolinha_azul[3],bolinha_azul[3]->w/2,bolinha_azul[3]->h/2,10,makecol(0,100,170));
	
	
	// Looping do Jogo-------------------------------
	do
	{
		
		if (counter >= vel_control)
		{
			if ( (v_pos.x <= 0) || (v_pos.x + bolinha_vermelha[(counter%16)/4]->w >= MAX_X) )
				v_vel.x = -v_vel.x;
			if ( (v_pos.y <= 0) || (v_pos.y + bolinha_vermelha[(counter%16)/4]->h >= MAX_Y) )
				v_vel.y = -v_vel.y;
			
			
			// Movements---------------------------
			
			if ((collision(a_pos, 15, v_pos, 15)))
			{
				v_vel.x = -v_vel.x;
				v_vel.y = -v_vel.y;
			}
			
			v_pos.x += v_vel.x;			
			v_pos.y += v_vel.y;
			
			if ((key[KEY_UP]) && (a_pos.y > 0))
				a_pos.y -= a_vel.y;
			if ((key[KEY_DOWN]) && (a_pos.y < MAX_Y - bolinha_azul[(counter%16)/4]->h))
				a_pos.y += a_vel.y;
			if ((key[KEY_LEFT]) && (a_pos.x > 0))
				a_pos.x -= a_vel.x;
			if ((key[KEY_RIGHT]) && (a_pos.x < MAX_X - bolinha_azul[(counter%16)/4]->w))
				a_pos.x += a_vel.x;
			
			
			
			// Desenhando na tela-------------------------
			draw_sprite(buffer, bolinha_azul[(counter%16)/4],a_pos.x,a_pos.y);
			draw_sprite(buffer, bolinha_vermelha[(counter%16)/4],v_pos.x,v_pos.y);
			
			//Re-desenhando a tela (com Double Buffer)-------------------------
			blit(buffer, screen, 0, 0, 0, 0,MAX_X,MAX_Y);
			clear(buffer);
			
			
			vel_control = counter + 1;
		}
		
		if (key[KEY_ESC])    break;
	}while(1);
}

int collision(ARRAY centro1, int raio1, ARRAY centro2, int raio2)
{
	
	if ( (int) sqrt((centro2.x - centro1.x)*(centro2.x - centro1.x) + (centro2.y - centro1.y)*(centro2.y - centro1.y)) <= raio1 + raio2 )
		return TRUE;
	
	return FALSE;
}


void closing(void)
{
	remove_timer();
	remove_mouse();
	remove_sound();
	remove_keyboard();
	allegro_exit();
}
