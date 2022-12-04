
/*  A Bison parser, made from cnsconstprase.txt with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse cnsparse
#define yylex cnslex
#define yyerror cnserror
#define yylval cnslval
#define yychar cnschar
#define yydebug cnsdebug
#define yynerrs cnsnerrs
#define	EQUAL	258
#define	DATA	259
#define	LIFE	260
#define	ATTACK	261
#define	DEFENCE	262
#define	FALLDEFUP	263
#define	LIEDOWTIME	264
#define	AIRJUGGLE	265
#define	SPARKNO	266
#define	GUARDSPARKNO	267
#define	KOECHO	268
#define	VOLUME	269
#define	INTEGERINDEX	270
#define	FLOATINDEX	271
#define	SIZE	272
#define	XSCALE	273
#define	YSCALE	274
#define	GROUNDBACK	275
#define	GROUNDFRONT	276
#define	AIRBACK	277
#define	AIRFRONT	278
#define	HEIGHT	279
#define	ATTACKDIST	280
#define	PROJATTDIST	281
#define	PROJDOSCALE	282
#define	HEADPOS	283
#define	MIDPOS	284
#define	SHADOWOFFSET	285
#define	DRAWOFFSET	286
#define	VELOCITY	287
#define	WALKFWD	288
#define	WALKBACK	289
#define	RUNFWD	290
#define	RUNBACK	291
#define	JUMPNEU	292
#define	JUMPBACK	293
#define	JUMPFWD	294
#define	RUNJUMPBACK	295
#define	RUNJUMPFWD	296
#define	AIRJUMPNEU	297
#define	AIRJUMPBACK	298
#define	AIRJUMPFWD	299
#define	MOVEMENT	300
#define	AIRJUMPNUM	301
#define	AIRJUMPHEIGHT	302
#define	YACCEL	303
#define	STANDFRICTION	304
#define	CROUCHFRICTION	305
#define	NUMBER	306

#line 1 "cnsconstprase.txt"

#include <malloc.h>  // _alloca is used by the parser
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "Structs.h"
#include "Player.h"


extern CPlayer *m_pPlayer;


void yyerror(char *msg);


#define YYSTYPE myType


#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		168
#define	YYFLAG		-32768
#define	YYNTBASE	53

#define YYTRANSLATE(x) ((unsigned)(x) <= 306 ? yytranslate[x] : 59)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    52,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     6,     8,    10,    12,    14,    16,    20,
    24,    28,    32,    36,    40,    44,    48,    52,    56,    60,
    64,    66,    70,    74,    78,    82,    86,    90,    94,    98,
   102,   106,   112,   118,   122,   128,   130,   134,   138,   144,
   148,   154,   158,   164,   168,   174,   178,   184,   188,   194,
   198,   204,   208,   214,   218,   224,   228,   234,   238,   240,
   244,   248,   252,   256
};

static const short yyrhs[] = {    -1,
    53,    54,     0,     1,     0,    55,     0,    56,     0,    57,
     0,    58,     0,     4,     0,     5,     3,    51,     0,     6,
     3,    51,     0,     7,     3,    51,     0,     8,     3,    51,
     0,     9,     3,    51,     0,    10,     3,    51,     0,    11,
     3,    51,     0,    12,     3,    51,     0,    13,     3,    51,
     0,    14,     3,    51,     0,    15,     3,    51,     0,    16,
     3,    51,     0,    17,     0,    18,     3,    51,     0,    19,
     3,    51,     0,    20,     3,    51,     0,    21,     3,    51,
     0,    22,     3,    51,     0,    23,     3,    51,     0,    24,
     3,    51,     0,    25,     3,    51,     0,    26,     3,    51,
     0,    27,     3,    51,     0,    28,     3,    51,    52,    51,
     0,    29,     3,    51,    52,    51,     0,    30,     3,    51,
     0,    31,     3,    51,    52,    51,     0,    32,     0,    33,
     3,    51,     0,    34,     3,    51,     0,    35,     3,    51,
    52,    51,     0,    35,     3,    51,     0,    36,     3,    51,
    52,    51,     0,    36,     3,    51,     0,    37,     3,    51,
    52,    51,     0,    37,     3,    51,     0,    38,     3,    51,
    52,    51,     0,    38,     3,    51,     0,    39,     3,    51,
    52,    51,     0,    39,     3,    51,     0,    40,     3,    51,
    52,    51,     0,    40,     3,    51,     0,    41,     3,    51,
    52,    51,     0,    41,     3,    51,     0,    42,     3,    51,
    52,    51,     0,    42,     3,    51,     0,    43,     3,    51,
    52,    51,     0,    43,     3,    51,     0,    44,     3,    51,
    52,    51,     0,    44,     3,    51,     0,    45,     0,    46,
     3,    51,     0,    47,     3,    51,     0,    48,     3,    51,
     0,    49,     3,    51,     0,    50,     3,    51,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    76,    77,    78,    82,    83,    84,    85,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
   113,   114,   117,   120,   122,   126,   127,   128,   130,   132,
   134,   136,   138,   140,   142,   144,   146,   148,   150,   152,
   154,   156,   158,   160,   162,   164,   166,   168,   171,   172,
   173,   174,   175,   176
};

static const char * const yytname[] = {   "$","error","$undefined.","EQUAL",
"DATA","LIFE","ATTACK","DEFENCE","FALLDEFUP","LIEDOWTIME","AIRJUGGLE","SPARKNO",
"GUARDSPARKNO","KOECHO","VOLUME","INTEGERINDEX","FLOATINDEX","SIZE","XSCALE",
"YSCALE","GROUNDBACK","GROUNDFRONT","AIRBACK","AIRFRONT","HEIGHT","ATTACKDIST",
"PROJATTDIST","PROJDOSCALE","HEADPOS","MIDPOS","SHADOWOFFSET","DRAWOFFSET","VELOCITY",
"WALKFWD","WALKBACK","RUNFWD","RUNBACK","JUMPNEU","JUMPBACK","JUMPFWD","RUNJUMPBACK",
"RUNJUMPFWD","AIRJUMPNEU","AIRJUMPBACK","AIRJUMPFWD","MOVEMENT","AIRJUMPNUM",
"AIRJUMPHEIGHT","YACCEL","STANDFRICTION","CROUCHFRICTION","NUMBER","','","lines",
"SECTIONS","DATASECTION","SIZESECTION","VELOCITYSECTION","MOVEMENTSECTION",""
};
#endif

static const short yyr1[] = {     0,
    53,    53,    53,    54,    54,    54,    54,    55,    55,    55,
    55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
    56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
    56,    56,    56,    56,    56,    57,    57,    57,    57,    57,
    57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
    57,    57,    57,    57,    57,    57,    57,    57,    58,    58,
    58,    58,    58,    58
};

static const short yyr2[] = {     0,
     0,     2,     1,     1,     1,     1,     1,     1,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     5,     5,     3,     5,     1,     3,     3,     5,     3,
     5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
     5,     3,     5,     3,     5,     3,     5,     3,     1,     3,
     3,     3,     3,     3
};

static const short yydefact[] = {     0,
     3,     0,     8,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    59,     0,     0,     0,     0,     0,     2,
     4,     5,     6,     7,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
     0,    34,     0,    37,    38,    40,    42,    44,    46,    48,
    50,    52,    54,    56,    58,    60,    61,    62,    63,    64,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    32,    33,    35,    39,    41,    43,    45,
    47,    49,    51,    53,    55,    57,     0,     0
};

static const short yydefgoto[] = {     2,
    50,    51,    52,    53,    54
};

static const short yypact[] = {     0,
-32768,    51,-32768,    -1,    49,    50,    99,   100,   101,   102,
   103,   104,   105,   106,   107,-32768,   108,   109,   110,   111,
   112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
-32768,   122,   123,   124,   125,   126,   127,   128,   129,   130,
   131,   132,   133,-32768,   134,   135,   136,   137,   138,-32768,
-32768,-32768,-32768,-32768,   -48,     3,    91,    92,    93,    94,
    95,    96,    97,    98,   139,   140,   141,   142,   143,   144,
   145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
   155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
   165,   166,   167,   168,   169,   170,   171,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   172,
   173,-32768,   174,-32768,-32768,   175,   176,   177,   178,   179,
   180,   181,   182,   183,   184,-32768,-32768,-32768,-32768,-32768,
   186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
   196,   197,   198,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   185,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768
};


#define	YYLAST		249


static const short yytable[] = {    -1,
     1,    55,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   167,    56,    57,    99,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,   100,   101,   102,   103,   104,   105,   106,   107,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   168,     0,     0,     0,     0,   108,
   109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
   119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
   129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
   139,   140,     0,   141,   142,   143,   144,   145,   146,   147,
   148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
   158,   159,   160,   161,   162,   163,   164,   165,   166
};

static const short yycheck[] = {     0,
     1,     3,    51,     4,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     0,     3,     3,    51,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
    50,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,    51,    51,    51,    51,    51,    51,    51,    51,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,     0,    -1,    -1,    -1,    -1,    51,
    51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
    51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
    51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
    51,    51,    -1,    52,    52,    52,    52,    52,    52,    52,
    52,    52,    52,    52,    52,    52,    51,    51,    51,    51,
    51,    51,    51,    51,    51,    51,    51,    51,    51
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
    printf("Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
	printf("Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
    printf("Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
	printf("Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
	printf("Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
  	  printf("Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
    printf("Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);
	       
      printf ("Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
      {
	      fprintf (stderr, "%s ", yytname[yyrhs[i]]);
          printf ("%s ", yytname[yyrhs[i]]);
	  }
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
      printf (" -> %s\n", yytname[yyr1[yyn]]);
      
    }
#endif


  switch (yyn) {

case 8:
#line 88 "cnsconstprase.txt"
{/**/;
    break;}
