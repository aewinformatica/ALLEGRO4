#include <al/alut.c>
#include <iostream>

#include "ogg.h"

using namespace std;


int main(int argc, char* argv[])
{
    ogg_stream ogg;

    alutInit(&argc, argv);

	try
	{
        if(argc < 2)
            throw string("oggplayer *.ogg");
	
		ogg.open(argv[1]);

        ogg.display();

		if(!ogg.playback())
            throw string("Ogg refused to play.");
                    
        while(ogg.update())
        {
            if(!ogg.playing())
            {
                if(!ogg.playback())
                    throw string("Ogg abruptly stopped.");
                else
                    cout << "Ogg stream was interrupted.\n";
            }
        }
                    
        cout << "Program normal termination.";
        cin.get();
    }
    catch(string error)
	{
		cout << error;
        cin.get();
	}
	
	ogg.release();
	
	alutExit();

	return 0;
}
