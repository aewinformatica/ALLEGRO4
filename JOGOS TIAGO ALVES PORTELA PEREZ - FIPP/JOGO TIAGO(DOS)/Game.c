#include<stdio.h>
#include<conio2.h>
#include <windows.h>

void MENU(int p); //PROTÓTIPO DE PROCEDIMENTO,FALA QUE A FUNÇÃO ESTA LOGO ABAIXO DO MAIN()
void GAME();
void BORDAS(int p); //PROTÓTIPO DE PROCEDIMENTO,FALA QUE A FUNÇÃO ESTA LOGO ABAIXO DO MAIN()
void INSTRUCOES();
void CREDITOS();
void FINAL();
void CENARIOINTRO();
int CENARIO1(int fase,int volta);
int CENARIO2(int fase,int volta);
int CENARIO3(int fase,int volta,int chaveA);
int CENARIO4(int fase);
int CENARIO5(int fase,int volta);
int CENARIO6(int fase,int volta);
int CENARIO7(int fase,int volta);
int CENARIO8(int fase);
void EXTRAS();
void ENTRADA_MENU();
void INTRO();                  
int TIROINIMIGO(int tecla,int x,int l,int conttiro,int qual);//ini posicao do inimigo,x posicao personagem                   
int CONTROLES(int tecla,int x,int l,int s,int nadar,int R,int qual); //FUNCAO QUE PASSA A TECLA,A POSICAO DE X E L E RETORNA A POSICAO DE X E L
                                                 // O S DIZ SE EH PARTE DA ENTRADA SE 0 EH DA ENTRADA SE 1 NAO EH
                                                 //NADAR diz se eh fase que nada ou nao 0 nao 1 sim
                                                 //R é para fase nadar diz se vai retornar L ou X,se 0 x se 1 L
                                                 //ini diz a posicao do inimigo que o inimigo esta para morrer
                                                 //qual eh o tipo de tiro q vai ser lançado no inimigo,se -1 tir
//FALTA INIMIGOS ANDAR E ATIRAR NA FASE 6
//COLOCAR PARA DEFENDER,VARIAVEL Q QUANDO APERTA ESCUDO,ELA RECEBE 1,E ENQAUNTO NAO APERTAR PARA OUTRA DIRECAO DEFENDE,APERTANDO PARA OUTRA DIRECAO RECEBE 0                                                 
                                                 
int FASEX=8,CHAVEX=0,AUXFASE3=0,ARMAX=0,LADOTIRO,VIDAS=8,ESCUDOX=0,DEFESAX=0,INIX=10,INIX2=3,VIDASINI1,VIDASINI2,MOSTROX=5,CHEFAOX=5;//VARIAVEIS GLOBAIS    
                                       
//LADOTIRO diz se atira e vai para esquerda ou direita, 0 esquerda, 1 direita
//FASEX eh para dizer para que fase vai
//CHAVEX eh se pegou a chave na fase da agua para abrir porta na 3,
//ARMAX é se pego a arma e pode usar
//VIDAS é o numero de vidas que o jogador tem
//ESCUDOX se o escudo for 1 pode usar o escudo
//DEFESAX se apertar F defesa,atribui 1,cancelando os tiros ateh a posicao do personagem e ao mover o personagem atribui 0 liberando
//INIX diz se inimigo1 morreu ou nao,se 1 sim,0 nao
//INIX2 diz se inimigo2 morreu ou nao,se 1 sim,0 nao
// VIDASINI1,VIDASINI2,MOSTROX ,CHEFAOX,são as vidas dos inimigos
//AUXFASE3,fala para fase 3 de qual fase ta voltando se for 4 eh da fase agua ,se 5 eh da fase 5

main()
{          
  ENTRADA_MENU();
  MENU(0);
  
}

void GAME()
{
  int fase=1,carteira=0,volta=0,verifica,vida;
  INTRO(); 
  clrscr();
  do 
  {  
     if(VIDAS==0)//GAME OVER
     fase=10;      
            
     clrscr();      
     if(fase==1)//FASE 1
     { 
       verifica=fase;        
       fase=CENARIO1(fase,volta);
  
       if(fase>verifica)
       {             
         volta=0;
       }  
       if(fase<verifica)
       {             
         volta=1;
       }    
       
      /* gotoxy(3,3);
       printf("Fase=%d",fase);
       gotoxy(3,4);
       printf("Verifica=%d",verifica);
       gotoxy(3,20);
       printf("Volta=%d",volta);
       getch();*/
     } 

     if(VIDAS==0)//GAME OVER
     fase=10;

     clrscr();
     if(fase==2) //FASE 2
     { 
       verifica=fase;         
       fase=CENARIO2(fase,volta);
 
       if(fase>verifica)
         volta=0;
       else
         volta=1;  
         
      /* gotoxy(3,3);
       printf("Fase=%d",fase);
       gotoxy(3,4);
       printf("Verifica=%d",verifica);
       gotoxy(3,20);
       printf("Volta=%d",volta);
       getch();*/
     } 
     
     if(VIDAS==0)//GAME OVER
     fase=10;
     
     
     clrscr();
     if(fase==3) //FASE 3
     { 
       verifica=fase;         
       fase=CENARIO3(fase,volta,CHAVEX);
       
       if(fase>verifica)
         volta=0;
       else
         volta=1;  
         
      /* gotoxy(3,3);
       printf("Fase=%d",fase);
       gotoxy(3,4);
       printf("Verifica=%d",verifica);
       gotoxy(3,20);
       printf("Volta=%d",volta);
       getch();*/
     } 
     
     if(VIDAS==0)//GAME OVER
     fase=10;
     
      clrscr();
     if(fase==4) //FASE 4
     { 
       verifica=fase;         
       fase=CENARIO4(fase);
       if(fase>verifica)
         volta=0;
       else
         volta=1;    
         
       /*gotoxy(3,3);
       printf("Fase=%d",fase);
       gotoxy(3,4);
       printf("Verifica=%d",verifica);
       gotoxy(3,20);
       printf("Volta=%d",volta);
       getch();*/
     }  
     
     if(VIDAS==0)//GAME OVER
     fase=10;
     
     
      clrscr();
     if(fase==5) //FASE 5
     { 
       verifica=fase;  
       vida=VIDAS;       
       fase=CENARIO5(fase,volta);
       if(fase>verifica || vida>VIDAS)//se morreu fala que nao eh volta
         volta=0;
       else
         volta=1;    
         
      /* gotoxy(3,3);
       printf("Fase=%d",fase);
       gotoxy(3,4);
       printf("Verifica=%d",verifica);
       gotoxy(3,20);
       printf("Volta=%d",volta);
       gotoxy(3,22);
       printf("VIDAS=%d",VIDAS);
       getch();*/
     }  
     
     if(VIDAS==0)//GAME OVER
     fase=10;
     
     
     clrscr();
     if(fase==6) //FASE 6
     { 
       verifica=fase;  
       vida=VIDAS;       
       fase=CENARIO6(fase,volta);
       if(fase>verifica || vida>VIDAS)//se morreu fala que nao eh volta
         volta=0;
       else
         volta=1;    
         
       /*gotoxy(3,3);
       printf("Fase=%d",fase);
       gotoxy(3,4);
       printf("Verifica=%d",verifica);
       gotoxy(3,20);
       printf("Volta=%d",volta);
       gotoxy(3,22);
       printf("VIDAS=%d",VIDAS);
       getch();*/
     }
     
     if(VIDAS==0)//GAME OVER
     fase=10;
     
     clrscr();
     if(fase==7) //FASE 7
     { 
       verifica=fase;  
       vida=VIDAS;       
       fase=CENARIO7(fase,volta);
       if(fase>verifica || vida>VIDAS)//se morreu fala que nao eh volta
         volta=0;
       else
         volta=1;    
         
      /* gotoxy(3,3);
       printf("Fase=%d",fase);
       gotoxy(3,4);
       printf("Verifica=%d",verifica);
       gotoxy(3,20);
       printf("Volta=%d",volta);
       gotoxy(3,22);
       printf("VIDAS=%d",VIDAS);
       getch();*/
     }
     
     if(VIDAS==0)//GAME OVER
     {
     fase=10;
     }
     
     clrscr();
     if(fase==8) //FASE 7
     {    
       fase=CENARIO8(fase);  
       /*gotoxy(3,3);
       printf("Fase=%d",fase);
       gotoxy(3,4);
       printf("Verifica=%d",verifica);
       gotoxy(3,20);
       printf("Volta=%d",volta);
       gotoxy(3,22);
       printf("VIDAS=%d",VIDAS);
       getch();*/
     }
     
  }while(fase<9); 
  if(VIDAS>0)  
  FINAL();
  if(VIDAS<=0)
  {
    BORDAS(3);
    textcolor(WHITE);
    gotoxy(10,12);
    printf("========================( GAME OVER )========================");  
    gotoxy(10,14);
    printf("=====================( TENTE NOVAMENTE )=====================");
    beep(1450,100);          
    sleep(3000);            
  }
  clrscr();
  textbackground(BLACK);
}

void MENU(int p) //PROCEDIMENTO MENU SEM TIPO E SEM PARAMENTRO - VOID = SEM TIPO
{
     int x=0,tecla,op=0,fim=0,c=0,y;   
       ///////////////////////////LETRAS NOME TIAGO//////////////////////////
     //ENTRADA ANTES DO MENU
     
     /////////////////////
     if(p==0)
     {
         ///LETRA T-1
         for(x=11;x<22;x++)
         {
         y=2;
         sleep(15);  
         gotoxy(x, y);
         textcolor(YELLOW);
         printf("%c",186);
         beep(x,10); 
         //parametro do percurso
         }
         
         ///LETRA T-2
         for(y=3;y<12;y++)
         {
         x=16;
         sleep(10);  
         gotoxy(x, y);
         beep(y,10);  
         printf("%c",186);
         //parametro do percurso
         }
         
         ///LETRA I
         for(y=3;y<12;y++)
         {
         x=25;
         sleep(15);  
         gotoxy(x, y);
         beep(y,10);  
         printf("%c",186);
         //parametro do percurso
         }
         
         ///LETRA A1
         for(y=3;y<12;y++)
         {
         x=30;
         sleep(15);  
         gotoxy(x, y);
         beep(y,10); 
         printf("%c",186);
         //parametro do percurso
         }
         
          ///LETRA A2
         for(x=30;x<38;x++)
         {
         y=3;
         sleep(15);  
         gotoxy(x, y);
         beep(x,10);  
         printf("%c",186);
         //parametro do percurso
         }
         
           ///LETRA A3
         for(x=30;x<38;x++)
         {
         y=6;
         sleep(15);  
         beep(x,10);  
         gotoxy(x,y);
         printf("%c",186);
         //parametro do percurso
         }
         
          ///LETRA A4
         for(y=3;y<12;y++)
         {
         x=38;
         sleep(15); 
         beep(x,10);   
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         }
         
         
         
          ///LETRA G1
         for(y=3;y<12;y++)
         {
         x=41;
         sleep(15);  
         beep(x,10);  
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         }
         
          ///LETRA G2
         for(x=41;x<51;x++)
         {
         y=3;
         sleep(15);
         beep(x,10);    
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         
         }
         
          ///LETRA G3
         for(x=41;x<51;x++)
         {
         y=11;
         sleep(15); 
         beep(x,10);   
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         }
         
          ///LETRA G4
         for(y=11;y>7;y--)
         {
         x=51;
         sleep(15); 
         beep(x,10);   
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         }
         
           ///LETRA G5
         for(x=51;x>42;x--)
         {
         y=7;
         sleep(15); 
         beep(x,10);   
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         }
         
         
           ///LETRA 01
         for(y=3;y<11;y++)
         {
         x=54;
         sleep(15);  
         beep(x,10);  
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         }
         
          ///LETRA 02
         for(x=54;x<61;x++)
         {
         y=3;
         sleep(15);  
         beep(x,10);  
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         }
         
          ///LETRA 03
         for(x=54;x<61;x++)
         {
         y=11;
         sleep(15);  
         beep(x,10);  
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         }
         
          ///LETRA 04
         for(y=11;y>2;y--)
         {
         x=61;
         sleep(15);  
         beep(x,10);  
         gotoxy(x, y);
         printf("%c",186);
         //parametro do percurso
         } 
     }     
         
      ///  //TITULO DO JOGO////////////////////////////////////
           p=1;
           gotoxy(46,12);
           Beep(1960,100);
           sleep(300);
           textcolor(WHITE);
           printf("PORTELA EM    ");
           Beep(1300,100);
           sleep(800);
           
           gotoxy(46,12);
           printf("SUA PRIMEIRA  ");
           Beep(2200,100); 
           sleep(800);
           
           textcolor(LIGHTMAGENTA);
           gotoxy(46,12);
           printf("AVENTURA NO JOGO  ");
           Beep(1640,100);
           sleep(800);
       ///////////////////////////////////////////////////////
       
           gotoxy(20,14);
           textcolor(WHITE);
           printf("\" A PROCURA DE SUA CARTEIRA ROUBADA  \"");
           Beep(1300,100);
           sleep(800);
           
           textcolor(RED);
           gotoxy(20,14);
           printf("\" A PROCURA DE SUA CARTEIRA ROUBADA  \"");
           Beep(1460,100);
           sleep(800);
           
           textcolor(GREEN);
           gotoxy(20,14);
           printf("\"  A PROCURA DE SUA CARTEIRA ROUBADA   \"");
           Beep(1300,100); //DO
           sleep(100);
           Beep(1460,100); //RÉ
           sleep(400);
               
      
           gotoxy(34,16);
           textcolor(WHITE);
           printf("1- NOVO JOGO");
           Beep(1300,100);
           sleep(200);
           gotoxy(34,18);
           printf("2- EXTRAS");
           Beep(1300,100);
           sleep(200);
           gotoxy(34,20);
           printf("3- INSTRUCOES");
           Beep(1300,100);
           sleep(200);
           gotoxy(34,22);
           printf("4- CREDITOS");
           Beep(1300,100);
           gotoxy(34,24);
           printf("5- SAIR");
           Beep(1300,100);
           
       //OBSERVACAO REPETI AS OPCOES DO MENU DE VERIFICACAO PARA NAO ESPERAR MUSICA PARAR    
       BORDAS(0); 
       gotoxy(4,4); //POSICAO CURSOR NA TELA
       do
       {
           //EFEITO NO MENU //MUSIQUINHA DOUBLE DRAGON CLASSICO STAGE 1
           gotoxy(20,14);
           textcolor(WHITE);
           printf("\"  A PROCURA DE SUA CARTEIRA ROUBADA  \"");
           
           Beep(900,200);
          Beep(820,400);
          Beep(900,200);
          Beep(820,400);
          
          Beep(900,300);
          Beep(900,300);
          Beep(820,200);
          
          Beep(1100,600);
          Beep(960,500);
          Sleep(100);
           
           textcolor(RED);
           gotoxy(20,14);
           printf("\"  A PROCURA DE SUA CARTEIRA ROUBADA   \""); 
           
           Beep(900,200);
          Beep(820,400);
          Beep(900,200);
          Beep(820,400);
          
          Beep(900,300);
          Beep(900,300);
          Beep(820,200);
          
          Beep(960,600);
          Beep(1100,500);
          Sleep(100);
            
           textcolor(GREEN);
           gotoxy(20,14);
           printf("\"  A PROCURA DE SUA CARTEIRA ROUBADA  \"");
           
           Beep(900,200);
          Beep(820,400);
          Beep(900,200);
          Beep(820,400);
          
          Beep(900,300);
          Beep(900,300);
          Beep(820,200);
          
          Beep(1100,600);
          Beep(960,600);
          Sleep(50);
          
          Beep(900,200);
          Beep(820,400);
          Beep(900,200);
          Beep(820,400);
          
          Beep(960,400);
          Beep(1100,300);
          Sleep(100);
          
          Beep(820,200);
          Beep(860,200);
          Beep(960,200);
          Beep(1100,200);
          Beep(1140,200);
           
           
          
         
          // FIM EFEITOS
          //OPCAO DO MENU//////////////////////////////////     
             
             
             if(kbhit())
             tecla=getch();
                   
           if(tecla==49)//TECLA 1
           {
              clrscr();       
              GAME();
              MENU(0);
           } 
          // if(tecla==50)//TECLA 2
           //OPCOES
          // if(op=4)
           //SAIR
           if(tecla==51)//TECLA 3
           {
              INSTRUCOES();
              MENU(0);
           }
           
           if(tecla==52) //CHAMAR  
            {
              CREDITOS();
              MENU(0);     
            }
             
            if(tecla==53) //SAIR DO JOGO 
            {
              int x;
              clrscr();     
              textcolor(YELLOW);
              for(x=0;x<=3;x++)
              {  
                if(x==1)
                {                                              
                  gotoxy(32,12);
                  printf("%s","SAINDO..");
                  beep(1460,100);
                  sleep(1000);
                } 
                if(x==2)
                {                                              
                  gotoxy(32,12);
                  printf("%s","SAINDO....");
                  beep(1460,100);
                  sleep(1600);
                }  
                if(x==3)
                {                                              
                  gotoxy(32,12);
                  printf("%s","SAINDO......");
                  beep(1460,100);
                  sleep(1600);
                }  
              }  
              fim=1;       
            }
            tecla=0; 
     }while(fim==0);                 

}

void BORDAS(int p)
{
      int c,c2,l,l2;
      
      switch(p)
      {
        case 0:textcolor(RED);
        break;
        
        case 1:textcolor(WHITE);
        break;
        
        case 2:textcolor(BLUE);
        break;
        
        case 3:textcolor(GREEN);
        break;
        
        case 4:textcolor(YELLOW);
        break;
        
        case 5:textcolor(MAGENTA);
        break;
      }    
     
      for(c=1,c2=1;c<81;c++,c2++)
      {
           sleep(10); 
           //CIMA
           gotoxy(c,1);
           printf("%c",219);
           
           //BAIXO
           gotoxy(c2,25); 
           printf("%c",219);
       }
       
      for(l=1,l2=1;l<25;l++,l2++)
      {
           //ESQUERDA                          
           c=1;
           c2=80;
           sleep(10); 
           gotoxy(c,l);
           printf("%c",219);
           
           //DIREITA
           gotoxy(c2,l2);
           printf("%c",219);
       }
}

