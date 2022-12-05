#pragma once
#include <string>
#include <allegro.h>
#include "Level.h"
#include "Global.h"


using namespace std;

enum { WASD = 0, IJKL = 1, ARROWS = 2, NUMPAD = 3, GAMEPAD1 = 4, GAMEPAD2 = 5, GAMEPAD3 = 6, GAMEPAD4 = 7 };
enum { IDLE = 0, WALK = 1, ATK1 = 2, ATK2 = 3, DEFEND = 4, JUMP = 5, PAIN = 6 };
enum { LEFT = 4, RIGHT = 6, UP = 8, DOWN = 2 };

float modifier = 11.2;
const float GRAVITY = 1.0;

class Player
{
    private:
        /*      FLOATS
            x = x coordinate, y = y coordinate, damage = HP or amt of damage acquired
            frame = tells program which frame to use, will be referenced as an int, but
                    since I want the frame to change slower than the counter runs, it will
                    be incremented by a small amount.
            hVelocity and vVelocity for when player is hit and flies backward.
            hurtTimer to make sure they have a bit of time to recover before getting hit again
            deathTimer is to keep track of which frame it should play when they explode
            defendTimer is how much time they have to block
                    INTEGER
            w = width, h = height,
            rLeft, rRight, rTop, rBottom = collision area, for bounding box
            img = which image it uses (will be an array of character filimstrips)
            ctrlScheme = # code for which control scheme they're using
            dir = direction
            action = walking, jumping, attacking, etc.
            jumpHeight = max jump height
            speed = movement speed
            jumpTimer = so they stop jumping after a certain amount of time =P
            color = the player's color, mainly used for the barrier circle color
                    MISC
            bool dead = if the player is dead or not
            bool cantJump = to make sure the player does not continue to jump after jumping
            string name = the character's name
            BITMAP *tempImg = used to flip the character's image, since the filmstrip is all one direction
            */

        float x, y, damage, frame, atk1, atk2, deathTimer,
            hVelocity, vVelocity, hurtTimer, defendTimer;
        int w, h, rLeft, rRight, rTop, rBottom, img, ctrlScheme,
                dir, action, jumpHeight, speed, jumpTimer, color, foodEaten;
        bool dead, cantJump;
        string name;
        BITMAP *temp;
        int carryItem;
    public:
        Player();
        float Hurt() { return hurtTimer; }
        bool Dead() { return dead; }
        float X() { return x; }
        void X( int value ) { x = value; }
        float Y() { return y; }
        void Y( int value ) { y = value; }
        void Y( float val ) { y = val; }
        int W() { return w; }
        int H() { return h; }
        int FoodEaten() { return foodEaten; }
        void IncFoodEaten() { foodEaten += 1; }
        void Frame( int value ) { frame = value; }
        int CtrlScheme() { return ctrlScheme; }
        int Img() { return img; }
        void Img( int val ) { img = val; }
        int JumpTimer() { return jumpTimer; }
        int Action() { return action; }
        void Action( int val ) { action = val; }
        float Damage() { return damage; }
        void AddHealth( float value );
        int Left() { return rLeft; }
        int Right() { return rRight; }
        int Top() { return rTop; }
        int Bottom() { return rBottom; }
        void SetCtrlScheme( int val ) { ctrlScheme = val; }
        void Draw( BITMAP *image, BITMAP *buffer, int index );
        void Move( int direction, Level *level );
        void IncrementFrame();
        void Setup( int newx, int newy, int newimage, int controls );
        void Update( Level *level );
        bool Jump( SAMPLE *sound );
        bool IsCollision( Level *level, int direction );
        float Atk( int type );
        void Hurt( float amt, int hDir, int vDir, Level *level );
        void Kill();
        void Push( int direction, Level *level );
};

void Player::AddHealth( float value )
{
    damage += value;
    if ( damage > 100 )
        damage = 100;
}

