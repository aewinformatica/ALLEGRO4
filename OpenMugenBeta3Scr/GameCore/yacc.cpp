
/*  A Bison parser, made from praserrules.txt with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	MOD	258
#define	LOGNOT	259
#define	LOGAND	260
#define	LOGOR	261
#define	LOGXOR	262
#define	NOT	263
#define	AND	264
#define	OR	265
#define	XOR	266
#define	EQUAL	267
#define	NOTEQUAL	268
#define	GREATER	269
#define	LESS	270
#define	LESSEQUAL	271
#define	GREATEREQUAL	272
#define	SQUARE	273
#define	ACOS	274
#define	ABS	275
#define	ANIMELEMNO	276
#define	ANIMELEMTIME	277
#define	ANIMEXIST	278
#define	ASIN	279
#define	ATAN	280
#define	CEIL	281
#define	COS	282
#define	EXP	283
#define	FLOOR	284
#define	GETHITVAR	285
#define	LN	286
#define	NUMEXPLOD	287
#define	NUMHELPER	288
#define	NUMPROJID	289
#define	NUMTARGET	290
#define	PLAYERIDEXIST	291
#define	PROJCANCLETIME	292
#define	SELFANIMEXIST	293
#define	SIN	294
#define	SYSFVAR	295
#define	SYSVAR	296
#define	TAN	297
#define	VAR	298
#define	PROJCONTACTTIME	299
#define	PROJGUARDEDTIME	300
#define	IFELSE	301
#define	LOG	302
#define	CONST	303
#define	UMINUS	304
#define	UPLUS	305
#define	ID	306
#define	STRING	307
#define	NEGONE	308
#define	DATA	309
#define	ATTACK	310
#define	DEFENCE	311
#define	FALLDEFUP	312
#define	LIEDOWTIME	313
#define	AIRJUGGLE	314
#define	SPARKNO	315
#define	GUARDSPARKNO	316
#define	KOECHO	317
#define	VOLUME	318
#define	INTEGERINDEX	319
#define	FLOATINDEX	320
#define	SIZE	321
#define	XSCALE	322
#define	YSCALE	323
#define	GROUNDBACK	324
#define	GROUNDFRONT	325
#define	AIRBACK	326
#define	AIRFRONT	327
#define	HEIGHT	328
#define	ATTACKDIST	329
#define	PROJATTDIST	330
#define	PROJDOSCALE	331
#define	HEADPOSX	332
#define	HEADPOSY	333
#define	MIDPOSX	334
#define	MIDPOSY	335
#define	SHADOWOFFSET	336
#define	DRAWOFFSETX	337
#define	DRAWOFFSETY	338
#define	VELOCITY	339
#define	WALKFWD	340
#define	WALKBACK	341
#define	RUNFWDX	342
#define	RUNFWDY	343
#define	RUNBACKX	344
#define	RUNBACKY	345
#define	JUMPNEU	346
#define	JUMPBACK	347
#define	JUMPFWD	348
#define	RUNJUMPBACK	349
#define	RUNJUMPFWD	350
#define	AIRJUMPNEU	351
#define	AIRJUMPBACK	352
#define	AIRJUMPFWD	353
#define	MOVEMENT	354
#define	AIRJUMPNUM	355
#define	AIRJUMPHEIGHT	356
#define	YACCEL	357
#define	STANDFRICTION	358
#define	CROUCHFRICTION	359
#define	NUMBER	360
#define	TRIGGER	361
#define	TRIGGERALL	362
#define	STATEDEF	363
#define	TYPE	364
#define	MOVETYPE	365
#define	PHYSICS	366
#define	ANIM	367
#define	JUGGLE	368
#define	FACEP2	369
#define	HITDEFPERSIST	370
#define	MOVEHITPERSIST	371
#define	HITCOUNTPERSIST	372
#define	STATE	373
#define	ALIVE	374
#define	ANIMELEM	375
#define	ANIMTIME	376
#define	AUTHORNAME	377
#define	BACKEEDGEBODYDIST	378
#define	BACKEDGEDIST	379
#define	CANRECOVER	380
#define	COMMAND	381
#define	CTRL	382
#define	DRAWGAME	383
#define	FACING	384
#define	FRONTEDGEBODYDIST	385
#define	FRONTEDGEDIST	386
#define	FVAR	387
#define	GAMETIME	388
#define	HITCOUNT	389
#define	HITDEFATTR	390
#define	HITFALL	391
#define	HITOVER	392
#define	HITPAUSETIME	393
#define	HITSHAKEOVER	394
#define	HITVELX	395
#define	HITVELY	396
#define	PLID	397
#define	INGUARDDIST	398
#define	ISHELPER	399
#define	ISHOMETEAM	400
#define	LIFE	401
#define	LIFEMAX	402
#define	LOSE	403
#define	LOSEKO	404
#define	LOSETIME	405
#define	MATCHNO	406
#define	MATCHOVER	407
#define	MOVECONTACT	408
#define	MOVEGUARDED	409
#define	MOVEHIT	410
#define	MOVEREVERSED	411
#define	NAME	412
#define	NUMENEMY	413
#define	NUMPARTNER	414
#define	NUMPROJ	415
#define	P1NAME	416
#define	P2BODYDISTX	417
#define	P2BODYDISTY	418
#define	P2DISTX	419
#define	P2DISTY	420
#define	P2LIFE	421
#define	P2MOVETYPE	422
#define	P2NAME	423
#define	P2STATENO	424
#define	P2STATETYP	425
#define	P3NAME	426
#define	P4NAME	427
#define	PALNO	428
#define	PARENTDISTX	429
#define	PARENTDISTY	430
#define	PI	431
#define	PREVSTATENO	432
#define	POSX	433
#define	POSY	434
#define	POWER	435
#define	POWERMAX	436
#define	PROJCONTACT	437
#define	PROJGUARDED	438
#define	PROJHIT	439
#define	PROJHITTIME	440
#define	RANDOM	441
#define	ROOTDISTX	442
#define	ROOTDISTY	443
#define	ROUNDNO	444
#define	ROUNDSEXISTED	445
#define	ROUNDSTATE	446
#define	SCREENPOSX	447
#define	SCREENPOSY	448
#define	STATENO	449
#define	STATETYPE	450
#define	TEAMMODE	451
#define	TEAMSIDE	452
#define	TICKSPERSECOND	453
#define	TIME	454
#define	TIMEMODE	455
#define	UNIQHITCOUNT	456
#define	VELX	457
#define	VELY	458
#define	WIN	459
#define	WINKO	460
#define	WINTIME	461
#define	WINPERFECT	462
#define	TNULL	463
#define	AFTERIMAGE	464
#define	AFTERIMAGETIME	465
#define	ALLPALFX	466
#define	ANGLEADD	467
#define	ANGLEDRAW	468
#define	ANGLEMUL	469
#define	ANGLESET	470
#define	APPENDTOCLIPBOARD	471
#define	ASSERTSPECIAL	472
#define	ATTACKMULSET	473
#define	BGPALFX	474
#define	BINDTOPARENT	475
#define	BINDTOROOT	476
#define	BINDTOTARGET	477
#define	CHANGEANIME	478
#define	CHANGEANIME2	479
#define	CHANGESTATE	480
#define	CLEARCLIPBOARD	481
#define	CTRLSET	482
#define	DEFENCEMULSET	483
#define	DESTROYSELF	484
#define	DISPLAYTOCLIPBORAD	485
#define	ENVSHAKE	486
#define	EXPLOD	487
#define	EXPLODBINDTIME	488
#define	FALLENVSHAKE	489
#define	FORCEFEDDBACK	490
#define	GAMEMAKEANIM	491
#define	GRAVITY	492
#define	HELPER	493
#define	HITADD	494
#define	HITBY	495
#define	HITDEF	496
#define	HITFALLDAMAGE	497
#define	HITFALLSET	498
#define	HITFALLVEL	499
#define	HITOVERRIDE	500
#define	HITVELSET	501
#define	LIFEADD	502
#define	LIFESET	503
#define	MAKEDUST	504
#define	MODIFYEXPLOD	505
#define	MOVEHITRESET	506
#define	NOTHITBY	507
#define	NUL	508
#define	OFFSET	509
#define	PALFX	510
#define	PARENTVARADD	511
#define	PARENTVARSET	512
#define	PAUSE	513
#define	PLAYERPUSH	514
#define	PLAYSND	515
#define	POSADD	516
#define	POSFREEZ	517
#define	POSSET	518
#define	POWERADD	519
#define	POWERSET	520
#define	PROJECTILE	521
#define	REMOVEEXPLOD	522
#define	REVERSALDEF	523
#define	SCREENBOUND	524
#define	SELFSTATE	525
#define	SNDPAN	526
#define	SPRPRIORITY	527
#define	STATETYPESET	528
#define	STOPSND	529
#define	SUPERPAUSE	530
#define	TARGETBIND	531
#define	TARGETDROP	532
#define	TARGETFACING	533
#define	TARGETLIFEADD	534
#define	TARGETPOWERADD	535
#define	TARGETSTATE	536
#define	TARGETVELADD	537
#define	TARGETVELSET	538
#define	TURN	539
#define	VARADD	540
#define	VARRANDOM	541
#define	VARRANGESET	542
#define	VARSET	543
#define	VELADD	544
#define	VELMUL	545
#define	VELSET	546
#define	WIDTH	547
#define	LENGTH	548
#define	PALCOLOR	549
#define	PALINVERTALL	550
#define	PALBRIGHT	551
#define	PALCONTRAST	552
#define	PALPOSTBRIGHT	553
#define	PALADD	554
#define	PALMUL	555
#define	TIMEGAP	556
#define	FRAMEGAP	557
#define	TRANS	558
#define	ACBEGIN	559
#define	ACTION	560
#define	LOOPSTART	561
#define	CLSN2DEFAULT	562
#define	CLSN1DEFAULT	563
#define	CLSN2	564
#define	CLSN1	565
#define	HORIZONTALFLIP	566
#define	VERTIKALFLIP	567
#define	HVFLIP	568
#define	ALPHA	569
#define	INFO	570
#define	DISPLAYNAME	571
#define	VERSIONSDATA	572
#define	MUGENVERSION	573
#define	AUTHOR	574
#define	FILES	575
#define	CMD	576
#define	CNS	577
#define	ST	578
#define	STCOMMON	579
#define	SPRITE	580
#define	SOUND	581
#define	PAL	582
#define	PALDEF	583
#define	ARCADE	584
#define	INTROSTORYBOARD	585
#define	ENDINGSTORYBOARD	586
#define	VALUE	587
#define	XVALUE	588
#define	YVALUE	589

#line 1 "praserrules.txt"

#include <malloc.h>  // _alloca is used by the parser
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "Structs.h"
#include "Player.h"
//#include "StateManager.h"


CPlayer *m_pPlayer;
CStateManager *myStateManager;

void yyerror(char *msg);
void SetPlayer(CPlayer *p);
void SetStateManager(CStateManager *p);


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



#define	YYFINAL		460
#define	YYFLAG		-32768
#define	YYNTBASE	344

#define YYTRANSLATE(x) ((unsigned)(x) <= 589 ? yytranslate[x] : 352)

static const short yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,   342,
   343,    21,    19,     3,    20,     2,    23,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
   340,     2,   341,     2,     2,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     2,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    22,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
   111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
   121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
   131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
   141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
   151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
   161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
   171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
   181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
   191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
   201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
   211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
   231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
   241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
   251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
   261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
   271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
   281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
   291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
   301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
   311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
   321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
   331,   332,   333,   334,   335,   336,   337,   338,   339
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     6,    11,    15,    19,    23,    27,    31,
    35,    39,    43,    47,    51,    55,    59,    65,    67,    74,
    81,    89,    96,    98,   100,   102,   106,   110,   114,   118,
   122,   126,   130,   134,   138,   142,   146,   150,   154,   158,
   162,   166,   170,   174,   178,   182,   186,   190,   194,   198,
   202,   206,   210,   214,   218,   222,   226,   230,   234,   238,
   242,   246,   250,   254,   258,   262,   266,   270,   274,   278,
   282,   286,   290,   294,   298,   302,   306,   310,   314,   318,
   322,   326,   330,   334,   338,   342,   346,   350,   354,   358,
   362,   366,   370,   374,   378,   382,   386,   390,   394,   398,
   402,   406,   410,   414,   418,   422,   426,   430,   434,   438,
   442,   446,   450,   454,   456,   458,   461,   464,   466,   468,
   470,   473,   476,   479,   481,   484,   487,   489,   491,   493,
   495,   498,   500,   505,   508,   510,   512,   515,   517,   520,
   522,   524,   526,   528,   531,   533,   535,   537,   539,   541,
   543,   545,   547,   549,   558,   560,   562,   564,   566,   568,
   571,   578,   580,   582,   584,   586,   588,   590,   592,   594,
   596,   598,   600,   602,   605,   607,   610,   612,   614,   616,
   619,   622,   624,   626,   628,   630,   632,   634,   636,   638,
   640,   642,   644,   646,   648,   650,   652,   654,   656,   659,
   661,   663,   665,   667,   669,   672,   674,   677,   679,   682,
   684,   686,   688,   690,   692,   694,   696,   698,   700,   702,
   705,   708,   710,   712,   715,   718,   721,   723,   725,   727,
   729,   731,   733,   736,   738,   740,   742,   744,   746,   748,
   751,   755,   759,   763,   766,   770,   774,   778,   782,   786,
   790,   794,   798,   802,   810,   818,   826,   834,   838,   846,
   854,   862,   870,   874,   878,   882,   886,   890,   894,   899,
   904,   909,   914,   919,   924,   928,   931,   933,   935,   937,
   939,   941,   943,   945,   947,   949,   951,   953,   955,   957,
   959,   961,   963,   965,   967,   969,   971,   973,   975,   977,
   979,   981,   983,   985,   987,   989,   991,   993,   995,   997,
   999,  1001,  1003,  1005,  1007,  1009,  1011,  1013,  1015,  1017,
  1019,  1021,  1023,  1027,  1031
};

static const short yyrhs[] = {    -1,
   344,   345,     0,     1,     0,   340,   113,   349,   341,     0,
   114,    13,   349,     0,   115,    13,   349,     0,   116,    13,
   349,     0,   117,    13,   349,     0,   132,    13,   349,     0,
   269,    13,   349,     0,   118,    13,   349,     0,   120,    13,
   349,     0,   121,    13,   349,     0,   122,    13,   349,     0,
   277,    13,   349,     0,   119,    13,   349,     0,   296,    13,
   349,     3,   349,     0,   346,     0,   340,   123,   349,     3,
   349,   341,     0,   340,   123,   349,     3,    56,   341,     0,
   340,   123,   349,     3,    56,   349,   341,     0,   340,   123,
   349,     3,     1,   341,     0,   347,     0,   348,     0,   351,
     0,   114,    13,   213,     0,   114,    13,   214,     0,   114,
    13,   215,     0,   114,    13,   216,     0,   114,    13,   217,
     0,   114,    13,   218,     0,   114,    13,   219,     0,   114,
    13,   220,     0,   114,    13,   221,     0,   114,    13,   222,
     0,   114,    13,    79,     0,   114,    13,   223,     0,   114,
    13,   224,     0,   114,    13,   225,     0,   114,    13,   226,
     0,   114,    13,   227,     0,   114,    13,   228,     0,   114,
    13,   229,     0,   114,    13,   230,     0,   114,    13,   231,
     0,   114,    13,   232,     0,   114,    13,   233,     0,   114,
    13,   234,     0,   114,    13,   235,     0,   114,    13,   236,
     0,   114,    13,   237,     0,   114,    13,   238,     0,   114,
    13,   239,     0,   114,    13,   240,     0,   114,    13,   241,
     0,   114,    13,   242,     0,   114,    13,   243,     0,   114,
    13,   244,     0,   114,    13,   245,     0,   114,    13,   246,
     0,   114,    13,   247,     0,   114,    13,   248,     0,   114,
    13,   249,     0,   114,    13,   250,     0,   114,    13,   251,
     0,   114,    13,   252,     0,   114,    13,   253,     0,   114,
    13,   254,     0,   114,    13,   255,     0,   114,    13,   256,
     0,   114,    13,   257,     0,   114,    13,   258,     0,   114,
    13,   259,     0,   114,    13,   260,     0,   114,    13,   261,
     0,   114,    13,   262,     0,   114,    13,   263,     0,   114,
    13,   264,     0,   114,    13,   265,     0,   114,    13,   266,
     0,   114,    13,   267,     0,   114,    13,   268,     0,   114,
    13,   269,     0,   114,    13,   270,     0,   114,    13,   271,
     0,   114,    13,   272,     0,   114,    13,   273,     0,   114,
    13,   274,     0,   114,    13,   275,     0,   114,    13,   276,
     0,   114,    13,   277,     0,   114,    13,   278,     0,   114,
    13,   279,     0,   114,    13,   280,     0,   114,    13,   281,
     0,   114,    13,   282,     0,   114,    13,   283,     0,   114,
    13,   284,     0,   114,    13,   285,     0,   114,    13,   286,
     0,   114,    13,   287,     0,   114,    13,   288,     0,   114,
    13,   289,     0,   114,    13,   290,     0,   114,    13,   291,
     0,   114,    13,   292,     0,   114,    13,   293,     0,   114,
    13,   294,     0,   114,    13,   295,     0,   114,    13,   296,
     0,   114,    13,   297,     0,   111,    13,   349,     0,   112,
    13,   349,     0,   110,     0,    57,     0,    25,   349,     0,
    24,   349,     0,   124,     0,   117,     0,   125,     0,    26,
   349,     0,    27,   349,     0,    28,   349,     0,   126,     0,
    29,   349,     0,    30,   349,     0,   127,     0,   128,     0,
   129,     0,   130,     0,    31,   349,     0,   131,     0,    53,
   342,   350,   343,     0,    32,   349,     0,   132,     0,   133,
     0,    33,   349,     0,   134,     0,    34,   349,     0,   135,
     0,   136,     0,   137,     0,   138,     0,    35,   349,     0,
   139,     0,   140,     0,   141,     0,   142,     0,   143,     0,
   144,     0,   145,     0,   146,     0,   147,     0,    51,   342,
   349,     3,   349,     3,   349,   343,     0,   148,     0,   149,
     0,   150,     0,   151,     0,   152,     0,    36,   349,     0,
    52,   342,   349,     3,   349,   343,     0,   153,     0,   154,
     0,   155,     0,   156,     0,   157,     0,   158,     0,   159,
     0,   160,     0,   115,     0,   161,     0,   162,     0,   163,
     0,    37,   349,     0,    37,     0,    38,   349,     0,    38,
     0,   164,     0,   165,     0,    39,   349,     0,    40,   349,
     0,    40,     0,   166,     0,   167,     0,   168,     0,   169,
     0,   170,     0,   171,     0,   172,     0,   173,     0,   174,
     0,   175,     0,   176,     0,   177,     0,   178,     0,   179,
     0,   180,     0,   181,     0,    41,   349,     0,   182,     0,
   183,     0,   184,     0,   185,     0,   186,     0,    42,   349,
     0,   187,     0,    49,   349,     0,   188,     0,    50,   349,
     0,   189,     0,   190,     0,   191,     0,   192,     0,   193,
     0,   194,     0,   195,     0,   196,     0,   197,     0,   198,
     0,    43,   349,     0,    44,   349,     0,   199,     0,   200,
     0,    45,   349,     0,    46,   349,     0,    47,   349,     0,
   201,     0,   202,     0,   203,     0,   204,     0,   205,     0,
   206,     0,    48,   349,     0,   207,     0,   208,     0,   209,
     0,   210,     0,   211,     0,   212,     0,     9,   349,     0,
   349,    10,   349,     0,   349,    11,   349,     0,   349,    12,
   349,     0,     5,   349,     0,   349,     6,   349,     0,   349,
     7,   349,     0,   349,     8,   349,     0,   349,    19,   349,
     0,   349,    20,   349,     0,   349,    21,   349,     0,   349,
    23,   349,     0,   349,    22,   349,     0,   349,    13,   349,
     0,   349,    13,   340,   349,     3,   349,   341,     0,   349,
    13,   340,   349,     3,   349,   343,     0,   349,    13,   342,
   349,     3,   349,   341,     0,   349,    13,   342,   349,     3,
   349,   343,     0,   349,    14,   349,     0,   349,    14,   340,
   349,     3,   349,   341,     0,   349,    14,   340,   349,     3,
   349,   343,     0,   349,    14,   342,   349,     3,   349,   341,
     0,   349,    14,   342,   349,     3,   349,   343,     0,   349,
    15,   349,     0,   349,    16,   349,     0,   349,    17,   349,
     0,   349,    18,   349,     0,   342,   349,   343,     0,   342,
   349,     1,     0,   349,     3,    13,   349,     0,   349,     3,
    14,   349,     0,   349,     3,    15,   349,     0,   349,     3,
    16,   349,     0,   349,     3,    17,   349,     0,   349,     3,
    18,   349,     0,   349,     4,   349,     0,    20,   349,     0,
   151,     0,    60,     0,    61,     0,    62,     0,    63,     0,
    64,     0,    65,     0,    66,     0,    67,     0,    69,     0,
    70,     0,    72,     0,    73,     0,    74,     0,    75,     0,
    76,     0,    77,     0,    79,     0,    80,     0,    81,     0,
    82,     0,    83,     0,    84,     0,    85,     0,    86,     0,
    87,     0,    88,     0,    90,     0,    91,     0,    92,     0,
    93,     0,    94,     0,    95,     0,    96,     0,    97,     0,
    98,     0,    99,     0,   100,     0,   101,     0,   102,     0,
   103,     0,   105,     0,   106,     0,   107,     0,   108,     0,
   109,     0,   337,    13,   349,     0,   338,    13,   349,     0,
   339,    13,   349,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   383,   384,   385,   389,   390,   391,   392,   393,   394,   395,
   396,   397,   398,   399,   400,   401,   402,   404,   407,   408,
   409,   410,   411,   412,   413,   417,   418,   419,   420,   421,
   422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
   432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
   442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
   452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
   462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
   472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
   482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
   492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
   502,   506,   507,   513,   514,   517,   518,   519,   520,   522,
   524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
   534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
   544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
   555,   558,   560,   562,   564,   565,   566,   567,   568,   569,
   570,   572,   575,   578,   581,   582,   583,   584,   585,   586,
   587,   588,   589,   591,   594,   597,   600,   602,   603,   604,
   606,   608,   611,   613,   616,   619,   622,   624,   626,   627,
   628,   629,   630,   631,   632,   634,   636,   639,   640,   641,
   643,   645,   647,   648,   649,   650,   651,   652,   653,   654,
   655,   656,   658,   661,   664,   665,   666,   668,   670,   673,
   674,   675,   676,   677,   678,   679,   680,   681,   682,   683,
   684,   685,   686,   688,   691,   694,   695,   696,   697,   699,
   700,   701,   702,   704,   705,   706,   707,   711,   712,   713,
   714,   716,   718,   719,   720,   721,   722,   724,   725,   726,
   727,   728,   730,   731,   732,   733,   735,   737,   740,   747,
   754,   761,   768,   775,   782,   783,   787,   788,   789,   790,
   791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
   801,   802,   803,   804,   805,   806,   807,   808,   809,   810,
   811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
   821,   822,   823,   824,   825,   826,   827,   828,   829,   830,
   831,   832,   835,   836,   837
};

static const char * const yytname[] = {   "$","error","$undefined.","','","MOD",
"LOGNOT","LOGAND","LOGOR","LOGXOR","NOT","AND","OR","XOR","EQUAL","NOTEQUAL",
"GREATER","LESS","LESSEQUAL","GREATEREQUAL","'+'","'-'","'*'","SQUARE","'/'",
"ACOS","ABS","ANIMELEMNO","ANIMELEMTIME","ANIMEXIST","ASIN","ATAN","CEIL","COS",
"EXP","FLOOR","GETHITVAR","LN","NUMEXPLOD","NUMHELPER","NUMPROJID","NUMTARGET",
"PLAYERIDEXIST","PROJCANCLETIME","SELFANIMEXIST","SIN","SYSFVAR","SYSVAR","TAN",
"VAR","PROJCONTACTTIME","PROJGUARDEDTIME","IFELSE","LOG","CONST","UMINUS","UPLUS",
"ID","STRING","NEGONE","DATA","ATTACK","DEFENCE","FALLDEFUP","LIEDOWTIME","AIRJUGGLE",
"SPARKNO","GUARDSPARKNO","KOECHO","VOLUME","INTEGERINDEX","FLOATINDEX","SIZE",
"XSCALE","YSCALE","GROUNDBACK","GROUNDFRONT","AIRBACK","AIRFRONT","HEIGHT","ATTACKDIST",
"PROJATTDIST","PROJDOSCALE","HEADPOSX","HEADPOSY","MIDPOSX","MIDPOSY","SHADOWOFFSET",
"DRAWOFFSETX","DRAWOFFSETY","VELOCITY","WALKFWD","WALKBACK","RUNFWDX","RUNFWDY",
"RUNBACKX","RUNBACKY","JUMPNEU","JUMPBACK","JUMPFWD","RUNJUMPBACK","RUNJUMPFWD",
"AIRJUMPNEU","AIRJUMPBACK","AIRJUMPFWD","MOVEMENT","AIRJUMPNUM","AIRJUMPHEIGHT",
"YACCEL","STANDFRICTION","CROUCHFRICTION","NUMBER","TRIGGER","TRIGGERALL","STATEDEF",
"TYPE","MOVETYPE","PHYSICS","ANIM","JUGGLE","FACEP2","HITDEFPERSIST","MOVEHITPERSIST",
"HITCOUNTPERSIST","STATE","ALIVE","ANIMELEM","ANIMTIME","AUTHORNAME","BACKEEDGEBODYDIST",
"BACKEDGEDIST","CANRECOVER","COMMAND","CTRL","DRAWGAME","FACING","FRONTEDGEBODYDIST",
"FRONTEDGEDIST","FVAR","GAMETIME","HITCOUNT","HITDEFATTR","HITFALL","HITOVER",
"HITPAUSETIME","HITSHAKEOVER","HITVELX","HITVELY","PLID","INGUARDDIST","ISHELPER",
"ISHOMETEAM","LIFE","LIFEMAX","LOSE","LOSEKO","LOSETIME","MATCHNO","MATCHOVER",
"MOVECONTACT","MOVEGUARDED","MOVEHIT","MOVEREVERSED","NAME","NUMENEMY","NUMPARTNER",
"NUMPROJ","P1NAME","P2BODYDISTX","P2BODYDISTY","P2DISTX","P2DISTY","P2LIFE",
"P2MOVETYPE","P2NAME","P2STATENO","P2STATETYP","P3NAME","P4NAME","PALNO","PARENTDISTX",
"PARENTDISTY","PI","PREVSTATENO","POSX","POSY","POWER","POWERMAX","PROJCONTACT",
"PROJGUARDED","PROJHIT","PROJHITTIME","RANDOM","ROOTDISTX","ROOTDISTY","ROUNDNO",
"ROUNDSEXISTED","ROUNDSTATE","SCREENPOSX","SCREENPOSY","STATENO","STATETYPE",
"TEAMMODE","TEAMSIDE","TICKSPERSECOND","TIME","TIMEMODE","UNIQHITCOUNT","VELX",
"VELY","WIN","WINKO","WINTIME","WINPERFECT","TNULL","AFTERIMAGE","AFTERIMAGETIME",
"ALLPALFX","ANGLEADD","ANGLEDRAW","ANGLEMUL","ANGLESET","APPENDTOCLIPBOARD",
"ASSERTSPECIAL","ATTACKMULSET","BGPALFX","BINDTOPARENT","BINDTOROOT","BINDTOTARGET",
"CHANGEANIME","CHANGEANIME2","CHANGESTATE","CLEARCLIPBOARD","CTRLSET","DEFENCEMULSET",
"DESTROYSELF","DISPLAYTOCLIPBORAD","ENVSHAKE","EXPLOD","EXPLODBINDTIME","FALLENVSHAKE",
"FORCEFEDDBACK","GAMEMAKEANIM","GRAVITY","HELPER","HITADD","HITBY","HITDEF",
"HITFALLDAMAGE","HITFALLSET","HITFALLVEL","HITOVERRIDE","HITVELSET","LIFEADD",
"LIFESET","MAKEDUST","MODIFYEXPLOD","MOVEHITRESET","NOTHITBY","NUL","OFFSET",
"PALFX","PARENTVARADD","PARENTVARSET","PAUSE","PLAYERPUSH","PLAYSND","POSADD",
"POSFREEZ","POSSET","POWERADD","POWERSET","PROJECTILE","REMOVEEXPLOD","REVERSALDEF",
"SCREENBOUND","SELFSTATE","SNDPAN","SPRPRIORITY","STATETYPESET","STOPSND","SUPERPAUSE",
"TARGETBIND","TARGETDROP","TARGETFACING","TARGETLIFEADD","TARGETPOWERADD","TARGETSTATE",
"TARGETVELADD","TARGETVELSET","TURN","VARADD","VARRANDOM","VARRANGESET","VARSET",
"VELADD","VELMUL","VELSET","WIDTH","LENGTH","PALCOLOR","PALINVERTALL","PALBRIGHT",
"PALCONTRAST","PALPOSTBRIGHT","PALADD","PALMUL","TIMEGAP","FRAMEGAP","TRANS",
"ACBEGIN","ACTION","LOOPSTART","CLSN2DEFAULT","CLSN1DEFAULT","CLSN2","CLSN1",
"HORIZONTALFLIP","VERTIKALFLIP","HVFLIP","ALPHA","INFO","DISPLAYNAME","VERSIONSDATA",
"MUGENVERSION","AUTHOR","FILES","CMD","CNS","ST","STCOMMON","SPRITE","SOUND",
"PAL","PALDEF","ARCADE","INTROSTORYBOARD","ENDINGSTORYBOARD","VALUE","XVALUE",
"YVALUE","'['","']'","'('","')'","lines","STATEDEFSECTION","STATESECTION","TYPESECTION",
"TRIGGERSECTION","expression","CONSTPARAM","PARAMSECTION",""
};
#endif

static const short yyr1[] = {     0,
   344,   344,   344,   345,   345,   345,   345,   345,   345,   345,
   345,   345,   345,   345,   345,   345,   345,   345,   346,   346,
   346,   346,   346,   346,   346,   347,   347,   347,   347,   347,
   347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
   347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
   347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
   347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
   347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
   347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
   347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
   347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
   347,   348,   348,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
   349,   349,   349,   349,   349,   349,   350,   350,   350,   350,
   350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
   350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
   350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
   350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
   350,   350,   351,   351,   351
};

static const short yyr2[] = {     0,
     0,     2,     1,     4,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     5,     1,     6,     6,
     7,     6,     1,     1,     1,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     1,     1,     2,     2,     1,     1,     1,
     2,     2,     2,     1,     2,     2,     1,     1,     1,     1,
     2,     1,     4,     2,     1,     1,     2,     1,     2,     1,
     1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     8,     1,     1,     1,     1,     1,     2,
     6,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     2,     1,     2,     1,     1,     1,     2,
     2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
     1,     1,     1,     1,     2,     1,     2,     1,     2,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
     2,     1,     1,     2,     2,     2,     1,     1,     1,     1,
     1,     1,     2,     1,     1,     1,     1,     1,     1,     2,
     3,     3,     3,     2,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     7,     7,     7,     7,     3,     7,     7,
     7,     7,     3,     3,     3,     3,     3,     3,     4,     4,
     4,     4,     4,     4,     3,     2,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     3,     3,     3
};

static const short yydefact[] = {     0,
     3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     2,    18,    23,    24,    25,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   175,   177,     0,   182,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
   114,   170,   119,   118,   120,   124,   127,   128,   129,   130,
   132,   135,   136,   138,   140,   141,   142,   143,   145,   146,
   147,   148,   149,   150,   151,   152,   153,   155,   156,   157,
   158,   159,   162,   163,   164,   165,   166,   167,   168,   169,
   171,   172,   173,   178,   179,   183,   184,   185,   186,   187,
   188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
   198,   200,   201,   202,   203,   204,   206,   208,   210,   211,
   212,   213,   214,   215,   216,   217,   218,   219,   222,   223,
   227,   228,   229,   230,   231,   232,   234,   235,   236,   237,
   238,   239,     0,   112,   113,    36,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    37,    38,    39,    40,
    41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
   111,     5,     6,     7,     8,    11,    16,    12,    13,    14,
     9,    10,    15,     0,   323,   324,   325,     0,     0,   244,
   240,   276,   117,   116,   121,   122,   123,   125,   126,   131,
   134,   137,   139,   144,   160,   174,   176,   180,   181,   199,
   205,   220,   221,   224,   225,   226,   233,   207,   209,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     4,     0,     0,     0,   278,   279,   280,
   281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
   291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
   301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
   311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
   321,   322,   277,     0,   268,   267,     0,     0,     0,     0,
     0,     0,   275,   245,   246,   247,   241,   242,   243,     0,
     0,   253,     0,     0,   258,   263,   264,   265,   266,   248,
   249,   250,   252,   251,    17,     0,     0,     0,     0,     0,
   133,   269,   270,   271,   272,   273,   274,     0,     0,     0,
     0,    22,    20,     0,    19,     0,     0,     0,     0,     0,
     0,    21,     0,   161,     0,     0,     0,     0,     0,   254,
   255,   256,   257,   259,   260,   261,   262,   154,     0,     0
};

static const short yydefgoto[] = {     2,
    22,    23,    24,    25,   174,   384,    26
};

static const short yypact[] = {   365,
-32768,   402,    37,    39,    42,    62,    65,    69,    85,    88,
    92,   109,   113,   131,   133,   134,   135,   136,   139,   143,
   -64,-32768,-32768,-32768,-32768,-32768,  2185,  2185,   990,  2185,
  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,
  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,
  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,  2185,
  2185,  2185,  2404,  2404,  2185,  2404,  2185,  2185,  2185,  2185,
  2185,  2185,  2185,  2185,  2185,  2185,  -169,  -165,  -148,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,  2185,   117,   117,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   117,   117,   117,   117,   117,   117,   117,   117,   117,
   117,   117,   117,   791,   117,   117,   117,   703,   812,   252,
   252,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  2185,
  2185,   155,    50,   263,  2185,  2185,  2185,  2185,  2185,  2185,
  2185,  1528,  1747,  2185,  2185,  2185,  2185,  2185,  2185,  2185,
  2185,  2185,  1309,-32768,   416,   833,   854,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  -145,-32768,-32768,  2185,  2185,  2185,  2185,
  2185,  2185,   252,   252,   252,   252,   252,   252,   252,  2185,
  2185,   331,  2185,  2185,   331,   331,   331,   331,   331,   120,
   120,-32768,-32768,-32768,   117,  -128,  1966,   749,  1309,  1309,
-32768,   331,   331,   331,   331,   331,   331,   875,    73,   896,
    96,-32768,-32768,   770,-32768,   917,   326,  1309,  1309,  1309,
  1309,-32768,  1309,-32768,   147,   168,   189,   305,   670,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   214,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,   -28,-32768,-32768
};


#define	YYLAST		2746


static const short yytable[] = {   175,
   262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
   272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
   282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
   292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
   302,   303,   304,   305,   306,   307,   308,   309,    45,    27,
   385,    28,   314,   315,    29,   316,   317,   318,    46,   319,
   320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
   330,   331,   332,   385,    30,   439,   315,    31,   316,   317,
   318,    32,   319,   320,   321,   322,   323,   324,   325,   326,
   327,   328,   329,   330,   331,   332,   385,    33,   441,   315,
    34,   316,   317,   318,    35,   319,   320,   321,   322,   323,
   324,   325,   326,   327,   328,   329,   330,   331,   332,   314,
   315,    36,   316,   317,   318,    37,   319,   320,   321,   322,
   323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
   330,   331,   332,    38,   313,    39,    40,    41,    42,   314,
   315,    43,   316,   317,   318,    44,   319,   320,   321,   322,
   323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
   314,   315,   310,   316,   317,   318,   311,   319,   320,   321,
   322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
   332,   314,   315,   312,   316,   317,   318,   421,   319,   320,
   321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
   331,   332,   432,   460,   338,   339,   340,   341,   342,   343,
   344,   345,     0,   346,   347,     0,   348,   349,   350,   351,
   352,   353,     0,   354,   355,   356,   357,   358,   359,   360,
   361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
   370,   371,   372,   373,   374,   375,   376,   377,     0,   378,
   379,   380,   381,   382,   322,   323,   324,   325,   326,   327,
   328,   329,   330,   331,   332,   387,   388,   389,   390,   391,
   392,   336,   337,     0,     0,     0,   393,   394,   395,   396,
   397,   398,   399,   402,   405,   406,   407,   408,   409,   410,
   411,   412,   413,   414,   415,   383,   418,   314,   315,     0,
   316,   317,   318,     0,   319,   320,   321,   322,   323,   324,
   325,   326,   327,   328,   329,   330,   331,   332,   314,   315,
     0,   316,   317,   318,     0,   319,   320,   321,   322,   323,
   324,   325,   326,   327,   328,   329,   330,   331,   332,   328,
   329,   330,   331,   332,     0,     0,     0,     0,   422,   423,
   424,   425,   426,   427,    -1,     1,     0,     0,     0,     0,
     0,   428,   429,     0,   430,   431,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
   436,   437,   386,     0,     0,     0,     0,     0,     0,     0,
     0,   459,     0,     0,     0,     0,     0,     0,     0,   445,
   446,   447,   448,     0,   449,   386,   416,     0,     0,     0,
    47,     0,     0,     0,    48,     0,     0,     0,   387,   388,
   389,   390,   391,   392,     0,    49,     0,     0,   386,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,     0,
     0,   417,    80,     0,     0,    -1,    -1,     0,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   450,     0,   451,
     0,     0,     0,     0,     0,     0,    -1,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   452,     0,
   453,     0,     3,     4,     0,     5,     6,     7,     8,     9,
    10,    11,    12,    13,     0,    81,     0,     0,     0,   454,
    82,   455,    83,    14,     0,     0,     0,     0,     0,    84,
    85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
   105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
   115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
   125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
   135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
   145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
   155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
   165,   166,   167,   168,   169,   170,   171,   172,     0,     0,
     0,     0,     0,    -1,     0,     0,     0,     0,     0,     0,
     0,    -1,     0,     0,     0,   456,     0,   457,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -1,     0,     0,     0,     0,     0,     0,     0,   444,     0,
    15,     0,   314,   315,     0,   316,   317,   318,    16,   319,
   320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
   330,   331,   332,     0,     0,     0,     0,    17,     0,     0,
     0,    -1,    -1,    -1,    -1,   314,   315,     0,   316,   317,
   318,     0,   319,   320,   321,   322,   323,   324,   325,   326,
   327,   328,   329,   330,   331,   332,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    18,    19,
    20,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   314,   315,     0,   316,   317,   318,   173,   319,   320,
   321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
   331,   332,   314,   315,     0,   316,   317,   318,     0,   319,
   320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
   330,   331,   332,   333,   315,     0,   316,   317,   318,     0,
   319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
   329,   330,   331,   332,   335,   315,     0,   316,   317,   318,
     0,   319,   320,   321,   322,   323,   324,   325,   326,   327,
   328,   329,   330,   331,   332,   419,   315,     0,   316,   317,
   318,     0,   319,   320,   321,   322,   323,   324,   325,   326,
   327,   328,   329,   330,   331,   332,   420,   315,     0,   316,
   317,   318,     0,   319,   320,   321,   322,   323,   324,   325,
   326,   327,   328,   329,   330,   331,   332,   438,   315,     0,
   316,   317,   318,     0,   319,   320,   321,   322,   323,   324,
   325,   326,   327,   328,   329,   330,   331,   332,   440,   315,
     0,   316,   317,   318,     0,   319,   320,   321,   322,   323,
   324,   325,   326,   327,   328,   329,   330,   331,   332,   443,
   315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
   323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    47,     0,     0,     0,    48,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
     0,     0,   458,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,   334,     0,     0,    80,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   176,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   435,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
     0,     0,     0,     0,    82,     0,    83,     0,     0,     0,
   442,     0,     0,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
   111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
   121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
   131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
   141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
   151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
   161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
   171,   172,   177,   178,   179,   180,   181,   182,   183,   184,
   185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
   195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
   205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
   215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
   235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
   245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
   255,   256,   257,   258,   259,   260,   261,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    47,     0,     0,     0,    48,     0,     0,
     0,   387,   388,   389,   390,   391,   392,     0,    49,     0,
     0,   173,    50,    51,    52,    53,    54,    55,    56,    57,
    58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
    78,    79,     0,     0,     0,    80,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    81,     0,
     0,     0,     0,    82,     0,    83,     0,     0,     0,     0,
     0,     0,    84,    85,    86,    87,    88,    89,    90,    91,
    92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
   102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
   112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
   122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
   132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
   142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
   152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
   162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
   172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    47,     0,     0,     0,    48,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    49,     0,     0,
     0,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,     0,     0,     0,    80,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    81,     0,     0,
     0,     0,    82,     0,    83,     0,     0,     0,     0,     0,
   173,    84,    85,    86,    87,    88,    89,    90,    91,    92,
    93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
   103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
   113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
   123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
   133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
   143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
   153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
   163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    47,     0,     0,     0,    48,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
    50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
    60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
     0,     0,     0,    80,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    81,     0,     0,     0,
     0,    82,     0,    83,     0,     0,     0,   400,     0,   401,
    84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
    94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
   104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
   114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
   124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
   134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
   144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
   154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
   164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    47,     0,     0,     0,    48,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    49,     0,     0,     0,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,     0,
     0,     0,    80,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    81,     0,     0,     0,     0,
    82,     0,    83,     0,     0,     0,   403,     0,   404,    84,
    85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
   105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
   115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
   125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
   135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
   145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
   155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
   165,   166,   167,   168,   169,   170,   171,   172,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
     0,     0,     0,    48,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    49,     0,     0,     0,    50,    51,
    52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
    62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
    72,    73,    74,    75,    76,    77,    78,    79,     0,     0,
     0,    80,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    81,     0,     0,     0,     0,    82,
     0,    83,     0,     0,     0,     0,   433,   173,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
   106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
   116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
   126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
   136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
   146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
   156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
   166,   167,   168,   169,   170,   171,   172,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    47,     0,
     0,     0,    48,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,     0,     0,     0,
    80,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    81,     0,     0,     0,     0,    82,     0,
    83,     0,     0,     0,     0,     0,   173,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
   107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
   117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
   127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
   137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
   147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
   157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
   167,   168,   169,   170,   171,   172,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   173
};

static const short yycheck[] = {    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,   113,    13,
     1,    13,     3,     4,    13,     6,     7,     8,   123,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,     1,    13,     3,     4,    13,     6,     7,
     8,    13,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    20,    21,    22,    23,     1,    13,     3,     4,
    13,     6,     7,     8,    13,    10,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    21,    22,    23,     3,
     4,    13,     6,     7,     8,    13,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
    21,    22,    23,    13,   173,    13,    13,    13,    13,     3,
     4,    13,     6,     7,     8,    13,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
     3,     4,   342,     6,     7,     8,   342,    10,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
    23,     3,     4,   342,     6,     7,     8,   343,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,   341,     0,    60,    61,    62,    63,    64,    65,
    66,    67,    -1,    69,    70,    -1,    72,    73,    74,    75,
    76,    77,    -1,    79,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
    96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
   106,   107,   108,   109,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,    13,    14,    15,    16,    17,
    18,   310,   311,    -1,    -1,    -1,   315,   316,   317,   318,
   319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
   329,   330,   331,   332,   333,   151,   335,     3,     4,    -1,
     6,     7,     8,    -1,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,     3,     4,
    -1,     6,     7,     8,    -1,    10,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    21,    22,    23,    19,
    20,    21,    22,    23,    -1,    -1,    -1,    -1,   387,   388,
   389,   390,   391,   392,     0,     1,    -1,    -1,    -1,    -1,
    -1,   400,   401,    -1,   403,   404,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   417,    -1,
   419,   420,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,     0,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   438,
   439,   440,   441,    -1,   443,   343,     1,    -1,    -1,    -1,
     5,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    14,
    15,    16,    17,    18,    -1,    20,    -1,    -1,   343,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
    -1,    56,    57,    -1,    -1,   111,   112,    -1,   114,   115,
   116,   117,   118,   119,   120,   121,   122,   341,    -1,   343,
    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   341,    -1,
   343,    -1,   111,   112,    -1,   114,   115,   116,   117,   118,
   119,   120,   121,   122,    -1,   110,    -1,    -1,    -1,   341,
   115,   343,   117,   132,    -1,    -1,    -1,    -1,    -1,   124,
   125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
   135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
   145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
   155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
   165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
   175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
   185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
   195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
   205,   206,   207,   208,   209,   210,   211,   212,    -1,    -1,
    -1,    -1,    -1,   269,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   277,    -1,    -1,    -1,   341,    -1,   343,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   343,    -1,
   269,    -1,     3,     4,    -1,     6,     7,     8,   277,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    -1,    -1,    -1,    -1,   296,    -1,    -1,
    -1,   337,   338,   339,   340,     3,     4,    -1,     6,     7,
     8,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
   339,   340,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,     3,     4,    -1,     6,     7,     8,   342,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,     3,     4,    -1,     6,     7,     8,    -1,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,     3,     4,    -1,     6,     7,     8,    -1,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    20,    21,    22,    23,     3,     4,    -1,     6,     7,     8,
    -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,     3,     4,    -1,     6,     7,
     8,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    20,    21,    22,    23,     3,     4,    -1,     6,
     7,     8,    -1,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,     3,     4,    -1,
     6,     7,     8,    -1,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,     3,     4,
    -1,     6,     7,     8,    -1,    10,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    21,    22,    23,     3,
     4,    -1,     6,     7,     8,    -1,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,     5,    -1,    -1,    -1,     9,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
    -1,    -1,   343,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
    51,    52,    53,   341,    -1,    -1,    57,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   341,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
    -1,    -1,    -1,    -1,   115,    -1,   117,    -1,    -1,    -1,
   341,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
   131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
   141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
   151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
   161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
   171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
   181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
   191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
   201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
   211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
   231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
   241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
   251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
   261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
   271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
   281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
   291,   292,   293,   294,   295,   296,   297,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,     5,    -1,    -1,    -1,     9,    -1,    -1,
    -1,    13,    14,    15,    16,    17,    18,    -1,    20,    -1,
    -1,   342,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
    52,    53,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
    -1,    -1,    -1,   115,    -1,   117,    -1,    -1,    -1,    -1,
    -1,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
   132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
   142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
   152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
   162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
   172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
   182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
   192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
   202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,     5,    -1,    -1,    -1,     9,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
    -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
    -1,    -1,   115,    -1,   117,    -1,    -1,    -1,    -1,    -1,
   342,   124,   125,   126,   127,   128,   129,   130,   131,   132,
   133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
   143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
   153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
   163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
   173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
   183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
   193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
   203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,     5,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
    -1,   115,    -1,   117,    -1,    -1,    -1,   340,    -1,   342,
   124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
   134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
   144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
   154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
   164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
   174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
   184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
   194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
   204,   205,   206,   207,   208,   209,   210,   211,   212,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     5,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
   115,    -1,   117,    -1,    -1,    -1,   340,    -1,   342,   124,
   125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
   135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
   145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
   155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
   165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
   175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
   185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
   195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
   205,   206,   207,   208,   209,   210,   211,   212,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,
    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    -1,    -1,
    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,   115,
    -1,   117,    -1,    -1,    -1,    -1,   341,   342,   124,   125,
   126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
   136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
   146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
   156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
   166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
   176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
   186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
   196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
   206,   207,   208,   209,   210,   211,   212,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,
    -1,    -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    -1,    -1,    -1,
    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,   115,    -1,
   117,    -1,    -1,    -1,    -1,    -1,   342,   124,   125,   126,
   127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
   137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
   147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
   157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
   167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
   177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
   187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
   197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
   207,   208,   209,   210,   211,   212,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   342
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

case 3:
#line 385 "praserrules.txt"
{yyclearin;;
    break;}
case 4:
#line 389 "praserrules.txt"
{myStateManager->AddStateDef((int)yyvsp[-1].nFloat);;
    break;}
case 5:
#line 390 "praserrules.txt"
{myStateManager->GetCurrStateDef()->type=(int)yyvsp[0].nFloat;;
    break;}
case 6:
#line 391 "praserrules.txt"
{myStateManager->GetCurrStateDef()->movetype=(int)yyvsp[0].nFloat;;
    break;}
case 7:
#line 392 "praserrules.txt"
{myStateManager->GetCurrStateDef()->physics=(int)yyvsp[0].nFloat;;
    break;}
case 8:
#line 393 "praserrules.txt"
{myStateManager->GetCurrStateDef()->nAnim=(int)yyvsp[0].nFloat;;
    break;}
case 9:
#line 394 "praserrules.txt"
{myStateManager->GetCurrStateDef()->bCtrl=(int)yyvsp[0].nFloat;;
    break;}
case 10:
#line 395 "praserrules.txt"
{myStateManager->GetCurrStateDef()->nPoweradd=(int)yyvsp[0].nFloat;;
    break;}
case 11:
#line 396 "praserrules.txt"
{myStateManager->GetCurrStateDef()->nJuggle=(int)yyvsp[0].nFloat;;
    break;}
case 12:
#line 397 "praserrules.txt"
{myStateManager->GetCurrStateDef()->bHitdefpersist=(int)yyvsp[0].nFloat;;
    break;}
case 13:
#line 398 "praserrules.txt"
{myStateManager->GetCurrStateDef()->bMovehitpersist=(int)yyvsp[0].nFloat;;
    break;}
case 14:
#line 399 "praserrules.txt"
{myStateManager->GetCurrStateDef()->bHitcountpersist=(int)yyvsp[0].nFloat;;
    break;}
case 15:
#line 400 "praserrules.txt"
{myStateManager->GetCurrStateDef()->nSprpriority=(int)yyvsp[0].nFloat;;
    break;}
case 16:
#line 401 "praserrules.txt"
{myStateManager->GetCurrStateDef()->bFacep2=(int)yyvsp[0].nFloat;;
    break;}
case 17:
#line 402 "praserrules.txt"
{myStateManager->GetCurrStateDef()->Velset.x=yyvsp[-2].nFloat;
                                                         myStateManager->GetCurrStateDef()->Velset.y=yyvsp[0].nFloat;;
    break;}
case 19:
#line 407 "praserrules.txt"
{myStateManager->AddState((int)yyvsp[-3].nFloat,"Nothing");;
    break;}
case 20:
#line 408 "praserrules.txt"
{myStateManager->AddState((int)yyvsp[-3].nFloat,yyvsp[-1].str);;
    break;}
case 21:
#line 409 "praserrules.txt"
{myStateManager->AddState((int)yyvsp[-4].nFloat,yyvsp[-2].str);;
    break;}
case 22:
#line 410 "praserrules.txt"
{myStateManager->AddState((int)yyvsp[-3].nFloat,"Nothing");;
    break;}
case 26:
#line 417 "praserrules.txt"
{myStateManager->AddTypeToState(TNULL);;
    break;}
case 27:
#line 418 "praserrules.txt"
{myStateManager->AddTypeToState(AFTERIMAGE);;
    break;}
case 28:
#line 419 "praserrules.txt"
{myStateManager->AddTypeToState(AFTERIMAGETIME);;
    break;}
case 29:
#line 420 "praserrules.txt"
{myStateManager->AddTypeToState(ALLPALFX);;
    break;}
case 30:
#line 421 "praserrules.txt"
{myStateManager->AddTypeToState(ANGLEADD);;
    break;}
case 31:
#line 422 "praserrules.txt"
{myStateManager->AddTypeToState(ANGLEDRAW);;
    break;}
case 32:
#line 423 "praserrules.txt"
{myStateManager->AddTypeToState(ANGLEMUL);;
    break;}
case 33:
#line 424 "praserrules.txt"
{myStateManager->AddTypeToState(ANGLESET);;
    break;}
case 34:
#line 425 "praserrules.txt"
{myStateManager->AddTypeToState(APPENDTOCLIPBOARD);;
    break;}
case 35:
#line 426 "praserrules.txt"
{myStateManager->AddTypeToState(ASSERTSPECIAL);;
    break;}
case 36:
#line 427 "praserrules.txt"
{myStateManager->AddTypeToState(ATTACKDIST);;
    break;}
case 37:
#line 428 "praserrules.txt"
{myStateManager->AddTypeToState(ATTACKMULSET);;
    break;}
case 38:
#line 429 "praserrules.txt"
{myStateManager->AddTypeToState(BGPALFX);;
    break;}
case 39:
#line 430 "praserrules.txt"
{myStateManager->AddTypeToState(BINDTOPARENT);;
    break;}
case 40:
#line 431 "praserrules.txt"
{myStateManager->AddTypeToState(BINDTOROOT);;
    break;}
case 41:
#line 432 "praserrules.txt"
{myStateManager->AddTypeToState(BINDTOTARGET);;
    break;}
case 42:
#line 433 "praserrules.txt"
{myStateManager->AddTypeToState(CHANGEANIME);;
    break;}
case 43:
#line 434 "praserrules.txt"
{myStateManager->AddTypeToState(CHANGEANIME2);;
    break;}
case 44:
#line 435 "praserrules.txt"
{myStateManager->AddTypeToState(CHANGESTATE);;
    break;}
case 45:
#line 436 "praserrules.txt"
{myStateManager->AddTypeToState(CLEARCLIPBOARD);;
    break;}
case 46:
#line 437 "praserrules.txt"
{myStateManager->AddTypeToState(CTRLSET);;
    break;}
case 47:
#line 438 "praserrules.txt"
{myStateManager->AddTypeToState(DEFENCEMULSET);;
    break;}
case 48:
#line 439 "praserrules.txt"
{myStateManager->AddTypeToState(DESTROYSELF);;
    break;}
case 49:
#line 440 "praserrules.txt"
{myStateManager->AddTypeToState(DISPLAYTOCLIPBORAD);;
    break;}
case 50:
#line 441 "praserrules.txt"
{myStateManager->AddTypeToState(ENVSHAKE);;
    break;}
case 51:
#line 442 "praserrules.txt"
{myStateManager->AddTypeToState(EXPLOD);;
    break;}
case 52:
#line 443 "praserrules.txt"
{myStateManager->AddTypeToState(EXPLODBINDTIME);;
    break;}
case 53:
#line 444 "praserrules.txt"
{myStateManager->AddTypeToState(FALLENVSHAKE);;
    break;}
case 54:
#line 445 "praserrules.txt"
{myStateManager->AddTypeToState(FORCEFEDDBACK);;
    break;}
case 55:
#line 446 "praserrules.txt"
{myStateManager->AddTypeToState(GAMEMAKEANIM);;
    break;}
case 56:
#line 447 "praserrules.txt"
{myStateManager->AddTypeToState(GRAVITY);;
    break;}
case 57:
#line 448 "praserrules.txt"
{myStateManager->AddTypeToState(HELPER);;
    break;}
case 58:
#line 449 "praserrules.txt"
{myStateManager->AddTypeToState(HITADD);;
    break;}
case 59:
#line 450 "praserrules.txt"
{myStateManager->AddTypeToState(HITBY);;
    break;}
case 60:
#line 451 "praserrules.txt"
{myStateManager->AddTypeToState(HITDEF);;
    break;}
case 61:
#line 452 "praserrules.txt"
{myStateManager->AddTypeToState(HITFALLDAMAGE);;
    break;}
case 62:
#line 453 "praserrules.txt"
{myStateManager->AddTypeToState(HITFALLSET);;
    break;}
case 63:
#line 454 "praserrules.txt"
{myStateManager->AddTypeToState(HITFALLVEL);;
    break;}
case 64:
#line 455 "praserrules.txt"
{myStateManager->AddTypeToState(HITOVERRIDE);;
    break;}
case 65:
#line 456 "praserrules.txt"
{myStateManager->AddTypeToState(HITVELSET);;
    break;}
case 66:
#line 457 "praserrules.txt"
{myStateManager->AddTypeToState(LIFEADD);;
    break;}
case 67:
#line 458 "praserrules.txt"
{myStateManager->AddTypeToState(LIFESET);;
    break;}
case 68:
#line 459 "praserrules.txt"
{myStateManager->AddTypeToState(MAKEDUST);;
    break;}
case 69:
#line 460 "praserrules.txt"
{myStateManager->AddTypeToState(MODIFYEXPLOD);;
    break;}
case 70:
#line 461 "praserrules.txt"
{myStateManager->AddTypeToState(MOVEHITRESET);;
    break;}
case 71:
#line 462 "praserrules.txt"
{myStateManager->AddTypeToState(NOTHITBY);;
    break;}
case 72:
#line 463 "praserrules.txt"
{myStateManager->AddTypeToState(NUL);;
    break;}
case 73:
#line 464 "praserrules.txt"
{myStateManager->AddTypeToState(OFFSET);;
    break;}
case 74:
#line 465 "praserrules.txt"
{myStateManager->AddTypeToState(PALFX);;
    break;}
case 75:
#line 466 "praserrules.txt"
{myStateManager->AddTypeToState(PARENTVARADD);;
    break;}
case 76:
#line 467 "praserrules.txt"
{myStateManager->AddTypeToState(PARENTVARSET);;
    break;}
case 77:
#line 468 "praserrules.txt"
{myStateManager->AddTypeToState(PAUSE);;
    break;}
case 78:
#line 469 "praserrules.txt"
{myStateManager->AddTypeToState(PLAYERPUSH);;
    break;}
case 79:
#line 470 "praserrules.txt"
{myStateManager->AddTypeToState(PLAYSND);;
    break;}
case 80:
#line 471 "praserrules.txt"
{myStateManager->AddTypeToState(POSADD);;
    break;}
case 81:
#line 472 "praserrules.txt"
{myStateManager->AddTypeToState(POSFREEZ);;
    break;}
case 82:
#line 473 "praserrules.txt"
{myStateManager->AddTypeToState(POSSET);;
    break;}
case 83:
#line 474 "praserrules.txt"
{myStateManager->AddTypeToState(POWERADD);;
    break;}
case 84:
#line 475 "praserrules.txt"
{myStateManager->AddTypeToState(POWERSET);;
    break;}
case 85:
#line 476 "praserrules.txt"
{myStateManager->AddTypeToState(PROJECTILE);;
    break;}
case 86:
#line 477 "praserrules.txt"
{myStateManager->AddTypeToState(REMOVEEXPLOD);;
    break;}
case 87:
#line 478 "praserrules.txt"
{myStateManager->AddTypeToState(REVERSALDEF);;
    break;}
case 88:
#line 479 "praserrules.txt"
{myStateManager->AddTypeToState(SCREENBOUND);;
    break;}
case 89:
#line 480 "praserrules.txt"
{myStateManager->AddTypeToState(SELFSTATE);;
    break;}
case 90:
#line 481 "praserrules.txt"
{myStateManager->AddTypeToState(SNDPAN);;
    break;}
case 91:
#line 482 "praserrules.txt"
{myStateManager->AddTypeToState(SPRPRIORITY);;
    break;}
case 92:
#line 483 "praserrules.txt"
{myStateManager->AddTypeToState(STATETYPESET);;
    break;}
case 93:
#line 484 "praserrules.txt"
{myStateManager->AddTypeToState(STOPSND);;
    break;}
case 94:
#line 485 "praserrules.txt"
{myStateManager->AddTypeToState(SUPERPAUSE);;
    break;}
case 95:
#line 486 "praserrules.txt"
{myStateManager->AddTypeToState(TARGETBIND);;
    break;}
case 96:
#line 487 "praserrules.txt"
{myStateManager->AddTypeToState(TARGETDROP);;
    break;}
case 97:
#line 488 "praserrules.txt"
{myStateManager->AddTypeToState(TARGETFACING);;
    break;}
case 98:
#line 489 "praserrules.txt"
{myStateManager->AddTypeToState(TARGETLIFEADD);;
    break;}
case 99:
#line 490 "praserrules.txt"
{myStateManager->AddTypeToState(TARGETPOWERADD);;
    break;}
case 100:
#line 491 "praserrules.txt"
{myStateManager->AddTypeToState(TARGETSTATE);;
    break;}
case 101:
#line 492 "praserrules.txt"
{myStateManager->AddTypeToState(TARGETVELADD);;
    break;}
case 102:
#line 493 "praserrules.txt"
{myStateManager->AddTypeToState(TARGETVELSET);;
    break;}
case 103:
#line 494 "praserrules.txt"
{myStateManager->AddTypeToState(TURN);;
    break;}
case 104:
#line 495 "praserrules.txt"
{myStateManager->AddTypeToState(VARADD);;
    break;}
case 105:
#line 496 "praserrules.txt"
{myStateManager->AddTypeToState(VARRANDOM);;
    break;}
case 106:
#line 497 "praserrules.txt"
{myStateManager->AddTypeToState(VARRANGESET);;
    break;}
case 107:
#line 498 "praserrules.txt"
{myStateManager->AddTypeToState(VARSET);;
    break;}
case 108:
#line 499 "praserrules.txt"
{myStateManager->AddTypeToState(VELADD);;
    break;}
case 109:
#line 500 "praserrules.txt"
{myStateManager->AddTypeToState(VELMUL);;
    break;}
case 110:
#line 501 "praserrules.txt"
{myStateManager->AddTypeToState(VELSET);;
    break;}
case 111:
#line 502 "praserrules.txt"
{myStateManager->AddTypeToState(WIDTH);;
    break;}
case 112:
#line 506 "praserrules.txt"
{myStateManager->AddInstruction(OP_STOP,0,"#");myStateManager->AddTriggerToState((int)yyvsp[-2].nFloat);;
    break;}
case 113:
#line 507 "praserrules.txt"
{myStateManager->AddInstruction(OP_STOP,0,"#");myStateManager->AddTriggerToState(192);;
    break;}
case 114:
#line 513 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,yyvsp[0].nFloat,"#");/*printf("%f\n",$1.nFloat);*/;
    break;}