void INSTRUCOES()
{
  clrscr();
  BORDAS(3);  
  gotoxy(29,3);
  textcolor(LIGHTRED);  
  printf("CONTROLES DE MOVIMENTO"); 
  beep(1300,100); 
 
 ////////////////INICIO DA CAIXA DE TEXTO////////////////////// 
   textcolor(WHITE);  
   int c,c2,l,l2;
   for(c=14,c2=14;c<65;c++,c2++)
   {
      l=5;
      l2=15;
      sleep(35); 
      //CIMA
      gotoxy(c,l);
      printf("%c",287);
       
      //BAIXO
      gotoxy(c2,l2);
      printf("%c",286);
   }
       
       
   for(l=5,l2=5;l<16;l++,l2++)
   {
     //ESQUERDA                          
      c=14;
      c2=65;
      sleep(35); 
      gotoxy(c,l);
      printf("%c",272);
       
      //DIREITA
      gotoxy(c2,l2);
      printf("%c",273);
     }
       
      //MOVIMENTOS //
      //cima
      gotoxy(25,7);
      textcolor(WHITE);
      printf("CIMA");
      gotoxy(26,8);
      textcolor(YELLOW);
      printf("%c",280);
      //baixo
      gotoxy(24,13);
      textcolor(WHITE);
      printf("BAIXO");
      gotoxy(26,12);
      textcolor(YELLOW);
      printf("%c",281);
      //tras
      gotoxy(20,10);
      textcolor(WHITE);
      printf("TRAS");
      gotoxy(24,10);
      textcolor(YELLOW);
      printf("%c",283);
      //frente
      gotoxy(29,10);
      textcolor(WHITE);
      printf("FRENTE");
      gotoxy(28,10);
      textcolor(YELLOW);
      printf("%c",282);
     
      c=43;
      while(c<54)
      {  
        c++;         
        textcolor(YELLOW);
        gotoxy(c,10); 
        printf("%c",219); 
      }
      gotoxy(44,8);
      textcolor(WHITE);
      printf("SPACE (ATIRA)");
      
      gotoxy(44,12);
      textcolor(WHITE);
      printf("F (DEFENDE)");
     ////////////////FIM DA CAIXA DE TEXTO//////////////////////   
     //INTRUCOES
     gotoxy(29,17);
     textcolor(LIGHTRED);  
     printf("CONTROLES DE MOVIMENTO"); 
     textcolor(LIGHTCYAN); 
     gotoxy(10,19);
     printf("  Tiago na sua casa da praia dormindo,quando um ladrao entrou,"); 
      beep(1300,100); 
      sleep(200);
     
     gotoxy(9,21);
     printf("  e roubou seu notebook.Quando Tiago acordou percebeu que foi"); 
      beep(1300,100); 
     sleep(200);
     gotoxy(10,23);
     printf("   roubado.Seu objetivo eh ajudalo a encontrar seu notebook."); 
      beep(1300,100); 
     sleep(200);
      gotoxy(3,24);
     //SAIR
     textcolor(YELLOW);
     printf("ESC(SAIR)"); 
      beep(1300,100); 
     do
     {
      c=getch();
     }while(c!=27);
     //fim sair
     clrscr();
}


void CREDITOS()
{
  int x=0;
    
  clrscr();
  do
  {    
     if(x>3) x=0;          
     x++;  
     BORDAS(x);   
     //FAZER EFEITO
     textcolor(WHITE); 
     gotoxy(25,10); 
     printf("PRODUZIDO POR TIAGO PORTELA!!"); 
     gotoxy(31,12); 
     printf("LINGUAGEM C PURO"); 
     gotoxy(27,15); 
     printf("      23/12/2009             "); 
     
     
     //APAGANDO DANÇAS
     textcolor(YELLOW);
     gotoxy(68,4);
     printf("%s","    ");
     gotoxy(68,6);
     printf("%s","    ");
     gotoxy(68,7);
     printf("%s","     ");
     sleep(250); 
     
     textcolor(YELLOW);
     gotoxy(15,4);
     printf("%s","    ");
     gotoxy(15,6);
     printf("%s","    ");
     gotoxy(15,7);
     printf("%s","     ");
     sleep(250); 
     ///////////////////////////////////
     
     //DANÇA DIREITA
     textcolor(WHITE);
     gotoxy(15,4);
     printf("%s"," 0");
     gotoxy(15,6);
     printf("%s","/|\\ ");
     gotoxy(15,7);
     printf("%s","/ \\ ");
     Beep(1300,100); //DO
     sleep(250); 
     
     textcolor(LIGHTRED);
     gotoxy(15,4);
     printf("%s","\\0");
     gotoxy(15,6);
     printf("%s"," |\\ ");
     gotoxy(15,7);
     printf("%s","/ \\ ");
     Beep(1460,100); //RÉ
     sleep(250); 
     
     textcolor(YELLOW);
     gotoxy(15,4);
     printf("%s","\\0/");
     gotoxy(15,6);
     printf("%s"," | ");
     gotoxy(15,7);
     printf("%s","/ \\ ");
     
     Beep(1640,100); //MI
     sleep(200); 
     Beep(1740,100); //FA
     sleep(200); 

     
      //DANÇA ESQUERDA
     textcolor(WHITE);
     gotoxy(68,4);
     printf("%s"," 0");
     gotoxy(68,6);
     printf("%s","/|\\ ");
     gotoxy(68,7);
     printf("%s","/ \\ ");
     Beep(1300,100); //DO
     sleep(250); 
     
     textcolor(LIGHTRED);
     gotoxy(68,4);
     printf("%s","\\o");
     gotoxy(68,6);
     printf("%s"," |\\ ");
     gotoxy(68,7);
     printf("%s","/ \\ ");
     Beep(1460,100); //RÉ
     sleep(250); 
     
     textcolor(YELLOW);
     gotoxy(68,4);
     printf("%s","\\0/");
     gotoxy(68,6);
     printf("%s"," | ");
     gotoxy(68,7);
     printf("%s","/ \\ ");
     Beep(1640,100); //MI
     sleep(250); 
     
     Beep(1740,100); //FA
     sleep(200); 
     Beep(1740,100); //FA
     sleep(200); 
     
   }while(x<3);
   clrscr();
}

void FINAL()
{
  int x,c;
  clrscr(); 
  BORDAS(0);
  textcolor(WHITE); 
  gotoxy(23,10); 
  printf("   O TIAGO ENCONTROU SEU NOTEBOOK...!!"); 
  gotoxy(22,12); 
  printf("  APOS DERROTAR O LADRAO E SUA GANGUE.!!"); 
  gotoxy(23,14); 
  printf("  E NADOU NA PRAIA TRANQUILAMENTE...!!"); 
  
   //COQUEIRO
      textcolor(GREEN);
      gotoxy(71,19);
      printf("     |");
      gotoxy(73,20);
      printf(" \\\\|//");
      textbackground(BLACK);
      textcolor(BROWN);
      gotoxy(71,18);
      printf("    o0o");
      gotoxy(73,21);
      printf("%c",219);
      gotoxy(74,21);
      printf("%c",219);
      gotoxy(75,21);
      printf("%c",219);
      gotoxy(76,21);
      printf("%c",219);
      gotoxy(77,21);
      printf("%c",219);
      gotoxy(78,21);
      printf("%c",219);
      gotoxy(79,21);
      printf("%c",219);
      
      //DESENHANDO PRAIA
      for(c=2;c<80;c++)
      {
       textbackground(LIGHTBLUE);   
       textcolor(LIGHTBLUE);                                 
       gotoxy(c,22); 
       printf("%c",219);   
       gotoxy(c,23); 
       printf("%c",219); 
       gotoxy(c,24); 
       printf("%c",219);             
      }        
  ///////////////////
  //NADANDO
  x=2;
  do
  {
    x++;   
    
    //NADADOR
    textbackground(LIGHTBLUE); 
    textcolor(WHITE);
    gotoxy(x-1,22);     
    printf("%s","  __0");
    gotoxy(x-1,23);
    printf("%s","     "); 
    sleep(30);     
    Beep(2460,50); //SI       
    gotoxy(x,22);
    printf("%s","\\__0");
    gotoxy(x,23);
    printf("%s","/   \\");  
    sleep(30);  
    Beep(2460,50); //SI
    gotoxy(x,22);
    printf("%s","\\___0");
    gotoxy(x,23);
    printf("%s","/  /  ");  
    sleep(30); 
  /////////////////////////////////
  //PASSARO 1
    textbackground(BLACK); 
    textcolor(WHITE);
    gotoxy(14,8);     
    printf("%s"," \\/  ");
    sleep(30);   
    gotoxy(14,8);
    printf("%s"," / \\  "); 
      
  //PASSARO 2
    textbackground(BLACK); 
    textcolor(WHITE);
    gotoxy(65,8);     
    printf("%s"," \\/   ");
    sleep(30);   
    gotoxy(65,8);
    printf("%s"," / \\  ");     
    
  }while(x<65);   
  //////////////////
  textbackground(BLACK);
}


void ENTRADA_MENU()
{ //musica solo entrada double dragon
  BORDAS(3);   
  textcolor(WHITE);
  gotoxy(31,12);
  printf(" *      2009      * ");
  
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,100);
  Beep(860,400);
  
  BORDAS(4);  
  textcolor(WHITE);
  gotoxy(30,12); 
  printf(" *  TIAGO PORTELA  *    ");
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,100);
  Beep(960,400);
  
  BORDAS(5);  
  textcolor(WHITE);
  gotoxy(31,12);
  printf(" *   APRESENTA   *    ");
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  Beep(740,100);
  Beep(860,400);
  
  system("color 51");//ROXO
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  system("color 61");//AMARELO
  Beep(740,250);
  Beep(740,250);
  Beep(740,250);
  system("color F1");//BRANCO SINTILANTE
  Beep(740,100);
  Beep(960,400);
  sleep(200);
  clrscr();
}

void INTRO()
{
  textbackground(BLACK);  
  textcolor(WHITE); 
  int x=0,c,c2,tecla=0;  
  
  BORDAS(0);
  //INICIO CENARIO
  
 CENARIOINTRO();
  
  
  //CORTAR CENA
  textcolor(YELLOW);
  gotoxy(2,21);
  printf("ESC(CORTA ENTRADA)"); 
  
  x=0;
  
  do
  { 
            
       if(kbhit())
       tecla=getch();
                    
      x++;
      
        //LEGENDA
       switch(x)
      {
        case 1:
             gotoxy(15,18);
             printf("TIAGO DORMINDO TRANQUILAMENTE EM SUA CASA DA PRAIA ");
             break;
        case 2:
             gotoxy(15,18);
             printf("                                                   ");
             gotoxy(16,18);
             printf("              QUANDO DERREPENTE                ");
             break;
        case 3:
             gotoxy(15,18);
             printf("                                                   ");
             gotoxy(20,18);
             printf("    APARECE UM LADRAO A FIM DE ROUBAR ELE      ");
             break;
      } 
      //FIM LEGENDA
             
      //APAGANDO 
      gotoxy(3,6);
      printf("%s","     ");
      gotoxy(3,5);
      printf("%s","      ");
      if(x<=5)
      {
        //PERSONAGEM DORMINGO
        textcolor(YELLOW);
        gotoxy(3,7);
        printf("%s","0");//cabeça
        textcolor(WHITE);
        gotoxy(4,7);
        printf("%s","--^  \\__");//corpo deitado
        sleep(1500);
        textcolor(YELLOW);
        gotoxy(3,6);
        printf("%s","z    ");//dormindo
        beep(1650,30);
        sleep(1500);
        beep(1650,30);
        gotoxy(3,5);
        printf("%s"," Z    ");//dormindo
        sleep(1500);
      }
       
        if(x>5)
        {
              
              textbackground(BLACK);      
             //PERSONAGEM DORMINGO
            textcolor(YELLOW);
            gotoxy(3,7);
            printf("%s","0");//cabeça
            textcolor(WHITE);
            gotoxy(4,7);
            printf("%s","--^  ");//corpo deitado
            sleep(1500);
            textcolor(YELLOW);
              gotoxy(3,6);
              printf("%s","z    ");//dormindo
              beep(1650,30);
              sleep(1500);
              beep(1650,30);
              gotoxy(3,5);
              printf("%s"," Z    ");//dormindo
              sleep(1500);
        }
      
      //INIMIGO
      if(x==3)
      {
         textcolor(LIGHTBLUE);
         gotoxy(69,4);
         printf("%c",1);//cabeça
         textcolor(WHITE);
         gotoxy(68,5);
         printf("%s","/|\\ ");
         gotoxy(68,6);
         printf("%s"," | ");
         gotoxy(68,7);
         printf("%s","/ \\ ");
         Beep(1300,100); //DO
         sleep(250);       
      }
       
      if(x==4)//INIMIGO OLHANDO CENA 1
      {   
          clrscr();
          gotoxy(15,3);
          printf("    O LADRAO OLHA QUE ELE ESTA DORMINDO       ");
          sleep(1300);
          gotoxy(15,5);
          printf("    E DIZ:                           ");
          //BORDAS DA IMAGEM  
          textcolor(RED);  
          for(c=1,c2=1;c<81;c++,c2++)
          {
               sleep(20); 
               //CIMA
               gotoxy(c,8);
               printf("%c",219);
               
               //BAIXO
               gotoxy(c2,23); 
               printf("%c",219);
           }
           textcolor(BLUE);
           /////////////////////  
            gotoxy(30,10);
            printf("%s","   ----------------- ");     
            gotoxy(30,11);
            printf("%s"," /                    \\"); 
            gotoxy(30,12);
            printf("%s","|   ---         ---    |"); 
            gotoxy(30,13);
            printf("%s","| <----->      <-----> |");  
            gotoxy(30,14);
            printf("%s","|          -           |"); 
            gotoxy(30,15);
            printf("%s","  \\                  /");     
            gotoxy(30,16);
            printf("%s","   |    --------     |");  
            gotoxy(30,17);
            printf("%s","   |       --        |");  
            gotoxy(30,18);
            printf("%s","   \\_______________/");
            gotoxy(30,19);
            printf("%s","     |             |");
            sleep(2500);      
            Beep(1640,100); //MI
            
            //legenda
            textcolor(WHITE); 
            beep(1650,100);
            gotoxy(50,19);
            printf("%s"," \\ AQUELE NOTEBOOK VOU ROUBAR"); 
            textcolor(BLUE);        
            
            //CENINHA INIMIGO      
            gotoxy(30,10);
            printf("%s","   ----------------- ");     
            gotoxy(30,11);
            printf("%s"," /                    \\"); 
            textcolor(WHITE);
            gotoxy(31,12);
            printf("%s","    ---         ---   "); 
            gotoxy(31,13); 
            printf("%s","   | @ |       | @ |  ");  
            gotoxy(31,14);
            printf("%s","    \\ /         \\ / "); 
            textcolor(BLUE);   
            gotoxy(30,15);
            printf("%s"," \\          -         /"); 
            gotoxy(30,16);
            printf("%s","   |   vvvvvvvvvv     |");  
            gotoxy(30,17);
            printf("%s","   |                  |");  
            gotoxy(30,18);
            printf("%s","   |    ^^^^^^^^     |");  
            gotoxy(30,19);
            printf("%s","   \\_______________/");
            gotoxy(30,20);
            printf("%s","     |             |");
            sleep(4000); 
            clrscr();
            
              
              textbackground(BLACK);
              textcolor(GREEN);
               //DESENHANDO CAMINHO
              for(c=2;c<80;c++)
              {                                 
               gotoxy(c,8); 
               printf("%c",219);   
               gotoxy(c,8); 
               printf("%c",219); 
               gotoxy(c,8); 
               printf("%c",219);             
              }
           
            textbackground(BLACK);
            //APARECENDO INIMIGO NOVAMENTE
            textcolor(LIGHTBLUE);
            gotoxy(69,4);
            printf("%c",1);//cabeça
            textcolor(WHITE);
            gotoxy(68,5);
            printf("%s","/|\\ ");
            gotoxy(68,6);
            printf("%s"," |   ");
            gotoxy(68,7);
            printf("%s","/ \\ ");
            Beep(1300,100); //DO
            sleep(250); 
            CENARIOINTRO();
            BORDAS(0);
      } 

      
      if(x==5) // INIMIGO CORRENDO A DIRECAO DE TIAGO
      {
        int o; 
        
         //PERSONAGEM DORMINGO
            textcolor(YELLOW);
            gotoxy(3,7);
            printf("%s","0");//cabeça
            textcolor(WHITE);
            gotoxy(4,7);
            printf("%s","--^  ");//corpo deitado
            sleep(1500);
            textcolor(YELLOW);
              gotoxy(3,6);
              printf("%s","z    ");//dormindo
              beep(1650,30);
              sleep(1500);
              beep(1650,30);
              gotoxy(3,5);
              printf("%s"," Z    ");//dormindo
              sleep(1500);
              CENARIOINTRO();
              
        gotoxy(18,14);
        printf("    O LADRAO CORRE ATEH O NOTEBOOK PARA ROUBAR       ");
        o=69;//posicao inicial do inimigo
        do       //loop usando funcao controle sem teclas
        {
                 
          CONTROLES(97,o,7,0,0,0,0);
          o--;//decrementa para correr para traz na funcao acima
        }while(o>10);          
      
        //INIMIGO ROUBANDO CENA 2
        
            gotoxy(10,14);
            printf("    O LADRAO PEGA O NOTEBOOK E SAI CORRENDO RAPIDAMENTE  ");
            o=8;//posicao inicial do inimigo
            do       //loop usando funcao controle sem teclas
            {
              
              tecla=100;       
              CONTROLES(tecla,o,7,0,0,0,0);
              o++;//decrementa para correr para traz na funcao acima
            }while(o<76); 
            
            //APAGANDO COISAS
            gotoxy(75,4); 
            printf("%s","    ");
            gotoxy(75,5);
            printf("%s","    ");
            gotoxy(75,6);
            printf("%s","    ");
            gotoxy(75,7); 
            printf("%s","    ");  
            
            gotoxy(10,14);
            printf("                                                           "); 
            gotoxy(2,21);//apagando legenda esc corta
            printf("                   ");   
      }      
  }while(x<7 && tecla!=27); 
    
  clrscr();  
  BORDAS(0);
  textcolor(WHITE);
  beep(1300,50);
  
  textcolor(YELLOW);//personagem
  gotoxy(35,14);
  printf("%c",2);//cabeça
  textcolor(WHITE);
  gotoxy(34,15);
  printf("%s","/|\\ ");
  gotoxy(34,16);
  printf("%s"," | ");
  gotoxy(34,17);
  printf("%s","/ \\ ");
  sleep(300);
  
  gotoxy(38,16); //mostrando numero de vidas
  printf("VIDAS x%d",VIDAS);
  
  gotoxy(18,12);
  printf(" TIAGO AGORA PRECISA ENCONTRAR SEU NOTEBOOK ");
  beep(1300,50);
  sleep(3500);
  gotoxy(18,12);
  printf("     SENAO NAO VAI PODER IR EMBORA EM PAZ   ");
  sleep(3500);
  clrscr();
}       