float Player::Atk( int type )
{
    if ( type == ATK1 )
        return atk1;
    else if ( type == ATK2 )
        return atk2;
}

void Player::Push( int hDir, Level *level )
{
    if ( !dead )
    {
        if ( hDir == LEFT )
            {
                if ( IsCollision( level, LEFT ) == false )
                    hVelocity = 10;
            }
            else if ( hDir == RIGHT )
            {
                if ( IsCollision( level, RIGHT ) == false )
                    hVelocity = -10;
            }
    }
}

void Player::Hurt( float amt, int hDir, int vDir, Level *level )
{
    if ( !dead )
    {
        action = PAIN;
        //player is hurt by someone else
        if ( hurtTimer < 0 )
        {
            hurtTimer = 10;
            if ( action == DEFEND && defendTimer > 0 )
                damage -= (amt*modifier)/2;
            else
                damage -= amt*modifier;
            if ( hDir == LEFT )
            {
                if ( IsCollision( level, LEFT ) == false )
                    hVelocity = 10;
            }
            else if ( hDir == RIGHT )
            {
                if ( IsCollision( level, RIGHT ) == false )
                    hVelocity = -10;
            }

            if ( vDir == UP )
            {
                if ( IsCollision( level, UP ) == false )
                    vVelocity = 10;
            }
            else if ( vDir == DOWN )
            {
                if ( IsCollision( level, DOWN ) == false )
                    vVelocity = -10;
            }
        }
    }
}

bool Player::Jump( SAMPLE *sound )
{
    if ( !dead )
    {
        if ( jumpTimer < 0 && cantJump == false )
        {
            //Can begin jump
            jumpTimer = 0;
            action = JUMP;
            play_sample( sound, 255, 128, 1000, false );
            return true;
        }
    }
    return false;
}

Player::Player()
{
    dead = false;
    defendTimer = 16;
    damage = 100;
    jumpTimer = -1;
    jumpHeight = 25;
    w = 16;
    h = 32;
    action = IDLE;
    frame = 0.0;
    speed = (int)1.0;
    dir = LEFT;
    rLeft = rTop = 0;
    rRight = 16;
    rBottom = 32;
}

void Player::Setup( int newx, int newy, int newimage, int controls )
{
    defendTimer = 16;
    carryItem = -1;
    action = IDLE;
    dead = false;
    hVelocity = -1;  //damage velocity
    vVelocity = -1;
    x = newx;
    y = newy;
    atk1 = 0.2;
    atk2 = 0.1;
    img = newimage;
    ctrlScheme = controls;
    jumpTimer = -1;
    cantJump = false;
    damage = 100;
    if ( img == NAME )
    {
        rTop = 8;
        rBottom = 32;
        rLeft = 1;
        rRight = 13;
        jumpHeight = 37;
        color = makecol( 0, 255, 0 );
        name = "name";
    }
    else if ( img == PLUM )
    {
        rTop = 2;
        rBottom = 32;
        rLeft = 3;
        rRight = 14;
        jumpHeight = 40;
        color = makecol( 150, 0, 150 );
        name = "plum";
    }
    else if ( img ==WONKY )
    {
        rTop = 3;
        rBottom = 32;
        rLeft = 0;
        rRight = 16;
        jumpHeight = 37;
        color = makecol( 255, 0, 0 );
        name = "wonky";
    }
    else if ( img == PHONICS )
    {
        rTop = 10;
        rBottom = 32;
        rLeft = 3;
        rRight = 11;
        jumpHeight = 40;
        color = makecol( 0, 0, 255 );
        name = "phonics";
    }
    else if ( img == RANK )
    {
        rTop = 10;
        rBottom = 32;
        rLeft = 3;
        rRight = 11;
        jumpHeight = 40;
        color = makecol( 0, 150, 0 );
        name = "rank";
    }
    else if ( img == MARXIO )
    {
        rTop = 10;
        rBottom = 32;
        rLeft = 3;
        rRight = 11;
        jumpHeight = 37;
        color = makecol( 255, 0, 0 );
        name = "marxio";
    }
    else if ( img == POOKIE )
    {
        rTop = 10;
        rBottom = 32;
        rLeft = 3;
        rRight = 11;
        jumpHeight = 45;
        color = makecol( 200, 200, 0 );
        name = "pookie";
    }
    else if ( img == MARS )
    {
        rTop = 10;
        rBottom = 32;
        rLeft = 3;
        rRight = 11;
        jumpHeight = 37;
        color = makecol( 255, 255, 0 );
        name = "mars";
    }
}