case 115:
#line 514 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,yyvsp[0].str);;
    break;}
case 116:
#line 517 "praserrules.txt"
{myStateManager->AddInstruction(OP_ABS,0,"#");;
    break;}
case 117:
#line 518 "praserrules.txt"
{myStateManager->AddInstruction(OP_ACOS,0,"#");;
    break;}
case 118:
#line 519 "praserrules.txt"
{myStateManager->AddInstruction(OP_ALIVE,0,"#");;
    break;}
case 119:
#line 520 "praserrules.txt"
{myStateManager->AddInstruction(OP_ANIM,0,"#");;
    break;}
case 120:
#line 522 "praserrules.txt"
{myStateManager->AddInstruction(OP_ANIMELEM,0,"#");;
    break;}
case 121:
#line 524 "praserrules.txt"
{myStateManager->AddInstruction(OP_ANIMELEMNO,0,"#");;
    break;}
case 122:
#line 525 "praserrules.txt"
{myStateManager->AddInstruction(OP_ANIMELEMTIME,0,"#");;
    break;}
case 123:
#line 526 "praserrules.txt"
{myStateManager->AddInstruction(OP_ANIMEXIST,0,"#");;
    break;}
case 124:
#line 527 "praserrules.txt"
{myStateManager->AddInstruction(OP_ANIMTIME,0,"#");;
    break;}