int CONTROLES(int tecla,int x,int l,int s,int nadar,int R,int qual)
{
    int tiro;
    
    if(tecla==100)//PARA FRENTE
    {
        beep(x,50);                 
        if(nadar==0)
        {                 
            x++;  

            gotoxy(x-1,l-3); 
            printf("%s","    ");
            gotoxy(x-1,l-2);
            printf("%s","    ");
            gotoxy(x-1,l-1);
            printf("%s","    ");
            gotoxy(x-1,l); 
            printf("%s","    ");           
            
            if(s==0)//cor da cabeça
              textcolor(BLUE); //inimigo
            else
              textcolor(YELLOW);//personagem
            gotoxy(x+1,l-3);
            printf("%c",2);//cabeça
            textcolor(WHITE);
            gotoxy(x,l-2);
            printf("%s","<|\\ ");
            gotoxy(x,l-1);
            printf("%s"," | ");
            gotoxy(x,l);
            printf("%s","> \\");
            sleep(100);
            
          
            gotoxy(x,l-3); 
            printf("%s","    ");
            gotoxy(x,l-2);
            printf("%s","    ");
            gotoxy(x,l-1);
            printf("%s","    ");
            gotoxy(x,l); 
            printf("%s","    ");     
                
            if(s==0)//cor da cabeça
              textcolor(BLUE); //inimigo
            else
              textcolor(YELLOW);//personagem
            gotoxy(x+1,l-3);
            printf("%c",2);//cabeça
            textcolor(WHITE);
            gotoxy(x,l-2);
            printf("%s","/|> ");
            gotoxy(x,l-1);
            printf("%s"," | ");
            gotoxy(x,l);
            printf("%s","/ >");
          } 
          else //FASE NADAR FRENTE
            {
                x++;      
               //NADADOR
                textbackground(LIGHTBLUE); 
                textcolor(WHITE);
                gotoxy(x-1,l);     
                printf("%s","     ");
                gotoxy(x-1,l+1);
                printf("%s","     "); 
                sleep(30);     
                Beep(2460,50); //SI       
                gotoxy(x,l);
                printf("%s","\\__0");
                gotoxy(x,l+1);
                printf("%s","/   \\");  
                sleep(30);  
                Beep(2460,50); //SI
                gotoxy(x,l);
                printf("%s"," ___0");
                gotoxy(x,l+1);
                printf("%s","/  / ");  
                sleep(30);           
            }     
    }
       //ANDAR PARA TRAZ  
       if(tecla==97 && x>2)
       {
             beep(x,50);        
            if(nadar==0)
            {             
                textcolor(WHITE);  
                if(s==1);     
                x--;               
                    
                gotoxy(x+1,l-3); 
                printf("%s","    ");
                gotoxy(x+1,l-2);
                printf("%s","     ");
                gotoxy(x+1,l-1);
                printf("%s","     ");
                gotoxy(x+1,l); 
                printf("%s","     ");  
                
                if(s==0)//cor da cabeça
                  textcolor(BLUE); //inimigo
                else
                  textcolor(YELLOW);//personagem
                gotoxy(x+1,l-3);
                printf("%c",2);//cabeça
                textcolor(WHITE);
                gotoxy(x,l-2);
                printf("%s","<|\\ ");
                gotoxy(x,l-1);
                printf("%s"," | ");
                gotoxy(x,l);
                printf("%s","/ <");
                sleep(100);
                 
               
                gotoxy(x+1,l-3); 
                printf("%s","    ");
                gotoxy(x+1,l-2);
                printf("%s","    ");
                gotoxy(x+1,l-1);
                printf("%s","    ");
                gotoxy(x+1,l); 
                printf("%s","    "); 
                            
                if(s==0)//cor da cabeça
                  textcolor(BLUE); //inimigo
                else
                  textcolor(YELLOW);//personagem
                gotoxy(x+1,l-3);
                printf("%c",2);//cabeça
                textcolor(WHITE);
                gotoxy(x,l-2);
                printf("%s","/|> ");
                gotoxy(x,l-1);
                printf("%s"," | ");
                gotoxy(x,l);
                printf("%s","< \\");
            }
            else //FASE NADAR PARA TRAS
            { 
                x--;      
               //NADADOR
                textbackground(LIGHTBLUE); 
                textcolor(WHITE);
                gotoxy(x+1,l);     
                printf("%s","     ");
                gotoxy(x+1,l+1);
                printf("%s","     "); 
                sleep(30);     
                Beep(2460,50); //SI       
                gotoxy(x,l);
                printf("%s","0___/");
                gotoxy(x,l+1);
                printf("%s","/   ");  
                sleep(30);  
                Beep(2460,50); //SI
                gotoxy(x,l);
                printf("%s","0___");
                gotoxy(x,l+1);
                printf("%s"," \\  \\");  
                sleep(30);           
            } 
       }
       
       //PARA BAIXO - ABAIXANDO////
       if(tecla==115)
       {
             beep(x,50);         
            if(nadar==0)
            {              
                textcolor(WHITE);       
                            
                gotoxy(x,l-3); 
                printf("%s","    ");
                gotoxy(x,l-2);
                printf("%s","    ");
                gotoxy(x,l-1);
                printf("%s","    ");
                gotoxy(x,l); 
                printf("%s","     ");  
                          
                textcolor(YELLOW);
                gotoxy(x+1,l-2);
                printf("%c",2);//cabeça
                textcolor(WHITE);
                gotoxy(x,l-1);
                printf("%s","/|\\ ");
                gotoxy(x,l);
                printf("%s","< >");
                sleep(100);
                beep(x,50); 
           
           }
           else //FASE NADAR PARA BAIXO
            { 
                l++;      
               //NADADOR
                textbackground(LIGHTBLUE); 
                textcolor(WHITE);
                gotoxy(x,l-1);     
                printf("%s","     ");
                gotoxy(x,l);
                printf("%s","     "); 
                sleep(30);     
                Beep(2460,50); //SI       
                gotoxy(x,l);
                printf("%s","\\___0");
                gotoxy(x,l+1);
                printf("%s","/   \\");  
                sleep(30);  
                Beep(2460,50); //SI
                gotoxy(x,l);
                printf("%s"," ___0");
                gotoxy(x,l+1);
                printf("%s","/  / ");  
                sleep(30);           
            } 

       }
         
       
       //PARA CIMA////
       if(tecla==119 )
       {
             beep(x,50);         
            if(nadar==0)
            {         
                
                textcolor(WHITE);       
                            
                gotoxy(x,l-3);
                printf("%s","      ");
                gotoxy(x,l-2);
                printf("%s","      ");
                gotoxy(x,l-1);
                printf("%s","      "); 
                gotoxy(x,l);
                printf("%s","      "); 
                          
                textcolor(YELLOW);
                gotoxy(x+1,l-3);
                printf("%c",2);//cabeça
                textcolor(WHITE);
                gotoxy(x,l-2);
                printf("%s","/|\\ ");
                gotoxy(x,l-1);
                printf("%s"," | ");
                gotoxy(x,l);
                printf("%s","/ \\ ");
                sleep(100);
                beep(x,50); 
            }
            else //FASE NADAR PARA CIMA
            { 
                l--;      
               //NADADOR
                textbackground(LIGHTBLUE); 
                textcolor(WHITE);
                gotoxy(x,l+2);     
                printf("%s","     ");
                gotoxy(x,l+1);
                printf("%s","     "); 
                sleep(30);     
                Beep(2460,50); //SI       
                gotoxy(x,l);
                printf("%s","\\__0");
                gotoxy(x,l+1);
                printf("%s","/   \\");  
                sleep(30);  
                Beep(2460,50); //SI
                gotoxy(x,l);
                printf("%s"," ___0");
                gotoxy(x,l+1);
                printf("%s","/  / ");  
                sleep(30);           
            } 
       }
       
       //TIRO PARA DIREITA
       if(tecla==32 && LADOTIRO==1 )
       {      
            gotoxy(x,l-3);
            printf("%s","      ");
            gotoxy(x,l-2);
            printf("%s","      ");
            gotoxy(x,l-1);
            printf("%s","      "); 
            gotoxy(x,l);
            printf("%s","      "); 
           
            textcolor(YELLOW);
            gotoxy(x+1,l-3);
            printf("%c",2);//cabeça
            textcolor(WHITE);
            gotoxy(x,l-2);
            printf("%s"," |== ");
            gotoxy(x,l-1);
            printf("%s"," | ");
            gotoxy(x,l);
            printf("%s","/ \\ ");
            sleep(100);      
            tiro=x+4;
            
            if(qual==4)//chefe final canhao
           { 
               //PERSONAGEM COM ARMA NOVA
                 textcolor(YELLOW);
                 gotoxy(x+1,l-3);
                 printf("%c",2);//cabeça
                 textcolor(WHITE);
                 gotoxy(x,l-2);
                 printf("%s"," OOOO(*");
                 gotoxy(x,l-1);
                 printf("%s"," | ");
                 gotoxy(x,l);
                 printf("%s","/ \\"); 
                 sleep(2000);        
           }   

         do
         {   
            tiro++;      
            
            if(qual==2 || qual==3)
            {        
              gotoxy(tiro,l-2);
              printf("-");
              sleep(20); 
              gotoxy(tiro,l-2);
              printf("  ");
              sleep(10); 
            }
            if(tiro%2==0 && qual==4)
            {        
              gotoxy(tiro,l-2);
              textcolor(RED);
              printf("%c",219);
              sleep(20); 
              gotoxy(tiro,l-2);
              printf("  ");
              sleep(10); 
            }
            if(qual==4 && tiro==15 && l==20 )//matando chefao
              CHEFAOX--;
          if(qual==2)
          {  
            if(tiro==64 && l==20 && INIX2==0)//matando inimigo 1 fase 6
               VIDASINI2--;
               
                if(VIDASINI2==0)
                {          
                      gotoxy(64,17);
                      printf("%s","      ");
                      gotoxy(64,18);
                      printf("%s","      ");
                      gotoxy(64,19);
                      printf("%s","      "); 
                      gotoxy(64,20);
                      printf("%s","      "); 
                              
                      //INIMIGO MORTO
                      textcolor(RED);
                      gotoxy(64+4,20);
                      printf("%s","0");//cabeça
                      textcolor(WHITE);
                      gotoxy(64,20);
                      printf("%s","^-- ");//corpo deitado   
                      
                      gotoxy(64,18);//limpando legenda
                      printf("                    ");      
                      
                      if(VIDASINI2==0)
                       INIX2=1;        
                      //LIMPANDO LEGENDA ALERTA AO MATAR INIMIGO
                     gotoxy(15,4);
                     printf("                           ");
                }    
            } 
            if(qual==3) //acertando mostro fase 7
            {
              if(tiro==63)
              MOSTROX--;
            }           
         }while(tiro<70);            
       
         //voltando ao normal apos tiro
         gotoxy(x,l-3);
         printf("%s","      ");
         gotoxy(x,l-2);
         printf("%s","      ");
         gotoxy(x,l-1);
         printf("%s","      "); 
         gotoxy(x,l);
         printf("%s","      "); 
                  
         textcolor(YELLOW);
         gotoxy(x+1,l-3);
         printf("%c",2);//cabeça
         textcolor(WHITE);
         gotoxy(x,l-2);
         printf("%s","/|\\ ");
         gotoxy(x,l-1);
         printf("%s"," | ");
         gotoxy(x,l);
         printf("%s","/ \\ ");
       }
       
       //LADOS QUE ATIRA(TIRO)
         //TIRO PARA ESQUERDA
       if(tecla==32  && LADOTIRO==0)
       {      
            gotoxy(x,l-3);
            printf("%s","      ");
            gotoxy(x,l-2);
            printf("%s","      ");
            gotoxy(x,l-1);
            printf("%s","      "); 
            gotoxy(x,l);
            printf("%s","      "); 
           
            textcolor(YELLOW);
            gotoxy(x+2,l-3);
            printf("%c",2);//cabeça
            textcolor(WHITE);
            gotoxy(x,l-2);
            printf("%s","==| ");
            gotoxy(x,l-1);
            printf("%s","  | ");
            gotoxy(x,l);
            printf("%s"," / \\ ");
            sleep(100);      
            tiro=x-4;
           if(qual==4)//chefe final canhao
           { 
               //PERSONAGEM COM ARMA NOVA
                 textcolor(YELLOW);
                 gotoxy(x+2,l-3);
                 printf("%c",2);//cabeça
                 textcolor(WHITE);
                 gotoxy(x-4,l-2);
                 printf("%s","*)OOOO ");
                 gotoxy(x,l-1);
                 printf("%s","  | ");
                 gotoxy(x,l);
                 printf("%s"," / \\"); 
                 sleep(2000);        
           }      
         do 
         {   
            tiro--;  
            if(qual==1 || qual==3)
            {        
              gotoxy(tiro,l-2);
              printf("-");
              sleep(20); 
              gotoxy(tiro,l-2);
              printf(" ");
              sleep(10); 
            }
            if(tiro%2==0 && qual==4)
            {        
              gotoxy(tiro,l-2);
              textcolor(RED);
              printf("%c",219);
              sleep(20); 
              gotoxy(tiro,l-2);
              printf("  ");
              sleep(10); 
            }
            if(qual==4 && tiro==15 && l==20)//matando chefao
              CHEFAOX--;
          if(qual==1)
          {  
            if(tiro==15 && l==6 && INIX==0)//matando inimigo 1 fase 6
              VIDASINI1--;
              
              if(VIDASINI1==0)
              {          
                  gotoxy(15,3);
                  printf("%s","      ");
                  gotoxy(15,4);
                  printf("%s","      ");
                  gotoxy(15,5);
                  printf("%s","      "); 
                  gotoxy(15,6);
                  printf("%s","      "); 
                          
                  //PERSONAGEM DORMINGO
                  textcolor(RED);
                  gotoxy(14-1,6);
                  printf("%s","0");//cabeça
                  textcolor(WHITE);
                  gotoxy(15,6);
                  printf("%s","--^ ");//corpo deitado         
                  
                  if(VIDASINI1==0)
                   INIX=1;        
                  //LIMPANDO LEGENDA ALERTA AO MATAR INIMIGO
                 gotoxy(15,4);
                 printf("                           ");
              }   
          }  
               
         }while(tiro>6);      
         
         //voltando ao normal apos tiro
         gotoxy(x+2,l-3);
         printf("%s","      ");
         gotoxy(x-4,l-2);
         printf("%s","      ");
         gotoxy(x,l-1);
         printf("%s","      "); 
         gotoxy(x,l);
         printf("%s","      "); 
       
                  
         textcolor(YELLOW);
         gotoxy(x+1,l-3);
         printf("%c",2);//cabeça
         textcolor(WHITE);
         gotoxy(x,l-2);
         printf("%s","/|\\ ");
         gotoxy(x,l-1);
         printf("%s"," | ");
         gotoxy(x,l);
         printf("%s","/ \\ ");      
       }
       ///FIM TIROS
       
        //ESCUDO  
        if(tecla==102 && ESCUDOX==1)
        {
           //DIREITA         
           if(LADOTIRO==1)
           {      
                //LIMPANDO AO MOSTRAR
                gotoxy(x,l-3);
                printf("%s","  ");
                gotoxy(x-1,l-2);
                printf("%s","   ");
                gotoxy(x+1,l-1);
                printf("%s","   ");
                gotoxy(x+1,l);
                printf("%s","    ");   
                       
                textcolor(YELLOW);
                gotoxy(x+1,l-3);
                printf("%c",2);//cabeça
                textcolor(WHITE);
                gotoxy(x,l-2);
                printf("%s"," |=I ");
                gotoxy(x,l-1);
                printf("%s"," | I");
                gotoxy(x,l);
                printf("%s","/ \\I ");
                sleep(300);  
                 
           } 
           //ESQUERDA
           if(LADOTIRO==0)
           {    
                //LIMPANDO AO MOSTRAR
                gotoxy(x,l-3);
                printf("%s","  ");
                gotoxy(x+1,l-2);
                printf("%s","   ");
                gotoxy(x+1,l-1);
                printf("%s","   ");
                gotoxy(x+1,l);
                printf("%s","     ");        
                
                textcolor(YELLOW);
                gotoxy(x+2,l-3);
                printf("%c",2);//cabeça
                textcolor(WHITE);
                gotoxy(x,l-2);
                printf("%s","I=|");
                gotoxy(x,l-1);
                printf("%s","I |");
                gotoxy(x,l);
                printf("%s","I/ \\");
                sleep(300);   
                
           }          
        } 
          
       if(R==0)
        return x;
       if(R==1)
        return l; 
}

int CENARIO1(int fase,int volta)
{  
  int x=2,fim=0,l=0,tecla,s;
  
  BORDAS(0);
  
  //COQUEIRO
  textcolor(GREEN);
  gotoxy(71,20);
  printf("     |");
  gotoxy(73,21);
  printf(" \\\\|//");
  textbackground(BLACK);
  textcolor(BROWN);
  gotoxy(71,19);
  printf("    o0o");
  
  //DESENHANDO PRAIA
  for(x=2;x<80;x++)
  {
   textbackground(LIGHTBLUE);   
   textcolor(LIGHTBLUE);                                 
   gotoxy(x,22); 
   printf("%c",219);   
   gotoxy(x,23); 
   printf("%c",219); 
   gotoxy(x,24); 
   printf("%c",219);             
  }
  
  textbackground(BLACK);
  textcolor(GREEN);
   //DESENHANDO CAMINHO
  for(x=2;x<80;x++)
  {                                 
   gotoxy(x,8); 
   printf("%c",219);   
   gotoxy(x,8); 
   printf("%c",219); 
   gotoxy(x,8); 
   printf("%c",219);             
  }
   
  //AREIA
  textbackground(BROWN);
  gotoxy(72,22);
  printf("    ");
  gotoxy(76,22);
  printf("    ");
  
  if(volta==1)
   x=69;
  if(volta==0)
    x=2;
  
      textbackground(BLACK);
      textcolor(YELLOW);
      gotoxy(+3,2);
      printf("%s","TIAGO");
      gotoxy(x+1,4);
      printf("%c",2);//cabeça
      textcolor(WHITE);
      gotoxy(x,5);
      printf("%s","/|\\ ");
      gotoxy(x,6);
      printf("%s"," | ");
      gotoxy(x,7);
      printf("%s","/ \\ ");
      sleep(130); 
      
  
  BORDAS(0);
  
    
  textbackground(BLACK);
  do 
  {                
      //MOVIMENTOS 
      textcolor(WHITE);      
      s=1;//indica que eh por controle por parametro
      tecla=getch(); //PEGA O MOVIMENTO (ESQUERDA,DIREITA,CIMA OU BAIXO)
      x=CONTROLES(tecla,x,7,s,0,0,0); //MANDA PARA FUNCAO A TECLA APERTADA PARA VERIFICAR E O VALOR DE X E A FUNCAO RETORNA X
       //////////////////////////////////////////
       
       if(x==73)//FIM PERCURSO
       {
       fim=1;
       fase++;
       }
        
  }while(fim==0);
  return fase;
}


