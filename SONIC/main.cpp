#include <allegro.h>
#include "sonic.h"

#define SPIKEYENT(NUM,X,Y) SPRITE[NUM].type = SPIKEY; SPRITE[NUM].originx = X; SPRITE[NUM].originy = Y; SPRITE[NUM].x = SPRITE[NUM].originx; SPRITE[NUM].y = SPRITE[NUM].originy;
#define GOOMBAENT(NUM,X,Y) SPRITE[NUM].type = GOOMBA; SPRITE[NUM].originx = X; SPRITE[NUM].originy = Y; SPRITE[NUM].x = SPRITE[NUM].originx; SPRITE[NUM].y = SPRITE[NUM].originy;
#define SWOOPERENT(NUM,X,Y) SPRITE[NUM].type = SWOOPER; SPRITE[NUM].originx = X; SPRITE[NUM].originy = Y; SPRITE[NUM].x = SPRITE[NUM].originx; SPRITE[NUM].y = SPRITE[NUM].originy;         
#define PLATFORM_HORI(NUM,X,Y,DIST) SPRITE[NUM].type=PLATFORM; SPRITE[NUM].x=X; SPRITE[NUM].y=Y; SPRITE[NUM].width=100; SPRITE[NUM].height=10; SPRITE[NUM].platDistance=DIST; SPRITE[NUM].xMovement = 2;
#define PLATFORM_VERT(NUM,X,Y,DIST) SPRITE[NUM].type=PLATFORM; SPRITE[NUM].x=X; SPRITE[NUM].y=Y; SPRITE[NUM].width=100; SPRITE[NUM].height=10; SPRITE[NUM].platDistance=DIST; SPRITE[NUM].yMovement = 2;
#define PLATFORM_DIAG(NUM,X,Y,DIST) SPRITE[NUM].type=PLATFORM; SPRITE[NUM].x=X; SPRITE[NUM].y=Y; SPRITE[NUM].width=100; SPRITE[NUM].height=10; SPRITE[NUM].platDistance=(DIST *2); SPRITE[NUM].xMovement = 2;SPRITE[NUM].yMovement = 2;

void init();
void deinit();

const int GRAVITY = 6;
const int JUMPTIME = 20;
const int JUMPPEAK = 15;
const int JUMPPOWER = 15;

const int HOPTIME = 15;
const int HOPPEAK = 11;
const int HOPPOWER = 10;


