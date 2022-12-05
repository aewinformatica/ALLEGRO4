/******************************************/
/*                screen.c                */
/*                                        */
/*         ˮ��֮Լ�е����Ժ�����         */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/

/*��ʾ��ͼ�;���ĺ����⣬����ǣ�浽����任�������߼��ж��޹أ����Ƿ���ͬһ���ļ���*/

#define SCREEN_LOCKED  0
#define SCREEN_STATIC  1

#define SCN_DRV_YES    1
#define SCN_DRV_NO     0


struct
{
 float xx;
 float yy;   /*��Ļ����ڵ�ͼ������*/

 char script_drv; /*��Ļ�Ƿ񱻽ű�����*/
 float target_x;
 float target_y;
 float inc_x;
 float inc_y;
 int mov_counter;

 char status;  /*��Ļ�Ƿ���������*/
}Screen={0};

/******************************************************************/


/*������Ļ����ڵ�ͼ������*/
void ScreenXY(NPC_TYPE *npc)
{
 if(Screen.status==SCREEN_LOCKED)     /*������ĻΪ��������״̬*/
 {
  /*�ȼ���������Ϊ���ģ�������Ļ����*/
  Screen.xx=npc->map_x-288;
  Screen.yy=npc->map_y-224;

  /*�����ʱ�������ͼ�߽�̫����������������������ֹ��Ļ�Ƴ���ͼ*/
  if(Screen.xx<0||npc->xx<9)Screen.xx=0;
  else if(Screen.xx>RPG_Map.max_x*32-640||npc->xx>RPG_Map.max_x-9)Screen.xx=RPG_Map.max_x*32-640;

  if(Screen.yy<0||npc->yy<7)Screen.yy=0;
  else if(Screen.yy>RPG_Map.max_y*32-480||npc->yy>RPG_Map.max_y-7)Screen.yy=RPG_Map.max_y*32-480;
 }
 else if(Screen.script_drv==SCN_DRV_YES)  /*������ĻΪ�ű�����*/
 {
  if(Screen.mov_counter==0)
  {
   Screen.xx=Screen.target_x;             /*�Ը������������У��*/
   Screen.yy=Screen.target_y;

   Screen.script_drv=SCN_DRV_NO;
   RPG_Script.script_flag=SCRIPT_WAIT;    /*�ر�������־���ű�����ȴ�*/
  }
  else
  {
   Screen.xx+=Screen.inc_x;
   Screen.yy+=Screen.inc_y;

   Screen.mov_counter--;
  }
 }
}


/*��ʾ�����*/
void ShowFloor()
{
 QPutImage(-Screen.xx,-Screen.yy,RPG_Map.floor);
}


/*��ʾ��ղ�*/
void ShowSky()
{
 PutImage(-Screen.xx,-Screen.yy,RPG_Map.sky);
}

/******************************************************************/

/*����NPC�߼��������NPC��������ĺ���,һ����step������*/
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


/*����Ļ��ʾNPC*/
void ShowNPC(NPC_TYPE *npc)
{
 /*��ʾӰ��*/
 alphaimage(npc->map_x-Screen.xx,npc->map_y-Screen.yy+15,NPC_Image.shadow);

 /*��ʾNPC������*/
 putspr(npc->map_x-Screen.xx,npc->map_y-Screen.yy-24,NPC_Image_Order[npc->counter2],npc->dir,NPC_Image.image[npc->image_num]);
}

/*��ʾһ��͸��NPC*/
void AlphaNPC(NPC_TYPE *npc)
{
  alphaputspr(npc->map_x-Screen.xx,npc->map_y-Screen.yy-24,NPC_Image_Order[npc->counter2],npc->dir,NPC_Image.image[npc->image_num]);
}


/******************************************************************/

/*���뵭��ϵͳ*/

#define FADE_ACTIVE  1
#define FADE_NOTACT  0

#define FADE_IN      0
#define FADE_OUT     1


struct
{
 char active_flag; /*�Ƿ��������뵭��*/

 int target_color;                /*Ŀ����ɫ*/
 char inout_flag;                 /*���뻹�ǵ���*/

 unsigned char counter1;          /*��ɫ������*/
 unsigned char counter2;          /*��ʱ������*/
}Fade={0,0,0,255,0};


void Fade_Inout()
{
 int k;

 /*����ű��ǹرյģ��򲻴��ڵ��뵭���Ĳ���*/
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

/*����ϵͳ*/

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

/*����ϵͳ*/

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
/*��ĻС����ϵͳ*/

#define IMAGE_SHUT   -1

struct
{
 BITMAP *image[30];

 char max_num;

 int xx;
 int yy;
 char show_num;   /*����ʾ��ͼƬ���*/
 int  alpha_num;  /*ALPHAЧ����͸����*/
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
