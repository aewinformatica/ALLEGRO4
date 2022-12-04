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
// lex.h: lexer defines & declarations

#ifndef _LEX_H_
#define _LEX_H_

#include <stdio.h>
#include "structs.h"
#include "Player.h"



typedef union {
   char    str[255];     // a character string
   long nNumber;  
   float nFloat;   
} myType;

extern CPlayer* m_pPlayer;
extern CAirManager *m_pAir;
extern CStateManager *myStateManager;
extern myType yylval;
extern myType airlval;
extern myType cnslval;

extern char *yytext;
extern char *airtext;
extern char *cnstext;

   extern int nLineNr;

   extern FILE *yyin;
   extern FILE *airin;  // the input stream
   extern FILE *cnsin;

   // Function prototype
   int yylex ();
   int airlex();
   int cnslex();

#endif
