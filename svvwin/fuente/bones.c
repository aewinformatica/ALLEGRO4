#include "emd.h"



void multiplicar_matriz( OSEO *skelly, int numero ){

  //traslado
   float lerp_x=skelly->hueso[numero].k_traslado[skelly->lerp_de].x+(skelly->hueso[numero].k_traslado[skelly->lerp_a].x-skelly->hueso[numero].k_traslado[skelly->lerp_de].x)*skelly->lerp;
   float lerp_y=skelly->hueso[numero].k_traslado[skelly->lerp_de].y+(skelly->hueso[numero].k_traslado[skelly->lerp_a].y-skelly->hueso[numero].k_traslado[skelly->lerp_de].y)*skelly->lerp;
   float lerp_z=skelly->hueso[numero].k_traslado[skelly->lerp_de].z+(skelly->hueso[numero].k_traslado[skelly->lerp_a].z-skelly->hueso[numero].k_traslado[skelly->lerp_de].z)*skelly->lerp;

 //rotacion
   float lerp_rx=skelly->hueso[numero].k_rotacion[skelly->lerp_de].x+(skelly->hueso[numero].k_rotacion[skelly->lerp_a].x-skelly->hueso[numero].k_rotacion[skelly->lerp_de].x)*skelly->lerp;
   float lerp_ry=skelly->hueso[numero].k_rotacion[skelly->lerp_de].y+(skelly->hueso[numero].k_rotacion[skelly->lerp_a].y-skelly->hueso[numero].k_rotacion[skelly->lerp_de].y)*skelly->lerp;
   float lerp_rz=skelly->hueso[numero].k_rotacion[skelly->lerp_de].z+(skelly->hueso[numero].k_rotacion[skelly->lerp_a].z-skelly->hueso[numero].k_rotacion[skelly->lerp_de].z)*skelly->lerp;

  //escala
   float lerp_sx=skelly->hueso[numero].k_escala[skelly->lerp_de].x+(skelly->hueso[numero].k_escala[skelly->lerp_a].x-skelly->hueso[numero].k_escala[skelly->lerp_de].x)*skelly->lerp;
   float lerp_sy=skelly->hueso[numero].k_escala[skelly->lerp_de].y+(skelly->hueso[numero].k_escala[skelly->lerp_a].y-skelly->hueso[numero].k_escala[skelly->lerp_de].y)*skelly->lerp;
   float lerp_sz=skelly->hueso[numero].k_escala[skelly->lerp_de].z+(skelly->hueso[numero].k_escala[skelly->lerp_a].z-skelly->hueso[numero].k_escala[skelly->lerp_de].z)*skelly->lerp;

   MATRIX_f inv_traslado;
   MATRIX_f rotacion;
   MATRIX_f rotacion2;
   MATRIX_f rotacion_x;
   MATRIX_f rotacion_y;
   MATRIX_f rotacion_z;
   MATRIX_f escala;
   MATRIX_f traslado;
   MATRIX_f traslado2;

   MATRIX_f matriz_multiplicada;
   MATRIX_f matriz_multiplicada2;
   MATRIX_f matriz_multiplicada3;

   // sacamos la matriz inversa de la posicion del hueso
   get_translation_matrix_f(&inv_traslado,skelly->hueso[numero].x*-1,skelly->hueso[numero].y*-1,skelly->hueso[numero].z*-1);
   // lo giramos segun el keyframe
   get_x_rotate_matrix_f(&rotacion_x,lerp_rx);
   get_y_rotate_matrix_f(&rotacion_y,lerp_ry);
   get_z_rotate_matrix_f(&rotacion_z,lerp_rz);
   matrix_mul_f(&rotacion_x,&rotacion_z,&rotacion2);
   matrix_mul_f(&rotacion2,&rotacion_y,&rotacion);
   // lo trasladamos segun el keyframe
   get_translation_matrix_f(&traslado2,lerp_x,lerp_y,lerp_z);
   // lo escalamos segun el keyframe
   get_scaling_matrix_f(&escala,lerp_sx,lerp_sy,lerp_sz);
   // lo devolvemos a su lugar
   get_translation_matrix_f(&traslado,skelly->hueso[numero].x,skelly->hueso[numero].y,skelly->hueso[numero].z);
   // multiplicamos la primera matriz por la segunda
   matrix_mul_f(&inv_traslado,&rotacion,&matriz_multiplicada);
   // despues la segunda y la tercera
   matrix_mul_f(&matriz_multiplicada,&traslado2,&matriz_multiplicada2);
   // tercera y cuarta
   matrix_mul_f(&matriz_multiplicada2,&escala,&matriz_multiplicada3);
   // cuarta y ultima
   matrix_mul_f(&matriz_multiplicada3,&traslado,&skelly->hueso[numero].final);

   //rotacion final para iluminacion
   skelly->hueso[numero].rotacion_final=rotacion;
 }










