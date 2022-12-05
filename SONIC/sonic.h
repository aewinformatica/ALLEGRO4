#ifndef SONIC
   #define SONIC
   #include <string>
   #include <cstdlib>
   #include <iostream>
   #include <allegro.h>
   
   #define SPIKEYENT(NUM,X,Y) SPRITE[NUM].type = SPIKEY; SPRITE[NUM].originx = X; SPRITE[NUM].originy = Y; SPRITE[NUM].x = SPRITE[NUM].originx; SPRITE[NUM].y = SPRITE[NUM].originy;
#define GOOMBAENT(NUM,X,Y) SPRITE[NUM].type = GOOMBA; SPRITE[NUM].originx = X; SPRITE[NUM].originy = Y; SPRITE[NUM].x = SPRITE[NUM].originx; SPRITE[NUM].y = SPRITE[NUM].originy;
#define SWOOPERENT(NUM,X,Y) SPRITE[NUM].type = SWOOPER; SPRITE[NUM].originx = X; SPRITE[NUM].originy = Y; SPRITE[NUM].x = SPRITE[NUM].originx; SPRITE[NUM].y = SPRITE[NUM].originy;         
#define PLATFORM_HORI(NUM,X,Y,DIST) SPRITE[NUM].type=PLATFORM; SPRITE[NUM].x=X; SPRITE[NUM].y=Y; SPRITE[NUM].width=100; SPRITE[NUM].height=10; SPRITE[NUM].platDistance=DIST; SPRITE[NUM].xMovement = 2;
#define PLATFORM_VERT(NUM,X,Y,DIST) SPRITE[NUM].type=PLATFORM; SPRITE[NUM].x=X; SPRITE[NUM].y=Y; SPRITE[NUM].width=100; SPRITE[NUM].height=10; SPRITE[NUM].platDistance=DIST; SPRITE[NUM].yMovement = 2;
#define PLATFORM_DIAG(NUM,X,Y,DIST) SPRITE[NUM].type=PLATFORM; SPRITE[NUM].x=X; SPRITE[NUM].y=Y; SPRITE[NUM].width=100; SPRITE[NUM].height=10; SPRITE[NUM].platDistance=(DIST *2); SPRITE[NUM].xMovement = 2;SPRITE[NUM].yMovement = 2;
   
   #define TEXT makecol(255,255,255)
   #define RED makecol(255,0,0)
   #define PURPLE makecol(172,49,247)
   #define YELLOW makecol(255,255,0)
   #define BLUE makecol(0,128,255)
   #define GREEN makecol(0,204,0)
   
   #define NORTH 0
   #define SOUTH 1
   #define EAST 2
   #define WEST 3
   #define NODIRECTION 4
   #define STOMP 5 // this is used to check the bottom of the player's sprite if they stomped on an enemy (the collision box needed to be bigger)
   #define NUMDIR 6
   
   #define PLAYER 0
   #define GOOMBA 1 // slow, normal moving enemy
   #define PLATFORM 2 // this is used for moving platforms mostly. It can be jumped through and the player can land on it
   #define INVISIPLATFORM 3 // this platform does not draw a graphic but it can be jumped through and landed on
   #define PUP 4 // Power up sprite
   #define RING 5 // This represents a ring that is thrown by the player
   #define GROUND 6 // ground is something the player can land on but cannot jump through or walk through
   #define NOSPRITE 7 // if a sprite is not used it will have this type
   #define SPIKEY 8 // enemy that cannot be stomped on
   #define SWOOPER 9 // enemy that flies
   #define NUMSPRITES 10
   
   #define STOMPED 0 // if a player kills an enemy by stomping on it
   #define SHOT 1 // if a player kills an enemy by shooting it with a ring
   
   #define WALKLEFT 0
   #define WALKRIGHT 1
   #define JUMPLEFT 2
   #define JUMPRIGHT 3
   #define PLAYERSTANDLEFT 4
   #define PLAYERSTANDRIGHT 5
   #define NUMACTIONS 6
   
   #define PLAYERWALKFRAMES 6
   #define PLAYERJUMPFRAMES 4
   #define PLAYERIDLEFRAMES 2
   #define SPIKEYWALKFRAMES 4
   #define RINGFRAMES 4
   #define BONKFRAMES 4
   #define TITLEFRAMES 4
   #define SWOOPERFRAMES 4
   
   struct SPRITE{
        int x,y, originx, originy; // originx and y are the original coordinates a sprite is set before it is moved
        int xspeed, yspeed;
        int width,height;
        int xdelay, ydelay;
        int xcount,ycount;
        int curframe,maxframe,animdir;
        int framecount, framedelay;
        int action; // used to determine what animation to play
        bool stillAlive; // check to see if player or enemies are still alive
        bool dying; // use to play the dying animation
        int xMovement; //platform x movement
        int yMovement; //platform y movement
        int platDistance; // distance the platform will move back and forth
        bool jumping; // if the player is in the middle of a jump this will be true
        bool hopping; // if the player is hopping due to stomping on an enemy, this will be true
        bool ringThrown; // if the player has the powerup and he throws a ring this will be true
        int type; // type of sprite: PLAYER, ENEMY, PLATFORM ect...
        bool poweredUp; // player has acquired the powerup
        bool inAir; // if the player is not standing on a platform of some sort, this will be false: used to make sure player isn't jumping in mid air
        int timeInAir; // amount of time player or enemy has been in the air
        int currentPlatform; // current platform player/enemy is on. Used to determine whether they are moving with it or not and for enemy movement
        int platCounter; // how far the platform has moved from it's original position
        int level; // what level the player is on
        int deathClock; // the object has died and is playing the dying animation, this tells how long the animation has been going
        int dir;
        bool invincible;
        int invincibleTime;
        int idleCounter;
        BITMAP *currentFrame;
        
        
        // the following variables are used by the swooper ONLY
        bool hanging, swooping, zipping; // hanging is wait period before an attack, swooping is leveling swooper's ycoordinate with the player's and zipping is when it zips across the screen at player
        int hangtime, ziptime;
   }SPRITE[50];
   
   /* DUSTIN: THE CODE HAS GOTTEN PRETTY JUMBLED AND IT'S HARD TO SEE WHAT'S HAPPENING IN SOME OF IT.
      HOPEFULLY THESE NOTES WILL HELP YOU FIND THE BEST PLACES TO INSERT YOUR ANIMATION DATA AND HELP EXPLAIN SOME OF THE MESS
   
   * Use the dying and deathClock variables to help control animations for a sprite that's dying.
   deathClock increments 1 every loop and can be used to control how long the dying animation will be. Right now it is set to last
   for 150 loops but change it as you see fit. The best place to update the frames for a dying sprite would probably be toward the end of the
   main loop. There's a for loop controlling all dying sprites. Right now, after deathClock reaches it's limit (150 or whatever), dying retuns to
   false and the stillAlive variable for the sprite goes to true which keeps it from being drawn and makes the sprite disappear. Just letting you know
   in case you'd like to change any of it.
   
   * I have a killPlayer() function but all it does right now is reset the player back to the original start point. If you'd like a death animation
   or a music sound to play when the player dies, you could put it in this function with a while loop or set the PLAYER's dying variable to true and
   go from there but know that this function only gets called once so you couldn't update frame animation here unless you put it in a while loop. 
   Other things that you might add could be a "lives left" screen like old Mario.
   
   * FYI, the way the code is written for moving platforms right now will not work if you try to make a platform move northeast. Yes it could be written
   to work so it would work but it's a pain in the ass and it'll probably be easier if we just don't have platforms that move northeast. Feel free to 
   fix it if you'd like though.
   
   * To make an animation if the player is standing still,  changing dir to NODIRECTION if the player isn't pressing any keys might be a good way. 
   You may want to make a time variable that increments whenever the player isn't pressing any keys if you don't want the "Player Idle" animation 
   to play right away
   
   * Enemy movement is handled in a for loop at the beginning of the main game loop. That's probably the best place to update any enemy movement
   animations. The GOOMBA only moves WEST so you won't need to worry about direction of animation. Spikey moves EAST and WEST but the dir variable is
   already updated in the moveSpikey() function so you'll already know which way he's going. SWOOPER only moves one direction, WEST. So don't worry
   about his dir either.
   
   
*/
   
   
   int camerax = 0;
   int playerMiddlex = SCREEN_W/2 - 50;
   
   BITMAP *ground;
   BITMAP *buffer;
   BITMAP *level2;
   BITMAP *level1;
   BITMAP *teamscreen;
   BITMAP *sonicwalkLeft[6];
   BITMAP *sonicwalkRight[6];
   BITMAP *sonicjumpLeft[4];
   BITMAP *sonicjumpRight[4];
   BITMAP *sonicstandLeft;
   BITMAP *sonicstandRight;
   BITMAP *sonicidleRight[2];
   BITMAP *sonicidleLeft[2];
   BITMAP *spikeywalkLeft[4];
   BITMAP *spikeywalkRight[4];
   BITMAP *ring[4];
   BITMAP *goombawalkLeft[4];
   BITMAP *goombawalkRight[4];
   BITMAP *titlesprite[4];
   BITMAP *swoopersprite[4];
   BITMAP *duckright;
   BITMAP *duckleft;
   BITMAP *upright;
   BITMAP *upleft;

   
   void setupSprites(); // intialize sprite variables
   void drawSprites(); // draw sprites to the buffer
   void checkGravity(int); // make a sprite drop if it's not standing on something
   bool checkBoxCollision(int, int); // check collision of two sprites
   bool checkSideCollision(int, int, int); // check collision of two boxes based on a side
   void updateJump(int); // update a player's/enemy's jump
   void initializeSprites();
   void updatePlatform(int); // update moving platforms
   void playerHitSprite(int); // determines what to do based on what sprite the player hit
   void killBadGuy(int, int); //handles everything if a player killed the badguy: parameters (spriteKilled, methodKilledi.e(STOMPED OR SHOT))
   void killPlayer(); //speaks for itself
   void updateHop(int); // update a player's/enemy's/ring's hop
   void moveSpikey(int); //moves an enemy back and forth on whatever platform they're on
   void moveSwooper(int); // moves an enemy in a flying/swooping down motion
   void updateInvincibility();
   void killRing(); // ring thrown is dead
   void updateFrame(int); // update animation for a sprite
   void spriteDying(int);
   void newLevel();
   void moveGoomba(int);
   
   MIDI *level1music;
   MIDI *level2music;
   MIDI *titlemusic;
   
   SAMPLE *jumpsound;
   SAMPLE *killenemy;
   SAMPLE *powerup;
   SAMPLE *ringthrow;
   SAMPLE *teamsound;
    
    BITMAP *grabframe(BITMAP *source,int width, int height,int startx, int starty,int columns, int frame)
    {
    BITMAP *temp = create_bitmap(width,height);
    
    int x = startx + (frame % columns) * width;
    int y = starty + (frame / columns) * height;
    
    blit(source,temp,x,y,0,0,width,height);


    return temp;
    }

    
   #endif
   
   
   
   

