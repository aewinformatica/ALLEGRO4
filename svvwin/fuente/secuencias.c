#include "emd.h"

//entra en la habitacion y se cae
void secuencia1(){


   hueso_camara=53;
   hueso_target=54;

   free(esqueleto);
   esqueleto=leer_esqueleto( "sec1.esq");    

   color_de_ambiente[0]=0.70;
   color_de_ambiente[1]=0.70;   
   color_de_ambiente[2]=0.70;   
   color_de_ambiente[3]=1.00;   
   
   color_de_luz[0]=0.30;
   color_de_luz[1]=0.30;   
   color_de_luz[2]=0.40;   
   color_de_luz[3]=1.00;   
   
   posicion_de_luz[0]=33.00;
   posicion_de_luz[1]=0.00;   
   posicion_de_luz[2]=18.00;   
   posicion_de_luz[3]=1.00;      
   


   allegro_gl_begin();
      glLightfv(GL_LIGHT1, GL_AMBIENT, color_de_ambiente);		
      glLightfv(GL_LIGHT1, GL_DIFFUSE, color_de_luz);		
      glLightfv(GL_LIGHT1, GL_POSITION, posicion_de_luz);	
      glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001);
      
      glEnable(GL_LIGHT1);      
      glDisable(GL_LIGHT2);
      
   allegro_gl_end();      

   lerp=0;
   lerp_de=1;
   lerp_a=2;
   retardo=10;
   }
   
//se quita la toalla de la cabeza
void secuencia2(){

   strcpy(linea1,"");
   strcpy(linea2,"");  

   lerp=0;
   lerp_de=35;
   lerp_a=36;
   retardo=15;
   }
   
//mira los juguetes
void secuencia3(){

   char copia_de_linea1[256];
   char copia_de_linea2[256];

   strcpy(copia_de_linea1,"¡¡Carajo!! ¡Mi 'querido' hermanito");
   strcpy(copia_de_linea2,"volvió a dejar sus juguetes tirados!");  
  
   do_uconvert(copia_de_linea1, U_ASCII, linea1, U_UTF8, 256);   
   do_uconvert(copia_de_linea2, U_ASCII, linea2, U_UTF8, 256);     
   

   lerp=0;
   lerp_de=50;
   lerp_a=51;
   retardo=180;
   }   
   
   
//patea los juguetes
void secuencia4(){

   strcpy(linea1,"");
   strcpy(linea2,"");  

   lerp=0;
   lerp_de=52;
   lerp_a=53;
   retardo=15;
   }
   
//se levanta
void secuencia5(){
   lerp=0;
   lerp_de=55;
   lerp_a=56;
   retardo=15;
   }
   
//enciende la lampara
void secuencia6(){

   
   free(esqueleto);   
   esqueleto=leer_esqueleto( "sec2.esq");    
   
   hueso_camara=39;
   hueso_target=40;         

   lerp=0;
   lerp_de=1;
   lerp_a=2;
   retardo=15;
   }
   
//toma el control y enciende la tele
void secuencia7(){
   lerp=0;
   lerp_de=20;
   lerp_a=21;
   retardo=20;
   }
   
//se sienta a ver la tele   
void secuencia8(){

   free(esqueleto);
   esqueleto=leer_esqueleto( "sec3.esq");    
   
   hueso_camara=38;
   hueso_target=39;

   lerp=0;
   lerp_de=1;
   lerp_a=2;
   retardo=20;
   }
   
   
 
   
   
   



