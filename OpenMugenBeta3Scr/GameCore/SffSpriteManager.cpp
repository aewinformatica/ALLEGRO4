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
#include "SffSpriteManager.h"


CSffSpriteManager::CSffSpriteManager()
{
	bPalletLoaded=false;
	bShowClsn=true;
	//right facing
	bFacing=true;

}

CSffSpriteManager::~CSffSpriteManager()
{
    // this->ClearUpSpriteManger();
	
}
//Loads the give Sff file and decode the images 
void CSffSpriteManager::InitSpriteManger(char* strFileName)
{
	//Local Variables
	SFFHEADER SffHeader;
	char strErrorMsg[255];
	short SingCheck=0;

	nCurrentImage=0;
	Uint32 startTime,endTime;

	//Strore the current time to calulate how much time the procedure takes
	startTime=SDL_GetTicks();
	PrintMessage("CSffSpriteManager Loading %s",strFileName);

	memset(&SffHeader,0,sizeof(SffHeader));

	SffFile=NULL;
	//Open the Sff file
	SffFile=fopen(strFileName,"rb");

	//if cant open the file then say it
	if(SffFile==NULL)
	{
	  PrintMessage("CSffSpriteManager %s File not found",strFileName);

	  return;

	}

	//read the header of the file
	fread(&SffHeader,sizeof(SffHeader),1,SffFile);

	//now check the File Signatur (i know there are better ways to do that)
	for(int i=0;i<12;i++)
		SingCheck+=SffHeader.signature[i];

	//if it is not an SFF file then say it and quit
	if(SingCheck!=1122)
	{
		PrintMessage("CSffSpriteManager Sff file Signature of %s is Invalid",strFileName);
		return;

	}
	//now move the filepointer to 1st Sub header
	fseek(SffFile,SffHeader.SubHeaderFileOffset,SEEK_SET);
	//this procedure reads all the subheaders and decodes the images
	DecodePcxFromSff(SffFile,SffHeader.NumberOfImage);
	//Strore the current time and caluate now how much time this has take
	endTime=SDL_GetTicks();
	PrintMessage("CSffSpriteManager Sff images loaded in %i milliseconds!",endTime-startTime);
	
	fclose(SffFile);
}
//Set the ColorPallet file the pcx images
void CSffSpriteManager::SetColorPallet(char* strFileName)
{
	FILE *Act=fopen(strFileName,"rb");

	if(Act==NULL)
	   	PrintMessage("CSffSpriteManager %s File not found",strFileName);
		

	for(int j=255;j>=0;j--)
	{
		ColorPallet[j].R=fgetc(Act);
		ColorPallet[j].G=fgetc(Act);
		ColorPallet[j].B=fgetc(Act);


	}

	bPalletLoaded=true;

	fclose(Act);

}
//The main Image decode function
void CSffSpriteManager::DecodePcxFromSff(FILE *SffFile,short nImages)
{
	//Sff flie Subheader struct	
	SFFSUBHEADER SffSubHeader;
	BYTE *		 tempByte;
	FILE *		pPcxFile=NULL;

	PCXHEADER PcxHeader;
	
	SFFSPRITE tempSffSprite;

	SffSpriteLinkedList* tempSffList;

	DWORD dwUseOfRam=0;
		
	nTotalImages=0;
	char str[255];

		
	memset(&SffSubHeader,0,sizeof(SffSubHeader));
	//memset(&ColorPallet,0,sizeof(ColorPallet));
	memset(&PcxHeader,0,sizeof(PcxHeader));
	memset(&tempSffSprite,0,sizeof(tempSffSprite));

	fread(&SffSubHeader,sizeof(SffSubHeader),1,SffFile);

	//File offset of the Next SubHeader is Null if it is the last?Is this right????

	//Loop till the file reaches the end
	while(!feof(SffFile))
	{
		//Create a new Linked list enterny
		tempSffList=new SffSpriteLinkedList;

		//fill it withe the right values
		tempSffSprite.x=SffSubHeader.x;
		tempSffSprite.y=SffSubHeader.y;
		tempSffSprite.ImageNumber=SffSubHeader.ImageNumber;
		tempSffSprite.GroupNumber=SffSubHeader.GroubNumber;

		//Read the pcx header
		fread(&PcxHeader,sizeof(PcxHeader),1,SffFile);

		//Correct the Image dimension 
		PcxHeader.widht=PcxHeader.widht - PcxHeader.x + 1;
		PcxHeader.height=PcxHeader.height - PcxHeader.y +1;

		//Copy the PCX header 
		tempSffSprite.PcxHeader=PcxHeader;
				
		//A littel trick to be sure that the data is an PCX
		if(PcxHeader.Manufacturer==10 && SffSubHeader.LenghtOfSubheader!=0)
		{
			//the pcx header is valid so it is not an linked image
			tempSffSprite.bIsLinked=false;
			//Allocate memory to save the image in to the memory
			tempByte = new BYTE[SffSubHeader.LenghtOfSubheader-128];
			//now read and copy the image in to the allocated memory
			fread(tempByte,SffSubHeader.LenghtOfSubheader-128,1,SffFile);
			//move the file pointer to Color pallet of the PCX file
			fseek(SffFile,-768L,SEEK_CUR);

			//Eat an Empty 8bit block
			fgetc(SffFile);
			
			//now check if it is nesecery to read the color table
			if(fgetc(SffFile)==12 && !SffSubHeader.PalletSame && PcxHeader.NPlanes<=1)
			{

				if(!bPalletLoaded)
				{
					for(int j=0;j<256;j++)
					{
						ColorPallet[j].R=fgetc(SffFile);
						ColorPallet[j].G=fgetc(SffFile);
						ColorPallet[j].B=fgetc(SffFile);
					}
				}
				//copy the color pallet to the Sffsprite Struct
				memcpy(&tempSffSprite.ColorPallet,&ColorPallet,sizeof(ColorPallet));

			}
			
			tempSffSprite.byPcxFile=tempByte;
			tempSffSprite.bPalletSame=SffSubHeader.PalletSame;
			tempSffList->SpriteData=tempSffSprite;
			SffSpriteList.Hinzufuegen(tempSffList);
			
		}
		else
		{
			//if is a Previous copy it
			if(SffSubHeader.IndexOfPrevious>0 && SffSubHeader.LenghtOfSubheader==0)
			{
				SffSpriteList.SetStart();
								
				//goto the previeus
				for(int j=0;j<=SffSubHeader.IndexOfPrevious-1;j++)
					SffSpriteList.Next();

				tempSffSprite.byPcxFile=SffSpriteList.GetData()->SpriteData.byPcxFile;
				tempSffSprite.bIsLinked=true;
				tempSffSprite.PcxHeader=SffSpriteList.GetData()->SpriteData.PcxHeader;
				memcpy(&tempSffSprite.ColorPallet,&SffSpriteList.GetData()->SpriteData.ColorPallet,sizeof(ColorPallet));
				SffSpriteList.SetEnd();

				tempSffList->SpriteData=tempSffSprite;
				
				SffSpriteList.Hinzufuegen(tempSffList);
			}

		}
		dwUseOfRam+=SffSubHeader.LenghtOfSubheader-128;

		nTotalImages++;

		//Seek to the next Subheader
		fseek(SffFile,SffSubHeader.NextSubheaderFileOffset,SEEK_SET);
				
		//read the next Subheader
		fread(&SffSubHeader,sizeof(SffSubHeader),1,SffFile);
				
	}
		PrintMessage("CSffSpriteManager %i images are Decoded",nImages);
		
		PrintMessage("CSffSpriteManager Sff File has used %i KBytes of ram",dwUseOfRam/1024);
		

}
//this decodes the pcx file
BYTE *CSffSpriteManager::DecodePcxFile(BYTE *byPcx,PCXHEADER *PcxHeader,PcxPallet *ColorPallet,bool bPalletSame)
{
    BYTE *byImageBuffer=NULL;
	short x,y,Pos=0;
	BYTE byData;
	int BPP;
	int size;
	WORD widht;
	
	DWORD nCountByte,nTotalyByte,
          nHowManyBytes,nHowManyBlank;

	nTotalyByte=PcxHeader->bytesPerLine*PcxHeader->NPlanes;
	
	nHowManyBytes=0;
	nHowManyBlank=0;
	nCountByte=0;
	
	BPP=PcxHeader->NPlanes*8;
	//Create a Buffer to hold the image date (change height *2)
	byImageBuffer=new BYTE[PcxHeader->widht*PcxHeader->NPlanes*PcxHeader->height+1];
	
	if(byImageBuffer==NULL)
	{
		PrintMessage("Can't allocatet memory for Pcx Image buffer");
		return NULL;

	}

	widht=PcxHeader->widht;

	if(widht<PcxHeader->bytesPerLine * PcxHeader->NPlanes)
		widht=PcxHeader->bytesPerLine*PcxHeader->NPlanes;

	size=0;
	y=0;
	x=0;
	for(y=0;y<PcxHeader->height;y++)
	{
		x=0;
	  
		while(x < widht)
		{
	     
			  byData=byPcx[Pos++];

			  if( (byData & 0xC0) == 0xC0 )
			  {
				  size = byData & 0x3F;
				  byData=byPcx[Pos++];

			  }
			  else
			  {
				size=1;
			  }

			  if(BPP==8)
			  {
				  while(size-- > 0)
				  {
					  if(x <= PcxHeader->widht)
					  {
						byImageBuffer[x + (y * PcxHeader->widht)]=byData;
						
					  }
					//this it to Skip blank data on PCX image wich are on the right side
					//TODO:OK? Skip two bytes
      				  if(x == widht && widht != PcxHeader->widht)
      				  {
      				         nHowManyBlank=widht-PcxHeader->widht;
      				         for(int i=0;i<nHowManyBlank;i++)
					              Pos+=2;
					  }
					  
					  x++;

					  					 
				  }
				 
			  }
			  else
			  {//24 bit
			 				  
				  while(size-- > 0)
				  {
					  
					  if(x < (PcxHeader->widht*3))
					  {
						byImageBuffer[x + (y * PcxHeader->widht*3)]=byData;
					  }
					  x++;

					  if(x>PcxHeader->widht*3)
					  {
						  x=0;
						  
						  y++;
					  }
		  
					  
				  }
		  
			}
		}

  }
	return byImageBuffer;

}


