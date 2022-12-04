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


#ifndef STRUCT_H
#define STRUCT_H


#include <sdl/SDL.H>

#ifndef _XBOX
#define VER "Beta FSM"
#else
#define VER "XBOX Beta FSM"
#endif

#define OMTITLE "OpenMugen Beta FSM Version(not playable) Try to press Left or Right"

//Some define for screen
#define XMAX 319
#define YMAX 239

//Define Action FLIPFLAGS
#define HFLIPFLAG   0x11
#define VFLIPFLAG   0x12
#define HVFLIPFLAG  0x13

#define UCHANGE 255

//Define statetypes
#define STSTAND  0x53   //ascii code for S
#define STCROUCH 0x43   //ascii code for C
#define STATTACK 0x41   //ascii code for A
#define STLSTATE 0x4c   //ascii code for L

//Define move types
#define MOVATTACK 0x41   //ascii code for A
#define MOVIDLE   0x49   //ascii code for I
#define MOVHIT    0x42   //ascii code for H

//define Physic types
#define PHSTAND  0x52   //ascii code for S
#define PHCROUCH 0x43   //ascii code for C
#define PHAIR    0x41   //ascii code for A
#define PHNONE   0x4e   //ascii code for N





//Define Blit flags
#define BLT_NORMAL          0x21
#define BLT_NORMALFLIPH     0x22
#define BLT_NORMALFLIPV     0x23
#define BLT_NORMALFLIPHV    0x24

//define AnimTypes
#define light	0xa0
#define medium	0xa1
#define hard	0xa2
#define back	0xa3
#define up		0xa4
#define diup	0xa5

//define attack_types
#define high	0xa6
#define low		0xa7
#define trip	0xa8

//define State Controller
#define dAfterImage			1
#define dAfterImageTime		2
#define dAllPalFX			3
#define dAngleAdd			4
#define dAngleDraw			5
#define dAngleMul			6
#define dAngleSet			7
#define dAppendToClipboard	8
#define dAssertSpecial		9
#define dAttackDist			10
#define dAttackMulSet		11
#define dBGPalFX			12
#define dBindToParent		13
#define dBindToRoot			14
#define dBindToTarget		15
#define dChangeAnim			16
#define dChangeAnim2		17
#define dChangeState		18
#define dClearClipboard		19
#define dCtrlSet			20
#define dDefenceMulSet		21
#define dDestroySelf		22
#define dDisplayToClipboard	23
#define dEnvShake			24
#define dExplod				25
#define dExplodBindTime		26
#define dFallEnvShake		27
#define dForceFeedback		28
#define dGameMakeAnim		29
#define dGravity			30
#define dHelper				31
#define dHitAdd				32
#define dHitBy				33
#define dHitDef				34
#define dHitFallDamage		35
#define dHitFallSet			36
#define dHitFallVel			37
#define dHitOverride		38
#define dHitVelSet			39
#define dLifeAdd			40
#define dLifeSet			41
#define dMakeDust			42
#define dModifyExplod		43
#define dMoveHitReset		44
#define dNotHitBy			45
#define dNull				46
#define dOffset				47
#define dPalFX				48
#define dParentVarAdd		49
#define dParentVarSet		50
#define dPause				51
#define dPlayerPush			52
#define dPlaySnd			53
#define dPosAdd				54
#define dPosFreeze			55
#define dPosSet				56
#define dPowerAdd			57
#define dPowerSet			58
#define dProjectile			59
#define dRemoveExplod		60
#define dReversalDef		61
#define dScreenBound		62
#define dSelfState			63
#define dSndPan				64
#define dSprPriority		65
#define dStateTypeSet		66
#define dStopSnd			67
#define dSuperPause			68
#define dTargetBind			69
#define dTargetDrop			70
#define dTargetFacing		71
#define dTargetLifeAdd		72
#define dTargetPowerAdd		73
#define dTargetState		74
#define dTargetVelAdd		75
#define dTargetVelSet		76
#define dTurn				77
#define dVarAdd				78
#define dVarRandom			79
#define dVarRangeSet		80
#define dVarSet				81
#define dVelAdd				82
#define dVelMul				83
#define dVelSet				84
#define dWidth				85

//define the ImageList types
#define IMP1 0xc1
#define IMP2 0xc2
#define IMP3 0xc3
#define IMP4 0xc4
#define IMMENU 0xc5
#define IMSTAGE 0xc6

