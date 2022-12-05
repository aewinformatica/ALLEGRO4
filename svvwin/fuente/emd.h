#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <allegro.h>
#include <alleggl.h> 
#include <gl/glu.h> 
//#include <gl/glext.h>
#include <fmod/fmod.h>

/*
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;*/

FONT *demofont;
DATAFILE *dat;
DATAFILE *pal;   

GLuint texturas[25];
GLuint texturas_titulo[5];

int no_triangulos;


char linea1[256];
char linea2[256];  

int marcador; 
int marcador_maximo; 

FSOUND_STREAM *mp3;
FSOUND_SAMPLE *pistola;
FSOUND_SAMPLE *ametralladora;
FSOUND_SAMPLE *pistola_malo;
FSOUND_SAMPLE *dolor_mono;
FSOUND_SAMPLE *dolor_malo;
FSOUND_SAMPLE *golpe_en_pared;
FSOUND_SAMPLE *sonido_aditamento;
FSOUND_SAMPLE *aparicion_de_letras;


GLfloat frustum[6][4];

GLfloat color_de_ambiente[4];
GLfloat color_de_luz[4];     
GLfloat posicion_de_luz[4];  

GLfloat color_de_luz2[4];    
GLfloat posicion_de_luz2[4]; 


//general
GLfloat modelx;
GLfloat modely;
GLfloat modelyy;
GLfloat desaparecer_destello_mono;
GLfloat desaparecer_destello_malo[10];
GLfloat giro_de_camara;
GLint frame_actual;
GLint cambiar_frame;
GLint presionar_1;
GLint presionar_2;
GLint retardo_disparo;
GLint retardo_malo[10];
GLint dificultad_decision;
GLint contador_para_final_de_round;
GLint contador_para_inicio_de_round;
GLint contador_para_mensajes;
GLint municiones;    
GLbyte hora_del_dia;
GLbyte juego_iniciado;
GLbyte cronometro_fin_del_juego;
GLbyte fin_del_juego;
GLbyte tipo_de_arma;
GLbyte no_de_enemigos;    
GLbyte retardo_disparos_enemigos;
GLbyte retardo_disparo_mono;
GLbyte nivel_del_juego;
GLbyte detener_malos;
GLbyte tiempo_para_round;
GLbyte cronometro_para_round;
GLbyte no_de_vidas;
GLbyte pantalla_de_final_de_round;
GLbyte pantalla_de_inicio_de_round;
GLbyte no_de_cancion;
GLbyte presentar_controles;
GLbyte presionar_enter;
GLbyte presionar_boton_mouse;
GLint volumen_de_musica;
GLfloat velocidad_de_disparo;

//titulo
GLfloat traslado_en_z;


// animacion
int frame;
volatile int segundos;
volatile int speed_counter;


//
double angle_speed;
double dist_speed;

int frames;
volatile int secs;



typedef struct CAMERA {
	float posx, posy, posz;
	float targetx, targety, targetz; 
	float rotacion;
}CAMERA;


CAMERA camera;





typedef struct GRUPOS
{
   char nombre[20];
}GRUPOS;

GRUPOS *grupo;

typedef struct ARCHIVO_TEX
{
   char nombre[256];
}ARCHIVO_TEX;



typedef struct VERTICE
{
   float x,y,z;
}VERTICE;



typedef struct TEXTURA
{
 float u,v;
}TEXTURA;




typedef struct POLIGONO
{
 unsigned char id;              //Identificacion del poligono. Puede usarse para lo que sea
 unsigned char doble_cara;
 unsigned char transparencia;
 unsigned short capa;
 unsigned short l1,l2,l3;       // vertices del poligono
 long siguiente_poli;           // indice para la tabla de ordenamiento
 
 TEXTURA tl1,tl2,tl3;           // coordendas de textura de cada uno de los vertices
 TEXTURA mt1,mt2,mt3;           // coordendas de mapa de iluminaci¢n
 unsigned char tipo;            /* tipo de poligono (0=texturizado affine,
                                                     1=texturizado por perspectiva,
                                                     2=masked affine,
                                                     3=masked perspective)*/
 unsigned short tex;    //no. de identificacion de la textura
 unsigned short mtex;    //no. de identificacion del mapa de iluminacion
 }POLIGONO;







typedef struct POLIGONO_CON_MASCARA
{
 unsigned char id;              //Identificacion del poligono. Puede usarse para lo que sea
 unsigned short capa;
 
 
 VERTICE rotacion;
 VERTICE posicion;

 VERTICE l1;
 VERTICE l2;
 VERTICE l3;

 VERTICE l1_t;
 VERTICE l2_t;
 VERTICE l3_t;
 
 VERTICE luz_1;
 VERTICE luz_2;
 VERTICE luz_3;
 
 TEXTURA tl1,tl2,tl3;           // coordendas de textura de cada uno de los vertices
 TEXTURA mt1,mt2,mt3;           // coordendas de mapa de iluminaci¢n

 unsigned short tex;            //no. de identificacion de la textura
 unsigned short mtex;           //no. de identificacion del mapa de iluminacion
 long siguiente_poli;           // indice para la tabla de ordenamiento

 }POLIGONO_CON_MASCARA;



