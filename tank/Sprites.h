// Include the header to the data file
#include "alphadat.h"
#include "sprdat.h"

// The max amount of sprites that can be showed at once
#define   MAX_SPRITES   150

// Only tagged sprites will be loaded on startup as having an alpha mask on them
#define		NORMAL_STATE	1
#define		HAS_ALPHA			2

// Sprite flags to determine different types of sprites
#define		MF_NORMAL					     1
#define		MF_TRANSLUC						 2
#define		MF_CENTERCOORD				 4
#define		MF_ALPHA							 8
#define		MF_ADVANCETURN				16
#define		MF_NOTVISABLE					32
#define		MF_USEANGLE						64
#define		MF_NOGRAVITY				 128
#define		MF_BUBBLEGRAVITY		 256
#define		MF_ONGROUND					 512
#define		MF_POSTTURNATTACK		1024
#define		MF_ALLOWSMALLSCALE	2048
#define		MF_COOLEFFECT1			4096
#define		MF_FLIPPED					8192
#define		MF_TPGRAVITY			 16384
#define		MF_SHOOTABLE		   32768
#define		MF_GROUNDCLIP			 65536
#define		MF_BOUNCE					131072		// Sprite bounces (only works with MF_GROUNDCLIP)
#define		MF_FOLLOWTANK			262144		// Sprites mirrors a tanks movements


