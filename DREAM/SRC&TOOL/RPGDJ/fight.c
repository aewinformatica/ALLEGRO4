/******************************************/
/*                 fight.c                */
/*                                        */
/*          水晶之约中的战斗系统          */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom  1.6.2003            */
/******************************************/


#define FIGHT_WE_ATK    0      //我方攻击
#define FIGHT_THEY_ATK  1      //敌方攻击
#define FIGHT_MENU      2      //我方操作
#define CHOOSE_TARGET   3      //我方选择敌人
#define THEY_CHOOSE     4      //敌方选择攻击对象

#define FIGHT_ON        0
#define FIGHT_OFF       1

#define NO_FIGHTER      -1

#define FIGHT_SHOW_ACTOR        0
#define FIGHT_SHOW_ENIMY        1



typedef struct
{
 int hp;
 int mp;

 int attack;
 int defence;

 int exp;
 int grade;

 Animate stand;   //战斗静止态
 Animate kick;    //攻击准备态
 Animate nok;     //攻击生效态
 Animate die;     //死亡动画
 Animate def;     //被攻击动画

 SAMPLE *kick_snd;
 SAMPLE *attack_snd;
 SAMPLE *defence_snd;

}FIGHTER;


struct
{
 FIGHTER fighter[20];

 char max_num;
}Fighter_Lib={0};        //fighter查询库


typedef struct
{
 char fighter_num;

 int hp;
 int mp;

 int xx;
 int yy;

 int counter;             //用于显示等待状态的战士的帧计数
 int timetick;            //显示等待战士的计时

 char target;
}ENIMY_FIGHT;                  //敌人类型



typedef struct
{
 char actor_num;          //该主角在主角查询库中的编号

 char fighter_num;

 int xx;
 int yy;

 int counter;             //用于显示等待状态的战士的帧计数
 int timetick;            //显示等待战士的计时

 char target;
}ACTOR_FIGHT;            //主角类型


struct
{
 char show;
 char state;
 char mover;         //当前操作的战士号

 ENIMY_FIGHT enimy[3];
 ACTOR_FIGHT actor[3];

}Fight={0};


///////////////////////////////////////////////////////////////////////

void Init_Fighter_Lib(int hp,int mp,int attack,int defence,int exp,int grade,char *pic_name)
{
 char str[30];

 Fighter_Lib.fighter[Fighter_Lib.max_num].hp=hp;
 Fighter_Lib.fighter[Fighter_Lib.max_num].mp=mp;
 Fighter_Lib.fighter[Fighter_Lib.max_num].attack=attack;
 Fighter_Lib.fighter[Fighter_Lib.max_num].defence=defence;
 Fighter_Lib.fighter[Fighter_Lib.max_num].exp=exp;
 Fighter_Lib.fighter[Fighter_Lib.max_num].grade=grade;

 sprintf(str,"%s_stand.bmp",pic_name);
 Fighter_Lib.fighter[Fighter_Lib.max_num].stand=init_animate(32,48,9,2,str);
 sprintf(str,"%s_kick.bmp",pic_name);
 Fighter_Lib.fighter[Fighter_Lib.max_num].kick=init_animate(32,48,9,2,str);
 sprintf(str,"%s_nok.bmp",pic_name);
 Fighter_Lib.fighter[Fighter_Lib.max_num].nok=init_animate(32,48,9,2,str);
 sprintf(str,"%s_def.bmp",pic_name);
 Fighter_Lib.fighter[Fighter_Lib.max_num].def=init_animate(32,48,9,2,str);
 sprintf(str,"%s_die.bmp",pic_name);
 Fighter_Lib.fighter[Fighter_Lib.max_num].die=init_animate(32,48,9,2,str);

 Fighter_Lib.max_num++;
}


