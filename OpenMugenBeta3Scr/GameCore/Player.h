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



#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"
#include "SffSpriteManager.h"
#include "AirManager.h"
#include "VideoSystem.h"
#include "StateManager.h"
#include "VM.h"
#include "ControllerExecuter.h"

#define KEYBUFFER 60

/*
    The main class of an player 
*/
class CPlayer
{
public:
    CPlayer();
    ~CPlayer();
    
//Load
    void LoadPlayer(char *strPlayerDef);
//Set functions
    void SetID(short n){nID=n;}
    void SetVideoSystem(CVideoSystem *p){m_pVideoSystem=p;}
    void ClearPlayer();
    void DebugInfo();
    void SetDebug(){bDebug=true;}
//Update Functions
    void UpDatePlayer();
    void UpDatePlayerStates();
    void UpDatePlayerVel();
    void UpDateFacing();
///////////////////////////////
    void DrawPlayer();
    void SetPos(int x,int y);
    void SetPlayer2(CPlayer *p2);
    void ExecuteState();
    bool CheckTriggers();
    void ExecuteController();
    void ResetPlayer();
//Player Controllers
    void ChangeAnim(int nAnimToChange);
    void ChangeState(int nStateToChange);
    
//Input Functions
    void CheckInput();
    void GetInput();


//Persist Varibales Functions
    float GetFVar(int index)                {return FloatPersist[index];}
    void SetFVar(int index,float value)     {FloatPersist[index]=value;}
    float GetIVar(int index)                {return IntPersist[index];}
    void SetIVar(int index,float value)     {IntPersist[index]=value;}
//Get Functions
    float GetConst(int index);
    float GetParamValue(int nParam);
    
//Debug function
    void DisplayDebugData();


//All needed Classes
     CSffSpriteManager     m_SffManager;
     CAirManager           m_AirManager;
     CStateManager         m_StateManager;
     CVideoSystem         *m_pVideoSystem;
     CVirtualMachine       m_VM;
     CControllerExecuter   m_ControllerExec;  
     
//Pointer to Player2
CPlayer *m_pPlayer2;

//Pointer to his current statedef
PLSTATEDEF* lpCurrentStateDef;
//Pointer to current state in the statedef
PLSTATE *tempState;
     
//Player Constants
PLAYERCONST myPlayerConst;
//Def Filr data myPlayerData
PLAYERDATA myPlayerData;
//Player Runtime info
PlRunTimeInfo PlRtInfo;
     
//Variables
short nID;
float IntPersist[60];
float FloatPersist[40];
//saves the current command name
char strCommand[50];
//Variable of the current Input works also with Joystick
KEYBOARDDATA *pKey;
//this is the Keyboard buffer
KEYELEMENT KeyBuffer[KEYBUFFER];
short nKeyIndex;
short nMaxKeyBuffer;
short nMaxCommandTime;
short nCommandTime;

//helper Varaibles
bool bDebug;



};


#endif
