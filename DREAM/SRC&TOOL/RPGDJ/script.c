/******************************************/
/*                script.c                */
/*                                        */
/*         水晶之约中的脚本解释机         */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/


/******************************************************************/

/*加载脚本*/
void Load_Script(char *fname)
{
 FILE *fp;
 char temp;
 int script_ctr=0,i=0;       /*字符循环的计数器*/
 int label_ctr=0;              /*跳转数组赋值的计数器*/
 int str_flag=FALSE;           //当前是否再读字串 
    
 if(RPG_Script.script_flag!=SCRIPT_SHUT)return;  /*假如脚本还在运行中则返回*/

 fp=fopen(fname,"r");

 strcpy(RPG_Script.filename,fname);

 while(1)
 {
    temp = getc(fp);
    if(feof(fp))break; 
    
    if(temp=='@')              /*假如遇到@为标号行*/
    {
     i=0;

     do{
         RPG_Script.label_sign[label_ctr][i]=getc(fp);
         if(RPG_Script.label_sign[label_ctr][i]=='\n'||feof(fp))
         {
          RPG_Script.label_sign[label_ctr][i]='\0';
          break;
         }
         i++;
     }while(1);

     RPG_Script.label_offset[label_ctr]=script_ctr+1;
     label_ctr++;
    }
    else if(temp=='#')         /*假如遇到#为注释行*/
      while(!feof(fp)&&getc(fp)!='\n');
    else if(temp=='\n')continue;/*假如遇到回车则忽略*/
    else if(temp==' ' && str_flag==FALSE)continue; /*假如遇到空格忽略*/
    else if(temp==';')continue; /*忽略分号*/
    else                        /*否则做命令读入*/
    {
     if(temp=='\"')
     {
          if(str_flag==TRUE)str_flag=FALSE;
          else str_flag=TRUE;
     }
     RPG_Script.command[script_ctr]=temp;

     script_ctr++;
    }
 }

 RPG_Script.command[script_ctr] = '\0';
 RPG_Script.size=script_ctr;

 fclose (fp);

 RPG_Script.cmd_offset=0;              /*脚本读取复位*/
 RPG_Script.script_flag=SCRIPT_WAIT;   /*脚本标志设为等待*/
}


/******************************************************************/

/*整数乘方*/
int Pow_Int(int a,int b)
{
 int temp=1,i;

 for(i=0;i<b;i++)
 {
  temp*=a;
 }

 return temp;
}


/*读入字符串*/
void Read_Str(char *str)
{
 int i;
 char temp;

 RPG_Script.cmd_offset++;  /*跳过第一个引号*/

 i=0;
 while(1)
 {
  temp=RPG_Script.command[RPG_Script.cmd_offset];
  if(temp=='\"')break;
  else str[i]=temp,i++,RPG_Script.cmd_offset++;
 }
 str[i]='\0';
 RPG_Script.cmd_offset++;  /*跳过第二个引号*/
 RPG_Script.cmd_offset++;  /*跳过括号或逗号*/
}


/*读入整数,如果发现$则返回该地址变量的值*/
int Read_Num()
{
 int i=0,j;
 int num[6],answer=0;
 char temp;

 if(RPG_Script.command[RPG_Script.cmd_offset]=='-')
 {
  RPG_Script.cmd_offset++;

  return -Read_Num();
 }
 else if(RPG_Script.command[RPG_Script.cmd_offset]=='$')
 {
  RPG_Script.cmd_offset++;

  return RPG_Script.temp[Read_Num()];
 }
 else
 {
  while(1)
  {
   temp=RPG_Script.command[RPG_Script.cmd_offset];
   if(temp==','||temp==')')break;
   else
   {
    num[i]=temp-48;
    i++;
    RPG_Script.cmd_offset++;
   }
  }
  RPG_Script.cmd_offset++;     /*跳过括号或逗号*/

  for(j=0;j<i;j++)
    answer+=(num[j]*Pow_Int(10,i-j-1));

  return answer;
 }
}


/******************************************************************/


