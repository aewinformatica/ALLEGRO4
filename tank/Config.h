
//
// Config.h
//

#define		CFG_FILE		"cdirt.cfg"

// External cvars
//

extern	cvar_t*		fullscreen;
extern	cvar_t*		novideoanimation;
extern	cvar_t*		hideHUD;

extern	cvar_t*		musicvolume;
extern	cvar_t*		soundvolume;
extern	cvar_t*		cashstart;
extern	cvar_t*		walltype;
extern	cvar_t*		gravityflags;
extern	cvar_t*		talkingtanks;
extern	cvar_t*		shotclocktime;
extern	cvar_t*		intermissiontime;
extern	cvar_t*		useKeyboard;
extern	cvar_t*		notifymasterserver;

//
// Externals
extern		int			nosound;
extern		int			nomusic;
extern		int			demomode;

//
// External functions
void	SetConfigDefaults(void);
void	LoadConfig(void);
void	SaveConfig(void);