int main() {
	init();
	
	initializeSprites();
	
	
	// LOAD BITMAPS
    buffer = create_bitmap(6000,480);
    ground = load_bitmap("ground.bmp", NULL);
    level1 = load_bitmap("Level1.bmp", NULL);
    level2 = load_bitmap("level2.bmp", NULL);
    BITMAP *temp;
    temp = load_bitmap("sonicsprite.bmp",NULL);
    for(int i = 0; i < PLAYERWALKFRAMES; i++){
        sonicwalkRight[i] = grabframe(temp,40,40,0,0,PLAYERWALKFRAMES,i);
        sonicwalkLeft[i] = grabframe(temp,40,40,0,40,PLAYERWALKFRAMES,i);
    }
    
    for(int i = 0; i < PLAYERJUMPFRAMES; i++){
            sonicjumpRight[i] = grabframe(temp,40,40,0,80,PLAYERJUMPFRAMES,i);
            sonicjumpLeft[i] = grabframe(temp,40,40,0,120,PLAYERJUMPFRAMES,i);
    }
    
    sonicstandRight = grabframe(temp,40,40,0,160,1,0);
    sonicstandLeft = grabframe(temp,40,40,40,160,1,0);
    upright = grabframe(temp,40,40,0,320,1,0);
    upleft = grabframe(temp,40,40,40,320,1,0);
    duckright = grabframe(temp,40,40,0,280,1,0);
    duckleft = grabframe(temp,40,40,40,280,1,0);
    
    for(int i = 0; i < PLAYERIDLEFRAMES; i++){
            sonicidleRight[i] = grabframe(temp,40,40,0,200,PLAYERIDLEFRAMES,i);
            sonicidleLeft[i] = grabframe(temp,40,40,0,240,PLAYERIDLEFRAMES,i);
    }
    
    temp = load_bitmap("spikey.bmp", NULL);
    for(int i = 0; i < SPIKEYWALKFRAMES; i++){
            spikeywalkRight[i] = grabframe(temp,40,40,0,0,SPIKEYWALKFRAMES,i);
            spikeywalkLeft[i] = grabframe(temp,40,40,0,40,SPIKEYWALKFRAMES,i);
    }
    
    temp = load_bitmap("ring.bmp", NULL);
    for(int i = 0; i < RINGFRAMES; i++)
            ring[i] = grabframe(temp,16,16,0,0,RINGFRAMES,i);
            
    temp = load_bitmap("bonk.bmp", NULL);
    for(int i = 0; i < BONKFRAMES; i++){
            goombawalkLeft[i] = grabframe(temp,25,32,0,0,BONKFRAMES,i);
            goombawalkRight[i] = grabframe(temp,25,32,0,32,BONKFRAMES,i);
    }
    
    temp = load_bitmap("titlesprite.bmp", NULL);
    for(int i = 0; i < TITLEFRAMES; i++)
            titlesprite[i] = grabframe(temp,640,480,0,0,TITLEFRAMES,i);
            
    temp = load_bitmap("swooper.bmp",NULL);
    for(int i = 0; i < SWOOPERFRAMES; i++)
            swoopersprite[i] = grabframe(temp,40,14,0,0,SWOOPERFRAMES,i);
            
    teamscreen = load_bitmap("teamscreen.bmp",NULL);
            
    // LOAD AUDIO
    titlemusic = load_midi("title.mid");
    level1music = load_midi("level1bgm.mid");
    level2music = load_midi("level2bgm.mid");
    jumpsound = load_sample("jump.wav");
    killenemy = load_sample("killenemy.wav");
    powerup = load_sample("ring.wav");
    ringthrow = load_sample("ringthrow.wav");
    teamsound = load_sample("titlesound.wav");
    
    play_sample(teamsound,255,255,1000,0);
    int teamcount = 0;
    while(teamcount < 4000){
                    stretch_sprite(screen, teamscreen,0,0,SCREEN_W,SCREEN_H);
                    teamcount++;
    }
    
    initializeSprites();        
    // do title screen!!!!
    SPRITE[1].maxframe = TITLEFRAMES;
    SPRITE[1].framedelay = 8;
    SPRITE[1].height = 480;
    SPRITE[1].width = 640;
    play_midi(titlemusic,0);
    while(!key[KEY_SPACE]){
        SPRITE[1].currentFrame = titlesprite[SPRITE[1].curframe];
        stretch_sprite(buffer,SPRITE[1].currentFrame,0,0,640,480);
        updateFrame(1);
        acquire_screen();
    	blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    	rest(10);
        release_screen();
    }
    stop_midi();
    
    
    // START THE GAME!!!!
    initializeSprites();
    SPRITE[PLAYER].poweredUp = false;
    SPRITE[PLAYER].level = 1;
    setupSprites();
	while (!key[KEY_ESC]) {
        playerMiddlex = camerax + 270; 
        
        // for all sprites
        for(int i = 0; i < 50; i++){
                
                // check gravity
                if(SPRITE[i].type == PLAYER || SPRITE[i].type == GOOMBA || SPRITE[i].type == SPIKEY || SPRITE[i].type == RING) 
                                  checkGravity(i);
                //if the sprite type is an enemy or pup, do the following
                if(SPRITE[i].type == GOOMBA || SPRITE[i].type == SPIKEY || SPRITE[i].type == SWOOPER || SPRITE[i].type == PUP){
                    // check all enemies and if they are appearing on the screen, move them in the proper manner
                    int rightScreen = SPRITE[PLAYER].x+600;
                    int leftScreen = SPRITE[PLAYER].x-500;
                    if(SPRITE[i].x < rightScreen && SPRITE[i].x > leftScreen && SPRITE[i].stillAlive == true && SPRITE[i].y < 500 && SPRITE[i].dying == false){
                                   if(SPRITE[i].type == GOOMBA)
                                      moveGoomba(i);
                                   if(SPRITE[i].type == SPIKEY)
                                      moveSpikey(i);
                                   if(SPRITE[i].type == SWOOPER)
                                      moveSwooper(i);
                                   if(SPRITE[i].type == PUP){
                                      updateFrame(i);
                                      SPRITE[i].currentFrame = ring[SPRITE[i].curframe];
                                   }
                    }
                    
                    // if the enemy is far away from the player and has been moved, reset it's x and y to the origin
                    if(SPRITE[i].originx != SPRITE[i].x && SPRITE[i].originy != SPRITE[i].y && SPRITE[i].stillAlive == true && (SPRITE[i].x > SPRITE[PLAYER].x+1500 || SPRITE[i].x < SPRITE[PLAYER].x-1300)){
                                         SPRITE[i].x = SPRITE[i].originx;
                                         SPRITE[i].y = SPRITE[i].originy;
                    } 
                }       
        } 
                         
        // check if player hit ground on each side
        bool hitEast = false;
        bool hitWest = false;
        bool hitNorth = false;
        for(int i = 0; i < 50; i++){
                if(SPRITE[i].type == GROUND){
                            if(hitEast == false)
                                       hitEast = checkSideCollision(PLAYER, i, EAST);
                            if(hitWest == false)
                                       hitWest = checkSideCollision(PLAYER, i, WEST);
                            if(hitNorth == false)
                                        hitNorth = checkSideCollision(PLAYER, i, NORTH);
                }
        }   
        
        bool hitObject = false;
        //check to see if a player has hit an enemy, powerup, or anything else
        for(int i = 0; i < 50; i++){
                if((SPRITE[i].type == GOOMBA || SPRITE[i].type == SPIKEY || SPRITE[i].type == SWOOPER || SPRITE[i].type == PUP) && SPRITE[i].stillAlive == true && SPRITE[i].dying == false && SPRITE[PLAYER].invincible == false){ // put all objects types you'd like to check here
                                  hitObject = checkBoxCollision(PLAYER, i);
                                  if(hitObject){
                                            hitObject = false;
                                            playerHitSprite(i);
                                  }
                }
        }
        
        // check to make sure player hasn't fallen off the screen and if they have, kill them
        if(SPRITE[PLAYER].y > 550)
                            killPlayer();
        
        // take user input
        if(key[KEY_RIGHT] && !key[KEY_LEFT] && !key[KEY_UP] && !key[KEY_DOWN] && hitEast == false){
                          
                              if(camerax <= 5360 && SPRITE[PLAYER].x >= playerMiddlex) 
                                        camerax = camerax + SPRITE[PLAYER].xspeed;
                              
                              if(SPRITE[PLAYER].x+SPRITE[PLAYER].width < 6000)          
                                        SPRITE[PLAYER].x = SPRITE[PLAYER].x + SPRITE[PLAYER].xspeed;
                              
                              // handle direction and animation info
                              SPRITE[PLAYER].dir = EAST;
                              SPRITE[PLAYER].action = WALKRIGHT;
                              SPRITE[PLAYER].maxframe = PLAYERWALKFRAMES;
                              SPRITE[PLAYER].idleCounter = 0;
                              updateFrame(PLAYER);
                              SPRITE[PLAYER].currentFrame = sonicwalkRight[SPRITE[PLAYER].curframe];
                              
        }
        
        if(key[KEY_LEFT] && !key[KEY_RIGHT] && !key[KEY_UP] && !key[KEY_DOWN] && hitWest == false){
                          
                             if(camerax > 0 && SPRITE[PLAYER].x <= playerMiddlex)
                                        camerax = camerax - SPRITE[PLAYER].xspeed;
                             
                             if(SPRITE[PLAYER].x > 0)
                                        SPRITE[PLAYER].x = SPRITE[PLAYER].x - SPRITE[PLAYER].xspeed;
                                        
                             SPRITE[PLAYER].dir = WEST;
                             SPRITE[PLAYER].action = WALKLEFT;
                             SPRITE[PLAYER].maxframe = PLAYERWALKFRAMES;
                             SPRITE[PLAYER].idleCounter = 0;
                             updateFrame(PLAYER);
                             SPRITE[PLAYER].currentFrame = sonicwalkLeft[SPRITE[PLAYER].curframe];
        }
        
        if(key[KEY_SPACE] && SPRITE[PLAYER].jumping == false && SPRITE[PLAYER].inAir == false){
                          SPRITE[PLAYER].jumping = true;
                          SPRITE[PLAYER].inAir = true;
                          SPRITE[PLAYER].timeInAir = 0;
                          SPRITE[PLAYER].idleCounter = 0;
                          play_sample(jumpsound,255,255,1000,0);
        }
        
        // make player run
        if(key[KEY_LSHIFT] || key[KEY_RSHIFT]){
                         SPRITE[PLAYER].xspeed = 5;
                         SPRITE[PLAYER].framedelay = 1;
        }
        else{
                         SPRITE[PLAYER].xspeed = 3;
                         SPRITE[PLAYER].framedelay = 4;
        }
        // make player sprite look up
        if(key[KEY_UP]){
                       SPRITE[PLAYER].idleCounter = 0;
                       if(SPRITE[PLAYER].dir == EAST){
                            updateFrame(PLAYER);
                            SPRITE[PLAYER].currentFrame = upright;
                       }
                       if(SPRITE[PLAYER].dir == WEST){
                            updateFrame(PLAYER);
                            SPRITE[PLAYER].currentFrame = upleft;
                       }
        }
        // make player duck
        if(key[KEY_DOWN]){
                        SPRITE[PLAYER].idleCounter = 0;
                        if(SPRITE[PLAYER].dir == EAST){
                            updateFrame(PLAYER);
                            SPRITE[PLAYER].currentFrame = duckright;
                        }
                        if(SPRITE[PLAYER].dir == WEST){
                            updateFrame(PLAYER);
                            SPRITE[PLAYER].currentFrame = duckleft;
                        }
        }
        
        // if player is in the air, change the animation accordingly
        if(SPRITE[PLAYER].inAir == true){
                                SPRITE[PLAYER].idleCounter = 0;
                                SPRITE[PLAYER].maxframe = PLAYERJUMPFRAMES;
                                updateFrame(PLAYER);
                                if(SPRITE[PLAYER].dir == EAST)
                                        SPRITE[PLAYER].currentFrame = sonicjumpRight[SPRITE[PLAYER].curframe];
                                else
                                        SPRITE[PLAYER].currentFrame = sonicjumpLeft[SPRITE[PLAYER].curframe];
        }
                                
        
        // if player is powered up, allow him to throw rings
        if(key[KEY_F] && SPRITE[PLAYER].poweredUp == true && SPRITE[PLAYER].ringThrown == false){
                      SPRITE[PLAYER].idleCounter = 0;
                      SPRITE[PLAYER].ringThrown = true;
                      SPRITE[RING].stillAlive = true;
                      if(SPRITE[PLAYER].dir == EAST)
                          SPRITE[RING].x = SPRITE[PLAYER].x+SPRITE[PLAYER].width+1;
                          
                      if(SPRITE[PLAYER].dir == WEST)
                          SPRITE[RING].x = SPRITE[PLAYER].x - 1;
                      
                      SPRITE[RING].y = SPRITE[PLAYER].y+(SPRITE[PLAYER].height/2);
                      SPRITE[RING].dir = SPRITE[PLAYER].dir;
                      play_sample(ringthrow,255,255,1000,0);                      
        }
        
        // update animation data if player is standing still
        if(!key[KEY_LEFT] && !key[KEY_RIGHT] && SPRITE[PLAYER].inAir == false && !key[KEY_UP] && !key[KEY_DOWN]){
                          SPRITE[PLAYER].idleCounter = SPRITE[PLAYER].idleCounter+1;
                          if(SPRITE[PLAYER].dir == EAST)
                                                SPRITE[PLAYER].currentFrame = sonicstandRight;
                          else
                                                SPRITE[PLAYER].currentFrame = sonicstandLeft;
                          //play idle animation if player has remained idle for a while
                          if(SPRITE[PLAYER].idleCounter > 200){
                                                        SPRITE[PLAYER].framedelay = 15;
                                                        SPRITE[PLAYER].maxframe = PLAYERIDLEFRAMES;
                                                        updateFrame(PLAYER);
                                                        if(SPRITE[PLAYER].dir == EAST)
                                                                              SPRITE[PLAYER].currentFrame = sonicidleRight[SPRITE[PLAYER].curframe];
                                                        else
                                                                              SPRITE[PLAYER].currentFrame = sonicidleLeft[SPRITE[PLAYER].curframe];
                          }
                                                        
        }
        
        // if player jumps and hits it's head on GROUND type, end the jump
        if(hitNorth == true){
                    SPRITE[PLAYER].jumping = false;
                    SPRITE[PLAYER].hopping = false;
        }
        
        
        // update a jump velocity if the player is jumping
        if(SPRITE[PLAYER].jumping == true && hitNorth == false)
                    updateJump(PLAYER);
	    
	    // update hop velocity if player is hopping
	    if(SPRITE[PLAYER].hopping == true && hitNorth == false)
                    updateHop(PLAYER);
     
        if(SPRITE[PLAYER].hopping == true)
                                  SPRITE[PLAYER].jumping = false; 
        
        // handle ringThrown movement
        if(SPRITE[PLAYER].ringThrown == true && SPRITE[RING].stillAlive == true){
                      updateFrame(RING);
                      SPRITE[RING].currentFrame = ring[SPRITE[RING].curframe];
                     // update x movement
                     if(SPRITE[RING].dir == EAST)
                                         SPRITE[RING].x = SPRITE[RING].x+SPRITE[RING].xspeed;
                     if(SPRITE[RING].dir == WEST)
                                         SPRITE[RING].x = SPRITE[RING].x - SPRITE[RING].xspeed;
                     
                     // if ring is hopping, update hop movement
                     if(SPRITE[RING].hopping == true)
                                updateHop(RING);
                                
                     // if fell off screen, kill ring           
                     if(SPRITE[RING].y > 500 || SPRITE[RING].x > SPRITE[PLAYER].x+500 || SPRITE[RING].x < SPRITE[PLAYER].x-500){
                                       killRing();
                     }
                     
                     // if ring hits anything, kill ring. If it hit an enemy, kill that enemy
                     for(int i = 0; i < 50; i++){
                             if((SPRITE[i].type == GOOMBA || SPRITE[i].type == SPIKEY || SPRITE[i].type == SWOOPER) && SPRITE[i].stillAlive == true && SPRITE[i].dying == false){
                                               if(checkBoxCollision(RING, i)){
                                                        killRing();
                                                        killBadGuy(i, SHOT);
                                               }
                             }
                             if(SPRITE[i].type == GROUND && SPRITE[i].y < SPRITE[RING].y){
                                               if(checkBoxCollision(RING,i))
                                                         killRing();
                             }
                     }
        }
	    
	    // update player x and y if they're on a moving platform
        if((SPRITE[SPRITE[PLAYER].currentPlatform].platDistance != 0) && SPRITE[PLAYER].inAir == false){
                      SPRITE[PLAYER].x = SPRITE[PLAYER].x + SPRITE[SPRITE[PLAYER].currentPlatform].xMovement;
                      SPRITE[PLAYER].y = SPRITE[PLAYER].y + SPRITE[SPRITE[PLAYER].currentPlatform].yMovement;
                      camerax = camerax + SPRITE[SPRITE[PLAYER].currentPlatform].xMovement;
        }
        
	    // update moving platforms
        for(int i = 0; i < 50; i++){
                if(SPRITE[i].type == PLATFORM && SPRITE[i].platDistance != 0)
                                  updatePlatform(i);
        }
        
        // update sprites that are dying
        for(int i = 0; i < 50; i++){
                if(SPRITE[i].dying == true){
                                   SPRITE[i].deathClock = SPRITE[i].deathClock+1;
                                   spriteDying(i);
                }
                if(SPRITE[i].deathClock == 150){
                                        SPRITE[i].dying = false;
                                        SPRITE[i].stillAlive = false;
                                        SPRITE[i].deathClock = 0;
                }
        }
        
        //update invincibility
        if(SPRITE[PLAYER].invincible == true)
               updateInvincibility();
        
	    
        drawSprites();
	    
	    // if player is at the end, make the next level
	    if(SPRITE[PLAYER].x >= 5850)
	                        newLevel();
	    
        acquire_screen();
    	blit(buffer,screen,camerax,0,0,0,SCREEN_W,SCREEN_H);
    	rest(10);
        release_screen();
    }
    destroy_bitmap(buffer);

	deinit();
	return 0;
}
END_OF_MAIN()

