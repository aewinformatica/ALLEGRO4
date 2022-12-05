#include "Window.h"

Window::Window()
{
}

Window::Window( int tindex, string tid )
{
    visible = true;
    Setup( tindex, tid );
}

void Window::Draw( BITMAP *buffer, BITMAP *image )
{
    rectfill( buffer, x1, y1, x2, y2, makecol( color[1].r, color[1].g, color[1].b ) );
    rect( buffer, x1, y1, x2, y2, makecol( color[2].r, color[2].g, color[2].b ) );
    rect( buffer, x1, y1, x2-1, y2-1, makecol( color[0].r, color[0].g, color[0].b ) );

    for ( int i=0; i<buttonsUsed; i++ )
    {
        button[i].Draw( buffer, image, x1, y1 );
    }

    if ( id == "sub" )
    {
        textprintf(buffer, font, x1+5, y1+5,makecol( color[3].r, color[3].g, color[3].b ), "Options");
        textprintf(buffer, font, x1+100, y1+5,makecol( color[3].r, color[3].g, color[3].b ), "Tools");
        textprintf(buffer, font, x1+105, y1+20,makecol( color[3].r, color[3].g, color[3].b ), "Layer");
        textprintf(buffer, font, x1+105, y1+60,makecol( color[3].r, color[3].g, color[3].b ), "Brush Size");
        textprintf(buffer, font, x1+220, y1+5,makecol( color[3].r, color[3].g, color[3].b ), "Events");
        textprintf(buffer, font, x1+300, y1+5,makecol( color[3].r, color[3].g, color[3].b ), "Brush");
        textprintf(buffer, font, x1+865, y1+5,makecol( color[3].r, color[3].g, color[3].b ), "Editors");
    }
}

void Window::Setup( int tindex, string tid )
{
    id = tid;
    index = tindex;

    //Window colors
    color[0].SetRGB( 200, 200, 200 );   //light
    color[1].SetRGB( 150, 150, 255 );   //window color
    color[2].SetRGB( 150, 150, 150 );   //darker
    //Text color
    color[3].SetRGB( 0, 0, 0 );

    if ( tid == "sub" )
    {
        x1 = 0;
        x2 = 1024;
        y1 = 668;
        y2 = 768;
        for ( int i=0; i<14; i++ )
        {
            button[i].Setup( i );
        }
        buttonsUsed = 14;
    }
    else if ( tid == "tile" )
    {
        x1 = 0;
        x2 = 138;
        y1 = 35;
        y2 = 60;
        button[0].Setup( 0, "<<" );
        button[1].Setup( 1, "<-" );
        button[2].Setup( 2, "->" );
        button[3].Setup( 3, ">>" );
        button[4].Setup( 4, "|<" );
        button[5].Setup( 5, ">|" );
        buttonsUsed = 6;
    }
}

void Window::ResetButtons()
{
    for ( int i=0; i<buttonsUsed; i++ )
    {
        button[i].SetClicked( false );
        button[i].SetHighlight( false );
    }
}

void Window::HighlightButtons()
{
    for ( int i=0; i<buttonsUsed; i++ )
    {
        if ( mouse_x >= button[i].X1() && mouse_x <= button[i].X2() &&
            mouse_y >= button[i].Y1() + y1 && mouse_y <= button[i].Y2() + y1 )
        {
            //Collision
            button[i].SetHighlight( true );
        }
    }
}

string Window::CheckClick()
{
    for ( int i=0; i<buttonsUsed; i++ )
    {
        //check collision
        if ( mouse_x >= button[i].X1()+x1 && mouse_x <= button[i].X2()+x1 &&
                mouse_y >= button[i].Y1()+y1 && mouse_y <= button[i].Y2()+y1 )
        {
            //collision
            button[i].SetClicked( true );
            return button[i].Id();
        }
    }
    return "none";
}

void Window::SetButtonEnable( string name, bool value )
{
    for ( int i=0; i<buttonsUsed; i++ )
    {
        if ( button[i].Id() == name )
        {
            button[i].SetEnable( value );
        }
    }
}