#define STRINGSIZE 20
#define INSTSIZE 30


//#define NULL 0

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

//#define MAX_PATH 255

#define PRESSED  200
#define RELEASED 100
#define NONE     0

//Define  Controller buttoms
#define RIGHT     1
#define LEFT      2
#define UP        3    
#define DOWN      4
#define BUTTONA   5
#define BUTTONB   6
#define BUTTONC   7
#define BUTTONX   8
#define BUTTONY   9
#define BUTTONZ   10



struct PLKEY
{
    Uint16 key;
    Uint32 nTime;
    bool bPressed;

};

struct KEYELEMENT
{
    Uint16 nKey;
    Uint32 nTime;
    Uint32 nKeyTime;
    

};

#define MAXCOMMAND 30

//now lets define a command
struct PLCOMMAND
{
    Uint16 nCommand[MAXCOMMAND];
    Uint8 nHowManyCommand;
    Uint8 nCommandTime;
    char strCommand[STRINGSIZE];

};


//Movedata Struct for Saving the current Button States
struct KEYBOARDDATA
{
  PLKEY Right;
  PLKEY Left;
  PLKEY Up;
  PLKEY Down;
  PLKEY ButtonA;
  PLKEY ButtonB;
  PLKEY ButtonC;
  PLKEY ButtonX;
  PLKEY ButtonY;
  PLKEY ButtonZ;
  PLKEY bStart;
  PLKEY bPause;
  short nKey;
  bool bKeyBoard;
};

//Pcx File Header to get the File Header
struct PCXHEADER
{
	unsigned char Manufacturer;
	unsigned char Version;
	unsigned char Encoding;
	unsigned char BPP;
	unsigned short x,y;
	unsigned short widht,height;
	unsigned short HRES,VRES;
	unsigned char ColorMap[48];
	unsigned char reserved;
	unsigned char NPlanes;
	unsigned char bytesPerLine;
	unsigned char palletInfo;
	unsigned char Filler[58];

};



struct XY
{
	int x;
	int y;

};
//Color Pallet to save Sff Color Pallet
struct PcxPallet
{
	BYTE R;
	BYTE G;
	BYTE B;

};
//Pal color
struct PalColor
{
	BYTE R;
	BYTE G;
	BYTE B;

};

struct PalColorFloat
{
	float R;
	float G;
	float B;

};
//Sff File header 
struct SFFHEADER
{
	unsigned char signature[11];
	unsigned char verhi;
	unsigned char verlo;
	unsigned char verhi2;
	unsigned char verlo2;
	long		  NumberOfGroups;
	long		  NumberOfImage;
	long		  SubHeaderFileOffset;
	long		  SizeOfSubheader;
	unsigned char PaletteType;
	unsigned char BLANK[479];

};
//Sff Subheader
struct SFFSUBHEADER
{
	long NextSubheaderFileOffset;
	long LenghtOfSubheader;
	short x;
	short y;
	short GroubNumber;
	short ImageNumber;
	short IndexOfPrevious;
	bool  PalletSame;
	unsigned char BALNK[13];

};
//Sound  header
struct SNDHEADER
{
	char strSignatur[12];
	short verhi;
	short verlo;
	long nNumberOfSounds;
	long SubHeaderFileOffset;
	char strBlank[488];

};
//Sound SubHeader
struct SNDSUBHEADER
{
	long  NextSubHeaderFileOffset;
	long  SubFileLenght;
	long  nGroupNumber;
	long  SampleNumber;

};
//Sound data struct
struct SNDDATA
{
	int nGroupNumber;
	int nSampleNumber;
	BYTE *bySndData;

};

//Linked list struct
struct SoundLinkedList
{
	SNDDATA SoundData;
	SoundLinkedList *VorGaenger;
	SoundLinkedList *NachFolger;


};

//A Structure to save Sprite information in a Linked list
struct SFFSPRITE
{
	short x;
	short y;
	PCXHEADER PcxHeader;
	int GroupNumber;
	int ImageNumber;
	bool bIsLinked;
	bool bPalletSame;
	PcxPallet ColorPallet[256];
	BYTE *byPcxFile;

};
//Linked list Struct
struct SffSpriteLinkedList
{
	SFFSPRITE SpriteData;
	SffSpriteLinkedList *VorGaenger;
	SffSpriteLinkedList *NachFolger;
};