typedef enum _state
{
    S_NULL,
		S_EXPLODE1_1,
		S_EXPLODE1_2,
		S_EXPLODE1_3,
		S_EXPLODE1_4,
		S_EXPLODE1_5,
		S_EXPLODE1_6,
		S_EXPLODE1_7,
		S_EXPLODE1_8,
		S_BABYMISSILE,
		S_MISSILE_1,
		S_MISSILE_2,
		S_BLUEMISSILE_1,
		S_BLUEMISSILE_2,
		S_BLACKMISSILE_1,
		S_BLACKMISSILE_2,
		S_IRONBALL,
		S_FIREBALL,
		S_FATBOY_1,
		S_FATBOY_2,
		S_SPLASH1,
		S_SPLASH2,
		S_SPLASH3,
		S_SPLASH4,
		S_BUBBLE_LARGE,
		S_BUBBLE_MEDIUM,
		S_BUBBLE_SMALL,
		S_SMOKE_GRAY_XLARGE,
		S_SMOKE_GRAY_LARGE,
		S_SMOKE_GRAY_MEDIUM,
		S_SMOKE_GRAY_SMALL,
		S_SMOKE_BLACK_XLARGE,
		S_SMOKE_BLACK_LARGE,
		S_SMOKE_BLACK_MEDIUM,
		S_SMOKE_BLACK_SMALL,
		S_SHIELD_ELECTRIC_1,
		S_SHIELD_ELECTRIC_2,
		S_SHIELD_ELECTRIC_3,
		S_SHIELD_ELECTRIC_4,
		S_SHIELD_ELECTRIC_5,
		S_SHIELD_ELECTRIC_6,
		S_SHIELD_FIRE_1,
		S_SHIELD_FIRE_2,
		S_SHIELD_FIRE_3,
		S_SHIELD_FIRE_4,
		S_SHIELD_FIRE_5,
		S_SHIELD_BALLISTIC_1,
		S_SHIELD_BALLISTIC_2,
		S_SHIELD_BALLISTIC_3,
		S_SHIELD_BALLISTIC_4,
		S_SHIELD_BALLISTIC_5,
		S_SHIELD_BALLISTIC_6,
		S_HEATWAVE,
		S_HEATWAVE_VERYSMALL1_1,
		S_HEATWAVE_SMALL1_1,
		S_HEATWAVE_MEDIUM1_1,
		S_HEATWAVE_LARGE1_1,
		S_HEATWAVE_XLARGE1_1,
		S_HEATWAVE_LARGE1_2,
		S_HEATWAVE_MEDIUM1_2,
		S_HEATWAVE_SMALL1_2,
		S_HEATWAVE_VERYSMALL1_2,
		S_HEATWAVE_VERYSMALL1_3,

		// Left flame
		S_HEATWAVE_LEFT_VERYSMALL2_1,
		S_HEATWAVE_LEFT_SMALL2_1,
		S_HEATWAVE_LEFT_MEDIUM2_1,
		S_HEATWAVE_LEFT_LARGE2_1,
		S_HEATWAVE_LEFT_MEDIUM2_2,
		S_HEATWAVE_LEFT_SMALL2_2,
		S_HEATWAVE_LEFT_VERYSMALL2_2,
		S_HEATWAVE_LEFT_VERYSMALL2_3,
		S_HEATWAVE_LEFT_VERYSMALL3_1,
		S_HEATWAVE_LEFT_SMALL3_1,
		S_HEATWAVE_LEFT_MEDIUM3_1,
		S_HEATWAVE_LEFT_SMALL3_2,
		S_HEATWAVE_LEFT_VERYSMALL3_2,
		S_HEATWAVE_LEFT_VERYSMALL3_3,
		S_HEATWAVE_LEFT_VERYSMALL4_1,
		S_HEATWAVE_LEFT_SMALL4_1,
		S_HEATWAVE_LEFT_VERYSMALL4_2,
		S_HEATWAVE_LEFT_VERYSMALL4_3,
		S_HEATWAVE_LEFT_VERYSMALL5_1,
		S_HEATWAVE_LEFT_VERYSMALL5_2,

		// Right flame
		S_HEATWAVE_RIGHT_VERYSMALL2_1,
		S_HEATWAVE_RIGHT_SMALL2_1,
		S_HEATWAVE_RIGHT_MEDIUM2_1,
		S_HEATWAVE_RIGHT_LARGE2_1,
		S_HEATWAVE_RIGHT_MEDIUM2_2,
		S_HEATWAVE_RIGHT_SMALL2_2,
		S_HEATWAVE_RIGHT_VERYSMALL2_2,
		S_HEATWAVE_RIGHT_VERYSMALL2_3,
		S_HEATWAVE_RIGHT_VERYSMALL3_1,
		S_HEATWAVE_RIGHT_SMALL3_1,
		S_HEATWAVE_RIGHT_MEDIUM3_1,
		S_HEATWAVE_RIGHT_SMALL3_2,
		S_HEATWAVE_RIGHT_VERYSMALL3_2,
		S_HEATWAVE_RIGHT_VERYSMALL3_3,
		S_HEATWAVE_RIGHT_VERYSMALL4_1,
		S_HEATWAVE_RIGHT_SMALL4_1,
		S_HEATWAVE_RIGHT_VERYSMALL4_2,
		S_HEATWAVE_RIGHT_VERYSMALL4_3,
		S_HEATWAVE_RIGHT_VERYSMALL5_1,
		S_HEATWAVE_RIGHT_VERYSMALL5_2,

		S_BABYNUKE,
		S_NUKE,
		S_SHELL,
		S_SPIKEDBALL,
		S_DRILL,
		S_MIRV_PARENT1,
		S_MIRV_PARENT2,
		S_MIRV_CHILD,
		S_ELECTRICARC1,
		S_ELECTRICARC2,
		S_FLAME_PARENT,
		S_FLAME_BIG,
		S_FLAME_MEDIUM,
		S_FLAME_SMALL,
		S_IONCANNON1,
		S_IONCANNON2,
		S_IONCANNON_DIE1,
		S_IONCANNON_DIE2,
		S_IONCANNON_DIE3,
		S_IONCANNON_DIE4,
		S_IONCANNON_DIE5,
		S_IONCANNON_FIRE1,
		S_IONCANNON_FIRE2,
		S_IONCANNON_CHARGE1,
		S_IONCANNON_CHARGE2,
		S_IONCANNON_CHARGE3,
		S_IONCANNON_CHARGE4,
		S_IONCANNON_CHARGE5,
		S_GREENLASER_PULSE,
		S_GREENLASER_PULSE_DIE1,
		S_GREENLASER_PULSE_DIE2,
		S_GREENLASER_PULSE_DIE3,
		S_GREENLASER_PULSE_DIE4,
		S_GREENLASER_PULSE_DIE5,
		S_GREENLASER_PULSE_DIE6,
		S_URANIUMSHELL,
		S_ROLLER,
		S_WATERCANISTER,
		S_NAPALMCANISTER,
		S_PLASMACANISTER,
		S_ACIDCANISTER,
		S_BULLETTRACER,
		S_FLAKSHELL,
		S_FLAKSHARD1,
		S_FLAKSHARD2,
    S_EMPMISSILE1,
		S_EMPMISSILE2,
		S_EMPEXPLODE,
		S_LIGHTNINGROD1,
		S_LIGHTNINGROD_LIGHTNING1,
		S_LIGHTNINGROD_LIGHTNING2,
		S_LIGHTNINGROD_FUNC,
		S_FIRESTARTER,
		S_FIRESTARTER_F1,
		S_FIRESTARTER_F2,
		S_FIRESTARTER_F3,
		S_FIRESTARTER_F4,
		S_FIRESTARTER_F5,
		S_FIRESTARTER_F6,
		S_TOWERPOD1,
		S_TOWERPOD2,
		S_TOWERPOD_BUILD1,
		S_TOWERPOD_BUILD2,
		S_TOWERPOD_BUILD3,
		S_TOWERPOD_BUILD4,
		S_TOWERPOD_BUILD5,
		S_TOWERPOD_BUILD6,
		S_TOWERPOD_BUILD7,
		S_TOWERPOD_BUILD8,
		S_TOWERPOD_BUILD9,
		S_TOWERPOD_DIE1,
		S_TOWERPOD_DIE2,
		S_MISSILETURRET1,
		S_MISSILETURRET2,
		S_MISSILETURRET_BUILD1,
		S_MISSILETURRET_BUILD2,
		S_MISSILETURRET_BUILD3,
		S_MISSILETURRET_BUILD4,
		S_MISSILETURRET_BUILD5,
		S_MISSILETURRET_BUILD6,
		S_MISSILETURRET_BUILD7,
		S_MISSILETURRET_BUILD8,
		S_MISSILETURRET_BUILD9,
		S_MISSILETURRET_BUILD10,
		S_MISSILETURRET_BUILD11,
		S_MISSILETURRET_ATTACK1,
		S_MISSILETURRET_DIE1,
		S_MISSILETURRET_DIE2,
		S_EXPLODE_A1,
		S_EXPLODE_A2,
		S_EXPLODE_A3,
		S_EXPLODE_A4,
		S_EXPLODE_A5,
		S_EXPLODE_A6,
		S_EXPLODE_A7,
		S_EXPLODE_A8,
		S_EXPLODE_B1,
		S_EXPLODE_B2,
		S_EXPLODE_B3,
		S_EXPLODE_B4,
		S_EXPLODE_B5,
		S_EXPLODE_B6,
		S_EXPLODE_B7,
		S_EXPLODE_B8,
		S_SMALLEXPLODE_A1,
		S_SMALLEXPLODE_A2,
		S_SMALLEXPLODE_A3,
		S_SMALLEXPLODE_A4,
		S_SMALLEXPLODE_A5,
		S_SMALLEXPLODE_A6,
		S_SMALLEXPLODE_A7,
		S_SMALLEXPLODE_A8,
		S_SMALLEXPLODE_B1,
		S_SMALLEXPLODE_B2,
		S_SMALLEXPLODE_B3,
		S_SMALLEXPLODE_B4,
		S_SMALLEXPLODE_B5,
		S_SMALLEXPLODE_B6,
		S_SPARKBALL1,
		S_SPARKBALL2,
		S_LARGESPARKBALL1,
		S_LARGESPARKBALL2,
		S_LARGESPARKBALL3,
		S_HSENTRY1,
		S_HSENTRY_SHOT,
		S_HSENTRY_SHOT1,
		S_HSENTRY2,
		S_HSENTRY3,
		S_HSENTRY4,
		S_HSENTRY5,
		S_HSENTRY_ATTACK1,
		S_HSENTRY_DIE1,
		S_ESENTRY1,
		S_ESENTRY_SHOT,
		S_ESENTRY2,
		S_ESENTRY3,
		S_ESENTRY4,
		S_ESENTRY5,
		S_ESENTRY_ATTACK1,
		S_ESENTRY_DIE1,
		S_BSENTRY1,
		S_BSENTRY2,
		S_BSENTRY3,
		S_BSENTRY4,
		S_BSENTRY5,
		S_BSENTRY_ATTACK1,
		S_BSENTRY_DIE1,
		S_EMPTYSHELL,
		S_NEEDLEMISSILE1,
		S_NEEDLEMISSILE2,
		S_BIGLAVAROCK,
		S_BIGLAVAROCKEFFECT,
		S_SMALLLAVAROCK,
		S_SMALLLAVAROCKEFFECT,
		S_LAVASPAWNER,
		S_LAVASPAWNER_ATTACK,
		S_LAVASPAWNER_ATTACK1,
		S_SNOWFLAKE_THINKER,
		S_SNOWFLAKE_LARGE,
		S_SNOWFLAKE_MEDIUM,
		S_SNOWFLAKE_SMALL,
		S_HEATRAY1,
		S_HEATRAY2,
		S_HEATRAY_START1,
		S_HEATRAY_START2,
		S_HEATRAY_EFFECT,
		S_HEATRAY_F1,
		S_HEATRAY_F2,
		S_HEATRAY_F3,
		S_HEATRAY_F4,
		S_HEATRAY_F5,
		S_HEATRAY_F6,
		S_RAILGUNSMOKE,
		S_FIREFLY,
		S_FIREFLY_DIE,
		S_FIREFLY_EFFECT,
		S_ALIENBEAM1,
		S_ALIENBEAM2,
		S_ALIENBEAM3,
		S_ALIENBEAM4,
		S_ALIENBEAMUP1,
		S_ALIENBEAMUP2,
		S_ALIENBEAMUPPARTICLE,
		S_CAVESPIKE1_IDLE,
		S_CAVESPIKE1_SHAKE1,
		S_CAVESPIKE1_SHAKE2,
		S_CAVESPIKE1_FALL1,
		S_CAVESPIKE1_DIE,
		S_CAVESPIKE2_IDLE,
		S_CAVESPIKE2_SHAKE1,
		S_CAVESPIKE2_SHAKE2,
		S_CAVESPIKE2_FALL1,
		S_CAVESPIKE2_DIE,
		S_CAVESPIKE3_IDLE,
		S_CAVESPIKE3_SHAKE1,
		S_CAVESPIKE3_SHAKE2,
		S_CAVESPIKE3_FALL1,
		S_CAVESPIKE3_DIE,
		S_CAVESPIKE4_IDLE,
		S_CAVESPIKE4_SHAKE1,
		S_CAVESPIKE4_SHAKE2,
		S_CAVESPIKE4_FALL1,
		S_CAVESPIKE4_DIE,
		S_CAVESPIKE5_IDLE,
		S_CAVESPIKE5_SHAKE1,
		S_CAVESPIKE5_SHAKE2,
		S_CAVESPIKE5_FALL1,
		S_CAVESPIKE5_DIE,
		S_CAVESPIKESHARD1,
		S_CAVESPIKESHARD2,
		S_CAVESPIKESHARD3,
		S_CAVESPIKESHARD4,
		S_CAVESPIKESHARD5,
		S_CAVESPIKESHARD6,
		S_DEATHSHEADCHILD,
		S_DEATHSHEADEMPTY_1,
		S_DEATHSHEADEMPTY_2,
		S_DEATHSHEADFULL_1,
		S_DEATHSHEADFULL_2,
		S_TANKGIB1,
		S_TANKGIB2,
		S_TANKGIB3,
		S_TANKGIB4,
		S_TANKGIB5,
		S_CHERRYBLOSS_1,
		S_CHERRYBLOSS_1SIT,
		S_CHERRYBLOSS_2,
		S_CHERRYBLOSS_2SIT,
		S_CHERRYBLOSS_3,
		S_CHERRYBLOSS_3SIT,
		S_CHERRYBLOSS_SPAWNER,
		S_RAINDROP,
		S_RAINDROP_SPAWNER,
		S_RAINDROP_ATTACK,
		S_LAVASHOOTER,
		S_LAVASHOOTER_ATTACK1,
		S_LAVASHOOTER_ATTACK2,
		S_HEATCORONA,
		S_HEATCORONA_SPAWNER,
		S_LAVAFLOORDAMAGE_IDLE,
		S_LAVAFLOORDAMAGE,
		S_TURNARROW,
		S_GIGANUKE,
		S_SHIELD_CONVENTIONAL,
		S_SHIELD_SUPERCONVENTIONAL,
		S_MORTARSHELL,
		S_MORTARBASE,
		S_POLARISMISSILE1,
		S_POLARISMISSILE2,
		S_POLARISOPEN1,
		S_POLARISOPEN2,
		S_POLARISOPEN3,
		S_POLARISEMPBLAST,
		S_NITROGENPUFF_MEDIUM,
		S_NITROGENPUFF_LARGE,
		ICESHARD_BOTTOMLEFT,
		ICESHARD_BOTTOMRIGHT,
		ICESHARD_BOTTOM,
		ICESHARD_TOP,
		ICESHARD_TOPLEFT,
		ICESHARD_TOPRIGHT,
		S_BHBOMB,
		S_BHBOMB_OPEN1,
		S_BHBOMB_OPEN2,
		S_BHBOMB_OPEN3,
		S_BHBOMB_BLOB1,
		S_BHBOMB_BLOB2,
		S_BHBOMB_BLOB3,
		S_BHBOMB_BLOB4,
		S_BHBOMB_CLOUDS1,
		S_BHBOMB_CLOUDS2,
		S_BHBOMB_IMPLODEWAVE,
		S_BHBOMB_SHRINK1,
		S_BHBOMB_SHRINK2,
		S_BHBOMB_SHRINK3,
		S_BHBOMB_SHRINK4,
		S_BHBOMB_SHRINK5,
		S_BHBOMB_SHRINK6,
		S_TEL_NEGATIVECHARGE,
		S_TEL_POSITIVECHARGE,
		S_TEL_NEGATIVE,
		S_TEL_POSITIVE,
		S_LIGHTNINGROD_HELLSTRIKE1,
		S_LIGHTNINGROD_HELLSTRIKE2,
		S_AS_AIRSHIP,
		S_AS_FLARESHOT,
		S_AS_FLARE,
		S_AS_FLARESMOKE,
		S_IONBATTERY_THINKER,
		S_IONBATTERY,
		S_IONBATTERY_FLARE1,
		S_IONBATTERY_FLARE2,
		S_HEATCANNON_THINKER,
		S_HEATCANNON_SHOT1,
		S_HEATCANNON_SHOT2,
		S_AS_TANKBAY1,
		S_AS_TANKBAY2,
		S_AS_FLYER,
		S_FUNKYBOMB,
		S_FUNKYNUKE,
		S_FIRETOAD,
		S_FIRETOAD_STAGE1,
		S_FIRETOAD_STAGE2,
		S_PROTONFLARE,
		S_TANKMUZZLEFLARE,
		S_TANKPHOTONFLARE,
		S_SHIELD_NIV_B1,
		S_SHIELD_NIV_B2,
		S_SHIELD_NIV_B3,
		S_SHIELD_NIV_B4,
		S_SHIELD_NIV_B5,
		S_SHIELD_NIV_1,
		S_SHIELD_NIV_2,
		S_SHIELD_NIV_3,
		S_SHIELD_NIV_4,
		S_SHIELD_NIV_DIE1,
		S_SHIELD_NIV_DIE2,
		S_SHIELD_NIV_DIE3,
		S_SHIELD_NIV_DIE4,
		S_SHIELD_NIV_DIE5,
		S_FUNKYBOMB_THINKER,
		S_FUNKYNUKE_THINKER,
		S_OBCBEACON_SHOT,
		S_OBCBEACON1,
		S_OBCBEACON2,
		S_OBCTHINKER,
		S_OBCTHINBEAM1,
		S_OBCTHINBEAMFINISH1,
		S_OBCTHINBEAMFINISH2,
		S_OBCMEDIUMBEAM,
		S_OBCBIGBEAM1,
		S_OBCBIGBEAM2,
		S_OBCIONFLAME1,
		S_OBCIONFLAME2,
		S_OBCIONFLAME3,
		S_OBCANI1,
		S_OBCANI2,
		S_OBCANI3,
		S_OBCANI4,
		S_OBCANI5,
		S_OBCANI6,
		S_OBCANI7,
		S_OBCANI8,
		S_OBCANI9,
		S_OBCANI10,
		S_OBCANI11,
		S_OBCANI12,
		S_OBCANI13,
		S_OBCANI14,
		S_OBCANI15,
		S_OBCANI16,
		S_OBCANI17,
		S_OBCANI18,
		S_OBCANI19,
		S_OBCANI20,
		S_OBCANI21,
		S_OBCANI22,
		S_SUPERRAILSMOKE1,
		S_SUPERRAILSMOKE2,
		S_SUPERRAILSMOKE3,
		S_SUPERRAILSMOKE4,
		S_SUPERRAILSMOKE5,
		S_SUPERRAILSMOKE6,
		S_ICESPIKE1_IDLE,
		S_ICESPIKE1_SHAKE1,
		S_ICESPIKE1_SHAKE2,
		S_ICESPIKE1_FALL1,
		S_ICESPIKE1_DIE,
		S_ICESPIKE2_IDLE,
		S_ICESPIKE2_SHAKE1,
		S_ICESPIKE2_SHAKE2,
		S_ICESPIKE2_FALL1,
		S_ICESPIKE2_DIE,
		S_ICESPIKE3_IDLE,
		S_ICESPIKE3_SHAKE1,
		S_ICESPIKE3_SHAKE2,
		S_ICESPIKE3_FALL1,
		S_ICESPIKE3_DIE,
		S_ICESPIKESHARD1_1,
		S_ICESPIKESHARD1_2,
		S_ICESPIKESHARD1_3,
		S_ICESPIKESHARD1_4,
		S_ICESPIKESHARD2_1,
		S_ICESPIKESHARD2_2,
		S_ICESPIKESHARD2_3,
		S_ICESPIKESHARD2_4,
		S_ICESPIKESHARD3_1,
		S_ICESPIKESHARD3_2,
		S_ROF_PROJECTILE,
		S_ROF_OPEN1,
		S_ROF_OPEN2,
		S_ROF_OPEN3,
		S_ROF_OPEN4,
		S_ROF_FIRE1,
		S_ROF_FIRE2,
		S_ROF_FIRE3,
		S_SB_PROJECTILE,
		S_SB_SILVERBULLET,
		S_SB_MOONSHOT,
		S_SB_FULLMOON,
		S_FUELCYLINDER,
		S_FUELCYLINDER_DIE,
		S_TREASURECHEST,
		S_TREASUREOPEN,
		S_TREASURESYMBOL,
		S_MAGCLOSED,
    S_MAGOPEN_1,
    S_MAGOPEN_2,	
    S_MAGOPEN_3,
    S_MAGOPEN_4,	
    S_MAGOPEN_5,
    S_MAGOPEN_6,	
    S_MAGPULSE1,	
    S_MAGPULSE2,
    S_MAGPULSE3,
    S_MAGPULSE4,
    S_MAGPULSE5,
    S_MAGPULSE6,	
		S_MAGDEAD,
    
		NUM_STATES
};


