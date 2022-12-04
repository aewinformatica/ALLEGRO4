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


#include "Game.h"


//Constructor
CGame::CGame()
{
    bError=false;
    bGame=true;
    nStartTime=nEndTime=0;
    nGameType=GFIGHTGAME;
}

//Destructor
CGame::~CGame()
{

}
//Init the hole game 
void CGame::InitGame()
{
    //First init the Log file system
    InitLogFile();
    //Now print Welcome Message
    PrintMessage("Welcome to OpenMugen Version %s",VER);
    //Now init SDL system
    PrintMessage("CGame:Init SDL system");
    
    if(!m_SdlManager.Init())
    {
        PrintMessage("CGame:SDL init failed");
        bError=true;
    
    }
    
    memset(&Player1Keys,0,sizeof(Player1Keys));
    memset(&Player2Keys,0,sizeof(Player2Keys));
    //Setup Player1 key config
    Player1Keys.Up.key        =SDLK_UP;
    Player1Keys.Down.key      =SDLK_DOWN;
    Player1Keys.Left.key      =SDLK_LEFT;  
    Player1Keys.Right.key     =SDLK_RIGHT;
    Player1Keys.ButtonA.key   =SDLK_KP4; 
    Player1Keys.ButtonB.key   =SDLK_KP5; 
    Player1Keys.ButtonC.key   =SDLK_KP6;
    Player1Keys.ButtonX.key   =SDLK_KP7;  
    Player1Keys.ButtonY.key   =SDLK_KP8;
    Player1Keys.ButtonZ.key   =SDLK_KP9;
    Player1Keys.bStart.key    =SDLK_RETURN;
    Player1Keys.bPause.key    =SDLK_TAB;
    //for now only Keyboard is support
    Player1Keys.bKeyBoard=true;
       
    
    //Restet the Game timer
    ResetGameTimer();
    
    m_FightEngine.InitFightEngine(&m_SdlManager.m_VideoSystem);
    m_FightEngine.LoadPlayer1("chars\\kfm\\kfm.def");
    m_FightEngine.LoadPlayer2("chars\\kfm\\kfm.def");
    
     m_FightEngine.lpPlayer1->pKey=&Player1Keys;
     m_FightEngine.lpPlayer2->pKey=&Player2Keys;
       
      
    

}
//The main game Loop
void CGame::RunGame()
{
    while(bGame)
    {
       //Checks SDL system and Get Input
       CheckSDL();
                                     
       //Clear the Screen
       m_SdlManager.m_VideoSystem.Clear();
    
       //switch to the current Subsystem
       switch(nGameType)
       {
          //We running the Fight engine
          case GFIGHTGAME:
              //Update the game timer
              UpdateTimer();      
              //Update Game
              m_FightEngine.RunEngine();      
          break;
          //We are in the menu screen
          case GMENU:
          
          break;
       
       
       }
       //Now draw the content to the screen     
       m_SdlManager.m_VideoSystem.Draw();
       //Limits the game speed to 60Hz
      
    }

}
//Checks the SDL event list
void CGame::CheckSDL()
{

   //Clear the prev state of the keys
   Player1Keys.nKey=0;
   Player2Keys.nKey=0;
   memset(&event,0,sizeof(event));

   SDL_PollEvent(&event);
  
   //Process Player1 and Player2 input
   m_GameInput.ProcessInput(&Player1Keys,event);
   m_GameInput.ProcessInput(&Player2Keys,event);
     
        
     if(event.type==SDL_QUIT)
        bGame=false;
   
     if(event.key.keysym.sym==SDLK_ESCAPE)
        bGame=false;
      
   

}

//Exit the game and cleans the memory and SDL
void CGame::Quit()
{
    m_SdlManager.CleanSDL();
    SDL_Quit();
    

}

void CGame::Crash()
{
    
}
