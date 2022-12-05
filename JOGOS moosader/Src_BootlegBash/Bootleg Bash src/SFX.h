#pragma once

class SFX
{
    public:
        int x, y;
        float frame;
        int img;
        bool exists;
        SFX() { exists = false; img = 0; frame = 0; x = 0; y = 0; }
        void IncrementFrame()
        {
            frame += 0.25;
            if ( frame >= 4.0 )
            {
                exists = false;
                frame = 0;
            }
        }
        void Setup( int tx, int ty, int image )
        {
            frame = 0;
            exists = true;
            img = image;
            if ( image == 0 )
            {
                x = tx - 8;
                y = ty + 16;
            }
            else if ( image == 1)
            {
                x = tx - 32;
                y = ty;
            }
            else if ( image == 2 )
            {
                x = tx + 16;
                y = ty;
            }
        }
};

