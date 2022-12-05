#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"
#include "editor/editor.h"

FSOUND_STREAM	*stream = NULL;
int				stream_channel = -1;

void	InitMusic(void)
{
	if (nomusic > 0)
		return;

	FSOUND_Stream_SetBufferSize(1000);
	FSOUND_SetBufferSize(150);

	if (!FSOUND_Init(44100, 32, 0))
    {
#ifdef		DEDICATED_SERVER
		Printf("Error  --  Unable to start FMOD\n");
#else
		alert("Error", "Unable to start FMOD", NULL, "Ok", NULL, 0,0);
#endif
    }
	else
		Printf("InitMusic: FMOD started sucessfully.\n");
}


void	ShutdownMusic(void)
{
	if (nomusic > 0)
		return;

	Printf("ShutdownMusic: Shutting down FMOD\n");

	FSOUND_Stream_Close(stream);
	FSOUND_Close();
}




void	BufferStream_Proc(void)
{
	if (!stream)
		return;

	if (stream_channel < 0)
    {
		stream_channel = FSOUND_Stream_Play(stream_channel, stream);
		FSOUND_SetPaused(stream_channel, FALSE);
	}
}
END_OF_FUNCTION(BufferStream_Proc);



void	StartBufferStream(void)
{
	install_int_ex(BufferStream_Proc, BPS_TO_TIMER(2));//SECS_TO_TIMER(1));
}

void	StopBufferStream(void)
{
	remove_int(BufferStream_Proc);
}


int		LoadLevelOGG(char *filename)
{
	char		path[128];

	// Disabled sound
	if (nomusic > 0)
		return 1;

	//if (stream != NULL)
	StopLevelOGG();

	// Cannot load anything if nothing is there
	if (ustrlen(filename) <= 0)
	{
		stream = NULL;
		return -1;
	}

	// Look for this in the music directory
	sprintf(path, ".\\music\\%s", filename);

	// Make sure file exists
	if (exists(path) == 0)
		return -1;

	stream = FSOUND_Stream_Open(path, FSOUND_LOOP_NORMAL, 0, 0);

	return 1;
}



int		PlayLevelOGG(char *filename)
{
	char	path[128];

	if (nomusic > 0)
		return 1;

	if (stream != NULL)
		StopLevelOGG();

	// Look for this in the music directory
	sprintf(path, ".\\music\\%s", filename);

	stream = FSOUND_Stream_Open(path, FSOUND_LOOP_NORMAL, 0, 0);

	if (!stream)
		return -1;

#ifdef	_DEBUG
	//Printf("Playing MUSIC: %s\n", path);
#endif


	//stream_channel = FSOUND_Stream_Play(stream_channel, stream);
	stream_channel = FSOUND_Stream_Play(FSOUND_FREE, stream);
	FSOUND_SetVolume(stream_channel, Cvar_VariableValue("musicvolume"));
	return 1;
}


int		StopLevelOGG(void)
{
	if (nomusic > 0)
		return 1;

	if (FSOUND_Stream_GetOpenState(stream) == 0)
	{ FSOUND_Stream_Close(stream); }
	stream_channel = -1;
	stream = NULL;

	return 1;
}




//
// StartSound - Generic StartSound function
//

void StartSound(int sound)
{
	if (nosound > 0)
		return;

	play_sample((SAMPLE *)sounddata[sound].dat, Cvar_VariableValue("soundvolume"), 128, 1000, 0);
}

void	StopAllSounds(void)
{
	int		i;
	
	// 256 is defined as VIRTUAL_VOICES or the max number of voices that
	// allegro handles
	for (i=0; i<256; i++)
	{
		deallocate_voice(i);
	}
}

//
// SetMusicVolume - Sets FMODS music volume
//

void	SetMusicVolume(void)
{
	if (nomusic > 0)
		return;

	if (stream_channel < 0)
		return;

	FSOUND_SetVolume(stream_channel, Cvar_VariableValue("musicvolume"));
}
