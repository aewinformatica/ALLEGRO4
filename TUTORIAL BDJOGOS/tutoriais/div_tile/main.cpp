#include <allegro.h> 
 
#include <winalleg.h> 

#define mov   5
#define vel   300

#define resh 640
#define resw 480
/*---------------------------------------------------------------------------------------------*/ 

//estrutura de dados básica para a criação e manupulação de sprites 

struct sprite 

{ 

int px, py; // posição na tela 

int height,width; // largura e altura da imagem 

float dx, dy; // velocidade 

BITMAP *img ; 

}; 


void desenha_b();
void desenha_c();
void desenha_d();
void desenha_e();

void mov_c();
void mov_b();
void mov_e();
void mov_d();
void controlador();

volatile int velocidade = 0;

 BITMAP *buffer = NULL; // double buffer 
 BITMAP *fundo;
int indice = 0,px,py; //quantidade de imagens 

/*---------------------------------------------------------------------------------------------*/ 
int main() 
{ 





/*---------------------------------------------------------------------------------------------*/ 

//inicia o allegro e sua dependecias 
allegro_init(); 
 
install_keyboard(); 

//rotinas para controlar a velocidade da animação 
install_timer();
install_int(controlador,160); 
/*---------------------------------------------------------------------------------------------*/ 

set_color_depth(32); 
set_gfx_mode(GFX_AUTODETECT, resh, resw, 0, 0); 


fundo = load_bmp("fstar.bmp",NULL);
buffer = create_bitmap (640, 480); // cria um buffer de tela para o double buffer 

/*---------------------------------------------------------------------------------------------*/ 
desenha_b();


while (!key[KEY_ESC]) // enqunato não precionar a tecla ESC faça 

{
 	  while(velocidade >0)
	   {
if(key[KEY_UP]){ 
mov_c();
}
if(key[KEY_DOWN]){
mov_b();
}
if(key[KEY_LEFT]){
mov_e();
}
if(key[KEY_RIGHT]){
mov_d();
}
velocidade--;
}


//desenha a imagem mostra ela em tela e limpa o buffer de memoria 



 
}
 

/*---------------------------------------------------------------------------------------------*/ 
allegro_exit(); 
return 0; 
} 
END_OF_MAIN(); 

void mov_c()
{
indice=indice+1;
 py=py-mov;
if(indice>=4)
indice=0;
desenha_c();

}
void mov_b()
{

indice=indice+1; 
py=py+mov;
if(indice>=4)
indice=0;
desenha_b();
 

}
void mov_d()
{

indice=indice+1;
px=px+mov; 
if(indice>=4)
indice=0;
desenha_d();


}
void mov_e()
{

indice=indice+1; 
px=px-mov;
if(indice>=4)
indice=0;
desenha_e();
 

}

 void texto()
{
//          __ Cor do fundo do texto
//         | 

	
}
void desenha_b()
{
struct sprite pose ; //variavel sprite do tipo struct que controlara a pose 


pose.px = 10; //inicia o sprite na posição 400 do eixo x 
pose.py = 10; //inicia o sprite na posição 300 do eixo y 
pose.img = load_bitmap("tdiv_b.bmp",NULL); 
pose.height = 192; //divide a sprite por seis no sentido da largura 
pose.width = 128/4; 


masked_blit (fundo, buffer, 0, 0, 0, 0, 640, 480);
masked_blit (pose.img, buffer, (pose.width)*indice, 0, px, py, pose.width, pose.height); 
texto();
masked_blit (buffer, screen, 0, 0, 0, 0, 640, 480);

clear (buffer); 

}
void desenha_c()
{
struct sprite pose ; //variavel sprite do tipo struct que controlara a pose 


pose.px = 10; //inicia o sprite na posição 400 do eixo x 
pose.py = 10; //inicia o sprite na posição 300 do eixo y 
pose.img = load_bitmap("tdiv_c.bmp",NULL); 
pose.height = 48; //divide a sprite por seis no sentido da largura 
pose.width = 128/4; 

masked_blit (fundo, buffer, 0, 0, 0, 0, 640, 480);
masked_blit (pose.img, buffer, (pose.width)*indice, 0, px, py, pose.width, pose.height); 
 texto();
masked_blit (buffer, screen, 0, 0, 0, 0, 640, 480);

clear (buffer); 

}
void desenha_e()
{
struct sprite pose ; //variavel sprite do tipo struct que controlara a pose 


pose.px = 400; //inicia o sprite na posição 400 do eixo x 
pose.py = 400; //inicia o sprite na posição 400 do eixo y 
pose.img = load_bitmap("tdiv_e.bmp",NULL); 
pose.height = 48; //divide a sprite por seis no sentido da largura 
pose.width = 128/4; 

masked_blit (fundo, buffer, 0, 0, 0, 0, 640, 480);
masked_blit (pose.img, buffer, (pose.width)*indice, 0,px, py, pose.width, pose.height); 
texto();
masked_blit (buffer, screen, 0, 0, 0, 0, 640, 480);

clear (buffer); 

}
void desenha_d()
{
struct sprite pose ; //variavel sprite do tipo struct que controlara a pose 


pose.px = 10; //inicia o sprite na posição 400 do eixo x 
pose.py = 10; //inicia o sprite na posição 300 do eixo y 
pose.img = load_bitmap("tdiv_d.bmp",NULL); 
pose.height = 48; //divide a sprite por seis no sentido da largura 
pose.width = 128/4; 


masked_blit (fundo, buffer, 0, 0, 0, 0, 640, 480);
masked_blit (pose.img, buffer, (pose.width)*indice, 0, px, py, pose.width, pose.height); 
texto();
masked_blit (buffer, screen, 0, 0, 0, 0, 640, 480);

clear (buffer); 

}
void controlador()
{
 	 velocidade++;
 }
