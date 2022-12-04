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

#include "VideoSystem.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "DT_drawtext.h"

void scale2x(SDL_Surface *src, SDL_Surface *dst);

ConsoleInformation *mMainConsole;
//Konstructor
CVideoSystem::CVideoSystem()
{
    work=screen=NULL;
    nowTime=lastTime=0;
    nFpsCount=0;
     
}
//Destructor
CVideoSystem::~CVideoSystem()
{
    PrintMessage("CVideoSystem::~CVideoSystem() Cleaning Up");
    m_Player1List.CleanUp();
    m_Player2List.CleanUp();
    m_Player3List.CleanUp();
    m_Player4List.CleanUp();
    m_StageList.CleanUp();
    m_MenuList.CleanUp();
    CleanUp();
    
}

void CVideoSystem::CleanUp()
{
   // CON_Destroy(mMainConsole);
    SDL_FreeSurface(work);
    SDL_FreeSurface(screen); 
   
}

bool CVideoSystem::InitSystem()
{
    Uint32 Color;
    SDL_Rect ConRect;
    
    PrintMessage("CVideoSystem::InitSystem()");
     //Set Video mode and get main Surface   
    screen=SDL_SetVideoMode(640,480,16,SDL_SWSURFACE|SDL_DOUBLEBUF);
    //Check the surface for validate
    if(screen==NULL)
    {
        PrintMessage("SDL_SetVideoMode Failed");
        return false;
       
    }
    //NextGen Mugen
    SDL_WM_SetCaption(OMTITLE,NULL);
    
    //Create the work surface
    work=CreateSurface(320,240);
    Color=SDL_MapRGB(screen->format,255,0,255);
    SDL_FillRect(work,NULL,Color);
    //Now create the console
    ConRect.y=ConRect.x=0;
    ConRect.h=200;
    ConRect.w=50;
    
#ifdef _XBOX
	mMainConsole=CON_Init("d:\\ConsoleFont.bmp", screen, 100, ConRect);
#else    
	mMainConsole=CON_Init("ConsoleFont.bmp", screen, 100, ConRect);
#endif
	if(mMainConsole==NULL)    
        PrintMessage("Can't allocate console");
        
    //Now attach the command_handler function
    CON_SetExecuteFunction(mMainConsole,Command_Handler);    
        
    CON_Topmost(mMainConsole);
    
    //CON_Show(mMainConsole);    

    //Set the frame manager to 60 Hz
    SDL_initFramerate(&m_FPSmanager);
    SDL_setFramerate(&m_FPSmanager,60);
       
                         
    return true;
}


//To clear the screen
void CVideoSystem::Clear()
{
    Uint32 Color;
    
    Color=SDL_MapRGB(screen->format,0,255,0);
    SDL_FillRect(work,NULL,Color);
    

}
//Copy the work surface to screen backbuffer and then flip
void CVideoSystem::Draw()
{
        
    nowTime=SDL_GetTicks();
    if(nowTime > lastTime+500)
    {
    
            
        nFps=(float)nFpsCount*1000 / (nowTime-lastTime);
        nFpsCount=0;
        lastTime=nowTime;
        
    
    }
    DrawText(0,0,"%2.2f FPS",nFps);
    
    FilterImage();
    
    //scale2x(work,screen);
    
  //  SDL_BlitSurface(work,NULL,screen,NULL);
            
    //CON_DrawConsole(mMainConsole);
    
    SDL_Flip(screen);
    
     //Limit the frame rate to 60 Hz
    SDL_framerateDelay(&m_FPSmanager);
    
    nFpsCount++;
    
   
}

//this is the MMX version of Scale2x only uses this if we have a CPU with MMX support
void CVideoSystem::FilterImage()
{
    if(SDL_LockSurface(work) < 0)
        PrintMessage("Was not able to lock work surface");
    
    if(SDL_LockSurface(screen) < 0)
        PrintMessage("Was not able to lock screen surface");
    
   
   scale2x_uint16 *dst0,*dst1;
   scale2x_uint16 *scr0,*scr1,*scr2;
   
   
   dst0=(scale2x_uint16*)screen->pixels;
   dst1=dst0+(screen->pitch/2);   
   
   scr0=(scale2x_uint16*)work->pixels;
   scr1=scr0 + (work->pitch/2);
   scr2=scr1 + (work->pitch/2);
   
   scale2x_16_mmx(dst0,dst1,scr0,scr1,scr2,640);
   
  for(int i=0;i<239;i++)
  {
      scale2x_16_mmx(dst0,dst1,scr0,scr1,scr2,640);
      dst0+=(screen->pitch);
      dst1+=(screen->pitch);
      
      scr0=scr1;
      scr1=scr2;
      scr2+=(work->pitch/2);
     
     
  }
  
   SDL_UnlockSurface(work);
   SDL_UnlockSurface(screen);
   
   //Clear the MMX 
   scale2x_mmx_emms();

}