void ajustar_matrices( OSEO *skelly ){
  int conteo;
  MATRIX_f matriz_multiplicada;

  for ( conteo=0; conteo<skelly->no_de_huesos; conteo++){

     multiplicar_matriz(skelly,conteo);

     if (skelly->hueso[conteo].hueso_anterior>-1){
       matrix_mul_f(&skelly->hueso[conteo].final,&skelly->hueso[skelly->hueso[conteo].hueso_anterior].final,&matriz_multiplicada);
       skelly->hueso[conteo].final=matriz_multiplicada;
       matrix_mul_f(&skelly->hueso[conteo].rotacion_final,&skelly->hueso[skelly->hueso[conteo].hueso_anterior].rotacion_final,&matriz_multiplicada);
       skelly->hueso[conteo].rotacion_final=matriz_multiplicada;
       }
     }


 }






void calcular_esqueleto( OSEO *skelly_cara, OSEO *skelly_cuerpo, OBJETO *obj ){

  int conteo;

  ajustar_matrices(skelly_cara);
  ajustar_matrices(skelly_cuerpo);

  for ( conteo=0; conteo<obj->no_de_vertices; conteo++ ){

       if (obj->huesocara[conteo]!=-1 && obj->huesocara[conteo]<255){
         apply_matrix_f(&skelly_cara->hueso[ obj->huesocara[conteo] ].final,
                         obj->vertices[conteo].x,
                         obj->vertices[conteo].y,
                         obj->vertices[conteo].z,
                         &obj->cara[conteo].x,
                         &obj->cara[conteo].y,
                         &obj->cara[conteo].z);
                         }
       else{
          obj->cara[conteo].x=obj->vertices[conteo].x;
          obj->cara[conteo].y=obj->vertices[conteo].y;
          obj->cara[conteo].z=obj->vertices[conteo].z;
         }


       if (obj->huesocuerpo[conteo]!=-1 && obj->huesocuerpo[conteo]<255){
         apply_matrix_f(&skelly_cuerpo->hueso[ obj->huesocuerpo[conteo] ].final,
                        obj->cara[conteo].x,
                        obj->cara[conteo].y,
                        obj->cara[conteo].z,
                        &obj->cuerpo[conteo].x,
                        &obj->cuerpo[conteo].y,
                        &obj->cuerpo[conteo].z);                        

         apply_matrix_f(&skelly_cuerpo->hueso[ obj->huesocuerpo[conteo] ].rotacion_final,
                        obj->luz[conteo].x,
                        obj->luz[conteo].y,
                        obj->luz[conteo].z,
                        &obj->luz_t[conteo].x,
                        &obj->luz_t[conteo].y,
                        &obj->luz_t[conteo].z);

           }
       else{
          obj->cuerpo[conteo].x=obj->cara[conteo].x;
          obj->cuerpo[conteo].y=obj->cara[conteo].y;
          obj->cuerpo[conteo].z=obj->cara[conteo].z;

          obj->luz_t[conteo].x=obj->luz[conteo].x;
          obj->luz_t[conteo].y=obj->luz[conteo].y;
          obj->luz_t[conteo].z=obj->luz[conteo].z;
          }

    }



  }





void calcular_cuerpo( OSEO *skelly, OBJETO *obj ){

  int conteo;

  ajustar_matrices(skelly);

  for ( conteo=0; conteo<obj->no_de_vertices; conteo++ ){

       if (obj->huesocuerpo[conteo]!=-1 && obj->huesocuerpo[conteo]<255){
         apply_matrix_f(&skelly->hueso[ obj->huesocuerpo[conteo] ].final,
                        obj->vertices[conteo].x,
                        obj->vertices[conteo].y,
                        obj->vertices[conteo].z,
                        &obj->cuerpo[conteo].x,
                        &obj->cuerpo[conteo].y,
                        &obj->cuerpo[conteo].z);
                        

         apply_matrix_f(&skelly->hueso[ obj->huesocuerpo[conteo] ].rotacion_final,
                        obj->luz[conteo].x,
                        obj->luz[conteo].y,
                        obj->luz[conteo].z,
                        &obj->luz_t[conteo].x,
                        &obj->luz_t[conteo].y,
                        &obj->luz_t[conteo].z);
                        }

       else{
          obj->cuerpo[conteo].x=obj->vertices[conteo].x;
          obj->cuerpo[conteo].y=obj->vertices[conteo].y;
          obj->cuerpo[conteo].z=obj->vertices[conteo].z;

          obj->luz_t[conteo].x=obj->luz[conteo].x;
          obj->luz_t[conteo].y=obj->luz[conteo].y;
          obj->luz_t[conteo].z=obj->luz[conteo].z;
          }

    }



  }



