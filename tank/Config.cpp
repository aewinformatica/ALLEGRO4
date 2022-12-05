#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "dialogs.h"
#include "scorch.h"
#include "secrets.h"

int				nosound=0;
int				nomusic=0;
int				demomode;

cvar_t			*fullscreen;
cvar_t			*novideoanimation;
cvar_t			*hideHUD;

cvar_t			*musicvolume;
cvar_t			*soundvolume;

cvar_t			*cashstart;
cvar_t			*walltype;
cvar_t			*gravityflags;

cvar_t			*talkingtanks;
cvar_t			*useKeyboard;

cvar_t			*shotclocktime;
cvar_t			*intermissiontime;
cvar_t			*notifymasterserver;

void	SetConfigDefaults(void)
{
	fullscreen =				Cvar_Get("fullscreen", "0", CVAR_ARCHIVE);
	novideoanimation =	Cvar_Get("novideoanimation", "0", CVAR_ARCHIVE);
	hideHUD =						Cvar_Get("hideHUD", "0", CVAR_ARCHIVE);

	musicvolume = Cvar_Get ("musicvolume", "200", CVAR_ARCHIVE);
	soundvolume = Cvar_Get ("soundvolume", "255", CVAR_ARCHIVE);

	cashstart = Cvar_Get ("cashstart", "50000", CVAR_ARCHIVE);
	walltype = Cvar_Get ("walltype", "0", CVAR_ARCHIVE);
	gravityflags = Cvar_Get ("gravityflags", "0", CVAR_ARCHIVE);

	talkingtanks = Cvar_Get ("talkingtanks", "0", CVAR_ARCHIVE);
	useKeyboard = Cvar_Get ("usekeyboard", "1", CVAR_ARCHIVE);	usekeyboard = true;

	shotclocktime = Cvar_Get ("shotclocktime", "2000", CVAR_ARCHIVE);	// 40*60
	intermissiontime = Cvar_Get ("intermissiontime", "5000", CVAR_ARCHIVE);	// 90*60
	notifymasterserver = Cvar_Get ("notifymasterserver", "0", CVAR_ARCHIVE);
}

void	LoadConfig(void)
{
	const	char	*val;

	// Always on
	drawscoreboard = 10;

	SetConfigDefaults();

	//Cmd_ExecuteString("exec cdirt.cfg");
	Cbuf_AddText("exec cdirt.cfg\n");

	// check if keyboard is turned off
	usekeyboard = (useKeyboard->value > 0);


	// Over-write all timers with command line options?
	val = Args.CheckValue ("-shotclock");
	if (val)
		Cvar_SetValue("shotclocktime", atoi(val)*TICRATE);

	val = Args.CheckValue ("-intertimer");
	if (val)
		Cvar_SetValue("intermissiontime", atoi(val)*TICRATE);
}


void	SaveConfig(void)
{
	FILE	*f;

	f = fopen ("cdirt.cfg", "w");
	if (!f)
	{
		Printf ("Couldn't write cdirt.cfg.\n");
		return;
	}

	fprintf (f, "// --------------------------------------\n");
	fprintf (f, "// CHARRED DIRT CFG\n");
	fprintf (f, "// --------------------------------------\n\n");
	fclose (f);

	Cvar_WriteVariables ("cdirt.cfg");

	// Also, write the secret file varibles as well	// Degauss why???? 
//	WriteSecretStuff();
}
