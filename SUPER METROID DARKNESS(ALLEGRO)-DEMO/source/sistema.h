#pragma once

#include <allegro.h>
#include <alpng.h>
#include <algif.h>
static volatile int FecharJogo = FALSE;
#define MAX_X 800
#define MAX_Y 600
#define FPS_V 50
#define COLOR_BITS 16 

void inicia (void);
void finaliza();
void BotaoFechar(void);
void draw_gif_sprite(); 