long *to;


typedef struct OBJETO
{
 short *huesocuerpo;
 short *huesocara;  

 unsigned short *capa;

 unsigned short no_de_vertices;
 unsigned short no_de_caras;

 VERTICE *vertices;             //datos de los vertices originales
 VERTICE *cara;
 VERTICE *cuerpo;

 unsigned short *caras_transparentes;

 unsigned short *ocurrencia;
 VERTICE *normales_de_ocurrencias;
 
 VERTICE *luz;
 VERTICE *luz_t;

 POLIGONO *poligonos; //datos de las caras

 }OBJETO;




typedef struct KEYFRAME{
  float x,y,z;
  }KEYFRAME;




typedef struct HUESOS{
  float x,y,z;  //coordenadas de origen

  MATRIX_f final;
  MATRIX_f rotacion_final;

  MATRIX_f anim;
  MATRIX_f anim_rot;

  KEYFRAME k_traslado[256];
  KEYFRAME k_rotacion[256];
  KEYFRAME k_escala[256];
  short hueso_anterior;
  }HUESOS;


typedef struct OSEO{

   float retardo;
   float lerp;
   short lerp_de;
   short lerp_de_ant;
   short lerp_a_ant;
   short lerp_a;
   short curr_act;
   short actual_fr;
   int caminar;
    
   short no_de_huesos;
   HUESOS hueso[256];
   VERTICE trans[256];
  }OSEO;
  
  
typedef struct PERSONAJE{
   float x,y,ry;   
   char decision;
   char decision_disparo;
   int periodo_decision;
   int periodo_disparo;
   int vida;
   }PERSONAJE;
   
typedef struct DISPAROS
{
   char tipo;
   char disponible;
   float x, y, ry;
}DISPAROS;



typedef struct ADITAMENTO
{
   char disponible;
   float x,y, ry;
   char tipo; /*
                0 - vida +25
                1 - ametralladora
                2 - +1500 pts
              */

}ADITAMENTO;




DISPAROS mono_disparo[12];
DISPAROS malo_disparo[10][12];   
PERSONAJE pos_mono;           
PERSONAJE pos_malo[10];
ADITAMENTO aditamento;


OSEO *esqueleto;
OSEO *esqueleto2;
OBJETO *mono;
OBJETO *proyectil;
OBJETO *malo[10];
OBJETO *escenario;
OBJETO *esfera;
OBJETO *armamento;

int mickey_x;
int mickey_y;



unsigned short no_de_poligonos_con_mascara;
POLIGONO_CON_MASCARA *poligonos_con_mascara;



//general

void iniciar_posiciones();
void iniciar_aditamento();
void iniciar_round();     
void iniciar_vida();
void finalizar_round();
void iniciar_variables();
int iniciar_multitextura();
void leer_teclado();
void incrementar_segundos();
void incrementar_speed_counter();
void secs_timer();
void actualizar_disparo( DISPAROS *disparo );
void actualizar_disparo_enemigo( DISPAROS *disparo );
void actualizar_posiciones_de_malos();
void revisar_aditamento();


//rutinas de modelos
OBJETO *leer_modelo( char *archivo );
GLuint leer_textura ( char *archivo, int tipo);
void encontrar_normal( unsigned long vertice_actual, OBJETO *obj );
void destruir_modelo( OBJETO *obj );

//esqueleto
OSEO *leer_esqueleto( char *archivo_esqueleto );
void multiplicar_matriz( OSEO *skelly, int numero );
void ajustar_matrices( OSEO *skelly );
void calcular_esqueleto( OSEO *skelly_cara, OSEO *skelly_cuerpo, OBJETO *obj );
void calcular_cuerpo( OSEO *skelly, OBJETO *obj );
void transformar_objeto_sin_esqueleto( OBJETO *obj );
void copiar_lerp ( OSEO *skelly, int numero);
void animar_modelo( OSEO *skelly ); 

//rutinas de dibujo
void desplegar_modelo ( float rotx, float roty, float rotz, float posx, float posy, float posz, OBJETO *obj, int no_de_modelo );
void desplegar_poligonos_con_mascara();
void dibujar_disparos( DISPAROS *particulas, int no_de_modelo );
void dibujar_mira();
void dibujar_sombra( float posx, float posy );
void destello( float fx, float fy, float transp );

//pantalla principal
void actualizar_titulo();
void dibujar_controles();
void dibujar_titulo();
void rutina_de_titulo();

//hora del dia
void color_de_alba();
void color_de_atardecer();
void color_de_noche();

//frustum
void normalizar_plano(int side);
void extraer_frustum();
int punto_en_frustum(float x, float y, float z);
int esfera_en_frustum(float x, float y, float z, float radio);