void scr_talk()
{
 char str[200],temp;

 if(RPG_Script.script_flag==SCRIPT_HANG)
 {
  printf("talk:This command cannot run in hanged mode.");
  return;
 }

 temp=Read_Num();
 Read_Str(str);

 /*printf("%d,",temp);
 puts(str); */

 /*启动对话模块*/
 TalkBox.photo_num=temp;
 memset(TalkBox.string,0,300);
 strcpy(TalkBox.string,str);
 TalkBox.talk_flag=TALK_YES;

 RPG_Script.script_flag=SCRIPT_BUSY;      /*启动一条语句后，将标志设为语句执行中。由语句的执行函数来恢复等待*/
}


void scr_delay()
{
 int temp;

 if(RPG_Script.script_flag==SCRIPT_HANG)
 {
  printf("delay:This command cannot run in hanged mode.");
  return;
 }

 temp=Read_Num();

 DelayData.delay_time=temp;
 DelayData.delay_flag=DELAY_YES;

 RPG_Script.script_flag=SCRIPT_BUSY;      /*启动一条语句后，将标志设为语句执行中。由语句的执行函数来恢复等待*/
}


void scr_add()
{
 int temp1,temp2,temp3;

 temp1=Read_Num();
 temp2=Read_Num();
 temp3=Read_Num();

 if(temp3==0)RPG_Script.temp[temp1]-=temp2;
 else RPG_Script.temp[temp1]+=temp2;
}


void scr_if()
{
 int temp1,temp2,i;
 char str[32];

 temp1=Read_Num();
 temp2=Read_Num();
 Read_Str(str);

 if(RPG_Script.temp[temp1]==temp2)
 {
  for(i=0;i<32;i++)
    if(!strcmp(str,RPG_Script.label_sign[i]))
    {
     RPG_Script.cmd_offset=RPG_Script.label_offset[i]-1;
     break;
    }
 }
}


void scr_goto()
{
 char str[32];
 int i;

 Read_Str(str);

 for(i=0;i<32;i++)
  if(!strcmp(str,RPG_Script.label_sign[i]))
  {
   RPG_Script.cmd_offset=RPG_Script.label_offset[i]-1;
   break;
  }
}


void scr_npc_walk()
{
 int temp1,temp2,temp3;

 if(RPG_Script.script_flag==SCRIPT_HANG)
 {
  printf("NPC_walk:This command cannot run in hanged mode.");
  return;
 }

 temp1=Read_Num();
 temp2=Read_Num();
 temp3=Read_Num();

 NPC_Lib.scr_npc_num=temp1;
 NPC_Lib.npc[temp1].dir=temp2;
 NPC_Lib.scr_step=temp3;
 
 RPG_Script.script_flag=SCRIPT_BUSY;      /*启动一条语句后，将标志设为语句执行中。由语句的执行函数来恢复等待*/
}


void scr_npc_status()
{
 int temp1,temp2;

 temp1=Read_Num();
 temp2=Read_Num();

 NPC_Lib.npc[temp1].status=temp2;
}


void scr_scn_status()
{
 int temp;

  temp=Read_Num();

  Screen.status=temp;
}


void scr_scn_set()
{
 int tempx,tempy;

  tempx=Read_Num();
  tempy=Read_Num();

  Screen.xx=tempx;
  Screen.yy=tempy;
}


void scr_scn_mov()
{
 float tempx,tempy,temps,k;

 if(RPG_Script.script_flag==SCRIPT_HANG)
 {
  printf("screen_mov:This command cannot run in hanged mode.");
  return;
 }

 tempx=Read_Num();
 tempy=Read_Num();
 temps=Read_Num();

 Screen.target_x=tempx;
 Screen.target_y=tempy;


 if(Screen.xx==tempx)
 {
  Screen.inc_x=0;
  Screen.inc_y=(tempy>=Screen.yy?1:-1)*temps;
  Screen.mov_counter=abs(Screen.yy-tempy)/temps;
 }
 else if(Screen.yy==tempy)
 {
  Screen.inc_y=0;
  Screen.inc_x=(tempx>=Screen.xx?1:-1)*temps;
  Screen.mov_counter=abs(Screen.xx-tempx)/temps;
 }
 else
 {
  k=fabs((tempy-Screen.yy)/(tempx-Screen.xx));

  if(k<=1)
  {
   Screen.inc_x=(tempx>=Screen.xx?1:-1)*temps;
   Screen.inc_y=(tempy>=Screen.yy?k:-k)*temps;
   Screen.mov_counter=abs(Screen.xx-tempx)/temps;
  }
  else
  {
   Screen.inc_x=(tempx>=Screen.xx?1/k:-1/k)*temps;
   Screen.inc_y=(tempy>=Screen.yy?1:-1)*temps;
   Screen.mov_counter=abs(Screen.yy-tempy)/temps;
  }
 }

 Screen.script_drv=SCN_DRV_YES;

 RPG_Script.script_flag=SCRIPT_BUSY;      /*启动一条语句后，将标志设为语句执行中。由语句的执行函数来恢复等待*/
}


