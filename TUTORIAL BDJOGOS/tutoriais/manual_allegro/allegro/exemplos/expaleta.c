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
	int i;
	int cor;
	PALETTE pal;

	/* Limpa a tela (para o codigo de cor 0). */
	clear(screen);

	/*
		Torna ativa a black_palette, paleta pre-definida pelo
		Allegro que contem apenas a cor preta, para todos os codigos.
		Fazemos isso para que os retangulos feitos a seguir nao sejam
		vistos ao serem desenhados.
	*/
	set_palette(black_palette);

	/*
		Desenha retangulos preenchidos, cada um com codigos
		de cor variando de 0 a 63, em sequencia.
	*/
	for (i = 0; i < 64; i++)
	{
		rectfill(screen, i * 12.5, 0, (i + 1) * 12.5, 599, i);
	}

	/* Gera tons de azuis */
	for (i = 0; i < 64; i++)
	{
		pal[i].r = 0;
		pal[i].g = 0;
		pal[i].b = i;
	}

	while (!keypressed())
	{
		/* Torna ativa a paleta pal */
		set_palette(pal);

		/*
			Troca os valores de cada codigo de cor,
			causando um efeito de animacao.
		*/
		cor = pal[63].b;
		for (i = 63; i > 0; i--)
		{
			pal[i].b = pal[i - 1].b;
		}
		pal[0].b = cor; 
	}
}

void finaliza (void)
{
	allegro_exit();
}
