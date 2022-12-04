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

#ifndef STATEMANAGER_H 
#define STAGEMANAGER_H

#include "Global.h"

#define S_type					2
#define S_movetype				4
#define S_physics				8
#define S_anime					10
#define S_ctrl					12
#define S_poweradd				14
#define S_juggle				16
#define S_hitdefpersist			18
#define S_movehitpresist		20
#define S_hitcountpresist		22
#define S_sprprio				24
#define S_facep2				26

class CStateManager
{
public:
	//Konstructor and Destructor
	CStateManager();
	~CStateManager();

	void AddStateDef(short nStateDefNum);
	void AddState(short nStateNum,char *strSomeNumber);
	void AddTriggerToState(int nType);
	void AddTypeToState(short nType);
	void AddInstruction(Uint16 nOpCode,float value,char *strValue);
	void ExchangeIns();
	void *GetController();
	void CleanUp();
	INSTRUCTION *GetInst(){return lpCurrentState->lpLink->triggers[0].pInts;};
	//search a State
	PLSTATEDEF* GetStateDef(int nStateNumber);
	PLSTATEDEF* GetCurrStateDef(){return lpCurrentState;}
	
	//SetFuction
	void SetVelSet(float x,float y);
	void SetInstAsParam(){bInst=false;bParam=true;};
	void SetInstAsInst(){bInst=true;bParam=false;};
	void DisableInst(){bParam=bInst=false;};
	void SetParam(int nParam);
	void SetDefaultStatedef();
	void Reset();

private:
	PLSTATEDEF* lpRootState;
	PLSTATEDEF* lpCurrentState;
	PLSTATE *lpCurrent;
	INSTRUCTION pInst[INSPERTRIGGER];
	int nCurrInst;
	int nCurrParamInst;
	int nCurrTrigger;
	int nCurrParam;
	bool bInst;
	bool bParam;



};

#endif
