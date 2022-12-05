//PlayerClass.cpp
#include "PlayerClass.h"

PlayerClass::PlayerClass()
{
    x = y = 0;
    speed = 6.0;
    w = h = 32;
    frame = 0;
    dir = RIGHT;
    jumping = doubleJumping = false;
    falling = true;
    jumpMax = 50;
}

void PlayerClass::Draw(BITMAP *buffer, BITMAP *sprite, float offsetX, float offsetY)
{
    if ( dir == LEFT )
        masked_blit(sprite, buffer, (int)frame*100, 0, x - offsetX, y - offsetY, w, h);
    else
        masked_blit(sprite, buffer, (int)frame*32, 32, x - offsetX, y - offsetY, w, h);
}

void PlayerClass::Update()
{
    if ( falling )
    {
        if ( airTime < 20 )
            airTime++;
        y += airTime/10;
    }
    if ( jumping )
    {
        jumpTime++;
        if ( jumpTime >= jumpMax )
        {
            jumping = false;
            doubleJumping = false;

        }
        else
        {
            y -= 7 - jumpTime/5;
        }
    }
}

void PlayerClass::Jump()
{
    if ( !jumping && !falling )
    {
        jumping = true;
        jumpTime = 0;
    }
    else if ( jumping && !doubleJumping && jumpTime >= jumpMax/2 )
    {
        doubleJumping = true;
        jumpTime = 0;
    }
}

void PlayerClass::Move(int direction)
{
    dir = direction;
    NextFrame();
    if ( direction == LEFT )
    {
        x -= speed;
    }
    else if ( direction == RIGHT )
    {
        x += speed;
    }
}

void PlayerClass::NextFrame()
{
    frame += 0.1;
    if ( frame > 3 )
        frame = 0;
}