void transformar_objeto_sin_esqueleto( OBJETO *obj ){
  int conteo;

  for ( conteo=0; conteo<obj->no_de_vertices; conteo++ ){

    obj->cuerpo[conteo].x=obj->vertices[conteo].x;
    obj->cuerpo[conteo].y=obj->vertices[conteo].y;
    obj->cuerpo[conteo].z=obj->vertices[conteo].z;

    obj->luz_t[conteo].x=obj->luz[conteo].x;
    obj->luz_t[conteo].y=obj->luz[conteo].y;
    obj->luz_t[conteo].z=obj->luz[conteo].z;

    }

 }

  


void animar_modelo( OSEO *skelly ){
              
  int conteo;
  
  skelly->lerp+=1/skelly->retardo;     
     
  if (skelly->curr_act!=skelly->caminar){
     for(conteo=0;conteo<skelly->no_de_huesos;conteo++){
        //traslado
         skelly->hueso[conteo].k_traslado[0].x =skelly->hueso[conteo].k_traslado[skelly->lerp_de].x+(skelly->hueso[conteo].k_traslado[skelly->lerp_a].x-skelly->hueso[conteo].k_traslado[skelly->lerp_de].x)*skelly->lerp;
         skelly->hueso[conteo].k_traslado[0].y =skelly->hueso[conteo].k_traslado[skelly->lerp_de].y+(skelly->hueso[conteo].k_traslado[skelly->lerp_a].y-skelly->hueso[conteo].k_traslado[skelly->lerp_de].y)*skelly->lerp;
         skelly->hueso[conteo].k_traslado[0].z =skelly->hueso[conteo].k_traslado[skelly->lerp_de].z+(skelly->hueso[conteo].k_traslado[skelly->lerp_a].z-skelly->hueso[conteo].k_traslado[skelly->lerp_de].z)*skelly->lerp;

       //rotacion
         skelly->hueso[conteo].k_rotacion[0].x =skelly->hueso[conteo].k_rotacion[skelly->lerp_de].x+(skelly->hueso[conteo].k_rotacion[skelly->lerp_a].x-skelly->hueso[conteo].k_rotacion[skelly->lerp_de].x)*skelly->lerp;
         skelly->hueso[conteo].k_rotacion[0].y =skelly->hueso[conteo].k_rotacion[skelly->lerp_de].y+(skelly->hueso[conteo].k_rotacion[skelly->lerp_a].y-skelly->hueso[conteo].k_rotacion[skelly->lerp_de].y)*skelly->lerp;
         skelly->hueso[conteo].k_rotacion[0].z =skelly->hueso[conteo].k_rotacion[skelly->lerp_de].z+(skelly->hueso[conteo].k_rotacion[skelly->lerp_a].z-skelly->hueso[conteo].k_rotacion[skelly->lerp_de].z)*skelly->lerp;
  
        //escala
         skelly->hueso[conteo].k_escala[0].x =skelly->hueso[conteo].k_escala[skelly->lerp_de].x+(skelly->hueso[conteo].k_escala[skelly->lerp_a].x-skelly->hueso[conteo].k_escala[skelly->lerp_de].x)*skelly->lerp;
         skelly->hueso[conteo].k_escala[0].y =skelly->hueso[conteo].k_escala[skelly->lerp_de].y+(skelly->hueso[conteo].k_escala[skelly->lerp_a].y-skelly->hueso[conteo].k_escala[skelly->lerp_de].y)*skelly->lerp;
         skelly->hueso[conteo].k_escala[0].z =skelly->hueso[conteo].k_escala[skelly->lerp_de].z+(skelly->hueso[conteo].k_escala[skelly->lerp_a].z-skelly->hueso[conteo].k_escala[skelly->lerp_de].z)*skelly->lerp;

         }       
     skelly->lerp=2;
     skelly->lerp_a=0;
     skelly->curr_act=skelly->caminar;
     } 
     
  if (skelly->lerp>1) {
     skelly->lerp=0;                 
     skelly->actual_fr=skelly->lerp_a;
     
     skelly->lerp_de_ant=skelly->lerp_de;
     skelly->lerp_a_ant=skelly->lerp_a;
     
     //idle
     if (skelly->caminar==0 && skelly->lerp_a>7 && skelly->lerp_a<13){
        if (skelly->lerp_de<8 || skelly->lerp_de>12) {skelly->lerp_de=8;skelly->lerp_a=9;}
        skelly->retardo=15;
        skelly->lerp_de++;
        skelly->lerp_a++;
        if (skelly->lerp_de>12) skelly->lerp_de=8;
        if (skelly->lerp_a>12) skelly->lerp_a=8;          
        }              
        
     if (skelly->caminar==0 && (skelly->actual_fr<8 ||skelly->actual_fr>12 )) {
        skelly->retardo=6;
        skelly->lerp_de=skelly->lerp_a;
        skelly->lerp_a=8;
        }        
        
     //caminar adelante
     if (skelly->caminar==1 && skelly->lerp_a>0 && skelly->lerp_a<7){
        if (skelly->lerp_de<1 || skelly->lerp_de>6) {skelly->lerp_de=1;skelly->lerp_a=2;}
        skelly->retardo=5;
        skelly->lerp_de++;
        skelly->lerp_a++;
        if (skelly->lerp_de>6) skelly->lerp_de=1;
        if (skelly->lerp_a>6) skelly->lerp_a=1;
        }              
        
     if (skelly->caminar==1 && (skelly->actual_fr<1 ||skelly->actual_fr>6 )) {
        skelly->retardo=5;
        skelly->lerp_de=skelly->lerp_a;
        skelly->lerp_a=1;
        }        
        
     //caminar izquierda
     if (skelly->caminar==2 && skelly->lerp_a>12 && skelly->lerp_a<19){
        if (skelly->lerp_de<13 || skelly->lerp_de>18) {skelly->lerp_de=13;skelly->lerp_a=14;}
        skelly->retardo=5;
        skelly->lerp_de++;
        skelly->lerp_a++;
        if (skelly->lerp_de>18) skelly->lerp_de=13;
        if (skelly->lerp_a>18) skelly->lerp_a=13;
        }              
        
     if (skelly->caminar==2 && (skelly->actual_fr<13 ||skelly->actual_fr>18 )) {
        skelly->retardo=5;
        skelly->lerp_de=skelly->lerp_a;
        skelly->lerp_a=13;
        }        
        
     //caminar derecha
     if (skelly->caminar==3 && skelly->lerp_a>18 && skelly->lerp_a<25){
        if (skelly->lerp_de<19 || skelly->lerp_de>24) {skelly->lerp_de=19;skelly->lerp_a=20;}
        skelly->retardo=5;
        skelly->lerp_de++;
        skelly->lerp_a++;
        if (skelly->lerp_de>24) skelly->lerp_de=19;
        if (skelly->lerp_a>24) skelly->lerp_a=19;
        }              
        
     if (skelly->caminar==3 && (skelly->actual_fr<19 ||skelly->actual_fr>24 )) {
        skelly->retardo=5;
        skelly->lerp_de=skelly->lerp_a;
        skelly->lerp_a=19;
        }                
        
     //caminar hacia atras
     if (skelly->caminar==4 && skelly->lerp_a>24 && skelly->lerp_a<31){
        if (skelly->lerp_de<25 || skelly->lerp_de>30) {skelly->lerp_de=25;skelly->lerp_a=26;}
        skelly->retardo=5;
        skelly->lerp_de++;
        skelly->lerp_a++;
        if (skelly->lerp_de>30) skelly->lerp_de=25;
        if (skelly->lerp_a>30) skelly->lerp_a=25;
        }              
        
     if (skelly->caminar==4 && (skelly->actual_fr<25 ||skelly->actual_fr>30 )) {
        skelly->retardo=5;
        skelly->lerp_de=skelly->lerp_a;
        skelly->lerp_a=25;
        }                
        
     }
     
}





