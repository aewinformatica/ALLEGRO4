/******************************************/
/*                 map.c                  */
/*                                        */
/*         水晶之约中的地图函数库         */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/


/*地图的加载和调用，显示在screen.c中*/

#define WALK_NO  -2
#define WALK_YES -1

#define HOOK_NO     0
#define HOOK_1      1
#define HOOK_2      2

/******************************************************************/

typedef struct
{
 int max_x;                    /* x方向的格数 */
 int max_y;
 char data[100][100];           /* 地图数据 */
 char name[16];                /* 地图名称 */

  /*下面这个数据为了兼容地图逻辑文件的格式，策划时有"天空层"，
    后来发现速度不行，于是取消，但在地图编辑器中仍然保留*/
 int sky_num;                  /*供显示的天空图元数*/

 BITMAP *floor;                 /* 地图地面层的图片 */
 BITMAP *sky;                   /* 地图天空层的图片*/

}MAPDATA;                      /* 地图数据文件类型 */


MAPDATA RPG_Map;   /*游戏的地图文件*/


struct
{
 char script[30][40];
 int max_num;

 char hook_data[100][100];
 char hook_kind[100][100];

}Hook_Lib={0};               /*陷阱库*/

/******************************************************************/
/*加载陷阱脚本查询库*/
void HookScriptLib(char *script)
{
 strcpy(Hook_Lib.script[Hook_Lib.max_num],script);

 Hook_Lib.max_num++;
}

/*设置陷阱*/
void AddHook(int xx,int yy,char data,char kind)
{
 Hook_Lib.hook_data[xx][yy]=data;
 Hook_Lib.hook_kind[xx][yy]=kind;
}


/*删除陷阱*/
void DelHook(int xx,int yy)
{
 Hook_Lib.hook_data[xx][yy]=0;
 Hook_Lib.hook_kind[xx][yy]=0;
}


/*清除所有陷阱信息*/
void KillAllHook()
{
 int i,j;

 Hook_Lib.max_num=0;

 for(i=0;i<100;i++)
   for(j=0;j<100;j++)
   {
    Hook_Lib.hook_data[i][j]=0;
    Hook_Lib.hook_kind[i][j]=0;
   }
}


/*读入新地图*/
void ReadMap(MAPDATA *mp,char *filename,char *floor,char *sky)
{
 FILE *fp;
 int i,j;
 fp=fopen(filename,"rb");
 fread(&mp->max_x,2,1,fp);
 fread(&mp->max_y,2,1,fp);
 for(i=0;i<mp->max_y;i++)
   for(j=0;j<mp->max_x;j++)
     fread(&mp->data[j][i],1,1,fp);
 fread(&mp->sky_num,2,1,fp);
 fgets(mp->name,16,fp);
 fclose(fp);

 destroy_bitmap(mp->floor);
 destroy_bitmap(mp->sky);
 mp->floor=LoadBMP(floor);
 mp->sky=LoadBMP(sky);

 KillAllHook();
}



/******************************************************************/


