/*
		Scorch.h 
*/
#define	MAX_GAMETEXT		25
#define	MAX_GAMELINES		10
#define MAX_TANKS				 8
#define	MAX_TANKTEAMS		(MAX_TANKS/2)
#define N_SHOTS					50

#include "cvars.h"
#include "multiplayer.h"
#include "snddat.h"
#include "sprites.h"
#include "ground.h"
#include "shot.h"
#include "tank.h"
#include "sedit.h"
#include "ppcol_src/ppcol.h"
#include "drectangles.h"
#include "music.h"

#include "cmd_args.h"
#include "conalleg.h"
#include "consolecmd.h"

#include "config.h"
#include "tankdeaths.h"

#define		VERSION				"Release Candidate v0.99R"
#define		WINDOW_TEXT			"Charred Dirt"

//				ADDED A RANDOM CHECK FUNCTION		DegaussRND
//				the tail is commented out when not required. ie a null define
#define		CheckRandomFunc(A)	// Printf("%s :%s[%d]\n", A, __FILE__, __LINE__);

#define		FILE_FORMAT_VERSION		10

// Running in the foreground or background?
#define		SWITCHSTATE_OUT				0
#define		SWITCHSTATE_IN				1

#define		NUM_NORMALCHARACTERS	8
#define		NUM_SECRETCHARACTERS	4
#define		NUM_BOTCHARACTERS			5

#define		NUM_CHARACTERS			(NUM_NORMALCHARACTERS + NUM_SECRETCHARACTERS + NUM_BOTCHARACTERS)

#define		CHAR_STOOPID			12
#define		CHAR_LOBBER				13
#define		CHAR_SHOOTER			14
#define		CHAR_GUNNER				15
#define		CHAR_SNIPER				16

#define		SCREEN_BPP				16
#define		SCREENWIDTH				800
#define		SCREENHEIGHT			600

// Number of tics per second or the max FPS the game will run
#define		TICRATE			 50

#define GROUND_START	390
#define	GROUND_COLOR	makecol(208,171,128)

// Some crazy math stuff
#define ACHANGE			256/360

#ifndef PI
#define PI				3.14159265358979323846		// matches value in gcc v2 math.h
#endif

#define TAN(A)  fixtof(ftan(itofix(A)))
#define COS(A)  fixtof(fcos(itofix(A)))
#define SIN(A)  fixtof(fsin(itofix(A)))

#define TRANSCOLOR		makecol(255, 0, 255)
extern int	CONCRETE;	// color of concrete now dynmic not static.
extern int	SOLID;  	// color of solid now dynmic not static.
extern int	GOLD;			// color of treasure.
extern int	goldX;		// x location of treasure.
extern int	goldY;		// y location of treasure.

#define	TANK_W			20
#define	TANK_H			14

#define CANNON_L		15
#define CANNON_W		2
#define CANNON_C		makecol(200, 200, 200)

#define MAX_LOAD		30

#define LIGHTGREEN	makecol(128, 255, 128)
#define RED					makecol(255, 0, 0)
#define BLUE				makecol(0,0, 255)

// Gamestate stuff
#define		MENU_MAIN					1
#define		MENU_OPTIONS			2
#define		MENU_GAME					3
#define		MENU_MULTIPLAYER	4

// Gamestates
#define	GS_LOADINGGAME	0
#define	GS_INGAME				1
#define	GS_MENU					2
#define	GS_TITLE				3
#define GS_EDITOR				4
#define	GS_CONNECTING		5
#define	GS_INTERMISSION	6
#define	GS_INTERWAIT		7

extern	int							gamestate;
extern	int							switchstate;
extern	volatile int		gametics;
extern	volatile int		update_count;
extern	volatile int		frame_count;
extern	bool						menuactive;
extern	int							mrbmmg_mode;

// Money amounts! (default)
#define	MONEY_STARTCASH			 50000
#define	MONEY_MAXCASHSTART 1000000
#define	MONEY_ROUNDWIN			 35000
#define	MONEY_NOROUNDWIN		 15000
#define	MONEY_KILL					 20000

#define GRAVITY			Gravity()

#define r_s(A,B,C,D,E) rotate_sprite(A,B,C-(int)(B->w/2),D-(int)(B->h/2),itofix(E))
#define p_s(A,B,C,D,E) pivot_sprite(A,B,C,D,(int)(B->w),(int)(B->h/2),itofix(E))
//#define d_s(A,B,C,D) draw_sprite(A,B,C-(int)(B->w/2),D-(int)(B->h/2))

