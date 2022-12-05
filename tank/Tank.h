#include "keycfg.h"
#include "defense.h"
#include "specials.h"

#define	DEFAULT_TANK_HEALTH		100


typedef	struct
{
	char		name[64];
	int			ammo;
} playerweapons_t;

typedef	struct
{
	char		name[64];
	int			amount;
} playerdefense_t;

typedef struct
{
	char		name[64];
	int			amount;
} playerspecials_t;



// Flags for the tank (modify how the tank is drawn)
#define		TANKFLAG_NORMAL				0
#define		TANKFLAG_PARACHUTE			2
#define		TANKFLAG_FLOATATION			4
#define		TANKFLAG_MONKEYMORTAR		8

// Flags for special items
#define		TANKSPECIAL_NONE			0
#define		TANKSPECIAL_LASERSIGHT		1
#define		TANKSPECIAL_USEDTRACER		2
#define		TANKSPECIAL_MADESHOT		4
#define		TANKSPECIAL_AUTODEFENSEWAIT	8		// Multiplayer only: wait for other clients to finish with
												// autodefense
#define		TANKSPECIAL_MADETELPOSITIVE	16		// Shot the positive counter part to the TEL super weapon


#define		TANK_LANDSIT				4		// Numer of pixels down from on top of the ground

#define		TANK_OPACITY				50


// Different Types of skill levels
#define		TANKSKILL_HUMAN				0
#define		TANKSKILL_STOOPID			1
#define		TANKSKILL_LOBBER			2
#define		TANKSKILL_SHOOTER			3
#define		TANKSKILL_GUNNER			4
#define		TANKSKILL_SNIPER			5

// Different tank states
#define		TANKSTATE_PLAYING			1
#define		TANKSTATE_DEFENSE			2
#define		TANKSTATE_WEAPONSEL			3

// Update flags for multiplayer
#define		TUPDATE_HEALTH				1
#define		TUPDATE_X					2
#define		TUPDATE_Y					4
#define		TUPDATE_DEG					8
#define		TUPDATE_POWER				16
#define		TUPDATE_ACTIVEWEAPON		32
#define		TUPDATE_FLAGS				64
#define		TUPDATE_SPECIALFLAGS		128
#define		TUPDATE_ENERGY				256
#define		TUPDATE_FUEL				512

#define		TANKTEAM_RED				1
#define		TANKTEAM_BLUE				2


//
// Standard class for a player's tank
//

class TANK
{
public:

  float x,y,oldx,oldy;
	float	xvel,yvel;
	int	indexnum;
	int	index;

	int	team;
    
  int sizex,sizey;

  float lpower, oldlpower;
	
	// Varibles for multiplayer
	netsock_t			*netsock;
	netaddress_t	address;
	packet_t			*inbuf;
	packet_t			*outbuf;
	int					lasttic;		// Last gametic we got info from this guy
	int					netstate;		// Tells us what the client is doing (ingame, buying, setting up..etc)
	
	int					ping;
	int					framedelay;
	
	int					lastshotnetid;	// netid of the last shot
	int					spectator;		// Spectating?
	int					pingwait;		// Waiting for client?
	bool				pingwaitReady;

	int					barrelshotx;	// Two varibles that get calculate so we know where to spawn
	int					barrelshoty;	// shots.

	// Functions for multiplayer
	void				Disconnect(void);
     
	// Tank (player) color
	int					colorr, colorg, colorb;
	int					kills;
	int					score;
	int					character;
	int					tankbmp;
	char				name[64];
	int					skill;
	int					tankstate,oldtankstate;		// Human tank state (calculating shot, defense box...etc)
	int					idletime;					// While positive, computer AI does not run
	int					shieldidletime;				// Wait for shields to animate
	int					deathtics;					// Tank is playing a death sequence
	int					deathseq;					// Death sequence that is currently being played

	int					tankpos;					// Map position that the tank is occuping

	float				health, oldhealth, starthealth;
	int					healthbartrans;

	playerweapons_t		weapons[NUM_PROJECTILES];
	int					weaponsused[NUM_PROJECTILES];	// Keep track of weapons fired

	playerdefense_t		defense[NUM_DEFENSE];
	int					defenseused[NUM_DEFENSE];		// Keep track of defense items used
	
	playerspecials_t	specials[NUM_SPECIALS];
	int					specialsused[NUM_SPECIALS];		// Keep track of special items used

	BITMAP				*cannon;
	int					flaretics;		// How long to draw the muzzle flare?

	int					lastweaponused;
	int					activeweapon, oldactiveweapon;
	int					activedefense;
	
	int					energyleft, oldenergyleft;		// Amount of defense energy left
	int					h,e,b;
	int					shieldh,shielde,shieldb;
	sprite_t		*shield;

	// For fuel and moving around
	int					startfuel, oldfuel;		// For keeping track of the % left

	int					money;
	int					needstodie;
	int					lasthit;					// Player who hit the tank last
	int					fallenpixels;				// Count the number of pixels the tank has falled from
	int					flags, oldflags;			// These flags are used to modify how the tank is drawn
	int					specialflags, oldspecialflags;
	float				napalmdamageh,
							napalmdamagee,
							napalmdamageb;	// Keeps track of how much napalm damage was delt out
	
	// Reserve damage, for things like flame throwers, am
	int					reservedamageh,reservedamagee,reservedamageb;

	bool				underwater;		// Are we underwater?
	int					bubbleticker;	// Underwater bubbles will not spawn until this is 0

