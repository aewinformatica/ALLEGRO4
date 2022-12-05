#include "emd.h"

//no de modelo     texturas que usa
// 0 - mono           0, 1 y 2
// 1 - mona           3, 4 y 5
// 2 - malo           6
// 3 - disparo mono   7
// 4 - disparo malo   8
// 5 - muerte         9 y 10
// 6 - esfera         11, 12 y 13
// 7 - escenario      14 -
// 8 - armamento      24     


void normalizar_plano(int side)
{
	float magnitude = (float)sqrt( frustum[side][0] * frustum[side][0] + 
								   frustum[side][1] * frustum[side][1] + 
								   frustum[side][2] * frustum[side][2] );

	frustum[side][0] /= magnitude;
	frustum[side][1] /= magnitude;
	frustum[side][2] /= magnitude;
	frustum[side][3] /= magnitude; 
}


void extraer_frustum(){
    GLfloat matriz_model[16];
    GLfloat matriz_proj[16];
    GLfloat matriz_r[16];    
    
    glGetFloatv(GL_MODELVIEW_MATRIX,matriz_model);
    glGetFloatv(GL_PROJECTION_MATRIX,matriz_proj);    

    // en x
    
    matriz_r[0] = matriz_model[0]* matriz_proj[0] + matriz_model[1]* matriz_proj[4] + matriz_model[2]* matriz_proj[8] + matriz_model[3]* matriz_proj[12];
    matriz_r[1] = matriz_model[0]* matriz_proj[1] + matriz_model[1]* matriz_proj[5] + matriz_model[2]* matriz_proj[9] + matriz_model[3]* matriz_proj[13];   
    matriz_r[2] = matriz_model[0]* matriz_proj[2] + matriz_model[1]* matriz_proj[6] + matriz_model[2]* matriz_proj[10] + matriz_model[3]* matriz_proj[14];      
    matriz_r[3] = matriz_model[0]* matriz_proj[3] + matriz_model[1]* matriz_proj[7] + matriz_model[2]* matriz_proj[11] + matriz_model[3]* matriz_proj[15];   
    
    // en y
        
    matriz_r[4]  = matriz_model[4]* matriz_proj[0] + matriz_model[5]* matriz_proj[4] + matriz_model[6]* matriz_proj[8] + matriz_model[7]* matriz_proj[12];
    matriz_r[5]  = matriz_model[4]* matriz_proj[1] + matriz_model[5]* matriz_proj[5] + matriz_model[6]* matriz_proj[9] + matriz_model[7]* matriz_proj[13];
    matriz_r[6]  = matriz_model[4]* matriz_proj[2] + matriz_model[5]* matriz_proj[6] + matriz_model[6]* matriz_proj[10] + matriz_model[7]* matriz_proj[14];
    matriz_r[7]  = matriz_model[4]* matriz_proj[3] + matriz_model[5]* matriz_proj[7] + matriz_model[6]* matriz_proj[11] + matriz_model[7]* matriz_proj[15];
    
    // en z
        
    matriz_r[8]  = matriz_model[8]* matriz_proj[0] + matriz_model[9]* matriz_proj[4] + matriz_model[10]* matriz_proj[8] + matriz_model[11]* matriz_proj[12];
    matriz_r[9]  = matriz_model[8]* matriz_proj[1] + matriz_model[9]* matriz_proj[5] + matriz_model[10]* matriz_proj[9] + matriz_model[11]* matriz_proj[13];
    matriz_r[10] = matriz_model[8]* matriz_proj[2] + matriz_model[9]* matriz_proj[6] + matriz_model[10]* matriz_proj[10] + matriz_model[11]* matriz_proj[14];
    matriz_r[11] = matriz_model[8]* matriz_proj[3] + matriz_model[9]* matriz_proj[7] + matriz_model[10]* matriz_proj[11] + matriz_model[11]* matriz_proj[15];
    
    // en d
        
    matriz_r[12]  = matriz_model[12]* matriz_proj[0] + matriz_model[13]* matriz_proj[4] + matriz_model[14]* matriz_proj[8] + matriz_model[15]* matriz_proj[12];
    matriz_r[13]  = matriz_model[12]* matriz_proj[1] + matriz_model[13]* matriz_proj[5] + matriz_model[14]* matriz_proj[9] + matriz_model[15]* matriz_proj[13];
    matriz_r[14]  = matriz_model[12]* matriz_proj[2] + matriz_model[13]* matriz_proj[6] + matriz_model[14]* matriz_proj[10] + matriz_model[15]* matriz_proj[14];
    matriz_r[15]  = matriz_model[12]* matriz_proj[3] + matriz_model[13]* matriz_proj[7] + matriz_model[14]* matriz_proj[11] + matriz_model[15]* matriz_proj[15];

    // ahora a sacar los lados del frustum
    
    //derecho
    
    frustum[0][0]= matriz_r[3] - matriz_r[0];
    frustum[0][1]= matriz_r[7] - matriz_r[4];    
    frustum[0][2]= matriz_r[11] - matriz_r[8];    
    frustum[0][3]= matriz_r[15] - matriz_r[12];       
    
    //izquierdo
    
    frustum[1][0]= matriz_r[3] + matriz_r[0];
    frustum[1][1]= matriz_r[7] + matriz_r[4];    
    frustum[1][2]= matriz_r[11] + matriz_r[8];    
    frustum[1][3]= matriz_r[15] + matriz_r[12];       
    
    //abajo
    
    frustum[2][0]= matriz_r[3] + matriz_r[1];
    frustum[2][1]= matriz_r[7] + matriz_r[5];    
    frustum[2][2]= matriz_r[11] + matriz_r[9];    
    frustum[2][3]= matriz_r[15] + matriz_r[13];           
    
    //arriba
    
    frustum[3][0]= matriz_r[3] - matriz_r[1];
    frustum[3][1]= matriz_r[7] - matriz_r[5];    
    frustum[3][2]= matriz_r[11] - matriz_r[9];    
    frustum[3][3]= matriz_r[15] - matriz_r[13];           
    
    //atras
    
    frustum[4][0]= matriz_r[3] - matriz_r[2];
    frustum[4][1]= matriz_r[7] - matriz_r[6];    
    frustum[4][2]= matriz_r[11] - matriz_r[10];    
    frustum[4][3]= matriz_r[15] - matriz_r[14];       
    
    //adelante
    
    frustum[5][0]= matriz_r[3] + matriz_r[2];
    frustum[5][1]= matriz_r[7] + matriz_r[6];    
    frustum[5][2]= matriz_r[11] + matriz_r[10];    
    frustum[5][3]= matriz_r[15] + matriz_r[14]; 
    
    //normalizar planos
    
    normalizar_plano(0); //derecha
    normalizar_plano(1); //izquierda   
    normalizar_plano(2); //abajo   
    normalizar_plano(3); //arriba   
    normalizar_plano(4); //atras   
    normalizar_plano(5); //adelante   
    
   }
   