case 125:
#line 528 "praserrules.txt"
{myStateManager->AddInstruction(OP_ASIN,0,"#");;
    break;}
case 126:
#line 529 "praserrules.txt"
{myStateManager->AddInstruction(OP_ATAN,0,"#");;
    break;}
case 127:
#line 530 "praserrules.txt"
{myStateManager->AddInstruction(OP_AUTHORNAME,0,"#");;
    break;}
case 128:
#line 531 "praserrules.txt"
{myStateManager->AddInstruction(OP_BACKEDGEBODY,0,"#");;
    break;}
case 129:
#line 532 "praserrules.txt"
{myStateManager->AddInstruction(OP_BACKEDGE,0,"#");;
    break;}
case 130:
#line 533 "praserrules.txt"
{myStateManager->AddInstruction(OP_CANRECOVER,0,"#");;
    break;}
case 131:
#line 534 "praserrules.txt"
{myStateManager->AddInstruction(OP_CEIL,0,"#");;
    break;}
case 132:
#line 535 "praserrules.txt"
{myStateManager->AddInstruction(OP_COMMAND,0,"#");;
    break;}
case 133:
#line 536 "praserrules.txt"
{myStateManager->AddInstruction(OP_CONST,0,"#");;
    break;}
case 134:
#line 537 "praserrules.txt"
{myStateManager->AddInstruction(OP_COS,0,"#");;
    break;}