case 9:
#line 89 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nLife			=(int)	yyvsp[0].nFloat;;
    break;}
case 10:
#line 90 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nAttack		=(int)	yyvsp[0].nFloat;;
    break;}
case 11:
#line 91 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nDefence		=(int)	yyvsp[0].nFloat;;
    break;}
case 12:
#line 92 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nFallDefenceUp	=(int)	yyvsp[0].nFloat;;
    break;}
case 13:
#line 93 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nLieDownTime		=(int)	yyvsp[0].nFloat;;
    break;}
case 14:
#line 94 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nAirjuggle		=(int)	yyvsp[0].nFloat;;
    break;}
case 15:
#line 95 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nSparkno		=(int)	yyvsp[0].nFloat;;
    break;}
case 16:
#line 96 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nGuardSparkno		=(int)	yyvsp[0].nFloat;;
    break;}
case 17:
#line 97 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.bKoEcho		=(int)	yyvsp[0].nFloat;;
    break;}
case 18:
#line 98 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nVolumen		=(int)	yyvsp[0].nFloat;;
    break;}
case 19:
#line 99 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nIntPersistIndex	=(int)	yyvsp[0].nFloat;;
    break;}
case 20:
#line 100 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerData.nFloatPersistIndex	=(int)	yyvsp[0].nFloat;;
    break;}