int punto_en_frustum(float x, float y, float z) {
   int i;

   	for(i = 0; i < 6; i++ )
	{
		// Calculate the plane equation and check if the point is behind a side of the frustum
		if(frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3] <= 0)
		{
			// The point was behind a side, so it ISN'T in the frustum
			return 0;
		}
	}

	// The point was inside of the frustum (In front of ALL the sides of the frustum)
	return 1;


   }   
   
int esfera_en_frustum(float x, float y, float z, float radio) {
   int i;

   	for(i = 0; i < 6; i++ )
	{
		// Calculate the plane equation and check if the point is behind a side of the frustum
		if(frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3] <= -radio)
		{
			// The point was behind a side, so it ISN'T in the frustum
			return 0;
		}
	}

	// The point was inside of the frustum (In front of ALL the sides of the frustum)
	return 1;


   }   



void desplegar_modelo ( float rotx, float roty, float rotz, float posx, float posy, float posz, OBJETO *obj, int no_de_modelo ){

 
// GLfloat gen_s[] = {0.25, 0.0, 0.0, 0.5};
// GLfloat gen_t[] = {0.0, 0.25, 0.0, 0.5}; 
 int conteo;
 int no_de_caras_transparentes=0;
 GLint textura_actual=0;
 int inicio_de_textura=0;
 //GLint reflejo_actual=0;

 GLfloat v1x;
 GLfloat v1y;
 GLfloat v1z;

 GLfloat v2x;
 GLfloat v2y;
 GLfloat v2z;

 GLfloat v3x;
 GLfloat v3y;
 GLfloat v3z;

 GLfloat n1x;
 GLfloat n1y;
 GLfloat n1z;

 GLfloat n2x;
 GLfloat n2y;
 GLfloat n2z;

 GLfloat n3x;
 GLfloat n3y;
 GLfloat n3z;

 GLfloat u1;
 GLfloat v1;

 GLfloat u2;
 GLfloat v2;

 GLfloat u3;
 GLfloat v3;
 
 //mono
 if (no_de_modelo==0) inicio_de_textura=0;
 //mono
 if (no_de_modelo==2) inicio_de_textura=6; 
 //disparo mono
 if (no_de_modelo==3) inicio_de_textura=7;
 //disparo malo
 if (no_de_modelo==4) inicio_de_textura=8; 
 //esfera
 if (no_de_modelo==6 && hora_del_dia==0) inicio_de_textura=11;
 if (no_de_modelo==6 && hora_del_dia==1) inicio_de_textura=12;
 if (no_de_modelo==6 && hora_del_dia==2) inicio_de_textura=13;
 //escenario
 if (no_de_modelo==7) inicio_de_textura=14; 
 //caja de armamento
 if (no_de_modelo==8) inicio_de_textura=24; 
 


 allegro_gl_begin();

    textura_actual=0;     
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    
    glBindTexture(GL_TEXTURE_2D, texturas[ inicio_de_textura ] );
    glEnable(GL_TEXTURE_2D);
    if (no_de_modelo==6) glDisable( GL_LIGHTING );
      

    glLoadIdentity();    
       
    gluLookAt(camera.posx, camera.posy, camera.posz, camera.targetx, camera.targety, camera.targetz,0,0,camera.rotacion);
	glLightfv(GL_LIGHT0, GL_POSITION,posicion_de_luz);

    glTranslatef( posx ,posy, posz);
    glRotatef (rotz, 0, 0, 1);            
    glRotatef (roty, 0, 1, 0);    
    glRotatef (rotx, 1, 0, 0);    
    
    /*if ( no_de_modelo==6 || no_de_modelo==7 || 
         esfera_en_frustum( posx, posy, posz, 1.0 )){*/
         
    extraer_frustum();

    for ( conteo=0; conteo<obj->no_de_caras; conteo++ ){

     if( obj->poligonos[conteo].tipo==0 || obj->poligonos[conteo].tipo==1 ){
    

       if ( obj->poligonos[conteo].tex!=textura_actual){

          textura_actual=obj->poligonos[conteo].tex;
          glBindTexture(GL_TEXTURE_2D, texturas[ inicio_de_textura+textura_actual ] );
          }
          
          

        if ( obj->poligonos[conteo].doble_cara==1)
           glDisable(GL_CULL_FACE);
        else
           glEnable(GL_CULL_FACE);


        //vertices

        v1x=obj->cuerpo[ obj->poligonos[conteo].l1 ].x;
        v1y=obj->cuerpo[ obj->poligonos[conteo].l1 ].y;
        v1z=obj->cuerpo[ obj->poligonos[conteo].l1 ].z;

        v2x=obj->cuerpo[ obj->poligonos[conteo].l2 ].x;
        v2y=obj->cuerpo[ obj->poligonos[conteo].l2 ].y;
        v2z=obj->cuerpo[ obj->poligonos[conteo].l2 ].z;

        v3x=obj->cuerpo[ obj->poligonos[conteo].l3 ].x;
        v3y=obj->cuerpo[ obj->poligonos[conteo].l3 ].y;
        v3z=obj->cuerpo[ obj->poligonos[conteo].l3 ].z;


        //normales trasladados

        n1x=obj->luz_t[ obj->poligonos[conteo].l1 ].x;
        n1y=obj->luz_t[ obj->poligonos[conteo].l1 ].y;        
        n1z=obj->luz_t[ obj->poligonos[conteo].l1 ].z;        
        
        n2x=obj->luz_t[ obj->poligonos[conteo].l2 ].x;
        n2y=obj->luz_t[ obj->poligonos[conteo].l2 ].y;        
        n2z=obj->luz_t[ obj->poligonos[conteo].l2 ].z;        
        
        n3x=obj->luz_t[ obj->poligonos[conteo].l3 ].x;
        n3y=obj->luz_t[ obj->poligonos[conteo].l3 ].y;        
        n3z=obj->luz_t[ obj->poligonos[conteo].l3 ].z;        
        
        
        

        //coordenadas de texturas


        u1=obj->poligonos[ conteo ].tl1.u;
        v1=obj->poligonos[ conteo ].tl1.v;

        u2=obj->poligonos[ conteo ].tl2.u;
        v2=obj->poligonos[ conteo ].tl2.v;

        u3=obj->poligonos[ conteo ].tl3.u;
        v3=obj->poligonos[ conteo ].tl3.v;

        //dibujamos el poligono
        if ( no_de_modelo==6 || no_de_modelo==7 ||
             (punto_en_frustum(v1x,v1y,v1z)==1 ||
              punto_en_frustum(v2x,v2y,v2z)==1 ||            
              punto_en_frustum(v3x,v3y,v3z)==1) ){

        glBegin(GL_TRIANGLES);
        
        glNormal3f( n1x, n1y, n1z);
        glTexCoord2f(u1,v1);
        glVertex3d(v1x,v1y,v1z);

        glNormal3f( n2x, n2y, n2z);
        glTexCoord2f(u2,v2);
        glVertex3d(v2x,v2y,v2z);

        glNormal3f( n3x, n3y, n3z);
        glTexCoord2f(u3,v3);
        glVertex3d(v3x,v3y,v3z);          
       
        glEnd();
        }
        
        no_triangulos++;        

        }



     if( obj->poligonos[conteo].tipo==2 || obj->poligonos[conteo].tipo==3 ){
     
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].rotacion.x=rotx;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].rotacion.y=roty;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].rotacion.z=rotz;
        
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].posicion.x=posx;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].posicion.y=posy;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].posicion.z=posz;       

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].id=obj->poligonos[conteo].id;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].capa=obj->poligonos[conteo].capa;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].tex=obj->poligonos[conteo].tex;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].mtex=obj->poligonos[conteo].mtex;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l1.x=obj->cuerpo[obj->poligonos[conteo].l1].x;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l1.y=obj->cuerpo[obj->poligonos[conteo].l1].y;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l1.z=obj->cuerpo[obj->poligonos[conteo].l1].z;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l2.x=obj->cuerpo[obj->poligonos[conteo].l2].x;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l2.y=obj->cuerpo[obj->poligonos[conteo].l2].y;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l2.z=obj->cuerpo[obj->poligonos[conteo].l2].z;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l3.x=obj->cuerpo[obj->poligonos[conteo].l3].x;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l3.y=obj->cuerpo[obj->poligonos[conteo].l3].y;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].l3.z=obj->cuerpo[obj->poligonos[conteo].l3].z;


        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_1.x=obj->luz_t[ obj->poligonos[conteo].l1 ].x;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_1.y=obj->luz_t[ obj->poligonos[conteo].l1 ].y;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_1.z=obj->luz_t[ obj->poligonos[conteo].l1 ].z;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_2.x=obj->luz_t[ obj->poligonos[conteo].l2 ].x;;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_2.y=obj->luz_t[ obj->poligonos[conteo].l2 ].y;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_2.z=obj->luz_t[ obj->poligonos[conteo].l2 ].z;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_3.x=obj->luz_t[ obj->poligonos[conteo].l3 ].x;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_3.y=obj->luz_t[ obj->poligonos[conteo].l3 ].y;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].luz_3.z=obj->luz_t[ obj->poligonos[conteo].l3 ].z;


        poligonos_con_mascara[ no_de_poligonos_con_mascara ].tl1.u=obj->poligonos[conteo].tl1.u;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].tl1.v=obj->poligonos[conteo].tl1.v;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].tl2.u=obj->poligonos[conteo].tl2.u;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].tl2.v=obj->poligonos[conteo].tl2.v;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].tl3.u=obj->poligonos[conteo].tl3.u;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].tl3.v=obj->poligonos[conteo].tl3.v;


        poligonos_con_mascara[ no_de_poligonos_con_mascara ].mt1.u=obj->poligonos[conteo].mt1.u;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].mt1.v=obj->poligonos[conteo].mt1.v;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].mt2.u=obj->poligonos[conteo].mt2.u;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].mt2.v=obj->poligonos[conteo].mt2.v;

        poligonos_con_mascara[ no_de_poligonos_con_mascara ].mt3.u=obj->poligonos[conteo].mt3.u;
        poligonos_con_mascara[ no_de_poligonos_con_mascara ].mt3.v=obj->poligonos[conteo].mt3.v;


        no_de_poligonos_con_mascara++;
        }
        
        
        

        
     if( obj->poligonos[conteo].tipo==4 || obj->poligonos[conteo].tipo==5 ){
        obj->caras_transparentes[no_de_caras_transparentes]=conteo;
        no_de_caras_transparentes++;     
        }

     }
     