	bool				falling;		// If true, this tank is currently falling. Do not allow any computers
										// to take their turn until all players have reached the ground
	bool				moving;			// Is the player controlling the tanks movements?

	// This is for HUD display
	BITMAP				*hudleft;
	BITMAP				*hudright;

	float				frozen;
	
	int					angle;
	float				deg, olddeg;
  int					exist;

	float				targetpower;
	unsigned char		targetangle;

	int					lastinflictor;	// Player who hit us last

	// Tank speech
	int					textlettercount;
	int					textlettertimer;
	int					texttimer;
	char				textbuf[256];
	char				textblitbuf[256];
	int					textx;
	int					texty;
	int					textw;
	int					texth;

	// Tank sounds
	SAMPLE				*snd;
	void	StartSound(int sound, int loop=0, int stopcurrent=0);

	void	SetupBotCharacter(int character);
	void	GiveMoney(int amount);
	void	GivePoints(int amount);
	bool	SwitchWep(void);
	void	CycleWeapons(void);
	void	CycleWeaponsReverse(void);
	void	FindGoodXPOS(void);
	void	DefaultAngle(void);
	void	TankThinker(void);
	void	init(int i, int a, int b, int d, int e,int f, int c);
	void	setcolor(int r, int g, int b);
	void	SetTeamColor(int t);
	bool	IsSameTeam(int tank);
	void	AutoAssignTeam(void);
	void	SwitchTeam(void);
  int		CheckSteepness(int x,int y, int direction);
	int		CheckFallHeight(int x, int y, int direction);
  int		move(float a, float b);
	void	load(void);
	void	handleinput(void);
  void	shoot(void);
	void	Onground(BITMAP	*bmp);
	bool	IsOnGround(BITMAP *bmp);
	void	DrawHUD(BITMAP *b);
	void	DrawPowerEnergy(BITMAP *b, int opacity);
	void	DrawNameBox(BITMAP *b);
  void	fall(BITMAP *bmp, BITMAP *wbmp);
	void	tankfloat(BITMAP *bmp, BITMAP *wbmp);
	void	ApplyFallingDamage(void);
	void	Damage(float damageh,float damagee,float damageb, int inflictor, bool killtank=true, bool damageshields=true, bool scattertext=false);
	void	KillShield(void);
	void	Kill(void);
	void	SetHEB(int sh, int se, int sb);
	void	SetShieldHEB(int sh, int se, int sb);
	void	SetOldVars(void);
	void	FindMapPosition(void);

	// Flare tic stuff
	void	SetFlareTics(int tics);
	void	SetPhotonTics(int tics);

	// Computer AI stuff
	int		AdjustAngleGivenClearance (int targetx, int targety);
	void	LaserSight(BITMAP *buf);
	int		SelectTarget(void);
	void	MyTurn(void);
	int		BuyDefense(int pnum, int amount);
	int		BuyOffense(int pnum, int amount);
	void	ComputerBuyStuff(void);
	bool	AttemptSniperWeapon(int wep);
	void	SelectRandomWeapon(void);
	void	SelectTargetWeapon(void);
	void	SetupDefense(void);
	void	SetupShields(void);

	int		targettank;
	int		targetx,targety;
	BITMAP				*bmp;
	void				draw(BITMAP *b);
   
};


typedef struct	
{
	char	*name;
	int		count;
	int		refnum;
}weaponinv_t;



extern		int					tank_showstats;
extern		int					checktanktimer;

extern		int					tank_shottimer;
extern		int					tank_orginalshottimer;

// Computer AI aiming stuff
extern		unsigned char		bestangle;
extern		float				bestpower;

// Turn ques
extern		int					tankturnque[MAX_TANKS];
extern		int					tankturnnextque[MAX_TANKS];


// A bunch of other functions
void	CycleWeapons(int player);
void	GiveAmmo(int	player, int ammotype, int	amount);
void	KillTanks(bool		advanceturn=true);
int		OnGround(int x, int y, BITMAP *bmp);
int		OnTopGround(int x, int y, BITMAP *bmp);
void	DeAllocateTankMemory(void);
void	ClearAllShotFlags(void);
void	CheckTankReserveDamage(int inflictor);
void	CheckTankDeaths(void);
void	SetTankTimer(int time, bool broadcast=false);

bool	AllPlayerBots(void);
bool	TanksFalling(void);
bool	AreTanksFalling(void);	// Same thing, only in realtime though

void	Tank_SetOldVars(void);
void	ToggleScoreBoard(void);
void	DrawTankScoreBoard(BITMAP *b);
void	TankStatDrawer(BITMAP *b);
void	CommonPlayerInputDrawer(BITMAP *b);
void	DrawHEBStats(BITMAP *buf);
void	DrawAllHebStats(BITMAP *buf);
void	Start_TankMessage(void);
int		IsTankMessage(void);
void	TankMessage_Input(void);
void	TankMessage_Drawer(BITMAP *b);
void	TankMessage_Break(void);
void	ResetTanks(void);

// Main thinker for all players in the game
void	TankThinker(void);
int		Aim(int origx, int origy, int	targetx, int targety, float deg, int length);

double	_sc_traj_power(double deltax, double deltay, double angle);

// Shot clock
void	Tank_ResetShotClock(int	timer);
void	Tank_ShotClockTicker(void);
void	Tank_ShotClockDrawer(BITMAP *b);

// Turn arrow
void	SpawnTurnArrow(int tank);

// Weapon selection dialog box
void	StartWeaponsList(int	player);