void scr_npc_set()
{
 int temp1,temp2,temp3;

 temp1=Read_Num();
 temp2=Read_Num();
 temp3=Read_Num();

 RPG_Map.data[NPC_Lib.npc[temp1].xx][NPC_Lib.npc[temp1].yy]=WALK_YES;
 NPC_Lib.npc[temp1].xx=temp2;
 NPC_Lib.npc[temp1].yy=temp3;
 NPC_Lib.npc[temp1].map_x=temp2*32;
 NPC_Lib.npc[temp1].map_y=temp3*32;
 NPC_Lib.npc[temp1].counter1=0;
 NPC_Lib.npc[temp1].counter2=0;
 RPG_Map.data[NPC_Lib.npc[temp1].xx][NPC_Lib.npc[temp1].yy]=WALK_NO;

}


void scr_npc_turn()
{
 int temp1,temp2;

 temp1=Read_Num();
 temp2=Read_Num();

 NPC_Lib.npc[temp1].dir=temp2;
}


void scr_set()
{
 int temp1,temp2;

 temp1=Read_Num();
 temp2=Read_Num();

 RPG_Script.temp[temp1]=temp2;
}


void scr_game_status()
{
 int temp;

 temp=Read_Num();

 if(temp==1)RPG_Script.script_flag=SCRIPT_HANG;
 else RPG_Script.script_flag=SCRIPT_WAIT;

}


void scr_load_map()
{
 char str1[30],str2[30],str3[30];

 Read_Str(str1);
 Read_Str(str2);
 Read_Str(str3);

 ReadMap(&RPG_Map,str1,str2,str3);
}


void scr_go_inout()
{
 int temp1,temp2,r,g,b,k=0;

 if(RPG_Script.script_flag==SCRIPT_HANG)
 {
  printf("screen_mov:This command cannot run in hanged mode.");
  return;
 }

 temp1=Read_Num();
 temp2=Read_Num();

 Fade.target_color=temp1;
 Fade.inout_flag=temp2;

 if(temp2==FADE_IN)Fade.counter1=255;
 else Fade.counter1=0;

 Fade.active_flag=FADE_ACTIVE;
 RPG_Script.script_flag=SCRIPT_BUSY;

}


void scr_end()
{

 RPG_Script.cmd_offset++;

 /*清空堆栈*/
 RPG_Script.fname_stack_off=0;
 RPG_Script.cmd_stack_off=0;
 RPG_Script.flag_stack_off=0;

 RPG_Script.script_flag=SCRIPT_SHUT;
}


void scr_call()
{
 char str[30];

 Read_Str(str);

 /*把当前文件名压入堆栈*/
 strcpy(RPG_Script.fname_stack[RPG_Script.fname_stack_off],RPG_Script.filename);
 RPG_Script.fname_stack_off++;

 /*当前状态压入堆栈*/
 RPG_Script.flag_stack[RPG_Script.flag_stack_off]=RPG_Script.script_flag;
 RPG_Script.flag_stack_off++;

 /*把当前指针偏移压入堆栈*/
 RPG_Script.cmd_stack[RPG_Script.cmd_stack_off]=RPG_Script.cmd_offset;
 RPG_Script.cmd_stack_off++;

 RPG_Script.script_flag=SCRIPT_SHUT;
 Load_Script(str);

 /*puts(RPG_Script.command);
 while(1); */
}