//caras transparentes


    textura_actual=0;
    
    glBindTexture(GL_TEXTURE_2D, texturas[ inicio_de_textura ] );
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH);    
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );    
    glEnable(GL_BLEND);    
    glDepthMask(GL_FALSE);             
    glColor3f(1.00,1.0,1.0);



    for ( conteo=0; conteo<no_de_caras_transparentes; conteo++ ){




       if ( obj->poligonos[ obj->caras_transparentes[conteo] ].tex!=textura_actual){

          textura_actual=obj->poligonos[ obj->caras_transparentes[conteo] ].tex;
          glBindTexture(GL_TEXTURE_2D, texturas[ inicio_de_textura+textura_actual] );
          }

        if ( obj->poligonos[ obj->caras_transparentes[conteo] ].doble_cara==1)
           glDisable(GL_CULL_FACE);
        else
           glEnable(GL_CULL_FACE);


        //vertices

        v1x=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l1 ].x;
        v1y=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l1 ].y;
        v1z=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l1 ].z;

        v2x=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l2 ].x;
        v2y=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l2 ].y;
        v2z=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l2 ].z;

        v3x=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l3 ].x;
        v3y=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l3 ].y;
        v3z=obj->cuerpo[ obj->poligonos[ obj->caras_transparentes[conteo] ].l3 ].z;


        //normales trasladados

        n1x=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l1 ].x;
        n1y=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l1 ].y;        
        n1z=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l1 ].z;        
        
        n2x=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l2 ].x;
        n2y=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l2 ].y;        
        n2z=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l2 ].z;        
        
        n3x=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l3 ].x;
        n3y=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l3 ].y;        
        n3z=obj->luz_t[ obj->poligonos[ obj->caras_transparentes[conteo] ].l3 ].z;        
        
        

        //coordenadas de texturas


        u1=obj->poligonos[ obj->caras_transparentes[conteo] ].tl1.u;
        v1=obj->poligonos[ obj->caras_transparentes[conteo] ].tl1.v;

        u2=obj->poligonos[ obj->caras_transparentes[conteo] ].tl2.u;
        v2=obj->poligonos[ obj->caras_transparentes[conteo] ].tl2.v;

        u3=obj->poligonos[ obj->caras_transparentes[conteo] ].tl3.u;
        v3=obj->poligonos[ obj->caras_transparentes[conteo] ].tl3.v;

        //dibujamos el poligono

        glBegin(GL_TRIANGLES);
        glNormal3f( n1x, n1y, n1z);
        glTexCoord2f(u1,v1);
        glVertex3d(v1x,v1y,v1z);

        glNormal3f( n2x, n2y, n2z);
        glTexCoord2f(u2,v2);
        glVertex3d(v2x,v2y,v2z);

        glNormal3f( n3x, n3y, n3z);
        glTexCoord2f(u3,v3);
        glVertex3d(v3x,v3y,v3z);
        
        glEnd();
        no_triangulos++;                

        }           
        
 glDisable(GL_BLEND);        
 glEnable(GL_LIGHTING); 
 glDepthMask(GL_TRUE);           
     
 allegro_gl_end();
 


}







