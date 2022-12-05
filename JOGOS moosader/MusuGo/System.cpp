#include "System.h"

System::System()
{
    Init();
}

void System::Init()
{
    screenWidth = 680;
    screenHeight = 440;
    fullscreen = false;
    done = false;

    allegro_init();
    install_keyboard();
    install_timer();
    install_mouse();
    install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, 0 );
    set_color_depth( 16 );
    text_mode( -1 );
    if ( fullscreen )
    {
        set_gfx_mode(GFX_AUTODETECT, screenWidth, screenHeight, 0, 0);
    }
    else
    {
        set_gfx_mode(GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);
    }
}

void System::ToggleFullscreen()
{
    if ( fullscreen )
    {
        fullscreen = false;
        set_gfx_mode(GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);

    }
    else
    {
        fullscreen = true;
        set_gfx_mode(GFX_AUTODETECT, screenWidth, screenHeight, 0, 0);
    }
}

void System::Quit()
{
    done = true;
}

