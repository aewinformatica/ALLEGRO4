#include <allegro.h>

int main()
{
	allegro_init();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

	int i=0;
	int x=0;
	int y=0;

	const int xTile = 32; // Largura do Tile
	const int yTile = 32; // Altura do Tile

	// Posições dos tiles no mapa bidimencional.
	int Mapa[15][20] = {
	3,3,3,3,4,5,5,5,5,6,2,2,1,10,10,8,9,9,9,9,
 	3,3,3,3,4,5,5,5,5,6,2,2,1,7,7,7,9,9,9,9,
 	3,3,3,3,4,5,5,5,5,6,2,2,1,7,7,7,9,9,9,9,
 	3,3,3,3,4,5,5,5,5,6,2,2,1,7,7,8,9,9,9,9,
 	3,3,3,3,4,5,5,5,5,6,2,2,1,7,7,8,8,8,8,8,
 	3,3,3,3,4,5,5,5,5,6,2,2,1,7,7,7,7,7,7,7,
 	3,3,3,3,4,5,5,5,5,6,2,2,1,7,7,7,7,7,7,7,
 	3,3,3,3,4,5,5,5,5,6,2,2,1,7,0,10,7,7,10,0,
 	3,3,3,3,4,5,5,5,5,6,2,2,1,7,10,0,7,7,10,0,
 	3,3,3,3,3,4,5,5,5,6,2,2,1,7,0,10,7,7,10,0,
 	3,3,3,3,3,3,4,5,5,6,2,2,1,7,10,0,7,7,10,0,
 	3,3,3,3,3,3,4,5,5,6,2,2,1,7,0,10,7,7,10,0,
 	3,3,3,3,3,3,4,5,5,6,2,2,1,7,10,0,7,7,10,0,
 	3,3,3,3,3,3,4,5,5,6,2,2,1,7,0,10,7,7,10,0,
 	3,3,3,3,3,3,4,5,5,6,2,2,1,7,10,0,7,7,10,0};
	
	// Carregando o ponteiro com a imagem que possui todos os tiles
	BITMAP *bmpTiles=NULL;
	bmpTiles = create_bitmap(224,64);
	bmpTiles = load_bitmap("tiles.bmp",NULL);
	
	// Array que guarda as texturas separadas
	BITMAP *bmpTexturas[11];
	
	// Definindo o tamanho 32x32 para cada tile
	for (i=0;i<11;i++)
	{
		bmpTexturas[i] = create_bitmap(xTile,yTile);
	}

	// A rotina abaixo separa cada tile e armazena no array.
	blit(bmpTiles,bmpTexturas[0],64,0,0,0,xTile,yTile);  // Grama normal
 	blit(bmpTiles,bmpTexturas[1],32,0,0,0,xTile,yTile);  // Grama com areia
	blit(bmpTiles,bmpTexturas[2],0,0,0,0,xTile,yTile);   // Areia
	blit(bmpTiles,bmpTexturas[3],96,0,0,0,xTile,yTile);  // Agua funda	
	blit(bmpTiles,bmpTexturas[4],128,0,0,0,xTile,yTile); // Agua funda com agua clara
	blit(bmpTiles,bmpTexturas[5],160,0,0,0,xTile,yTile); // Agua clara
	blit(bmpTiles,bmpTexturas[6],192,0,0,0,xTile,yTile); // Agua clara com areia
	blit(bmpTiles,bmpTexturas[7],0,32,0,0,xTile,yTile);	 // Pedras
	blit(bmpTiles,bmpTexturas[8],32,32,0,0,xTile,yTile); // Parede
	blit(bmpTiles,bmpTexturas[9],64,32,0,0,xTile,yTile); // Piso
	blit(bmpTiles,bmpTexturas[10],96,32,0,0,xTile,yTile);// Flores

	//Monta o Cenário
	for (x=0;x<15;x++)
	{
		for (y=0;y<20;y++)
		{
			draw_sprite(screen,bmpTexturas[Mapa[x][y]],y*32,x*32);
		}
	}

	//Escreve mercado na parede.
	textout_ex(screen, font, "MERCADO", 540, 135, makecol(255,255,0), -1);

	readkey();

	//Desaloca as Texturas
	for (i=0;i<11;i++)
	{
		destroy_bitmap(bmpTexturas[i]);
	}
	destroy_bitmap(bmpTiles);
	
	allegro_exit();

	return 0;
}

END_OF_MAIN();