// externals
extern BITMAP		*background;
extern BITMAP		*buffer;
extern BITMAP		*cannon;
extern	int			quit;
extern	int			tankturn;
extern	int			lasttankturn;
extern	int			lasttankshot;
extern	int			numplayers;
extern	int			numrounds;
extern	int			numround_setting;
extern	int			numplayer_setting;
extern	char		numplayer_string[32];
extern	char		numround_string[32];
extern	int			global_attr;
extern	int			level_filecount;
extern	char		**level_filenames;
//extern	int			tankxpos[MAX_TANKS];
//extern	int			tankypos[MAX_TANKS];
//extern	char		logfilename[64];

#define	MP_CLIENT		0
#define	MP_SERVER		1

// Generic multiplayer stuff
extern	int			multiplayer;
extern	int			multiplayermode;

// Dirty rectangle routines
extern	int			redraw_ground;

#define	MAX_KILL_TEXT		20
#define	MAX_KILLED_TEXT		20
#define	MAX_SELFKILL_TEXT	20
#define	MAX_FALLKILL_TEXT	20

extern	char		*kill_text[MAX_KILL_TEXT];
extern	char		*killed_text[MAX_KILLED_TEXT];
extern	char		*selfkill_text[MAX_SELFKILL_TEXT];
extern	char		*fallkill_text[MAX_FALLKILL_TEXT];

// The buystuff menus has different 'states' to lookup and display
// certain menus. Basically because I'm too lazy to create
// seperate menu fuctions for all of these.
#define		BUYSTUFF_WEAPONS		0
#define		BUYSTUFF_DEFENSE		1
#define		BUYSTUFF_SPECIAL		2
#define		BUYSTUFF_SELLSTUFF	3

// All the totals for weapons, shields, special...etc added.
#define			MAX_SELL_ITEMS		(NUM_PROJECTILES+NUM_DEFENSE+NUM_SPECIALS)


#define SHOT_SIZE 		2
#define SHOT_COLOR		makecol(255, 255, 255);

#define LEFT 		-1
#define RIGHT		 1

extern SHOT				sh[N_SHOTS];
extern TANK				ta[MAX_TANKS];
extern GROUND			ground;
extern DATAFILE*	maindata;
extern DATAFILE*	sounddata;
extern DATAFILE*	spritedata;
extern DATAFILE*	alphadata;
extern int				mouseflags;
extern int				drawscoreboard;
extern bool				showtrace;
extern int				fastshot;
extern bool				ScriptedGame;
extern char				ServerClient;


extern	volatile int	fps;

// Generic functions
float	Gravity(float gv = 0);
void	LogStr(char	*str);
float	CalculateAngle(int ox, int oy, int dx, int dy);
float	FCalculateAngle(float ox, float oy, float dx, float dy);
void	SetShakeTimer(int	tics, int factor);
void	LoopDrawer(BITMAP *bmp);
void	Blit_Mouse(BITMAP *buf);
void	Screen_Blit(bool blitmouse=true);
int		findshot();
bool	shotinair(void);
int		numtanks(void);
int		numalivetanks(void);
int		numaliveteam(int team);
int		numingametanks(void);
bool	isallbots(void);
void	advanceturn(bool onlyadvance=false);
void	InitNextTurnQue(void);
void	InitDefaultTurnQue(void);
void	AddNextTurnQue(int player);
void	RemoveFromTurnQue(int	tank);
int		FindFirstTurn(void);
void	inittanks(void);
void	freetanks(void);
void	ticker();
void	stop_timer(void);
void	start_timer(void);
void	ScreenShot(void);
void	save_jpeg(char *filename,BITMAP * shotbuf);
void	quitgame(void);
void	endgame(bool ShowAlert=true);
void	end_game(void);	// for dialogs
void	joingame(void);
void	joinLANgame(void);
int		rANDom(void);
int		NumberOfPrizes(void);
int		GetPrizeCode(int index);
char* GetPrizeName(int index);
int		GetCodeIndex(char* code);
void	ShowMsg(char* msg);

void	StartNewGame(int nplayers);
void	StartLevel(bool	resetShotClock=false);

void	InGame_Runner(void);

// Dialog stuff
extern	DIALOG dialog_playersetup[];
extern	DIALOG dialog_buystuff[];

void		Kill_Dialog(DIALOG *dlg);
void		PS_Refresh(int player);

// Various wall-types
#define	WALLS_NORMAL			0
#define	WALLS_RUBBER			1
#define	WALLS_CEMENT			2
#define	WALLS_WRAP				3