int CENARIO2(int fase,int volta)
{
  int x,l,tecla,p=0,elevador=0,chave1=0,chave2=0,fim=0,desvio=4;
  
  int M[25][80];
  //////////////////////PREENCHENDO PARTES LIVRE MATRIZ ESQUELETO////////////
  for(x=1;x<80;x++)
  {
   for(l=1;l<25;l++)
    M[l][x]=0;
  }  
  //COLOCANDO BARREIRAS LATERAIS ESQUELETO NA MATRIZ
  for(x=1;x<80;x++)
  {
    l=1;
    M[l][x]=1;
    M[24][x]=1;
  }  
  for(l=1;l<25;l++)
  {
    M[l][1]=1;
    M[l][79]=1;
  }
  //DESENHANDO CENARIO ESQUELETO NA MATRIZ
  for(x=1;x<79;x++)
  {
    M[8][x]=1;//caminhos
    M[16][x]=1;//caminhos
  }   
  
  if(volta==0)
  {
      //PORTAS
      M[6][36]=1;//1
      M[7][36]=1;//1
      M[6][56]=1;//2
      M[7][56]=1;//2
  }   
  
  //BLOQUEIO PARTE DE BAIXO PARA PASSAR CENARIO SOH POR CIMA
  M[13][75]=1;//1
  M[14][75]=1;//1
  M[15][75]=1;//2
  //////////////////////FIM MATRIZ ESQUELETO//////////// 
  
  //BORDAS ATRAVES DA FUNCAO 
  BORDAS(0);  
  
  if(volta==0)
  {
      textcolor(YELLOW);//personagem
      gotoxy(39,14);
      printf("%c",2);//cabeça
      textcolor(WHITE);
      gotoxy(38,15);
      printf("%s","/|\\ ");
      gotoxy(38,16);
      printf("%s"," | ");
      gotoxy(38,17);
      printf("%s","/ \\ ");
      sleep(300);  
      
      //LEGENDA
      textcolor(WHITE);
      gotoxy(18,12);
      printf(" TIAGO AINDA CONTINUA A PROCURAR SUA CARTEIRA "); 
      gotoxy(18,12);
      printf("  ENCONTRE AS CHAVES PARA LIBERAR AS PORTAS   "); 
      sleep(3000); 
  }   
 
  ////////////DESENHANDO CENARIO//////////////////
  
  clrscr();
  BORDAS(0);
  
   
  //Caminho
  for(x=2;x<79;x++)
  {
    textcolor(BROWN);               
    gotoxy(x,8);
    printf("%c",219);  
    gotoxy(x,16);
    printf("%c",219);              
  }
  
   //DESENHANDO PRAIA
  for(x=2;x<80;x++)
  {
   textbackground(LIGHTBLUE);   
   textcolor(LIGHTBLUE);                                 
   gotoxy(x,22); 
   printf("%c",219);   
   gotoxy(x,23); 
   printf("%c",219); 
   gotoxy(x,24); 
   printf("%c",219);             
  }
  textbackground(BLACK);  
  
  if(volta==0)
  {
              
      //CHAVE 1
      textcolor(YELLOW);
      gotoxy(3,14); 
      printf("%c",268); 
      
      //CHAVE 2
      textcolor(YELLOW);
      gotoxy(75,14); 
      printf("%c",268);
      
      textcolor(WHITE);    
      //PORTA 1
      gotoxy(40,4);
      printf("%c",185);//porta 
      gotoxy(40,5);
      printf("%c",185);//porta 
      gotoxy(40,6);
      printf("%c",185);//porta 
      gotoxy(40,7);
      printf("%c",185);//porta 
      //PORTA 2
      gotoxy(60,4);
      printf("%c",185);//porta 
      gotoxy(60,5);
      printf("%c",185);//porta 
      gotoxy(60,6);
      printf("%c",185);//porta 
      gotoxy(60,7);
      printf("%c",185);//porta
 }    
   
  textcolor(YELLOW);
  //ELEVADOR 1
  gotoxy(21,3);
  printf("%c",219);
  gotoxy(22,3);
  printf("%c",219);
  gotoxy(20,4);
  printf("%c",219);
  gotoxy(20,5);
  printf("%c",219);
  gotoxy(20,6);
  printf("%c",219);
  gotoxy(20,7);
  printf("%c",219);
  
  gotoxy(23,3);
  printf("%c",219);
  gotoxy(24,4);
  printf("%c",219);
  gotoxy(24,5);
  printf("%c",219);
  gotoxy(24,6);
  printf("%c",219);
  gotoxy(24,7);
  printf("%c",219);
  
   //ELEVADOR 2
  gotoxy(61,11);
  printf("%c",219);
  gotoxy(62,11);
  printf("%c",219);
  gotoxy(60,12);
  printf("%c",219);
  gotoxy(60,13);
  printf("%c",219);
  gotoxy(60,14);
  printf("%c",219);
  gotoxy(60,15);
  printf("%c",219);
  
  gotoxy(63,11);
  printf("%c",219);
  gotoxy(64,12);
  printf("%c",219);
  gotoxy(64,13);
  printf("%c",219);
  gotoxy(64,14);
  printf("%c",219);
  gotoxy(64,15);
  printf("%c",219);
  
  //////////////////////////FIM GRAFICO CENARIO////////////// */
  
  if(volta==0)
  {
   x=3;
   l=7;
  }
  if(volta==1)
  {
   x=70;
   l=7;
  }
  
  textcolor(YELLOW);//personagem
  gotoxy(x+1,4);
  printf("%c",2);//cabeça
  textcolor(WHITE);
  gotoxy(x,5);
  printf("%s","/|\\ ");
  gotoxy(x,6);
  printf("%s"," | ");
  gotoxy(x,7);
  printf("%s","/ \\ ");
  sleep(300);
  
  
  do
  {
       tecla=0;      
       if(kbhit())  
       tecla=getch();
       //////////////////
       
       //CIMA ELEVADOR 1
       if(tecla==119 && x==21 && l==7 ) 
       {   
          beep(1200,100);  
          l=15;
          x=61;
          x=CONTROLES(tecla,x,l,1,0,0,0);
        //ELEVADOR 1
          gotoxy(21,3);
          printf("%c",219);
          gotoxy(22,3);
          printf("%c",219);
          gotoxy(20,4);
          printf("%c",219);
          gotoxy(20,5);
          printf("%c",219);
          gotoxy(20,6);
          printf("%c",219);
          gotoxy(20,7);
          printf("%c",219);
          
          gotoxy(23,3);
          printf("%c",219);
          gotoxy(24,4);
          printf("%c",219);
          gotoxy(24,5);
          printf("%c",219);
          gotoxy(24,6);
          printf("%c",219);
          gotoxy(24,7);
          printf("%c",219);
          
          textcolor(BLACK);
          //LIMPANDO PERSONAGEM NO ELEVADOR
          gotoxy(21,4);
          printf("   ");
          gotoxy(21,5);
          printf("   ");
          gotoxy(21,6);
          printf("   ");
          gotoxy(21,7);
          printf("   ");
          
       elevador=2;   
          
       } 
        else   
              //CIMA ELEVADOR 2
            if(tecla==119 && x==61 && l==15 ) 
            { 
              beep(1200,100);                  
              l=7;
              x=21;
              x=CONTROLES(tecla,x,l,1,0,0,0);
             //ELEVADOR 2
              gotoxy(61,11);
              printf("%c",219);
              gotoxy(62,11);
              printf("%c",219);
              gotoxy(60,12);
              printf("%c",219);
              gotoxy(60,13);
              printf("%c",219);
              gotoxy(60,14);
              printf("%c",219);
              gotoxy(60,15);
              printf("%c",219);
              
              gotoxy(63,11);
              printf("%c",219);
              gotoxy(64,12);
              printf("%c",219);
              gotoxy(64,13);
              printf("%c",219);
              gotoxy(64,14);
              printf("%c",219);
              gotoxy(64,15);
              printf("%c",219);
              
              textcolor(BLACK);
              //LIMPANDO PERSONAGEM NO ELEVADOR
              gotoxy(61,12);
              printf("   ");
              gotoxy(61,13);
              printf("   ");
              gotoxy(61,14);
              printf("   ");
              gotoxy(61,15);
              printf("   ");
              
              elevador=1;//para sair soh por cima,e terminar o cenario
            }  
           
                
       //BAIXO
       if(tecla==115)
        x=CONTROLES(tecla,x,l,1,0,0,0);
                   
       //TRAS
       if(tecla==97 && M[l][x-1]==0)
        x=CONTROLES(tecla,x,l,1,0,0,0);//ultimo numero 0 retorna x
         
       //FRENTE
       if(tecla==100 && M[l][x+1]==0)
        x=CONTROLES(tecla,x,l,1,0,0,0);//retorna x
        
       
       //pegando chave 1 libera porta 1
       if(x==3 && l==15 && chave1==0 && volta==0)
       {
          if(chave1==0)//para evitar bug,q fica pisando a parede q some quando vc fica na posicao q libera
          {         
              gotoxy(40,4);
              printf("  ");//porta 
              gotoxy(40,5);
              printf("  ");//porta 
              gotoxy(40,6);
              printf("  ");//porta 
              gotoxy(40,7);
              printf("  ");//porta  
              beep(1350,60);
              beep(1350,60);
           chave1=1;
          }    
          
          //LIBERANDO PORTA1 DA MATRIZ
          M[6][36]=0;//1
          M[7][36]=0;//1
       }
       
       //pegando chave 2 libera porta 2
       if(x==72 && l==15 && chave2==0 && volta==0)
       { 
           //PORTA 2
           gotoxy(60,4);
           printf("  ");//porta   
           gotoxy(60,5);
           printf("  ");//porta   
           gotoxy(60,6);
           printf("  ");//porta   
           gotoxy(60,7);
           printf("  ");//porta    
           
            chave2=1;
            beep(1350,60);
         
          //LIBERANDO PORTA2 DA MATRIZ
          M[6][56]=0;//2
          M[7][56]=0;//2
       }
  if(x==76 && l==7) //termino
  {
   fim=1;
   fase++;
  }    
  if(x<3 && l==7) //termino
  {
   fim=1;
   fase--;
  }  
  }while(fim==0);  
  return fase;
   
}

int CENARIO3(int fase,int volta,int chaveA)
{
      int x,l,tecla,elevador=0,chave1=0,chave2=0,fim=0,c;
      int M[25][80];
      for(x=1;x<80;x++)//COLOCANDO  ESQUELETO 0
      {
       for(l=1;l<25;l++)
        M[l][x]=0;
      }  
      /*//MOSTRANDO ESQUELETO PARA TESTES
      for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
       {
        gotoxy(x,l);                
        printf("%d",M[l][x]);
       }
      }
      getch();*/
      
      //COLOCANDO BARREIRAS LATERAIS ESQUELETO
      for(x=1;x<80;x++)
      {
        l=1;
        M[l][x]=1;
        M[24][x]=1;
      }  
      for(l=1;l<25;l++)
      {
        M[l][1]=1;
        M[l][76]=1;
      }
      
      /*//MOSTRANDO ESQUELETO PARA TESTES
      for(x=1;x<76;x++)
      {
       for(l=1;l<25;l++)
       {
        gotoxy(x,l);                
        printf("%d",M[l][x]);
       }
      }
      getch();*/
      
      //DESENHANDO CENARIO ESQUELETO
      //CAMINHO
      for(x=1;x<79;x++)
      {
        M[8][x]=1;
        M[20][x]=1;
      }
      if(volta==0)
      {
        //PORTAO
        for(l=8;l<20;l++)               
          M[l][56]=1;
          
          
      }  
      if(AUXFASE3==8)//colocando portao
      {
        //PORTAO
        for(l=8;l<20;l++)               
          M[l][56]=1;  
          
          textcolor(WHITE);
           for(l=8;l<20;l++)
           {
             gotoxy(11,l);
             printf("%c",186); 
            } 
      }               
                     
      
      if(chaveA==0 && MOSTROX>0)
      {
        //PORTA QUE ABRE AO PEGAR CHAVE NO CENARIO NADAR
        M[6][48]=1;//1
        M[7][48]=1;//1
      }
        
      //PORTAO FINAL
      for(l=8;l<20;l++)               
        M[l][11]=1;
      
      /*//MOSTRANDO ESQUELETO PARA TESTES
      for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
       {
        gotoxy(x,l);                
        printf("%d",M[l][x]);
       }
      } 
     
      getch();*/
      
      //DESENHANDO CENARIO GRAFICO
      //CAMINHO
      
      BORDAS(0);
      
  
      for(x=2;x<79;x++)
      {          
        textcolor(BROWN);               
        gotoxy(x,8);
        printf("%c",219);  
        if(x<75)
        {     
          gotoxy(x,20);
          printf("%c",219); 
        }  
        textcolor(LIGHTBLUE); 
        gotoxy(x,21);
        printf("%c",219); 
        gotoxy(x,22);
        printf("%c",219);   
        gotoxy(x,23);
        printf("%c",219);            
      }
      
      if(volta==0 && MOSTROX>0)
      {
          //CHAVE 1
          textcolor(YELLOW);
          gotoxy(35,6); 
          printf("%c",268);
          
          //CHAVE 2
          textcolor(YELLOW);
          gotoxy(21,18); 
          printf("%c",268); 
      }
      //DESENHANDO PORTAO 
          for(l=8;l<20;l++)
          {
            textcolor(WHITE);               
            /*gotoxy(15,l);
            printf("%c",205); //escada 
            gotoxy(16,l);
            printf("%c",205); //escada*/
            if(volta==0 && MOSTROX>0)
            {
              gotoxy(60,l);
              printf("%c",186); //PORTA
             }
            gotoxy(11,l);
            printf("%c",186); //PORTA FINAL PARA PEGAR A CARTEIRA DO ESCONDERIJO AO MATAR O ULTIMO CHEFE    
        
      }
      
      if(chaveA==0 && MOSTROX>0)
      {  
          //PORTA DA CHAVE DO CENARIO AGUA
          gotoxy(52,4);
          printf("%c",185);//porta 
          gotoxy(52,5);
          printf("%c",185);//porta 
          gotoxy(52,6);
          printf("%c",185);//porta 
          gotoxy(52,7);
          printf("%c",185);//porta   
      }
         
      //SEGREDO
      gotoxy(4,17);
      printf("? ? ?");
      
      
      //ELEVADOR 1
      gotoxy(21,3);
      printf("%c",219);
      gotoxy(22,3);
      printf("%c",219);
      gotoxy(20,4);
      printf("%c",219);
      gotoxy(20,5);
      printf("%c",219);
      gotoxy(20,6);
      printf("%c",219);
      gotoxy(20,7);
      printf("%c",219);
      
      gotoxy(23,3);
      printf("%c",219);
      gotoxy(24,4);
      printf("%c",219);
      gotoxy(24,5);
      printf("%c",219);
      gotoxy(24,6);
      printf("%c",219);
      gotoxy(24,7);
      printf("%c",219);
      if(AUXFASE3!=8)
      {
           //ELEVADOR 2
          gotoxy(53,15);
          printf("%c",219);
          gotoxy(54,15);
          printf("%c",219);
          gotoxy(52,16);
          printf("%c",219);
          gotoxy(52,17);
          printf("%c",219);
          gotoxy(52,18);
          printf("%c",219);
          gotoxy(52,19);
          printf("%c",219);
          
          gotoxy(55,15);
          printf("%c",219);
          gotoxy(56,16);
          printf("%c",219);
          gotoxy(56,17);
          printf("%c",219);
          gotoxy(56,18);
          printf("%c",219);
          gotoxy(56,19);
          printf("%c",219);
      }   
      
      //FIM GRAFICO
       
      
       if(volta==0)
      {
       x=4;
       l=7;
      }
      if(volta==1 && FASEX==5 && AUXFASE3==5)
      {
       x=70;
       l=7;
      }
      if(volta==1 && AUXFASE3==4)//voltando da agua
      {
       x=67;
       l=19;                      
      }
      if(MOSTROX==0 && AUXFASE3==8)//ao matar o mostro aparece no meio e libera a porta final
      {
         //PORTAO FINAL LIBERANDO MATRIZ
        for(c=8;c<20;c++)               
          M[c][11]=0;
          
          for(c=9;c<20;c++)//LIBERANDO PORTA GRAFICO FINAL
          {
            gotoxy(11,c);
            printf("  ");
            beep(1450,100);
            
            gotoxy(60,c); //DESENHANDO PORTAO PARA BLOQUEAR ENTRADA DA FASE AGUA
            printf("%c",186); //PORTA
          } 
          x=35;
          l=19;
          
          textcolor(YELLOW);//personagem
          gotoxy(x+1,l-3);
          printf("%c",2);//cabeça
          textcolor(WHITE);
          gotoxy(x,l-2);
          printf("%s","/|\\ ");
          gotoxy(x,l-1);
          printf("%s"," | ");
          gotoxy(x,l);
          printf("%s","/ \\ ");
          sleep(300);
          
          //LEGENDA
          gotoxy(x-19,l-6);
          printf(" A HORA DE RECUPERAR MEU NOTEBOOK CHEGOU.!");
          sleep(3000);
          gotoxy(x-19,l-6);
          printf("                                             ");
      }  
  
      textcolor(YELLOW);//personagem
      gotoxy(x+1,l-3);
      printf("%c",2);//cabeça
      textcolor(WHITE);
      gotoxy(x,l-2);
      printf("%s","/|\\ ");
      gotoxy(x,l-1);
      printf("%s"," | ");
      gotoxy(x,l);
      printf("%s","/ \\ ");
      sleep(300); 
  
  do
  {  
   if(kbhit())           
   tecla=getch();
   
  // tecla=getch();
   //CIMA
  // if(tecla==119 && x!=21 )
  //  x=CONTROLES(tecla,x,l,1);
   
   //CIMA ELEVADOR 1
   if(tecla==119 && x==21 && l==7 ) 
   {     
      l=19;
      x=53;
      x=CONTROLES(tecla,x,l,1,0,0,0);
    ///ELEVADOR 1
      gotoxy(21,3);
      printf("%c",219);
      gotoxy(22,3);
      printf("%c",219);
      gotoxy(20,4);
      printf("%c",219);
      gotoxy(20,5);
      printf("%c",219);
      gotoxy(20,6);
      printf("%c",219);
      gotoxy(20,7);
      printf("%c",219);
      
      gotoxy(23,3);
      printf("%c",219);
      gotoxy(24,4);
      printf("%c",219);
      gotoxy(24,5);
      printf("%c",219);
      gotoxy(24,6);
      printf("%c",219);
      gotoxy(24,7);
      printf("%c",219);
      
      textcolor(BLACK);
      //LIMPANDO PERSONAGEM NO ELEVADOR
      gotoxy(21,4);
      printf("   ");
      gotoxy(21,5);
      printf("   ");
      gotoxy(21,6);
      printf("   ");
      gotoxy(21,7);
      printf("   ");
      
   elevador=2;   
      
   } 
    else   
          //CIMA ELEVADOR 2
        if(tecla==119 && x==53 && l==19 && AUXFASE3!=8) 
        {     
          l=7;
          x=21;
          x=CONTROLES(tecla,x,l,1,0,0,0);
          //ELEVADOR 2
          gotoxy(53,15);
          printf("%c",219);
          gotoxy(54,15);
          printf("%c",219);
          gotoxy(52,16);
          printf("%c",219);
          gotoxy(52,17);
          printf("%c",219);
          gotoxy(52,18);
          printf("%c",219);
          gotoxy(52,19);
          printf("%c",219);
          
          gotoxy(55,15);
          printf("%c",219);
          gotoxy(56,16);
          printf("%c",219);
          gotoxy(56,17);
          printf("%c",219);
          gotoxy(56,18);
          printf("%c",219);
          gotoxy(56,19);
          printf("%c",219);
          
          textcolor(BLACK);
          //LIMPANDO PERSONAGEM NO ELEVADOR
          gotoxy(53,16);
          printf("   ");
          gotoxy(53,17);
          printf("   ");
          gotoxy(53,18);
          printf("   ");
          gotoxy(53,19);
          printf("   ");
          
          elevador=1;//para sair soh por cima,e terminar o cenario
        }  
       
            
   //BAIXO
   if(tecla==115)
    x=CONTROLES(tecla,x,l,1,0,0,0);
               
   //TRAS
   if(tecla==97 && M[l][x-1]==0)
    x=CONTROLES(tecla,x,l,1,0,0,0);//ultimo numero 0 retorna x
     
   //FRENTE
   if(tecla==100 && M[l][x+1]==0)
    x=CONTROLES(tecla,x,l,1,0,0,0);//retorna x
   
   tecla=0; 
   
   //pegando chave 1 
   if(x==21 && l==19 && chave1==0 && volta==0 && MOSTROX>0)
   {
    beep(1350,100);       
    chave1=1;
   } 
   //pegando chave 2 
   if(x==32 && l==7 && chave2==0 && volta==0 && MOSTROX>0)
   {
    beep(1350,100);        
    chave2=1;
   } 
   
   //LIBERANDO PORTAO APOS PEGAR AS 2 CHAVES
   if(chave1==1 && chave2==1 && volta==0 && MOSTROX>0)
   {
      int a;          
    //LIBERANDO PORTA DA MATRIZ
      for(a=8;a<20;a++)               
      M[a][56]=0;     
      
      //LIBERANDO PORTA GRAFICO
      for(a=9;a<20;a++) 
      {              
        gotoxy(60,a);
        printf("  ");
      }  
      chave1=3;//coloca para nao ficar apagando ao passar no portao apos liberar,senao apaga o personagem
      chave2=3;          
   }
   //PARA LIBERAR PORTA AO PEGAR CHAVE DA AGUA
    //LIBERANDO PORTA1 DA MATRIZ
  
  
  
  if(x==2 && l==7)//volta para cenario anterior
  {
   fase--;
   fim=1;  
  }          
  if(x==73 && l==19)//vai para cenario da agua
  {
    fase++;
    fim=1;  
  } 
  if(x==74 && l==7) //passa de cenario
  {
   fase=5;
   fim=1;        
  } 
  if(x<3 && l==19)//entrando pela porta final vai para ultima fase
  {
   fase=8;    
   fim=1;  
  }
                                            
  }while(fim!=1); 
  return fase;
  clrscr(); 
  FASEX=0;
}

