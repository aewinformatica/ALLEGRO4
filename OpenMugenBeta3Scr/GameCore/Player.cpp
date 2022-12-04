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

#include "VM.h"
#include "Player.h"

#include "Compiler.h"

//Constructor of the Player init all the variables to zero
CPlayer::CPlayer()
{
    memset(&myPlayerConst,0,sizeof(myPlayerConst));
    memset(&myPlayerData,0,sizeof(myPlayerData));
    memset(&PlRtInfo,0,sizeof(PlRtInfo));
    memset(IntPersist,0,sizeof(IntPersist));
    memset(FloatPersist,0,sizeof(FloatPersist));
    memset(&KeyBuffer,0,sizeof(KeyBuffer));
    bDebug=false;
    
    //input stuff
    nKeyIndex=0;
    nMaxKeyBuffer=KEYBUFFER;
    pKey=NULL;
    
    nMaxCommandTime=15;
    nCommandTime=0;

    
    m_pPlayer2=NULL;
  

}


CPlayer::~CPlayer()
{
    ClearPlayer();
}

void CPlayer::LoadPlayer(char *strPlayerDef)
{
    char strPath[255];
    char strTemp[255];
    
    PrintMessage("CPLayer Loading %s",strPlayerDef);
    
    //Set All the pointers for the Parser
    SetPlayer(this);
    SetStateManager(&m_StateManager);
    
  /*  ParseFile(strPlayerDef);
    
    PrintMessage("CPLayer Player`s Name: %s",myPlayerData.strName);
    PrintMessage("CPLayer Player`s Display Name: %s",myPlayerData.strDisplayName);
    PrintMessage("CPLayer Author of Player: %s",myPlayerData.strAuthor);
          
    //Extract the Charackter path
    strcpy(strPath,strPlayerDef);
    ExtractPath(strPath);
    
    //Now complet the file paths for the files
    sprintf(strTemp,"%s%s",strPath,myPlayerData.strAirFile);   
    strcpy(myPlayerData.strAirFile,strTemp);

    sprintf(strTemp,"%s%s",strPath,myPlayerData.strSffFile);   
    strcpy(myPlayerData.strSffFile,strTemp);
    
    sprintf(strTemp,"%s%s",strPath,myPlayerData.strCmdFile);   
    strcpy(myPlayerData.strCmdFile,strTemp);
    
    sprintf(strTemp,"%s%s",strPath,myPlayerData.strSndFile);   
    strcpy(myPlayerData.strSndFile,strTemp);
    
    sprintf(strTemp,"%s%s",strPath,myPlayerData.strPalFile[0]);   
    strcpy(myPlayerData.strPalFile[0],strTemp);
    
  
 */ m_SffManager.SetID(nID);
    m_SffManager.SetVideoSystem(m_pVideoSystem);
    m_SffManager.InitSpriteManger("chars\\kfm\\kfm.SFF"/*myPlayerData.strSffFile*/);
    m_SffManager.SetAirManager(&m_AirManager);
    m_SffManager.SetGameTicks(60);
	m_AirManager.OpenAir("chars\\kfm\\kfm.AIR"/*myPlayerData.strAirFile*/);
	
	 //Get the Consatnt part of the cns file
    ParseConstantsFromCns("chars\\kfm\\kfm.cns");
  
    PrintMessage("Reading the common1.cns file...");
    //Fist get the statesdef's from the Common1.cns
    ParseFile("data\\common1.cns");
    //and now the players cns files
    ParseFile("chars\\kfm\\kfm.cns");
    
    DebugInfo();
    
    lpCurrentStateDef=m_StateManager.GetStateDef(20);

    
    ResetPlayer();
    
    
    //Set the pointer of the Player
    m_ControllerExec.SetPlayer(this);
    
   
}

