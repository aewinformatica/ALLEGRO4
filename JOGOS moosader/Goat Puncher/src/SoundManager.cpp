#include "SoundManager.h"

void SoundManager::DecCounter() 
{ 
    if ( soundCounter[0] > 0 )
        soundCounter[0] -= 0.5; 
    if ( soundCounter[1] > 0 )
        soundCounter[1] -= 0.5; 
    if ( soundCounter[2] > 0 )
        soundCounter[2] -= 0.5; 
    if ( soundCounter[3] > 0 )  
        soundCounter[3] -= 0.5; 
}

SoundManager::SoundManager()
{
    soundCounter[0] = 0;
    soundCounter[1] = 0;
    soundCounter[2] = 0;
    soundCounter[3] = 0;
    sound[0].Setup( "hit1" );
    sound[1].Setup( "hit2" );
    sound[2].Setup( "hit3" );
    sound[3].Setup( "hit4" );
    sound[4].Setup( "jump1" );
    sound[5].Setup( "jump2" );
    sound[6].Setup( "jump3" );
    sound[7].Setup( "jump4" );
    sound[8].Setup( "collect" );
    sound[9].Setup( "down" );
    sound[10].Setup( "explode" );
    sound[11].Setup( "power" );
}

SoundManager::~SoundManager()
{
    for ( int i=0; i<SNDAMT; i++ )
    {
        if ( sound[i].snd != NULL )
            destroy_sample( sound[i].snd );
    }
}

void SoundManager::Play( int index )
{
    if ( soundCounter[index%4] <= 0 )
    {
        play_sample( sound[index].snd, 255, 128, 1000, false );
        soundCounter[index%4] = 10;
    }
}

void SoundManager::Play( string name )
{
    bool success = false;
    for ( int i=0; i<SNDAMT; i++ )
    {
        if ( sound[i].description == name )
        {
            Play( i );
            success = true;
        }
    }
    
    if ( !success ) { cerr<<"Error playing "<<name<<endl; }
}        


