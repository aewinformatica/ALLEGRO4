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
	BITMAP *buf;
	int x, y;
	int dx, dy;

	buf = create_bitmap(MAX_X, MAX_Y);

	x = 100;
	y = 100;
	dx = 2;
	dy = 2;

	set_palette(desktop_palette);

	while (!keypressed())
	{
		clear(screen);
		textout(screen, font, "Sem Double Buffering (pressione uma tecla para continuar)", 0, 0, -1);
		rectfill(screen, x, y, x + 40, y + 40, 4);
		x += dx;
		y += dy;
		if ((x < 2) || (x > (MAX_X - 42)))
		{
			dx = -dx;
		}
		if ((y < 2) || (y > (MAX_Y - 42)))
		{
			dy = -dy;
		}
		rest(10);
	}
	readkey();

	while (!keypressed())
	{
		clear(buf);
		textout(buf, font, "Com Double Buffering (pressione uma tecla para continuar)", 0, 0, -1);
		rectfill(buf, x, y, x + 40, y + 40, 4);
		blit(buf, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
		x += dx;
		y += dy;
		if ((x < 2) || (x > (MAX_X - 42)))
		{
			dx = -dx;
		}
		if ((y < 2) || (y > (MAX_Y - 42)))
		{
			dy = -dy;
		}
		rest(10);
	}
	readkey();
}

void finaliza (void)
{
	allegro_exit();
}