//在一定坐标显示相应等待状态的战士,fighter是屏幕上的序号
void Show_Fighter_Stand(int x,int y,int fighter,char ftr_kind)
{
 if(ftr_kind==FIGHT_SHOW_ENIMY)
 {
  Fight.enimy[fighter].timetick--;
  if(Fight.enimy[fighter].timetick<0)
  {
   Fight.enimy[fighter].timetick=Fighter_Lib.fighter[Fight.enimy[fighter].fighter_num].stand.timedelay;
   Fight.enimy[fighter].counter++;
   if(Fight.enimy[fighter].counter==Fighter_Lib.fighter[Fight.enimy[fighter].fighter_num].stand.frame)Fight.enimy[fighter].counter=0;
  }
 
  putspr(x,y,Fight.enimy[fighter].counter,0,Fighter_Lib.fighter[Fight.enimy[fighter].fighter_num].stand.slib);
 }
 else
 {
  Fight.actor[fighter].timetick--;
  if(Fight.actor[fighter].timetick<0)
  {
   Fight.actor[fighter].timetick=Fighter_Lib.fighter[Fight.actor[fighter].fighter_num].stand.timedelay;
   Fight.actor[fighter].counter++;
   if(Fight.actor[fighter].counter==Fighter_Lib.fighter[Fight.actor[fighter].fighter_num].stand.frame)Fight.actor[fighter].counter=0;
  }
 
  putspr(x,y,Fight.actor[fighter].counter,0,Fighter_Lib.fighter[Fight.actor[fighter].fighter_num].stand.slib);
 }
}


/*计数器清零*/
void Clr_Fight_Ctr()
{
 int i;

 for(i=0;i<3;i++)
 {
  Fight.actor[i].counter=0;
  Fight.actor[i].timetick=0;

  Fight.enimy[i].counter=0;
  Fight.enimy[i].timetick=0;
 }
}

///////////////////////////////////////////////////////////////////////

/*临时:初始化战斗系统*/
void init_fight()
{
 int i;

 Init_Fighter_Lib(100,100,3,40,20,2,"pic/fight/exp");
 Init_Fighter_Lib(20,20,20,20,20,20,"pic/fight/exp");

 for(i=0;i<3;i++)
 {
  Fight.enimy[i].hp=Fighter_Lib.fighter[Fight.enimy[i].fighter_num].hp;
  Fight.enimy[i].mp=Fighter_Lib.fighter[Fight.enimy[i].fighter_num].mp;

  Fight.enimy[i].xx=0;
  Fight.enimy[i].yy=i*50;

  Fight.actor[i].xx=300;
  Fight.actor[i].yy=i*50;
 }

 //Fight.enimy[1].fighter_num=NO_FIGHTER;
 //Fight.actor[0].fighter_num=NO_FIGHTER;
 //Fight.actor[1].fighter_num=NO_FIGHTER;
 //Fight.actor[2].fighter_num=NO_FIGHTER;
}

///////////////////////////////////////////////////////////////////////
void Show_Back()
{
 clrscreen(YELLOW);
}


/*显示所有战士*/
void Show_All_Fighter()
{
 int i;

 for(i=0;i<3;i++)
 {
  Show_Fighter_Stand(Fight.enimy[i].xx,Fight.enimy[i].yy,i,FIGHT_SHOW_ENIMY);

  Show_Fighter_Stand(Fight.actor[i].xx,Fight.actor[i].yy,i,FIGHT_SHOW_ACTOR);
 }

}


void They_Attack()
{
 int i;

 /*如此处没有敌人，则向后找到或结束,如有则启动攻击画面到结束*/
 if(Fight.enimy[Fight.mover].fighter_num==NO_FIGHTER)
 {
  Fight.mover++;
  if(Fight.mover>2)
  {
   Fight.mover=0;
   Clr_Fight_Ctr();
   Fight.show=FIGHT_MENU;
  }
  return;
 }
 else
 {
  putspr(Fight.actor[Fight.enimy[Fight.mover].target].xx,Fight.actor[Fight.enimy[Fight.mover].target].yy,Fight.enimy[Fight.mover].counter,0,Fighter_Lib.fighter[Fight.actor[Fight.enimy[Fight.mover].target].fighter_num].def.slib);
  putspr(Fight.actor[Fight.enimy[Fight.mover].target].xx,Fight.actor[Fight.enimy[Fight.mover].target].yy,Fight.enimy[Fight.mover].counter,0,Fighter_Lib.fighter[Fight.enimy[Fight.mover].fighter_num].nok.slib);
  putspr(Fight.enimy[Fight.mover].xx,Fight.enimy[Fight.mover].yy,Fight.enimy[Fight.mover].counter,0,Fighter_Lib.fighter[Fight.enimy[Fight.mover].fighter_num].kick.slib);

  for(i=0;i<3;i++)
  {
   if(i!=Fight.mover)
     Show_Fighter_Stand(Fight.enimy[i].xx,Fight.enimy[i].yy,i,FIGHT_SHOW_ENIMY);

   if(i!=Fight.enimy[Fight.mover].target)
     Show_Fighter_Stand(Fight.actor[i].xx,Fight.actor[i].yy,i,FIGHT_SHOW_ACTOR);
  }

  Fight.enimy[Fight.mover].timetick--;
  if(Fight.enimy[Fight.mover].timetick<0)
  {
   Fight.enimy[Fight.mover].timetick=Fighter_Lib.fighter[Fight.enimy[Fight.mover].fighter_num].kick.timedelay;
   Fight.enimy[Fight.mover].counter++;

   if(Fight.enimy[Fight.mover].counter==Fighter_Lib.fighter[Fight.enimy[Fight.mover].fighter_num].kick.frame)
   {
    Fight.enimy[Fight.mover].counter=0;

    Fight.mover++;
    if(Fight.mover>2)
    {
     Fight.mover=0;
     Clr_Fight_Ctr();
     Fight.show=FIGHT_MENU;
    }
   }
  }
 }
}