void scr_return()
{
 char str[30];

 RPG_Script.cmd_offset++;

 /*把上级文件名弹出堆栈*/
 RPG_Script.fname_stack_off--;
 strcpy(str,RPG_Script.fname_stack[RPG_Script.fname_stack_off]);

 RPG_Script.script_flag=SCRIPT_SHUT;
 Load_Script(str);

 /*上级状态弹出堆栈*/
 RPG_Script.flag_stack_off--;
 RPG_Script.script_flag=RPG_Script.flag_stack[RPG_Script.flag_stack_off];


 /*把上级指针偏移弹出堆栈*/
 RPG_Script.cmd_stack_off--;
 RPG_Script.cmd_offset=RPG_Script.cmd_stack[RPG_Script.cmd_stack_off];

}


void scr_npc_img()
{
 char str[30];

 Read_Str(str);

 Load_NPC_Img(str);
}


void scr_init_npc()
{
 int tempx,tempy,tempd,tempstep,temps,tempp;
 char str[30];

 tempx=Read_Num();
 tempy=Read_Num();
 tempd=Read_Num();
 tempstep=Read_Num();
 temps=Read_Num();
 tempp=Read_Num();
 Read_Str(str);

 InitNPC(tempx,tempy,tempd,tempstep,temps,tempp,str);
}


void scr_hook_data()
{
 char str[30];

 Read_Str(str);

 HookScriptLib(str);
}


void scr_add_hook()
{
 int tempx,tempy,tempn,tempk;

 tempx=Read_Num();
 tempy=Read_Num();
 tempn=Read_Num();
 tempk=Read_Num();

 AddHook(tempx,tempy,tempn,tempk);
}


void scr_del_hook()
{
 int tempx,tempy;

 tempx=Read_Num();
 tempy=Read_Num();

 DelHook(tempx,tempy);
}


void scr_clr_hook()
{

 RPG_Script.cmd_offset++;

 KillAllHook();
}


void scr_clr_npc()
{

 RPG_Script.cmd_offset++;

 Clr_All_NPC();
}


void scr_load_ph()
{
 char str[30];

 Read_Str(str);

 LoadPhoto(str);
}


void scr_chs_plr()
{
 int temp;

 temp=Read_Num();

 NPC_Lib.npc_ctrl=temp;
}


void scr_init_obj()
{
 int add_hp,add_mp,add_atk,add_def,add_exp,need_flag;
 char name[30],photo[30];

 add_hp=Read_Num();
 add_mp=Read_Num();
 add_atk=Read_Num();
 add_def=Read_Num();
 add_exp=Read_Num();
 need_flag=Read_Num();
 Read_Str(name);
 Read_Str(photo);

 Init_Object_Lib(add_hp,add_mp,add_atk,add_def,add_exp,need_flag,name,photo);
}


void scr_init_actor()
{
 int hp,max_hp,mp,max_mp,atk,def,exp,grade;
 char name[30],photo[30];

 hp=Read_Num();
 max_hp=Read_Num();
 mp=Read_Num();
 max_mp=Read_Num();
 atk=Read_Num();
 def=Read_Num();
 exp=Read_Num();
 grade=Read_Num();
 Read_Str(name);
 Read_Str(photo);

 Init_Actor_Lib(hp,max_hp,mp,max_mp,atk,def,exp,grade,name,photo);
}


void scr_add_actor()
{
 int actor_num;

 actor_num=Read_Num();

 Add_Actor(actor_num);
}


void scr_add_obj()
{
 int type,num;

 type=Read_Num();
 num=Read_Num();

 Add_Object(type,num);
}


void scr_weather()
{
 int weather;

 weather=Read_Num();

 Weather.weather=weather;
}


void scr_light()
{
 int color,light;

 color=Read_Num();
 light=Read_Num();

 Light.color=color;
 Light.light=light;
}


void scr_map_choice()
{
 int num;
 char str1[30],str2[30],str3[30],str4[30];

 num=Read_Num();
 Read_Str(str1);
 Read_Str(str2);
 Read_Str(str3);
 Read_Str(str4);

 RPG_Script.temp[num]=Map_Choice(str1,str2,str3,str4);
}


