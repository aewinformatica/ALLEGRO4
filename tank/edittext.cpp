// edittext.cpp

#include <allegro.h>
#include <string>
using namespace std;

#define WHITE makecol(255, 255, 255)

int main()
{
        // typical Allegro initialization
        allegro_init();
        install_keyboard();
        set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0);
        
        // all variables are here
        BITMAP* buffer = create_bitmap(320, 240); // initialize the double buffer
        string  edittext;                         // an empty string for editting
        string::iterator iter = edittext.begin(); // string iterator
        int     caret  = 0;                       // tracks the text caret
        bool    insert = true;                    // true of should text be inserted
        
        // the game loop
        do
        {
                while(keypressed())
                {
                        int  newkey   = readkey();
                        char ASCII    = newkey & 0xff;
                        char scancode = newkey >> 8;
                        
                        // a character key was pressed; add it to the string
                        if(ASCII >= 32 && ASCII <= 126)
                        {
                                // add the new char, inserting or replacing as need be
                                if(insert || iter == edittext.end())
                                iter = edittext.insert(iter, ASCII);
                                else
                                edittext.replace(caret, 1, 1, ASCII);
                                
                                // increment both the caret and the iterator
                                caret++;
                                iter++;
                        }
                        // some other, "special" key was pressed; handle it here
                        else
                        switch(scancode)
                        {
                                case KEY_DEL:
                                if(iter != edittext.end()) iter = edittext.erase(iter);
                                break;
                                
                                case KEY_BACKSPACE:
                                if(iter != edittext.begin())
                                {
                                        caret--;
                                        iter--;
                                        iter = edittext.erase(iter);
                                }
                                break;
                                
                                case KEY_RIGHT:
                                if(iter != edittext.end())   caret++, iter++;
                                break;
                                
                                case KEY_LEFT:
                                if(iter != edittext.begin()) caret--, iter--;
                                break;
                                
                                case KEY_INSERT:
                                if(insert) insert = 0; else insert = 1;
                                break;
                                
                                default:
                                
                                break;
                        }
                }
                
                // clear screen
                clear(buffer);
                
                // output the string to the screen
                textout(buffer, font, edittext.c_str(), 0, 10, WHITE);
                
                // output some stats using Allegro's printf functions
                textprintf(buffer, font,  0, 20, WHITE, "length:   %d", edittext.length());
                textprintf(buffer, font,  0, 30, WHITE, "capacity: %d", edittext.capacity());
                textprintf(buffer, font,  0, 40, WHITE, "empty?:   %d", edittext.empty());
                if(insert)
                textout(buffer, font, "Inserting", 0, 50, WHITE);
                else
                textout(buffer, font, "Replacing", 0, 50, WHITE);
                
                // draw the caret
                vline(buffer, caret * 8, 8, 18, WHITE);
                
                // blit to screen
                blit(buffer, screen, 0, 0, 0, 0, 320, 240);
                
        }while(!key[KEY_ESC]); // end of game loop
        
        // clean up
        destroy_bitmap(buffer);
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        
        return 0;
}
END_OF_MAIN()