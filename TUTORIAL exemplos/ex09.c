#include <stdio.h>
#include <stdlib.h>

#include <allegro.h>

#define MAX_X		800
#define MAX_Y		600
#define V_MAX_X		0
#define V_MAX_Y		0
#define COLOR_BITS	8

#define PASSO		2

int inicia(void);
void principal(void);
void finaliza(void);

int main (void)
{
	if (!inicia())
	{
		finaliza();
		return -1;
	}

	principal();

	finaliza();
	return 0;
}
END_OF_MAIN();

int inicia (void)
{
	allegro_init();

	install_keyboard();

	set_color_depth(COLOR_BITS);
	if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
	{
		printf("Erro ao inicializar o modo grafico.\n");
		return (FALSE);
	}

	return (TRUE);
}

void principal (void)
{
	int		x;
	BITMAP	*tela;
	BITMAP	*nave;
	PALETTE	pal;
	
	nave = load_bitmap("nave.bmp", pal);

	x = (MAX_X - nave->w) / 2;

	tela = create_bitmap(MAX_X, MAX_Y);

	set_palette(black_palette);

	clear(tela);
	draw_sprite(tela, nave, x, 4 * MAX_Y / 5);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);

	fade_in(pal, 2);

	while (!key[KEY_ESC])
	{
		clear(tela);
		draw_sprite(tela, nave, x, 4 * MAX_Y / 5);
		blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
		if ((key[KEY_LEFT]) && (!key[KEY_RIGHT]))
		{
			x -= PASSO;
		}
		if ((key[KEY_RIGHT]) && (!key[KEY_LEFT]))
		{
			x += PASSO;
		}
	}

	fade_out(4);
}

void finaliza (void)
{
	allegro_exit();
}
