#pragma once
#include <allegro.h>
#include <string>
#include <iostream>
using namespace std;

const int QTD = 9;

struct Image
{
    
    BITMAP *img;
    string descricao;
    void Carregar( string filename )
    {
        string file;
        descricao = filename;
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
        BITMAP *Buffer;
        
        ImageManager();
        
        ~ImageManager();
        
        void Draw( string name, int x, int y, int w, int h, int fx, int fy );
        
        void Draw( int index, int x, int y, int w, int h, int fx, int fy );
};