void moveGoomba(int sprite1){
     for(int i = 0; i < 50; i++){
             if(SPRITE[i].type == GROUND){
                          if(checkBoxCollision(sprite1, i) && SPRITE[i].y < SPRITE[sprite1].y+SPRITE[sprite1].height-10 && SPRITE[sprite1].inAir == false){
                                                         SPRITE[sprite1].xspeed = -SPRITE[sprite1].xspeed;
                                                         bool founddir = false;
                                                         if(SPRITE[sprite1].dir == EAST){
                                                                                SPRITE[sprite1].dir = WEST;
                                                                                founddir = true;
                                                         }
                                                                                
                                                         if(SPRITE[sprite1].dir == WEST && founddir == false)
                                                                                SPRITE[sprite1].dir = EAST;
                          }
             }
     }
     if(SPRITE[sprite1].dir == WEST)
                                    SPRITE[sprite1].currentFrame = goombawalkLeft[SPRITE[sprite1].curframe];
     if(SPRITE[sprite1].dir == EAST)
                                    SPRITE[sprite1].currentFrame = goombawalkRight[SPRITE[sprite1].curframe];
     SPRITE[sprite1].x = SPRITE[sprite1].x - SPRITE[sprite1].xspeed;
     updateFrame(sprite1);
}

void newLevel(){
     initializeSprites();
     SPRITE[PLAYER].level = SPRITE[PLAYER].level + 1;
     setupSprites();
}

void spriteDying(int sprite1){
     SPRITE[sprite1].x = SPRITE[sprite1].x + 1;
     SPRITE[sprite1].y = SPRITE[sprite1].y + 2;
}