void Player::Move( int direction, Level *level )
{
    if ( !dead )
    {
        float tx = x;
    //    if ( collision == false )
        if ( IsCollision( level, direction ) == false )
        {
            if ( direction == LEFT )
                x -= speed;
            else
                x += speed;
            IncrementFrame();
        }

        dir = direction;
        if ( action != JUMP )
            action = WALK;
    }
}

void Player::IncrementFrame()
{
    if ( action == IDLE || action == DEFEND || action == JUMP || action == PAIN )
    {
        frame = 0;
    }
    else
    {
        frame += 0.1;
        if ( frame >= 4 )
        {
            frame = 0;
            if ( action == ATK1 || action == ATK2 )
                action = IDLE;
        }
    }
}

void Player::Kill()
{
    if ( !dead )
    {
        dead = true;
        deathTimer = 20;
    }
    damage = 0;
}

void Player::Draw( BITMAP *image, BITMAP *buffer, int index )
{
    if ( !dead )
    {
        temp = create_bitmap( w, h);
        rectfill( temp, 0, 0, w, h, makecol( 255, 0, 255 ) );

        if ( action != DEFEND && action != JUMP && action != PAIN )
            masked_blit( image, temp, (int)frame*16, action*32, 0, 0, w, h );
        else if ( action == DEFEND )
        {
            masked_blit( image, temp, 0, action*32, 0, 0, w, h );
            if ( defendTimer > 0 )
                circle( buffer, (int)(x+w/2), (int)(y+h/2), (int)((w + defendTimer)/2), color );
        }
        else if ( action == JUMP )
            masked_blit( image, temp, 0, action*32, 0, 0, w, h );
        else if ( action == PAIN )
            masked_blit( image, temp, 0, action*32, 0, 0, w, h );

        //damage bar
        if ( damage > 75 )
            rectfill( buffer, x, y, x+( 16 * damage/100 ), y+1, makecol( 0, 255, 0 ) );
        else if ( damage >= 25 && damage <= 75 )
            rectfill( buffer, x, y, x+( 16 * damage/100 ), y+1, makecol( 240, 130, 0 ) );
        else
            rectfill( buffer, x, y, x+( 16 * damage/100 ), y+1, makecol( 255, 0, 0 ) );

        if ( dir == LEFT )
            draw_sprite( buffer, temp, (int)x, (int)y );
        else
            draw_sprite_h_flip( buffer, temp, (int)x, (int)y );

//        textprintf_ex( buffer, font, x, y-10, makecol( 0, 0, 0 ), -1, "p%i", index );
        //Draw player # above head
        if ( index == 0 )
        {
            line( buffer, x+w/2, y-10, x+w/2, y-5, color );
            line( buffer, x+w/2, y-10, x+w/2-2, y-8, color );
        }
        else if ( index == 1 )
        {
            line( buffer, x+5, y-5, x+w-5, y-5, color );
            line( buffer, x+5, y-7, x+w-5, y-7, color );
            line( buffer, x+5, y-10, x+w-5, y-10, color );

            line( buffer, x+5, y-5, x+5, y-6, color );
            line( buffer, x+w-5, y-10, x+w-5, y-7, color );
        }
        else if ( index == 2 )
        {
            line( buffer, x+5, y-5, x+w-5, y-5, color );
            line( buffer, x+5, y-7, x+w-5, y-7, color );
            line( buffer, x+5, y-10, x+w-5, y-10, color );
            line( buffer, x+w-5, y-10, x+w-5, y-5, color );
        }
        else if ( index == 3 )
        {
            line( buffer, x+5, y-10, x+5, y-7, color );
            line( buffer, x+w-5, y-10, x+w-5, y-5, color );
            line( buffer, x+5, y-7, x+w-5, y-7, color );
        }

    }
    if ( deathTimer > 0 )
    {
        line( buffer, (int)(x-deathTimer*2), (int)(y+h/2), (int)(x+deathTimer*3), (int)(y+h/2), makecol( 255, 255, 255 ) );

        line( buffer, (int)(x-deathTimer*2), (int)(y+h/2+2), (int)(x+deathTimer*2), (int)(y+h/2+2), makecol( 255, 255, 255 ) );
        line( buffer, (int)(x-deathTimer*2), (int)(y+h/2-2), (int)(x+deathTimer*2), (int)(y+h/2-2), makecol( 255, 255, 255 ) );

        line( buffer, (int)(x-deathTimer), (int)(y+h/2+1), (int)(x+deathTimer), (int)(y+h/2+1), makecol( 255, 255, 255 ) );
        line( buffer, (int)(x-deathTimer), (int)(y+h/2-1), (int)(x+deathTimer), (int)(y+h/2-1), makecol( 255, 255, 255 ) );

        line( buffer, (int)(x-deathTimer), (int)(y+h/2+3), (int)(x+deathTimer), (int)(y+h/2+3), makecol( 255, 255, 255 ) );
        line( buffer, (int)(x-deathTimer), (int)(y+h/2-3), (int)(x+deathTimer), (int)(y+h/2-3), makecol( 255, 255, 255 ) );
    }
    //draw bounding box - for debug
//    rect( buffer, (int)x+rLeft, (int)y+rTop, (int)x+rRight, (int)y+rBottom, makecol( 255, 255, 255 ) );
}