//reste the player to his default values(after a finsihed round)
void CPlayer::ResetPlayer()
{
    PlRtInfo.xvel=0;
    PlRtInfo.nP2Distance=0;
    PlRtInfo.nAnime=0;
    PlRtInfo.bAlive=true;
    PlRtInfo.nID=33;
    PlRtInfo.nLife=myPlayerConst.PlayerData.nLife;
    PlRtInfo.nState=170;
    PlRtInfo.nAnimFlag=BLT_NORMAL;
    PlRtInfo.bAutoTurning=true;
    PlRtInfo.bFacing=true;
    PlRtInfo.nStateTyp=STSTAND;
    
    this->m_SffManager.PrepareAnim(0);
    

}

//Debug info
void CPlayer::DebugInfo()
{
    PrintMessage("*****Show Player Const******");
    PrintMessage("Life\t\t %i",myPlayerConst.PlayerData.nLife);
    PrintMessage("Defence\t\t %i",myPlayerConst.PlayerData.nDefence);
    PrintMessage("Attack\t\t %i",myPlayerConst.PlayerData.nAttack);
    PrintMessage("PresistIndex\t\t %i",myPlayerConst.PlayerData.nIntPersistIndex);
    PrintMessage("Velocity forward\t\t %f",myPlayerConst.PlayerVelocity.nWalkFwd);
    PrintMessage("Velocity backward\t\t %f",myPlayerConst.PlayerVelocity.nWalkBack);
    PrintMessage("Player Width\t\t %i",myPlayerConst.PlayerSize.nGroundBack+myPlayerConst.PlayerSize.nGroundFront);
    PrintMessage("******End of Show***********");
    

}

//Updates all the interna of the Player
void CPlayer::UpDatePlayer()
{
    //First Update some internel player stuff
    UpDatePlayerStates();
    //UpDate Velocity of the player
    UpDatePlayerVel();    
    //Executes the statedef block
    ExecuteState();
    //increase Statetime by One Game tick
    PlRtInfo.nStateTime++;
    
  
    
    
}

void CPlayer::UpDatePlayerVel()
{
     PlRtInfo.y+=PlRtInfo.yvel;
    //true = right , false = left
    if(PlRtInfo.bFacing)
      PlRtInfo.x+=PlRtInfo.xvel;
    else  
     PlRtInfo.x-=PlRtInfo.xvel;
}

//Update some player internel stuff
void CPlayer::UpDatePlayerStates()
{
    strcpy(strCommand,"none"); 
    //First check input;
    GetInput();
    CheckInput();
    
    UpDateFacing();
    
    if(strcmp(strCommand,"none")==0 && lpCurrentStateDef->StateNumber != 0)
    {
        ChangeState(0);
    }
    

    
}

void CPlayer::UpDateFacing()
{
    //true = right facing false = left facing
    if(PlRtInfo.bFacing)
       PlRtInfo.nP2Distance=m_pPlayer2->PlRtInfo.x-PlRtInfo.x;
    else
       PlRtInfo.nP2Distance=PlRtInfo.x-m_pPlayer2->PlRtInfo.x;
    
    //check the players facing direction
    //if player2 is in back of player1 then turn him
    if(PlRtInfo.nP2Distance < 0 && PlRtInfo.bFacing && PlRtInfo.bAutoTurning)
    {
      PlRtInfo.nAnimFlag=BLT_NORMALFLIPH;
      //Only change the anim if it is the statedef 0
      if(lpCurrentStateDef->StateNumber==0)
      {
         ChangeAnim(5);
      }
      m_SffManager.SetFacing(false);
      PlRtInfo.bFacing=false;  
    }
    else if(PlRtInfo.nP2Distance < 0 && !PlRtInfo.bFacing && PlRtInfo.bAutoTurning)
    {
       PlRtInfo.nAnimFlag=BLT_NORMAL;  
       
       if(lpCurrentStateDef->StateNumber==0)
      {
         ChangeAnim(5);
      }
       
      m_SffManager.SetFacing(true);
      PlRtInfo.bFacing=true; 
    
    }
    
   
}
//This executes the current Satedef Block and executes StateControllers if their triggers return true
void CPlayer::ExecuteState()
{
    tempState = lpCurrentStateDef->lpLink;
   
      // if(CheckTriggers())
      while(tempState)
      {      
        if(CheckTriggers())
           ExecuteController();
       
        tempState=tempState->lpNext;
      }
    
}