void updateFrame(int sprite1){
     if(SPRITE[sprite1].framecount <= SPRITE[sprite1].framedelay)
            SPRITE[sprite1].framecount++;
     else{
            SPRITE[sprite1].framecount = 0;
            SPRITE[sprite1].curframe++;
     }
     if(SPRITE[sprite1].curframe >= SPRITE[sprite1].maxframe)
                   SPRITE[sprite1].curframe = 0;
}

void killRing(){
     SPRITE[PLAYER].ringThrown = false;
     SPRITE[RING].stillAlive = false;
     SPRITE[RING].hopping = false;
}

void updateInvincibility(){
     SPRITE[PLAYER].invincibleTime = SPRITE[PLAYER].invincibleTime+1;
     // make player flash while invincible
     SPRITE[PLAYER].stillAlive = !SPRITE[PLAYER].stillAlive;
     
     // when invincible time is up, turn it off
     if(SPRITE[PLAYER].invincibleTime >= 150){
             SPRITE[PLAYER].stillAlive = true;
             SPRITE[PLAYER].invincible = false;
     }
}

void moveSwooper(int sprite1){
     SPRITE[sprite1].currentFrame = swoopersprite[SPRITE[sprite1].curframe];
     updateFrame(sprite1);
     if(SPRITE[sprite1].hanging == true && SPRITE[sprite1].y <= 480)
               SPRITE[sprite1].hangtime = SPRITE[sprite1].hangtime +1;
     if(SPRITE[sprite1].hangtime == 80){
               SPRITE[sprite1].hanging = false;
               SPRITE[sprite1].swooping = true;
               SPRITE[sprite1].hangtime = 0;
     }
     
     // make the enemy swoop
     if(SPRITE[sprite1].swooping == true){
                     // fly diagonally down until level with the player's y coordinate
                     if(SPRITE[sprite1].y < SPRITE[PLAYER].y){
                                          SPRITE[sprite1].x = SPRITE[sprite1].x - 2;
                                          SPRITE[sprite1].y = SPRITE[sprite1].y + 5;
                     }
                     // when level with player's y, start the zip
                     if(SPRITE[sprite1].y >= SPRITE[PLAYER].y){
                                          SPRITE[sprite1].swooping = false;
                                          SPRITE[sprite1].zipping = true;
                                          SPRITE[sprite1].ziptime = 0;
                     }
     }
     if(SPRITE[sprite1].zipping == true){
                                SPRITE[sprite1].ziptime = SPRITE[sprite1].ziptime+1;
                                SPRITE[sprite1].x = SPRITE[sprite1].x - SPRITE[sprite1].xspeed;
                                // fly the swooper off the screen after it zips past
                                if(SPRITE[sprite1].x < SPRITE[PLAYER].x-400)
                                           SPRITE[sprite1].y = SPRITE[sprite1].y - 10;
                                if(SPRITE[sprite1].y > 490){
                                           SPRITE[sprite1].zipping = false;
                                           SPRITE[sprite1].hanging = true;
                                           SPRITE[sprite1].hangtime = 0;
                                }
     }         
}

void moveSpikey(int sprite1){
     // get the range of how far the enemy can move based on how wide the platform it's on is
     int range1 = SPRITE[SPRITE[sprite1].currentPlatform].x;
     int range2 = SPRITE[SPRITE[sprite1].currentPlatform].x + SPRITE[SPRITE[sprite1].currentPlatform].width;
     int leftSide = SPRITE[sprite1].x;
     int rightSide = SPRITE[sprite1].x + SPRITE[sprite1].width;
     
     if(leftSide-1 < range1){
                   SPRITE[sprite1].dir = EAST;
                   SPRITE[sprite1].xspeed = -SPRITE[sprite1].xspeed;
     }
     
     if(rightSide+1 > range2){
                    SPRITE[sprite1].dir = WEST;
                    SPRITE[sprite1].xspeed = -SPRITE[sprite1].xspeed;
     }
     if(SPRITE[sprite1].dir == EAST)
                             SPRITE[sprite1].currentFrame = spikeywalkRight[SPRITE[sprite1].curframe];
     if(SPRITE[sprite1].dir == WEST)
                             SPRITE[sprite1].currentFrame = spikeywalkLeft[SPRITE[sprite1].curframe];
     
     SPRITE[sprite1].x = SPRITE[sprite1].x + SPRITE[sprite1].xspeed;
     updateFrame(sprite1);
}

void updateHop(int sprite1){
     SPRITE[sprite1].timeInAir = SPRITE[sprite1].timeInAir+1;
     if(SPRITE[sprite1].timeInAir < HOPPEAK)
                                  SPRITE[sprite1].y = SPRITE[sprite1].y - HOPPOWER;
     if(SPRITE[sprite1].timeInAir >= HOPPEAK)
                                  SPRITE[sprite1].y = SPRITE[sprite1].y - GRAVITY;
     if(SPRITE[sprite1].timeInAir == HOPTIME)                
                 SPRITE[sprite1].hopping = false;
}

void playerHitSprite(int spriteCol){
     bool stompedBaddy = false;
         // if player hit an enemy, check to see if it was stomped on. If not, kill the player
         if((SPRITE[spriteCol].type == GOOMBA || SPRITE[spriteCol].type == SWOOPER) && SPRITE[spriteCol].stillAlive == true && SPRITE[spriteCol].dying == false){
                           stompedBaddy = checkSideCollision(PLAYER,spriteCol, STOMP);
                           if(stompedBaddy)
                                           killBadGuy(spriteCol, STOMPED);
                           else
                                           killPlayer();
         }
         
         // if the enemy was spikey, kill the player
         if(SPRITE[spriteCol].type == SPIKEY && SPRITE[spriteCol].stillAlive == true && SPRITE[spriteCol].dying == false)
                                   killPlayer();
                                   
         // if sprite is PUP, powerup the player
         if(SPRITE[spriteCol].type == PUP && SPRITE[spriteCol].stillAlive == true){
                                   rest(50);
                                   SPRITE[spriteCol].stillAlive = false;
                                   SPRITE[PLAYER].poweredUp = true;
                                   play_sample(powerup,255,255,1000,0);
         }
}

void killBadGuy(int enemy, int method){
     SPRITE[enemy].dying = true;
     if(method == STOMPED){
               SPRITE[PLAYER].hopping = true;
               SPRITE[PLAYER].inAir = true;
               SPRITE[PLAYER].timeInAir = 0;
     }  
     play_sample(killenemy,255,255,1000,0);   
}

void killPlayer(){
     // if player isn't powered up, kill the player and reset everything
     if(SPRITE[PLAYER].poweredUp == false){
         SPRITE[PLAYER].x = SPRITE[PLAYER].originx;
         SPRITE[PLAYER].y = SPRITE[PLAYER].originy;
         camerax = 0;
         for(int i = 0; i < 50; i++){
                 if(SPRITE[i].type == GOOMBA || SPRITE[i].type == SPIKEY || SPRITE[i].type == SWOOPER || SPRITE[i].type == PUP){
                                   SPRITE[i].stillAlive = true;
                                   SPRITE[i].dying = false;
                                   SPRITE[i].x = SPRITE[i].originx;
                                   SPRITE[i].y = SPRITE[i].originy;
                                   SPRITE[i].hanging = true;
                                   SPRITE[i].zipping = false;
                                   SPRITE[i].hangtime = 0;
                 }
         }
     }
     
     // if player IS powered up, take the power up away and begin temporary invincibility
     if(SPRITE[PLAYER].poweredUp == true){
         SPRITE[PLAYER].poweredUp = false;
         SPRITE[PLAYER].invincible = true;
         SPRITE[PLAYER].invincibleTime = 0;
     }
}
       
     