int CENARIO4(int fase)
{
      int x,l,tecla=0,fim=0,chave1=0;
      int M[25][80];
      for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
        M[l][x]=0;
      }  
      
      
      //COLOCANDO BARREIRAS LATERAIS
      for(x=1;x<80;x++)
      {
        l=1;
        M[l][x]=1;
        M[24][x]=1;
      }  
      for(l=1;l<25;l++)
      {
        M[l][1]=1;
        M[l][79]=1;
      }
      //PAREDE CENTRAL
      for(x=8;x<72;x++)
      {
        for(l=2;l<20;l++)
        {
          if(x<22 || x>35)                              
          M[l][x]=1;
        }                  
      }
      //PASSAGEM MEIO DA PAREDE
      for(x=25;x<70;x++)
      {                          
          M[2][x]=0;
          M[3][x]=0;
          M[4][x]=0;
          M[5][x]=0;
      }       
    
     /*  for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
       {
        gotoxy(x,l);                
        printf("%d",M[l][x]);
       }
      }
      getch();*/

      //CENARIO GRAFICO
      BORDAS(0);
      system("color 94");
      for(l=2;l<20;l++)
      {
        textcolor(GREEN);               
      //PAREDE CENTRAL
        for(x=8;x<72;x++)
        {
          for(l=2;l<20;l++)
          { 
            if(x<22 || x>35)
            {                             
              gotoxy(x,l);
              printf("%c",219);
            }   
          }                  
        }
      }
       //PARTE DO MEIO DA PAREDE
      for(x=25;x<70;x++)
      {   
          textcolor(LIGHTBLUE);
          gotoxy(x,2);
          printf("%c",219);   
          gotoxy(x,3l);
          printf("%c",219);
          gotoxy(x,4l);
          printf("%c",219);
          gotoxy(x,5);
          printf("%c",219);
          gotoxy(x,6);
          printf("%c",219);                
      }  
      
      if(CHAVEX==0)
      {
           //FIM PAREDE
           
          //CHAVE 
          textbackground(LIGHTBLUE);
          textcolor(YELLOW);
          gotoxy(67,4); 
          printf("%c",268);
          
          
      }
      //CONTROLES NADAR
        x=2;
        l=4;
        
        textcolor(WHITE);
        //PERSONAGEM
        gotoxy(x,l);
        printf("%s","\\___0");
        gotoxy(x,l+1);
        printf("%s","/   \\");
        Beep(2460,50); //SI
    
      //FIM GRAFICO
      
      do
      {
            if(kbhit())
            tecla=getch();
            
            //LIBERANDO PORTA AO PEGAR CHAVE
            if(x==63 && l==4 && chave1==0 && CHAVEX==0)
            {
               int aux;
                     
               textbackground(LIGHTBLUE);
               textcolor(YELLOW);
               gotoxy(64,4); 
               printf("  "); 
               beep(1350,100);
               
                //PORTA DA CHAVE GRAFICA
              textcolor(WHITE);
              for(aux=20;aux<25;aux++)
              {                   
                gotoxy(60,aux);
                printf("  "); //PORTA
              }  
              //PORTA DA CHAVE ESQUELETO
              for(aux=20;aux<25;aux++)                 
                M[aux][60]=0;
                
              chave1=1; //para parar bug som
              CHAVEX=1; //variavel global
            }  
            
            
            //COMANDOS
            
            //CIMA   
            if(tecla==119 && M[l-1][x]==0 && M[l-1][x+1]==0 && M[l-1][x+2]==0 && M[l-1][x+3]==0 && M[l-1][x+4]==0)    
             l=CONTROLES(tecla,x,l,1,1,1,0);  
                   
           //BAIXO
           if(tecla==115 && M[l+1][x+5]==0 )
            l=CONTROLES(tecla,x,l,1,1,1,0);
                       
           //TRAS
           if(tecla==97 && M[l][x-1]==0)
            x=CONTROLES(tecla,x,l,1,1,0,0);
             
           //FRENTE
           if(tecla==100 && M[l][x+6]==0)
            x=CONTROLES(tecla,x,l,1,1,0,0);
            
           
             tecla=0;
      if(x==2 && l==2)//voltando
      {
       AUXFASE3=4;//fala para fase 3 que ta voltando da fase 4 da agua       
       fim=1;
       fase=3;
      }    
    }while(fim==0); 
      
      textcolor(WHITE);
      textbackground(BLACK);
      clrscr();  
      
      return fase;    
}

int CENARIO5(int fase,int volta)
{
      int x,l,fim=0,tecla=0,elevador=0,chave1=0,tiro,desvio=4,vida; //desvio se for 3,eh pq abaixou e o tiro nao acerta se for 4 nao abaixou
      int M[25][80];                                                //tiro eh para q lado vai o tiro
      for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
        M[l][x]=0;
      }  
      
      //COLOCANDO BARREIRAS LATERAIS
      for(x=1;x<80;x++)
      {
        l=1;
        M[l][x]=1;
        M[24][x]=1;
      }  
      for(l=1;l<25;l++)
      {
        M[l][1]=1;
        M[l][79]=1;
      }
      /*for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
       {
        gotoxy(x,l);                
        printf("%d",M[l][x]);
       }
      }
      getch();*/
      
      //DESENHANDO CENARIO ESQUELETO
      //CAMINHO
      for(x=1;x<79;x++)
      {
        M[7][x]=1;
        M[14][x]=1;
        M[21][x]=1;
      }
      //PORTAO para passar
          for(l=14;l<20;l++)               
            M[l+1][57]=1;
      //Bloqueio para nao encostar no tiro da parede
      M[12][4]=1;//
      M[13][4]=1;//     
            
            
     // M[8][14]=0;//escada
     // M[8][15]=0;//escada
     // M[8][16]=0;//escada
     // M[8][17]=0;//escada
      
      /*for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
       {
        gotoxy(x,l);                
        printf("%d",M[l][x]);
       }
      }*/
     
      //getch();
      
      //DESENHANDO CENARIO GRAFICO
      //CAMINHO
      for(x=2;x<80;x++)
      {          
        textcolor(BROWN);               
        gotoxy(x,7);
        printf("%c",219);  
        gotoxy(x,14);
        printf("%c",219);   
        gotoxy(x,21);
        printf("%c",219); 
         
        //AGUA
        textcolor(LIGHTBLUE); 
        gotoxy(x,22);
        printf("%c",219); 
        gotoxy(x,23);
        printf("%c",219);   
        gotoxy(x,24);
        printf("%c",219);            
      }
      
      //CHAVE 1
      textcolor(YELLOW);
      gotoxy(79,4); 
      printf("%c",219);
      gotoxy(78,4); 
      printf("/");
      
      //CHAVE 2
      textcolor(YELLOW);
      gotoxy(2,18); 
      printf("%c",219); 
      gotoxy(3,18); 
      printf("\\"); 
      
      //TIRO DA PAREDE
      textcolor(YELLOW);
      gotoxy(2,10); 
      printf("%c%s",219,"=");
      
      if(volta==0)
      {
        textcolor(RED);
        //ARMA
        gotoxy(62,17);
        printf("ARMA"); 
        gotoxy(64,19);
        printf("="); 
      }
      //PORTA PARA PASSAR
      for(l=14;l<20;l++)
      {
        textcolor(WHITE);               
        
        gotoxy(60,l+1);
        printf("%c",186); //PORTA
            
      }
      
        //ELEVADOR 2   1ºMEIO esquerdo
      gotoxy(14,9);
      printf("%c",219);
      gotoxy(15,9);
      printf("%c",219);
      gotoxy(13,10);
      printf("%c",219);
      gotoxy(13,11);
      printf("%c",219);
      gotoxy(13,12);
      printf("%c",219);
      gotoxy(13,13);
      printf("%c",219);
      
      gotoxy(16,9);
      printf("%c",219);
      gotoxy(17,10);
      printf("%c",219);
      gotoxy(17,11);
      printf("%c",219);
      gotoxy(17,12);
      printf("%c",219);
      gotoxy(17,13);
      printf("%c",219);
      
        //ELEVADOR 2   3ºMEIO direito
      gotoxy(58,9);
      printf("%c",219);
      gotoxy(59,9);
      printf("%c",219);
      gotoxy(57,10);
      printf("%c",219);
      gotoxy(57,11);
      printf("%c",219);
      gotoxy(57,12);
      printf("%c",219);
      gotoxy(57,13);
      printf("%c",219);
      
      gotoxy(60,9);
      printf("%c",219);
      gotoxy(61,10);
      printf("%c",219);
      gotoxy(61,11);
      printf("%c",219);
      gotoxy(61,12);
      printf("%c",219);
      gotoxy(61,13);
      printf("%c",219);
      
       //ELEVADOR 4 - ultimo
      gotoxy(53,16);
      printf("%c",219);
      gotoxy(54,16);
      printf("%c",219);
      gotoxy(52,17);
      printf("%c",219);
      gotoxy(52,18);
      printf("%c",219);
      gotoxy(52,19);
      printf("%c",219);
      gotoxy(52,20);
      printf("%c",219);
      
      gotoxy(55,16);
      printf("%c",219);
      gotoxy(56,17);
      printf("%c",219);
      gotoxy(56,18);
      printf("%c",219);
      gotoxy(56,19);
      printf("%c",219);
      gotoxy(56,20);
      printf("%c",219);
      
      BORDAS(0);
      
      //FIM GRAFICO
     
        
       if(volta==0)
       {
        x=4;
        l=6;                
       } 
       
       if(volta==1)
       {
        x=73;
        l=20;                 
       } 
       
       textcolor(YELLOW);//personagem
       gotoxy(x+1,l-3);
       printf("%c",2);//cabeça
       textcolor(WHITE);
       gotoxy(x,l-2);
       printf("%s","/|\\ ");
       gotoxy(x,l-1);
       printf("%s"," | ");
       gotoxy(x,l);
       printf("%s","/ \\ ");
       sleep(300); 
      
      tiro=4;//tiro que sai da parede
      vida=VIDAS;
    //CONTROLES
    do
    {
       if(kbhit())     
       tecla=getch();   
       
       //TIRO QUE SAI DA PAREDE///////////
       if(tiro-2==x && desvio==4 && l==13 && DEFESAX==0) //desvio diz se a pessoa abaixou,se abaixou vai ta 3 e nao 4
       {          
        gotoxy(3,22);
        printf("MORTO");  
        VIDAS--;        
       }
       if(l==13 && tiro+1>x && DEFESAX==1)//TIRO CHEGA A POSICAO X DO PERSONAGEM E RETORNA A 4 DE NOVO
         tiro=4;
      
       if(tiro>79)// CHEGA AO FIM DA TELA E VOLTA PARA 4
        tiro=4;
         
         gotoxy(tiro,10);
         printf("*");
         sleep(30); 
         gotoxy(tiro,10);
         printf(" ");
         sleep(10);  
         
         tiro++;
         tiro++;
       //FIM TIRO PAREDE/////////  
         
       if(x==74 && l==6 ) // SE LIGAR CHAVE 1
       {
             //ELEVADOR 1//primeiro
          textcolor(WHITE);
          gotoxy(21,2);
          printf("%c",219);
          gotoxy(22,2);
          printf("%c",219);
          gotoxy(20,3);
          printf("%c",219);
          gotoxy(20,4);
          printf("%c",219);
          gotoxy(20,5);
          printf("%c",219);
          gotoxy(20,6);
          printf("%c",219);
          
          gotoxy(23,2);
          printf("%c",219);
          gotoxy(24,3);
          printf("%c",219);
          gotoxy(24,4);
          printf("%c",219);
          gotoxy(24,5);
          printf("%c",219);
          gotoxy(24,6);
          printf("%c",219);  
          
          //CHAVE 1
          textcolor(YELLOW);
          gotoxy(79,4); 
          printf("%c",219);
          gotoxy(78,4); 
          printf("\\"); 
          
          chave1=1;
          
       }
       
       
       if(x==5 && l==20 ) // SE LIGAR CHAVE 2
       {
           int a;    
          //Liberando PORTA PARA PASSAR
          for(a=14;a<20;a++)               
            M[a+1][57]=0;           
            
          //CHAVE 2
          textcolor(YELLOW);
          gotoxy(2,18); 
          printf("%c",219); 
          gotoxy(3,18); 
          printf("/"); 
          
          //PORTA PARA PASSAR
          for(a=14;a<20;a++)
          {
            textcolor(WHITE);               
            
            gotoxy(60,a+1);
            printf("  "); //PORTA
                
          }
           
       }
               
          //CIMA ELEVADOR 1
       if(tecla==119 && x==21 && l==6 && chave1==1) 
       {   
          DEFESAX=0;    
          l=13;
          x=58;
          x=CONTROLES(tecla,x,l,1,0,0,0);
          textcolor(WHITE);
        ///ELEVADOR 1
          gotoxy(21,2);
          printf("%c",219);
          gotoxy(22,2);
          printf("%c",219);
          gotoxy(20,3);
          printf("%c",219);
          gotoxy(20,4);
          printf("%c",219);
          gotoxy(20,5);
          printf("%c",219);
          gotoxy(20,6);
          printf("%c",219);
          
          gotoxy(23,2);
          printf("%c",219);
          gotoxy(24,3);
          printf("%c",219);
          gotoxy(24,4);
          printf("%c",219);
          gotoxy(24,5);
          printf("%c",219);
          gotoxy(24,6);
          printf("%c",219);
          
          textcolor(BLACK);
          //LIMPANDO PERSONAGEM NO ELEVADOR
          gotoxy(21,3);
          printf("   ");
          gotoxy(21,4);
          printf("   ");
          gotoxy(21,5);
          printf("   ");
          gotoxy(21,6);
          printf("   ");
             
          
       } 
        else   
              //CIMA ELEVADOR 2
            if(tecla==119 && x==58 && l==13 ) 
            {     
              DEFESAX=0;      
              l=6;
              x=21;
              x=CONTROLES(tecla,x,l,1,0,0,0);
              //ELEVADOR 2   3ºMEIO direito
              textcolor(WHITE);
              gotoxy(58,9);
              printf("%c",219);
              gotoxy(59,9);
              printf("%c",219);
              gotoxy(57,10);
              printf("%c",219);
              gotoxy(57,11);
              printf("%c",219);
              gotoxy(57,12);
              printf("%c",219);
              gotoxy(57,13);
              printf("%c",219);
              
              gotoxy(60,9);
              printf("%c",219);
              gotoxy(61,10);
              printf("%c",219);
              gotoxy(61,11);
              printf("%c",219);
              gotoxy(61,12);
              printf("%c",219);
              gotoxy(61,13);
              printf("%c",219);
              
              textcolor(BLACK);
              //LIMPANDO PERSONAGEM NO ELEVADOR
              gotoxy(53,10);
              printf("   ");
              gotoxy(53,11);
              printf("   ");
              gotoxy(53,13);
              printf("   ");
              gotoxy(53,13);
              printf("   ");
              
            }  
           
            
                          //CIMA ELEVADOR 3
               if(tecla==119 && x==14 && l==13) 
               {     
                  DEFESAX=0;     
                  l=20;
                  x=53;
                  x=CONTROLES(tecla,x,l,1,0,0,0);
                  //ELEVADOR 2   1ºMEIO esquerdo
                  textcolor(WHITE);
                  gotoxy(14,9);
                  printf("%c",219);
                  gotoxy(15,9);
                  printf("%c",219);
                  gotoxy(13,10);
                  printf("%c",219);
                  gotoxy(13,11);
                  printf("%c",219);
                  gotoxy(13,12);
                  printf("%c",219);
                  gotoxy(13,13);
                  printf("%c",219);
                  
                  gotoxy(16,9);
                  printf("%c",219);
                  gotoxy(17,10);
                  printf("%c",219);
                  gotoxy(17,11);
                  printf("%c",219);
                  gotoxy(17,12);
                  printf("%c",219);
                  gotoxy(17,13);
                  printf("%c",219);
                  
                  textcolor(BLACK);
                  //LIMPANDO PERSONAGEM NO ELEVADOR
                  gotoxy(14,10);
                  printf("   ");
                  gotoxy(14,11);
                  printf("   ");
                  gotoxy(14,12);
                  printf("   ");
                  gotoxy(14,13);
                  printf("   "); 
                  
               } 
                else   
                      //CIMA ELEVADOR 4
                    if(tecla==119 && x==53 && l==20 ) 
                    {     
                      DEFESAX=0;      
                      l=13;
                      x=14;
                      x=CONTROLES(tecla,x,l,1,0,0,0);
                      //ELEVADOR 4 - ultimo
                      textcolor(WHITE);
                      gotoxy(53,16);
                      printf("%c",219);
                      gotoxy(54,16);
                      printf("%c",219);
                      gotoxy(52,17);
                      printf("%c",219);
                      gotoxy(52,18);
                      printf("%c",219);
                      gotoxy(52,19);
                      printf("%c",219);
                      gotoxy(52,20);
                      printf("%c",219);
                      
                      gotoxy(55,16);
                      printf("%c",219);
                      gotoxy(56,17);
                      printf("%c",219);
                      gotoxy(56,18);
                      printf("%c",219);
                      gotoxy(56,19);
                      printf("%c",219);
                      gotoxy(56,20);
                      printf("%c",219);
                      
                      textcolor(BLACK);
                      //LIMPANDO PERSONAGEM NO ELEVADOR
                      gotoxy(53,17);
                      printf("   ");
                      gotoxy(53,18);
                      printf("   ");
                      gotoxy(53,19);
                      printf("   ");
                      gotoxy(53,20);
                      printf("   ");
                      
                    }  
           
           
           //BAIXO
           if(tecla==115)
           {
            DEFESAX=0;               
            x=CONTROLES(tecla,x,l,1,0,0,0);
            desvio=3;
            }      
           //TRAS
           if(tecla==97 && M[l][x-1]==0)
           {
            DEFESAX=0;            
            x=CONTROLES(tecla,x,l,1,0,0,0);//ultimo numero 0 retorna x
            LADOTIRO=0;
            desvio=4;
           }  
           //FRENTE
           if(tecla==100 && M[l][x+4]==0)
           {
            DEFESAX=0;               
            x=CONTROLES(tecla,x,l,1,0,0,0);//retorna x
            LADOTIRO=1;
            desvio=4;
            }
            //TIRO
           if(tecla==32 && ARMAX==1)
           {
             DEFESAX=0;             
             CONTROLES(tecla,x,l,1,0,0,0);//retorna x 
           }
           //ESCUDO  
           if(tecla==102 && ESCUDOX==1)
           {
             DEFESAX=1;            
             CONTROLES(tecla,x,l,1,0,0,0);//retorna x  
           } 
           
           if(x==2 && l==6)//VOLTA FASE
           {
            fase=3;
            AUXFASE3=5;//fala para fase 3 que ta voltando da fase 5
            fim=1;      
            FASEX=5;   
           } 
           
           if(x==75 && l==20)//TERMINA FASE
           {
            fase++;
            fim=1;         
           } 
           if(x==62 && l==20) //PEGA ARMA
            ARMAX=1; //variavel global que verifica se arma foi pega
           
            tecla=0;
           
           if(vida>VIDAS)//se levou tiro perde vida e termina sem incrementar a fase
           {
              fim=1;
              fase=5;
              
              clrscr();
              beep(1350,100);
              textcolor(YELLOW);//personagem
              gotoxy(35,14);
              printf("%c",2);//cabeça
              textcolor(WHITE);
              gotoxy(34,15);
              printf("%s","/|\\ ");
              gotoxy(34,16);
              printf("%s"," | ");
              gotoxy(34,17);
              printf("%s","/ \\ ");
              sleep(300);
              
              gotoxy(38,16); //mostrando numero de vidas
              printf("VIDAS x%d",VIDAS);
              
              textcolor(YELLOW);
              gotoxy(25,20); //DICA
              printf("DICA: ABAIXE PARA DESVIAR DO TIRO .!");
              sleep(5000);
            }
  }while(fim==0);
  return fase;
}

