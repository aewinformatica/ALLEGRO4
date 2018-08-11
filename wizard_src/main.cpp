#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "allegro.h"
#include "monster.h"
#include "player.h"
#include "elevator.h"
#include "music.h"
#include "main.h"
#include "data.h"
#include "guy.h"

monsters mon[MAXMON];
main_player player;
elevator elev[10];



void play_music(){
    play_midi((MIDI*)game[MIDI001].dat, TRUE);
}


void play_sound(int samp){
  play_sample((SAMPLE*)game[SAMP001+samp].dat, 130, 130, 1000, FALSE);
}


void init_player(){
player.x=120;
player.y=160;
player.dir=RIGHT;
player.small=0;
player.frame=12;
player.hp=100;
}

void show_stats(){
  char msg[80];
     //text_mode(-1); DEPRECATED
      sprintf(msg, "HP:%d", player.hp);
      textout_ex(double_buffer,(FONT*)game[FONT_1].dat,msg, 10,10,15,-1);
if (player.hp<=0) {exit(0);}
}

int check_elev(int x,int y){
int i;
for (i=0; i<maxel; i++){
    
    if (elev[i].state==ALIVE){
        if (
             x>(elev[i].x-12) &&
             x<(elev[i].x+44) &&
             y>=(elev[i].y-32) &&
             y<=(elev[i].y-28)
           ) {
           player.y=elev[i].y-32;
           return(1);
           }
       }
}
return(0);
}

int cell_value(int x,int y){
int p,q;
p=MAXWALL;
if (getpixel(double_buffer,x,y)>0) {

q=getpixel(gamemap,((x+ssx)>>4)+sx,((y+ssy)>>4)+sy);

if (q==MAXCELL+1) {item[MORPH_BALL]=1;
    putpixel(gamemap,((x+ssx)>>4)+sx,((y+ssy)>>4)+sy,MAXCELL-2);}
if (q==MAXCELL+2) {item[SPIKE_BOOT]=1;
    putpixel(gamemap,((x+ssx)>>4)+sx,((y+ssy)>>4)+sy,MAXCELL-2);}
if (q==MAXCELL+3) {item[HIGH_JUMP]=1;
    putpixel(gamemap,((x+ssx)>>4)+sx,((y+ssy)>>4)+sy,MAXCELL-2);}
if (q==MAXCELL+4) {exit(0);}
if (q==11 && cnt==0 && item[SPIKE_BOOT]==0) {cnt=10;player.hp-=6;
                                              play_sound(S_HURT);}


return(getpixel(gamemap,((x+ssx)>>4)+sx,((y+ssy)>>4)+sy)); }
return(MAXWALL);
}

int floor(){
int i;
         for (i=player.x+4; i<player.x+11; i++)
          {
            if (cell_value(i,player.y+32)<MAXWALL) {fl=0;fv=0;return(0);}
          }
    if (check_elev(player.x,player.y)==1) {fl=0;fv=0;return(0);}
return(1);
}