void updatePlatform(int plat){
     SPRITE[plat].x = SPRITE[plat].x + SPRITE[plat].xMovement;
     SPRITE[plat].y = SPRITE[plat].y + SPRITE[plat].yMovement;
     SPRITE[plat].platCounter = SPRITE[plat].platCounter + SPRITE[plat].xMovement + SPRITE[plat].yMovement;
     if(SPRITE[plat].platCounter >= SPRITE[plat].platDistance || SPRITE[plat].platCounter < 0){
                                 SPRITE[plat].xMovement = -SPRITE[plat].xMovement;
                                 SPRITE[plat].yMovement = -SPRITE[plat].yMovement;
     }
}
                                 

void updateJump(int sprite1){
     SPRITE[sprite1].timeInAir = SPRITE[sprite1].timeInAir+1;
     if(SPRITE[sprite1].timeInAir < JUMPPEAK)
                                  SPRITE[sprite1].y = SPRITE[sprite1].y - JUMPPOWER;
     if(SPRITE[sprite1].timeInAir >= JUMPPEAK)
                                  SPRITE[sprite1].y = SPRITE[sprite1].y - GRAVITY;
     if(SPRITE[sprite1].timeInAir == JUMPTIME)                
                 SPRITE[sprite1].jumping = false;

}

void drawSprites(){
     if(SPRITE[PLAYER].level == 1)
                             draw_sprite(buffer,level1,0,0);
     if(SPRITE[PLAYER].level == 2)
                             draw_sprite(buffer,level2,0,0);
     
     for(int i = 0; i < 50; i++){
             if(SPRITE[i].type == PLATFORM)
                               stretch_sprite(buffer,ground,SPRITE[i].x,SPRITE[i].y,SPRITE[i].width,SPRITE[i].height);
             if(SPRITE[i].type == GOOMBA && SPRITE[i].stillAlive == true)
                               stretch_sprite(buffer,SPRITE[i].currentFrame,SPRITE[i].x,SPRITE[i].y,SPRITE[i].width,SPRITE[i].height);
             if(SPRITE[i].type == SPIKEY && SPRITE[i].stillAlive == true)
                               stretch_sprite(buffer,SPRITE[i].currentFrame,SPRITE[i].x,SPRITE[i].y,SPRITE[i].width,SPRITE[i].height);
             if(SPRITE[i].type == SWOOPER && SPRITE[i].stillAlive == true)
                               stretch_sprite(buffer,SPRITE[i].currentFrame,SPRITE[i].x,SPRITE[i].y,SPRITE[i].width,SPRITE[i].height);
             if(SPRITE[i].type == PUP && SPRITE[i].stillAlive == true)
                               stretch_sprite(buffer,SPRITE[i].currentFrame,SPRITE[i].x,SPRITE[i].y,SPRITE[i].width,SPRITE[i].height);
     }
     if(SPRITE[PLAYER].stillAlive == true)
                                  stretch_sprite(buffer, SPRITE[PLAYER].currentFrame, SPRITE[PLAYER].x, SPRITE[PLAYER].y, SPRITE[PLAYER].width, SPRITE[PLAYER].height);
     if(SPRITE[RING].stillAlive == true)
                                stretch_sprite(buffer, SPRITE[RING].currentFrame, SPRITE[RING].x, SPRITE[RING].y, SPRITE[RING].width, SPRITE[RING].height);
     
     //textprintf_ex(buffer, font, SPRITE[PLAYER].x-175,SPRITE[PLAYER].y-200 , RED,-1, "%i", SPRITE[28].curframe);
     //textprintf_ex(buffer, font, SPRITE[PLAYER].x-175,SPRITE[PLAYER].y-175 , RED,-1, "%i", SPRITE[PLAYER].y);
}

