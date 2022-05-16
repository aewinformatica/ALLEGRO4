#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro.h>


#include "mario.h"
#include "imagens.h"
#include "scroll.h"
#include "sprites.h"
#include "fase.h"
#include "colisoes.h"
#include "entrada.h"

void my_timer_handler(void)
{
   counter++;
}
END_OF_FUNCTION(my_timer_handler);

int inicia_allegro(int max_x, int max_y, int prof)
{
	allegro_init(); install_keyboard();
	install_mouse(); install_timer();

	if (install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL) < 0)
	{
		printf("Erro no som aê!\n");
		return(FALSE);
	}

	set_volume(vol_som,vol_musica);
	set_color_depth(prof);
	if (set_gfx_mode(GFX_AUTODETECT,max_x,max_y,0,0) < 0)
	{
		printf("Erro no video aê!\n");
		return(FALSE);
	}

   LOCK_VARIABLE(counter);
   LOCK_FUNCTION(my_timer_handler);
   install_int_ex(my_timer_handler,BPS_TO_TIMER(FRAME_RATE));

	return(TRUE);
}

void inicia_jogo(void)
{
   int _pulo[DURACAO_PULO] = {8, 8, 8, 6, 6, 6, 4, 4, 4, 2, 2, 1};
   int _super_pulo[DURACAO_SUPER_PULO] = {14, 14, 14, 12, 12, 12, 10, 10, 10,
      8, 8, 8, 6, 6, 6, 4, 4, 4, 2, 2, 2, 1, 1, 1};
   int _proximo_quadro[MAX_IMAGEM] = 
{
   0,
   2, 3, 4, 1, /* interrogacao */
   5,
   7, 8, 9, 6, /* bloco */
   10,
   11,
   13, 14, 15, 12, /* nota */
   17, 16, /* goomba */
   18,
   20, 19, /* peixe */
   22, 23, 24, 21, /* moeda */
   26, 27, 28, 29, 25, /* fumaca */
   31, 30, /* spiny */
   33, 34, 35, 32, /* koopa chutado */
   36,
   37,
   38,
   39,
   40,
   41,
   42,
   43,
   44,
   45,
   46,
   47,
   48,
   50, 51, 49, /* grama 1 */
   53, 54, 52, /* grama 2 */
   56, 57, 55, /* grama 3 */
   58,
   59,
   60,
   62, 63, 61, /* palmeira centro */
   65, 66, 64, /* palmeira esquerda */
   68, 69, 67, /* palmeira direita */
   71, 72, 70, /* palmeira topo */
   73,
   74,
   76, 75, /* mario pequeno */
   77,
   78,
   79,
   80,
   82, 83, 84, 81, /* bloco azulado */
   87, /* folha esquerda */
   88, /* folha direita */
   89, /* folha esquerda */
   90, /* folha direita */
   91, /* folha esquerda */
   92, /* folha direita */
   89, /* folha esquerda */
   86, /* folha direita */
   93,
   94,
   95,
   96,
   97,
   98,
   99,
   100,
   101,
   102,
   103,
   104,
   105,
   106,
   107,
   108,
   110, 111, 112, 113, 109, /* lava */
   114,
   115,
   117, 118, 119, 116, /* bola de fogo */
   120,
   122, 123, 124, 121, /* mario grande */
   125, 
   126,
   128, 127, /* koopa */
   129,
   130,
   131
};

   srand(time(NULL));

   /* inicia os valores de pulo/queda e de proximo_quadro */
   memcpy(pulo,_pulo,sizeof(pulo));
   memcpy(super_pulo,_super_pulo,sizeof(super_pulo));
   memcpy(proximo_quadro,_proximo_quadro,sizeof(proximo_quadro));

   /* cria buffer duplo */
   #ifdef VERSAO_640_480
      tela = create_bitmap(RES_TILE_X*(TILE_SIZE+2)*2,(RES_TILE_Y+2)*(TILE_SIZE)*2);
   #else
      tela = create_bitmap(RES_TILE_X*(TILE_SIZE+2),(RES_TILE_Y+2)*(TILE_SIZE));
   #endif

   configura_teclas();

   /* mario aponta para o primeiro sprite do vetor */
   mario = sprite;

   /* le os sons do jogo */
   wav_moeda = load_wav("sfx/moeda.wav");
   wav_pulo = load_wav("sfx/pulo.wav");
   wav_quebra = load_wav("sfx/quebra.wav");
   wav_bump = load_wav("sfx/bump.wav");
   wav_cresce = load_wav("sfx/cresce.wav");
   wav_diminui = load_wav("sfx/diminui.wav");
   wav_morreu = load_wav("sfx/morreu.wav");
   wav_cogumelo = load_wav("sfx/cogumelo.wav");
   wav_cano = load_wav("sfx/cano.wav");
   wav_parabola = load_wav("sfx/parabola.wav");
   wav_1up = load_wav("sfx/1up.wav");
   wav_fim = load_wav("sfx/fim.wav");

   mario_vidas = 3;
   mario_moedas = 0;

   le_imagens();
   le_fase(arq_fase[fase_atual]);
}

void processa_linha_de_comando(int argc, char **argv)
{
   register int i;

   /* opcoes default */
   modo_16_bits = FALSE;
   strcpy(arq_fase[0],"maps/level1-1.map");
   strcpy(arq_fase[1],"maps/castelo.map");
   vol_musica = 255;
   vol_som = 128;
   fase_atual = 0;
   

   for(i = 0; i < argc; i++)
   {
      if (strcmp(argv[i],"-mapa") == 0)
         if (i < argc - 1)
            strcpy(arq_fase[fase_atual],argv[i+1]);
      if (strcmp(argv[i],"-som") == 0)
         if (i < argc - 1)
            vol_som = atoi(argv[i+1]);
      if (strcmp(argv[i],"-musica") == 0)
         if (i < argc - 1)
            vol_musica = atoi(argv[i+1]);
      if (strcmp(argv[i],"-16") == 0)
         modo_16_bits = TRUE;
   }
}

int main(int argc, char **argv)
{
    

   processa_linha_de_comando(argc,argv);
   
   inicia_allegro(RES_X,RES_Y,(modo_16_bits) ? 16 : 8);
   inicia_jogo();

   counter = 0;
   lacos = 1;

   while(!key[KEY_ESC] && mario_vidas > -1)
   {
      if (counter)
      {
         clear_to_color(tela,224);
         if (mario->estado < ESTADO_MORTO) move_sprites();
         move_mario();
         calcula_scroll();
         detecta_colisoes();
         desenha();
         counter = 0;
         lacos++;
      }

     
      
      if (mario->estado == ESTADO_MORTO) le_fase(arq_fase[fase_atual]);
      if (passou_de_fase) le_fase(arq_fase[1]);
  	 blit_scrolled(); 
   }

   // if (passou_de_fase) le_fase(arq_fase[1]);
	 fim_de_jogo();


   allegro_exit();
	return(0);
}
END_OF_MAIN();
