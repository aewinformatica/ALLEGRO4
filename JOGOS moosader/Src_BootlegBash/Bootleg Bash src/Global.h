#pragma once
#include "Player.h"
#include "Level.h"

enum { PLUM = 0, RANK = 1, MARXIO = 2, POOKIE = 3, NAME = 4, MARS = 5, WONKY = 6, PHONICS = 7 };

bool IsCollision( int top1, int bottom1, int left1, int right1, int top2, int bottom2, int left2, int right2 )
{
    if ( left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2 )
        return true;
    return false;
}