void CSffSpriteManager::ClearUpSpriteManger()
{
	SffSpriteList.SetStart();
	
	char str[255];
	int i=0;

	while(SffSpriteList.GetData())
	{
		
		if(!SffSpriteList.GetData()->SpriteData.bIsLinked)
		{
			delete[] SffSpriteList.GetData()->SpriteData.byPcxFile;

		}
		
		SffSpriteList.Next();
		i++;

	}
	
	SffSpriteList.CleanUpTheList();

	PrintMessage("CSffSpriteManager %i of %i images are cleaned from RAM",i,nTotalImages);
	
}



SFFSPRITE CSffSpriteManager::GetSffSprite(short nGroupNumber,short nImageNumber)
{
	SffSpriteList.SetStart();
	SFFSPRITE NoSprite;
	memset(&NoSprite,0,sizeof(NoSprite));
//	char str[255];



		
	while(SffSpriteList.GetData())
	{
		if(SffSpriteList.GetData()->SpriteData.ImageNumber==nImageNumber 
		&& SffSpriteList.GetData()->SpriteData.GroupNumber==nGroupNumber)
		{
						
			if(!m_pVideoSystem->IsImageAviable(nID,nGroupNumber,nImageNumber))
			{
					   
				//If 8bit image then give the Color Pallet to the Plugin
				if(SffSpriteList.GetData()->SpriteData.PcxHeader.NPlanes==1)
				{
				    

					//Add the image in the List	
					m_pVideoSystem->AddImage(nID,DecodePcxFile(SffSpriteList.GetData()->SpriteData.byPcxFile,
														&SffSpriteList.GetData()->SpriteData.PcxHeader,
														SffSpriteList.GetData()->SpriteData.ColorPallet,
														SffSpriteList.GetData()->SpriteData.bPalletSame),
														SffSpriteList.GetData()->SpriteData.ColorPallet,
														nGroupNumber,nImageNumber,
														SffSpriteList.GetData()->SpriteData.PcxHeader.widht,
														SffSpriteList.GetData()->SpriteData.PcxHeader.height);
					
				
				}
				//Else it is a 24 bit image  and do not give the Color Pallet
				else 
				{
					m_pVideoSystem->AddImage(nID,DecodePcxFile(SffSpriteList.GetData()->SpriteData.byPcxFile,
														&SffSpriteList.GetData()->SpriteData.PcxHeader,
														SffSpriteList.GetData()->SpriteData.ColorPallet,
														SffSpriteList.GetData()->SpriteData.bPalletSame),
														NULL,
														nGroupNumber,nImageNumber,
														SffSpriteList.GetData()->SpriteData.PcxHeader.widht,
														SffSpriteList.GetData()->SpriteData.PcxHeader.height);

                
				}

			}
				
				
			return SffSpriteList.GetData()->SpriteData;

			
		}
		SffSpriteList.Next();
		

	}

	PrintMessage("CSffSpriteManager Sprite with groubnumber %i and imagenumber %i not found",nGroupNumber,nImageNumber);
	
	return NoSprite;

}