// Externally decarled, so that our following structure
// knows what the sprite_t class is
class sprite_t;

typedef struct state_t
{
        _state           currentstate;   // Defined state
        _state           nextstate;      // Next state
        void            (* func)(sprite_t* s);          // State the sprite is in
        int             datafilelookup;
		int				flags;
        int             tics;

		// End of stuff that is defined by hand
		// The rest of this stuff will be setup by init_sprites()
		BITMAP			*alpha_sprite;
} state_t;

extern state_t states[NUM_STATES];

typedef	struct
{
		char			name[64];
		int				width,height;
		int				health;
		int				h,e,b;
		int				damageh, damagee, damageb;
		int				flags;
		_state		spawnstate;
		_state		attackstate;
		_state		damagestate;
		_state		deathstate;
		bool			editoruse;
    _state		icon; // state for editor to get display icon
}wobject_t;


typedef	enum _wobject {
	WO_TOWERPODCONSTRUCTION,
	WO_MISSILETURRET,
	WO_HEATSENTRYPOD,
	WO_ENERGYSENTRYPOD,
	WO_BALLISTICSENTRYPOD,
	WO_LAVAROCKSPAWNER,
	WO_SNOWFLAKESPAWNER,
	WO_CAVESPIKE1,
	WO_CAVESPIKE2,
	WO_CAVESPIKE3,
	WO_CAVESPIKE4,
	WO_CAVESPIKE5,
	WO_CHERRYBLOSSSPAWNER,
	WO_RAINSPAWNER,
	WO_LAVASHOOTER,
	WO_HEATCORONASPAWNER,
	WO_LAVAFLOORDAMAGE,
	WO_ICESPIKE1,
	WO_ICESPIKE2,
	WO_ICESPIKE3,
	WO_FUELCYLINDER,
	WO_TREASURECHEST,
	WO_MAGGEN,
	NUM_WOBJECTS
};

