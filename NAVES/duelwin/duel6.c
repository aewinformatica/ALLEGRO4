#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "allegro.h"
//#include <ppcol.h>
//#include <dime.h>
#include "duel.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

volatile int sec;
int cycles=0;
int esc=0;
int out=0;
int part_n=0;

void startup(void);
void one_loop(void);
void go_sec(void);
int collide(int x1,int y1,int x2,int y2, int xx1,int yy1,int xx2,int yy2);
void menu(void);
void set_game(void);
void animate(int x, int y, int n);
//fixed trig(float fx1,float fy1, float fx2, float fy2);

void explosion(int x, int y);
void win(int n);
int check_col(BITMAP *b1, BITMAP *b2,float x1,float y1,float x2,float y2);

typedef struct
	{
     float x,y;
     float move_x,move_y;
     float m_x,m_y;
     float speed, angle;
     float max_speed;
     BITMAP *bmp;

     int damage;
     int type;
     int target;
     int fired;
     int wep;

     fixed a2;
    } proj_set;

typedef struct
	{
     char name[20];
     BITMAP *bmp;
     SAMPLE *smp;
     int damage;
     int type;
     int max_ammo;
     float max_speed;
     int wait;
    } wep_set;


struct
	{
     float x,y;
     float move_x,move_y;
     float m_x,m_y;
     float speed, max_speed, accel;
     float angle;

     proj_set proj[200];

     int number;
     int wep_selected;
     int wait;
     BITMAP *bmp;

     int s_up;
  	 int s_angle;
     int life;
     int frags;
     int ammo[10];

     int counter;
    } s[4];

struct
	{
     float x,y;
     float move_x,move_y;
     float speed;
     int colour;
     int angle;
     int exist;
    } part[1500];


struct
	{
     int x,y;
     BITMAP *bmp[10];
     int number_of_frames;
     int frame;
    } anim[3];


BITMAP *backgr, *shot;
DATAFILE *data;
RGB *pa;

wep_set wep[10];

int number_of_players=2;

void go_sec()
	{
    sec++;
	}
END_OF_FUNCTION(go_sec);

int main(void)
	{
     startup();
     while (out==0)
      {
       menu();
	   sec=0;
	   cycles=0;
       while(esc==0)//when not trying to leave game
         {

          while(sec*2>cycles && esc==0)      //regulates time
            one_loop();
         }


      }
    unload_datafile(data);
    destroy_bitmap(backgr);
    }
END_OF_MAIN();

