#include <iostream>
#include <string>
#include <fstream>
#include <allegro.h>
#include "sistema.h"
#include "Cenario.h"
#include "logica.h"


#include "PlayerClass.h"

using namespace std;

#define LEFT       4
#define RIGHT     6
#define UP           8
#define DOWN     2

volatile long spdCounter = 0;
void IncrementCounter();

//int Tempo = 99;
int i = 2;


void incrementa_I();

bool isCollision(const PlayerClass*, const PlayerClass*);

int main()
{
    
    Sistema Jogo;
    Cenario Cachoeira;
    PlayerClass player[2];
    Logica timer;
    
    player[0].setup(0, 440, 128, 128, 0, 0,0);
    player[1].setup(800-128, 440, 128, 400, 0, 1,1);
    
    BITMAP *Buffer = create_bitmap(640,480);
    BITMAP *background = load_bitmap("background.bmp", NULL);

    BITMAP *Maga = load_bitmap("hulk.bmp",NULL);
    BITMAP *Fighter = load_bitmap("shuma.bmp",NULL);
    
     DATAFILE  *datFontes = NULL;
                datFontes = load_datafile("fontes.dat");
    
    int win = -1;     
    
    
    
    LOCK_VARIABLE( spdCounter );
    LOCK_FUNCTION( IncrementCounter );
    install_int_ex( IncrementCounter, BPS_TO_TIMER( 90 ) );
    
    //LOCK_VARIABLE( );
    //LOCK_FUNCTION( );
    //install_int_ex( timer.Conta_Tempo, BPS_TO_TIMER( 1 ) );
    
     LOCK_VARIABLE( i );
    LOCK_FUNCTION( incrementa_I );
    install_int_ex( incrementa_I, BPS_TO_TIMER( 7 ) );



    while ( !Jogo.Fim() ){
 
 
             
  
 

          
        while ( spdCounter > 0 ){
            
            if ( key[KEY_ESC] || key[KEY_F4] ) { Jogo.Sair(); }
            if ( key[KEY_F5] ) { Jogo.Mudar_Tela(); }
            
            //movement - player 1
            if ( key[KEY_A] )
            {
                player[0].move(LEFT);
            }
            else if ( key[KEY_D] )
            {
                player[0].move(RIGHT);
            }
            if ( !key[KEY_A] && !key[KEY_D] )
            {
                player[0].noMove();
            }
            if ( key[KEY_Y] )
            {
                player[0].attack();
            }
            if ( key[KEY_W] )
            {
                player[0].jump();
            }
            //movement - player 2
            if ( key[KEY_LEFT] )
            {
                player[1].move(LEFT);
            }
            else if ( key[KEY_RIGHT] )
            {
                player[1].move(RIGHT);
            }
            if ( !key[KEY_LEFT] && !key[KEY_RIGHT] )
            {
                player[1].noMove();
            }
            if ( key[KEY_7_PAD] )
            {
                player[1].attack();
            }
            if ( key[KEY_UP] )
            {
                player[1].jump();
            }

            if (isCollision(&player[0], &player[1]))
            {
                if ( player[0].isAttacking && player[1].invincible == false )
                {
                    player[1].hit();
                }
                if ( player[1].isAttacking && player[0].invincible == false )
                {
                    player[0].hit();
                }
                if ( player[0].HP <= 0 )
                    win = 1;
                else if ( player[1].HP <= 0 )
                    win = 0;
                  
            }

            player[0].update();
            player[1].update();
            spdCounter --;
           
}
        vsync();
        acquire_screen();
              
 if ( win == -1 )
        {
            /*stretch_sprite(Buffer, background, 0, 0, 640, 480);
            */

     Cachoeira.Desenhar(Buffer,Cachoeira.Imagem[0],0,0);
     Cachoeira.Desenhar(Buffer,Cachoeira.Imagem[i],0,180);
     Cachoeira.Desenhar(Buffer,Cachoeira.Imagem[1],0,318);
     
     //     textprintf_centre_ex(Buffer, font, SCREEN_W/2, 0, makecol(54,54,54),-1,"Tempo:");
     //textprintf_centre_ex(Buffer, (FONT *)datFontes[0].dat, SCREEN_W/2, 10, makecol(54,54,54),-1,"%d",Tempo);
     timer.desenhar(Buffer);
     
            //draw_sprite(Buffer, background, 0, 0);
           /* textprintf_ex(Buffer,font,0-1,30-1,makecol(0, 0, 0),-1, "Player 1 - WASD to move     Space to jump     F to attack");
            textprintf_ex(Buffer,font,0+1,30-1,makecol(0, 0, 0),-1, "Player 1 - WASD to move     Space to jump     F to attack");
            textprintf_ex(Buffer,font,0+1,30+1,makecol(0, 0, 0),-1, "Player 1 - WASD to move     Space to jump     F to attack");
            textprintf_ex(Buffer,font,0-1,30+1,makecol(0, 0, 0),-1, "Player 1 - WASD to move     Space to jump     F to attack");
            textprintf_ex(Buffer,font,0-1,40-1,makecol(0, 0, 0),-1, "Player 2 - Arrow keys to move     Shift to jump     Ctrl to attack");
            textprintf_ex(Buffer,font,0+1,40-1,makecol(0, 0, 0),-1, "Player 2 - Arrow keys to move     Shift to jump     Ctrl to attack");
            textprintf_ex(Buffer,font,0-1,40+1,makecol(0, 0, 0),-1, "Player 2 - Arrow keys to move     Shift to jump     Ctrl to attack");
            textprintf_ex(Buffer,font,0+1,40+1,makecol(0, 0, 0),-1, "Player 2 - Arrow keys to move     Shift to jump     Ctrl to attack");
            textprintf_ex(Buffer,font,0,30,makecol(255,50,200),-1, "Player 1 - WASD to move     Space to jump     F to attack");
            textprintf_ex(Buffer,font,0,40,makecol(150,150,255),-1, "Player 2 - Arrow keys to move     Shift to jump     Ctrl to attack");
          
            textout_centre_ex(Buffer, font, "C Fighter!  Demo", SCR_W -60, 120,makecol(255,255,255),-1);
            textout_centre_ex(Buffer, font, "Wagner G de Jesus", SCR_W -60, 60,makecol(255,255,255),-1);
  */
           



  //VIDA

            
            player[0].draw(Buffer,Maga); 
            player[1].draw(Buffer,Fighter);
  
  rectfill(Buffer, 30, 6, player[0].HP, 15, makecol(255,255,0));
  rectfill(Buffer, 610, 6, SCREEN_W-player[1].HP, 15, makecol(255,255,0));         
            

        }
        else if ( win == 0 )
        {
            textout_centre_ex(Buffer, font, "P l a y e r  0  w i n s ", 640/2, 480/2, makecol(255,100,100),-1);
        }
        else if ( win == 1 )
        {
            textout_centre_ex(Buffer, font, "P l a y e r  1  w i n s ", SCREEN_W/2, SCREEN_H/2, makecol(100,100,255),-1);
        }
 
 textprintf_right_ex(Buffer, font,SCREEN_W, 450, makecol(255,0,0), -1,"FPS: %d",spdCounter);
 
        blit(Buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        clear_bitmap(Buffer);
        release_screen();
    }

    return 0;
}
END_OF_MAIN();
void IncrementCounter()
{
    spdCounter++;
}
bool isCollision( const PlayerClass *player1, const PlayerClass *player2 )
{
    if ( (player1->x + player1->w > player2->x && player1->x < player2->x + player2->w) &&
            (player1->y + player1->h > player2->y && player1->y < player2->y + player2->h))
        return true;

    return false;
}

void incrementa_I()
{
     i++;
     if (i>=5)
     {
                  i = 2;
                  
     }
 }