void Player::Update( Level *level )
{
    if ( deathTimer > 0 )
        deathTimer -= 1.5;
    if ( !dead )
    {
        //Defending
        if ( action == DEFEND )
        {
            defendTimer -= 0.1;
        }
        else
        {
            if ( defendTimer < 16 )
                defendTimer += 0.05;
        }
        //Being attacked
        if ( hVelocity < 0 )
        {
            hVelocity += 1.0;
            if ( IsCollision( level, LEFT ) == false )
                x -= 1.0;
        }
        else if ( hVelocity > 0 )
        {
            hVelocity -= 1.0;
            if ( IsCollision( level, RIGHT ) == false )
                x += 1.0;
        }

        if ( vVelocity < 0 )
        {
            vVelocity += 1.0;
            if ( IsCollision( level, UP ) == false )
                y -= 1.0;
        }
        else if ( vVelocity > 0 )
        {
            vVelocity -= 1.0;
            if ( IsCollision( level, DOWN ) == false )
                y += 1.0;
        }

        if ( hurtTimer >= 0 )
            hurtTimer -= 1.0;

        //Attackingz
        if ( action == ATK1 || action == ATK2 )
            IncrementFrame();

        //JUMPING
        if ( jumpTimer >= 0 && jumpTimer < jumpHeight )
        {
            if ( IsCollision( level, UP ) == false )
            {
                y -= 2.0;
                jumpTimer++;
                cantJump = true;
            }
            else        //end jump
            {
                jumpTimer = -1;
            }
        }
        else if ( jumpTimer >= 25 )
        {
            jumpTimer = -1;
        }

            if ( IsCollision( level, DOWN ) == false )                                                  //GRAVITY
            {
                if (  x < 256-w/2 && x > 0-w/2 )
                    y += GRAVITY;
            }
            else
            {
                if ( action == JUMP )
                    action = WALK;
                cantJump = false;
            }
    }
}