void CSffSpriteManager::BltSprite(short nGroupNumber,short nImageNumber,short x,short y,short nMask,SDL_Rect *r,Uint32 nFlags)
{
	
	SFFSPRITE tempSprite;
	PcxPallet Color;
	SDL_Rect sourcRect;
	
	tempSprite=GetSffSprite(nGroupNumber,nImageNumber);

	if(tempSprite.byPcxFile==NULL)
		return;

	//Axis calulation	
//	x-=tempSprite.x;
//	y-=tempSprite.y;

	sourcRect.x=x;
	sourcRect.y=y;
	sourcRect.w=x+tempSprite.PcxHeader.widht;
	sourcRect.h=y+tempSprite.PcxHeader.height;



	Color=tempSprite.ColorPallet[0];
	
    m_pVideoSystem->BltSpirite(nID,x,y,tempSprite.x,tempSprite.y,
                               nGroupNumber,nImageNumber,
                               true,Color,&sourcRect,nFlags,0);
    
  
}

Uint16 CSffSpriteManager::GetFlags(Uint16 nFlags)
{
    switch(nFlags)
    {
        case HFLIPFLAG:
          return BLT_NORMALFLIPH;
        break;
        
        case VFLIPFLAG:
         return BLT_NORMALFLIPV;
        break;
        
        case HVFLIPFLAG:
          return BLT_NORMALFLIPHV;      
        break;
        
        case 0:
          return BLT_NORMAL;      
        break;
    
    }

}