int CENARIO6(int fase,int volta)
{
      int M[25][80];
      int x,l,tecla=0,desvio=4,fim=0,ini1=0,ini2=0,dir1=1,dir2=0,conttiro=0,vida,tiro;                                                
      for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
        M[l][x]=0;
      }  
      
      //COLOCANDO BARREIRAS LATERAIS
      for(x=1;x<80;x++)
      {
        l=1;
        M[l][x]=1;
        M[24][x]=1;
      }  
      for(l=1;l<25;l++)
      {
        M[l][1]=1;
        M[l][79]=1;
      }
     
      
      //DESENHANDO CENARIO ESQUELETO
      //CAMINHO
      for(x=1;x<79;x++)
      {
        M[7][x]=1;
        M[14][x]=1;
        M[21][x]=1;
      }    
            
      
      
      //DESENHANDO CENARIO GRAFICO
      //CAMINHO
      for(x=2;x<80;x++)
      {          
        textcolor(BROWN);               
        gotoxy(x,7);
        printf("%c",219);  
        gotoxy(x,14);
        printf("%c",219);   
        gotoxy(x,21);
        printf("%c",219); 
         
        //AGUA
        textcolor(LIGHTBLUE); 
        gotoxy(x,22);
        printf("%c",219); 
        gotoxy(x,23);
        printf("%c",219);   
        gotoxy(x,24);
        printf("%c",219);            
      }
      
      //INIMIGO 1
      textcolor(RED);
      gotoxy(15+1,3);
      printf("%c",2);//cabeça
      textcolor(WHITE);
      gotoxy(15,4);
      printf("%s","/|\\ ");
      gotoxy(15,5);
      printf("%s"," | ");
      gotoxy(15,6);
      printf("%s","/ \\ ");  
      
       //INIMIGO 2
       textcolor(RED);
       gotoxy(64+1,17);
       printf("%c",2);//cabeça
       textcolor(WHITE);
       gotoxy(64,18);
       printf("%s","/|\\ ");
       gotoxy(64,19);
       printf("%s"," | ");
       gotoxy(64,20);
       printf("%s","/ \\ ");  
      
      //ESCUDO
      
      gotoxy(34,9); 
      printf("-ESCUDO"); 
      textcolor(LIGHTBLUE);
      gotoxy(37,11); 
      printf("I"); 
      gotoxy(37,12); 
      printf("I");    
           
      //SETA -->
      textcolor(YELLOW);
      gotoxy(73,11); 
      printf("%c",282);
      
      //SETA <--
      textcolor(YELLOW);
      gotoxy(73,4); 
      printf("%c",283);
           
     //ELEVADOR 1//primeiro
      textcolor(WHITE);
      gotoxy(7,2);
      printf("%c",219);
      gotoxy(8,2);
      printf("%c",219);
      gotoxy(6,3);
      printf("%c",219);
      gotoxy(6,4);
      printf("%c",219);
      gotoxy(6,5);
      printf("%c",219);
      gotoxy(6,6);
      printf("%c",219);
      
      gotoxy(9,2);
      printf("%c",219);
      gotoxy(10,3);
      printf("%c",219);
      gotoxy(10,4);
      printf("%c",219);
      gotoxy(10,5);
      printf("%c",219);
      gotoxy(10,6);
      printf("%c",219); 
      
        //ELEVADOR 2   
      textcolor(WHITE);
      gotoxy(7,16);
      printf("%c",219);
      gotoxy(8,16);
      printf("%c",219);
      gotoxy(6,17);
      printf("%c",219);
      gotoxy(6,18);
      printf("%c",219);
      gotoxy(6,19);
      printf("%c",219);
      gotoxy(6,20);
      printf("%c",219);
      
      gotoxy(9,16);
      printf("%c",219);
      gotoxy(10,17);
      printf("%c",219);
      gotoxy(10,18);
      printf("%c",219);
      gotoxy(10,19);
      printf("%c",219);
      gotoxy(10,20);
      printf("%c",219); 
      
      BORDAS(0);
      //FIM GRAFICO///////////////////
     
      
      if(volta==0)
      {
       x=4;           
       l=13;           
      }
      if(volta==1)
      {
       x=73;           
       l=20;           
      }
      //teste 
      
       textcolor(YELLOW);//personagem
       gotoxy(x+1,l-3);
       printf("%c",2);//cabeça
       textcolor(WHITE);
       gotoxy(x,l-2);
       printf("%s","/|\\ ");
       gotoxy(x,l-1);
       printf("%s"," | ");
       gotoxy(x,l);
       printf("%s","/ \\ ");
       sleep(300);
       
      //CONTROLES//////////////////////////////
      
      vida=VIDAS;
      //POSICOES DOS INIMIGOS
      ini1=15;
      ini2=65;
      VIDASINI1=3; //numero de tiros que o inimigo 1 tem que levar
      VIDASINI2=3; //numero de tiros que o inimigo 2 tem que levar
      INIX=0;
      INIX2=0;
      do
      {
         if(kbhit())
          tecla=getch();
        
          //CIMA
             //CIMA ELEVADOR 1
           if(tecla==119 && x==7 && l==6 ) 
           {  
              DEFESAX=0;              
              l=20;
              x=7;
              x=CONTROLES(tecla,x,l,1,0,0,0);
              textcolor(WHITE);
              //ELEVADOR 1
              gotoxy(7,2);
              printf("%c",219);
              gotoxy(8,2);
              printf("%c",219);
              gotoxy(6,3);
              printf("%c",219);
              gotoxy(6,4);
              printf("%c",219);
              gotoxy(6,5);
              printf("%c",219);
              gotoxy(6,6);
              printf("%c",219);
              
              gotoxy(9,2);
              printf("%c",219);
              gotoxy(10,3);
              printf("%c",219);
              gotoxy(10,4);
              printf("%c",219);
              gotoxy(10,5);
              printf("%c",219);
              gotoxy(10,6);
              printf("%c",219);
              
              textcolor(BLACK);
              //LIMPANDO PERSONAGEM NO ELEVADOR
              gotoxy(7,3);
              printf("   ");
              gotoxy(7,4);
              printf("   ");
              gotoxy(7,5);
              printf("   ");
              gotoxy(7,6);
              printf("   ");
                 
              
           } 
            else   
                  //CIMA ELEVADOR 2
                if(tecla==119 && x==7 && l==20 ) 
                {  
                  DEFESAX=0;               
                  l=6;
                  x=7;
                  x=CONTROLES(tecla,x,l,1,0,0,0);
                  //ELEVADOR 2   3ºMEIO direito
                  //ELEVADOR 2   
                  textcolor(WHITE);
                  gotoxy(7,16);
                  printf("%c",219);
                  gotoxy(8,16);
                  printf("%c",219);
                  gotoxy(6,17);
                  printf("%c",219);
                  gotoxy(6,18);
                  printf("%c",219);
                  gotoxy(6,19);
                  printf("%c",219);
                  gotoxy(6,20);
                  printf("%c",219);
                  
                  gotoxy(9,16);
                  printf("%c",219);
                  gotoxy(10,17);
                  printf("%c",219);
                  gotoxy(10,18);
                  printf("%c",219);
                  gotoxy(10,19);
                  printf("%c",219);
                  gotoxy(10,20);
                  printf("%c",219); 
                  
                  textcolor(BLACK);
                  //LIMPANDO PERSONAGEM NO ELEVADOR
                  gotoxy(7,17);
                  printf("   ");
                  gotoxy(7,18);
                  printf("   ");
                  gotoxy(7,19);
                  printf("   ");
                  gotoxy(7,20);
                  printf("   ");
                  
                }  
    
          //BAIXO
           if(tecla==115)
           {
            DEFESAX=0;             
            x=CONTROLES(tecla,x,l,1,0,0,0);
            desvio=3;
            }      
           //TRAS
           if(tecla==97 && M[l][x-1]==0)
           {
            DEFESAX=0;            
            x=CONTROLES(tecla,x,l,1,0,0,0);//ultimo numero 0 retorna x
            LADOTIRO=0;//lado que o personagem esta virado para saber para onde vai atirar
            desvio=4;
           }  
           //FRENTE
           if(tecla==100 && M[l][x+4]==0)
           {
            DEFESAX=0;             
            x=CONTROLES(tecla,x,l,1,0,0,0);//retorna x
            LADOTIRO=1;//lado que o personagem esta virado para saber para onde vai atirar
            desvio=4;
            }
            //TIRO
           if(tecla==32 && ARMAX==1)
           {
             DEFESAX=0;         
             if(LADOTIRO==0 && INIX==0) //se 0 inimigo 1  
             CONTROLES(tecla,x,l,1,0,0,1);//retorna x 
             if(LADOTIRO==1 && INIX2==0) //se 0 inimigo 1  
             CONTROLES(tecla,x,l,1,0,0,2);//retorna x
           } 
           //ESCUDO  
           if(tecla==102 && ESCUDOX==1)
           {
             DEFESAX=1;            
             CONTROLES(tecla,x,l,1,0,0,0);//retorna x  
           } 
           //FIM CONTROLES  
           
           
           //INIMIGOS DO CENARIO MOVIMENTOS E ACOES//////////////////////////
           
           if(l==6 && x<64 && INIX==0)
            conttiro=TIROINIMIGO(32,x,l,conttiro,1);//inimigo 1
           if(l==20 && x>10 && INIX2==0)
            conttiro=TIROINIMIGO(32,x,l,conttiro,2);//inimigo 2
    
           
           //FIM INIMIGOS DO CENARIO MOVIMENTOS E ACOES//////////////////
           
           
           //CONDICOES DO CENARIO//////////////////////////////////////////////////
           if(x>74 && l==13)
           {
             //LIMPA PERSONAGEM ONDE ESTAVA
             gotoxy(x,l-3);
             printf("%s","   ");
             gotoxy(x,l-2);
             printf("%s","    ");
             gotoxy(x,l-1);
             printf("%s","   "); 
             gotoxy(x,l);
             printf("%s","     ");   
                 
             x=73;           
             l=6;    
             tecla=0;//atribui 0,para nao ficar alternando para tras   
             
              //DESENHANDO NA NOVA POSICAO O PERSONAGEM
             textcolor(YELLOW);
             gotoxy(x+1,l-3);
             printf("%c",2);//cabeça
             textcolor(WHITE);
             gotoxy(x,l-2);
             printf("%s","/|\\");
             gotoxy(x,l-1);
             printf("%s"," | ");
             gotoxy(x,l);
             printf("%s","/ \\ ");    
           }
           if(x>74 && l==6)
           {
             //LIMPA PERSONAGEM ONDE ESTAVA
             gotoxy(x,l-3);
             printf("%s","   ");
             gotoxy(x,l-2);
             printf("%s","    ");
             gotoxy(x,l-1);
             printf("%s","   "); 
             gotoxy(x,l);
             printf("%s","     "); 
                   
             x=73;           
             l=13;   
             tecla=0;//atribui 0,para nao ficar alternando para tras  
             
             //DESENHANDO NA NOVA POSICAO O PERSONAGEM
             textcolor(YELLOW);
             gotoxy(x+1,l-3);
             printf("%c",2);//cabeça
             textcolor(WHITE);
             gotoxy(x,l-2);
             printf("%s","/|\\");
             gotoxy(x,l-1);
             printf("%s"," | ");
             gotoxy(x,l);
             printf("%s","/ \\");       
           }
           
           if(x==34 && l==13)
           {
             gotoxy(34,9); 
             printf("       ");       
             ESCUDOX=1;//variavel global recebe 1 falando que pegou o escudo
           } 
           //FIM CONDICOES DO CENARIO//////////////////////////////////////////////////
           
           if(x<3 && l==13)
           {
             fase--;          
             fim=1;         
           }
           if(x>74 && l==20)
           {
             fase++;          
             fim=1;         
           }
           if(vida>VIDAS)//se levou tiro perde vida e termina sem incrementar a fase
           {
               fim=1;
               fase=6;
               
               clrscr();
               beep(1350,100);
               textcolor(YELLOW);//personagem
               gotoxy(35,14);
               printf("%c",2);//cabeça
               textcolor(WHITE);
               gotoxy(34,15);
               printf("%s","/|\\ ");
               gotoxy(34,16);
               printf("%s"," | ");
               gotoxy(34,17);
               printf("%s","/ \\ ");
               sleep(300);
              
               gotoxy(38,16); //mostrando numero de vidas
               printf("VIDAS x%d",VIDAS);
              
               textcolor(YELLOW);
               gotoxy(22,20); //DICA
               printf("DICA: USE O ESCUDO PARA DEFENDER O TIRO .!");
               sleep(5000);
            }
             
           tecla=0;
     }while(fim==0);
     
     return fase;     
}

