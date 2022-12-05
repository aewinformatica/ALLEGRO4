#pragma once
#include <allegro.h>
#include <string>
#include <iostream>
using namespace std;

const int QTD = 7;

struct Image
{
    
    BITMAP *img;
    string description;
    void Setup( string filename )
    {
        string file;
        description = filename;
        file = "data/";
        file += filename;
        file += ".bmp";

        img = load_bitmap( file.c_str(), NULL );
    }
};


class ImageManager
{
    private:
    public:        
        Image SPRITE[QTD];
        BITMAP *buffer;
        
        ImageManager();
        
        ~ImageManager();
        
        void Draw( string name, int x, int y, int w, int h, int fx, int fy );
        
        void Draw( int index, int x, int y, int w, int h, int fx, int fy );
};


