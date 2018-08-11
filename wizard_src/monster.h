#ifndef MONSTER_H
#define MONSTER_H
#define MAXMON      10

typedef struct monsters
{
    int state;
    int x;
    int y;
    int dx;
    int dy;
    int dir;
    int type;
    int hp;
} monsters;

void init_mon();
void draw_mon();
void move_mon();
#endif
