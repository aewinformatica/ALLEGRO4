#include <alleggl.h>
#include <conio2.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <allegro.h>
#include <stdlib.h>
#include <ctype.h>
#include <alpng.h>
#include <algif.h>
#include <alleggl.h>

volatile int FecharJogo = FALSE;
volatile int tempo = 0;//variavel para contador de tempo1
volatile int tempo2 = 0;//variavel para contador de tempo2
volatile int tempo3 = 0;//variavel para contador de tempo3
int SALVEJOGO=0;//variavel que diz se o jogador vai salvar no arquivo 1,2 ou 3 ou qual jogo eh para carregar do arquivo 1,2 ou 3
int energia=99;///////controla a vida do jogador
int auxsalve=0;//auxiliar para verificar se tem salve
#define TF 50 //normal eh 20 coloca 50 para caso cadastrar mais ter espaco por ser estatico
#define MAX_X 800
#define MAX_Y 600
#define FPS_V 50
#define COLOR_BITS 16 

//-lalpng -lalleg -lalgif

struct Tiro
{
	int x, y;
	int vel;
	int ativo;
};

struct salvamento
{
  int ativacenax,
  ativax,
  roupax,
  cenacapitaox,
  cenamonitorx,
  somdeexplosaox,
  energiax;
  char localdejogo[20];//diz qual local o personagem esta    
};



void inicia();//inicia allegro
void BotaoFechar(void);
int menuprincipal(SAMPLE *tema,BITMAP *fundoy,BITMAP *buffery,BITMAP *cursory);
int telainicial();
void finaliza();
void entrada();
void historia();
void jogo();

void movimentos(int d,int a,int x,int y,int mf,int mt,char posicao[20],int sinalpv,SAMPLE *passo,SAMPLE *voz/*,SAMPLE *pt*/,int roupa,int lpulo);
void pulo(int &x,int &y,int &lpulo,int &sinalpv,int matriz[800][600]);

void elevador(int xe,int ye,int yf,BITMAP *buffer,BITMAP *fundo2,BITMAP *fundo1,BITMAP *painel,BITMAP *elevadorc1,BITMAP *elevadorc2,int estado,int fps,int xcenario,int roupa);
void elevador2(int xe,int ye,int yf,int pbaseelevador,BITMAP *buffer,BITMAP *fundo2,BITMAP *fundo1,BITMAP *painel,BITMAP *elevadorc1,BITMAP *elevadorc2,int estado,BITMAP *elevadorb1,BITMAP *elevadorb2,int fps,int xcenario,int roupa);
void passaporta(int estado,int x,int y,int fps);

void atirar(int x,int y,int d,Tiro tirof[3],Tiro tirot[3],Tiro tiroc[3],SAMPLE *ptiro);
void show_tiros(int x,int y,BITMAP *buffer,int d,Tiro tirof[3],Tiro tirot[3],Tiro tiroc[3],int matriz[800][600],int roupa);
void iniciatiro(Tiro tirof[3],Tiro tirot[3],Tiro tiroc[3],int d,int &flag);
int pause1(int opcao);
int pause2(int opcao);
void movimentopersonagem(BITMAP *bufferS,BITMAP *imagemfS,int &xs,int &ys,int inicio,int fim,int &direcao,int posicaoimagem,int qualimagem);
            
//////////FASE 1/////////////////////////////////
int cenario1(int estado,int fps,int roupa);//longo na horizontal
int cenario12(int estado,int fps,int roupa);
int cenario13(int estado,int fps,int roupa);
int cenario14(int estado,int fps,int fundo,int roupa);
int cenario15(int estado,int fps,int roupa);
int cenario16(int estado,int fps,int roupa);
int cenario16p2(int estado,int fps,int roupa);
int cenario17(int estado,int fps,int roupa);//longo na horizontal
int cenario18(int estado,int fps,int ativa,int roupa);//com 3 caminhos
int cenario19(int fps,int &ativa,int roupa);//chamado dentro do cenario 18
int cenario110(int estado,int fps,int roupa,int &cenamonitor,int cenacapitao);//longo na horizontal com 3 caminhos
int cenario1102(int estado,int fps,int &roupa,MIDI *musicaxx,SAMPLE *musica2,int &ativacena);//sala da armadura,musica2 eh a musica a parar q vem da funcao jogo
int cenario111(int estado,int fps,int roupa,int &somdeexplosao,int &cenacapitao,int ativacena,int ativa,int cenamonitor,int &salvado);
int cenario112(int estado,int fps,int roupa);
int cenario1122(int estado,int fps,int roupa,int &cenacapitao);//sala alternativa do 112 sala do capitao
int cenario113(int estado,int fps,int roupa);
/////////////////////////////////////////////////////////////////////////

////////CONTROLADORES DE TEMPO///////////////////////////////
void IncrementaT()  
{
   tempo ++;
   if(tempo>1)
    tempo=0;  
}
END_OF_FUNCTION(IncrementaT);  
LOCK_FUNCTION(IncrementaT);  
LOCK_VARIABLE(IncrementaT);

void IncrementaT2()//contador de tempo 2 ,referente a coisas com 2 movimentos 
{
   tempo2 ++;
  /* if(tempo2>3)
    tempo=0;*/  
}
END_OF_FUNCTION(IncrementaT2);  
LOCK_FUNCTION(IncrementaT2);  
LOCK_VARIABLE(IncrementaT2);

void IncrementaT3()//contador de tempo 3 (referente a movimentos de personagens 
{
   tempo3 ++;
   if(tempo3>5)
    tempo3=0;  
}
END_OF_FUNCTION(IncrementaT3);  
LOCK_FUNCTION(IncrementaT3);  
LOCK_VARIABLE(IncrementaT3);
///////////////////////////////////////////////////

///////////////////CONTROLADOR DE FPS/////////////////////
//////////////////////////////////////////////////////////////////////////////
int fps = 0;
int fps_antigo = 0;
int fps_speed = 0;
void frame_rate();
// prototipo do contador de velocidade
void incrementa_speed();
///////////////////////////////////////////////////////////////



int main()
{
   int fimdojogo=0;
   
   inicia();//inicia allegro
   do
   { 
        clock();//funcao conta tempo de jogo             
        entrada();
        fimdojogo=telainicial();
        //menuprincipal();
       if(fimdojogo==0 && auxsalve==0)
         historia();
       
         if(fimdojogo==0)
          jogo();
  
   }while(fimdojogo==0);//enquanto o menu principal nao retornar que o usuario deseja sair do jogo pelo menuprincial o jogo roda 
    allegro_exit(); 
    return 1;
}
END_OF_MAIN();

void BotaoFechar(void) 
{ 
   FecharJogo = TRUE; 
} 
//////////////////////////////////////////////////
void inicia (void)
{
	allegro_init();
	install_timer();
	install_mouse();
	alpng_init();
	algif_init();
	set_alpha_blender();//transparencia nas imagens
    set_uformat(U_ASCII); // acentuação
    set_window_title("METROID DARKNESS                                                                                       TIAGO PORTELA(2010)");
	set_color_depth(COLOR_BITS);
    set_close_button_callback(BotaoFechar);  
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,MAX_X,MAX_Y,0,0) < 0)
	{
		printf("Erro ao inicializar o modo grafico.\n");
	    getch();
	}

	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
	{
		printf("Erro ao inicializar o som.\n");
		getch();
	}
	set_volume(250, 250);
	set_display_switch_mode(SWITCH_BACKGROUND);//continuar tocando musica ao minimizar

}
void finaliza() 
{
    stop_midi(); 
	clear_keybuf();
	remove_keyboard(); 
	remove_mouse(); 
	remove_timer();
	allegro_exit();
	/* add other deinitializations here */
}

void entrada()
{
      BITMAP *fundo,*buffer; 
      MIDI	*musica; 
      SAMPLE *voz,/***musicaee*/; 
      
      
      
      
      int i=0,m=0,e=0;//variaveis que pega a posicao do mause
     
       install_keyboard();
       fundo = create_bitmap(MAX_X,MAX_Y);
       buffer = create_bitmap(MAX_X,MAX_Y);
       musica = load_midi("musicas/Metroid Prime 3 Corruption Title.mid");
       play_midi(musica, TRUE);//inicia musica
      //musicaee = load_wav("musicas/tema.wav");
     // play_sample(musicaee,250,130,1000,0);
     
     
       //ENTRADA SEQUENCIAS//////////
      i=1;
      e=-400;
      do
      {
                  if(i==1)
                  { 
                      fundo = load_gif("fundos/inicio1.gif", NULL); 
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(800);
            
                  }    
                   else
                    if(i==2)
                    {  
                      fundo = load_gif("fundos/inicio2.gif", NULL); 
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(800);
               
                    }  
                   else
                    if(i==3)
                    {  
                      fundo = load_gif("fundos/inicio3.gif", NULL); 
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(800);
                
                    }  
                    else
                    if(i==4)
                    {  
                      fundo = load_gif("fundos/inicio4.gif", NULL); 
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(900);
                     
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
                      fundo = load_gif("fundos/inicio8.gif", NULL);  
                      draw_sprite(screen,fundo,330,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(850);
                      
                      voz = load_wav("musicas/entradav1.wav");
                      play_sample(voz,200,130,1000,0);
                  
                   } 
                    else
                    if(i==7)
                    {  
                      fundo = load_gif("fundos/inicio9.gif", NULL);  
                      draw_sprite(screen,fundo,330,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(550);
                    
                    }
                   else
                    if(i==8)
                    {  
                      clear(screen);      
                      fundo = load_gif("fundos/inicio10.gif", NULL);  
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(550);
                  
                      voz = load_wav("musicas/entradav2.wav");
                      play_sample(voz,200,130,1000,0);
                    } 
                     else
                    if(i==9)
                    {      
                      fundo = load_gif("fundos/inicio11.gif", NULL);   
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(550);
                   
                    }
                    else
                   if(i==10)
                   {   
                      fundo = load_gif("fundos/inicio12.gif", NULL);  
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(550);
                      voz = load_wav("musicas/entradav2.wav");
                      play_sample(voz,200,130,1000,0);
                  
                   }
                   else
                   if(i==11)
                   {   
                      fundo = load_gif("fundos/inicio13.gif", NULL);  
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(800);
                    
                   }   

                   else
                   if(i==12)
                   {   
                      clear(buffer);
                      voz = load_wav("musicas/eletro.wav");
                      play_sample(voz,200,130,1000,0);
                      
                      fundo = load_gif("fundos/inicio15.gif", NULL);  
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(400);
                  
                   } 
                     else
                    if(i==13)
                    {  
                      fundo = load_gif("fundos/inicio16.gif", NULL);  
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(400);
                 
                    }
                     else
                    if(i==14)
                    {  
                      fundo = load_gif("fundos/inicio17.gif", NULL);  
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(400);
                  
                    }
                    else
                    if(i==15)
                    {  
                      fundo = load_gif("fundos/inicio18.gif", NULL);  
                      draw_sprite(screen,fundo,0,0);//desenha na tela a nova imagem
                      destroy_bitmap(fundo);
                      rest(400);
                  
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
   //   destroy_sample(musicaee);
      
}
int telainicial()
{
      BITMAP *cursory,*buffery,*fundoy,*metroid1ee,*metroid2ee; 
      SAMPLE *tema;
      DATAFILE *datFontes = NULL;
      datFontes = load_datafile("fonthistoria.dat");
       
      int x=0,y=0,opcao=0,i=0,m=0,e=0,fimdejogo=0;//variaveis que pega a posicao do mause

      //free (frames);
      //free (durations);
      stop_midi();
      install_keyboard();
      buffery = create_bitmap(MAX_X,MAX_Y);
      fundoy = create_bitmap(MAX_X,MAX_Y);
      
      fundoy = load_bitmap("fundos/telainicial.gif", NULL); 
      tema = load_wav("musicas/temadojogo.wav");
      metroid1ee = load_gif("objetos/BASEESPACIAL/metroid1m.gif", NULL);
      metroid2ee = load_gif("objetos/BASEESPACIAL/metroid2m.gif", NULL); 
      
      play_sample(tema,250,130,1000,1);
      stretch_sprite(buffery,fundoy, -300, -15, 1400,1900);
      draw_sprite(screen,buffery,0,0);
      
      rest(3000);
      
      ////////////////////////////
      remove_keyboard();
      install_keyboard();
      
      
     // show_mouse(fundo);//coloca mouse no bitmap fundo
      do
      { 
           show_mouse(NULL);//tira o mouse para desenhar a nova imagem no buffer2                     
           clear(buffery);//limpa variavel auxiliar buffer   
           draw_sprite(buffery,fundoy,0,0);//copia o bitmap fundo no auxiliar bitmap buffer  
              
           x=mouse_x;
           y=mouse_y;
           
        
           install_int(IncrementaT, 1000);
           if(tempo==0)
           {
             draw_sprite(buffery,metroid1ee,20,10);
             draw_sprite(buffery,metroid1ee,655,10);
           }  
           else
            {
              draw_sprite(buffery,metroid2ee,20,10); 
              draw_sprite(buffery,metroid2ee,655,10);
            } 
            
            
            ///////////////////////CREDITOS DO JOGO////////////////////////////////////////////////////
            install_int(IncrementaT3, 8000);//mostra creditos do jogo
            if(tempo3==1)
            {
               textprintf_ex(buffery,(FONT *)datFontes[0].dat,10,530,makecol(255,0,0),-1,"%s","JOGO BASEADO EM METROID PRODUZIDO POR");
               textprintf_ex(buffery,(FONT *)datFontes[0].dat,10,560,makecol(255,0,0),-1,"%s","          NINTENDO EM 1996");          
            } 
            else
             if(tempo3==2)
             {
               textprintf_ex(buffery,(FONT *)datFontes[0].dat,10,530,makecol(255,0,0),-1,"%s","     PROGRAMAS UTILIZADOS:");
               textprintf_ex(buffery,(FONT *)datFontes[0].dat,10,560,makecol(255,0,0),-1,"%s", "PHOTOSHOP CS3 E SOUND FORGE 10");
             }  
            else
             if(tempo3==3)
             {
                textprintf_ex(buffery,(FONT *)datFontes[0].dat,10,530,makecol(255,0,0),-1,"%s", "ALGUNS SPRITES SÃO ORIGINAIS");
                textprintf_ex(buffery,(FONT *)datFontes[0].dat,10,560,makecol(255,0,0),-1,"%s", "DA SÉRIE METROID DA NINTENDO");          
             } 
            else
             if(tempo3==4)
             {
                textprintf_ex(buffery,(FONT *)datFontes[0].dat,80,530,makecol(255,0,0),-1,"%s", "HISTÓRIA E JOGO FEITO EM LINGUAGEM C + ALLEGRO");
                textprintf_ex(buffery,(FONT *)datFontes[0].dat,80,560,makecol(255,0,0),-1,"%s","          POR TIAGO PORTELA(2010)");           
            } 
            /////////////////////////////////////////////////////////////////////////////////////////////
                    
           if(m==0)
           {
             textout_centre_ex(buffery, font, "PRESSIONE ENTER",400,180,makecol(255,0,0), makecol(0, 0, 0));
             m=1;
           }
           else
             {
               textout_centre_ex(buffery, font, "PRESSIONE ENTER",400,180,makecol(255,255,0), makecol(0, 0, 0));
               m=0;   
             }  
     
             if (key[KEY_F1]) 
             { 
               if (is_windowed_mode())     
                   set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
               else            
                   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
             }
             
            //vsync();
           draw_sprite(screen,buffery,0,0);//desenha na tela a nova imagem 
      }while (!key[KEY_ENTER]);
      
      stop_midi(); 
      remove_keyboard();
      install_keyboard();
     // destroy_sample(tema);

     // destroy_bitmap(metroid1ee);
      destroy_bitmap(metroid2ee);
      
      fimdejogo=menuprincipal(tema,fundoy,buffery,cursory);
      if(fimdejogo==1)//verifica se o jogador colocou sair no menuprincipal para sair do jogo
       return 1;
      else
       return 0; 
      //destroy_sample(vozy);
     
      
}

int menuprincipal(SAMPLE *tema,BITMAP *fundom,BITMAP *bufferm,BITMAP *cursory)
{
      BITMAP *cursormm,*cursormm2; 
      SAMPLE *cursorm;
      BITMAP *auxm;
      DATAFILE *datFontes = NULL;
      datFontes = load_datafile("fonthistoria.dat");
      salvamento SALVOx;
      FILE *arqSALVO;
       
      int x=0,y=0,opcao=0,i=0,m=0,e=0,op=0,op2=0,op3=0;//variaveis que pega a posicao do mause
     
      install_keyboard();
      bufferm = create_bitmap(MAX_X,MAX_Y);
      auxm = create_bitmap(MAX_X,MAX_Y);
      cursormm = load_gif("objetos//BASEESPACIAL/metroidkid1.gif",NULL);
      cursormm2 = load_gif("objetos/BASEESPACIAL/metroidkid2.gif",NULL);
      remove_keyboard();
      install_keyboard();
      
    //  stop_midi(); 
     
      cursorm = load_wav("musicas/Click.wav");
      clear(screen);
      
      set_volume(150, 150);
      
      for(e=0;e<400;e++)
      {     
         e++;   
         e++;
         clear(bufferm); 
         draw_sprite(bufferm,fundom,e,0);//desenha na tela a nova imagem                                 
         draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
        // rest(2); 
      }   
      
      rect(bufferm, 1, 50, 390, 598, makecol(0,150,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 2, 51, 391, 599, makecol(0,0,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 3, 52, 392, 590, makecol(0,130,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 4, 53, 393, 591, makecol(0,120,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 5, 54, 394, 592, makecol(0,110,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 6, 55, 395, 593, makecol(0,50,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 7, 56, 396, 594, makecol(0,60,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 8, 57, 397, 595, makecol(0,70,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 9, 58, 398, 596, makecol(0,80,255));
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      rest(150);
      rect(bufferm, 10, 59, 399, 597, makecol(0,100,255));
      
      draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
      draw_sprite(auxm,bufferm,0,0);//desenha na tela a nova imagem
      clear(screen);
      
      clear(bufferm);
      i=115;
      
      
      
          
      
      /////////////////////////////////////////////////////////////////////////////////////////
      install_int(IncrementaT, 700);
      do
      {    
            //clear(buffer);                        
            draw_sprite(bufferm,fundom,400,0);//desenha na tela a nova imagem 
            draw_sprite(bufferm,auxm,0,0);//desenha na tela a nova imagem                  
            textprintf_ex(bufferm,(FONT *)datFontes[0].dat,120,20,makecol(255,255,0),-1,"%s","SAMUS DATA");
            
            /////////////////////CARREGANDO SALVES SE EXISTIR NO ARQUIVO///////////////////////////////
            if(arqSALVO = fopen("salves/salve1.dat","rb"))
            {
              fread(&SALVOx,sizeof(SALVOx),1,arqSALVO); 
              op=1; //tem salve
              textprintf_centre_ex(bufferm, font,210,130, makecol(80, 255,150), makecol(0,0,0),"  %s    %s  ENERGIA:%d","SAMUS A",SALVOx.localdejogo,SALVOx.energiax); 
              fclose(arqSALVO); 
            }     
            else
              textprintf_centre_ex(bufferm, font,151,130, makecol(255, 255, 255), makecol(0,0,0),"  %s    %s","SAMUS A"," NO DATA "); 
            
            if(arqSALVO = fopen("salves/salve2.dat","rb"))
            {
              fread(&SALVOx,sizeof(SALVOx),1,arqSALVO); 
              op2=1; //tem salve 
              textprintf_centre_ex(bufferm, font,210,200, makecol(80, 255,150), makecol(0,0,0),"  %s    %s  ENERGIA:%d","SAMUS A",SALVOx.localdejogo,SALVOx.energiax); 
              fclose(arqSALVO); 
            }     
            else  
             textprintf_centre_ex(bufferm, font,151,200, makecol(255, 255, 255), makecol(0,0,0),"  %s    %s","SAMUS A"," NO DATA "); 
             
             if(arqSALVO = fopen("salves/salve3.dat","rb"))
             {
               //rewind(arqSALVO);          
               fread(&SALVOx,sizeof(SALVOx),1,arqSALVO);  
               op3=1; //tem salve
               textprintf_centre_ex(bufferm, font,210,270, makecol(80, 255,150), makecol(0,0,0),"  %s    %s  ENERGIA:%d","SAMUS A",SALVOx.localdejogo,SALVOx.energiax); 
               fclose(arqSALVO); 
             }     
             else
              textprintf_centre_ex(bufferm, font,151,270, makecol(255, 255, 255), makecol(0,0,0),"  %s    %s","SAMUS A"," NO DATA ");
                          
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            textprintf_centre_ex(bufferm, font,120,350, makecol(255, 255, 255), makecol(0,0,0),"  %s","LIMPAR SALVE"); 
            textprintf_centre_ex(bufferm, font,95,420, makecol(255, 255, 255), makecol(0,0,0),"  %s","OPÇÕES"); 
            textprintf_centre_ex(bufferm, font,118,490, makecol(255, 255, 255), makecol(0,0,0),"  %s","SAIR DO JOGO");
            
            if(key[KEY_W])
            {            
              if(i>115)            
              i=i-70; 
              play_sample(cursorm,250,130,1000,0);
              
              remove_keyboard();
              install_keyboard();             
            }
            else
             if(key[KEY_S])
             {
               if(i<420)            
                i=i+70;  
                play_sample(cursorm,250,130,1000,0); 
                
                remove_keyboard();
                install_keyboard();           
             }
              if(tempo==0)
                 draw_sprite(bufferm,cursormm,20,i);
               else
                   draw_sprite(bufferm,cursormm2,20,i);
                   
             textprintf_centre_ex(bufferm, font,50,410, makecol(0, 0, 0), makecol(255, 255, 255)," I :%d",i);
             draw_sprite(screen,bufferm,0,0);//desenha na tela a nova imagem
             
       }while (!key[KEY_ENTER]);  
       
       cursorm = load_wav("musicas/go.wav");
       play_sample(cursorm,200,130,1000,0);
       rest(1500);
       
       stop_sample(cursorm);
       stop_midi();
       
       destroy_bitmap(auxm);
      // destroy_midi(musicaz); 
       remove_keyboard();
       destroy_sample(cursorm);
       destroy_sample(tema);
       
       destroy_bitmap(fundom);
      // destroy_bitmap(bufferm);
    //   destroy_bitmap(cursory);
       destroy_bitmap(cursormm);
       destroy_bitmap(cursormm2);
       
       ////////////OPCOES DO MENUPRINCIAL;////////////////
       if(i==115)
       {
          SALVEJOGO=1;
          if(op==1)
           auxsalve=1;
       } 
       else
         if(i==185)
         {
           SALVEJOGO=2;
           if(op2==1)
            auxsalve=1;
         } 
         else
          if(i==255)
          {
            SALVEJOGO=3;
            if(op3==1)
              auxsalve=1;
          } 
        /*  else
           if(i==325)
             SALVEJOGO=1000;///exclui salve
           else
            if(i==395)/////opcoes
             SALVEJOGO=2000;    
            else */
               if(i==465)//se I == 465 quer dizer que o usuario optou pela opcao sair do jogo no menuprincipal
                return 1;
               else
                 return 0; 
       ///////////////////////////////////////////////  
}



void historia()
{  
      MIDI	*musica; 
      SAMPLE *musica2,*nave;
      BITMAP *buffer,*fundo,*x,*aux,*rocha1,*rocha2;
      DATAFILE *datFontes = NULL;
      datFontes = load_datafile("fonthistoria.dat");
      rocha1 = load_gif("objetos/BASEESPACIAL/rocha.gif", NULL);
      rocha2 = load_gif("objetos/BASEESPACIAL/rocha2.gif", NULL);
      
      int i,i2=0,e,c,corta=0;
      
      
      buffer = create_bitmap(MAX_X,MAX_Y);
      fundo = create_bitmap(MAX_X,MAX_Y);
      aux = create_bitmap(MAX_X,MAX_Y);
      musica = load_midi("musicas/historia.mid");
      musica2 = load_wav("musicas/Click.wav");
      nave = load_wav("musicas/nave.wav");                               
     // play_sample(musica2,255,90,1000,0);
      
     // cursor = load_png("fundos/a.png",NULL);
      play_midi(musica, TRUE);//inicia musica
     
              clear(screen);
             // rest(2500);
      c=1;     
      
      fundo = load_gif("fundos/introg1.gif", NULL);
      ///verifica se vai cortar cena
      i=0;
      remove_keyboard();
      install_keyboard();
      do
      {
          clear(buffer);         
          if(i>-750)
          {         
             stretch_sprite(buffer,fundo, i,-150, 1800,1000); 
             rest(12);
          } 
          else
            stretch_sprite(buffer,fundo, -750,-150, 1800,1000); 

           i--;  
             
          textprintf_ex(buffer,(FONT *)datFontes[0].dat,510,5,makecol(0,255,0),-1,"%s","PRESSIONE ESC  PARA");
          textprintf_ex(buffer,(FONT *)datFontes[0].dat,525,35,makecol(255,255,0),-1,"%s","(PULAR INTRODUÇÃO)");
          draw_sprite(screen,buffer,0,0);
         
      }while(!key[KEY_ESC] && i>-1500);
      
      //rest(1000);
      if(key[KEY_ESC])
       corta=1;
      //////////////////////////////////////////////////
      destroy_bitmap(fundo);
      fundo =load_gif("fundos/introg1.gif", NULL); 
      if(corta==0)
      {   
                  do
                  {      
                      /* if(c==1)
                       {     
                              for(i=0;i>-750;i--)
                              {       
                                  clear(buffer);      
                                  stretch_sprite(buffer,fundo, i,-150, 1800,1000);
                                  draw_sprite(screen,buffer,0,0);
                                  rest(12);
            
                              }  
                           //   rest(2000);
                       } 
                       else*/
                         if(c==2)
                         {    
                                  destroy_bitmap(fundo);
                                   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                  fundo = load_gif("fundos/introg2.gif", NULL);
                                 
                                 // musica2 = load_wav("musicas/introg2.wav");
                               //   play_sample(musica2,200,130,1000,0);
                                  draw_sprite(buffer,fundo,0,0);//desenha na tela a nova imagem 
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,330,makecol(255,255,0),-1,"%s","Samus uma órfã criada pelos Chozos, uma raça alienígena extinta. ");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,365,makecol(255,255,0),-1,"%s","Foram os Chozos que treinaram e equiparam Samus com ");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,405,makecol(255,255,0),-1,"%s","uma armadura de combate que desafia a Física, fazendo dela a ");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,440,makecol(255,255,0),-1,"%s","garota a quem a Federação Galáctica recorre para missões ");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,475,makecol(255,255,0),-1,"%s","na escuridão.....");
                                  //draw_trans_sprite(buffer,cursor,20,20);
                                  
                                  draw_sprite(aux,buffer,0,0);
                                
                         
                                  x = load_gif("fundos/intro.gif", NULL);
                                  for(i=400;i>15;i--)
                                  {       
                                      clear(aux);      
                                      draw_sprite(aux,buffer,0,0);
                                      draw_sprite(aux,x,i,50);
                                      draw_sprite(screen,aux,0,0);
                                      rest(12);
             
                                  }  
                              
                                  destroy_bitmap(x);
                                  
                            
                                      x = load_gif("fundos/intro2.gif", NULL);
                                      for(i=0;i<400;i++)
                                      {       
                                          clear(aux);      
                                          draw_sprite(aux,buffer,0,0);
                                          draw_sprite(aux,x,i,50);
                                          draw_sprite(screen,aux,0,0);
                                          rest(12);
            
                                      } 
                                      destroy_bitmap(x);
                                    
                            
                                          x = load_gif("fundos/intro3.gif", NULL);
                                          for(i=0;i<420;i++)
                                          {       
                                              clear(aux);      
                                              draw_sprite(aux,buffer,0,0);
                                              draw_sprite(aux,x,i,50);
                                              draw_sprite(screen,aux,0,0);
                                              rest(12);
               
                                          } 
                                          draw_sprite(buffer,aux,0,0);//deixando imagem na tela
                                          
                                          destroy_bitmap(x);
                                      
                                          draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                          play_sample(musica2,200,130,1000,0);
                                          do
                                          {
                                            textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,550,makecol(0,255,0),-1,"%s","Pressione Enter(PARA CONTINUAR)");                                       
                                            draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem                                       
                                          }while(!key[KEY_ENTER]);
                                          remove_keyboard();
                                          install_keyboard();
                               }
                               else
                                 if(c==3)
                                 {
                                      clear(screen);
                                      fundo = load_gif("fundos/introg3.gif", NULL);
                                      for(i=0;i>-750;i--)
                                      {       
                                          clear(buffer);      
                                          stretch_sprite(buffer,fundo, i,-150, 1800,1000);
                                          draw_sprite(screen,buffer,0,0);
                                          rest(12);
            
                                      }  
                                      rest(1000);
                                      destroy_bitmap(fundo);
                                    
                                 
                           
                                       ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                      fundo = load_gif("fundos/introg2.gif", NULL);
                                     // musica2 = load_wav("musicas/introg3.wav");
                                   //   play_sample(musica2,200,130,1000,0);
                                      draw_sprite(buffer,fundo,0,0);//desenha na tela a nova imagem 
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,430,makecol(255,255,0),-1,"%s","Solitária nos confins do universo, Samus desejava mais que  ");
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,465,makecol(255,255,0),-1,"%s","fazer um favor. Isso deu a ela excelentes oportunidades ");
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,505,makecol(255,255,0),-1,"%s","de exigir com artilharia pesada sua vingança sobre os ");
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,540,makecol(255,255,0),-1,"%s","piratas que assassinaram sua família. ");
                          
                    
                                      clear(screen);
                                      x = load_gif("fundos/introx.gif", NULL);
                                      for(i=10;i<350;i++)
                                      {       
                                          clear(aux);      
                                          draw_sprite(aux,buffer,0,0);
                                          draw_sprite(aux,x,i,10);
                                          draw_sprite(screen,aux,0,0);
                                          rest(30);
            
                                      }  
                                      destroy_bitmap(x);
                                      draw_sprite(buffer,aux,0,0);//deixando imagem na tela
                                      
                                      play_sample(musica2,200,130,1000,0);
                                      do
                                      {
                                            textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,380,makecol(0,255,0),-1,"%s","Pressione Enter(PARA CONTINUAR)");                                       
                                            draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem                                       
                                       }while(!key[KEY_ENTER]);
                                       remove_keyboard();
                                          install_keyboard();
                                          destroy_bitmap(fundo);
                              }
                              else
                              if(c==4)
                              {
                                  clear(screen);
                                  fundo = load_gif("fundos/introg4.gif", NULL);
                                  for(i=0;i>-600;i--)
                                  {       
                                      clear(buffer);      
                                      stretch_sprite(buffer,fundo, i,-150, 2000,900);
                                      draw_sprite(screen,buffer,0,0);
                                      rest(12);
                         
                                  }  
                                  rest(1000);
                                  destroy_bitmap(fundo);
                      
                                  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                  
                                  fundo = load_gif("fundos/introg2.gif", NULL);
                                 // musica2 = load_wav("musicas/introg4.wav");
                                 // play_sample(musica2,200,130,1000,0);
                                  draw_sprite(buffer,fundo,0,0);//desenha na tela a nova imagem 
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,330,makecol(255,255,0),-1,"%s","Samus Aran em sua ultima missão, apareceu na superfície do  ");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,365,makecol(255,255,0),-1,"%s","planeta Zebes com objetivo de eliminar o líder pirata ");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,405,makecol(255,255,0),-1,"%s","Mother Brain, destruir toda a pesquisa sobre uma criatura ");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,440,makecol(255,255,0),-1,"%s","parasita criada pelos Chozos e encontrar o Metroid capturado");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,475,makecol(255,255,0),-1,"%s","da estação espacial por um cruel dragão pirata chamado Ridley.");
                          
                          
                                  clear(screen);
                                  x = load_gif("fundos/intro3x.gif", NULL);
                                  for(i=-700;i<0;i++)
                                  {       
                                      clear(aux);      
                                      draw_sprite(aux,buffer,0,0);
                                      draw_sprite(aux,x,i,10);
                                      draw_sprite(screen,aux,0,0);
                                      rest(30);
            
                                  }  
                                  destroy_bitmap(x);
                                  draw_sprite(buffer,aux,0,0);//deixando imagem na tela
                                  
                                  play_sample(musica2,200,130,1000,0);
                                  do
                                  {
                                     textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,550,makecol(0,255,0),-1,"%s","Pressione Enter(PARA CONTINUAR)");                                       
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem                                       
                                  }while(!key[KEY_ENTER]);
                                  remove_keyboard();
                                          install_keyboard();
                              }
                              else
                              if(c==5)
                              {
                                  clear(screen);
                                  fundo = load_gif("fundos/introg5.gif", NULL);
                                  for(i=0;i>-700;i--)
                                  {       
                                      clear(buffer);      
                                      stretch_sprite(buffer,fundo, i,-50, 1800,700);
                                      draw_sprite(screen,buffer,0,0);
                                      rest(12);
                                      
            
                                  }  
                                  rest(1000);
                                  destroy_bitmap(fundo);
                            
                                       ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                      fundo = load_gif("fundos/introg2.gif", NULL);
                                    //  musica2 = load_wav("musicas/introg5.wav");
                                    //  play_sample(musica2,200,130,1000,0);
                                      draw_sprite(buffer,fundo,0,0);//desenha na tela a nova imagem 
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,360,makecol(255,255,0),-1,"%s","Samus no meio de sua batalha com Mother Brain 2.0, um titã ");
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,395,makecol(255,255,0),-1,"%s","cibernético que facilmente a encurralou deixando sua armadura,");
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,435,makecol(255,255,0),-1,"%s","totalmente danificada. Mas logo antes que Mother Brain 2.0");
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,465,makecol(255,255,0),-1,"%s","pudesse dar o golpe de misericórdia, o Metroid capturado da ");
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,495,makecol(255,255,0),-1,"%s","estação espacial, a salvou. No entando isso custou à heróica");
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,525,makecol(255,255,0),-1,"%s","água-viva sua vida. ");
                                      //draw_trans_sprite(buffer,cursor,20,20);
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      
                                      clear(screen);
                                      x = load_gif("fundos/intromx.gif", NULL);
                                      for(i=400;i>15;i--)
                                      {       
                                          clear(aux);      
                                          draw_sprite(aux,buffer,0,0);
                                          draw_sprite(aux,x,i,20);
                                          draw_sprite(screen,aux,0,0);
                                          rest(20);
                 
                                      }  
                                      destroy_bitmap(x);
                                      draw_sprite(buffer,aux,0,0);//deixando imagem na tela
                                      
                                      play_sample(musica2,200,130,1000,0);
                                      do
                                      {
                                            textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,575,makecol(0,255,0),-1,"%s","Pressione Enter(PARA CONTINUAR)");                                       
                                            draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem                                       
                                      }while(!key[KEY_ENTER]);
                                      remove_keyboard();
                                          install_keyboard();
                                          destroy_bitmap(fundo);
                                          
                            }
                            else
                            if(c==6)
                            {
                                  
                                  clear(screen);
                                  fundo = load_gif("fundos/introg6.gif", NULL);
                                  
            
                                   
                                 // musica2 = load_wav("musicas/introg6.wav");
                                 // play_sample(musica2,200,130,1000,0);
                                  
                                  draw_sprite(buffer,fundo,0,0);//desenha na tela a nova imagem 
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,030,makecol(255,255,0),-1,"%s","Após derrotar Mother Brain 2.0, o planeta Zebes entra em ");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,065,makecol(255,255,0),-1,"%s","autodestruição e Samus imediatamente foge do planeta.");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,90,makecol(255,255,0),-1,"%s","A missão foi um sucesso porém custou lhe sua armadura....");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,110,makecol(255,255,0),-1,"%s",".......");
                                  draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                  rest(5000);
                                  do
                                  {
                                    textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,550,makecol(0,255,0),-1,"%s","Pressione Enter(PARA CONTINUAR)");                                       
                                    draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem                                       
                                  }while(!key[KEY_ENTER]);
                                  
                                  destroy_bitmap(fundo);
                                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
                            }    
                            else
                            if(c==7)
                            {
                                  clear(screen);
                                  fundo = load_gif("fundos/introg7.gif", NULL);
                                  for(i=-405;i<0;i++)
                                  {       
                                      clear(buffer);      
                                      draw_sprite(buffer,fundo,0,i);
                                      draw_sprite(screen,buffer,0,0);
                                      rest(15);
            
                                  }  
                                  
                                //  musica2 = load_wav("musicas/introg7.wav");
                                 // play_sample(musica2,200,130,1000,0);
                                  
                                  draw_sprite(buffer,fundo,0,0);//desenha na tela a nova imagem 
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,30,makecol(255,255,20),-1,"%s","  Ao sair do planeta zebes que estava em auto-destruição");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,60,makecol(255,255,20),-1,"%s","Samus remove sua armadura danificada e retorna para sua base");
                                  textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,90,makecol(255,255,20),-1,"%s",".......");
                                  draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                  rest(4000);
                                  
                                  play_sample(musica2,200,130,1000,0);
                                  do
                                  {
                                     textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,550,makecol(0,255,0),-1,"%s","Pressione Enter(PARA CONTINUAR)");                                       
                                     draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem                                       
                                  }while(!key[KEY_ENTER]);
                                  remove_keyboard();
                                  install_keyboard();
            
                          
                                      stop_midi();
                                      stop_sample(musica2);
                                    //  musica2 = load_wav("musicas/Click.wav");
                                      play_sample(musica2,200,130,1000,0);
                                      
                                      
                                      rest(400);
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,200,makecol(255,255,0),-1,"%s",".");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,235,makecol(255,255,0),-1,"%s","...");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,270,makecol(255,255,0),-1,"%s","......");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,305,makecol(255,255,0),-1,"%s","..........");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,340,makecol(255,255,0),-1,"%s","................");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,375,makecol(255,255,0),-1,"%s","................... ");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,410,makecol(255,255,0),-1,"%s","............................. ");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,445,makecol(255,255,0),-1,"%s","...........................................");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,480,makecol(255,255,0),-1,"%s","...................................................");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,20,515,makecol(255,255,0),-1,"%s","..........................................................");
                                      draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem
                                      play_sample(musica2,255,128,1000,0);
                                      rest(400);
                                      destroy_bitmap(fundo);
                            }
                            else
                            if(c==8)
                            {
                          
                                  clear(screen);
                                  musica = load_midi("musicas/colonia.mid");
                                  play_midi(musica, TRUE);//inicia musica
                                  musica2 = load_wav("musicas/baseespacial.wav");
                                  play_sample(musica2,200,130,1000,0);
                                  
                                  fundo = load_gif("fundos/coloniaespacial.gif", NULL);
                                  x = load_gif("objetos/BASEESPACIAL/navex.gif", NULL);
                                  for(i=0;i>-350;i--)
                                  {     
                                      if(i%2==0)    
                                       i2++;    
                                      clear(buffer);      
                                      draw_sprite(buffer,fundo,i,0);
                                      rotate_sprite(buffer,rocha2, i2-90, 200, itofix(i2));
                                      rotate_sprite(buffer,rocha2, i2-90, 400, itofix(i2));
                                      rotate_sprite(buffer,rocha1, i2-70, 50, itofix(i2)); 
                                      rotate_sprite(buffer,rocha2, i2+50, 300, itofix(i2)); 
                                      rotate_sprite(buffer,rocha1, i2+50, 400, itofix(i2)); 
                                      rotate_sprite(buffer,rocha2, i2+250, 500, itofix(i2)); 
                                      rotate_sprite(buffer,rocha1, i2, 200, itofix(i2)); 
                                      rotate_sprite(buffer,rocha2, i2+380, 550, itofix(i2)); 
                                      rotate_sprite(buffer,rocha1, i2+220, 230, itofix(i2)); 
                                      rotate_sprite(buffer,rocha2, i2+340, 380, itofix(i2));
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,10,550,makecol(255,255,0),-1,"%s","BASE ESPACIAL");
                                      draw_sprite(screen,buffer,0,0);
                                      rest(20);
            
                                  }  
                                  
                                 // rest(1000);
                                                                 
                                  for(i=0;i<300;i++)
                                  {       
                                      clear(buffer);      
                                      draw_sprite(buffer,fundo,-350,0);
                                      if(i%2==0)    
                                       i2++;  
                      
                                      if(i==10)
                                       play_sample(nave,220,130,1000,0);
                                       
                                      draw_sprite(buffer,x,i,570-i);
                                      
                                      rotate_sprite(buffer,rocha2, i2-90, 200, itofix(i2));
                                      rotate_sprite(buffer,rocha2, i2-90, 400, itofix(i2));
                                      rotate_sprite(buffer,rocha1, i2-70, 50, itofix(i2)); 
                                      rotate_sprite(buffer,rocha2, i2+50, 300, itofix(i2)); 
                                      rotate_sprite(buffer,rocha1, i2+50, 400, itofix(i2)); 
                                      rotate_sprite(buffer,rocha2, i2+250, 500, itofix(i2)); 
                                      rotate_sprite(buffer,rocha1, i2, 200, itofix(i2)); 
                                      rotate_sprite(buffer,rocha2, i2+380, 550, itofix(i2)); 
                                      rotate_sprite(buffer,rocha1, i2+220, 230, itofix(i2)); 
                                      rotate_sprite(buffer,rocha2, i2+340, 380, itofix(i2));
                                                                          
                                      textprintf_ex(buffer,(FONT *)datFontes[0].dat,10,550,makecol(255,255,0),-1,"%s","BASE ESPACIAL");
                                      draw_sprite(screen,buffer,0,0);
                                      rest(15);
            
                                  }  
                                  rest(2000);
                                  destroy_bitmap(fundo);
                                  destroy_bitmap(x);
                          }       
                          
                          c++;//incrementa cena
                 }while (corta==0 && c<9);
      }
    // if(corta==0)
    // { 
            destroy_bitmap(buffer);
        //  destroy_bitmap(cursor);
          destroy_bitmap(aux);
           // destroy_bitmap(fundo);
          
            destroy_midi(musica); 
          destroy_sample(musica2);
          destroy_sample(nave);
     /* }
      else
        {
          destroy_bitmap(buffer);   
          destroy_midi(musica);  
          destroy_bitmap(aux);
          //destroy_bitmap(fundo);               
        }*/
}

void jogo()
{
          int estado=1,contador=1,musica=0,caux=0; 
          SAMPLE *musica2,*musicaxx,*musica3;
          salvamento SALVOx;
          FILE *arqSALVO;
          MIDI* musicaxxxx;
          char aux[25];///usado para dizer qual arquivo vai ser lido do salve ou verificar se existe
        
          int //fps=60,frames=0,//controla velocidade do jogo
          i,
          musicax=0,
          musicax2=0,
          salvado=0,///passa por parametro dizendo se ta começando apos carregar o salve
          ativacena=0,//controla cena da nave pirata no cenario 111 apos pegar armadura e andar pra sair,diz se a cena ja foi mostrada
          ativa=0,//variavel que informa se elevador foi ativado ou nao no cenario 18
          roupa=0,//variavel que passa para a funcao que passa para funcao movimento dizendo se ta sem armadura ou nao ou com qual
          cenacapitao=-1,//variavel que diz se samus ja foi falar com capitao,dae libera para ela ir pegar armadura apos explosao
          cenamonitor=0,//controla que imagem e cena deve acontecer no cenario 10
          somdeexplosao=0;//som de explosao ao subir elevador apos falar com capitao
          
          musica2 = load_wav("musicas/cenario2d.wav");
          musicaxx = load_wav("musicas/Escape.wav"); 
          musicaxxxx = load_midi("musicas/escape.mid");
          
        
          /////////////////////CARREGANDO SALVE SE EXISTIR/////////////
             //verificando qual salve carregar que o menu principal jogou na variavel global
              if(SALVEJOGO==1)
               strcpy(aux,"salves/salve1.dat");
              else 
                if(SALVEJOGO==2)
                 strcpy(aux,"salves/salve2.dat");
                else
                  if(SALVEJOGO==3)
                   strcpy(aux,"salves/salve3.dat");
          ///////////////////////////
          
          if(arqSALVO = fopen(aux,"rb"))
          {     
              fread(&SALVOx,sizeof(SALVOx),1,arqSALVO);  
               
              ///PEGANDO JOGO SALVO E JOGANDO NAS VARIAVEIS DE CONTROLE////////
              ativacena=SALVOx.ativacenax;//controla cena da nave pirata no cenario 111 apos pegar armadura e andar pra sair,diz se a cena ja foi mostrada
              ativa=SALVOx.ativax;//variavel que informa se elevador foi ativado ou nao no cenario 18
              roupa=SALVOx.roupax;//variavel que passa para a funcao que passa para funcao movimento dizendo se ta sem armadura ou nao ou com qual
              cenacapitao=SALVOx.cenacapitaox;//variavel que diz se samus ja foi falar com capitao,dae libera para ela ir pegar armadura apos explosao
              cenamonitor=SALVOx.cenamonitorx;//controla que imagem e cena deve acontecer no cenario 10
              somdeexplosao=SALVOx.somdeexplosaox;//som de explosao ao subir elevador apos falar com capitao
              energia=SALVOx.energiax;//variavel global energia recebe a ultima energia gravada
              ///////////////////////////////////////////////////////////
              contador=11;
              play_sample(musicaxx,230,130,1000,1); 
              salvado=1;
              fclose(arqSALVO); 
          }     
          
          //////////////////////////////////////////////////////////////////
          
              do
              {   
                  if(estado!=4 && estado!=3 && estado!=10 && estado!=2)//se nao tiver chamando a funcao 18 apos voltar da 19 pq a 18 retorna 4 ao voltar
                  {
                      if(estado==1)
                        contador++;
                      else
                        if(estado==0)
                          contador--;
                  }       
                  
                   //else 
                      if(contador==1)
                      {
                            stop_sample(musica2); 
                            stop_sample(musicaxx);   
                            musicax2=0;        
                      } 
                      else
                      if(contador>1 && roupa==0)//controla som dos cenarios
                      {
                        stop_sample(musicaxx);            
                        if(musicax2==0)            
                        {
                           //som = load_midi("musicas/cenario2d.mid");
                           //play_midi(musicaxxxx,TRUE);//inicia musica
                          
                              //stop_midi();         
                           play_sample(musica2,230,130,1000,1);                    
                           musicax2=1;
                        }  
                      } 
                      else
                        {
                         // stop_midi();
                          stop_sample(musica2);                  
                        }
        
        
        
                  if(estado!=-10000)
                  { 
                         
                        if(estado==11)//sala do capitao parte 2 do 112
                          estado=cenario1122(estado,fps,roupa,cenacapitao); //se estado 1 significa ida,0 significa volta  
                        else  
                         if(estado==5)//estado 5 eh o terceiro caminho q o cenario 110 retornou para chamar sala de armadura
                         {  
                            passaporta(0,20,495-30,fps);//passa porta   
                            estado=cenario1102(estado,fps,roupa,musicaxxxx,musica2,ativacena);                
                            
                         }  
                         else
                         if(estado==3)//estado 3 q o cenario 18 retornou
                         {  
                            passaporta(0,20,495-30,fps);//passa porta                   
                            estado=cenario19(fps,ativa,roupa); //se estado 1 significa ida,0 significa volta 
                         }  
                         else            
                          if(contador==1 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)    
                             estado=cenario1(estado,fps,roupa);//se estado 0 significa volta,1 significa ida     
                          else
                            if(contador==2 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)   
                              estado=cenario12(estado,fps,roupa);//se estado 1 significa ida,0 significa volta
                            else
                              if(contador==3 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)
                                estado=cenario13(estado,fps,roupa); //se estado 1 significa ida,0 significa volta 
                              else
                                if(contador==4 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)
                                  estado=cenario14(estado,fps,1,roupa); //se estado 1 significa ida,0 significa volta  
                                else
                                 if(contador==5 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)
                                   estado=cenario15(estado,fps,roupa); //se estado 1 significa ida,0 significa volta 
                                 else
                                  if(contador==6 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)
                                    estado=cenario16(estado,fps,roupa); //se estado 1 significa ida,0 significa volta  
                                  else
                                   if(contador==7 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)
                                     estado=cenario16p2(estado,fps,roupa); //se estado 1 significa ida,0 significa volta
                                   else  
                                     if(contador==8 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=12 && estado!=4 )
                                       estado=cenario17(estado,fps,roupa); //se estado 1 significa ida,0 significa volta      
                                     else
                                       if(contador==9 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=12)//se estado nao for 3 entao chama,senao chama o cenario 19 q eh uma sala apos 18,estado eh 3 pq o 18 retorna 3 dizendo que entrou na sala esquerda e nao no elevador
                                         estado=cenario18(estado,fps,ativa,roupa); //se estado 1 significa ida,0 significa volta                                        
                                         else//elevador
                                          if(contador==10 && estado!=3 && estado!=5 && estado!=11 && estado!=4 && estado!=12)//se estado nao for 3 entao chama,senao chama o cenario 19 q eh uma sala apos 18,estado eh 3 pq o 18 retorna 3 dizendo que entrou na sala esquerda e nao no elevador
                                            estado=cenario14(estado,fps,2,roupa); //se estado 1 significa ida,0 significa volta 
                                          else
                                             if(contador==11 && estado!=3 && estado!=5 && estado!=11 && estado!=4 && estado!=12)//salao com cientistas
                                               estado=cenario110(estado,fps,roupa,cenamonitor,cenacapitao); //se estado 1 significa ida,0 significa volta     
                                             else
                                              if(contador==12 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)//sala       
                                                 estado=cenario111(estado,fps,roupa,somdeexplosao,cenacapitao,ativacena,ativa,cenamonitor,salvado); //se estado 1 significa ida,0 significa volta //se estado 1 significa ida,0 significa    
                                               else//elevador
                                                  if(contador==13 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)// elevador
                                                    estado=cenario14(estado,fps,2,roupa); //se estado 1 significa ida,0 significa volta 
                                                 else
                                                    if(contador==14 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4)//sala entre laboratorio e sala de comando
                                                      estado=cenario112(estado,fps,roupa); //se estado 1 significa ida,0 significa volta
                                                    else
                                                     if(contador==15 && estado!=3 && estado!=5 && estado!=11 && estado!=10 && estado!=4 && estado!=12)//sala entre laboratorio e sala de comando
                                                      estado=cenario113(estado,fps,roupa); //se estado 1 significa ida,0 significa volta  
                                                       
                   }              
                                           
              }                         
              while(contador<16 && estado!=-10000);
           destroy_sample(musica2);
           destroy_sample(musicaxx);
           //destroy_midi(som); 
   
}

int cenario1(int estado,int fps,int roupa)
{
      BITMAP *bufferu,*fundo2u,*fundo1u,*pu,*naveu,*nave2u,*painelu,*fportau; 
      SAMPLE *abreportau,*fechaportau,*pmu;
      SAMPLE *passou,*vozu,*ptu; 
      MIDI *som;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      //fps,
      opcao=0,
      porta=1,//controla se porta fica aberta ou nao
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      xcenario,//controla movimento do cenario
      tiroflag=0,
      vida=energia,
      tiro,
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      loop=0,//controle de escrita no buffer do fps
      auxfps=0,
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz do pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo
      

     // install_keyboard();
      bufferu = create_bitmap(MAX_X,MAX_Y);
      fundo1u = create_bitmap(1600,2000);
      fundo2u = create_bitmap(MAX_X,MAX_Y);
      
      fundo1u = load_gif("fundos/cenario1.gif", NULL); 
      fundo2u = load_gif("fundos/fcenario1.gif", NULL);
      naveu = load_gif("objetos/BASEESPACIAL/nave1.gif",NULL);
      nave2u = load_gif("objetos/BASEESPACIAL/nave2.gif",NULL);
      painelu = load_gif("fundos/painel.gif",NULL);
      fportau = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      
      abreportau = load_wav("musicas/Dooropen.wav");
      fechaportau = load_wav("musicas/Doorclose.wav");
      
      passou = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptu = load_wav("musicas/tirox.wav");
      else
         ptu = load_wav("musicas/tiro.wav");
           
      vozu = load_wav("musicas/vpulo.wav");
      
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
                   
      
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////
               
               for(x=618;x<800;x++)//parede da direita
               {
                 for(y=0;y<440;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               
       
                 /////////  
      ////////////////////////////////////////////////////
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      
      ////////////////////////////
   
     if(estado==1)
     { 
              som = load_midi("musicas/crash.mid");
              play_midi(som,TRUE);//inicia musica
              //nave chegando
              for(i=-100;i>-1220;i--)
              {                       
                 show_mouse(NULL);//tira o mouse para desenhar a nova imagem no buffer2                     
                 clear(bufferu);//limpa variavel auxiliar buffer   
                 draw_sprite(bufferu,fundo2u,0,i); 
                 draw_sprite(bufferu,fundo1u,0,i);
                 if(i%2==0)
                  draw_sprite(bufferu,naveu,100,30);
                 else 
                   draw_sprite(bufferu,nave2u,100,30);
                   
                 draw_sprite(screen,bufferu,0,0);
                 
                 if(fps<20000)
                   rest(8);
                 else 
                   rest(13);
               }  
               for(i=30;i<300;i++)
               {                       
                 show_mouse(NULL);//tira o mouse para desenhar a nova imagem no buffer2                     
                 clear(bufferu);//limpa variavel auxiliar buffer   
                 draw_sprite(bufferu,fundo2u,0,-1220); 
                 draw_sprite(bufferu,fundo1u,0,-1220);
        
                     if(i%2==0)
                      draw_sprite(bufferu,naveu,100,i);
                     else 
                       draw_sprite(bufferu,nave2u,100,i);
                 
                    
                 draw_sprite(screen,bufferu,0,0);
                 rest(13);
               }  
               
               clear(bufferu);
               draw_sprite(bufferu,fundo2u,0,-1220); 
               draw_sprite(bufferu,fundo1u,0,-1220);
               draw_sprite(bufferu,nave2u,100,300);
               pu = load_gif("personagem/a8.gif",NULL);
               draw_sprite(bufferu,pu,245,495);
               draw_sprite(screen,bufferu,0,0);//desenha na tela a nova imagem
               rest(2000);
               
               stop_midi();
               destroy_midi(som);
               
               pmu = load_wav("musicas/samusinicia.wav");
               pu = load_gif("fundos/introjogo1.gif",NULL);
               play_sample(pmu,200,130,1000,0);
               draw_sprite(bufferu,pu,0,0);
               draw_sprite(screen,bufferu,0,0);//desenha na tela a nova imagem
               rest(1800);
               destroy_bitmap(pu);
               
               pu = load_gif("fundos/introjogo2.gif",NULL);
               draw_sprite(bufferu,pu,0,0);
               draw_sprite(screen,bufferu,0,0);//desenha na tela a nova imagem
               rest(1800);
               destroy_bitmap(pu);
               
               clear(bufferu);
               draw_sprite(bufferu,fundo2u,0,-1220); 
               draw_sprite(bufferu,fundo1u,0,-1220);
               draw_sprite(bufferu,nave2u,100,300);
               
               clear(bufferu);
        
        }      
        
      y=495;  
      if(estado==1)//se 0 eh pq ta voltando,se 1 eh pq ta indo,valor passando por funcao jogo 
      {
        x=245;
        xcenario=0;//estado inicial do cenario
      }  
      else
        if(estado==0)
        {
          x=650; 
          xcenario=-800;//estado inicial do cenario
          play_sample(fechaportau,250,130,1000,0);  
        }  
        
       clear(bufferu);
         
       if(estado==1)//se 0 eh pq ta voltando,se 1 eh pq ta indo,valor passando por funcao jogo
       {
          if(roupa==0)               
            strcpy(posicao,"personagem/a8.gif");
          else
            strcpy(posicao,"personagem/ARMADURA/ax8.gif");  
          draw_sprite(bufferu,fundo2u,0,-1220); 
          draw_sprite(bufferu,fundo1u,0,-1220);       
          draw_sprite(bufferu,nave2u,100,300);
       }   
       else
         {
           if(roupa==0)                                   
             strcpy(posicao,"personagem/d1.gif");  
           else
              strcpy(posicao,"personagem/ARMADURA/dx1.gif"); 
         }  
          

       if(estado==1)
         rest(4400);
      
  
         som = load_midi("musicas/musica1.mid");
         play_midi(som,FALSE);//inicia musica
         //show_mouse(fundo);//coloca mouse no bitmap fundo
        i=1;
        
         // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
         install_int( frame_rate, 1000 ); 
        install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );    

       d=-1;//direcao inicial do personagem -1 nenhuma
      do
      {    
            
           
           
          while ( ::fps_speed > 0 )
          { 
                    if(loop==0)
                    { 
                           clear(bufferu);            
                           
                           /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                              if(tirof[0].x-15>695 && porta==1 && tirof[0].y>450)
                              {
                                   porta=0;
                
                                   for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportau,250,130,1000,0);
                              }      
                           
                           if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                             
                             if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                             { 
                               if (is_windowed_mode())     
                                   set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                
                               else            
                                   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                             }
                             
                             if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                             {                                               
                                       movimentos(d,a,x,y,mf,mt,posicao,1,passou,vozu,roupa,lpulo); 
                                    
                                      //MOVENDO CENARIO  PARRA TRAS
                                        if(x>330 && a==0 && matriz[x-1][y]==0)
                                        {
                                               x--;
                                               if(matriz[x-1][y]==0)
                                                 x--;  
                                               if(matriz[x-1][y]==0)
                                                 x--;    
                                               if(matriz[x-1][y]==0)
                                                 x--;  
                                               if(matriz[x-1][y]==0)
                                                 x--;    
                                               if(matriz[x-1][y]==0)
                                                 x--;    
                                        }         
                                        else   
                                          if(xcenario<0 && a==0)     
                                            xcenario=xcenario+6; 
                                          else
                                           if(a==0 && matriz[x-1][y]==0)//se nao for x>300 para mover cenario e o cenario ja movido entao x anda para tras
                                           {
                                              x--; 
                
                                                    if(a==0 && matriz[x-1][y]==0)
                                                     x--;  
                                                     
                                                    if(a==0 && matriz[x-1][y]==0)
                                                     x--;  
                          
                                                     if(a==0 && matriz[x-1][y]==0)
                                                         x--;  
                                            }               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0; 
                                    mt=mt+9;//incrementa numero de sprites 
                                    
                                       
                                    d=1;         
                             }
                             else
                               mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                               
                               if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                               {
                                 if(roupa==0)      
                                  strcpy(posicao,"personagem/d1.gif");
                                 else
                                   strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                               }
                              // else
                                 //strcpy(posicao,"personagem/p1.gif"); // SE Y NAO FOR IGUAL O 495 CHAO ENTAO MOSTRA IMAGEM DE PULO
                               
                                if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                {                           
                                                                                
                                        movimentos(d,a,x,y,mf,mt,posicao,1,passou,vozu,roupa,lpulo); 
                                           
                                        //MOVENDO CENARIO PARA FRENTE
                                        if(xcenario>-800 && x>330 && a==0)
                                             xcenario=xcenario-6;                                   
                                        else
                                          if(a==0 && matriz[x+1][y]==0)
                                          {
                                             x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++; 
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;    
                                                    
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                 if(a==0 && matriz[x+1][y]==0)
                                                   x++;  
                                                 if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                 
                                                           
                                          }  
                
                                  
                                  if(mf>180)//se numero de sprites for o maximo retorna
                                    mf=0; 
                                  mf=mf+9;//incrementa numero de sprites 
                                  
                                     
                                  d=2; 
                                }
                                else
                                   mf=0; //se nao for para frente entao M contador de sprites fica o primeiro                
                               
                               
                               
                                    if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                    {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d2.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                    }   
                               
                               
                                    if(key[KEY_S]) //BAIXO
                                    {
                                      a=1;             
                                      movimentos(d,a,x,y,mf,mt,posicao,1,passou,vozu,roupa,lpulo);        
                                    }
                                    else
                                       if(key[KEY_W]) //CIMA
                                       {         
                                          a=2;
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passou,vozu,roupa,lpulo);    
                                        }
                                        else
                                          a=0;
                                          
                                           if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                           {
                                               if(tiro==0)            
                                                sinalpv=2;
                                               
                                               if (tiroflag == 0)
                		                       {
                		                        	tiroflag = 1;
                		                        	
                		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptu);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptu);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptu);                                                                                                                                        
                                               }  
                                               movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passou,vozu,roupa,lpulo);     
                                            } 
                                            else
                	                       	  tiroflag = 0;
                	                       
                	                       
                                           
                                            if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                             {                
                                                     movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passou,vozu,roupa,lpulo);  
                                                     pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                       
                                              }
                                              //  else
                                                 // {                              
                                                     if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                     {
                                                        lpulo=0; 
                                                        sinalpv=0;
                      
                                                     } 
                                                      
                                                 // }     
                                              
                                              //////////////////////CONTROLE DE GRAVIDADE
                                              if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                              {     
                                                 y++;
                                                 
                                                     if(matriz[x][y+1]==0)
                                                       y++;
                                                     if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++; 
                                                       if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++;
                                                       if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++; 
                
                                              }        
                                              //////////////////CENARIO//////////////////////////////////////////////////// 
                                              ////////////////CENARIO//////////////////////////////////////////////////////  
                                               draw_sprite(bufferu,fundo2u,xcenario,-1220);
                                               pu = load_gif(posicao,NULL);
                                               
                                               if(a==2)
                                                 show_tiros(x,y,bufferu,3,tirof,tirot,tiroc,matriz,roupa);
                                               else
                    	                         show_tiros(x,y,bufferu,d,tirof,tirot,tiroc,matriz,roupa);
                    	                         
                                               if(i%2==0)//NAVE
                                               {
                                                  draw_sprite(bufferu,naveu,xcenario+100,300);
                                                  i=1;
                                               }   
                                                else 
                                                  {
                                                    draw_sprite(bufferu,nave2u,xcenario+100,300);
                                                    i=0;
                                                   }
                                              
                                              
                                              
                                              
                                              if(roupa==1)     
                                              { 
                                                if(!key[KEY_W] && !key[KEY_S])                                  
                                                  draw_sprite(bufferu,pu,x,y-10); 
                                                else
                                                  if(key[KEY_W])
                                                   draw_sprite(bufferu,pu,x,y-22);
                                                  else
                                                    if(key[KEY_S]) 
                                                     draw_sprite(bufferu,pu,x,y);
                                              }
                                              else
                                                 draw_sprite(bufferu,pu,x,y);
                                              
                                             // if(xcenario<-600)
                                               // draw_sprite(bufferu,soldadofu,750,-1220);   
                                              draw_sprite(bufferu,fundo1u,xcenario,-1220);
                                              if(porta==1)    
                                                draw_sprite(bufferu,fportau,xcenario+1492,465);
                                                                             
                                              destroy_bitmap(pu);                 
                                              ////////////////////////////////////////////////////////////////////////////////////////
                                              
                            textprintf_centre_ex(bufferu, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                           textprintf_centre_ex(bufferu, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                           textprintf_centre_ex(bufferu, font,30,450, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                           textprintf_centre_ex(bufferu, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                           textprintf_centre_ex(bufferu, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);    
                           textprintf_centre_ex(bufferu, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"LPULO :%d",lpulo);
                           textprintf_centre_ex(bufferu, font,50,380, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);                
                           textprintf_centre_ex(bufferu, font,50,400, makecol(0, 0, 0), makecol(255, 255, 255)," FPS-SPEEDY :%d",fps_speed); 
                           textprintf_centre_ex(bufferu, font,50,420, makecol(0, 0, 0), makecol(255, 255, 255)," FPS AUX :%d",auxfps); 
                            //vsync();
                           draw_sprite(bufferu,painelu,0,0);
                           textprintf_centre_ex(bufferu, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                           
                           loop=1;
                    }                             
                        ::fps_speed--;
                       ::fps++;
                }   
                loop=0;
                draw_sprite(screen,bufferu,0,0);//desenha na tela a nova imagem 
                install_keyboard();//ativa o teclado para andar e for mostrado pelo menos uma vez a imagem na tela
                vsync();
                
      }while (x<690 && fim!=-10000);
      
      destroy_bitmap(fundo1u);
      destroy_bitmap(fundo2u);
      destroy_bitmap(naveu);
      destroy_bitmap(nave2u);
      destroy_bitmap(bufferu);
      destroy_bitmap(painelu);
      
      destroy_bitmap(fportau);
      destroy_midi(som);
      //destroy_sample(pmu);
      destroy_sample(abreportau);
      destroy_sample(fechaportau);
      destroy_sample(passou);
      destroy_sample(vozu);
      destroy_sample(ptu); 
      destroy_sample(pmu);
      remove_keyboard();
      
      if(x>689)
      {
         passaporta(1,20,495-30,fps);      
         return 1;//diz que ta indo para o proximo cenario
      }   
      else
        if(fim==-10000)
         return -10000;
      
                    
    //  install_keyboard();
      
     
}





int cenario12(int estado,int fps,int roupa)
{
      BITMAP *buffera,*fundo2a,*fundo1a,*pa,*painela,*xxxa,*chaoa,*tetoa,*c1a,*c2a,*portaa,*teto2a,*fx1a,*fx2a,*fportaa,*fporta2a; 
      SAMPLE *musicaa,*passoa,*voza,*pta,*abreportaa,*fechaportaa;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta1=1,//diz se porta da esquerda ta aberta(1 fechado)
      porta2=1,//diz se porta da direita ta aberta(1 fechado)
      opcao=0,
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      vida=energia,
      tiroflag=0,
      tiro,      
      fimpulo=0,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      auxfps=0,
      //fps,
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo
      

      //install_keyboard();
      buffera = create_bitmap(MAX_X,MAX_Y);
      fundo1a = create_bitmap(MAX_X,MAX_Y);
      fundo2a = create_bitmap(MAX_X,MAX_Y);
      
      fundo1a = load_gif("fundos/cenario12.gif", NULL); 
      fundo2a = load_gif("fundos/fundobase.gif", NULL);
      painela = load_gif("fundos/painel.gif",NULL);
      chaoa = load_gif("fundos/BASEESPACIAL/chao.gif", NULL);
      tetoa = load_gif("fundos/BASEESPACIAL/xxx.gif",NULL);
      teto2a = load_gif("fundos/BASEESPACIAL/xx.gif",NULL);
      xxxa = load_gif("fundos/xxx.gif",NULL);
      c1a = load_gif("fundos/BASEESPACIAL/c1.gif", NULL); 
      c2a = load_gif("fundos/BASEESPACIAL/c2.gif", NULL);
      fx1a = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2a = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portaa = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      fportaa = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      fporta2a = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      passoa = load_wav("musicas/passo.wav");
      
      voza = load_wav("musicas/vpulo.wav");
      abreportaa = load_wav("musicas/Dooropen.wav");
      fechaportaa = load_wav("musicas/Doorclose.wav");
      
      if(roupa==0)
        pta = load_wav("musicas/tirox.wav");
      else
         pta = load_wav("musicas/tiro.wav");
      
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      play_sample(fechaportaa,250,130,1000,0);                                        
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<375;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<93;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<440;y++)
                   matriz[x][y]=1;
               }
               
               for(x=658;x<800;x++)//parede da direita
               {
                 for(y=0;y<440;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<93;x++)//porta de tras trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
                 /////////  
      ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      
      ////////////////////////////
      
      
      if(estado==1)  
      {
          x=100;
          strcpy(posicao,"personagem/d2.gif");
          d=2;
      }    
      else
        if(estado==0) 
        { 
          strcpy(posicao,"personagem/d1.gif");
          x=640;
          d=1;
        }  

      i=1;
      y=495;
      
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
           
           // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
            // install_int( frame_rate, 1000 );   
            while ( ::fps_speed > 0 )
            {          
                  if(loop==0)
                  {  
                           clear(buffera);
                              
                              if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                            //draw_sprite(buffer,fundo2,400,450); 
                                    
                              /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                              if(tirof[0].x-15>695 && porta2==1 && tirof[0].y>450)
                              {
                                   porta2=0;
                
                                   for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportaa,250,130,1000,0);
                              }  
                              if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                              {
                                   porta1=0; 
                                   for(x2=0;x2<93;x2++)//porta de tras liberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportaa,250,130,1000,0);
                              } 
                              /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////                       
                             
                             if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                             { 
                               if (is_windowed_mode())     
                                   set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                
                               else            
                                   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                             }
                             
                             if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                             {                                               
                                       movimentos(d,a,x,y,mf,mt,posicao,1,passoa,voza,roupa,lpulo); 
                                    
                                           if(a==0 && matriz[x-1][y]==0)
                                             x--;  
        
                                             if(a==0 && matriz[x-1][y]==0)
                                               x--;   
                                             if(a==0 && matriz[x-1][y]==0)
                                               x--;  
                                             if(a==0 && matriz[x-1][y]==0)
                                               x--;  
                                             if(a==0 && matriz[x-1][y]==0)
                                               x--;  
                                             if(a==0 && matriz[x-1][y]==0)
                                               x--;    
                                           
                                if(mt>180)//se numero de sprites for o maximo retorna
                                 mt=0;  
                                                       
                                 mt=mt+9; 
        
                                d=1;         
                             }
                             else
                               mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                               
                               if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                               {
                                 if(roupa==0)      
                                  strcpy(posicao,"personagem/d1.gif");
                                 else
                                   strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                               }
                               
                                if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                {                           
                                                                   
                                        movimentos(d,a,x,y,mf,mt,posicao,1,passoa,voza,roupa,lpulo); 
                                           
                                          if(a==0 && matriz[x+1][y]==0)
                                            x++; 
                                          if(a==0 && matriz[x+1][y]==0)
                                            x++;
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;  
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;  
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;       
                
                                  
                                      if(mf>180)//se numero de sprites for o maximo retorna
                                        mf=0; 
                                      mf=mf+9;
                                      d=2; 
                                }
                                else
                                   mf=0; //se nao for para frente entao M contador de sprites fica o primeiro               
                               
                               
                               if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                {
                                 if(roupa==0)      
                                  strcpy(posicao,"personagem/d2.gif");
                                 else
                                   strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                }
                               
                               
                                    if(key[KEY_S]) //BAIXO
                                    {
                                      a=1;             
                                      movimentos(d,a,x,y,mf,mt,posicao,1,passoa,voza,roupa,lpulo);         
                                    }
                                    else
                                       if(key[KEY_W]) //CIMA
                                       {                    
                                          a=2;
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passoa,voza,roupa,lpulo);                                      
                                        }
                                        else
                                          a=0;
                                          
                                           if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                           {
                                               //if(tiro==0)            
                                               // sinalpv=2;
                                               
                                               if (tiroflag == 0)
                		                       {
                		                        	tiroflag = 1;
                		                        	
                		                        	if(a==2)
                                                      atirar(x,y,3,tirof,tirot,tiroc,pta);
                                                    else
                                                      if(a==1)
                                                        atirar(x,y+18,d,tirof,tirot,tiroc,pta);//tiro agaixado
                                                      else  
                                                        atirar(x,y,d,tirof,tirot,tiroc,pta);                                                                                                                                       
                                               }  
                                               movimentos(d,a,x,y,mf,mt,posicao,2,passoa,voza,roupa,lpulo);    
                                            } 
                                            else
                	                       	  tiroflag = 0;
                	                       
                	                            
                                                                                                                                                 
                                            if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                             {                
                                                     movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoa,voza,roupa,lpulo);  
                                                     pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                       
                                              }
                                              //  else
                                                 // {                              
                                                     if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                     {
                                                        lpulo=0; 
                                                        sinalpv=0;
                      
                                                     } 
                                                      
                                                 // }     
                                              
                                              //////////////////////CONTROLE DE GRAVIDADE
                                              if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                              {     
                                                 y++;
                                                 
                                                     if(matriz[x][y+1]==0)
                                                       y++;
                                                     if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++; 
                                                       if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++;
                                                       if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++; 
                
                                              }     
                                              ///////////////CENARIO///////////////////////////////////////////////////////
                                              
                                              
                                              draw_sprite(buffera,fundo2a,0,0);
                                            //  draw_sprite(buffera,fundo1a,0,60);
                                            
                                            //////////////////////////PERSONAGEM
                                               pa = load_gif(posicao,NULL);   
                                                                              
                                              if(roupa==1)     
                                              { 
                                                if(!key[KEY_W] && !key[KEY_S])                                  
                                                  draw_sprite(buffera,pa,x,y-10); 
                                                else
                                                  if(key[KEY_W])
                                                   draw_sprite(buffera,pa,x,y-22);
                                                  else
                                                    if(key[KEY_S]) 
                                                     draw_sprite(buffera,pa,x,y);
                                              }  
                                              else
                                                draw_sprite(buffera,pa,x,y);
                                            
                                               ///////////TIRO///////
                                                if(a==2)
                                                 show_tiros(x,y,buffera,3,tirof,tirot,tiroc,matriz,roupa);
                                               else
                    	                         show_tiros(x,y,buffera,d,tirof,tirot,tiroc,matriz,roupa);
                                               ////////////////////////

                                              //////////MONTANDO CENARIO                                            
                                               draw_sprite(buffera,fx1a,0,265);
                                               draw_sprite(buffera,fx1a,0,265);
                                               draw_sprite(buffera,fx1a,0,210);
                                               draw_sprite(buffera,fx1a,0,155);
                                               draw_sprite(buffera,fx1a,0,100);
                                               draw_sprite(buffera,fx1a,0,45);
                                               
                                               draw_sprite(buffera,fx2a,-100,365);
                                               draw_sprite(buffera,fx2a,-100,415);
                                               draw_sprite(buffera,fx2a,-100,310);
                                               draw_sprite(buffera,fx2a,700,365);
                                               draw_sprite(buffera,fx2a,700,415);
                                               draw_sprite(buffera,fx2a,700,310);
                                               
                                               draw_sprite(buffera,c1a,70,375);
                                               draw_sprite(buffera,c2a,700,375);
                                               draw_sprite(buffera,portaa,705,465);
                                               draw_sprite(buffera,portaa,-13,465);
                                               
                                               if(porta2==1)//portas fechadas ou abertas
                                                draw_sprite(buffera,fportaa,695,465);
                                               if(porta1==1) 
                                                draw_sprite(buffera,fporta2a,87,465);
                                                
                                               draw_sprite(buffera,chaoa,0,562);
                                               
                                               draw_sprite(buffera,tetoa,71,320);
                                               draw_sprite(buffera,tetoa,190,320);
                                               draw_sprite(buffera,tetoa,309,320);
                                               draw_sprite(buffera,tetoa,428,320);
                                               draw_sprite(buffera,tetoa,547,320);
                                               draw_sprite(buffera,teto2a,666,320);
                                              /////////////////////////////////////
                                 
                                            //  draw_sprite(buffera,fundo1a,0,60);                                   
                                              destroy_bitmap(pa);                 
                                              /////////////////////////////////////////////////////
                             
                            
                            textprintf_centre_ex(buffera, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                           textprintf_centre_ex(buffera, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                           //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                           textprintf_centre_ex(buffera, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                           textprintf_centre_ex(buffera, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                           textprintf_centre_ex(buffera, font,50,350, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                           
                           draw_sprite(buffera,painela,0,0);
                           textprintf_centre_ex(buffera, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida);
                           loop=1;
                }          
                           ::fps_speed--;
                           ::fps++;
                           
                         
                
            }         
                   loop=0;
                   draw_sprite(screen,buffera,0,0);//desenha na tela a nova imagem 
                   install_keyboard();//ativa o teclado para andar e for mostrado pelo menos uma vez a imagem na tela
              if(x<55)//volta
               x=-1;     
           vsync();

      }while (x<710 && x!=-1 && fim!=-10000);

      destroy_bitmap(fundo1a);
      destroy_bitmap(fundo2a);
      destroy_bitmap(buffera);
      destroy_bitmap(painela);
      destroy_bitmap(chaoa);
      destroy_bitmap(tetoa);
      
      destroy_bitmap(chaoa);
      destroy_bitmap(tetoa);
      destroy_bitmap(c1a);
     // destroy_bitmap(xxxa);
      destroy_bitmap(c2a);
      destroy_bitmap(portaa);
      destroy_bitmap(teto2a);
      destroy_bitmap(fx1a);
      destroy_bitmap(fx2a);
      
      destroy_bitmap(fportaa);
      destroy_bitmap(fporta2a);
      
      remove_keyboard();               
      destroy_sample(abreportaa);
      destroy_sample(fechaportaa);
      destroy_sample(passoa);
      destroy_sample(voza);
      destroy_sample(pta); 
      remove_keyboard();
      
      if(x==-1)//diz que o jogador ta voltando um cenario
      {
         passaporta(0,20,495-30,fps);            
         return 0;
      } 
      else
       if(x>709)
       {
         passaporta(1,20,495-30,fps);       
         return 1;//diz que ta indo para o proximo cenario 
       }  
       else
        if(fim==-10000)
         return -10000;
      
      
}

int cenario13(int estado,int fps,int roupa)
{
      BITMAP *bufferx,*fundo2x,*fundo1x,*px,*painelx,*elevadorc1x,*elevadorc2x,*soldadofx,*soldadofx2,*chaox,*chao2x,*tetox,*c1x,*c2x,*portax,*teto2x,*fx1x,*fx2x,*c3x,*caixax,*suportex,*fportax; 
      SAMPLE *passox,*vozx,*ptx,*abreportax,*fechaportax;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta1=1,//controle de porta,se 1 porta fechada
      opcao=0,
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
     fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
     // fps,
      vida=energia,
      tiroflag=0,
      tiro,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo


     // install_keyboard();
      bufferx = create_bitmap(MAX_X,MAX_Y);
      fundo1x = create_bitmap(MAX_X,MAX_Y);
      fundo2x = create_bitmap(MAX_X,MAX_Y);
      
     // fundo1x = load_gif("fundos/cenario13.gif", NULL); 
      fundo2x = load_gif("fundos/fundobase.gif", NULL);
      painelx = load_gif("fundos/painel.gif",NULL);
      elevadorc1x = load_gif("objetos/BASEESPACIAL/elevadorcentro.gif", NULL);
      elevadorc2x = load_gif("objetos/BASEESPACIAL/elevadorcentro2.gif", NULL);
      soldadofx = load_gif("objetos/BASEESPACIAL/soldadof.gif",NULL);
      soldadofx2 = load_gif("objetos/BASEESPACIAL/soldadof2.gif",NULL);
      
      caixax = load_gif("objetos/BASEESPACIAL/caixax.gif",NULL);
      suportex = load_gif("objetos/BASEESPACIAL/suporte.gif",NULL);
      chaox = load_gif("fundos/BASEESPACIAL/xxx2.gif", NULL);
      chao2x = load_gif("fundos/BASEESPACIAL/xx2.gif", NULL);
      tetox = load_gif("fundos/BASEESPACIAL/xxx.gif",NULL);
      teto2x = load_gif("fundos/BASEESPACIAL/xx.gif",NULL);
      c1x = load_gif("fundos/BASEESPACIAL/c1.gif", NULL); 
      c2x = load_gif("fundos/BASEESPACIAL/a1.gif", NULL);
      c3x = load_gif("fundos/BASEESPACIAL/a2.gif", NULL);
      fx1x = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2x = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portax = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      fportax = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      
      abreportax = load_wav("musicas/Dooropen.wav");
      fechaportax = load_wav("musicas/Doorclose.wav");
      passox = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptx = load_wav("musicas/tirox.wav");
      else
         ptx = load_wav("musicas/tiro.wav");
      
      ////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;      
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<158;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<115;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<438;y++)
                   matriz[x][y]=1;
               }
               
               for(x=630;x<800;x++)//parede da direita
               {
                 for(y=0;y<475;y++)
                   matriz[x][y]=1;
               }
               
               for(x=339;x<420;x++)//SUPORTE ELEVADOR
               {
                 for(y=303;y<450;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
                 
                 //CAIXA 
                       for(x=515;x<660;x++)//caixa
                       {
                         for(y=429;y<580;y++)
                           matriz[x][y]=1;
                       }
                 /////////  
                 
                    for(x=0;x<84;x++)//porta de tras trancando passagem ateh atirar para abrir
                   {
                     for(y=435;y<496;y++)
                       matriz[x][y]=1;
                   }
                 
                 /////////////CARREGANDO TIROS///////////////////////////////////////
                   iniciatiro(tirof,tirot,tiroc,0,tiroflag);
                 //////////////////////////////////////////////////////////////////
                 
        
           
                 /////////////MONTANDO CENARIO////////////////////////
           draw_sprite(fundo1x,fundo2x,0,0);
           
           draw_sprite(fundo1x,fx1x,0,100);
           draw_sprite(fundo1x,fx1x,0,45);
           
           draw_sprite(fundo1x,fx2x,-60,365);
           draw_sprite(fundo1x,fx2x,-60,415);
           draw_sprite(fundo1x,fx2x,-60,310);
           draw_sprite(fundo1x,fx2x,-60,255);
           draw_sprite(fundo1x,fx2x,-60,205);
           draw_sprite(fundo1x,fx2x,-60,155);

           draw_sprite(fundo1x,fx2x,687,520);
           draw_sprite(fundo1x,fx2x,687,470);
           draw_sprite(fundo1x,fx2x,687,365);
           draw_sprite(fundo1x,fx2x,687,415);
           draw_sprite(fundo1x,fx2x,687,310);
           draw_sprite(fundo1x,fx2x,687,255);
           draw_sprite(fundo1x,fx2x,687,155);
           draw_sprite(fundo1x,fx2x,687,205);
           
           
            draw_sprite(fundo1x,chaox,0,560);
            draw_sprite(fundo1x,chao2x,118,562);
            draw_sprite(fundo1x,chao2x,178,562);
            draw_sprite(fundo1x,chao2x,238,562);
            draw_sprite(fundo1x,chao2x,298,562);                                                  
            draw_sprite(fundo1x,chao2x,438,562);
            draw_sprite(fundo1x,chao2x,498,562);
            draw_sprite(fundo1x,chao2x,558,562);
            draw_sprite(fundo1x,chao2x,618,562);
            draw_sprite(fundo1x,chaox,678,562);
            
               
               draw_sprite(fundo1x,c1x,90,165);
               draw_sprite(fundo1x,c1x,90,379);
               draw_sprite(fundo1x,c1x,90,225);
               draw_sprite(fundo1x,c3x,90,265);
               draw_sprite(fundo1x,c3x,90,325);
               draw_sprite(fundo1x,c2x,660,380);
               draw_sprite(fundo1x,c2x,660,442);
               draw_sprite(fundo1x,c2x,660,503);
               draw_sprite(fundo1x,c2x,660,320);
               draw_sprite(fundo1x,c2x,660,260);
               draw_sprite(fundo1x,c2x,660,200);
               draw_sprite(fundo1x,c2x,660,160);
               draw_sprite(fundo1x,portax,-13,465);
               draw_sprite(fundo1x,fportax,85,465);
               draw_sprite(fundo1x,chaox,0,562);
               
               draw_sprite(fundo1x,tetox,89,110);
               draw_sprite(fundo1x,tetox,190,110);
               draw_sprite(fundo1x,tetox,309,110);
               draw_sprite(fundo1x,tetox,428,110);
               draw_sprite(fundo1x,tetox,547,110);
               draw_sprite(fundo1x,teto2x,666,110);
               
               draw_sprite(fundo1x,caixax,558,496);
               draw_sprite(fundo1x,suportex,329,366); 
               
               draw_sprite(fundo1x,soldadofx,225,495-18);
               draw_sprite(fundo1x,fx2x,688,112);
               draw_sprite(fundo1x,suportex,329,366);
           
           ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      
      if(estado==0)//elevador subindo                  
        elevador(365,700,495,bufferx,fundo2x,fundo1x,painelx,elevadorc1x,elevadorc2x,estado,fps,0,roupa);

      
      vozx = load_wav("musicas/vpulo.wav");
       
      //remove_keyboard();
      //install_keyboard();
      
       if(roupa==0)
        strcpy(posicao,"personagem/d2.gif");
       else 
         strcpy(posicao,"personagem/ARMADURA/dx2.gif");

       
       
      i=1;
      if(estado==0)
      {
        x=374;
        y=495;
        d=1;//posicao inicial 
      }
      else
        if(estado==1)
        {
          x=103;
          y=495;
          d=2;//posicao inicial
          play_sample(fechaportax,250,130,1000,0); 
        } 
      

      // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
             
             while ( ::fps_speed > 0 )
             {        
                   if(loop==0)
                   {     
                               clear(bufferx);
                               
                               /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////                                  
                              if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                              {
                                   porta1=0; 
                                   for(x2=0;x2<84;x2++)//porta de tras liberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportax,250,130,1000,0);
                              } 
                               
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                 
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passox,vozx,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
                    
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
                    
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passox,vozx,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;   
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;    
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro 
                                  
                                   
                                   
                                   
                                if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                {
                                 if(roupa==0)      
                                  strcpy(posicao,"personagem/d2.gif");
                                 else
                                   strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                }
                    
                                   
                                   
                                        if(key[KEY_S]) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passox,vozx,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W]) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passox,vozx,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                               {
                                                 //  if(tiro==0)            
                                                 //   sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptx);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptx);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptx);                                                                                                                                        
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passox,vozx,roupa,lpulo);     
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                                                        
                                                                                                                                                                                        
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passox,vozx,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }       
                                                  
                                                  //////////////////////CONTROLE DE GRAVIDADE
                                                  if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                  {     
                                                     y++;
                                                     
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                         if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                    
                                                  }     
                                                  /////////////////////CENARIO/////////////////////////////////////////////////    
                                                   draw_sprite(bufferx,fundo2x,0,0); 
                                                  //  draw_sprite(bufferx,fundo1x,0,0);
                                                    
                                                   if(a==2)
                                                     show_tiros(x,y,bufferx,3,tirof,tirot,tiroc,matriz,roupa);
                                                   else
                        	                         show_tiros(x,y,bufferx,d,tirof,tirot,tiroc,matriz,roupa);
                                                  
                                                    install_int(IncrementaT, 1500);//movimenta cientista
                                                      if(tempo==1)                                      
                                                         draw_sprite(bufferx,soldadofx,225,495-18);
                                                      else    
                                                         draw_sprite(bufferx,soldadofx2,225,495-18); 
                                                         
                        	                         px = load_gif(posicao,NULL);
                        	                         if(roupa==1)     
                                                      { 
                                                        if(!key[KEY_W] && !key[KEY_S])                                  
                                                          draw_sprite(bufferx,px,x,y-10); 
                                                        else
                                                          if(key[KEY_W])
                                                           draw_sprite(bufferx,px,x,y-22);
                                                          else
                                                            if(key[KEY_S]) 
                                                             draw_sprite(bufferx,px,x,y);
                                                      }
                                                      else
                                                          draw_sprite(bufferx,px,x,y); 
                                                  
                                                   /////////////MONTANDO CENARIO////////////////////////
                                                   draw_sprite(bufferx,fx1x,0,100);
                                                   draw_sprite(bufferx,fx1x,0,45);
                                                   
                                                   draw_sprite(bufferx,fx2x,-60,365);
                                                   draw_sprite(bufferx,fx2x,-60,415);
                                                   draw_sprite(bufferx,fx2x,-60,310);
                                                   draw_sprite(bufferx,fx2x,-60,255);
                                                   draw_sprite(bufferx,fx2x,-60,205);
                                                   draw_sprite(bufferx,fx2x,-60,155);
                                        
                                                   draw_sprite(bufferx,fx2x,687,520);
                                                   draw_sprite(bufferx,fx2x,687,470);
                                                   draw_sprite(bufferx,fx2x,687,365);
                                                   draw_sprite(bufferx,fx2x,687,415);
                                                   draw_sprite(bufferx,fx2x,687,310);
                                                   draw_sprite(bufferx,fx2x,687,255);
                                                   draw_sprite(bufferx,fx2x,687,155);
                                                   draw_sprite(bufferx,fx2x,687,205);
                                                   
                                                   
                                                    draw_sprite(bufferx,chaox,0,560);
                                                    draw_sprite(bufferx,chao2x,118,562);
                                                    draw_sprite(bufferx,chao2x,178,562);
                                                    draw_sprite(bufferx,chao2x,238,562);
                                                    draw_sprite(bufferx,chao2x,298,562);                                                  
                                                    draw_sprite(bufferx,chao2x,438,562);
                                                    draw_sprite(bufferx,chao2x,498,562);
                                                    draw_sprite(bufferx,chao2x,558,562);
                                                    draw_sprite(bufferx,chao2x,618,562);
                                                    draw_sprite(bufferx,chaox,678,562);
                                                    
                                                       
                                                       draw_sprite(bufferx,c1x,90,165);
                                                       draw_sprite(bufferx,c1x,90,379);
                                                       draw_sprite(bufferx,c1x,90,225);
                                                       draw_sprite(bufferx,c3x,90,265);
                                                       draw_sprite(bufferx,c3x,90,325);
                                                       draw_sprite(bufferx,c2x,660,380);
                                                       draw_sprite(bufferx,c2x,660,442);
                                                       draw_sprite(bufferx,c2x,660,503);
                                                       draw_sprite(bufferx,c2x,660,320);
                                                       draw_sprite(bufferx,c2x,660,260);
                                                       draw_sprite(bufferx,c2x,660,200);
                                                       draw_sprite(bufferx,c2x,660,160);
                                                       draw_sprite(bufferx,portax,-13,465);
                                                       
                                                       if(porta1==1)
                                                         draw_sprite(bufferx,fportax,85,465);
                                                         
                                                       draw_sprite(bufferx,chaox,0,562);
                                                       
                                                       draw_sprite(bufferx,tetox,89,110);
                                                       draw_sprite(bufferx,tetox,190,110);
                                                       draw_sprite(bufferx,tetox,309,110);
                                                       draw_sprite(bufferx,tetox,428,110);
                                                       draw_sprite(bufferx,tetox,547,110);
                                                       draw_sprite(bufferx,teto2x,666,110);
                                                       
                                                       draw_sprite(bufferx,caixax,558,496);
                                                       draw_sprite(bufferx,suportex,329,366); 
                                                       
                                                       draw_sprite(bufferx,fx2x,688,112);
                                                       
                                                       
                                                   ////////////////////////////////////////////////////

                                                //   draw_sprite(bufferx,fundo1x,0,0);//cenario
                                                   
                                                   if(i%2==0)//ELEVADOR
                                                   {
                                                      draw_sprite(bufferx,elevadorc1x,365,552);
                                                      i=1;
                                                   }   
                                                    else 
                                                      {
                                                        draw_sprite(bufferx,elevadorc2x,365,552);
                                                        i=0;
                                                       }     
                                                     
                                                        

                                                 // draw_sprite(bufferx,fundo1x,0,60);
                                                  destroy_bitmap(px);                                  
                                                  /////////////////////////////////////////////////////                 
                                
                                textprintf_centre_ex(bufferx, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferx, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferx, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferx, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferx, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferx, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferx, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               textprintf_centre_ex(bufferx, font,50,410, makecol(0, 0, 0), makecol(255, 255, 255)," Estado :%d",estado);
                               show_mouse(bufferx);
                               draw_sprite(bufferx,painelx,0,0);
                               textprintf_centre_ex(bufferx, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                           loop=1;
                       }      
                           ::fps_speed--;
                           ::fps++;
                           
                          // show_mouse(buffer);
                     
              }             
               loop=0;            
                           draw_sprite(screen,bufferx,0,0);//desenha na tela a nova imagem 
                           install_keyboard();//ativa soh quando for mostrado pelo menos uma vez a imagem na tela
          
                         
                       if(x>363 && x<392 && y==495 && key[KEY_S])//apertou para baixo no elevador
                       {
                         x=-1;    
                         if(roupa==0)
                          strcpy(posicao,"personagem/a8.gif");
                         else
                           strcpy(posicao,"personagem/ARMADURA/ax8.gif");
                       }  
                       else
                         if(x<10)//voltou cenario
                           i=-1;  
                         
                          vsync();
                   
      }while (x!=-1 && i!=-1 && fim!=-10000);
      
      
     if(i==-1)//diz que o jogador ta voltando um cenario
     {
       passaporta(0,20,495-30,fps);
       
       destroy_bitmap(fundo1x);
       destroy_bitmap(fundo2x);
       destroy_bitmap(bufferx);
       destroy_bitmap(painelx);
       destroy_bitmap(soldadofx);
       destroy_bitmap(soldadofx2);
       destroy_bitmap(elevadorc1x);
       destroy_bitmap(elevadorc2x); 
       
       destroy_bitmap(chaox);
       destroy_bitmap(chao2x);
       destroy_bitmap(tetox); 
       
       destroy_bitmap(fportax);
       destroy_sample(abreportax);
       destroy_sample(fechaportax);
       
      destroy_bitmap(c1x);
      destroy_bitmap(c2x);
      destroy_bitmap(c3x);
      destroy_bitmap(caixax);
      destroy_bitmap(suportex);
      destroy_bitmap(portax);
      destroy_bitmap(teto2x);
      destroy_bitmap(fx1x);
      destroy_bitmap(fx2x);
       
       destroy_sample(passox);
       destroy_sample(vozx);
       destroy_sample(ptx); 
       remove_keyboard(); 
       return 0;
     }  
      else
       if(x==-1)
       {               
          elevador(365,495,700,bufferx,fundo2x,fundo1x,painelx,elevadorc1x,elevadorc2x,1,fps,0,roupa); //elevador descendo       
          
          destroy_bitmap(fundo1x);
           destroy_bitmap(fundo2x);
           destroy_bitmap(bufferx);
           destroy_bitmap(painelx);
           destroy_bitmap(soldadofx);
           destroy_bitmap(soldadofx2);
           destroy_bitmap(elevadorc1x);
           destroy_bitmap(elevadorc2x); 
           
           destroy_bitmap(chaox);
           destroy_bitmap(chao2x);
           destroy_bitmap(tetox); 
           
           destroy_bitmap(fportax);
           destroy_sample(abreportax);
           destroy_sample(fechaportax);
           
          destroy_bitmap(c1x);
          destroy_bitmap(c2x);
          destroy_bitmap(c3x);
          destroy_bitmap(caixax);
          destroy_bitmap(suportex);
          destroy_bitmap(portax);
          destroy_bitmap(teto2x);
          destroy_bitmap(fx1x);
          destroy_bitmap(fx2x);
           
           destroy_sample(passox);
           destroy_sample(vozx);
           destroy_sample(ptx); 
           remove_keyboard(); 
          return 1;//diz que ta indo para o proximo cenario 
       }     
       else
        if(fim==-10000)
         return -10000;   
}


int cenario14(int estado,int fps,int fundo,int roupa)
{
      BITMAP *bufferc,*fundo2c,*fundo1c,*painelc,*elevadorc1c,*elevadorc2c,*fx1c,*auxc;  
       
      int x=0,i=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      vida=energia;
      
      bufferc = create_bitmap(MAX_X,MAX_Y);
      fundo1c = create_bitmap(MAX_X,MAX_Y);
      fundo2c = create_bitmap(MAX_X,MAX_Y);
      
      auxc = load_gif("fundos/BASEESPACIAL/elevadorx.gif", NULL); 
      fundo2c = load_gif("fundos/fundobase.gif", NULL);
      painelc = load_gif("fundos/painel.gif",NULL);
      fx1c = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL); 
      elevadorc1c = load_gif("objetos/BASEESPACIAL/elevadorcentro.gif", NULL);
      elevadorc2c = load_gif("objetos/BASEESPACIAL/elevadorcentro2.gif", NULL);

      //DESENHANDO CENARIO//////////////
      
      draw_sprite(fundo1c,fundo2c,0,0); 
      draw_sprite(fundo1c,fx1c,0,45);
      draw_sprite(fundo1c,fx1c,0,100);
      draw_sprite(fundo1c,fx1c,0,155);
      draw_sprite(fundo1c,fx1c,0,205);
      draw_sprite(fundo1c,fx1c,0,260);
      draw_sprite(fundo1c,fx1c,0,315);
      draw_sprite(fundo1c,fx1c,0,370);
      draw_sprite(fundo1c,fx1c,0,425);
      draw_sprite(fundo1c,fx1c,0,480);
      draw_sprite(fundo1c,fx1c,0,535);
      draw_sprite(fundo1c,fx1c,0,590);
      
      draw_sprite(fundo1c,fx1c,150,45);
      draw_sprite(fundo1c,fx1c,150,100);
      draw_sprite(fundo1c,fx1c,150,155);
      draw_sprite(fundo1c,fx1c,150,205);
      draw_sprite(fundo1c,fx1c,150,260);
      draw_sprite(fundo1c,fx1c,150,315);
      draw_sprite(fundo1c,fx1c,150,370);
      draw_sprite(fundo1c,fx1c,150,425);
      draw_sprite(fundo1c,fx1c,150,480);
      draw_sprite(fundo1c,fx1c,150,535);
      draw_sprite(fundo1c,fx1c,150,590);
      
      draw_sprite(fundo1c,fx1c,450,45);
      draw_sprite(fundo1c,fx1c,450,100);
      draw_sprite(fundo1c,fx1c,450,155);
      draw_sprite(fundo1c,fx1c,450,205);
      draw_sprite(fundo1c,fx1c,450,260);
      draw_sprite(fundo1c,fx1c,450,315);
      draw_sprite(fundo1c,fx1c,450,370);
      draw_sprite(fundo1c,fx1c,450,425);
      draw_sprite(fundo1c,fx1c,450,480);
      draw_sprite(fundo1c,fx1c,450,535);
      draw_sprite(fundo1c,fx1c,450,590);
      
      draw_sprite(fundo1c,fx1c,620,45);
      draw_sprite(fundo1c,fx1c,620,100);
      draw_sprite(fundo1c,fx1c,620,155);
      draw_sprite(fundo1c,fx1c,620,205);
      draw_sprite(fundo1c,fx1c,620,260);
      draw_sprite(fundo1c,fx1c,620,315);
      draw_sprite(fundo1c,fx1c,620,370);
      draw_sprite(fundo1c,fx1c,620,425);
      draw_sprite(fundo1c,fx1c,620,480);
      draw_sprite(fundo1c,fx1c,620,535);
      draw_sprite(fundo1c,fx1c,620,590);
      
      draw_sprite(fundo1c,auxc,310,0);                                            
      draw_sprite(fundo2c,fundo1c,0,0);
      draw_sprite(fundo1c,fundo2c,0,0);
      
      /////////////////////////////////

      
      if(estado==1)
      {
              elevador(365,0,660,bufferc,fundo2c,fundo1c,painelc,elevadorc1c,elevadorc2c,estado,fps,0,roupa);
              
              
              
              destroy_bitmap(fundo1c);
              destroy_bitmap(fundo2c);
              destroy_bitmap(elevadorc1c);
              destroy_bitmap(elevadorc2c);
              destroy_bitmap(bufferc);
              destroy_bitmap(painelc);
              destroy_bitmap(fx1c);
              destroy_bitmap(auxc);
              remove_keyboard();              
              install_keyboard();
              
              return 1;
      }        
      else
         if(estado==0)
         {
             elevador(365,660,-100,bufferc,fundo2c,fundo1c,painelc,elevadorc1c,elevadorc2c,estado,fps,0,roupa);    
             
              destroy_bitmap(fundo1c);
              destroy_bitmap(fundo2c);
              destroy_bitmap(elevadorc1c);
              destroy_bitmap(elevadorc2c);
              destroy_bitmap(bufferc);
              destroy_bitmap(painelc);
              destroy_bitmap(fx1c);
              destroy_bitmap(auxc);
              remove_keyboard();              
              install_keyboard();
              
              return 0;
         }    
         

}

int cenario15(int estado,int fps,int roupa)
{
      BITMAP *bufferd,*fundo2d,*fundo1d,*pd,*paineld,*elevadorc1d,*elevadorc2d,*elevadorb1d,*elevadorb2d,*soldadofd,
      *soldadofd2,*soldadofd3,*chaod,*chao2d,*c1d,*c2d,*portad,*teto2d,*fx1d,*fx2d,*c3d,*caixad,*fportad; 
      
      SAMPLE *passod,*voz3d,*ptd,*abreportad,*fechaportad;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta=1,//controla se porta aberto ou fechado 1 eh fechado
      opcao=1,
      i=0,//diz qual bitmap do elevador vai ser mostrado
      i2=0,//mostra base do elevador dizendo qual deve aparecer
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      vida=energia,
      tiroflag=0,
      tiro,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      xs=230,//controla a posicao da coluna inicial do soldado se movimentando atraves da funcao
      ys=475,//controla a posicao da linha inicial do soldado se movimentando atraves da funcao
      direcao=2,//controla direcao do soldado movimentando na tela passando por funcao
      //fps,
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo
    /*  pleiturax=0;
      pleituray=0;*/

      install_keyboard();
      bufferd = create_bitmap(MAX_X,MAX_Y);
      fundo1d = create_bitmap(MAX_X,MAX_Y);
      fundo2d = create_bitmap(MAX_X,MAX_Y);
      
    //  fundo1d = load_gif("fundos/cenario15.gif", NULL); 
      fundo2d = load_gif("fundos/fundobase.gif", NULL);
      paineld = load_gif("fundos/painel.gif",NULL);
      elevadorc1d = load_gif("objetos/BASEESPACIAL/elevadorcentro.gif", NULL);
      elevadorc2d = load_gif("objetos/BASEESPACIAL/elevadorcentro2.gif", NULL);
      elevadorb1d = load_gif("objetos/BASEESPACIAL/elevador1.gif", NULL);
      elevadorb2d = load_gif("objetos/BASEESPACIAL/elevador2.gif", NULL);
      soldadofd = load_gif("objetos/BASEESPACIAL/soldadof.gif",NULL);
      soldadofd2 = load_gif("objetos/BASEESPACIAL/soldadof2.gif",NULL);
      soldadofd3 = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadox.gif",NULL);
      voz3d = load_wav("musicas/vpulo.wav");
      abreportad = load_wav("musicas/Dooropen.wav");
      fechaportad = load_wav("musicas/Doorclose.wav");
      fportad = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      
      passod = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptd = load_wav("musicas/tirox.wav");
      else
         ptd = load_wav("musicas/tiro.wav");
      
      caixad = load_gif("objetos/BASEESPACIAL/caixax.gif",NULL);
      chaod = load_gif("fundos/BASEESPACIAL/chao.gif", NULL);
      chao2d = load_gif("fundos/BASEESPACIAL/xx.gif", NULL);
      teto2d = load_gif("fundos/BASEESPACIAL/xx.gif",NULL);
      c1d = load_gif("fundos/BASEESPACIAL/c2.gif", NULL);
      c2d = load_gif("fundos/BASEESPACIAL/a1.gif", NULL);
      c3d = load_gif("fundos/BASEESPACIAL/a2.gif", NULL);
      fx1d = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2d = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portad = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      ////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<150;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<115;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=640;x<800;x++)//parede da direita
               {
                 for(y=0;y<443;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               //for(x=0;x<800;x++)//chao
               //  matriz[x][496]=1; 
                 
                 //CAIXA 
                       for(x=115;x<203;x++)//caixa
                       {
                         for(y=436;y<580;y++)
                           matriz[x][y]=1;
                       }
                       
                       for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
                       {
                         for(y=440;y<496;y++)
                           matriz[x][y]=1;
                       }
                 /////////  
      //////////////////////////////////////////////////////////           
         /////////////MONTANDO CENARIO////////////////////////
          // draw_sprite(bufferd,fx1d,0,100);
          // draw_sprite(bufferd,fx1d,0,45);
           draw_sprite(fundo1d,fundo2d,0,0);
           
           draw_sprite(fundo1d,fx2d,-60,365);
           draw_sprite(fundo1d,fx2d,-60,415);
           draw_sprite(fundo1d,fx2d,-60,310);
           draw_sprite(fundo1d,fx2d,-60,255);
           draw_sprite(fundo1d,fx2d,-60,205);
           draw_sprite(fundo1d,fx2d,-60,155);
           draw_sprite(fundo1d,fx2d,-60,100);
           draw_sprite(fundo1d,fx2d,-60,50);
           draw_sprite(fundo1d,fx2d,-60,525);
           draw_sprite(fundo1d,fx2d,-60,470);
           draw_sprite(fundo1d,fx2d,687,365);
           draw_sprite(fundo1d,fx2d,-60,415);
          // draw_sprite(bufferd,fx2d,687,520);
          // draw_sprite(bufferd,fx2d,687,470);
           draw_sprite(fundo1d,fx2d,687,415);
           draw_sprite(fundo1d,fx2d,687,310);
           draw_sprite(fundo1d,fx2d,687,255);
           draw_sprite(fundo1d,fx2d,687,155);
           draw_sprite(fundo1d,fx2d,687,205);
           draw_sprite(fundo1d,fx2d,687,100);
           draw_sprite(fundo1d,fx2d,687,50);
           
           
            draw_sprite(fundo1d,chaod,0,560);
            draw_sprite(fundo1d,chao2d,118,70);
            draw_sprite(fundo1d,chao2d,178,70);
            draw_sprite(fundo1d,chao2d,238,70);
            draw_sprite(fundo1d,chao2d,298,70);                                                  
            draw_sprite(fundo1d,chao2d,438,70);
            draw_sprite(fundo1d,chao2d,498,70);
            draw_sprite(fundo1d,chao2d,558,70);
            draw_sprite(fundo1d,chao2d,618,70);
          //  draw_sprite(bufferd,chaod,678,150);
            
               
               draw_sprite(fundo1d,c3d,90,385);
               draw_sprite(fundo1d,c3d,90,445);
               draw_sprite(fundo1d,c3d,90,505);
               draw_sprite(fundo1d,c3d,90,265);
               draw_sprite(fundo1d,c3d,90,205);
               draw_sprite(fundo1d,c3d,90,145);
               draw_sprite(fundo1d,c3d,90,325);
               draw_sprite(fundo1d,c3d,90,90);
               draw_sprite(fundo1d,c3d,90,40);
            //   draw_sprite(bufferd,c2d,660,380);
             //  draw_sprite(bufferd,c2d,660,442);
            //   draw_sprite(bufferd,c2d,660,503);
               draw_sprite(fundo1d,c2d,680,340);
               draw_sprite(fundo1d,c2d,680,300);
               draw_sprite(fundo1d,c2d,680,240);
               draw_sprite(fundo1d,c2d,680,180);
               draw_sprite(fundo1d,c2d,680,150);
               draw_sprite(fundo1d,c2d,680,90);
               draw_sprite(fundo1d,c2d,680,40);
               draw_sprite(fundo1d,portad,700,465);
               draw_sprite(fundo1d,fportad,695,465);
               draw_sprite(fundo1d,chaod,0,562);
               
               draw_sprite(fundo1d,c1d,680,378);
               draw_sprite(fundo1d,caixad,115,496);
               draw_sprite(fundo1d,soldadofd,550,474);
               draw_sprite(fundo1d,soldadofd3,230,475);
               
              // draw_sprite(bufferd,fx2d,688,112);
           
           ////////////////////////////////////////////////////          
      ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      
      remove_keyboard();
      install_keyboard();
      
      if(estado==1)
      {
           if(roupa==0)
             strcpy(posicao,"personagem/a8.gif");
           else
             strcpy(posicao,"personagem/ARMADURA/ax8.gif"); 
           
           
          ///////////////////////////////ELEVADOR DECENDO/////////////////////////////
                 if(estado==1)
                   elevador2(365,0,490,325,bufferd,fundo2d,fundo1d,paineld,elevadorc1d,elevadorc2d,estado,elevadorb1d,elevadorb2d,fps,0,roupa);                        
                  
           //tira som de elevador e volta para voz de samus       
        x=374;
      }
      else
          if(estado==0)
          {
            x=655;
            d=1;//posicao inicial
             if(roupa==0)
               strcpy(posicao,"personagem/d1.gif");
             else
               strcpy(posicao,"personagem/ARMADURA/dx1.gif"); 
            play_sample(fechaportad,250,130,1000,0);   
          }  
       
       i=1;
      y=495;
     
       // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
             while ( ::fps_speed > 0 )
             { 
                    if(loop==0)
                    {       
                               clear(bufferd);
                              
                              /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                              if(tirof[0].x-15>695 && porta==1 && tirof[0].y>450)
                              {
                                   porta=0;
                
                                   for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportad,250,130,1000,0);
                              }   
                              
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                    
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passod,voz3d,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
            
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--; 
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;    
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
            
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passod,voz3d,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;       
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro               
                                   
                                   
                                   if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                    {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d2.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                    }
                    
                                   
                                   
                                        if(key[KEY_S]) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passod,voz3d,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W]) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passod,voz3d,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                               {
                                                   //if(tiro==0)            
                                                  //  sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptd);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptd);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptd);                                                                                                                                        
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,2,passod,voz3d,roupa,lpulo);     
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                 
                                                                                                                                                                                        
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passod,voz3d,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }     
                                                  
                                                  //////////////////////CONTROLE DE GRAVIDADE
                                                  if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                  {     
                                                     y++;
                                                     
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                         if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                    
                                                  }     
                                                  ////////////////////////CENARIO////////////////////////////////////////////// 
                                                  pd = load_gif(posicao,NULL);                
                                                  draw_sprite(bufferd,fundo2d,0,0);
                                                  
                                                  
                                                   if(i2%2==0)//ELEVADOR BASE
                                                    {
                                                      draw_sprite(bufferd,elevadorb1d,325,469);
                                                      i2=1;
                                                    }   
                                                    else 
                                                      {
                                                        draw_sprite(bufferd,elevadorb2d,325,469);
                                                        i2=0;
                                                       } 
                                                       
                                                       if(i%2==0)//ELEVADOR
                                                       {
                                                          draw_sprite(bufferd,elevadorc1d,364,542);
                                                          i=1;
                                                       }   
                                                        else 
                                                          {
                                                            draw_sprite(bufferd,elevadorc2d,364,542);
                                                            i=0;
                                                           }   
                                                  
                                                  ////////////soldados////////////////////////////
                                                      install_int(IncrementaT, 1500);//movimenta soldado
                                                      if(tempo==1)                                      
                                                         draw_sprite(bufferd,soldadofd,550,474);
                                                      else    
                                                         draw_sprite(bufferd,soldadofd2,550,474);
                                              
                                                      //draw_sprite(bufferd,soldadofd3,225,495-16);
                                                      install_int(IncrementaT3,300);
                                                      movimentopersonagem(bufferd,soldadofd3,xs,ys,220,600,direcao,0,1);//funcao movimenta soldado
                                                      /////////////////////////////////////////////////
                                                  
                                                  if(a==2)
                                                     show_tiros(x,y,bufferd,3,tirof,tirot,tiroc,matriz,roupa);
                                                   else
                        	                         show_tiros(x,y,bufferd,d,tirof,tirot,tiroc,matriz,roupa); 
                        	                      
                                                   
                                                     
                                                  if(roupa==1)     
                                                  { 
                                                    if(!key[KEY_W] && !key[KEY_S])                                  
                                                      draw_sprite(bufferd,pd,x,y-10); 
                                                    else
                                                      if(key[KEY_W])
                                                       draw_sprite(bufferd,pd,x,y-22);
                                                      else
                                                        if(key[KEY_S]) 
                                                         draw_sprite(bufferd,pd,x,y);
                                                  }
                                                  else
                                                    draw_sprite(bufferd,pd,x,y);
                                                  
                                                  
                                                       
                                                  /////////////MONTANDO CENARIO////////////////////////
                                                  // draw_sprite(bufferd,fx1d,0,100);
                                                  // draw_sprite(bufferd,fx1d,0,45);
                                                   
                                                   draw_sprite(bufferd,fx2d,-60,365);
                                                   draw_sprite(bufferd,fx2d,-60,415);
                                                   draw_sprite(bufferd,fx2d,-60,310);
                                                   draw_sprite(bufferd,fx2d,-60,255);
                                                   draw_sprite(bufferd,fx2d,-60,205);
                                                   draw_sprite(bufferd,fx2d,-60,155);
                                                   draw_sprite(bufferd,fx2d,-60,100);
                                                   draw_sprite(bufferd,fx2d,-60,50);
                                                   draw_sprite(bufferd,fx2d,-60,525);
                                                   draw_sprite(bufferd,fx2d,-60,470);
                                                   draw_sprite(bufferd,fx2d,687,365);
                                                   draw_sprite(bufferd,fx2d,-60,415);
                                                  // draw_sprite(bufferd,fx2d,687,520);
                                                  // draw_sprite(bufferd,fx2d,687,470);
                                                   draw_sprite(bufferd,fx2d,687,415);
                                                   draw_sprite(bufferd,fx2d,687,310);
                                                   draw_sprite(bufferd,fx2d,687,255);
                                                   draw_sprite(bufferd,fx2d,687,155);
                                                   draw_sprite(bufferd,fx2d,687,205);
                                                   draw_sprite(bufferd,fx2d,687,100);
                                                   draw_sprite(bufferd,fx2d,687,50);
                                                   
                                                   
                                                    draw_sprite(bufferd,chaod,0,560);
                                                    draw_sprite(bufferd,chao2d,118,70);
                                                    draw_sprite(bufferd,chao2d,178,70);
                                                    draw_sprite(bufferd,chao2d,238,70);
                                                    draw_sprite(bufferd,chao2d,298,70);                                                  
                                                    draw_sprite(bufferd,chao2d,438,70);
                                                    draw_sprite(bufferd,chao2d,498,70);
                                                    draw_sprite(bufferd,chao2d,558,70);
                                                    draw_sprite(bufferd,chao2d,618,70);
                                                  //  draw_sprite(bufferd,chaod,678,150);
                                                    
                                                       
                                                       draw_sprite(bufferd,c3d,90,385);
                                                       draw_sprite(bufferd,c3d,90,445);
                                                       draw_sprite(bufferd,c3d,90,505);
                                                       draw_sprite(bufferd,c3d,90,265);
                                                       draw_sprite(bufferd,c3d,90,205);
                                                       draw_sprite(bufferd,c3d,90,145);
                                                       draw_sprite(bufferd,c3d,90,325);
                                                       draw_sprite(bufferd,c3d,90,90);
                                                       draw_sprite(bufferd,c3d,90,40);
                                                    //   draw_sprite(bufferd,c2d,660,380);
                                                     //  draw_sprite(bufferd,c2d,660,442);
                                                    //   draw_sprite(bufferd,c2d,660,503);
                                                       draw_sprite(bufferd,c2d,680,340);
                                                       draw_sprite(bufferd,c2d,680,300);
                                                       draw_sprite(bufferd,c2d,680,240);
                                                       draw_sprite(bufferd,c2d,680,180);
                                                       draw_sprite(bufferd,c2d,680,150);
                                                       draw_sprite(bufferd,c2d,680,90);
                                                       draw_sprite(bufferd,c2d,680,40);
                                                       draw_sprite(bufferd,portad,700,465);
                                                       if(porta==1)//portas fechadas ou abertas
                                                         draw_sprite(bufferd,fportad,695,465);
                                                       draw_sprite(bufferd,chaod,0,562);
                                                       
                                                       draw_sprite(bufferd,c1d,680,378);
                                                       draw_sprite(bufferd,caixad,115,496);
                                                       
                                                      // draw_sprite(bufferd,fx2d,688,112);
                                                   
                                                   ////////////////////////////////////////////////////

                                                                                                                                                                          
                                                 // draw_sprite(bufferd,pd,x,y); 
                                                  //draw_sprite(bufferd,fundo1d,0,64);
                                                  destroy_bitmap(pd);  
                                                //  destroy_bitmap(soldadofd3);                                
                                                 ///////////////////////////////////////////////////////////////////                  
                                
                                textprintf_centre_ex(bufferd, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferd, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferd, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferd, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferd, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferd, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferd, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               
                               draw_sprite(bufferd,paineld,0,0);
                               textprintf_centre_ex(bufferd, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                
                      loop=1;
                      }                             
                      ::fps_speed--;
                      ::fps++;
               }    
               loop=0;                      
                            //   show_mouse(bufferd);
                   draw_sprite(screen,bufferd,0,0);//desenha na tela a nova imagem 
                   
               if(x>363 && x<392 && y==495 && key[KEY_W])//apertou para baixo no elevador
                 x=-1;    
               else
                 if(x>700)//passou
                   i=-1;  
               
               //rest(fps/3300);//controle de frames
                vsync();
      }while (x!=-1 && i!=-1 && fim!=-10000);
      
      
      //////////////////////////////ELEVADOR SUBINDO/////////////////////////////
             if(x==-1)
             {
                      elevador2(365,490,-100,325,bufferd,fundo2d,fundo1d,paineld,elevadorc1d,elevadorc2d,0,elevadorb1d,elevadorb2d,fps,0,roupa);
                      
                      destroy_bitmap(fundo1d);
                      destroy_bitmap(fundo2d);
                      destroy_bitmap(bufferd);
                      destroy_bitmap(paineld);
                      destroy_bitmap(soldadofd);
                      destroy_bitmap(soldadofd2);
                      destroy_bitmap(elevadorc1d);
                      destroy_bitmap(elevadorc2d);
                     // destroy_bitmap(elevadorb1d);
                      destroy_bitmap(elevadorb2d);
                    //  destroy_bitmap(soldadofd3); 
                       destroy_bitmap(fportad);
                       destroy_sample(abreportad);
                       destroy_sample(fechaportad);
                                    
                      destroy_bitmap(c1d);
                      destroy_bitmap(c2d);
                      destroy_bitmap(c3d);
                      destroy_bitmap(caixad);
                      destroy_bitmap(portad);
                      destroy_bitmap(teto2d);
                      destroy_bitmap(fx1d);
                      destroy_bitmap(fx2d);
                      
                   //   destroy_sample(musica);
                      destroy_sample(passod);
                      destroy_sample(voz3d);
                      destroy_sample(ptd); 
                      
                      return 0;
              }
                   
      // stop_sample(voz);
      //////////////////////////////////////////////////////////////////////////////
      
     if(i==-1)
     {
          passaporta(1,20,495-30,fps); 
       
          destroy_bitmap(fundo1d);
          destroy_bitmap(fundo2d);
          destroy_bitmap(bufferd);
          destroy_bitmap(paineld);
        //  destroy_bitmap(soldadofd);//desalocar da erro
          destroy_bitmap(soldadofd2);
          destroy_bitmap(soldadofd3);
          destroy_bitmap(elevadorc1d);
          destroy_bitmap(elevadorc2d);
         // destroy_bitmap(elevadorb1d);
         // destroy_bitmap(soldadofd3); 
          destroy_bitmap(c1d);
          destroy_bitmap(c2d);
          destroy_bitmap(c3d);
          destroy_bitmap(caixad);
          destroy_bitmap(portad);
          destroy_bitmap(teto2d);
          destroy_bitmap(fx1d);
          destroy_bitmap(fx2d);
          
          destroy_bitmap(fportad);
          destroy_sample(abreportad);
          destroy_sample(fechaportad);
          
          destroy_bitmap(elevadorb2d);
          destroy_bitmap(soldadofd);
          //destroy_sample(musica);
          destroy_sample(passod);
          destroy_sample(voz3d);
          destroy_sample(ptd); 
             
       return 1;//diz que o jogador passou um cenario 
     }   
     else
        if(fim==-10000)
         return -10000;
}

int cenario16(int estado,int fps,int roupa)
{
      BITMAP *buffere,*fundo2e,*pe,*painele,*soldadofe,*pilare,*pilarne,*paredede,*portaxe,*caixae,*fportae; 
      SAMPLE *passoe,*voz3e,*pte,*abreportae,*fechaportae;
      char posicao[20];
      char posicaos[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta1=1,//controle de porta,se 1 porta fechada
      opcao=0,
      i=0,//diz qual bitmap do elevador vai ser mostrado
      i2=0,//mostra base do elevador dizendo qual deve aparecer
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      loop=0,//controle de escrita no buffer com o uso do while do fps
      vida=energia,
      tiroflag=0,
      tiro,
      xs=230,//controla a posicao da coluna inicial do soldado se movimentando atraves da funcao
      ys=481,//controla a posicao da linha inicial do soldado se movimentando atraves da funcao
      direcao=2,//controla direcao do soldado movimentando na tela passando por funcao
      //fps,
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo

      install_keyboard();
      buffere = create_bitmap(MAX_X,MAX_Y);
      fundo2e = create_bitmap(MAX_X,MAX_Y);
      
      caixae = load_gif("objetos/BASEESPACIAL/caixax.gif",NULL);
      fundo2e = load_gif("fundos/fundobase.gif", NULL);
      painele = load_gif("fundos/painel.gif",NULL);
      portaxe = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      paredede = load_gif("fundos/BASEESPACIAL/pareded.gif",NULL);
      voz3e = load_wav("musicas/vpulo.wav");
      pilare = load_gif("fundos/BASEESPACIAL/pilar.gif",NULL);
      pilarne = load_gif("fundos/BASEESPACIAL/pilarn.gif",NULL);
      passoe = load_wav("musicas/passo.wav");
      
      fportae = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      
      abreportae = load_wav("musicas/Dooropen.wav");
      fechaportae = load_wav("musicas/Doorclose.wav");
      
      if(roupa==0)
        pte = load_wav("musicas/tirox.wav");
      else
         pte = load_wav("musicas/tiro.wav");
      soldadofe = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadox.gif",NULL);
      
      
      ////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400; 
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;
              }
            /////////////////////////////

               for(x=0;x<500;x++)//pilar de cima
               {
                 for(y=0;y<110;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<115;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<438;y++)
                   matriz[x][y]=1;
               }
               
               for(x=640;x<800;x++)//parede da direita
               {
                 for(y=0;y<600;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=230;x<392;x++)//pilar meio
               {
                 for(y=340;y<450;y++)
                   matriz[x][y]=1;
               }
               
               for(x=470;x<680;x++)//pilar 1 direita 
               {
                 for(y=239;y<347;y++)
                   matriz[x][y]=1;
               }
               
               for(x=550;x<680;x++)//pilar 2 direita 
               {
                 for(y=93;y<192;y++)
                   matriz[x][y]=1;
               }
                 
                 //CAIXA 
                       for(x=440;x<750;x++)//caixa
                       {
                         for(y=435;y<580;y++)
                           matriz[x][y]=1;
                       }
                       
                       for(x=0;x<84;x++)//porta de tras trancando passagem ateh atirar para abrir
                       {
                         for(y=440;y<496;y++)
                           matriz[x][y]=1;
                       }
                 /////////  
      ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      

      i=1;
      
      if(estado==1)
      {
        x=90;
        y=495;
        d=2;//posicao inicial   
        play_sample(fechaportae,250,130,1000,0);          
      }
      else
        {
           x=608;
           y=10;
           d=1;//posicao inicial              
        }
      
      // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
            while ( ::fps_speed > 0 )
            { 
                    if(loop==0)
                    {        
                               clear(buffere);
                               
                                /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////                                  
                              if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                              {
                                   porta1=0; 
                                   for(x2=0;x2<84;x2++)//porta de tras liberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportae,250,130,1000,0);
                              } 
                               
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                 
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passoe,voz3e,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
            
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--; 
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;    
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
            
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passoe,voz3e,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;       
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro 
                           
                                   
                                   
                                   
                                if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                {
                                 if(roupa==0)      
                                  strcpy(posicao,"personagem/d2.gif");
                                 else
                                   strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                }
                    
                                   
                                   
                                        if(key[KEY_S]) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passoe,voz3e,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W]) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passoe,voz3e,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                               {
                                               //    if(tiro==0)            
                                               //     sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,pte);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,pte);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,pte);                                                                                                                                        
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,2,passoe,voz3e,roupa,lpulo);     
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                   
                                                                                                                                                                                        
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoe,voz3e,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }     
                                                  
                                                  //////////////////////CONTROLE DE GRAVIDADE
             
                                                if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                  {     
                                                     y++;
                                                     
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                         if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                    
                                                  }     
                                                  /////////////////////////////CENARIO/////////////////////////////////////////
                                                  
                                                  draw_sprite(buffere,fundo2e,0,0);  
                                                  
                                                  draw_sprite(buffere,pilare,315,445);
                                                  install_int(IncrementaT3,300);
                                                  movimentopersonagem(buffere,soldadofe,xs,ys,160,430,direcao,0,1);//funcao movimenta soldado
                                                   
                                                  
                                                  if(a==2)
                                                     show_tiros(x,y,buffere,3,tirof,tirot,tiroc,matriz,roupa);
                                                   else
                        	                         show_tiros(x,y,buffere,d,tirof,tirot,tiroc,matriz,roupa);
                                                                             
                                                    pe = load_gif(posicao,NULL);
                                                                                         
                                                  if(roupa==1)     
                                                  { 
                                                    if(!key[KEY_W] && !key[KEY_S])                                  
                                                      draw_sprite(buffere,pe,x,y-10); 
                                                    else
                                                      if(key[KEY_W])
                                                       draw_sprite(buffere,pe,x,y-22);
                                                      else
                                                        if(key[KEY_S]) 
                                                         draw_sprite(buffere,pe,x,y);
                                                  }
                                                  else
                                                    draw_sprite(buffere,pe,x,y);
                                                  
                                                   ////////MONTANDO CENARIO/////////// 
                                                  draw_sprite(buffere,pilarne,0,564);
                                                  draw_sprite(buffere,pilarne,150,564);
                                                  draw_sprite(buffere,pilarne,300,564);
                                                  draw_sprite(buffere,pilarne,450,564);
                                                  draw_sprite(buffere,pilarne,600,564);
                                                  
                                                  draw_sprite(buffere,pilarne,0,70);
                                                  draw_sprite(buffere,pilarne,150,70);
                                                  draw_sprite(buffere,pilarne,300,70);
                                                  draw_sprite(buffere,pilarne,393,70);

                                                  draw_sprite(buffere,pilarne,260,406);
                                                  draw_sprite(buffere,pilarne,505,306);
                                                  draw_sprite(buffere,pilarne,583,157);
                                                  draw_sprite(buffere,pilarne,655,306);
                                                  draw_sprite(buffere,paredede,685,20);
                                                  draw_sprite(buffere,paredede,-8,-119);
                                                  draw_sprite(buffere,portaxe,-13,465);
                                                  if(porta1==1)
                                                    draw_sprite(buffere,fportae,85,465);
                                                  
                                                  draw_sprite(buffere,caixae,483,500);
                                                  draw_sprite(buffere,caixae,583,500);
                                                  ///////////////////////////////////////////////
                                                  
                                                  destroy_bitmap(pe);   
                                                //  destroy_bitmap(soldadofe);                               
                                                  /////////////////////////////////////////////////////                 
                                
                                textprintf_centre_ex(buffere, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(buffere, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(buffere, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(buffere, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(buffere, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(buffere, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(buffere, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               
                               
                               draw_sprite(buffere,painele,0,0);
                               textprintf_centre_ex(buffere, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                               show_mouse(buffere);
                               loop=1;
                         }                             
                         ::fps_speed--;
                         ::fps++;
                  }   
                  loop=0;           
                               
                   draw_sprite(screen,buffere,0,0);//desenha na tela a nova imagem 
                   
               if(x<47)//volta
                 x=-1;    
               else
                 if(y<10)//passou para parte 2 da tela
                   i=-1;  
               
               //rest(fps/3300);//controle de frames
               
             vsync();
      }while (x!=-1 && i!=-1 && fim!=-10000);
      
       
      destroy_bitmap(soldadofe);
      destroy_bitmap(fundo2e);
      
      destroy_bitmap(pilare);
      destroy_bitmap(pilarne);
      destroy_bitmap(paredede);
      destroy_bitmap(portaxe);
      destroy_bitmap(caixae);
      
    //  destroy_bitmap(fportae);
      destroy_sample(abreportae);
      destroy_sample(fechaportae);
      
    //  destroy_bitmap(buffere);
      destroy_bitmap(painele);
      destroy_sample(passoe);
      destroy_sample(voz3e);
      destroy_sample(pte); 
      
     if(i==-1)//chama tela de cima do mesmo cenario
     {
       remove_keyboard();               
       return 1;//diz que o jogador passando um cenario
     }  
     else
       if(x==-1)
       {
         passaporta(0,20,495-30,fps);        
         return 0;//voltando um cenario     
       }   
       else
        if(fim==-10000)
         return -10000;
         
        
}

int cenario16p2(int estado,int fps,int roupa)
{
      BITMAP *bufferf,*fundo2f,*pf,*painelf,*soldadoff,*pilarf,*pilarnf,*parededf,*portaxf,*fportaf;  
      SAMPLE *passof,*voz3f,*ptf,*abreportaf,*fechaportaf;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta1=1,//controle de porta,se 1 porta fechada
      opcao=0,
      tiroflag=0,
      i=0,//diz qual bitmap do elevador vai ser mostrado
      i2=0,//mostra base do elevador dizendo qual deve aparecer
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      vida=energia,
      tiro,
      xs=230,//controla a posicao da coluna inicial do soldado se movimentando atraves da funcao
      ys=479,//controla a posicao da linha inicial do soldado se movimentando atraves da funcao
      direcao=2,//controla direcao do soldado movimentando na tela passando por funcao
      loop=0,//controle de escrita no buffer com o uso do while do fps
      //fps,
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo


      install_keyboard();
      bufferf = create_bitmap(MAX_X,MAX_Y);
      fundo2f = create_bitmap(MAX_X,MAX_Y);
      
      
      portaxf = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      parededf = load_gif("fundos/BASEESPACIAL/pareded.gif",NULL);
      pilarf = load_gif("fundos/BASEESPACIAL/pilar.gif",NULL);
      pilarnf = load_gif("fundos/BASEESPACIAL/pilarn.gif",NULL);
      
      fportaf = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      
      abreportaf = load_wav("musicas/Dooropen.wav");
      fechaportaf = load_wav("musicas/Doorclose.wav");
      
      fundo2f = load_gif("fundos/fundobase.gif", NULL);
      painelf = load_gif("fundos/painel.gif",NULL);
      voz3f = load_wav("musicas/vpulo.wav");
      passof = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptf = load_wav("musicas/tirox.wav");
      else
         ptf = load_wav("musicas/tiro.wav");
      soldadoff = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadox.gif",NULL);
      ////////////////////////////
       //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;
              }
            /////////////////////////////
               
               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<107;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<115;x++)//parede dA ESQUERDA
               {
                 for(y=133;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=634;x<800;x++)//parede da direita
               {
                 for(y=0;y<600;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<535;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=360;x<680;x++)//pilar 1 direita 
               {
                 for(y=367;y<463;y++)
                   matriz[x][y]=1;
               }
               
               for(x=516;x<680;x++)//pilar 2 direita 
               {
                 for(y=222;y<323;y++)
                   matriz[x][y]=1;
               }
               for(x=100;x<416;x++)//pilar esquerda 
               {
                 for(y=133;y<238;y++)
                   matriz[x][y]=1;
               }
               for(x=0;x<97;x++)//porta de tras trancando passagem ateh atirar para abrir
               {
                 for(y=0;y<133;y++)
                   matriz[x][y]=1;
               }
                 
      ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      
      
       strcpy(posicao,"personagem/a8.gif");
       
      if(estado==0)
      {
        x=100;
        y=132;
        d=2;//posicao inicial    
        play_sample(fechaportaf,250,130,1000,0);         
      }
      else
        {
           x=483;
           y=495;
           d=1;//posicao inicial              
        }
        
      i=1;
      
       // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
            while ( ::fps_speed > 0 )
            { 
                    if(loop==0)
                    {       
                               clear(bufferf);
                               
                                 /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////                                  
                              if(tirot[0].x+15<87 && porta1==1 && tirot[0].y<190)
                              {
                                   porta1=0; 
                                   for(x2=0;x2<97;x2++)//porta de tras liberando passagem apos atirar para abrir
                                   {
                                     for(y2=0;y2<133;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportaf,250,130,1000,0);
                              } 
                                                 
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                    
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passof,voz3f,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
            
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
            
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passof,voz3f,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;     
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro               
                                   
                                   
                                   if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                    {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d2.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                    }
                    
                                   
                                   
                                        if(key[KEY_S]) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passof,voz3f,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W]) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passof,voz3f,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                               {
                                                   //if(tiro==0)            
                                                  //  sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptf);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptf);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptf);                                                                                                                                        
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,2,passof,voz3f,roupa,lpulo);     
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                   
                                                                                                                                                                                        
                                                     if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                     {                
                                                             movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passof,voz3f,roupa,lpulo);  
                                                             pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                               
                                                      }
                                                      //  else
                                                         // {                              
                                                             if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                             {
                                                                lpulo=0; 
                                                                sinalpv=0;
                              
                                                             } 
                                                              
                                                         // }     
                                                      
                                                      //////////////////////CONTROLE DE GRAVIDADE
                                                      if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                      {     
                                                         y++;
                                                         
                                                             if(matriz[x][y+1]==0)
                                                               y++;
                                                             if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++;  
                        
                                                      }    
                                                  //////////////////////////CENARIO//////////////////////////////////////////// 
                                                  draw_sprite(bufferf,fundo2f,0,0);  
                                                  
                                                  //draw_sprite(bufferf,soldadoff,150,495-13);
                                                  
                                                  install_int(IncrementaT3,300);
                                                  movimentopersonagem(bufferf,soldadoff,xs,ys,135,480,direcao,0,1);//funcao movimenta soldado
                                                  
                                                  if(a==2)
                                                     show_tiros(x,y,bufferf,3,tirof,tirot,tiroc,matriz,roupa);
                                                   else
                        	                         show_tiros(x,y,bufferf,d,tirof,tirot,tiroc,matriz,roupa);                      
                                                    pf = load_gif(posicao,NULL);
                                                                                         
                                                 
            
                                                  if(roupa==1)     
                                                  { 
                                                    if(!key[KEY_W] && !key[KEY_S])                                  
                                                      draw_sprite(bufferf,pf,x,y-10); 
                                                    else
                                                      if(key[KEY_W])
                                                       draw_sprite(bufferf,pf,x,y-22);
                                                      else
                                                        if(key[KEY_S]) 
                                                         draw_sprite(bufferf,pf,x,y);
                                                  }
                                                  else
                                                   draw_sprite(bufferf,pf,x,y);
                                                           
                                                  
                                                   ////////MONTANDO CENARIO/////////// 
                                                  draw_sprite(bufferf,pilarnf,0,55);
                                                  draw_sprite(bufferf,pilarnf,150,55);
                                                  draw_sprite(bufferf,pilarnf,300,55);
                                                  draw_sprite(bufferf,pilarnf,450,55);
                                                  draw_sprite(bufferf,pilarnf,600,55);
                                                  
                                                  draw_sprite(bufferf,pilarnf,0,564);
                                                  draw_sprite(bufferf,pilarnf,150,564);
                                                  draw_sprite(bufferf,pilarnf,300,564);
                                                  draw_sprite(bufferf,pilarnf,393,564);
                                                  
                                                   draw_sprite(bufferf,pilarnf,0,200);
                                                  draw_sprite(bufferf,pilarnf,150,200);
                                                  draw_sprite(bufferf,pilarnf,280,200);
                                               //   draw_sprite(bufferf,pilarnf,353,198);

                                                  draw_sprite(bufferf,pilarnf,548,286);
                                                  draw_sprite(bufferf,pilarnf,390,429);
                                                  draw_sprite(bufferf,pilarnf,538,429);
                                                //  draw_sprite(bufferf,pilarnf,655,306);
                                                  draw_sprite(bufferf,parededf,685,20);
                                                  draw_sprite(bufferf,parededf,-20,185);
                                                  draw_sprite(bufferf,portaxf,-5,102);
                                                  if(porta1==1)
                                                    draw_sprite(bufferf,fportaf,90,100);

                                                  ///////////////////////////////////////////////
                                                  
                                                  destroy_bitmap(pf);     
                                                  //destroy_bitmap(soldadoff);                             
                                                  //////////////////////////////////////////////////////////////////////                 
                                
                                textprintf_centre_ex(bufferf, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferf, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferf, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferf, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferf, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferf, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferf, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               
                               
                               draw_sprite(bufferf,painelf,0,0);
                               textprintf_centre_ex(bufferf, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                               show_mouse(bufferf);
                               
                               loop=1;
                          }                             
                          ::fps_speed--;
                          ::fps++;
                   }   
                   loop=0;            
                   draw_sprite(screen,bufferf,0,0);//desenha na tela a nova imagem 
                   
               if(x<50 && y==132)//passou cenario
                 x=-1;    
               else
                 if(y>596)//voltou para parte 1 do cenario
                   i=-1;  
               

              vsync();
              // rest(fps/3300);//controle de frames
      }while (x!=-1 && i!=-1 && fim!=-10000);
      
      destroy_bitmap(fundo2f);
      destroy_bitmap(soldadoff);
      destroy_bitmap(pilarf);
      destroy_bitmap(parededf);
      destroy_bitmap(portaxf);
      
      //  destroy_bitmap(fportae);
      destroy_sample(abreportaf);
      destroy_sample(fechaportaf);
      
      //destroy_bitmap(bufferf);
      destroy_bitmap(painelf);
      destroy_sample(passof);
      destroy_sample(voz3f);
      destroy_sample(ptf);
      
     if(x==-1)//passou
     {
       passaporta(0,20,495-30,fps);                
       return 1;//diz que o jogador passando um cenario
     }  
     else
       if(i==-1)
       {
         remove_keyboard();       
         return 0;//voltando um cenario    
       }  
       else
        if(fim==-10000)
         return -10000;  
         
}


int cenario17(int estado,int fps,int roupa)
{
      BITMAP *bufferg,*fundo2g,*fundo1g,*pg,*painelg,*soldadofg,*chaolg,*tetolg,*paredelg,*portalg,*tuboslg,*fportag,*fporta2g; 
      SAMPLE *maquinag;
      SAMPLE *passog,*vozg,*ptg,*abreportag,*fechaportag; 
      char posicao[20];
      int matriz[800][600],time=0;
      Tiro tirof[3],tirot[3],tiroc[3];
       
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      //fps,
      opcao=0,
      kx,
      ky,
      x2=0,
      y2=0,
      porta1=1,//diz se porta da esquerda ta aberta(1 fechado)
      porta2=1,//diz se porta da direita ta aberta(1 fechado)
      tiroflag=0,
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      xcenario,//controla movimento do cenario
      vida=energia,
      tiro,
      xs=-500,//controla a posicao da coluna inicial do soldado se movimentando atraves da funcao
      ys=479,//controla a posicao da linha inicial do soldado se movimentando atraves da funcao
      direcao=2,//controla direcao do soldado movimentando na tela passando por funcao
      posicaosoldado=0,//posicao que o soldado deve aparecer na tela em movimento
      loop=0,//controle de escrita no buffer com o uso do while do fps
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz do pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo
      


      install_keyboard();
      bufferg = create_bitmap(1600,600);
      fundo1g = create_bitmap(1600,600);
      fundo2g = create_bitmap(1600,600);
      
    //  fundo1g = load_gif("fundos/cenario17.gif", NULL); 
      fundo2g = load_gif("fundos/fundobase2.gif", NULL);
      painelg = load_gif("fundos/painel.gif",NULL);
      
      passog = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptg = load_wav("musicas/tirox.wav");
      else
         ptg = load_wav("musicas/tiro.wav");
      vozg = load_wav("musicas/vpulo.wav");
      soldadofg = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadox.gif",NULL);
      
      chaolg = load_gif("fundos/BASEESPACIAL/fd.gif",NULL);
      tetolg = load_gif("fundos/BASEESPACIAL/fd2.gif",NULL);
      paredelg = load_gif("fundos/BASEESPACIAL/pareded.gif",NULL);
      portalg = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      tuboslg = load_gif("fundos/BASEESPACIAL/tubulacao.gif",NULL); 
      
      abreportag = load_wav("musicas/Dooropen.wav");
      fechaportag = load_wav("musicas/Doorclose.wav");
      fportag = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      fporta2g = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      ////////////////////////////////////////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      play_sample(fechaportag,250,130,1000,0); 
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////
               
               for(x=618;x<800;x++)//parede da direita
               {
                 for(y=0;y<455;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<93;x++)//porta de tras trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }  
               
         // strcpy(posicao,"personagem/d1.gif");     
       
                 /////////  
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
    
        
      y=495;  
      if(estado==1)//se 0 eh pq ta voltando,se 1 eh pq ta indo,valor passando por funcao jogo 
      {
        x=657;
        xcenario=-800;//estado inicial do cenario
        strcpy(posicao,"personagem/d1.gif"); 
        d=1;
      }  
      else
        if(estado==0)
        {
          x=95; 
          posicaosoldado=700;
          xcenario=0;//estado inicial do cenario
          strcpy(posicao,"personagem/d2.gif"); 
          d=2;
        }  
        
      
    //  show_mouse(fundo);//coloca mouse no bitmap fundo
      i=1;
      maquinag = load_wav("musicas/maquina.wav");//som da maquina na tela
      play_sample(maquinag,100,130,1000,1); 
      
      
       // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) ); 
      do
      {    
           
           while ( ::fps_speed > 0 )
           { 
                    if(loop==0)
                    {  
                           clear(bufferg);
                           
                           /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                              if(tirof[0].x-15>695 && porta2==1 && tirof[0].y>450)
                              {
                                   porta2=0;
                
                                   for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportag,250,130,1000,0);
                              }  
                              if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                              {
                                   porta1=0; 
                                   for(x2=0;x2<93;x2++)//porta de tras liberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportag,250,130,1000,0);
                              }  
                           
                            
                            if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                    
                          /* //time=clock();
                           if(time>4000)
                           {
                             time=0; 
                             time=clock();
                           }  
                            */
                           tempo=::tempo;//chama a funcao incrementando a cada 1 segundo conforme declarado na funcao jogo
                           if(tempo>2)
                           {
                              tiro=0;
                              tempo=0;
                           }                  
                   
                             
                             if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                             { 
                               if (is_windowed_mode())     
                                   set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                
                               else            
                                   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                             }
                             
                             if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                             {                                               
                                       movimentos(d,a,x,y,mf,mt,posicao,1,passog,vozg,roupa,lpulo); 
                                    
                                      //MOVENDO CENARIO  PARRA TRAS
                                        if(x>330 && a==0 && matriz[x-1][y]==0)
                                        {
                                               x--;
                         
                                               if(x>330 && a==0)
                                                 x--;  
                                               if(x>330 && a==0)
                                                 x--;    
                                               if(x>330 && a==0)
                                                 x--; 
                                               if(x>330 && a==0)
                                                 x--;    
                                               if(x>330 && a==0)
                                                 x--; 
                                               if(x>330 && a==0)
                                                 x--;   
                
                                        }         
                                        else   
                                          if(xcenario<0 && a==0)
                                          {
                                               xcenario=xcenario+6; 
                                               posicaosoldado=posicaosoldado+6;
                                          }     
                                          else
                                           if(a==0 && matriz[x-1][y]==0)
                                           {
                                              x--; 
                                             
                                               for(kx=0;kx<130;kx++)//parede da esqueda matriz
                                               {
                                                 for(ky=0;ky<455;ky++)
                                                   matriz[kx][ky]=1;
                                               }
                                               
                                                    if(a==0 && matriz[x-1][y]==0)
                                                     x--;  
                                                     
                                                    if(a==0 && matriz[x-1][y]==0)
                                                     x--;  
                
                                                     if(a==0 && matriz[x-1][y]==0)
                                                         x--;  
                                                    if(a==0 && matriz[x-1][y]==0)
                                                     x--;  
                                                     
                                                    if(a==0 && matriz[x-1][y]==0)
                                                     x--;  
                
                                                     if(a==0 && matriz[x-1][y]==0)
                                                         x--;     
                
                                           }     
                                 
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0; 
                                    mt=mt+9;//incrementa numero de sprites 
                                    
                                       
                                    d=1;         
                             }
                             else
                               mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                               
                               if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                               {
                                 if(roupa==0)      
                                  strcpy(posicao,"personagem/d1.gif");
                                 else
                                   strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                               }
                             
                               
                                if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                {                           
                                                                                
                                        movimentos(d,a,x,y,mf,mt,posicao,1,passog,vozg,roupa,lpulo); 
                                           
                                        //MOVENDO CENARIO PARA FRENTE
                                        if(xcenario>-800 && x>330 && a==0)
                                        {
                                             xcenario=xcenario-6;     
                                             posicaosoldado=posicaosoldado-6;                               
                                        }     
                                        else
                                          if(a==0 && matriz[x+1][y]==0)
                                          {
                                             x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++; 
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;    
                                                    
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                 if(a==0 && matriz[x+1][y]==0)
                                                   x++; 
                                                 if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                 if(a==0 && matriz[x+1][y]==0)
                                                   x++;    
                                          }     
                
                                  
                                      if(mf>180)//se numero de sprites for o maximo retorna
                                        mf=0; 
                                      mf=mf+9;//incrementa numero de sprites 
                    
                                         
                                      d=2; 
                                }
                                else
                                   mf=0; //se nao for para frente entao M contador de sprites fica o primeiro               
                               
                               
                               if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                {
                                 if(roupa==0)      
                                  strcpy(posicao,"personagem/d2.gif");
                                 else
                                   strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                }
                                
                               
                               
                                    if(key[KEY_S]) //BAIXO
                                    {
                                      a=1;             
                                      movimentos(d,a,x,y,mf,mt,posicao,1,passog,vozg,roupa,lpulo);        
                                    }
                                    else
                                       if(key[KEY_W]) //CIMA
                                       {         
                                          a=2;
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passog,vozg,roupa,lpulo);    
                                        }
                                        else
                                          a=0;
                                          
                                           if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                           {

                                              // time = clock(); 
                                               
                                               if (tiroflag == 0)
                		                       {
                		                        	tiroflag = 1;
                		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptg);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptg);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptg);                                                                                                                                      
                                               }  
                                               movimentos(d,a,x,y,mf,mt,posicao,2,passog,vozg,roupa,lpulo);    
                                            } 
                                            else
                	                       	  tiroflag = 0;
                	                       	                        
                                           
                                            if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                             {                
                                                     movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passog,vozg,roupa,lpulo);  
                                                     pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                       
                                              }
                                              //  else
                                                 // {                              
                                                     if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                     {
                                                        lpulo=0; 
                                                        sinalpv=0;
                      
                                                     } 
                                                      
                                                 // }    
                                              
                                              //////////////////////CONTROLE DE GRAVIDADE
                                              if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                              {     
                                                 y++;
                                                 
                                                     if(matriz[x][y+1]==0)
                                                       y++;
                                                     if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++; 
                                                       if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++;
                                                       if(matriz[x][y+1]==0)
                                                       y++;  
                                                     if(matriz[x][y+1]==0)
                                                       y++; 
                
                                              }     
                                              //////////////////CENARIO////////////////////////////////////////////////////  
                                              draw_sprite(bufferg,fundo2g,xcenario,0);
                                              pg = load_gif(posicao,NULL); 
                                              //draw_sprite(bufferg,fundo1g,xcenario,60);
                                              //draw_sprite(bufferg,fundo2g,xcenario,0);
                                              
                                             // xs=xs+posicaosoldado;
                                              install_int(IncrementaT3,300);
                                              movimentopersonagem(bufferg,soldadofg,xs,ys,-500,630,direcao,posicaosoldado,1);//funcao movimenta soldado
                                              
                                              if(a==2)
                                                show_tiros(x,y,bufferg,3,tirof,tirot,tiroc,matriz,roupa);
                                              else
                	                            show_tiros(x,y,bufferg,d,tirof,tirot,tiroc,matriz,roupa);  
                	                            
                	                          
                                              if(roupa==1)     
                                              { 
                                                if(!key[KEY_W] && !key[KEY_S])                                  
                                                  draw_sprite(bufferg,pg,x,y-10); 
                                                else
                                                  if(key[KEY_W])
                                                   draw_sprite(bufferg,pg,x,y-22);
                                                  else
                                                    if(key[KEY_S]) 
                                                     draw_sprite(bufferg,pg,x,y);
                                              }
                                              else
                                                 draw_sprite(bufferg,pg,x,y);
                                                                 
                                               ////////MONTANDO FUNDO DA FRENTE//////////////
                                              
                                              draw_sprite(bufferg,paredelg,1460+xcenario,-120);
                                              draw_sprite(bufferg,paredelg,0+xcenario,-120);
                                              draw_sprite(bufferg,chaolg,0+xcenario,564);
                                              draw_sprite(bufferg,chaolg,800+xcenario,564);
                                              draw_sprite(bufferg,tetolg,0+xcenario,32);
                                              draw_sprite(bufferg,tetolg,800+xcenario,32);
                                              draw_sprite(bufferg,portalg,1515+xcenario,465);
                                              draw_sprite(bufferg,portalg,-10+xcenario,465);
                                              draw_sprite(bufferg,tuboslg,375+xcenario,165);
                                              
                                               if(porta2==1)//portas fechadas ou abertas
                                                 draw_sprite(bufferg,fportag,1505+xcenario,465);
                                               if(porta1==1) 
                                                 draw_sprite(bufferg,fporta2g,86+xcenario,465);
                                              /////////////////////////////////////////////
                                        
                                              destroy_bitmap(pg); 
                                           //   destroy_bitmap(soldadofg);    
                                                          
                                             ///////////////////////////////////////////////////////////////////
                            textprintf_centre_ex(bufferg, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                           textprintf_centre_ex(bufferg, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                           textprintf_centre_ex(bufferg, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                           textprintf_centre_ex(bufferg, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                           textprintf_centre_ex(bufferg, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);    
                           textprintf_centre_ex(bufferg, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"LPULO :%d",lpulo);
                           textprintf_centre_ex(bufferg, font,50,380, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);                
                        //   textprintf_centre_ex(bufferg, font,50,400, makecol(0, 0, 0), makecol(255, 255, 255)," CLOCK :%d",clock()); 
                           textprintf_centre_ex(bufferg, font,50,420, makecol(0, 0, 0), makecol(255, 255, 255)," TIME :%d",time);
                           textprintf_centre_ex(bufferg, font,50,420, makecol(0, 0, 0), makecol(255, 255, 255)," Estado :%d",estado);
                           textprintf_centre_ex(bufferg, font,50,420, makecol(0, 0, 0), makecol(255, 255, 255)," SinalPV :%d",sinalpv);
                           textprintf_centre_ex(bufferg, font,50,420, makecol(0, 0, 0), makecol(255, 255, 255)," XS :%d",xs);
                           
                           draw_sprite(bufferg,painelg,0,0);
                           textprintf_centre_ex(bufferg, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                           loop=1;
                    }                             
                    ::fps_speed--;
                    ::fps++;
           }   
           loop=0;      
           draw_sprite(screen,bufferg,0,0);//desenha na tela a nova imagem 
           
           
           if(x<40)
            fim=-1;
            
            //rest(fps/3300);//controle de frames
            

            vsync();
      }while (x<740 && fim!=-1 && fim!=-10000);
      
      destroy_bitmap(fundo1g);
      destroy_bitmap(fundo2g);
      destroy_bitmap(bufferg);
      destroy_bitmap(painelg);
      destroy_bitmap(chaolg);
      destroy_bitmap(portalg);
      destroy_bitmap(tetolg);
      destroy_bitmap(paredelg);
      destroy_bitmap(soldadofg);
      destroy_bitmap(tuboslg);
      destroy_sample(passog);
      destroy_sample(maquinag);
      destroy_sample(vozg);
      destroy_sample(ptg);
      
      destroy_bitmap(fportag);
      destroy_bitmap(fporta2g);
      destroy_sample(abreportag);
      destroy_sample(fechaportag);

      if(x>740)
      {
         passaporta(1,20,495-363,fps);
         return 0;//diz que ta indo para o proximo cenario
      }   
      else
        if(fim==-1)
        {  
           passaporta(0,20,495-30,fps);
           return 1; 
        }  
        else
        if(fim==-10000)
         return -10000;
        
    //  remove_keyboard();              
    //  install_keyboard();
      
     
}


int cenario18(int estado,int fps,int ativa,int roupa)
{
      BITMAP *bufferh,*fundo2h,*fundo1h,*ph,*painelh,*elevadorc1h,*elevadorc2h,*elevadorsh,*chaoh,*chao2h,*tetoh,*c1h,*c2h,*portah,*teto2h,*fx1h,*fx2h,*c3h,*c4h,*fportah,*fporta2h; 
      SAMPLE *passoh,*vozh,*pth,*abreportah,*fechaportah;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      opcao=0,
      x2=0,
      y2=0,
      porta1=1,//diz se porta da esquerda ta aberta(1 fechado)
      porta2=1,//diz se porta da direita ta aberta(1 fechado)
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
     // fps,
      vida=energia,
      tiroflag=0,
      tiro,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo

      install_keyboard();
      bufferh = create_bitmap(MAX_X,MAX_Y);
      fundo1h = create_bitmap(MAX_X,MAX_Y);
      fundo2h = create_bitmap(MAX_X,MAX_Y);
      
      fundo2h = load_gif("fundos/fundobase.gif", NULL);
      painelh = load_gif("fundos/painel.gif",NULL);
      elevadorc1h = load_gif("objetos/BASEESPACIAL/elevadorcentro.gif", NULL);
      elevadorc2h = load_gif("objetos/BASEESPACIAL/elevadorcentro2.gif", NULL);
      elevadorsh = load_gif("objetos/BASEESPACIAL/elevadorcx.gif", NULL);
      
      chaoh = load_gif("fundos/BASEESPACIAL/xxx2.gif", NULL);
      chao2h = load_gif("fundos/BASEESPACIAL/xx2.gif", NULL);
      tetoh = load_gif("fundos/BASEESPACIAL/xxx.gif",NULL);
      teto2h = load_gif("fundos/BASEESPACIAL/xx.gif",NULL);
      c1h = load_gif("fundos/BASEESPACIAL/c1.gif", NULL); 
      c4h = load_gif("fundos/BASEESPACIAL/c2.gif", NULL); 
      c2h = load_gif("fundos/BASEESPACIAL/a1.gif", NULL);
      c3h = load_gif("fundos/BASEESPACIAL/a2.gif", NULL);
      fx1h = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2h = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portah = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      
      passoh = load_wav("musicas/passo.wav");
      if(roupa==0)
        pth = load_wav("musicas/tirox.wav");
      else
         pth = load_wav("musicas/tiro.wav");
      
      abreportah = load_wav("musicas/Dooropen.wav");
      fechaportah = load_wav("musicas/Doorclose.wav");
      fportah = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      fporta2h = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      ////////////////////////////////////////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<100;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<113;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<463;y++)
                   matriz[x][y]=1;
               }
               
               for(x=623;x<800;x++)//parede da direita
               {
                 for(y=0;y<458;y++)
                   matriz[x][y]=1;
               }
               
               if(estado==0)//se voltando cenario eh pq ja ativou o elevador antes
                ativa=1;
               
               if(ativa==0)//elevador desativado
               {
                   for(x=320;x<430;x++)//SUPORTE ELEVADOR
                   {
                     for(y=391;y<500;y++)
                       matriz[x][y]=1;
                   }
               }
               else    
                  if(ativa==1)//elevador ativado
                  {  
                       for(x=339;x<420;x++)//SUPORTE ELEVADOR
                       {
                           for(y=303;y<450;y++)
                             matriz[x][y]=1;
                        }
                  }      
                 
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<93;x++)//porta de tras trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
                 
               /////////  
      ////////////////////////////////////////////////////
      
            /////////////MONTANDO CENARIO////////////////////////
           draw_sprite(fundo1h,fundo2h,0,0);
           draw_sprite(fundo1h,fx1h,0,100);
           draw_sprite(fundo1h,fx1h,0,45);
           
           draw_sprite(fundo1h,fx2h,-60,365);
           draw_sprite(fundo1h,fx2h,-60,415);
           draw_sprite(fundo1h,fx2h,-60,310);
           draw_sprite(fundo1h,fx2h,-60,255);
           draw_sprite(fundo1h,fx2h,-60,205);
           draw_sprite(fundo1h,fx2h,-60,155);

           draw_sprite(fundo1h,fx2h,687,365);
           draw_sprite(fundo1h,fx2h,687,415);
           draw_sprite(fundo1h,fx2h,687,310);
           draw_sprite(fundo1h,fx2h,687,255);
           draw_sprite(fundo1h,fx2h,687,155);
           draw_sprite(fundo1h,fx2h,687,205);
           
            draw_sprite(fundo1h,chaoh,0,560);
            draw_sprite(fundo1h,chao2h,118,562);
            draw_sprite(fundo1h,chao2h,178,562);
            draw_sprite(fundo1h,chao2h,238,562);
            draw_sprite(fundo1h,chao2h,298,562);                                                  
            draw_sprite(fundo1h,chao2h,438,562);
            draw_sprite(fundo1h,chao2h,498,562);
            draw_sprite(fundo1h,chao2h,558,562);
            draw_sprite(fundo1h,chao2h,618,562);
            draw_sprite(fundo1h,chaoh,678,562);         
               
               draw_sprite(fundo1h,c1h,90,165);
               draw_sprite(fundo1h,c1h,90,379);
               draw_sprite(fundo1h,c1h,90,225);
               draw_sprite(fundo1h,c3h,90,265);
               draw_sprite(fundo1h,c3h,90,325);
               
               draw_sprite(fundo1h,c2h,670,380);
               draw_sprite(fundo1h,c2h,670,320);
               draw_sprite(fundo1h,c2h,670,260);
               draw_sprite(fundo1h,c2h,670,200);
               draw_sprite(fundo1h,c2h,670,160);
               draw_sprite(fundo1h,c4h,670,380);
               
               draw_sprite(fundo1h,portah,-5,465);
               draw_sprite(fundo1h,portah,700,465);
               draw_sprite(fundo1h,chaoh,0,562);
               
               draw_sprite(fundo1h,tetoh,89,110);
               draw_sprite(fundo1h,tetoh,190,110);
               draw_sprite(fundo1h,tetoh,309,110);
               draw_sprite(fundo1h,tetoh,428,110);
               draw_sprite(fundo1h,tetoh,547,110);
               draw_sprite(fundo1h,teto2h,666,110);
               
               
               draw_sprite(fundo1h,fx2h,688,112);
               draw_sprite(fundo1h,elevadorsh,340,370);
               draw_sprite(fundo1h,fportah,692,465);
               draw_sprite(fundo1h,fporta2h,90,465);
           ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      
      
      if(estado==0)//elevador subindo
        elevador(365,700,495,bufferh,fundo2h,fundo1h,painelh,elevadorc1h,elevadorc2h,estado,fps,0,roupa);

      
      vozh = load_wav("musicas/vpulo.wav");
      
       
       strcpy(posicao,"personagem/d2.gif");
             
      i=1;
      if(estado==1)
      {
        x=643;
        y=495;
        d=1;
        play_sample(fechaportah,250,130,1000,0); 
      }
      else
        if(estado==4)
        {
          x=93;
          y=495;
          d=2;
          play_sample(fechaportah,250,130,1000,0); 
        } 
        else
          if(estado==0)
          {
               if(roupa==0)        
                strcpy(posicao,"personagem/a8.gif");
               else
                 strcpy(posicao,"personagem/ARMADURA/ax8.gif");
                x=372;
                y=495;
          }
     //posicao inicial
      // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {                     
            while ( ::fps_speed > 0 )
            { 
                    if(loop==0)
                    {
                                   clear(bufferh);
                                   
                                   /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                                  if(tirof[0].x-15>695 && porta2==1 && tirof[0].y>450)
                                  {
                                       porta2=0;
                    
                                       for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                       {
                                         for(y2=440;y2<496;y2++)
                                           matriz[x2][y2]=0;
                                       }
                                       play_sample(abreportah,250,130,1000,0);
                                  }  
                                  if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                                  {
                                       porta1=0; 
                                       for(x2=0;x2<93;x2++)//porta de tras liberando passagem apos atirar para abrir
                                       {
                                         for(y2=440;y2<496;y2++)
                                           matriz[x2][y2]=0;
                                       }
                                       play_sample(abreportah,250,130,1000,0);
                                  } 
                                   
                                   
                                   if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                     
                                     if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                     { 
                                       if (is_windowed_mode())     
                                           set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                        
                                       else            
                                           set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                     }
                                     
                                     if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                     {                                               
                                               movimentos(d,a,x,y,mf,mt,posicao,1,passoh,vozh,roupa,lpulo); 
                                            
                                                   if(a==0 && matriz[x-1][y]==0)
                                                     x--;  
                        
                                                     if(a==0 && matriz[x-1][y]==0)
                                                       x--;   
                                                     if(a==0 && matriz[x-1][y]==0)
                                                       x--;  
                                                     if(a==0 && matriz[x-1][y]==0)
                                                       x--; 
                                                     if(a==0 && matriz[x-1][y]==0)
                                                       x--;  
                                                     if(a==0 && matriz[x-1][y]==0)
                                                       x--;    
                                                   
                                        if(mt>180)//se numero de sprites for o maximo retorna
                                         mt=0;  
                                                               
                                         mt=mt+9; 
                        
                                        d=1;         
                                     }
                                     else
                                       mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                        
                                       
                                       if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                       {
                                         if(roupa==0)      
                                          strcpy(posicao,"personagem/d1.gif");
                                         else
                                           strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                       }
                                       
                                       
                                       
                                        if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                        {                           
                                                                           
                                                movimentos(d,a,x,y,mf,mt,posicao,1,passoh,vozh,roupa,lpulo); 
                                                   
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++; 
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;
                                                      if(a==0 && matriz[x+1][y]==0)
                                                        x++;  
                                                      if(a==0 && matriz[x+1][y]==0)
                                                        x++;  
                                                      if(a==0 && matriz[x+1][y]==0)
                                                        x++;  
                                                      if(a==0 && matriz[x+1][y]==0)
                                                        x++;      
                        
                                          
                                              if(mf>180)//se numero de sprites for o maximo retorna
                                                mf=0; 
                                              mf=mf+9;
                                              d=2; 
                                        }
                                        else
                                           mf=0; //se nao for para frente entao M contador de sprites fica o primeiro                
                                       
                                       
                                       if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                        {
                                         if(roupa==0)      
                                          strcpy(posicao,"personagem/d2.gif");
                                         else
                                           strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                        }
                        
                                       
                                       
                                            if(key[KEY_S]) //BAIXO
                                            {
                                              a=1;             
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passoh,vozh,roupa,lpulo);      
                                            }
                                            else
                                               if(key[KEY_W]) //CIMA
                                               {                               
                                                  a=2;
                                                  movimentos(d,a,x,y,mf,mt,posicao,1,passoh,vozh,roupa,lpulo);                                      
                                                }
                                                else
                                                  a=0;
                                                  
                                                   if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                                   {
                                                     //  if(tiro==0)            
                                                     //   sinalpv=2;
                                                       
                                                       if (tiroflag == 0)
                        		                       {
                        		                        	tiroflag = 1;
                        		                        	
                        		                        	if(a==2)
                                                              atirar(x,y,3,tirof,tirot,tiroc,pth);
                                                            else
                                                              if(a==1)
                                                                atirar(x,y+18,d,tirof,tirot,tiroc,pth);//tiro agaixado
                                                              else  
                                                                atirar(x,y,d,tirof,tirot,tiroc,pth);                                                                                                                                       
                                                       }  
                                                       movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoh,vozh,roupa,lpulo);     
                                                    } 
                                                    else
                        	                       	  tiroflag = 0;
                        	                       
                        	                                 
                                                                                                                                                                                            
                                                     if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                     {                
                                                             movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoh,vozh,roupa,lpulo);  
                                                             pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                               
                                                      }
                                                      //  else
                                                         // {                              
                                                             if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                             {
                                                                lpulo=0; 
                                                                sinalpv=0;
                              
                                                             } 
                                                              
                                                         // }     
                                                      
                                                      //////////////////////CONTROLE DE GRAVIDADE
                                                      if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                      {     
                                                         y++;
                                                         
                                                             if(matriz[x][y+1]==0)
                                                               y++;
                                                             if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++; 
                                                               if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++;
                                                               if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++; 
                        
                                                      }     
                                                      /////////////////////CENARIO/////////////////////////////////////////////////    
                                                       draw_sprite(bufferh,fundo2h,0,0); 
                                                       
                                                       
                                                       ph = load_gif(posicao,NULL);
                                                                                 
                                                      if(roupa==1)     
                                                      { 
                                                        if(!key[KEY_W] && !key[KEY_S])                                  
                                                          draw_sprite(bufferh,ph,x,y-10); 
                                                        else
                                                          if(key[KEY_W])
                                                           draw_sprite(bufferh,ph,x,y-22);
                                                          else
                                                            if(key[KEY_S]) 
                                                             draw_sprite(bufferh,ph,x,y);
                                                      }
                                                      else
                                                       draw_sprite(bufferh,ph,x,y);
                                                       
                                                       if(a==2)
                                                          show_tiros(x,y,bufferh,3,tirof,tirot,tiroc,matriz,roupa);
                                                        else
                            	                          show_tiros(x,y,bufferh,d,tirof,tirot,tiroc,matriz,roupa);
                            	                          
                                                       /////////////MONTANDO CENARIO////////////////////////
                                                       draw_sprite(bufferh,fx1h,0,100);
                                                       draw_sprite(bufferh,fx1h,0,45);
                                                       
                                                       draw_sprite(bufferh,fx2h,-60,365);
                                                       draw_sprite(bufferh,fx2h,-60,415);
                                                       draw_sprite(bufferh,fx2h,-60,310);
                                                       draw_sprite(bufferh,fx2h,-60,255);
                                                       draw_sprite(bufferh,fx2h,-60,205);
                                                       draw_sprite(bufferh,fx2h,-60,155);
                                            
                                                       draw_sprite(bufferh,fx2h,687,365);
                                                       draw_sprite(bufferh,fx2h,687,415);
                                                       draw_sprite(bufferh,fx2h,687,310);
                                                       draw_sprite(bufferh,fx2h,687,255);
                                                       draw_sprite(bufferh,fx2h,687,155);
                                                       draw_sprite(bufferh,fx2h,687,205);
                                                       
                                                        draw_sprite(bufferh,chaoh,0,560);
                                                        draw_sprite(bufferh,chao2h,118,562);
                                                        draw_sprite(bufferh,chao2h,178,562);
                                                        draw_sprite(bufferh,chao2h,238,562);
                                                        draw_sprite(bufferh,chao2h,298,562);                                                  
                                                        draw_sprite(bufferh,chao2h,438,562);
                                                        draw_sprite(bufferh,chao2h,498,562);
                                                        draw_sprite(bufferh,chao2h,558,562);
                                                        draw_sprite(bufferh,chao2h,618,562);
                                                        draw_sprite(bufferh,chaoh,678,562);         
                                                           
                                                           draw_sprite(bufferh,c1h,90,165);
                                                           draw_sprite(bufferh,c1h,90,379);
                                                           draw_sprite(bufferh,c1h,90,225);
                                                           draw_sprite(bufferh,c3h,90,265);
                                                           draw_sprite(bufferh,c3h,90,325);
                                                           
                                                           draw_sprite(bufferh,c2h,670,380);
                                                           draw_sprite(bufferh,c2h,670,320);
                                                           draw_sprite(bufferh,c2h,670,260);
                                                           draw_sprite(bufferh,c2h,670,200);
                                                           draw_sprite(bufferh,c2h,670,160);
                                                           draw_sprite(bufferh,c4h,670,380);
                                                           
                                                           draw_sprite(bufferh,portah,-5,465);
                                                           draw_sprite(bufferh,portah,700,465);
                                                           if(porta2==1)//portas fechadas ou abertas
                                                            draw_sprite(bufferh,fportah,692,465);
                                                           if(porta1==1) 
                                                            draw_sprite(bufferh,fporta2h,90,465);
                                                           
                                                           draw_sprite(bufferh,chaoh,0,562);
                                                           
                                                           draw_sprite(bufferh,tetoh,89,110);
                                                           draw_sprite(bufferh,tetoh,190,110);
                                                           draw_sprite(bufferh,tetoh,309,110);
                                                           draw_sprite(bufferh,tetoh,428,110);
                                                           draw_sprite(bufferh,tetoh,547,110);
                                                           draw_sprite(bufferh,teto2h,666,110);
                                                           
                                                           
                                                           draw_sprite(bufferh,fx2h,688,112);
                                                       ////////////////////////////////////////////////////
                                                   
                                                       if(i%2==0)//ELEVADOR
                                                       {
                                                          draw_sprite(bufferh,elevadorc1h,365,552);
                                                          i=1;
                                                       }   
                                                        else 
                                                          {
                                                            draw_sprite(bufferh,elevadorc2h,365,552);
                                                            i=0;
                                                           }       
                                                                           

                                                      if(ativa==0)
                                                        draw_sprite(bufferh,elevadorsh,340,460); 
                                                      else  
                                                       if(ativa==1)                                             
                                                         draw_sprite(bufferh,elevadorsh,340,370);    
                                                     
                                                     
                                                      destroy_bitmap(ph);                                  
                                                      /////////////////////////////////////////////////////                 
                                    
                                    textprintf_centre_ex(bufferh, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                                   textprintf_centre_ex(bufferh, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                                   //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                                   textprintf_centre_ex(bufferh, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                                   textprintf_centre_ex(bufferh, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                                   textprintf_centre_ex(bufferh, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                                   textprintf_centre_ex(bufferh, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                                   textprintf_centre_ex(bufferh, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                                   
                                   
                                   draw_sprite(bufferh,painelh,0,0);
                                   textprintf_centre_ex(bufferh, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                                  // show_mouse(bufferh);
                                  loop=1;
                        }                             
                        ::fps_speed--;
                        ::fps++;
                }   
                loop=0;               
                   draw_sprite(screen,bufferh,0,0);//desenha na tela a nova imagem 
                   
               if(x>363 && x<392 && y==495 && key[KEY_S])//apertou para baixo no elevador
               {
                 x=-1;    
                 strcpy(posicao,"personagem/a8.gif");
               }  
               else
                 if(x>700)//voltou cenario
                   i=-1;
                 else
                   if(x<10)
                    y=-1;  
                      
                   //rest(fps/3300);//controle de frames
                    vsync();
      }while (x!=-1 && i!=-1 && y!=-1 && fim!=-10000);
      
     if(i==-1)//diz que o jogador ta voltando um cenario
     {
           passaporta(1,20,495-30,fps);
           destroy_bitmap(fundo1h);
           destroy_bitmap(fundo2h);
           destroy_bitmap(bufferh);
           destroy_bitmap(painelh);
           destroy_bitmap(elevadorc1h);
           destroy_bitmap(elevadorc2h);
           destroy_bitmap(elevadorsh);
           
           destroy_bitmap(chaoh);
           destroy_bitmap(chao2h);
           destroy_bitmap(tetoh);
           destroy_bitmap(teto2h);
           destroy_bitmap(portah);
           destroy_bitmap(fx1h);
           destroy_bitmap(fx2h);
           destroy_bitmap(c1h);
           destroy_bitmap(c2h);
           destroy_bitmap(c3h);
           destroy_bitmap(c4h);
           
           destroy_sample(passoh);
           destroy_sample(vozh);
           destroy_sample(pth);
           
           return 0;
     }  
      else
       if(x==-1)
       {
          elevador(365,495,700,bufferh,fundo2h,fundo1h,painelh,elevadorc1h,elevadorc2h,1,fps,0,roupa); //elevador descendo
          
           destroy_bitmap(fundo1h);
           destroy_bitmap(fundo2h);
           destroy_bitmap(bufferh);
           destroy_bitmap(painelh);
           destroy_bitmap(elevadorc1h);
           destroy_bitmap(elevadorc2h);
           destroy_bitmap(elevadorsh);
                     
           destroy_bitmap(chaoh);
           destroy_bitmap(chao2h);
           destroy_bitmap(tetoh);
           destroy_bitmap(teto2h);
           destroy_bitmap(portah);
           destroy_bitmap(fx1h);
           destroy_bitmap(fx2h);
           destroy_bitmap(c1h);
           destroy_bitmap(c2h);
           destroy_bitmap(c3h);
           destroy_bitmap(c4h);
           
           destroy_sample(passoh);
           destroy_sample(vozh);
           destroy_sample(pth);
          return 1;
       }   
       else
         if(y==-1)//chama tela segurança 
         {
            destroy_bitmap(fundo1h);
           destroy_bitmap(fundo2h);
           destroy_bitmap(bufferh);
           destroy_bitmap(painelh);
           destroy_bitmap(elevadorc1h);
           destroy_bitmap(elevadorc2h);
           destroy_bitmap(elevadorsh);
                     
           destroy_bitmap(chaoh);
           destroy_bitmap(chao2h);
           destroy_bitmap(tetoh);
           destroy_bitmap(teto2h);
           destroy_bitmap(portah);
           destroy_bitmap(fx1h);
           destroy_bitmap(fx2h);
           destroy_bitmap(c1h);
           destroy_bitmap(c2h);
           destroy_bitmap(c3h);
           destroy_bitmap(c4h);
           
           destroy_sample(passoh);
           destroy_sample(vozh);
           destroy_sample(pth);              
           return 3;
         }  
         else
          if(fim==-10000)
           return -10000;
}

int cenario19(int fps,int &ativa,int roupa)
{
      BITMAP *bufferi,*fundo2i,*fundo1i,*pi,*paineli,*chaoi,*chao2i,*tetoi,*f3i,*f4i,*f5i,*portai,*teto2i,*fx1i
      ,*fx2i,*c3i,*caixai,*fxi,*painelai,*painela2i,*fportai; 
      
      SAMPLE *passoi,*vozi,*pti,*ativandoi,*abreportai,*fechaportai;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta=1,//controla se porta aberto ou fechado 1 eh fechado
      opcao=0,
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
     // fps,
      vida=energia,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      tiroflag=0,
      tiro,
      somativa=0,//diz se mostra ou nao som de ativando o painel do elevador
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo


      install_keyboard();
      bufferi = create_bitmap(MAX_X,MAX_Y);
      fundo1i = create_bitmap(MAX_X,MAX_Y);
      fundo2i = create_bitmap(MAX_X,MAX_Y);

      fundo2i = load_gif("fundos/fundobase.gif", NULL);
      paineli = load_gif("fundos/painel.gif",NULL);
      
      passoi = load_wav("musicas/passo.wav");
      if(roupa==0)
        pti = load_wav("musicas/tirox.wav");
      else
         pti = load_wav("musicas/tiro.wav");
      ativandoi = load_wav("musicas/ativando.wav");
      
      caixai = load_gif("objetos/BASEESPACIAL/caixax.gif",NULL);
      chaoi = load_gif("fundos/BASEESPACIAL/fd.gif", NULL);
      chao2i = load_gif("fundos/BASEESPACIAL/xx2.gif", NULL);
      painelai = load_gif("objetos/BASEESPACIAL/painelx.gif", NULL);
      painela2i = load_gif("objetos/BASEESPACIAL/painelx2.gif", NULL);
      tetoi = load_gif("fundos/BASEESPACIAL/fd2.gif",NULL);
      teto2i = load_gif("fundos/BASEESPACIAL/f3.gif",NULL);
      f3i = load_gif("fundos/BASEESPACIAL/fd3.gif", NULL); 
      f4i = load_gif("fundos/BASEESPACIAL/fd4.gif", NULL);
      f5i = load_gif("fundos/BASEESPACIAL/fd5.gif", NULL);
      fx1i = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2i = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portai = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      fxi = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      
      abreportai = load_wav("musicas/Dooropen.wav");
      fechaportai = load_wav("musicas/Doorclose.wav");
      fportai = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      ////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      play_sample(fechaportai,250,130,1000,0); 
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<150;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<133;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=658;x<800;x++)//parede da direita
               {
                 for(y=0;y<458;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=100;x<185;x++)//painel
               {
                 for(y=427;y<502;y++)
                   matriz[x][y]=1;
               }
               
               for(x=275;x<595;x++)//3 caixas abaixo
               {
                 for(y=430;y<505;y++)
                   matriz[x][y]=1;
               }
               for(x=370;x<500;x++)//caixa em cima das 3
               {
                 for(y=365;y<450;y++)
                   matriz[x][y]=1;
               }
                 
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               /////////  
      ////////////////////////////////////////////////////

       
       if(ativa==0)
       {
         draw_sprite(bufferi,painelai,130,480);
         somativa=0;
       }  
       else
         {
           draw_sprite(bufferi,painela2i,130,480); 
           somativa=1; 
         }  
       ////////////////////////////////////////////////////

      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////

      
      vozi = load_wav("musicas/vpulo.wav");
      
       
       strcpy(posicao,"personagem/d2.gif");
       pi = load_gif(posicao,NULL);
       draw_sprite(bufferi,pi,55,0);
       draw_sprite(screen,bufferi,0,0);//desenha na tela a nova imagem
       
       
      i=1;

      x=643;
      y=495;

      d=1;//posicao inicial
      
      // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
               while ( ::fps_speed > 0 )
               { 
                    if(loop==0)
                    {
                               clear(bufferi);
                               
                               /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                              if(tirof[0].x-15>695 && porta==1 && tirof[0].y>450)
                              {
                                   porta=0;
                
                                   for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportai,250,130,1000,0);
                              }   
                               
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                 
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passoi,vozi,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
                    
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
                    
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passoi,vozi,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;     
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro                
                                   
                                   
                                  
                                    if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                    {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d2.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                    }
                    
                                   
                                   
                                        if(key[KEY_S]) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passoi,vozi,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W]) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passoi,vozi,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                               {
                                                   //if(tiro==0)            
                                                  //  sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,pti);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,pti);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,pti);                                                                                                                                       
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoi,vozi,roupa,lpulo); 
                                                  // rest(10);    
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                 
                                                                                                                                                                                        
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoi,vozi,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }     
                                                  
                                                  //////////////////////CONTROLE DE GRAVIDADE
                                                  if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                  {     
                                                     y++;
                                                     
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                         if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                    
                                                  }      
                                                  /////////////////////CENARIO/////////////////////////////////////////////////    
                                                   draw_sprite(bufferi,fundo2i,0,0); 
                                                 //  draw_sprite(bufferi,fundo1i,0,0); 
                                                    
                                                    pi = load_gif(posicao,NULL);
                                                                
                                                    if(a==2)
                                                      show_tiros(x,y,bufferi,3,tirof,tirot,tiroc,matriz,roupa);
                                                    else
                        	                          show_tiros(x,y,bufferi,d,tirof,tirot,tiroc,matriz,roupa);
                                                                           
                                                  if(roupa==1)     
                                                  { 
                                                    if(!key[KEY_W] && !key[KEY_S])                                  
                                                      draw_sprite(bufferi,pi,x,y-10); 
                                                    else
                                                      if(key[KEY_W])
                                                       draw_sprite(bufferi,pi,x,y-22);
                                                      else
                                                        if(key[KEY_S]) 
                                                         draw_sprite(bufferi,pi,x,y);
                                                  } 
                                                  else
                                                    draw_sprite(bufferi,pi,x,y);
                                                    
                                                   /////////////MONTANDO CENARIO////////////////////////
                    
                                                       
                                                       draw_sprite(bufferi,fx1i,0,100);
                                                       draw_sprite(bufferi,fx1i,0,45);
                                                       draw_sprite(bufferi,f4i,80,10);
                                                       draw_sprite(bufferi,f5i,700,10);             
                                                       draw_sprite(bufferi,chaoi,0,564);
                                                       draw_sprite(bufferi,tetoi,0,55);                                                   
                                                       
                                                       draw_sprite(bufferi,fxi,-90,365);
                                                       draw_sprite(bufferi,fxi,-90,410);
                                                       draw_sprite(bufferi,fxi,-90,310);
                                                       draw_sprite(bufferi,fxi,-90,255);
                                                       draw_sprite(bufferi,fxi,-90,205);
                                                       draw_sprite(bufferi,fxi,-90,160);
                                                       draw_sprite(bufferi,fxi,-90,510);
                                                       draw_sprite(bufferi,fxi,-90,460);
                                                      
                                                       draw_sprite(bufferi,fxi,750,365);
                                                       draw_sprite(bufferi,fxi,750,420);
                                                       draw_sprite(bufferi,fxi,750,310);
                                                       draw_sprite(bufferi,fxi,750,255);
                                                       draw_sprite(bufferi,fxi,750,160);
                                                       draw_sprite(bufferi,fxi,750,205);  
                                                       
                                                       draw_sprite(bufferi,caixai,407,430);
                                                       draw_sprite(bufferi,caixai,307,495); 
                                                       draw_sprite(bufferi,caixai,407,495);
                                                       draw_sprite(bufferi,caixai,507,495);
                                                       
                                                       draw_sprite(bufferi,portai,700,463);
                                                       if(porta==1)//portas fechadas ou abertas
                                                         draw_sprite(bufferi,fportai,695,465);
                                                         
                                                       if(ativa==0)
                                                         draw_sprite(bufferi,painelai,130,480);
                                                       else
                                                         draw_sprite(bufferi,painela2i,130,480);  
                                                       ////////////////////////////////////////////////////

                                                  destroy_bitmap(pi);                                  
                                                  /////////////////////////////////////////////////////                 
                                
                                textprintf_centre_ex(bufferi, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferi, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferi, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferi, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferi, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferi, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferi, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               
                               
                               draw_sprite(bufferi,paineli,0,0);
                               textprintf_centre_ex(bufferi, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                               //show_mouse(bufferi);
                               loop=1;
                          }                             
                          ::fps_speed--;
                          ::fps++;
               }   
               loop=0;               
               draw_sprite(screen,bufferi,0,0);//desenha na tela a nova imagem 
               
               
             if(x>720)//voltou cenario
               i=-1;
             
             if(x==185)
             {
               ativa=1;//ativou para abrir elevador na fase atras  
               if(somativa==0)
               {
                 play_sample(ativandoi,250,130,1000,0);
                 somativa=1;
               }  
             }  
        
            // rest(fps/3300);//controle de frames
            vsync();
    }while (i!=-1 && x!=-1 && fim!=-10000);
     
      destroy_bitmap(fundo2i);
      destroy_bitmap(bufferi);
      destroy_bitmap(paineli);
      
      destroy_bitmap(chaoi);
      destroy_bitmap(chao2i);
      destroy_bitmap(tetoi);
      destroy_bitmap(teto2i);
      destroy_bitmap(f3i);
      destroy_bitmap(f4i);
      destroy_bitmap(f5i);
      destroy_bitmap(portai);
      destroy_bitmap(fx1i);
      destroy_bitmap(fx2i);
    //  destroy_bitmap(c3i);
      destroy_bitmap(caixai);
      destroy_bitmap(fxi);
      destroy_bitmap(painelai);
      destroy_bitmap(painela2i);
      
      destroy_sample(passoi);
      destroy_sample(vozi);
      destroy_sample(pti);  
      
      //destroy_bitmap(fportai);
      destroy_sample(abreportai);
      destroy_sample(fechaportai);
      
 
     if(i==-1)//diz que o jogador ta voltando um cenario
     {
       passaporta(1,20,495-30,fps);
       return 4;//diz pra funcao jogo pra chamar a funcao 18 que ta voltano desse cenario
     }  
     else
       if(fim==-10000)
          return -10000;
      
    
}


int cenario110(int estado,int fps,int roupa,int &cenamonitor,int cenacapitao)
{
      BITMAP *bufferj,*fundo2j,*fundo1j,*pj,*painelj,*elevadorc1j,*elevadorc2j,*elevadorb1j,*elevadorb2j
      ,*imaquinasj,*imaquinas2j,*cientista1j,*cientista2j,*cientista3j,*cientista4j,*cenaj,*mensagemxj
      ,*soldadofj,*soldadofj2,*chaojg,*tetojg,*paredejg,*portajg,*teto2jg,*fportaj,*fporta2j; 
      
      SAMPLE pmj,*maquinaj;
      SAMPLE *passoj,*vozj,*ptj,*abreportaj,*fechaportaj; 
      char posicao[20];
      int matriz[800][600],time=0,ielevador=0,ipainel;//ilevador controla posicao do elevador,ipainel posicao da tela e pcs
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      //fps,
      opcao=0,
      kx,
      ky,
      x2=0,
      y2=0,
      porta1=1,//diz se porta da esquerda ta aberta(1 fechado)
      porta2=1,//diz se porta da direita ta aberta(1 fechado)
      tiroflag=0,
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      i=0,i2,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      xcenario,//controla movimento do cenario
      vida=energia,
      tiro,
      xs=100,//controla a posicao da coluna inicial do soldado se movimentando atraves da funcao
      ys=479,//controla a posicao da linha inicial do soldado se movimentando atraves da funcao
      direcao=2,//controla direcao do soldado movimentando na tela passando por funcao
      posicaosoldado=-500,//posicao que o soldado deve aparecer na tela em movimento
      loop=0,//controle de escrita no buffer com o uso do while do fps
      mensagem=0,//auxiliar da variavel passada por parametro cenamonitor
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz do pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo


      install_keyboard();
      bufferj = create_bitmap(1600,600);
      fundo1j = create_bitmap(1600,600);
      fundo2j = create_bitmap(1600,600);
      
     // fundo1j = load_gif("fundos/cenario1102.gif", NULL); 
      fundo2j = load_gif("fundos/fundobase2.gif", NULL);
      painelj = load_gif("fundos/painel.gif",NULL);
      imaquinasj = load_gif("objetos/BASEESPACIAL/maquinas.gif", NULL);
      imaquinas2j = load_gif("objetos/BASEESPACIAL/maquinas2.gif", NULL);
      elevadorc1j = load_gif("objetos/BASEESPACIAL/elevadorcentro.gif", NULL);
      elevadorc2j = load_gif("objetos/BASEESPACIAL/elevadorcentro2.gif", NULL);
      elevadorb1j = load_gif("objetos/BASEESPACIAL/elevador1.gif", NULL);
      elevadorb2j = load_gif("objetos/BASEESPACIAL/elevador2.gif", NULL);
      cientista1j = load_gif("objetos/BASEESPACIAL/cientista1.gif", NULL);
      cientista2j = load_gif("objetos/BASEESPACIAL/cientista2.gif", NULL);
      cientista3j = load_gif("objetos/BASEESPACIAL/cientista3.gif", NULL);
      cientista4j = load_gif("objetos/BASEESPACIAL/cientista4.gif", NULL);
      cenaj = load_gif("objetos/BASEESPACIAL/cenamonitor.gif", NULL);
      soldadofj = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadox.gif",NULL);
      soldadofj2 = load_gif("objetos/BASEESPACIAL/soldadof.gif",NULL);
      mensagemxj = load_gif("objetos/BASEESPACIAL/mensagem.gif", NULL);
      
      chaojg = load_gif("fundos/BASEESPACIAL/fd.gif",NULL);
      tetojg = load_gif("fundos/BASEESPACIAL/fd2.gif",NULL);
      teto2jg = load_gif("fundos/BASEESPACIAL/fd2x.gif",NULL);
      paredejg = load_gif("fundos/BASEESPACIAL/pareded.gif",NULL);
      portajg = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      
      passoj = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptj = load_wav("musicas/tirox.wav");
      else
         ptj = load_wav("musicas/tiro.wav");                        		
      vozj = load_wav("musicas/vpulo.wav");
      
       abreportaj = load_wav("musicas/Dooropen.wav");
      fechaportaj = load_wav("musicas/Doorclose.wav");
      fportaj = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      fporta2j = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      ////////////////////////////////////////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////
               
               for(x=618;x<800;x++)//parede da direita
               {
                 for(y=0;y<465;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }  
               
               for(x=0;x<93;x++)//porta de tras trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               
               if(cenacapitao==-1)
               {
                    for(x=300;x<326;x++)//soldado barrando para que samus va falar com capitao primeiro
                   {
                     for(y=0;y<500;y++)
                       matriz[x][y]=1;
                   }
               }   
                             
       
                 /////////  
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      if(estado==0)
        xcenario=-800;//estado inicial do cenario
      else
        xcenario=0;  
        
      ///////////FUNDO CENARIO 1/////////////
      draw_sprite(fundo1j,fundo2j,xcenario,0);
      
      draw_sprite(fundo1j,imaquinas2j,226,159);//posicao de maquinas pc conforme movimento na ipainel  
      draw_sprite(fundo1j,elevadorc2j,365,543);
      draw_sprite(fundo1j,elevadorb1j,325,465);
      
      draw_sprite(fundo1j,paredejg,1460+xcenario,-120);
      draw_sprite(fundo1j,paredejg,xcenario,-120);
      draw_sprite(fundo1j,chaojg,xcenario,564);
      draw_sprite(fundo1j,chaojg,800+xcenario,564);
      draw_sprite(fundo1j,tetojg,0+xcenario,50);
      draw_sprite(fundo1j,teto2jg,800+xcenario,50);
      draw_sprite(fundo1j,portajg,1515+xcenario,465);
      draw_sprite(fundo1j,portajg,-10+xcenario,465);    
      
       draw_sprite(fundo1j,fportaj,1505+xcenario,465);
       draw_sprite(fundo1j,fporta2j,86+xcenario,465);  
                                                      
      if(cenacapitao==-1)
        draw_sprite(fundo1j,soldadofj,1051,479);
      else
        draw_sprite(fundo1j,soldadofj2,1051,479);
          
      draw_sprite(fundo1j,cientista2j,925,487);

      
      
      //////////////////////////////////////////////////////  

      ///////////////////////////////ELEVADOR DECENDO/////////////////////////////
             if(estado==1)
               elevador2(365,0,490,325,bufferj,fundo2j,fundo1j,painelj,elevadorc1j,elevadorc2j,estado,elevadorb1j,elevadorb2j,fps,-800,roupa);                          
       
      //////////////////////////////////////////////////////////////////////////////
      //destroy_bitmap(fundo1j);
    //  fundo1j = load_gif("fundos/cenario110.gif", NULL);
      
      if(estado==10)
      {
        x=100;
        xcenario=0;//estado inicial do cenario
        ipainel=+800;
        ielevador=+800;
        d=2;//posicao inicial
        
        if(roupa==0)  
            strcpy(posicao,"personagem/d2.gif");
          else
            strcpy(posicao,"personagem/ARMADURA/dx2.gif");
            
        mensagem=0;
        tempo2=3;//coloca valor 3 para tempo 2 para que nao mostre a mensagem novamente
        play_sample(fechaportaj,250,130,1000,0); 
      }  
      else 
        if(estado==1)
        {
          x=374;
          d=1;
          ipainel=0;
          
           if(roupa==0)  
            strcpy(posicao,"personagem/a8.gif");
          else
            strcpy(posicao,"personagem/ARMADURA/ax8.gif"); 
            
          //ielevador=+800;
          xcenario=-800;//estado inicial do cenario
        } 
        else
          if(estado==0)
          {
            x=654;
            d=1;
            ipainel=0;
            
            if(roupa==0)  
              strcpy(posicao,"personagem/d1.gif");
            else
             strcpy(posicao,"personagem/ARMADURA/dx1.gif");
             play_sample(fechaportaj,250,130,1000,0); 
            xcenario=-800;//estado inicial do cenario
          } 
        y=495;
         
       
        
     
      
    //  show_mouse(fundo);//coloca mouse no bitmap fundo
      i=1;
      maquinaj = load_wav("musicas/Operation Deck.wav");//som da maquina na tela
      play_sample(maquinaj,100,130,1000,1);  
      
       // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
            
            while ( ::fps_speed > 0 )
            { 
                    if(loop==0)
                    {       
                                   //clock();
                                   clear(bufferj);
                                   
                                   /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                                  if(tirof[0].x-15>695 && porta2==1 && tirof[0].y>450)
                                  {
                                       porta2=0;
                    
                                       for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                       {
                                         for(y2=440;y2<496;y2++)
                                           matriz[x2][y2]=0;
                                       }
                                       play_sample(abreportaj,250,130,1000,0);
                                  }  
                                  if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                                  {
                                       porta1=0; 
                                       for(x2=0;x2<93;x2++)//porta de tras liberando passagem apos atirar para abrir
                                       {
                                         for(y2=440;y2<496;y2++)
                                           matriz[x2][y2]=0;
                                       }
                                       play_sample(abreportaj,250,130,1000,0);
                                  }  
                                    
                                   if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                
                                   
                                     if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                     { 
                                       if (is_windowed_mode())     
                                           set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                        
                                       else            
                                           set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                     }
                                     
                                     if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W] && mensagem==0) //TRAS
                                     {                                               
                                               movimentos(d,a,x,y,mf,mt,posicao,1,passoj,vozj,roupa,lpulo); 
                                            
                                              //MOVENDO CENARIO  PARRA TRAS
                                                if(x>326 && a==0 && matriz[x-1][y]==0)
                                                {
                                                       x--;
                                                       if(matriz[x-1][y]==0)
                                                         x--;  
                                                       if(matriz[x-1][y]==0)
                                                         x--;    
                                                       if(matriz[x-1][y]==0)
                                                         x--;  
                                                       if(matriz[x-1][y]==0)
                                                         x--;    
                                                       if(matriz[x-1][y]==0)
                                                         x--;   
                                                }         
                                                else   
                                                  if(xcenario<0 && a==0 && matriz[x-1][y]==0)
                                                  {     
                                                      xcenario=xcenario+6; 
                                                         if(xcenario>-800) 
                                                         {
                                                           ielevador=ielevador+6;
                                                           ipainel=ipainel+6;
                                                           posicaosoldado=posicaosoldado+6;
                                                         }
                                                  }  
                                                  else
                                                   if(a==0 && matriz[x-1][y]==0)//se nao for x>300 para mover cenario e o cenario ja movido entao x anda para tras
                                                   {
                                                      x--; 
                                                            
                                                            for(kx=0;kx<130;kx++)//parede da esqueda matriz redesenha a matriz colocando a parede da esqueda
                                                            {
                                                              for(ky=0;ky<455;ky++)
                                                                matriz[kx][ky]=1;
                                                            }
                                                            
                                                            if(a==0 && matriz[x-1][y]==0)
                                                             x--;  
                                                             
                                                            if(a==0 && matriz[x-1][y]==0)
                                                             x--;  
                                  
                                                             if(a==0 && matriz[x-1][y]==0)
                                                                 x--;  
                                                            if(a==0 && matriz[x-1][y]==0)
                                                             x--;  
                                  
                                                             if(a==0 && matriz[x-1][y]==0)
                                                                 x--; 
                                                    }
                                                  
                                            if(mt>200)//se numero de sprites for o maximo retorna
                                             mt=0; 
                                            mt=mt+9;//incrementa numero de sprites 
                                            
                                               
                                            d=1;         
                                     }
                                     else
                                       mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                               
                                    
                                       
                                       if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE] && mensagem==0)//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                       {
                                         if(roupa==0)      
                                          strcpy(posicao,"personagem/d1.gif");
                                         else
                                           strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                       }   
                                      // else
                                         //strcpy(posicao,"personagem/p1.gif"); // SE Y NAO FOR IGUAL O 495 CHAO ENTAO MOSTRA IMAGEM DE PULO
                                       
                                        if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W] && mensagem==0) //FRENTE
                                        {                           
                                                                                        
                                                movimentos(d,a,x,y,mf,mt,posicao,1,passoj,vozj,roupa,lpulo); 
                                                   
                                                //MOVENDO CENARIO PARA FRENTE
                                                if(xcenario>-800 && x>326 && a==0)
                                                {
                                                     xcenario=xcenario-6;
                                                     ielevador=ielevador-6; 
                                                     ipainel=ipainel-6;   
                                                     posicaosoldado=posicaosoldado-6;                                                          
                                                }  
                                                else
                                                  if(a==0 && matriz[x+1][y]==0)
                                                  {
                                                        x++;  
                                                          if(a==0 && matriz[x+1][y]==0)
                                                            x++;  
                                                            
                                                          if(a==0 && matriz[x+1][y]==0)
                                                            x++;  
                                                            
                                                         if(a==0 && matriz[x+1][y]==0)
                                                           x++; 
                                                         if(a==0 && matriz[x+1][y]==0)
                                                            x++;  
                                                            
                                                         if(a==0 && matriz[x+1][y]==0)
                                                           x++;        
                                                  }     
                        
                                          if(mf>200)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;//incrementa numero de sprites 
                                             
                                          d=2; 
                                        }
                                        else
                                           mf=0; //se nao for para frente entao M contador de sprites fica o primeiro               
                                       
                                       
                                       if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE] && mensagem==0)//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                        {
                                         if(roupa==0)      
                                          strcpy(posicao,"personagem/d2.gif");
                                         else
                                           strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                       } 
                          
                                       
                                       
                                            if(key[KEY_S] && mensagem==0) //BAIXO
                                            {
                                              a=1;             
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passoj,vozj,roupa,lpulo);        
                                            }
                                            else
                                               if(key[KEY_W] && mensagem==0) //CIMA
                                               {         
                                                  a=2;
                                                  movimentos(d,a,x,y,mf,mt,posicao,1,passoj,vozj,roupa,lpulo);    
                                                }
                                                else
                                                  a=0;
                                                  
                                                   if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE] && mensagem==0) //TIRO
                                                   {
                                                      // if(tiro==0)            
                                                     //   sinalpv=2;
                                                       
                                                      // time = clock(); 
                                                       
                                                       if (tiroflag == 0)
                        		                       {
                        		                        	tiroflag = 1;

                        		                        	if(a==2)
                                                              atirar(x,y,3,tirof,tirot,tiroc,ptj);
                                                            else
                                                              if(a==1)
                                                                atirar(x,y+18,d,tirof,tirot,tiroc,ptj);//tiro agaixado
                                                              else  
                                                                atirar(x,y,d,tirof,tirot,tiroc,ptj);                                                                                                                                        
                                                       }  
                                                       movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoj,vozj,roupa,lpulo);    
                                                    } 
                                                    else
                        	                       	    tiroflag = 0; 
                        	                       	                        
                                                   
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0 && mensagem==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoj,vozj,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }     
                                                      
                                                      //////////////////////CONTROLE DE GRAVIDADE
                                                      if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                      {     
                                                         y++;
                                                         
                                                             if(matriz[x][y+1]==0)
                                                               y++;
                                                             if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++; 
                                                               if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++;
                                                               if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++; 
                        
                                                      }     
                                                      //////////////////CENARIO////////////////////////////////////////////////////  
                                                      
                                                      
                                                      draw_sprite(bufferj,fundo2j,xcenario,0);
                                                      
                                                        if(i2%2==0)//ELEVADOR BASE
                                                        {
                                                          draw_sprite(bufferj,elevadorb1j,ielevador+325,465);//posicao do elevador conforme movimento do cenario na ilevador
                                                          i2=1;
                                                        }   
                                                        else 
                                                          {
                                                            draw_sprite(bufferj,elevadorb2j,ielevador+325,465);//posicao do elevador conforme movimento do cenario na ilevador
                                                            i2=0;
                                                           }   
                                                       if(i%2==0)//ELEVADOR
                                                       {
                                                          draw_sprite(bufferj,imaquinasj,ipainel-574,159);//posicao de maquinas pc conforme movimento na ipainel                 
                                                          draw_sprite(bufferj,elevadorc1j,ielevador+365,543);//posicao do elevador conforme movimento do cenario na ilevador
                                                          i=1;
                                                       }   
                                                        else 
                                                          {
                                                            draw_sprite(bufferj,imaquinas2j,ipainel-574,159);//posicao de maquinas pc conforme movimento na ipainel  
                                                            draw_sprite(bufferj,elevadorc2j,ielevador+365,543);//posicao do elevador conforme movimento do cenario na ilevador
                                                            i=0;
                                                           }                                                         
                        	                            
                        	                           
                        	                          install_int(IncrementaT, 2000);
                        	                          if(tempo==0)//MAQUINAS CIENTISTAS
                                                       {
                                                          draw_sprite(bufferj,cientista1j,ipainel+125,487);                                  
                                                          draw_sprite(bufferj,cientista3j,ipainel-330,487);
                                                          
                                                          //i=1;
                                                       }   
                                                        else 
                                                          if(tempo==1)
                                                          {                                       
                                                            draw_sprite(bufferj,cientista2j,ipainel+125,487);
                                                            draw_sprite(bufferj,cientista4j,ipainel-330,487);
                                                            //i=0;
                                                           } 
                                                           
                                                     /////////////////CENA MONITOR/////////////////////////////////////////
                                                     if(xcenario>-365 && xcenario<-355 && cenamonitor==0 && roupa==0)//cena monitor
                                                     {
                                                             //   if(roupa==1)
                                                                 mensagem=1;
                                                              
                                                           //   if(mensagem==1)  
                                                          //    {
                                                                     draw_sprite(bufferj,cenaj,279,284); 
                                                                     draw_sprite(bufferj,mensagemxj,0,170);
                                                                     strcpy(posicao,"personagem/a00.gif");
                                                                     
                                                                     if(key[KEY_SPACE] && tempo2>2)
                                                                     {
                                                                       cenamonitor=1;
                                                                       mensagem=0;
                                                                     }  
                                                                     
                                                                      textprintf_centre_ex(bufferj, font,380,185, makecol(0, 255, 0), makecol(0, 0, 0),"Samus captamos o sinal de uma nave pirata vindo em direção");
                                                                      textprintf_centre_ex(bufferj, font,380,205, makecol(0, 255, 0), makecol(0, 0, 0),"da Base Espacial.Use a armadura da Federação Galática que esta"); 
                                                                      textprintf_centre_ex(bufferj, font,380,225, makecol(0, 255, 0), makecol(0, 0, 0),"no laboratório a frente,pegue-a e defenda a colônia..rapido!!");
                                                                      
                                                                      if(tempo2>2)
                                                                       textprintf_centre_ex(bufferj, font,650,250, makecol(0, 255, 255), makecol(0, 0, 0),"Pressione SPACE para continuar!!");
                                                                      
                                                                      install_int(IncrementaT2, 2800);                                                                                                                            
                                                               //}
                                                               ////////////////////////////////////////////////////////////////////// 
                                                     }
                                                     
                                                      if(cenacapitao==-1)//soldado que barra para nao passar
                                                        draw_sprite(bufferj,soldadofj,ielevador+252,479);
                                                      else  
                                                        draw_sprite(bufferj,soldadofj2,ielevador+252,479);
                                                      
                                                      
                                                      install_int(IncrementaT3,300);
                                                      movimentopersonagem(bufferj,soldadofj2,xs,ys,60,530,direcao,posicaosoldado,1);//funcao movimenta soldado
                                                      
                                                      if(a==2)
                                                        show_tiros(x,y,bufferj,3,tirof,tirot,tiroc,matriz,roupa);
                                                      else
                        	                            show_tiros(x,y,bufferj,d,tirof,tirot,tiroc,matriz,roupa); 
                        	                            
                                                      pj = load_gif(posicao,NULL); 
                                                      if(roupa==1)     
                                                      { 
                                                        if(!key[KEY_W] && !key[KEY_S])                                  
                                                          draw_sprite(bufferj,pj,x,y-10); 
                                                        else
                                                          if(key[KEY_W])
                                                           draw_sprite(bufferj,pj,x,y-22);
                                                          else
                                                            if(key[KEY_S]) 
                                                             draw_sprite(bufferj,pj,x,y);
                                                      }  
                                                      else
                                                       draw_sprite(bufferj,pj,x,y);  
                                                      
                                                      ////////MONTANDO FUNDO DA FRENTE//////////////
                                              
                                                      draw_sprite(bufferj,paredejg,1460+xcenario,-120);
                                                      draw_sprite(bufferj,paredejg,0+xcenario,-120);
                                                      draw_sprite(bufferj,chaojg,0+xcenario,564);
                                                      draw_sprite(bufferj,chaojg,800+xcenario,564);
                                                      draw_sprite(bufferj,tetojg,0+xcenario,50);
                                                      draw_sprite(bufferj,teto2jg,800+xcenario,50);
                                                      draw_sprite(bufferj,portajg,1515+xcenario,465);
                                                      draw_sprite(bufferj,portajg,-10+xcenario,465);
                                                      
                                                      if(porta2==1)//portas fechadas ou abertas
                                                         draw_sprite(bufferj,fportaj,1505+xcenario,465);
                                                       if(porta1==1) 
                                                         draw_sprite(bufferj,fporta2j,86+xcenario,465);
                                                      /////////////////////////////////////////////
                                                         
                                                  //    draw_sprite(bufferj,fundo1j,xcenario,64);  
                                                                                        
                                                      destroy_bitmap(pj); 
                                                     // destroy_bitmap(soldadofj2);    
                                                                  
                                                     ///////////////////////////////////////////////////////////////////
                                    textprintf_centre_ex(bufferj, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                                   textprintf_centre_ex(bufferj, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                                   textprintf_centre_ex(bufferj, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                                   textprintf_centre_ex(bufferj, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                                   textprintf_centre_ex(bufferj, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);    
                                   textprintf_centre_ex(bufferj, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"LPULO :%d",lpulo);
                                   textprintf_centre_ex(bufferj, font,50,380, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);                
                                   textprintf_centre_ex(bufferj, font,50,400, makecol(0, 0, 0), makecol(255, 255, 255)," CLOCK :%d",clock()); 
                                   textprintf_centre_ex(bufferj, font,50,420, makecol(0, 0, 0), makecol(255, 255, 255)," TIME :%d",time);
                                   textprintf_centre_ex(bufferj, font,50,440, makecol(0, 0, 0), makecol(255, 255, 255)," Pele :%d",ielevador); 
                                   textprintf_centre_ex(bufferj, font,50,460, makecol(0, 0, 0), makecol(255, 255, 255)," Ppcs :%d",ipainel);
                                   textprintf_centre_ex(bufferj, font,30,480, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                                   textprintf_centre_ex(bufferj, font,30,500, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                                    //vsync();
                                   draw_sprite(bufferj,painelj,0,0);
                                   textprintf_centre_ex(bufferj, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                                   loop=1;
                            }                             
                            ::fps_speed--;
                            ::fps++;
                   }   
                   loop=0;                
                   draw_sprite(screen,bufferj,0,0);//desenha na tela a nova imagem 
                   
                   
                   if(x>710)//avança cenario
                    fim=-1;
                   else 
                    if(x>363 && x<392 && y==495 && key[KEY_W])//apertou para cima no elevador
                     x=-1000; 
                    else
                      if(x<40)//entra no cenario neutro q nao retorna 1 nem 0 no estado e sim um numero referente a essa tela
                       i=-1; 
                    
                    //rest(fps/3300);//controle de frames
                     vsync();
      }while (x!=-1000 && fim!=-1 && i!=-1 && fim!=-10000);    
      
      if(x==-1000)//voltando cenario pelo elevador
      {
                // fundo1j = load_gif("fundos/cenario1102.gif", NULL); 
                  ///////////FUNDO CENARIO 1/////////////
              //draw_sprite(fundo1j,fundo2j,xcenario,0);
              clear(fundo1j);
              draw_sprite(fundo1j,fundo2j,xcenario,0);
              draw_sprite(fundo1j,imaquinas2j,-574,159);//posicao de maquinas pc conforme movimento na ipainel  
              draw_sprite(fundo1j,elevadorc2j,-574,543);
              draw_sprite(fundo1j,elevadorb1j,-574,465);
              
              draw_sprite(fundo1j,paredejg,1460+xcenario,-120);
              draw_sprite(fundo1j,paredejg,xcenario,-120);
              draw_sprite(fundo1j,chaojg,xcenario,564);
              draw_sprite(fundo1j,chaojg,800+xcenario,564);
              draw_sprite(fundo1j,tetojg,0+xcenario,50);
              draw_sprite(fundo1j,teto2jg,800+xcenario,50);
              draw_sprite(fundo1j,portajg,1505+xcenario,465);
              draw_sprite(fundo1j,portajg,xcenario,465);    
              draw_sprite(fundo1j,fportaj,1505+xcenario,465);
                                                       
                                                              
              if(cenacapitao==-1)
                draw_sprite(fundo1j,soldadofj,252,479);
              else
                draw_sprite(fundo1j,soldadofj2,252,479);
                  
              draw_sprite(fundo1j,cientista2j,128,487);
        

                  //////////////////////////////////////////////////////                
              elevador2(365,490,-100,325,bufferj,fundo2j,fundo1j,painelj,elevadorc1j,elevadorc2j,0,elevadorb1j,elevadorb2j,fps,0,roupa);
    
             stop_sample(maquinaj); 
             
              destroy_bitmap(fundo1j);
              destroy_bitmap(fundo2j);
              destroy_bitmap(bufferj);
              destroy_bitmap(painelj);
              destroy_bitmap(soldadofj);
              //destroy_bitmap(soldadofj2);
              destroy_sample(passoj);
              destroy_sample(maquinaj);
              destroy_bitmap(imaquinasj);
              destroy_bitmap(imaquinas2j);
              destroy_bitmap(elevadorc1j);
              destroy_bitmap(elevadorc2j);
              destroy_bitmap(elevadorb1j);
              destroy_bitmap(elevadorb2j);
    
              destroy_bitmap(cientista1j);
              destroy_bitmap(cientista2j);
              destroy_bitmap(cientista3j);
              destroy_bitmap(cientista4j);
              destroy_bitmap(cenaj);
              destroy_bitmap(mensagemxj);
              
              destroy_bitmap(chaojg);
              destroy_bitmap(tetojg);
              destroy_bitmap(teto2jg);
              destroy_bitmap(paredejg);
              destroy_bitmap(portajg);
              
              destroy_sample(vozj);
              destroy_sample(ptj);
              
              destroy_bitmap(fportaj);
              destroy_bitmap(fporta2j);
              destroy_sample(abreportaj);
              destroy_sample(fechaportaj);
              
             
             return 0;
      }   
      else
        if(fim==-1)//avança
        {  
           passaporta(1,20,495-30,fps);
           stop_sample(maquinaj);
           
           destroy_bitmap(fundo1j);
          destroy_bitmap(fundo2j);
          destroy_bitmap(bufferj);
          destroy_bitmap(painelj);
          destroy_sample(passoj);
          destroy_bitmap(soldadofj);
          //destroy_bitmap(soldadofj2);
          destroy_sample(maquinaj);

          destroy_bitmap(elevadorc1j);
          destroy_bitmap(elevadorc2j);
          destroy_bitmap(elevadorb1j);
          destroy_bitmap(elevadorb2j);
                    
          destroy_bitmap(chaojg);
          destroy_bitmap(tetojg);
          destroy_bitmap(teto2jg);
          destroy_bitmap(paredejg);
          destroy_bitmap(portajg);
          
          destroy_bitmap(cientista1j);
          destroy_bitmap(cientista2j);
          destroy_bitmap(cientista3j);
          destroy_bitmap(cientista4j);
          destroy_bitmap(cenaj);
          destroy_bitmap(mensagemxj);
          destroy_sample(vozj);
          destroy_sample(ptj);
          
            destroy_bitmap(fportaj);
              destroy_bitmap(fporta2j);
              destroy_sample(abreportaj);
              destroy_sample(fechaportaj);
          
           return 1; 
        }  
        else
          if(i==-1)//entrando sala alternativa
          {               
             stop_sample(maquinaj); 
            //  destroy_bitmap(fundo1j);
              destroy_bitmap(fundo2j);
              destroy_bitmap(bufferj);
              destroy_bitmap(painelj);
              destroy_bitmap(soldadofj);
             // destroy_bitmap(soldadofj2);
              destroy_sample(passoj);
              destroy_sample(maquinaj);
              destroy_bitmap(imaquinasj);
              destroy_bitmap(imaquinas2j);
              destroy_bitmap(elevadorc1j);
              destroy_bitmap(elevadorc2j);
              destroy_bitmap(elevadorb1j);
              destroy_bitmap(elevadorb2j);
                        
              destroy_bitmap(chaojg);
              destroy_bitmap(tetojg);
              destroy_bitmap(teto2jg);
              destroy_bitmap(paredejg);
              destroy_bitmap(portajg);
          
              destroy_bitmap(cientista1j);
              destroy_bitmap(cientista2j);
              destroy_bitmap(cientista3j);
              destroy_bitmap(cientista4j);
              destroy_bitmap(cenaj);
              destroy_bitmap(mensagemxj);
              destroy_sample(vozj);
              destroy_sample(ptj);
              
              //destroy_bitmap(fportaj);
             // destroy_bitmap(fporta2j);
              destroy_sample(abreportaj);
              destroy_sample(fechaportaj);
            
            return 5;
          }  
          else
           if(fim==-10000)
            return -10000;
    //  remove_keyboard();              
    //  install_keyboard();
      
}


int cenario1102(int estado,int fps,int &roupa,MIDI *musicaxx,SAMPLE *musica2,int &ativacena)
{
      BITMAP *bufferl,*fundo2l,*fundo1l,*pl,*painell,*cientista1l,*cientista2l,*pc1l,*pc2l,*armadural,*luzl,
      *cena1l,*cena2l,*auxcenal,*luzcena1l,*luzcena2l,*fcena2l,*navepiratal,*metroid1l,*metroid2l,*piratal1,
      *piratal2,*voadorl,*chaol,*chao2l,*tetol,*f3l,*f4l,*f5l,*portal,*teto2l,*fx1l,*fx2l,*fxl,*mmaquinal,*fportal; 
      
      SAMPLE *passol,*vozl,*voz2l,*ptl,*choquel,*abreportal,*fechaportal;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta=1,//controla se porta aberto ou fechado 1 eh fechado
      opcao=0,
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      ci=0,//controla loop cena
     // fps,
      vida=energia,
      tiroflag=0,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      tiro,
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo

      install_keyboard();
      bufferl = create_bitmap(MAX_X,MAX_Y);
      fundo1l = create_bitmap(MAX_X,MAX_Y);
      fundo2l = create_bitmap(MAX_X,MAX_Y);
      
      fundo1l = load_gif("fundos/cenario111.gif", NULL); 
      fundo2l = load_gif("fundos/fundobase.gif", NULL);
      painell = load_gif("fundos/painel.gif",NULL);
      cientista1l = load_gif("objetos/BASEESPACIAL/cientista1.gif", NULL);
      cientista2l = load_gif("objetos/BASEESPACIAL/cientista2.gif", NULL);
      mmaquinal = load_gif("objetos/BASEESPACIAL/marmadura.gif", NULL);
      pc1l = load_gif("objetos/BASEESPACIAL/pcx1.gif", NULL);
      pc2l = load_gif("objetos/BASEESPACIAL/pcx2.gif", NULL);
      armadural = load_gif("personagem/armadura/xx.gif", NULL);
      luzl = load_gif("objetos/BASEESPACIAL/luzx.gif", NULL);
      cena1l = load_gif("fundos/cena1.gif", NULL);//imagem para animacao ao pegar armadura
      cena2l = load_gif("fundos/cenario1.gif", NULL);
      fcena2l = load_gif("fundos/fcenario1.gif", NULL);
      navepiratal = load_gif("objetos/BASEESPACIAL/navepirata.gif", NULL);
      metroid1l = load_gif("objetos/BASEESPACIAL/metroid1.gif", NULL);
      metroid2l = load_gif("objetos/BASEESPACIAL/metroid2.gif", NULL); 
      piratal1 = load_gif("objetos/BASEESPACIAL/pirata1.gif", NULL);
      piratal2 = load_gif("objetos/BASEESPACIAL/pirata2.gif", NULL);
      voadorl = load_gif("objetos/BASEESPACIAL/voador.gif",NULL);
      passol = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptl = load_wav("musicas/tirox.wav");
      else
         ptl = load_wav("musicas/tiro.wav");
      luzcena1l = load_gif("objetos/BASEESPACIAL/luzcena1.gif",NULL);
      luzcena2l = load_gif("objetos/BASEESPACIAL/luzcena2.gif",NULL);
      voz2l = load_wav("musicas/entradav1.wav");
      choquel = load_wav("musicas/eletro.wav");
      
      chaol = load_gif("fundos/BASEESPACIAL/fd.gif", NULL);
      chao2l = load_gif("fundos/BASEESPACIAL/xx2.gif", NULL);
      tetol = load_gif("fundos/BASEESPACIAL/fd2.gif",NULL);
      teto2l = load_gif("fundos/BASEESPACIAL/f3.gif",NULL);
      f3l = load_gif("fundos/BASEESPACIAL/fd3.gif", NULL); 
      f4l = load_gif("fundos/BASEESPACIAL/fd4.gif", NULL);
      f5l = load_gif("fundos/BASEESPACIAL/fd5.gif", NULL);
      fx1l = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2l = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portal = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      fxl = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL); 
      tempo2==0;//zera tempo
      ////////////////////////////
      abreportal = load_wav("musicas/Dooropen.wav");
      fechaportal = load_wav("musicas/Doorclose.wav");
      fportal = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      ////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      play_sample(fechaportal,250,130,1000,0); 
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<150;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<70;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=638;x<800;x++)//parede da direita
               {
                 for(y=0;y<448;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=105;x<185;x++)//pino do meio do tanque
               {
                 for(y=443;y<502;y++)
                   matriz[x][y]=1;
               }
               
               for(x=100;x<260;x++)//parte de cima do tanque da armadura
               {
                 for(y=100;y<380;y++)
                   matriz[x][y]=1;
               }
               for(x=70;x<250;x++)//parte de baixo do tanque
               {
                 for(y=455;y<505;y++)
                   matriz[x][y]=1;
               }
               
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
                 
               /////////  
      ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      
      
       vozl = load_wav("musicas/vpulo.wav");
       strcpy(posicao,"personagem/d2.gif");
       
       
      i=1;

      x=643;
      y=495;

      d=1;//posicao inicial
      
       // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
           while ( ::fps_speed > 0 )
           { 
                    if(loop==0)
                    { 
                               clear(bufferl);
                               
                                /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                              if(tirof[0].x-15>695 && porta==1 && tirof[0].y>450)
                              {
                                   porta=0;
                
                                   for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportal,250,130,1000,0);
                              } 
                               
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                 
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passol,vozl,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
                    
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;    
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
                    
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passol,vozl,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;   
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;    
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro                
                                   
                                   
                                   if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                    {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d2.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                    } 
                    
                                   
                                   
                                        if(key[KEY_S]) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passol,vozl,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W]) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passol,vozl,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                               {
                                                  // if(tiro==0)            
                                                 //   sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptl);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptl);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptl);                                                                                                                                       
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passol,vozl,roupa,lpulo); 
                                                  // rest(10);    
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                 
                                                                                                                                                                                        
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passol,vozl,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // } 
                           
                                                  //////////////////////////////CENA AO PEGAR ARMADURA/////////////////////////////////////
                                                
                                                          if(x<155 && roupa==0 && ativacena==0)
                                                          {        
                                                                 stop_sample(musica2);//para musica da funcao jogo  
                                                                 destroy_sample(musica2);//para musica da funcao jogo
                                                                  
                                                                 play_sample(voz2l,250,130,1000,0);                                                                       
                                                                // play_sample(musicaxx,180,130,1000,1); 
                                                                 play_midi(musicaxx,TRUE); 
                                                                 play_sample(choquel,200,130,1000,1); 
                                                                 install_int(IncrementaT, 100);
                                                                 for(ci=-580;ci<100;ci++)
                                                                 {
                                                                    while ( ::fps_speed > 0 )
                                                                    {     
                                                                      if(loop==0)
                                                                      {                      
                                                                        clear(bufferl);                                                                                       
                                                                        draw_sprite(bufferl,cena1l,0,ci);              
                                                                        if(tempo==1)//ELEVADOR
                                                                              draw_sprite(bufferl,luzcena1l,85,470);//luz piscando subindo em samus
                                                                        else 
                                                                          draw_sprite(bufferl,luzcena2l,85,470);//luz piscando subindo em samus  
                                                                          
                                                                          loop=1;  
                                                                          ::fps_speed--;
                                                                          ::fps++;
                                                                      }                                                                      
                                                                      loop=0;
                                                                    }       
                                                                    

                                                                        draw_sprite(screen,bufferl,0,0);
                                                                        //rest(fps/5000);//controle de frames;    
                                                                         vsync();   
                                                                         ci++;         
                                                                }  
                                                                 stop_sample(choquel); 
                                                                 stop_sample(voz2l);
                                                                                              
                                                                 
                                                                 /////////////////////////////////////////   
                                                                 roupa=1;    
                                                                 ptl = load_wav("musicas/tiro.wav");                                                                                                               
                                                          }    
                                                          else
                                                            if(x>500 && roupa==1 && ativacena==0) //cena nave pirata chegando//////////////////////
                                                            {                                                             
                                                                 for(ci=-100;ci<250;ci++)
                                                                 {
                                                                      while ( ::fps_speed > 0 )
                                                                      {     
                                                                            if(loop==0)
                                                                            {
                                                                                clear(bufferl);
                                                                                draw_sprite(bufferl,fcena2l,-650,-1220);//fundo cenario1                                                                                       
                                                                                draw_sprite(bufferl,cena2l,-650,-1220); //parte da frente cenario1             
                                                                   
                                                                                draw_sprite(bufferl,navepiratal,85,ci);//nave pirata descendo no cenario1   
                                                                                
                                                                                loop=1;
                                                                             }
                                                                             ::fps_speed--;
                                                                             ::fps++;
                                                                      }   
                                                                      loop=0;
                                                                      
                                                                        draw_sprite(screen,bufferl,0,0);
                                                                        //rest(fps/5000);//controle de frames;   
                                                                         vsync();     
                                                                         ci++;        
                                                                 }     
                                                                 
                                                                 destroy_sample(voz2l);//para musica
                                                                 voz2l = load_wav("musicas/metroid.wav");  
                                                                 play_sample(voz2l,250,130,1000,1); 
                                                                 ///metroid saindo da nave
                                                                 install_int(IncrementaT, 1000);//movimenta cientista
                                                                 for(ci=-580;ci<1000;ci++)//metroids saindo da nave
                                                                 {
                                                                    while ( ::fps_speed > 0 )
                                                                    {     
                                                                        if(loop==0)
                                                                        {
                                                                                clear(bufferl);
                                                                                draw_sprite(bufferl,fcena2l,-650,-1220);//fundo cenario1                                                                                       
                                                                                draw_sprite(bufferl,cena2l,-650,-1220); //parte da frente cenario1             
                                                                                draw_sprite(bufferl,navepiratal,85,250);//nave pirata descendo no cenario1
                                                                                                                                                                 
                                                                                if(tempo==1)//metroids se movendo e pirata
                                                                                {
                                                                                   draw_sprite(bufferl,metroid1l,ci+60,50);
                                                                                   draw_sprite(bufferl,metroid1l,ci+100,150);
                                                                                   draw_sprite(bufferl,metroid1l,ci+250,250);
                                                                                   draw_sprite(bufferl,metroid1l,ci+300,350);
                                                                                   draw_sprite(bufferl,metroid1l,ci+350,450);                    
                                                                                   draw_sprite(bufferl,metroid1l,ci+100,50);
                                                                                   draw_sprite(bufferl,metroid1l,ci+150,150);
                                                                                   draw_sprite(bufferl,metroid1l,ci+200,250);
                                                                                   draw_sprite(bufferl,metroid1l,ci+100,350);
                                                                                   draw_sprite(bufferl,metroid1l,ci+150,450);
                                                                                   draw_sprite(bufferl,metroid1l,ci-100,50);
                                                                                   draw_sprite(bufferl,metroid1l,ci-150,150);
                                                                                   draw_sprite(bufferl,metroid1l,ci-200,250);
                                                                                   draw_sprite(bufferl,metroid1l,ci-100,350);
                                                                                   draw_sprite(bufferl,metroid1l,ci-150,450);
                                                                                   
                                                                                   draw_sprite(bufferl,piratal1,ci+400,470);
                                                                                   draw_sprite(bufferl,piratal1,ci+500,470);
                                                                                   draw_sprite(bufferl,piratal1,ci+600,470);
                                                                                   draw_sprite(bufferl,piratal1,ci+700,470);
                                                                                   draw_sprite(bufferl,piratal1,ci+800,470);
                                                                                }   
                                                                                else 
                                                                                  {
                                                                                      draw_sprite(bufferl,metroid2l,ci+60,50);
                                                                                      draw_sprite(bufferl,metroid2l,ci+100,150);
                                                                                      draw_sprite(bufferl,metroid2l,ci+250,250);
                                                                                      draw_sprite(bufferl,metroid2l,ci+300,350);
                                                                                      draw_sprite(bufferl,metroid2l,ci+350,450);                                     
                                                                                      draw_sprite(bufferl,metroid2l,ci+100,50);
                                                                                      draw_sprite(bufferl,metroid2l,ci+150,150);
                                                                                      draw_sprite(bufferl,metroid2l,ci+200,250);
                                                                                      draw_sprite(bufferl,metroid2l,ci+100,350);
                                                                                      draw_sprite(bufferl,metroid2l,ci+150,450);
                                                                                      draw_sprite(bufferl,metroid2l,ci-100,50);
                                                                                      draw_sprite(bufferl,metroid2l,ci-150,150);
                                                                                      draw_sprite(bufferl,metroid2l,ci-200,250);
                                                                                      draw_sprite(bufferl,metroid2l,ci-100,350);
                                                                                      draw_sprite(bufferl,metroid2l,ci-150,450);
                                                                                      
                                                                                      draw_sprite(bufferl,piratal2,ci+400,470);
                                                                                      draw_sprite(bufferl,piratal2,ci+500,470);
                                                                                      draw_sprite(bufferl,piratal2,ci+600,470);
                                                                                      draw_sprite(bufferl,piratal2,ci+700,470);
                                                                                      draw_sprite(bufferl,piratal2,ci+800,470);
                                                                                   } 
                                                                                   draw_sprite(bufferl,voadorl,ci+650,100);
                                                                                   draw_sprite(bufferl,voadorl,ci+550,200);
                                                                                   draw_sprite(bufferl,voadorl,ci+450,100);
                                                                                   draw_sprite(bufferl,voadorl,ci+350,200);
                                                                                   
                                                                                   loop=1;
                                                                         }       
                                                                        ::fps_speed--;
                                                                        ::fps++;
                                                                   }  
                                                                   loop=0;
                                                                   
                                                                        draw_sprite(screen,bufferl,0,0);
                                                                        //rest(fps/5000);//controle de frames; 
                                                                         vsync();    
                                                                         ci++;          
                                                                 }                                                      
                                                                 stop_sample(voz2l);
                                                                 destroy_sample(voz2l);//para musica da funcao jogo  
                                                                 ativacena=1; 
                                                            }  
                                                               
                                                                                 
                                                  ////////////////////////////////////////////////////////////////////////////////////////
                
                                                              //////////////////////CONTROLE DE GRAVIDADE
                                                              if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                              {     
                                                                 y++;
                                                                 
                                                                     if(matriz[x][y+1]==0)
                                                                       y++;
                                                                     if(matriz[x][y+1]==0)
                                                                       y++;  
                                                                     if(matriz[x][y+1]==0)
                                                                       y++; 
                                                                       if(matriz[x][y+1]==0)
                                                                       y++;  
                                                                     if(matriz[x][y+1]==0)
                                                                       y++;
                                                                       if(matriz[x][y+1]==0)
                                                                       y++;  
                                                                     if(matriz[x][y+1]==0)
                                                                       y++; 
                                
                                                              }     
                                                              /////////////////////CENARIO/////////////////////////////////////////////////    
                                                               draw_sprite(bufferl,fundo2l,0,0); 
                                                               
                                                               if(i%2==0)//ELEVADOR
                                                               {
                                                                  draw_sprite(bufferl,pc1l,400,390);
                                                                  draw_sprite(bufferl,luzl,152,403);//luz piscando encima da armadura
                                                                  draw_sprite(bufferl,luzl,152,499);//luz piscando embaixo da armadura
                                                                  i=1;
                                                               }   
                                                                else 
                                                                  {
                                                                    draw_sprite(bufferl,pc2l,400,390);
                                                                    i=0;
                                                                   }  
                                                                   
                                                                  install_int(IncrementaT, 1000);//movimenta cientista
                                                                  if(tempo==1)                                      
                                                                      draw_sprite(bufferl,cientista1l,450,487);
                                                                  else    
                                                                      draw_sprite(bufferl,cientista2l,450,487);
                                                                                  
                                                                pl = load_gif(posicao,NULL);
                                                                            
                                                                if(a==2)
                                                                  show_tiros(x,y,bufferl,3,tirof,tirot,tiroc,matriz,roupa);
                                                                else
                                    	                          show_tiros(x,y,bufferl,d,tirof,tirot,tiroc,matriz,roupa);
                                    	                          
                                                              if(roupa==1)     
                                                              { 
                                                                if(!key[KEY_W] && !key[KEY_S])                                  
                                                                  draw_sprite(bufferl,pl,x,y-10); 
                                                                else
                                                                  if(key[KEY_W])
                                                                   draw_sprite(bufferl,pl,x,y-22);
                                                                  else
                                                                    if(key[KEY_S]) 
                                                                     draw_sprite(bufferl,pl,x,y);
                                                              }  
                                                              else
                                                               draw_sprite(bufferl,pl,x,y);  
                                                              
                                                              
                                                              /////////////MONTANDO CENARIO////////////////////////
                    
                                                       
                                                               draw_sprite(bufferl,fx1l,0,100);
                                                               draw_sprite(bufferl,fx1l,0,45);
                                                               draw_sprite(bufferl,f4l,30,10);
                                                               draw_sprite(bufferl,f5l,700,10);             
                                                               draw_sprite(bufferl,chaol,0,564);
                                                               draw_sprite(bufferl,tetol,0,55);                                                   
                                                               
                                                               draw_sprite(bufferl,fxl,-130,365);
                                                               draw_sprite(bufferl,fxl,-130,410);
                                                               draw_sprite(bufferl,fxl,-130,310);
                                                               draw_sprite(bufferl,fxl,-130,255);
                                                               draw_sprite(bufferl,fxl,-130,205);
                                                               draw_sprite(bufferl,fxl,-130,160);
                                                               draw_sprite(bufferl,fxl,-130,510);
                                                               draw_sprite(bufferl,fxl,-130,460);
                                                              
                                                               draw_sprite(bufferl,fxl,750,365);
                                                               draw_sprite(bufferl,fxl,750,420);
                                                               draw_sprite(bufferl,fxl,750,310);
                                                               draw_sprite(bufferl,fxl,750,255);
                                                               draw_sprite(bufferl,fxl,750,160);
                                                               draw_sprite(bufferl,fxl,750,205);  
                                                               
                                                               draw_sprite(bufferl,mmaquinal,95,60); 
                                                               
                                                               draw_sprite(bufferl,portal,700,463);
                                                               if(porta==1)//portas fechadas ou abertas
                                                                 draw_sprite(bufferl,fportal,690,465);
                                                              /////////////////////////////////////////////////////////                                                         
                                                             // draw_sprite(bufferl,fundo1l,0,60);
                                                              
                                                              if(roupa==0) //se for zero aparece armadura pq nao pegou ainda                                
                                                                draw_sprite(bufferl,armadural,153,420);//armadura a pegar
                                                                        
                                                              
                                                              destroy_bitmap(pl);                                  
                                                              /////////////////////////////////////////////////////                 
                                                     
                                                 
                                                 
                                textprintf_centre_ex(bufferl, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferl, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferl, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferl, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferl, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferl, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferl, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               textprintf_centre_ex(bufferl, font,50,410, makecol(0, 0, 0), makecol(255, 255, 255)," LPULO :%d",lpulo);
                               
                               draw_sprite(bufferl,painell,0,0);
                               textprintf_centre_ex(bufferl, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                              // show_mouse(bufferl);
                              loop=1;
                        }                             
                        ::fps_speed--;
                        ::fps++;
               }   
               loop=0;               
               draw_sprite(screen,bufferl,0,0);//desenha na tela a nova imagem 
              // blit(bufferl, screen, 0,0,0,0,MAX_X,MAX_Y);
               
             if(x>720)//voltou cenario
               i=-1;
             
              vsync();
            // rest(fps/3300);//controle de frames
    }while (i!=-1 && x!=-1 && fim!=-10000);
    
      destroy_bitmap(fundo1l);
      destroy_bitmap(fundo2l);
      destroy_bitmap(bufferl);
      destroy_bitmap(painell);
      destroy_bitmap(cientista1l);
      destroy_bitmap(cientista2l);
      destroy_bitmap(armadural);
      destroy_bitmap(pc1l);
      destroy_bitmap(pc2l);
      destroy_bitmap(luzl);
      destroy_bitmap(cena1l);
      destroy_bitmap(cena2l);
      destroy_bitmap(luzcena1l);
      destroy_bitmap(luzcena2l);
      destroy_bitmap(fcena2l);
      destroy_bitmap(navepiratal);
      destroy_bitmap(metroid1l);
      destroy_bitmap(metroid2l);
      destroy_bitmap(piratal1);
      destroy_bitmap(piratal2);
      destroy_bitmap(voadorl);
      destroy_sample(passol);
      destroy_sample(vozl);
      destroy_sample(voz2l);
      destroy_sample(ptl);  
      destroy_sample(choquel); 
      
      destroy_bitmap(chaol);
      destroy_bitmap(chao2l);
      destroy_bitmap(tetol);
      destroy_bitmap(teto2l);
      destroy_bitmap(fx1l);
      destroy_bitmap(fx2l);
      destroy_bitmap(fxl);

      destroy_bitmap(portal);
      destroy_bitmap(f3l);
      destroy_bitmap(f4l);
      destroy_bitmap(f5l);
      destroy_bitmap(mmaquinal);
      
      //destroy_bitmap(fportal);
      destroy_sample(abreportal);
      destroy_sample(fechaportal);
     
      
     if(i==-1)//diz que o jogador ta voltando um cenario
     {
       passaporta(1,20,495-30,fps);
       return 10;//diz pra funcao jogo pra chamar a funcao 18 que ta voltano desse cenario
     }  
     else
        if(fim==-10000)
         return -10000;

}


int cenario111(int estadox,int fps,int roupa,int &somdeexplosao,int &cenacapitao,int ativacena,int ativa,int cenamonitor,int &salvado)//sala com salve terceiro caminho do cenario 110
{
      BITMAP *bufferz,*fundo2z,*fundo1z,*pz,*painelz,*imagemsalve,*salve1z,*salve2z,*elevadorc1z,*elevadorc2z,*chaoz,*chao2z,*tetoz,*c1z,*c2z,*portaz,*teto2z,*fx1z,*fx2z,*c3z,*cursormz,*cursormz2,*fportaz; 
      SAMPLE *passoz,*vozz,*ptz,*explosaoz,*salvezmusica,*abreportaz,*fechaportaz;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
      DATAFILE *datFontes = NULL;
      datFontes = load_datafile("fonthistoria.dat");
      salvamento SALVOx;
      FILE *arqSALVO;
      char aux[25];//usado para verificar qual arquivo deve ser salvo o 1,2 ou 3
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta1=1,//controle de porta,se 1 porta fechada
      opcao=0,
      i=0,//diz qual bitmap do elevador vai ser mostrado
      i3,//faz movimento de explosao na tela
      isalve=0,//controla a posicao do cursor da pergunta do salve
      salvar=0,//controla se ta na posicao de salvamento ou nao para bloquear os controles
      jasalvou=0,//////controla salve 
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      ci=0,//controla loop cena
     // fps,
      vida=energia,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      tiroflag=0,
      tiro,
      xcenario=0,//controla posicao do cenario na hora da explosao
      treme=0,//variavel que indica se vai tremer a tela ou nao
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo
      tempo2==0;//zera tempo
      tempo3==0;//zera tempo

      install_keyboard();
      bufferz = create_bitmap(MAX_X,MAX_Y);
      fundo1z = create_bitmap(MAX_X,MAX_Y);
      fundo2z = create_bitmap(MAX_X,MAX_Y);
      
      fundo2z = load_gif("fundos/fundobase.gif", NULL);
      painelz = load_gif("fundos/painel.gif",NULL);
      salve1z = load_gif("objetos/BASEESPACIAL/salve1.gif", NULL);
      salve2z = load_gif("objetos/BASEESPACIAL/salve2.gif", NULL);
      imagemsalve = load_gif("fundos/salvejogo1.gif", NULL);
      passoz = load_wav("musicas/passo.wav");
      salvezmusica = load_wav("musicas/samusinicia.wav");
      if(roupa==0)
        ptz = load_wav("musicas/tirox.wav");
      else
         ptz = load_wav("musicas/tiro.wav");
      explosaoz = load_wav("musicas/explosao.wav");
      elevadorc1z = load_gif("objetos/BASEESPACIAL/elevadorcentro.gif", NULL);
      elevadorc2z = load_gif("objetos/BASEESPACIAL/elevadorcentro2.gif", NULL);
      
      chaoz = load_gif("fundos/BASEESPACIAL/xxx2.gif", NULL);
      chao2z = load_gif("fundos/BASEESPACIAL/xx2.gif", NULL);
      tetoz = load_gif("fundos/BASEESPACIAL/xxx.gif",NULL);
      teto2z = load_gif("fundos/BASEESPACIAL/xx.gif",NULL);
      c1z = load_gif("fundos/BASEESPACIAL/c1.gif", NULL); 
      c2z = load_gif("fundos/BASEESPACIAL/a1.gif", NULL);
      c3z = load_gif("fundos/BASEESPACIAL/a2.gif", NULL);
      fx1z = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2z = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portaz = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      
      cursormz = load_gif("objetos//BASEESPACIAL/metroidkid1.gif",NULL);
      cursormz2 = load_gif("objetos/BASEESPACIAL/metroidkid2.gif",NULL);

      fportaz = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      ////////////////////////////
      abreportaz = load_wav("musicas/Dooropen.wav");
      fechaportaz = load_wav("musicas/Doorclose.wav");
      ////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400; 
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<150;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<110;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<455;y++)
                   matriz[x][y]=1;
               }
               
               for(x=624;x<800;x++)//parede da direita
               {
                 for(y=0;y<600;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=466;x<588;x++)//base do salve
               {
                 for(y=466;y<500;y++)
                   matriz[x][y]=1;
               }
               for(x=0;x<84;x++)//porta de tras trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               x=0;
               y=0;
                 
               /////////  
      ////////////////////////////////////////////////////
      
      /////////////MONTANDO CENARIO////////////////////////
           draw_sprite(fundo1z,fundo2z,0,0);

       draw_sprite(fundo1z,fx1z,0,100);
       draw_sprite(fundo1z,fx1z,0,45);
       
       draw_sprite(fundo1z,fx2z,-60,365);
       draw_sprite(fundo1z,fx2z,-60,415);
       draw_sprite(fundo1z,fx2z,-60,310);
       draw_sprite(fundo1z,fx2z,-60,255);
       draw_sprite(fundo1z,fx2z,-60,205);
       draw_sprite(fundo1z,fx2z,-60,155);

       draw_sprite(fundo1z,fx2z,687,520);
       draw_sprite(fundo1z,fx2z,687,470);
       draw_sprite(fundo1z,fx2z,687,365);
       draw_sprite(fundo1z,fx2z,687,415);
       draw_sprite(fundo1z,fx2z,687,310);
       draw_sprite(fundo1z,fx2z,687,255);
       draw_sprite(fundo1z,fx2z,687,155);
       draw_sprite(fundo1z,fx2z,687,205);
       
       
        draw_sprite(fundo1z,chaoz,0,560);
        draw_sprite(fundo1z,chao2z,118,562);
        draw_sprite(fundo1z,chao2z,178,562);
        draw_sprite(fundo1z,chao2z,238,562);
        draw_sprite(fundo1z,chao2z,298,562);                                                  
        draw_sprite(fundo1z,chao2z,438,562);
        draw_sprite(fundo1z,chao2z,498,562);
        draw_sprite(fundo1z,chao2z,558,562);
        draw_sprite(fundo1z,chao2z,618,562);
        draw_sprite(fundo1z,chaoz,678,562);
        
           
           draw_sprite(fundo1z,c1z,90,165);
           draw_sprite(fundo1z,c1z,90,379);
           draw_sprite(fundo1z,c1z,90,225);
           draw_sprite(fundo1z,c3z,90,265);
           draw_sprite(fundo1z,c3z,90,325);
           draw_sprite(fundo1z,c2z,660,380);
           draw_sprite(fundo1z,c2z,660,442);
           draw_sprite(fundo1z,c2z,660,503);
           draw_sprite(fundo1z,c2z,660,320);
           draw_sprite(fundo1z,c2z,660,260);
           draw_sprite(fundo1z,c2z,660,200);
           draw_sprite(fundo1z,c2z,660,160);
           draw_sprite(fundo1z,portaz,-10,465);
           draw_sprite(fundo1z,fportaz,85,465);
           draw_sprite(fundo1z,chaoz,0,562);
           
           draw_sprite(fundo1z,tetoz,89,110);
           draw_sprite(fundo1z,tetoz,190,110);
           draw_sprite(fundo1z,tetoz,309,110);
           draw_sprite(fundo1z,tetoz,428,110);
           draw_sprite(fundo1z,tetoz,547,110);
           draw_sprite(fundo1z,teto2z,666,110);

           draw_sprite(fundo1z,fx2z,688,112);
           draw_sprite(fundo1z,salve1z,500,465);
       
       ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      
      
       vozz = load_wav("musicas/vpulo.wav");
       strcpy(posicao,"personagem/d2.gif");
       if(salvado==1)
       {

             if(roupa==0)
             {
               strcpy(posicao,"personagem/a8.gif"); 
               y=465; 
               x=525;
             }  
             else
               if(roupa==1)
               {
                 x=523;     
                 strcpy(posicao,"personagem/ARMADURA/ax8.gif");  
                 y=455; 
               }  
                
             play_sample(salvezmusica,250,130,1000,0); 
             jasalvou=1;         
             
             draw_sprite(screen,imagemsalve,0,0);
             rest(2500);
             destroy_bitmap(imagemsalve);
             imagemsalve = load_gif("fundos/salvejogo2.gif", NULL);
             draw_sprite(screen,imagemsalve,0,0);
             rest(2500);
             destroy_bitmap(imagemsalve);
             
             salvado=0;
       }
       else
           if(estadox==0)
           {
              elevador(365,700,495,bufferz,fundo2z,fundo1z,painelz,elevadorc1z,elevadorc2z,estadox,fps,0,roupa);
              
              if(roupa==0)
                strcpy(posicao,"personagem/d1.gif");    
              else
                strcpy(posicao,"personagem/ARMADURA/d1.gif"); 
                
                d=1;//posicao inicial 
                x=374;
                y=495;
           }
           else
             if(estadox==1)
             {
               x=100;  
               y=495;
               d=2;//posicao inicial
               play_sample(fechaportaz,250,130,1000,0);
             }  
       
      i=1;
      isalve=257;
       // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
           //////////////////////////////FAZENDO CENA DE EXPLOSÃO///////////////////////////////////
           install_int(IncrementaT3, 2000);
           if(somdeexplosao==0 && cenacapitao==1)//som de explosao apos falar com capitao e subir elevador
           {
             if(treme==0)
              play_sample(explosaoz,250,130,1000,0);
                                
             if(i3%2==0)                        
               xcenario=3;
             else
               xcenario=-3;
                              
              treme=1;
              if(tempo3>3)
              {
                 treme=0;
                somdeexplosao=1;                   
              }  
              i3++;
                 
           }
           else
              {
                xcenario=0;
                remove_int(IncrementaT3);
              }  
          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            while ( ::fps_speed > 0 )
            { 
                    if(loop==0)
                    {
                               clear(bufferz);
                              
                                /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////                                  
                              if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                              {
                                   porta1=0; 
                                   for(x2=0;x2<84;x2++)//porta de tras liberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportaz,250,130,1000,0);
                              } 
                               
                              if (key[KEY_ENTER] && salvar==0)//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                 
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W] && treme==0 && salvar==0) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passoz,vozz,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
                    
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
                    
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                         if(roupa==0)      
                                          strcpy(posicao,"personagem/d1.gif");
                                         else
                                           strcpy(posicao,"personagem/ARMADURA/dx1.gif");
                                   }       
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W] && treme==0 && salvar==0) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passoz,vozz,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;   
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;    
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro                
                                   
                                   
                                   
                                       if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                       {
                                         if(roupa==0)      
                                          strcpy(posicao,"personagem/d2.gif");
                                         else
                                           strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                       } 
                    
                                   
                                   
                                        if(key[KEY_S] && treme==0 && salvar==0) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passoz,vozz,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W] && treme==0 && salvar==0) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passoz,vozz,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE] && treme==0 && salvar==0) //TIRO
                                               {
                                                  // if(tiro==0)            
                                                  //  sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptz);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptz);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptz);                                                                                                                                       
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoz,vozz,roupa,lpulo); 
                                                  // rest(10);    
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                 
                                                                                                                                                                                        
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0 && treme==0 && salvar==0 || salvar==3) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoz,vozz,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }     
                                                      
                                                      //////////////////////CONTROLE DE GRAVIDADE
                                                      if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                      {     
                                                         y++;
                                                         
                                                             if(matriz[x][y+1]==0)
                                                               y++;
                                                             if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++; 
                                                               if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++;
                                                               if(matriz[x][y+1]==0)
                                                               y++;  
                                                             if(matriz[x][y+1]==0)
                                                               y++; 
                        
                                                      }      
                                                          
                                                      
                                                      
                                                          /////////////////////CENARIO/////////////////////////////////////////////////    
                                                           draw_sprite(bufferz,fundo2z,0,0); 
                                                           
                                                             
                                                               
                                                              install_int(IncrementaT, 500);//movimenta salve
                                                              if(tempo==1)                                      
                                                                  draw_sprite(bufferz,salve1z,500,465);
                                                              else    
                                                                  draw_sprite(bufferz,salve2z,500,465);
                                                            
                                                              if(i%2==0)//ELEVADOR
                                                               {
                                                                  draw_sprite(bufferz,elevadorc1z,365,552);
                                                                  i=1;
                                                               }   
                                                                else 
                                                                  {
                                                                    draw_sprite(bufferz,elevadorc2z,365,552);
                                                                    i=0;
                                                                   }       
                                                            

                                                            ////////////////SALVANDO////////////////////////
                                                             
                                                              if(x>500 && x<550 && y>454 && y<466 && jasalvou==0)
                                                              {
                                                                      salvar=1; 
                                                                      if(roupa==0)
                                                                      {
                                                                        strcpy(posicao,"personagem/a8.gif"); 
                                                                        x=525;  
                                                                      }  
                                                                      else
                                                                       if(roupa==1)
                                                                       {
                                                                         x=523;     
                                                                         strcpy(posicao,"personagem/ARMADURA/ax8.gif");  
                                                                         y=455; 
                                                                       }   
                                                                       
                                                                      rect(bufferz, 215, 300, 600, 408, makecol(0,150,255));
                                                                      rect(bufferz, 217, 302, 602, 410, makecol(0,150,255));
                                                                      textprintf_ex(bufferz,(FONT *)datFontes[0].dat,250,330,makecol(255,255,0),-1," DESEJA SALVAR O JOGO ?");             
                                                                      textprintf_ex(bufferz,(FONT *)datFontes[0].dat,250,370,makecol(255,255,0),-1,"    SIM         NÃO");
                                                                      
                                                                      if(tempo==0)
                                                                         draw_sprite(bufferz,cursormz,isalve,360);
                                                                       else
                                                                           draw_sprite(bufferz,cursormz2,isalve,360);
                                                                      
                                                                      if(key[KEY_A])
                                                                        isalve=257; //SIM SALVAR O JOGO NO ARQUIVO
                                                                      else
                                                                        if(key[KEY_D])
                                                                          isalve=428;// NAO SALVAR O JOGO NO ARQUIVO
                                                                      
                                                                      if(key[KEY_ENTER]) 
                                                                      { 
                                                                              if(isalve==257)//SALVANDO JOGO SE SIM
                                                                              {
                                                                                      /////////////////////CARREGANDO SALVE SE EXISTIR/////////////
                                                                                         //verificando qual salve carregar que o menu principal jogou na variavel global
                                                                                          if(SALVEJOGO==1)
                                                                                           strcpy(aux,"salves/salve1.dat");
                                                                                          else 
                                                                                            if(SALVEJOGO==2)
                                                                                             strcpy(aux,"salves/salve2.dat");
                                                                                            else
                                                                                              if(SALVEJOGO==3)
                                                                                               strcpy(aux,"salves/salve3.dat");
                                                                                      ///////////////////////////                  
                                                                                                        
                                                                                     /////////////////////SALVANDO/////////////
                                                                                      if(arqSALVO = fopen(aux,"wb"))
                                                                                      {
                                                                                      
                                                                                          ///PEGANDO JOGO SALVO E JOGANDO NAS VARIAVEIS DE CONTROLE////////
                                                                                          SALVOx.ativacenax,ativacena;//controla cena da nave pirata no cenario 111 apos pegar armadura e andar pra sair,diz se a cena ja foi mostrada
                                                                                          SALVOx.ativax=ativa;//variavel que informa se elevador foi ativado ou nao no cenario 18
                                                                                          SALVOx.roupax=roupa;//variavel que passa para a funcao que passa para funcao movimento dizendo se ta sem armadura ou nao ou com qual
                                                                                          SALVOx.cenacapitaox=cenacapitao;//variavel que diz se samus ja foi falar com capitao,dae libera para ela ir pegar armadura apos explosao
                                                                                          SALVOx.cenamonitorx=cenamonitor;//controla que imagem e cena deve acontecer no cenario 10
                                                                                          SALVOx.somdeexplosaox=somdeexplosao;//som de explosao ao subir elevador apos falar com capitao
                                                                                          strcpy(SALVOx.localdejogo,"B. ESPACIAL");//local que o salve foi efetuado,como base espacial darkenss
                                                                                          SALVOx.energiax=energia;//guarda energia do personagem
                                                                                          ///////////////////////////////////////////////////////////
                                                                                          fwrite(&SALVOx,sizeof(SALVOx),1,arqSALVO);
                                                                                          fclose(arqSALVO); 
                                                                                      }     
                                                                                      //////////////////////////////////////////////////////////////////             
                                                                              }     
                                                                              remove_keyboard();
                                                                              install_keyboard();
                                                                              jasalvou=1;//diz que ja foi salvo,nisso tem que voltar fora da maquina de salve para liberar
                                                                              salvar=0;
                                                                              
                                                                      }         
                                                              }
                                                              else
                                                                {
                                                                     if(y==495)    
                                                                     {       
                                                                       salvar=0;
                                                                       jasalvou=0;
                                                                     }  
                                                                }  
                                                         
                                                              
                                                              ///////////////////////////////////////////////           
                                                            if(a==2)
                                                              show_tiros(x,y,bufferz,3,tirof,tirot,tiroc,matriz,roupa);
                                                            else
                                	                          show_tiros(x,y,bufferz,d,tirof,tirot,tiroc,matriz,roupa);
                                	                      
                                                          pz = load_gif(posicao,NULL);   
                                                          if(roupa==1)     
                                                          { 
                                                            if(!key[KEY_W] && !key[KEY_S])                                  
                                                              draw_sprite(bufferz,pz,x,y-10); 
                                                            else
                                                              if(key[KEY_W])
                                                               draw_sprite(bufferz,pz,x,y-22);
                                                              else
                                                                if(key[KEY_S]) 
                                                                 draw_sprite(bufferz,pz,x,y);
                                                          }  
                                                          else
                                                           draw_sprite(bufferz,pz,x,y); 
                                                          
                                                          /////////////MONTANDO CENARIO////////////////////////
                                                           draw_sprite(bufferz,fx1z,0,100);
                                                           draw_sprite(bufferz,fx1z,0,45);
                                                           
                                                           draw_sprite(bufferz,fx2z,-60,365);
                                                           draw_sprite(bufferz,fx2z,-60,415);
                                                           draw_sprite(bufferz,fx2z,-60,310);
                                                           draw_sprite(bufferz,fx2z,-60,255);
                                                           draw_sprite(bufferz,fx2z,-60,205);
                                                           draw_sprite(bufferz,fx2z,-60,155);
                                                
                                                           draw_sprite(bufferz,fx2z,687,520);
                                                           draw_sprite(bufferz,fx2z,687,470);
                                                           draw_sprite(bufferz,fx2z,687,365);
                                                           draw_sprite(bufferz,fx2z,687,415);
                                                           draw_sprite(bufferz,fx2z,687,310);
                                                           draw_sprite(bufferz,fx2z,687,255);
                                                           draw_sprite(bufferz,fx2z,687,155);
                                                           draw_sprite(bufferz,fx2z,687,205);
                                                           
                                                           
                                                            draw_sprite(bufferz,chaoz,0,560);
                                                            draw_sprite(bufferz,chao2z,118,562);
                                                            draw_sprite(bufferz,chao2z,178,562);
                                                            draw_sprite(bufferz,chao2z,238,562);
                                                            draw_sprite(bufferz,chao2z,298,562);                                                  
                                                            draw_sprite(bufferz,chao2z,438,562);
                                                            draw_sprite(bufferz,chao2z,498,562);
                                                            draw_sprite(bufferz,chao2z,558,562);
                                                            draw_sprite(bufferz,chao2z,618,562);
                                                            draw_sprite(bufferz,chaoz,678,562);
                                                            
                                                               
                                                               draw_sprite(bufferz,c1z,90,165);
                                                               draw_sprite(bufferz,c1z,90,379);
                                                               draw_sprite(bufferz,c1z,90,225);
                                                               draw_sprite(bufferz,c3z,90,265);
                                                               draw_sprite(bufferz,c3z,90,325);
                                                               draw_sprite(bufferz,c2z,660,380);
                                                               draw_sprite(bufferz,c2z,660,442);
                                                               draw_sprite(bufferz,c2z,660,503);
                                                               draw_sprite(bufferz,c2z,660,320);
                                                               draw_sprite(bufferz,c2z,660,260);
                                                               draw_sprite(bufferz,c2z,660,200);
                                                               draw_sprite(bufferz,c2z,660,160);
                                                               draw_sprite(bufferz,portaz,-10,465);
                                                               if(porta1==1)
                                                                draw_sprite(bufferz,fportaz,85,465);
                                                                
                                                               draw_sprite(bufferz,chaoz,0,562);
                                                               
                                                               draw_sprite(bufferz,tetoz,89,110);
                                                               draw_sprite(bufferz,tetoz,190,110);
                                                               draw_sprite(bufferz,tetoz,309,110);
                                                               draw_sprite(bufferz,tetoz,428,110);
                                                               draw_sprite(bufferz,tetoz,547,110);
                                                               draw_sprite(bufferz,teto2z,666,110);

                                                               draw_sprite(bufferz,fx2z,688,112);
                                                               
                                                                
                                                           ////////////////////////////////////////////////////
                                                                                                                    
                                                          //draw_sprite(bufferz,fundo1z,0,60);
                                                                                                            
                                                          destroy_bitmap(pz);                                  
                                                          /////////////////////////////////////////////////////                 
                                                // }
                                                 
                                                 
                                textprintf_centre_ex(bufferz, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferz, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferz, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferz, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferz, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferz, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferz, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               textprintf_centre_ex(bufferz, font,50,420, makecol(0, 0, 0), makecol(255, 255, 255)," ESTADO :%d",estadox);
                               show_mouse(bufferz);
                               draw_sprite(bufferz,painelz,0,0);
                               textprintf_centre_ex(bufferz, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                               //show_mouse(bufferz);
                               loop=1;
                        }                             
                        ::fps_speed--;
                        ::fps++;
               }   
               loop=0;                
               draw_sprite(screen,bufferz,xcenario,0);//desenha na tela a nova imagem 
               //vsync();
               
             if(x<10)//voltou cenario
               i=-1;
               
               if(x>363 && x<392 && y==495 && key[KEY_S] && treme==0)//apertou para baixo no elevador
               {
                 x=1200;    
                 if(roupa==0)
                  strcpy(posicao,"personagem/a8.gif");
                 else
                   strcpy(posicao,"personagem/ARMADURA/ax8.gif");
               } 
             
              vsync();
        
            // rest(fps/3300);//controle de frames
    }while (i!=-1 && x!=1200 && fim!=-10000);

     if(i==-1)//diz que o jogador ta voltando um cenario
     {
       destroy_bitmap(fundo1z);
       destroy_bitmap(fundo2z);
       destroy_bitmap(elevadorc1z);
       destroy_bitmap(elevadorc2z);
       //destroy_bitmap(bufferz);
       destroy_bitmap(painelz);
       destroy_bitmap(salve1z);
       destroy_bitmap(salve2z);
       destroy_bitmap(cursormz);
       destroy_bitmap(cursormz2);
       destroy_sample(passoz);
       destroy_sample(salvezmusica);
       destroy_sample(vozz);
       destroy_sample(ptz); 
               
       destroy_bitmap(fportaz);
       destroy_sample(abreportaz);
       destroy_sample(fechaportaz);
              
       passaporta(0,20,495-30,fps);
       return 0;//diz pra funcao jogo pra chamar a funcao 110 que ta voltano desse cenario
     }  
     else
       if(x==1200)
       {
          elevador(365,495,700,bufferz,fundo2z,fundo1z,painelz,elevadorc1z,elevadorc2z,1,fps,0,roupa); //elevador descendo       
          
          destroy_bitmap(fundo1z);
          destroy_bitmap(fundo2z);
          destroy_bitmap(elevadorc1z);
          destroy_bitmap(elevadorc2z);
          destroy_bitmap(bufferz);
          destroy_bitmap(painelz);
          destroy_bitmap(salve1z);
          destroy_bitmap(salve2z);
          destroy_bitmap(cursormz);
          destroy_bitmap(cursormz2);
          destroy_sample(passoz);
          destroy_sample(salvezmusica);
          destroy_sample(vozz);
          destroy_sample(ptz); 
          
          destroy_bitmap(fportaz);
          destroy_sample(abreportaz);
          destroy_sample(fechaportaz);
          return 1;//diz que ta indo para o proximo cenario 
       }
       else
          if(fim==-10000)
           return -10000;
      

}

int cenario112(int estado,int fps,int roupa)
{
      BITMAP *bufferA,*fundo2A,*fundo1A,*pA,*painelA,*elevadorc1A,*elevadorc2A,*elevadorb1A,*elevadorb2A,*soldadofA
      ,*soldadofA2,*chaoA,*chao2A,*tetoA,*c1A,*c2A,*portaA,*teto2A,*fx1A,*fx2A,*c3A,*c4A,*fportaA,*fporta2A; 
      
      SAMPLE *passoA,*voz3A,*ptA,*abreportaA,*fechaportaA;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta1=1,//diz se porta da esquerda ta aberta(1 fechado)
      porta2=1,//diz se porta da direita ta aberta(1 fechado)
      opcao=0,
      i=0,//diz qual bitmap do elevador vai ser mostrado
      i2=0,//mostra base do elevador dizendo qual deve aparecer
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
      vida=energia,
      tiroflag=0,
      tiro,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      //fps,
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo
      
      ///////zerando globais de tempo
      tempo2=0;
      tempo=0;
      tempo3=0;
      //////////////////////////////////////
    /*  pleiturax=0;
      pleituray=0;*/

      install_keyboard();
      bufferA = create_bitmap(MAX_X,MAX_Y);
      fundo1A = create_bitmap(MAX_X,MAX_Y);
      fundo2A = create_bitmap(MAX_X,MAX_Y);
      
      fundo2A = load_gif("fundos/fundobase12.gif", NULL);
      painelA = load_gif("fundos/painel.gif",NULL);
      elevadorc1A = load_gif("objetos/BASEESPACIAL/elevadorcentro.gif", NULL);
      elevadorc2A = load_gif("objetos/BASEESPACIAL/elevadorcentro2.gif", NULL);
      elevadorb1A = load_gif("objetos/BASEESPACIAL/elevador1.gif", NULL);
      elevadorb2A = load_gif("objetos/BASEESPACIAL/elevador2.gif", NULL);
      soldadofA = load_gif("objetos/BASEESPACIAL/soldadof.gif",NULL);
      soldadofA2 = load_gif("objetos/BASEESPACIAL/soldadof2.gif",NULL);
      
      chaoA = load_gif("fundos/BASEESPACIAL/xxx2.gif", NULL);
      chao2A = load_gif("fundos/BASEESPACIAL/xx2.gif", NULL);
      tetoA = load_gif("fundos/BASEESPACIAL/xxx.gif",NULL);
      teto2A = load_gif("fundos/BASEESPACIAL/xx.gif",NULL);
      c1A = load_gif("fundos/BASEESPACIAL/c1.gif", NULL); 
      c4A = load_gif("fundos/BASEESPACIAL/c2.gif", NULL); 
      c2A = load_gif("fundos/BASEESPACIAL/a1.gif", NULL);
      c3A = load_gif("fundos/BASEESPACIAL/a2.gif", NULL);
      fx1A = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2A = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portaA = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      
      voz3A = load_wav("musicas/vpulo.wav");
      passoA = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptA = load_wav("musicas/tirox.wav");
      else
         ptA = load_wav("musicas/tiro.wav");
      ////////////////////////////
      abreportaA = load_wav("musicas/Dooropen.wav");
      fechaportaA = load_wav("musicas/Doorclose.wav");
      fportaA = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      fporta2A = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
           
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<150;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<113;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<448;y++)
                   matriz[x][y]=1;
               }
               
               for(x=630;x<800;x++)//parede da direita
               {
                 for(y=0;y<443;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<93;x++)//porta de tras trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }

      ////////////////////////////////////////////////////
      
       /////////////MONTANDO CENARIO////////////////////////
           draw_sprite(fundo1A,fundo2A,0,0);

           draw_sprite(fundo1A,fx2A,-60,365);
           draw_sprite(fundo1A,fx2A,-60,415);
           draw_sprite(fundo1A,fx2A,-60,310);
           draw_sprite(fundo1A,fx2A,-60,255);
           draw_sprite(fundo1A,fx2A,-60,205);
           
           
           draw_sprite(fundo1A,fx2A,687,365);
           draw_sprite(fundo1A,fx2A,687,415);
           draw_sprite(fundo1A,fx2A,687,310);
           draw_sprite(fundo1A,fx2A,687,255);
           draw_sprite(fundo1A,fx2A,687,155);
           draw_sprite(fundo1A,fx2A,687,55);
           draw_sprite(fundo1A,fx2A,680,210);
           
            draw_sprite(fundo1A,chaoA,0,563);
            draw_sprite(fundo1A,chaoA,110,563);
            draw_sprite(fundo1A,chaoA,220,563);                                                  
            draw_sprite(fundo1A,chaoA,778,563);
            draw_sprite(fundo1A,chaoA,330,563);
            draw_sprite(fundo1A,chaoA,660,563);
            draw_sprite(fundo1A,chaoA,440,563);
            draw_sprite(fundo1A,chaoA,550,563);         
               
               draw_sprite(fundo1A,c1A,90,165);
               draw_sprite(fundo1A,c1A,90,379);
               draw_sprite(fundo1A,c1A,90,225);
               
               draw_sprite(fundo1A,c3A,90,120);
               draw_sprite(fundo1A,c3A,90,180);
               draw_sprite(fundo1A,c3A,90,265);
               draw_sprite(fundo1A,c3A,90,325);
               
               draw_sprite(fundo1A,c2A,670,380);
               draw_sprite(fundo1A,c2A,670,320);
               draw_sprite(fundo1A,c2A,670,260);
               draw_sprite(fundo1A,c2A,670,200);
               draw_sprite(fundo1A,c2A,670,160);
               draw_sprite(fundo1A,c2A,670,100);
               draw_sprite(fundo1A,c4A,670,380);
               
               draw_sprite(fundo1A,portaA,-5,465);
               draw_sprite(fundo1A,portaA,700,465);
           //    draw_sprite(fundo1A,fportaA,692,465);
               
               draw_sprite(fundo1A,chaoA,0,562);
               
                draw_sprite(fundo1A,chaoA,0,563);
                draw_sprite(fundo1A,chaoA,118,70);
                draw_sprite(fundo1A,chao2A,178,70);
                draw_sprite(fundo1A,chao2A,238,70);
                draw_sprite(fundo1A,chao2A,298,70);                                                  
                draw_sprite(fundo1A,chao2A,438,70);
                draw_sprite(fundo1A,chao2A,498,70);
                draw_sprite(fundo1A,chao2A,558,70);
                draw_sprite(fundo1A,chao2A,618,70);
               
               draw_sprite(fundo1A,fx2A,-80,155);
               draw_sprite(fundo1A,fx2A,-80,100);
               draw_sprite(fundo1A,fx2A,-56,65);
               draw_sprite(fundo1A,fx2A,678,60);
               draw_sprite(fundo1A,fx2A,688,112);
               
               draw_sprite(fundo1A,soldadofA2,550,495-20);
               draw_sprite(fundo1A,soldadofA,225,495-20);
               
               draw_sprite(fundo1A,fportaA,692,465);
               draw_sprite(fundo1A,fporta2A,90,465);
           ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////
      
      remove_keyboard();
      install_keyboard();
      
      if(estado==1)
      {
           if(roupa==0)
             strcpy(posicao,"personagem/a8.gif");
           else
             strcpy(posicao,"personagem/ARMADURA/ax8.gif"); 
           
           
          ///////////////////////////////ELEVADOR DECENDO/////////////////////////////
                 if(estado==1)
                   elevador2(365,0,490,325,bufferA,fundo2A,fundo1A,painelA,elevadorc1A,elevadorc2A,estado,elevadorb1A,elevadorb2A,fps,0,roupa);                        
                  
           //tira som de elevador e volta para voz de samus       
        x=374;
      }
      else
          if(estado==0)
          {
            x=650;
            d=1;//posicao inicial
             if(roupa==0)
               strcpy(posicao,"personagem/d1.gif");
             else
               strcpy(posicao,"personagem/ARMADURA/dx1.gif"); 
               play_sample(fechaportaA,250,130,1000,0);
          }  
          else
              if(estado==12)
              {
                x=100;
                d=2;//posicao inicial
                 if(roupa==0)
                   strcpy(posicao,"personagem/dx2.gif");
                 else
                   strcpy(posicao,"personagem/ARMADURA/dx2.gif"); 
                   play_sample(fechaportaA,250,130,1000,0);
              } 
       
       i=1;
      y=495;
     
       // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
             while ( ::fps_speed > 0 )
             { 
                    if(loop==0)
                    {       
                               clear(bufferA);
                               
                               /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                              if(tirof[0].x-15>695 && porta2==1 && tirof[0].y>450)
                              {
                                   porta2=0;
                
                                   for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportaA,250,130,1000,0);
                              }  
                              if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                              {
                                   porta1=0; 
                                   for(x2=0;x2<93;x2++)//porta de tras liberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportaA,250,130,1000,0);
                              } 
                               
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                               
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passoA,voz3A,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
            
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;    
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
            
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passoA,voz3A,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++; 
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;     
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro               
                                   
                                   
                                   if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                    {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d2.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                    }
                    
                                   
                                   
                                        if(key[KEY_S]) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passoA,voz3A,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W]) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passoA,voz3A,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                               {
                                                   //if(tiro==0)            
                                                  //  sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptA);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptA);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptA);                                                                                                                                        
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,2,passoA,voz3A,roupa,lpulo);     
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                 
                                                                                                                                                                                        
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoA,voz3A,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }     
                                                  
                                                  //////////////////////CONTROLE DE GRAVIDADE
                                                  if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                  {     
                                                     y++;
                                                     
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                         if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                    
                                                  }     
                                                  ////////////////////////CENARIO////////////////////////////////////////////// 
                                                  pA = load_gif(posicao,NULL);                
                                                  draw_sprite(bufferA,fundo2A,0,0);
                                                  

                                                       draw_sprite(bufferA,fundo2A,0,0);
                                                    //   draw_sprite(fundo1A,fx1A,0,100);
                                                      // draw_sprite(fundo1A,fx1A,0,45);
                                                       
                                                        
                                                       
                                                       if(i2%2==0)//ELEVADOR BASE
                                                        {
                                                          draw_sprite(bufferA,elevadorb1A,325,465);
                                                          i2=1;
                                                        }   
                                                        else 
                                                          {
                                                            draw_sprite(bufferA,elevadorb2A,325,465);
                                                            i2=0;
                                                           }   
                                                        
                                                       if(i%2==0)//ELEVADOR
                                                       {
                                                          draw_sprite(bufferA,elevadorc1A,364,543);
                                                          i=1;
                                                       }   
                                                        else 
                                                          {
                                                            draw_sprite(bufferA,elevadorc2A,364,543);
                                                            i=0;
                                                           } 
                                                      
                                                      install_int(IncrementaT, 1500);//movimenta cientista
                                                      if(tempo==1)                               
                                                      {       
                                                         draw_sprite(bufferA,soldadofA,550,495-20);
                                                         draw_sprite(bufferA,soldadofA,225,495-20);
                                                      }
                                                      else   
                                                        { 
                                                             draw_sprite(bufferA,soldadofA2,550,495-20);
                                                             draw_sprite(bufferA,soldadofA2,225,495-20);
                                                        }
                                                       
                                                       if(a==2)
                                                          show_tiros(x,y,bufferA,3,tirof,tirot,tiroc,matriz,roupa);
                                                        else
                        	                              show_tiros(x,y,bufferA,d,tirof,tirot,tiroc,matriz,roupa); 
                                                      
                                                      
                                                      if(roupa==1)     
                                                      { 
                                                        if(!key[KEY_W] && !key[KEY_S])                                  
                                                          draw_sprite(bufferA,pA,x,y-10); 
                                                        else
                                                          if(key[KEY_W])
                                                           draw_sprite(bufferA,pA,x,y-22);
                                                          else
                                                            if(key[KEY_S]) 
                                                             draw_sprite(bufferA,pA,x,y);
                                                      }
                                                      else
                                                        draw_sprite(bufferA,pA,x,y); 
                                                        
                                                       //////////////MONTANDO CENARIO///////////////////
                                                       draw_sprite(bufferA,fx2A,-60,365);
                                                       draw_sprite(bufferA,fx2A,-60,415);
                                                       draw_sprite(bufferA,fx2A,-60,310);
                                                       draw_sprite(bufferA,fx2A,-60,255);
                                                       draw_sprite(bufferA,fx2A,-60,205);
                                                       
                                                       
                                                       draw_sprite(bufferA,fx2A,687,365);
                                                       draw_sprite(bufferA,fx2A,687,415);
                                                       draw_sprite(bufferA,fx2A,687,310);
                                                       draw_sprite(bufferA,fx2A,687,255);
                                                       draw_sprite(bufferA,fx2A,687,155);
                                                       draw_sprite(bufferA,fx2A,687,55);
                                                       draw_sprite(bufferA,fx2A,680,210);
                                                       
                                                        draw_sprite(bufferA,chaoA,0,563);
                                                        draw_sprite(bufferA,chaoA,110,563);
                                                        draw_sprite(bufferA,chaoA,220,563);                                                  
                                                        draw_sprite(bufferA,chaoA,778,563);
                                                        draw_sprite(bufferA,chaoA,330,563);
                                                        draw_sprite(bufferA,chaoA,660,563);
                                                        draw_sprite(bufferA,chaoA,440,563);
                                                        draw_sprite(bufferA,chaoA,550,563);         
                                                           
                                                           draw_sprite(bufferA,c1A,90,165);
                                                           draw_sprite(bufferA,c1A,90,379);
                                                           draw_sprite(bufferA,c1A,90,225);
                                                           
                                                           draw_sprite(bufferA,c3A,90,120);
                                                           draw_sprite(bufferA,c3A,90,180);
                                                           draw_sprite(bufferA,c3A,90,265);
                                                           draw_sprite(bufferA,c3A,90,325);
                                                           
                                                           draw_sprite(bufferA,c2A,670,380);
                                                           draw_sprite(bufferA,c2A,670,320);
                                                           draw_sprite(bufferA,c2A,670,260);
                                                           draw_sprite(bufferA,c2A,670,200);
                                                           draw_sprite(bufferA,c2A,670,160);
                                                           draw_sprite(bufferA,c2A,670,100);
                                                           draw_sprite(bufferA,c4A,670,380);
                                                           
                                                           draw_sprite(bufferA,portaA,-5,465);
                                                           draw_sprite(bufferA,portaA,700,465);
                                                           if(porta2==1)//portas fechadas ou abertas
                                                            draw_sprite(bufferA,fportaA,692,465);
                                                           if(porta1==1) 
                                                            draw_sprite(bufferA,fporta2A,90,465);
                                                           
                                                           draw_sprite(bufferA,chaoA,0,562);
                                                           
                                                            draw_sprite(bufferA,chaoA,0,563);
                                                            draw_sprite(bufferA,chaoA,118,70);
                                                            draw_sprite(bufferA,chao2A,178,70);
                                                            draw_sprite(bufferA,chao2A,238,70);
                                                            draw_sprite(bufferA,chao2A,298,70);                                                  
                                                            draw_sprite(bufferA,chao2A,438,70);
                                                            draw_sprite(bufferA,chao2A,498,70);
                                                            draw_sprite(bufferA,chao2A,558,70);
                                                            draw_sprite(bufferA,chao2A,618,70);
                                                           
                                                           draw_sprite(bufferA,fx2A,-80,155);
                                                           draw_sprite(bufferA,fx2A,-80,100);
                                                           draw_sprite(bufferA,fx2A,-56,65);
                                                           draw_sprite(bufferA,fx2A,678,60);
                                                           draw_sprite(bufferA,fx2A,688,112);
                                                       ////////////////////////////////////////////////////
                                                  

                                                  destroy_bitmap(pA);                                  
                                                 ///////////////////////////////////////////////////////////////////                  
                                
                                textprintf_centre_ex(bufferA, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferA, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferA, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferA, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferA, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferA, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferA, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               
                               draw_sprite(bufferA,painelA,0,0);
                               textprintf_centre_ex(bufferA, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                
                      loop=1;
                      }                             
                      ::fps_speed--;
                      ::fps++;
               }    
               loop=0;                      
                            //   show_mouse(bufferd);
                   draw_sprite(screen,bufferA,0,0);//desenha na tela a nova imagem 
                   
               if(x>363 && x<392 && y==495 && key[KEY_W])//apertou para baixo no elevador
                 x=-1;    
               else
                 if(x>700)//passou
                   i=-1;  
                 if(x<10)
                  fim=-1;  
               
               //rest(fps/3300);//controle de frames
                vsync();
      }while (x!=-1 && i!=-1 && fim!=-1 && fim!=-10000);
      
      
      //////////////////////////////ELEVADOR SUBINDO/////////////////////////////
             if(x==-1)
             {
                      elevador2(365,490,-100,325,bufferA,fundo2A,fundo1A,painelA,elevadorc1A,elevadorc2A,0,elevadorb1A,elevadorb2A,fps,0,roupa);
                      
                      destroy_bitmap(fundo1A);
                      destroy_bitmap(fundo2A);
                      destroy_bitmap(bufferA);
                      destroy_bitmap(painelA);
                      destroy_bitmap(soldadofA);
                      destroy_bitmap(soldadofA2);
                      destroy_bitmap(elevadorc1A);
                      destroy_bitmap(elevadorc2A);
                      destroy_bitmap(elevadorb1A);
                      destroy_bitmap(elevadorb2A);
                      
                      destroy_bitmap(chaoA);
                      destroy_bitmap(chao2A);
                      destroy_bitmap(tetoA);
                      destroy_bitmap(teto2A);
                      destroy_bitmap(c1A);
                      destroy_bitmap(c2A);
                      destroy_bitmap(fx1A);
                      destroy_bitmap(fx2A);
                      destroy_bitmap(portaA);
                      destroy_bitmap(c3A);
                      destroy_bitmap(c4A);
                      
                   //   destroy_sample(musica);
                      destroy_sample(passoA);
                      destroy_sample(voz3A);
                      destroy_sample(ptA); 
                      
                    //  destroy_bitmap(fportaA);
                      destroy_bitmap(fporta2A);
                      destroy_sample(abreportaA);
                      destroy_sample(fechaportaA);

                      return 0;
              }
              else        
                 if(i==-1)
                 {
                      passaporta(1,20,495-30,fps); 
                   
                      destroy_bitmap(fundo1A);
                      destroy_bitmap(fundo2A);
                      destroy_bitmap(bufferA);
                      destroy_bitmap(painelA);
                      destroy_bitmap(soldadofA);
                      destroy_bitmap(elevadorc1A);
                      destroy_bitmap(elevadorc2A);
                      destroy_bitmap(elevadorb1A);
                      destroy_bitmap(elevadorb2A);
                      
                      destroy_bitmap(chaoA);
                      destroy_bitmap(chao2A);
                      destroy_bitmap(tetoA);
                      destroy_bitmap(teto2A);
                      destroy_bitmap(c1A);
                      destroy_bitmap(c2A);
                      destroy_bitmap(fx1A);
                      destroy_bitmap(fx2A);
                      destroy_bitmap(portaA);
                      destroy_bitmap(c3A);
                      destroy_bitmap(c4A);
                      
                      //destroy_sample(musica);
                      destroy_sample(passoA);
                      destroy_sample(voz3A);
                      destroy_sample(ptA); 
                      
                      destroy_bitmap(fportaA);
                      destroy_bitmap(fporta2A);
                      destroy_sample(abreportaA);
                      destroy_sample(fechaportaA);
                         
                   return 1;//diz que o jogador passou um cenario 
                 }   
                 else
                    if(fim==-1)
                     {
                          passaporta(0,20,495-30,fps); 
                       
                          destroy_bitmap(fundo1A);
                          destroy_bitmap(fundo2A);
                          destroy_bitmap(bufferA);
                          destroy_bitmap(painelA);
                          destroy_bitmap(soldadofA);
                          destroy_bitmap(elevadorc1A);
                          destroy_bitmap(elevadorc2A);
                          destroy_bitmap(elevadorb1A);
                          destroy_bitmap(elevadorb2A);
                          
                          destroy_bitmap(chaoA);
                          destroy_bitmap(chao2A);
                          destroy_bitmap(tetoA);
                          destroy_bitmap(teto2A);
                          destroy_bitmap(c1A);
                          destroy_bitmap(c2A);
                          destroy_bitmap(fx1A);
                          destroy_bitmap(fx2A);
                          destroy_bitmap(portaA);
                          destroy_bitmap(c3A);
                          destroy_bitmap(c4A);
                          
                          //destroy_sample(musica);
                          destroy_sample(passoA);
                          destroy_sample(voz3A);
                          destroy_sample(ptA); 
                          
                         // destroy_bitmap(fportaA);
                          destroy_bitmap(fporta2A);
                          destroy_sample(abreportaA);
                          destroy_sample(fechaportaA);
                             
                       return 11;//diz que o jogador passou um cenario 
                     } 
                     else
                      if(fim==-10000)
                       return -10000;
}

int cenario1122(int estado,int fps,int roupa,int &cenacapitao)
{
      BITMAP *bufferB,*fundo2B,*pB,*painelB,*pc1B,*pc2B,*pc1Bf,*pc2Bf,*telaB,*cientista1B,*cientista2B,*cientista3B,
      *cientista4B,*cientistaX,*chaoB,*chao2B,*tetoB,*f3B,*f4B,*f5B,*portaB,*teto2B,*fx1B,*fx2B,*c3B,*caixaB,*fxB,*capitaoB,*mensagemxB,*fportaB; 
      
      SAMPLE *passoB,*vozB,*ptB,*abreportaB,*fechaportaB;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta=1,//controla se porta aberto ou fechado 1 eh fechado
      opcao=0,
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
     // fps,
      vida=energia,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      tiroflag=0,
      tiro,
      mensagem,
      xs=355,//controla a posicao da coluna inicial do capitao se movimentando atraves da funcao
      ys=479,//controla a posicao da linha inicial do capitao se movimentando atraves da funcao
      direcao=2,//controla direcao do capitao movimentando na tela passando por funcao
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo
      tempo2==0;//zera tempo
      tempo3==0;//zera tempo


      install_keyboard();
      bufferB = create_bitmap(MAX_X,MAX_Y);
      fundo2B = create_bitmap(MAX_X,MAX_Y);
      
      fundo2B = load_gif("fundos/fundobase112capitao.gif", NULL);
      painelB = load_gif("fundos/painel.gif",NULL);
      pc1B = load_gif("objetos/BASEESPACIAL/pcx1.gif", NULL);
      pc2B = load_gif("objetos/BASEESPACIAL/pcx2.gif", NULL);
      pc1Bf = load_gif("objetos/BASEESPACIAL/pcx1f.gif", NULL);
      pc2Bf = load_gif("objetos/BASEESPACIAL/pcx2f.gif", NULL);
      telaB = load_gif("objetos/BASEESPACIAL/telacapitao.gif", NULL);
      cientista1B = load_gif("objetos/BASEESPACIAL/cientista1.gif", NULL);
      cientista2B = load_gif("objetos/BASEESPACIAL/cientista2.gif", NULL);
      cientista3B = load_gif("objetos/BASEESPACIAL/cientista3.gif", NULL);
      cientista4B = load_gif("objetos/BASEESPACIAL/cientista4.gif", NULL);
      cientistaX = load_gif("objetos/BASEESPACIAL/cientistaX.gif", NULL);
      mensagemxB = load_gif("objetos/BASEESPACIAL/mensagem.gif", NULL);
      
      chaoB = load_gif("fundos/BASEESPACIAL/fd.gif", NULL);
      chao2B = load_gif("fundos/BASEESPACIAL/xx2.gif", NULL);
      tetoB = load_gif("fundos/BASEESPACIAL/fd2.gif",NULL);
      teto2B = load_gif("fundos/BASEESPACIAL/f3.gif",NULL);
      f3B = load_gif("fundos/BASEESPACIAL/fd3.gif", NULL); 
      f4B = load_gif("fundos/BASEESPACIAL/fd4.gif", NULL);
      f5B = load_gif("fundos/BASEESPACIAL/fd5.gif", NULL);
      fx1B = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2B = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portaB = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      fxB = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      

      passoB = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptB = load_wav("musicas/tirox.wav");
      else
         ptB = load_wav("musicas/tiro.wav");
      
      ////////////////////////////
      abreportaB = load_wav("musicas/Dooropen.wav");
      fechaportaB = load_wav("musicas/Doorclose.wav");
      fportaB = load_gif("objetos/BASEESPACIAL/tporta.gif", NULL);
      ////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400;
      play_sample(fechaportaB,250,130,1000,0); 
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<150;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<50;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=652;x<800;x++)//parede da direita
               {
                 for(y=0;y<448;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=496;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=658;x<800;x++)//porta da frente trancando passagem ateh atirar para abrir
               {
                 for(y=440;y<496;y++)
                   matriz[x][y]=1;
               }
               
                 
               /////////  
      ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////

      
      vozB = load_wav("musicas/vpulo.wav");
      
       if(roupa==0)
         strcpy(posicao,"personagem/d2.gif");
       else
         strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
      
      if(cenacapitao==-1)//se ja mostrou cena do capitao entao nao mostra mais mensagem
       mensagem=-1;
      else
       mensagem=0;  
       
      i=1;

      x=643;
      y=495;

      d=1;//posicao inicial
      
      // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
               while ( ::fps_speed > 0 )
               { 
                    if(loop==0)
                    {
                               clear(bufferB);
                               
                                /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////      
                              if(tirof[0].x-15>695 && porta==1 && tirof[0].y>450)
                              {
                                   porta=0;
                
                                   for(x2=658;x2<800;x2++)//porta da frenteliberando passagem apos atirar para abrir
                                   {
                                     for(y2=440;y2<496;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportaB,250,130,1000,0);
                              } 
                               
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                 
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W] && mensagem==0) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passoB,vozB,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
                    
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--; 
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;    
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
                    
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W] && mensagem==0) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passoB,vozB,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;     
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro                
                                   
                                   
                                  
                                    if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                    {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d2.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                    }
                    
                                   
                                   
                                        if(key[KEY_S] && mensagem==0) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passoB,vozB,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W] && mensagem==0) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passoB,vozB,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE] && mensagem==0) //TIRO
                                               {
                                                  // if(tiro==0)            
                                                  //  sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptB);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptB);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptB);                                                                                                                                       
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoB,vozB,roupa,lpulo); 
                                                  // rest(10);    
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                 
                                                                                                                                                                                        
                                                 if(key[KEY_SPACE] && matriz[x][y-1]==0 && mensagem==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoB,vozB,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+1]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }     
                                                  
                                                  
                                                  
                                                  
                                                  
                                                  //////////////////////CONTROLE DE GRAVIDADE
                                                  if(matriz[x][y+1]==0 && !key[KEY_SPACE] && mensagem==0)//personagem caindo apos um pulo,ou se nao tiver chao   
                                                  {     
                                                     y++;
                                                     
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                         if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                    
                                                  }      
                                                  /////////////////////CENARIO/////////////////////////////////////////////////    
                                                   draw_sprite(bufferB,fundo2B,0,0); 
                                                                                                    
                                                   
                                                   if(i%2==0)//COMPUTADOR PISCANDO
                                                   {
                                                      draw_sprite(bufferB,pc1B,450,390);
                                                      draw_sprite(bufferB,pc1Bf,130,390);
                                                      i=1;
                                                   }   
                                                    else 
                                                      {
                                                        draw_sprite(bufferB,pc2B,450,390);
                                                        draw_sprite(bufferB,pc2Bf,130,390);
                                                        i=0;
                                                       }
                                                   
                                                   install_int(IncrementaT, 1500);//movimenta cientista e pica monitor grande
                                                   if(tempo==1)   
                                                   {                            
                                                     draw_sprite(bufferB,telaB,259,216);//tela grande picando 
                                                     draw_sprite(bufferB,cientista1B,505,487);                                  
                                                     draw_sprite(bufferB,cientista3B,240,487);
                                                   }
                                                   else
                                                     {
                                                        draw_sprite(bufferB,cientista2B,505,487);
                                                        draw_sprite(bufferB,cientista4B,240,487);                                       
                                                     }  
                                                     
                                                     draw_sprite(bufferB,cientistaX,55,474);     
                                                                 
                                                    pB = load_gif(posicao,NULL);
                                                   
                                                   install_int(IncrementaT3,400);
                                                   if(tempo3<5 && direcao!=10)
                                                    movimentopersonagem(bufferB,capitaoB,xs,ys,160,430,direcao,0,2);//funcao movimento personagem
                                                   else
                                                     {
                                                        direcao=10;//capitao ja se apresentou
                                                        capitaoB = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao5.gif",NULL); 
                                                        draw_sprite(bufferB,capitaoB,355,479);
                                                      }
                                                                  
                                                    if(a==2)
                                                      show_tiros(x,y,bufferB,3,tirof,tirot,tiroc,matriz,roupa);
                                                    else
                        	                          show_tiros(x,y,bufferB,d,tirof,tirot,tiroc,matriz,roupa);
                                                                           
                                                  if(roupa==1)     
                                                  { 
                                                    if(!key[KEY_W] && !key[KEY_S])                                  
                                                      draw_sprite(bufferB,pB,x,y-10); 
                                                    else
                                                      if(key[KEY_W])
                                                       draw_sprite(bufferB,pB,x,y-22);
                                                      else
                                                        if(key[KEY_S]) 
                                                         draw_sprite(bufferB,pB,x,y);
                                                  } 
                                                  else
                                                    draw_sprite(bufferB,pB,x,y);
                                                 
                                                  /////////////MONTANDO CENARIO////////////////////////
                    
                                                       
                                                       draw_sprite(bufferB,f4B,5,10);
                                                       draw_sprite(bufferB,f5B,700,10);             
                                                       draw_sprite(bufferB,chaoB,0,564);
                                                       draw_sprite(bufferB,tetoB,0,55);                                                   
                                                       
                                                       draw_sprite(bufferB,fxB,-150,365);
                                                       draw_sprite(bufferB,fxB,-150,410);
                                                       draw_sprite(bufferB,fxB,-150,310);
                                                       draw_sprite(bufferB,fxB,-150,255);
                                                       draw_sprite(bufferB,fxB,-150,205);
                                                       draw_sprite(bufferB,fxB,-150,160);
                                                       draw_sprite(bufferB,fxB,-150,510);
                                                       draw_sprite(bufferB,fxB,-150,460);
                                                      
                                                       draw_sprite(bufferB,fxB,750,365);
                                                       draw_sprite(bufferB,fxB,750,420);
                                                       draw_sprite(bufferB,fxB,750,310);
                                                       draw_sprite(bufferB,fxB,750,255);
                                                       draw_sprite(bufferB,fxB,750,160);
                                                       draw_sprite(bufferB,fxB,750,205);                                                        
                                                       
                                                       draw_sprite(bufferB,portaB,700,463);
                                                       if(porta==1)//portas fechadas ou abertas
                                                         draw_sprite(bufferB,fportaB,695,465);

                                                       ////////////////////////////////////////////////////
                                                       
                                                       
                                                       /////////////////CENA CAPITAO/////////////////////////////////////////
                                                   /*  if(xcenario>-365 && xcenario<-355 && tempo2<3 && roupa==0)//cena monitor
                                                        mensagem=1;
                                                      else
                                                     //   if(roupa==1)
                                                         mensagem=0;*/
                                                      
                                                      if(cenacapitao==-1)  
                                                      {    install_int(IncrementaT2, 5000);
                                                           if(mensagem==-1)
                                                           {                       
                                                            // draw_sprite(bufferj,cenaj,279,284); 
                                                             draw_sprite(bufferB,mensagemxB,0,170);
                                                            // strcpy(posicao,"personagem/a00.gif");
                                                        
                                                               if(key[KEY_SPACE] && tempo2>3)
                                                               {
                                                                 mensagem=1;
                                                                tempo2=0;
                                                                remove_keyboard();
                                                                install_keyboard();
                                                             }  
                                                  
                                                              textprintf_centre_ex(bufferB, font,400,185, makecol(0, 255, 0), makecol(0, 0, 0),"Samus sua missão foi um sucesso, finalmente o lider pirata Mother Brain foi destruido,");
                                                              textprintf_centre_ex(bufferB, font,400,205, makecol(0, 255, 0), makecol(0, 0, 0),"porém perdemos o ultimo metroid e não podemos mais continuar nossa pesquisa sobre os"); 
                                                              textprintf_centre_ex(bufferB, font,400,225, makecol(0, 255, 0), makecol(0, 0, 0),"metroids.Também sua armadura foi muito danificada,não foi uma missão facil para você!!"); 
                                                              if(tempo2>3)        
                                                                textprintf_centre_ex(bufferB, font,670,250, makecol(0, 255, 255), makecol(0, 0, 0),"Pressione SPACE para continuar!!");                                               
                                                              
                                                           }
                                                           else  
                                                               if(mensagem==1)
                                                               {                  
                                                                 //draw_sprite(bufferj,cenaj,279,284); 
                                                                 draw_sprite(bufferB,mensagemxB,0,170);
                                                                // strcpy(posicao,"personagem/a00.gif");
                                                                 
                                                            
                                                                  if(key[KEY_SPACE] && tempo2>3)
                                                                  {
                                                                     mensagem=2;
                                                                     tempo2=0;
                                                                     remove_keyboard();
                                                                     install_keyboard();
                                                                     remove_int(IncrementaT2);
                                                                  }  
                                                   
                                                                  textprintf_centre_ex(bufferB, font,390,185, makecol(0, 255, 0), makecol(0, 0, 0),"Finalmente a galáxia agora esta em paz,pois derrotamos todos os piratas espaciais que ");
                                                                  textprintf_centre_ex(bufferB, font,390,205, makecol(0, 255, 0), makecol(0, 0, 0),"estavam refugiados no planete zebes.Aproveite e va olhar a nova armadura da federação"); 
                                                                  textprintf_centre_ex(bufferB, font,390,225, makecol(0, 255, 0), makecol(0, 0, 0),"galática,que esta no laboratório,o guarda vai liberar o caminho para você ir la.");
                                                                  if(tempo2>3)        
                                                                     textprintf_centre_ex(bufferB, font,670,250, makecol(0, 255, 255), makecol(0, 0, 0),"Pressione SPACE para continuar!!");                                                           
                                                               } 
                                                                else
                                                                  if(mensagem==2)
                                                                   {          
                                                                    // install_int(IncrementaT2, 1000);            
                                                                   //  draw_sprite(bufferj,cenaj,279,284); 
                                                                     draw_sprite(bufferB,mensagemxB,0,170);
                                                                  //   strcpy(posicao,"personagem/a00.gif");
                                                                  
                                                                  
                                                                      if(key[KEY_SPACE] && tempo2>3)
                                                                      {
                                                                        mensagem=0;
                                                                        cenacapitao=1;
                                                                        remove_keyboard();
                                                                        install_keyboard();
                                                                      }  

                                                                      textprintf_centre_ex(bufferB, font,390,185, makecol(0, 255, 0), makecol(0, 0, 0),"No momento estamos criando uma nova armadura,pois percebemos que após a explosão");
                                                                      textprintf_centre_ex(bufferB, font,390,205, makecol(0, 255, 0), makecol(0, 0, 0),"do planeta Zebes,uma nave saiu do meio da explosão assim como você para um planeta"); 
                                                                      textprintf_centre_ex(bufferB, font,390,225, makecol(0, 255, 0), makecol(0, 0, 0)," ainda não indentificado.Descanse um pouco e aguarde novas instruções!!");
                                                                      if(tempo2>3)     
                                                                         textprintf_centre_ex(bufferB, font,670,250, makecol(0, 255, 255), makecol(0, 0, 0),"Pressione SPACE para continuar!!");                                               
                                                                    
                                                                   }
                                                                                                                                                                   
                                                       }
                                                       ////////////////////////////////////////////////////////////////////// 
                                                       
                                                  destroy_bitmap(pB);   
                                                 // destroy_bitmap(capitaoB);                                
                                                  /////////////////////////////////////////////////////                 
                                
                                textprintf_centre_ex(bufferB, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferB, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferB, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferB, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferB, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferB, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferB, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               textprintf_centre_ex(bufferB, font,50,410, makecol(0, 0, 0), makecol(255, 255, 255)," Tempo2 :%d",tempo2);
                               textprintf_centre_ex(bufferB, font,50,440, makecol(0, 0, 0), makecol(255, 255, 255)," MSN :%d",mensagem);
                               
                               draw_sprite(bufferB,painelB,0,0);
                               textprintf_centre_ex(bufferB, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                               //show_mouse(bufferi);
                               loop=1;
                          }                             
                          ::fps_speed--;
                          ::fps++;
               }   
               loop=0;               
               draw_sprite(screen,bufferB,0,0);//desenha na tela a nova imagem 
               
               
             if(x>720)//voltou cenario
               i=-1;
             
          //   if(x==185)
           //    ativa=1;//ativou para abrir elevador na fase atras  
        
            // rest(fps/3300);//controle de frames
            vsync();
    }while (i!=-1 && x!=-1 && fim!=-10000);
     
      destroy_bitmap(fundo2B);
      destroy_bitmap(bufferB);
      destroy_bitmap(painelB);
      destroy_bitmap(pc1B);
      destroy_bitmap(pc2B);
      destroy_bitmap(pc1Bf);
      destroy_bitmap(pc2Bf);
      destroy_bitmap(telaB);
      destroy_bitmap(cientista1B);
      destroy_bitmap(cientista2B);
      destroy_bitmap(cientista3B);
      destroy_bitmap(cientista4B);
      destroy_bitmap(cientistaX);
      //destroy_bitmap(capitaoB);
    //  remove_int(IncrementaT1);
      remove_int(IncrementaT2);
      remove_int(IncrementaT3);
      
      destroy_bitmap(chaoB);
      destroy_bitmap(chao2B);
      destroy_bitmap(tetoB);
      destroy_bitmap(teto2B);
      destroy_bitmap(f3B);
      destroy_bitmap(f4B);
      destroy_bitmap(f5B);
      destroy_bitmap(portaB);
      destroy_bitmap(fx1B);
      destroy_bitmap(fx2B);
      
      destroy_sample(passoB);
      destroy_sample(vozB);
      destroy_sample(ptB);
      
     // destroy_bitmap(fportaB);
      destroy_sample(abreportaB);
      destroy_sample(fechaportaB);


     if(i==-1)//diz que o jogador ta voltando um cenario
     {
       passaporta(1,20,495-30,fps);
       return 12;//diz pra funcao jogo pra chamar a funcao 112 que ta voltano desse cenario
     }  
     else
        if(fim==-10000)
         return -10000; 
    
}

int cenario113(int estado,int fps,int roupa)
{
      BITMAP *bufferC,*fundo2C,*fundo1C,*pC,*painelC,*elevadorc1C,*elevadorc2C,*pcmC,*mpcmC,*cientista1C,*cientista2C,*cientista3C,*capsula1C,*capsula2C,*capsulasuporteC,*chaoC,*chao2C,
      *tetoC,*f3C,*f4C,*f5C,*portaC,*teto2C,*fx1C,*fx2C,*c3C,*caixaC,*fxC,*fportaC;
       
      SAMPLE *passoC,*vozC,*ptC,*abreportaC,*fechaportaC;
      char posicao[20];
      int matriz[800][600];
      Tiro tirof[3],tirot[3],tiroc[3];
       
      int x=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      x2=0,
      y2=0,
      porta1=1,//controle de porta,se 1 porta fechada
      opcao=0,
      i=0,//diz qual bitmap da nave vai ser mostrado
      d=0,//direcao q o personagem esta virado
      a=0,//diz qual o estado do personagem,em pé,agaixado,virado para frente,para tras
      fim=0,////controla de o cara ta saindo do jogo e voltando para o menu ou algo q usa essa mesma variavel
     // fps,
      vida=energia,
      loop=0,//controle de escrita no buffer com o uso do while do fps
      tiroflag=0,
      tiro,
      mt=0,//variavel q controla sprites para tras
      mf=0,//variavel q controla sprites para frente
      sinalpv=0,//variavel q diz se sai som da voz o pulo ou nao por parametro
      lpulo=0;//variavel q diz se eh limite de pulo


      install_keyboard();
      bufferC = create_bitmap(MAX_X,MAX_Y);
      fundo1C = create_bitmap(MAX_X,MAX_Y);
      fundo2C = create_bitmap(MAX_X,MAX_Y);
      
      fundo1C = load_gif("fundos/cenario113.gif", NULL); 
      fundo2C = load_gif("fundos/fundobase.gif", NULL);
      painelC = load_gif("fundos/painel.gif",NULL);
      elevadorc1C = load_gif("objetos/BASEESPACIAL/elevadorcentro.gif", NULL);
      elevadorc2C = load_gif("objetos/BASEESPACIAL/elevadorcentro2.gif", NULL);
      
      pcmC = load_gif("objetos/BASEESPACIAL/pcm.gif", NULL); 
      mpcmC = load_gif("objetos/BASEESPACIAL/monitorx.gif", NULL);
      cientista1C = load_gif("objetos/BASEESPACIAL/cientistax1.gif",NULL);
      cientista2C = load_gif("objetos/BASEESPACIAL/cientistax2.gif", NULL);
      cientista3C = load_gif("objetos/BASEESPACIAL/cientistax3.gif", NULL);
      capsula1C = load_gif("objetos/BASEESPACIAL/capsula1.gif", NULL);
      capsula2C = load_gif("objetos/BASEESPACIAL/capsula2.gif", NULL);
      capsulasuporteC = load_gif("objetos/BASEESPACIAL/capsulasuporte.gif", NULL);
      
      chaoC = load_gif("fundos/BASEESPACIAL/fd.gif", NULL);
      chao2C = load_gif("fundos/BASEESPACIAL/xx2.gif", NULL);
      tetoC = load_gif("fundos/BASEESPACIAL/fd2.gif",NULL);
      teto2C = load_gif("fundos/BASEESPACIAL/f3.gif",NULL);
      f3C = load_gif("fundos/BASEESPACIAL/fd3.gif", NULL); 
      f4C = load_gif("fundos/BASEESPACIAL/fd4.gif", NULL);
      f5C = load_gif("fundos/BASEESPACIAL/fd5.gif", NULL);
      fx1C = load_gif("fundos/BASEESPACIAL/fx.gif", NULL); 
      fx2C = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      portaC = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
      fxC = load_gif("fundos/BASEESPACIAL/fxc.gif", NULL);
      
      passoC = load_wav("musicas/passo.wav");
      if(roupa==0)
        ptC = load_wav("musicas/tirox.wav");
      else
         ptC = load_wav("musicas/tiro.wav");
      
      fportaC = load_gif("objetos/BASEESPACIAL/fporta.gif", NULL);
      
      abreportaC = load_wav("musicas/Dooropen.wav");
      fechaportaC = load_wav("musicas/Doorclose.wav");
      ////////////////////////////
      //deixando tiros no centro.pois quando carregados,sera verificado se a posicao x do tiro da posicao 0 do vetor chegou a posicao para abrir a porta
      tirof[0].x=400;
      tirot[0].x=400;
      tirof[0].y=-400;
      tirot[0].y=-400; 
      play_sample(fechaportaC,250,130,1000,0); 
      ////////////////////////////
      
      //////CARREGANDO ESQUELETO DO CENARIO NA MATRIZ//////
            ///preenchendo matriz com zeros
              for(x=0;x<800;x++)
              {
                 for(y=0;y<600;y++)
                   matriz[x][y]=0;//parede de cima
              }
            /////////////////////////////

               for(x=0;x<800;x++)//parede de cima
               {
                 for(y=0;y<150;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<85;x++)//parede dA ESQUERDA
               {
                 for(y=0;y<460;y++)
                   matriz[x][y]=1;
               }
               
               for(x=710;x<800;x++)//parede da direita
               {
                 for(y=0;y<600;y++)
                   matriz[x][y]=1;
               }
              
               for(x=0;x<800;x++)//chao
               {
                 for(y=497;y<600;y++)
                   matriz[x][y]=1;
               }
               
               for(x=0;x<85;x++)//porta de tras trancando passagem ateh atirar para abrir
               {
                 for(y=430;y<497;y++)
                   matriz[x][y]=1;
               }
               
                           
                 
               /////////  
      ////////////////////////////////////////////////////
      
      /////////////CARREGANDO TIROS///////////////////////////////////////
      iniciatiro(tirof,tirot,tiroc,0,tiroflag);
      //////////////////////////////////////////////////////////////////

      
      vozC = load_wav("musicas/vpulo.wav");
      
       if(roupa==0)
        strcpy(posicao,"personagem/d2.gif");
       else 
         strcpy(posicao,"personagem/ARMADURA/d2.gif");
        
      i=1;

      x=105;
      y=495;
      
      d=2;//posicao inicial
      
      // a cada 1 segundo mostra quantas vezes a imagem foi impressa na tela
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(FPS_V) );
      do
      {    
               while ( ::fps_speed > 0 )
               { 
                    if(loop==0)
                    {
                               clear(bufferC);
                               
                                 /////////////////////////VERIFICANDO SE PORTA ESTA SENDO ABERTA COM O TIRO E JA FAZENDO O SOM DELA ABRINDO/////////////                                  
                              if(tirot[0].x+15<87 && porta1==1 && tirot[0].y>450)
                              {
                                   porta1=0; 
                                   for(x2=0;x2<85;x2++)//porta de tras liberando passagem apos atirar para abrir
                                   {
                                     for(y2=430;y2<497;y2++)
                                       matriz[x2][y2]=0;
                                   }
                                   play_sample(abreportaC,250,130,1000,0);
                              } 
                               
                               if (key[KEY_ENTER])//MENU DO PAUSE COM MAPA
                                    fim=pause1(0);
                                 
                                 if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
                                 { 
                                   if (is_windowed_mode())     
                                       set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
                                    
                                   else            
                                       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
                                 }
                                 
                                 if (key[KEY_A] && !key[KEY_F] && !key[KEY_D] && !key[KEY_W]) //TRAS
                                 {                                               
                                           movimentos(d,a,x,y,mf,mt,posicao,1,passoC,vozC,roupa,lpulo); 
                                        
                                               if(a==0 && matriz[x-1][y]==0)
                                                 x--;  
                    
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;   
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--; 
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;  
                                                 if(a==0 && matriz[x-1][y]==0)
                                                   x--;     
                                               
                                    if(mt>180)//se numero de sprites for o maximo retorna
                                     mt=0;  
                                                           
                                     mt=mt+9; 
                    
                                    d=1;         
                                 }
                                 else
                                   mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
                                   
                                   if(d==1 && mt==0 && a==0 && !key[KEY_A] && !key[KEY_SPACE])//se A para tras e contador de SPRITES M == 0 entao coloca foto personagem parado para tras e A==0 para tras em peh
                                   {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d1.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                   }
                                   
                                   
                                   
                                    if (key[KEY_D] && !key[KEY_F] && !key[KEY_A] && !key[KEY_W]) //FRENTE
                                    {                           
                                                                       
                                            movimentos(d,a,x,y,mf,mt,posicao,1,passoC,vozC,roupa,lpulo); 
                                               
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++; 
                                              if(a==0 && matriz[x+1][y]==0)
                                                x++;
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;  
                                                  if(a==0 && matriz[x+1][y]==0)
                                                    x++;      
                    
                                      
                                          if(mf>180)//se numero de sprites for o maximo retorna
                                            mf=0; 
                                          mf=mf+9;
                                          d=2; 
                                    }
                                    else
                                       mf=0; //se nao for para frente entao M contador de sprites fica o primeiro                
                                   
                                   
                                  
                                    if(d==2 && mf==0 && a==0 && !key[KEY_D] && !key[KEY_SPACE])//se D para frente e contador de SPRITES M == 0 entao coloca foto personagem parado para frente A==0 para tras agaixado
                                    {
                                     if(roupa==0)      
                                      strcpy(posicao,"personagem/d2.gif");
                                     else
                                       strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                    }
                    
                                   
                                   
                                        if(key[KEY_S]) //BAIXO
                                        {
                                          a=1;             
                                          movimentos(d,a,x,y,mf,mt,posicao,1,passoC,vozC,roupa,lpulo);      
                                        }
                                        else
                                           if(key[KEY_W]) //CIMA
                                           {                               
                                              a=2;
                                              movimentos(d,a,x,y,mf,mt,posicao,1,passoC,vozC,roupa,lpulo);                                      
                                            }
                                            else
                                              a=0;
                                              
                                               if(key[KEY_F] && tiroflag==0 && !key[KEY_SPACE]) //TIRO
                                               {
                                              //     if(tiro==0)            
                                               //     sinalpv=2;
                                                   
                                                   if (tiroflag == 0)
                    		                       {
                    		                        	tiroflag = 1;
                    		                        	
                    		                        	if(a==2)
                                                          atirar(x,y,3,tirof,tirot,tiroc,ptC);
                                                        else
                                                          if(a==1)
                                                            atirar(x,y+18,d,tirof,tirot,tiroc,ptC);//tiro agaixado
                                                          else  
                                                            atirar(x,y,d,tirof,tirot,tiroc,ptC);                                                                                                                                       
                                                   }  
                                                   movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoC,vozC,roupa,lpulo); 
                                                  // rest(10);    
                                                } 
                                                else
                    	                       	  tiroflag = 0;
                    	                       
                    	                                 
                                                                                                                                                                                        
                                                  if(key[KEY_SPACE] && matriz[x][y-1]==0) //PULO
                                                 {                
                                                         movimentos(d,a,x,y,mf,mt,posicao,sinalpv,passoC,vozC,roupa,lpulo);  
                                                         pulo(x,y,lpulo,sinalpv,matriz);                                                                                
                           
                                                  }
                                                  //  else
                                                     // {                              
                                                         if(matriz[x][y+2]==1 && sinalpv==1)//caso esteja encima de uma caixa
                                                         {
                                                            lpulo=0; 
                                                            sinalpv=0;
                          
                                                         } 
                                                          
                                                     // }     
                                                  
                                                  //////////////////////CONTROLE DE GRAVIDADE
                                                  if(matriz[x][y+1]==0 && !key[KEY_SPACE])//personagem caindo apos um pulo,ou se nao tiver chao   
                                                  {     
                                                     y++;
                                                     
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                         if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++;
                                                           if(matriz[x][y+1]==0)
                                                           y++;  
                                                         if(matriz[x][y+1]==0)
                                                           y++; 
                    
                                                  }      
                                                  /////////////////////CENARIO/////////////////////////////////////////////////    
                                                   draw_sprite(bufferC,fundo2C,0,0); 
                                                   
                                                   draw_sprite(bufferC,pcmC,120,133); //pccentral
                                                   
                                                   draw_sprite(bufferC,capsula1C,553,493); //parte de baixo da capsula
                                                   draw_sprite(bufferC,capsula2C,553,320); //parte de cima da capsula
                                                   draw_sprite(bufferC,capsulasuporteC,594,110); //suporte encima da capsula que a levanta
                                                   
                                                   if(i%2==0)//ELEVADOR
                                                   {
                                                      draw_sprite(bufferC,elevadorc1C,365,552);
                                                      draw_sprite(bufferC,mpcmC,273,477); //luz que pisca no monitor do pc central
                                                      i=1;
                                                   }   
                                                    else 
                                                      {
                                                        draw_sprite(bufferC,elevadorc2C,365,552);
                                                        i=0;
                                                       }       
                                                    
                                                    draw_sprite(bufferC,cientista1C,288,473); //cientista na frente do pc                  
                                                    pC = load_gif(posicao,NULL);
                                                                
                                                    if(a==2)
                                                      show_tiros(x,y,bufferC,3,tirof,tirot,tiroc,matriz,roupa);
                                                    else
                        	                          show_tiros(x,y,bufferC,d,tirof,tirot,tiroc,matriz,roupa);
                                                                           
                                                  if(roupa==1)     
                                                  { 
                                                    if(!key[KEY_W] && !key[KEY_S])                                  
                                                      draw_sprite(bufferC,pC,x,y-15); 
                                                    else
                                                      if(key[KEY_W])
                                                       draw_sprite(bufferC,pC,x,y-22);
                                                      else
                                                        if(key[KEY_S]) 
                                                         draw_sprite(bufferC,pC,x,y);
                                                  } 
                                                  else
                                                    draw_sprite(bufferC,pC,x,y-2);
                                                 
                                                 
                                                 /////////////MONTANDO CENARIO////////////////////////
                    
                                                       
                                                       draw_sprite(bufferC,f4C,48,10);
                                                       draw_sprite(bufferC,f5C,750,10);             
                                                       draw_sprite(bufferC,chaoC,0,564);
                                                       draw_sprite(bufferC,tetoC,0,55);                                                   
                                                       
                                                       draw_sprite(bufferC,fxC,-120,365);
                                                       draw_sprite(bufferC,fxC,-120,410);
                                                       draw_sprite(bufferC,fxC,-120,310);
                                                       draw_sprite(bufferC,fxC,-120,255);
                                                       draw_sprite(bufferC,fxC,-120,205);
                                                       draw_sprite(bufferC,fxC,-120,160);                                              
                                                       
                                                       draw_sprite(bufferC,portaC,-5,465);
                                                       if(porta1==1)
                                                          draw_sprite(bufferC,fportaC,95,465);

                                                       ////////////////////////////////////////////////////
                                                  destroy_bitmap(pC);                                  
                                                  /////////////////////////////////////////////////////                 
                                
                                textprintf_centre_ex(bufferC, font,30,280, makecol(0, 0, 0), makecol(255, 255, 255),"X:%d",x);
                               textprintf_centre_ex(bufferC, font,30,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y); 
                               //textprintf_centre_ex(buffer, font,30,300, makecol(0, 0, 0), makecol(255, 255, 255),"C:%d",xcenario);
                               textprintf_centre_ex(bufferC, font,30,310, makecol(0, 0, 0), makecol(255, 255, 255),"MT :%d",mt);
                               textprintf_centre_ex(bufferC, font,30,330, makecol(0, 0, 0), makecol(255, 255, 255),"MF :%d",mf);
                               textprintf_centre_ex(bufferC, font,30,350, makecol(0, 0, 0), makecol(255, 255, 255),"MX :%d",mouse_x);
                               textprintf_centre_ex(bufferC, font,30,370, makecol(0, 0, 0), makecol(255, 255, 255),"MY :%d",mouse_y);
                               textprintf_centre_ex(bufferC, font,50,390, makecol(0, 0, 0), makecol(255, 255, 255)," FPS :%d",fps_antigo);
                               
                               
                               draw_sprite(bufferC,painelC,0,0);
                               textprintf_centre_ex(bufferC, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                               //show_mouse(bufferi);
                               loop=1;
                          }                             
                          ::fps_speed--;
                          ::fps++;
               }   
               loop=0;               
               draw_sprite(screen,bufferC,0,0);//desenha na tela a nova imagem 
               
               
             if(x<15)//voltou cenario
               i=-1;
             
            // if(x==185)
            //   ativa=1;//ativou para abrir elevador na fase atras  
        
            // rest(fps/3300);//controle de frames
            vsync();
    }while (i!=-1 && fim!=-10000);
     
      destroy_bitmap(pcmC);
      destroy_bitmap(mpcmC);
      destroy_bitmap(cientista1C);
      destroy_bitmap(cientista2C);
      destroy_bitmap(cientista3C);
      destroy_bitmap(capsula1C);
      destroy_bitmap(capsula2C);
      destroy_bitmap(capsulasuporteC);
      
      destroy_bitmap(fportaC);
      destroy_sample(abreportaC);
      destroy_sample(fechaportaC);
     
      destroy_bitmap(fundo1C);
      destroy_bitmap(fundo2C);
      destroy_bitmap(bufferC);
      destroy_bitmap(painelC);
      destroy_bitmap(elevadorc1C);
      destroy_bitmap(elevadorc2C);
      destroy_sample(passoC);
      destroy_sample(vozC);
      destroy_sample(ptC);  

     if(i==-1)//diz que o jogador ta voltando um cenario
     {
       passaporta(0,20,495-30,fps);
       return 0;//diz pra funcao jogo pra chamar a funcao 18 que ta voltano desse cenario
     }  
      else
        if(fim==-10000)
         return -10000;
    
}


void movimentos(int d,int a,int x,int y,int mf,int mt,char posicao[20],int sinalpv,SAMPLE *passo,SAMPLE *voz,int roupa,int lpulo)
{
      int opcao=0,i=0,e=0,fim=0,vida=99,tiro;//variaveis que pega a posicao do mause

             if (key[KEY_F1]) //TELA TODA OU TELA PEQUENA
             { 
               if (is_windowed_mode())     
                   set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0); 
               else            
                   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
             }
             else
             if (key[KEY_A] && !key[KEY_S] && lpulo==0)//TRAS
             {
                   //  if(y==495) 
                    // {       
                              if(d==1 && a==1 && mt==0)  
                              {           
                                if(roupa==0)          
                                  strcpy(posicao,"personagem/e1.gif"); 
                                else 
                                  strcpy(posicao,"personagem/ARMADURA/ex1.gif");  
                              } 
                              else 
                                if(d==2 && a==1 && mt==0) 
                                {
                                  if(roupa==0)      
                                    strcpy(posicao,"personagem/e2.gif");
                                  else 
                                    strcpy(posicao,"personagem/ARMADURA/ex2.gif");   
                                }  
                                else        
                                  if(a==0 && mt==0)  
                                  {   
                                    if(roupa==0)       
                                      strcpy(posicao,"personagem/d1.gif");
                                    else   
                                      strcpy(posicao,"personagem/ARMADURA/dx1.gif");
                                  }  
                                   else
                                      if(mt>0 && mt<20)
                                      {
                                          //som do passo  
                                                  
                                        /////////////////////////////////
                                        if(mt==18)
                                        {
                                          if(sinalpv==1 && !key[KEY_SPACE] && !key[KEY_S] && !key[KEY_W])//se nao for voz de pulo faz som de andar
                                              play_sample(passo,250,130,1000,0);          
                                        }
                                          if(roupa==0)
                                            strcpy(posicao,"personagem/mt1.gif");
                                          else
                                            strcpy(posicao,"personagem/ARMADURA/mtx1.gif");  
                                      } 
                                      else 
                                          if(mt>20 && mt<40)
                                          {  
                                             if(roupa==0)
                                                strcpy(posicao,"personagem/mt2.gif");
                                             else
                                               strcpy(posicao,"personagem/ARMADURA/mtx2.gif");  
                                          }  
                                          else
                                              if(mt>40 && mt<60)
                                              {
                                                if(sinalpv==1 && !key[KEY_SPACE] && !key[KEY_S] && !key[KEY_W])//se nao for voz de pulo faz som de andar
                                                {         
                                                    rest(5);        
                                                    play_sample(passo,250,130,1000,0);
                                                } 
                                                      if(roupa==0)
                                                        strcpy(posicao,"personagem/mt3.gif");
                                                      else
                                                        strcpy(posicao,"personagem/ARMADURA/mtx3.gif");
                                              } 
                                              else
                                                if(mt>60 && mt<90)
                                                {
                                                    if(roupa==0)
                                                        strcpy(posicao,"personagem/mt4.gif");
                                                      else
                                                        strcpy(posicao,"personagem/ARMADURA/mtx4.gif");    
                                                }         
                                                else
                                                  if(mt>90 && mt<110)
                                                  {
                                                      if(roupa==0)
                                                        strcpy(posicao,"personagem/mt5.gif");
                                                      else
                                                        strcpy(posicao,"personagem/ARMADURA/mtx5.gif");         
                                                  }  
                                                  else
                                                   if(mt>110 && mt<130)
                                                   {
                                                          if(roupa==0)
                                                            strcpy(posicao,"personagem/mt6.gif");
                                                          else
                                                            strcpy(posicao,"personagem/ARMADURA/mtx6.gif");          
                                                   }  
                                                    else
                                                       if(mt>130 && mt<150)
                                                       {
                                                          if(roupa==0)
                                                            strcpy(posicao,"personagem/mt7.gif");
                                                          else
                                                            strcpy(posicao,"personagem/ARMADURA/mtx7.gif");         
                                                       } 
                                                       else
                                                         if(mt>150 && mt<170)
                                                         {
                                                           if(roupa==0)
                                                            strcpy(posicao,"personagem/mt8.gif");
                                                          else
                                                            strcpy(posicao,"personagem/ARMADURA/mtx8.gif");          
                                                         }    
                                                         else
                                                          if(mt>170 && mt<190)
                                                          {
                                                             if(roupa==0)
                                                                strcpy(posicao,"personagem/mt9.gif");
                                                              else
                                                                strcpy(posicao,"personagem/ARMADURA/mtx9.gif");          
                                                          }   
                                                          else
                                                           if(mt>190)
                                                           {
                                                              if(roupa==0)
                                                                strcpy(posicao,"personagem/mt10.gif");
                                                              else
                                                                strcpy(posicao,"personagem/ARMADURA/mtx10.gif");           
                                                           }          
                  //   }
                    // else
                      // mt=0; //se nao for para tras entao M contador de sprites fica o primeiro
              }
              else 
                if (key[KEY_D] && !key[KEY_S] && lpulo==0)//FRENTE 
                {
                        //if (y==495 ) 
                        //{                           
                                                           
                              if(d==1 && a==1 && mf==0)  
                              {           
                                if(roupa==0)          
                                  strcpy(posicao,"personagem/e1.gif"); 
                                else 
                                  strcpy(posicao,"personagem/ARMADURA/ex1.gif");  
                              } 
                              else 
                                if(d==2 && a==1 && mf==0) 
                                {
                                  if(roupa==0)      
                                    strcpy(posicao,"personagem/e2.gif");
                                  else 
                                    strcpy(posicao,"personagem/ARMADURA/ex2.gif");   
                                }  
                                else        
                                  if(a==0 && mf==0)  
                                  {   
                                    if(roupa==0)       
                                      strcpy(posicao,"personagem/d2.gif");
                                    else   
                                      strcpy(posicao,"personagem/ARMADURA/dx2.gif");
                                  }  
                                   else
                                      if(mf>0 && mf<20)
                                      {
                                          //som do passo  
                                                  
                                        /////////////////////////////////
                                        if(mf==16)
                                        {
                                          if(sinalpv==1 && !key[KEY_SPACE] && !key[KEY_S] && !key[KEY_W])//se nao for voz de pulo faz som de andar
                                              play_sample(passo,250,130,1000,0);          
                                        }
                                          if(roupa==0)
                                            strcpy(posicao,"personagem/mf1.gif");
                                          else
                                            strcpy(posicao,"personagem/ARMADURA/mfx1.gif");  
                                      } 
                                      else 
                                          if(mf>20 && mf<40)
                                          {  
                                             if(roupa==0)
                                                strcpy(posicao,"personagem/mf2.gif");
                                             else
                                               strcpy(posicao,"personagem/ARMADURA/mfx2.gif");  
                                          }  
                                          else
                                              if(mf>40 && mf<60)
                                              {
                                                if(sinalpv==1 && !key[KEY_SPACE] && !key[KEY_S] && !key[KEY_W])//se nao for voz de pulo faz som de andar
                                                {         
                                                    rest(5);        
                                                    play_sample(passo,250,130,1000,0);
                                                } 
                                                      if(roupa==0)
                                                        strcpy(posicao,"personagem/mf3.gif");
                                                      else
                                                        strcpy(posicao,"personagem/ARMADURA/mfx3.gif");
                                              } 
                                              else
                                                if(mf>60 && mf<90)
                                                {
                                                    if(roupa==0)
                                                        strcpy(posicao,"personagem/mf4.gif");
                                                      else
                                                        strcpy(posicao,"personagem/ARMADURA/mfx4.gif");    
                                                }         
                                                else
                                                  if(mf>90 && mf<110)
                                                  {
                                                      if(roupa==0)
                                                        strcpy(posicao,"personagem/mf5.gif");
                                                      else
                                                        strcpy(posicao,"personagem/ARMADURA/mfx5.gif");         
                                                  }  
                                                  else
                                                   if(mf>110 && mf<130)
                                                   {
                                                          if(roupa==0)
                                                            strcpy(posicao,"personagem/mf6.gif");
                                                          else
                                                            strcpy(posicao,"personagem/ARMADURA/mfx6.gif");          
                                                   }  
                                                    else
                                                       if(mf>130 && mf<150)
                                                       {
                                                          if(roupa==0)
                                                            strcpy(posicao,"personagem/mf7.gif");
                                                          else
                                                            strcpy(posicao,"personagem/ARMADURA/mfx7.gif");         
                                                       } 
                                                       else
                                                         if(mf>150 && mf<170)
                                                         {
                                                           if(roupa==0)
                                                            strcpy(posicao,"personagem/mf8.gif");
                                                          else
                                                            strcpy(posicao,"personagem/ARMADURA/mfx8.gif");          
                                                         }    
                                                         else
                                                          if(mf>170 && mf<190)
                                                          {
                                                             if(roupa==0)
                                                                strcpy(posicao,"personagem/mf9.gif");
                                                              else
                                                                strcpy(posicao,"personagem/ARMADURA/mfx9.gif");          
                                                          }   
                                                          else
                                                           if(mf>190)
                                                           {
                                                              if(roupa==0)
                                                                strcpy(posicao,"personagem/mf10.gif");
                                                              else
                                                                strcpy(posicao,"personagem/ARMADURA/mfx10.gif");           
                                                           }     
        
                     //   }
                      //  else
                       //    mf=0; //se nao for para frente entao M contador de sprites fica o primeiro

                }
                else
                    if(key[KEY_S]) //BAIXO
                    {
                      a=1;             
                      if(d==1)             
                      {
                        if(roupa==0)                   
                         strcpy(posicao,"personagem/e1.gif");  
                        else 
                          strcpy(posicao,"personagem/ARMADURA/ex1.gif");
                      } 
                      else 
                        if(d==2) 
                        {
                           if(roupa==0)                   
                             strcpy(posicao,"personagem/e2.gif");  
                            else 
                              strcpy(posicao,"personagem/ARMADURA/ex2.gif");                  
                        }  
                    }
                    else
                       if(key[KEY_W]) //CIMA
                       {
                          a=2;
                              if(d==1 && a==2) 
                              {
                                    if(roupa==0)                   
                                     strcpy(posicao,"personagem/c1.gif");  
                                    else 
                                      strcpy(posicao,"personagem/ARMADURA/cx1.gif");  
                              }  
                              else
                               if(d==2 && a==2) 
                                {
                                    if(roupa==0)                   
                                     strcpy(posicao,"personagem/c2.gif");  
                                    else 
                                      strcpy(posicao,"personagem/ARMADURA/cx2.gif");  
                                 }          
                        }
                        else                     
                           a=0;
                      
                            if(key[KEY_SPACE]) //PULO
                            {   
                                if(d==1)//virado tras
                                {
                                    if(roupa==0)                   
                                     strcpy(posicao,"personagem/p1.gif");  
                                    else 
                                      strcpy(posicao,"personagem/ARMADURA/px1.gif");  
                                }                 
                                if(d==2)  //virado frente
                                {
                                    if(roupa==0)                   
                                     strcpy(posicao,"personagem/p2.gif");  
                                    else 
                                      strcpy(posicao,"personagem/ARMADURA/px2.gif");  
                                 }
                                    
                                if(sinalpv==0)//se 0 eh pra fazer voz de pulo                                                              
                                   play_sample(voz,240,130,1000,0);   
                         
                            }      
                            else
                                if(key[KEY_F]) //TIRO
                                {                                                                                         
                                  
                                     if(d==1 && a==0 && !key[KEY_S]) //atira para tras
                                      {
                                        if(roupa==0)                   
                                         strcpy(posicao,"personagem/d1.gif");  
                                        else 
                                          strcpy(posicao,"personagem/ARMADURA/dx1.gif");  
                                       }     
                                      else
                                        if(d==2 && a==0 && !key[KEY_S]) //atira para frente
                                        {
                                            if(roupa==0)                   
                                             strcpy(posicao,"personagem/d2.gif");  
                                            else 
                                              strcpy(posicao,"personagem/ARMADURA/dx2.gif");  
                                         } 
                                         else
                                         if(d==1 && a==1)
                                          {
                                            if(roupa==0)                   
                                             strcpy(posicao,"personagem/e1.gif");  
                                            else 
                                              strcpy(posicao,"personagem/ARMADURA/ex1.gif");  
                                         }
                                         else
                                             if(d==2 && a==1)
                                             {
                                                if(roupa==0)                   
                                                 strcpy(posicao,"personagem/e2.gif");  
                                                else 
                                                  strcpy(posicao,"personagem/ARMADURA/ex2.gif");  
                                             }
                                                              
                                  }                         

}

void pulo(int &x,int &y,int &lpulo,int &sinalpv,int matriz[800][600])
{
        sinalpv=1;//ja mostrou som de pulo 
               if(lpulo<155) 
               {                                                        
                 lpulo++; 
                 
                    lpulo++;
                    lpulo++;
                    lpulo++;
                    lpulo++;
                    lpulo++;
                    lpulo++;
                  
                  if(matriz[x][y-5]==1 && lpulo<155)//chegou no teto ou pulou embaixo de uma parede nisso eh o fim do pulo
                   lpulo=155;
                 
                  if(matriz[x][y-1]==0 && lpulo<155)             
                   y--;
                  
                     if(matriz[x][y-1]==0 && lpulo<155)
                       y--;
                     if(matriz[x][y-1]==0 && lpulo<155)
                       y--;
                      if(matriz[x][y-1]==0 && lpulo<155)
                       y--;
                       if(matriz[x][y-1]==0 && lpulo<155)
                       y--;
                      if(matriz[x][y-1]==0 && lpulo<155)
                       y--;
                       if(matriz[x][y-1]==0 && lpulo<155)
                       y--;


               } 
               else
                 {
                   if(y<495 && matriz[x][y+1]==0)//caso pular segurando o pulo o personagem sobe,e se nao tiver soltado o botao pulo ele volta caindo      
                     y++;    
                   if(y<495 && matriz[x][y+1]==0)//caso pular segurando o pulo o personagem sobe,e se nao tiver soltado o botao pulo ele volta caindo      
                     y++;
                   if(y<495 && matriz[x][y+1]==0)//caso pular segurando o pulo o personagem sobe,e se nao tiver soltado o botao pulo ele volta caindo      
                     y++;
                   if(y<495 && matriz[x][y+1]==0)//caso pular segurando o pulo o personagem sobe,e se nao tiver soltado o botao pulo ele volta caindo      
                     y++;
                   if(y<495 && matriz[x][y+1]==0)//caso pular segurando o pulo o personagem sobe,e se nao tiver soltado o botao pulo ele volta caindo      
                     y++;
                   if(y<495 && matriz[x][y+1]==0)//caso pular segurando o pulo o personagem sobe,e se nao tiver soltado o botao pulo ele volta caindo      
                     y++;
                    if(y<495 && matriz[x][y+1]==0)//caso pular segurando o pulo o personagem sobe,e se nao tiver soltado o botao pulo ele volta caindo      
                     y++;
                   if(y<495 && matriz[x][y+1]==0)//caso pular segurando o pulo o personagem sobe,e se nao tiver soltado o botao pulo ele volta caindo      
                     y++;  
                 }                         
                 
}


void elevador(int xe,int ye,int yf,BITMAP *buffer,BITMAP *fundo2,BITMAP *fundo1,BITMAP *painel,BITMAP *elevadorc1,BITMAP *elevadorc2,int estado,int fps,int xcenario,int roupa)
{
      SAMPLE *voz2; 
      BITMAP *p;
      char posicao[20];
       
      int x=0,i=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      loop=0,//controle de escrita no buffer com o uso do while do fps
      vida=99;

      voz2 = load_wav("musicas/elevador.wav");
      //som elevador
      play_sample(voz2,100,130,1000,1);
      if(roupa==0)
        p = load_gif("personagem/a8.gif",NULL);
      else
        p = load_gif("personagem/ARMADURA/ax8.gif",NULL);  
  
     // fps=MSEC_TO_TIMER(1);
      install_int( frame_rate, 1000 );
      install_int_ex( incrementa_speed, BPS_TO_TIMER(50) );
      if(estado==1)
      {
              for(y=ye;y<yf;y++)//elevador descendo
              {     
                   
                   
                  while ( ::fps_speed > 0 )
                  {     
                        if(loop==0)
                        { 
                                clear(buffer);
                               
                               draw_sprite(buffer,fundo1,xcenario,0); //draw_sprite(buffer,fundo2,0,0);
                                
                               if(i%2==0)//ELEVADOR
                               {
                                  draw_sprite(buffer,elevadorc1,xe,y+50);
                                  i=1;
                               }   
                                else 
                                  {
                                    draw_sprite(buffer,elevadorc2,xe,y+50);
                                    i=0;
                                   }    
                                          
                              
                             //  draw_sprite(buffer,fundo1,xcenario,0);
                               if(roupa==0)
                                draw_sprite(buffer,p,xe+5,y-13);   
                               else 
                                 draw_sprite(buffer,p,xe+5,y-33);  
                               
                               draw_sprite(buffer,painel,0,0);
                               textprintf_centre_ex(buffer, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                               textprintf_centre_ex(buffer, font,130,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y);
                               textprintf_centre_ex(buffer, font,130,290, makecol(0, 0, 0), makecol(255, 255, 255),"FPS:%d",fps_antigo);
                               textprintf_centre_ex(buffer, font,400,290, makecol(0, 0, 0), makecol(255, 255, 255),"ESTADO:%d",estado);
                               loop=1;
                        }       
                        ::fps_speed--;
                        ::fps++;
                  }  
                  loop=0;  
                       draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem   
                         y=y+2;
                        vsync();
              }  
      }
      else
         if(estado==0)
         {
                  for(y=ye;y>yf;y--)//elevador subindo
                  {          
                        while ( ::fps_speed > 0 )
                        {      
                               if(loop==0)
                               {
                                        clear(buffer);
                                       
                                        draw_sprite(buffer,fundo1,xcenario,0); //draw_sprite(buffer,fundo2,0,0);
                                        
                                       if(i%2==0)//ELEVADOR
                                       {
                                          draw_sprite(buffer,elevadorc1,xe,y+50);
                                          i=1;
                                       }   
                                        else 
                                          {
                                            draw_sprite(buffer,elevadorc2,xe,y+50);
                                            i=0;
                                           }                               
                                           
                                     //  draw_sprite(buffer,fundo1,xcenario,0);
                                       if(roupa==0)
                                          draw_sprite(buffer,p,xe+5,y-13);   
                                       else 
                                         draw_sprite(buffer,p,xe+5,y-33);   
                                        
                                      // destroy_bitmap(p);   
                                       
                                       draw_sprite(buffer,painel,0,0);
                                       textprintf_centre_ex(buffer, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                                       textprintf_centre_ex(buffer, font,130,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y);
                                       textprintf_centre_ex(buffer, font,400,290, makecol(0, 0, 0), makecol(255, 255, 255),"ESTADO:%d",estado);
                                 loop=1;
                              }        
                                       
                             ::fps_speed--;
                             ::fps++;
                         }  
                         loop=0;
                         
                         draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem     
                         y=y-2;
                         vsync();
                      
                  }  
                  
          }
          stop_sample(voz2);
          destroy_sample(voz2);
          destroy_bitmap(p);
}

void elevador2(int xe,int ye,int yf,int pbaseelevador,BITMAP *buffer,BITMAP *fundo2,BITMAP *fundo1,BITMAP *painel,BITMAP *elevadorc1,BITMAP *elevadorc2,int estado,BITMAP *elevadorb1,BITMAP *elevadorb2,int fps,int xcenario,int roupa)
{
      SAMPLE *voz5; 
      BITMAP *pff;
      char posicao[20];
       
      int x=0,i=0,i2=0,//coluna q o personagem esta
      y=0,//linha q o personagem esta
      loop=0,//controle de escrita no buffer com o uso do while do fps
      vida=99;
      voz5 = load_wav("musicas/elevador.wav");
      //strcpy(posicao,"personagem/a8.gif");
      //som elevador
      play_sample(voz5,100,130,1000,1);
      if(roupa==0)
        strcpy(posicao,"personagem/a8.gif");
      else
        strcpy(posicao,"personagem/ARMADURA/ax8.gif");  
      
      
      //fps=MSEC_TO_TIMER(1);
      if(estado==1)
      {
              for(y=ye;y<yf;y++)//elevador descendo
              {    
                   
                 //  while ( ::fps_speed > 0 )
                 //  {      
                  //        if(loop==0)
                  //        {      
                                clear(buffer);
                               
                                draw_sprite(buffer,fundo2,0,0);
                                draw_sprite(buffer,fundo1,xcenario,0); //draw_sprite(buffer,fundo2,0,0);
                                        
                                if(i2%2==0)//ELEVADOR BASE
                                {
                                  draw_sprite(buffer,elevadorb1,pbaseelevador,465);
                                  i2=1;
                                }   
                                else 
                                  {
                                    draw_sprite(buffer,elevadorb2,pbaseelevador,465);
                                    i2=0;
                                   }      
                                
                                
                               if(i%2==0)//ELEVADOR
                               {
                                  draw_sprite(buffer,elevadorc1,xe,y+50);
                                  i=1;
                               }   
                                else 
                                  {
                                    draw_sprite(buffer,elevadorc2,xe,y+50);
                                    i=0;
                                   }    
                                          
                               pff = load_gif(posicao,NULL);
                             //  draw_sprite(buffer,fundo1,xcenario,64);
                               if(roupa==0)
                                draw_sprite(buffer,pff,xe+5,y-13);   
                               else 
                                 draw_sprite(buffer,pff,xe+5,y-33);   
                                
                               //destroy_bitmap(p);   
                               
                               draw_sprite(buffer,painel,0,0);
                               textprintf_centre_ex(buffer, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                               textprintf_centre_ex(buffer, font,130,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y);
                               textprintf_centre_ex(buffer, font,330,250, makecol(0, 0, 0), makecol(255, 255, 255),"ESTADO :%d",estado);
                   //            loop=1;
                   //       }                             
                   //       ::fps_speed--;
                   //       ::fps++;
                  //  }   
                   //loop=0; 
                               
                   draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem   
                   vsync();
                   y=y+3;
              }  
      }
      else
         if(estado==0)
         { 
                  for(y=ye;y>yf;y--)//elevador subindo
                  {          
                             
                         while ( ::fps_speed > 0 )
                         {      
                             if(loop==0)
                             {    
                                    clear(buffer);
                                    
                                    draw_sprite(buffer,fundo2,0,0);
                                    draw_sprite(buffer,fundo1,xcenario,0); //draw_sprite(buffer,fundo2,0,0);
                                    
                                    if(i2%2==0)//ELEVADOR BASE
                                    {
                                      draw_sprite(buffer,elevadorb1,pbaseelevador,465);
                                      i2=1;
                                    }   
                                    else 
                                      {
                                        draw_sprite(buffer,elevadorb2,pbaseelevador,465);
                                        i2=0;
                                       }    
                                    
                                   if(i%2==0)//ELEVADOR
                                   {
                                      draw_sprite(buffer,elevadorc1,xe,y+50);
                                      i=1;
                                   }   
                                    else 
                                      {
                                        draw_sprite(buffer,elevadorc2,xe,y+50);
                                        i=0;
                                       }                               
                                       
                                   pff = load_gif(posicao,NULL);
                                //   draw_sprite(buffer,fundo1,xcenario,60);
                                   if(roupa==0)
                                     draw_sprite(buffer,pff,xe+5,y-13);   
                                   else 
                                     draw_sprite(buffer,pff,xe+5,y-33);   
                                     
                                   
                                   draw_sprite(buffer,painel,0,0);
                                   textprintf_centre_ex(buffer, font,148,38, makecol(255, 255, 255), makecol(0, 0, 0),"ENERGIA %d",vida); 
                                   textprintf_centre_ex(buffer, font,130,290, makecol(0, 0, 0), makecol(255, 255, 255),"Y:%d",y);
                                   textprintf_centre_ex(buffer, font,330,250, makecol(0, 0, 0), makecol(255, 255, 255),"ESTADO :%d",estado);
                                   loop=1;
                            }                             
                            ::fps_speed--;
                            ::fps++;
                       }   
                       loop=0;            
                       draw_sprite(screen,buffer,0,0);//desenha na tela a nova imagem   
                       vsync();
                       y=y-3;
                  }  
          }
      stop_sample(voz5);
      destroy_sample(voz5);
      destroy_bitmap(pff);
}

void passaporta(int estado,int x,int y,int fps)
{
      BITMAP *bufferq,*fundoq,*portaq;  
     
      bufferq = create_bitmap(MAX_X,MAX_Y);
      fundoq = create_bitmap(MAX_X,MAX_Y);

      fundoq = load_gif("fundos/introg2.gif", NULL); 
      portaq = load_gif("objetos/BASEESPACIAL/porta2.gif", NULL);
 
      //fps=MSEC_TO_TIMER(1);
 
      if(estado==0)
      {
         for(x=0;x<710;x++)
         {
            clear(bufferq);                
            draw_sprite(bufferq,portaq,x,y);
            draw_sprite(screen,bufferq,0,0); 
            x=x+10;
            vsync();

         }             
      }
      else
         if(estado==1)
         {
             for(x=720;x>-5;x--) 
             {
                clear(bufferq);                 
                draw_sprite(bufferq,portaq,x,y);
                draw_sprite(screen,bufferq,0,0);
                x=x-10; 
                vsync();
             }     
         }  
      destroy_bitmap(fundoq);
      destroy_bitmap(portaq);
      destroy_bitmap(bufferq);
      
}

void atirar(int x,int y,int d,Tiro tirof[3],Tiro tirot[3],Tiro tiroc[3],SAMPLE *tiro)
{
	// Temos que percorrer o array até achar um tiro que não está ativo, assim:nisso ja fazemos o som de tiro ao ativar ele
	int i;
	
	if(d==2)
	{
    	for (i = 0; i < 3; i++)
    	{
    		if (tirof[i].ativo == 0)
    		{
    			tirof[i].ativo = 1;
      	         play_sample(tiro,230,130,1000,0);
    			// Setamos a coordenada que ele vai sair
    		    tirof[i].x = x + 28;     
    		   	tirof[i].y = y;
   			 
    			break;
    		}
    	
    	}
    } 	
    else
       if(d==1)
       {
        	for (i = 0; i < 3; i++)
        	{
        		if (tirot[i].ativo == 0)
        		{
        			tirot[i].ativo = 1;
        			play_sample(tiro,230,130,1000,0);
                    tirot[i].x = x - 28;
        			tirot[i].y = y;
        			
        			break;
        		}
        	
        	}
       }  	
	   else
	     if(d==3)
	     {
            	for (i = 0; i < 3; i++)
            	{
                   // posicao=posicao+10;
            		if (tiroc[i].ativo == 0)
            		{
            			tiroc[i].ativo = 1;
            			play_sample(tiro,230,130,1000,0);
                        tiroc[i].y = y;
                        tiroc[i].x=x+19;                 
            			
            			break;
            		}
            	
            	}
         }   	
	//show_tiros(x,y);
}

void show_tiros(int x,int y,BITMAP *buffer,int d,Tiro tirofv[3],Tiro tirotv[3],Tiro tirocv[3],int matriz[800][600],int roupa)
{
	// Essa função não só mostra os tiros na tela, como os atualiza.
	// Vai mudando sua posição e verifica se ele sai da tela (desativa-o).
    BITMAP *tiroc,*tirof,*tirot;
    tirof = create_bitmap(15, 8);
    if(roupa==0)//tiro frente
	 tirof = load_gif("objetos/BASEESPACIAL/tirof.gif", NULL);
    else
     tirof = load_gif("objetos/BASEESPACIAL/ticef.gif", NULL);
     
    tirot= create_bitmap(15, 8);//tiro tras
    if(roupa==0)
    	tirot = load_gif("objetos/BASEESPACIAL/tirot.gif", NULL);
    else
      tirot = load_gif("objetos/BASEESPACIAL/ticet.gif", NULL);	
	
	tiroc = create_bitmap(15, 8);//tiro frente
    if(roupa==0)	
	  tiroc = load_gif("objetos/BASEESPACIAL/tiroc.gif", NULL);
	else
	  tiroc = load_gif("objetos/BASEESPACIAL/ticec.gif", NULL);
	
	int i;
	
	//if(d==1)//tras
//	{
        	for (i = 0; i < 1; i++)
        	{
        		if (tirotv[i].ativo == 1)
        		{                     
                    //clear(buffer);  
                    //if(matriz[tirotv[i-1].x][tirotv[i].y]==0)               
        		    	draw_sprite(buffer,tirot, tirotv[i].x, tirotv[i].y+12);
        			//draw_sprite(screen,tirox, tiro[i].x, tiro[i].y);
        			
            		if (tirotv[i].x >-10)
            		{
            			tirotv[i].x -= 15;
            		}
            		else
            		  {
            	    	tirotv[i].ativo = 0;
            	    	tirotv[i].x=400;
                      } 	
                           
                            
        		}
        	//	rest(1);
        	}
  //  }
  //  else
    //  if(d==2)//frente
    //  {
            for (i = 0; i < 1; i++)
        	{
        		if (tirofv[i].ativo == 1)
        		{      
                    //if(matriz[tirofv[i+1].x][tirofv[i].y]==0)                   
                      draw_sprite(buffer,tirof, tirofv[i].x+15, tirofv[i].y+12); 
                       
        			if (tirofv[i].x < 800)
        			{
        				tirofv[i].x += 15;
        			}
        			else
            		  {
            	    	tirofv[i].ativo = 0;
            	    	tirofv[i].x=400;
                      } 
                           
        		}
        	//	rest(1);
	         }      
                
    //  }   
    //  else
     //   if(d==3)//cima
     //   {
            for (i = 0; i < 1; i++)
        	{
        		if (tirocv[i].ativo == 1)
        		{                     
                     if(roupa==1)
        		    	draw_sprite(buffer,tiroc, tirocv[i].x-4, tirocv[i].y-45);
        		     else
                       draw_sprite(buffer,tiroc, tirocv[i].x, tirocv[i].y - 28); 	
        			//draw_sprite(screen,tirox, tiro[i].x, tiro[i].y);
        			
        		//	if(d==3)//tiro para cima
        		//	{
            			if (tirocv[i].y >-10)
            			{
            				tirocv[i].y -= 15;
            			}
            			else
                		  {
                	    	tirocv[i].ativo = 0;
                	    	tirocv[i].x=400;
                          } 
                   // }     
                } 
           }
           
       destroy_bitmap(tirof);
      destroy_bitmap(tirot);
      destroy_bitmap(tiroc);    	
}

void iniciatiro(Tiro tirof[3],Tiro tirot[3],Tiro tiroc[3],int d,int &flag)
{
       int i;
        
       if(d==1)//limpa vetor tiro tras
       {  
           for(i=0;i<3;i++)//dizendo q nenhum tido esta ativo no vetor
           {
              tirot[i].ativo=0; 
              tirot[i].vel = 70;
           }
       }    
       else 
         if(d==2)//limpa vetor tiro frente
         {
                  for(i=0;i<3;i++)//dizendo q nenhum tido esta ativo no vetor
                  {               
                      tirof[i].ativo=0; 
                      tirof[i].vel = 1;
                  }            
         }
         else
           if(d==3) // limpa o vetor para cima
           {
               for(i=0;i<3;i++)//dizendo q nenhum tido esta ativo no vetor
               {                 
                  tiroc[i].ativo=0; 
                  tiroc[i].vel = 70;
               }       
                   
           }
           else
               if(d==0)//limpa vetor tiro de todos lados
               {  
                   for(i=0;i<1;i++)//dizendo q nenhum tido esta ativo no vetor
                   {
                      tirot[i].ativo=0; 
                      tirot[i].vel = 70;
                      
                      tirof[i].ativo=0; 
                      tirof[i].vel = 70;
                      
                      tiroc[i].ativo=0; 
                      tiroc[i].vel = 70;
                   }     
               }   
               flag=0;
}

int pause1(int opcao)
{
      BITMAP *bufferaa,*fundoaa;  
      DATAFILE *datFontes = NULL;
      datFontes = load_datafile("fonthistoria.dat");
      
      int x=0,i=0,fimdejogo=0;//coluna q o personagem esta
      
      bufferaa = create_bitmap(MAX_X,MAX_Y);
      fundoaa = create_bitmap(MAX_X,MAX_Y);
      
      fundoaa = load_gif("fundos/pause.gif", NULL); 
      remove_keyboard();              
      install_keyboard();
      do
      {
         clear(bufferaa);                                
         draw_sprite(bufferaa,fundoaa,0,0);    
         textprintf_centre_ex(bufferaa, font,405,25, makecol(255, 255, 255), makecol(0, 0, 0),"BASE ESPACIAL"); 
         textprintf_centre_ex(bufferaa, font,705,18, makecol(255, 255, 255), makecol(0, 0, 0),"ARMAS");
         textprintf_centre_ex(bufferaa, font,93,20, makecol(255, 255, 255), makecol(0, 0, 0),"SAIR DO JOGO");   
         textprintf_ex(bufferaa,(FONT *)datFontes[0].dat,373,550,makecol(100,0,230),-1,"%s","MAPA");                                                  
         draw_sprite(screen,bufferaa,0,0);   
         
         if(key[KEY_L])
          fimdejogo=1;
         else
          if(key[KEY_R])//segunda tela do pause
           pause2(0);  
          
      }while(!key[KEY_ENTER] && fimdejogo!=1);
          
      //stop_sample(voz);    
      destroy_bitmap(fundoaa);
      destroy_bitmap(bufferaa);

      remove_keyboard();              
      install_keyboard();
      
      if(fimdejogo==1)
       return -10000;
      
}
int pause2(int opcao)
{
      BITMAP *bufferkk,*fundoab;  
       
      int x=0,i=0;//coluna q o personagem esta
      
      bufferkk = create_bitmap(MAX_X,MAX_Y);
      fundoab = create_bitmap(MAX_X,MAX_Y);
      
      fundoab = load_gif("fundos/pause2.gif", NULL); 

      do
      {
         clear(bufferkk);                                
         draw_sprite(bufferkk,fundoab,0,0); 
         textprintf_centre_ex(bufferkk, font,125,555, makecol(255, 255, 255), makecol(0, 0, 0),"VOLTAR"); 
         draw_sprite(screen,bufferkk,0,0);
      }while(!key[KEY_L]);
          
      //stop_sample(voz);    
      destroy_bitmap(fundoab);
      destroy_bitmap(bufferkk);

      remove_keyboard();              
      install_keyboard();
      
}

void movimentopersonagem(BITMAP *bufferS,BITMAP *imagemfS,int &xs,int &ys,int inicio,int fim,int &direcao,int posicaoimagem,int qualimagem)
{

  if(qualimagem==1)
  {
       if(xs==inicio)
          direcao=2;
       else
         if(xs==fim)
          direcao=1;  
          
       if(direcao==2)
           xs++;
       else
         if(direcao==1)
          xs--; 
  }            
 /* if(tempo3==0)   
  {  
    soldadofS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadox.gif",NULL);  
    xs++;
  }  
  else*/
  if(qualimagem==1)//MOSTRAR SOLDADO
  {
        if(tempo3>3)
         tempo3=0;  
                             
        if(tempo3==0)     
        {
          if(direcao==2)                
            imagemfS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadomf2.gif",NULL);
          else
            imagemfS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadomt2.gif",NULL);  
          
        }  
        else
         if(tempo3==1)     
         {
           if(direcao==2)                
             imagemfS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadomf3.gif",NULL);
           else
             imagemfS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadomt3.gif",NULL);   
           /*if(xs<fim)
             xs++;
           else
             xs--;*/
         }  
         else
          if(tempo3==2)     
          {
            if(direcao==2)                 
              imagemfS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadomf4.gif",NULL);
            else
             imagemfS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadomt4.gif",NULL);  
           /* if(xs<fim)
              xs++;
            else
              xs--;   */
          }  
          else
           if(tempo3==3)     
           {
             if(direcao==2)                
               imagemfS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadomf5.gif",NULL);
             else
               imagemfS = load_gif("objetos/BASEESPACIAL/SOLDADO/soldadomt5.gif",NULL);  
           /*  if(xs<fim)
               xs++;
             else
              xs--;*/
           } 
           
          // else
           // if(tempo3==5)     
            //  strcpy(posicao,"objetos\BASEESPACIAL\soldadomf2.gif"); */ 
   } 
   else
     if(qualimagem==2)//MOSTRAR SOLDADO
      {
           // if(tempo3>4)
            // tempo3=0;  
                                
            if(tempo3==0)     
            {
              if(direcao==2)                
                imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao1.gif",NULL);
              else
                imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao1.gif",NULL);  
              
            }  
            else
             if(tempo3==1)     
             {
               if(direcao==2)                
                 imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao2.gif",NULL);
               else
                 imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao2.gif",NULL);   
               /*if(xs<fim)
                 xs++;
               else
                 xs--;*/
             }  
             else
              if(tempo3==2)     
              {
                if(direcao==2)                 
                  imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao3.gif",NULL);
                else
                 imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao3.gif",NULL);  
               /* if(xs<fim)
                  xs++;
                else
                  xs--;   */
              }  
              else
               if(tempo3==3)     
               {
                 if(direcao==2)                
                   imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao4.gif",NULL);
                 else
                   imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao4.gif",NULL);  
               /*  if(xs<fim)
                   xs++;
                 else
                  xs--;*/
               } 
               else
                  if(tempo3==4)     
                   {
                     if(direcao==2)                
                       imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao5.gif",NULL);
                     else
                       imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao5.gif",NULL);  
                   /*  if(xs<fim)
                       xs++;
                     else
                      xs--;*/
                   } 
                   else
                     imagemfS = load_gif("objetos/BASEESPACIAL/CAPITAO/capitao5.gif",NULL);    
                  
                  
       }   
        

      draw_sprite(bufferS,imagemfS,xs+posicaoimagem,ys);
      destroy_bitmap(imagemfS);
}

void frame_rate()
{
   ::fps_antigo = ::fps;
   ::fps = 0;
}

   
void incrementa_speed()
{
   ::fps_speed++;
}