void setupSprites(){
     for(int i = 0; i < 50; i++)
             SPRITE[i].type = NOSPRITE;
     
     
     // SPRITE[0] AND SPRITE[5] ARE USED FOR THE PLAYER AND THE RINGS HE THROWS
     // DO NOT USE THEM FOR ANYTHING ELSE!!!!!!!!
     camerax = 0;
     
     SPRITE[PLAYER].xspeed = 3;
     SPRITE[PLAYER].x = 50;
     SPRITE[PLAYER].y = 150;
     SPRITE[PLAYER].originx = SPRITE[PLAYER].x;
     SPRITE[PLAYER].originy = SPRITE[PLAYER].y;
     SPRITE[PLAYER].width = 40;
     SPRITE[PLAYER].height = 40;
     SPRITE[PLAYER].type = PLAYER;
     SPRITE[PLAYER].dir = EAST;
     SPRITE[PLAYER].action = WALKRIGHT;
     SPRITE[PLAYER].currentFrame = sonicwalkRight[0];
     SPRITE[PLAYER].framedelay = 3;
     
     SPRITE[RING].type = RING;
     SPRITE[RING].stillAlive = false;
     SPRITE[RING].width = 10;
     SPRITE[RING].height = 10;
     SPRITE[RING].xspeed = 6;
     SPRITE[RING].hopping = false;
     SPRITE[RING].currentFrame = ring[0];
     SPRITE[RING].framedelay = 5;
     SPRITE[RING].maxframe = RINGFRAMES;
     
     
     // setup the sprites based on player's level
     if(SPRITE[PLAYER].level == 1){
         // make all platforms and ground first
         SPRITE[1].type = GROUND;
         SPRITE[1].x = 0;
         SPRITE[1].y = 320;
         SPRITE[1].width = 720;
         SPRITE[1].height = 160;
         
         SPRITE[2].type = GROUND;
         SPRITE[2].x = 321;
         SPRITE[2].y = 241;
         SPRITE[2].width = 199;
         SPRITE[2].height = 39;
         
         SPRITE[3].type = GROUND;
         SPRITE[3].x = 839;
         SPRITE[3].y = 161;
         SPRITE[3].width = 241;
         SPRITE[3].height = 39;
         
        /* SPRITE[4].type = GROUND;
         SPRITE[4].x = 839;
         SPRITE[4].y = 241;
         SPRITE[4].width = 241;
         SPRITE[4].height = 39;*/
         
         /* The number 5 is a reserved keyword */
                  
         SPRITE[6].type = GROUND;
         SPRITE[6].x = 1160;
         SPRITE[6].y = 321;
         SPRITE[6].width = 200; // 1360
         SPRITE[6].height = 320; // 479
         
         SPRITE[7].type = GROUND;
         SPRITE[7].x = 1359;
         SPRITE[7].y = 360;
         SPRITE[7].width = 41; //1400
         SPRITE[7].height = 117; // 479
         
         SPRITE[8].type = GROUND;
         SPRITE[8].x = 1398;
         SPRITE[8].y = 402;
         SPRITE[8].width = 42; //1440
         SPRITE[8].height = 77;//479
         
         SPRITE[9].type = GROUND;
         SPRITE[9].x = 1438;
         SPRITE[9].y = 441;
         SPRITE[9].width = 200; //1638
         SPRITE[9].height = 38; //479
         
         SPRITE[10].type = GROUND;
         SPRITE[10].x = 1479;
         SPRITE[10].y = 402;
         SPRITE[10].width = 119; //1598
         SPRITE[10].height = 38; //440
         
         SPRITE[11].type = GROUND;
         SPRITE[11].x = 1518;
         SPRITE[11].y = 362;
         SPRITE[11].width = 39; //1557
         SPRITE[11].height = 40;//402
         
         SPRITE[12].type = PLATFORM;
         SPRITE[12].x = 1680;
         SPRITE[12].y = 393;
         SPRITE[12].width = 100;
         SPRITE[12].height = 10;
         SPRITE[12].platDistance = 150;
         SPRITE[12].xMovement = 2;
         
         SPRITE[13].type = PLATFORM;
         SPRITE[13].x = 1900;
         SPRITE[13].y = 293;
         SPRITE[13].width = 100;
         SPRITE[13].height = 10;
         SPRITE[13].platDistance = 160;
         SPRITE[13].xMovement = 2;
         
         SPRITE[14].type = GROUND;
         SPRITE[14].x = 2242;
         SPRITE[14].y = 401;
         SPRITE[14].width = 1075; //3317
         SPRITE[14].height = 78;//479
         
         SPRITE[15].type = INVISIPLATFORM;
         SPRITE[15].x = 2440;
         SPRITE[15].y = 320;
         SPRITE[15].width = 236; //2676
         SPRITE[15].height = 10;//330
         
         SPRITE[16].type = PLATFORM;
         SPRITE[16].x = 3440;
         SPRITE[16].y = 94;
         SPRITE[16].width = 100;
         SPRITE[16].height = 10;
         SPRITE[16].platDistance = 311;
         SPRITE[16].yMovement = 1;
         
         SPRITE[17].type = PLATFORM;
         SPRITE[17].x = 3560;
         SPRITE[17].y = 110;
         SPRITE[17].width = 100;
         SPRITE[17].height = 10;
         SPRITE[17].platDistance = 150;
         SPRITE[17].xMovement = 2;
         
         SPRITE[18].type = PLATFORM;
         SPRITE[18].x = 3800;
         SPRITE[18].y = 90;
         SPRITE[18].width = 100;
         SPRITE[18].height = 10;
         SPRITE[18].platDistance = 150;
         SPRITE[18].yMovement = 2;
         
         SPRITE[19].type = PLATFORM;
         SPRITE[19].x = 4000;
         SPRITE[19].y = 190;
         SPRITE[19].width = 100;
         SPRITE[19].height = 10;
         SPRITE[19].platDistance = 220;
         SPRITE[19].xMovement = 2;
         
         SPRITE[20].type = PLATFORM;
         SPRITE[20].x = 4400;
         SPRITE[30].y = 100;
         SPRITE[20].width = 100;
         SPRITE[20].height = 10;
         SPRITE[20].platDistance = 300;
         SPRITE[20].yMovement = 2;
         
         SPRITE[21].type = PLATFORM;
         SPRITE[21].x = 4600;
         SPRITE[21].y = 160;
         SPRITE[21].width = 100;
         SPRITE[21].height = 10;
         SPRITE[21].platDistance = 220;
         SPRITE[21].xMovement = 2;
         
         SPRITE[22].type = PLATFORM;
         SPRITE[22].x = 5000;
         SPRITE[22].y = 200;
         SPRITE[22].width = 100;
         SPRITE[22].height = 10;
         SPRITE[22].platDistance = 200;
         SPRITE[22].xMovement = 2;
         SPRITE[22].yMovement = 2;
         
         SPRITE[23].type = GROUND;
         SPRITE[23].x = 5400;
         SPRITE[23].y = 442;
         SPRITE[23].width = 40; //3317
         SPRITE[23].height = 40;//479
         
         SPRITE[24].type = GROUND;
         SPRITE[24].x = 5439;
         SPRITE[24].y = 362;
         SPRITE[24].width = 41; //3317
         SPRITE[24].height = 117;//479
         
         SPRITE[25].type = GROUND;
         SPRITE[25].x = 5482;
         SPRITE[25].y = 322;
         SPRITE[25].width = 518; //5997
         SPRITE[25].height = 157;//479
         
         SPRITE[26].type = GOOMBA;
         SPRITE[26].originx = 542;
         SPRITE[26].originy = 100;
         SPRITE[26].x = SPRITE[26].originx;
         SPRITE[26].y = SPRITE[26].originy;
         
         SPRITE[27].type = GOOMBA;
         SPRITE[27].originx = 1330;
         SPRITE[27].originy = 293;
         SPRITE[27].x = SPRITE[27].originx;
         SPRITE[27].y = SPRITE[27].originy;
         
         SPRITE[28].type = SPIKEY;
         SPRITE[28].originx = 2640;
         SPRITE[28].originy = 289;
         SPRITE[28].x = SPRITE[28].originx;
         SPRITE[28].y = SPRITE[28].originy;
         
         
         SPRITE[29].type = SWOOPER;
         SPRITE[29].originx = 2206;
         SPRITE[29].originy = 128;
         SPRITE[29].x = SPRITE[29].originx;
         SPRITE[29].y = SPRITE[29].originy;

         
         SPRITE[30].type = PUP;
         SPRITE[30].originx = 2538;
         SPRITE[30].originy = 188;
         SPRITE[30].x = SPRITE[30].originx;
         SPRITE[30].y = SPRITE[30].originy;
         
         SPRITE[31].type = GROUND;
         SPRITE[31].x = 839;
         SPRITE[31].y = 321;
         SPRITE[31].width = 241;
         SPRITE[31].height = 159;
         
         SPRITE[32].type = GOOMBA;
         SPRITE[32].originx = 595;
         SPRITE[32].originy = 280;
         SPRITE[32].x = SPRITE[32].originx;
         SPRITE[32].y = SPRITE[32].originy;
         
         SPRITE[33].type = GOOMBA;
         SPRITE[33].originx = 1290;
         SPRITE[33].originy = 293;
         SPRITE[33].x = SPRITE[33].originx;
         SPRITE[33].y = SPRITE[33].originy;
         
         SPRITE[34].type = GOOMBA;
         SPRITE[34].originx = 3207;
         SPRITE[34].originy = 368;
         SPRITE[34].x = SPRITE[34].originx;
         SPRITE[34].y = SPRITE[34].originy;

         
         SPRITE[35].type = GOOMBA;
         SPRITE[35].originx = 3257;
         SPRITE[35].originy = 368;
         SPRITE[35].x = SPRITE[35].originx;
         SPRITE[35].y = SPRITE[35].originy;
         
         
         SPRITE[36].type = GOOMBA;
         SPRITE[36].originx = 3160;
         SPRITE[36].originy = 368;
         SPRITE[36].x = SPRITE[36].originx;
         SPRITE[36].y = SPRITE[36].originy;
         
         play_midi(level1music,1);
     }
     
     if(SPRITE[PLAYER].level == 2){
         stop_midi();
         play_midi(level2music,1);
         SPRITE[1].type = GROUND;
         SPRITE[1].x = 401;
         SPRITE[1].y = 0;
         SPRITE[1].width = 520;
         SPRITE[1].height = 160;
         
         SPRITE[2].type = GROUND;
         SPRITE[2].x = 0;
         SPRITE[2].y = 241;
         SPRITE[2].width = 197;
         SPRITE[2].height = 240;
         
         SPRITE[3].type = GROUND;
         SPRITE[3].x = 401;
         SPRITE[3].y = 241;
         SPRITE[3].width = 157;
         SPRITE[3].height = 200;
         
         SPRITE[4].type = GROUND;
         SPRITE[4].x = 561;
         SPRITE[4].y = 281;
         SPRITE[4].width = 197;
         SPRITE[4].height = 160;
         
         /*The number 5 is a reserved keyword*/
         
         SPRITE[6].type = GROUND;
         SPRITE[6].x = 761;
         SPRITE[6].y = 321;
         SPRITE[6].width = 157;
         SPRITE[6].height = 240;
         
         SPRITE[7].type = GROUND;
         SPRITE[7].x = 921;
         SPRITE[7].y = 241;
         SPRITE[7].width = 117;
         SPRITE[7].height = 240;
         
         SPRITE[8].type = GROUND;
         SPRITE[8].x = 1481;
         SPRITE[8].y = 0;
         SPRITE[8].width = 400;
         SPRITE[8].height = 160;
         
         SPRITE[9].type = GROUND;
         SPRITE[9].x = 2081;
         SPRITE[9].y = 81;
         SPRITE[9].width = 200;
         SPRITE[9].height = 160;
         
         SPRITE[10].type = GROUND;
         SPRITE[10].x = 2268;
         SPRITE[10].y = 81;
         SPRITE[10].width = 80;
         SPRITE[10].height = 80;
         
         SPRITE[11].type = GROUND;
         SPRITE[11].x = 2318;
         SPRITE[11].y = 161;
         SPRITE[11].width = 40;
         SPRITE[11].height = 80;
         
         SPRITE[12].type = GROUND;
         SPRITE[12].x = 2681;
         SPRITE[12].y = 281;
         SPRITE[12].width = 77;
         SPRITE[12].height = 200;
         
         SPRITE[13].type = GROUND;
         SPRITE[13].x = 2761;
         SPRITE[13].y = 321;
         SPRITE[13].width = 477;
         SPRITE[13].height = 200;
         
         SPRITE[14].type = GROUND;
         SPRITE[14].x = 3241;
         SPRITE[14].y = 281;
         SPRITE[14].width = 76;
         SPRITE[14].height = 240;
         
         SPRITE[15].type = GROUND;
         SPRITE[15].x = 3961;
         SPRITE[15].y = 0;
         SPRITE[15].width = 1360;
         SPRITE[15].height = 80;
         
         SPRITE[16].type = GROUND;
         SPRITE[16].x = 5801;
         SPRITE[16].y = 241;
         SPRITE[16].width = 199;
         SPRITE[16].height = 240;
                  
/*         SPRITE[17].type = GOOMBA;
         SPRITE[17].originx = 538;
         SPRITE[17].originy = 214;
         SPRITE[17].x = SPRITE[17].originx;
         SPRITE[17].y = SPRITE[17].originy;*/
         
         GOOMBAENT(17,538,214);
         
         SPRITE[18].type = GOOMBA;
         SPRITE[18].originx = 1002;
         SPRITE[18].originy = 205;
         SPRITE[18].x = SPRITE[18].originx;
         SPRITE[18].y = SPRITE[18].originy;
         
         /*SPRITE[19].type = SPIKEY;
         SPRITE[19].originx = 593;
         SPRITE[19].originy = 246;
         SPRITE[19].x = SPRITE[19].originx;
         SPRITE[19].y = SPRITE[19].originy;
                  This is now a goomba*/
         SPRITE[19].type = PLATFORM;
         SPRITE[19].x = 2254;
         SPRITE[19].y = 302;
         SPRITE[19].width = 100;
         SPRITE[19].height = 10;

         SPRITE[20].type = PLATFORM;
         SPRITE[20].x = 1030;
         SPRITE[20].y = 200;
         SPRITE[20].width = 100;
         SPRITE[20].height = 10;
         SPRITE[20].platDistance = 350;
         SPRITE[20].xMovement = 2;
         SPRITE[20].yMovement = 2;

         SPRITE[21].type = PLATFORM;
         SPRITE[21].x = 239;
         SPRITE[21].y = 215;
         SPRITE[21].width = 100;
         SPRITE[21].height = 10;
         SPRITE[21].platDistance = 130;
         SPRITE[21].yMovement = 2;

         SPRITE[22].type = PLATFORM;
         SPRITE[22].x = 1340;
         SPRITE[22].y = 380;
         SPRITE[22].width = 150;
         SPRITE[22].height = 10;
         SPRITE[22].platDistance = 374;
         SPRITE[22].xMovement = 2;
         
         SPRITE[23].type = SWOOPER;
         SPRITE[23].originx = 1400;
         SPRITE[23].originy = 175;
         SPRITE[23].x = SPRITE[23].originx;
         SPRITE[23].y = SPRITE[23].originy;
         
         SPRITE[24].type = SPIKEY;
         SPRITE[24].originx = 2240;
         SPRITE[24].originy = 50;
         SPRITE[24].x = SPRITE[23].originx;
         SPRITE[24].y = SPRITE[23].originy;
         
         SPRITE[25].type = SWOOPER;
         SPRITE[25].originx = 1666;
         SPRITE[25].originy = 239;
         SPRITE[25].x = SPRITE[25].originx;
         SPRITE[25].y = SPRITE[25].originy;

         SPRITE[26].type = SWOOPER;
         SPRITE[26].originx = 1886;
         SPRITE[26].originy = 198;
         SPRITE[26].x = SPRITE[26].originx;
         SPRITE[26].y = SPRITE[26].originy;
         
         SPRITE[27].type = SWOOPER;
         SPRITE[27].originx = 2127;
         SPRITE[27].originy = 272;
         SPRITE[27].x = SPRITE[27].originx;
         SPRITE[27].y = SPRITE[27].originy;
         
         PLATFORM_HORI(28,1630,330,320)
         PLATFORM_VERT(29,1950,40,360)
         PLATFORM_DIAG(30,3320,255,220)
         PLATFORM_HORI(31,3524,420,470)
         PLATFORM_VERT(32,3990,140,290)
         PLATFORM_DIAG(33,3990,140,125)
         PLATFORM_HORI(34,4145,255,380)
         PLATFORM_HORI(35,4520,200,380)
         PLATFORM_VERT(36,4460,222,222)
         
         SPRITE[37].type = GROUND;
         SPRITE[37].x = 2282;
         SPRITE[37].y = 362;
         SPRITE[37].width = 278;
         SPRITE[37].height = 278;
         
         PLATFORM_VERT(38,5070,165,225)
         PLATFORM_VERT(39,5240,165,175)
         PLATFORM_DIAG(40,5320,150,185)
         PLATFORM_VERT(41,5650,120,310)
         
         GOOMBAENT(42,2119,40)
         GOOMBAENT(43,2264,35)
         
         /*PUT RING HERE, CIRCA 2285,167*/
         SPRITE[44].type = PUP;
         SPRITE[44].originx = 2295;
         SPRITE[44].originy = 167;
         SPRITE[44].x = SPRITE[44].originx;
         SPRITE[44].y = SPRITE[44].originy;
         
         SPIKEYENT(45,2525,320)
         GOOMBAENT(46,2910,290)
         GOOMBAENT(47,3111,290)
         SWOOPERENT(48,4085,250)
         SWOOPERENT(49,5200,110)
         //MORE ENTITIES MAKE BABY JESUS CRY
         
     }
         
     // setup beginning animation variables for any enemies or pups
     for(int i = 0; i < 50; i++){
             if(SPRITE[i].type == SPIKEY){
                 SPRITE[i].currentFrame = spikeywalkLeft[0];
                 SPRITE[i].framedelay = 3;
                 SPRITE[i].maxframe =  SPIKEYWALKFRAMES;
                 SPRITE[i].width = 30;
                 SPRITE[i].height = 30;
                 SPRITE[i].xspeed = 2;
                 SPRITE[i].dir = WEST;
             }
             if(SPRITE[i].type == PUP){
                 SPRITE[i].currentFrame = ring[0];
                 SPRITE[i].framedelay = 5;
                 SPRITE[i].maxframe = RINGFRAMES;
                 SPRITE[i].width = 16;
                 SPRITE[i].height = 16;
             }
             if(SPRITE[i].type == GOOMBA){
                 SPRITE[i].currentFrame = goombawalkLeft[0];
                 SPRITE[i].framedelay = 5;
                 SPRITE[i].maxframe = BONKFRAMES;
                 SPRITE[i].width = 30;
                 SPRITE[i].height = 30;
                 SPRITE[i].xspeed = 1;
                 SPRITE[i].dir = WEST;
                 SPRITE[i].stillAlive = true;
             }
             if(SPRITE[i].type == SWOOPER){
                 SPRITE[i].width = 36;
                 SPRITE[i].height = 14;
                 SPRITE[i].xspeed = 7;
                 SPRITE[i].hanging = true;
                 SPRITE[i].framedelay = 5;
                 SPRITE[i].maxframe = SWOOPERFRAMES;
                 SPRITE[i].stillAlive = true;
             }   
     }
                               
}

