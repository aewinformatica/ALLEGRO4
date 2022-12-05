#include "System.h"

System::System()
{
}

void System::Setup()
{
    state = sMENU;
    scrWidth = 640;
    scrHeight = 480;
    fullscreen = true;
    
    //Initialize Allegro
    allegro_init();
    install_keyboard();
    install_timer();
    install_mouse();
    install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, 0 );
    set_color_depth( 16 );
    
    if ( fullscreen ) 
        set_gfx_mode( GFX_AUTODETECT, scrWidth, scrHeight, 0, 0 );
    else
        set_gfx_mode( GFX_AUTODETECT_WINDOWED, scrWidth, scrHeight, 0, 0 );
            
//    text_mode( -1 );
}

void System::ToggleFullscreen()
{
    fullscreen = -fullscreen;
    
    if ( fullscreen ) 
        set_gfx_mode( GFX_AUTODETECT, scrWidth, scrHeight, 0, 0 );
    else
        set_gfx_mode( GFX_AUTODETECT_WINDOWED, scrWidth, scrHeight, 0, 0 );    
}

void System::DrawToScreen( BITMAP *buffer )
{
    // Draw to buffer image
    blit( buffer, screen, 0, 0, 0, 0, scrWidth, scrHeight );
    release_screen();
    clear_bitmap( buffer );
}