bool Player::IsCollision( Level *level, int direction )
{
    bool collision = false;
    int amtY = 0, amtX = 0;
    int bottom = rBottom;
    int top = rTop;
    bool phase = false;
    int t;
    if ( direction == DOWN )
    {
        amtY = GRAVITY;
    }
    else if ( direction == UP )
    {
        top = 0;
        bottom = 16;
        phase = true;
    }
    else if ( direction == LEFT )
    {
        amtX -= speed;
        phase = true;
    }
    else if ( direction == RIGHT )
    {
        amtX += speed;
        phase = true;
    }
    for ( int i=0; i<MAXTILEX; i++ )
    {
        for ( int j=0; j<MAXTILEY; j++ )
        {
            if (               level->tile[0][i][j].Right() + level->tile[0][i][j].X()    !=        level->tile[0][i][j].Left() +       level->tile[0][i][j].X()
                    &&      level->tile[0][i][j].Top() +    level->tile[0][i][j].Y()    !=        level->tile[0][i][j].Bottom() + level->tile[0][i][j].Y()
                    && x+rRight+       amtX     >       level->tile[0][i][j].Left() +      level->tile[0][i][j].X()
                    && x+rLeft+         amtX     <       level->tile[0][i][j].Right() +    level->tile[0][i][j].X()
                    && y+bottom+     amtY     >       level->tile[0][i][j].Top() +       level->tile[0][i][j].Y()
                    && y+top+            amtY     <       level->tile[0][i][j].Bottom() + level->tile[0][i][j].Y() )
            {
                if ( ! phase )
                {
                    collision = true;
                }
                else
                {
                    t = level->tile[0][i][j].FX();
                    if ( t != 16 || t != 32 || t != 48 || t != 64 || t != 144 || t != 160 || t != 240 || t != 256 || t != 400 || t != 496 || t != 544 || t != 560 || t != 576 || t != 392 || t != 608 || t != 624 || t != 640 || t != 720 || t != 784 || t != 800 || t != 816 || t != 944 || t != 1056 || t != 1072 || t != 1088 )
                    {
                        collision = true;
                    }
                }
            }//if (               level->tile[0][i][j].Right() + level->tile[0][i][j].X()    !=        level->tile[0][i][j].Left() +       level->tile[0][i][j].X()...
            else if (     level->tile[1][i][j].Right() + level->tile[1][i][j].X()    !=       level->tile[1][i][j].Left() +       level->tile[1][i][j].X()
                    &&      level->tile[1][i][j].Top() +    level->tile[1][i][j].Y()    !=      level->tile[1][i][j].Bottom() + level->tile[1][i][j].Y()
                    && x+rRight+       amtX     >       level->tile[1][i][j].Left() +      level->tile[1][i][j].X()
                    && x+rLeft+         amtX     <       level->tile[1][i][j].Right() +    level->tile[1][i][j].X()
                    && y+bottom+     amtY     >       level->tile[1][i][j].Top() +       level->tile[1][i][j].Y()
                    && y+top+           amtY     <       level->tile[1][i][j].Bottom() + level->tile[1][i][j].Y() )
            {
                if ( ! phase )
                {
                    collision = true;
                }
                else
                {
                    t = level->tile[1][i][j].FX();
                    if ( t != 16 || t != 32 || t != 48 || t != 64 || t != 144 || t != 160 || t != 240 || t != 256 || t != 400 || t != 496 || t != 544 || t != 560 || t != 576 || t != 392 || t != 608 || t != 624 || t != 640 || t != 720 || t != 784 || t != 800 || t != 816 || t != 944 || t != 1056 || t != 1072 || t != 1088 )
                    {
                        collision = true;
                    }
                }
            }//else if (     level->tile[1][i][j].Right() + level->tile[1][i][j].X()    !=       level->tile[1][i][j].Left() +       level->tile[1][i][j].X() ...
        }
    }
    return collision;
}










