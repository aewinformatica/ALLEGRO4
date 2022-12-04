#pragma once

class Fase
{
      public:
             Bloco bloco[CAMADAS][BLOCO_X][BLOCO_Y];
             BITMAP Fundo;
             
             void Carrega_Mapa(string filename);
             void Desenha_Topo(BITMAP * Buffer,BITMAP *Desenho_Bloco);
             void Desenha_Base(BITMAP * Buffer, BITMAP *Desenho_Bloco);
      };
