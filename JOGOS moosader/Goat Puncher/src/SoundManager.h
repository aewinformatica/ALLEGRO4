#pragma once
#include <allegro.h>
#include <string>
#include <iostream>
using namespace std;

const int SNDAMT = 12;

struct Sound
{
    SAMPLE *snd;
    string description;
    void Setup( string filename )
    {
        cout<<"\nSetup sound "<<filename<<endl;
        
        description = filename;
        
        //Setup path to load from
        string file = "data/";
        file += filename;   
        file += ".wav";
        
        cout<<"Load in "<<file<<endl;
        
        snd = load_sample( file.c_str() );
        
        if ( snd != NULL ) { cout<<"Success!"<<endl; }
        else { cout<<"Error loading!"<<endl; }
    }
};

class SoundManager
{
    private:
        float soundCounter[4];
    public:
        
        Sound sound[SNDAMT];
        
        void DecCounter();
        
        SoundManager();
        
        ~SoundManager();
        
        void Play( int index );
        
        void Play( string name );     
};