void checkGravity(int spriteNumber){
    bool collided = false;
    for(int i = 0; i < 50; i++){
            if((SPRITE[i].type == GROUND || SPRITE[i].type == PLATFORM || SPRITE[i].type == INVISIPLATFORM) && collided == false){
                  collided = checkSideCollision(spriteNumber, i, SOUTH);
                  if(collided == true)
                              SPRITE[spriteNumber].currentPlatform = i;
            }
    }
    if(collided == false){
                SPRITE[spriteNumber].y = SPRITE[spriteNumber].y + GRAVITY;
                SPRITE[spriteNumber].inAir = true;
    }
    if(collided == true){
                SPRITE[spriteNumber].inAir = false;
                // if a thrown ring hit the ground, start a new hop and set inAir back to true
                if(SPRITE[spriteNumber].type == RING && SPRITE[RING].stillAlive == true){
                        SPRITE[spriteNumber].hopping = true;
                        SPRITE[spriteNumber].inAir = true;
                        SPRITE[spriteNumber].timeInAir = 0;
                }
    }
}

bool checkSideCollision(int sprite1, int sprite2, int direction)
{
     int x1, y1, w1, h1;
     
     if(direction == SOUTH){
                  x1 = SPRITE[sprite1].x+15;
                  y1 = SPRITE[sprite1].y+SPRITE[sprite1].height-3;
                  w1 = 10;
                  h1 = 5;
     }
     //SPRITE[PLAYER].x+SPRITE[PLAYER].width, SPRITE[PLAYER].y+10, SPRITE[PLAYER].x+SPRITE[PLAYER].width+1, SPRITE[PLAYER].y+SPRITE[PLAYER].height-5
     if(direction == EAST){
                  x1 = SPRITE[sprite1].x+SPRITE[PLAYER].width-4;
                  y1 = SPRITE[sprite1].y+4;
                  w1 = 5;
                  h1 = 30;
     }
     if(direction == WEST){
                  x1 = SPRITE[sprite1].x-1;
                  y1 = SPRITE[sprite1].y+4;
                  w1 = 5;
                  h1 = 30;
     }
     if(direction == NORTH){
                  x1 = SPRITE[sprite1].x+15;
                  y1 = SPRITE[sprite1].y-1;
                  w1 = 10;
                  h1 = 4;
     }
     
     if(direction == STOMP){
                  x1 = SPRITE[sprite1].x+8;
                  y1 = SPRITE[sprite1].y+SPRITE[sprite1].height-3;
                  w1 = 24;
                  h1 = 5;
     }
     
     int x2 = SPRITE[sprite2].x;
     int y2 = SPRITE[sprite2].y;
     int w2 = SPRITE[sprite2].width;
     int h2 = SPRITE[sprite2].height;
      int left1, left2;
      int right1, right2;
      int top1, top2;
      int bottom1, bottom2;

            left1 = x1;
            left2 = x2;
            right1 = x1 + w1;
            right2 = x2 + w2;
            top1 = y1;
            top2 = y2;
            bottom1 = y1 + h1;
            bottom2 = y2 + h2;

            if (right2 > left1 && left2 < right1
            && bottom2 > top1 && top2 < bottom1)
            {
               return(true);
            }else{
               return(false);
            }        
}