//Executes a controller of a State
void CPlayer::ExecuteController()
{
#ifdef DEBUG
    PrintMessage("Execute Controler");
#endif
    //executes the  controller of the current state
    m_ControllerExec.ExecuteController(tempState->nType);

}
//Get the Parameter from Controller
float CPlayer::GetParamValue(int nParam)
{
#ifdef DEBUG
   PrintMessage("GetParamValue(%i)",nParam); 
#endif
    for(int i=0 ;i < PARAMS;i++)
    {
        if(tempState->params[i].nParam==nParam)
          return m_VM.Execute(tempState->params[i].pInts);
    
    }
    //PrintMessage("CPlayer::GetParamValue(%i) failed",nParam);
    return NOPARAM;

}
//executes the trigger and chaecks
bool CPlayer::CheckTriggers()
{
    
    bool bTrigger=false;
    bool bTriggerAll=true;
  
    //check all the trigger
    for(int i=0; i < tempState->nHowManyTriggers; i++)
    {
        //first check the trigger for a trigger all
        if(tempState->triggers[i].nTriggerType==T_TRIGGERALL)
        {
             //if a triggerall return false then stop checking the other triggers
             if(m_VM.Execute(tempState->triggers[i].pInts) < 1)
                bTriggerAll=false;
           
              
        }
        else
        {
 
          if(m_VM.Execute(tempState->triggers[i].pInts) >= 1)
              bTrigger=true && bTriggerAll; 
           
         }
    
    }
    
    return  bTrigger;
    
}
//Set the Axis Position of the player
void CPlayer::SetPos(int x,int y)
{
    PlRtInfo.x=XROOT+x;
    PlRtInfo.y=y;

}
//Draws the Player to the screen
void CPlayer::DrawPlayer()
{
     m_SffManager.BlitAnim((int)PlRtInfo.x,(int)PlRtInfo.y,PlRtInfo.nAnime,PlRtInfo.nAnimFlag);
   
     if(bDebug)               
          DisplayDebugData();
   
}
//Clears all the Managerrs and allocated Memory
void CPlayer::ClearPlayer()
{
    m_SffManager.ClearUpSpriteManger();
    m_AirManager.CleanUp();
    m_StateManager.CleanUp();
   
}
//Sets the Pointer to the oppenent of player1 (hench player2)
void CPlayer::SetPlayer2(CPlayer* p2)
{
    if(p2==NULL)
        PrintMessage("CPlayer:SetPlayer 2 NULL");
    CPlayer *p1;
    p1=this;
    m_pPlayer2=p2;
    
    m_VM.SetPlayers((void*)p1,(void*)p2);
  

}