// Option flags (can be more than 1)
// Originally was only used for gravity but left
// the "NOGRAVITY_" label as the indicator of a flag
#define	NOGRAVITY_NORMAL		0
#define	NOGRAVITY_GROUND		1
#define	NOGRAVITY_TANKS			2
#define	NOGRAVITY_WATER			4
#define	NOGRAVITY_CONCRETE	8
#define	NOGRAVITY_SOLID		 16
#define	NOGRAVITY_GOLD		 32
#define	NOGRAVITY_PRIZE		 64
#define	NOGRAVITY_NEXT		128

// GUI Buttons
#define		NUM_MAIN_BUTTONS				6
#define		NUM_SPMAIN_BUTTONS			3
#define		NUM_MPMAIN_BUTTONS			3
#define		NUM_MPTEAM_BUTTONS			4
#define		NUM_OPTIONS_BUTTONS			4
#define		NUM_GAME_BUTTONS				5
#define		NUM_MULTIPLAYER_BUTTONS	4

#define		BUTTON_NORMAL			0
#define		BUTTON_DISABLED		1

class	level_t
{
public:
	char	name[64];							// Name of level
	char	author[64];							// Author of this level
	char	leveldesc[128];						// Short description of the level
	char	song[64];
	char	filename[128];
	int		tankxpos[MAX_TANKS];
	int		tankypos[MAX_TANKS];
	bool	posoccupied[MAX_TANKS];
	int		flags;
	float	foreground_lightlevel;
	float	background_lightlevel;
	int		whiteflashtics;
	int		staticwaterlevel;
	int		watercolor;
	int		afterturnsprite_active;
	int		numshotsfired;

	// Extra level options
	int		walltype;
	int		gravityflags;

	// Secret stuff
	int		secretcharflag;
};

typedef struct {
	char	name[64];
	int		iconbmp;
	int		profilebmp;
	int		chartbmp;
	int		tankicon;
	int		smalltank;
	int		x;
	int		y;
	int		w;
	int		h;
	int		cannonypos;
	int		cannonxpos;
	int		cannonlength;
	int		attrib_h,attrib_e,attrib_b;			// Protection against heat, electric, balistic weapons (x/5)
	int		mu,md,ml,mr;						// Keeping track of where to move useing the keyboard
	int		unlocked;							// Whether this is an unlocked character or not
} character_t;

typedef	struct
{
	char	name[64];	// The name of course
	int		cost;		// Cost (like in $$)
	int		numrecieve; // Number you recieve
	int		refnum;		// Index reference to parent array
	int		num;		// Number current player has
	int		parent;		// Parent varible (BUYSTUFF_STATE, BUYSTUFF_DEFENSE...)
	int		flags;		// Inherit any flags from previous defense/weapons list
} menuitem_t;

typedef	struct {
	char		text[64];					// Text to display
	int			timer;						// How long it lasts (tics)
	float		x,y,xvel,yvel;				// Coordinates, velocity
	int			color;						// Inital color
	int			flags;						// Additional flags
	BITMAP		*bitmap;					// Bitmap that is drawn directly onto the buffer
	void		(*loopfunc) (int gtext);	// Function that will be called on every tic
	int			opacity;					// Transparent? (255=solid 0=clear)
} gametext_t;

typedef	struct {
	char		text[128];				// Text to display
	int			timer;					// How long it lasts (tics)
	float		x,y;					// Coordinates, velocity
	int			color;					// Inital color
	int			opacity;
	int			tankfrom;
} gameline_t;

typedef struct {
	bool	allowoverflow;	// if false, do a Com_Error
	bool	overflowed;		// set to true if the buffer size failed
	byte	*data;
	int		maxsize;
	int		cursize;
	int		readcount;
} sizebuf_t;

typedef struct {
	float		x;
	float		y;
	void		(*proc) ();
	char		text[128];
	int			flags;
	int			w,h;
	bool		active;
} button_t;

typedef	struct {
	char		name[64];
	button_t	*buttons;
	int			numbuttons;
	void		(*inproc) ();
	void		(*outproc) ();
} menu_t;

enum
{
	CHARACTER_MARIERICKY,
	CHARACTER_SARU,
	CHARACTER_APRIL,
	CHARACTER_JACK,
	CHARACTER_MIA,
	CHARACTER_LOCK,
	CHARACTER_MIKI,
	CHARACTER_MESHKO,
	CHARACTER_JIRA,
	CHARACTER_RICHIE,
	CHARACTER_SMACKBRINGER,
	CHARACTER_SHADOWFANG,
	//NUM_CHARACTERS,
};

extern	character_t	characters[NUM_CHARACTERS];
extern	level_t			currentlevel;