OSEO *leer_esqueleto( char *archivo_esqueleto ){

 int conteo,conteo2;
 PACKFILE *archivo;
 OSEO *skelly;
 char nulch[5];

 archivo=pack_fopen(archivo_esqueleto,"r");

 if ( archivo==NULL) return NULL;


 skelly=malloc( sizeof(OSEO) );
 if (skelly==NULL) return NULL;

 memset ( skelly, 0, sizeof(OSEO) );

 pack_fread(&nulch,sizeof(char)*5,archivo);

 pack_fread(&skelly->no_de_huesos,sizeof(short),archivo);

 for (conteo=0;conteo<skelly->no_de_huesos;conteo++){
    pack_fread(&skelly->hueso[conteo].x,sizeof(float),archivo);
    pack_fread(&skelly->hueso[conteo].y,sizeof(float),archivo);
    pack_fread(&skelly->hueso[conteo].z,sizeof(float),archivo);
    pack_fread(&skelly->hueso[conteo].hueso_anterior,sizeof(short),archivo);
    }

 for (conteo=0;conteo<skelly->no_de_huesos;conteo++){
     for (conteo2=0;conteo2<256;conteo2++){
       pack_fread(&skelly->hueso[conteo].k_traslado[conteo2],sizeof(KEYFRAME),archivo);
       pack_fread(&skelly->hueso[conteo].k_escala[conteo2],sizeof(KEYFRAME),archivo);
       pack_fread(&skelly->hueso[conteo].k_rotacion[conteo2],sizeof(KEYFRAME),archivo);
       }
      }
  pack_fclose(archivo);


  return skelly;
 }


