#include <iostream>
#include <string>
#include <fstream>
#include <allegro.h>
#include "sistema.h"
#include "personagem.h"
#include "ImagemManager.h"
#include "bloco.h"
#include "fase.h"


using namespace std;

volatile long spdCounter = 0;
void IncrementCounter();

int main()
{
    
    
    
    Sistema Jogo;
    Personagem MegaMan;
    ImageManager imgMgr; 
    Fase  fase[1];
    fase[0].Carrega_Mapa("mapa.txt");
    
    LOCK_VARIABLE( spdCounter );
    LOCK_FUNCTION( IncrementCounter );
    install_int_ex( IncrementCounter, BPS_TO_TIMER( 90 ) );


    while ( !Jogo.Fim() ){
          
        while ( spdCounter > 0 ){
            
            if ( key[KEY_ESC] || key[KEY_F4] ) { Jogo.Sair(); }
            if ( key[KEY_F5] ) { Jogo.Mudar_Tela(); }
              
             spdCounter --;
              }
              


      for(int i = 0; i <6;i++)
      {        
      if(i >6)
               i= 0;
               
      imgMgr.Draw( "Player", 0, 0, 90*i, 480, 0, 0 );
      }
       Jogo.Desenhar( imgMgr.Buffer );
       
       textprintf_right_ex(imgMgr.Buffer, font,SCREEN_W, 0, makecol(255,0,0), -1,"SpdCounter.: %d",spdCounter);
    }

    return 0;
}
END_OF_MAIN();
void IncrementCounter()
{
    spdCounter++;
}

