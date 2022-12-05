/******************************************/
/*               dialog.c                 */
/*                                        */
/*           水晶之约中的对话框           */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
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
 Animate talk_arrow;      /*说话时闪动的光标*/
 unsigned char photo_num; /*照片序号*/
 char string[300];        /*和NPC说的话*/

 char talk_flag;
}TalkBox={0};

/******************************************************************/

/*加载角色照片*/
void LoadPhoto(char *filename)
{
 Photo_Lib.photo[Photo_Lib.max_num]=LoadBMP(filename);
 Photo_Lib.max_num++;
}


/******************************************************************/

/*画一个对话框的函数*/
void dialogbox(int x,int y,int width,int height)
{
 int i;

 alphabar(x,y,width,height,BLUE);

 for(i=0;i<5;i++)
   rectangle(x+i,y+i,x+width-i,y+height-i,ORANGE);
}


/*对话模块*/
void RPG_Talk()
{
 if(TalkBox.talk_flag!=TALK_YES)return;  /*如果非谈话状态则返回*/

 if(TalkBox.photo_num!=60)
 {
  dialogbox(31,335,576,128);
  if(TalkBox.photo_num<50)PutImage(38,135,Photo_Lib.photo[TalkBox.photo_num]);
  boxputs16b(TalkBox.string,48,348,62,10,BLACK);
  boxputs16b(TalkBox.string,46,346,62,10,WHITE);

  showanimate(310,440,&TalkBox.talk_arrow);
 }

 /*如果按了X键则结束本句对话并将脚本设为等待*/
 if(key[KEY_X])
 {
  PressKey(KEY_X);
  TalkBox.talk_flag=TALK_NO;
  RPG_Script.script_flag=SCRIPT_WAIT;
 }
}