extern	button_t	main_buttons[NUM_MAIN_BUTTONS];
extern	button_t	options_buttons[NUM_OPTIONS_BUTTONS];
extern  button_t	game_buttons[NUM_GAME_BUTTONS];
extern	button_t	multiplayer_buttons[NUM_MULTIPLAYER_BUTTONS];
extern	menu_t		*activemenu;
extern	menu_t		main_menu[];
extern	menu_t		sp_main_menu[];
extern	menu_t		mp_main_menu[];
extern	menu_t		mpteam_main_menu[];
extern	menu_t		options_menu[];
extern	menu_t		game_menu[];
extern	menu_t		multiplayer_menu[];

// Startup stuff
void		InitSwitchState(void);
void		StartLog(void);
void		LoadAmmo(void);

// GUI Specific functions
void 		GUI_Thinker(void);
void 		GUI_GetInput(void);
void		GUI_SetupMenu (menu_t*	menu);
void		GUI_InitButtons(void);
int			GUI_DoDialog(DIALOG *dlg, int focus);

// In-Game stuff
void		InitFire(void);
void		LoadLevel(char	*filename);
void		ResetLevel(void);
//void		newlevel(int nplayers, bool newgame, char *filename, bool runbuystuff=true);
void		filecounter(const char *filename, int attrib, int param);		// level files
void		filearraystack(const char *filename, int attrib, int param);	// level files
void		GenerateLevelList(void);										// Generates a level list
char		*LoadRandomLevel(void);
void    CreateRandomLevel(const char *filename); // Creates a level if file failed to load.
int			CheckForShots(int	shot);
sprite_t*	CheckForSprites(int shot, int x, int y);
sprite_t*	CheckForSpriteXY(int x, int y);
bool		CheckPointInSprite(sprite_t *s, int x, int y);
void		SV_SpawnSprite(sprite_t*	sp);
bool		GUI_MouseInside(int x, int y, int w, int h);
float		ConvertFromGamePower(int	num);
float		ConvertToGamePower(float	num);
void		Idle(int time);

// Tank stuff
bool		CheckAfterTurnSprites(int tank);

// Title screen stuff
void		Title_Input(void);
void		Title_Drawer(BITMAP *b);
void		GUI_Drawer(BITMAP *b);

// Player setup menus
void		PS_RunSetup(int player);
void		PS_RunMultiplayerSetup(int player, int time);
void		PS_RunReClassFor(int player);
void		PS_RunReclass(void);
void		AddBot_Realtime(int	skill, char *name);

// Intermission functions
void	Inter_Run(char	*filename=NULL);
void	Inter_CheckGame(bool	advturn=true);
void	InterTicker(void);
void	InterDrawer(void);
void	Inter_Refresh(void);

int		Inter_GetBlueScore(void);
int		Inter_GetRedScore(void);
int		Inter_FindTeamWinner(void);

// Purchase Menu functions
void	StartBuyStuff(void);
void	BuyStuffFor(int player);

// Cheating BASRADS!!
void	cheat_checkcheats(void);

// Dynamic text stuff
////////////////////////////////
#define		TEXT_NORMAL				0
#define		TEXT_INVERSEGRAVITY		1
#define		TEXT_FADEOUT			2
extern		gametext_t		gametext[MAX_GAMETEXT];
extern		gameline_t		gamelines[MAX_GAMELINES];

int		SpawnGameText(char *text, int timer, int x, int y, int xvel, int yvel, int color, FONT *font = (FONT*)maindata[MS].dat, int flags=0);
void	DestroyGameText(int	ref);
void	DestroyAllGameText(void);
void	GameTextThinker(void);
void	DrawGameText(BITMAP *bmp);
void	TankSay(char	*text, int tank, int timer);
void	CharacterSay(char *text, int tank, int timer);

// Game lines
void	GameLinesDrawer(BITMAP *b);
void	GameLinesThinker(void);
void	G_Printf(char	*text, int tankfrom=-1, int color=makecol(255,255,255));

void	DrawPowerEnergyBar(BITMAP *b, int x, int y,
						   int w, int h,
						   int opacity,
						   int currenthealth,
						   int startinghealth,
						   int currentenergy=0,
						   int startingenergy=0,
						   int currentfuel=0,
						   int startingfuel=0);

int		AreTanksTalking(void);
void	TankSayThinker(int tank);

//////////////////////////////////////

// Level Editor
void	CDEdit(void);

// Console
extern	bool	console_active;
void	Init_Console(void);
void	DrawConsole(BITMAP	*b);
void	ToggleConsole(bool	conOn);
void	ShutDown_Console(void);
void	ConsoleInput(void);
void	Printf(char	*txt, ...);

// Cvar stuff
char	*CopyString (char *in);
