#include "emd.h"

void color_de_alba(){

    color_de_ambiente[0]=1.50;
    color_de_ambiente[1]=1.50;
    color_de_ambiente[2]=1.50;
    color_de_ambiente[3]=1.0;

    color_de_luz[0]=0.7;
    color_de_luz[1]=0.7;
    color_de_luz[2]=0.8;
    color_de_luz[3]=1.0;     

    posicion_de_luz[0]=-0.75;
    posicion_de_luz[1]=0.0;
    posicion_de_luz[2]=0.33;
    posicion_de_luz[3]=0.0;  

    allegro_gl_begin();
   	   glLightfv(GL_LIGHT0, GL_AMBIENT, color_de_ambiente);
  	   glLightfv(GL_LIGHT0, GL_DIFFUSE, color_de_luz);
	   glLightfv(GL_LIGHT0, GL_POSITION, posicion_de_luz);	
    allegro_gl_end();    
    
   }
   
void color_de_atardecer(){

    color_de_ambiente[0]=1.5;
    color_de_ambiente[1]=1.5;
    color_de_ambiente[2]=1.5;
    color_de_ambiente[3]=1.0;

    color_de_luz[0]=1.3;
    color_de_luz[1]=0.8;     
    color_de_luz[2]=0.0;     
    color_de_luz[3]=1.0;     

    posicion_de_luz[0]=0.70;
    posicion_de_luz[1]=-0.75;
    posicion_de_luz[2]=0.33;
    posicion_de_luz[3]=0.0;  
    
    allegro_gl_begin();
   	   glLightfv(GL_LIGHT0, GL_AMBIENT, color_de_ambiente);
  	   glLightfv(GL_LIGHT0, GL_DIFFUSE, color_de_luz);
	   glLightfv(GL_LIGHT0, GL_POSITION, posicion_de_luz);	
    allegro_gl_end();        
   }

void color_de_noche(){

    color_de_ambiente[0]=0.5;
    color_de_ambiente[1]=0.5;
    color_de_ambiente[2]=0.5;
    color_de_ambiente[3]=1.0;

    color_de_luz[0]=1.0;
    color_de_luz[1]=1.0;     
    color_de_luz[2]=1.0;     
    color_de_luz[3]=1.0;     

    posicion_de_luz[0]=0.00;
    posicion_de_luz[1]=0.75;
    posicion_de_luz[2]=0.33;
    posicion_de_luz[3]=0.0;  

    allegro_gl_begin();
   	   glLightfv(GL_LIGHT0, GL_AMBIENT, color_de_ambiente);
  	   glLightfv(GL_LIGHT0, GL_DIFFUSE, color_de_luz);
	   glLightfv(GL_LIGHT0, GL_POSITION, posicion_de_luz);	
    allegro_gl_end();        

   }
   


void iniciar_posiciones(){

   int conteo;

   for (conteo=0; conteo<10; conteo++){
      pos_malo[conteo].vida=100;
      
      pos_malo[conteo].x=(float)(rand()%56)-28;
      while( fabs(pos_malo[conteo].x-pos_mono.x)<5 ){
         pos_malo[conteo].x=(float)(rand()%56)-28;
         }         
         
      pos_malo[conteo].y=(float)(rand()%56)-28;         
      while( fabs(pos_malo[conteo].y-pos_mono.y)<5 ){
         pos_malo[conteo].y=(float)(rand()%56)-28;
         }
      
      pos_malo[conteo].ry=(float)(rand()%628)/100;
      
      pos_malo[conteo].decision=0;      
      pos_malo[conteo].decision_disparo=0;            
      }

  }


  
