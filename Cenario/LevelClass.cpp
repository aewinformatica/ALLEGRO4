/*LevelClass.cpp*/
#include <iostream>
#include <fstream>
#include <allegro.h>
#include "LevelClass.h"

using namespace std;

LevelClass::LevelClass(){

this->iTilesX = 20;
this->iTilesY = 15;
this->iQtdeFases =3;

this->ArqFases[0] = "fases/fase1.txt";
this->ArqFases[1] = "fases/fase2.txt";
this->ArqFases[2] = "fases/fase3.txt";   

this->Mapa[iQtdeFases][iTilesX][iTilesY];

//funciona this->CarregaMatriz();
                        
}
void LevelClass::CarregaMatriz(){

   int x = 0;
   int y = 0;
   int iFase = 0;

   char sLinha[ iTilesX ]; // Qtde de caracteres por linha

   //O FOR abaixo é para preencher todas as fases.
   for (iFase=0; iFase<this->iQtdeFases; iFase++)
   {
      // Abre o arquivo de fases para leitura com base no iFase e o Array de fases
      std::ifstream oLendoFase(ArqFases[iFase],ios::in );    

      //Preenche a matriz na horizontal e vertical
      for (y=0; y<iTilesY; y++)
      {   
         /* 
         Atribui na variável sLinha a linha atual do arquivo texto.
         Toda vez que essa rotina for chamada ele pula para a proxima linha e
         joga o valor para sLinha, até finalizar as 15 linhas.
         */
         oLendoFase >> sLinha;

         /* 
            Já com o sLinha preenchido, agora, o FOR abaixo varre todas as 
            posições dessa linha e acrescenta na linha da matriz. Após terminar
            de varrer o sLinha sai do for e espera próxima linha, se houver.
         */
         for (x=0; x<iTilesX; x++)
         {
            
            Mapa[iFase][x][y] = sLinha[x];
         }
      }
      oLendoFase.close();
   }
}
void LevelClass::DesenhaCenario(BITMAP *Cenario, int iFase){
   int x = 0;
   int y = 0;
   
   tile.CarregaTexturas();
   CarregaMatriz();

   
   for (y=0; y < iTilesY; y++){
       
      for (x=0; x < iTilesX; x++){
      
         if  ((char)Mapa[iFase][x][y] == '#') // Parede
           draw_sprite(Cenario,tile.Textura[0], x*tile.w , y * this->tile.h);
         else
         if  ((char)Mapa[iFase][x][y] == '-') // Grama
            draw_sprite(Cenario,tile.Textura[1], x*tile.w, y*tile.h);
         else
         if  ((char)Mapa[iFase][x][y] == '.') // Chão
            draw_sprite(Cenario,tile.Textura[2], x*tile.w, y*tile.h);

      }
   }  
   
}

LevelClass::~LevelClass(){
                           
}