case 135:
#line 538 "praserrules.txt"
{myStateManager->AddInstruction(OP_CTRL,0,"#");;
    break;}
case 136:
#line 539 "praserrules.txt"
{myStateManager->AddInstruction(OP_DRAWGAME,0,"#");;
    break;}
case 137:
#line 540 "praserrules.txt"
{myStateManager->AddInstruction(OP_EXP,0,"#");;
    break;}
case 138:
#line 541 "praserrules.txt"
{myStateManager->AddInstruction(OP_FACING,0,"#");;
    break;}
case 139:
#line 542 "praserrules.txt"
{myStateManager->AddInstruction(OP_FLOOR,0,"#");;
    break;}
case 140:
#line 543 "praserrules.txt"
{myStateManager->AddInstruction(OP_FEBD,0,"#");;
    break;}
case 141:
#line 544 "praserrules.txt"
{myStateManager->AddInstruction(OP_FED,0,"#");;
    break;}
case 142:
#line 545 "praserrules.txt"
{myStateManager->AddInstruction(OP_FVAR,0,"#");;
    break;}
case 143:
#line 546 "praserrules.txt"
{myStateManager->AddInstruction(OP_GAMETIME,0,"#");;
    break;}
case 144:
#line 547 "praserrules.txt"
{myStateManager->AddInstruction(OP_GETHITVAR,0,"#");;
    break;}
