#include "emd.h"

void revisar_aditamento(){
   
   if ( aditamento.disponible==1 &&
        fabs( pos_mono.x-aditamento.x )<=1 && 
        fabs( pos_mono.y-aditamento.y )<=1){
        
        FSOUND_PlaySound( 6,sonido_aditamento );
        contador_para_mensajes=4;
        aditamento.disponible=0;
                
        //aumenta la vida 25 pts
        if (aditamento.tipo==0){
           pos_mono.vida+=25;
           strcpy(linea1,"Health +25");           
           if (pos_mono.vida>150) pos_mono.vida=150;
        }
        
        //ametralladora
        if (aditamento.tipo==1){
           strcpy(linea1,"Machine Gun");
           retardo_disparo_mono=5;
           tipo_de_arma=1;
           municiones=100;
        }
        
        //+1500 pts
        if (aditamento.tipo==2){
           strcpy(linea1,"+1500 pts");
           marcador+=1500;
        }
        
        
           
   }

}