int check_move(int o,int l){
int y1,i,dn=0,tx=0,j,t=0;
if (player.small) {y1=player.y+16;}
      else {y1=player.y+2;}
if (o==LEFT) {player.frame++;if (player.frame<8 || player.frame>11) {player.frame=8;}
     for (j=1; j<l; j++)
       {
         for (i=y1; i<player.y+32; i++)
          {
            if (cell_value(player.x-j+4,i)<MAXWALL)
                                         {player.x=player.x-tx; return(1);}
                                    else

            if (cell_value(player.x-j+4,i)==MAXCELL)
                                         {player.x=204;sx=wleft-15;
                                          if (ssx<0) {ssx=0;sx--;}
                                          return(2);}
                                    else {
                                          t=1;}
          }
       if (t==1) {t=0;tx++;}
       }
            player.x=player.x-tx;
            return(0);
            }

if (o==RIGHT) {player.frame++;if (player.frame<12 || player.frame>15) {player.frame=12;}
     for (j=1; j<l; j++)
       {
         for (i=y1; i<player.y+32; i++)
          {
            if (cell_value(player.x+j+11,i)<MAXWALL)
                                         {player.x=player.x+tx; return(1);}
                                      else
            if (cell_value(player.x+j+11,i)==MAXCELL)
                                         {player.x=20;sx=wright+1;
                                          if (ssx>0) {ssx=0;sx++;}
                                          return(2);}
                                      else {t=1;}
          }
       if (t==1) {t=0;tx++;}
       }
            player.x=player.x+tx;
            return(0);
            }

if (o==UP) {
     for (j=player.y+6; j<player.y+16; j++)
       {
         for (i=player.x+4; i<player.x+12; i++)
          {
            if (cell_value(i,j)<MAXWALL)
                                         {return(1);}
          }
       }
            return(0);
            }
if (o==JUMP) {
     for (j=1; j<l; j++)
       {
         for (i=player.x+4; i<player.x+12; i++)
          {
            if (cell_value(i,player.y+6-j)<MAXWALL)
                                         {player.y=player.y-j; return(1);}
                                    else {t=1;}
          }
       if (t==1) {t=0;tx++;}
       }
            player.y=player.y-tx;
            return(0);
            }
if (o==FALL) {
     for (j=1; j<l; j++)
       {
         for (i=player.x+4; i<player.x+11; i++)
          {
            if (cell_value(i,player.y+j+31)<MAXWALL)
                                         {player.y=player.y+tx; return(1);}
                                    else {t=1;}
          }
          if (check_elev(player.x,player.y+j-1)==1) {return(1);}
          if (t==1) {t=0;tx++;}
       }
            player.y=player.y+tx;
            return(0);
            }
    return 0;
}


void init_elevator(){
int x,y;
maxel=0;
for (x=wleft; x<=wright; x++)
 {
  for (y=wup; y<=wdown; y++)
    {
    if (getpixel(gamemap,x,y)==MAXCELL+7)
      {
        elev[maxel].x     = (x-sx)*16;
        elev[maxel].y     = (y-sy)*16;
        elev[maxel].state = ALIVE;
        elev[maxel].dir   = Random(2)+2;
        maxel++;
      }
    }
 }
}

void draw_elevator(){
int i;
for (i=0; i<maxel; i++)
    {
    if (elev[i].state==ALIVE)
       {
         if (elev[i].x>-32 && elev[i].x<240 &&
             elev[i].y>-32 && elev[i].y<240)
           {
       draw_sprite(double_buffer,(BITMAP*)guy[ELEV01].dat,elev[i].x   ,elev[i].y);
       draw_sprite(double_buffer,(BITMAP*)guy[ELEV01].dat,elev[i].x+16,elev[i].y);
       draw_sprite(double_buffer,(BITMAP*)guy[ELEV01].dat,elev[i].x+32,elev[i].y);
           }
       }
    }
}



