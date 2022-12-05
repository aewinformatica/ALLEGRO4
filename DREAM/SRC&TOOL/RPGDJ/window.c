/******************************************/
/*                window.c                */
/*                                        */
/*          水晶之约中的窗体系统          */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom  1.5.2003            */
/******************************************/


#define SHOW_STATUS      0
#define USE_OBJECT       1
#define CHOOSE_ACTOR     2
#define SHOW_HELP        3
#define SHOW_SYSTEM      4
#define ASK_QUIT         5


#define WIN_ON           1
#define WIN_OFF          0

#define NO_CHOICE        -1


struct
{
 Sprlib pic;  //图片文件

 char show;      //显示的页面号
 char state;    //循环状态
}Window;


typedef struct
{
 int xx;
 int yy;

 int height;

 char max_point;
 char point_at;
}CHOICE;


struct
{
 CHOICE choice_box[20];
 int max_num;

 Animate arrow;
 SAMPLE *button;
}Choice_Lib={0};           //选择框资源库

///////////////////////////////////////////////////////////////////////

/*画一个空白窗口*/
void Draw_Window(int x,int y,int width,int height)
{
 int i,j;
 putspr(x,y,0,0,Window.pic);
 putspr(x+width*32-32,y,2,0,Window.pic);
 putspr(x,y+height*32-32,0,2,Window.pic);
 putspr(x+width*32-32,y+height*32-32,2,2,Window.pic);
 for(i=1;i<width-1;i++)
   putspr(x+32*i,y,1,0,Window.pic);
 for(i=1;i<height-1;i++)
   putspr(x,y+32*i,0,1,Window.pic);
 for(i=1;i<height-1;i++)
   putspr(x+32*width-32,y+32*i,2,1,Window.pic);
 for(i=1;i<width-1;i++)
   putspr(x+32*i,y+32*height-32,1,2,Window.pic);
 for(i=1;i<height-1;i++)
   for(j=1;j<width-1;j++)
     putspr(x+32*j,y+32*i,1,1,Window.pic);
}


/*初始化选择框*/
void Init_Choice_Lib(int xx,int yy,int height,char max_point)
{
 Choice_Lib.choice_box[Choice_Lib.max_num].xx=xx;
 Choice_Lib.choice_box[Choice_Lib.max_num].yy=yy;

 Choice_Lib.choice_box[Choice_Lib.max_num].height=height;

 Choice_Lib.choice_box[Choice_Lib.max_num].max_point=max_point;

 Choice_Lib.max_num++;
}


/*选择框的函数*/
int Choice_Box(int i)
{
 showanimate(Choice_Lib.choice_box[i].xx,Choice_Lib.choice_box[i].yy+Choice_Lib.choice_box[i].height*Choice_Lib.choice_box[i].point_at,&Choice_Lib.arrow);

 if(key[KEY_UP])
 {
  PressKey(KEY_UP);
  Choice_Lib.choice_box[i].point_at--;
  if(Choice_Lib.choice_box[i].point_at<0)Choice_Lib.choice_box[i].point_at=0;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);

  return NO_CHOICE;
 }
 else if(key[KEY_DOWN])
 {
  PressKey(KEY_DOWN);
  Choice_Lib.choice_box[i].point_at++;
  if(Choice_Lib.choice_box[i].point_at>Choice_Lib.choice_box[i].max_point-1)
    Choice_Lib.choice_box[i].point_at=Choice_Lib.choice_box[i].max_point-1;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);

  return NO_CHOICE;
 }
 else if(key[KEY_X])
 {
  PressKey(KEY_X);

  play_sample(Choice_Lib.button,255,127,1000,FALSE);

  return Choice_Lib.choice_box[i].point_at;
 }

 return NO_CHOICE;
}