void iniciar_aditamento(){

   int suerte=rand()%1000;
   
   if ( suerte<1000 ){
      aditamento.disponible=1;
      
      //tipo de aditamento
      suerte=rand()%75;
      //vida
      if (suerte<25){
         aditamento.tipo=0;
      }
      //ametralladora
      if (suerte>=25 && suerte<50){
         aditamento.tipo=1;
      }
      //puntos
      if (suerte>=50 && suerte<=75){
         aditamento.tipo=2;
      }
      
      //localizacion
      suerte=rand()%100;
      //esquina superior izquierda
      if (suerte<25){
         aditamento.x=-25;
         aditamento.y=25;         
      }
      //esquina superior derecha
      if (suerte>=25 && suerte<50){
         aditamento.x=-25;
         aditamento.y=25;         
      }
      //esquina inferior izquierda
      if (suerte>=50 && suerte<75){
         aditamento.x=-25;
         aditamento.y=-25;
      }
      //esquina inferior derecha
      if (suerte>=75 && suerte<=100){
         aditamento.x=25;
         aditamento.y=-25;         
      }
      
      
      
      
   }
}


  
  
  
void iniciar_round(){

   hora_del_dia++;
   if (hora_del_dia>2) hora_del_dia=0;

   if (hora_del_dia==0) color_de_alba();   
   if (hora_del_dia==1) color_de_atardecer();      
   if (hora_del_dia==2) color_de_noche();      
   
   nivel_del_juego++;
   if (nivel_del_juego==2) 
      no_de_enemigos++;
      
   if (nivel_del_juego==4) 
      no_de_enemigos++;
      
   if (nivel_del_juego==6) 
      no_de_enemigos++;
      
   if (nivel_del_juego==8) 
      no_de_enemigos++;
      
   if (nivel_del_juego==10) 
      no_de_enemigos++;      
      
   if (nivel_del_juego==12) 
      no_de_enemigos++;      
      
   if (nivel_del_juego==14) 
      no_de_enemigos++;      
      
      

   if ( no_de_cancion==0 )
      mp3 = FSOUND_Stream_OpenFile("datos/mdf01.mp3",FSOUND_LOOP_NORMAL,0);
     
   if ( no_de_cancion==1 )
      mp3 = FSOUND_Stream_OpenFile("datos/mdf01.mp3",FSOUND_LOOP_NORMAL,0);

      
   velocidad_de_disparo+=0.05;
   if (velocidad_de_disparo>0.8)
      velocidad_de_disparo=0.8;
     
   //animacion    
   esqueleto->caminar=0;
   esqueleto->lerp_de=8;
   esqueleto->lerp_a=9;   
   esqueleto2->lerp_de=8;
   esqueleto2->lerp_a=9; 
        
   camera.posx=0;
   camera.posy=0;
   camera.posz=10;
   volumen_de_musica=255;   
   FSOUND_Stream_Play( 0, mp3 );
   FSOUND_SetVolume( 0,255 );      

/*   
   dificultad_decision-=5;
   if (dificultad_decision<50)
      dificultad_decision=50;*/

   
   retardo_disparos_enemigos-=5;  
   if (retardo_disparos_enemigos<15)
      retardo_disparos_enemigos=15;

   pos_mono.vida=150;         
   detener_malos=1;
   tiempo_para_round=99;
   iniciar_posiciones();
   pantalla_de_inicio_de_round=1;
   contador_para_inicio_de_round=3;
   retardo_disparo_mono=30;
   municiones=-1;
   tipo_de_arma=0;  /* 0 - default
                       1 - ametralladora
                    */                    
                    
   }
   
   
   
void iniciar_vida(){


   esqueleto->caminar=0;   
   camera.posx=0;
   camera.posy=0;
   camera.posz=10;
   pos_mono.vida=150;         
   detener_malos=1;
   tiempo_para_round=99;
   iniciar_posiciones();
   pantalla_de_inicio_de_round=1;
   contador_para_inicio_de_round=3;
   retardo_disparo_mono=30;
   municiones=-1;
   tipo_de_arma=0;  /* 0 - default
                       1 - ametralladora
                    */                    
                    
   }
   
   
void finalizar_round(){

   esqueleto->caminar=0;   
   giro_de_camara=0;
   no_de_cancion++;
   if (no_de_cancion>1) no_de_cancion=0;
   marcador+=(tiempo_para_round*100)+(pos_mono.vida*100);
   if (pos_mono.vida==150) marcador+=10000;
   pantalla_de_final_de_round=1;
   detener_malos=1;
   contador_para_final_de_round=5;
   }
   


void iniciar_variables(){

 int conteo, conteo2;
 
 srand(time(0));
 
 marcador=0;

 no_de_poligonos_con_mascara=0;  


 //texto
 strcpy(linea1,"");
 strcpy(linea2,""); 
 
 //posicion del mono
 pos_mono.x=0.0;
 pos_mono.y=-16.0; 
 pos_mono.ry=0; 
 pos_mono.vida=150;  

 //general
 juego_iniciado=0;
 modelx=0;
 hora_del_dia=-1;
 modely=0;
 modelyy=0;
 frame_actual=1;
 cambiar_frame=0;
 presionar_1=0;
 presionar_2=0;
 retardo_disparo=0;
 no_de_enemigos=3;
 dificultad_decision=70; 
 retardo_disparos_enemigos=60;
 retardo_disparo_mono=30;
 nivel_del_juego=0;
 detener_malos=0;
 tiempo_para_round=99;
 cronometro_para_round=0;
 no_de_vidas=3;
 no_de_cancion=0;
 pantalla_de_final_de_round=0;
 pantalla_de_inicio_de_round=0;
 contador_para_final_de_round=0;
 contador_para_inicio_de_round=0;
 contador_para_mensajes=0;
 aditamento.disponible=0;
 fin_del_juego=0;
 cronometro_fin_del_juego=0;
 velocidad_de_disparo=0.5;


 camera.posx=pos_mono.x;
 camera.posy=pos_mono.y-10.0; 
 camera.posz=1.0; 
 
 camera.targetx=pos_mono.x;
 camera.targety=pos_mono.y; 
 camera.targetz=1.0; 
 camera.rotacion=1.0;   

 angle_speed = 0.5;
 dist_speed = 0.5;

 frames = 0;
 
 desaparecer_destello_mono=0;
 
 for( conteo=0; conteo<no_de_enemigos; conteo++ ){
    desaparecer_destello_malo[conteo]=0;
    for( conteo2=0; conteo2<12; conteo2++ ){
       malo_disparo[conteo][conteo2].disponible=0;
       mono_disparo[conteo2].disponible=0;
       }
    }

}


   