extern	wobject_t	wobjects[NUM_WOBJECTS];

class sprite_t {

	public:

		_state		spawnstate;
		_state		damagestate;
		_state		deathstate;
		int				h,e,b;					// HEB protection for shootable sprites
		int				radius;

		state_t   startstate;
		int				wobj;
		float			angle;
    int             active;
		int				health;
		int				healthbartrans;
		int				starthealth;
    int       flags;
    float     x,xvel;
    float     y,yvel;
		float			oldx,oldy;
		float			oldxvel, oldyvel;
		int				width,height;
		int				rotatex,rotatey;
		float			gravity;
		float			scale;				// Percent to scale of the sprite (1=normal 2=2x size)
		int				opacity;			// How transparent in the sprite? (default: 255 - solid)
    int       id;						// ID # of this sprite
    int       tics;					// What do you think?
		int				life;					// Life is counted up instead of down
		int				invulntics;		// Counted down where for sprite is invunerable to damage (shootable only)
		int				bouncecount;	// Number of times sprite has bounced off of something
    int       misc1;
    int       misc2;

		// Tinting color
		int				tintr, tintg, tintb, tintvalue;

		// Network stuph (for multiplayer)
		int				netid;
		float			shotx,shoty,shotxvel,shotyvel;
		int				shotdeg;