///////////////////////////////////////////////////////////////////////
/*
void init_obj_actor()
{
 int i;

 Init_Actor_Lib(300,300,300,300,700,500,60,10,"兰特","pic/win01.bmp");
 Init_Actor_Lib(200,200,200,200,500,300,20,5,"拉菲","pic/win02.bmp");
 Init_Actor_Lib(300,300,300,300,600,200,20,6,"克莉斯","pic/win03.bmp");


 RPG_Team.money=7500;
 //Add_Actor(0);
 Add_Actor(1);
 Add_Actor(2);



 Init_Object_Lib(250,0,0,0,0,OBJ_UNNEED,"月兔之灵丹","pic/obj1.bmp");
 Init_Object_Lib(100,100,0,0,0,OBJ_UNNEED,"星之甘露","pic/obj2.bmp");
 Init_Object_Lib(0,0,0,0,0,OBJ_NEED,"林心如照片","pic/obj3.bmp");

 Add_Object(0,10);
 Add_Object(1,20);
 Add_Object(2,1);


} */


/*初始化窗体系统*/
void Init_GUI()
{
 Init_Choice_Lib(16,32,32,6);
 Init_Choice_Lib(220,32,32,13);
 Init_Choice_Lib(220,48,150,3);
 Init_Choice_Lib(145,200,32,3);
 Init_Choice_Lib(235,200,32,2);
 Init_Choice_Lib(32,32,32,4);
 Init_Choice_Lib(0,32,48,3);
}

///////////////////////////////////////////////////////////////////////

/*选择主角使用物品页面*/
void ChooseActor()
{
 int i;
 char str[20],choice;

 Draw_Window(32,0,19,15);
 Draw_Window(0,0,6,8);

 for(i=0;i<RPG_Team.max_actor_num;i++)
 {
  QPutImage(300,i*150+32,Actor_Lib.actor[RPG_Team.actor_num[i]].photo);

  shadow_puts(Actor_Lib.actor[RPG_Team.actor_num[i]].name,370,i*150+32,WHITE);

  sprintf(str,"等级:%d",Actor_Lib.actor[RPG_Team.actor_num[i]].grade);
  shadow_puts(str,370,i*150+64,WHITE);

  sprintf(str,"HP:%d/%d",Actor_Lib.actor[RPG_Team.actor_num[i]].hp,Actor_Lib.actor[RPG_Team.actor_num[i]].max_hp);
  shadow_puts(str,370,i*150+96,WHITE);

  sprintf(str,"MP:%d/%d",Actor_Lib.actor[RPG_Team.actor_num[i]].mp,Actor_Lib.actor[RPG_Team.actor_num[i]].max_mp);
  shadow_puts(str,520,i*150+96,WHITE);

  sprintf(str,"攻击:%d",Actor_Lib.actor[RPG_Team.actor_num[i]].attack);
  shadow_puts(str,370,i*150+128,WHITE);

  sprintf(str,"防御:%d",Actor_Lib.actor[RPG_Team.actor_num[i]].defence);
  shadow_puts(str,520,i*150+128,WHITE);
 }

 sprintf(str,"%s /%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].name,RPG_Team.object_hm[Choice_Lib.choice_box[1].point_at]);
 shadow_puts(str,32,32,WHITE);

 sprintf(str,"加生命:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_hp);
 shadow_puts(str,32,64,WHITE);

 sprintf(str,"加魔法:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_mp);
 shadow_puts(str,32,96,WHITE);

 sprintf(str,"加攻击:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_attack);
 shadow_puts(str,32,128,WHITE);

 sprintf(str,"加防御:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_defence);
 shadow_puts(str,32,160,WHITE);

 sprintf(str,"加经验:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_exp);
 shadow_puts(str,32,192,WHITE);

 choice=Choice_Box(2);
 if(choice!=NO_CHOICE&&RPG_Team.actor_num[choice]!=NO_ACTOR)
 {
  Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].hp+=Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_hp;
  if(Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].hp>Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].max_hp)
    Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].hp=Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].max_hp;
  Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].mp+=Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_mp;
  if(Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].mp>Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].max_mp)
    Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].mp=Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].max_mp;
  Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].attack+=Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_attack;
  Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].defence+=Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_defence;
  Actor_Lib.actor[RPG_Team.actor_num[Choice_Lib.choice_box[2].point_at]].exp+=Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_exp;

  RPG_Team.object_hm[Choice_Lib.choice_box[1].point_at]--;
  if(RPG_Team.object_hm[Choice_Lib.choice_box[1].point_at]==0)
  {
   RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]=NO_OBJECT;
   Window.show=USE_OBJECT;
  }
 }

 if(key[KEY_Z])
 {
  PressKey(KEY_Z);
  Window.show=USE_OBJECT;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);
 }
}


