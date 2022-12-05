#include "jogo.h"
#include "sistema.h"

static volatile int gifTempo = 0;

static void gifTimer(void){
gifTempo++;
}
END_OF_FUNCTION(gifTimer);  
LOCK_FUNCTION(gifTimer);  
LOCK_VARIABLE(gifTimer);

void entrada()
{
      BITMAP *fundo,*buffer; 
      MIDI	*musica; 
      SAMPLE *voz,*musicaee;
      
      BITMAP **frames;
      int *durations;
      char const *name = "fundos/2010.gif";
      int f,iFrame,espera;
            iFrame = algif_load_animation (name, &frames, &durations);
            GIF_ANIMATION *gif = algif_load_raw_animation (name);

            
                     // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
         install_int( gifTimer, 1000 ); 
         //install_int_ex( gifTimer, BPS_TO_TIMER(100) );
            
             
      int i=0,m=0,e=0;//variaveis que pega a posicao do mause
     
       install_keyboard();
       fundo = create_bitmap(MAX_X,MAX_Y);
       buffer = create_bitmap(MAX_X,MAX_Y);
       musica = load_midi("musicas/Metroid Prime 3 Corruption Title.mid");
       play_midi(musica, TRUE);//inicia musica
      musicaee = load_wav("musicas/tema.wav");
     // play_sample(musicaee,250,130,1000,0);
     
       //ENTRADA SEQUENCIAS//////////
      i=1;
      e=-400;
      do
      {
                  if(i==1)
                  {    
                     for (f = 0; f < iFrame; f++){
                          //for (f = 0; f < gif->frames[f].duration; f++){ descobrir 
                         //rest(900);
                     draw_sprite(screen, frames[f], 0, 0);
                     //algif_render_frame (gif, screen, f,0, 0);
                     espera = gifTempo + durations[f];
                     
                     if(gifTempo < espera)
                     rest(1000);
                     }
                  }
                   else
                    if(i==5)
                    { 
                        clear(buffer);
                        e++;        
        
                        fundo = load_gif("fundos/inicio5.gif", NULL);                  
                        draw_sprite(buffer,fundo,0,e);//desenha na tela a nova imagem
                        rest(60); 
                       
                        if(e<-280)
                        {
                          fundo = load_gif("fundos/inicio6.gif", NULL);  
                          draw_sprite(buffer,fundo,330,0);//desenha na tela a nova imagem       
                        }
                        else
                          if(e>-280 && e<-150)
                          {
                             destroy_bitmap(fundo);       
                             fundo = load_gif("fundos/inicio7.gif", NULL);  
                             draw_sprite(buffer,fundo,330,0);//desenha na tela a nova imagem   
                          }
                          else
                            if(e>-150)
                            {
                               destroy_bitmap(fundo);       
                               fundo = load_gif("fundos/inicio8x.gif", NULL);  
                               draw_sprite(buffer,fundo,330,0);//desenha na tela a nova imagem   
                            }
                            
                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                     
                      
                   }   
                    else
                   if(i==6)
                   {   
                      char const *luz = "fundos/luz.gif";
                      iFrame = algif_load_animation (luz, &frames, &durations);
                      
                     for (f = 0; f < iFrame; f++){
                         rest(900);
                     draw_sprite(screen, frames[f], 330, 0);
                     espera = gifTempo + durations[f];
                     if(gifTempo < espera)
                     rest(1000);
                     } 

                      
                      voz = load_wav("musicas/entradav1.wav");//get ready
                      play_sample(voz,200,130,1000,0);
                  
                   } 
                   else
                   if(i==12)
                   {   
                      clear(buffer);                      
                     voz = load_wav("musicas/entradav2.wav");//respiração
                      
                      
                      char const *shock = "fundos/capacete.gif";
                      iFrame = algif_load_animation (shock, &frames, &durations);
                      
                     for (f = 0; f < iFrame; f++){
                         rest(900);
                     play_sample(voz,200,130,1000,0);
                     draw_sprite(screen, frames[f], 0, 0);
                     espera = gifTempo + durations[f];
                     if(gifTempo < espera)
                     rest(1000);
                     }
                                        
                   }
                   if(i==15){
                            clear(buffer);
                       voz = load_wav("musicas/eletro.wav");
                       
                       char const *shock = "fundos/shock.gif";
                      iFrame = algif_load_animation (shock, &frames, &durations);
                      play_sample(voz,200,130,1000,0);
                     for (f = 0; f < iFrame; f++){
                         //rest(1);
                     draw_sprite(screen, frames[f], 0, 0);
                     espera = gifTempo + durations[f];
                     if(gifTempo < espera)
                     rest(250);
                     
                     }
                   }
                                    
if(key[KEY_ENTER])
i=16; 

if(i!=5)
i++;  
          if(i==5 && e<0)
              {
               e++;
               e++;
               if(e>0)
                i++;
              } 
    
      }while (!key[KEY_ENTER] && i<16);  
      
      stop_midi();
      destroy_midi(musica);
      destroy_bitmap(buffer);
      destroy_sample(voz);
     destroy_sample(musicaee);
      
}
