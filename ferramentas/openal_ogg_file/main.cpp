/************************************************************************************************************************
	The base code for this tutorial was taken from  Jesse Maurais's Lesson 8 OpenAL tutorial, which was converted to MS Dev 
	Studio 6 by TheCell (TheCell61@hotmail.com).  The only things that have changed are the ogg load functions, and a few bits 
	and peices throughout the code.  Have fun...

	Spree (SpreeTree@hotmail.com)
************************************************************************************************************************/


#include <al\alut.h>
#include <iostream>

#include "ogg.h"			//Custom ogg.h provided as the source, not the one included with the Ogg Vorbis SDK

using namespace std;


int main(int argc, char* argv[])
{
    ogg_stream ogg;

    alutInit(&argc, argv);

	try
	{
		// I just changed this toa specific file, bacuse i was having trouble with 'drag-n-drop' :s
		ogg.open("bomb.ogg");

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