void move_elevator(){
int i;

for (i=0; i<maxel; i++)
    {
    if (elev[i].state==ALIVE)
       {
        if (elev[i].dir==LEFT)
            {
            if (getpixel(gamemap,((elev[i].x+ssx-1)>>4)+sx,((elev[i].y+ssy)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((elev[i].x+ssx-1)>>4)+sx,((elev[i].y+ssy)>>4)+sy)==MAXCELL)
                     {elev[i].dir=RIGHT;} else {
                                            if (
                                                 player.x>(elev[i].x-12) &&
                                                 player.x<(elev[i].x+44) &&
                                                 player.y==(elev[i].y-32)
                                               ) {
                            if (check_move(LEFT,2)==1) {}   }
                                                elev[i].x--;}
            }
          else
        if (elev[i].dir==RIGHT)
            {
            if (getpixel(gamemap,((elev[i].x+ssx+48)>>4)+sx,((elev[i].y+ssy)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((elev[i].x+ssx+48)>>4)+sx,((elev[i].y+ssy)>>4)+sy)==MAXCELL)
                     {elev[i].dir=LEFT;} else {
                                            if (
                                                 player.x>(elev[i].x-12) &&
                                                 player.x<(elev[i].x+44) &&
                                                 player.y==(elev[i].y-32)
                                               ) {
                            if (check_move(RIGHT,2)==1) {}   }
                                                elev[i].x++;}
            }
       }
    }
}

void get_screen(int x,int y){
int xx,yy,dn=0;
ssx=ssy=0;
xx=x;
while (!dn)
{
xx--;
if (getpixel(gamemap,xx,y)==0 ||
    getpixel(gamemap,xx,y)==MAXCELL){
    wleft=xx;
    dn=1;}
}
dn=0;

xx=x;
while (!dn){
xx++;
if (getpixel(gamemap,xx,y)==0 ||
    getpixel(gamemap,xx,y)==MAXCELL) {
    wright=xx;
    dn=1;
    }
}
dn=0;

yy=y;
while (!dn){
yy--;
if (getpixel(gamemap,x,yy)==0 ||
    getpixel(gamemap,x,yy)==MAXCELL) {
    wup=yy;
    dn=1;
    }
}
dn=0;

yy=y;
while (!dn)
{
yy++;
if (getpixel(gamemap,x,yy)==0 ||
    getpixel(gamemap,x,yy)==MAXCELL){
    wdown=yy;
    dn=1;
    }
}
dn=0;
}

void draw_screen(int k){

int cell_id,dx,dy,dn=0,osx=0,osy=0,   // the cell id
    ossx,ossy,
    index_x,index_y,i;// looping variables

osx=sx;osy=sy;
ossx=ssx;ossy=ssy;


if (player.x>qr && sx<(wright-14)) {dx=player.x-qr;player.x=qr;
                         ssx=ssx+dx;
                         if (ssx>16) {sx++;ssx=ssx-16;}
                         if (sx>=(wright-14)) {sx=wright-14;ssx=0;}
                         if (sx==wleft && ssx!=0) {sx++;ssx-=16;}
                        }
if (player.x<ql && sx>wleft) {dx=ql-player.x;player.x=ql;
                       ssx=ssx-dx;
                       if (ssx<-16) {sx--;ssx=ssx+16;}
                       if (sx<=wleft) {sx=wleft;ssx=0;}
                       if (sx==(wright-14) && ssx!=0) {sx--;ssx+=16;}
                      }

if (player.y>qd && sy<(wdown-14)) {dy=player.y-qd;player.y=qd;
                         ssy=ssy+dy;
                         if (ssy>16) {sy++;ssy=ssy-16;}
                         if (sy>=(wdown-14)) {sy=wdown-14;ssy=0;}
                         if (sy==wup && ssy!=0) {sy++;ssy-=16;}
                        }
if (player.y < qu && sy > wup){
                dy=qu-player.y;player.y=qu;
                ssy=ssy-dy;
                if (ssy<-16){
                sy--;
                ssy=ssy+16;
                }
                if (sy<=wup){
                sy=wup;
                ssy=0;
                }
                if (sy==(wdown-14) && ssy!=0){
                sy--;
                ssy+=16;
                }
}
//DESENHANDO O MAPA
for (index_y=(-1); index_y<CELL_Y; index_y++){
    for (index_x=(-1); index_x<CELL_X; index_x++){
        cell_id = getpixel(gamemap,index_x+sx,index_y+sy);
        
        if (k==0){ //usando o zero
           if (cell_id >=MAXCELL+5){
               cell_id=MAXCELL-2;
           }
        }
        blit((BITMAP*)game[WALL000+cell_id].dat,double_buffer, 0, 0, (index_x << 4)-ssx, (index_y << 4)-ssy, 16, 16);
        
        if (k==1){//usando um
                if (cell_id >= MAXWALL){
                    putpixel(double_buffer, (index_x << 4)-ssx+8, (index_y << 4)-ssy+8,73);
                }
                if (cell_id == 0){
                putpixel(double_buffer, (index_x << 4)-ssx+8, (index_y << 4)-ssy+8,0);
                }
        }
    }
}

for (i=0; i<MAXMON; i++){
    if (mon[i].state==ALIVE){
         mon[i].x+=16*(osx-sx)+(ossx-ssx);
         mon[i].y+=16*(osy-sy)+(ossy-ssy);
    }
    if (elev[i].state==ALIVE){
         elev[i].x+=16*(osx-sx)+(ossx-ssx);
         elev[i].y+=16*(osy-sy)+(ossy-ssy);
    }
}
show_stats();
}

void draw_guy(void){
int i=0;
if (cnt>0) {//se for maior
cnt--; //diminui
cv=-cv;//diminui

       if (cnt==0) { //se for igual a zero
       cnt=0; // sem necessidade
       cv=1; //cv descobrir o que e
       } 
}
       if (cv!=1) {
       i=OGUY01-GUY01;
       }
       
if (!player.small){// se for diferente de grande

   if (jp!=0 || fl!=0){ //difente de caindo e pulando
             
        if (player.dir==LEFT) {player.frame=8;}
        if (player.dir==RIGHT) {player.frame=12;} 
   }

     draw_sprite(double_buffer,(BITMAP*)guy[GUY01+i+player.frame].dat, player.x, player.y);//GRANDE
}else
 draw_sprite(double_buffer,(BITMAP*)guy[GUY01+i+player.frame-8].dat, player.x, player.y+16);//PEQUENO
}

void behind_guy(void){
        blit(double_buffer,behind, player.x,player.y,0,0,16,16);
}

void erase_guy(void){
        blit(behind,double_buffer, 0,0,player.x,player.y,16,16);
}

void save(){
int p;
p=save_pcx("map.map", gamemap, map_pal);
}

void edit_screen(){
int done=0,x,y,e,p;
BITMAP *mouse; 
        mouse = load_bitmap("mouse.bmp",NULL);
        set_mouse_sprite(mouse);

show_mouse(screen);

p=cell_edit;
draw_screen(1);
acquire_screen();

blit(double_buffer, screen, 0, 0, 0, 0,240,240);

release_screen();

while (!done){
        blit((BITMAP*)game[WALL000+p].dat,screen, 0, 0, 0,0,16,16);
        if (p==0){
        putpixel(screen, 8,8,0);
        }
        
if (key[KEY_A]) {p--;rest(150);if (p<0) {p=0;}}
if (key[KEY_S]) {p++;rest(150);if (p>MAXCELL) {p=MAXCELL;}}
if (key[KEY_1]) {p=0;}
if (key[KEY_2]) {p=MAXWALL;}
if (key[KEY_3]) {p=MAXCELL-2;}
if (key[KEY_4]) {p=MAXCELL;}
if (key[KEY_Z]) {p=MAXCELL+1;}
if (key[KEY_X]) {p=MAXCELL+2;}
if (key[KEY_C]) {p=MAXCELL+3;}
if (key[KEY_V]) {p=MAXCELL+4;}
if (key[KEY_B]) {p=MAXCELL+5;}
if (key[KEY_N]) {p=MAXCELL+6;}
if (key[KEY_M]) {p=MAXCELL+7;}



  if ( (mouse_b & 1) || (mouse_b & 2) ){
         x=mouse_x+ssx;
         y=mouse_y+ssy;
         
         if (mouse_b & 1) {e=p;}
         
         if (mouse_b & 2) {e=MAXCELL-2;}
         show_mouse(NULL);
         
         putpixel(gamemap,(x>>4)+sx,(y>>4)+sy,e);
         draw_screen(1);
         
         blit((BITMAP*)game[WALL000+p].dat,double_buffer, 0, 0, 0,0,16,16);
         
         if (p==0){
                   putpixel(double_buffer, 8,8,73);
         }
         acquire_screen();
         blit(double_buffer, screen, 0, 0, 0, 0,240,240);
         release_screen();
         show_mouse(screen);
            }
  if (key[KEY_Q]){
  done=1;
  }
}
save();
show_mouse(NULL);
cell_edit=p;
}

void init_mon(){
int x,y,i=0;
for (i=0; i<MAXMON; i++)
    {
    mon[i].state=DEAD;
    }
i=0;
for (x=wleft; x<=wright; x++)
  for (y=wup; y<=wdown; y++)
    {
    if (getpixel(gamemap,x,y)==MAXCELL+5)
                {i++;
                 mon[i].state=ALIVE;
                 mon[i].x=(x-sx)*16;
                 mon[i].y=(y-sy)*16;
               if (Random(2)==0) {mon[i].dx=1;} else {mon[i].dx=-1;}
               if (Random(2)==0) {mon[i].dy=1;} else {mon[i].dy=-1;}
                 mon[i].dir=0;
                 mon[i].type=1;
                 mon[i].hp=100;
                }
    if (getpixel(gamemap,x,y)==MAXCELL+6)
                {i++;
                 mon[i].state=ALIVE;
                 mon[i].x=(x-sx)*16;
                 mon[i].y=(y-sy)*16;
                 mon[i].dx=0;
                 mon[i].dy=0;
                 mon[i].dir=1;
                 mon[i].type=2;
                 mon[i].hp=100;
                }
    }
}

void draw_mon(){
int i,j;

for (i=0; i<MAXMON; i++)
    {
    if (mon[i].state==ALIVE)
       {
         if (mon[i].x>-4 && mon[i].x<236 &&
             mon[i].y>-4 && mon[i].y<236)
           {
    if (mon[i].type==1)  {j=MON01;}
    if (mon[i].type==2)  {j=MON02;}
       draw_sprite(double_buffer,(BITMAP*)guy[j].dat,mon[i].x,mon[i].y);
           }
       }
    }
}

void move_mon(){
int i,j=0,x,y,t=1,g=1;

for (i=0; i<MAXMON; i++)
    { t=1,g=1;
  if (mon[i].state==ALIVE)
     {
    if (mon[i].type==1)
       {
          if (mon[i].dx==1)  {x=mon[i].x+13;}
          if (mon[i].dx==-1) {x=mon[i].x+2;}
          if (mon[i].dy==1)  {y=mon[i].y+13;}
          if (mon[i].dy==-1) {y=mon[i].y+2;}


            if (getpixel(gamemap,((x+ssx+mon[i].dx)>>4)+sx,((y+ssy)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((x+ssx+mon[i].dx)>>4)+sx,((y+ssy)>>4)+sy)==MAXCELL)
                     {mon[i].dx=-mon[i].dx;}
                          else
                     {mon[i].x+=mon[i].dx;}
            if (getpixel(gamemap,((x+ssx)>>4)+sx,((y+ssy+mon[i].dy)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((x+ssx)>>4)+sx,((y+ssy+mon[i].dy)>>4)+sy)==MAXCELL)
                     {mon[i].dy=-mon[i].dy;}
                          else
                     {mon[i].y+=mon[i].dy;}
      }

    if (mon[i].type==2)
       {
         if (mon[i].dir==1) {
            if (getpixel(gamemap,((mon[i].x+ssx-1)>>4)+sx,((mon[i].y+ssy)>>4)+sy)>MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx-1)>>4)+sx,((mon[i].y+ssy)>>4)+sy)<MAXCELL &&
                getpixel(gamemap,((mon[i].x+ssx-1)>>4)+sx,((mon[i].y+ssy+15)>>4)+sy)>MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx-1)>>4)+sx,((mon[i].y+ssy+15)>>4)+sy)<MAXCELL)
                     {mon[i].dir=2;mon[i].x--;} else
            if (getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)==MAXCELL)
                     {mon[i].dir=4;
            if (getpixel(gamemap,((mon[i].x+ssx+16)>>4)+sx,((mon[i].y+ssy)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((mon[i].x+ssx+16)>>4)+sx,((mon[i].y+ssy)>>4)+sy)==MAXCELL)
                        {mon[i].dir=3;}
                     }
                                    }
                           else
         if (mon[i].dir==2) {
            if (getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy+16)>>4)+sy)>MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy+16)>>4)+sy)<MAXCELL &&
                getpixel(gamemap,((mon[i].x+ssx+15)>>4)+sx,((mon[i].y+ssy+16)>>4)+sy)>MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx+15)>>4)+sx,((mon[i].y+ssy+16)>>4)+sy)<MAXCELL)
                     {mon[i].dir=3;mon[i].y++;} else
            if (getpixel(gamemap,((mon[i].x+ssx-1)>>4)+sx,((mon[i].y+ssy)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((mon[i].x+ssx-1)>>4)+sx,((mon[i].y+ssy)>>4)+sy)==MAXCELL)
                     {mon[i].dir=1;
            if (getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)==MAXCELL)
                        {mon[i].dir=4;}
                    }
                                    }
                           else
         if (mon[i].dir==3) {
            if (getpixel(gamemap,((mon[i].x+ssx+16)>>4)+sx,((mon[i].y+ssy)>>4)+sy)>MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx+16)>>4)+sx,((mon[i].y+ssy)>>4)+sy)<MAXCELL &&
                getpixel(gamemap,((mon[i].x+ssx+16)>>4)+sx,((mon[i].y+ssy+15)>>4)+sy)>MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx+16)>>4)+sx,((mon[i].y+ssy+15)>>4)+sy)<MAXCELL)
                     {mon[i].dir=4;mon[i].x++;} else
          if (getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy+16)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy+16)>>4)+sy)==MAXCELL)
                     {mon[i].dir=2;
            if (getpixel(gamemap,((mon[i].x+ssx-1)>>4)+sx,((mon[i].y+ssy)>>4)+sy)<=MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx-1)>>4)+sx,((mon[i].y+ssy)>>4)+sy)==MAXCELL)
                        {mon[i].dir=1;}
                    }
                                    }
                           else
         if (mon[i].dir==4) {
            if (getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)>MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)<MAXCELL &&
                getpixel(gamemap,((mon[i].x+ssx+15)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)>MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx+15)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)<MAXCELL)
                     {mon[i].dir=1;mon[i].y--;} else
            if (getpixel(gamemap,((mon[i].x+ssx+16)>>4)+sx,((mon[i].y+ssy)>>4)+sy)<=MAXWALL ||
                getpixel(gamemap,((mon[i].x+ssx+16)>>4)+sx,((mon[i].y+ssy)>>4)+sy)==MAXCELL)
                     {mon[i].dir=3;
            if (getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)<=MAXWALL &&
                getpixel(gamemap,((mon[i].x+ssx)>>4)+sx,((mon[i].y+ssy-1)>>4)+sy)==MAXCELL)
                        {mon[i].dir=2;}
                      }
                                    }
        if (mon[i].dir==1) {mon[i].y--;}
        if (mon[i].dir==2) {mon[i].x--;}
        if (mon[i].dir==3) {mon[i].y++;}
        if (mon[i].dir==4) {mon[i].x++;}

       }
     } // end of if ALIVE
  } // 1 to MAXMON
} // end of procedure