case 145:
#line 548 "praserrules.txt"
{myStateManager->AddInstruction(OP_HITCOUNT,0,"#");;
    break;}
case 146:
#line 549 "praserrules.txt"
{;
    break;}
case 147:
#line 550 "praserrules.txt"
{myStateManager->AddInstruction(OP_HITFALL,0,"#");;
    break;}
case 148:
#line 551 "praserrules.txt"
{myStateManager->AddInstruction(OP_HITOVER,0,"#");;
    break;}
case 149:
#line 552 "praserrules.txt"
{myStateManager->AddInstruction(OP_HITPAUSETIME,0,"#");;
    break;}
case 150:
#line 553 "praserrules.txt"
{myStateManager->AddInstruction(OP_HITSHAKEOVER,0,"#");;
    break;}
case 151:
#line 555 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                myStateManager->AddInstruction(OP_HITVAL,0,"#");;
    break;}
case 152:
#line 558 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,2,"#");
                                                myStateManager->AddInstruction(OP_HITVAL,0,"#");;
    break;}
case 153:
#line 560 "praserrules.txt"
{myStateManager->AddInstruction(OP_ID,0,"#");;
    break;}
case 154:
#line 562 "praserrules.txt"
{myStateManager->AddInstruction(OP_IFELSE,0,"#");;
    break;}