int CENARIO7(int fase,int volta)
{
   int M[25][80];
      int x,l,tecla=0,mostro=0,acordado=0,t,conttiro=1,tirom,vida,fim=0,chave=0;                                                
      for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
        M[l][x]=0;
      }  
      
      //COLOCANDO BARREIRAS LATERAIS
      for(x=1;x<80;x++)
      {
        l=1;
        M[l][x]=1;
        M[24][x]=1;
      }  
      for(l=1;l<25;l++)
      {
        M[l][1]=1;
        M[l][79]=1;
      }
      
      //DESENHANDO CENARIO ESQUELETO
      //CAMINHO
      for(x=1;x<79;x++)
      {
        M[7][x]=1;
        M[21][x]=1;
      }    
      
      //DESENHANDO CENARIO GRAFICO
      //CAMINHO
      for(x=2;x<80;x++)
      {          
        textcolor(DARKGRAY);               
        gotoxy(x,7);
        printf("%c",219);   
        gotoxy(x,21);
        printf("%c",219); 
         
        //AGUA
        textcolor(RED); 
        gotoxy(x,22);
        printf("%c",219); 
        gotoxy(x,23);
        printf("%c",219);   
        gotoxy(x,24);
        printf("%c",219);            
      }
          
     //ELEVADOR 1//primeiro
      textcolor(WHITE);
      gotoxy(7,2);
      printf("%c",219);
      gotoxy(8,2);
      printf("%c",219);
      gotoxy(6,3);
      printf("%c",219);
      gotoxy(6,4);
      printf("%c",219);
      gotoxy(6,5);
      printf("%c",219);
      gotoxy(6,6);
      printf("%c",219);
      
      gotoxy(9,2);
      printf("%c",219);
      gotoxy(10,3);
      printf("%c",219);
      gotoxy(10,4);
      printf("%c",219);
      gotoxy(10,5);
      printf("%c",219);
      gotoxy(10,6);
      printf("%c",219); 
      
      //MOSTRO DORMINDO
          textcolor(GREEN);
          gotoxy(63,8);
          printf("                ");
          gotoxy(63,9);
          printf("                ");
          gotoxy(63,10);
          printf("            ///");
          gotoxy(63,11);
          printf("        __-(---)/");
          gotoxy(63,12);
          printf("   ___--       ");
          gotoxy(63,13);
          printf("  |            ");
          gotoxy(63,14);
          printf(" / o          ");
          gotoxy(63,15);
          printf(" \\/\\/\\/\\/\\/\\  ");
          gotoxy(63,16);
          printf(" /\\/\\/\\/\\/\\/  ");
          gotoxy(63,17);
          printf("|             ");
          gotoxy(63,18);
          printf("|             ");
          gotoxy(63,19);
          printf("\\_____       ");
          gotoxy(63,20);
          printf("      \\_____/");
      
      //DESENHANDO NA NOVA POSICAO O PERSONAGEM
         textcolor(YELLOW);
         gotoxy(65+1,3);
         printf("%c",2);//cabeça
         textcolor(WHITE);
         gotoxy(65,4);
         printf("%s","/|\\");
         gotoxy(65,5);
         printf("%s"," | ");
         gotoxy(65,6);
         printf("%s","/ \\");  
      
      BORDAS(2);
      textcolor(WHITE);
      //FIM GRAFICO
      
      mostro=5; //mostro eh numero de vidas do mostro
      //CONTROLES///////////////////////////////////////
      x=65;
      l=6;
      tirom=62;
      vida=VIDAS;
      MOSTROX=5;//vida do mostro
      chave=0;
      do
      {
             if(kbhit())
              tecla=getch();
             if(MOSTROX>0) 
              conttiro++;
             if(conttiro>15) 
              conttiro=1;
              
             sleep(100);
            
              //MOSTRO ACORDADO PARA ATIRAR
             if(l==20 && conttiro==7 & MOSTROX>0)
             { 
                  textcolor(WHITE);
                  gotoxy(53,10);
                  printf("AAAAAHHHH.!!");
                  sleep(200);
                  textcolor(GREEN);
                  gotoxy(63,8);
                  printf("            ///");
                  gotoxy(63,9);
                  printf("        __-(O)/");
                  gotoxy(63,10);
                  printf("   ___--       ");
                  gotoxy(63,11);
                  printf("  |            ");
                  gotoxy(63,12);
                  printf(" / o          ");
                  gotoxy(63,13);
                  printf(" \\/\\/\\/\\/\\/\\");
                  gotoxy(63,14);
                  printf("            |");
                  gotoxy(63,15);
                  printf("            |");
                  gotoxy(63,16);
                  printf("            |");
                  gotoxy(63,17);
                  printf("            |");
                  gotoxy(63,18);
                  printf(" /\\/\\/\\/\\/\\/");
                  gotoxy(63,19);
                  printf("|_____       ");
                  gotoxy(63,20);
                  printf("      \\_____/");
                  acordado=1;
             }    
                        
            if(l==20 && conttiro==15 && MOSTROX>0)
            {
                  do
                  {  
                        tirom--; 
                        
                        textcolor(RED);
                        gotoxy(tirom,15);
                        printf("%c",219);
                        gotoxy(tirom,16);
                        printf("%c",219);
                        sleep(10); 
                        gotoxy(tirom,17);
                        printf("%c",219);
                                    
                        if(tirom==x && DEFESAX==0)//inimigo acerta personagem e o mata perdendo uma vida         
                          VIDAS--;
                       
                        if(tirom-6<x && DEFESAX==1)//defende tiro
                         tirom=4;
                        
                  }while(tirom>=14);            
                  
                  tirom=62;
                   textcolor(WHITE);
                   
                    do //LIMPANDO TIRO DO MOSTRO
                    {
                       tirom--;           
                      //tiro boca do mostro
                       
                       gotoxy(tirom,15);
                       printf("   "); 
                       gotoxy(tirom,16);
                       printf("   ");
                       gotoxy(tirom,17);
                       printf("   ");          
                                    
                       if(tirom-6<x && DEFESAX==1)//defende tiro
                         tirom=4;             
                   }while(tirom>=14);
                   tirom=62;
                   
            }         
             if(MOSTROX<=0)
             {
               //MOSTRO MORTO
                  textcolor(WHITE);
                  gotoxy(53,10);
                  printf("MORTO....!!!!!  ");
                  textcolor(GREEN);
                  gotoxy(63,8);
                  printf("                ");
                  gotoxy(63,9);
                  printf("                ");
                  gotoxy(63,10);
                  printf("            ///");
                  gotoxy(63,11);
                  printf("        __-(X)/");
                  gotoxy(63,12);
                  printf("   ___--       ");
                  gotoxy(63,13);
                  printf("  |            ");
                  gotoxy(63,14);
                  printf(" / o          ");
                  gotoxy(63,15);
                  printf(" \\/\\/\\/\\/\\/\\  ");
                  gotoxy(63,16);
                  printf(" /\\/\\/\\/\\/\\/  ");
                  gotoxy(63,17);
                  printf("|             ");
                  gotoxy(63,18);
                  printf("|             ");
                  gotoxy(63,19);
                  printf("\\_____       ");
                  gotoxy(63,20);
                  printf("      \\_____/"); 
                  
                  //CHAVE 1
                  textcolor(YELLOW);
                  gotoxy(40,18); 
                  printf("%c",268);     
                            
                             
             }
             if(conttiro==1 && MOSTROX>0)
             {
                  //MOSTRO ACORDADO DE BOCA FECHADA
                  textcolor(WHITE);
                  gotoxy(53,10);
                  printf("..............");
                  sleep(200);
                  textcolor(GREEN);
                  gotoxy(63,8);
                  printf("                ");
                  gotoxy(63,9);
                  printf("                ");
                  gotoxy(63,10);
                  printf("            ///");
                  gotoxy(63,11);
                  printf("        __-(O)/");
                  gotoxy(63,12);
                  printf("   ___--       ");
                  gotoxy(63,13);
                  printf("  |            ");
                  gotoxy(63,14);
                  printf(" / o          ");
                  gotoxy(63,15);
                  printf(" \\/\\/\\/\\/\\/\\  ");
                  gotoxy(63,16);
                  printf(" /\\/\\/\\/\\/\\/  ");
                  gotoxy(63,17);
                  printf("|             ");
                  gotoxy(63,18);
                  printf("|             ");
                  gotoxy(63,19);
                  printf("\\_____       ");
                  gotoxy(63,20);
                  printf("      \\_____/");
                  
             }    
                  //CIMA
                     //CIMA ELEVADOR 1
                   if(tecla==119 && x==7 && l==6 ) 
                   {  
                      DEFESAX=0;              
                      l=20;
                      x=7;
                      x=CONTROLES(tecla,x,l,1,0,0,0);
                      textcolor(WHITE);
                      //ELEVADOR 1
                      gotoxy(7,2);
                      printf("%c",219);
                      gotoxy(8,2);
                      printf("%c",219);
                      gotoxy(6,3);
                      printf("%c",219);
                      gotoxy(6,4);
                      printf("%c",219);
                      gotoxy(6,5);
                      printf("%c",219);
                      gotoxy(6,6);
                      printf("%c",219);
                      
                      gotoxy(9,2);
                      printf("%c",219);
                      gotoxy(10,3);
                      printf("%c",219);
                      gotoxy(10,4);
                      printf("%c",219);
                      gotoxy(10,5);
                      printf("%c",219);
                      gotoxy(10,6);
                      printf("%c",219);
                      
                      textcolor(BLACK);
                      //LIMPANDO PERSONAGEM NO ELEVADOR
                      gotoxy(7,3);
                      printf("   ");
                      gotoxy(7,4);
                      printf("   ");
                      gotoxy(7,5);
                      printf("   ");
                      gotoxy(7,6);
                      printf("   ");
                         
                      
                   } 
                    
                  //BAIXO
                   if(tecla==115)
                   {
                    DEFESAX=0;             
                    x=CONTROLES(tecla,x,l,1,0,0,0);
                    }      
                   //TRAS
                   if(tecla==97 && M[l][x-1]==0)
                   {
                    DEFESAX=0;            
                    x=CONTROLES(tecla,x,l,1,0,0,0);//ultimo numero 0 retorna x
                    LADOTIRO=0;//lado que o personagem esta virado para saber para onde vai atirar
                   }  
                   //FRENTE
                   if(tecla==100 && M[l][x+4]==0)
                   {
                    DEFESAX=0;             
                    x=CONTROLES(tecla,x,l,1,0,0,0);//retorna x
                    LADOTIRO=1;//lado que o personagem esta virado para saber para onde vai atirar
                    }
                    //TIRO
                   if(tecla==32 && ARMAX==1)
                   {
                     DEFESAX=0;            
                     if(LADOTIRO==0)
                     CONTROLES(tecla,x,l,1,0,0,3);//retorna x 
                     if(LADOTIRO==1)//atirando no mostro
                     CONTROLES(tecla,x,l,1,0,0,3);//retorna x
                   } 
                   //ESCUDO  
                   if(tecla==102 && ESCUDOX==1)
                   {
                     DEFESAX=1;            
                     CONTROLES(tecla,x,l,1,0,0,0);//retorna x  
                   } 
                   //FIM CONTROLES  ////////////////////////////////
                   tecla=0;
                   
                   if(vida>VIDAS)//MORRE AO LEVAR TIRO
                   {
                       fase=7;
                       fim=1;
                       
                       clrscr();
                       beep(1350,100);
                       textcolor(YELLOW);//personagem
                       gotoxy(35,14);
                       printf("%c",2);//cabeça
                       textcolor(WHITE);
                       gotoxy(34,15);
                       printf("%s","/|\\ ");
                       gotoxy(34,16);
                       printf("%s"," | ");
                       gotoxy(34,17);
                       printf("%s","/ \\ ");
                       sleep(300);
                      
                       gotoxy(38,16); //mostrando numero de vidas
                       printf("VIDAS x%d",VIDAS);
                      
                       textcolor(YELLOW);
                       gotoxy(22,20); //DICA
                       printf("DICA: USE O ESCUDO PARA DEFENDER O TIRO .!");
                       sleep(5000);
                   } 
                  
                   if(x>36 && chave==0)
                   {
                    //CHAVE 1
                      textcolor(YELLOW);
                      gotoxy(40,18); 
                      printf("  ");     
                      chave=1;    
                      beep(1300,100);   
                   }
                   if(MOSTROX<=0 && x>39)
                   {
                     beep(1350,100);             
                     textcolor(YELLOW);
                     gotoxy(x,17);
                     printf("\\%c/",2);//cabeça
                     textcolor(WHITE);
                     gotoxy(x,18);
                     printf("%s"," |  MATEI ESSE BICHO");
                     gotoxy(x,19);
                     printf("%s"," | ");
                     gotoxy(x,20);
                     printf("%s","/ \\");  
                     
                     sleep(2000);   
                     gotoxy(x,18);
                     printf("%s"," |  DO CACETE,MERDAO ");
                     sleep(2000);   
                     gotoxy(x,18);
                     printf("%s"," |  LADRAO AI VOU EU.!");
                     sleep(2500);
                     
                     fim=1;
                     fase=3;      
                     AUXFASE3=8;    
                   }
                   
      }while(fim==0);     
      return fase;
}            