void desplegar_poligonos_con_mascara(){

 int conteo;
 unsigned long promedio_z=0;
 int temporal;

 GLint textura_actual=0;
// GLint reflejo_actual=0;

 GLfloat v1x;
 GLfloat v1y;
 GLfloat v1z;

 GLfloat v2x;
 GLfloat v2y;
 GLfloat v2z;

 GLfloat v3x;
 GLfloat v3y;
 GLfloat v3z;

 GLfloat n1x;
 GLfloat n1y;
 GLfloat n1z;

 GLfloat n2x;
 GLfloat n2y;
 GLfloat n2z;

 GLfloat n3x;
 GLfloat n3y;
 GLfloat n3z;

 GLfloat u1;
 GLfloat v1;

 GLfloat u2;
 GLfloat v2;

 GLfloat u3;
 GLfloat v3;

 MATRIX_f rotacion;
 MATRIX_f traslado;
 MATRIX_f matriz_camara;
 MATRIX_f mult;
 MATRIX_f mult2;

                             

 get_camera_matrix_f(&matriz_camara,
               camera.posx, camera.posy, camera.posz,  
               camera.targetx-camera.posx, camera.targety-camera.posy, camera.targetz-camera.posz,        /* ¨hacia donde mira la camara? */
               0,0,1,         /* ¨hacia donde es "arriba"? */
               35,            /* campo de vision */
               1);            /* razon de aspecto */


 
 
 // borramos la tabla de ordenamiento
 for ( conteo=0; conteo<50000; conteo++ ){
    to[conteo]=-1;
    } 
    


 for ( conteo=0; conteo<no_de_poligonos_con_mascara; conteo++ ){
 
 

 
 
  get_translation_matrix_f( &traslado,
                                 poligonos_con_mascara[conteo].posicion.x,
                                 poligonos_con_mascara[conteo].posicion.y,
                                 poligonos_con_mascara[conteo].posicion.z );


                     
  get_rotation_matrix_f( &rotacion,
                           (( poligonos_con_mascara[conteo].rotacion.x*255)/360)*-1,    
                           (( poligonos_con_mascara[conteo].rotacion.y*255)/360)*-1,                               
                           (( poligonos_con_mascara[conteo].rotacion.z*255)/360)*-1
                           );
 
    //multiplicar las matrices

  matrix_mul_f( &rotacion, &traslado, &mult2 ); 
  matrix_mul_f( &mult2, &matriz_camara, &mult ); 
   

    apply_matrix_f(&mult,
                  poligonos_con_mascara[conteo].l1.x,
                  poligonos_con_mascara[conteo].l1.y,
                  poligonos_con_mascara[conteo].l1.z,
                  &poligonos_con_mascara[conteo].l1_t.x,
                  &poligonos_con_mascara[conteo].l1_t.y,
                  &poligonos_con_mascara[conteo].l1_t.z);
                  
    apply_matrix_f(&mult,
                  poligonos_con_mascara[conteo].l2.x,
                  poligonos_con_mascara[conteo].l2.y,
                  poligonos_con_mascara[conteo].l2.z,
                  &poligonos_con_mascara[conteo].l2_t.x,
                  &poligonos_con_mascara[conteo].l2_t.y,
                  &poligonos_con_mascara[conteo].l2_t.z);
                  
    apply_matrix_f(&mult,
                  poligonos_con_mascara[conteo].l3.x,
                  poligonos_con_mascara[conteo].l3.y,
                  poligonos_con_mascara[conteo].l3.z,
                  &poligonos_con_mascara[conteo].l3_t.x,
                  &poligonos_con_mascara[conteo].l3_t.y,
                  &poligonos_con_mascara[conteo].l3_t.z);


    promedio_z=((poligonos_con_mascara[conteo].l1_t.z+
                 poligonos_con_mascara[conteo].l2_t.z+
                 poligonos_con_mascara[conteo].l3_t.z)/3)*10;
/*                 
    promedio_z=poligonos_con_mascara[conteo].l1_t.z;
    
    if ( poligonos_con_mascara[conteo].l2_t.z>promedio_z)
       promedio_z=poligonos_con_mascara[conteo].l2_t.z;
       
    if ( poligonos_con_mascara[conteo].l3_t.z>promedio_z)
       promedio_z=poligonos_con_mascara[conteo].l3_t.z;
       
    promedio_z*=10;             */
                 


     if ( promedio_z<=49999 ){

        if ( to[promedio_z]==-1 ){

           to[promedio_z]=conteo;
           poligonos_con_mascara[conteo].siguiente_poli=-1;
           }

        else{
           poligonos_con_mascara[conteo].siguiente_poli=to[ promedio_z ];
           to[promedio_z]=conteo;
           }

        }



    }






 allegro_gl_begin();

 glDisable(GL_CULL_FACE);        
 glEnable(GL_BLEND);        
 glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
 
 glColor3f(1.00,1.00,1.00);
 



 textura_actual=0;
     
 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 glBindTexture(GL_TEXTURE_2D, texturas[18] );
 glEnable(GL_TEXTURE_2D);
 
 
 
 
 


 for ( conteo=49999; conteo>-1; conteo-- ){

    if ( to[conteo]!=-1){

       temporal=to[conteo];

       while(temporal!=-1){


         /*****************/


    
      

       glLoadIdentity();
       gluLookAt(camera.posx, camera.posy, camera.posz, camera.targetx, camera.targety, camera.targetz,0,0, camera.rotacion);
       glLightfv(GL_LIGHT0, GL_POSITION,posicion_de_luz);	// Position The Light       
      
       

       // Translate and rotate the object
       glTranslatef( poligonos_con_mascara[temporal].posicion.x , 
                     poligonos_con_mascara[temporal].posicion.y ,
                     poligonos_con_mascara[temporal].posicion.z );

       glRotatef ( poligonos_con_mascara[temporal].rotacion.z, 0, 0, 1);
       glRotatef ( poligonos_con_mascara[temporal].rotacion.y, 0, 1, 0);       
       glRotatef ( poligonos_con_mascara[temporal].rotacion.x, 1, 0, 0);       
        


           //vertices

           v1x=poligonos_con_mascara[temporal].l1.x;
           v1y=poligonos_con_mascara[temporal].l1.y;
           v1z=poligonos_con_mascara[temporal].l1.z;

           v2x=poligonos_con_mascara[temporal].l2.x;
           v2y=poligonos_con_mascara[temporal].l2.y;
           v2z=poligonos_con_mascara[temporal].l2.z;

           v3x=poligonos_con_mascara[temporal].l3.x;
           v3y=poligonos_con_mascara[temporal].l3.y;
           v3z=poligonos_con_mascara[temporal].l3.z;




           //normales trasladados

           n1x=poligonos_con_mascara[temporal].luz_1.x;
           n1y=poligonos_con_mascara[temporal].luz_1.y;
           n1z=poligonos_con_mascara[temporal].luz_1.z;

           n2x=poligonos_con_mascara[temporal].luz_2.x;
           n2y=poligonos_con_mascara[temporal].luz_2.y;
           n2z=poligonos_con_mascara[temporal].luz_2.z;

           n3x=poligonos_con_mascara[temporal].luz_3.x;
           n3y=poligonos_con_mascara[temporal].luz_3.y;
           n3z=poligonos_con_mascara[temporal].luz_3.z;



           //coordenadas de texturas


           u1=poligonos_con_mascara[temporal].tl1.u;
           v1=poligonos_con_mascara[temporal].tl1.v;

           u2=poligonos_con_mascara[temporal].tl2.u;
           v2=poligonos_con_mascara[temporal].tl2.v;

           u3=poligonos_con_mascara[temporal].tl3.u;
           v3=poligonos_con_mascara[temporal].tl3.v;



           //dibujamos el poligono

           glBegin(GL_TRIANGLES);
           glNormal3f( n1x, n1y, n1z);
           glTexCoord2f(u1,v1);
           glVertex3d(v1x,v1y,v1z);

           glNormal3f( n2x, n2y, n2z);
           glTexCoord2f(u2,v2);
           glVertex3d(v2x,v2y,v2z);

           glNormal3f( n3x, n3y, n3z);
           glTexCoord2f(u3,v3);
           glVertex3d(v3x,v3y,v3z);           

           glEnd();
           
           no_triangulos++;                      

         /****************/

        temporal=poligonos_con_mascara[temporal].siguiente_poli;
          }

      }
   }



     
        
 glDisable(GL_BLEND);        
 glEnable(GL_LIGHTING); 
 glEnable(GL_CULL_FACE); 
     
     
 allegro_gl_end();


}