/*struct TIME
{
	int nMilliseconds;
	int nSeconds;
	int nMinutes;
	int nHouers;

};*/

//Sprite info Struct
struct SpriteInfo
{
//	LPDIRECTDRAWSURFACE7 lpSurface;
	char *strGameName;
	char *strFileName;
	int Type;
	int MemType;
};

struct ClsnRECT
{
    short x,y;
    short h,w;

};
//Clsn Struct to hold one Clns Rectangle with type
struct Clsn
{
	bool bClsn1;
	ClsnRECT ClsnRect;

};
//Element Strcut to save one Elment of an Action Block
struct Element
{
	int nGroupNumber;
	int nImageNumber;
	int x;
	int y;
	int nDuringTime;
	Uint16 FlipFlags;
	Uint16 ColorFlags;
	Clsn *pClnsData;
	int nNumberOfClsn;
};

//Action Element to hold one Action Block with its Elements
struct ActionElement
{
	int nActionNumber;
	Element* AnimationElement;
	int loopStart;
	int nNumberOfElements;
	int nCurrentImage;
	Uint16 dwStartTime;
	Uint16 dwCompletAnimTime;
	Uint16 dwCurrTime;
	Uint16 dwCurrentImageTime;
	bool bLooped;


};
//Linked list struct for Airmanger
struct AirLinkedList
{
	ActionElement ElementData;
	AirLinkedList *NachFolger;
	AirLinkedList *VorGaenger;

};
//Data of tim Struct to save the time
struct DataOfTime
{
	int Days;
	int Month;
	int Years;

};
//Playerdata to save the path and names defined in the def File
struct PLAYERDATA
{
	char strName[255];
	char strDisplayName[255];
	char strAuthor[255];
	DataOfTime CharVersion;
	DataOfTime MugenVersion;

	char strAirFile[255];
	char strSffFile[255];
	char strSndFile[255];
	char strCmdFile[255];
	char strCnsFile[255];
	char strPalFile[255][12];

};
//a struct to save X and Y values
struct XYVALUE
{
	float x;
	float y;

};
//Player Data
struct PLDATA
{
	int nLife;
	int nAttack;
	int nDefence;
	int nFallDefenceUp;
	int nLieDownTime;
	int nAirjuggle;
	int nSparkno;
	int nGuardSparkno;
	bool bKoEcho;
	int nVolumen;
	int nIntPersistIndex;
	int nFloatPersistIndex;

};
//Player Size
struct PLAYERSIZE
{
	int nXscale;
	int nYscale;
	int nGroundBack;
	int nGroundFront;
	int nAirBack;
	int nAirFront;
	int nHeight;
	XY nAttackWidth;
	int nAttackDistance;
	int nProjAttackDist;
	bool bProjDoScale;
	XY nHeadPos;
	XY nMidPos;
	int nShadowOffset;
	XY nDrawOffset;

};
//Playervelocity struct
struct PLAYERVELOCITY
{
	float nWalkFwd;
	float nWalkBack;
	XYVALUE RunFwd;
	XYVALUE RunBack;
	XYVALUE JumpNeu;
	XYVALUE JumpBack;
	XYVALUE JumpFwd;
	XYVALUE RunjumpBack;
	XYVALUE RunjumpFwd;
	XYVALUE AirjumpNeu;
	XYVALUE AirjumpBack;
	XYVALUE AirjumpFwd;

};
//Playermovement Struct
struct PLAYERMOVEMENT
{
	int AirJumpNum;
	int nAirJumpHight;
	float yaccel;
	float StandFriction;
	float CrouchFriction;

};
//Playerconstant struct
struct PLAYERCONST
{
	PLDATA			PlayerData;
	PLAYERSIZE		PlayerSize;
	PLAYERVELOCITY	PlayerVelocity;
	PLAYERMOVEMENT	PlayerMovement;


};




struct StagePlayerInfo
{
	int nP1startx; 
	int nP1starty;
	int nP1startz;
	int nP1facing;
	
	int nP2startx; 
	int nP2starty;
	int nP2startz;
	int nP2facing;

};


//State controller

//AfterImage controller
struct AfterImage
{
	int nTime;
	int nLength;
	int nPalColor;
	bool bPallInvertAll;
	PalColor PalBright;
	PalColor PalContrast;
	PalColor PalPostBright;
	PalColor PalAdd;
	PalColorFloat PalMul;
	int nTimeHap;
	int nFrameGap;
//	char strTrans[MAX_PATH];

};