void They_Choose()
{
 int i,choose;

 for(i=0;i<3;i++)
 {
  if(Fight.enimy[i].fighter_num!=NO_FIGHTER)
  {
   do
   {
    choose=randnum(3);
   }while(Fight.actor[choose].fighter_num==NO_FIGHTER);

   Fight.enimy[i].target=choose;
  }
 }

 Fight.show=FIGHT_THEY_ATK;
}


void We_Attack()
{
 int i;

 /*如此处没有主角，则向后找到或结束,如有则启动攻击画面到结束*/
 if(Fight.actor[Fight.mover].fighter_num==NO_FIGHTER)
 {
  Fight.mover++;
  if(Fight.mover>2)
  {
   Fight.mover=0;
   Clr_Fight_Ctr();
   Fight.show=THEY_CHOOSE;
  }
  return;
 }
 else
 {
  putspr(Fight.enimy[Fight.actor[Fight.mover].target].xx,Fight.enimy[Fight.actor[Fight.mover].target].yy,Fight.actor[Fight.mover].counter,0,Fighter_Lib.fighter[Fight.enimy[Fight.actor[Fight.mover].target].fighter_num].def.slib);
  putspr(Fight.enimy[Fight.actor[Fight.mover].target].xx,Fight.enimy[Fight.actor[Fight.mover].target].yy,Fight.actor[Fight.mover].counter,0,Fighter_Lib.fighter[Fight.actor[Fight.mover].fighter_num].nok.slib);
  putspr(Fight.actor[Fight.mover].xx,Fight.actor[Fight.mover].yy,Fight.actor[Fight.mover].counter,0,Fighter_Lib.fighter[Fight.actor[Fight.mover].fighter_num].kick.slib);

  for(i=0;i<3;i++)
  {
   if(i!=Fight.actor[Fight.mover].target)
     Show_Fighter_Stand(Fight.enimy[i].xx,Fight.enimy[i].yy,i,FIGHT_SHOW_ENIMY);

   if(i!=Fight.mover)
     Show_Fighter_Stand(Fight.actor[i].xx,Fight.actor[i].yy,i,FIGHT_SHOW_ACTOR);
  }

  Fight.actor[Fight.mover].timetick--;
  if(Fight.actor[Fight.mover].timetick<0)
  {
   Fight.actor[Fight.mover].timetick=Fighter_Lib.fighter[Fight.actor[Fight.mover].fighter_num].kick.timedelay;
   Fight.actor[Fight.mover].counter++;

   if(Fight.actor[Fight.mover].counter==Fighter_Lib.fighter[Fight.actor[Fight.mover].fighter_num].kick.frame)
   {
    Fight.actor[Fight.mover].counter=0;

    Fight.mover++;
    if(Fight.mover>2)
    {
     Fight.mover=0;
     Clr_Fight_Ctr();
     Fight.show=THEY_CHOOSE;
    }
   }
  }
 }
}


void Choose_Target()
{
 char choice;

 Show_All_Fighter();

 choice=Choice_Box(6);
 if(choice!=NO_CHOICE)
 {
  //如果此处没有敌人则不可选，如有敌人则选择，主角向后一个
  if(Fight.enimy[choice].fighter_num!=NO_FIGHTER)
  {
   Fight.actor[Fight.mover].target=choice;
   Fight.mover++;

   Fight.show=FIGHT_MENU;
  }
 }

 if(key[KEY_Z])
 {
  PressKey(KEY_Z);
  Fight.show=FIGHT_MENU;

  play_sample(Choice_Lib.button,255,127,1000,FALSE);
 }
}