void dibujar_disparos( DISPAROS *particulas, int no_de_modelo ){

 int conteo;
 GLfloat v1x;
 GLfloat v1y;
 GLfloat v1z;

 GLfloat v2x;
 GLfloat v2y;
 GLfloat v2z; 
 
 GLfloat v3x;
 GLfloat v3y;
 GLfloat v3z;

 GLfloat v4x;
 GLfloat v4y;
 GLfloat v4z;    

 GLfloat matriz[16];
 
 VERTICE vectorx;
 VERTICE vectory;


 allegro_gl_begin();

 glDisable(GL_CULL_FACE);        
 glEnable(GL_BLEND);        
 glDisable(GL_LIGHTING);         
 glDepthMask(GL_FALSE);
 glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );
      

 glLoadIdentity();
 gluLookAt(camera.posx, camera.posy, camera.posz, camera.targetx, camera.targety, camera.targetz,0,0, camera.rotacion);
 
 glGetFloatv(GL_MODELVIEW_MATRIX, matriz);
 
 vectorx.x=matriz[0];
 vectorx.y=matriz[4]; 
 vectorx.z=matriz[8]; 
 
 vectory.x=matriz[1];
 vectory.y=matriz[5]; 
 vectory.z=matriz[9];  
       

   //vertices
   
 for ( conteo=0; conteo<12; conteo++ ){
 
   //bolitas
   if ( particulas[conteo].tipo==0 && particulas[conteo].disponible==1 ){
  
      v1x=particulas[conteo].x+((-vectorx.x - vectory.x) * 1.0);
      v1y=particulas[conteo].y+((-vectorx.y - vectory.y) * 1.0);
      v1z=1+((-vectorx.z - vectory.z) * 1.0);

      v2x=particulas[conteo].x+((vectorx.x - vectory.x) * 1.0);
      v2y=particulas[conteo].y+((vectorx.y - vectory.y) * 1.0);
      v2z=1+((vectorx.z - vectory.z) * 1.0);

      v3x=particulas[conteo].x+((vectorx.x + vectory.x) * 1.0);
      v3y=particulas[conteo].y+((vectorx.y + vectory.y) * 1.0);
      v3z=1+((vectorx.z + vectory.z) * 1.0);
   
      v4x=particulas[conteo].x+((-vectorx.x + vectory.x) * 1.0);
      v4y=particulas[conteo].y+((-vectorx.y + vectory.y) * 1.0);
      v4z=1+((-vectorx.z + vectory.z) * 1.0);
     
     //disparo mono
     if (no_de_modelo==0)        
        glBindTexture( GL_TEXTURE_2D, texturas[7] );
     //disparo malo        
     if (no_de_modelo==1)        
        glBindTexture( GL_TEXTURE_2D, texturas[8] );
        

   //dibujamos el poligono
     
     glColor3f(1.0, 1.0, 1.0);

     glBegin(GL_QUADS);

        glTexCoord2f(0,0);
        glVertex3d(v1x,v1y,v1z);

        glTexCoord2f(1,0);
        glVertex3d(v2x,v2y,v2z);

        glTexCoord2f(1,1);
        glVertex3d(v3x,v3y,v3z);           

        glTexCoord2f(0,1);
        glVertex3d(v4x,v4y,v4z);           
     

      glEnd();
           
    no_triangulos++;                           
    
    }
    
 //proyectil   
 if ( particulas[conteo].tipo==1 && particulas[conteo].disponible==1 ) {
    transformar_objeto_sin_esqueleto(proyectil); 
    desplegar_modelo( 0,
                      0,
                      (particulas[conteo].ry*-1)*57.2957,
                      particulas[conteo].x,
                      particulas[conteo].y,
                      1,
                      proyectil,
                      5 );
    }   
    
 }

 glDepthFunc(GL_LESS);        
 glDisable(GL_BLEND);   
 glDepthMask(GL_TRUE);     
 glEnable(GL_LIGHTING); 
 glEnable(GL_CULL_FACE); 
 
 allegro_gl_end();

}



