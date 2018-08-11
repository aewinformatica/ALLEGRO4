#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>
#include <alpng.h>  
#include "sistema.h"

#include "Canim.h"
#define BLACK makecol(0,0,0)
#define WHITE makecol(255,255,255)


#define POS_VER		220
#define VEL_HOR		4

//sprite variables
BITMAP *IMG[44];

SPRITE lutador;

BITMAP *grabframe(BITMAP *source,
int width, int height,
int startx, int starty,
int columns, int frame)
{
BITMAP *temp = create_bitmap(width,height);
int x = startx + (frame % columns) * width;
int y = starty + (frame / columns) * height;
blit(source,temp,x,y,0,0,width,height);
return temp;
}

volatile int  velocidade = 0; 

void loop();

int vel_vertical, pulo_vel_hor;

int anim_parado, anim_andando, anim_recuando, anim_soco, anim_chute,
		anim_abaixar, anim_soco_baixo, anim_chute_baixo, anim_pulo;

bool atacando=false, pulando=false;

int ANIM_PARADO[] = { 0,1,2,3,4,5,4,3,2,1 };
int ANIM_ANDANDO[] = {6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
int ANIM_RECUANDO[] = {21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6};
int ANIM_SOCO[] = {34,35,36,36,35,34};
int ANIM_CHUTE[] = {37,38,39,39,38,37};
int ANIM_ABAIXAR[] = {22,23,24};
int ANIM_SOCO_BAIXO[] = {42,40,41,41,40,42};
int ANIM_CHUTE_BAIXO[] = {42,43,44,44,43,42};
int ANIM_PULO[] = {25,26,27,28,29,29,30,31,32,33};

CAnim anim;

void Desenha_imagem(BITMAP * IMG , BITMAP * Buffer);

static void toilet(BITMAP *b)
{
    int c1, c2, c;
    int x, y;

    c1 = makecol(0x00, 0x00, 0xe0);
    c2 = makecol(0xe0, 0xe0, 0xf0);

    for (y = 0; y < b->h; y += 16) {
	c = (y % 32) ? c1 : c2;
	for (x = 0; x < b->w; x += 16) {
	    rectfill(b, x, y, x+15, y+15, c);
	    c = (c == c1) ? c2 : c1;
	}
    }
}
void main(void)
{
Sistema Jogo;     

install_int_ex(loop,BPS_TO_TIMER(25));

srand(time(NULL));

BITMAP *Buffer = NULL;
        Buffer = create_bitmap(800,600);

BITMAP *temp;
temp = load_bitmap("karate.png", NULL);

BITMAP *bg1,*bg2;
       bg1  =load_bitmap("bg1.png",NULL);
       bg2  =load_bitmap("bg2.png",NULL);

for (int n=0; n<=44; n++){
IMG[n] = grabframe(temp,104,124,0,0,9,n);
}
destroy_bitmap(temp);

			lutador.y = POS_VER;
			lutador.x = 50;
			
//---------------------------------------------------------------------------			
			//Carregar as animacoes
                          //retorna o indice da anim
			anim_parado = anim.Adiciona_Anim(10, ANIM_PARADO);
			anim_andando = anim.Adiciona_Anim(16, ANIM_ANDANDO);
			anim_recuando = anim.Adiciona_Anim(16, ANIM_RECUANDO);
			anim_soco = anim.Adiciona_Anim(6, ANIM_SOCO);
			anim_chute = anim.Adiciona_Anim(6, ANIM_CHUTE);
			anim_abaixar = anim.Adiciona_Anim(3, ANIM_ABAIXAR);
			anim_soco_baixo = anim.Adiciona_Anim(6, ANIM_SOCO_BAIXO);
			anim_chute_baixo = anim.Adiciona_Anim(6, ANIM_CHUTE_BAIXO);
			anim_pulo = anim.Adiciona_Anim(10, ANIM_PULO);
//---------------------------------------------------------------------------

while (!key[KEY_ESC])
{


clear(Buffer);

 
while(velocidade > 0){
if(pulando)
	{
		lutador.y += vel_vertical;
		vel_vertical++;

		lutador.x += pulo_vel_hor;
		//testar os limites da tela
		if(lutador.x < VEL_HOR  || lutador.x > (640 - lutador.largura))
		{
			pulo_vel_hor = 0;
		}
		

		//testar fim de pulo
		if(lutador.y >= POS_VER)
		{
			lutador.y = POS_VER;
			pulando=false;
//---------------------------------------------------------------------------			
			anim.Atribuir_Anim_Atual(anim_parado);
//---------------------------------------------------------------------------
		}
	}
	else
	if(atacando)
	{
		if(anim.Anim_Feita())
		{
			atacando = false;		
		}
	}
  else 
	{
	 if(key[KEY_LSHIFT])
	 {
	  if(key[KEY_DOWN]) 
		{
		 anim.Atribuir_Anim_Atual(anim_soco_baixo);
		}
		else
		{
		 anim.Atribuir_Anim_Atual(anim_soco);
		}
		
		atacando = true;
	 }
	 else
	 if(key[KEY_LCONTROL])
	 {
		if(key[KEY_DOWN]) 
		{
		 anim.Atribuir_Anim_Atual(anim_chute_baixo);
		}
		else
		{
		 anim.Atribuir_Anim_Atual(anim_chute);
		}

		atacando = true;
	 }
	 else
	 if(key[KEY_UP])
	 {
		//o 'true' no final indica que a animacao deve
		//parar no ultimo quadro ao inves de reiniciar.
		anim.Atribuir_Anim_Atual(anim_pulo,true);
		

		pulando = true;
		vel_vertical = -15;

		if(key[KEY_LEFT]) pulo_vel_hor = - VEL_HOR;
		else
		if(key[KEY_RIGHT]) pulo_vel_hor = VEL_HOR;
		else pulo_vel_hor = 0;

	 }
	 else
	 if(key[KEY_DOWN])
	 {
		//o 'true' no final indica que a animacao deve
		//parar no ultimo quadro ao inves de reiniciar.
		anim.Atribuir_Anim_Atual(anim_abaixar,true);
	 }
	 else
	 if(key[KEY_LEFT])
	 {
		anim.Atribuir_Anim_Atual(anim_recuando);

		if(lutador.x > VEL_HOR)
		{
		  lutador.x -= VEL_HOR;
		}
	 }
	 else
   if(key[KEY_RIGHT])
	 {
		anim.Atribuir_Anim_Atual(anim_andando);

		if(lutador.x < (640 - lutador.largura))
		{
		  lutador.x += VEL_HOR;
		}
	 }
	 else
	 {
		anim.Atribuir_Anim_Atual(anim_parado);
	 }
	}//else - if(atacando)

	anim.Atualizar();
	lutador.quadro_atual = anim.Quadro_Atual();

velocidade--;
}
//toilet(Buffer);
draw_trans_sprite(Buffer, bg2,55,10);
draw_trans_sprite(Buffer, bg1,0,0);
draw_trans_sprite(Buffer, IMG[lutador.quadro_atual], lutador.x, lutador.y);
textprintf_ex(Buffer, font, 0, 0, makecol(0, 255, 255),-1,"SPRITE EM MOVIMENTO");
textprintf_ex(Buffer,font,0,10,makecol(255,0,0),-1,"Frame:%d",lutador.quadro_atual);

blit(Buffer,screen,0,0,0,0,768,368);
release_screen();
}
for (int n=0; n<44; n++)
destroy_bitmap(IMG[n]);
return ;
}
END_OF_MAIN();

void loop()
{
     velocidade++;
 }

     
