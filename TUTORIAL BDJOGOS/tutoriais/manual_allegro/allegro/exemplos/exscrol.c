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
	BITMAP *tela;
	BITMAP *fundo;
	BITMAP *boneco;
	PALETTE pal;
	int fx, fy;
	int x, y;
	int i;

	tela = create_bitmap(MAX_X, MAX_Y);
	fundo = create_bitmap(MAX_X, MAX_Y);

	boneco = load_bitmap("boneco.bmp", pal);

	clear(fundo);
	for (i = 0; i < 200; i++)
	{
		x = rand() % MAX_X;
		y = rand() % MAX_Y;
		putpixel(fundo, x, y, 255);
	}
	fx = fy = 0;

	set_palette(pal);

	while (!keypressed())
	{
		clear(tela);
		blit(fundo, tela, 0, 0, fx, fy, MAX_X, MAX_Y);
		blit(fundo, tela, 0, 0, fx, fy - MAX_Y, MAX_X, MAX_Y);
		draw_sprite(tela, boneco, ((MAX_X) - boneco->w) / 2, ((MAX_Y) - boneco->h) / 2);
		blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
		fy -= 5;
		if (fy < 0)
		{
			fy += MAX_Y;
		}
	}
	readkey();
}

void finaliza (void)
{
	allegro_exit();
}