case 155:
#line 564 "praserrules.txt"
{myStateManager->AddInstruction(OP_INGUARDDIST,0,"#");;
    break;}
case 156:
#line 565 "praserrules.txt"
{myStateManager->AddInstruction(OP_ISHELPER,0,"#");;
    break;}
case 157:
#line 566 "praserrules.txt"
{myStateManager->AddInstruction(OP_ISHOMETEAM,0,"#");;
    break;}
case 158:
#line 567 "praserrules.txt"
{myStateManager->AddInstruction(OP_LIFE,0,"#");;
    break;}
case 159:
#line 568 "praserrules.txt"
{myStateManager->AddInstruction(OP_LIFEMAX,0,"#");;
    break;}
case 160:
#line 569 "praserrules.txt"
{myStateManager->AddInstruction(OP_LN,0,"#");;
    break;}
case 161:
#line 570 "praserrules.txt"
{myStateManager->AddInstruction(OP_LOG,0,"#");;
    break;}
case 162:
#line 572 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_LOSE,0,"#");;
    break;}
case 163:
#line 575 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,2,"#");
                                                           myStateManager->AddInstruction(OP_LOSE,0,"#");;
    break;}
case 164:
#line 578 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,3,"#");
                                                           myStateManager->AddInstruction(OP_LOSE,0,"#");;
    break;}
