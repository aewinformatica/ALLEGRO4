/******************************************/
/*                delay.c                 */
/*                                        */
/*          水晶之约中的延时模块          */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/


#define DELAY_NO   0
#define DELAY_YES  1

struct
{
 char delay_flag;  /*脚本延时开关*/
 int delay_time;   /*延时时间*/
 int counter;      /*延时的计数器*/
}DelayData;


void RPG_Delay()
{
 if(DelayData.delay_flag!=DELAY_YES)return;  /*如果为非延时状态则返回*/

 DelayData.counter++;

 if(DelayData.counter==DelayData.delay_time)
 {
  DelayData.counter=0;
  DelayData.delay_flag=DELAY_NO;
  RPG_Script.script_flag=SCRIPT_WAIT;
 }
}
