#include "emd.h"




OBJETO *leer_modelo( char *archivo ){

 PACKFILE *apuntador_a_archivo;
 OBJETO *obj;
 unsigned char nula;
 char  tipo_de_archivo[5];
 short nulos;
 unsigned short nuloss;
 float nulf;
 int conteo;
 GRUPOS grupos;
 ARCHIVO_TEX ubicacion_texturas; 


  apuntador_a_archivo=pack_fopen(archivo,"r");

  if ( apuntador_a_archivo==NULL ) return NULL;

  obj=malloc( sizeof(OBJETO) );


  pack_fread(&tipo_de_archivo,sizeof(char)*5,apuntador_a_archivo);
  
  //leemos el numero de texturas 
 pack_fread(&nula,sizeof(unsigned char),apuntador_a_archivo);  
 
 //ubicacion
 for (conteo=0; conteo<nula+1; conteo++)
    pack_fread(&ubicacion_texturas,sizeof(ARCHIVO_TEX),apuntador_a_archivo);
  


  //numero de grupos

  pack_fread(&nuloss,sizeof(unsigned short),apuntador_a_archivo);

  //despues los nombres

  for (conteo=0; conteo<nuloss; conteo++){
     pack_fread(&grupos,sizeof(GRUPOS),apuntador_a_archivo);
     }


 //numero de vertices

  pack_fread(&obj->no_de_vertices,sizeof(unsigned short),apuntador_a_archivo);


 //reservamos memoria para los vertices

  obj->huesocuerpo = malloc( sizeof(short)*obj->no_de_vertices );
  if (obj->huesocuerpo==NULL) return NULL;  //si algo salio mal, no seguir adelante
  memset(obj->huesocuerpo, 0, sizeof(short)*obj->no_de_vertices);

  obj->huesocara = malloc( sizeof(short)*obj->no_de_vertices );
  if (obj->huesocara==NULL) return NULL;
  memset(obj->huesocara, 0, sizeof(short)*obj->no_de_vertices);

  obj->capa = malloc( sizeof(unsigned short)*obj->no_de_vertices );
  if (obj->capa ==NULL) return NULL;
  memset(obj->capa, 0, sizeof(unsigned short)*obj->no_de_vertices);

  obj->vertices = malloc( sizeof(VERTICE)*obj->no_de_vertices );
  if (obj->vertices==NULL) return NULL;
  memset(obj->vertices, 0, sizeof(VERTICE)*obj->no_de_vertices);
  
  obj->luz = malloc( sizeof(VERTICE)*obj->no_de_vertices );
  if (obj->vertices==NULL) return NULL;
  memset(obj->vertices, 0, sizeof(VERTICE)*obj->no_de_vertices);
  
  obj->luz_t = malloc( sizeof(VERTICE)*obj->no_de_vertices );
  if (obj->vertices==NULL) return NULL;
  memset(obj->vertices, 0, sizeof(VERTICE)*obj->no_de_vertices);    

  obj->cara = malloc( sizeof(VERTICE)*obj->no_de_vertices );
  if (obj->cara==NULL) return NULL;
  memset(obj->cara, 0, sizeof(VERTICE)*obj->no_de_vertices);

  obj->cuerpo = malloc( sizeof(VERTICE)*obj->no_de_vertices );
  if (obj->cuerpo==NULL) return NULL;
  memset(obj->cuerpo, 0, sizeof(VERTICE)*obj->no_de_vertices);

  obj->ocurrencia = malloc( sizeof(unsigned short)*100 );
  if (obj->ocurrencia ==NULL) return NULL;
  memset(obj->ocurrencia, 0, sizeof(unsigned short)*100);

  obj->normales_de_ocurrencias = malloc( sizeof(VERTICE)*100 );
  if (obj->normales_de_ocurrencias ==NULL) return NULL;
  memset(obj->normales_de_ocurrencias, 0, sizeof(VERTICE)*100);

    

 //datos de cada vertice

 for ( conteo=0; conteo<obj->no_de_vertices; conteo++){

       pack_fread(&nulos,sizeof(short),apuntador_a_archivo);
       obj->huesocara[ conteo ]=nulos;

       pack_fread(&nulos,sizeof(short),apuntador_a_archivo);
       obj->huesocuerpo[ conteo ]=nulos;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->vertices[ conteo ].x=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->vertices[ conteo ].y=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->vertices[ conteo ].z=nulf;

       pack_fread(&nuloss,sizeof(unsigned short),apuntador_a_archivo);
       obj->capa[ conteo ]=nuloss;

       }





 //leemos el numero de caras

 pack_fread(&obj->no_de_caras,sizeof(unsigned short),apuntador_a_archivo);


 //reservar memoria para poligonos

  obj->poligonos = malloc( sizeof(POLIGONO)*obj->no_de_caras );
  if (obj->poligonos ==NULL) return NULL;
  memset(obj->poligonos, 0, sizeof(POLIGONO)*obj->no_de_caras);


  obj->caras_transparentes = malloc( sizeof(unsigned short)*obj->no_de_caras );
  if (obj->caras_transparentes ==NULL) return NULL;
  memset(obj->caras_transparentes, 0, sizeof(unsigned short)*obj->no_de_caras);



 //leemos los datos de las caras

  for ( conteo=0; conteo<obj->no_de_caras; conteo++){


       //datos generales
       pack_fread(&nula,sizeof(unsigned char),apuntador_a_archivo);
       obj->poligonos[ conteo ].id=nula;

       pack_fread(&nula,sizeof(unsigned char),apuntador_a_archivo);
       obj->poligonos[ conteo ].doble_cara=nula;

       pack_fread(&nuloss,sizeof(unsigned short),apuntador_a_archivo);
       obj->poligonos[ conteo ].capa=nuloss;
       
       pack_fread(&nula,sizeof(unsigned char),apuntador_a_archivo);
       obj->poligonos[ conteo ].transparencia=nula;

       pack_fread(&nuloss,sizeof(unsigned short),apuntador_a_archivo);
       obj->poligonos[ conteo ].l1=nuloss;

       pack_fread(&nuloss,sizeof(unsigned short),apuntador_a_archivo);
       obj->poligonos[ conteo ].l2=nuloss;

       pack_fread(&nuloss,sizeof(unsigned short),apuntador_a_archivo);
       obj->poligonos[ conteo ].l3=nuloss;

       pack_fread(&nula,sizeof(unsigned char),apuntador_a_archivo);
       obj->poligonos[ conteo ].tipo=nula;

       pack_fread(&nuloss,sizeof(unsigned short),apuntador_a_archivo);
       obj->poligonos[ conteo ].tex=nuloss;

       pack_fread(&nuloss,sizeof(unsigned short),apuntador_a_archivo);
       obj->poligonos[ conteo ].mtex=nuloss;

       //coordenadas de las caras
       //cara 1
       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].tl1.u=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].tl1.v=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].mt1.u=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].mt1.v=nulf;


       //cara 2
       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].tl2.u=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].tl2.v=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].mt2.u=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].mt2.v=nulf;


       //cara 3
       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].tl3.u=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].tl3.v=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].mt3.u=nulf;

       pack_fread(&nulf,sizeof(float),apuntador_a_archivo);
       obj->poligonos[ conteo ].mt3.v=nulf;

       }
  
 pack_fclose(apuntador_a_archivo);                                                           

 return obj;

}




