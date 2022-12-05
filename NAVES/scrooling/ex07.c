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
	
	nave = load_bitmap("nave1.bmp", pal);

	set_palette(pal);

	tela = create_bitmap(MAX_X, MAX_Y);
	while (1)
	{
		for (x = (MAX_X / 5); x < (4 * MAX_X / 5); x += PASSO)
		{
			clear(tela);
			draw_sprite(tela, nave, x, MAX_Y / 2);
			blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
			if (keypressed())
			{
				return;
			}
		}
		for ( ; x > (MAX_X / 5); x -= PASSO)
		{
			clear(tela);
			draw_sprite(tela, nave, x, MAX_Y / 2);
			blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
			if (keypressed())
			{
				return;
			}
		}
	}
}

void finaliza (void)
{
	allegro_exit();
}
