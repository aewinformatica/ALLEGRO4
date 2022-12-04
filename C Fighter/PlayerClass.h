#pragma once
#include <iostream>
#include <allegro.h>
#include "global.h"
using namespace std;

#define STAND    0
#define RUN         1
#define ATTACK  2
#define LEFT       4
#define RIGHT     6
#define UP           8
#define DOWN     2

const int LowestY = SCR_H-150;
const int maxJump = 15;
const int invincibilityLength = 30;


class PlayerClass
{
    public:
        float x, y; 
        float speed, frameSpeed;
        bool isAttacking;
        bool isJumping;
        int w, h;
        int HP;
        
        PlayerClass();
        
        BITMAP *parado[9];
        BITMAP *andando[7];
        BITMAP *t_teia[10];
        BITMAP *pulando[5];
        
        float runFrame, attackFrame, jumpFrame, hitFrame;
        int action, spritecode,dir;
        void draw(BITMAP*, BITMAP*);
        void setup(float, float, int, int, int, int,int);
        void move(int);
        void noMove() { action = STAND; }
        void attack();
        void jump();
        void update();
        void hit();
        bool invincible;
};




