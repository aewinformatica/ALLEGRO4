/******************************************/
/*               sprite.c                 */
/*                                        */
/*          水晶之约中的NPC函数库         */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/
/*NPC的加载和调用，显示在screen.c中*/

#define NPC_UP          2
#define NPC_DOWN        0
#define NPC_LEFT        1
#define NPC_RIGHT       3

#define NPC_STATIC      0        //NPC静止或脚本操作
#define NPC_MOVING      1        //NPC自由运动
#define NPC_ESCAPE      2        //NPC逃离主角

#define NPC_FREE        -1
/*NPC静止或脚本驱动，随机运动*/

/******************************************************************/

char NPC_Image_Order[8]={0,1,1,0,0,2,2,0}; /*用于查找NPC图片的数组*/



struct
{
 unsigned char max_num; /*图片总数序号*/
 Sprlib image[30];      /*字画面库数组*/            /*影子图片*/
 BITMAP *shadow;
}NPC_Image={0};             /*NPC行走画面库*/


typedef struct
{
 int xx;
 int yy;               /*用于判断逻辑方位的坐标，以子画面为单位*/
 int map_x;
 int map_y;            /*用于屏显的坐标，以象素为单位，相对于地图*/
 unsigned char dir;    /*NPC的方向*/
 unsigned char step;       /*NPC的步长*/
 unsigned char counter1;   /*用于数半步中各小步的计数器*/
 unsigned char counter2;   /*数显示那张图片的计数器*/
 unsigned char image_num;  /*图片在NPC图片库中的序号*/
 char status;          /*NPC是否移动的标志*/
 char script[30];          /*NPC的脚本文件*/
}NPC_TYPE;


struct
{
 unsigned char max_num;   /*总数序号*/
 NPC_TYPE npc[30];        /*NPC数组*/

 char scr_npc_num;        /*当前被脚本驱动行走的NPC的序号*/
 int scr_step;            /*被脚本驱动行走的步数*/

 char npc_ctrl;           /*被玩家控制的NPC的序号*/

}NPC_Lib={0};                 /*NPC库,0号NPC即为主角*/

/******************************************************************/

/*向NPC图库中加载NPC图片*/
void Load_NPC_Img(char *filename)
{
 if(NPC_Image.max_num+1>29)return;
 NPC_Image.image[NPC_Image.max_num]=loadsprlib(32,48,filename);
 NPC_Image.max_num++;
}

/******************************************************************/

/*初始化NPC*/
void InitNPC(int xx,int yy,char dir,char step,char status,unsigned char image_num,char *str)
{
 if(NPC_Lib.max_num==30)return;

 NPC_Lib.npc[NPC_Lib.max_num].xx=xx;
 NPC_Lib.npc[NPC_Lib.max_num].yy=yy;
 NPC_Lib.npc[NPC_Lib.max_num].map_x=xx*32;
 NPC_Lib.npc[NPC_Lib.max_num].map_y=yy*32;
 NPC_Lib.npc[NPC_Lib.max_num].dir=dir;
 NPC_Lib.npc[NPC_Lib.max_num].step=step;
 NPC_Lib.npc[NPC_Lib.max_num].counter1=0;
 NPC_Lib.npc[NPC_Lib.max_num].counter2=0;
 NPC_Lib.npc[NPC_Lib.max_num].image_num=image_num;
 NPC_Lib.npc[NPC_Lib.max_num].status=status;

 strcpy(NPC_Lib.npc[NPC_Lib.max_num].script,str);

 RPG_Map.data[NPC_Lib.npc[NPC_Lib.max_num].xx][NPC_Lib.npc[NPC_Lib.max_num].yy]=WALK_NO;

 NPC_Lib.max_num++;
}


/*清除所有NPC*/
void Clr_All_NPC()
{
 int i;

 for(i=0;i<NPC_Lib.max_num;i++)
 {
  RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;
 }

 NPC_Lib.max_num=0;
}