void CPlayer::DisplayDebugData()
{
     //m_pVideoSystem->DrawText(0,100,"Image %2i of %2i",tActionElement->nCurrentImage,tActionElement->nNumberOfElements);

     m_pVideoSystem->DrawText(0,20,"%3i/%3i AnimElemNo",m_AirManager.GetAction(PlRtInfo.nAnime)->nCurrentImage+1,m_AirManager.GetAction(PlRtInfo.nAnime)->nNumberOfElements+1);     
     m_pVideoSystem->DrawText(0,30,"%3i/%3i AnimTime",m_AirManager.GetAction(PlRtInfo.nAnime)->dwCurrTime,m_AirManager.GetAction(PlRtInfo.nAnime)->dwCompletAnimTime);
     m_pVideoSystem->DrawText(0,40,"X=%2f,Y=%2f",PlRtInfo.x-XROOT,PlRtInfo.y);
     m_pVideoSystem->DrawText(0,50,"State=%i",PlRtInfo.nState);
     m_pVideoSystem->DrawText(0,60,"ACTIONID=%i",PlRtInfo.nAnime);
     m_pVideoSystem->DrawText(0,70,"StateTime=%i",PlRtInfo.nStateTime);
     m_pVideoSystem->DrawText(0,80,"P2 Distance=%f",PlRtInfo.nP2Distance);
     m_pVideoSystem->DrawText(0,90,"StateType=%c",(char)PlRtInfo.nStateTyp);
    
    for(int i=0;i<KEYBUFFER;i++) 
    {   
     m_pVideoSystem->DrawText(i*20,100,"%i",KeyBuffer[i].nKey);
     
    }
                                                               
     m_pVideoSystem->DrawText(0,110,"Facing %i",this->PlRtInfo.bFacing);
     m_pVideoSystem->DrawText(0,130,"%s",strCommand);
}

//return the Constant data of the Player
float CPlayer::GetConst(int index)
{
    return 0;

}

//Input functions

//Check the input for valid input information and sets the strCommand variable to the command name
void CPlayer::CheckInput()
{
    PLCOMMAND CurrCommand;
    //save the time of the first and last key element
    Uint32 nTime,nLastTime;
    bool bCommand=false;
    int i=0;
    int j=0;
    int nNextEllement=0;
  
    strcpy(CurrCommand.strCommand,"holdfwd");
    CurrCommand.nCommandTime=15;
    CurrCommand.nHowManyCommand=3;
   
    CurrCommand.nCommand[0]=RIGHT+PRESSED+DOWN+PRESSED;
    CurrCommand.nCommand[1]=RIGHT+PRESSED;
    CurrCommand.nCommand[2]=BUTTONX+PRESSED;
//    CurrCommand.nCommand[3]=BUTTONX+PRESSED;
    
    
    if( RIGHT+PRESSED == KeyBuffer[KEYBUFFER-1].nKey)
    {
        if(this->lpCurrentStateDef->StateNumber != 20)
                this->ChangeState(20);
        strcpy(strCommand,"holdfwd"); 
    }
    
    if(LEFT+PRESSED == KeyBuffer[KEYBUFFER-1].nKey)
    {
      if(this->lpCurrentStateDef->StateNumber != 20)
                this->ChangeState(20);
                
      strcpy(strCommand,"holdback"); 
    }

    
    for(i=0;i<CurrCommand.nHowManyCommand;i++)
    {
      bCommand=false;
      for(j=nNextEllement;j<KEYBUFFER;j++)
      {

        //check if the elemt is in the buffer   
         if(KeyBuffer[j].nKey==CurrCommand.nCommand[i])
         {
       //    PrintMessage("%i==%i",KeyBuffer[j].nKey,CurrCommand.nCommand[i]);
           //if its the first element store the time of it
           if(i==0)
           {
            nTime=KeyBuffer[j].nTime;
           }
            
           if(i==CurrCommand.nHowManyCommand-1)
           {
             nLastTime=KeyBuffer[j].nTime;   
            }
           
           //the next search will be the element +1 wich we have found
           nNextEllement=j+1;
      
           //we found one element now skipp all the others in the buffer       
           bCommand=true;
           j=KEYBUFFER;
           
            
         }
              
      }
      if(!bCommand)
      {
          //Command failed so skip the rest  
          i=3;
      }
    
    }
    
    //Ok the Command sequenz is right but is it typed in the time for it?
    if(bCommand)
    {
      
      //the last button of the sequenz must be pressed int the Current game tick to
      //be valid and then it must be check for how long it has taken to do the input
       if(nLastTime==GetGameTicks() && (nLastTime-nTime) <= CurrCommand.nCommandTime )
       {
      //    PrintMessage("Lasttime=%i,CurrTime=%i",nLastTime,nTime);
       PrintMessage("COMMAND OK");
       }
     /* else
        PrintMessage("Command Failed"); */

    }
/*    else     
      PrintMessage("Command Failed");   */
      
    
}
//overwrites the first Key and addeds one ley to the end
void CPlayer::GetInput()
{

    //for exchange
    int nTemp;
    //if the char is turned change LEFT and RIGHT 
    //true = right(normal) false= left
    int nKey=pKey->nKey;
    
    if(!PlRtInfo.bFacing)
    {
        switch(nKey)
        {
          //reverse a right      
          case RIGHT+PRESSED:
             nKey=LEFT+PRESSED;       
          break;
          
          case RIGHT+RELEASED:
             nKey=LEFT+RELEASED;
          break;      
          
          //reverse a left
          case LEFT+PRESSED:
           nKey=RIGHT+PRESSED;
          break;
          
          case LEFT+RELEASED:
           nKey=RIGHT+RELEASED;         
          break;
        
        }
    
       
    }
   
   //shift all keys by one left
     for(int i=0;i<KEYBUFFER-1;i++)
     {
       KeyBuffer[i].nKey=KeyBuffer[i+1].nKey;
       KeyBuffer[i].nTime=KeyBuffer[i+1].nTime;
     }
      
     //Added new element to the end
     KeyBuffer[KEYBUFFER-1].nKey=nKey; 
     //store the current time
     KeyBuffer[KEYBUFFER-1].nTime=GetGameTicks();
          
  
}


