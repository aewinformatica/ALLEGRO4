#include "fmod.h"
#include "fmod_errors.h"    // fmod optional

extern		int		stream_channel;

// Music stuff
int		PlayLevelOGG(char *filename);
int		StopLevelOGG(void);

void	StartBufferStream(void);
void	StopBufferStream(void);
void	InitMusic(void);
void	SetMusicVolume(void);
void	ShutdownMusic(void);


void	StartSound(int sound);
void	StopAllSounds(void);
