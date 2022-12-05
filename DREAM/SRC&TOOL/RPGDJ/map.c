/******************************************/
/*                 map.c                  */
/*                                        */
/*         ˮ��֮Լ�еĵ�ͼ������         */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/


/*��ͼ�ļ��غ͵��ã���ʾ��screen.c��*/

#define WALK_NO  -2
#define WALK_YES -1

#define HOOK_NO     0
#define HOOK_1      1
#define HOOK_2      2

/******************************************************************/

typedef struct
{
 int max_x;                    /* x����ĸ��� */
 int max_y;
 char data[100][100];           /* ��ͼ���� */
 char name[16];                /* ��ͼ���� */

  /*�����������Ϊ�˼��ݵ�ͼ�߼��ļ��ĸ�ʽ���߻�ʱ��"��ղ�"��
    ���������ٶȲ��У�����ȡ�������ڵ�ͼ�༭������Ȼ����*/
 int sky_num;                  /*����ʾ�����ͼԪ��*/

 BITMAP *floor;                 /* ��ͼ������ͼƬ */
 BITMAP *sky;                   /* ��ͼ��ղ��ͼƬ*/

}MAPDATA;                      /* ��ͼ�����ļ����� */


MAPDATA RPG_Map;   /*��Ϸ�ĵ�ͼ�ļ�*/


struct
{
 char script[30][40];
 int max_num;

 char hook_data[100][100];
 char hook_kind[100][100];

}Hook_Lib={0};               /*�����*/

/******************************************************************/
/*��������ű���ѯ��*/
void HookScriptLib(char *script)
{
 strcpy(Hook_Lib.script[Hook_Lib.max_num],script);

 Hook_Lib.max_num++;
}

/*��������*/
void AddHook(int xx,int yy,char data,char kind)
{
 Hook_Lib.hook_data[xx][yy]=data;
 Hook_Lib.hook_kind[xx][yy]=kind;
}


/*ɾ������*/
void DelHook(int xx,int yy)
{
 Hook_Lib.hook_data[xx][yy]=0;
 Hook_Lib.hook_kind[xx][yy]=0;
}


/*�������������Ϣ*/
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


/*�����µ�ͼ*/
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