int CENARIO8(int fase)
{
 int M[25][80];
      int x,l,tecla=0,fim=0,conttiro=1,aux=1,tiro,vida;                                                
      for(x=1;x<80;x++)
      {
       for(l=1;l<25;l++)
        M[l][x]=0;
      }  
      
      //COLOCANDO BARREIRAS LATERAIS
      for(x=1;x<80;x++)
      {
        l=1;
        M[l][x]=1;
        M[24][x]=1;
      }  
      for(l=1;l<25;l++)
      {
        M[l][1]=1;
        M[l][79]=1;
      }
    
      
      //DESENHANDO CENARIO ESQUELETO
      //CAMINHO
      for(x=1;x<79;x++)
        M[21][x]=1;
        
            
      
      //DESENHANDO CENARIO GRAFICO
      //CAMINHO
      for(x=2;x<80;x++)
      {          
        textcolor(DARKGRAY);                  
        gotoxy(x,21);
        printf("%c",219); 
         
        //AGUA
        textcolor(RED); 
        gotoxy(x,22);
        printf("%c",219); 
        gotoxy(x,23);
        printf("%c",219);   
        gotoxy(x,24);
        printf("%c",219);            
      } 
      BORDAS(3);
     // if(INIX==10)
     // {
          textcolor(WHITE);
          //NOTEBOOK
          gotoxy(2,20);
          printf(" \\__");
          //INIMIGO
          textbackground(BLACK);
            //APARECENDO INIMIGO NOVAMENTE
            textcolor(LIGHTBLUE);
            gotoxy(10+1,17);
            printf("%c",1);//cabeça
            textcolor(WHITE);
            gotoxy(10,18);
            printf("%s","/|\\ ");
            gotoxy(10,19);
            printf("%s"," |   ");
            gotoxy(10,20);
            printf("%s","/ \\ ");
            Beep(1300,100); //DO
            sleep(250); 
            
            //PERSONAGEM
            textcolor(YELLOW);
             gotoxy(65+1,17);
             printf("%c",2);//cabeça
             textcolor(WHITE);
             gotoxy(65,18);
             printf("%s","/|\\");
             gotoxy(65,19);
             printf("%s"," | ");
             gotoxy(65,20);
             printf("%s","/ \\"); 
             sleep(2000);
             //LEGENDAS////////////////////////////////////////
          
             gotoxy(65-2,18);
             printf("%s","---|\\");//personagem apontando
             
             gotoxy(50,14); //personagem
             printf("POW MEU NOTEBOOK,MALDITO");
             sleep(3000);
             gotoxy(50,14); //limpando
             printf("                        ");
             sleep(1000);
             
             gotoxy(10,14); //INIMIGO
             printf("E DAI AGORA EH MEU");
             sleep(3000);
             gotoxy(10,14); //LIMPANDO LEGENDA
             printf("                   ");
             sleep(1000);
             
             gotoxy(50,14); //personagem
             printf("CARA ME DEVOLVE LOGO ");
             sleep(3000);
             gotoxy(50,14); //limpando
             printf("                     ");
             sleep(1000);
             
             gotoxy(10,14); //INIMIGO
             printf("NAO VOU SEU TROUXA ");
             sleep(3000);
             gotoxy(10,14); //LIMPANDO LEGENDA
             printf("                   ");
             sleep(1000);
             
             gotoxy(50,14); //personagem
             printf("ENTAO VOU TER QUE TE MATAR.!");
             sleep(3000);
             gotoxy(50,14); //limpando
             printf("                            ");
             sleep(1000);
             
             gotoxy(10,14); //INIMIGO
             printf("ISSO EH O QUE VEREMOS..!!");
             sleep(3000);
             gotoxy(10,14); //LIMPANDO LEGENDA
             printf("                         ");
             sleep(1000);
             
             gotoxy(50,14); //personagem
             printf("COITADO DE VOCE ");
             sleep(3000);
             gotoxy(50,14); //limpando
             printf("                ");
             sleep(1000);
             
             gotoxy(10,14); //INIMIGO
             printf("VOU USAR MEU PODER SECRETO");
             sleep(3000);
             gotoxy(10,14); //LIMPANDO LEGENDA
             printf("                          ");
             sleep(1000);
             
             gotoxy(10,14); //INIMIGO
             printf("AAAAAAAAAAAAAAAHHHHHHHH...!!");
             sleep(3000);
             gotoxy(10,14); //LIMPANDO LEGENDA
             printf("                            ");
             sleep(1000);
          
     
          //INIMIGO CRECENDO COM PODER
          textcolor(LIGHTBLUE);
          gotoxy(10,10);
          printf("      --        ");
          gotoxy(10,11);
          printf("     ( 0 ) ");
          textcolor(WHITE);
          gotoxy(10,12);
          printf("      ||        ");
          gotoxy(10,13);
          printf("   /  ||   \\    ");
          gotoxy(10,14);
          printf(" /    ||    \\   ");
          gotoxy(10,15);
          printf("      ||     ");
          gotoxy(10,16);
          printf("      ||     ");
          gotoxy(10,17);
          printf("    /    \\     ");
          gotoxy(10,18);
          printf("  /        \\   ");
          gotoxy(10,19);
          printf(" /           \\");
          gotoxy(10,20);
          printf("/             \\");
          
          gotoxy(10,8); //INIMIGO
         printf("E AGORA VAI ME MATAR AINDA ?");
         sleep(3000);
         gotoxy(10,8); //LIMPANDO LEGENDA
         printf("                            ");
         sleep(1000);
         
         gotoxy(50,14); //personagem
         printf("AAHHHHHHH..!! FUDEU .!! ");
         sleep(3000);
         gotoxy(50,14); //limpando
         printf("                        ");
         sleep(1000);
         
         gotoxy(10,8); //INIMIGO
         printf("HA HA HA HA HA HA HA HA ..!!");
         sleep(3000);
         gotoxy(10,8); //LIMPANDO LEGENDA
         printf("                            ");
         sleep(1000);
         
         gotoxy(50,14); //personagem
         printf("........................ ");
         sleep(3000);
         gotoxy(50,14); //limpando
         printf("                        ");
         sleep(1000);
         
          gotoxy(10,8); //INIMIGO
         printf("MORRA TROUXA,PISAO TERREMOTO ..!! AAAHH..!!");
         sleep(3000);
         gotoxy(10,8); //LIMPANDO LEGENDA
         printf("                                           ");
         sleep(1000);
          
         //PISAO DO INIMIGO
          //INIMIGO CRECENDO COM PODER
          textcolor(LIGHTBLUE);
          gotoxy(10,10);
          printf("      --        ");
          gotoxy(10,11);
          printf("     ( 0 ) ");
          textcolor(WHITE);
          gotoxy(10,12);
          printf("      ||        ");
          gotoxy(10,13);
          printf("   /  ||   \\    ");
          gotoxy(10,14);
          printf(" /    ||    \\   ");
          gotoxy(10,15);
          printf("      ||  //   ");
          gotoxy(10,16);    
          printf("      || //  \\  ");
          gotoxy(10,17);
          printf("    /         \\  ");
          gotoxy(10,18);
          printf("  /            \\ ");
          gotoxy(10,19);
          printf(" /              \\  ");
          gotoxy(10,20);
          printf("/                   ");
          sleep(2000);
          
          //VOLTANDO AO NORMAL
          
          //INIMIGO CRECENDO COM PODER
          textcolor(LIGHTBLUE);
          gotoxy(10,10);
          printf("      --        ");
          gotoxy(10,11);
          printf("     ( 0 ) ");
          textcolor(WHITE);
          gotoxy(10,12);
          printf("      ||        ");
          gotoxy(10,13);
          printf("   /  ||   \\    ");
          gotoxy(10,14);
          printf(" /    ||    \\   ");
          gotoxy(10,15);
          printf("      ||         ");
          gotoxy(10,16);
          printf("      ||          ");
          gotoxy(10,17);
          printf("    /    \\       ");
          gotoxy(10,18);
          printf("  /        \\      ");
          gotoxy(10,19);
          printf(" /           \\    ");
          gotoxy(10,20);
          printf("/             \\   ");  
          
          //TIRO DO ATAQUE TERREMOTO
           for(tiro=2;tiro<80;tiro++)
           {          
             textcolor(YELLOW);                  
             gotoxy(tiro,21);
             printf("%c",219); 
           }  
           sleep(100);
           
           for(tiro=2;tiro<80;tiro++)
           {     
             textcolor(BLUE);                  
             gotoxy(tiro,21);
             printf("%c",219); 
           }  
           sleep(100);
             
           for(tiro=2;tiro<80;tiro++)
           {       
             textcolor(DARKGRAY);                  
             gotoxy(tiro,21);
             printf("%c",219);
           }   
          
          
          //PERSONAGEM DEFENDENDO GOLPE
           //PERSONAGEM
            textcolor(YELLOW);
            gotoxy(65+1,17);
            printf(" ");
            gotoxy(65+2,17);
            printf("%c",2);//cabeça
            textcolor(WHITE);
            gotoxy(65,18);
            printf("%s","I=|");
            gotoxy(62,18);
            printf("   ");
            gotoxy(65,19);
            printf("%s","I |");
            gotoxy(65,20);
            printf("%s","I/ \\");
            sleep(1000); 
            
            gotoxy(10,8); //INIMIGO
            printf("O QUE VOCE DEFENDEU ?");
            sleep(3000);
            gotoxy(10,8); //LIMPANDO LEGENDA
            printf("                            ");
            sleep(1000);
         
             gotoxy(35,14); //personagem
             printf("ISSO,E EU TAMBEM TENHO UMA ARMA SECRETA ");
             sleep(3000);
             gotoxy(35,14); //limpando
             printf("                                        ");
             sleep(1000);
             
             gotoxy(35,14); //personagem
             printf("\" CANHAO LASER\",FEITO POR ANOS DE ESTUDOS");
             sleep(3000);
             gotoxy(35,14); //limpando
             printf("                                           ");
             sleep(1000);
             
                      
             //PERSONAGEM COM ARMA NOVA
             textcolor(YELLOW);
             gotoxy(65+2,17);
             printf("  ");//cabeça
             gotoxy(65+1,17);
             printf("%c",2);//cabeça
             textcolor(WHITE);
             gotoxy(65-4,18);
             printf("%s","*)OOOO ");
             gotoxy(65,19);
             printf("%s"," | ");
             gotoxy(65,20);
             printf("%s","/ \\"); 
             gotoxy(68,20);
             printf("  ");
             sleep(2000);
             
             gotoxy(10,8); //INIMIGO
             printf("VAMOS VER ENTAO QUEM VENCE AAHHH");
             sleep(3000);
             gotoxy(10,8); //LIMPANDO LEGENDA
             printf("                                "); 
             sleep(1000);
         
             gotoxy(35,14); //personagem
             printf("DEMOROU VAMOS LOGO AAAAAHH.! ");
             sleep(3000);
             gotoxy(35,14); //limpando
             printf("                             ");
             sleep(1000);                            
          //FIM GRAFICO///////////////////////////
    //  } 
         
    //  if(INIX==10)
    //  {   
      
         
         //PERSONAGEM
             textcolor(YELLOW);
             gotoxy(65+1,17);
             printf("%c",2);//cabeça
             textcolor(WHITE);
             gotoxy(65,18);
             printf("%s","/|\\");
             gotoxy(62,18);
             printf("   ");
             gotoxy(65,19);
             printf("%s"," | ");
             gotoxy(65,20);
             printf("%s","/ \\"); 
             sleep(2000);  
         
         //INIMIGO CRECENDO COM PODER
          textcolor(LIGHTBLUE);
          gotoxy(10,10);
          printf("      --        ");
          gotoxy(10,11);
          printf("     ( 0 ) ");
          textcolor(WHITE);
          gotoxy(10,12);
          printf("      ||        ");
          gotoxy(10,13);
          printf("   /  ||   \\    ");
          gotoxy(10,14);
          printf(" /    ||    \\   ");
          gotoxy(10,15);
          printf("      ||         ");
          gotoxy(10,16);
          printf("      ||          ");
          gotoxy(10,17);
          printf("    /    \\       ");
          gotoxy(10,18);
          printf("  /        \\      ");
          gotoxy(10,19);
          printf(" /           \\    ");
          gotoxy(10,20);
          printf("/             \\   ");
          
          textcolor(WHITE);
          //NOTEBOOK
          gotoxy(2,20);
          printf(" \\__");
     // }        
        
      x=65;
      l=20;
      CHEFAOX=5;
      vida=VIDAS;
      //CONTROLES////////////////////////////////////////
      do
      {
          if(conttiro==13)
          {
           aux++;               
           conttiro=1;
          }
          if(aux==10)
          aux=1;
           
          conttiro++;
            
          if(kbhit())                                               
          tecla=getch();                                               
          
          
      //ATAQUE DO CHEFAO////////////////////////////////////////////////
          //PISAO DO INIMIGO
      //INIMIGO CRECENDO COM PODER
      if(aux==3 && CHEFAOX>0)
      {
          //LEGENDA ATAQUE
          gotoxy(10,8); //INIMIGO
          printf("ATAQUE TERREMOTO AAHH ..!!");        
                     
          textcolor(LIGHTBLUE);
          gotoxy(10,10);
          printf("      --        ");
          gotoxy(10,11);
          printf("     ( 0 ) ");
          textcolor(WHITE);
          gotoxy(10,12);
          printf("      ||        ");
          gotoxy(10,13);
          printf("   /  ||   \\    ");
          gotoxy(10,14);
          printf(" /    ||    \\   ");
          gotoxy(10,15);
          printf("      ||  //   ");
          gotoxy(10,16);    
          printf("      || //  \\  ");
          gotoxy(10,17);
          printf("    /         \\  ");
          gotoxy(10,18);
          printf("  /            \\ ");
          gotoxy(10,19);
          printf(" /              \\  ");
          gotoxy(10,20);
          printf("/                   ");
          sleep(200);
      }
      //VOLTANDO AO NORMAL
      
      if(aux==8 && CHEFAOX>0)
      {           
                     
          //INIMIGO CRECENDO COM PODER
          textcolor(LIGHTBLUE);
          gotoxy(10,10);
          printf("      --        ");
          gotoxy(10,11);
          printf("     ( 0 ) ");
          textcolor(WHITE);
          gotoxy(10,12);
          printf("      ||        ");
          gotoxy(10,13);
          printf("   /  ||   \\    ");
          gotoxy(10,14);
          printf(" /    ||    \\   ");
          gotoxy(10,15);
          printf("      ||         ");
          gotoxy(10,16);
          printf("      ||          ");
          gotoxy(10,17);
          printf("    /    \\       ");
          gotoxy(10,18);
          printf("  /        \\      ");
          gotoxy(10,19);
          printf(" /           \\    ");
          gotoxy(10,20);
          printf("/             \\   ");
          
          //TIRO DO ATAQUE TERREMOTO
           for(tiro=2;tiro<80;tiro++)
           {          
             textcolor(YELLOW);                  
             gotoxy(tiro,21);
             printf("%c",219); 
           }  
           sleep(100);
           
           for(tiro=2;tiro<80;tiro++)
           {     
             textcolor(BLUE);                  
             gotoxy(tiro,21);
             printf("%c",219); 
           }  
           sleep(100);
             
           for(tiro=2;tiro<80;tiro++)
           {       
             textcolor(DARKGRAY);                  
             gotoxy(tiro,21);
             printf("%c",219);
           }  
          
          //SE NAO DEFENDER NO TERREMOTO MORRE
          if(DEFESAX==0)
          {
            VIDAS--;
            INIX=1;  
          }     
          gotoxy(10,8); //LIMPANDO LEGENDA ATAQUE
          printf("                                "); 
          sleep(500);
      }    
      
      //FIM ATAQUE DO CHEFAO/////////////////////////////////////////
          
          
          //BAIXO
           if(tecla==115)
           {
            DEFESAX=0;             
            x=CONTROLES(tecla,x,l,1,0,0,0);
            }      
           //TRAS
           if(tecla==97 && M[l][x-1]==0)
           {
            DEFESAX=0;            
            x=CONTROLES(tecla,x,l,1,0,0,0);//ultimo numero 0 retorna x
            LADOTIRO=0;//lado que o personagem esta virado para saber para onde vai atirar
           }  
           //FRENTE
           if(tecla==100 && M[l][x+4]==0)
           {
            DEFESAX=0;             
            x=CONTROLES(tecla,x,l,1,0,0,0);//retorna x
            LADOTIRO=1;//lado que o personagem esta virado para saber para onde vai atirar
            }
            //TIRO
           if(tecla==32 && ARMAX==1)
           {
             DEFESAX=0;            
             if(LADOTIRO==0)
             CONTROLES(tecla,x,l,1,0,0,4);//retorna x 
             if(LADOTIRO==1)
             CONTROLES(tecla,x,l,1,0,0,4);//retorna x
           } 
           //ESCUDO  
           if(tecla==102 && ESCUDOX==1)
           {
             DEFESAX=1;            
             CONTROLES(tecla,x,l,1,0,0,0);//retorna x  
           } 
           //FIM CONTROLES  ////////////////////////////////
           tecla=0;
           
           if(vida>VIDAS)//se levou ataque retorna a fase
           {
              fase=8;
              fim=1;  
              
               clrscr();
               beep(1350,100);
               textcolor(YELLOW);//personagem
               gotoxy(35,14);
               printf("%c",2);//cabeça
               textcolor(WHITE);
               gotoxy(34,15);
               printf("%s","/|\\ ");
               gotoxy(34,16);
               printf("%s"," | ");
               gotoxy(34,17);
               printf("%s","/ \\ ");
               sleep(300);
              
               gotoxy(38,16); //mostrando numero de vidas
               printf("VIDAS x%d",VIDAS);
              
               textcolor(YELLOW);
               gotoxy(22,20); //DICA
               printf("DICA: USE O ESCUDO PARA DEFENDER O ATAQUE.!");
               sleep(5000);           
           }
           
           if(CHEFAOX<=0 && tiro>1)
           {
              //LEGENDA ATAQUE
              gotoxy(10,8); //INIMIGO
              printf("AAAHHHH ,FUI DERROTADO ..!!");
              sleep(1500);   
              tiro=-1;        
              
              //LIMPANDO MOSTRAO
              textcolor(LIGHTBLUE);
              gotoxy(10,10);
              printf("                ");
              gotoxy(10,11);
              printf("           ");
              textcolor(WHITE);
              gotoxy(10,12);
              printf("                ");
              gotoxy(10,13);
              printf("                 ");
              gotoxy(10,14);
              printf("                 ");
              gotoxy(10,15);
              printf("               ");
              gotoxy(10,16);    
              printf("                 ");
              gotoxy(10,17);
              printf("                  ");
              gotoxy(10,18);
              printf("                  ");
              gotoxy(10,19);
              printf("                   ");
              gotoxy(10,20);
              printf("                    ");
        
          
          
              //LEGENDA ATAQUE
              gotoxy(10,8); //INIMIGO
              printf("                            ");           
             //INIMIGO MORTO
              textcolor(LIGHTBLUE);
              gotoxy(10+4,20);
              printf("%s","0");//cabeça
              textcolor(WHITE);
              gotoxy(10,20);
              printf("%s","^-- ");//corpo deitado  
                          
           }
           
           if(x<4)//TERMINOU JOGO
           {
            fase++;
            fim=1;   
            
             beep(1350,100);  //PERSONAGEM SEGURANDO NOTEBOOK
             gotoxy(x,16);
             printf("\\__");//notebook           
             textcolor(YELLOW);
             gotoxy(x,17);
             printf("\\%c/",2);//cabeça
             textcolor(WHITE);
             gotoxy(x,18);
             printf("%s"," |  MEU NOTEBOOK ATEH QUE ENFIM,AGORA VOU PARA CASA");
             gotoxy(x,19);
             printf("%s"," | ");
             gotoxy(x,20);
             printf("%s","/ \\"); 
             sleep(3500);          
           }
           
     }while(fim==0);       
      
     return fase;
}

void CENARIOINTRO()
{
  int c,c2,l,l2;
  //DESENHANDO CENARIO NOVAMENTE
             //COQUEIRO
              textcolor(GREEN);
              gotoxy(71,19);
              printf("     |");
              gotoxy(73,20);
              printf(" \\\\|//");
              textbackground(BLACK);
              textcolor(BROWN);
              gotoxy(71,18);
              printf("    o0o");
              gotoxy(73,21);
              printf("%c",219);
              gotoxy(74,21);
              printf("%c",219);
              gotoxy(75,21);
              printf("%c",219);
              gotoxy(76,21);
              printf("%c",219);
              gotoxy(77,21);
              printf("%c",219);
              gotoxy(78,21);
              printf("%c",219);
              gotoxy(79,21);
              printf("%c",219);
              
              //DESENHANDO PRAIA
              for(c=2;c<80;c++)
              {
               textbackground(LIGHTBLUE);   
               textcolor(LIGHTBLUE);                                 
               gotoxy(c,22); 
               printf("%c",219);   
               gotoxy(c,23); 
               printf("%c",219); 
               gotoxy(c,24); 
               printf("%c",219);             
              }        
              textbackground(BLACK);   
               textcolor(GREEN);  
               //DESENHANDO CAMINHO
              for(c=2;c<80;c++)
              {                                 
               gotoxy(c,8); 
               printf("%c",219);   
               gotoxy(c,8); 
               printf("%c",219); 
               gotoxy(c,8); 
               printf("%c",219);             
              }    
              
              textcolor(RED);
              for(c=1,c2=1;c<81;c++,c2++)
              {
                   sleep(10); 
                   //CIMA
                   gotoxy(c,1);
                   printf("%c",219);
                   
                   //BAIXO
                   gotoxy(c2,25); 
                   printf("%c",219);
               }
               
              for(l=1,l2=1;l<25;l++,l2++)
              {
                   //ESQUERDA                          
                   c=1;
                   c2=80;
                   sleep(10); 
                   gotoxy(c,l);
                   printf("%c",219);
                   
                   //DIREITA
                   gotoxy(c2,l2);
                   printf("%c",219);
               }
               textbackground(BLACK);   
               textcolor(WHITE);
}

int TIROINIMIGO(int tecla,int x,int l,int conttiro,int qual)
{
     int tiro;
     
     if(qual==1)
       tiro=18;
     if(qual==2)
       tiro=61;
     
    //TIRO PARA DIREITA
    conttiro++;   //ESPERA PARA PROXIMO TIRO
    if(conttiro==10)
    conttiro=1;
        
    if(qual==1)
    {
       if(15+50>x && l==6 && INIX==0)//inimigo da sinal que vai atirar
           {
                textcolor(RED);
                gotoxy(15+1,3);
                printf("%c",2);//cabeça
                textcolor(WHITE);
                
                gotoxy(15,5);
                printf("%s"," | ");
                gotoxy(15,6);
                printf("%s","/ \\ ");
                sleep(100);   
                
                //mensagens ao levar tiro
               if(VIDASINI1==3)
               { 
                 gotoxy(15,4);
                 printf("%s","/|\\ PARADO AE!");            
               } 
               if(VIDASINI1==2)
               { 
                 gotoxy(15,4);
                 printf("%s","/|\\ NAO ATIRE!");            
               } 
               if(VIDASINI1==1)
               { 
                 gotoxy(15,4);
                 printf("%s","/|\\ VO MORRER!");            
               } 
           }  
           else
             {                          
                  if(INIX==0)
                  { 
                       //INIMIGO 1
                      textcolor(RED);
                      gotoxy(15+1,3);
                      printf("%c",2);//cabeça
                      textcolor(WHITE);
                      gotoxy(15,4);
                      printf("%s","/|\\           ");
                      gotoxy(15,5);
                      printf("%s"," | ");
                      gotoxy(15,6);
                      printf("%s","/ \\ ");                            
                                                  
                     conttiro=1;
                 }                        
             }
           if(x<=64 && l==6 && conttiro==5 && INIX==0)//INIX,é se inimigo ta vivo
           {           
                 //TIRO PARA DIREITA
                    
                    gotoxy(15,3);
                    printf("%s","      ");
                    gotoxy(15,4);
                    printf("%s","      ");
                    gotoxy(15,5);
                    printf("%s","      "); 
                    gotoxy(15,6);
                    printf("%s","      "); 
                   
                    textcolor(RED);
                    gotoxy(15+1,3);
                    printf("%c",2);//cabeça
                    textcolor(WHITE);
                    gotoxy(15,4);
                    printf("%s"," |== ");
                    gotoxy(15,5);
                    printf("%s"," | ");
                    gotoxy(15,6);
                    printf("%s","/ \\ ");
                    sleep(100);      
                   
        
                 do
                 {  
                    tiro++; 
                    gotoxy(tiro,l-2);
                    printf("-");
                    sleep(15); 
                    gotoxy(tiro,l-2);
                    printf("  ");
                    sleep(10); 
                                
                    if(tiro==x && DEFESAX==0)//inimigo acerta personagem e o mata perdendo uma vida
                      VIDAS--;
                     
                    if(tiro+3>x && DEFESAX==1)//defende tiro
                     tiro=70;
                            
                      
                 }while(tiro<70);            
                 if(tiro>69)
                  tiro=18;
                  
                 if(INIX==0)//voltando ao normal apos tiro
                 {
                       
                     gotoxy(15,3);
                     printf("%s","      ");
                     gotoxy(15,4);
                     printf("%s","      ");
                     gotoxy(15,5);
                     printf("%s","      "); 
                     gotoxy(15,6);
                     printf("%s","      "); 
                              
                     textcolor(RED);
                     gotoxy(15+1,l-3);
                     printf("%c",2);//cabeça
                     textcolor(WHITE);
                     gotoxy(15,l-2);
                     printf("%s","/|\\ ");
                     gotoxy(15,l-1);
                     printf("%s"," | ");
                     gotoxy(15,l);
                     printf("%s","/ \\ ");          
                 }
                 
           }
    }      
    else
      if(qual==2)
      {    
           //LADOS QUE ATIRA(TIRO)
             //TIRO PARA ESQUERDA
           if(64-50<x && l==20 && INIX2==0)//inimigo da sinal que vai atirar
           {
                textcolor(RED);
                gotoxy(64+1,17);
                printf("%c",2);//cabeça
                textcolor(WHITE);
                gotoxy(64,18);
                printf("%s","/|\\  PARADO AE!");
                gotoxy(64,19);
                printf("%s"," | ");
                gotoxy(64,20);
                printf("%s","/ \\ ");
                sleep(100);      
                
           }  
           else
             {                          
                  if(INIX2==0)
                  { 
                       //INIMIGO 1
                      textcolor(RED);
                      gotoxy(64+1,17);
                      printf("%c",2);//cabeça
                      textcolor(WHITE);
                      gotoxy(64,18);
                      printf("%s","/|\\          ");
                      gotoxy(64,19);
                      printf("%s"," | ");
                      gotoxy(64,20);
                      printf("%s","/ \\ ");                            
                                                  
                     conttiro=1;
                 }                        
             }
           if(x>=10 && l==20 && conttiro==5 && INIX2==0)//INIX2,é se inimigo ta vivo
           {           
                 //TIRO PARA ESQUERDA
                    
                    gotoxy(64,17);
                    printf("%s","     ");
                    gotoxy(64,18);
                    printf("%s","     ");
                    gotoxy(64,19);
                    printf("%s","     "); 
                    gotoxy(64,20);
                    printf("%s","      "); 
                   
                    textcolor(RED);
                    gotoxy(64+2,17);
                    printf("%c",2);//cabeça
                    textcolor(WHITE);
                    gotoxy(64,18);
                    printf("%s","==|");
                    gotoxy(64,19);
                    printf("%s","  | ");
                    gotoxy(64,20);
                    printf("%s"," / \\");
                    sleep(100);      
                   
                 do
                 {  
                    tiro--; 
                    gotoxy(tiro,20-2);
                    printf("-");
                    sleep(20); 
                    gotoxy(tiro,20-2);
                    printf("  ");
                    sleep(10); 
                                
                    if(tiro==x && DEFESAX==0)//inimigo acerta personagem e o mata perdendo uma vida
                      VIDAS--;
                         
                    if(tiro-5<x && DEFESAX==1)//defende tiro
                     tiro=9;
                            
                 }while(tiro>=10);            
                 if(tiro<=10)
                  tiro=61;
                  
                 if(INIX2==0)//voltando ao normal apos tiro
                 {
                       
                     gotoxy(64,17);
                     printf("%s","      ");
                     gotoxy(64,18);
                     printf("%s","      ");
                     gotoxy(64,19);
                     printf("%s","      "); 
                     gotoxy(64,20);
                     printf("%s","      "); 
                              
                     textcolor(RED);
                     gotoxy(64+1,17);
                     printf("%c",2);//cabeça
                     textcolor(WHITE);
                     gotoxy(64,18);
                     printf("%s","/|\\ ");
                     gotoxy(64,19);
                     printf("%s"," | ");
                     gotoxy(64,20);
                     printf("%s","/ \\ ");          
                 }
                 
           }
      }     
       ///FIM TIROS
  
       return conttiro; 
     
}     

