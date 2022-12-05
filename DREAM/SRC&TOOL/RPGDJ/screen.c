/******************************************/
/*                screen.c                */
/*                                        */
/*         水晶之约中的屏显函数库         */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/

/*显示地图和精灵的函数库，由于牵涉到坐标变换，且与逻辑判断无关，于是放在同一个文件里*/

#define SCREEN_LOCKED  0
#define SCREEN_STATIC  1

#define SCN_DRV_YES    1
#define SCN_DRV_NO     0


struct
{
 float xx;
 float yy;   /*屏幕相对于地图的坐标*/

 char script_drv; /*屏幕是否被脚本驱动*/
 float target_x;
 float target_y;
 float inc_x;
 float inc_y;
 int mov_counter;

 char status;  /*屏幕是否锁定主角*/
}Screen={0};

/******************************************************************/


/*计算屏幕相对于地图的坐标*/
void ScreenXY(NPC_TYPE *npc)
{
 if(Screen.status==SCREEN_LOCKED)     /*假如屏幕为锁定主角状态*/
 {
  /*先假设以主角为中心，计算屏幕坐标*/
  Screen.xx=npc->map_x-288;
  Screen.yy=npc->map_y-224;

  /*如果此时主角离地图边界太近，则对坐标加以修正，防止屏幕移出地图*/
  if(Screen.xx<0||npc->xx<9)Screen.xx=0;
  else if(Screen.xx>RPG_Map.max_x*32-640||npc->xx>RPG_Map.max_x-9)Screen.xx=RPG_Map.max_x*32-640;

  if(Screen.yy<0||npc->yy<7)Screen.yy=0;
  else if(Screen.yy>RPG_Map.max_y*32-480||npc->yy>RPG_Map.max_y-7)Screen.yy=RPG_Map.max_y*32-480;
 }
 else if(Screen.script_drv==SCN_DRV_YES)  /*假如屏幕为脚本驱动*/
 {
  if(Screen.mov_counter==0)
  {
   Screen.xx=Screen.target_x;             /*对浮点数结果进行校正*/
   Screen.yy=Screen.target_y;

   Screen.script_drv=SCN_DRV_NO;
   RPG_Script.script_flag=SCRIPT_WAIT;    /*关闭驱动标志，脚本进入等待*/
  }
  else
  {
   Screen.xx+=Screen.inc_x;
   Screen.yy+=Screen.inc_y;

   Screen.mov_counter--;
  }
 }
}


/*显示地面层*/
void ShowFloor()
{
 QPutImage(-Screen.xx,-Screen.yy,RPG_Map.floor);
}


/*显示天空层*/
void ShowSky()
{
 PutImage(-Screen.xx,-Screen.yy,RPG_Map.sky);
}

/******************************************************************/

/*根据NPC逻辑坐标计算NPC屏显坐标的函数,一步走step个象素*/
void NPCXY(NPC_TYPE *npc)
{
 switch(npc->dir)
 {
  case NPC_UP:    npc->map_y=npc->yy*32+npc->counter1*npc->step;
                  npc->map_x=npc->xx*32;
                  break;

  case NPC_DOWN:  npc->map_y=npc->yy*32-npc->counter1*npc->step;
                  npc->map_x=npc->xx*32;
                  break;

  case NPC_LEFT:  npc->map_x=npc->xx*32+npc->counter1*npc->step;
                  npc->map_y=npc->yy*32;
                  break;

  case NPC_RIGHT: npc->map_x=npc->xx*32-npc->counter1*npc->step;
                  npc->map_y=npc->yy*32;
                  break;

  default:;
 }
}


/*在屏幕显示NPC*/
void ShowNPC(NPC_TYPE *npc)
{
 /*显示影子*/
 alphaimage(npc->map_x-Screen.xx,npc->map_y-Screen.yy+15,NPC_Image.shadow);

 /*显示NPC或主角*/
 putspr(npc->map_x-Screen.xx,npc->map_y-Screen.yy-24,NPC_Image_Order[npc->counter2],npc->dir,NPC_Image.image[npc->image_num]);
}

/*显示一个透明NPC*/
void AlphaNPC(NPC_TYPE *npc)
{
  alphaputspr(npc->map_x-Screen.xx,npc->map_y-Screen.yy-24,NPC_Image_Order[npc->counter2],npc->dir,NPC_Image.image[npc->image_num]);
}


/******************************************************************/

/*淡入淡出系统*/

#define FADE_ACTIVE  1
#define FADE_NOTACT  0