void CVideoSystem::DrawRect(Sint16 x,Sint16 y,Sint16 x1,Sint16 y1,Uint8 R,Uint8 G,Uint8 B)
{
    Uint16 Color;
    Uint16 *lpData=NULL;
    Uint16 nPitch;
    Sint16 temp;
    Sint16 w,h;
       
    Color=SDL_MapRGB(screen->format,R,G,B);
    
    SDL_LockSurface(work);
    
    lpData=(Uint16*)work->pixels;
    nPitch=work->pitch/2;
    
    if(x>x1)
    {
        temp=x1;
        x1=x;
        x=temp;
    
    }
    
    if(y>y1)
    {    
        temp=y1;
        y1=y;
        y=temp;
    
    }
    //Clipping
    if(y1>SCREENY_LIMIT)
        y1=SCREENY_LIMIT;

    if(x1>SCREENX_LIMIT)
        x1=SCREENX_LIMIT;
        
    if(x<0)
        x=0;
    
    if(y<0)
        y=0;
   //Endof Clipping
    
    temp=y;
    w=x+(x1-x);
    
    for(;y<y1;y++)
    {
       lpData[y*nPitch+x]=Color;
       lpData[y*nPitch+w]=Color;
    
    }
    
    y=temp;
    h=y+(y1-y);
    
    for(;x<x1;x++)
    {
       lpData[y*nPitch+x]=Color;
       lpData[h*nPitch+x]=Color;
    }

   
    SDL_UnlockSurface(work);
    
//    Color=SDL_MapRGB(screen->format,R,G,B);
//    SDL_FillRect(work,&rect,Color);

}

//Draw a string to video
void CVideoSystem::DrawText(int x,int y,char *strText,...)
{
  char string[255];                  // Temporary string

  va_list ap;                // Pointer To List Of Arguments
  va_start(ap, strText);         // Parses The String For Variables
  vsprintf(string, strText, ap); // Converts Symbols To Actual Numbers
  va_end(ap);  
  
  DT_DrawText(string,work,0,x,y);

}
//Creates a SDL Surface
SDL_Surface * CVideoSystem::CreateSurface(int x,int y)
{
    SDL_Surface* temp=NULL;

    temp=SDL_CreateRGBSurface(SDL_SWSURFACE,x,y,16,screen->format->Rmask
                                                  ,screen->format->Gmask
                                                  ,screen->format->Bmask
                                                  ,screen->format->Amask);
                                                  

    if(temp==NULL)
    {
        PrintMessage("CVideoSystem::CreateSurfac Create Surfacce Failed %s",SDL_GetError());
        return NULL;
    }
    return temp;                                                                                          
                                                  
}

