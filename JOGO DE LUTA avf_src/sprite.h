#ifndef SPRITE_H
#define SPRITE_H



extern int debug;



#define SPRITE_DONE 1



typedef struct SPRITE SPRITE;
typedef struct ANIMATION ANIMATION;
typedef struct VECTOR VECTOR;
typedef struct BOX BOX;



struct ANIMATION
{
        int first_frame;    // first frame in animation
        int length;         // number of frames in animation
        int max_frame_time; // how long to show each frame in this anim
        int loop;           // whether we want this anim to loop or not
};



struct VECTOR
{
        int x, y;
};



struct BOX
{
        int x1, y1;
        int x2, y2;
};



struct SPRITE
{
        int flags;
        BITMAP *image;
        VECTOR speed;
        VECTOR size;
        VECTOR pos;
        BOX bound;
        int default_anim;
        int current_anim;
        int current_frame;
        int frame_timer;
        ANIMATION *anims;
        DATAFILE *data;
};



void play_sprite (SPRITE *s);
void reset_sprite (SPRITE *s);
void move_sprite (SPRITE *s);
int collision (SPRITE *s1, SPRITE *s2);



#endif

