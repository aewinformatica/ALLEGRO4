#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

#define MAX_X		800
#define MAX_Y		600
#define V_MAX_X		0
#define V_MAX_Y		0
#define COLOR_BITS	8
#define VIDEO_CARD	GFX_VESA1

#define DIGI_CARD	DIGI_SB16
#define MIDI_CARD	MIDI_SB_OUT

#define DX			3
#define DY			3
#define MAX_FRAMES	8

typedef struct _SPRITE
{
	BITMAP **frame;
	int x, y;
	int dx, dy;
	int dir;
} SPRITE;

enum { DIR_N, DIR_NE, DIR_E, DIR_SE, DIR_S, DIR_SO, DIR_O, DIR_NO };

int inicia(void);
void principal(void);
void finaliza(void);

int main (void)
{
	if (!inicia())
	{
	finaliza();
	exit(-1);
	}

	principal();

	finaliza();
	exit(0);
}

int inicia (void)
{
	allegro_init();

	install_keyboard();
	install_mouse();
	install_timer();

	set_color_depth(COLOR_BITS);
	if (set_gfx_mode(VIDEO_CARD, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
	{
		if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
		{
			printf("Erro ao inicializar o modo grafico");
			return (FALSE);
		}
	}

	if (install_sound(DIGI_CARD, MIDI_CARD, NULL) < 0)
	{
		if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
		{
			printf("Erro ao inicializar o som");
			return (FALSE);
		}
	}

	return (TRUE);
}

void principal (void)
{
	BITMAP *tela;
	PALETTE pal;
	SPRITE sp;

	sp.frame = (BITMAP **)malloc(MAX_FRAMES * sizeof(BITMAP *));
	sp.frame[DIR_N] = load_bitmap("CarroN.bmp", pal);
	sp.frame[DIR_NE] = load_bitmap("CarroNE.bmp", pal);
	sp.frame[DIR_E] = load_bitmap("CarroE.bmp", pal);
	sp.frame[DIR_SE] = load_bitmap("CarroSE.bmp", pal);
	sp.frame[DIR_S] = load_bitmap("CarroS.bmp", pal);
	sp.frame[DIR_SO] = load_bitmap("CarroSO.bmp", pal);
	sp.frame[DIR_O] = load_bitmap("CarroO.bmp", pal);
	sp.frame[DIR_NO] = load_bitmap("CarroNO.bmp", pal);

	sp.x = 370;
	sp.y = 50;
	sp.dx = -2 * DX;
	sp.dy = 0;
	sp.dir = DIR_O;

	tela = create_bitmap(MAX_X, MAX_Y);

	set_palette(pal);
	while (!keypressed())
	{
		sp.x += sp.dx;
		sp.y += sp.dy;
		switch (sp.dir)
		{
			case DIR_N:
				if (sp.y < 100)
				{
					sp.dx = -DX;
					sp.dy = -DY;
					sp.dir = DIR_NO;
				}
				break;
			case DIR_NE:
				if (sp.x > (MAX_X - 100))
				{
					sp.dx = 0;
					sp.dy = -2 * DY;
					sp.dir = DIR_N;
				}
				break;
			case DIR_E:
				if (sp.x > (MAX_X - 150))
				{
					sp.dx = DX;
					sp.dy = -DY;
					sp.dir = DIR_NE;
				}
				break;
			case DIR_SE:
				if (sp.y > (MAX_Y - 100))
				{
					sp.dx = 2 * DX;
					sp.dy = 0;
					sp.dir = DIR_E;
				}
				break;
			case DIR_S:
				if (sp.y > (MAX_Y - 150))
				{
					sp.dx = DX;
					sp.dy = DY;
					sp.dir = DIR_SE;
				}
				break;
			case DIR_SO:
				if (sp.x < 50)
				{
					sp.dx = 0;
					sp.dy = 2 * DY;
					sp.dir = DIR_S;
				}
				break;
			case DIR_O:
				if (sp.x < 100)
				{
					sp.dx = -DX;
					sp.dy = DY;
					sp.dir = DIR_SO;
				}
				break;
			case DIR_NO:
				if (sp.y < 50)
				{
					sp.dx = -2 * DX;
					sp.dy = 0;
					sp.dir = DIR_O;
				}
				break;
		}
		clear(tela);
		draw_sprite(tela, sp.frame[sp.dir], sp.x, sp.y);
		blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
		rest(20);
	}
	readkey();
}

void finaliza (void)
{
	allegro_exit();
}