void secuenciador(){   

  char copia_de_linea1[256];
  char copia_de_linea2[256];  

  //entra en la habitacion y se cae
  if (contador_de_secuencias==1){
     if (inicio_de_secuencia==0) {
        secuencia1();
        inicio_de_secuencia=1;
        }

     if (lerp_a==31) { 
        strcpy(copia_de_linea1,"           ¡¡AY GÜERO!!");
        strcpy(copia_de_linea2,"");  
  
        do_uconvert(copia_de_linea1, U_ASCII, linea1, U_UTF8, 256);   
        do_uconvert(copia_de_linea2, U_ASCII, linea2, U_UTF8, 256);          
        }        
        
     if (lerp_a==35) { 
        contador_de_secuencias++;
        inicio_de_secuencia=0;
        }
     }

  //se quita la toalla de la cabeza   
  if (contador_de_secuencias==2){
     if (inicio_de_secuencia==0) {
        secuencia2();
        inicio_de_secuencia=1;
        }
     if (lerp_a==50) { 
        contador_de_secuencias++;
        inicio_de_secuencia=0;
        }
     }
     
  //mira los juguetes
  if (contador_de_secuencias==3){
     if (inicio_de_secuencia==0) {
        secuencia3();
        inicio_de_secuencia=1;
        }
     if (lerp_a==53) { 
        contador_de_secuencias++;
        inicio_de_secuencia=0;
        }
     }
     
  //patea los juguetes
  if (contador_de_secuencias==4){
     if (inicio_de_secuencia==0) {
        secuencia4();
        inicio_de_secuencia=1;
        }
     if (lerp_a==55) { 
        contador_de_secuencias++;
        inicio_de_secuencia=0;
        }
     }
     
  //se levanta
  if (contador_de_secuencias==5){
     if (inicio_de_secuencia==0) {
        secuencia5();
        inicio_de_secuencia=1;
        }
     if (lerp_a==70) { 
        contador_de_secuencias++;
        inicio_de_secuencia=0;
        }
     }
     
  //enciende la lampara
  if (contador_de_secuencias==6){
     if (inicio_de_secuencia==0) {
        secuencia6();
        inicio_de_secuencia=1;
        }

     if (lerp_a==9) { 
        color_de_ambiente[0]=1.50;
        color_de_ambiente[1]=1.50;   
        color_de_ambiente[2]=1.50;   
        color_de_ambiente[3]=1.00;   
   
        color_de_luz[0]=1.00;
        color_de_luz[1]=0.80;   
        color_de_luz[2]=0.30;   
        color_de_luz[3]=1.00;   
   
        posicion_de_luz[0]=26.80;
        posicion_de_luz[1]=17.20;   
        posicion_de_luz[2]=9.10;   
        posicion_de_luz[3]=1.00;      
        
        allegro_gl_begin();
          glLightfv(GL_LIGHT1, GL_AMBIENT, color_de_ambiente);		
          glLightfv(GL_LIGHT1, GL_DIFFUSE, color_de_luz);		
          glLightfv(GL_LIGHT1, GL_POSITION, posicion_de_luz);	
          glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.001);           
        allegro_gl_end();              
     
        }
        
        
     if (lerp_a==20) { 
        contador_de_secuencias++;
        inicio_de_secuencia=0;
        }
     }
     
  //toma el control y enciende la tele
  if (contador_de_secuencias==7){
     if (inicio_de_secuencia==0) {
        secuencia7();
        inicio_de_secuencia=1;
        }
     if (lerp_a==24) { 
        color_de_luz2[0]=0.50;
        color_de_luz2[1]=0.50;   
        color_de_luz2[2]=0.80;   
        color_de_luz2[3]=1.00;   
   
        posicion_de_luz2[0]=5.00;
        posicion_de_luz2[1]=-12.00;   
        posicion_de_luz2[2]=10.00;   
        posicion_de_luz2[3]=1.00;        
        
        allegro_gl_begin();
          glLightfv(GL_LIGHT2, GL_DIFFUSE, color_de_luz2);
          glLightfv(GL_LIGHT2, GL_POSITION, posicion_de_luz2);
          glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.001);
          glEnable(GL_LIGHT2);
        allegro_gl_end();                   
        }
        
     if (lerp_a==27) { 
        contador_de_secuencias++;
        inicio_de_secuencia=0;
        }
     }
     
     
  //se sienta a ver la tele
  if (contador_de_secuencias==8){
     if (inicio_de_secuencia==0) {
        secuencia8();
        inicio_de_secuencia=1;
        }
     if (lerp_a==30) { 
        contador_de_secuencias++;
        inicio_de_secuencia=0;
        }
     }
     
     
     
     
     
     
  }
   



