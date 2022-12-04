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

#include "Compiler.h"
#include "parse.h"
#include "lex.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char strPraseFile[255];
int  nWho=0;


// Function used to report errors
void Error (char *format, ...)   {
   va_list args;

   switch(nWho)
   {
      case 0:
         fprintf (stderr, "File %s Line %d: %s ",strPraseFile,nLineNr,yytext);
      break;
      
      case 1:
          fprintf (stderr, "File %s Line %d: %s ",strPraseFile,nLineNr,airtext);  
      break;
      
      case 2:
         fprintf (stderr, "File %s Line %d: %s ",strPraseFile,nLineNr,cnstext);  
      break;
      
      default:
            
      break;
   }
   
   va_start(args, format);
   vfprintf(stderr, format, args);
   va_end(args);
   fprintf(stderr,"\n");
}

void yyerror(char *msg) 
{
   nWho=0;
   Error(msg);

}

void SetPlayer(CPlayer *p)
{
    m_pPlayer=p;
    PrintMessage("SetPlayer %x",p);

}

void SetAir(CAirManager *p)
{
    m_pAir=p;

}

void SetStateManager(CStateManager *p)
{
    myStateManager=p;

}

//air praser def
void airerror(char *msg)
{
    nWho=1;
    Error(msg);
}

void cnserror(char *msg)
{
    nWho=2;
    Error(msg);
}

extern "C" int airwrap()
{
	return 1;

}

extern "C" int cnswrap()
{
	return 1;

}

void PraseAir(char *strFileName)
{
    nLineNr=1;
    
    strcpy(strPraseFile,strFileName);
    
    airin = fopen(strFileName,"r");
    
    airparse();
 
}
//end of air praser
void ParseFile(char *strFileName)
{
    nLineNr=1;
    
    strcpy(strPraseFile,strFileName);
    
    yyin = fopen(strFileName,"r");
    
   yyparse();

}

void ParseConstantsFromCns(char *strFileName)
{
    nLineNr=1;
    
    strcpy(strPraseFile,strFileName);
    
    cnsin = fopen(strFileName,"r");
    
    cnsparse();

}

extern "C" int yywrap()
{
	return 1;

}
