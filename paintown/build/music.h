#ifndef _music_class_h
#define _music_class_h

#include <string>
#include <vector>

using namespace std;

struct AL_DUH_PLAYER;
struct DUH;

/* The music class. Dont be late or youll get an F!
 */
class Music{
public:
	Music();

	~Music();

	static bool loadSong( const char * song );
	static bool loadSong( const string & song );

	/* load one of the songs in 'songs' */
	static void loadSong( const vector< string > & songs );

	static void pause();
	static void play();
	static void soften();
	static void louden();
	static void fadeIn( double vol );
	static void fadeOut( double vol );
	static void setVolume( double v );
	static double getVolume();
	static void mute();
	static void unmute();
	
	void doPlay();

protected:

	void _setVolume( double vol );
	void _play();
	void _pause();
	void _soften();
	void _louden();
	void _fadeIn();
	void _fadeOut();

	bool playing;

	int fading;

	bool internal_loadSong( const char * path );

	AL_DUH_PLAYER * player;
	DUH * music_file;
};

#endif