		float			deginc;
		float			targetx;
		float			targety;

		// How sprites take damage (MF_SHOOTABLE)
		int				damageh,damagee,damageb;				// Will the sprite dish out any damage to tanks?
		float			reservedamageh,							// Reserve damage that is tallied up and applied as
						reservedamagee,							// regular damage at a later time
						reservedamageb;
		int				tankfrom;								// Who is this damage coming from?
		bool			tankdamaged[MAX_TANKS];					// Who have we damaged? (To prevent damaging a tank twice)
		bool			spritedamaged;							// Have we dished any damaged?

		// For simply playing sounds
		SAMPLE			*snd;
		int				sndvoice;

		void			SetState(_state state);
		void			Damage(float damageh, float damagee, float damageb, bool scattertext=false);
		void			CheckReserveDamage(void);
		sprite_t		*Kill(void);
		int				needstodie;		// This is a positive value if the sprite needs to go bye-bye

		void			StartSound(int sound, int loop=false, int stopcurrent=false);
		void			StopSound(void);
		void			OnGround2(BITMAP *bmp);
		void			DrawDescBox(BITMAP *b, char	*line1, char *line2);

		// From whom did I spawn from
		sprite_t		*spawnfrom;

		// Attacking anyone?
		int				endlevelshoot;		// Has this projectile had a chance to shoot
											// at someone at the end of a tank's turn?
		int				targettank;

		// For the linked list

		sprite_t		*next;
		sprite_t		*prev;
};

sprite_t		*KillSprite(sprite_t* s);


extern	int				numsprites;
extern	int				numwobjsprites;
extern	sprite_t*		sprites;

void A_Kill(int num);


// Now finally, some functions
void		Setup_Sprites(int bpp);
void		G_InitSprites(void);
sprite_t*	G_SpawnWO(_wobject objectnum, int x, int y);
sprite_t*	G_SpawnSprite(int x, int y, _state state, int flags);
void		G_KillSprite(sprite_t* s);
void		G_DestroyAllSprites(void);
void		G_SpriteTicker(void);
void		G_SpriteDrawer(BITMAP	*buf);
bool		G_IsAdvanceTurnSprite(void);
void		G_DeleteAllSprites(void);

// Multiplayer stuff
sprite_t*	G_FindNetID(int id);
int			G_GetNetID(void);