void main(void)
{
   int done=0,game_play=0,kl=NIN,i;

   allegro_init();
   install_keyboard(); 
   install_timer();
   install_music();
   install_mouse();  
   
   set_color_depth(8);
   set_gfx_mode(GFX_AUTODETECT, 320,240, 0, 0);

    game=load_datafile("data.dat");
    guy=load_datafile("guy.dat");
   gamemap = load_pcx("map.map",map_pal);

   double_buffer = create_bitmap(SCREEN_W, SCREEN_H);
   clear(double_buffer);
   
   behind = create_bitmap(16,16);
   clear(behind);

   srand((int)time(NULL));
   set_pallete((RGB*)game[PALLETE_01].dat);
   
   play_music();
   
   get_screen(sx+8,sy+8); // SCROLL DA TELA
   
   init_player();
   init_mon();
   init_elevator();

while (!game_play){

while (!done){
      
if (kl==LEFT || kl==RIGHT) {
        get_screen(sx+8,sy+8);
        
        kl=NIN;
        
        if (sy < wup) {
        player.y = player.y-16*(wup-sy)-ssy;
        sy=wup;
        ssy=0;
        }
        
        if (sy >(wdown-14)) {
        player.y = player.y+16*(sy-(wdown-14))+ssy;
        sy = wdown-14;
        ssy = 0;
        }
        init_mon();
        init_elevator();
}
     draw_screen(0);
     behind_guy();
     draw_guy();
     draw_mon();
     draw_elevator();
     acquire_screen();
     blit(double_buffer, screen, 0, 0, 0, 0,240,240);
     release_screen();
     rest(60);
     erase_guy();
     for (i=0; i<4; i++){
         move_mon();
         move_elevator();
     }

if (key[KEY_LEFT]){
                   
            if (player.dir==RIGHT) {
            mv=0;
            }
            
            mv++;
            
            if (mv==10) {
            mv=9;
            }
            
            player.dir=LEFT;
            
            if (check_move(LEFT,move[mv])==2){
            kl=LEFT;
            }
}

if (key[KEY_RIGHT]){
    
        if (player.dir==LEFT){
        mv=0;
        }
      
      mv++;
      if (mv==10) {
      mv=9;
      }
      player.dir=RIGHT;
      if (check_move(RIGHT,move[mv])==2) {
      kl=RIGHT;
      }
}

if (kl==NIN){
             
    if (key[KEY_UP]){
        if (player.small && check_move(UP,0)==0){
         player.small=0;
         jp=0;
        } 
    }

if (key[KEY_DOWN]) {
    if (!player.small && item[MORPH_BALL]==1){
     player.small=1;
     jp=3;
    } 
}

if ( (key[KEY_SPACE]) && jp==0 && fl==0) {
     jp=1;
     jv=3;
     fl=1;
     play_sound(S_JUMP);
     play_sample((SAMPLE*)game[SAMP001].dat, 130, 130, 1000, FALSE);
     
     if (item[HIGH_JUMP]==1) {
     jv=0;
     } 
}

if (!(key[KEY_SPACE]) && !player.small) {
      jp=0;
}

if ( (key[KEY_SPACE]) && jp==1) {
    jv++;
    if (jv>14) {jp=2;fv=0;} 
    else {
       if (check_move(JUMP,mjump[jv])==1) {
        jp=2;fv=0;
       }
    }
}
if (jp!=1)
     if (floor()==1) {
        fv++;
        if (fv>7){
          fv=7;
        }
        if (check_move(FALL,fall[fv])==0){
        
        }
     }else {
            fl=0;
     }
}

if (key[KEY_E]){

edit_screen();

}
//wagner
control_music();

if (key[KEY_1]) {item[MORPH_BALL]=1;}
if (key[KEY_2]) {item[SPIKE_BOOT]=1;}
if (key[KEY_3]) {item[HIGH_JUMP]=1;}


if (key[KEY_ESC])  {done=1;game_play=1;}

}
done=0;
}

    destroy_bitmap(double_buffer);
    destroy_bitmap(gamemap);
    destroy_bitmap(behind);
}
END_OF_MAIN();