void CVideoSystem::AddImage(short nID,BYTE* byImageData,PcxPallet *ColorPallet,
                            short nGroupNumber,short nImageNumber,short nWidht,
                            short nHeight)
{
    ImageList *temp;
    temp=new ImageList;
   
    //Now fill the values
    temp->nGroupNumber=nGroupNumber;
    temp->nImageNumber=nImageNumber;

    temp->lpSurface=ConvertImageToSDL(byImageData,ColorPallet,nWidht,nHeight);
    

    
    switch(nID)
    {
        case IMP1:
           m_Player1List.Add(temp);
        break;

        case IMP2:
           m_Player2List.Add(temp);     
        break;

        case IMP3:
           m_Player3List.Add(temp);
        break;

        case IMP4:
           m_Player4List.Add(temp);
        break;
        
        case IMMENU:
           m_MenuList.Add(temp);
        break;
        
        case IMSTAGE:
           this->m_StageList.Add(temp);
        break;
        
        default:
                PrintMessage("CVideoSystem::AddImage can't use the ID");
        break;
    
    }
   
}
//Convets the decompressed PCX image to 16bit SDL_SURFACE 
//TODO:Handel possible BYTE order changes in other system like Appel MacOSX?
SDL_Surface * CVideoSystem::ConvertImageToSDL(BYTE *byImageData,PcxPallet* ColorPallet
                                    ,short nWidth,short nHeight)
{
    SDL_Surface *temp=NULL;
    WORD* lpData=NULL;
    short nPitch;
    
    temp=CreateSurface(nWidth,nHeight);
    
    if(temp==NULL)
        PrintMessage("CreateSurface failed");
    
    SDL_LockSurface(temp);
    
    // Todo: Wrong Pitch  must be korrect
    lpData=(WORD*)temp->pixels;
    nPitch=temp->pitch/2 ;
    

    //It is a 8bit image with a ColorPallet
	if(ColorPallet!=NULL)
	{
		for(short x=0;x<nWidth;x++)
		{
			for(short y=0;y<nHeight;y++)
			{
			          
				lpData[y*nPitch+x] = (WORD) SDL_MapRGB(screen->format,           
                                                      ColorPallet[byImageData[x + y * nWidth]].R,
                                                      ColorPallet[byImageData[x + y * nWidth]].G,
                                                      ColorPallet[byImageData[x + y * nWidth]].B);
			}


		}

	}
	else
	{
	//this is for a true Color PCX image(24bit)
		for(short x=0;x<nWidth;x++)
		{
			for(short y=0;y<nHeight;y++)
			{
				lpData[y*nPitch+x] = (WORD) SDL_MapRGB(screen->format,
                                                       byImageData[x + (y * nWidth*3)],
                                                       byImageData[x + nWidth   + (y * nWidth*3)],
                                                       byImageData[x + (nWidth*2) + (y * nWidth*3)]);
											


			}


		}
	}
	
	SDL_UnlockSurface(temp);
	
	return temp;


    
}
//Check if the image is already aviable
bool CVideoSystem::IsImageAviable(short nID,short nGroupNumber,short nImageNumber)
{
    switch(nID)
    {
        case IMP1:
           if (m_Player1List.GetImage(nGroupNumber,nImageNumber)!= NULL )
            return true;
        break;
        

        case IMP2:
           if (m_Player2List.GetImage(nGroupNumber,nImageNumber)!= NULL )
            return true;    
        break;

        case IMP3:
           if (m_Player3List.GetImage(nGroupNumber,nImageNumber)!= NULL )
            return true;
        break;

        case IMP4:
           if (m_Player4List.GetImage(nGroupNumber,nImageNumber)!= NULL )
            return true;
        break;
        
        case IMMENU:
           if (m_MenuList.GetImage(nGroupNumber,nImageNumber)!= NULL )
            return true;
        break;
        
        case IMSTAGE:
           if (m_StageList.GetImage(nGroupNumber,nImageNumber)!= NULL )
            return true;
        break;
        
        default:
                PrintMessage("CVideoSystem::IsImageAviable can't use the ID");
        break;
    
    }
    return false;

}
//Function which is called when a blit must be performed
void CVideoSystem::BltSpirite(short nID,short x,short y,short xOffset,short yOffset,short nGroupNumber,short nImageNumber
                              ,bool bColorKey,PcxPallet Color,SDL_Rect *scrRect,Uint32 nFlags,int nAlpha)
                
