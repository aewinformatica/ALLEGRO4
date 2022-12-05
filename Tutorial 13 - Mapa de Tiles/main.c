#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>

const int TILESIZE = 50;

enum
{
  SOLO = 0,
  GRAMA = 1,
  AGUA = 2,
  LAVA = 3,
  PEDRA = 4
};


int** Carregar_Mapa(const char* nome_arquivo, int* linhas, int* colunas)
{
   FILE* f = fopen(nome_arquivo, "r");
   int** matriz;

   if(f != NULL)
   {
     int i, j;

     fscanf(f, "%d %d", linhas, colunas);

     //ALOCA O MAPA
     matriz = (int**) malloc ( (*linhas) * sizeof(int*));
     for(i = 0; i < *linhas; i++)
      matriz[i] = (int*) malloc( (*colunas) * sizeof(int));

     //CARREGA TILES
     for(i = 0; i < *linhas; i++)
     {
        for(j = 0; j < *colunas; j++)
        {
          fscanf(f, "%d", &matriz[i][j]);
        }
     }

     fclose(f);
   }

   return matriz;
}

void Desenhar_Mapa(BITMAP* buffer, int** mapa, int linhas, int colunas)
{
    int i, j;

    for(i = 0; i < linhas; i++)
     {
        for(j = 0; j < colunas; j++)
        {
           if(mapa[i][j] == SOLO)
             rectfill(buffer, j * TILESIZE, i * TILESIZE, (j * TILESIZE) + TILESIZE, (i * TILESIZE) + TILESIZE, makecol(183,150,89));

           else if(mapa[i][j] == GRAMA)
             rectfill(buffer, j * TILESIZE, i * TILESIZE, (j * TILESIZE) + TILESIZE, (i * TILESIZE) + TILESIZE, makecol(0,230,0));

           else if(mapa[i][j] == AGUA)
             rectfill(buffer, j * TILESIZE, i * TILESIZE, (j * TILESIZE) + TILESIZE, (i * TILESIZE) + TILESIZE, makecol(70,160,255));

           else if(mapa[i][j] == LAVA)
             rectfill(buffer, j * TILESIZE, i * TILESIZE, (j * TILESIZE) + TILESIZE, (i * TILESIZE) + TILESIZE, makecol(215,0,0));

           else if(mapa[i][j] == PEDRA)
             rectfill(buffer, j * TILESIZE, i * TILESIZE, (j * TILESIZE) + TILESIZE, (i * TILESIZE) + TILESIZE, makecol(133,133,133));
        }
     }
}

void Libera_Mapa(int** mapa, int linhas)
{
   int i;
   for(i = 0; i < linhas; i++)
    free(mapa[i]);

   free(mapa);
}

volatile int exit_program;
void fecha_programa() { exit_program = TRUE; }
END_OF_FUNCTION(fecha_programa)

volatile int ticks;
void tick_counter() { ticks++; }
END_OF_FUNCTION(tick_counter)

int main()
{
  allegro_init();
  install_timer();
  install_keyboard();
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
  set_window_title("Tutorial 13 - Mapa de Tiles");

  exit_program = FALSE; 
  LOCK_FUNCTION(fecha_programa);
  LOCK_VARIABLE(exit_program);
  set_close_button_callback(fecha_programa);
  
  ticks = 0;
  LOCK_FUNCTION(tick_counter);
  LOCK_VARIABLE(ticks);
  install_int_ex(tick_counter, BPS_TO_TIMER(60));

  ///BITMAPS
  BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

  ///MAPA
  int linhas, colunas;
  int** mapa = Carregar_Mapa("mapa.txt", &linhas, &colunas);

  ///GAME LOOP
  while(!exit_program)
  {
     while(ticks > 0 && !exit_program)
     {
      //INPUT
      if(key[KEY_ESC])
       fecha_programa();

      //UPDATE

      //DRAW
      Desenhar_Mapa(buffer, mapa, linhas, colunas);
      draw_sprite(screen, buffer, 0, 0);
      clear_to_color(buffer, makecol(255,255,255));

     ticks--;
     }
  }

  ///FINALIZAÇÂO
  Libera_Mapa(mapa, linhas);
  destroy_bitmap(buffer);

  return 0;
}
END_OF_MAIN();
