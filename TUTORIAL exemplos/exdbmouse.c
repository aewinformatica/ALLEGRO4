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

	buf = create_bitmap(MAX_X, MAX_Y);

	set_palette(desktop_palette);

	while (!key[KEY_ESC])
	{
		show_mouse(NULL);
		clear(buf);
		show_mouse(buf);
		blit(buf, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
	}
}

void finaliza (void)
{
	allegro_exit();
}
