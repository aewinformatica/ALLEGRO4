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

	set_palette(desktop_palette);

	return (TRUE);
}

void principal (void)
{
	int		x;

	while (1)
	{
		for (x = (MAX_X / 5); x < (4 * MAX_X / 5); x += PASSO)
		{
			clear(screen);
			circlefill(screen, x, MAX_Y / 2, MAX_X / 10, 10);
			if (keypressed())
			{
				return;
			}
		}
		for ( ; x > (MAX_X / 5); x -= PASSO)
		{
			clear(screen);
			circlefill(screen, x, MAX_Y / 2, MAX_X / 10, 10);
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
