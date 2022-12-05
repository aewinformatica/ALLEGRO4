/******************************************/
/*                MyRPG.c                 */
/*                                        */
/*           ˮ��֮Լ�е����ļ�           */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*         ��ֲ��DEV-C++ ���벻��         */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/

//��3.15.2004��ֲ��DEV-CPP�£����벻�䣬�ɼ�Allegro��ֲ�Ժ�ǿ�� :)

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

 //////////////////////�����ʼ��/////////////////////
 /*����ʱ��*/
 install_timer();
 install_int_ex(AddTime,BPS_TO_TIMER(24));

 /*��������*/
 install_keyboard();

 /*������Ч,���ز�����midi�����ذ�ť��Ч*/
 install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,"");
 set_volume(128,128);
 Choice_Lib.button=load_sample("sound/button.wav");

 /*����һЩ����ر�ͼƬ*/
 RPG_Map.floor=LoadBMP("pic/blank.bmp");
 RPG_Map.sky=LoadBMP("pic/blank.bmp");
 NPC_Image.shadow=LoadBMP("pic/shadow.bmp");
 TalkBox.talk_arrow=init_animate(20,10,8,1,"pic/arrow.bmp");
 Window.pic=loadsprlib(32,32,"pic/dialog.bmp");
 Choice_Lib.arrow=init_animate(60,19,4,2,"pic/hand.bmp");

 /*û��NPC���ű�����*/
 NPC_Lib.scr_npc_num=NPC_FREE;

 /*��ʼ������ϵͳ*/
 Init_Weather();

 /*��ʼ���Ŷ�ϵͳ*/
 Init_Team();

 /*��ʼ������ϵͳ*/
 Init_GUI();

 /*�ر���ĻС����ϵͳ*/
 Shut_Screen_Image();

 /*�ӽű�start��ʼ*/
 Load_Script("script/start.txt");

 //��ʱ��ʼ��
 //init_obj_actor();
 init_fight();

 ///////////////////////��ѭ��//////////////////////////
 /*��ѭ������ESC�˳�*/
 while(GameFlag!=GAMEQUIT)
 {

  //////////////////�ű�ִ��ģ��///////////////////
  /*�ű�ִ��,���Ϊ����ģʽ������һѭ��*/
  Run_Script();
  if(RPG_Script.script_flag==SCRIPT_HANG)continue;

  //////////////////ϵͳ����ģ��///////////////////
  /*��ѭ���Ŀ���ģ�飬�˳��Ͳ˵������޽ű�ʱ����Ч*/
  //if(RPG_Script.script_flag==SCRIPT_SHUT)
  //{
    /*��ESC�˳�*/
  //  if(key[KEY_ESC])     GameFlag=GAMEQUIT;
  //}
  /*��F1����ͼ�γ��Ի����κ�ʱ����Ч*/
  if(key[KEY_F1]){set_color_depth(16);set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);}
  //��F5����ս��
  if(key[KEY_F5])Fight_Loop();
  //��F2������Ļ
  if(key[KEY_F2])save_bmp("temp.bmp",V_Buffer,Pal);


  //////////////////NPC����ģ��/////////////////////
  /*NPC������ģ�飬��0�������*/
  for(i=0;i<NPC_Lib.max_num;i++)
  {
   /*�����NPCӦ����ֹ�Ҳ�Ϊ�ű������Ҳ������ǣ�������*/
   if(NPC_Lib.npc[i].status==NPC_STATIC&&NPC_Lib.scr_npc_num!=i&&NPC_Lib.npc_ctrl!=i)continue;


   /*���Ϊ�ű�����������˶�����һ������ʱ�漴������һ��*/
   else if(NPC_Lib.npc[i].counter1==0)
   {
    /*��Ϊ�ű���������NPC*/
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


    /*��Ϊ���ǣ�����鵱ǰ�����Ƿ�������壬��������ֵ*/
    else if(NPC_Lib.npc_ctrl==i)
    {
     /*����ű�ִ���У�������*/
     if(RPG_Script.script_flag!=SCRIPT_SHUT)continue;

     /*�����Ƿ�������壬���򴥷��¼�*/
     if(Hook_Lib.hook_kind[NPC_Lib.npc[NPC_Lib.npc_ctrl].xx][NPC_Lib.npc[NPC_Lib.npc_ctrl].yy]==HOOK_1)
     {
      Load_Script(Hook_Lib.script[Hook_Lib.hook_data[NPC_Lib.npc[NPC_Lib.npc_ctrl].xx][NPC_Lib.npc[NPC_Lib.npc_ctrl].yy]]);
      continue;
     }
     else if(key[KEY_UP])NPC_Lib.npc[i].dir=NPC_UP;
     else if(key[KEY_DOWN])NPC_Lib.npc[i].dir=NPC_DOWN;
     else if(key[KEY_LEFT])NPC_Lib.npc[i].dir=NPC_LEFT;
     else if(key[KEY_RIGHT])NPC_Lib.npc[i].dir=NPC_RIGHT;
     /*��D����˵�ϵͳ*/
     else if(key[KEY_D])
     {
      PressKey(KEY_D);

      Win_Loop();
      continue;
     }
     /*��S�������⹦�ܱ����л�����*/
     else if(key[KEY_S])
     {
      PressKey(KEY_S);

      Load_Script("script/s_pressed.txt");
      continue;
     }
     /*��X���ܼ�������NPC�Ͳ���������*/
     else if(key[KEY_X])
     {
      int npc_xx,npc_yy,npc_dir;

      PressKey(KEY_X);

      /*������ǵ���������*/
      switch(NPC_Lib.npc[NPC_Lib.npc_ctrl].dir)
      {
       case NPC_UP      : npc_xx=NPC_Lib.npc[NPC_Lib.npc_ctrl].xx,npc_yy=NPC_Lib.npc[NPC_Lib.npc_ctrl].yy-1;npc_dir=NPC_DOWN;break;
       case NPC_DOWN    : npc_xx=NPC_Lib.npc[NPC_Lib.npc_ctrl].xx,npc_yy=NPC_Lib.npc[NPC_Lib.npc_ctrl].yy+1;npc_dir=NPC_UP;break;
       case NPC_LEFT    : npc_xx=NPC_Lib.npc[NPC_Lib.npc_ctrl].xx-1,npc_yy=NPC_Lib.npc[NPC_Lib.npc_ctrl].yy;npc_dir=NPC_RIGHT;break;
       case NPC_RIGHT   : npc_xx=NPC_Lib.npc[NPC_Lib.npc_ctrl].xx+1,npc_yy=NPC_Lib.npc[NPC_Lib.npc_ctrl].yy;npc_dir=NPC_LEFT;break;
       default:;
      }

      /*����˴��ǲ��������壬����ű�,������NPC*/
      if(Hook_Lib.hook_kind[npc_xx][npc_yy]==HOOK_2)
      {
       Load_Script(Hook_Lib.script[Hook_Lib.hook_data[npc_xx][npc_yy]]);
      }
      else for(i=0;i<NPC_Lib.max_num;i++)
      {
       if(NPC_Lib.npc[i].xx==npc_xx&&NPC_Lib.npc[i].yy==npc_yy&&NPC_Lib.npc[i].counter1==0)
       {
        NPC_Lib.npc[i].dir=npc_dir;
        Load_Script(NPC_Lib.npc[i].script);  /*���ظ�NPC�ű�*/
        break;
       }
      }

      continue;
     }
     else continue;

    }


    /*���NPC����˶�����һ�����ʵ�ʱ��NPC������*/
    else if(NPC_Lib.npc[i].status==NPC_MOVING)
    {
     if(randnum(10))continue;
     NPC_Lib.npc[i].dir=randnum(4);
    }


    /*�����NPC����������*/
    else if(NPC_Lib.npc[i].status==NPC_ESCAPE)
    {
     int temp_dir,temp_xx,temp_yy,temp_compare;

     /*��������ǻ���Զ����Ъ��,�������ܼӴ����ĵط���,��������һ���Ӵ����ķ���*/
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

    /*���ݷ��򼤻��µ�һ��*/
    switch(NPC_Lib.npc[i].dir)
    {
     case NPC_UP:     if(NPC_Lib.npc[i].yy-1>-1&&RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy-1]==WALK_YES) /*���ǰ���ǿ��ߵ��򼤻�һ��*/
                      {
                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;

                       NPC_Lib.npc[i].yy--;
                       NPC_Lib.npc[i].counter1=32/NPC_Lib.npc[i].step-1;
                       NPC_Lib.npc[i].counter2++;
                       if(NPC_Lib.npc[i].counter2>7)NPC_Lib.npc[i].counter2=0;

                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_NO;
                      }
                      break;

     case NPC_DOWN:   if(NPC_Lib.npc[i].yy+1<RPG_Map.max_y&&RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy+1]==WALK_YES) /*���ǰ���ǿ��ߵ��򼤻�һ��*/
                      {
                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;

                       NPC_Lib.npc[i].yy++;
                       NPC_Lib.npc[i].counter1=32/NPC_Lib.npc[i].step-1;
                       NPC_Lib.npc[i].counter2++;
                       if(NPC_Lib.npc[i].counter2>7)NPC_Lib.npc[i].counter2=0;

                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_NO;
                      }
                      break;

     case NPC_LEFT:   if(NPC_Lib.npc[i].xx-1>-1&&RPG_Map.data[NPC_Lib.npc[i].xx-1][NPC_Lib.npc[i].yy]==WALK_YES) /*���ǰ���ǿ��ߵ��򼤻�һ��*/
                      {
                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;

                       NPC_Lib.npc[i].xx--;
                       NPC_Lib.npc[i].counter1=32/NPC_Lib.npc[i].step-1;
                       NPC_Lib.npc[i].counter2++;
                       if(NPC_Lib.npc[i].counter2>7)NPC_Lib.npc[i].counter2=0;

                       RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_NO;
                      }
                      break;

     case NPC_RIGHT:  if(NPC_Lib.npc[i].xx+1<RPG_Map.max_x&&RPG_Map.data[NPC_Lib.npc[i].xx+1][NPC_Lib.npc[i].yy]==WALK_YES) /*���ǰ���ǿ��ߵ��򼤻�һ��*/
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
    NPC_Lib.npc[i].counter1--;/*��һ����δ���꣬������ò�*/
   }
  }

  /*���Ǻ�NPC������任�������߼���������ڵ�ͼ�е�ʵ������*/
  for(i=0;i<=NPC_Lib.max_num;i++)
    NPCXY(NPC_Lib.npc+i);

  /*��������λ�������Ļ������*/
  ScreenXY(NPC_Lib.npc+NPC_Lib.npc_ctrl);


  ////////////////////////��Ļ��ʾģ��////////////////////
  /*��ʾ��ͼ�����*/
  ShowFloor();

  /*��ʾ���Ǻ�NPC,���������yֵ����,����Զ����˳����NPC*/
  for(i=0;i<RPG_Map.max_y;i++)
    for(j=0;j<NPC_Lib.max_num;j++)
      if(NPC_Lib.npc[j].yy==i)ShowNPC(NPC_Lib.npc+j);

  /*��ʾ��ͼ��ղ�*/
  ShowSky();

  /*��ʾһ��͸�������ǻ������ͽ�ɫ*/
  for(i=0;i<NPC_Lib.max_num;i++)
  {
   if(i==NPC_Lib.npc_ctrl||NPC_Lib.npc[i].status==NPC_ESCAPE)
     AlphaNPC(NPC_Lib.npc+i);
  }

  shadow_puts(RPG_Map.name,20,40,WHITE);


  /*����Ч��*/
  Weather_Effect();

  /*����Ч��*/
  Light_Effect();

  /*��ĻС����*/
  Show_Screen_Image();

  /*����ʱ��־Ϊ��ʱ������ʱ*/
  RPG_Delay();

  /*���뵭����־Ϊ��ʱ�������뵭��*/
  Fade_Inout();

  /*���Ի���־Ϊ��ʱ�����Ի�*/
  RPG_Talk();


  /*{
   char str[30];
   sprintf(str,"%f,%f,%d,%d",Screen.xx,Screen.yy,Fade.counter1,Fade.counter2);
   puts16b(str,20,20,BLUE);
  }*/


  /////////////////////��ʱ��ʾͼ��//////////////////////
  /*��ʱ�ӵ���ʱ��ʾ���棬������һѭ�����̣�����ȴ�*/
  while(Timercounter<1);
  Timercounter=0;
  vpost();

 }


 ////////////////////////////����ر�///////////////////////////
 /*�ر�����*/
 remove_sound();

 /*�رռ���*/
 remove_keyboard();

 /*�ر�ʱ��*/
 remove_timer();

}

int main()
{

 allegro_init();
 set_config_file("game.cfg");

 /*ͼ�γ�ʼ��*/
 init_graph();

 newgame();

 allegro_exit();
 return 0;
}

END_OF_MAIN();

