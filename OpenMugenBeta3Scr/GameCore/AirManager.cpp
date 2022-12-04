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

#include "AirManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "Compiler.h"



CAirManager::CAirManager()
{
    pTempList=NULL;
    nCurrentElement=0;
    nCurrentClsn=0;
    bClsnDefault=false;

}

CAirManager::~CAirManager()
{
}

void CAirManager::Reset()
{
    pTempList=NULL;
    nCurrentElement=0;
    nCurrentClsn=0;
    bClsnDefault=false;

}

void CAirManager::AddAction(int nActionNumber)
{
    nHowManyBlocks++;
    //This reallocates all the memory from the previos Action block
    //but the last block is ignored
    RealocateActionBlock();
 
    //PrintMessage("AddAction");
     //Reset Variables for new element
    nCurrentElement=0;
    nCurrentClsn=0;
    nCurrentClsn2=0;
    pTempList=NULL;
    pTempClsn=NULL;
    bClsnDefault=false;
    
    //PrintMessage("Action %i",nActionNumber);
    
    pTempList=new AirLinkedList;
    
    //Set all to Zero
    memset(pTempList,0,sizeof(AirLinkedList));
    pTempList->ElementData.loopStart=-1;
    pTempList->ElementData.nActionNumber=nActionNumber;
    
    //Allocate 100 Elemant and then realc it 
    pTempList->ElementData.AnimationElement= (Element*) malloc(sizeof(Element)* 100);
    
    if(pTempList->ElementData.AnimationElement==NULL)
        PrintMessage("Cannot allocate Memory for Airmanager");
    
   //Allocate 100 Clsn boxes for evry Element
    for(int i=0;i<100;i++)
        pTempList->ElementData.AnimationElement[i].pClnsData=(Clsn*) malloc(sizeof(Clsn)*100);
        
    
    m_AirList.Hinzufuegen(pTempList);
}
//Reallocates unessery memory
void CAirManager::RealocateActionBlock()
{
    Clsn *pTemp;
  

    if(pTempList!=NULL && nCurrentElement!=0 )
    {
        for(int i=nCurrentElement;i<100;i++)
         {
          pTemp=pTempList->ElementData.AnimationElement[i].pClnsData;
          free(pTemp);
          pTemp=NULL; 
         
         }
         pTempList->ElementData.AnimationElement= (Element*) realloc(pTempList->ElementData.AnimationElement,sizeof(Element)*(nCurrentElement));
       
    }

}

void CAirManager::AddElement(int nGroupNumber,int nImageNumber,int x,int y,
                             int nDuringTime,Uint16 nFlip,Uint16 nColorFlag)
{
   
    //if we have more then 100 elements reallocate 100 elements to it
    if(nCurrentElement==100)
    {
       pTempList->ElementData.AnimationElement=(Element*) realloc(pTempList->ElementData.AnimationElement,sizeof(Element)*200);
        for(int i=100;i<200;i++)
        pTempList->ElementData.AnimationElement[i].pClnsData=(Clsn*) malloc(sizeof(Clsn)*100);
       
       PrintMessage("temp=%x",pTempList);    
    }
    
    // PrintMessage("%i,%i,%i,%i,%i", nGroupNumber,nImageNumber,x,y,nDuringTime);
     
    //Fill the element with the Values
    pTempList->ElementData.AnimationElement[nCurrentElement].nGroupNumber=nGroupNumber;
    pTempList->ElementData.AnimationElement[nCurrentElement].nImageNumber=nImageNumber;
    pTempList->ElementData.AnimationElement[nCurrentElement].x=x;
    pTempList->ElementData.AnimationElement[nCurrentElement].y=y;
    pTempList->ElementData.AnimationElement[nCurrentElement].nNumberOfClsn=nCurrentClsn;
    pTempList->ElementData.AnimationElement[nCurrentElement].nDuringTime=nDuringTime;
    pTempList->ElementData.AnimationElement[nCurrentElement].ColorFlags=nColorFlag;
    pTempList->ElementData.AnimationElement[nCurrentElement].FlipFlags=nFlip;
    pTempList->ElementData.nNumberOfElements=nCurrentElement;
    //calculate the time
    pTempList->ElementData.dwCompletAnimTime+=nDuringTime;
       
    if(bClsnDefault)
    {
          memcpy(pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData,pTempClsn,sizeof(Clsn)*nCurrentClsn2);
          pTempList->ElementData.AnimationElement[nCurrentElement].nNumberOfClsn=nCurrentClsn2;
          //PrintMessage("In Action %i",pTempList->ElementData.nActionNumber);
    } 


    //Now realloc the 100 Clsn element to to the right amount of it
    if(nCurrentClsn!=0 && !bClsnDefault )
    {
        pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData=(Clsn*)realloc(pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData,sizeof(Clsn)*(nCurrentClsn));
    }
    
    if(nCurrentClsn!=0 && bClsnDefault )
    {
        pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData=(Clsn*)realloc(pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData,sizeof(Clsn)*(nCurrentClsn2));
    }
    
    if(nCurrentClsn==0 && !bClsnDefault)
    {
       free(pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData);
       pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData=NULL;
    }
       
    //End of realocation stuff
    
    //reset Clsn counter            
    nCurrentClsn=0;  
    nCurrentElement++;

}

