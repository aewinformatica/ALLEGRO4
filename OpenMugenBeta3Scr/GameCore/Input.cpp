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

#include "Input.h"

//Constructor
CInput::CInput()
{

}
//Destructor
CInput::~CInput()
{

}
//Process InputData
void CInput::ProcessInput(KEYBOARDDATA *lpKeyBoard,SDL_Event event)
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    
    //TODO:Start and Pause are ignored

    
    //Process keyboard input
    if(lpKeyBoard->bKeyBoard)
    {
          //Check down Key///////////////
          if(keystate[lpKeyBoard->Up.key])
          {
             lpKeyBoard->nKey+=UP+PRESSED;
             lpKeyBoard->Up.bPressed=true;
             lpKeyBoard->Up.nTime++;
          }
          else if(lpKeyBoard->Up.bPressed)
          {
             lpKeyBoard->nKey+=UP+RELEASED;
             lpKeyBoard->Up.bPressed=false;
             lpKeyBoard->Up.nTime=0;
          }
          
 
           if(keystate[lpKeyBoard->Down.key])
           {
              lpKeyBoard->nKey+=DOWN+PRESSED;
              lpKeyBoard->Down.bPressed=true;
              lpKeyBoard->Down.nTime++;
           }
           else if(lpKeyBoard->Down.bPressed)
           {
             lpKeyBoard->nKey+=DOWN+RELEASED;
             lpKeyBoard->Down.bPressed=false;
             lpKeyBoard->Down.nTime=0;
           }
              
           if(keystate[lpKeyBoard->Left.key])
           {
              lpKeyBoard->nKey=LEFT+PRESSED;
              lpKeyBoard->Left.bPressed=true;
              lpKeyBoard->Left.nTime++;
           }
           else if(lpKeyBoard->Left.bPressed)
           {
             lpKeyBoard->nKey+=LEFT+RELEASED;
             lpKeyBoard->Left.bPressed=false;
             lpKeyBoard->Left.nTime=0;
           }
 
 

           if(keystate[lpKeyBoard->Right.key])
           {
              lpKeyBoard->nKey+=RIGHT+PRESSED;
              lpKeyBoard->Right.bPressed=true;
              lpKeyBoard->Right.nTime++;
           }
           else if(lpKeyBoard->Right.bPressed)
           {
             lpKeyBoard->nKey+=RIGHT+RELEASED;
             lpKeyBoard->Right.bPressed=false;
             lpKeyBoard->Right.nTime=0;
           }
           
              
           if(keystate[lpKeyBoard->ButtonA.key])
           {
              lpKeyBoard->nKey+=BUTTONA+PRESSED;
              lpKeyBoard->ButtonA.bPressed=true;
              lpKeyBoard->ButtonA.nTime++;
           }
           else if(lpKeyBoard->ButtonA.bPressed)
           {
             lpKeyBoard->nKey+=BUTTONA+RELEASED;
             lpKeyBoard->ButtonA.bPressed=false;
             lpKeyBoard->ButtonA.nTime=0;
           }
           
              
           if(keystate[lpKeyBoard->ButtonB.key])
           {
              lpKeyBoard->nKey+=BUTTONB+PRESSED;
              lpKeyBoard->ButtonB.bPressed=true;
              lpKeyBoard->ButtonB.nTime++;
           }
           else if(lpKeyBoard->ButtonB.bPressed)
           {
             lpKeyBoard->nKey+=BUTTONB+RELEASED;
             lpKeyBoard->ButtonB.bPressed=false;
             lpKeyBoard->ButtonB.nTime=0;
           }
              
           if(keystate[lpKeyBoard->ButtonC.key])
           {
              lpKeyBoard->nKey+=BUTTONC+PRESSED;
              lpKeyBoard->ButtonC.bPressed=true;
              lpKeyBoard->ButtonC.nTime++;
           }
           else if(lpKeyBoard->ButtonC.bPressed)
           {
             lpKeyBoard->nKey+=BUTTONC+RELEASED;
             lpKeyBoard->ButtonC.bPressed=false;
             lpKeyBoard->ButtonC.nTime=0;
           }

           if(keystate[lpKeyBoard->ButtonX.key])
           {
              lpKeyBoard->nKey+=BUTTONX+PRESSED;
              lpKeyBoard->ButtonX.bPressed=true;
              lpKeyBoard->ButtonX.nTime++;
           }
           else if(lpKeyBoard->ButtonX.bPressed)
           {
             lpKeyBoard->nKey+=BUTTONX+RELEASED;
             lpKeyBoard->ButtonX.bPressed=false;
             lpKeyBoard->ButtonX.nTime=0;
           }
              
           if(keystate[lpKeyBoard->ButtonY.key])
           {
              lpKeyBoard->nKey+=BUTTONY+PRESSED;
              lpKeyBoard->ButtonY.bPressed=true;
              lpKeyBoard->ButtonY.nTime++;
           }
           else if(lpKeyBoard->ButtonY.bPressed)
           {
             lpKeyBoard->nKey+=BUTTONY+RELEASED;
             lpKeyBoard->ButtonY.bPressed=false;
             lpKeyBoard->ButtonY.nTime=0;
           }
              
           if(keystate[lpKeyBoard->ButtonZ.key])
           {
              lpKeyBoard->nKey+=BUTTONZ+PRESSED;
              lpKeyBoard->ButtonZ.bPressed=true;
              lpKeyBoard->ButtonZ.nTime++;
           }
           else if(lpKeyBoard->ButtonZ.bPressed)
           {
             lpKeyBoard->nKey+=BUTTONZ+RELEASED;
             lpKeyBoard->ButtonZ.bPressed=false;
             lpKeyBoard->ButtonZ.nTime=0;
           }
       
           //Check for a key release  
            
     
              
        
      
        
    }
  
}
