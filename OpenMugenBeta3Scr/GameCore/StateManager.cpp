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

#include "StateManager.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "global.h"



char *strOpCode[] =
{
    "OP_PUSH",
    "OP_POP",
    "OP_ADD",
    "OP_SUB",
    "OP_MUL",
    "OP_DIV",
    "OP_EQUAL",
    "OP_NOTEQUAL",
    "OP_LESS",
    "OP_GREATER",
    "OP_LESSEQUAL",
    "OP_GRAETEREQUAL",
    "OP_INTERVALOP1",
    "OP_INTERVALOP2",
    "OP_INTERVALOP3",
    "OP_INTERVALOP4",
    "OP_INTERVALOP5",
    "OP_INTERVALOP6",
    "OP_INTERVALOP7",
    "OP_INTERVALOP8",
    "OP_LOGNOT",
    "OP_LOGAND",
    "OP_LOGOR",
    "OP_LOGXOR",
    "OP_NOT",
    "OP_AND",
    "OP_OR",
    "OP_XOR",
    "OP_SQUARE",
    "OP_ABS",
    "OP_NEG",
    "OP_ACOS",
    "OP_ALIVE",
    "OP_ANIM",
    "OP_ANIMELEM",
    "OP_SAVE",
    "OP_RESTORE",
    "OP_ANIMELEMNO",
    "OP_ANIMELEMTIME",
    "OP_ANIMEXIST",
    "OP_ANIMTIME",
    "OP_ASIN",
    "OP_ATAN",
    "OP_AUTHORNAME",
    "OP_BACKEDGEBODY",
    "OP_BACKEDGE",
    "OP_CANRECOVER",
    "OP_CEIL",
    "OP_COMMAND",
    "OP_CONST",
    "OP_COS",
    "OP_CTRL",
    "OP_DRAWGAME",
    "OP_EXP",
    "OP_FACING",
    "OP_FLOOR",
    "OP_FEBD",
    "OP_FED",
    "OP_FVAR",
    "OP_GAMETIME",
    "OP_GETHITVAR",
    "OP_HITCOUNT",
    "OP_HITFALL",
    "OP_HITOVER",
    "OP_HITPAUSETIME",
    "OP_HITSHAKEOVER",
    "OP_HITVAL",
    "OP_ID",
    "OP_IFELSE",
    "OP_INGUARDDIST",
    "OP_ISHELPER",
    "OP_ISHOMETEAM",
    "OP_LIFE",
    "OP_LIFEMAX",
    "OP_LN",
    "OP_LOG",
    "OP_LOSE",
    "OP_MATCHNO",
    "OP_MATCHOVER",
    "OP_MOVECONTACT",
    "OP_MOVEGUARDED",
    "OP_MOVEHIT",
    "OP_MOVETYPE",
    "OP_MOVEREVERSED",
    "OP_NAME",
    "OP_NUMENEMY",
    "OP_NUMEXPLOD",
    "OP_NUMHELPER",
    "OP_NUMPARTNER",
    "OP_NUMPROJ",
    "OP_NUMPROJID",
    "OP_NUMTARGET",
    "OP_P2BODYDIST",
    "OP_P2DIST",
    "OP_P2LIFE",
    "OP_P2MOVETYPE",
    "OP_P2NAME",
    "OP_P2STATENO",
    "OP_P2STATETYPE",
    "OP_P3NAME",
    "OP_P4NAME",
    "OP_PALNO",
    "OP_PARENTDIST",
    "OP_PI",
    "OP_PLAYERIDEXIST",
    "OP_PREVSTATENO",
    "OP_POS",
    "OP_POWER",
    "OP_POWERMAX",
    "OP_PROJCANCTIME",
    "OP_RANDOM",
    "OP_ROOTDIST",
    "OP_ROUNDNO",
    "OP_ROUNDSEXISTED",
    "OP_ROUNDSTATE",
    "OP_SCREENPOS",
    "OP_SELFANIMEXIST",
    "OP_SIN",
    "OP_STAGENO",
    "OP_STATETYPE",
    "OP_SYSFVAR",
    "OP_SYSVAR",
    "OP_TAN",
    "OP_TEAMMODE",
    "OP_TEAMSIDE",
    "OP_TICKSPERSCOND",
    "OP_TIME",
    "OP_UNIQHITCOUNT",
    "OP_VAR",
    "OP_VEL",
    "OP_WIN",
    "OP_PROJCONTACT",
    "OP_PROJCONTACTTIME",
    "OP_PROJGUARDED",
    "OP_PROJGUARDEDTIME",
    "OP_STOP",
    "OP_MOD",
    "OP_NOP"

};