/*显示物品页面*/
void UseObject()
{
 int i;
 char str[20];

 Draw_Window(32,0,19,15);
 Draw_Window(0,0,5,5);
 Draw_Window(0,160,6,7);

 for(i=0;i<13;i++)
 {
  if(RPG_Team.object_num[i]==NO_OBJECT)
  {
   shadow_puts("[没有物品]",300,32*i+31,LIGHTGRAY);
  }
  else if(Object_Lib.object[RPG_Team.object_num[i]].usage==OBJ_NEED)
  {
   shadow_puts(Object_Lib.object[RPG_Team.object_num[i]].name,300,32*i+31,LIGHTGRAY);

   sprintf(str,"/%d",RPG_Team.object_hm[i]);
   shadow_puts(str,500,32*i+31,LIGHTGRAY);
  }
  else
  {
   shadow_puts(Object_Lib.object[RPG_Team.object_num[i]].name,300,32*i+31,WHITE);

   sprintf(str,"/%d",RPG_Team.object_hm[i]);
   shadow_puts(str,500,32*i+31,WHITE);
  }
 }

 if(RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]!=NO_OBJECT)
 {
  QPutImage(32,32,Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].photo);

  sprintf(str,"加生命:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_hp);
  shadow_puts(str,32,192,WHITE);

  sprintf(str,"加魔法:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_mp);
  shadow_puts(str,32,224,WHITE);

  sprintf(str,"加攻击:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_attack);
  shadow_puts(str,32,256,WHITE);

  sprintf(str,"加防御:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_defence);
  shadow_puts(str,32,288,WHITE);

  sprintf(str,"加经验:%d",Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].add_exp);
  shadow_puts(str,32,320,WHITE);
 }

 if(Choice_Box(1)!=NO_CHOICE)
 {
  if(RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]!=NO_OBJECT&&Object_Lib.object[RPG_Team.object_num[Choice_Lib.choice_box[1].point_at]].usage!=OBJ_NEED)
  {
   Window.show=CHOOSE_ACTOR;
  }
 }

 if(key[KEY_Z])
 {
  PressKey(KEY_Z);
  Window.show=SHOW_STATUS;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);
 }

}


/*显示帮助页面*/
void ShowHelp()
{
 Draw_Window(128,128,15,3);

 shadow_puts("D:菜单    X:确认    Z:返回    F1:恢复显示模式",160,168,WHITE);

 if(key[KEY_Z])
 {
  PressKey(KEY_Z);
  Window.show=SHOW_STATUS;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);
 }
}


/*显示系统页面*/
void ShowSystem()
{
 Draw_Window(128,160,5,5);

 shadow_puts("储存",210,200,LIGHTGRAY);
 shadow_puts("读取",210,232,LIGHTGRAY);
 shadow_puts("退出",210,264,WHITE);

 if(Choice_Box(3)==2)
 {
  Window.show=ASK_QUIT;
 }

 if(key[KEY_Z])
 {
  PressKey(KEY_Z);
  Window.show=SHOW_STATUS;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);
 }
}


