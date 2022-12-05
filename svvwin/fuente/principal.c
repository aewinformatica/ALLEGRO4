#include "emd.h" 



int main(){   
  
   int conteo,conteo2;
                          
   allegro_init();
   install_allegro_gl();
   install_timer();
   install_mouse();
   install_keyboard();
   
   marcador_maximo=25000;

   poligonos_con_mascara=malloc( sizeof(POLIGONO_CON_MASCARA)*8192 );
   if ( poligonos_con_mascara==NULL )return 0;
   memset( poligonos_con_mascara, 0, sizeof(POLIGONO_CON_MASCARA)*8192 );
   
   to=malloc( sizeof(long)*50000 );   
   if ( to==NULL )return 0;
   
   FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
   FSOUND_SetDriver(0);  
   FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH);  
   
   
   //sonidos
   pistola = FSOUND_Sample_Load(FSOUND_FREE,"datos/rifle.wav",FSOUND_LOOP_OFF | FSOUND_16BITS | FSOUND_STEREO ,0);
   ametralladora = FSOUND_Sample_Load(FSOUND_FREE,"datos/mach.wav",FSOUND_LOOP_OFF | FSOUND_16BITS | FSOUND_STEREO ,0);
   pistola_malo = FSOUND_Sample_Load(FSOUND_FREE,"datos/laser.wav",FSOUND_LOOP_OFF | FSOUND_16BITS | FSOUND_STEREO ,0);
   dolor_mono = FSOUND_Sample_Load(FSOUND_FREE,"datos/groan.wav",FSOUND_LOOP_OFF | FSOUND_16BITS | FSOUND_STEREO ,0);
   dolor_malo = FSOUND_Sample_Load(FSOUND_FREE,"datos/groan.wav",FSOUND_LOOP_OFF | FSOUND_16BITS | FSOUND_STEREO ,0);
   golpe_en_pared = FSOUND_Sample_Load(FSOUND_FREE,"datos/mach.wav",FSOUND_LOOP_OFF | FSOUND_16BITS | FSOUND_STEREO ,0);
   sonido_aditamento = FSOUND_Sample_Load(FSOUND_FREE,"datos/aditamento.wav",FSOUND_LOOP_OFF | FSOUND_16BITS | FSOUND_MONO ,0);
   aparicion_de_letras = FSOUND_Sample_Load(FSOUND_FREE,"datos/blast.wav",FSOUND_LOOP_OFF | FSOUND_16BITS | FSOUND_MONO ,0);
   
      
   //modelos
   mono=leer_modelo( "datos/hombre.emd" );
   escenario=leer_modelo( "datos/a.emd" );      
   esfera=leer_modelo( "datos/esfera.emd" );      
   proyectil=leer_modelo( "datos/disparo.emd" );   
   armamento=leer_modelo( "datos/armamento.emd" );      
   for (conteo=0; conteo<10; conteo++){
      malo[conteo]=leer_modelo( "datos/malo.emd" );
      }      
   
   esqueleto=leer_esqueleto( "datos/hombre.esq");
   esqueleto2=leer_esqueleto( "datos/morlock.esq");   


   for (conteo=0; conteo<mono->no_de_vertices; conteo++){
      encontrar_normal( conteo, mono );
      }    
      
   for (conteo=0; conteo<proyectil->no_de_vertices; conteo++){
      encontrar_normal( conteo, proyectil );
      }    
      
   for (conteo=0; conteo<esfera->no_de_vertices; conteo++){
      encontrar_normal( conteo, esfera );
      }    
      
   for (conteo2=0; conteo2<10; conteo2++){
      for (conteo=0; conteo<malo[conteo2]->no_de_vertices; conteo++){
         encontrar_normal( conteo, malo[conteo2] );
      }    
    }
      
      
   for (conteo=0; conteo<escenario->no_de_vertices; conteo++){
      encontrar_normal( conteo, escenario );
      }    
      
   for (conteo=0; conteo<armamento->no_de_vertices; conteo++){
      encontrar_normal( conteo, armamento );
      }    
      
 

   allegro_gl_clear_settings();
   allegro_gl_set (AGL_COLOR_DEPTH, 16);
   allegro_gl_set (AGL_Z_DEPTH, 16);
   allegro_gl_set (AGL_DOUBLEBUFFER, 1);
   allegro_gl_set (AGL_RENDERMETHOD, 1);
   allegro_gl_set (AGL_SUGGEST, AGL_COLOR_DEPTH | AGL_Z_DEPTH | AGL_DOUBLEBUFFER | AGL_RENDERMETHOD);

   set_color_depth(16);
   if (set_gfx_mode(GFX_OPENGL_FULLSCREEN, 640, 480, 0, 0) < 0) {
      allegro_message ("Error al inicializar");
      return -1;
      }
   show_mouse(NULL);
   
   
   dat=load_datafile("datos/fuente.dat");
   pal=load_datafile("datos/pal.dat");      
   
   set_palette(pal[0].dat);
   unload_datafile(pal);
   
   //leer texturas

   //texturas mono
   texturas[0]=leer_textura( "datos/tex1.raw", 1 ); 
   texturas[1]=leer_textura( "datos/tex2.raw", 1 );    
   texturas[2]=leer_textura( "datos/arma.raw", 1 );       
   //texturas malo
   texturas[6]=leer_textura( "datos/malo.raw", 1 );
   //texturas disparos
   texturas[7]=leer_textura( "datos/disparo_malo.raw", 0 );
   texturas[8]=leer_textura( "datos/disparo_mono.raw", 0 );
   texturas[9]=leer_textura( "datos/muerte_1.raw", 0 );   
   texturas[10]=leer_textura( "datos/muerte_2.raw", 0 );      
   //texturas esfera
   texturas[11]=leer_textura( "datos/alba.raw", 0 );
   texturas[12]=leer_textura( "datos/atardecer.raw", 0 );
   texturas[13]=leer_textura( "datos/noche.raw", 0 );
   //texturas escenario
   texturas[14]=leer_textura( "datos/piso1.raw", 3 );
   texturas[15]=leer_textura( "datos/piso2.raw", 3 );
   texturas[16]=leer_textura( "datos/pared1.raw", 3 );   
   texturas[17]=leer_textura( "datos/ruina.raw", 3 );      
   texturas[18]=leer_textura( "datos/hangar2.raw", 3 );         
   texturas[19]=leer_textura( "datos/hangar1.raw", 3 );
   texturas[20]=leer_textura( "datos/techo.raw", 3 );   
   texturas[21]=leer_textura( "datos/torre_1.raw", 3 );
   texturas[22]=leer_textura( "datos/torre_2.raw", 3 );
   texturas[23]=leer_textura( "datos/1.raw", 3 );      
   texturas[24]=leer_textura( "datos/armamento.raw", 0 );         
   
   texturas_titulo[0]=leer_textura( "datos/titulo.raw", 0 );
   texturas_titulo[1]=leer_textura( "datos/controles_teclado.raw", 0 );
   texturas_titulo[2]=leer_textura( "datos/controles_mouse.raw", 0 );
   texturas_titulo[3]=leer_textura( "datos/destello.raw", 0 );

   //inicializar OpenGL
   allegro_gl_begin();

    glClearColor (0.0, 0.0, 0.0, 1.0);
    glShadeModel (GL_SMOOTH);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
    glEnable (GL_DEPTH_TEST);
    glCullFace (GL_BACK);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
    
    //luz
    color_de_alba();    
    glEnable( GL_LIGHT0 );

    glEnable(GL_LIGHTING);								// Enable Light One
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
    glEnable(GL_DITHER);
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    glViewport(0,0,640,480);						// Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix
    gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.5f,1000.0f);
    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
      

    demofont = allegro_gl_convert_allegro_font( dat[0].dat, AGL_FONT_TYPE_TEXTURED);
    unload_datafile(dat);


   allegro_gl_end();


   LOCK_FUNCTION(secs_timer);
   LOCK_VARIABLE(secs);
   LOCK_VARIABLE(segundos);
   LOCK_FUNCTION(incrementar_segundos);

   install_int (secs_timer, 1000);
   install_int_ex (incrementar_segundos, BPS_TO_TIMER(60));
   segundos=0;         
   
   iniciar_variables();   
  

   while ( !key[KEY_ESC] ){
   
        while(segundos>0){                                     
        
        if (juego_iniciado){
          
           get_mouse_mickeys( &mickey_x, &mickey_y );
           if (fin_del_juego==0) leer_teclado();
                         
           animar_modelo( esqueleto );
           calcular_cuerpo(esqueleto, mono);
           
           if (!pantalla_de_inicio_de_round) esqueleto2->caminar=1;
           else esqueleto2->caminar=0;
           animar_modelo( esqueleto2 );                     
           for (conteo=0; conteo<no_de_enemigos; conteo++){
              calcular_cuerpo(esqueleto2, malo[conteo]);
              }

           transformar_objeto_sin_esqueleto( escenario);
           
           //transparencia de destello
           if ( desaparecer_destello_mono>0.0 ){
              desaparecer_destello_mono-=0.025;
              if (desaparecer_destello_mono<0.05){
                 if (no_de_vidas<1 && fin_del_juego==0) {
                    fin_del_juego=1;
                    cronometro_fin_del_juego=5;
                    }
                 if (no_de_vidas>0 && fin_del_juego==0) iniciar_vida();
                 }
              }
           
           for (conteo=0; conteo<no_de_enemigos; conteo++){
              if ( desaparecer_destello_malo[conteo]>0 ) {
                 desaparecer_destello_malo[conteo]-=0.05;
                 }
              }
           
           if (!detener_malos) 
              actualizar_posiciones_de_malos();           
              
           actualizar_disparo( mono_disparo );
           for (conteo=0; conteo<no_de_enemigos; conteo++){
              actualizar_disparo_enemigo( malo_disparo[conteo] );
              }      
              
           //fade para la musica
           if (pantalla_de_final_de_round){
              if (volumen_de_musica>0) volumen_de_musica-=2;
              if (volumen_de_musica<1){
                 volumen_de_musica=0;
                 FSOUND_Stream_Stop( mp3 );
               }
              FSOUND_SetVolume( 0,volumen_de_musica );   
              }
              
           //actualizar_cronometros   
           cronometro_para_round++;
           if (cronometro_para_round>59) {
              if (fin_del_juego){
                 cronometro_fin_del_juego--;
                 if (cronometro_fin_del_juego<1){
                    juego_iniciado=0;
                    }                    
                 }
              if (contador_para_final_de_round>0) {              
                 if (contador_para_final_de_round>2)
                    FSOUND_PlaySound( 1,aparicion_de_letras );
                 contador_para_final_de_round--;
                 }
              if (contador_para_mensajes>0) contador_para_mensajes--;
              if (contador_para_inicio_de_round>0) contador_para_inicio_de_round--;
              if (pantalla_de_inicio_de_round==0 && pantalla_de_final_de_round==0){
                 tiempo_para_round--;              
                 if ( aditamento.disponible==0 && 
                      (tiempo_para_round==98 || tiempo_para_round==50)  ) 
                      iniciar_aditamento();
                 }
              cronometro_para_round=0;
              }
           if (tiempo_para_round==0){
              tiempo_para_round=99;
              no_de_vidas--;         
              desaparecer_destello_mono=1.0;
              }    
           if (aditamento.disponible)
              aditamento.ry+=5;
              
           revisar_aditamento();   
           
           //actualizar marcador
           if (marcador>marcador_maximo) marcador_maximo=marcador;
           }
           else {
              rutina_de_titulo();
              }
              
           segundos--;
        }   

     no_de_poligonos_con_mascara=0;        
     
     allegro_gl_begin();

     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     if (juego_iniciado){

        if (desaparecer_destello_mono<0.05 && fin_del_juego==0){
          desplegar_modelo ( 0, 
                             0, 
                             ((pos_mono.ry+3.1415)*-1)*57.2957,
                             pos_mono.x,
                             pos_mono.y,
                             0,                       
                             mono, 0 );
             }
    
       for (conteo=0; conteo<no_de_enemigos; conteo++){
    
         if (pos_malo[conteo].vida>0){
    
             desplegar_modelo ( 0, 
                                0, 
                                ((pos_malo[conteo].ry+3.1415)*-1)*57.2957, 
                                pos_malo[conteo].x,
                                pos_malo[conteo].y,
                                0,                       
                                malo[conteo], 2 );
            }                 
                          
                          
          }
                       
                       
       desplegar_modelo ( 0, 
                          0, 
                          0, 
                          0,
                          0,
                          0,
                          escenario, 7 );
    
                    
       if (aditamento.disponible){                                                            
    
          transformar_objeto_sin_esqueleto( armamento );
          desplegar_modelo ( 0, 
                             0, 
                             aditamento.ry, 
                             aditamento.x,
                             aditamento.y,
                             0,
                             armamento, 8 );
        }
           
          transformar_objeto_sin_esqueleto( esfera );     
          desplegar_modelo ( 0, 
                             0, 
                             0, 
                             0,
                             0,
                             0,
                             esfera, 6 );
                 
                           
       desplegar_poligonos_con_mascara();
       dibujar_sombra( pos_mono.x, pos_mono.y );                         
    
       //disparos
       dibujar_disparos( mono_disparo, 0 );
    
       for (conteo=0; conteo<no_de_enemigos; conteo++)
       {
          dibujar_disparos( malo_disparo[conteo], 1 );
       }
    
       //destellos
       for (conteo=0; conteo<no_de_enemigos; conteo++)
       {
          if (desaparecer_destello_malo[conteo]>0.0)
             destello( pos_malo[conteo].x, pos_malo[conteo].y, desaparecer_destello_malo[conteo] );
       }
    
      if (desaparecer_destello_mono>0.0) {
         destello( pos_mono.x, pos_mono.y, desaparecer_destello_mono );
         }    
    
    
       dibujar_mira();
    
    

       glEnable(GL_BLEND);     
       glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
       glDepthFunc(GL_ALWAYS);
       glDisable(GL_CULL_FACE);     
       glDisable(GL_LIGHTING);         
    
       glLoadIdentity();
       glTranslatef(0,0,0);    
    
       if (!pantalla_de_final_de_round && !pantalla_de_inicio_de_round && !fin_del_juego ){
    
          allegro_gl_printf(demofont,-40.00,32.00,-80.0,makecol(255,255,255), "P1 %12d                  Max %12d", marcador, marcador_maximo  );
          if (tiempo_para_round>25) {
             allegro_gl_printf(demofont,-2.00,18.00,-45.0,makecol(255,255,128), "%2d", tiempo_para_round );
          }
          else {
             if (cronometro_para_round<25)
                allegro_gl_printf(demofont,-2.00,18.00,-45.0,makecol(255,0,0), "%2d", tiempo_para_round );
             else
                allegro_gl_printf(demofont,-2.00,18.00,-45.0,makecol(255,255,128), "%2d", tiempo_para_round );          
          }
    
          allegro_gl_printf(demofont,-5.50,40.00,-120.0,makecol(128,128,255), "TIME");    
    
          if (pos_mono.vida>25) allegro_gl_printf(demofont,-13.50,-7.00,-25.0,makecol(128,127,255), "%d",pos_mono.vida );
          else allegro_gl_printf(demofont,-13.50,-7.00,-25.0,makecol(255,0,0), "%d",pos_mono.vida );   
    
          allegro_gl_printf(demofont,-8.0,-15.00,-50.0,makecol(128,255,128), "Life x %d",no_de_vidas );
          if (municiones>-1) allegro_gl_printf(demofont,7.50,-7.00,-25.0,makecol(255,255,0), "%d",municiones );
          if (municiones==-1) allegro_gl_printf(demofont,7.50,-7.00,-25.0,makecol(255,255,0), "Inf");
       }
    
       if (pantalla_de_inicio_de_round){
          allegro_gl_printf(demofont,-8.0,10.00,-40.0,makecol( cronometro_para_round*4 ,cronometro_para_round*4,cronometro_para_round*4), "READY" );
          allegro_gl_printf(demofont,-8.0,-15.00,-50.0,makecol(128,255,128), "Round %d",nivel_del_juego );
          if (contador_para_inicio_de_round<1) {
             pantalla_de_inicio_de_round=0;
             detener_malos=0;
             }
          }
    

       if (pantalla_de_final_de_round){
          if (contador_para_final_de_round<5 && pos_mono.vida==150 && cronometro_para_round>=25) allegro_gl_printf(demofont,-15,7.00,-50.0,makecol(255,128,128), "PERFECT");
          if (contador_para_final_de_round<5 && pos_mono.vida==150 && cronometro_para_round<25) allegro_gl_printf(demofont,-15,7.00,-50.0,makecol(255,255,255), "PERFECT");
          if (contador_para_final_de_round<5) allegro_gl_printf(demofont,-25,2.00,-70.0,makecol(128,128,128), "Time %d x 100 = %d",tiempo_para_round, tiempo_para_round*100 );
          if (contador_para_final_de_round<4) allegro_gl_printf(demofont,-25,-3.00,-70.0,makecol(128,128,128), "Health %d x 100 = %d",pos_mono.vida, pos_mono.vida*100 );
          if (contador_para_final_de_round<3 && pos_mono.vida<150) allegro_gl_printf(demofont,-20,-8.00,-60.0,makecol(255,255,128), "Total = %d",marcador );              
          if (contador_para_final_de_round<3 && pos_mono.vida==150) allegro_gl_printf(demofont,-20,-8.00,-60.0,makecol(255,255,128), "Total + 10000 = %d ",marcador );
          if (contador_para_final_de_round<1) {
             pantalla_de_final_de_round=0;
             iniciar_round();
             }
          }
          
       if (fin_del_juego>0){
          allegro_gl_printf(demofont,-40.00,32.00,-80.0,makecol(255,255,255), "P1 %12d                  Max %12d", marcador, marcador_maximo  );
          allegro_gl_printf(demofont,-10.00,2.00,-30.0,makecol(255,255,255), "Game Over");
          }
       
       if (contador_para_mensajes>0 && 
           !pantalla_de_final_de_round && 
           !pantalla_de_final_de_round && !fin_del_juego )
          allegro_gl_printf(demofont,15.00,0.00,-80.0,makecol(255,255,255), "%s", linea1 );
          
      //debug
      // allegro_gl_printf(demofont,-20.00,-10.00,-80.0,makecol(255,255,255), "%d %d %d %5.4f", lerp_de, lerp_a, caminar, retardo );    
      
       glEnable(GL_LIGHTING);             
       glEnable(GL_CULL_FACE);
       glDisable(GL_BLEND);
       glDepthFunc(GL_LESS);
       
       }//juego iniciado
    else {
       if (!presentar_controles) {
          actualizar_titulo();
          }
       else {
          dibujar_controles();
          transformar_objeto_sin_esqueleto( armamento );          
          desplegar_modelo ( 0, 
                             0, 
                            ((traslado_en_z*-1)*360), 
                             0,
                             0,
                             0,
                             armamento, 8 ); 
          }
       }

    glFlush();
    frames++;
    allegro_gl_flip();        
    allegro_gl_end();
     
    }
  
   set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
   
   destruir_modelo(mono);
   destruir_modelo(proyectil);   
   
   for (conteo=0; conteo<no_de_enemigos; conteo++){
      destruir_modelo( malo[conteo] );
      }
      
   destruir_modelo(escenario);   
   destruir_modelo(esfera);      
   free(poligonos_con_mascara);
   free(to);   
   free(esqueleto);
   
   set_gfx_mode(GFX_TEXT,0,0,0,0);
      
   FSOUND_Stream_Stop(mp3);
   FSOUND_Stream_Close(mp3);
   FSOUND_Close();        
   

   return 0;
   }
END_OF_MAIN();