#define FADE_IN      0
#define FADE_OUT     1


struct
{
 char active_flag; /*是否启动淡入淡出*/

 int target_color;                /*目标颜色*/
 char inout_flag;                 /*淡入还是淡出*/

 unsigned char counter1;          /*颜色计数器*/
 unsigned char counter2;          /*延时计数器*/
}Fade={0,0,0,255,0};


void Fade_Inout()
{
 int k;

 /*如果脚本是关闭的，则不存在淡入淡出的操作*/
 if(RPG_Script.script_flag==SCRIPT_SHUT)return;

 else if(Fade.active_flag==FADE_ACTIVE)
 {
  set_trans_blender(0,0,0,Fade.counter1);
  drawing_mode(DRAW_MODE_TRANS ,V_Buffer,640,480);
  rectfill(V_Buffer,0,0,639,479,Fade.target_color);
  
  if(Fade.inout_flag==FADE_IN)
  {
   Fade.counter1-=15;
   if(Fade.counter1==0)
   {
    Fade.active_flag=FADE_NOTACT;
    RPG_Script.script_flag=SCRIPT_WAIT;
   }
  }
  else
  {
   Fade.counter1+=15;
   if(Fade.counter1==255)
   {
    Fade.active_flag=FADE_NOTACT;
    RPG_Script.script_flag=SCRIPT_WAIT;
   }   
  }
 }
 else
 {
  set_trans_blender(0,0,0,Fade.counter1);
  drawing_mode(DRAW_MODE_TRANS ,V_Buffer,640,480);
  rectfill(V_Buffer,0,0,639,479,Fade.target_color);
 }
}


/******************************************************************/

/*光照系统*/

struct
{
 int color;
 unsigned char light;
}Light={0,0};


void Light_Effect()
{
 if(Light.light!=0)
 {
  set_trans_blender(0,0,0,Light.light);
  drawing_mode(DRAW_MODE_TRANS ,V_Buffer,640,480);
  rectfill(V_Buffer,0,0,639,479,Light.color);
 }
}

/******************************************************************/

/*天气系统*/

#define GOOD_DAY   0
#define SNOW_DAY   1

typedef struct
{
 int xx;
 int yy;
}SNOW;


struct
{
 int weather;

 SNOW snow[3][100];
}Weather={0};


void Init_Weather()
{
 int i,j;

 for(i=0;i<3;i++)
   for(j=0;j<100;j++)
   {
    Weather.snow[i][j].xx=randnum(640);
    Weather.snow[i][j].yy=randnum(480);
   }
}


void Weather_Effect()
{
 int i,j;

 if(Weather.weather==SNOW_DAY)
 {
  for(i=0;i<3;i++)
    for(j=0;j<100;j++)
    {
     Weather.snow[i][j].yy+=(i+1);
     if(Weather.snow[i][j].yy>479)Weather.snow[i][j].yy=0;

     putpixel(V_Buffer,Weather.snow[i][j].xx,Weather.snow[i][j].yy,WHITE);
     putpixel(V_Buffer,Weather.snow[i][j].xx+1,Weather.snow[i][j].yy,LIGHTGRAY);
     putpixel(V_Buffer,Weather.snow[i][j].xx,Weather.snow[i][j].yy+1,LIGHTGRAY);
     putpixel(V_Buffer,Weather.snow[i][j].xx+1,Weather.snow[i][j].yy+1,DARKGRAY);
    }
 }
}


/******************************************************************/
/*屏幕小画面系统*/

#define IMAGE_SHUT   -1

struct
{
 BITMAP *image[30];

 char max_num;

 int xx;
 int yy;
 char show_num;   /*被显示的图片序号*/
 int  alpha_num;  /*ALPHA效果的透明度*/
}Screen_Image_Lib={0};


void Init_Screen_Image(char *filename)
{
 Screen_Image_Lib.image[Screen_Image_Lib.max_num]=LoadBMP(filename);

 Screen_Image_Lib.max_num++;
}


void Show_Screen_Image()
{
 if(Screen_Image_Lib.show_num==IMAGE_SHUT)return;

 set_trans_blender(0,0,0,Screen_Image_Lib.alpha_num);
 draw_trans_sprite(V_Buffer,Screen_Image_Lib.image[Screen_Image_Lib.show_num],Screen_Image_Lib.xx,Screen_Image_Lib.yy);
}


void Shut_Screen_Image()
{
 Screen_Image_Lib.show_num=IMAGE_SHUT;
}
