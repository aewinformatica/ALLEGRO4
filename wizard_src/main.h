#ifndef MAIN_H
#define MAIN_H

#define CELL_Y      16
#define CELL_X      16

#define ALIVE       1       //game states
#define DEAD        2

#define JUMP        1
#define LEFT        2
#define RIGHT       3       //directions
#define FALL        4
#define UP          5
#define NIN         6

#define MAXWALL     52
#define MAXCELL     74




// sounds
#define S_JUMP      0
#define S_HURT      1

// items
#define MORPH_BALL     0
#define SPIKE_BOOT     1
#define HIGH_JUMP      2

DATAFILE *game,*guy;
   BITMAP *double_buffer,*gamemap,*behind;
   PALLETE map_pal;
/*
extern volatile int mouse_x;
extern volatile int mouse_y;
extern volatile int mouse_b;
*/






int item[10]={0,0,0,0,0,0,0,0,0,0},
    move[10]={2,2,2,2,4,4,4,4,4,8},
    fall[8]={4,4,4,8,8,8,12,12},
    mjump[15]={32,24,16,16,16,12,12,8,8,8,4,4,4},
    mv=0,jv=3,fl=0,fv=0,jp=0,maxel,cnt=0,cv=1;



int cell_edit=0;

int Random(int u) {return rand() % u;}

int sx=66,sy=151,ssx=0,ssy=0;
int wleft,wright,wup,wdown;
//int qr=148,ql=76,qu=96,qd=132;
int qr=148,ql=80,qu=96,qd=128;
void play_sound(int samp);

void init_player();
void show_stats();

int check_elev(int x,int y);
int cell_value(int x,int y);
int floor();
int check_move(int o,int l);

void init_elevator();
void draw_elevator();
void move_elevator();

void get_screen(int x,int y);
void draw_screen(int k);

void draw_guy(void);
void behind_guy(void);
void erase_guy(void);

void save();
void edit_screen();
#endif