//AfterImageTime controller
struct AfterImageTime
{
	int nTime;

};

//AllPalFX
struct AllPalFX
{
	int nDuration;
	PalColor Add;
	PalColor Mul;
	PalColor SinAdd;
	bool bIntervall;
	int nColor;


};

struct PalFx
{
	int nDuration;
	PalColor Add;
	PalColor Mul;
	PalColor SinAdd;
	bool bIntervall;
	int nColor;


};


//AngleAdd
struct AngleAdd
{
	float add_angle;

};

//AngleDraw



//AngleMul
struct AngleMul
{
	float angle_multiplier;

};


//AngleSet
struct AngleSet
{
	float angle;

};

//AppendToClipboard = DisplayToClipboard

//AssertSpecial
struct AssertSpecial
{
/*	char strFlag[MAX_PATH];
	char strFlag2[MAX_PATH];
	char strFlag3[MAX_PATH];
*/	

};

//AttacktDist
struct AttackDist
{
	int nGuardDist;
};

//AttackMulSet
struct AttackMulSet
{
	float attack_mul;

};

//BGPalFX =PalFX
struct BGPalFX
{
	int nDuration;
	PalColor Add;
	PalColor Mul;
	PalColor SinAdd;
	bool bIntervall;
	int nColor;


};


//BindToParent
struct BindToParent
{
	int nBindTime;
	int nFacingFlag;
	XYVALUE pos;
};

//BindToRoot
struct BindToRoot
{
	int nBindTime;
	int nFacingFlag;
	XYVALUE pos;

};

//BindToTarget
struct BindToTarget
{
	int nBindTime;
	int nBindID;
	XYVALUE pos;

};

//ChangeAnim
struct ChangeAnim
{
	int nAninmNo;
	int nElemNo;
};

//ChangeAnim2
struct ChangeAnim2
{
	int nAninmNo;
	int nElemNo;

};

//ChangeState
struct ChangeState
{
	int nStateNo;
	int nCtrlFlag;
	int nAnimNo;

};

//ClearClipBord



//CtrlSet
struct CtrlSet
{
	int nCtrlFlag;
};

//DefenceMulSet
struct DefenceMulSet
{
	float defense_mul;
};

//DestroySelf


//DisplayToClipboard
struct DisplayToClipboard
{
	//char strText[MAX_PATH];
	//Parms

};

//EnvColor
struct EnvColor
{
	PalColor value;
	int nTime;
	int nUnder;


};

//EnvShake
struct EnvShake
{
	int nShakeTime;
	float shake_speed;
	int shake_amplitude;
	float phase_offset;

};

//Explod
struct Explod
{
/*	bool bFightDef;
	int anim_no;
	int id_no;
	XYVALUE pos;
	char strPostype[MAX_PATH];
	int nFacing;
	int nVfacing;
	int nBindTime;
	XYVALUE vel;
	XYVALUE accel;
	XYVALUE random;
	int nRemoveTime;
	bool bSuperMove;
	int nSuperMoveTime;
	int nPauseMoveTime;
	XYVALUE scale;
	int nSprPriority;
	bool bOnTop;
	PalColor Shadow;
	bool bOwnPal;
	bool bRemoveOnGetHit;*/

};

//ExplodBindTime
struct ExplodBindTime
{
	int nIdNo;
	int nBindingTime;

};

//ForceFeedback
/*struct ForceFeedback
{
	char strWaveType[MAX_PATH];
	int nDuration;

};*/

//FallEnvShake

//GameMakeAnim
struct GameMakeAnim
{
	int nAnimNo;
	int nUnderFlag;
	XYVALUE pos;
	int rand_amt;

};
//Gravity


//Helper
struct Helper
{
/*	char strHelperType[MAX_PATH];
	char strName[MAX_PATH];
	int nIdNo;
	XYVALUE pos;
	char strPosType[MAX_PATH];
	int nFacing;
	int nStartState;
	int nKeyCtrl;
	int nOwnPal;
	PLAYERSIZE HelperSize;*/

};

//HitAdd
struct HitAdd
{
	int nAddCount;
};


//HitBy
struct HitBy
{
	//char strAttrString[MAX_PATH];
	int nEffectiveTime;

};


struct priority
{
	int nHitPrior;
//	char strHitType[MAX_PATH];

};

