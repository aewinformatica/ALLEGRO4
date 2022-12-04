#include <allegro.h>
#include <iostream>
#include <fstream>
using namespace std;

void CarregaTextura();
void CarregaMatriz();
void DesenhaCenario(BITMAP * Cenario,int);

	const int QTilex = 40;
	const int QTiley = 30;
	
    const int xTile = 16; // Largura do Tile
	const int yTile = 16; // Altura do Tile
	const int exyTile =27;
    const int QtdFases = 3;
    const char *ArqFases[] = {"fase1.txt","fase2.txt","fase3.txt"};
    int Fase_Atual = 0;
    
const int MAX_TILE = 160;
const int MAX_IMAGEM = 120;

BITMAP *bmpTexturas[MAX_TILE];
BITMAP *Buffer;
BITMAP *Fundo; 
BITMAP *Mouse;
BITMAP *Icone;

BITMAP *grafico;
	
int Mapa[QtdFases][QTilex][QTiley];
const int RES_Y  = 480;
const int RES_X  = 640;

extern volatile int mouse_x; 
extern volatile int mouse_y; 
extern volatile int mouse_z; 
extern volatile int mouse_b; 
extern volatile int mouse_pos;

	
int main()
{
	allegro_init();
	install_keyboard();
	install_mouse();
	install_timer();
	void install_event();
	
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
   set_window_title("CRIADOR DE MAPA - HK");
   

Buffer = create_bitmap(640,480);

grafico = create_bitmap(SCREEN_W,SCREEN_H);
Fundo = create_bitmap(64,64);
Fundo = load_bitmap("fundo.bmp",NULL);
Mouse = load_bitmap("mario_glove.bmp",NULL); 

set_mouse_sprite(Mouse);

	CarregaTextura();
    CarregaMatriz();
         for(int j = 0; j < RES_Y; j += Fundo->h)
         for(int i = 0; i < RES_X; i += Fundo->w)
            stretch_blit(Fundo,Buffer,0,0,Fundo->w,Fundo->h,2*i,2*j,2*Fundo->w,2*Fundo->h);
            
    
    
 int i = 0;
 int des = 16;   
    
     for (i=0; i<640; i+=16){
        line(grafico, i, 640, i, 0, makecol(255,255,255));  
    }
    for (i=0; i<480; i+=16){
        line(grafico, 0, i+des, 640, i+des, makecol(255,255,255));  
    }
    

draw_sprite(Buffer, grafico, 0, 0);
DesenhaCenario(Buffer,Fase_Atual);   
   /*  // desenhando o eixo x
    line(Buffer, 50, 420, 580, 420, makecol(255,255,255));
    // desenhando o eixo y
    line(Buffer, 50, 50, 50, 420, makecol(255,255,255));
    // desenhando a seta do eixo y
    triangle(Buffer, 45,50 , 50,45 , 55,50, makecol(255,255,255));
    // desenhando a seta do eixo y
    triangle(Buffer, 580,415 , 585,420 , 580,425, makecol(255,255,255));             
   */

   show_mouse(Buffer); 
   

 
	while(!key[KEY_ESC]){
                         


textprintf_ex(Buffer,font,SCREEN_W-200,0,makecol(0,0,0),255,"Mouse x: %d", mouse_x);
textprintf_ex(Buffer,font,SCREEN_W-200,10,makecol(0,0,0),255,"Mouse y: %d", mouse_y);

if (mouse_b & 1)
textprintf_ex(screen, font, 0, 0, makecol(255, 100, 200),-1, "Botao esquerdo clicado");

if (mouse_b & 2)
textprintf_ex(screen, font, 0, 0, makecol(255, 100, 200),-1, "Botao direito clicado");

if (mouse_b & 4)
textprintf_ex(screen, font, 0, 0, makecol(255, 100, 200),-1, "Rodinha do mouse Pressionada");

                    
                    blit(Buffer,screen,0,0,0,0,640,480);
                         }

	
	allegro_exit();

	return 0;
}

END_OF_MAIN();
void CarregaTextura(){
int i,j,k,l,m;
	// Carregando o ponteiro com a imagem que possui todos os tiles
	BITMAP *bmpTiles=NULL;
	bmpTiles = create_bitmap(320,240);
	bmpTiles = load_bitmap("blocos.bmp",NULL);
	

     	// Definindo o tamanho 32x32 para cada tile
    for(j = 0,k =1;(j<12 )&& (k < MAX_IMAGEM); j++)  	
	for (i = 0; (i < 20) && (k < MAX_IMAGEM);k++){
	 if(k<MAX_TILE)
	{
		bmpTexturas[k] = create_bitmap(xTile,yTile);
		blit(bmpTiles,bmpTexturas[k],i*xTile,j*yTile,0,0,xTile,yTile);  
	}
	else
	{
      bmpTexturas[k] = create_bitmap(xTile,exyTile);
      blit(bmpTiles,bmpTexturas[k],i*xTile,j*yTile,0,0,xTile,exyTile);  
    }
    bmpTexturas[0] = create_bitmap(xTile,yTile);
                   blit(bmpTiles,bmpTexturas[0],bmpTiles->w-xTile,bmpTiles->h-yTile,0,0,xTile,yTile);
    //clear(bmpTexturas[0]);
    //clear_to_color(bmpTexturas[0], makecol(0, 0, 90));
   }

     }
 void CarregaMatriz(){
      int x,y;
      int Fase = 0;
      
      ifstream Leitor(ArqFases[Fase_Atual]);
      
      char sLinha[QTilex];
      
      for(y=0;y<QTiley;y++){
                      Leitor >> sLinha;      
              for(x = 0;x<QTiley;x++){
                    Mapa[Fase][x][y] = sLinha[x];
                    }              
      
      }
      Leitor.close();
 }
void DesenhaCenario(BITMAP * Cenario,int Fase){
     int x = 0;
     int y = 0;
     
     	//Monta o Cenário
	for (y=0;y<QTiley;y++)
	{
		for (x=0;x<QTilex;x++)
		{   if  ((char)Mapa[Fase][x][y] == '#') // Parede
			draw_sprite(Cenario,bmpTexturas[0],y*yTile,x*xTile);
			else
			if  ((char)Mapa[Fase][x][y] == '.') // Parede
			draw_sprite(Cenario,bmpTexturas[2],y*yTile,x*xTile);
			if  ((char)Mapa[Fase][x][y] == '-') // Parede
			draw_sprite(Cenario,bmpTexturas[3],y*yTile,x*xTile);
			if  ((char)Mapa[Fase][x][y] == '+') // Parede
			draw_sprite(Cenario,bmpTexturas[4],y*yTile,x*xTile);
		}
	}

     }

