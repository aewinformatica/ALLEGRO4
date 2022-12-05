/******************************************/
/*                object.c                */
/*                                        */
/*          水晶之约中的物品系统          */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/

#define OBJ_NEED       1
#define OBJ_UNNEED     0


typedef struct
{
 int add_hp;
 int add_mp;
 int add_attack;
 int add_defence;
 int add_exp;

 BITMAP *photo;
 char name[30]; 

 char usage;
}OBJECT_TYPE;


struct
{
 char max_num;

 OBJECT_TYPE object[50];
}Object_Lib={0};


void Init_Object_Lib(int add_hp,int add_mp,int add_attack,int add_defence,int add_exp,char usage,char *name,char *photoname)
{
 Object_Lib.object[Object_Lib.max_num].add_hp=add_hp;
 Object_Lib.object[Object_Lib.max_num].add_mp=add_mp;
 Object_Lib.object[Object_Lib.max_num].add_attack=add_attack;
 Object_Lib.object[Object_Lib.max_num].add_defence=add_defence;
 Object_Lib.object[Object_Lib.max_num].add_exp=add_exp;
 Object_Lib.object[Object_Lib.max_num].usage=usage;

 strcpy(Object_Lib.object[Object_Lib.max_num].name,name);
 Object_Lib.object[Object_Lib.max_num].photo=LoadBMP(photoname);

 Object_Lib.max_num++;
}
