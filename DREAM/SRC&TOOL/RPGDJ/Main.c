/******************************************/
/*                MyRPG.c                 */
/*                                        */
/*           水晶之约中的主文件           */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*         移植至DEV-C++ 代码不变         */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/

//于3.15.2004移植至DEV-CPP下，代码不变，可见Allegro移植性很强的 :)

#include <stdio.h>
#include <math.h>
#include <allegro.h>
#include "graph.c"

#include "rpg.h"

#include "scr_var.h"
#include "timer.c"
#include "key.c"
#include "map.c"
#include "sprite.c"
#include "screen.c"
#include "dialog.c"
#include "delay.c"

#include "object.c"
#include "actor.c"
#include "window.c"

#include "fight.c"

#include "choice.c"

#include "script.c"

/******************************************************************/


void newgame()
{
 int i,j;

 //////////////////////引擎初始化/////////////////////
 /*启动时钟*/
 install_timer();
 install_int_ex(AddTime,BPS_TO_TIMER(24));

 /*启动键盘*/
 install_keyboard();

 /*启动音效,加载并播放midi，加载按钮音效*/
 install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,"");
 set_volume(128,128);
 Choice_Lib.button=load_sample("sound/button.wav");

 /*加载一些引擎必备图片*/
 RPG_Map.floor=LoadBMP("pic/blank.bmp");
 RPG_Map.sky=LoadBMP("pic/blank.bmp");
 NPC_Image.shadow=LoadBMP("pic/shadow.bmp");
 TalkBox.talk_arrow=init_animate(20,10,8,1,"pic/arrow.bmp");
 Window.pic=loadsprlib(32,32,"pic/dialog.bmp");
 Choice_Lib.arrow=init_animate(60,19,4,2,"pic/hand.bmp");

 /*没有NPC被脚本驱动*/
 NPC_Lib.scr_npc_num=NPC_FREE;

 /*初始化天气系统*/
 Init_Weather();

 /*初始化团队系统*/
 Init_Team();

 /*初始化窗体系统*/
 Init_GUI();

 /*关闭屏幕小画面系统*/
 Shut_Screen_Image();

 /*从脚本start开始*/
 Load_Script("script/start.txt");

 //临时初始化
 //init_obj_actor();
 init_fight();

 ///////////////////////主循环//////////////////////////
 /*主循环，按ESC退出*/
 while(GameFlag!=GAMEQUIT)
 {

  //////////////////脚本执行模块///////////////////
  /*脚本执行,如果为挂起模式进入下一循环*/
  Run_Script();
  if(RPG_Script.script_flag==SCRIPT_HANG)continue;

  //////////////////系统控制模块///////////////////
  /*大循环的控制模块，退出和菜单，在无脚本时才有效*/
  //if(RPG_Script.script_flag==SCRIPT_SHUT)
  //{
    /*按ESC退出*/
  //  if(key[KEY_ESC])     GameFlag=GAMEQUIT;
  //}
  /*按F1重新图形初试化，任何时候都有效*/
  if(key[KEY_F1]){set_color_depth(16);set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);}
  //按F5调出战斗
  if(key[KEY_F5])Fight_Loop();
  //按F2保存屏幕
  if(key[KEY_F2])save_bmp("temp.bmp",V_Buffer,Pal);


  //////////////////NPC行走模块/////////////////////
  /*NPC的行走模块，从0号起遍历*/
  for(i=0;i<NPC_Lib.max_num;i++)
  {
   /*如果该NPC应当静止且不为脚本驱动且不是主角，则跳过*/
   if(NPC_Lib.npc[i].status==NPC_STATIC&&NPC_Lib.scr_npc_num!=i&&NPC_Lib.npc_ctrl!=i)continue;


   /*如果为脚本驱动或随机运动，当一步走完时随即激发下一步*/
   else if(NPC_Lib.npc[i].counter1==0)
   {
    /*如为脚本所驱动的NPC*/
    if(NPC_Lib.scr_npc_num==i)
    {
     NPC_Lib.scr_step--;

     if(NPC_Lib.scr_step<0)
     {
      NPC_Lib.scr_npc_num=NPC_FREE;
      NPC_Lib.scr_step=0;

      RPG_Script.script_flag=SCRIPT_WAIT;
      continue;
     }
    }


    /*如为主角，则检验当前坐标是否可立陷阱，否则检验键值*/
    else if(NPC_Lib.npc_ctrl==i)
    {
     /*如果脚本执行中，则跳过*/
     if(RPG_Script.script_flag!=SCRIPT_SHUT)continue;

     /*检验是否可立陷阱，是则触发事件*/
     if(Hook_Lib.hook_kind[NPC_Lib.npc[NPC_Lib.npc_ctrl].xx][NPC_Lib.npc[NPC_Lib.npc_ctrl].yy]==HOOK_1)
     {
      Load_Script(Hook_Lib.script[Hook_Lib.hook_data[NPC_Lib.npc[NPC_Lib.npc_ctrl].xx][NPC_Lib.npc[NPC_Lib.npc_ctrl].yy]]);
      continue;
     }
     else if(key[KEY_UP])NPC_Lib.npc[i].dir=NPC_UP;
     else if(key[KEY_DOWN])NPC_Lib.npc[i].dir=NPC_DOWN;
     else if(key[KEY_LEFT])NPC_Lib.npc[i].dir=NPC_LEFT;
     else if(key[KEY_RIGHT])NPC_Lib.npc[i].dir=NPC_RIGHT;
     /*按D进入菜单系统*/
     else if(key[KEY_D])
     {
      PressKey(KEY_D);

      Win_Loop();
      continue;
     }
     /*按S调用特殊功能比如切换主角*/
     else if(key[KEY_S])
     {
      PressKey(KEY_S);

      Load_Script("script/s_pressed.txt");
      continue;
     }
     /*按X功能键，检验NPC和不可立陷阱*/
     else if(key[KEY_X])
     {
      int npc_xx,npc_yy,npc_dir;

      PressKey(KEY_X);

      /*获得主角的趋势坐标*/
      switch(NPC_Lib.npc[NPC_Lib.npc_ctrl].dir)
      {
       case NPC_UP      : npc_xx=NPC_Lib.npc[NPC_Lib.npc_ctrl].xx,npc_yy=NPC_Lib.npc[NPC_Lib.npc_ctrl].yy-1;npc_dir=NPC_DOWN;break;
       case NPC_DOWN    : npc_xx=NPC_Lib.npc[NPC_Lib.npc_ctrl].xx,npc_yy=NPC_Lib.npc[NPC_Lib.npc_ctrl].yy+1;npc_dir=NPC_UP;break;
       case NPC_LEFT    : npc_xx=NPC_Lib.npc[NPC_Lib.npc_ctrl].xx-1,npc_yy=NPC_Lib.npc[NPC_Lib.npc_ctrl].yy;npc_dir=NPC_RIGHT;break;
       case NPC_RIGHT   : npc_xx=NPC_Lib.npc[NPC_Lib.npc_ctrl].xx+1,npc_yy=NPC_Lib.npc[NPC_Lib.npc_ctrl].yy;npc_dir=NPC_LEFT;break;
       default:;
      }

      /*如果此处是不可立陷阱，则调脚本,否则检查NPC*/
      if(Hook_Lib.hook_kind[npc_xx][npc_yy]==HOOK_2)
      {
       Load_Script(Hook_Lib.script[Hook_Lib.hook_data[npc_xx][npc_yy]]);
      }
      else for(i=0;i<NPC_Lib.max_num;i++)
      {
       if(NPC_Lib.npc[i].xx==npc_xx&&NPC_Lib.npc[i].yy==npc_yy&&NPC_Lib.npc[i].counter1==0)
       {
        NPC_Lib.npc[i].dir=npc_dir;
        Load_Script(NPC_Lib.npc[i].script);  /*加载该NPC脚本*/
        break;
       }
      }

      continue;
     }
     else continue;

    }


    /*如果NPC随机运动，当一定几率的时候NPC才行走*/
    else if(NPC_Lib.npc[i].status==NPC_MOVING)
    {
     if(randnum(10))continue;
     NPC_Lib.npc[i].dir=randnum(4);
    }


    /*如果该NPC逃着主角跑*/
    else if(NPC_Lib.npc[i].status==NPC_ESCAPE)
    {
     int temp_dir,temp_xx,temp_yy,temp_compare;

     /*如果离主角还够远，则歇着,否则向能加大距离的地方跑,否则生成一个加大距离的方向*/
     if((temp_compare=abs(NPC_Lib.npc[i].xx-NPC_Lib.npc[NPC_Lib.npc_ctrl].xx)+abs(NPC_Lib.npc[i].yy-NPC_Lib.npc[NPC_Lib.npc_ctrl].yy))>10)
     {
      continue;
     }
     else
     {
        do
        {
         temp_dir=randnum(4);

         switch(temp_dir)
         {
          case NPC_UP:   temp_xx=NPC_Lib.npc[i].xx;
                         temp_yy=NPC_Lib.npc[i].yy-1;
                         break;

          case NPC_DOWN: temp_xx=NPC_Lib.npc[i].xx;
                         temp_yy=NPC_Lib.npc[i].yy+1;
                         break;

          case NPC_LEFT: temp_xx=NPC_Lib.npc[i].xx-1;
                         temp_yy=NPC_Lib.npc[i].yy;
                         break;

          case NPC_RIGHT:temp_xx=NPC_Lib.npc[i].xx+1;
                         temp_yy=NPC_Lib.npc[i].yy;
                         break;
          default:;
         }
        }while(abs(NPC_Lib.npc[NPC_Lib.npc_ctrl].xx-temp_xx)+abs(NPC_Lib.npc[NPC_Lib.npc_ctrl].yy-temp_yy)<temp_compare);

        NPC_Lib.npc[i].dir=temp_dir;
     }
    }

    /*根据方向激活新的一步*/
    switch(NPC_Lib.npc[i].dir)
    {
     case NPC_UP:     if(NPC_Lib.npc[i].yy-1>-1&&RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy-1]==WALK_YES) /*如果前面是可走的则激活一步*/
                      {
                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;

                       NPC_Lib.npc[i].yy--;
                       NPC_Lib.npc[i].counter1=32/NPC_Lib.npc[i].step-1;
                       NPC_Lib.npc[i].counter2++;
                       if(NPC_Lib.npc[i].counter2>7)NPC_Lib.npc[i].counter2=0;

                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_NO;
                      }
                      break;

     case NPC_DOWN:   if(NPC_Lib.npc[i].yy+1<RPG_Map.max_y&&RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy+1]==WALK_YES) /*如果前面是可走的则激活一步*/
                      {
                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;

                       NPC_Lib.npc[i].yy++;
                       NPC_Lib.npc[i].counter1=32/NPC_Lib.npc[i].step-1;
                       NPC_Lib.npc[i].counter2++;
                       if(NPC_Lib.npc[i].counter2>7)NPC_Lib.npc[i].counter2=0;

                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_NO;
                      }
                      break;

     case NPC_LEFT:   if(NPC_Lib.npc[i].xx-1>-1&&RPG_Map.data[NPC_Lib.npc[i].xx-1][NPC_Lib.npc[i].yy]==WALK_YES) /*如果前面是可走的则激活一步*/
                      {
                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;

                       NPC_Lib.npc[i].xx--;
                       NPC_Lib.npc[i].counter1=32/NPC_Lib.npc[i].step-1;
                       NPC_Lib.npc[i].counter2++;
                       if(NPC_Lib.npc[i].counter2>7)NPC_Lib.npc[i].counter2=0;

                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_NO;
                      }
                      break;

     case NPC_RIGHT:  if(NPC_Lib.npc[i].xx+1<RPG_Map.max_x&&RPG_Map.data[NPC_Lib.npc[i].xx+1][NPC_Lib.npc[i].yy]==WALK_YES) /*如果前面是可走的则激活一步*/
                      {
                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;

                       NPC_Lib.npc[i].xx++;
                       NPC_Lib.npc[i].counter1=32/NPC_Lib.npc[i].step-1;
                       NPC_Lib.npc[i].counter2++;
                       if(NPC_Lib.npc[i].counter2>7)NPC_Lib.npc[i].counter2=0;

                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_NO;
                      }
                      break;

     default:;
    }
   }
   else
   {
    NPC_Lib.npc[i].counter2++;
    if(NPC_Lib.npc[i].counter2>7)NPC_Lib.npc[i].counter2=0;
    NPC_Lib.npc[i].counter1--;/*如一步还未走完，则走完该步*/
   }
  }

  /*主角和NPC的坐标变换，根据逻辑坐标算出在地图中的实际坐标*/
  for(i=0;i<=NPC_Lib.max_num;i++)
    NPCXY(NPC_Lib.npc+i);

  /*根据主角位置算出屏幕的坐标*/
  ScreenXY(NPC_Lib.npc+NPC_Lib.npc_ctrl);


  ////////////////////////屏幕显示模块////////////////////
  /*显示地图地面层*/
  ShowFloor();

  /*显示主角和NPC,根据增大的y值遍历,按从远到近顺序画上NPC*/
  for(i=0;i<RPG_Map.max_y;i++)
    for(j=0;j<NPC_Lib.max_num;j++)
      if(NPC_Lib.npc[j].yy==i)ShowNPC(NPC_Lib.npc+j);

  /*显示地图天空层*/
  ShowSky();

  /*显示一个透明的主角或逃跑型角色*/
  for(i=0;i<NPC_Lib.max_num;i++)
  {
   if(i==NPC_Lib.npc_ctrl||NPC_Lib.npc[i].status==NPC_ESCAPE)
     AlphaNPC(NPC_Lib.npc+i);
  }

  shadow_puts(RPG_Map.name,20,40,WHITE);


  /*天气效果*/
  Weather_Effect();

  /*光照效果*/
  Light_Effect();

  /*屏幕小画面*/
  Show_Screen_Image();

  /*当延时标志为真时引发延时*/
  RPG_Delay();

  /*淡入淡出标志为真时引发淡入淡出*/
  Fade_Inout();

  /*当对话标志为真时引发对话*/
  RPG_Talk();


  /*{
   char str[30];
   sprintf(str,"%f,%f,%d,%d",Screen.xx,Screen.yy,Fade.counter1,Fade.counter2);
   puts16b(str,20,20,BLUE);
  }*/


  /////////////////////定时显示图象//////////////////////
  /*当时钟到点时显示画面，进入下一循环过程，否则等待*/
  while(Timercounter<1);
  Timercounter=0;
  vpost();

 }


 ////////////////////////////引擎关闭///////////////////////////
 /*关闭声音*/
 remove_sound();

 /*关闭键盘*/
 remove_keyboard();

 /*关闭时钟*/
 remove_timer();

}

int main()
{

 allegro_init();
 set_config_file("game.cfg");

 /*图形初始化*/
 init_graph();

 newgame();

 allegro_exit();
 return 0;
}

END_OF_MAIN();

