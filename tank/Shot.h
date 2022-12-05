// The shot.h header file takes care of all the special 256 palette handeling.

#define			MISSILE_STANDARD		0
#define			MISSILE_NAPALM1			1
#define			MISSILE_NAPALM2			2
#define			MISSILE_NOSMOKE			4
#define			MISSILE_NOBUBBLES		8
#define			MISSILE_NOLIST			16
#define			MISSILE_NOHITSELF		32		// Wont clip the tank who shot it
#define			MISSILE_NODIRTCLIP		64		// Projectile will cut through dirt
#define			MISSILE_FLOAT			128		// Projectile will float on water
#define			MISSILE_NOBOTUSE		256		// Bots will not use this
#define			MISSILE_NOGRAVITY		512
#define			MISSILE_NOTANKCLIP		1024
#define			MISSILE_BEAM			2048	// Projectile is a instant hit
#define			MISSILE_CRUISELAND		4096	// Travels over landscapes
#define			MISSILE_NAPALM			8192
#define			MISSILE_PLASMA			16384
#define			MISSILE_WATER			32768
#define			MISSILE_INSTANTHIT		65536	// Bullet shot
#define			MISSILE_GHOST			131072	// Invisable w/ no thinkers
#define			MISSILE_DIRT			262144	// Dirt creator
#define			MISSILE_BOTUSEONLY		524288	// Bots only are allowed to buy this item			
#define			MISSILE_NOSPRITECLIP	1048576	// Wont clip any shootable sprites
#define			MISSILE_ALWAYSBOUNCE	2097152 // Will bounce of the screen no matter what

#define			MICON_3HEAT				1
#define			MICON_2HEAT				2
#define			MICON_1HEAT				4
#define			MICON_3ENERGY			8
#define			MICON_2ENERGY			16
#define			MICON_1ENERGY			32
#define			MICON_3BALLISTIC		64
#define			MICON_2BALLISTIC		128
#define			MICON_1BALLISTIC		256
#define			MICON_LIQUID			512
#define			MICON_EXTRASHOT			1024
#define			MICON_AI				2048
#define			MICON_SUPERHEAT			4096
#define			MICON_SUPERENERGY		8192
#define			MICON_SUPERBALLISTIC	16384	
#define			MICON_3CONVENTIONAL		32768
#define			MICON_2CONVENTIONAL		65536
#define			MICON_1CONVENTIONAL		131072
#define			MICON_SUPERCONVENTIONAL	262144



extern PALETTE		firepal;
extern PALETTE		riotpal;
extern PALETTE		greenpal;
extern PALETTE		blueflame;
extern PALETTE		darkbluepal;
extern PALETTE		fpurplepal, forangepal, fredpal, fvoiletpal;

typedef		struct
{
	char	name[64];									// Name of projectile
	char	gamename[64];							// Name that will show up on the HUD in case the normal name
																	// is too long
	int		iconflags;								// Flags that determine how to represent this weapon as icons
	int		flags;										// Special flags that this projectile uses
	int		cost;											// Starting price to purchase at
	int		numrecieve;								// Number you get w/ each purchase
	bool	rotates;									// Will we be rotating this?
	int		damageh,damagee,damageb;	// HEB damage
	int		expradius;								// Radius of explosion
	int		detonatespeed;						// Detonation speed (in tics)
	int		expspeed;									// Explosion speed (in tics)
	int		exptime;									// How long shall we do this for? (in tics)
	//_state	expsprite;						// Explosion sprite
	//int		expspriteflags;					// Flags to be passed to the explosion sprite?
	void	(* explodefunc)(int shot);
	_state   state;									// An actual sprite??
	PALETTE *usepal;								// Palette to use for generated explosions
	void	(* func)(int shot);				// Run this function every tic for special missile instructions
	void	(* hitfunc)(int shot, int tankhit);		// Called when a missile hits a tank
	void	(* groundfunc)(int shot);	// Called when a missile hits a tank
	int		firesound;								// Sound the tank makes when missile is fired
	int		explodesound;							// Sound the missile makes when it explodes
	int		thischaracter;						// This weapon can only be used by this specified character

	// For napalm shots
	int		pixelfill;								// Number of pixels to fill up
	float	pixeldamageh;							// Damage each particle does
	float	pixeldamagee;				
	float	pixeldamageb;				
	int		pixelpower;								// Pixel force
	int		pixeltime;								// Time napalm stays on the ground
	int		pixeldamagetime;					// Time before damage is no longer calculated
	int		pixelcolorr, pixelcolorg, pixelcolorb;	// Pixel color
} projectile_t;