void dibujar_mira(){

 GLfloat v1x;
 GLfloat v1y;
 GLfloat v1z;

 GLfloat v2x;
 GLfloat v2y;
 GLfloat v2z; 
 
 GLfloat v3x;
 GLfloat v3y;
 GLfloat v3z;

 GLfloat v4x;
 GLfloat v4y;
 GLfloat v4z;    

 GLfloat matriz[16];
 
 VERTICE vectorx;
 VERTICE vectory;


 allegro_gl_begin();

 glDisable(GL_CULL_FACE);        
 glEnable(GL_BLEND);        
 glDisable(GL_LIGHTING);         
 glDepthFunc(GL_ALWAYS);
 glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );
      

 glLoadIdentity();
 gluLookAt(camera.posx, camera.posy, camera.posz, camera.targetx, camera.targety, camera.targetz,0,0, camera.rotacion);
 
 glGetFloatv(GL_MODELVIEW_MATRIX, matriz);
 
 vectorx.x=matriz[0];
 vectorx.y=matriz[4]; 
 vectorx.z=matriz[8]; 
 
 vectory.x=matriz[1];
 vectory.y=matriz[5]; 
 vectory.z=matriz[9];  
       

   //vertices
   

 v1x=(pos_mono.x+sin(pos_mono.ry)*20)+((-vectorx.x - vectory.x) * 0.5);
 v1y=(pos_mono.y+cos(pos_mono.ry)*20)+((-vectorx.y - vectory.y) * 0.5);
 v1z=1.5+((-vectorx.z - vectory.z) * 0.5);

 v2x=(pos_mono.x+sin(pos_mono.ry)*20)+((vectorx.x - vectory.x) * 0.5);
 v2y=(pos_mono.y+cos(pos_mono.ry)*20)+((vectorx.y - vectory.y) * 0.5);
 v2z=1.5+((vectorx.z - vectory.z) * 0.5);

 v3x=(pos_mono.x+sin(pos_mono.ry)*20)+((vectorx.x + vectory.x) * 0.5);
 v3y=(pos_mono.y+cos(pos_mono.ry)*20)+((vectorx.y + vectory.y) * 0.5);
 v3z=1.5+((vectorx.z + vectory.z) * 0.5);
   
 v4x=(pos_mono.x+sin(pos_mono.ry)*20)+((-vectorx.x + vectory.x) * 0.5);
 v4y=(pos_mono.y+cos(pos_mono.ry)*20)+((-vectorx.y + vectory.y) * 0.5);
 v4z=1.5+((-vectorx.z + vectory.z) * 0.5);
         
 glBindTexture( GL_TEXTURE_2D, texturas[23] );

 //dibujamos el poligono
     
 glColor3f(1.00, 1.00, 1.00);

 glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3d(v1x,v1y,v1z);

    glTexCoord2f(1,0);
    glVertex3d(v2x,v2y,v2z);
    
    glTexCoord2f(1,1);
    glVertex3d(v3x,v3y,v3z);           

    glTexCoord2f(0,1);
    glVertex3d(v4x,v4y,v4z);           
     

  glEnd();
   

 glDepthFunc(GL_LESS);        
 glDisable(GL_BLEND);   
 glEnable(GL_LIGHTING); 
 glEnable(GL_CULL_FACE); 
 
 allegro_gl_end();

}