case 21:
#line 103 "cnsconstprase.txt"
{/*Check Size Section*/;
    break;}
case 22:
#line 104 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nXscale		=(int)	yyvsp[0].nFloat;;
    break;}
case 23:
#line 105 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nYscale		=(int)	yyvsp[0].nFloat;;
    break;}
case 24:
#line 106 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nGroundBack		=(int)	yyvsp[0].nFloat;;
    break;}
case 25:
#line 107 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nGroundFront		=(int)	yyvsp[0].nFloat;;
    break;}
case 26:
#line 108 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nAirBack		=(int)	yyvsp[0].nFloat;;
    break;}
case 27:
#line 109 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nAirFront		=(int)	yyvsp[0].nFloat;;
    break;}
case 28:
#line 110 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nHeight		=(int)	yyvsp[0].nFloat;;
    break;}
case 29:
#line 111 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nAttackDistance	=(int)	yyvsp[0].nFloat;;
    break;}
case 30:
#line 112 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nProjAttackDist	=(int)	yyvsp[0].nFloat;;
    break;}
case 31:
#line 113 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.bProjDoScale  	=(int)	yyvsp[0].nFloat;;
    break;}
case 32:
#line 114 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nHeadPos.x  		=(int)	yyvsp[-2].nFloat;
							 m_pPlayer->myPlayerConst.PlayerSize.nHeadPos.y  		=(int)	yyvsp[0].nFloat;;
    break;}
case 33:
#line 117 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nMidPos.x  		=(int)	yyvsp[-2].nFloat;
								 m_pPlayer->myPlayerConst.PlayerSize.nMidPos.y  		=(int)	yyvsp[0].nFloat;;
    break;}
case 34:
#line 120 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nShadowOffset  	=(int)	yyvsp[0].nFloat;;
    break;}
case 35:
#line 122 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerSize.nDrawOffset.x  	=(int)	yyvsp[-2].nFloat;
								 m_pPlayer->myPlayerConst.PlayerSize.nDrawOffset.y  	=(int)	yyvsp[0].nFloat;;
    break;}
case 36:
#line 126 "cnsconstprase.txt"
{/*Check Velocity Section*/;
    break;}
case 37:
#line 127 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.nWalkFwd		=	yyvsp[0].nFloat;;
    break;}
case 38:
#line 128 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.nWalkBack		=	yyvsp[0].nFloat;;
    break;}
