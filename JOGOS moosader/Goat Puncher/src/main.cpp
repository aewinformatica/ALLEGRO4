#include <iostream>
#include <allegro.h>
#include "System.h"
#include "ImageManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "Rectangle.h"

using namespace std;

volatile long fps = 0;
void IncFps() { fps++; }

bool IsCollision( Rectangle A, Rectangle B );

int main()
{
    System program;
    program.Setup();
    
    ImageManager imgMgr; 
    Player player[4];
    Player goat;
    for ( int i=0; i<4; i++ )
    {
        player[i].Setup( i );
    }
    goat.Setup( 5 );
    SoundManager sndMgr;
    
    // Set up timer to regulate FPS
    LOCK_VARIABLE( program.fps );
    LOCK_FUNCTION( IncFps );
    install_int_ex( IncFps,BPS_TO_TIMER(90) );
    
    Rectangle rectA, rectB;
               
    while ( program.State() != sQUIT )
    {
        while ( fps > 0 )
        {
            if ( key[KEY_F4] )
            {
                sndMgr.Play( "power" );
                program.State( sQUIT ); 
            }
            else if ( key[KEY_ESC] )
            {
                program.State( sMENU );
                sndMgr.Play( "explode" );
            }
            else if ( key[KEY_ENTER] )
            {
                program.State( sINGAME );
                sndMgr.Play( "collect" );
            }
            
            /* player 0 */
            if ( key[KEY_A] ) { player[0].Move( LEFT ); }
            if ( key[KEY_D] ) { player[0].Move( RIGHT ); }
            if ( key[KEY_W] ) { player[0].Action( KICK ); }
            else if ( key[KEY_S] ) { player[0].Action( PUNCH ); }
            else { player[0].Action( IDLE ); }
            /* player 1 */
            if ( key[KEY_J] ) { player[1].Move( LEFT ); }
            if ( key[KEY_L] ) { player[1].Move( RIGHT ); }
            if ( key[KEY_I] ) { player[1].Action( KICK ); }
            else if ( key[KEY_K] ) { player[1].Action( PUNCH ); }
            else { player[1].Action( IDLE ); }
            /* player 2 */
            if ( key[KEY_LEFT] ) { player[2].Move( LEFT ); }
            if ( key[KEY_RIGHT] ) { player[2].Move( RIGHT ); }
            if ( key[KEY_UP] ) { player[2].Action( KICK ); }
            else if ( key[KEY_DOWN] ) { player[2].Action( PUNCH ); }
            else { player[2].Action( IDLE ); }
            /* player 3 */
            if ( key[KEY_4_PAD] ) { player[3].Move( LEFT ); }
            if ( key[KEY_6_PAD] ) { player[3].Move( RIGHT ); }
            if ( key[KEY_7_PAD] ) { player[3].Action( KICK ); }
            else if ( key[KEY_9_PAD] ) { player[3].Action( PUNCH ); }
            else { player[3].Action( IDLE ); }
            
            if ( key[KEY_P] ) { system("pause"); }
            
            /* Check collisions */
            for ( int i=0; i<4; i++ )
            {
                rectB.Setup( goat.x, goat.y, goat.w, goat.h );
                rectA.Setup( player[i].x, player[i].y, player[i].w, player[i].h );
                
                if ( IsCollision( rectA, rectB ) &&
                        player[i].action != IDLE )
                {
                    // play goat kicking noise
                    sndMgr.Play( i );
                    
                    if ( goat.x < player[i].x )
                        goat.GiveVelocity( LEFT );
                    else
                        goat.GiveVelocity( RIGHT );
                }
                
                // Check collision with other players
                for ( int j=0; j<4; j++ )
                {
                    if ( i != j )
                    {
                        rectA.Setup( player[i].x, player[i].y, player[i].w, player[i].h );
                        rectB.Setup( player[j].x, player[j].y, player[j].w, player[j].h );
                        
                        if ( IsCollision( rectA, rectB ) &&
                            player[i].action != IDLE )
                        {
                            sndMgr.Play( i );
                            if ( player[j].x < player[i].x )
                                player[j].GiveVelocity( LEFT );
                            else
                                player[j].GiveVelocity( RIGHT );
                        }
                    }
                    
                    //Update players
                    player[i].Update();
                }
            }
            
            goat.Update();
            
            sndMgr.DecCounter();
            fps--;
        }
        
        /* Draw */
        if ( program.State() == sMENU )
        {
            imgMgr.Draw( "gptitle", 0, 0, 640, 480, 0, 0 );
        }
        else if ( program.State() == sINGAME )
        {
            imgMgr.Draw( "background", 0, 0, 640, 480, 0, 0 );
            
            for ( int i=0; i<4; i++ )
            {
                imgMgr.Draw( i+1, player[i].x, player[i].y, 
                    player[i].w, player[i].h, player[i].fx, player[i].fy );
            }
            imgMgr.Draw( "goat", goat.x, goat.y, 
                    goat.w, goat.h, goat.fx, goat.fy );
        }
        
        program.DrawToScreen( imgMgr.buffer );
        
    }// while not done
            
    return 0;
}
END_OF_MAIN();

bool IsCollision( Rectangle A, Rectangle B )
{
    /*  
        Make sure what is passed is collision region + coordinates, OKAY?!    
        if 
        leftA < rightB &&
        rightA > leftB &&
        topA < bottomB &&
        bottomA > topB
        then collision
    */
    if (    A.L() < B.R() &&
            A.R() > B.L() &&
            A.T() < B.B() && 
            A.B() > B.T() )
    {
        return true;
    }
    return false;
}
