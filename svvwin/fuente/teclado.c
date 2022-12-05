#include "emd.h"



void leer_teclado(){

   int conteo;
   float adelante=0;
   float posicion_camarax=pos_mono.x+sin( pos_mono.ry+1.57 )*3;
   float posicion_camaray=pos_mono.y+cos( pos_mono.ry+1.57 )*3;
   
   
   
   if (!pantalla_de_final_de_round){
      if (!pantalla_de_inicio_de_round) pos_mono.ry+=(float)mickey_x/100;
      }
   else{ 
      giro_de_camara-=0.02;
      }
   
   if (!detener_malos){


      esqueleto->caminar=0;
      if (key[KEY_W])
         esqueleto->caminar=1;
      if (key[KEY_S])
         esqueleto->caminar=4;         
      if (key[KEY_A])
         esqueleto->caminar=2;
      if (key[KEY_D])
         esqueleto->caminar=3;
         
         
      
   
      if (key[KEY_W] ) adelante=0.25;
      if (key[KEY_S] ) adelante=-0.25;  
     
      pos_mono.x=pos_mono.x+sin( pos_mono.ry )*adelante;
      pos_mono.y=pos_mono.y+cos( pos_mono.ry )*adelante;         
      
   
      if (key[KEY_A])
      {           
         pos_mono.x=pos_mono.x+sin( pos_mono.ry-1.5707 )*0.25;
         pos_mono.y=pos_mono.y+cos( pos_mono.ry-1.5707 )*0.25;      
      } 
   
      if (key[KEY_D])
      {
         pos_mono.x=pos_mono.x+sin( pos_mono.ry+1.5707 )*0.25;
         pos_mono.y=pos_mono.y+cos( pos_mono.ry+1.5707 )*0.25;   
      }

      if (pos_mono.x<-26.5) pos_mono.x=-26.5;
      if (pos_mono.x>26.5) pos_mono.x=26.5;   
      if (pos_mono.y<-26.5) pos_mono.y=-26.5;
      if (pos_mono.y>26.5) pos_mono.y=26.5;   
   }
   
   
   if (!pantalla_de_inicio_de_round){   
   
      if (!pantalla_de_final_de_round){
         camera.posx=pos_mono.x+sin( pos_mono.ry+3.1416 )*5;
         camera.posy=pos_mono.y+cos( pos_mono.ry+3.1416 )*5;

         if ( fabs(camera.posx)>28 || fabs(camera.posy)>28 )
         {
            camera.posx=pos_mono.x+sin( pos_mono.ry+3.1416 )*1.5;
            camera.posy=pos_mono.y+cos( pos_mono.ry+3.1416 )*1.5;   
            }
         }
      else{
         camera.posx=pos_mono.x+sin( giro_de_camara+3.1416 )*2.5;
         camera.posy=pos_mono.y+cos( giro_de_camara+3.1416 )*2.5;

     /*    if ( fabs(camera.posx)>28 || fabs(camera.posy)>28 )
         {
            camera.posx=pos_mono.x+sin( giro_de_camara+3.1416 )*1.5;
            camera.posy=pos_mono.y+cos( giro_de_camara+3.1416 )*1.5;   
            }*/
         }
   
      if (!pantalla_de_final_de_round) camera.posz+=(float)mickey_y/100;
      else camera.posz=0.5;
      
      if (camera.posz<0.8) camera.posz=0.8;
      if (camera.posz>5) camera.posz=5;  
   }
   if (pantalla_de_inicio_de_round &&
         (fabs(camera.posx-posicion_camarax)>1 ||
         fabs(camera.posy-posicion_camaray)>1 ||
         fabs(camera.posz-2)>0.25) ){

      if ( fabs(camera.posx-posicion_camarax )>1){
         if (camera.posx>posicion_camarax ) camera.posx-=0.25;
         if (camera.posx<posicion_camarax ) camera.posx+=0.25;
         }
      if ( fabs(camera.posy-posicion_camaray )>1){
         if (camera.posy>posicion_camaray ) camera.posy-=0.25;
         if (camera.posy<posicion_camaray ) camera.posy+=0.25;         
         }
      if ((camera.posz-1)>0.25){      
         if (camera.posz>1) camera.posz-=0.25;
         }   
         
      }
   
   if (!pantalla_de_final_de_round && !pantalla_de_inicio_de_round){
      camera.targetx=pos_mono.x+sin(pos_mono.ry)*5;
      camera.targety=pos_mono.y+cos(pos_mono.ry)*5;
      }
   if (pantalla_de_final_de_round && !pantalla_de_inicio_de_round ){
      camera.targetx=pos_mono.x+sin(giro_de_camara)*5;
      camera.targety=pos_mono.y+cos(giro_de_camara)*5;
      }
   if (pantalla_de_inicio_de_round ){
      camera.targetx=pos_mono.x;
      camera.targety=pos_mono.y;
      }
      
      
   
   if (mouse_b==1 && detener_malos==0 )
   {
     for ( conteo=0; conteo<12; conteo++ ){
        if (mono_disparo[conteo].disponible==0  && retardo_disparo==0){
        
           if (tipo_de_arma==0) FSOUND_PlaySound(1,pistola);
           if (tipo_de_arma==1) FSOUND_PlaySound(1,ametralladora);
           
           if (municiones>-1 && tipo_de_arma==1) {
              municiones--;
              }
           if (municiones<0 && tipo_de_arma==1) {
              tipo_de_arma=0;
              retardo_disparo_mono=30;
              }
           retardo_disparo=retardo_disparo_mono;
           mono_disparo[conteo].tipo=0;
           mono_disparo[conteo].disponible=1;
           mono_disparo[conteo].x=pos_mono.x;
           mono_disparo[conteo].y=pos_mono.y;
           mono_disparo[conteo].ry=pos_mono.ry;
           }
        }
   } 
         
}