bool checkBoxCollision(int sprite1, int sprite2)
{
      int left1, left2;
      int right1, right2;
      int top1, top2;
      int bottom1, bottom2;

            left1 = SPRITE[sprite1].x;
            left2 = SPRITE[sprite2].x;
            right1 = SPRITE[sprite1].x + SPRITE[sprite1].width;
            right2 = SPRITE[sprite2].x + SPRITE[sprite2].width;
            top1 = SPRITE[sprite1].y;
            top2 = SPRITE[sprite2].y;
            bottom1 = SPRITE[sprite1].y + SPRITE[sprite1].height;
            bottom2 = SPRITE[sprite2].y + SPRITE[sprite2].height;

            if (right2 > left1 && left2 < right1
            && bottom2 > top1 && top2 < bottom1)
            {
               return(true);
            }
            else{
               return(false);
            }
}

void initializeSprites(){
     for(int i = 0; i < 50; i++){
             SPRITE[i].x = 0;
             SPRITE[i].y = 0;
             SPRITE[i].xspeed = 0;
             SPRITE[i].yspeed = 0;
             SPRITE[i].width = 0;
             SPRITE[i].height = 0;
             SPRITE[i].curframe = 0;
             SPRITE[i].maxframe = 0;
             SPRITE[i].animdir = 0;
             SPRITE[i].framecount = 0;
             SPRITE[i].framedelay = 0;
             SPRITE[i].stillAlive = true;
             SPRITE[i].dying = false;
             SPRITE[i].xMovement = 0;
             SPRITE[i].yMovement = 0;
             SPRITE[i].platDistance = 0;
             SPRITE[i].jumping = false;
             SPRITE[i].ringThrown = false;
             SPRITE[i].type = NOSPRITE;
             SPRITE[i].inAir = false;
             SPRITE[i].timeInAir = 0;
             SPRITE[i].currentPlatform = 0;
             SPRITE[i].platCounter = 0;
             SPRITE[i].originx = 0;
             SPRITE[i].originy = 0;
             SPRITE[i].deathClock = 0;
             SPRITE[i].hanging = false;
             SPRITE[i].swooping = false;
             SPRITE[i].zipping = false;
             SPRITE[i].hangtime = 0;
             SPRITE[i].ziptime = 0;
             SPRITE[i].invincible = false;
             SPRITE[i].framecount = 0;
             SPRITE[i].framedelay = 0;
             SPRITE[i].maxframe = 0;
             SPRITE[i].curframe = 0;
             SPRITE[i].action = WALKRIGHT;
             SPRITE[i].idleCounter = 0;
             SPRITE[i].currentFrame = create_bitmap(40,40);
     }
}

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL); //changed and moved here
    set_volume(75,100); //added (sets volume of digi(wave) and midi.
                        //set to 75% volume on digi(wave) set to 100% volume on midi
                        //normalizes audio to be not as loud

	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}
