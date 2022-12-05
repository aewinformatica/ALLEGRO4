#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <allegro.h>

#define MAX_X		800
#define MAX_Y		600
#define V_MAX_X		0
#define V_MAX_Y		0
#define COLOR_BITS	8

#define PASSO		2

#define SCROLL		1
#define NUM_ESTR	150

int		tempo;

int inicia(void);
void principal(void);
void finaliza(void);
void incrementa_tempo(void);

void incrementa_tempo (void)
{
	tempo++;
}
END_OF_FUNCTION(incrementa_tempo);

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

	install_timer();
	install_mouse();

	set_color_depth(COLOR_BITS);
	if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
	{
		printf("Erro ao inicializar o modo grafico.\n");
		return (FALSE);
	}

	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
	{
		printf("Erro ao inicializar o som.\n");
		return (FALSE);
	}
	set_volume(128, 255);

	return (TRUE);
}

void principal (void)
{
	int		x;
	int		y;
	int		fx,
			fy;
	int		i;
	int		botEsq,
			botDir;
	BITMAP	*tela;
	BITMAP	*nave;
	BITMAP	*fundo;
	PALETTE	pal;
	MIDI	*musica;
	SAMPLE	*som;
	
	nave = load_bitmap("nave.bmp", pal);

	x = (MAX_X - nave->w) / 2;

	tela = create_bitmap(MAX_X, MAX_Y);
	fundo = create_bitmap(MAX_X, MAX_Y);

	musica = load_midi("musica.mid");
	som = load_sample("tiro.wav");

	set_palette(black_palette);

	srand(time(0));
	clear(fundo);
	for (i = 0; i < NUM_ESTR; i++)
	{
		fx = rand() % MAX_X;
		fy = rand() % MAX_Y;
		putpixel(fundo, fx, fy, 14);
	}
	y = 0;

	clear(tela);
	blit(fundo, tela, 0, 0, 0, 0, MAX_X, MAX_Y);
	draw_sprite(tela, nave, x, 4 * MAX_Y / 5);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);

	fade_in(pal, 2);

	text_mode(-1);
	textout(screen, font, "Pressione qualquer tecla para iniciar", 3 * MAX_X / 10, 2 * MAX_Y / 5, 14);
	clear_keybuf();
	readkey();

	tempo = 0;
	LOCK_VARIABLE(tempo);
	LOCK_FUNCTION(incrementa_tempo);
	install_int(incrementa_tempo, 1000);

	play_midi(musica, TRUE);

	while (!key[KEY_ESC])
	{
		show_mouse(NULL);
		clear(tela);
		blit(fundo, tela, 0, 0, 0, y, MAX_X, MAX_Y);
		blit(fundo, tela, 0, 0, 0, y - MAX_Y, MAX_X, MAX_Y);
		textprintf_centre(tela, font, MAX_X / 2, 30, 14, "X: %d - MX: %d - MY: %d", x,
			mouse_x, mouse_y);
		botEsq = (mouse_b & 1) ? (1) : (0);
		botDir = (mouse_b & 2) ? (1) : (0);
		textprintf_centre(tela, font, MAX_X / 2, 40, 14, "Bot Esq: %d - Bot Dir: %d",
			botEsq, botDir);
		textprintf_centre(tela, font, MAX_X / 2, 50, 14, "Tempo: %2d:%2d", tempo / 60,
			tempo % 60);
		draw_sprite(tela, nave, x, 4 * MAX_Y / 5);
		show_mouse(tela);
		blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
		if ((key[KEY_LEFT]) && (!key[KEY_RIGHT]))
		{
			x -= PASSO;
		}
		if ((key[KEY_RIGHT]) && (!key[KEY_LEFT]))
		{
			x += PASSO;
		}
		if (key[KEY_UP])
		{
			play_sample(som, 255, 128, 1000, FALSE);
		}
		y += SCROLL;
		if (y > MAX_Y)
		{
			y -= MAX_Y;
		}
	}

	remove_int(incrementa_tempo);

	/* play_midi(NULL, FALSE); */
	stop_midi();

	textout_centre(screen, font, "Pressione qualquer tecla para sair", MAX_X / 2, 2 * MAX_Y / 5, 14);
	clear_keybuf();
	readkey();

	fade_out(4);
}

void finaliza (void)
{
	allegro_exit();
}