//Constructor
CStateManager::CStateManager()
{
	lpRootState=0;
	lpCurrentState=0;
	lpCurrent=0;
//	pInst=NULL;
	nCurrInst=0;
	nCurrTrigger=0;
	nCurrParam=0;
	nCurrParamInst=0;
	bInst=false;
	bParam=false;


}
//Destructor
CStateManager::~CStateManager()
{
    
}

//restet after clean up
void CStateManager::Reset()
{
  	lpRootState=0;
	lpCurrentState=0;
	lpCurrent=0;
//	pInst=NULL;
	nCurrInst=0;
	nCurrTrigger=0;
	nCurrParam=0;
	nCurrParamInst=0;
	bInst=false;
	bParam=false;

}

//Add a new statedef in the list
void CStateManager::AddStateDef(short nStateDefNum)
{
#ifdef DEBUG
    PrintMessage("CStateManager Add Statedef %i",nStateDefNum);
#endif
	//Restet Variables
	nCurrTrigger=0;
	nCurrInst=0;

	
	if(lpRootState==0)
	{
		lpRootState= new PLSTATEDEF;
		memset(lpRootState,0,sizeof(PLSTATEDEF));
		lpRootState->StateNumber=nStateDefNum;
		lpCurrentState=lpRootState;

	}
	else
	{
		lpCurrentState->lpNext=new PLSTATEDEF;
		lpCurrentState=lpCurrentState->lpNext;
		memset(lpCurrentState,0,sizeof(PLSTATEDEF));
		lpCurrentState->StateNumber=nStateDefNum;

	}

}
//Sets default values to the Statedef
void CStateManager::SetDefaultStatedef()
{
    lpCurrentState->type        =STSTAND;
    lpCurrentState->movetype    =MOVIDLE;
    lpCurrentState->physics     =PHNONE;
    lpCurrentState->nAnim       =-1; 
    lpCurrentState->Velset.x    = 0;
    lpCurrentState->Velset.y    = 0;
    lpCurrentState->bCtrl       =-1; 
    lpCurrentState->nPoweradd   = 0;
    lpCurrentState->nJuggle     = 0;
    lpCurrentState->bFacep2             =false;
    lpCurrentState->bHitcountpersist    =false;
    lpCurrentState->bHitdefpersist      =false;
    lpCurrentState->bMovehitpersist     =false;
    lpCurrentState->nSprpriority        =-1;
    
}

//Add a State tothe current Statedef
void CStateManager::AddState(short nStateNum,char* strSomeNumber)
{
#ifdef DEBUG
    PrintMessage("CStateManager::AddState %i,%s",nStateNum,strSomeNumber);
#endif
    
    //restet variable
    nCurrParam=0;   
    nCurrParamInst=0; 
    nCurrTrigger=0;
   
	
    if(lpCurrentState->lpLink==0)
	{
		lpCurrentState->lpLink=new PLSTATE;
	//	memset(lpCurrentState->lpLink,0,sizeof(PLSTATE));
		
		lpCurrentState->lpLink->nStateNumber=nStateNum;
		strcpy(lpCurrentState->lpLink->strSomeNumber,strSomeNumber);
		lpCurrent=lpCurrentState->lpLink;
		lpCurrent->lpNext=NULL;

	}
	else
	{
		lpCurrent->lpNext=new PLSTATE;
	//	memset(lpCurrent->lpNext,0,sizeof(PLSTATE));
		lpCurrent->lpNext->nStateNumber=nStateNum;
		memcpy(lpCurrentState->lpLink->strSomeNumber,strSomeNumber,strlen(strSomeNumber));
		lpCurrent=lpCurrent->lpNext;
    	lpCurrent->lpNext=NULL;

	}
	//allocate triggers for the state
	lpCurrent->triggers=(PLTRIGGER*)malloc(sizeof(PLTRIGGER)*50);

	if(lpCurrentState->lpLink->triggers==NULL)
	   PrintMessage("CStateManager::AddState cannot allocate triggers for state");

	memset(lpCurrent->params,0,sizeof(lpCurrent->params));
	pInst[0].n_OpCode=OP_NOP;

}
//Now save the controller type wich would be executeted
void CStateManager::AddTypeToState(short nType)
{
#ifdef DEBUG
    PrintMessage("Add Type to state %i",nType);
#endif
	lpCurrent->nType=nType;
	bInst=true;

}