struct damage
{
	int nHitDamage;
	int nGuardDamage;

};

struct pausetime
{
	int nP1pauseTime;
	int nP2ShakeTime;

};

struct hitsound
{
	int nSndGrp;
	int nSndItem;


};

struct nochainID
{
	int nochain1;
	int nochain2;

};
//EnvShakeStruct
struct EnvShakeSt
{
	int nEnvShakeTime;
	float EnvShakeFreq;
	int nEnvShakeAmpl;
	float EnvShakePhase;

};

//HitDef
struct HitDef
{
	char strAttrArg1;
	char strAttrArg2[2];
	char strHitFlag[3];
	char strGuardFlag[3];
	//Opt
	char strAffectteam;
	int nAnimType;
	int nAirAnimType;
	int nFallAnimType;
	priority HitPrior;
	damage Damage;
	pausetime PauseTime;
	pausetime GuardPauseTime;
	int nSparkNo;
	int nGuardSparkNo;
	XYVALUE SparkXY;
	hitsound HitSound;
	hitsound GuardHitSound;
	int nGroundType;
	int nAirType;
	int nGroundSlideTime;
	int nGroundHitTime;
	int nAirHitTime;
	int nGuardCtrlTime;
	int nGuardDist;
	float yaccel;
	XYVALUE GroundVelocity;
	float GuardVelocity;
	XYVALUE AirVelocity;
	XYVALUE AirGuardVelocity;
	float	GroundCornerPusVelOff;
	float	AirCornerPusVelOff;
	float	DownCornerPusVelOff;
	float	GuardCornerPusVelOff;
	float	AirGuardCornerPusVelOff;
	int nAirGuardCtrlTime;
	int AirJuggle;
	XYVALUE mindist;
	XYVALUE maxdist;
	XYVALUE snap;
	int nSprPriority;
	int nP1facing;
	int nP1getp2facing;
	int nP2facing;
	int nP1stateno;
	int nP2stateno;
	int nP2getp2state;
	int nForcestand;
	int nFall;
	float fallxvelocity;
	float fallyvelocity;
	int nFallRecover;
	int nFallRecoverTime;
	int nFallDamage;
	int nAirFall;
	XYVALUE DownVelocity;
	int nDownHitTime;
	int nDownBounce;
	int nID;
	int nChainID;
	nochainID NochainID;
	bool bHitOnce;
	int nKill;
	int nGuardKill;
	int nFallKill;
	int nNumHits;
	//getPower
	int nP1power;
	int nP1gpower;
	//give Power;
	int nP2power;
	int nP2gpower;
	int nPalFxTime;
	
	PalFx palFX; 	
	EnvShakeSt envshake;
	EnvShakeSt FallEnvShake;
	XYVALUE AttackWidth;
	
};

//HitFallDamage


//HitFallSet
struct HitFallSet
{
	int nFallSetFlag;
	XYVALUE Velocity;
};

//HitFallVel


//HitOverride
struct HitOverride
{
	int nSlot;
//	char strAttr[MAX_PATH];
	int nStateno;
	int nTime;
	bool forceair;
};

//HitVelSet
struct HitVelSet
{
	XYVALUE Velocity;
};

//LifeAdd
struct LifeAdd
{
	int nAddAmt;
	//opt
	int nKill;
	int nAbsFlag;

};

//LifeSet
struct LifeSet
{
	int nLifeAmt;

};

//MakeDust
struct MakeDust
{
	XYVALUE pos;
	XYVALUE pos2;
	int nSpacing;


};

//ModifyExplod == Explod

//MoveHitReset

//NotHitBy
struct NotHitBy
{
//	char strAttr[MAX_PATH];
	int nTime;


};

//NULL

//Offset
struct Offset
{
	XYVALUE pos;


};

//ParentVarAdd
struct ParentVarAdd
{
	int var_noI;
	int ValueI;

	int var_noF;
	float ValueF;

};

//ParentVarSet
struct ParentVarSet
{
	int var_noI;
	int ValueI;

	int var_noF;
	float ValueF;


};

//Pause
struct Pause
{
	int nTime;
	int nMoveTime;
};

//PlayerPush
struct PlayerPush
{
	int PushFlag;


};

//PlaySnd
struct PlaySnd
{
	int nGroupNumber;
	int nSampleNumber;
	//opt
	int nVolume;
	int nChannelNo;
	int nLowpriority;
	float freqmul;
	int nLoop;
	int nPan;
	int nAbsPan;

};