{
//Needed Variables
    ImageList *temp=NULL;
   
     switch(nID)
    {
        case IMP1:
           temp=m_Player1List.GetImage(nGroupNumber,nImageNumber);
         break;
        

        case IMP2:
           temp=m_Player2List.GetImage(nGroupNumber,nImageNumber);
        break;

        case IMP3:
           temp=m_Player3List.GetImage(nGroupNumber,nImageNumber);
        break;

        case IMP4:
           temp=m_Player4List.GetImage(nGroupNumber,nImageNumber);
        break;
        
        case IMMENU:
           temp=m_MenuList.GetImage(nGroupNumber,nImageNumber);
        break;
        
        case IMSTAGE:
           temp=m_StageList.GetImage(nGroupNumber,nImageNumber);
        break;
        
        default:
                PrintMessage("CVideoSystem::BltSpirite can't use the ID");
        break;
    
    }
    if(temp==NULL)
    {
        PrintMessage("CVideoSystem::BltSpirite Fatal error Group=%4i Image%4i",nGroupNumber,nImageNumber);
        return;
    }
   //Now find which blit we must do 
    switch(nFlags)
    {
        case BLT_NORMAL:
            scrRect->x-=xOffset;
            scrRect->y-=yOffset;    
            NormalBlit(temp->lpSurface,x,y,bColorKey,Color,scrRect);
        break;
        
        case BLT_NORMALFLIPH:
             NormalFlipBlitH(temp->lpSurface,x,y,xOffset,yOffset,bColorKey,Color,scrRect);   
        break;
    
    }
    
    


}
//Normal blit with ColorKey
void CVideoSystem::NormalBlit(SDL_Surface* lpSurface,short x,short y,bool bColorKey,PcxPallet Color,SDL_Rect *scrRect)
{
    if(bColorKey)
    {
        SDL_SetColorKey(lpSurface,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,
                                                              Color.R,Color.G,Color.B));
         
   }
   SDL_BlitSurface(lpSurface,NULL,work,scrRect);

}

//Makes a normal flip
//TODO:It comes to a strange artefect if the image goes down side the screen?
void CVideoSystem::NormalFlipBlitH(SDL_Surface* lpSurface,short x,short y,short xOffset,short yOffset,bool bColorKey,PcxPallet Color,SDL_Rect *scrRect)
{
//Variables///////////////////////////////
    Uint16 *lpSrcData;
    Uint16 *lpDesData;
    
    //for clipping reason 
    short nWidth;
    short nHeight;
    
    Uint16 nScrPitch;
    Uint16 nDestPitch;
    
    Uint16 nColor;
    
    int yClip=0;
    int xClip=0;
    int xClip2=0;
    int i=0;
    int j=0;   
    
////////////////////////////
    nWidth=lpSurface->w;
    nHeight=lpSurface->h;
    
    //Locks the surface       
    SDL_LockSurface(lpSurface);
    SDL_LockSurface(work);
    
    //Get the pointer
    nScrPitch=lpSurface->pitch/2;
    lpSrcData=(Uint16*)lpSurface->pixels;

    nDestPitch=work->pitch/2;
    lpDesData=(Uint16*)work->pixels;
    ///////////////////////////////    
  
    //Calculate the correct x and y  value which dependce from the offset
    y-=nHeight-(nHeight-yOffset);
    x-=nWidth-xOffset;
    
    //Check the left side of the image
    if(x < 0)
    {
        xClip=-x;   
        x=0;
    }
    //Check the right side
    if(x + nWidth-1 > XMAX)
    {
        xClip2=(x + nWidth-1) - XMAX;
        nWidth-=(x + nWidth-1) - XMAX;
               
    }
    
    //check the top of the image
    if(y < 0)
    {
        nHeight+=y;
        yClip=-y;
        y=0;
    
    }
    
    //Check the bottom of the image
    if(y + nHeight > XMAX)
    {
        nHeight-=(y + nHeight) - XMAX;
    
    }
   
    
    
    
    
   lpDesData+=y*nDestPitch;
   lpDesData+=x;
   lpSrcData+=yClip*nScrPitch;

//with ColorKey    
if(bColorKey)
{
    nColor=(Uint16)SDL_MapRGB(work->format,Color.R,Color.G,Color.B);
    for(i=0;i<nHeight;i++)
    {
        
        for(j=xClip;j<nWidth;j++)
        {
           if(nColor!=lpSrcData[(nWidth-j-1)+xClip2])
             lpDesData[j-xClip]=lpSrcData[(nWidth-j-1)+xClip2];      
        }
        lpSrcData+=nScrPitch;
        lpDesData+=nDestPitch;
    }
}//without ColorKey
else
{
    for(i=0;i<nHeight;i++)
    {
        
        for(j=xClip;j<nWidth;j++)
        {
           lpDesData[j-xClip]=lpSrcData[(nWidth-j-1)+xClip2];      
        }
        lpSrcData+=nScrPitch;
        lpDesData+=nDestPitch;
    }

}   


    SDL_UnlockSurface(lpSurface);
    SDL_UnlockSurface(work);

}                                              
                                              
                            