/*
int iniciar_multitextura(){

   //multitextura
   glActiveTextureARB = 0;
   glMultiTexCoord2fARB = 0;
   glClientActiveTextureARB = 0;

   glActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)
   wglGetProcAddress("glActiveTextureARB");

   glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)
   wglGetProcAddress("glMultiTexCoord2fARB");

   glClientActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) 
   wglGetProcAddress("glClientActiveTextureARB");

   if (!glActiveTextureARB || !glMultiTexCoord2fARB ||
       !glClientActiveTextureARB)
        return 0;

   return 1;
}

  */



void encontrar_normal( unsigned long vertice_actual, OBJETO *obj ){

   unsigned long conteo;
 //  unsigned long conteo2;
   unsigned long no_de_ocurrencias=0; //cuantas veces se encuentra el vertice en las caras
   float falsox;   //solo para poner algo en la funcion del normal  (falsox,falsoy,falsoz)
   float falsoy;
   float falsoz;
   float promedio_normales_x;
   float promedio_normales_y;
   float promedio_normales_z;

   //revisar cada cara del modelo para encontrar si el vertice se encuentra alli
   for ( conteo=0; conteo<obj->no_de_caras; conteo++ ){


         if (obj->poligonos[conteo].l1==vertice_actual ||
             obj->poligonos[conteo].l2==vertice_actual ||
             obj->poligonos[conteo].l3==vertice_actual){

             obj->ocurrencia[no_de_ocurrencias]=conteo;
             no_de_ocurrencias++;
             }

      }

   //si se encontraron caras, proseguir
   if (no_de_ocurrencias>0){
   //sacar normales de cada poligono encontrado
      for ( conteo=0; conteo<no_de_ocurrencias; conteo++ ){

         cross_product_f(obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l1 ].x - obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l2 ].x,
                         obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l1 ].y - obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l2 ].y,
                         obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l1 ].z - obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l2 ].z,
                         obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l2 ].x - obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l3 ].x,
                         obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l2 ].y - obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l3 ].y,
                         obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l2 ].z - obj->vertices[ obj->poligonos[ obj->ocurrencia[conteo] ].l3 ].z,
                         &falsox,&falsoy,&falsoz);

         obj->normales_de_ocurrencias[conteo].x=falsox;
         obj->normales_de_ocurrencias[conteo].y=falsoy;
         obj->normales_de_ocurrencias[conteo].z=falsoz;
     }

   //sacar el promedio de las caras
     promedio_normales_x=0;
     promedio_normales_y=0;
     promedio_normales_z=0;

     for( conteo=0; conteo<no_de_ocurrencias; conteo++){
        promedio_normales_x+=obj->normales_de_ocurrencias[conteo].x;
        promedio_normales_y+=obj->normales_de_ocurrencias[conteo].y;
        promedio_normales_z+=obj->normales_de_ocurrencias[conteo].z;
        }

     promedio_normales_x=promedio_normales_x/no_de_ocurrencias;
     promedio_normales_y=promedio_normales_y/no_de_ocurrencias;
     promedio_normales_z=promedio_normales_z/no_de_ocurrencias;
     normalize_vector_f(&promedio_normales_x, &promedio_normales_y, &promedio_normales_z );
     
     
     obj->luz[vertice_actual].x=promedio_normales_x;
     obj->luz[vertice_actual].y=promedio_normales_y;     
     obj->luz[vertice_actual].z=promedio_normales_z;         
     
     }


 }





void destruir_modelo( OBJETO *obj ){


 free(obj->huesocuerpo);
 free (obj->huesocara);


 free(obj->capa);

 free(obj->vertices);
 free(obj->cara);
 free(obj->cuerpo);


 free(obj->caras_transparentes); 

 free(obj->ocurrencia);
 free(obj->normales_de_ocurrencias);

 free(obj->poligonos);
 
 free(obj->luz); 
 free(obj->luz_t);  

 
 free(obj);
 


}