case 165:
#line 581 "praserrules.txt"
{myStateManager->AddInstruction(OP_MATCHNO,0,"#");;
    break;}
case 166:
#line 582 "praserrules.txt"
{myStateManager->AddInstruction(OP_MATCHOVER,0,"#");;
    break;}
case 167:
#line 583 "praserrules.txt"
{myStateManager->AddInstruction(OP_MOVECONTACT,0,"#");;
    break;}
case 168:
#line 584 "praserrules.txt"
{myStateManager->AddInstruction(OP_MOVEGUARDED,0,"#");;
    break;}
case 169:
#line 585 "praserrules.txt"
{myStateManager->AddInstruction(OP_MOVEHIT,0,"#");;
    break;}
case 170:
#line 586 "praserrules.txt"
{myStateManager->AddInstruction(OP_MOVETYPE,0,"#");;
    break;}
case 171:
#line 587 "praserrules.txt"
{myStateManager->AddInstruction(OP_MOVEREVERSED,0,"#");;
    break;}
case 172:
#line 588 "praserrules.txt"
{myStateManager->AddInstruction(OP_NAME,0,"#");;
    break;}
case 173:
#line 589 "praserrules.txt"
{myStateManager->AddInstruction(OP_NUMENEMY,0,"#");;
    break;}
case 174:
#line 591 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_NUMEXPLOD,0,"#");;
    break;}
case 175:
#line 594 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_NUMEXPLOD,0,"#");;
    break;}
case 176:
#line 597 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_NUMHELPER,0,"#");;
    break;}
case 177:
#line 600 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_NUMHELPER,0,"#");;
    break;}
case 178:
#line 602 "praserrules.txt"
{myStateManager->AddInstruction(OP_NUMPARTNER,0,"#");;
    break;}
case 179:
#line 603 "praserrules.txt"
{myStateManager->AddInstruction(OP_NUMPROJ,0,"#");;
    break;}
case 180:
#line 604 "praserrules.txt"
{myStateManager->AddInstruction(OP_NUMPROJID,0,"#");;
    break;}
case 181:
#line 606 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_NUMTARGET,1,"#");;
    break;}
case 182:
#line 608 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_NUMTARGET,1,"#");;
    break;}
case 183:
#line 611 "praserrules.txt"
{myStateManager->AddInstruction(OP_NAME,0,"#");;
    break;}
case 184:
#line 613 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_P2BODYDIST,0,"#");;
    break;}
case 185:
#line 616 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_P2BODYDIST,0,"#");;
    break;}
case 186:
#line 619 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_P2DIST,1,"#");;
    break;}
case 187:
#line 622 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
	                                                       myStateManager->AddInstruction(OP_P2DIST,0,"#");;
    break;}
case 188:
#line 624 "praserrules.txt"
{myStateManager->AddInstruction(OP_P2LIFE,0,"#");;
    break;}
case 189:
#line 626 "praserrules.txt"
{myStateManager->AddInstruction(OP_P2MOVETYPE,0,"#");;
    break;}
case 190:
#line 627 "praserrules.txt"
{myStateManager->AddInstruction(OP_P2NAME,0,"#");;
    break;}
case 191:
#line 628 "praserrules.txt"
{myStateManager->AddInstruction(OP_P2STATENO,0,"#");;
    break;}
case 192:
#line 629 "praserrules.txt"
{myStateManager->AddInstruction(OP_P2STATETYPE,0,"#");;
    break;}
case 193:
#line 630 "praserrules.txt"
{myStateManager->AddInstruction(OP_P3NAME,0,"#");;
    break;}
case 194:
#line 631 "praserrules.txt"
{myStateManager->AddInstruction(OP_P4NAME,0,"#");;
    break;}
case 195:
#line 632 "praserrules.txt"
{myStateManager->AddInstruction(OP_PALNO,0,"#");;
    break;}
case 196:
#line 634 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_PARENTDIST,0,"#");;
    break;}
case 197:
#line 636 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_PARENTDIST,0,"#");;
    break;}
case 198:
#line 639 "praserrules.txt"
{myStateManager->AddInstruction(OP_PI,0,"#");;
    break;}
case 199:
#line 640 "praserrules.txt"
{myStateManager->AddInstruction(OP_PLAYERIDEXIST,0,"#");;
    break;}
case 200:
#line 641 "praserrules.txt"
{myStateManager->AddInstruction(OP_PREVSTATENO,0,"#");;
    break;}
case 201:
#line 643 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_POS,0,"#");;
    break;}
case 202:
#line 645 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_POS,0,"#");;
    break;}
case 203:
#line 647 "praserrules.txt"
{myStateManager->AddInstruction(OP_POWER,0,"#");;
    break;}
case 204:
#line 648 "praserrules.txt"
{myStateManager->AddInstruction(OP_POWERMAX,0,"#");;
    break;}
case 205:
#line 649 "praserrules.txt"
{myStateManager->AddInstruction(OP_PROJCANCTIME,0,"#");;
    break;}
case 206:
#line 650 "praserrules.txt"
{myStateManager->AddInstruction(OP_PROJCONTACT,0,"#");;
    break;}
case 207:
#line 651 "praserrules.txt"
{myStateManager->AddInstruction(OP_PROJCONTACTTIME,0,"#");;
    break;}
case 208:
#line 652 "praserrules.txt"
{myStateManager->AddInstruction(OP_PROJGUARDED,0,"#");;
    break;}
case 209:
#line 653 "praserrules.txt"
{myStateManager->AddInstruction(OP_PROJGUARDEDTIME,0,"#");;
    break;}
case 210:
#line 654 "praserrules.txt"
{;
    break;}
case 211:
#line 655 "praserrules.txt"
{;
    break;}
case 212:
#line 656 "praserrules.txt"
{myStateManager->AddInstruction(OP_RANDOM,0,"#");;
    break;}
case 213:
#line 658 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_ROOTDIST,0,"#");;
    break;}
case 214:
#line 661 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_ROOTDIST,0,"#");;
    break;}
case 215:
#line 664 "praserrules.txt"
{myStateManager->AddInstruction(OP_ROUNDNO,0,"#");;
    break;}
case 216:
#line 665 "praserrules.txt"
{myStateManager->AddInstruction(OP_ROUNDSEXISTED,0,"#");;
    break;}
case 217:
#line 666 "praserrules.txt"
{myStateManager->AddInstruction(OP_ROUNDSTATE,0,"#");;
    break;}
case 218:
#line 668 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_SCREENPOS,0,"#");;
    break;}
case 219:
#line 670 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_SCREENPOS,0,"#");;
    break;}
case 220:
#line 673 "praserrules.txt"
{myStateManager->AddInstruction(OP_SELFANIMEXIST,0,"#");;
    break;}
case 221:
#line 674 "praserrules.txt"
{myStateManager->AddInstruction(OP_SIN,0,"#");;
    break;}
case 222:
#line 675 "praserrules.txt"
{myStateManager->AddInstruction(OP_STAGENO,0,"#");;
    break;}
case 223:
#line 676 "praserrules.txt"
{myStateManager->AddInstruction(OP_STATETYPE,0,"#");;
    break;}
case 224:
#line 677 "praserrules.txt"
{myStateManager->AddInstruction(OP_SYSFVAR,0,"#");;
    break;}
case 225:
#line 678 "praserrules.txt"
{myStateManager->AddInstruction(OP_SYSVAR,0,"#");;
    break;}
case 226:
#line 679 "praserrules.txt"
{myStateManager->AddInstruction(OP_TAN,0,"#");;
    break;}
case 227:
#line 680 "praserrules.txt"
{myStateManager->AddInstruction(OP_TEAMMODE,0,"#");;
    break;}
case 228:
#line 681 "praserrules.txt"
{myStateManager->AddInstruction(OP_TEAMSIDE,0,"#");;
    break;}
case 229:
#line 682 "praserrules.txt"
{myStateManager->AddInstruction(OP_TICKSPERSCOND,0,"#");;
    break;}
case 230:
#line 683 "praserrules.txt"
{myStateManager->AddInstruction(OP_TIME,0,"#");;
    break;}
case 231:
#line 684 "praserrules.txt"
{;
    break;}
case 232:
#line 685 "praserrules.txt"
{myStateManager->AddInstruction(OP_UNIQHITCOUNT,0,"#");;
    break;}