void scr_plr_walk()
{
 int dir,num;

 if(RPG_Script.script_flag==SCRIPT_HANG)
 {
  printf("player_walk:This command cannot run in hanged mode.");
  return;
 }

 dir=Read_Num();
 num=Read_Num();

 NPC_Lib.scr_npc_num=NPC_Lib.npc_ctrl;
 NPC_Lib.npc[NPC_Lib.npc_ctrl].dir=dir;
 NPC_Lib.scr_step=num;
 
 RPG_Script.script_flag=SCRIPT_BUSY;      /*启动一条语句后，将标志设为语句执行中。由语句的执行函数来恢复等待*/
}


void scr_get_dir()
{
 int num,NPC_num;

 NPC_num=Read_Num();
 num=Read_Num();
 
 RPG_Script.temp[num]=NPC_Lib.npc[NPC_num].dir;
}


void scr_show_img()
{
 int xx,yy,show,alpha;

 xx=Read_Num();
 yy=Read_Num();
 show=Read_Num();
 alpha=Read_Num();

 Screen_Image_Lib.xx=xx;
 Screen_Image_Lib.yy=yy;
 Screen_Image_Lib.show_num=show;
 Screen_Image_Lib.alpha_num=alpha;
}


void scr_init_img()
{
 char filename[30];

 Read_Str(filename);

 Init_Screen_Image(filename);
}


void scr_shut_img()
{
 RPG_Script.cmd_offset++;

 Shut_Screen_Image();
}


void scr_play_midi()
{
 char filename[30];

 Read_Str(filename);

 destroy_midi(Voc.music);
 Voc.music=load_midi(filename);
 play_midi(Voc.music,TRUE);
}

void scr_stop_midi()
{
 RPG_Script.cmd_offset++;

 stop_midi();
}

void scr_play_sound()
{
 char filename[30];

 Read_Str(filename);

 destroy_sample(Voc.sound);
 Voc.sound=load_wav(filename);
 play_sample(Voc.sound,255,127,1000,FALSE);
}


void scr_get_NPC_x()
{
 int npc_num,num;

 npc_num=Read_Num();
 num=Read_Num();

 RPG_Script.temp[num]=NPC_Lib.npc[npc_num].xx;
}


void scr_get_NPC_y()
{
 int npc_num,num;

 npc_num=Read_Num();
 num=Read_Num();

 RPG_Script.temp[num]=NPC_Lib.npc[npc_num].yy;
}


void scr_kill_obj()
{
 int type;

 type=Read_Num();

 Kill_Object(type);
}


void scr_quit()
{
 GameFlag=GAMEQUIT;
}


void scr_rgb()
{
 int num,r,g,b;

 num=Read_Num();
 r=Read_Num();
 g=Read_Num();
 b=Read_Num();

 RPG_Script.temp[num]=makecol(r,g,b);
}

void scr_midi_out()
{
 int i;

 RPG_Script.cmd_offset++;

 for(i=128;i>0;i-=8)
 {
  set_volume(255,i);
  while(Timercounter<1);
  Timercounter=0;
 }

 stop_midi();
 set_volume(255,128);
}


/******************************************************************/

