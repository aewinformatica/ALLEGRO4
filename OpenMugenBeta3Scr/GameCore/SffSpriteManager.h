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

#ifndef SFFMANAGER_H
#define SFFMANAGER_H

#include <stdio.h>
#include "Global.h"
#include "SffSpriteList.h"
#include "AirManager.h"
#include "VideoSystem.h"
#include <string.h>
#include <SDL.H>

class CSffSpriteManager
{
//Public Funktions
public:
	CSffSpriteManager();
	~CSffSpriteManager();
	void InitSpriteManger(char* strFileName);
	void ClearUpSpriteManger();
	SFFSPRITE GetSffSprite(short nGroupNumber,short nImageNumber);
	CSffSpriteList SffSpriteList;
	void SetColorPallet(char* strFileName);
	//Blit functions
    void BltSprite(short nGroupNumber,short nImageNumber,short x,short y,short nMask,SDL_Rect *r,Uint32 nFlags);
	void BlitAnim(short x,short y,short nActionNumber,Uint32 nFlags);
    
    void SetAirManager(CAirManager *p){m_pAirManager=p;}
	void SetVideoSystem(CVideoSystem *p){m_pVideoSystem=p;}
	void SetGameTicks(short nTicks){nGameTicks=nTicks;}
	void SetID(int n){nID=n;}
	void ResetAnim(int nAnim);
	void PrepareAnim(int nAnim);
//Set functions
    void SetFacing(bool bRightorLeft){bFacing=bRightorLeft;}
//Helper Function
    Uint16 GetFlags(Uint16 nFlags);
    Uint16 SetFlags(Uint16 nGetFlag,Uint16 nCurrFlag);
	
//Protected Variables
protected:
	FILE *SffFile;
	PcxPallet ColorPallet[256];
	short nTotalImages;
	CAirManager* m_pAirManager;
	CVideoSystem* m_pVideoSystem;
	short nGameTicks;
	short nCurrentImage;
	DWORD dwStartTime;
	int nID;
	bool bPalletLoaded;
	bool bFacing;
	

public:
   void	DecodePcxFromSff(FILE *SffFile,short nImages);
   BYTE * DecodePcxFile(BYTE *byPcx,PCXHEADER *PcxHeader,PcxPallet *ColorPallet,bool bPalletSame);
   bool bShowClsn;
  
  

	
};

#endif
