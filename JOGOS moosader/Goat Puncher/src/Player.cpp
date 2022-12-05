#include "Player.h"

void Player::Setup( int i )
{
    goat = false;
    vx = 0;
    vy = 0;
    speed = 3;
    index = i;
    fx = fy = 0;
    y = 200;
    w = 210;
    h = 210;
    switch( i )
    {
        case JITENSHA:
           x = 210; 
        break;
        case NOMI:
            x = 210;
        break;
        case SAKANA:
            x = 1;
        break;
        case TOIRE:
            x = 210;
        break;
        case 5:
            y = 352;
            x = 320;
            w = 210;
            h = 120;
            goat = true;
        break;
    }
}

void Player::Move( Dir direction )
{
    if ( direction == LEFT )
    {
        dir = direction;
        fy = 0;
        x -= speed;
    }
    else if ( direction == RIGHT )
    {
        dir = direction;
        fy = PLAYER_H;
        x += speed;
    }
}

void Player::Action( Act type )
{
    action = type;
    if ( type == KICK )
    {
        fx = 210;
    }
    else if ( type == PUNCH )
    {
        fx = 420;
    }
    else if ( type == IDLE )
    {
        fx = 0;
    }
}

void Player::GiveVelocity( Dir kickDir )
{
    if ( kickDir == LEFT )
    {
        if ( goat )
            vx = -20;
        else
            vx = -15;
    }
    else if ( kickDir == RIGHT )
    {
        if ( goat )
            vx = 20;
        else
            vx = 15;
    }
    
    if ( goat )
        vy = -10;
    else
        vy = -10;
}

void Player::Update()
{
    float friction = 1.0;
    
    if ( vx > 1 || vx < -1 )
    {
        x = (int)(x + vx);
    }
    else
    {
        vx = 0;
    }
    
    if ( vy < -1 )
    {
        y = (int)(y + vy);
        vy += friction;
    }
    else if ( vy > 1 ) //speed up (falling)
    {
        y = (int)(y + vy);
        vy += friction;
    }
    else // between -1 and 0
    {
        vy = 2;
    }
    
    //slow down velocity
    if ( vx < 0 )
        vx += friction;
    else if ( vx > 0 )
        vx -= friction;

    // no more velocity once hit the ground
    
    int maxY;
    if ( goat ) 
        maxY = GOAT_MAXY;
    else
        maxY = PLAYER_MAXY;
    
    if ( y >= maxY )
    {
        vy = 0;
        y = maxY;
        //slow down x too
        if ( vx < 0 )
            vx += friction;
        else if ( vx > 0 )
            vx -= friction;
    }
    
    if ( x < 0 )
        x = 0;
    else if ( x+32 > 640 )
        x = 640-64;
}


