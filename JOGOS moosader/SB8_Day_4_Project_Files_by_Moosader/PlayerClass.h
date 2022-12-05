#pragma once
#include <allegro.h>

#define LEFT          4
#define RIGHT        6
#define DOWN        2
#define UP              8

class PlayerClass
{
    public:
        float x, y, speed, maxSpeed, frame;
        int w, h;
        int dir;
        int jumpTime;
        int jumpMax;
        int airTime;
        bool jumping;
        bool doubleJumping;
        bool falling;
        void Draw(BITMAP*, BITMAP*, float, float);
        void Move(int);
        void Jump();
        void NextFrame();
        void Update();
        PlayerClass();
};