GLuint leer_textura ( char *archivo, int tipo) {

 GLuint textura;
 unsigned short tamanox;
 unsigned short tamanoy;
 GLubyte *datos_de_imagen;

 PACKFILE *file = pack_fopen(archivo, "r");

 if (file!=NULL){
    pack_fread(&tamanox,sizeof(unsigned short),file);
    pack_fread(&tamanoy,sizeof(unsigned short),file);
    }
 else {
    pack_fclose(file);
    return -1;
    }

 datos_de_imagen=(GLubyte *)malloc((tamanox*tamanoy)*4);


 if (datos_de_imagen!=NULL)
    pack_fread(datos_de_imagen,(tamanox*tamanoy)*4, file);
 else{
    pack_fclose (file);
    return -1;
    }

 pack_fclose (file);

 allegro_gl_begin();

     glGenTextures(1, &textura );
     glBindTexture(GL_TEXTURE_2D, textura );

     //sin mipmap sin alpha
     if (tipo==0){

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, tamanox, tamanoy, 0, GL_RGBA, GL_UNSIGNED_BYTE, datos_de_imagen);
        }

     //mipmap sin alpha
     if (tipo==1){
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB8,tamanox,tamanoy,GL_RGBA,GL_UNSIGNED_BYTE,datos_de_imagen);
     }
     
     //mipmap con alpha
     if (tipo==2){
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8,tamanox,tamanoy,GL_RGBA,GL_UNSIGNED_BYTE,datos_de_imagen);
     }
     
     //mipmap sin alpha repeat
     if (tipo==3){
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB8,tamanox,tamanoy,GL_RGBA,GL_UNSIGNED_BYTE,datos_de_imagen);
     }
     
     

  allegro_gl_end();
  
  free(datos_de_imagen);

  return textura;
}


















