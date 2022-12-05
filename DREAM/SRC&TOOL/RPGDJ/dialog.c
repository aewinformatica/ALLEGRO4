/******************************************/
/*               dialog.c                 */
/*                                        */
/*           ˮ��֮Լ�еĶԻ���           */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/


#define TALK_YES 1
#define TALK_NO  0

/******************************************************************/

struct
{
 unsigned char max_num;
 BITMAP *photo[20];
}Photo_Lib={0};


struct
{
 Animate talk_arrow;      /*˵��ʱ�����Ĺ��*/
 unsigned char photo_num; /*��Ƭ���*/
 char string[300];        /*��NPC˵�Ļ�*/

 char talk_flag;
}TalkBox={0};

/******************************************************************/

/*���ؽ�ɫ��Ƭ*/
void LoadPhoto(char *filename)
{
 Photo_Lib.photo[Photo_Lib.max_num]=LoadBMP(filename);
 Photo_Lib.max_num++;
}


/******************************************************************/

/*��һ���Ի���ĺ���*/
void dialogbox(int x,int y,int width,int height)
{
 int i;

 alphabar(x,y,width,height,BLUE);

 for(i=0;i<5;i++)
   rectangle(x+i,y+i,x+width-i,y+height-i,ORANGE);
}


/*�Ի�ģ��*/
void RPG_Talk()
{
 if(TalkBox.talk_flag!=TALK_YES)return;  /*�����̸��״̬�򷵻�*/

 if(TalkBox.photo_num!=60)
 {
  dialogbox(31,335,576,128);
  if(TalkBox.photo_num<50)PutImage(38,135,Photo_Lib.photo[TalkBox.photo_num]);
  boxputs16b(TalkBox.string,48,348,62,10,BLACK);
  boxputs16b(TalkBox.string,46,346,62,10,WHITE);

  showanimate(310,440,&TalkBox.talk_arrow);
 }

 /*�������X�����������Ի������ű���Ϊ�ȴ�*/
 if(key[KEY_X])
 {
  PressKey(KEY_X);
  TalkBox.talk_flag=TALK_NO;
  RPG_Script.script_flag=SCRIPT_WAIT;
 }
}
