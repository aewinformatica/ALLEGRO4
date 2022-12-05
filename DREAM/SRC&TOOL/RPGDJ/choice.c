/******************************************/
/*                choice.c                */
/*                                        */
/*           水晶之约中的选择框           */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 1.12.2003            */
/******************************************/


int Map_Choice(char *str1,char *str2,char *str3,char *str4)
{

 dialogbox(169,174,300,150);

 shadow_puts(str1,201,190,WHITE);
 shadow_puts(str2,201,222,WHITE);
 shadow_puts(str3,201,254,WHITE);
 shadow_puts(str4,201,286,WHITE);

 vpost();

 while(1)
 {
  if(key[KEY_1])
  {
   PressKey(KEY_1);
   return 1;
  }
  else if(key[KEY_2])
  {
   PressKey(KEY_2);
   return 2;
  }
  else if(key[KEY_3])
  {
   PressKey(KEY_3);
   return 3;
  }
 }

}