void Fight_Menu()
{
 int i;

 //如果该位没有主角，则向后到找到或结束，如有主角，则执行到结束
 if(Fight.actor[Fight.mover].fighter_num==NO_FIGHTER)
 {
  Fight.mover++;

  if(Fight.mover>2)
  {
   Fight.mover=0;
   Clr_Fight_Ctr();
   Fight.show=FIGHT_WE_ATK;
  }

  return;
 }
 else
 {
  if(Fight.mover>2)
  {
   Fight.mover=0;
   Clr_Fight_Ctr();
   Fight.show=FIGHT_WE_ATK;
  }
  else
  {
   Show_All_Fighter();

   Draw_Window(0,0,5,5);

   shadow_puts("攻击",100,32,WHITE);
   shadow_puts("魔法",100,64,LIGHTGRAY);
   shadow_puts("物品",100,96,WHITE);
   shadow_puts("逃跑",100,128,WHITE);

   switch(Choice_Box(5))
   {
    case 0:Fight.show=CHOOSE_TARGET;break;
    case 3:Fight.state=FIGHT_OFF;break;
    default:;
   }
  }
 }
}


//显示主角和敌人的血值
void Show_Blood()
{
 char str[20];
 int i;

 for(i=0;i<3;i++)
 {
  if(Fight.enimy[i].fighter_num!=NO_FIGHTER)
  {
   sprintf(str,"敌人%dHP:%d",i,Fight.enimy[i].hp);
   shadow_puts(str,0,i*32+200,BLUE);
  }

  if(Fight.actor[i].fighter_num!=NO_FIGHTER)
  {
   sprintf(str,"主角%dHP:%d",i,Actor_Lib.actor[Fight.actor[i].actor_num].hp);
   shadow_puts(str,300,i*32+200,BLUE);
  }
 }

}


//战斗系统初始化，从大地图中进行
//包括将所需的敌人从敌人库中取出
//以及将主角从链接入战斗系统
void Init_Fight_System(char enimy_fighter_num1,char enimy_fighter_num2,char enimy_fighter_num3)
{
 int i;

 Fight.enimy[0].fighter_num=enimy_fighter_num1;
 Fight.enimy[1].fighter_num=enimy_fighter_num2;
 Fight.enimy[2].fighter_num=enimy_fighter_num3;

 for(i=0;i<3;i++)
 {

  //如果此位被设置了敌人则加载相应信息
  if(Fight.enimy[i].fighter_num!=NO_FIGHTER)
  {
   Fight.enimy[i].hp=Fighter_Lib.fighter[Fight.enimy[i].fighter_num].hp;
   Fight.enimy[i].mp=Fighter_Lib.fighter[Fight.enimy[i].fighter_num].mp;
  }

  //如果队中该位有主角，则赋值，如无主角，则置空
  if(RPG_Team.actor_num[i]!=NO_ACTOR)
  {
   Fight.actor[i].actor_num=RPG_Team.actor_num[i];
   Fight.actor[i].fighter_num=Actor_Lib.actor[Fight.actor[i].actor_num].fighter_num;
  }
  else
  {
   Fight.actor[i].fighter_num=NO_FIGHTER;
  }

 }

}

///////////////////////////////////////////////////////////////////////
/*战斗循环*/
void Fight_Loop()
{
 Fight.state=FIGHT_ON;
 Fight.show=FIGHT_MENU;

 Init_Fight_System(0,0,1);

 while(Fight.state==FIGHT_ON)
 {
  Show_Back();

  switch(Fight.show)
  {
   case FIGHT_MENU:Fight_Menu();break;
   case CHOOSE_TARGET:Choose_Target();break;
   case FIGHT_WE_ATK:We_Attack();break;
   case THEY_CHOOSE:They_Choose();break;
   case FIGHT_THEY_ATK:They_Attack();break;
   default:;
  }

  {
   char str[20];
   sprintf(str,"%d,%d,%d",Fight.show,Fight.enimy[0].timetick,Fight.enimy[0].counter);
   puts16b(str,200,200,BLUE);
  }

  /*显示血值*/
  Show_Blood();

  /*延时显示缓冲区内容*/
  while(Timercounter<1);
  Timercounter=0;
  vpost();
 }

}