//PosAdd
struct PosAdd
{
	XYVALUE pos;


};

//PosFreeze
struct PosFreeze
{
	//opt
	int nFreezeFlag;

};

//PosSet
struct PosSet
{
	//Opt
	XYVALUE pos;

};

//PowerAdd
struct PowerAdd
{
	int nAddAmt;

};

//PowerSet
struct PowerSet
{
	int nAddAmt;

};

//Projectile
struct Projectile
{
	int nProjID;
	int nProjAnim;
	int ProjHitAnim;
	int ProjRemAnim;
	int ProjCancelAnim;
	int ProjRemove;
	XYVALUE VeloCity;
	XYVALUE RemVeloCity;
	XYVALUE AccelVeloCity;
	XYVALUE VelMul;
	int nProjHits;
	int nProjMissTime;
	int nProjPriority;
	int nSprProjPriority;
	int nProjEdgeBound;
	int nProjStageBound;
	XYVALUE ProjHeightBound;
	XYVALUE Offset;
//	char strPosType[MAX_PATH];
	PalColor ProjShadow;
	int nSuperMoveTime;
	int nPauseTime;


};

//RemoveExplod
struct RemoveExplod
{
	//opt
	int nRemoveId;

};

//ReversalDef
struct ReversalDef
{
//	char strReversal[MAX_PATH];

};

//ScreenBound
struct ScreenBound
{
	//opt;
	int BoundFlag;
	XYVALUE MoveCamera;


};

//SelfState

//SprPriority
struct SprPriority
{
	int nPriorityLevel;

};

//StateTypeSet
struct StateTypeSet
{
	char strStateType;
	char strMoveType;

};

//SndPan
struct SndPan
{
	int nChannelNo;
	int nPan;
};

//StopSnd
struct StopSnd
{
	int ChanelNo;

};

//SuperPause
struct SuperPause
{
	//opt
	int nPauseTime;
	int nAnimNo;
	hitsound sound;
	XYVALUE pos;
	int movetime;
	bool bDarken;
	float p2defmul;
	int nPowerAdd;
	bool bUnHitAble;


};

//TargetBind
struct TargetBind
{
	int nBindTime;
	int nBindID;
	XYVALUE pos;

};

//TargetDrop
struct TargetDrop
{
	int nExludeID;
	int nKeepOne;

};

//TargetFacing
struct TargetFacing
{
	int nFacingVal;

};

//TargetLifeAdd
struct TargetLifeAdd
{
	int nAddAmt;
	int nKillFlag;
	int nAbsFlag;
};

//TargetPowerAdd
struct TargetPowerAdd
{
	int nAddAmt;

};

//TargetState
struct TargetState
{
	int nStateNo;
};

//TargetVelAdd
struct TargetVelAdd
{
	XYVALUE VeloCity;

};

//TargetVelSet
struct TargetVelSet
{
	XYVALUE VeloCity;
};

//Turn

//VarAdd
struct VarAdd
{
	int nVarNoI;
	int nVarValueI;

	int   nVarNoF;
	float nVarValueF;

};

//VarRandom
struct VarRandom
{
	int nVarNo;
	//opt
	//Range
	int nLeastVal;
	int nGreatestVal;

};

//VarRangeSet
struct VarRangeSet
{
	int nIntExpr;
	float floatexpr;

	//opt
	int nFirstIdx;
	int nLastIdx;


};

//VarSet
struct VarSet
{
	int nVarNoI;
	int nValueI;

	int nVarNoF;
	float nValueF;

};

//VelAdd
struct VelAdd
{
	XYVALUE VeloCity;

};

//VelMul
struct VelMul
{
	XYVALUE VeloCity;

};

//VelSet
struct VelSet
{
	XYVALUE VeloCity;

};
//Width
struct Width
{
	XYVALUE edge;
	XYVALUE PLAYER;

};

