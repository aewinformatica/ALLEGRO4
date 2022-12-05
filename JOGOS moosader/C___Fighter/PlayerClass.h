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

const int LowestY = SCR_H - 150;
const int maxJump = 10;
const int invincibilityLength = 30;

class PlayerClass
{
    public:
        float x, y, speed, frameSpeed;
        bool isAttacking;
        bool isJumping;
        int w, h;
        int HP;
        float runFrame, attackFrame, jumpFrame, hitFrame;
        int action, spritecode;
        void draw(BITMAP*, BITMAP*);
        void setup(float, float, int, int, int, int);
        void move(int);
        void noMove() { action = STAND; }
        void attack();
        void jump();
        void update();
        void hit();
        bool invincible;
};

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
            if ( x + speed < SCR_W-w )
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

void PlayerClass::setup(float tx, float ty, int tw, int th, int tact, int tcode)
{
    if ( tcode == 1 )
        x = SCR_W - 64 - tw;
    else if ( tcode == 0 )
        x = 64;
    y = LowestY;
    w = tw;
    h = th;
    action = tact;
    spritecode = tcode;
    runFrame = attackFrame = 0;
    speed = 2.0;
    frameSpeed = 0.1;
    isAttacking = isJumping = false;
    HP = 10;
    invincible = false;
    hitFrame = 0;
}

void PlayerClass::hit()
{
    if ( !invincible )
    {
        invincible = true;
        hitFrame = invincibilityLength;
        HP -= 1;
    }
}

void PlayerClass::draw(BITMAP *buffer, BITMAP *image)
{
    if ( isAttacking )
        masked_blit(image, buffer, (int)attackFrame*w+640, spritecode*w, x, y, w, h);
    else if ( isJumping )
        masked_blit(image, buffer, 1152, spritecode*w, x, y, w, h);
    else if ( action == STAND )
        masked_blit(image, buffer, 0, spritecode*w, x, y, w, h);
    else if ( action == RUN )
        masked_blit(image, buffer, (int)runFrame*w+w, spritecode*w, x, y, w, h);
    if ( hitFrame > 0 )
    {
        ellipse(buffer, x+w/2, y+h/1.5, w/4+hitFrame/4, h/3+hitFrame, makecol(255, 255, 0));
        ellipse(buffer, x+w/2, y+h/1.5, w/4+hitFrame/2, h/3+hitFrame, makecol(255, 255, 255));
        ellipse(buffer, x+w/2, y+h/1.5, w/4+hitFrame, h/3+hitFrame, makecol(255, 255, 0));
    }
}