case 39:
#line 130 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.RunFwd.x		=	yyvsp[-2].nFloat;
							 	m_pPlayer->myPlayerConst.PlayerVelocity.RunFwd.y		=	yyvsp[0].nFloat;;
    break;}
case 40:
#line 132 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.RunFwd.x		=	yyvsp[0].nFloat;;
    break;}
case 41:
#line 134 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.RunBack.x		=	yyvsp[-2].nFloat;
							 	m_pPlayer->myPlayerConst.PlayerVelocity.RunBack.y		=	yyvsp[0].nFloat;;
    break;}
case 42:
#line 136 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.RunBack.x		=	yyvsp[0].nFloat;;
    break;}
case 43:
#line 138 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.JumpNeu.x		=	yyvsp[-2].nFloat;
							 m_pPlayer->myPlayerConst.PlayerVelocity.JumpNeu.y		=	yyvsp[0].nFloat;;
    break;}
case 44:
#line 140 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.JumpNeu.x		=	yyvsp[0].nFloat;;
    break;}
case 45:
#line 142 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.JumpBack.x	=	yyvsp[-2].nFloat;
							 m_pPlayer->myPlayerConst.PlayerVelocity.JumpBack.y	=	yyvsp[0].nFloat;;
    break;}
case 46:
#line 144 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.JumpBack.x	=	yyvsp[0].nFloat;;
    break;}
case 47:
#line 146 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.JumpFwd.x		=	yyvsp[-2].nFloat;
							m_pPlayer->myPlayerConst.PlayerVelocity.JumpFwd.y		=	yyvsp[0].nFloat;;
    break;}
case 48:
#line 148 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.JumpFwd.x		=	yyvsp[0].nFloat;;
    break;}
case 49:
#line 150 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.RunjumpBack.x	=	yyvsp[-2].nFloat;
							 m_pPlayer->myPlayerConst.PlayerVelocity.RunjumpBack.y	=	yyvsp[0].nFloat;;
    break;}
case 50:
#line 152 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.RunjumpBack.x	=	yyvsp[0].nFloat;;
    break;}
case 51:
#line 154 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.RunjumpFwd.x	=	yyvsp[-2].nFloat;
							m_pPlayer-> myPlayerConst.PlayerVelocity.RunjumpFwd.y	=	yyvsp[0].nFloat;;
    break;}
case 52:
#line 156 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.RunjumpFwd.x	=	yyvsp[0].nFloat;;
    break;}
case 53:
#line 158 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpNeu.x	=	yyvsp[-2].nFloat;
							 m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpNeu.y	=	yyvsp[0].nFloat;;
    break;}
case 54:
#line 160 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpNeu.x	=	yyvsp[0].nFloat;;
    break;}
case 55:
#line 162 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpBack.x	=	yyvsp[-2].nFloat;
							 m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpBack.y	=	yyvsp[0].nFloat;;
    break;}
case 56:
#line 164 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpBack.x	=	yyvsp[0].nFloat;;
    break;}
case 57:
#line 166 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpFwd.x	=	yyvsp[-2].nFloat;
								 m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpFwd.y	=	yyvsp[0].nFloat;;
    break;}
case 58:
#line 168 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerVelocity.AirjumpFwd.x	=	yyvsp[0].nFloat;;
    break;}
case 59:
#line 171 "cnsconstprase.txt"
{/*check movement section*/ ;
    break;}
case 60:
#line 172 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerMovement.AirJumpNum	=(int)	yyvsp[0].nFloat;;
    break;}
case 61:
#line 173 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerMovement.nAirJumpHight	=(int)	yyvsp[0].nFloat;;
    break;}
case 62:
#line 174 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerMovement.yaccel		=	yyvsp[0].nFloat;;
    break;}
case 63:
#line 175 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerMovement.StandFriction	= 	yyvsp[0].nFloat;;
    break;}
case 64:
#line 176 "cnsconstprase.txt"
{m_pPlayer->myPlayerConst.PlayerMovement.CrouchFriction	=	yyvsp[0].nFloat;;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      printf ("state stack now");
      while (ssp1 != yyssp)
      {
	      fprintf (stderr, " %d", *++ssp1);
	      printf (" %d", *++ssp1);
	  }   
      fprintf (stderr, "\n");
      printf ("\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
	printf("Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      printf ("Error: state stack now");
      while (ssp1 != yyssp)
      {
	      fprintf (stderr, " %d", *++ssp1);
	      printf (" %d", *++ssp1);	  
   	  }
      fprintf (stderr, "\n");
      printf ("\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
    printf("Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 178 "cnsconstprase.txt"