void startup(void)
     {
     char * noise;
     int x,y,n,n2;
     allegro_init();
     install_keyboard();
     install_timer();
     install_mouse();
     set_gfx_mode(GFX_AUTODETECT,640,480,0,0);
     install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
     srand(time(0));

     //dime_init();

     data=load_datafile("duel.dat");
     set_pallete(data[pal].dat);

     pa=data[pal].dat;

     backgr=create_bitmap(SCREEN_W,SCREEN_H);

     LOCK_VARIABLE(sec);
     LOCK_FUNCTION(go_sec);

     if(( color_map = malloc( sizeof( COLOR_MAP) )) == NULL)
        {
                printf("Unable to allocate colour map!\n");
                exit( 1);
        }
     create_trans_table(color_map,data[pal].dat, 70, 70, 70, NULL);//168


   /*  s[1].x=560;
     s[1].y=400;
     s[1].bmp=data[ship2].dat;
     s[1].speed=0;
     s[1].max_speed=5;
     s[1].accel=.1;
     s[1].angle=0;
     s[1].move_x=0;
     s[1].move_y=0;
     s[1].number=0;
     s[1].wep_selected=0;
     s[1].wait=0;
     s[1].s_up=0;
     s[1].life=100;
     s[1].frags=0;
     s[1].counter=0;
     for(n=0;n<200;n++)
        s[1].proj[n].fired=0; */

     strcpy(wep[0].name,"Main Gun");
     wep[0].bmp=data[shot1].dat;
     wep[0].max_speed=5;
     wep[0].type=1;
     wep[0].damage=5;
     wep[0].wait=7;
     wep[0].smp=data[shoot1].dat;
     wep[0].max_ammo=1000;

     strcpy(wep[1].name,"Tri-Shot");
     wep[1].bmp=data[shot1].dat;
     wep[1].max_speed=5;
     wep[1].type=2;
     wep[1].damage=5;
     wep[1].wait=7;
     wep[1].smp=data[shoot1].dat;
     wep[1].max_ammo=300;

     strcpy(wep[2].name,"Wrath Missile");
     wep[2].bmp=data[missile].dat;
     wep[2].max_speed=4;
     wep[2].type=3;
     wep[2].damage=20;
     wep[2].wait=20;
     wep[2].smp=data[m_shoot].dat;
     wep[2].max_ammo=30;

     strcpy(wep[3].name,"Spray Shot");
     wep[3].bmp=data[shot1].dat;
     wep[3].max_speed=4;
     wep[3].type=4;
     wep[3].damage=20;
     wep[3].wait=20;
     wep[3].smp=data[shoot1].dat;
     wep[3].max_ammo=2;

     anim[0].bmp[0]=data[play1].dat;
     anim[0].bmp[1]=data[play2].dat;
     anim[0].bmp[2]=data[play3].dat;
     anim[0].number_of_frames=2;
     anim[0].frame=0;

     anim[1].bmp[0]=data[exit1].dat;
     anim[1].bmp[1]=data[exit2].dat;
     anim[1].bmp[2]=data[exit3].dat;
     anim[1].number_of_frames=2;
     anim[1].frame=0;

     anim[2].bmp[0]=data[win1].dat;
     anim[2].bmp[1]=data[win2].dat;
     anim[2].bmp[2]=data[win3].dat;
     anim[2].bmp[3]=data[win4].dat;
     anim[2].number_of_frames=3;
     anim[2].frame=0;

     set_game();

     set_volume(200,200);
     text_mode(-1);

     play_midi(data[fates].dat,1);


     install_int_ex(go_sec, BPS_TO_TIMER(20));
     }