void* CStateManager::GetController()
{
	return NULL;
	//	return lpCurrent->lpController;
}

//Cleans up ;-)
void CStateManager::CleanUp()
{
	PLSTATEDEF* temp;
	PLSTATE* lpState=NULL;
	lpCurrentState=lpRootState;
	
	int j=0;
	
    PrintMessage("CStateManager::CleanUp") ;

	while(lpCurrentState)
	{
		temp=lpCurrentState->lpNext;
		
		while(lpCurrentState->lpLink)
		{
		  lpState=lpCurrentState->lpLink->lpNext;

		  //Clear the instruction in each trigger and the trigger it self
          for(int i=0;i<lpCurrentState->lpLink->nHowManyTriggers;i++)
		  {
		      //first clear posible strings in the instruction
		      while(lpCurrentState->lpLink->triggers[i].pInts[j].n_OpCode!=OP_STOP)
		      {
		            if(lpCurrentState->lpLink->triggers[i].pInts[j].strValue!=NULL)
                       delete[] lpCurrentState->lpLink->triggers[i].pInts[j].strValue;
                    j++;       
        	  }
              j=0;        	            
		      free(lpCurrentState->lpLink->triggers[i].pInts);
          }
          //now clear the triggers
          free(lpCurrentState->lpLink->triggers);

          delete lpCurrentState->lpLink;
		  lpCurrentState->lpLink=lpState;
		  
		}
        
        
        delete lpCurrentState;
		lpCurrentState =temp;
		

	}
	Reset();

}
//Add Instruction to currrent trigger
void CStateManager::AddInstruction(Uint16 nOpCode,float value,char *strValue)
{
    //we add a instruction to a trigger
    if(bInst)
    {
#ifdef DEBUG
         PrintMessage("AddInstruction = %s %f %s",strOpCode[nOpCode-20],value,strValue);
#endif     
          pInst[nCurrInst].n_OpCode=nOpCode;
          pInst[nCurrInst].Value=value;
          pInst[nCurrInst].strValue=NULL;
          
          //only add a string if we need one
          if(strValue[0]!='#')
          {
                 pInst[nCurrInst].strValue=new char[strlen(strValue)+1];   
                 strcpy(pInst[nCurrInst].strValue,strValue);
          }
             
         nCurrInst++;
    }


}
//Increase the index of the current trigger ref
void CStateManager::AddTriggerToState(int nType)
{   
    int i,j;

        
    if(nCurrTrigger > NUMTRIGGER)
        PrintMessage("CStateManager::AddTriggerToState current trigger is bigger then NUMTRIGER" );
        
    //First lets copy the instruction to the trigger
    //Create a new instance to store the instruction
    lpCurrent->triggers[nCurrTrigger].pInts=(INSTRUCTION*)malloc(sizeof(INSTRUCTION)*nCurrInst);
    
    memcpy(lpCurrent->triggers[nCurrTrigger].pInts,&pInst[0],sizeof(INSTRUCTION)*nCurrInst);
    
    lpCurrent->triggers[nCurrTrigger].nTriggerType=nType;
    
    
    //check a statement like 
    //trigger1 = expression1
    //trigger1 = expression2
    if(nCurrTrigger > 0)
    {
       if(lpCurrent->triggers[nCurrTrigger].nTriggerType==
          lpCurrent->triggers[nCurrTrigger-1].nTriggerType)
       {
          i=0;
          j=0;    
          //go to the end of the instruction from prev trigger    
          while(lpCurrent->triggers[nCurrTrigger-1].pInts[i].n_OpCode!=OP_STOP)
                i++;
          //now make the OP_STOP inst at the end to a OP_NOP        
          lpCurrent->triggers[nCurrTrigger-1].pInts[i].n_OpCode=OP_NOP;
         
          //now go to the end of the instruction from the current trigger
          while(lpCurrent->triggers[nCurrTrigger].pInts[j].n_OpCode!=OP_STOP)
            j++;        
            
         //now realocate the intruction of the prev trigger i+j+1 for an OP_LOG instruction
         lpCurrent->triggers[nCurrTrigger-1].pInts=(INSTRUCTION*)realloc(lpCurrent->triggers[nCurrTrigger-1].pInts,
                                                                         sizeof(INSTRUCTION)*(i+j+2));                                          
                                                                         
         //combine them now
         memcpy(&lpCurrent->triggers[nCurrTrigger-1].pInts[i],
                 lpCurrent->triggers[nCurrTrigger].pInts,
                 sizeof(INSTRUCTION)*j);
               
         lpCurrent->triggers[nCurrTrigger-1].pInts[i+j].n_OpCode=OP_LOGAND;
         lpCurrent->triggers[nCurrTrigger-1].pInts[i+j+1].n_OpCode=OP_STOP;
         
         
         //clear the current trigger 
         free(lpCurrent->triggers[nCurrTrigger].pInts);
         
         nCurrTrigger--;
         
       
       }
    
    }
#ifdef DEBUG       
    PrintMessage("trigger %i",nType);
#endif       
    nCurrTrigger++;
    
    //reallocate to save memory
    lpCurrent->triggers=(PLTRIGGER*)realloc(lpCurrent->triggers,sizeof(PLTRIGGER)*(nCurrTrigger+1));
    //save how many triggers we have to check
    lpCurrent->nHowManyTriggers=nCurrTrigger;
    nCurrInst=0;
}


