#pragma once
#include <allegro.h>
#include "Level.h"

enum { HEAL = 0 };

class Item
{
    private:
        int x, y, w, h, img;
        int rBottom, rTop, rLeft, rRight;
        float frame;
        bool exists;
        float lifeSpan;
        int carriedBy;
        int use;
    public:
        int X() { return x; }
        void X( int value ) { x = value; }
        int Y() { return y; }
        void Y( int value ) { y = value; }
        int W() { return w; }
        void W( int value ) { w = value; }
        int H() { return h; }
        void H( int value ) { h = value; }
        bool Exists() { return exists; }
        void Exists( bool value ) { exists = value; }
        int Bottom() { return rBottom; }
        int Top() { return rTop; }
        int Left() { return rLeft; }
        int Right() { return rRight; }
        float Frame() { return frame; }
        void IncrementFrame();
        void Setup( int tx, int ty, int tw, int th, int timg );
        void Update( int tx, int ty );
        void Update( Level *level );
        bool IsCollision( Level *level, int direction);
        int Carrier() { return carriedBy; }
        void Carrier( int value ) { carriedBy = value; }
        int Use() { return use; }
        Item()
        {
            exists = false;
        }
        void Draw( BITMAP *image, BITMAP *buffer );
};

void Item::Draw( BITMAP *image, BITMAP *buffer )
{
    if ( exists )
        masked_blit( image, buffer, (int)frame*16, img*16, x, y - 10, w, h );
}

bool Item::IsCollision( Level *level, int direction )
{
    bool collision = false;
    int amtY = 0, amtX = 0;
    int bottom = rBottom;
    int top = rTop;
    bool phase = false;
    int t;
    amtY = GRAVITY;
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

void Item::Setup( int tx, int ty, int tw, int th, int timg )
{
    carriedBy = -1;
    lifeSpan = 0;
    x = tx;
    y = ty;
    w = tw;
    h = th;
    img = timg;
    rTop = rLeft = 0;
    rBottom = rRight = 16;
    exists = true;
    use = HEAL;
}

void Item::Update( int tx, int ty )
{
    //being carried
    x = tx;
    y = ty;
    IncrementFrame();
}

void Item::Update(Level *level)
{
    lifeSpan += 0.01;
    if ( lifeSpan >= 10 )
        exists = false;
    IncrementFrame();
     if ( IsCollision( level, DOWN ) == false )                                                  //GRAVITY
    {
        if (  x < 256-w/2 && x > 0-w/2 )
            y += GRAVITY;
    }
}

void Item::IncrementFrame()
{
    frame += 0.1;
    if ( frame >= 2.0 )
        frame = 0;
}