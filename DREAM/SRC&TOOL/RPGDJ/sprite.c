/******************************************/
/*               sprite.c                 */
/*                                        */
/*          ˮ��֮Լ�е�NPC������         */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/
/*NPC�ļ��غ͵��ã���ʾ��screen.c��*/

#define NPC_UP          2
#define NPC_DOWN        0
#define NPC_LEFT        1
#define NPC_RIGHT       3

#define NPC_STATIC      0        //NPC��ֹ��ű�����
#define NPC_MOVING      1        //NPC�����˶�
#define NPC_ESCAPE      2        //NPC��������

#define NPC_FREE        -1
/*NPC��ֹ��ű�����������˶�*/

/******************************************************************/

char NPC_Image_Order[8]={0,1,1,0,0,2,2,0}; /*���ڲ���NPCͼƬ������*/



struct
{
 unsigned char max_num; /*ͼƬ�������*/
 Sprlib image[30];      /*�ֻ��������*/            /*Ӱ��ͼƬ*/
 BITMAP *shadow;
}NPC_Image={0};             /*NPC���߻����*/


typedef struct
{
 int xx;
 int yy;               /*�����ж��߼���λ�����꣬���ӻ���Ϊ��λ*/
 int map_x;
 int map_y;            /*�������Ե����꣬������Ϊ��λ������ڵ�ͼ*/
 unsigned char dir;    /*NPC�ķ���*/
 unsigned char step;       /*NPC�Ĳ���*/
 unsigned char counter1;   /*�������벽�и�С���ļ�����*/
 unsigned char counter2;   /*����ʾ����ͼƬ�ļ�����*/
 unsigned char image_num;  /*ͼƬ��NPCͼƬ���е����*/
 char status;          /*NPC�Ƿ��ƶ��ı�־*/
 char script[30];          /*NPC�Ľű��ļ�*/
}NPC_TYPE;


struct
{
 unsigned char max_num;   /*�������*/
 NPC_TYPE npc[30];        /*NPC����*/

 char scr_npc_num;        /*��ǰ���ű��������ߵ�NPC�����*/
 int scr_step;            /*���ű��������ߵĲ���*/

 char npc_ctrl;           /*����ҿ��Ƶ�NPC�����*/

}NPC_Lib={0};                 /*NPC��,0��NPC��Ϊ����*/

/******************************************************************/

/*��NPCͼ���м���NPCͼƬ*/
void Load_NPC_Img(char *filename)
{
 if(NPC_Image.max_num+1>29)return;
 NPC_Image.image[NPC_Image.max_num]=loadsprlib(32,48,filename);
 NPC_Image.max_num++;
}

/******************************************************************/

/*��ʼ��NPC*/
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


/*�������NPC*/
void Clr_All_NPC()
{
 int i;

 for(i=0;i<NPC_Lib.max_num;i++)
 {
  RPG_Map.data[NPC_Lib.npc[i].xx][NPC_Lib.npc[i].yy]=WALK_YES;
 }

 NPC_Lib.max_num=0;
}
