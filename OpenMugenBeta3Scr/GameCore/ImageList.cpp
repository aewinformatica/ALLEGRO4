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

#include "Global.h"
#include "ImageList.h"

CVideoImageList::CVideoImageList()
{
	Aktuell=NULL;
	Anfang=NULL;
	Ende=NULL;

}

CVideoImageList::~CVideoImageList()
{


}

void CVideoImageList::CleanUp()
{
    ImageList *temp;
    SetStart();
    
    while(Aktuell)
    {
        temp=Aktuell->Next;       
        SDL_FreeSurface(Aktuell->lpSurface);
        delete Aktuell;
        Aktuell=temp;
    
    }
    

}

void CVideoImageList::SetStart()
{
	Aktuell=Anfang;
}

ImageList *CVideoImageList::GetImage(short nGroupNumber,short nImageNumber)
{
    SetStart();
    
    while(Aktuell)
    {
        if(Aktuell->nGroupNumber==nGroupNumber && Aktuell->nImageNumber == nImageNumber)
                return Aktuell;
        
        Aktuell=Aktuell->Next;
    
    }
    
    

    return NULL;

}

void CVideoImageList::Add(ImageList *p)
{
	if(Anfang==NULL)
	{
		p->Next=NULL;
		p->Prev=NULL;
		Aktuell=Ende=Anfang=p;
	}
	else
	{
		Ende->Next=p;
		p->Prev=Ende;
		p->Next=NULL;
		Ende=p;
		Aktuell=p;

	}

}
