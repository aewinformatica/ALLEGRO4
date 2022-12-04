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

#ifndef VIDEO_H
#define VIDEO_H

#include <sdl/SDL.h>
#include "global.h"
#include "ImageList.h"
#include "SDL_console.h"
#include "SDL_framerate.h"
#include "scale2x.h"


#define SCREENX_LIMIT 319
#define SCREENY_LIMIT 239



 

class CVideoSystem
{
public:
    CVideoSystem();
    ~CVideoSystem();
    
bool InitSystem();
void CleanUp();
SDL_Surface* CreateSurface(int x,int y);
  
void AddImage(short nID,BYTE* byImageData,PcxPallet *ColorPallet,
                  short nGroupNumber,short nImageNumber,short nWidht,
                  short nHeight);
SDL_Surface* ConvertImageToSDL(BYTE *byImageData,PcxPallet* ColorPallet
                                    ,short nWidth,short nHeight);

void Draw();
void Clear();
bool IsImageAviable(short nID,short nGroupNumber,short nImageNumber);
void BltSpirite(short nID,short x,short y,short xOffset,short yOffset,short nGroupNumber,short nImageNumber
                ,bool bColorKey,PcxPallet Color,SDL_Rect *scrRect,Uint32 nFlags,int nAlpha);
                
                
void NormalBlit(SDL_Surface* lpSurface,short x,short y,bool bColorKey,PcxPallet Color,SDL_Rect *scrRect);
void NormalFlipBlitH(SDL_Surface* lpSurface,short x,short y,short xOffset,short yOffset,bool bColorKey,PcxPallet Color,SDL_Rect *scrRect);

void DrawRect(Sint16 x,Sint16 y,Sint16 w,Sint16 h,Uint8 R,Uint8 G,Uint8 B);
void DrawText(int x,int y,char *strText,...);
void FilterImage();


CVideoImageList m_Player1List,
                m_Player2List,
                m_Player3List,
                m_Player4List,
                m_MenuList,
                m_StageList;

    
private:
    SDL_Surface *screen;
    SDL_Surface *work;
    Uint8* deltaPtr;
    Uint32 nowTime,lastTime;
    Uint32 nFpsCount;
    float nFps;
    FPSmanager m_FPSmanager;
   
   
                
//Some stuff to Caculate Color in 16bit mode
int ShiftRed   ;
int ShiftGreen ;
int ShiftBlue  ;
int PosRed     ;
int PosGreen   ;
int PosBlue    ;

//Some other stuff
};


#endif