void one_loop(void)
	{
    int n,n2,c,ty=0,plus=0;
    cycles++;
    rectfill(backgr,0,0,SCREEN_W,SCREEN_H,0);

    if(rand()%60 == 0)
      {
       draw_sprite(backgr,data[lightning1].dat,rand()%640,10);
       play_sample(data[clap1].dat,160,128,1000,0);
      }
    else if(rand()%160 == 1)
      {
       draw_sprite(backgr,data[lightning2].dat,rand()%640,10);
       play_sample(data[clap2].dat,160,128,1000,0);
      }

    if(rand()%700 ==0)
      {
       rectfill(backgr,0,0,SCREEN_W,SCREEN_H,3);
       draw_sprite(backgr,data[lightning1].dat,rand()%640,10);
       play_sample(data[clap1].dat,250,128,1000,0);
       play_sample(data[clap2].dat,250,128,1000,0);
      }

    rect(backgr,10,10,630,430,252);

    draw_sprite(backgr,data[flames].dat,0,0);

    if(key[KEY_P])
      {
       shot=create_sub_bitmap(screen,0,0,SCREEN_W,SCREEN_H);
       save_bitmap("screenshot.pcx",shot,pa);
       destroy_bitmap(shot);
      }

    if(key[KEY_ESC])
       esc=1;

     if(key[KEY_UP] && s[1].speed<s[1].max_speed)
      		{
             s[1].speed+=s[1].accel;
             if(cycles%3==0)
               play_sample(data[going].dat,250,128,1000,0);
			}

     else if(s[1].speed>=s[1].accel)
         s[1].speed-=s[1].accel;

     if(key[KEY_RIGHT])
           {
            s[1].angle+=3;
           }
     else if(key[KEY_LEFT])
           {
            s[1].angle-=3;
           }

     if(key[KEY_L] && s[1].wep_selected<3 && s[1].counter==0)
	  {
       s[1].wep_selected++;
       s[1].counter=12;
      }
     else if(key[KEY_K] && s[1].wep_selected>0 && s[1].counter==0)
       {
        s[1].counter=12;
       s[1].wep_selected--;
       }

     if(key[KEY_M] && s[1].wait==0 && s[1].ammo[s[1].wep_selected]>0)
       {
        s[1].ammo[s[1].wep_selected]--;
        if(wep[s[1].wep_selected].type==2)
          ty=3;
        else if(wep[s[1].wep_selected].type==4)
          ty=128;
        else
          ty=1;

        for(n=0;n<ty;n++)
           {
            if(n==0)
              plus=0;
            else if(n==1 && ty==3)
              plus=3;
            else if(n==2 && ty==3)
              plus=-3;
            else if(ty==128)
              {
               plus=n*2;
              }

         //   if (wep[s[1].wep_selected].type==3)
         //     s[1].proj[s[1].number].target=1;

        	s[1].proj[s[1].number].fired=1;
        	s[1].proj[s[1].number].x=s[1].x+s[1].bmp->w/2+s[1].move_x*2;
        	s[1].proj[s[1].number].y=s[1].y+s[1].bmp->h/2+s[1].move_y*2;

			s[1].proj[s[1].number].move_y = -cos( ((s[1].angle+plus)*1.40625) *M_PI/180);
            s[1].proj[s[1].number].move_x = sin ( ((s[1].angle+plus)*1.40625) *M_PI/180);

        	s[1].proj[s[1].number].angle=s[1].angle;

        	s[1].proj[s[1].number].damage=wep[s[1].wep_selected].damage;
        	s[1].proj[s[1].number].max_speed=wep[s[1].wep_selected].max_speed;
        	s[1].proj[s[1].number].bmp=wep[s[1].wep_selected].bmp;
        	s[1].wait=wep[s[1].wep_selected].wait;
        	s[1].proj[s[1].number].wep=s[1].wep_selected;
        	s[1].proj[s[1].number].speed=5;
        	s[1].number++;

            if(n==0)  //play sound on first loop only
        		play_sample(wep[s[1].wep_selected].smp,100,128,1300,0);

        	if(s[1].number>=190)
          		s[1].number=0;
            }
       }

    if(key[KEY_W] && s[0].speed<s[0].max_speed)
      		{
             s[0].speed+=s[0].accel;
             if(cycles%3==0)
               play_sample(data[going].dat,250,128,1000,0);
			}
     else if(s[0].speed>=s[0].accel)
         s[0].speed-=s[0].accel;

     if(key[KEY_D])
           {
            s[0].angle+=3;
           }
     else if(key[KEY_A])
           {
            s[0].angle-=3;
           }

     if(key[KEY_X] && s[0].wep_selected<3 && s[0].counter==0)
       {
       s[0].wep_selected++;
       s[0].counter=12;
       }
     else if(key[KEY_Z] && s[0].wep_selected>0 && s[0].counter==0)
       {
       s[0].wep_selected--;
       s[0].counter=12;
       }

    if(key[KEY_F] && s[0].wait==0 && s[0].ammo[s[0].wep_selected]>0)
       {
        s[0].ammo[s[0].wep_selected]--;
        if(wep[s[0].wep_selected].type==2)
          ty=3;
        else if(wep[s[0].wep_selected].type==4)
          ty=128;
        else
          ty=1;

        for(n=0;n<ty;n++)
           {
            if(n==0)
              plus=0;
            else if(n==1 && ty==3)
              plus=3;
            else if(n==2 && ty==3)
              plus=-3;
            else if(ty==128)
              {
               plus=n*2;
              }

            if (wep[s[0].wep_selected].type==3)
              s[0].proj[s[0].number].target=0;

        	s[0].proj[s[0].number].fired=1;

        	s[0].proj[s[0].number].x=s[0].x+s[0].bmp->w/2+s[0].move_x*2;
        	s[0].proj[s[0].number].y=s[0].y+s[0].bmp->h/2+s[0].move_y*2;
			s[0].proj[s[0].number].move_y = -cos( ((s[0].angle+plus)*1.40625) *M_PI/180);
            s[0].proj[s[0].number].move_x = sin ( ((s[0].angle+plus)*1.40625) *M_PI/180);

        	s[0].proj[s[0].number].angle=s[0].angle;

        	s[0].proj[s[0].number].damage=wep[s[0].wep_selected].damage;
        	s[0].proj[s[0].number].max_speed=wep[s[0].wep_selected].max_speed;
        	s[0].proj[s[0].number].bmp=wep[s[0].wep_selected].bmp;
        	s[0].wait=wep[s[0].wep_selected].wait;
        	s[0].proj[s[0].number].wep=s[0].wep_selected;
        	s[0].proj[s[0].number].speed=5;
        	s[0].number++;

            if(n==0)
        		play_sample(wep[s[0].wep_selected].smp,100,128,1300,0);
        	if(s[0].number>=200)
          		s[0].number=0;
       		}
       }

    for(n=0;n<number_of_players;n++)
       {
      if(s[n].wait>0)
        s[n].wait--;

      s[n].move_y = -cos( (s[n].angle*1.40625) *M_PI/180);
	  s[n].move_x = sin( (s[n].angle*1.40625) *M_PI/180);


      for(n2=0;n2<200;n2++)
         {
          if(s[n].proj[n2].fired==1)
            {
           // if(s[n].proj[n2].type==3)
           //    {
                /*s[n].proj[n2].a2=findangle(s[n].proj[n2].x,s[n].proj[n2].y,
                                      s[s[n].proj[n2].target].x,s[s[n].proj[n2].target].y);
                /*s[n].proj[n2].angle=fixtof(s[n].proj[n2].a2);*/
            //    }

              if(s[n].proj[n2].speed < wep[s[n].proj[n2].wep].max_speed)
                s[n].proj[n2].speed+=.5;

              s[n].proj[n2].x+=s[n].proj[n2].move_x*s[n].proj[n2].speed;
              s[n].proj[n2].y+=s[n].proj[n2].move_y*s[n].proj[n2].speed;

              rotate_sprite(backgr,s[n].proj[n2].bmp,
                            s[n].proj[n2].x,s[n].proj[n2].y,
                            ftofix(s[n].proj[n2].angle));

             if(s[0].proj[n2].fired==1)
			  if (check_col(s[1].bmp,s[0].proj[n2].bmp
                  ,s[1].x,s[1].y,s[0].proj[n2].x,s[0].proj[n2].y) &&  n==0)
                {
                 play_sample(data[shield].dat,250,128,1000,0);
                 s[1].s_up=15;
        		 s[1].s_angle=s[0].proj[n2].angle+128;
                 s[1].life-=s[0].proj[n2].damage;
                 s[0].proj[n2].fired=0;

                 if(s[1].life<=0)
                   {
                    play_sample(data[explode2].dat,250,128,1000,0);
                    play_sample(data[explode1].dat,250,128,1000,0);
                    play_sample(data[explode2].dat,250,128,1000,0);
                    play_sample(data[explode1].dat,250,128,1000,0);
                    play_sample(data[explode2].dat,250,128,1000,0);

                    explosion(s[1].x+s[1].bmp->w/2,s[1].y+s[1].bmp->h/2);
                    s[1].x=540;
                 	s[1].y=400;
                    s[1].life=100;
                    s[0].frags++;

                   }
                 }
            if(s[1].proj[n2].fired==1) 
			 if (check_col(s[0].bmp,s[1].proj[n2].bmp
                  ,s[0].x,s[0].y,s[n].proj[n2].x,s[n].proj[n2].y) && n==1)
                {
                 play_sample(data[shield].dat,250,128,1000,0);
                 s[0].s_up=15;
        		 s[0].s_angle=s[1].proj[n2].angle+128;
                 s[0].life-=s[n].proj[n2].damage;
                 s[1].proj[n2].fired=0;

                 if(s[0].life<=0)
                   {
                    play_sample(data[explode2].dat,250,128,1000,0);
                    play_sample(data[explode1].dat,250,128,1000,0);
                    play_sample(data[explode2].dat,250,128,1000,0);
                    play_sample(data[explode1].dat,250,128,1000,0);
                    play_sample(data[explode2].dat,250,128,1000,0);

                    explosion(s[0].x+s[0].bmp->w/2,s[0].y+s[0].bmp->h/2);
                    s[0].x=100;
                 	s[0].y=400;
                    s[0].life=100;
                    s[1].frags++;
                   }
                 }

             if(s[n].proj[n2].x>630)
               s[n].proj[n2].fired=0;
             else if(s[n].proj[n2].x<10)
               s[n].proj[n2].fired=0;

             if(s[n].proj[n2].y>430)
               s[n].proj[n2].fired=0;
             else if(s[n].proj[n2].y<10)
               s[n].proj[n2].fired=0;
             }
          }

      if(s[n].speed>0 && s[n].speed<.1)
        s[n].speed=0;
      else if(s[n].speed<0 && s[n].speed>.1)
        s[n].speed=0;


      if(s[n].m_x < s[n].move_x *s[n].speed)
       s[n].m_x+=.05;
     else if(s[n].m_x > s[n].move_x *s[n].speed)
       s[n].m_x-=.05;

     if(s[n].m_y < s[n].move_y *s[n].speed)
       s[n].m_y+=.05;
     else if(s[n].m_y > s[n].move_y *s[n].speed)
       s[n].m_y-=.05;

      if(s[n].speed>0)
        {
         if(cycles%1==0)
            {
         	part[part_n].x=s[n].x+s[n].bmp->w/2+((rand()%10)-5);
         	part[part_n].y=s[n].y+s[n].bmp->h/2+((rand()%10)-5);

            part[part_n].angle=(rand()%30)+s[n].angle+128-15;

         	part[part_n].move_y = -cos( (part[part_n].angle*1.40625) *M_PI/180);
	     	part[part_n].move_x = sin( (part[part_n].angle*1.40625) *M_PI/180);
         	part[part_n].exist=1;
            part[part_n].colour=1;

         	part_n++;

            if(part_n>=1499)
              part_n=0;
            }
        }
      //64-92

      if(s[n].ammo[0]<1000 && cycles%60==0)
        s[n].ammo[0]++;

      if(s[n].counter>0)
        s[n].counter--;

      if(s[n].m_x>0 && s[n].m_x*s[n].speed<.1)
        s[n].m_x=0;
      else if(s[n].m_x<0 && s[n].m_x*s[n].speed>.1)
        s[n].m_x=0;

      if(s[n].m_y>0 && s[n].m_y*s[n].speed<.1)
        s[n].m_y=0;
      else if(s[n].m_y<0 && s[n].m_y*s[n].speed>.1)
        s[n].m_y=0;

      if(s[n].x>630-s[n].bmp->w)
        {
        s[n].m_x*=-1;
        s[n].x-=4;
        play_sample(data[shield].dat,250,128,1000,0);
        s[n].s_up=15;
        s[n].s_angle=64;
        if(s[n].life>10)
        	s[n].life-=10;
        }
      else if(s[n].x<10)
        {
        s[n].m_x*=-1;
        s[n].x+=4;
        play_sample(data[shield].dat,250,128,1000,0);
        s[n].s_up=15;
        s[n].s_angle=64*3;
        if(s[n].life>10)
        	s[n].life-=10;
        }

      if(s[n].y>430-s[n].bmp->h)
        {
        s[n].m_y*=-1;
        s[n].y-=4;
        play_sample(data[shield].dat,250,128,1000,0);
        s[n].s_up=15;
        s[n].s_angle=64*2;
        if(s[n].life>10)
        	s[n].life-=10;
        }
      else if(s[n].y<10)
        {
        s[n].m_y*=-1;
        s[n].y+=4;
        play_sample(data[shield].dat,250,128,1000,0);
        s[n].s_up=15;
        s[n].s_angle=0;
        if(s[n].life>10)
        	s[n].life-=10;
        }

      s[n].x += s[n].m_x;
      s[n].y += s[n].m_y;

      if(s[n].s_up>0)
        s[n].s_up--;

      rotate_sprite(backgr,s[n].bmp,s[n].x,s[n].y,ftofix(s[n].angle));
      if(s[n].s_up>0)
        rotate_sprite(backgr,data[shield1].dat,s[n].x,s[n].y,itofix(s[n].s_angle));
      }

    for(n=0;n<1500;n++)
       {
        if(part[n].exist==1)
          {
           part[n].x+=part[n].move_x;
           part[n].y+=part[n].move_y;

           putpixel(backgr,part[n].x,part[n].y,part[n].colour);
           if(cycles%7==0 && part[n].colour<29)  //makes exaust particles fade
             part[n].colour++;
           if(part[n].colour==29)     //makes exaust particles dissapear
             part[n].exist=0;


           if(part[n].x>630 || part[n].x<10 || part[n].y<10 || part[n].y>430)
             part[n].exist=0;
          }
        }

 //   textprintf(backgr,font,0,0,20,"x: %f, y: %f",s[0].m_x,s[0].m_y);
 //   textprintf(backgr,font,0,10,20,"s: %f",s[0].speed);

    if(s[0].s_up>0)
        rectfill(backgr,23,433,172,478,1);
    if(s[1].s_up>0)
        rectfill(backgr,492,433,632,478,1);

    rectfill(backgr,25,435,170,476,0);
    rectfill(backgr,496,435,630,476,0);


 /*   rect(backgr,25,435,170,476,cycles%40);
    rect(backgr,496,435,630,476,cycles%40);*/

    textprintf(backgr,data[f1].dat,40,450,84,"%d",s[0].life);
    textprintf(backgr,data[f1].dat,600,450,131,"%d",s[1].life);

    textprintf(backgr,data[f1].dat,80,450,80,"%d",s[0].frags);
    textprintf(backgr,data[f1].dat,560,450,128,"%d",s[1].frags);

    textprintf(backgr,data[f1].dat,110,450,90,"%d",s[0].ammo[s[0].wep_selected]);
    textprintf(backgr,data[f1].dat,500,450,118,"%d",s[1].ammo[s[1].wep_selected]);

    textprintf_centre(backgr,font,95,440,90,"%s",wep[s[0].wep_selected].name);
    textprintf_centre(backgr,font,560,440,118,"%s",wep[s[1].wep_selected].name);

    if(s[0].frags>=10)
      {
      win(0);
      esc=1;
      }
    else if(s[1].frags>=10)
      {
      win(1);
      esc=1;
      }
    blit(backgr,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }


int collide(int x1,int y1,int x2,int y2, int xx1,int yy1,int xx2,int yy2)
	{
     if(x1>xx1 && x2<xx2 && y1>yy1 && y2<yy2)
       return 1;
     else
       return 0;
     }

void explosion(int x, int y)
	{
     int c;
     for(c=0;c<300;c++)
        {
     	 part[part_n].x=x+((rand()%10)-5);
         part[part_n].y=y+((rand()%10)-5);

		 part[part_n].angle=rand()%256;

       	 part[part_n].move_y = -cos( (part[part_n].angle*1.40625) *M_PI/180);
	     part[part_n].move_x = sin( (part[part_n].angle*1.40625) *M_PI/180);
         part[part_n].exist=1;
         part[part_n].colour=(rand()%28)+64;
         part_n++;

         if(part_n>=1499)
         	part_n=0;
        }

    }

/*fixed trig(float fx1,float fy1, float fx2, float fy2)
	{
    int x1=fx1,y1=fy1,x2=fx2,y2=fy2;

	fixed a;
	if (x1==x2)
		{
		if (y2<y1)
			a=itofix(192);
		else
			a=itofix(64);
		}
	else
		{
		a=fatan(ftofix(((float)(y2-y1)/(float)(x2-x1))));
		if ((x2-x1<=0) && ((y2-y1)<=0))
			a+=itofix(128);
		else if ((x2-x1<=0) && (y2-y1>=0))
			a+=itofix(128);
		if ((x2-x1>=0) && ((y2-y1)<=0))
			a=fadd(itofix(256),a);
		}
    a-=itofix(90/1.40625);

	return a;
}   */

void menu(void)
	{
     int ax=250,ay=235,hit=0, num=1;
     //rectfill(backgr,0,0,SCREEN_W,SCREEN_H,239);
     set_pallete(data[title_pal].dat);
     play_sample(data[fire].dat,250,128,1000,1);
     while(hit==0)
       {
        //rectfill(backgr,0,0,SCREEN_W,SCREEN_H,239);
        blit(data[title_pic].dat,backgr,0,0,0,0,640,480);
     	//textprintf(backgr,data[f1].dat,280,250,75,"Play");
     	//textprintf(backgr,data[f1].dat,280,450,75,"Exit");
        if(num==1)
        	animate(220,250,0);
        else
          	draw_sprite(backgr,anim[0].bmp[0],220,250);

        if(num==2)
        	animate(210,390,1);
        else
          	draw_sprite(backgr,anim[1].bmp[0],210,390);
     	//draw_sprite(backgr,data[arr_right].dat,ax,ay+10);

        if(key[KEY_DOWN] && num<2)
          {
           num++;
           ay+=200;
           play_sample(data[clap1].dat,160,128,1000,0);
          }
        else if(key[KEY_UP] && num>1)
          {
           num--;
           ay-=200;
           play_sample(data[clap1].dat,160,128,1000,0);
          }
        blit(backgr,screen,0,0,0,0,640,480);

        if(key[KEY_P])
        {
            shot=create_sub_bitmap(screen,0,0,SCREEN_W,SCREEN_H);
           save_bitmap("screenshot.pcx",shot,data[title_pal].dat);
            destroy_bitmap(shot);
        }

        if(key[KEY_ENTER])
          {
          hit=1;
          play_sample(data[clap2].dat,160,128,1000,0);
          }

        rest(50);
       }

     if(num==1)
       {
        set_game();
        esc=0;
        }

     else if(num==2)
       {
       esc=1;
       out=1;
       }
      stop_sample(data[fire].dat);
     }

void set_game(void)
	{
     int n,n2,x;
     s[0].x=100;
     s[0].y=400;
     s[0].bmp=data[ship1].dat;

     s[1].x=560;
     s[1].y=400;
     s[1].bmp=data[ship2].dat;

     s[2].x=560;
     s[2].y=400;
     s[2].bmp=data[ship2].dat;

     s[3].x=560;
     s[3].y=400;
     s[3].bmp=data[ship2].dat;

     for(n=0;n<4;n++)
         {
     	s[n].speed=0;
     	s[n].max_speed=5;
     	s[n].accel=.1;
     	s[n].angle=0;
     	s[n].move_x=0;
     	s[n].move_y=0;

     	s[n].number=0;
     	s[n].wep_selected=0;
     	s[n].wait=0;
     	s[n].s_up=0;
     	s[n].life=100;
     	s[n].frags=0;
     	s[n].counter=0;
     	for(n2=0;n2<200;n2++)
        	s[n].proj[n2].fired=0;
         }

     s[0].ammo[0]=wep[0].max_ammo;
     s[0].ammo[1]=wep[1].max_ammo;
     s[0].ammo[2]=wep[2].max_ammo;
     s[0].ammo[3]=wep[3].max_ammo;

     s[1].ammo[0]=wep[0].max_ammo;
     s[1].ammo[1]=wep[1].max_ammo;
     s[1].ammo[2]=wep[2].max_ammo;
     s[1].ammo[3]=wep[3].max_ammo;

     for(x=0;x<4;x++)
     	for(n=0;n<200;n++)
        	{
         	s[x].proj[n].x=0;
         	s[x].proj[n].y=0;
         	s[x].proj[n].bmp=NULL;
        	s[x].proj[n].move_x=0;
         	s[x].proj[n].move_y=0;
         	s[x].proj[n].m_y=0;
         	s[x].proj[n].m_x=0;
         	s[x].proj[n].speed=0;
         	s[x].proj[n].angle=0;
         	s[x].proj[n].max_speed=0;
         	s[x].proj[n].damage=0;
         	s[x].proj[n].type=0;
         	s[x].proj[n].target=0;
         	s[x].proj[n].wep=0;
			s[x].proj[n].fired=0;
        	}

     for(n=0;n<1500;n++)
        part[n].exist=0;

     set_pallete(data[pal].dat);

     }

void animate (int x, int y, int n)
	{
      draw_sprite(backgr,anim[n].bmp[anim[n].frame],x,y);
      anim[n].frame++;
      if(anim[n].frame>anim[n].number_of_frames)
        anim[n].frame=0;


     }

void win(int n)
	{
     int counter=0,nn,r,num;
     //176-183
     float x[100],y[100],angle[100],m_x[100],m_y[100];
     int count[100],dist[100],exist[100];

     for(nn=0;nn<100;nn++)
        {
         x[nn]=0;
         y[nn]=0;
         m_x[nn]=0;
         m_y[nn]=0;
         angle[nn]=0;
         count[nn]=0;
         dist[nn]=0;
         exist[100]=0;
         }

     set_pallete(data[title_pal].dat);

     num=0;

     play_sample(data[fire].dat,150,128,1000,1);
     while(!key[KEY_ENTER])
       {
        counter++;
        rectfill(backgr,0,0,640,480,0);

        if(rand()%60 == 0)
      		{
       		draw_sprite(backgr,data[lightning1].dat,rand()%640,10);
       		play_sample(data[clap1].dat,160,128,1000,0);
      		}
    	else if(rand()%150 == 1)
      		{
       		draw_sprite(backgr,data[lightning2].dat,rand()%640,10);
       		play_sample(data[clap2].dat,160,128,1000,0);
      		}

        if(counter%12==0)
          {
           r=30+(rand()%120);

           x[num]=320;
           y[num]=480;
           m_x[num] = -cos( r *M_PI/180);
	       m_y[num] = -sin( r*M_PI/180);
           dist[num]=(rand()%50)+40;
           exist[num] =1;

           num++;
           if(num>=99)
             num=0;

           //play_sample(data[m_shoot].dat,150,128,1000,0);
           }

        for(nn=0;nn<99;nn++)
           {
            if(exist[nn]==1)
              {
               x[nn]+=m_x[nn]*5;
               y[nn]+=m_y[nn]*5;
               dist[nn]--;

               if(dist[nn]<=0)
                 {
                 explosion(x[nn],y[nn]);
                 exist[nn]=0;
                 play_sample(data[explode1].dat,150,128,1000,0);
                 }
               else
               	circlefill(backgr,x[nn],y[nn],1,179);
              }
            }

    for(nn=0;nn<1500;nn++)
       {
        if(part[nn].exist==1)
          {
           part[nn].x+=part[nn].move_x;
           part[nn].y+=part[nn].move_y;

           putpixel(backgr,part[nn].x,part[nn].y,part[nn].colour+60);

           if(cycles%9==0)
             part[nn].colour--;
           }
        }

        textprintf_centre(backgr,data[f1].dat,300,200,(counter%7)+176,"Congratulations Player %d",n+1);
        animate(200,200,2);

        blit(backgr,screen,0,0,0,0,640,480);
        rest(15);
      }
     stop_sample(data[fire].dat);
     clear_keybuf();
     }

int check_col(BITMAP *b1, BITMAP *b2,float x1,float y1,float x2,float y2)
     {
      if(x1+b1->w>x2 && x1 < x2+b2->w
      && y1+b1->h>y2 && y1 < y2+b2->h)
         return 1;
      else
         return 0;
      }