//set the param value
void CStateManager::SetParam(int nParam)
{
    if(!bInst)
        return;

    if(nCurrParam > PARAMS)
        PrintMessage("CStateManager::SetParam:Error More then 20 parameters");

#ifdef DEBUG
    PrintMessage("SetParam(%i)",nParam);
#endif

    lpCurrent->params[nCurrParam].nParam=nParam;
    //Now copy the instruction to the param
    memcpy(&lpCurrent->params[nCurrParam].pInts[0],&pInst[0],sizeof(INSTRUCTION)*INSPERTRIGGER);
    
    
    nCurrInst=0;
    nCurrParam++;

}
//search for the given state and return it
PLSTATEDEF* CStateManager::GetStateDef(int nStateNumber)
{
    lpCurrentState=lpRootState;
    
    while(lpCurrentState)
    {
        if(lpCurrentState->StateNumber==nStateNumber)
           return lpCurrentState;
        
        lpCurrentState=lpCurrentState->lpNext;
        
    }
    PrintMessage("*****Statedef with number %i not found******",nStateNumber);
    
    return NULL;

}

void CStateManager::ExchangeIns()
{
    INSTRUCTION temp;
    
    memcpy(&temp,&pInst[nCurrInst-1],sizeof(temp));
 
    memcpy(&pInst[nCurrInst-1],&pInst[nCurrInst-2],sizeof(temp));
    memcpy(&pInst[nCurrInst-2],&temp,sizeof(temp));

 //   PrintMessage("Exchange");
  
    

}


void CStateManager::SetVelSet(float x,float y)
{
	lpCurrentState->Velset.x=x;
	lpCurrentState->Velset.y=y;

}
