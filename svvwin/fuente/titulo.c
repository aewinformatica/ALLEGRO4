#include "emd.h"


void actualizar_titulo(){

   dibujar_titulo();
   
   }


void rutina_de_titulo(){

   traslado_en_z+=0.005;
   if (traslado_en_z>1.0)
      traslado_en_z=0;
      
   if (!key[KEY_ENTER]) presionar_enter=0;
      
   if (key[KEY_ENTER] && presentar_controles==1 && presionar_enter==0 ) {
      presionar_enter=1;
      presentar_controles=0;
      iniciar_variables();      
      iniciar_round();
      juego_iniciado=1;
      marcador=0;
      no_de_vidas=3;
      }      

   if (key[KEY_ENTER] && presentar_controles==0 && presionar_enter==0 ) {
      presionar_enter=1;   
      presentar_controles=1;
      }

      
      
   }