void CAirManager::SetLoop()
{
    pTempList->ElementData.loopStart=nCurrentElement;

}
//to add a Clsn box 
void CAirManager::AddClsnBox(short x,short y,short w,short h,int nNumberOfClsn)
{
    //Error handling
    if(nNumberOfClsn!=nCurrentClsn2)
    {
        PrintMessage("CAirManager::AddClsnBox Error Index of clsn is not equal to current index (%i!=%i)",nNumberOfClsn,nCurrentClsn);
        PrintMessage("Number of total Clsn boxes %i",nHowManyClsn);
    }

    pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData[nCurrentClsn].bClsn1=bClsn1or2; 
    pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData[nCurrentClsn].ClsnRect.x=x;
    pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData[nCurrentClsn].ClsnRect.y=y;
    pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData[nCurrentClsn].ClsnRect.w=w;
    pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData[nCurrentClsn].ClsnRect.h=h;

    if(bClsnDefault)
    {
           pTempClsn=pTempList->ElementData.AnimationElement[nCurrentElement].pClnsData;
//           PrintMessage("In Action %i",pTempList->ElementData.nActionNumber);
           
    }
    
 
    nCurrentClsn2++;
    nCurrentClsn++;   

}

void CAirManager::CreateClsnBox(int nNumberOfClsn,bool bClsn1,bool bDefault)
{
   // nCurrentClsn=0;
    nCurrentClsn2=0;
    nHowManyClsn=nNumberOfClsn;
    bClsnDefault=bDefault;
    bClsn1or2=bClsn1;    
    
   // PrintMessage("CreateClnsBox NumberOfClns=%i bClsn1 %i bDefault=%i",nNumberOfClsn,bClsn1,bDefault);

}

ActionElement* CAirManager::GetAction(int nActionNumber)
{
	m_AirList.SetStart();
	char str[255];

	while(m_AirList.GetAirData())
	{

		if(m_AirList.GetAirData()->ElementData.nActionNumber==nActionNumber)
			return &m_AirList.GetAirData()->ElementData;

		m_AirList.Next();

	}

	PrintMessage("%i Action number not found",nActionNumber);
	
    return NULL;


}

void CAirManager::OpenAir(char *strFileName)
{
    SetAir(this);
    nHowManyBlocks=0;
        
    PrintMessage("CAirManager Loading %s",strFileName);
    PraseAir(strFileName);
    
    //now reallocate the last Actionblock
    RealocateActionBlock();
       
    PrintMessage("CAirManager %s has %i action ellements",strFileName,nHowManyBlocks);

}

void CAirManager::CleanUp()
{
    PrintMessage("CAirManager Cleaning air list");
	m_AirList.SetStart();

	if(m_AirList.Aktuell!=NULL)
	{
		while(m_AirList.Aktuell)
		{
	
    	if(m_AirList.GetAirData()->ElementData.AnimationElement!=NULL)
		{
	   	    if(m_AirList.GetAirData()->ElementData.AnimationElement->pClnsData!=NULL)
               delete m_AirList.GetAirData()->ElementData.AnimationElement->pClnsData;
         
        	delete m_AirList.GetAirData()->ElementData.AnimationElement;
        	
		}

			m_AirList.Next();

		}
		
	}
	Reset();
	m_AirList.CleanUpTheList();

}