void destello( float fx, float fy, float transp ){

 GLfloat v1x;
 GLfloat v1y;
 GLfloat v1z;

 GLfloat v2x;
 GLfloat v2y;
 GLfloat v2z; 
 
 GLfloat v3x;
 GLfloat v3y;
 GLfloat v3z;

 GLfloat v4x;
 GLfloat v4y;
 GLfloat v4z;
  
 
 GLfloat r1x;
 GLfloat r1y;
 GLfloat r1z;

 GLfloat r2x;
 GLfloat r2y;
 GLfloat r2z; 
 
 GLfloat r3x;
 GLfloat r3y;
 GLfloat r3z;

 GLfloat r4x;
 GLfloat r4y;
 GLfloat r4z; 

 
 

 GLfloat matriz[16];
 
 VERTICE vectorx;
 VERTICE vectory;


 allegro_gl_begin();

 glDisable(GL_CULL_FACE);        
 glEnable(GL_BLEND);        
 glDisable(GL_LIGHTING);         
 glDepthFunc(GL_ALWAYS);
 glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );
      

 glLoadIdentity();
 gluLookAt(camera.posx, camera.posy, camera.posz, camera.targetx, camera.targety, camera.targetz,0,0, camera.rotacion);
 
 glGetFloatv(GL_MODELVIEW_MATRIX, matriz);
 
 vectorx.x=matriz[0];
 vectorx.y=matriz[4]; 
 vectorx.z=matriz[8]; 
 
 vectory.x=matriz[1];
 vectory.y=matriz[5]; 
 vectory.z=matriz[9];  
       

   //vertices
   

 v1x=fx+((-vectorx.x - vectory.x) * (4*transp));
 v1y=fy+((-vectorx.y - vectory.y) * (4*transp));
 v1z=0.5+((-vectorx.z - vectory.z) * (4*transp));

 v2x=fx+((vectorx.x - vectory.x) * (4*transp));
 v2y=fy+((vectorx.y - vectory.y) * (4*transp));
 v2z=0.5+((vectorx.z - vectory.z) * (4*transp));

 v3x=fx+((vectorx.x + vectory.x) * (4*transp));
 v3y=fy+((vectorx.y + vectory.y) * (4*transp));
 v3z=0.5+((vectorx.z + vectory.z) * (4*transp));
   
 v4x=fx+((-vectorx.x + vectory.x) * (4*transp));
 v4y=fy+((-vectorx.y + vectory.y) * (4*transp));
 v4z=0.5+((-vectorx.z + vectory.z) * (4*transp));
 
 r1x=fx-(10-(10*transp));
 r1y=fy-(10-(10*transp));
 r1z=0.5;
 
 r2x=fx+(10-(10*transp));
 r2y=fy-(10-(10*transp));
 r2z=0.5;
 
 r3x=fx+(10-(10*transp));
 r3y=fy+(10-(10*transp));
 r3z=0.5;
 
 r4x=fx-(10-(10*transp));
 r4y=fy+(10-(10*transp));
 r4z=0.5;
 

         
 glBindTexture( GL_TEXTURE_2D, texturas[9] );

 //dibujamos el poligono
     
 glColor3f(transp, transp, transp);

 glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3d(v1x,v1y,v1z);

    glTexCoord2f(1,0);
    glVertex3d(v2x,v2y,v2z);
    
    glTexCoord2f(1,1);
    glVertex3d(v3x,v3y,v3z);           

    glTexCoord2f(0,1);
    glVertex3d(v4x,v4y,v4z);                

  glEnd();
  
 glBindTexture( GL_TEXTURE_2D, texturas[10] );  
  
 glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3d(r1x,r1y,r1z);

    glTexCoord2f(1,0);
    glVertex3d(r2x,r2y,r2z);
    
    glTexCoord2f(1,1);
    glVertex3d(r3x,r3y,r3z);           

    glTexCoord2f(0,1);
    glVertex3d(r4x,r4y,r4z);           
    

  glEnd();
  
  
   

 glDepthFunc(GL_LESS);        
 glDisable(GL_BLEND);   
 glEnable(GL_LIGHTING); 
 glEnable(GL_CULL_FACE); 
 
 allegro_gl_end();

}


