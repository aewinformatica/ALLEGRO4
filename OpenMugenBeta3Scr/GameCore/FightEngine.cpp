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

#include "FightEngine.h"

CFightEngine::CFightEngine()
{

}

CFightEngine::~CFightEngine()
{
    delete lpPlayer1;
    delete lpPlayer2;
    //player2.ClearPlayer();
    
}

void CFightEngine::InitFightEngine(CVideoSystem *p)
{
    PrintMessage("CFightEngine:Init FightEngine");

    pVideo=p;
 
  lpPlayer1=new CPlayer();
  lpPlayer2=new CPlayer();
  
  lpPlayer1->SetID(IMP1) ;
  lpPlayer1->SetVideoSystem(p);
  lpPlayer1->SetPos(-70,200);
  lpPlayer1->SetDebug();
  
  PrintMessage("%x lpPlayer1",lpPlayer1);
  PrintMessage("%x lpPlayer2",lpPlayer2);
  lpPlayer1->SetPlayer2(lpPlayer2);
  
  lpPlayer2->SetID(IMP2) ;
  lpPlayer2->SetVideoSystem(p);
  lpPlayer2->SetPos(70,200);
  lpPlayer2->SetPlayer2(lpPlayer1);




}

void CFightEngine::RunEngine()
{
    //Update World
    //Draw BG
    //Update Player
    lpPlayer1->UpDatePlayer();
    lpPlayer2->UpDatePlayer();
    
    lpPlayer1->DrawPlayer();
    lpPlayer2->DrawPlayer();
    
    lpPlayer2->m_pVideoSystem->DrawText(0,120,"GameTicks=%i",GetGameTicks());


 
}
//Load Player1 ;-o
void CFightEngine::LoadPlayer1(char *strPlayer)
{
    PrintMessage("CFightEngine:Loading Player1(%s)",strPlayer);
    lpPlayer1->m_SffManager.SetColorPallet("chars\\kfm\\kfm.act");
    lpPlayer1->LoadPlayer(strPlayer);
    
    
}
//Load Player2  ;-o
void CFightEngine::LoadPlayer2(char *strPlayer)
{
    PrintMessage("CFightEngine:Loading Player2(%s)",strPlayer);
    
    lpPlayer2->m_SffManager.SetColorPallet("chars\\kfm\\kfm3.act");
    lpPlayer2->LoadPlayer(strPlayer);

}
