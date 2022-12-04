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

#ifndef _AIRMANAGER_H_
#define _AIRMANAGER_H_


#include "Global.h"
#include "AirList.h"

class CAirManager
{
public:
	CAirManager();
	~CAirManager();

public:
   void AddAction(int nActionNumber);
   void AddElement(int nGroupNumber,int nImageNumber,int x,int y,int nDuringTime,Uint16 nFlip,Uint16 nColorFlag);
   void SetLoop();
   void AddClsnBox(short x,short y,short w,short h,int nNumberOfClsn);
   void CreateClsnBox(int nNumberOfClsn,bool bClsn1,bool bDefault);
   void OpenAir(char *strFileName);
   ActionElement* GetAction(int nActionNumber);
   void CleanUp();
   void RealocateActionBlock();
   void Reset();


public:
	CAirList m_AirList;
	AirLinkedList *pTempList;
	int nCurrentElement;
	int nCurrentClsn;
	int nCurrentClsn2;
	int nHowManyClsn;
	int nHowManyBlocks;
	Clsn *pTempClsn;
	bool bClsnDefault;
	bool bClsn1or2;



};
#endif