typedef enum
{
		W_BABYMISSILE,
		W_MINIMIRV,
		W_MINIMIRVCHILD,
		W_NEEDLEMISSILE,
		W_MISSILE,
		W_TANKGIB,
		W_WATERMINE,
		W_DIRTBALL,
		W_TONODIRT,
		W_RIOTBLAST,
		W_HEAVYRIOTBLAST,
		W_WATERSPLASH,
		W_MOLTENROD,
		W_ELECTRICLASER,
		W_UZISPRAY,
		W_UZISPRAYSHOT,
		W_TOWERPOD,
		W_TRACERS,
		W_AFRAMM,
		W_ELECTRICARC,
		W_FLAMETHROWER,
		W_MIRV,
		W_MIRVSHOT,
		W_FUNKYBOMB,
		W_FUNKYBOMBCHILD,
		W_FIRETOAD,
		W_FIRETOAD_P1,
		W_FIRETOAD_P2,
		W_BABYNUKE,
		W_MISSILETURRETT,
		W_HEAVYROLLER,
		W_FIREBALL,
		W_SPARKBALL,
		W_DRILL,
		W_IONBATTERY,
		W_IONBATTERYCHILD,
		W_HEATCANNON,
		W_LEADSHELL,
		W_CANNONBALL,
		W_BLUEMISSILE,
		W_SHOTGUN,
		W_SHOTGUNSHOT,
		W_FIREFLY,
		W_FIREFLYCHILD,
		W_PROTONFLARE,
		W_PROTONCHILD,
		W_MACHINEGUN,
		W_MACHINEGUNSHOT,
		W_LASERPULSE,
		W_TUNNELLASER,
		W_URANIUMSHELL,
		W_LARGESPARKBALL,
		W_HEATWAVE,
		W_EMPMISSILE,
		W_BIGLAVAROCK,
		W_NAPALM,
		W_ACID,
		W_PLASMA,
		W_HEATSENTRYPOD,
		W_ENERGYSENTRYPOD,
		W_BALLISTICSENTRYPOD,
		W_MAGGEN,
		W_HEATRAY,
		W_IONCANNON,
		W_RAILGUN,
		W_SUPERNAPALM,
		W_SUPERPLASMA,
		W_SUPERACID,
		W_NUKE,
		W_MORTARSHELL,
		W_MOONSHOT,
		W_LIGHTNINGROD,
		W_FLAK,
		W_FLAKSHARD1,
		W_FLAKSHARD2,
		W_FIRESTARTER,
		W_DEATHSHEAD,
		W_DEATHSHEADCHILD,
		W_GIGANUKE,
		W_MONKEYMORTAR,
		W_POLARISMISSILE,
		W_BLACKHOLEBOMB,
		W_TWINELECTRICLANCES,
		W_TWINELECTRICLANCES_NEGATIVE,
		W_FUNKYNUKE,
		W_FUNKYNUKECHILD,
		W_AIRSTRIKE,
		W_OBCANNON,
		W_SUPERRAIL,
		W_SUPERRAILCHILD,
		W_RINGOFFIRE,
		W_SILVERBULLET,
        NUM_PROJECTILES
} projectile;


extern	projectile_t	projectiles[NUM_PROJECTILES];

// GENERIC FUNCTIONS
int			FindLineDistance		(float x1, float y1, float x2, float y2);


#define	SSTATE_INAIR			1
#define	SSTATE_EXPLODING		2
#define	SSTATE_NAPALMEXPLODE	3
#define	SSTATE_IDLE				4
#define SSTATE_FINISHING		5

// Varible for unlimited ammo
#define	UNLIMITED_AMMO			9398426


class SHOT
{   
public:

	float		x,y,xvel,yvel;
	int			angle;
	int			shottics;	// How long has the shot been active.
	int			flags;

	SAMPLE		*snd;		// For simply playing sounds
	int			sndvoice;

	sprite_t	*sprite;	// Sprite that we're binded too

	// Color of explosion
	PALETTE		*pal;

	int rad,color;

    int	tankfrom;			// The player who shot this
	int	indexnum;			// Our number in the array
	int exist;
	int	state;				// Current state of the projectile (in air? exploding?)
	int index;				// Blow up flag for do_line
	int	projectile;			// This identifies the missile on the projectiles[] array

	// The following 4 varibles are normally related to napalm/liquid shots
	int		life;				// Number of tics before missile will die automatically
	int		pixelfill;
	int		pixelfillcolor;
	int		damagelife;			// Number of tics to dish out lava damage
	bool	underwater;
	bool	onfloor;
	
	float	burntime;			// Tics before the projectile begins to lose velocity
	float	burnvel;

	
	// The following 6 varibles are used for explosions and their effects
	int		damagedone;
	int		damagetics;
	int		groundpixels;		// How many pixels the missile has traveled into the ground
	float	indexrate;
	int		colorindex;
	int		startcolor, usecolor, done;
	//////////////////////////////////////////////////////////////////////

	// Orginating x & y coordinates and angle from which this was shot
	float		origx, origy, origdeg;
	float		origxvel;
	float		origyvel;

	// Bouncing missiles
	int			bouncecount;
	
	// For multiplayer use only
	float		spawnx,spawny,spawndeg;
	float		spawnxvel, spawnyvel;

	BITMAP		*boombmp;

	// Misc stuff
	int			misc1,misc2;

  void	Init(int index, int projectiletype, float ax, float ay, float axvel, float ayvel, float adeg);
	void	InstantHit(int traceshot=0);
  void	draw(BITMAP *b);
	void	ThrowDirt(void);
	void	Blowup(BITMAP *bmp, int	damage);
  void	Thinker(void);
	void	explode(bool multiplayer_override=false);
	void	ExplodeEffect(BITMAP *buf, int damage, int tics, int speed);
	void	DeployDamage(int damage);
	int		inside(float x1, float y1, float x2, float y2);
	int		CheckShot(float shotx, float shoty);
	void	NapalmTanks(void);
	void	StartSound(int sound, int loop=0, int stopcurrent=0);
	void	StopSound(void);
	int		CheckSteepness(int destx,int desty);
	void  SHOT::deflect(int magX, int magY, int decay, int power);

};


int		numshots(void);

// Main thinker for all shots in the air
void	ShotThinker(void);
void	do_float_line(BITMAP *bmp, float x1, float y1, float x2, float y2, int d, void (*proc)(BITMAP *, float, float, int));

// Misc functions
void	KillTankSentryPods(int tank);
void	ApplyNapalmDamage(int tankfrom);

void	BCast_Shot(int s);
