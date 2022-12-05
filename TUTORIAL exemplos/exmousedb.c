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
	BITMAP *retang;
	int x, y, w, h, c;

	tela = create_bitmap(MAX_X, MAX_Y);
	retang = create_bitmap(MAX_X, MAX_Y);

	clear(retang);

	set_palette(desktop_palette);

	srand(time(0));

	while (!keypressed())
	{
		x = rand() % MAX_X;
		y = rand() % MAX_Y;
		w = rand() % (MAX_X - x);
		h = rand() % (MAX_Y - y);
		c = rand() % (1 << COLOR_BITS);
		rectfill(retang, x, y, x + w, y + h, c);
		show_mouse(NULL);
		clear(tela);
		blit(retang, tela, 0, 0, 0, 0, MAX_X, MAX_Y);
		show_mouse(tela);
		blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
	}
	readkey();
}

void finaliza (void)
{
	allegro_exit();
}