//Player Controllers

//Change Anim Controller
void CPlayer::ChangeAnim(int nAnimToChange)
{
    //reset the curretn animation
    m_SffManager.ResetAnim(PlRtInfo.nAnime);
    //Prepare the next animation
    m_SffManager.PrepareAnim(nAnimToChange);
    
    PlRtInfo.nAnime=nAnimToChange;
    
#ifdef DEBUG
    PrintMessage("ChangeAnim(%i)",nAnimToChange);
#endif

}
//Change State Controller
void CPlayer::ChangeState(int nStateToChange)
{
      //save the prevSataNumber 
    PlRtInfo.nPrevState=lpCurrentStateDef->StateNumber;
    
    //Restet the state time
    PlRtInfo.nStateTime=0;

#ifdef DEBUG    
    PrintMessage("ChangeState(%i)",nStateToChange);
#endif
    //Change the state
    lpCurrentStateDef=m_StateManager.GetStateDef(nStateToChange);
    PlRtInfo.nState=nStateToChange;


    //include the params/////////////////////////////////////////////////////////////////////
    PlRtInfo.nStateTyp    =lpCurrentStateDef->type;
    PlRtInfo.nMoveType    =lpCurrentStateDef->movetype;
    PlRtInfo.nPhysic      =lpCurrentStateDef->physics;

    PlRtInfo.xvel      =lpCurrentStateDef->Velset.x;
    PlRtInfo.yvel      =lpCurrentStateDef->Velset.y;

    //Added power
    PlRtInfo.nLevelPower   +=lpCurrentStateDef->nPoweradd;

    PlRtInfo.nJuggle    =lpCurrentStateDef->nJuggle;

    PlRtInfo.nJuggle    =lpCurrentStateDef->nJuggle;

    if( lpCurrentStateDef->bCtrl!=-1)
       PlRtInfo.bCtrl =   lpCurrentStateDef->bCtrl;
       
    //Change Anim if nAnim!=-1;
    if( lpCurrentStateDef->nAnim != -1)
    {
        this->ChangeAnim(lpCurrentStateDef->nAnim);
    }
    //TODO:ADDED the lines
/*
    lpCurrentState->bFacep2             =false;
    lpCurrentState->bHitcountpersist    =false;
    lpCurrentState->bHitdefpersist      =false;
    lpCurrentState->bMovehitpersist     =false;
    lpCurrentState->nSprpriority        =-1;
*/

}
