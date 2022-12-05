/******************************************/
/*                delay.c                 */
/*                                        */
/*          ˮ��֮Լ�е���ʱģ��          */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/


#define DELAY_NO   0
#define DELAY_YES  1

struct
{
 char delay_flag;  /*�ű���ʱ����*/
 int delay_time;   /*��ʱʱ��*/
 int counter;      /*��ʱ�ļ�����*/
}DelayData;


void RPG_Delay()
{
 if(DelayData.delay_flag!=DELAY_YES)return;  /*���Ϊ����ʱ״̬�򷵻�*/

 DelayData.counter++;

 if(DelayData.counter==DelayData.delay_time)
 {
  DelayData.counter=0;
  DelayData.delay_flag=DELAY_NO;
  RPG_Script.script_flag=SCRIPT_WAIT;
 }
}