//Stagedef
	enum Transparency
	{
		none=0,
		add,
		sub,
		avg

	};

	enum type
	{
		normal=0,
		parallax,
		height,
		anim	

	};

	struct BGDef
	{
		int nType;
		XY SpriteNo;
		XY Start;
		XYVALUE delta;
		int nTrans;
		bool bMask;
		XYVALUE velocity;
		XY tile;
		XY tilespacing;
		XYVALUE xscale;
		XY width;
		float yscalestart;
		float yscaledelta;
		bool bPositionLink;
		int nLayerno;
	//	RECT Winodw;
		int nActionNumber;
	};

	struct Camera
	{
		int nStartX;
		int nStartY;
		int nBoundLeft;
		int nBoundRight;
		int nBounHigh;
		int nBoundLow;
		float fVerticalFollow;
		int nFloorTesion;
		int nTension;

	};

	struct PlayerInfo
	{
		int nP1startX;
		int nP1startY;
		int nP1startZ;
		int nP1facing;


		int nP2startX;
		int nP2startY;
		int nP2startZ;
		int nP2facing;

		int nLeftBound  ;
		int nRightBound ;
		int nTopBound	;
		int nBotBound	;


	};

	struct Scaling
	{
		int nTopZ;
		int nBotZ;
		float nTopScale;
		float nBotScale;
	};

	struct Bound
	{
		int nScreenLeft;
		int nScreenRight;

	};

	struct StageInfo
	{
		int nZoffset;
		bool bAutoturn;

	};

	struct Shadow
	{
		int R,G,B;
		float yScal;

	};

	struct StageDef
	{
		char strStageName[255];
		Camera StageCamara;
		PlayerInfo StagePlayerInfo;
		Scaling StageScaling;
		Bound StageBound;
		StageInfo Info;
		Shadow StageShadow;
		char strBgmMusic[255];
		char strSprFile[255];


	};

struct BGdefLinkedList
{
	BGDef BgDefData;
	BGdefLinkedList* VorGeanger;
	BGdefLinkedList* NachFolger;


};

struct ImageList
{
    ImageList *Next;
    ImageList *Prev;
    short nGroupNumber;
    short nImageNumber;
    SDL_Surface * lpSurface;

};



/*
    Opcode definition for the VirtualMachine
*/
enum OPCODES
{
    OP_PUSH=20,
    OP_POP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_EQUAL,
    OP_NOTEQUAL,
    OP_LESS,
    OP_GREATER,
    OP_LESSEQUAL,
    OP_GRAETEREQUAL,
    OP_INTERVALOP1,
    OP_INTERVALOP2,
    OP_INTERVALOP3,
    OP_INTERVALOP4,
    OP_INTERVALOP5,
    OP_INTERVALOP6,
    OP_INTERVALOP7,
    OP_INTERVALOP8,
    OP_LOGNOT,
    OP_LOGAND,
    OP_LOGOR,
    OP_LOGXOR,
    OP_NOT,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_SQUARE,
    OP_ABS,
    OP_NEG,
    OP_ACOS,
    OP_ALIVE,
    OP_ANIM,
    OP_ANIMELEM,
    OP_SAVE,
    OP_RESTORE,
    OP_ANIMELEMNO,
    OP_ANIMELEMTIME,
    OP_ANIMEXIST,
    OP_ANIMTIME,
    OP_ASIN,
    OP_ATAN,
    OP_AUTHORNAME,
    OP_BACKEDGEBODY,
    OP_BACKEDGE,
    OP_CANRECOVER,
    OP_CEIL,
    OP_COMMAND,
    OP_CONST,
    OP_COS,
    OP_CTRL,
    OP_DRAWGAME,
    OP_EXP,
    OP_FACING,
    OP_FLOOR,
    OP_FEBD,
    OP_FED,
    OP_FVAR,
    OP_GAMETIME,
    OP_GETHITVAR,
    OP_HITCOUNT,
    OP_HITFALL,
    OP_HITOVER,
    OP_HITPAUSETIME,
    OP_HITSHAKEOVER,
    OP_HITVAL,
    OP_ID,
    OP_IFELSE,
    OP_INGUARDDIST,
    OP_ISHELPER,
    OP_ISHOMETEAM,
    OP_LIFE,
    OP_LIFEMAX,
    OP_LN,
    OP_LOG,
    OP_LOSE,
    OP_MATCHNO,
    OP_MATCHOVER,
    OP_MOVECONTACT,
    OP_MOVEGUARDED,
    OP_MOVEHIT,
    OP_MOVETYPE,
    OP_MOVEREVERSED,
    OP_NAME,
    OP_NUMENEMY,
    OP_NUMEXPLOD,
    OP_NUMHELPER,
    OP_NUMPARTNER,
    OP_NUMPROJ,
    OP_NUMPROJID,
    OP_NUMTARGET,
    OP_P2BODYDIST,
    OP_P2DIST,
    OP_P2LIFE,
    OP_P2MOVETYPE,
    OP_P2NAME,
    OP_P2STATENO,
    OP_P2STATETYPE,
    OP_P3NAME,
    OP_P4NAME,
    OP_PALNO,
    OP_PARENTDIST,
    OP_PI,
    OP_PLAYERIDEXIST,
    OP_PREVSTATENO,
    OP_POS,
    OP_POWER,
    OP_POWERMAX,
    OP_PROJCANCTIME,
    OP_RANDOM,
    OP_ROOTDIST,
    OP_ROUNDNO,
    OP_ROUNDSEXISTED,
    OP_ROUNDSTATE,
    OP_SCREENPOS,
    OP_SELFANIMEXIST,
    OP_SIN,
    OP_STAGENO,
    OP_STATETYPE,
    OP_SYSFVAR,
    OP_SYSVAR,
    OP_TAN,
    OP_TEAMMODE,
    OP_TEAMSIDE,
    OP_TICKSPERSCOND,
    OP_TIME,
    OP_UNIQHITCOUNT,
    OP_VAR,
    OP_VEL,
    OP_WIN,
    OP_PROJCONTACT,
    OP_PROJCONTACTTIME,
    OP_PROJGUARDED,
    OP_PROJGUARDEDTIME,
    OP_STOP,
    OP_MOD,
    OP_NOP
   
};

