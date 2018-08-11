#pragma once
#include <allegro.h> 
#include <alpng.h> 
#include <string>
#include <iostream>
using namespace std;
#define STAND    0
#define FORWARD  1  
#define BACKWARD 2
#define JUMP     8
#define CROUCH   9
#define SPRITESHEET 10
#define DATASHEET 20
#define RLESHEET 30
#define WHITE makecol(255,255,255)
#define MAX_ANIM 32
#define SETFRAME false
#define SETARRAY true



class Sprite {
      public:
             Sprite();
             int x,y,w,h;
             bool atacando, pulando;
             
             int dir;
             BITMAP *animations[255];
             int primeiro_frame;
             int ultimo_frame;
             int frame_atual;
             int iniciar;
             int estado;
             int tipo;
             
             //VARIAVEIS PARA RECEBER A ARRAY
             int anim_parado;
             int anim_pulo;
             int anim_abaixar;
             int anim_andando;
             int anim_recuando;
             int anim_soco;
             int anim_soco_baixo;
             int anim_chute;
             int anim_chute_baixo;
             //SpriteSheet,DAT,DAT RLE
             void carregarSprite(string arquivo);
             //void carregarSprite(string arquivo,int tipoSprite);
             void carregarSprite(string arquivo,int RLEindex);
             //Carregar animações de Arquivo
             void carregarAnimacoes();
             
             char *ArqFasesAnimacao[3];
             int iQtdAnimFases;
             int ANIMACOES[3][32];

             //void desenhar(BITMAP *Buffer, int tipoSprite);
             void desenhar(BITMAP *Buffer);
             //void desenhar(BITMAP *Buffer, bool AnimArray=false);
             //void desenhar(BITMAP *Buffer, bool AnimArray);
             void setFrame(int primeiro_frame, int quantidade_frames);
             int Adiciona_Anim(int num_pos, int *sequencia);//retorna o indice da anim
             int Atribuir_Anim_Atual(int anim, bool trava=false);
             int Atualizar(void); //calcula e retorna o quadro_atual
             
             void Reinicia_Anim();
             void controle();
             //|******************************************************************************|
             //| Nome: bmpGrabFrame                                                          *| 
             //| Desc: Funcao para pegar um tile especifico, do bitmap source, e retornalo.  *|
             //| Pams: BITMAP *source , o bitmap onde estao os tiles.                        *| 
             //|       int width , a largura de cada tile.                                   *|
             //|       int heigth, o comprimento de cada tile.                               *|
             //|       int startx, posicao x do source que começa a ser exibido.             *|
             //|       int starty, posicao y do source que começa a ser exibido.             *|
             //|       int columns,  o numero de colunas que o bitmap com os tiles possui.   *|
             //|       int frame ,  qual do frame do bitmap com os tiles eh para retornar.   *|
             //|******************************************************************************| 
               
              BITMAP *grabframe(BITMAP *source,int width, int height,int startx, int starty,int columns, int frame){
                BITMAP *temp = create_bitmap(width,height);
                int x = startx + (frame % columns) * width;
                int y = starty + (frame / columns) * height;
                blit(source,temp,x,y,0,0,width,height);
                return temp;
              };
              BITMAP *grabframe(BITMAP *source,int startx, int starty,int columns,int lines, int frame){

                int width = source->w/columns;
                int height = source->h /lines;
                
                
                BITMAP *temp = create_bitmap(width,height); 
                
                int x = startx + (frame % columns) * width;
                int y = starty + (frame / columns) * height;
                blit(source,temp,x,y,0,0,width,height);
                return temp;
              };
              
              //CRIADA POR WAGNER G JESUS Aka HITMAN
              BITMAP *rle_to_bitmap(RLE_SPRITE *source){
              //CRIA UM BITMAP Temporario para tirar a cor de fundo           
              BITMAP *RLEBMP;
              RLEBMP = create_bitmap_ex(source->color_depth, source->w, source->h);
            
              clear_to_color(RLEBMP, bitmap_mask_color(RLEBMP));
              draw_rle_sprite(RLEBMP, source, 0, 0);    
              source = NULL;
              
              return RLEBMP;
              };              
               
              
      protected:
                int num_anim;
                int anim_atual;
                int *anim_seq[MAX_ANIM];//array para guardar a sequencia de animaçoes
                bool trava_ult_quadro;
                bool anim_feita;
                int anim_cont;
                int anim_ind;
                int anim_freq;
                int quadro_atual;
            
};
