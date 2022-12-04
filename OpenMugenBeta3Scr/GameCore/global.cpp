//    Open Mugen is a redevelopment of Elecbyte's M.U.G.E.N wich will be 100% compatible to it
//    Copyright (C) 2004  Sahin Vardar
//
//    If you know bugs or have a wish on Open Muegn or (money/girls/a car) for me ;-)
//    Feel free and email me: sahin_v@hotmail.com  ICQ:317502935
//    Web: http://openmugen.sourceforge.net/
//    --------------------------------------------------------------------------
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#include "global.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>



FILE *pLogFile=NULL;
Uint32 nGameTicks;
extern ConsoleInformation *mMainConsole;

void ResetGameTimer()
{
    nGameTicks=0;
}

Uint32 GetGameTicks()
{
    return nGameTicks;
}

void UpdateTimer()
{
    nGameTicks++;
}
//Prints a formated string to the log file and to the console
void PrintMessage(char *str,...)
{
  char string[255];                  // Temporary string

  va_list ap;                // Pointer To List Of Arguments
  va_start(ap, str);         // Parses The String For Variables
  vsprintf(string, str, ap); // Converts Symbols To Actual Numbers
  va_end(ap);      
  
#ifdef _XBOX
  pLogFile=fopen("d:\\log.txt","a+");
#else
  pLogFile=fopen("log.txt","a+");
#endif  
  
  fprintf(pLogFile,string);
  fprintf(pLogFile,"\n");
  fclose(pLogFile);
  
  PrintConsole(string);
    
  printf(string);
  printf("\n");

}
void Command_Handler(ConsoleInformation *console, char* command)
{
      
}

void PrintConsole(char *strText)
{
    CON_Out(mMainConsole,strText);

}

void ExtractPath(char *strPath)
{
    int l;
    l=strlen(strPath);
    
    for(int i=l;i>0;i--)
    {
        if(strPath[i]=='\\')
        {
               strPath[i+1]=0;
               break;
        }
    
    }

}

void InitLogFile()
{
   #ifdef _XBOX
	pLogFile=fopen("d:\\log.txt","w");
   #else
	pLogFile=fopen("log.txt","w");
   #endif 
	fclose(pLogFile);

}
