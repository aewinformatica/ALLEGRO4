#include "emd.h"

void actualizar_disparo( DISPAROS *disparo ){
   int conteo;
   int conteo2;
   if (retardo_disparo>0) retardo_disparo--;

     for ( conteo=0; conteo<12; conteo++ ){
     
        if (disparo[conteo].disponible==1){
           disparo[conteo].x=disparo[conteo].x+sin( disparo[conteo].ry )*2;
           disparo[conteo].y=disparo[conteo].y+cos( disparo[conteo].ry )*2;
           
           if ( fabs(disparo[conteo].x)>28 || fabs(disparo[conteo].y)>28 ){
              disparo[conteo].disponible=0;
              FSOUND_PlaySound(4,golpe_en_pared);
              FSOUND_SetVolume( 4,50 );
              }
           
           for ( conteo2=0; conteo2<no_de_enemigos; conteo2++ ){
              if ( pos_malo[conteo2].vida>0 && 
                   fabs( disparo[conteo].x-pos_malo[conteo2].x )<1.0 &&
                   fabs( disparo[conteo].y-pos_malo[conteo2].y )<1.0  ){
                 FSOUND_PlaySound(2,dolor_malo);
                 pos_malo[conteo2].vida-=50;
                 if (pos_malo[conteo2].vida<1) desaparecer_destello_malo[conteo2]=1;
                 
                 marcador+=100;
                 disparo[conteo].disponible=0;
                 break;
                 }
              }
           
           
           }
        }   
   }   
   
  
    
void actualizar_disparo_enemigo( DISPAROS *disparo ){

   int conteo;

     for ( conteo=0; conteo<12; conteo++ ){
     
        if (disparo[conteo].disponible==1){
           disparo[conteo].x=disparo[conteo].x+sin( disparo[conteo].ry )*velocidad_de_disparo;
           disparo[conteo].y=disparo[conteo].y+cos( disparo[conteo].ry )*velocidad_de_disparo;
           
           if ( fabs(disparo[conteo].x)>28 || fabs(disparo[conteo].y)>28 ){
              disparo[conteo].disponible=0;
              }
           
           if ( !fin_del_juego &&
                fabs( disparo[conteo].x-pos_mono.x )<1.0 &&
                fabs( disparo[conteo].y-pos_mono.y )<1.0  ){
              if (!pantalla_de_final_de_round && !pantalla_de_inicio_de_round ){
                 FSOUND_PlaySound(2,dolor_mono);
                 pos_mono.vida-=15;                 
                 if (pos_mono.vida<1 && desaparecer_destello_mono<0.05){
                    no_de_vidas--;
                    desaparecer_destello_mono=1.0;
                    }
                 }
              disparo[conteo].disponible=0;
              }
           
           }
        }   
   }      
   
   
   
   
void actualizar_posiciones_de_malos(){
   int conteo, conteo2;
   int suma_de_muertos=0;
   float avance=0.25;
   double angulo_mono;
   double y;
   double x;

     for ( conteo=0; conteo<no_de_enemigos; conteo++ ){
     
        if (retardo_malo[conteo]>0) retardo_malo[conteo]--;
     
        if ( pos_malo[conteo].vida>0 ){        
           
           if ( pos_malo[conteo].periodo_decision==0){                              
              //momento de desicion
              pos_malo[conteo].periodo_decision=rand()%60+60;
              pos_malo[conteo].decision=rand()%100;
              if (pos_malo[conteo].decision<dificultad_decision)
                 pos_malo[conteo].decision=0;
              if (pos_malo[conteo].decision>=dificultad_decision)
                 pos_malo[conteo].decision=2;                 
              }                             
            
           //si el malo decide seguir al personaje
           if ( pos_malo[conteo].decision==2  && 
                fabs(pos_malo[conteo].x-pos_mono.x)>3 && 
                fabs(pos_malo[conteo].y-pos_mono.y)>3 &&                 
                fabs(pos_malo[conteo].x)<29 && 
                fabs(pos_malo[conteo].y)<29 ){
                
                 //malo disparando
                 for ( conteo2=0; conteo2<12; conteo2++ ){
                    if (malo_disparo[conteo][conteo2].disponible==0  && retardo_malo[conteo]==0 ){
                       FSOUND_PlaySound(3,pistola_malo);
                       retardo_malo[conteo]=retardo_disparos_enemigos;
                       malo_disparo[conteo][conteo2].disponible=1;
                       malo_disparo[conteo][conteo2].x=pos_malo[conteo].x;
                       malo_disparo[conteo][conteo2].y=pos_malo[conteo].y;
                       malo_disparo[conteo][conteo2].ry=pos_malo[conteo].ry;
                     }
                   }
                
              y=pos_malo[conteo].y-pos_mono.y;
              y*=-1;
              x=pos_malo[conteo].x-pos_mono.x;                     
              angulo_mono=atan2( y,x );           
              pos_malo[conteo].ry=angulo_mono-1.57;
              avance=0.1;
              }              

        
           pos_malo[conteo].x=pos_malo[conteo].x+sin( pos_malo[conteo].ry )*avance;
           pos_malo[conteo].y=pos_malo[conteo].y+cos( pos_malo[conteo].ry )*avance;
           
           if ( fabs(pos_malo[conteo].x)>28 || fabs(pos_malo[conteo].y)>28 ||
                (fabs(pos_malo[conteo].x-pos_mono.x)<1 && fabs(pos_malo[conteo].y-pos_mono.y)<1) )
              pos_malo[conteo].ry+=3.25; 
           
           if ( pos_malo[conteo].periodo_decision>0 ) pos_malo[conteo].periodo_decision--;
           }
        else suma_de_muertos++;

        }
        
     if (suma_de_muertos==no_de_enemigos && pos_mono.vida>0) finalizar_round();   
   
   }   
   