enum PARAMVALUES
{
    PA_VALUE=128,
    PA_XVALUE,
    PA_YVALUE


};

struct INSTRUCTION
{   
    Uint16 n_OpCode;
    float Value;
    char *strValue;

};


struct Stacktype
{
    float Value;
    char string[50]; 

};
#define INSPERTRIGGER 40
#define NUMTRIGGER  20
#define PARAMS 10
#define T_TRIGGERALL 192
#define NOPARAM -333

struct PLTRIGGER
{
    int nTriggerType;
    INSTRUCTION* pInts;

};

struct CONTROLLERPARAMS
{
    int nParam;
    INSTRUCTION pInts[INSPERTRIGGER];
};

struct PLSTATE
{
	short nStateNumber;
	char strSomeNumber[50];
	short nType;
	PLTRIGGER *triggers;
	CONTROLLERPARAMS params[PARAMS];
	short nHowManyTriggers;
	
	//Link to the next member
	PLSTATE *lpNext;

};

//PLayer Statedef Struct
struct PLSTATEDEF
{
	//recommend parameters
	short StateNumber;
	short type;
	short movetype;
	short physics;
	short nAnim;
	//optional param
	XYVALUE Velset;
	short bCtrl;
	short nPoweradd;
	short nJuggle;
	bool bFacep2;
	///Some  flags
    bool bHitdefpersist;
	bool bMovehitpersist;
	bool bHitcountpersist;
	//////////////////////
	short nSprpriority;
	
	//Link to the next member
	PLSTATEDEF *lpNext;
	//Link to the State member
	PLSTATE *lpLink;

};

//Runtime data of the player
/*
    nID            is the ID number of the player
    x,y            give the cordinate of the players axis
    nAnime         is the current animation number in which the player is
    nState         is the current state number in which the player is
    xvel,yvel      is the current velocity of the player
    bCtrl          has the Player the control(true:false)
    nLife          is the life power of the player
    nLevelPower    contains the current special power level for special tricks
    bAlive         is the player alive or not (true=alive;flase=dead)
    bFacing        gives the facing direction of the player(true=right;flase=left)

*/
struct PlRunTimeInfo
{
    //Current Position
    short nID;
    float x,y;
    short nAnime;
    short nState;
    short nStateTyp;
    short nMoveType;
    short nStateTime;
    short nPrevState;
    float xvel,yvel;
    bool bCtrl;
    short nLife;
    short nLevelPower;
    short nJuggle;
    short nPhysic;
    float nP2Distance;   
    bool bAlive;
    bool bFacing;
    bool bAutoTurning;
    Uint16 nAnimFlag;


};

#define XROOT 320/2

//Statedef Linked List Struct
struct StatedefLinkedList
{
	PLSTATEDEF StateDefData;
	StatedefLinkedList *VorGeanger;
	StatedefLinkedList *NachFolger;

};

#endif
