#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>

#define MAX_X		800
#define MAX_Y		600
#define V_MAX_X		0
#define V_MAX_Y		0
#define COLOR_BITS	8
#define VIDEO_CARD	GFX_VESA1

#define DIGI_CARD	DIGI_SB16
#define MIDI_CARD	MIDI_SB_OUT

#define FUNDOS		4

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
	BITMAP *fundo[FUNDOS];
	BITMAP *tela;
	PALETTE pal;
	int fx[FUNDOS];
	int x, y;
	int i, j;

	srand(time(0));

	for (i = 0; i < FUNDOS; i++)
	{
		fundo[i] = create_bitmap(MAX_X, MAX_Y);
		fx[i] = 0;
		clear(fundo[i]);
		for (j = 0; j < (FUNDOS - i) * 30; j++)
		{
			x = rand() % MAX_X;
			y = rand() % MAX_Y;
			rectfill(fundo[i], x, y, x + i, y + i, 1);
		}
	}

	tela = create_bitmap(MAX_X, MAX_Y);

	pal[0].r = 0;
	pal[0].g = 0;
	pal[0].b = 0;
	pal[1].r = 63;
	pal[1].g = 63;
	pal[1].b = 63;
	set_palette(pal);

	while (!keypressed())
	{
		clear(tela);
		for (i = 0; i < FUNDOS; i++)
		{
			draw_sprite(tela, fundo[i], fx[i], 0);
			draw_sprite(tela, fundo[i], fx[i] - MAX_X, 0);
			fx[i] += (i + 1) * 2;
			if (fx[i] > MAX_X)
			{
				fx[i] -= MAX_X;
			}
		}
		blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
	}
	readkey();
}

void finaliza (void)
{
	allegro_exit();
}
