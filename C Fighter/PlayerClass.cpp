#include "PlayerClass.h"

#define STAND    0
#define RUN         1
#define ATTACK  2
#define LEFT       4
#define RIGHT     6
#define UP           8
#define DOWN     2


PlayerClass::PlayerClass()
{
     DATAFILE *Imagens = NULL;
              Imagens = load_datafile("Personagens/spider.dat");
              
 parado[0] = (BITMAP *)Imagens[0].dat;                                                 
 parado[1] = (BITMAP *)Imagens[1].dat; 
 parado[2] = (BITMAP *)Imagens[2].dat; 
 parado[3] = (BITMAP *)Imagens[3].dat; 
 parado[4] = (BITMAP *)Imagens[4].dat; 
 parado[5] = (BITMAP *)Imagens[5].dat; 
 parado[6] = (BITMAP *)Imagens[6].dat; 
 parado[7] = (BITMAP *)Imagens[7].dat; 
 parado[8] = (BITMAP *)Imagens[8].dat; 


 //carrega as figuras para fazer a animação andando                                        
 andando[0] = (BITMAP *)Imagens[9].dat;                                                 
 andando[1] = (BITMAP *)Imagens[10].dat;  
 andando[2] = (BITMAP *)Imagens[11].dat; 
 andando[3] = (BITMAP *)Imagens[12].dat;  
 andando[4] = (BITMAP *)Imagens[13].dat;  
 andando[5] = (BITMAP *)Imagens[14].dat;  
 andando[6] = (BITMAP *)Imagens[15].dat;  
 
 t_teia[0] = (BITMAP *)Imagens[16].dat;
 t_teia[1] = (BITMAP *)Imagens[17].dat;
 t_teia[2] = (BITMAP *)Imagens[18].dat;
 t_teia[3] = (BITMAP *)Imagens[19].dat;
 t_teia[4] = (BITMAP *)Imagens[20].dat;
 t_teia[5] = (BITMAP *)Imagens[21].dat;
 t_teia[6] = (BITMAP *)Imagens[22].dat;
 t_teia[7] = (BITMAP *)Imagens[23].dat;
 t_teia[8] = (BITMAP *)Imagens[24].dat;
 t_teia[9] = (BITMAP *)Imagens[25].dat;

 
 pulando[0] = (BITMAP *)Imagens[26].dat;
 pulando[1] = (BITMAP *)Imagens[27].dat;
 pulando[2] = (BITMAP *)Imagens[28].dat;
 pulando[3] = (BITMAP *)Imagens[29].dat;
 pulando[4] = (BITMAP *)Imagens[30].dat;
 
                          }
void PlayerClass::update()
{
    if ( hitFrame > 0 )
    {
        hitFrame -= 0.2;
    }
    else
    {
        invincible = false;
    }
    if ( isAttacking )
    {
        if ( attackFrame > 4 )
        {
            attackFrame = 0.0;
            isAttacking = false;
        }
        else
        {
            attackFrame += frameSpeed/2;
        }
    }
    else if ( isJumping )
    {
//        if ( jumpFrame > 10 || y < LowestY )
        if ( jumpFrame > maxJump )
        {
            jumpFrame = 0;
            isJumping = false;
            y = LowestY;
        }
        else
        {
            if ( jumpFrame < maxJump/2 )
            {
                y -= speed*2;
            }
            else
            {
                if ( y > LowestY )
                {
                    y = LowestY;
                    jumpFrame = 0;
                    isJumping = false;
                }
                y += speed*2;
            }
            jumpFrame += frameSpeed;
        }
    }
}

void PlayerClass::attack()
{
    if ( !isJumping )
    {
        if ( !isAttacking )
        {
            attackFrame = 0;
            isAttacking = true;
        }
        else if ( isAttacking )
        {
            if ( attackFrame > 4 )
            {
                attackFrame = 0;
                isAttacking = false;
            }
            else
            {
                attackFrame += frameSpeed/2;
            }
        }
    }
}

void PlayerClass::jump()
{
    if ( !isAttacking )
    {
        if ( !isJumping )
        {
            jumpFrame = 0;
            isJumping = true;
        }
        else if ( isJumping )
        {
//            if ( jumpFrame > 10 || y < LowestY )
            if ( jumpFrame > maxJump )
            {
                jumpFrame = 0;
                isJumping = false;
                y = LowestY;
            }
            else
            {
                jumpFrame += frameSpeed;
            }
        }
    }
}

void PlayerClass::move(int direction)
{
    if ( !isAttacking )
    {
        if ( direction == LEFT )
        {
            if ( x - speed > 0 )
            {
                x -= speed;
                runFrame += frameSpeed;
                action = RUN;
            }
        }
        else if ( direction == RIGHT )
        {
            if ( x + speed < 640-w )
            {
                x += speed;
                runFrame += frameSpeed;
                action = RUN;
            }
        }
        if ( runFrame > 4 )
            runFrame = 0;
    }
}

void PlayerClass::setup(float tx, float ty, int tw, int th, int tact, int tcode ,int jog)
{
    if ( tcode == 1 )
        x = SCR_W - 64 - tw;
    else if ( tcode == 0 )
        x = 64;
    y = LowestY;
    w = tw;
    h = th;
    action = tact;
    dir = jog;
    spritecode = 0;
    runFrame = attackFrame = 0;
    speed = 2.0;
    frameSpeed = 0.1;
    isAttacking = isJumping = false;
    HP = 180;
    invincible = false;
    hitFrame = 0;
}

void PlayerClass::hit()
{
    if ( !invincible )
    {
        invincible = true;
        hitFrame = invincibilityLength;
        HP -= 30;
    }
}

void PlayerClass::draw(BITMAP *buffer, BITMAP *image)
{   
    BITMAP *temp;
    BITMAP *barra[2];
    
            barra[0] = load_bitmap("hulk_barra.bmp",NULL);           
            masked_blit(barra[0], buffer, 0, 0, 0, 0, 640, 480);
            barra[1] = load_bitmap("shuma_barra.bmp",NULL);
    BITMAP *dano[25];
           dano[0] = load_bitmap("dano.bmp",NULL);
           
            masked_blit(barra[0], buffer, 0, 0, 0, 0, 640, 480);
            masked_blit(barra[1], buffer, 0, 0, 460, 0, 640, 480);
    temp = image;
 
             
    if ( isAttacking )
        masked_blit(image, buffer, (int)attackFrame*w+640, spritecode*w, x, y, w, h);
    else if ( isJumping )
        masked_blit(image, buffer, 1152, spritecode*w, x, y, w, h);
    else if ( action == STAND &&(dir= 1)){
        
      
     draw_sprite_h_flip(image, temp, 0, 0);
     masked_blit(image, buffer, spritecode*w, spritecode*w, x, y, w, h);
     
    if ( dir == 0 )
          
          masked_blit(image, buffer, spritecode*w, spritecode*w, x, y, w, h);
          
 

}
   
    else if ( action == RUN )
        masked_blit(image, buffer, (int)runFrame*w+w, spritecode*w, x, y, w, h);
    if ( hitFrame > 0 )
    {

        masked_blit(dano[0], buffer, 0, 0,  x+w/10, y+h/9, 640, 480);
    }
    
}