//Sets the correct flags
Uint16 CSffSpriteManager::SetFlags(Uint16 nGetFlag,Uint16 nCurrFlag)
{
    //HFLIP && HLIP == NORMALBLT
    if(nCurrFlag==BLT_NORMALFLIPH && nCurrFlag==nGetFlag)
        return BLT_NORMAL;
    
    //NORMAL
    if(nCurrFlag==BLT_NORMAL && nCurrFlag==nGetFlag)
        return BLT_NORMAL;
    
            
    if(nCurrFlag==BLT_NORMALFLIPH && nGetFlag==BLT_NORMAL)
        return BLT_NORMALFLIPH;
}

//Blits the Animation
void CSffSpriteManager::BlitAnim(short x,short y,short nActionNumber,Uint32 nFlags)
{
	ActionElement* tActionElement;
	Uint32 nTempFlag;
	
	ActionElement temp;
	short h,w,x1,y1;
	
	tActionElement=m_pAirManager->GetAction(nActionNumber);
	
	if(tActionElement==NULL)
	{
	  PrintMessage("Fatal Error Can't get animation with action number %i",nActionNumber);
	
	}
	

	nTempFlag=GetFlags(tActionElement->AnimationElement[tActionElement->nCurrentImage].FlipFlags);
  	nFlags=SetFlags(nTempFlag,nFlags);
  	
	x=x+tActionElement->AnimationElement[tActionElement->nCurrentImage].x;
	y=y+tActionElement->AnimationElement[tActionElement->nCurrentImage].y;

	BltSprite(tActionElement->AnimationElement[tActionElement->nCurrentImage].nGroupNumber,
			  tActionElement->AnimationElement[tActionElement->nCurrentImage].nImageNumber,
			  x,
			  y,
			  1,
			  NULL,
              nFlags);
 
if(bShowClsn)
{  

     
    for(int i=0;i<tActionElement->AnimationElement[tActionElement->nCurrentImage].nNumberOfClsn;i++)
    {
    
    if(bFacing)
    {
       x1=x+tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].ClsnRect.x;
       y1=y+tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].ClsnRect.y;
   
       w=x+tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].ClsnRect.w;
       h=y+tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].ClsnRect.h;
       
    }
    else
    {
       x1=x-tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].ClsnRect.x;
       y1=y+tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].ClsnRect.y;
   
       w=x-tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].ClsnRect.w;
       h=y+tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].ClsnRect.h;
    
    }  
      
       if(tActionElement->AnimationElement[tActionElement->nCurrentImage].pClnsData[i].bClsn1)
              m_pVideoSystem->DrawRect(x1,y1,w,h,255,0,0);
       else
              m_pVideoSystem->DrawRect(x1,y1,w,h,0,0,255);
       
    }

    
}
    //increase the current image time
	tActionElement->dwCurrentImageTime++;
     
 	if(tActionElement->dwStartTime <= GetGameTicks() && tActionElement->AnimationElement[tActionElement->nCurrentImage].nDuringTime!=-1 )
	{
		tActionElement->nCurrentImage++;
		tActionElement->dwCurrentImageTime=0;
	
 			if(tActionElement->loopStart!=-1 && tActionElement->nCurrentImage > tActionElement->nNumberOfElements)
	 				tActionElement->nCurrentImage=tActionElement->loopStart;
	 		else if(tActionElement->nCurrentImage > tActionElement->nNumberOfElements)
	 		{
			 		tActionElement->nCurrentImage=0;
			 		tActionElement->bLooped=true;
			 		
			}
		  
		
		tActionElement->dwStartTime=GetGameTicks() + tActionElement->AnimationElement[tActionElement->nCurrentImage].nDuringTime;
	}
	
	
    //increase current animation time by one game Tick
    tActionElement->dwCurrTime++;
    
    
     //restet Current anim 
	if(tActionElement->dwCurrTime > tActionElement->dwCompletAnimTime)
	   tActionElement->dwCurrTime=0;
    

}
//Prepare the Animation
void CSffSpriteManager::PrepareAnim(int nAnim)
{
    ActionElement* tActionElement;
	
    tActionElement=m_pAirManager->GetAction(nAnim);
	
	tActionElement->dwStartTime=GetGameTicks()+tActionElement->AnimationElement[0].nDuringTime;
	tActionElement->nCurrentImage=0;
	tActionElement->dwCurrTime=0;
	tActionElement->dwCurrentImageTime=0;
	tActionElement->bLooped=false;


}
//Restet the current Animation
void CSffSpriteManager::ResetAnim(int nAnim)
{
    ActionElement* tActionElement;
	
     tActionElement=m_pAirManager->GetAction(nAnim);
	
	tActionElement->dwStartTime=0;
	tActionElement->nCurrentImage=0;
	tActionElement->dwCurrTime=0;
	tActionElement->dwCurrentImageTime=0;
	tActionElement->bLooped=false;

}