/*询问是否退出的页面*/
void AskQuit()
{
 Draw_Window(192,128,8,5);

 shadow_puts("真的要退出吗,记着存盘哦",224,160,RED);
 shadow_puts("再待一会!",300,200,WHITE);
 shadow_puts("要学习了!",300,232,WHITE);

 switch(Choice_Box(4))
 {
  case 0:Window.show=SHOW_STATUS;break;
  case 1:Window.state=WIN_OFF;GameFlag=GAMEQUIT;break;
  default:;
 }

 if(key[KEY_Z])
 {
  PressKey(KEY_Z);
  Window.show=SHOW_STATUS;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);
 }
}


/*显示状态页面*/
void ShowStatus()
{
 char str[30];
 int i;

 Draw_Window(32,0,19,15);
 Draw_Window(0,0,5,8);
 Draw_Window(0,320,5,5);

 for(i=0;i<RPG_Team.max_actor_num;i++)
 {
  QPutImage(200,i*150+32,Actor_Lib.actor[RPG_Team.actor_num[i]].photo);

  shadow_puts(Actor_Lib.actor[RPG_Team.actor_num[i]].name,270,i*150+32,WHITE);

  sprintf(str,"等级:%d",Actor_Lib.actor[RPG_Team.actor_num[i]].grade);
  shadow_puts(str,300,i*150+64,WHITE);

  sprintf(str,"HP:%d/%d",Actor_Lib.actor[RPG_Team.actor_num[i]].hp,Actor_Lib.actor[RPG_Team.actor_num[i]].max_hp);
  shadow_puts(str,300,i*150+96,WHITE);

  sprintf(str,"MP:%d/%d",Actor_Lib.actor[RPG_Team.actor_num[i]].mp,Actor_Lib.actor[RPG_Team.actor_num[i]].max_mp);
  shadow_puts(str,450,i*150+96,WHITE);

  sprintf(str,"攻击:%d",Actor_Lib.actor[RPG_Team.actor_num[i]].attack);
  shadow_puts(str,300,i*150+128,WHITE);

  sprintf(str,"防御:%d",Actor_Lib.actor[RPG_Team.actor_num[i]].defence);
  shadow_puts(str,450,i*150+128,WHITE);
 }

 sprintf(str,"金钱:%d",RPG_Team.money);
 shadow_puts(str,32,392,WHITE);

 shadow_puts("物品",82,32,WHITE);
 shadow_puts("装备",82,64,LIGHTGRAY);
 shadow_puts("魔法",82,98,LIGHTGRAY);
 shadow_puts("帮助",82,130,WHITE);
 shadow_puts("系统",82,162,WHITE);
 shadow_puts("返回",82,194,WHITE);

 if(key[KEY_Z])
 {
  PressKey(KEY_Z);
  Window.state=WIN_OFF;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);
 }

 switch(Choice_Box(0))
 {
  case 0:Window.show=USE_OBJECT;break;
  case 3:Window.show=SHOW_HELP;break;
  case 4:Window.show=SHOW_SYSTEM;break;
  case 5:Window.state=WIN_OFF;break;
 }
}


///////////////////////////////////////////////////////////////////////

/*窗体系统的总循环*/
void Win_Loop()
{
 Window.state=WIN_ON;

 while(Window.state==WIN_ON)
 {
  switch(Window.show)
  {
   case SHOW_STATUS: clrscreen(BLUE);ShowStatus();break;
   case USE_OBJECT: clrscreen(BLUE);UseObject();break;
   case CHOOSE_ACTOR: clrscreen(BLUE);ChooseActor();break;
   case SHOW_HELP:ShowHelp();break;
   case SHOW_SYSTEM:ShowSystem();break;
   case ASK_QUIT:clrscreen(BLUE);AskQuit();break;
   default:;
  }

  /*按F1重新图形初试化，任何时候都有效*/
  if(key[KEY_F1]){set_color_depth(16);set_gfx_mode(GFX_AUTODETECT,640,480,0,0);}

  /*延时显示缓冲区内容*/
  while(Timercounter<1);
  Timercounter=0;
  vpost();
 }
}


