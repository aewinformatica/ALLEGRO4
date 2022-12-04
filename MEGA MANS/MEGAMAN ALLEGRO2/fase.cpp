#include "bloco.h"
#include "fase.h"
/*#include <fstream>
*/
void Fase::Desenha_Base(BITMAP * Buffer,BITMAP *Desenho_Bloco)
{
     masked_blit(Desenho_Bloco,Buffer,0,0,0,0,SCREEN_W,SCREEN_H);
     
     for(int i=0;i<BLOCO_X;i++)
     {
             for(int j =0;j<BLOCO_Y;j++)
             {
                     masked_blit(Desenho_Bloco,Buffer,bloco[0][i][j].cx,bloco[0][i][j].cy,bloco[0][i][j].x,bloco[0][i][j].y,bloco[0][i][j].w,bloco[0][i][j].h);
                     
                     }
             }
 }
 
void Fase::Carrega_Mapa(string filename)
{
     ifstream infile;
     infile.open(filename.c_str());
     
     string temp;
     
     infile >> temp; //x
     infile >> temp; //160
     infile >> temp; //y
     infile >> temp; //60
     infile >> temp; //bgimage
     infile >> temp; //Fundo

     /*
     Fundo = load_bitmap(temp.c_str(),NULL); 
 */
 }
