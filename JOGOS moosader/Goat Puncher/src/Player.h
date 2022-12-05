#pragma once

#define JITENSHA 0
#define NOMI 1
#define SAKANA 2
#define TOIRE 3

enum Dir { LEFT = 0, RIGHT = 1 };
enum Act { IDLE, KICK, PUNCH };

const int PLAYER_W = 64;
const int PLAYER_H = 128;
const int GOAT_W = 128;
const int GOAT_H = 96;
const int PLAYER_MAXY = 320;
const int GOAT_MAXY = 352;

class Player
{
    private:
    public:
        bool goat;
        bool Jumping;
        int x, y, w, h, fx, fy, index;
        Dir dir;
        Act action;
        int speed;
        float vx, vy;
        
        void Setup( int i );
        
        void Move( Dir direction );
        
        void Action( Act type );
        
        void GiveVelocity( Dir kickDir );
        
        void Update();
};
