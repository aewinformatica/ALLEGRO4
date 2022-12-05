/******************************************/
/*                 actor.c                */
/*                                        */
/*          ˮ��֮Լ�е�����ϵͳ          */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom  1.6.2004            */
/******************************************/


#define NO_ACTOR    -1
#define NO_OBJECT   -1


typedef struct
{
 char name[20];

 int hp;
 int max_hp;
 int mp;
 int max_mp;
 int attack;
 int defence;

 //װ������

 int exp;
 int grade;

 char fighter_num;        //�����ǵ�ս����̬���

 BITMAP *photo;
}ACTOR_TYPE;


struct
{
 ACTOR_TYPE actor[8];

 char max_num;
}Actor_Lib;


struct
{
 int money;

 char actor_num[3];
 char max_actor_num;

 char object_num[13];
 int object_hm[13];               //ÿ����Ʒ��

 //װ������
}RPG_Team={0};


void Init_Actor_Lib(int hp,int max_hp,int mp,int max_mp,int attack,int defence,int exp,int grade,char *name,char *photoname)
{
 Actor_Lib.actor[Actor_Lib.max_num].hp=hp;
 Actor_Lib.actor[Actor_Lib.max_num].max_hp=max_hp;
 Actor_Lib.actor[Actor_Lib.max_num].mp=mp;
 Actor_Lib.actor[Actor_Lib.max_num].max_mp=max_mp;
 Actor_Lib.actor[Actor_Lib.max_num].attack=attack;
 Actor_Lib.actor[Actor_Lib.max_num].defence=defence;
 Actor_Lib.actor[Actor_Lib.max_num].exp=exp;
 Actor_Lib.actor[Actor_Lib.max_num].grade=grade;
 strcpy(Actor_Lib.actor[Actor_Lib.max_num].name,name);
 Actor_Lib.actor[Actor_Lib.max_num].photo=LoadBMP(photoname);

 Actor_Lib.max_num++;
}


/*��ʼ����������*/
void Init_Team()
{
 int i;

 //��ʼ������
 for(i=0;i<3;i++)
   RPG_Team.actor_num[i]=NO_ACTOR;

 //��ʼ����Ʒ��
 for(i=0;i<13;i++)
   RPG_Team.object_num[i]=NO_OBJECT;
}


void Add_Actor(char num)
{
 RPG_Team.actor_num[RPG_Team.max_actor_num]=num;
 RPG_Team.max_actor_num++;
}


void Add_Object(char type,char num)
{
 int i;

 //�����Ѿ����ֳɵĿռ�
 for(i=0;i<13;i++)
 {
  if(RPG_Team.object_num[i]==type)
  {
   RPG_Team.object_hm[i]+=num;
   return;
  }
 }

 //���绹û�д�����Ʒ
 for(i=0;i<13;i++)
 {
  if(RPG_Team.object_num[i]==NO_OBJECT)
  {
   RPG_Team.object_num[i]=type;
   RPG_Team.object_hm[i]+=num;
   return;
  }
 }
}


void Kill_Object(char type)
{
 int i;
 for(i=0;i<13;i++)
 {
  if(RPG_Team.object_num[i]==type)
  {
   RPG_Team.object_num[i]=NO_OBJECT;
   RPG_Team.object_hm[i]=0;
   return;
  }
 }
}
