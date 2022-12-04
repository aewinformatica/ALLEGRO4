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

#define MAX_MONSTRO	30
#define MONSTRO_X	32
#define MONSTRO_Y	32
#define MAX_DX		5
#define MAX_DY		5

#define ESP_PONTOS	200
#define ARENA_X		(MAX_X) - (ESP_PONTOS)
#define ARENA_Y		(MAX_Y)

typedef struct _MONSTRO
{
	int x, y;
	int dx, dy;
	int show;
} MONSTRO;

volatile int tempo;

void aumenta_tempo(void);
int inicia(void);
void principal(void);
void finaliza(void);
void imprime_placar(BITMAP *bmp, int pontos, int maior, int num_monstros);

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

void aumenta_tempo (void)
{
	tempo++;
}
END_OF_FUNCTION(aumenta_tempo);

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
			printf("Erro ao inicializar o modo grafico.\n");
			return (FALSE);
		}
	}

	if (install_sound(DIGI_CARD, MIDI_CARD, NULL) < 0)
	{
		if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
		{
			printf("Erro ao inicializar o som.\n");
			return (FALSE);
		}
	}
	set_volume(192, 255);

	set_config_file("jogo.rec");

	LOCK_VARIABLE(tempo);
	LOCK_FUNCTION(aumenta_tempo);

	return (TRUE);
}

void principal (void)
{
	MONSTRO monstro[MAX_MONSTRO];
	BITMAP *bmp_monstro;
	BITMAP *tela;
	PALETTE pal;
	SAMPLE *certo, *errado;
	MIDI *musica;
	int mx, my;
	int num_monstros;
	int pontos, maior;
	int acertou;
	int i;

	tela = create_bitmap(MAX_X, MAX_Y);

	bmp_monstro = load_bitmap("monstro.bmp", pal);

	certo = load_sample("certo.wav");
	errado = load_sample("errado.wav");
	musica = load_midi("caverns.mid");

	srand(time(0));
	for (i = 0; i < MAX_MONSTRO; i++)
	{
		monstro[i].x = rand() % (ARENA_X - MONSTRO_X);
		monstro[i].y = rand() % (ARENA_Y - MONSTRO_Y);
		do
		{
			monstro[i].dx = (rand() % (MAX_DX * 2)) - MAX_DX;
		} while (monstro[i].dx == 0);
		do
		{
			monstro[i].dy = (rand() % (MAX_DY * 2)) - MAX_DY;
		} while (monstro[i].dy == 0);
		monstro[i].show = TRUE;
	}
	num_monstros = MAX_MONSTRO;

	mx = my = -1;

	pontos = 0;
	maior = get_config_int(NULL, "HighScore", 0);

	fade_in(pal, 4);

	play_midi(musica, TRUE);

	tempo = 0;
	install_int(aumenta_tempo, 1000);

	clear_keybuf();
	do
	{
		if ((mouse_b & 1) && ((mouse_x != mx) || (mouse_y != my)))
		{
			mx = mouse_x;
			my = mouse_y;
			for (i = 0, acertou = FALSE; i < MAX_MONSTRO; i++)
			{
				if ((mouse_x >= monstro[i].x) && (mouse_x <= (monstro[i].x + MONSTRO_X)) &&
					(mouse_y >= monstro[i].y) && (mouse_y <= (monstro[i].y + MONSTRO_Y)) &&
					(monstro[i].show))
				{
					monstro[i].show = FALSE;
					acertou = TRUE;
					pontos += (abs(monstro[i].dx) + abs(monstro[i].dy)) * 50;
					num_monstros--;
				}
			}
			if (acertou)
			{
				play_sample(certo, 255, 128, 1000, FALSE);
			}
			else
			{
				play_sample(errado, 255, 128, 1000, FALSE);
				pontos -= 100;
			}
		}

		show_mouse(NULL);
		clear(tela);
		for (i = 0; i < MAX_MONSTRO; i++)
		{
			if (monstro[i].show)
			{
				monstro[i].x += monstro[i].dx;
				monstro[i].y += monstro[i].dy;
				if ((monstro[i].x < 0) || (monstro[i].x > (ARENA_X - MONSTRO_X)))
				{
					monstro[i].dx = -monstro[i].dx;
				}
				if ((monstro[i].y < 0) || (monstro[i].y > (ARENA_Y - MONSTRO_Y)))
				{
					monstro[i].dy = -monstro[i].dy;
				}
				draw_sprite(tela, bmp_monstro, monstro[i].x, monstro[i].y);
			}
		}
		imprime_placar(tela, pontos, maior, num_monstros);
		show_mouse(tela);
		blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
	} while ((!key[KEY_ESC]) && (num_monstros > 0));

	stop_midi();

	remove_int(aumenta_tempo);

	pontos -= num_monstros * 50;
	pontos -= (tempo - 30) * 5 / 3;
	if (pontos > maior)
	{
		maior = pontos;
		set_config_int(NULL, "HighScore", pontos);
	}
	clear(tela);
	imprime_placar(tela, pontos, maior, num_monstros);
	blit(tela, screen, 0, 0, 0, 0, MAX_X, MAX_Y);

	while (key[KEY_ESC]);
	clear_keybuf();
	textout_centre(screen, font, "Pressione qualquer tecla para continuar", (ARENA_X) / 2, (ARENA_Y) / 2 - 8, 8);
	while (!keypressed());

	fade_out(4);
}

void finaliza (void)
{
	allegro_exit();
}

void imprime_placar (BITMAP *bmp, int pontos, int maior, int num_monstros)
{
	textout_centre(bmp, font, "Pontos", (ARENA_X) + (ESP_PONTOS) / 2, 180, 8);
	textprintf_centre(bmp, font, (ARENA_X) + (ESP_PONTOS) / 2, 200, 9, "%d", pontos);
	textout_centre(bmp, font, "Maior Pontuacao", (ARENA_X) + (ESP_PONTOS) / 2, 230, 8);
	textprintf_centre(bmp, font, (ARENA_X) + (ESP_PONTOS) / 2, 250, 9, "%d", maior);
	textout_centre(bmp, font, "Restantes", (ARENA_X) + (ESP_PONTOS) / 2, 280, 8);
	textprintf_centre(bmp, font, (ARENA_X) + (ESP_PONTOS) / 2, 300, 9, "%d", num_monstros);
	textout_centre(bmp, font, "Tempo", (ARENA_X) + (ESP_PONTOS) / 2, 330, 8);
	textprintf_centre(bmp, font, (ARENA_X) + (ESP_PONTOS) / 2, 350, 9, "%2d:%2d", tempo / 60, tempo % 60);

	vline(bmp, ARENA_X, 0, ARENA_Y, 8);
}