void dibujar_titulo(){

 allegro_gl_begin();

 glDisable(GL_CULL_FACE);        
 glEnable(GL_BLEND);        
 glDisable(GL_LIGHTING);         
 glDepthFunc(GL_ALWAYS);
 glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );      
 
 //textos
 glLoadIdentity();  
 glTranslatef( 0,0,0 ); 
 
 allegro_gl_printf(demofont,-10,-5,-80.0,makecol(255*traslado_en_z,255*traslado_en_z,255*traslado_en_z), "Press Enter" );
 allegro_gl_printf(demofont,-30,-30,-80.0,makecol(128,128,255), "by Miguel Angel Pineda Nieto 2002" ); 
 allegro_gl_printf(demofont,-40.00,32.00,-80.0,makecol(255,255,255), "P1 %12d                  Max %12d", marcador, marcador_maximo  );

 glLoadIdentity();  
 glTranslatef( 0,1,(traslado_en_z*10) ); 
 glBindTexture( GL_TEXTURE_2D, texturas_titulo[0] );     
 glColor3f(1.0-traslado_en_z, 1.0-traslado_en_z, 1.0-traslado_en_z); 

 glBegin(GL_QUADS);

   //titulo
      
    glTexCoord2f(0,0);
    glVertex3d(-4,-2,-10);

    glTexCoord2f(1,0);
    glVertex3d(4,-2,-10);
    
    glTexCoord2f(1,1);
    glVertex3d(4,2,-10);           

    glTexCoord2f(0,1);
    glVertex3d(-4,2,-10); 

  glEnd();
  
   

 glDepthFunc(GL_LESS);        
 glDisable(GL_BLEND);   
 glEnable(GL_LIGHTING); 
 glEnable(GL_CULL_FACE); 
 
 allegro_gl_end();

}



void dibujar_controles(){

 allegro_gl_begin();

 glDisable(GL_CULL_FACE);        
 glEnable(GL_BLEND);        
 glDisable(GL_LIGHTING);         
 glDepthFunc(GL_ALWAYS);
 glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );      
 
 //textos
 glLoadIdentity();  
 glTranslatef( 0,3,0 );   
  
 allegro_gl_printf(demofont,-30,-30,-80.0,makecol(128,128,255), "Press Enter to continue" ); 
 allegro_gl_printf(demofont,0,-20,-100.0,makecol( (traslado_en_z*255), (traslado_en_z*255), (traslado_en_z*255) ), "Item Box" );

 glBindTexture( GL_TEXTURE_2D, texturas_titulo[0] );     
 glColor3f(1.0,1.0,1.0);  
 
 glLoadIdentity();  
 glTranslatef( 0,3,-10 );     

 glBegin(GL_QUADS);

   //titulo
      
    glTexCoord2f(0,0);
    glVertex3d(-2,-1,0);

    glTexCoord2f(1,0);
    glVertex3d(2,-1,0);
    
    glTexCoord2f(1,1);
    glVertex3d(2,1,0);           

    glTexCoord2f(0,1);
    glVertex3d(-2,1,0); 

  glEnd();
  
 glLoadIdentity();  
 glTranslatef( -2,1,0 );   
 glBindTexture( GL_TEXTURE_2D, texturas_titulo[1] );
  
 glBegin(GL_QUADS);

   //teclado
      
    glTexCoord2f(0,0);
    glVertex3d(-2,-1.5,-10);

    glTexCoord2f(1,0);
    glVertex3d(2,-1.5,-10);
    
    glTexCoord2f(1,1);
    glVertex3d(2,1.5,-10);           

    glTexCoord2f(0,1);
    glVertex3d(-2,1.5,-10); 

  glEnd();
  
 glLoadIdentity();  
 glTranslatef( 2,1,0 );   
 glBindTexture( GL_TEXTURE_2D, texturas_titulo[2] );
  
 glBegin(GL_QUADS);

   //mouse
      
    glTexCoord2f(0,0);
    glVertex3d(-2,-1.5,-10);

    glTexCoord2f(1,0);
    glVertex3d(2,-1.5,-10);
    
    glTexCoord2f(1,1);
    glVertex3d(2,1.5,-10);           

    glTexCoord2f(0,1);
    glVertex3d(-2,1.5,-10); 

  glEnd();

 glDepthFunc(GL_LESS);        
 glDisable(GL_BLEND);   
 glEnable(GL_LIGHTING); 
 glEnable(GL_CULL_FACE); 
 
 camera.posx=-1.5;
 camera.posy=6; 
 camera.posz=1.5; 
 
 camera.targetx=-1.5;
 camera.targety=0;
 camera.targetz=1.5;  
 
 allegro_gl_end(); 

}

void dibujar_sombra( float posx, float posy ){

   allegro_gl_begin();
   glLoadIdentity();     
   gluLookAt(camera.posx, camera.posy, camera.posz, camera.targetx, camera.targety, camera.targetz,0,0, camera.rotacion);
   glTranslatef(posx,posy,0.02);
   
   glEnable(GL_POLYGON_OFFSET_FILL);   
   glPolygonOffset(1.0,-20.0);
   
   glDisable(GL_CULL_FACE);
   glBindTexture(GL_TEXTURE_2D, texturas_titulo[3]);
     
   glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
   glEnable(GL_BLEND);          
   glDisable(GL_LIGHTING);        
               
   glBegin(GL_QUADS);             
     
     glColor3f(1,1,1);     
     glTexCoord2f(0.0,0.0);
     glVertex3f(0.5,0.5,0);
       
     glTexCoord2f(0.0,1.0);
     glVertex3f(-0.5,0.5,0);         
       
     glTexCoord2f(1.0,1.0);
     glVertex3f(-0.5,-0.5,0);  
       
     glTexCoord2f(1.0,0.0);
     glVertex3f(0.5,-0.5,0);  
     
   glEnd();   

   glDisable(GL_BLEND);                    
   glEnable(GL_CULL_FACE);    
   glDisable(GL_POLYGON_OFFSET_FILL);   
   glEnable(GL_LIGHTING);                  
   allegro_gl_end();   

 }