/*根据命令选择执行脚本*/
void run_command(char *command)
{
   if(!strcmp(command,SCR_TALK))scr_talk();
   else if(!strcmp(command,SCR_DELAY))scr_delay();
   else if(!strcmp(command,SCR_ADD))scr_add();
   else if(!strcmp(command,SCR_IF))scr_if();
   else if(!strcmp(command,SCR_GOTO))scr_goto();
   else if(!strcmp(command,SCR_NPC_WALK))scr_npc_walk();
   else if(!strcmp(command,SCR_NPC_STATUS))scr_npc_status();
   else if(!strcmp(command,SCR_SCN_STATUS))scr_scn_status();
   else if(!strcmp(command,SCR_SCN_SET))scr_scn_set();
   else if(!strcmp(command,SCR_SCN_MOV))scr_scn_mov();
   else if(!strcmp(command,SCR_NPC_SET))scr_npc_set();
   else if(!strcmp(command,SCR_NPC_TURN))scr_npc_turn();
   else if(!strcmp(command,SCR_SET))scr_set();
   else if(!strcmp(command,SCR_GAME_STATUS))scr_game_status();
   else if(!strcmp(command,SCR_LOAD_MAP))scr_load_map();
   else if(!strcmp(command,SCR_GO_INOUT))scr_go_inout();
   else if(!strcmp(command,SCR_END))scr_end();
   else if(!strcmp(command,SCR_CALL))scr_call();
   else if(!strcmp(command,SCR_RETURN))scr_return();
   else if(!strcmp(command,SCR_NPC_IMG))scr_npc_img();
   else if(!strcmp(command,SCR_INIT_NPC))scr_init_npc();
   else if(!strcmp(command,SCR_HOOK_DATA))scr_hook_data();
   else if(!strcmp(command,SCR_ADD_HOOK))scr_add_hook();
   else if(!strcmp(command,SCR_DEL_HOOK))scr_del_hook();
   else if(!strcmp(command,SCR_CLR_HOOK))scr_clr_hook();
   else if(!strcmp(command,SCR_CLR_NPC))scr_clr_npc();
   else if(!strcmp(command,SCR_LOAD_PH))scr_load_ph();
   else if(!strcmp(command,SCR_CHS_PLR))scr_chs_plr();
   else if(!strcmp(command,SCR_INIT_OBJ))scr_init_obj();
   else if(!strcmp(command,SCR_INIT_ACTOR))scr_init_actor();
   else if(!strcmp(command,SCR_ADD_ACTOR))scr_add_actor();
   else if(!strcmp(command,SCR_ADD_OBJ))scr_add_obj();
   else if(!strcmp(command,SCR_WEATHER))scr_weather();
   else if(!strcmp(command,SCR_LIGHT))scr_light();
   else if(!strcmp(command,SCR_MAP_CHOICE))scr_map_choice();
   else if(!strcmp(command,SCR_PLR_WALK))scr_plr_walk();
   else if(!strcmp(command,SCR_GET_DIR))scr_get_dir();
   else if(!strcmp(command,SHOW_SCR_IMG))scr_show_img();
   else if(!strcmp(command,INIT_SCR_IMG))scr_init_img();
   else if(!strcmp(command,SHUT_SCR_IMG))scr_shut_img();
   else if(!strcmp(command,PLAY_MIDI))scr_play_midi();
   else if(!strcmp(command,STOP_MIDI))scr_stop_midi();
   else if(!strcmp(command,PLAY_SOUND))scr_play_sound();
   else if(!strcmp(command,GET_NPC_X))scr_get_NPC_x();
   else if(!strcmp(command,GET_NPC_Y))scr_get_NPC_y();
   else if(!strcmp(command,SCR_KILL_OBJ))scr_kill_obj();
   else if(!strcmp(command,SCR_QUIT))scr_quit();
   else if(!strcmp(command,SCR_RGB))scr_rgb();
   else if(!strcmp(command,SCR_MIDI_OUT))scr_midi_out();
   else
   {
    puts(RPG_Script.command);
    puts(command);
    printf("syntax error!");
    getch();
    exit(0);
   }
}


/*运行脚本,当等待状态时执行下一指令*/
void Run_Script()
{
 char temp,command[30];
 int i;

 /*如果当前脚本状态忙碌或关闭，则返回*/
 if(RPG_Script.script_flag==SCRIPT_BUSY||RPG_Script.script_flag==SCRIPT_SHUT)return;

  if(RPG_Script.cmd_offset<RPG_Script.size)
  {

   i=0;
   /*读入一条指令*/
   while(1)
   {
    temp=RPG_Script.command[RPG_Script.cmd_offset];
    if(temp=='(')break;
    else command[i]=temp,RPG_Script.cmd_offset++,i++;
   }
   command[i]='\0';
   RPG_Script.cmd_offset++;       /*跳过括号*/

   run_command(command);
  }
  else
  {
   /*清空堆栈*/
   RPG_Script.fname_stack_off=0;
   RPG_Script.cmd_stack_off=0;
   RPG_Script.flag_stack_off=0;

   RPG_Script.script_flag=SCRIPT_SHUT; /*如果脚本结束了，则将标志关闭*/
  }

}


/*main()
{
 int i,j;

 Load_Script("rpg.txt");

 puts(RPG_Script.command);
 printf("\n");

 Run_Script();

 getch();
}*/