case 233:
#line 686 "praserrules.txt"
{myStateManager->AddInstruction(OP_VAR,0,"#");;
    break;}
case 234:
#line 688 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");
                                                           myStateManager->AddInstruction(OP_VEL,0,"#");;
    break;}
case 235:
#line 691 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");
                                                           myStateManager->AddInstruction(OP_VEL,0,"#");;
    break;}
case 236:
#line 694 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");myStateManager->AddInstruction(OP_WIN,0,"#");
    break;}
case 237:
#line 695 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,2,"#");myStateManager->AddInstruction(OP_WIN,0,"#");
    break;}
case 238:
#line 696 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,3,"#");myStateManager->AddInstruction(OP_WIN,0,"#");
    break;}
case 239:
#line 697 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,4,"#");myStateManager->AddInstruction(OP_WIN,0,"#");
    break;}
case 240:
#line 699 "praserrules.txt"
{myStateManager->AddInstruction(OP_NOT,0,"#");;
    break;}
case 241:
#line 700 "praserrules.txt"
{myStateManager->AddInstruction(OP_AND,0,"#");;
    break;}
case 242:
#line 701 "praserrules.txt"
{myStateManager->AddInstruction(OP_OR,0,"#");;
    break;}
case 243:
#line 702 "praserrules.txt"
{myStateManager->AddInstruction(OP_XOR,0,"#");;
    break;}
case 244:
#line 704 "praserrules.txt"
{myStateManager->AddInstruction(OP_LOGNOT,0,"#");;
    break;}
case 245:
#line 705 "praserrules.txt"
{myStateManager->AddInstruction(OP_LOGAND,0,"#");;
    break;}
case 246:
#line 706 "praserrules.txt"
{myStateManager->AddInstruction(OP_LOGOR,0,"#");;
    break;}
case 247:
#line 707 "praserrules.txt"
{myStateManager->AddInstruction(OP_LOGXOR,0,"#");;
    break;}
case 248:
#line 711 "praserrules.txt"
{myStateManager->AddInstruction(OP_ADD,0,"#");;
    break;}
case 249:
#line 712 "praserrules.txt"
{myStateManager->AddInstruction(OP_SUB,0,"#");;
    break;}
case 250:
#line 713 "praserrules.txt"
{myStateManager->AddInstruction(OP_MUL,0,"#");;
    break;}
case 251:
#line 714 "praserrules.txt"
{myStateManager->AddInstruction(OP_DIV,0,"#");;
    break;}
case 252:
#line 716 "praserrules.txt"
{myStateManager->AddInstruction(OP_SQUARE,0,"#");;
    break;}
case 253:
#line 718 "praserrules.txt"
{myStateManager->AddInstruction(OP_EQUAL,0,"#");/*printf("=\n");*/;
    break;}
case 254:
#line 719 "praserrules.txt"
{myStateManager->AddInstruction(OP_INTERVALOP1,0,"#");;
    break;}
case 255:
#line 720 "praserrules.txt"
{myStateManager->AddInstruction(OP_INTERVALOP2,0,"#");;
    break;}
case 256:
#line 721 "praserrules.txt"
{myStateManager->AddInstruction(OP_INTERVALOP3,0,"#");;
    break;}
case 257:
#line 722 "praserrules.txt"
{myStateManager->AddInstruction(OP_INTERVALOP4,0,"#");;
    break;}
case 258:
#line 724 "praserrules.txt"
{myStateManager->AddInstruction(OP_NOTEQUAL,0,"#");;
    break;}
case 259:
#line 725 "praserrules.txt"
{myStateManager->AddInstruction(OP_INTERVALOP5,0,"#");;
    break;}
case 260:
#line 726 "praserrules.txt"
{myStateManager->AddInstruction(OP_INTERVALOP6,0,"#");;
    break;}
case 261:
#line 727 "praserrules.txt"
{myStateManager->AddInstruction(OP_INTERVALOP7,0,"#");;
    break;}
case 262:
#line 728 "praserrules.txt"
{myStateManager->AddInstruction(OP_INTERVALOP8,0,"#");;
    break;}
case 263:
#line 730 "praserrules.txt"
{myStateManager->AddInstruction(OP_GREATER,0,"#");;
    break;}
case 264:
#line 731 "praserrules.txt"
{myStateManager->AddInstruction(OP_LESS,0,"#");;
    break;}
case 265:
#line 732 "praserrules.txt"
{myStateManager->AddInstruction(OP_LESSEQUAL,0,"#");;
    break;}
case 266:
#line 733 "praserrules.txt"
{myStateManager->AddInstruction(OP_GRAETEREQUAL,0,"#");;
    break;}
case 267:
#line 735 "praserrules.txt"
{/*expression Add*/;
    break;}
case 268:
#line 737 "praserrules.txt"
{/*expression Add*/;
    break;}
case 269:
#line 740 "praserrules.txt"
{myStateManager->AddInstruction(OP_SAVE,0,"#");
                                          myStateManager->ExchangeIns();
                                          myStateManager->AddInstruction(OP_EQUAL,0,"#");printf("=\n");
                                          myStateManager->AddInstruction(OP_RESTORE,0,"#");
                                          myStateManager->AddInstruction(OP_LOGAND,0,"#");
                                          ;
    break;}
case 270:
#line 747 "praserrules.txt"
{myStateManager->AddInstruction(OP_SAVE,0,"#");
                                          myStateManager->ExchangeIns();
                                          myStateManager->AddInstruction(OP_NOTEQUAL,0,"#");printf("=\n");
                                          myStateManager->AddInstruction(OP_RESTORE,0,"#");
                                          myStateManager->AddInstruction(OP_LOGAND,0,"#");
                                          ;
    break;}
case 271:
#line 754 "praserrules.txt"
{myStateManager->AddInstruction(OP_SAVE,0,"#");
                                          myStateManager->ExchangeIns();
                                          myStateManager->AddInstruction(OP_GREATER,0,"#");printf("=\n");
                                          myStateManager->AddInstruction(OP_RESTORE,0,"#");
                                          myStateManager->AddInstruction(OP_LOGAND,0,"#");
                                          ;
    break;}
case 272:
#line 761 "praserrules.txt"
{myStateManager->AddInstruction(OP_SAVE,0,"#");
                                          myStateManager->ExchangeIns();
                                          myStateManager->AddInstruction(OP_LESS,0,"#");printf("=\n");
                                          myStateManager->AddInstruction(OP_RESTORE,0,"#");
                                          myStateManager->AddInstruction(OP_LOGAND,0,"#");
                                          ;
    break;}
case 273:
#line 768 "praserrules.txt"
{myStateManager->AddInstruction(OP_SAVE,0,"#");
                                          myStateManager->ExchangeIns();
                                          myStateManager->AddInstruction(OP_LESSEQUAL,0,"#");printf("=\n");
                                          myStateManager->AddInstruction(OP_RESTORE,0,"#");
                                          myStateManager->AddInstruction(OP_LOGAND,0,"#");
                                          ;
    break;}
case 274:
#line 775 "praserrules.txt"
{myStateManager->AddInstruction(OP_SAVE,0,"#");
                                          myStateManager->ExchangeIns();
                                          myStateManager->AddInstruction(OP_GRAETEREQUAL,0,"#");printf("=\n");
                                          myStateManager->AddInstruction(OP_RESTORE,0,"#");
                                          myStateManager->AddInstruction(OP_LOGAND,0,"#");
                                          ;
    break;}
case 275:
#line 782 "praserrules.txt"
{myStateManager->AddInstruction(OP_MOD,0,"#");;
    break;}
case 276:
#line 783 "praserrules.txt"
{yyval.nFloat=-yyvsp[0].nFloat;myStateManager->AddInstruction(OP_NEG,0,"#");;
    break;}
case 277:
#line 787 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,0,"#");;
    break;}
case 278:
#line 788 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,1,"#");;
    break;}
case 279:
#line 789 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,2,"#");;
    break;}
case 280:
#line 790 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,3,"#");;
    break;}
case 281:
#line 791 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,4,"#");;
    break;}
case 282:
#line 792 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,5,"#");;
    break;}
case 283:
#line 793 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,6,"#");;
    break;}
case 284:
#line 794 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,7,"#");;
    break;}
case 285:
#line 795 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,8,"#");;
    break;}
case 286:
#line 796 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,9,"#");;
    break;}
case 287:
#line 797 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,10,"#");;
    break;}
case 288:
#line 798 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,11,"#");;
    break;}
case 289:
#line 799 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,12,"#");;
    break;}
case 290:
#line 800 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,13,"#");;
    break;}
case 291:
#line 801 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,14,"#");;
    break;}
case 292:
#line 802 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,15,"#");;
    break;}
case 293:
#line 803 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,16,"#");;
    break;}
case 294:
#line 804 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,17,"#");;
    break;}
case 295:
#line 805 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,18,"#");;
    break;}
case 296:
#line 806 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,19,"#");;
    break;}
case 297:
#line 807 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,20,"#");;
    break;}
case 298:
#line 808 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,21,"#");;
    break;}
case 299:
#line 809 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,22,"#");;
    break;}
case 300:
#line 810 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,23,"#");;
    break;}
case 301:
#line 811 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,24,"#");;
    break;}
case 302:
#line 812 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,25,"#");;
    break;}
case 303:
#line 813 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,26,"#");;
    break;}
case 304:
#line 814 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,27,"#");;
    break;}
case 305:
#line 815 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,28,"#");;
    break;}
case 306:
#line 816 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,29,"#");;
    break;}
case 307:
#line 817 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,30,"#");;
    break;}
case 308:
#line 818 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,31,"#");;
    break;}
case 309:
#line 819 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,32,"#");;
    break;}
case 310:
#line 820 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,33,"#");;
    break;}
case 311:
#line 821 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,34,"#");;
    break;}
case 312:
#line 822 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,35,"#");;
    break;}
case 313:
#line 823 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,36,"#");;
    break;}
case 314:
#line 824 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,37,"#");;
    break;}
case 315:
#line 825 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,38,"#");;
    break;}
case 316:
#line 826 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,39,"#");;
    break;}
case 317:
#line 827 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,40,"#");;
    break;}
case 318:
#line 828 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,41,"#");;
    break;}
case 319:
#line 829 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,42,"#");;
    break;}
case 320:
#line 830 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,43,"#");;
    break;}
case 321:
#line 831 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,44,"#");;
    break;}
case 322:
#line 832 "praserrules.txt"
{myStateManager->AddInstruction(OP_PUSH,45,"#");;
    break;}
case 323:
#line 835 "praserrules.txt"
{myStateManager->AddInstruction(OP_STOP,0,"#");myStateManager->SetParam(PA_VALUE);;
    break;}
case 324:
#line 836 "praserrules.txt"
{myStateManager->AddInstruction(OP_STOP,0,"#");myStateManager->SetParam(PA_XVALUE);;
    break;}
case 325:
#line 837 "praserrules.txt"
{myStateManager->AddInstruction(OP_STOP,0,"#");myStateManager->SetParam(PA_YVALUE);;
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
#line 846 "praserrules.txt"

