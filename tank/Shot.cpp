#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"

//	generic functions
int ShotLength(float x, float y, int fireangle);
void	ShotLength_Proc(BITMAP *b, int x, int y, int color);

// Varibles
float	contactx, contacty;		//	set by callback when contact found
int		shotindex;						//	the current shot number
bool	ignoreshot;						//	ignore the rest of the line segment
bool	showtrace = false;		// display shot trace when fired
int		fastshot = 1;					// shot in to go faster


// Hook functions
void			MiniMIRV_Thinker(int shot);
void			MIRV_Thinker(int shot);
void			Drill_Thinker(int shot);
void			ElectricArc_Thinker(int shot);
void			FlameThrower_Thinker(int shot);
void			LaserPulse_Thinker(int shot);
void			TunnelLaser_Thinker(int shot);
void			LaserBlast_Thinker(int shot);
void			MachineGun_Thinker(int shot);
void			Shotgun_Thinker(int shot);
void			FlakShard_Thinker(int shot);
void			FireStarter_Thinker(int shot);
void			FireStarter_KillFlames(int shot);
void			TowerPod_Thinker(int shot);
void			MissileTurretShot_Thinker(int shot);
void			HSentry_Spawn(int shot);
void			ESentry_Spawn(int shot);
void			BSentry_Spawn(int shot);
void			MoltenRod_Thinker(int shot);
void			ELaser_Thinker(int shot);
void			Uzi_Thinker(int shot);
void			Heatray_Thinker(int shot);
void			Railgun_Thinker(int shot);
void			FireFly_Thinker(int shot);
void			FireFly_ChildThinker(int shot);
void			Deathshead_Thinker(int shot);
void			Tankgib_Thinker(int shot);
void			MonkeyMortar_Thinker(int shot);
void			PolarisBeep_Thinker(int shot);
void			IonBatteryChild_Thinker(int shot);
void			IonBattery_Thinker(int shot);
void			HeatCannon_Thinker(int shot);
void			Proton_Thinker(int shot);
void			SuperRailgun_Thinker(int shot);
void			SuperRailgunChild_Thinker(int shot);
void			Moonshot_Thinker(int shot);

// Tank-hit hook functions
void			PopFlotation(int shot, int tankhit);
void			MachineGun_TankHit(int shot, int tankhit);
void			FlakShard_TankHit(int shot, int tankhit);
void			FireFly_ChildHitTank(int shot, int tankhit);

// Missile detionating functions (I can't spell)
void			Maggen_Decay(void);
void			Maggen_Release(int shot);
void			Flak_Explode(int shot);
void			Rod_Explode(int shot);
void			Heatwave_Explode(int shot);
void			EMP_Explode(int shot);
void			MoltenRod_Explode(int shot);
void			FireFly_ChildHit(int shot);
void			MortarShell_Explode(int shot);
void			Polaris_Explode(int shot);
void			BHBomb_Explode(int shot);
void			TEL_PositiveExplode(int shot);
void			TEL_NegativeExplode(int shot);
void			A_FunkyExplode(int shot);
void			A_FunkyNukeExplode(int shot);
void			A_AS_FlareExplode(int shot);
void			HeatCannon_Explode(int shot);
void			FireToad1_Explode(int shot);
void			FireToad2_Explode(int shot);
void			A_OBC_Explode(int shot);
void			ROF_Explode(int shot);
void			SB_Explode(int shot);

projectile_t	projectiles[NUM_PROJECTILES]={
	
	{
		"Baby Missile",					// Name
		"",								// HUD Name 0
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD | 
		 MISSILE_NOSMOKE | 
		 MISSILE_NOLIST),				// Missile Flags
		0,								// Cost
		-1,								// With each purchase
		false,							// Rotates?
		25,25,25,						// Total damage (HEB)
		10,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		5,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Mini-MIRV",					// Name
		"",								// HUD Name 1
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOLIST|
		 MISSILE_NOSMOKE),				// Missile Flags
		1000,							// Cost
		10,								// With each purchase
		true,							// Rotates?
		75,75,75,						// Total damage (HEB)
		15,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		15,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		MiniMIRV_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Mini-MIRV Child",				// Name
		"",								// HUD Name 2
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOLIST|
		 MISSILE_NOSMOKE),				// Missile Flags
		1000,							// Cost
		10,								// With each purchase
		true,							// Rotates?
		45,45,45,						// Total damage (HEB)
		15,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		15,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Needle Missile",				// Name
		"",								// HUD Name 3
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOLIST|
		 MISSILE_NOSMOKE),				// Missile Flags
		700,							// Cost
		10,								// With each purchase
		true,							// Rotates?
		45,45,45,						// Total damage (HEB)
		20,								// Damage radius
		2,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		10,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_NEEDLEMISSILE1,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Missile",						// Name
		"",								// HUD Name 4
		MICON_1CONVENTIONAL,			// Buystuff icon type
		MISSILE_STANDARD,				// Missile Flags
		1000,							// Cost
		10,								// With each purchase
		true,							// Rotates?
		70,70,70,						// Total damage (HEB)
		25,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		25,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_MISSILE_1,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Tank Shard",					// Name
		"",								// HUD Name 5
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOLIST),				// Missile Flags
		1000,							// Cost
		3,								// With each purchase
		false,							// Rotates?
		0,0,100,						// Total damage (HEB)
		25,								// Damage radius
		1,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		10,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_TANKGIB1,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		Tankgib_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Water Mine",					// Name
		"",								// HUD Name 
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD |
		 MISSILE_NOSMOKE |				// Missile Flags
		 MISSILE_FLOAT | 
		 MISSILE_NOBOTUSE),
		3000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		10,10,10,						// Total damage (HEB)
		20,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		20, 							// Explode time (in tics)
		NULL,							// Explosion hook
		S_SPIKEDBALL,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		PopFlotation,					// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Dirt Ball",					// Name
		"",								// HUD Name 
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD |
		 MISSILE_NOSMOKE |
		 MISSILE_DIRT |
		 MISSILE_NOLIST |
		 MISSILE_NOBOTUSE),				// Missile Flags
		4000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		0,0,0,							// Total damage (HEB)
		40,								// Damage radius
		2,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		0, 								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters

		// Extra Properties (needed for dirt color)
		0,												// Number of pixels to fill the land with
		0,0,0,											// Damage each pixel does to a tank (HEB)
		0,												// Particle force
		0,												// Time napalm stays on the ground
		0,												// Duration of calculated damage
		130,89,11,										// Color (R,G,B)

	},

	{
		"Ton-O-Dirt",					// Name
		"",								// HUD Name 
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD |
		 MISSILE_NOSMOKE |
		 MISSILE_DIRT | 
		 MISSILE_NOBOTUSE ),			// Missile Flags
		5000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,0,0,							// Total damage (HEB)
		80,								// Damage radius
		3,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		0, 								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters

		// Extran Properties (needed for dirt color)
		0,												// Number of pixels to fill the land with
		0,0,0,											// Damage each pixel does to a tank (HEB)
		0,												// Particle force
		0,												// Time napalm stays on the ground
		0,												// Duration of calculated damage
		130,89,11,										// Color (R,G,B)

	},

	{
		"Riot Blast",					// Name
		"",								// HUD Name 
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD |
		 MISSILE_NOLIST |
		 MISSILE_NOSMOKE),				// Missile Flags
		4000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		0,0,0,							// Total damage (HEB)
		40,								// Damage radius
		5,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		0, 								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)riotpal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Heavy Riot Bomb",				// Name
		"Hvy Riot Bomb",				// HUD Name 
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD |
		 MISSILE_NOSMOKE),				// Missile Flags
		5000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,0,0,							// Total damage (HEB)
		70,								// Damage radius
		4,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		0, 								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)riotpal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Water Splash",					// Name
		"",								// HUD Name 
		MICON_LIQUID,					// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_WATER|
		MISSILE_NOSMOKE|
		MISSILE_NOTANKCLIP|
		MISSILE_NOBOTUSE),				// Missile Flags
		5000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,0,0,							// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		(int)(1.2*TICRATE),				// Explode time (in tics)
		NULL,							// Explosion hook
		S_WATERCANISTER,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_CANISTER,				// Explosion sound
		-1,								// Can be used by ALL characters

		// Liquid Properties
		175,							// Number of pixels to fill the land with
		0,0,0,							// Damage each pixel does to a tank (HEB)
		100,								// Particle force
		(int)(0.5*TICRATE),				// Time napalm stays on the ground
		0,								// Duration of calculated damage
		0,50,255,						// Color (R,G,B)

	},


	{
		"Molten Rod",					// Name
		"",								// HUD Name 
		MICON_1HEAT,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOSMOKE|
		 MISSILE_NODIRTCLIP|
		 MISSILE_BEAM|
		 MISSILE_NOHITSELF|
		 MISSILE_NOLIST),				// Missile Flags
		5000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		10,0,0,							// Total damage (HEB)
		30,								// Damage radius
		10,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		35,								// Explode time (in tics)
		MoltenRod_Explode,				// Explosion hook
		S_HSENTRY_SHOT,					// An animated sprite
		(PALETTE *)greenpal,			// Selected palette to explode with
		MoltenRod_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
		
	},

	{
		"Electric Laser",				// Name
		"",								// HUD Name 
		MICON_1ENERGY,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOSMOKE|
		 MISSILE_NODIRTCLIP|
		 MISSILE_BEAM|
		 MISSILE_NOHITSELF|
		 MISSILE_NOLIST),				// Missile Flags
		5000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,140,0,						// Total damage (HEB)
		15,								// Damage radius
		10,								// Detonate speed (in tics)
		5,								// Explode speed (in tics)
		35,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_ESENTRY_SHOT,					// An animated sprite
		(PALETTE *)blueflame,			// Selected palette to explode with
		ELaser_Thinker,					// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		EXPLODE_MEDIUM3,				// Explosion sound
		-1,								// Can be used by ALL characters
		
	},


	{
		"Uzi Spray",						// Name
		"",									// HUD Name 
		MICON_1BALLISTIC,			// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOGRAVITY|
		MISSILE_GHOST|
		MISSILE_BEAM|
		MISSILE_NOLIST|
		MISSILE_NOHITSELF),					// Missile Flags
		5000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		0,0,140,							// Total damage (HEB)
		100,								// Damage radius
		2,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		60,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BULLETTRACER,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		Uzi_Thinker,						// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREUZI,						// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Uzi Child",						// Name
		"",									// HUD Name 
		MICON_1BALLISTIC,					// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOGRAVITY|
		MISSILE_NOLIST|
		MISSILE_BEAM|
		MISSILE_NOHITSELF),					// Missile Flags
		5000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		0,0,10,								// Total damage (HEB)
		7,									// Damage radius
		2,									// Detonate speed (in tics)
		10,									// Explode speed (in tics)
		5,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BULLETTRACER,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		MachineGun_TankHit,					// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREUZI,						// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Tower Pod",					// Name
		"",								// HUD Name 
		MICON_1CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOTANKCLIP|
		 MISSILE_NOBOTUSE),				// Missile Flags
		5000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,250,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		TowerPod_Thinker,				// Explosion hook
		S_TOWERPOD1,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Tracers",						// Name
		"",								// HUD Name 
		MICON_EXTRASHOT,				// Buystuff icon type
		(MISSILE_STANDARD | 
		MISSILE_NOSMOKE |
		MISSILE_NOBUBBLES | 
		MISSILE_NOBOTUSE),				// Missile Flags
		5000,							// Cost
		1,								// With each purchase
		false,							// Rotates?
		0,0,0,							// Total damage (HEB)
		5,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		5,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"A.F.R.A.M.M.",					// Name
		"",								// HUD Name 
		MICON_2CONVENTIONAL,			// Buystuff icon type
		MISSILE_STANDARD,				// Missile Flags
		5000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		70,70,70,						// Total damage (HEB)
		55,								// Damage radius
		2,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BLACKMISSILE_1,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Electric Arc",					// Name
		"",								// HUD Name
		MICON_2ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|				// Missile Flags
		 MISSILE_NOSMOKE | 
		 MISSILE_NOBOTUSE|
		 MISSILE_BEAM),
		6000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		0,300,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		ElectricArc_Thinker,			// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_ELECTRICCURRENT,			// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Flame Thrower",				// Name
		"",								// HUD Name 
		MICON_2HEAT,					// Buystuff icon type
		(MISSILE_STANDARD|				// Missile Flags
		 MISSILE_NOSMOKE | 
		 MISSILE_NOBOTUSE|
		 MISSILE_BEAM),
		6000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		300,0,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		FlameThrower_Thinker,			// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"M.I.R.V.",							// Name
		"",									// HUD Name 
		MICON_2CONVENTIONAL,				// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE),					// Missile Flags
		6000,								// Cost
		5,									// With each purchase
		true,								// Rotates?
		150,150,150,						// Total damage (HEB)
		30,									// Damage radius
		10,									// Detonate speed (in tics)
		2,									// Explode speed (in tics)
		35,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_MIRV_PARENT1,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		MIRV_Thinker,						// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_SMALL,						// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"MIRV Shot",						// Name
		"",									// HUD Name 
		MICON_1CONVENTIONAL,				// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOLIST|
		MISSILE_NOSMOKE),					// Missile Flags
		2000,								// Cost
		5,									// With each purchase
		true,								// Rotates?
		75,75,75,							// Total damage (HEB)
		20,									// Damage radius
		1,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		35,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_MIRV_CHILD,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		-1,									// Tank fire sound
		EXPLODE_SMALL,						// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Funky Bomb",						// Name
		"",									// HUD Name 
		MICON_2CONVENTIONAL,				// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE),					// Missile Flags
		6500,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		150,150,150,						// Total damage (HEB)
		30,									// Damage radius
		10,									// Detonate speed (in tics)
		2,									// Explode speed (in tics)
		35,									// Explode time (in tics)
		A_FunkyExplode,						// Explosion hook
		S_FUNKYBOMB,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_SMALL,						// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Funky Bomb Child",						// Name
		"",									// HUD Name 
		MICON_2CONVENTIONAL,				// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOLIST|
		MISSILE_NOBOTUSE),					// Missile Flags
		6500,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		85,85,85,						// Total damage (HEB)
		30,									// Damage radius
		20,									// Detonate speed (in tics)
		5,									// Explode speed (in tics)
		50,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BABYMISSILE,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_SMALL,						// Explosion sound
		-1,									// Can be used by ALL characters
	},


	{
		"Fire Toad",						// Name
		"",									// HUD Name 
		MICON_2HEAT,						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE),					// Missile Flags
		6500,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		250,0,0,							// Total damage (HEB)
		75,									// Damage radius
		2,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		50,									// Explode time (in tics)
		FireToad1_Explode,					// Explosion hook
		S_FIRETOAD,							// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Fire Toad (Phase 2)",				// Name
		"",									// HUD Name 
		MICON_2HEAT,						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOLIST|
		MISSILE_NOBOTUSE),					// Missile Flags
		6500,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		200,0,0,							// Total damage (HEB)
		55,									// Damage radius
		2,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		50,									// Explode time (in tics)
		FireToad2_Explode,					// Explosion hook
		S_FIRETOAD,							// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Fire Toad (Phase 3)",				// Name
		"",									// HUD Name 
		MICON_2HEAT,						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOLIST|
		MISSILE_NOBOTUSE),					// Missile Flags
		6500,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		150,0,0,							// Total damage (HEB)
		25,									// Damage radius
		1,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		25,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_FIRETOAD,							// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_SMALL,						// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Baby Nuke",					// Name
		"",								// HUD Name 
		MICON_2CONVENTIONAL,			// Buystuff icon type
		MISSILE_STANDARD,				// Missile Flags
		6500,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		90,90,90,						// Total damage (HEB)
		75,								// Damage radius
		2,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYNUKE,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		TANK_HEAVYEXPLODE1,				// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Missile Turret",				// Name
		"",								// HUD Name
		(MICON_1CONVENTIONAL|
		 MICON_AI),						// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOTANKCLIP|
		 MISSILE_NOBOTUSE),				// Missile Flags
		7000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,0,0,							// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		MissileTurretShot_Thinker,		// Explosion hook
		S_MISSILETURRET1,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Heavy Roller",					// Name
		"",								// HUD Name
		MICON_2BALLISTIC,				// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_CRUISELAND|
		 MISSILE_NOSMOKE),				// Missile Flags
		7000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,0,250,						// Total damage (HEB)
		60,								// Damage radius
		2,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_ROLLER,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		EXPLODE_BIG,					// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Large Fireball",				// Name
		"",								// HUD Name 
		MICON_2HEAT,					// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE),				// Missile Flags
		7500,							// Cost
		10,								// With each purchase
		false,							// Rotates?
		250,0,0,						// Total damage (HEB)
		75,								// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		75,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_FIREBALL, 					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		EXPLODE_BIG,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Sparkball",					// Name
		"",								// HUD Name
		MICON_2ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOLIST|
		 MISSILE_BOTUSEONLY|
		 MISSILE_NOBUBBLES),			// Missile Flags
		6000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,200,0,						// Total damage (HEB)
		40,								// Damage radius
		15,								// Detonate speed (in tics)
		50,								// Explode speed (in tics)
		(int)(1.2*TICRATE),				// Explode time (in tics)
		NULL,							// Explosion hook
		S_SPARKBALL1,					// An animated sprite
		(PALETTE *)darkbluepal,			// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Drill Missile",				// Name
		"",								// HUD Name 
		MICON_2CONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NODIRTCLIP|
		 MISSILE_NOTANKCLIP|
		 MISSILE_NOSMOKE),				// Missile Flags
		8000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		50,50,50,						// Total damage (HEB)
		60,								// Damage radius
		3,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		30,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_DRILL, 						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		Drill_Thinker,					// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
		
	},


	{
		"Ion Battery",					// Name
		"",								// HUD Name 
		MICON_2ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOSMOKE|
		 MISSILE_NODIRTCLIP|
		 MISSILE_BEAM|
		 MISSILE_NOBOTUSE|
		 MISSILE_NOHITSELF|
		 MISSILE_NOLIST),				// Missile Flags
		8000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		0,275,0,						// Total damage (HEB)
		30,								// Damage radius
		10,								// Detonate speed (in tics)
		5,								// Explode speed (in tics)
		35,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_IONBATTERY,					// An animated sprite
		(PALETTE *)blueflame,			// Selected palette to explode with
		IonBattery_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		EXPLODE_MEDIUM3,				// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Ion Battery Child",			// Name
		"",								// HUD Name 
		MICON_2ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOSMOKE|
		 MISSILE_NODIRTCLIP|
		 MISSILE_BEAM|
		 MISSILE_NOBOTUSE|
		 MISSILE_NOHITSELF|
		 MISSILE_NOLIST),			// Missile Flags
		8000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		0,275,0,						// Total damage (HEB)
		30,								// Damage radius
		10,								// Detonate speed (in tics)
		5,								// Explode speed (in tics)
		35,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_IONBATTERY,					// An animated sprite
		(PALETTE *)blueflame,			// Selected palette to explode with
		IonBatteryChild_Thinker,		// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		IONBATTERY_FIRE,				// Tank fire sound
		EXPLODE_MEDIUM3,				// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Heat Cannon",						// Name
		"",									// HUD Name 
		MICON_2HEAT,						// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOSMOKE|
		 MISSILE_BEAM|
		 MISSILE_NOBOTUSE|
		 MISSILE_NOHITSELF|
		 MISSILE_NOLIST),				// Missile Flags
		8000,								// Cost
		5,									// With each purchase
		true,								// Rotates?
		275,0,0,							// Total damage (HEB)
		1,									// Damage radius
		1,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		(int)(1.2*TICRATE),					// Explode time (in tics)
		HeatCannon_Explode,					// Explosion hook
		S_HEATCANNON_SHOT1,					// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		HeatCannon_Thinker,					// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREHEATCANNON,				// Tank fire sound
		EXPLODE_HEATWAVE,					// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Lead Shell",						// Name
		"",									// HUD Name 
		MICON_2BALLISTIC,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOLIST|
		 MISSILE_BOTUSEONLY),				// Missile Flags
		8000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		0,0,200,							// Total damage (HEB)
		35,									// Damage radius
		60,									// Detonate speed (in tics)
		60,									// Explode speed (in tics)
		1,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_SHELL,							// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_LEADSHELL,					// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Blue Missile",					// Name
		"",								// HUD Name 
		MICON_2ENERGY,					// Buystuff icon type
		MISSILE_STANDARD,				// Missile Flags
		10000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		0,275,0,						// Total damage (HEB)
		65,								// Damage radius
		2,								// Detonate speed (in tics)
		4,								// Explode speed (in tics)
		90, 							// Explode time (in tics)
		NULL,							// Explosion hook
		S_BLUEMISSILE_1,				// An animated sprite
		(PALETTE *)blueflame,			// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Cannon Ball",						// Name
		"",									// HUD Name 
		MICON_2BALLISTIC,					// Buystuff icon type
		MISSILE_STANDARD|MISSILE_NOSMOKE,	// Missile Flags
		10000,								// Cost
		5,									// With each purchase
		false,								// Rotates?
		0,0,250,							// Total damage (HEB)
		50,									// Damage radius
		60,									// Detonate speed (in tics)
		60,									// Explode speed (in tics)
		1,									// Explode time (in tics)
		NULL,							// Explosion hook
		S_IRONBALL,							// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_LEADSHELL,					// Explosion sound
		-1,									// Can be used by ALL characters
	},


	{
		"Shotgun Blast",					// Name
		"",									// HUD Name 
		MICON_2BALLISTIC,					// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOGRAVITY|
		MISSILE_GHOST|
		MISSILE_BEAM|
		MISSILE_NOHITSELF),					// Missile Flags
		10000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		0,0,360,							// Total damage (HEB)
		5,									// Damage radius
		2,									// Detonate speed (in tics)
		10,									// Explode speed (in tics)
		5,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BULLETTRACER,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		Shotgun_Thinker,					// Function hook
		MachineGun_TankHit,					// Hit function hook
		NULL,								// Ground-impact function hook
		-1,									// Tank fire sound
		-1,									// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Shotgun Child",					// Name
		"",									// HUD Name
		MICON_2BALLISTIC,					// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOGRAVITY|
		MISSILE_NOLIST|
		MISSILE_BEAM|
		MISSILE_NOHITSELF),					// Missile Flags
		10000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		0,0,15,								// Total damage (HEB)
		5,									// Damage radius
		2,									// Detonate speed (in tics)
		10,									// Explode speed (in tics)
		5,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BULLETTRACER,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		MachineGun_TankHit,					// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREMACHINEGUN,				// Tank fire sound
		-1,									// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Firefly",							// Name
		"",									// HUD Name
		MICON_2HEAT,						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOGRAVITY|
		MISSILE_GHOST|
		MISSILE_BEAM|
		MISSILE_NOHITSELF),					// Missile Flags
		10000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		360,0,0,							// Total damage (HEB)
		20,									// Damage radius
		2,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		60,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BULLETTRACER,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		FireFly_Thinker,					// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		-1,									// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},


	{
		"Firefly Child",					// Name
		"",									// HUD Name 
		MICON_2HEAT,						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOGRAVITY|
		MISSILE_NOLIST|
		MISSILE_NOHITSELF),					// Missile Flags
		1000,								// Cost
		1,									// With each purchase
		true,								// Rotates?
		20,0,0,								// Total damage (HEB)
		5,									// Damage radius
		2,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		10,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_FIREFLY,							// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		FireFly_ChildThinker,				// Function hook
		FireFly_ChildHitTank,				// Hit function hook
		FireFly_ChildHit,					// Ground-impact function hook
		TANK_FIREFIREFLY,					// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Proton Flare",						// Name
		"",									// HUD Name
		MICON_2ENERGY,						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_GHOST|
		MISSILE_NOHITSELF),					// Missile Flags
		10000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		0,360,0,							// Total damage (HEB)
		20,									// Damage radius
		2,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		60,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_PROTONFLARE,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		Proton_Thinker,						// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		-1,									// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Proton Child",						// Name
		"",									// HUD Name 
		MICON_2ENERGY,						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOBUBBLES|
		MISSILE_NOLIST),						// Missile Flags
		1000,								// Cost
		20,									// With each purchase
		true,								// Rotates?
		0,35,0,								// Total damage (HEB)
		10,									// Damage radius
		4,									// Detonate speed (in tics)
		2,									// Explode speed (in tics)
		10,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_PROTONFLARE,						// An animated sprite
		(PALETTE *)blueflame,				// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREPHOTON,					// Tank fire sound
		EXPLODE_PHOTON,						// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Machine Gun",						// Name
		"",									// HUD Name
		MICON_2BALLISTIC,					// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOGRAVITY|
		MISSILE_GHOST|
		MISSILE_BEAM|
		MISSILE_NOHITSELF),					// Missile Flags
		10000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		0,0,360,							// Total damage (HEB)
		100,								// Damage radius
		2,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		60,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BULLETTRACER,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		MachineGun_Thinker,					// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		-1,									// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},

	{
		"Machine Gun Child",				// Name
		"",									// HUD Name 
		MICON_2BALLISTIC,					// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOGRAVITY|
		MISSILE_NOLIST|
		MISSILE_BEAM|
		MISSILE_NOHITSELF),					// Missile Flags
		10000,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		0,0,10,								// Total damage (HEB)
		5,									// Damage radius
		2,									// Detonate speed (in tics)
		10,									// Explode speed (in tics)
		5,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BULLETTRACER,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		MachineGun_TankHit,					// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREMACHINEGUN,				// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},


	{
		"Laser Pulse",					// Name
		"",								// HUD Name 
		MICON_2ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOSMOKE|
		 MISSILE_NODIRTCLIP|
		 MISSILE_BEAM|
		 MISSILE_NOHITSELF),			// Missile Flags
		10000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		0,250,0,						// Total damage (HEB)
		30,								// Damage radius
		10,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		35,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_GREENLASER_PULSE,				// An animated sprite
		(PALETTE *)greenpal,			// Selected palette to explode with
		LaserPulse_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRELASER,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
		
	},

	{
		"Tunnel Laser",				// Name
		"",								// HUD Name 
		MICON_2ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOSMOKE|
		 MISSILE_NODIRTCLIP|
		 MISSILE_BEAM|
		 MISSILE_NOHITSELF),			// Missile Flags
		10000,							// Cost
		5,								// With each purchase
		true,							// Rotates?
		10,10,10,						// Total damage (HEB)
		30,								// Damage radius
		10,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		35,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_GREENLASER_PULSE,				// An animated sprite
		(PALETTE *)greenpal,			// Selected palette to explode with
		TunnelLaser_Thinker,			// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRELASER,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
		
	},


	{
		"Uranium Shell",				// Name
		"",								// HUD Name 
		MICON_2BALLISTIC,				// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE),				// Missile Flags
		10000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,0,275,						// Total damage (HEB)
		65,								// Damage radius
		60,								// Detonate speed (in tics)
		60,								// Explode speed (in tics)
		1,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_URANIUMSHELL,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		EXPLODE_URANIUMSHELL,			// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Large Sparkball",				// Name
		"",								// HUD Name 
		MICON_2ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOBUBBLES),			// Missile Flags
		10000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,275,0,						// Total damage (HEB)
		80,								// Damage radius
		15,								// Detonate speed (in tics)
		50,								// Explode speed (in tics)
		(int)(1.2*TICRATE),				// Explode time (in tics)
		NULL,							// Explosion hook
		S_SPARKBALL1,					// An animated sprite
		(PALETTE *)darkbluepal,			// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		EXPLODE_SMALL,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"HeatWave",							// Name
		"",									// HUD Name 
		MICON_2HEAT,						// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE),					// Missile Flags
		10000,								// Cost
		5,									// With each purchase
		false,								// Rotates?
		275,0,0,							// Total damage (HEB)
		1,									// Damage radius
		1,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		(int)(1.2*TICRATE),					// Explode time (in tics)
		Heatwave_Explode,					// Explosion hook
		S_HEATWAVE,							// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREBIG,						// Tank fire sound
		EXPLODE_HEATWAVE,					// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"EMP Missile",						// Name
		"",									// HUD Name 
		MICON_2CONVENTIONAL,				// Buystuff icon type
		(MISSILE_STANDARD),					// Missile Flags
		10000,								// Cost
		5,									// With each purchase
		true,								// Rotates?
		0,0,0,								// Total damage (HEB)
		1,									// Damage radius
		1,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		(int)(1.2*TICRATE),					// Explode time (in tics)
		EMP_Explode,						// Explosion hook
		S_EMPMISSILE1,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		-1,									// Explosion sound
		-1,									// Can be used by ALL characters
	},


	{
		"Large Lava Rock",					// Name
		"",									// HUD Name 
		(MICON_1HEAT|
		MICON_LIQUID),						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NAPALM1|
		MISSILE_NOTANKCLIP|
		MISSILE_NOLIST),					// Missile Flags
		10000,								// Cost
		5,									// With each purchase
		true,								// Rotates?
		150,0,0,							// Total damage (HEB)
		1,									// Damage radius
		1,									// Detonate speed (in tics)
		1,									// Explode speed (in tics)
		(int)(1.2*TICRATE),					// Explode time (in tics)
		NULL,								// Explosion hook
		S_BIGLAVAROCK,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		LAVA_FIRE,							// Tank fire sound
		EXPLODE_LAVA,						// Explosion sound
		-1,									// Can be used by ALL characters

		// Naplam Properties
		100,								// Number of pixels to fill the land with
		2,0,0,								// Damage each pixel does to a tank (HEB)
		65,									// Particle force
		(int)(4*TICRATE),					// Time napalm stays on the ground
		(int)(3*TICRATE-1.2*TICRATE),		// Duration of calculated damage
		255,25,0,//makecol(255,25,0),		// Color (R,G,B)

	},

	{
		"Napalm",							// Name
		"",									// HUD Name 
		(MICON_2HEAT|
		MICON_LIQUID),						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NAPALM1|
		MISSILE_NOSMOKE|
		MISSILE_NOTANKCLIP),							// Missile Flags
		10000,											// Cost
		3,												// With each purchase
		true,											// Rotates?
		150,0,0,										// Total damage (HEB)
		1,												// Damage radius
		1,												// Detonate speed (in tics)
		1,												// Explode speed (in tics)
		(int)(1.2*TICRATE),								// Explode time (in tics)
		NULL,							// Explosion hook
		S_NAPALMCANISTER,								// An animated sprite
		(PALETTE *)firepal,								// Selected palette to explode with
		NULL,											// Function hook
		NULL,											// Hit function hook
		NULL,											// Ground-impact function hook
		TANK_FIRESMALL,									// Tank fire sound
		EXPLODE_CANISTER,								// Explosion sound
		-1,												// Can be used by ALL characters

		// Naplam Properties
		125,											// Number of pixels to fill the land with
		2,0,0,											// Damage each pixel does to a tank (HEB)
		65,												// Particle force
		(int)(2*TICRATE),								// Time napalm stays on the ground
		(int)(1.5*TICRATE),//(int)(3*TICRATE-1.2*TICRATE),					// Duration of calculated damage
		255,25,0,//makecol(255,25,0),					// Color (R,G,B)

	},

	{
		"Acid Splash",						// Name
		"",												// HUD Name 
		(MICON_2BALLISTIC|
		MICON_LIQUID),						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NAPALM|
		MISSILE_NOSMOKE|
		MISSILE_NOTANKCLIP),			// Missile Flags
		10000,										// Cost
		3,												// With each purchase
		true,											// Rotates?
		0,0,150,									// Total damage (HEB)
		1,												// Damage radius
		1,												// Detonate speed (in tics)
		1,												// Explode speed (in tics)
		(int)(1.2*TICRATE),				// Explode time (in tics)
		NULL,											// Explosion hook
		S_ACIDCANISTER,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,											// Function hook
		NULL,											// Hit function hook
		NULL,											// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_CANISTER,					// Explosion sound
		-1,												// Can be used by ALL characters

		// Naplam Properties
		125,											// Number of pixels to fill the land with
		0,0,2,										// Damage each pixel does to a tank
		65,												// Particle force
		(int)(2*TICRATE),					// Time napalm stays on the ground
		(int)(1.5*TICRATE),				// Duration of calculated damage
		100,255,0,								// Color (R,G,B)

	},

	{
		"Plasma",									// Name
		"",												// HUD Name 
		(MICON_2ENERGY|
		MICON_LIQUID),						// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_PLASMA|
		MISSILE_NOSMOKE|
		MISSILE_NOTANKCLIP),			// Missile Flags
		10000,										// Cost
		3,												// With each purchase
		true,											// Rotates?
		0,150,0,									// Total damage (HEB)
		1,												// Damage radius
		1,												// Detonate speed (in tics)
		1,												// Explode speed (in tics)
		(int)(1.2*TICRATE),				// Explode time (in tics)
		NULL,											// Explosion hook
		S_PLASMACANISTER,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,											// Function hook
		NULL,											// Hit function hook
		NULL,											// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_CANISTER,					// Explosion sound
		-1,												// Can be used by ALL characters

		// Naplam Properties
		115,											// Number of pixels to fill the land with
		0,2,0,										// Damage each pixel does to a tank
		65,												// Particle force
		(int)(2*TICRATE),					// Time napalm stays on the ground
		(int)(1.5*TICRATE),				// Duration of calculated damage
		255,210,100,							// Color (R,G,B)

	},

	{
		"Fire Sentry Pod",				// Name
		"Fire Sentry",					// HUD Name 
		(MICON_2HEAT|
		MICON_AI),					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOTANKCLIP|
		 MISSILE_NOSMOKE),				// Missile Flags
		10000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		250,0,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		HSentry_Spawn,					// Explosion hook
		S_HSENTRY1,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Electric Sentry Pod",			// Name
		"Electric Sentry",				// HUD Name 
		(MICON_2ENERGY|
		MICON_AI),					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOTANKCLIP|
		 MISSILE_NOSMOKE),				// Missile Flags
		10000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,250,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		ESentry_Spawn,					// Explosion hook
		S_ESENTRY1,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Ballistic Sentry Pod",			// Name
		"Ballistic Sentry",				// HUD Name 
		(MICON_2BALLISTIC|
		MICON_AI),					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOTANKCLIP|
		 MISSILE_NOSMOKE),				// Missile Flags
		10000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,0,250,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		BSentry_Spawn,					// Explosion hook
		S_BSENTRY1,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Magnetic Generator Pod",			// Name
		"MagGen",							// HUD Name 
		MICON_2CONVENTIONAL,	// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOTANKCLIP|
		 MISSILE_NOSMOKE),		// Missile Flags
		10000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,0,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		0,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYNUKE,				// An animated sprite - use needle for want of something better.
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		Maggen_Release,		// Ground-impact function hook
		TANK_FIRESMALL,		// Tank fire sound
		TANK_ELECTRICCURRENT,								// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Heat Ray",						// Name
		"",								// HUD Name
		MICON_2HEAT,					// Buystuff icon type
		(MISSILE_STANDARD|				// Missile Flags
		 MISSILE_NOSMOKE|
		 MISSILE_BEAM|
		 MISSILE_GHOST),
		13000,							// Cost
		2,								// With each purchase
		true,							// Rotates?
		250,0,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		Heatray_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Ion Cannon",					// Name
		"",								// HUD Name 
		MICON_2ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|				// Missile Flags
		 MISSILE_NOSMOKE|
		 MISSILE_BEAM|
		 MISSILE_GHOST),
		13000,							// Cost
		2,								// With each purchase
		true,							// Rotates?
		0,250,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		LaserBlast_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Railgun",						// Name
		"",								// HUD Name 
		MICON_2BALLISTIC,				// Buystuff icon type
		(MISSILE_STANDARD|				// Missile Flags
		 MISSILE_NOSMOKE|
		 MISSILE_BEAM|
		 MISSILE_GHOST),
		13000,							// Cost
		2,								// With each purchase
		true,							// Rotates?
		0,0,250,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		Railgun_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Super Napalm",					// Name
		"",								// HUD Name 
		MICON_3HEAT|MICON_LIQUID,		// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NAPALM2|
		MISSILE_NOSMOKE|
		MISSILE_NOTANKCLIP),			// Missile Flags
		18000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		450,0,0,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		(int)(1.2*TICRATE),				// Explode time (in tics)
		NULL,							// Explosion hook
		S_NAPALMCANISTER,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		EXPLODE_CANISTER,				// Explosion sound
		-1,								// Can be used by ALL characters

		// Naplam Properties
		225,							// Number of pixels to fill the land with
		3,0,0,							// Damage each pixel does to a tank
		125,							// Particle force
		(int)(2.5*TICRATE),								// Time napalm stays on the ground
		(int)(2*TICRATE),								// Duration of calculated damage
		255,25,0,//makecol(255,25,0),	// Color (R,G,B)
	},

	{
		"Super Plasma",					// Name
		"",								// HUD Name 
		MICON_3ENERGY|MICON_LIQUID,		// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NAPALM|
		MISSILE_NOSMOKE|
		MISSILE_NOTANKCLIP|
		MISSILE_PLASMA),								// Missile Flags
		18000,											// Cost
		3,												// With each purchase
		true,											// Rotates?
		0,450,0,										// Total damage (HEB)
		1,												// Damage radius
		1,												// Detonate speed (in tics)
		1,												// Explode speed (in tics)
		(int)(1.2*TICRATE),								// Explode time (in tics)
		NULL,											// Explosion hook
		S_PLASMACANISTER,								// An animated sprite
		(PALETTE *)firepal,								// Selected palette to explode with
		NULL,											// Function hook
		NULL,											// Hit function hook
		NULL,											// Ground-impact function hook
		TANK_FIRESMALL,									// Tank fire sound
		EXPLODE_CANISTER,								// Explosion sound
		-1,												// Can be used by ALL characters

		// Naplam Properties
		225,											// Number of pixels to fill the land with
		0,3,0,											// Damage each pixel does to a tank
		125,											// Particle force
		(int)(2.5*TICRATE),								// Time napalm stays on the ground
		(int)(2*TICRATE),								// Duration of calculated damage
		255,210,100,//makecol(255,25,0),					// Color (R,G,B)

	},

	{
		"Super Acid",					// Name
		"",								// HUD Name 
		MICON_3BALLISTIC|MICON_LIQUID,	// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NAPALM|
		MISSILE_NOSMOKE|
		MISSILE_NOTANKCLIP),							// Missile Flags
		18000,											// Cost
		3,												// With each purchase
		true,											// Rotates?
		0,0,450,										// Total damage (HEB)
		1,												// Damage radius
		1,												// Detonate speed (in tics)
		1,												// Explode speed (in tics)
		(int)(1.2*TICRATE),								// Explode time (in tics)
		NULL,											// Explosion hook
		S_ACIDCANISTER,									// An animated sprite
		(PALETTE *)firepal,								// Selected palette to explode with
		NULL,											// Function hook
		NULL,											// Hit function hook
		NULL,											// Ground-impact function hook
		TANK_FIRESMALL,									// Tank fire sound
		EXPLODE_CANISTER,								// Explosion sound
		-1,												// Can be used by ALL characters

		// Naplam Properties
		225,											// Number of pixels to fill the land with
		0,0,3,												// Damage each pixel does to a tank
		125,											// Particle force
		(int)(2.5*TICRATE),								// Time napalm stays on the ground
		(int)(2*TICRATE),								// Duration of calculated damage
		100,255,0,										// Color (R,G,B)

	},

	{
		"Nuke",							// Name
		"",								// HUD Name 
		MICON_3CONVENTIONAL,			// Buystuff icon type
		MISSILE_STANDARD,				// Missile Flags
		20000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		110,110,110,					// Total damage (HEB)
		125,							// Damage radius
		3,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		90,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_NUKE,							// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		TANK_HEAVYEXPLODE2,				// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Mortar Shell",				// Name
		"",								// HUD Name 
		MICON_3BALLISTIC,				// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOLIST),				// Missile Flags
		20000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,0,650,						// Total damage (HEB)
		90,								// Damage radius
		60,								// Detonate speed (in tics)
		60,								// Explode speed (in tics)
		1,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_MORTARSHELL,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		MortarShell_Explode,			// Ground-impact function hook
		TANK_FIREMORTAR,				// Tank fire sound
		EXPLODE_MORTAR,					// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Moon Shot",					// Name
		"",								// HUD Name 
		MICON_3ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOLIST),				// Missile Flags
		20000,							// Cost
		3,								// With each purchase
		false,							// Rotates?
		0,650,0,						// Total damage (HEB)
		90,								// Damage radius
		60,								// Detonate speed (in tics)
		60,								// Explode speed (in tics)
		1,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_SB_MOONSHOT,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		Moonshot_Thinker,				// Function hook
		NULL,							// Hit function hook
		MortarShell_Explode,			// Ground-impact function hook
		TANK_FIREMOONSHOT,				// Tank fire sound
		EXPLODE_MOONSHOT,				// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Lightning Rod",				// Name
		"",								// HUD Name 
		MICON_3ENERGY,					// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE),				// Missile Flags
		20000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		0,475,0,						// Total damage (HEB)
		30,								// Damage radius
		2,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		30,								// Explode time (in tics)
		Rod_Explode,					// Explosion hook
		S_LIGHTNINGROD1,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Flak Detonator",				// Name
		"",								// HUD Name 
		MICON_3BALLISTIC,				// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE),				// Missile Flags
		20000,							// Cost
		3,								// With each purchase
		false,							// Rotates?
		0,0,450,						// Total damage (HEB)
		30,								// Damage radius
		2,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		30,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_FLAKSHELL,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		FlakShard_TankHit,				// Hit function hook
		Flak_Explode,					// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		EXPLODE_FLAK,					// Explosion sound
		-1,								// Can be used by ALL characters
	},

	{
		"Flak Shard 1",			// Name
		"",									// HUD Name
		MICON_2BALLISTIC,		// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOLIST|
		 MISSILE_NOGRAVITY|
		 MISSILE_NODIRTCLIP),	// Missile Flags
		200,							// Cost
		1,								// With each purchase
		false,						// Rotates?
		0,0,200,					// Total damage (HEB)
		15,								// Damage radius must equal "Flak Shard 2"
		2,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		10,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_FLAKSHARD1,			// An animated sprite
		(PALETTE *)firepal,	// Selected palette to explode with
		FlakShard_Thinker,	// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Flak Shard 2",		// Name
		"",								// HUD Name 
		MICON_2BALLISTIC,	// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOGRAVITY|
		 MISSILE_NOLIST|
		 MISSILE_NODIRTCLIP),	// Missile Flags
		200,							// Cost
		1,								// With each purchase
		false,						// Rotates?
		0,0,200,					// Total damage (HEB)
		15,								// Damage radius must equal "Flak Shard 1"
		2,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		10,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_FLAKSHARD2,			// An animated sprite
		(PALETTE *)firepal,	// Selected palette to explode with
		FlakShard_Thinker,	// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Fire Starter",				// Name
		"",							// HUD Name 
		MICON_3HEAT,				// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_CRUISELAND|
		 MISSILE_NOTANKCLIP),			// Missile Flags
		20000,							// Cost
		3,								// With each purchase
		true,							// Rotates?
		475,0,0,						// Total damage (HEB)
		60,								// Damage radius
		2,								// Detonate speed (in tics)
		2,								// Explode speed (in tics)
		30,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_FIRESTARTER,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		FireStarter_Thinker,			// Function hook
		NULL,							// Hit function hook
		FireStarter_KillFlames,			// Ground-impact function hook
		TANK_FIREBIG,					// Tank fire sound
		EXPLODE_FIRESTARTER,			// Explosion sound
		-1,								// Can be used by ALL characters
	},


	{
		"Death's Head",					// Name
		"",								// HUD Name 
		MICON_SUPERHEAT,				// Buystuff icon type
		(MISSILE_STANDARD),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		700,0,0,						// Total damage (HEB)
		135,							// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		60,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_DEATHSHEADFULL_1,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		Deathshead_Thinker,				// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREDEATHSHEAD,			// Tank fire sound
		DEATHSHEAD_EXPLODE,				// Explosion sound
		CHARACTER_APRIL,				// Can only be used by April
	},

	{
		"Death's Head Child",			// Name
		"",								// HUD Name 
		MICON_SUPERHEAT,				// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOLIST),				// Missile Flags
		5000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		400,0,0,						// Total damage (HEB)
		115,							// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		55,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_DEATHSHEADCHILD,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		DEATHSHEADCHILD_EXPLODE,		// Explosion sound
		CHARACTER_APRIL,				// Can only be used by April
	},

	{
		"Giga Nuke",					// Name
		"",								// HUD Name 
		MICON_SUPERCONVENTIONAL,		// Buystuff icon type
		(MISSILE_STANDARD),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		300,300,300,					// Total damage (HEB)
		255,							// Damage radius
		4,								// Detonate speed (in tics)
		5,								// Explode speed (in tics)
		120,							// Explode time (in tics)
		NULL,							// Explosion hook
		S_GIGANUKE,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREDEATHSHEAD,			// Tank fire sound
		DEATHSHEAD_EXPLODE,				// Explosion sound
		CHARACTER_MARIERICKY,			// Can only be used by Marie & Ricky
	},

	{
		"Monkey Mortar",				// Name
		"",								// HUD Name 
		MICON_SUPERBALLISTIC,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_GHOST),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,0,700,						// Total damage (HEB)
		70,								// Damage radius
		60,								// Detonate speed (in tics)
		60,								// Explode speed (in tics)
		1,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_MORTARSHELL,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		MonkeyMortar_Thinker,			// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_SARU,					// Can be only used by Saru
	},

	{
		"Polaris Missile",				// Name
		"",								// HUD Name 
		MICON_SUPERHEAT,				// Buystuff icon type
		(MISSILE_STANDARD),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		700,0,0,						// Total damage (HEB)
		150,								// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		20,								// Explode time (in tics)
		Polaris_Explode,				// Explosion hook
		S_POLARISMISSILE1,				// An animated sprite
		(PALETTE *)blueflame,			// Selected palette to explode with
		PolarisBeep_Thinker,			// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREDEATHSHEAD,			// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_MESHKO,				// Can be only used by Meshko
	},

	{
		"Black Hole Bomb",				// Name
		"Blck Hole Bomb",				// HUD Name 
		MICON_SUPERBALLISTIC,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,0,700,						// Total damage (HEB)
		175,							// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		20,								// Explode time (in tics)
		BHBomb_Explode,					// Explosion hook
		S_BHBOMB,						// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREDEATHSHEAD,			// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_JACK,					// Can be only used by Jack
	},

	{
		"Twin Electric Lances",			// Name
		"Twin Elct Lnces",				// HUD Name 
		MICON_SUPERENERGY,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_ALWAYSBOUNCE),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,700,0,						// Total damage (HEB)
		150,							// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		20,								// Explode time (in tics)
		TEL_PositiveExplode,			// Explosion hook
		S_TEL_POSITIVECHARGE,			// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_MIKI,					// Can be only used by Miki
	},

	{
		"Twin Elec. Lances - Negative Charge",// Name
		"Twin Elct Lances",			// HUD Name 
		MICON_SUPERENERGY,			// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE|
		 MISSILE_NOLIST|
		 MISSILE_NOBOTUSE|
		 MISSILE_ALWAYSBOUNCE),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,700,0,						// Total damage (HEB)
		150,							// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		20,								// Explode time (in tics)
		TEL_NegativeExplode,			// Explosion hook
		S_TEL_NEGATIVECHARGE,			// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIRESMALL,					// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_MIKI,					// Can be only used by Miki
	},

	{
		"Funky Nuke",						// Name
		"",									// HUD Name 
		MICON_SUPERCONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE),					// Missile Flags
		80000,								// Cost
		1,									// With each purchase
		true,								// Rotates?
		300,300,300,						// Total damage (HEB)
		30,									// Damage radius
		10,									// Detonate speed (in tics)
		2,									// Explode speed (in tics)
		35,									// Explode time (in tics)
		A_FunkyNukeExplode,					// Explosion hook
		S_FUNKYNUKE,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREDEATHSHEAD,				// Tank fire sound
		EXPLODE_FUNKYNUKE,					// Explosion sound
		CHARACTER_RICHIE,					// Can be only used by Richie
	},

	{
		"Funky Nuke Child",						// Name
		"",									// HUD Name 
		MICON_2CONVENTIONAL,				// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE|
		MISSILE_NOLIST|
		MISSILE_NOBOTUSE),					// Missile Flags
		6500,								// Cost
		3,									// With each purchase
		true,								// Rotates?
		200,200,200,						// Total damage (HEB)
		95,									// Damage radius
		30,									// Detonate speed (in tics)
		15,									// Explode speed (in tics)
		90,									// Explode time (in tics)
		NULL,								// Explosion hook
		S_BABYMISSILE,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		EXPLODE_SMALL,						// Explosion sound
		CHARACTER_RICHIE,					// Can be only used by Richie
	},

	{
		"Air Strike",						// Name
		"",									// HUD Name 
		MICON_SUPERCONVENTIONAL,			// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE),					// Missile Flags
		80000,								// Cost
		1,									// With each purchase
		true,								// Rotates?
		300,300,300,						// Total damage (HEB)
		200,								// Damage radius
		10,									// Detonate speed (in tics)
		2,									// Explode speed (in tics)
		35,									// Explode time (in tics)
		A_AS_FlareExplode,					// Explosion hook
		S_AS_FLARESHOT,						// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIRESMALL,						// Tank fire sound
		-1,									// Explosion sound
		CHARACTER_MIA,						// Can be only used by Mia
	},

	{
		"Orbital Beam Cannon",				// Name
		"O.B. Cannon",						// HUD Name 
		MICON_SUPERENERGY,				// Buystuff icon type
		(MISSILE_STANDARD|
		MISSILE_NOSMOKE),					// Missile Flags
		80000,								// Cost
		1,									// With each purchase
		true,								// Rotates?
		0,700,0,							// Total damage (HEB)
		225,								// Damage radius
		10,									// Detonate speed (in tics)
		2,									// Explode speed (in tics)
		35,									// Explode time (in tics)
		A_OBC_Explode,						// Explosion hook
		S_OBCBEACON_SHOT,					// An animated sprite
		(PALETTE *)firepal,					// Selected palette to explode with
		NULL,								// Function hook
		NULL,								// Hit function hook
		NULL,								// Ground-impact function hook
		TANK_FIREDEATHSHEAD,				// Tank fire sound
		-1,									// Explosion sound
		CHARACTER_LOCK,						// Can be only used by Lock
	},

	{
		"Rail Ricochet",				// Name
		"",								// HUD Name 
		MICON_SUPERBALLISTIC,			// Buystuff icon type
		(MISSILE_STANDARD|				// Missile Flags
		 MISSILE_NOSMOKE|
		 MISSILE_BEAM|
		 MISSILE_GHOST|
		 MISSILE_INSTANTHIT),
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,0,700,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		SuperRailgun_Thinker,			// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_SMACKBRINGER,			// Can be only used by smackbringer
	},


	{
		"Rail Ricochet Child",			// Name
		"",								// HUD Name 
		MICON_SUPERBALLISTIC,			// Buystuff icon type
		(MISSILE_STANDARD|				// Missile Flags
		 MISSILE_NOSMOKE|
		 MISSILE_BEAM|
		 MISSILE_GHOST|
		 MISSILE_NOLIST),
		20000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,0,700,						// Total damage (HEB)
		1,								// Damage radius
		1,								// Detonate speed (in tics)
		1,								// Explode speed (in tics)
		50,								// Explode time (in tics)
		NULL,							// Explosion hook
		S_BABYMISSILE,					// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		SuperRailgunChild_Thinker,		// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		-1,								// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_SMACKBRINGER,			// Can be only used by smackbringer
	},


	{
		"Ring of Fire",					// Name
		"",								// HUD Name 
		MICON_SUPERHEAT,				// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,0,700,						// Total damage (HEB)
		300,							// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		20,								// Explode time (in tics)
		ROF_Explode,					// Explosion hook
		S_ROF_PROJECTILE,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREDEATHSHEAD,			// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_JIRA,					// Can be only used by Jira
	},


	{
		"Silver Bullet",				// Name
		"",								// HUD Name 
		MICON_SUPERENERGY,				// Buystuff icon type
		(MISSILE_STANDARD|
		 MISSILE_NOSMOKE),				// Missile Flags
		80000,							// Cost
		1,								// With each purchase
		true,							// Rotates?
		0,700,0,						// Total damage (HEB)
		300,							// Damage radius
		2,								// Detonate speed (in tics)
		3,								// Explode speed (in tics)
		20,								// Explode time (in tics)
		SB_Explode,						// Explosion hook
		S_SB_PROJECTILE,				// An animated sprite
		(PALETTE *)firepal,				// Selected palette to explode with
		NULL,							// Function hook
		NULL,							// Hit function hook
		NULL,							// Ground-impact function hook
		TANK_FIREDEATHSHEAD,			// Tank fire sound
		-1,								// Explosion sound
		CHARACTER_SHADOWFANG,			// Can be only used by ShadowFang
	},

};

/* do_line:
 *  Calculates all the points along a line between x1, y1 and x2, y2, 
 *  calling the supplied function for each one. This will be passed a 
 *  copy of the bmp parameter, the x and y position, and a copy of the 
 *  d parameter (so do_line() can be used with putpixel()).
 */
void do_float_line(BITMAP *bmp, float x1, float y1, float x2, float y2, int d, void (*proc)(BITMAP *, float, float, int))
{
   float dx = x2-x1;
   float dy = y2-y1;
   int i1, i2;
   float x, y;
   int dd;

   /* worker macro */
   #define DO_LINE(pri_sign, pri_c, pri_cond, sec_sign, sec_c, sec_cond)     \
   {                                                                         \
      if(d##pri_c == 0) {                                                   \
	 proc(bmp, x1, y1, d);                                               \
	 return;                                                             \
      }                                                                      \
									     \
      i1 = 2 * d##sec_c;                                                     \
      dd = i1 - (sec_sign (pri_sign d##pri_c));                              \
      i2 = dd - (sec_sign (pri_sign d##pri_c));                              \
									     \
      x = x1;                                                                \
      y = y1;                                                                \
									     \
      while (pri_c pri_cond pri_c##2) {                                      \
	 proc(bmp, x, y, d);                                                 \
									     \
	 if(dd sec_cond 0) {                                                \
	    sec_c sec_sign##= 1;                                             \
	    dd += i2;                                                        \
	 }                                                                   \
	 else                                                                \
	    dd += i1;                                                        \
									     \
	 pri_c pri_sign##= 1;                                                \
      }                                                                      \
   }
	///	end macro

   if(dx >= 0) {
      if(dy >= 0) {
	 if(dx >= dy) {
	    /* (x1 <= x2) && (y1 <= y2) && (dx >= dy) */
	    DO_LINE(+, x, <=, +, y, >=);
	 }
	 else {
	    /* (x1 <= x2) && (y1 <= y2) && (dx < dy) */
	    DO_LINE(+, y, <=, +, x, >=);
	 }
      }
      else {
	 if(dx >= -dy) {
	    /* (x1 <= x2) && (y1 > y2) && (dx >= dy) */
	    DO_LINE(+, x, <=, -, y, <=);
	 }
	 else {
	    /* (x1 <= x2) && (y1 > y2) && (dx < dy) */
	    DO_LINE(-, y, >=, +, x, >=);
	 }
      }
   }
   else {
      if(dy >= 0) {
	 if(-dx >= dy) {
	    /* (x1 > x2) && (y1 <= y2) && (dx >= dy) */
	    DO_LINE(-, x, >=, +, y, >=);
	 }
	 else {
	    /* (x1 > x2) && (y1 <= y2) && (dx < dy) */
	    DO_LINE(+, y, <=, -, x, <=);
	 }
      }
      else {
	 if(-dx >= -dy) {
	    /* (x1 > x2) && (y1 > y2) && (dx >= dy) */
	    DO_LINE(-, x, >=, -, y, <=);
	 }
	 else {
	    /* (x1 > x2) && (y1 > y2) && (dx < dy) */
	    DO_LINE(-, y, >=, -, x, <=);
	 }
      }
   }
}



//
// LaserLine
//

#define			LASER_NORMAL		0
#define			LASER_NOKILLLAND	1
#define			LASER_NOBURNLAND	2
#define			LASER_CANDAMAGESELF	4

void LaserLine(int x, int y, float angle, int length, int thickness, int damageh, int damagee, int damageb, int tankfrom, int flags);

static float	lasercircle_angle;
static int		lasercircle_damageh, lasercircle_damagee, lasercircle_damageb;
static int		lasercircle_tankfrom;
static int		lasercircle_length;
static int		lasercircle_thickness;
static bool		lasercircle_damagedtank[MAX_TANKS];
static int		laserslice_count;
static int		laserburn_count;
static int		lasercircle_flags;
static int		lasercontactx;
static int		lasercontacty;

void	LaserLine_Proc(BITMAP *b, int x, int y, int color)
{
	// For clipping tanks
	int			tankx, tanky;
	int			i=0;

	// For clipping sprites
	sprite_t*	s = sprites;
	BITMAP		*sbmp;
	int			sx,sy;

	//
	// Clip any tanks first
	//
	for (i=0; i<MAX_TANKS; i++)
	{
		if(ta[i].health <= 0 || lasercircle_damagedtank[i] == true)
			continue;

		if(i==lasercircle_tankfrom && !(lasercircle_flags & LASER_CANDAMAGESELF))
			continue;

		tankx = ta[i].x-(ta[i].sizex/2);
		tanky = ta[i].y-(ta[i].sizey/2);

		if(x > tankx &&
			x <= tankx+ta[i].sizex &&
			y > tanky &&
			y <= tanky+14)
		{
			ta[i].Damage(lasercircle_damageh, lasercircle_damagee, lasercircle_damageb, lasercircle_tankfrom, false);
			lasercircle_damagedtank[i] = true;
		}
	}

	if(!OnScreen(buffer, x, y))
		return;
	else

	lasercontactx = x;
	lasercontacty = y;

	if(lasercontactx > SCREENWIDTH)
		lasercontactx = SCREENWIDTH;

	if(lasercontactx < 0)
		lasercontactx = 0;

	if(lasercontacty > SCREENHEIGHT)
		lasercontacty = SCREENHEIGHT;

	if(lasercontacty < 0)
		lasercontacty = 0;



	// Now clip against any sprites that are shootable
	while (s)
	{
		if(!(s->flags & MF_SHOOTABLE))
		{
			s = s->next;
			continue;
		}

		// Set our clipping bitmap
		sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);

		// Setup clipping coords
		sx = s->x;
		sy = s->y;	

		// New coordinate if this is centered
		if(s->flags & MF_CENTERCOORD)
		{
			sx = s->x - (sbmp->w/2);
			sy = s->y - (sbmp->h/2);
		}

		// Check pixel perfect collision
		if(CheckPointInSprite(s, x, y))
		{
			s->Damage(lasercircle_damageh, lasercircle_damagee, lasercircle_damageb);
			s->invulntics = 1.2*TICRATE;
		}

		s = s->next;
	}
}

void	LaserCircle_Proc(BITMAP *b, int x, int y, int color)
{
	do_line(buffer, x, y, x+(-lasercircle_length*COS(lasercircle_angle)), y+(lasercircle_length*-SIN(lasercircle_angle)), color, LaserLine_Proc);
}

void	LaserBurn_Proc(BITMAP *b, int x, int y, int color)
{
	int		burnrad = (lasercircle_thickness*2)+10;
	
	laserburn_count++;

	if(laserburn_count==(int)(lasercircle_thickness/2))	// 3;
	{
		BurnGround(ground.bmp, x, y, burnrad, 125);
		laserburn_count = 0;
	}
}

void	LaserSlice_Proc(BITMAP *b, int x, int y, int color)
{
	laserslice_count--;

	if(laserslice_count <= 0)
	{
		circlefill(b, x, y, lasercircle_thickness, TRANSCOLOR);
		laserslice_count = (lasercircle_thickness/2);
	}
}

void LaserLine(int x, int y, float angle, int length, int thickness, int damageh, int damagee, int damageb, int tankfrom, int flags)
{
	int				i;
	int				destx=0,desty=0;

	// Starting off, we havent hit any tanks yet
	for (i=0; i<MAX_TANKS; i++)
		lasercircle_damagedtank[i] = false;

	// Set damage varibles to pass to tanks that get hit
	lasercircle_damageh = damageh;
	lasercircle_damagee = damagee;
	lasercircle_damageb = damageb;

	// From whom is this coming from?
	lasercircle_tankfrom = tankfrom;

	// Setup flags
	lasercircle_flags = flags;

	// How long is this laser? Or the angle? Or the thickness?
	lasercircle_length = length;
	lasercircle_angle = angle;
	lasercircle_thickness = thickness;
	laserslice_count = (thickness/2);

	// Reset laser burning
	laserburn_count = 0;

	// Figure out exactly where we're going
	destx = x+(-lasercircle_length*COS(lasercircle_angle));
	desty = y+(lasercircle_length*-SIN(lasercircle_angle));
	
	// First, burn the ground
	if(!(lasercircle_flags & LASER_NOBURNLAND))
		do_line(buffer, x, y, destx, desty, 0, LaserBurn_Proc);

	//do_circle(buffer, x, y, thickness, TRANSCOLOR, LaserCircle_Proc);
	
	
	// [Sean] - Better instead of doing all these lines calls in a single do_circle. Although
	// do_circle is more through, it east up CPU time like crazy. As long as we get the top, bottom
	// left and right points, that should cover enough of the circle to still clip tanks/sprites accurately.

	// Top
	do_line(buffer, 
		x, 
		y-(lasercircle_thickness/2), 
		destx, 
		desty-(lasercircle_thickness/2), 
		TRANSCOLOR, LaserLine_Proc);

	// Bottom
	do_line(buffer, 
		x, 
		y+(lasercircle_thickness/2), 
		destx, 
		desty-(lasercircle_thickness/2), 
		TRANSCOLOR, LaserLine_Proc);

	// Left
	do_line(buffer, 
		x-(lasercircle_thickness/2), 
		y, 
		destx, 
		desty-(lasercircle_thickness/2), 
		TRANSCOLOR, LaserLine_Proc);

	// Right
	do_line(buffer, 
		x+(lasercircle_thickness/2), 
		y, 
		destx, 
		desty-(lasercircle_thickness/2), 
		TRANSCOLOR, LaserLine_Proc);

	// Now, slice the ground (from the middle to the destination x,y)
	// Place the transparent pixel onto the ground
	if(!(lasercircle_flags & LASER_NOKILLLAND))
	{
		do_line(ground.bmp, 
			x, 
			y, 
			destx, 
			desty, 
			TRANSCOLOR, LaserSlice_Proc);
	}

	// The laser most likely cut some of the ground away, update that here
	ground.needsupdate = true;
	
}


//
// ConvertFromGamePower - Converts the given number between 1 and 1000 to a varible
// that the sprites/particles can you for their physics.
//

float	ConvertFromGamePower(int	num)
{
	// Standard error checking
	if(num < 0)
		num = 0;
	//if(num > 1000)
	//	num = 1000;

	// Return das-uden value
	return num/33.33334;
}

//
// ConvertToLPower - Converts the decimal value into a power from 0-1000
//

float	ConvertToGamePower(float	num)
{
	// Standard error checking
	if(num < 0)
		num = 0;
	//if(num > 1000)
	//	num = 1000;

	// Return das-uden value
	return num*33.33334;
}

//
// GiveAmmo() - 
//
// Very generic function, gives the selected player some ammo
// and of course, adjusts the players money accordingly  =)
//

void GiveAmmo(int	player, int ammotype, int	amount)
{
	// For unlimited ammo types
	if(amount == UNLIMITED_AMMO)
	{
		ta[player].weapons[ammotype].ammo = amount;
		return;
	}
	
	ta[player].weapons[ammotype].ammo += amount;
}


//
// InitFire - Create a nice fire palette
//

void InitFire(void)
{
	
int i;
	for(i=0; i<64; i++) // Black to red
		{
			firepal[i].r=i;
			firepal[i].g=0;
			firepal[i].b=0;
		} // for(i=0; i<64; i++)*/
	for(i=64; i<128; i++) // Red to yellow  
		{
			firepal[i].r=63;
			firepal[i].g=i - 64;
			firepal[i].b=0;
		} // for(i=64; i<128; i++)
	for(i=128; i<256; i++) // Yellow to white
		{
			firepal[i].r=63;
			firepal[i].g=63;
			firepal[i].b=((i - 128) >> 1);
		} // for(i=128; i<256; i++)


	// Do the Blue fire now
	for(i=0; i<256; i++) // Black to red
	{
		blueflame[i].r=(i/4);
		blueflame[i].g=(i/4);
		blueflame[i].b=63;
	}

	// Do White-Green-Black
	for(i=0; i<128; i++) // Black to red
	{
		greenpal[i].r=0;
		greenpal[i].g=(i/2);
		greenpal[i].b=0;
	}

	for(i=128; i<256; i++) // Black to red
	{
		greenpal[i].r=((i-128)/2);
		greenpal[i].g=63;
		greenpal[i].b=((i-128)/2);
	}

	//
	// Riot blast palette
	riotpal[0].r = riotpal[0].g = riotpal[0].b = 0;

	for (i=1; i<256; i++)
	{ 
		riotpal[i].r = 55;
		riotpal[i].g = 22;
		riotpal[i].b = 63;
	}

	riotpal[2].r = 55;
	riotpal[2].g = 0;
	riotpal[2].b = 5;


	// Dark blue palette
	for(i=0; i<256; i++) 
	{
		darkbluepal[i].r=16;
		darkbluepal[i].g=8;
		darkbluepal[i].b=(i/4)+8;

		if(darkbluepal[i].b > 63)
			darkbluepal[i].b = 63;
	}

	// Funky bomb palette (various palettes)
	for (i=0; i<256; i+=4)
	{
		float	purple = 0.73*(float)i;
		float	orange = 0.56*(float)i;
		float	violet = 0.45*(float)i;

		fpurplepal[i].r = fpurplepal[i+1].r = fpurplepal[i+2].r = fpurplepal[i+3].r = purple/4;
		fpurplepal[i].g = fpurplepal[i+1].g = fpurplepal[i+2].g = fpurplepal[i+3].g = 0;
		fpurplepal[i].b = fpurplepal[i+1].b = fpurplepal[i+2].b = fpurplepal[i+3].b = i/4;

		forangepal[i].r = forangepal[i+1].r = forangepal[i+2].r = forangepal[i+3].r = i/4;
		forangepal[i].g = forangepal[i+1].g = forangepal[i+2].g = forangepal[i+3].g = orange/4;
		forangepal[i].b = forangepal[i+1].b = forangepal[i+2].b = forangepal[i+3].b = 0;

		fredpal[i].r = fredpal[i+1].r =fredpal[i+2].r =fredpal[i+3].r = i/4;
		fredpal[i].g = fredpal[i+1].g =fredpal[i+2].g =fredpal[i+3].g = 0;
		fredpal[i].b = fredpal[i+1].b =fredpal[i+2].b =fredpal[i+3].b = 0;

		fvoiletpal[i].r = fvoiletpal[i+1].r = fvoiletpal[i+2].r = fvoiletpal[i+3].r = i/4;
		fvoiletpal[i].g = fvoiletpal[i+1].g = fvoiletpal[i+2].g = fvoiletpal[i+3].g = violet/4;
		fvoiletpal[i].b = fvoiletpal[i+1].b = fvoiletpal[i+2].b = fvoiletpal[i+3].b = 0;
	}
}

//
// CycleWeapons() - Cycles through all the weapons the player has
//					ammo for.
//

bool TANK::SwitchWep(void)
{
	
	// Baby missile
	if(activeweapon > NUM_PROJECTILES-1) 
	{ 
	 		activeweapon = 0;
 		//{ return true; }
	}

	if(weapons[activeweapon].ammo > 0)
	{ return true; }

	return false;
}


void TANK::CycleWeapons(void)
{
	int		numtries=0;

	if(multiplayer && 
		multiplayermode == MP_CLIENT &&
		tankturn != net_localplayer)
		return;

#ifndef	DEDICATED_SERVER
	while(1)
		{ 
		activeweapon++;
		if(numtries == NUM_PROJECTILES)
			{
			G_Printf("CycleWeapons FAILED! Unable to find set an active weapon.");
			activeweapon = W_BABYMISSILE;
			return;
			}
		numtries++;

		if(SwitchWep()==true)
			return;

	}
#endif
}


void TANK::CycleWeaponsReverse(void)
{
#ifndef	DEDICATED_SERVER
	while(1)
	{ 
		activeweapon--;

		if(activeweapon < 0)
			activeweapon = NUM_PROJECTILES-1;

		if(weapons[activeweapon].ammo > 0)
			break;
	}
#endif
}

//
// countshot - Counts the number of active missiles still in the air
//

void SHOT::Init(int index, int projectiletype, float ax, float ay, float axvel, float ayvel, float adeg)
{
	exist=1;
    x=ax;
	y=ay;
	xvel=axvel;
	yvel=ayvel;
    rad=SHOT_SIZE;
    color=SHOT_COLOR;
	done=0;
	groundpixels=0;
	shottics = 0;

	// Clear out misc
	misc1= 0;
	misc2= 0;

	// Orginating degree, x and y values
	spawndeg = origdeg = adeg;
	spawnx = origx = ax;
	spawny = origy = ay;
	spawnxvel = axvel;
	spawnyvel = ayvel;

	origxvel = axvel;
	origyvel = ayvel;

	snd = NULL;

	// We're sailing through zee air
	onfloor = false;

	// Set our index num
	indexnum = index;

	// Identify the projectile
	projectile = projectiletype;

	// Set the default palettte
	pal = projectiles[projectiletype].usepal;

	// Setup flags
	flags = projectiles[projectiletype].flags;

	// Was the show spawned underwater?
	underwater = Underwater((int)x,(int)y);

	pixelfill = projectiles[projectile].pixelfill;

	// Check to see if this is a "instant hit" shot which is used for
	// bullet based weapons alot. If so, just apply ALOT of power onto
	// this shot (like, 3000 power. Not possible any time else)
	if(flags & MISSILE_INSTANTHIT)
	{
		xvel = (-ConvertFromGamePower(3000)*COS(origdeg));
		yvel = (ConvertFromGamePower(3000)*-SIN(origdeg));
	}

	// Shot is in the air!
	state = SSTATE_INAIR;
};



void	SHOT::InstantHit(int traceshot)
{
	while (this->state == SSTATE_INAIR && exist >= 1)
		Thinker();
}


//
// SHOT::ThrowDirt - When a shot first begins to explode, dirt below it
// is kicked up. This provides a neat little realistic effect
//

void SHOT::ThrowDirt(void)
{
	int		numparticles = (int)(projectiles[projectile].expradius/2);
	int		i;
	//float	oldxvel;
	//float	oldyvel;
	float   factor;
	particlenode_t		*p;
	

	
	for (i=0; i<numparticles; i++)
	{
		p = pdirt->InsertParticle(x,y-1,0,0,makecol(0,0,0), 1, PFLAGS_DIRT, 0, 0);
		
		// This will set the particle on the upper left hand corner
		// (where sprites are usually drawn)
		p->x -= projectiles[projectile].expradius;
		p->y -= projectiles[projectile].expradius;

		// From this point, keep picking out random points based on
		// how large the explosin will be
		p->x += (rANDom()%(projectiles[projectile].expradius*2));
		p->y += (rANDom()%(projectiles[projectile].expradius*2));

		// Finally, invert our xvel and yvel
		factor = (rANDom()%4)+1;
		p->xvel = -(xvel/factor);

		factor = (rANDom()%10);
		p->yvel = -(yvel/factor);

		p->bounce = 2;
	}
}

void SHOT::draw(BITMAP *b)
{
	int		drawx=(int)x;
	int		i;

	if(y <= 0)
	 {
		if(x > SCREENWIDTH) { x=SCREENWIDTH; }
		if(x <= 0) { x = 0; }

		// That looks about right
		AddRectangle(b, x-1, 34, 3, 1);

		putpixel(b, (int)x-1, 34, makecol(164,164,164));
		putpixel(b, (int)x,   34, makecol(255,255,255));
		putpixel(b, (int)x+1, 34, makecol(164,164,164));
		}

	 // [Sean] - The final two states are just drawing routines
	 // for the explosion effects. They in no way move the missile at
	 // at all.
	 // 
	 switch(state)
	 {
		case	SSTATE_NAPALMEXPLODE:
			{
			particlenode_t*		p;
			float	numpixels= projectiles[projectile].pixelfill;
			float	spawnangle;
			int		fflags=(PFLAGS_LIQUID|PFLAGS_DAMAGETANKS);

			// No more than 256 particles (complete circle) when spawning these.
			if(numpixels > 256)
				numpixels = 256;

			// If there is less than 1 particle, the game will crash with divide by zero error
			if(numpixels < 1)
				numpixels = 1;

			life = projectiles[projectile].pixeltime;
			damagelife = projectiles[projectile].pixeldamagetime;

			// Setup the flags for each of the two napalm missiles. Effect wise that is.
			if(flags & MISSILE_NAPALM1)
				{ fflags |= PFLAGS_NAPALMEFFECT1; }
			if(flags & MISSILE_NAPALM2)
				{ fflags |= PFLAGS_NAPALMEFFECT2; }
			if(flags & MISSILE_PLASMA)
				{ fflags |= (PFLAGS_GLOWING|PFLAGS_GLOWPULSE); }
			if(flags & MISSILE_WATER)
				{ fflags &= ~PFLAGS_DAMAGETANKS; }


			// [Sean] - 256 makes a complete circle
			for (i=0; i<numpixels; i++)
				{
				spawnangle = ((256/numpixels)*i);

					if(flags & MISSILE_WATER)
						{
						p = pwater->InsertParticle(x, y, spawnangle, 
							ConvertFromGamePower(projectiles[projectile].pixelpower), 
							makecol(projectiles[projectile].pixelcolorr,
									projectiles[projectile].pixelcolorg,
									projectiles[projectile].pixelcolorb), 
									0,
									fflags,
									0,0);
						if(p)
							{
							p->x += (p->xvel);
							p->y += (p->yvel);
							}
						ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
						}
					else
						{
						p = NewNapalmParticle((int)(x), (int)(y), 
							(int)spawnangle, 
							ConvertFromGamePower(projectiles[projectile].pixelpower), 
							makecol(projectiles[projectile].pixelcolorr,
									projectiles[projectile].pixelcolorg,
									projectiles[projectile].pixelcolorb), 
							life+(rANDom()%TICRATE), ///CheckRandomFunc
							fflags, 0,
							projectiles[projectile].pixeldamageh,
							projectiles[projectile].pixeldamagee,
							projectiles[projectile].pixeldamageb,
							damagelife);
	
						if(p && (flags & MISSILE_PLASMA))
							{
							p->glowr = 255;
							p->glowg = 250;
							p->glowb = 163;
							p->glowopacity = 1;
							}
						}

				}

				state = SSTATE_IDLE;
				break;
	
			}

		case	SSTATE_IDLE:
			{
			NapalmTanks();
			break;
			}

		case	SSTATE_EXPLODING:
			{
			Blowup(b, projectiles[projectile].expradius);
			break;
			}

		case	SSTATE_FINISHING:
			{
			ExplodeEffect(b, projectiles[projectile].expradius, 
			projectiles[projectile].exptime,
			projectiles[projectile].expspeed);
			break;
			}

		default:
			break;
	 }
}


void SHOT::explode(bool multiplayer_override)
{
	int damage = projectiles[projectile].expradius;

	// Ghost missiles exist only for other missiles, not to explode
	if(projectiles[projectile].flags & MISSILE_GHOST)
		return;
	
	if(state != SSTATE_INAIR)
		return;

	// If the missile has a explosion sound, play it here
	if(projectiles[projectile].explodesound >= 0)
		{
		StartSound(projectiles[projectile].explodesound, 0, 1);
		}

	if(projectiles[projectile].explodefunc != NULL)
		{
		// The server will take care of this for us
		//if(multiplayer && multiplayermode == MP_CLIENT)
		//	return;
		ServerClient = 'f';	//	function destroy dirt
		projectiles[projectile].explodefunc(indexnum);
		return;
		}

	// Set state
	state = SSTATE_EXPLODING;
	
	// Napalm shots do their own thing when exploding
	if((flags & MISSILE_NAPALM1) ||
		(flags & MISSILE_NAPALM2) ||
		(flags & MISSILE_NAPALM)  ||
		(flags & MISSILE_PLASMA)  ||
		(flags & MISSILE_WATER))
		state = SSTATE_NAPALMEXPLODE;
	
	// Reset all counters and set damage varibles. Prepare for explosion
	damagedone=1;
	damagetics=projectiles[projectile].detonatespeed;
	indexrate=0;
	colorindex=0;
	done=0;

	// Create our bitmap that will hold the explosion
	set_palette((RGB *)pal);
	if(flags & MISSILE_DIRT)
		{
		boombmp = create_bitmap(damage*2,damage*2);
		clear_to_color(boombmp, TRANSCOLOR);
		}

	// Sprite that the missile is attached to is no longer visable
	sprite->flags |= MF_NOTVISABLE;	
}

void	callbackshot(BITMAP *bmp, float x1, float y1, int color)
{
	float	tempx, tempy;
	int		p = -1;
	char	str[700];

	
	if(contactx >= 0 && contacty >= 0) return;	//	contact has been made
	if(ignoreshot) return;											//	wait for next line segment

	// Check and see if this particular missile has the "no clip" flag set which will allow it to pass
	// through the ground. It only keeps track of how many pixels it has gone through
	// with 'groundpixels'. Usually another external function will take care of the rest.
	if(sh[shotindex].flags & MISSILE_NODIRTCLIP)
		{ 
		// We only care if it hits the bottom part of the screen
		if(x1 < 0 || x1 > SCREENWIDTH || y1 < 0)
			return;

		// Hits the bottom of the screen
		if(y1 > SCREENHEIGHT)
			{
			contactx = x1;
			contacty = y1;
			return;
			}

		// Make a "slice" into the ground
		if(getpixel(ground.bmp, x1,y1) != TRANSCOLOR)
			{
			switch( sh[shotindex].projectile)
				{
				case  W_TUNNELLASER : // offset so concrete is on top part of tunnel only
					ConcreteGround(ground.bmp, x1, y1-2, 10);
					ServerClient = 't';
					///circlefill(ground.bmp,x1, y1, 5,TRANSCOLOR);
					DestroyDirt(x1, y1, 5);	//	TUNNELLASER
					break;
				default :
					BurnGround(ground.bmp, x1, y1, 14, 50);
					ServerClient = 'l';
					DestroyDirt(x1, y1, 3);	//	laser
				}
			sh[shotindex].groundpixels++; 
			ground.needsupdate = true;
			}
	}

	if(OnScreen(bmp, x1, y1) == -1)
		return;

	//	set up for CheckForShots()
	//	CheckForShots() may modify sh[shotindex] x & y and
	//	it may call explode which is not good policy
	//	for a routine that is to check!!
	// that is the reason for all this hackwork!!!
	tempx = sh[shotindex].x;
	tempy = sh[shotindex].y;
	
	sh[shotindex].x =x1;
	sh[shotindex].y =y1;

	// Tank clipping anyone?
	if(!(sh[shotindex].flags & MISSILE_NOTANKCLIP))
		p = CheckForShots(shotindex);	// checking if a tank was hit!

	x1 = sh[shotindex].x;
	y1 = sh[shotindex].y;
	sh[shotindex].x =tempx;	// reset from CheckForShots()
	sh[shotindex].y =tempy;

	// Check for a direct hit on a tank
	if(p >= 0 && !(sh[shotindex].flags & MISSILE_NOTANKCLIP))
		{
//		contactx = ta[p].x;		// TODO
//		contacty = ta[p].y;		// ?? I think this makes missile jump to tank???
		contactx = x1;
		contacty = y1;	//	try this

		// Activate a damage function hook in case this missile does any special
		// damage to the tank that isn't standard conventional damage
		// We've hit some sprite. Run any "ground impact" functions
		//	NOTE:	CheckForShots() may have called the explode func
		if(projectiles[sh[shotindex].projectile].hitfunc != NULL)
			{
			//if(!multiplayer || multiplayermode == MP_SERVER)
			projectiles[sh[shotindex].projectile].hitfunc(shotindex, p);
			}
		return;
		}

	// Hitting a "shootable" sprite will also cause this shot to explode
	// (per every 3 pixels to speed things up)
	//if(!(gametics % 3))  /// degauss debug this causes misses on the client
		{
		if(CheckForSprites(shotindex, x1, y1) != NULL)
			{
			contactx = x1;
			contacty = y1;
			// We've hit some sprite. Run any "ground impact" functions
			if(projectiles[sh[shotindex].projectile].groundfunc != NULL)
				{
				// The server will take care of this for us
				//if (multiplayer && multiplayermode == MP_CLIENT)
				//	return;
				projectiles[sh[shotindex].projectile].groundfunc(shotindex);
				}
			return;
			}
		}

	if(getpixel(ground.bmp, x1, y1) != TRANSCOLOR &&	// just hit dirt
		x1 < SCREENWIDTH &&								// and still on screen
		x1 > 0 &&
		y1 < SCREENHEIGHT &&
		y1 > 0)
		{ 
		// We havent hit the floor yet, still cruising
		// but the next pixel is dirt!!
		if(sh[shotindex].flags & MISSILE_CRUISELAND)	// Roller or Firestarter
			{
			int	leftheight = sh[shotindex].CheckSteepness(x1-1, y1);
			int	rightheight = sh[shotindex].CheckSteepness(x1+1, y1);

			if(!sh[shotindex].onfloor)
				{
				// Determine which direction we need to go
				// as we are in flight and just hit something
				if(getpixel(ground.bmp, x1, y1+2) == TRANSCOLOR)
					{
					//	there is air under us so just fall
					sh[shotindex].xvel = (sh[shotindex].xvel>0)?0.1: -0.1;	// vertical drop
					sh[shotindex].yvel = +2.35;		// down
					y1++;	// shift out of the dirt
					sh[shotindex].bouncecount = 0;	
					{ sh[shotindex].x = x1; sh[shotindex].y = y1; ignoreshot = true; return;}
					}
				if(getpixel(ground.bmp, x1-1, y1+2) == TRANSCOLOR)
					{
					//	there is air under us so just fall to the left
					sh[shotindex].xvel = -1.1;	// left
					sh[shotindex].yvel =  1.35;	// down
					x1--; y1++;	// shift out of the dirt
					{ sh[shotindex].x = x1; sh[shotindex].y = y1; ignoreshot = true; return;}
					}
				if(getpixel(ground.bmp, x1+1, y1+2) == TRANSCOLOR)
					{
					//	there is air under us so just fall to the right
					sh[shotindex].xvel =  1.1;		// right
					sh[shotindex].yvel =  1.35;		// down
					x1++; y1++;	// shift out of the dirt
					{ sh[shotindex].x = x1; sh[shotindex].y = y1; ignoreshot = true; return;}
					}

				//	We are in dirt at this point ie pixel(x1,y1) is dirt
				//	else we wouldn't be at this point.
				//	The DOLINE routine MAY continue to pass points to this routine
				//	it does not break until the line segment is complete.
				//	thus we may go deeper into the dirt as it depends on the distance and velocity
				//	as to where the line segment will start.
				//	So we set ignoreshot to true and  skip the rest of the line segment.
				//	really need a last point to backup too.

				// Check if we are on flat land, so continue in flight direction	
				if(leftheight == rightheight )
					{
					if(sh[shotindex].xvel < 0)
						{
						sh[shotindex].xvel = -1.35;		// left
						}
					else if(sh[shotindex].xvel > 0)
						{
						sh[shotindex].xvel = +1.35; // right
						}
					else	//	xvel is zero so we must have stopped so go boom
						{
						contactx = x1;
						contacty = y1;
						return;
						}
					}
				else if(leftheight > rightheight)		// head right down slope
					{
					sh[shotindex].xvel = 1.35;
					}
				else
					{
					sh[shotindex].xvel = -1.35;
					}
				sh[shotindex].yvel =  0.0;	// on dirt so no vertical movement.
				sh[shotindex].onfloor = true;	//	and we let the floor section handle it from now on!!!
				{ sh[shotindex].x = x1; sh[shotindex].y = y1; ignoreshot = true; return;}
				}
			}
		else
			{
			// Stop checking for non-cruseland missiles
			contactx = x1;
			contacty = y1;
			}
		}	//	end of just hit dirt

	// Crusing land missiles (rollers) will continue to scoot across land until they
	// either hit a tank or the slope becomes too much for them to climb up (4 pixels)
	if((sh[shotindex].flags & MISSILE_CRUISELAND) && 
		sh[shotindex].onfloor)
		{
		int	leftheight = sh[shotindex].CheckSteepness(x1-1, y1);
		int	rightheight = sh[shotindex].CheckSteepness(x1+1, y1);
		float	diff;

		// How many pixels we will go up before exploding
		int		bumptol = 4;

		// More tolerable for firestarters
		if(sh[shotindex].projectile == W_FIRESTARTER)
			bumptol = 8;

		if(sh[shotindex].y >= SCREENHEIGHT)
			sh[shotindex].y = SCREENHEIGHT-2;
		/*
		need to check if we are still on ground and haven't dropped over the edge
		or hit a vertical wall and falling
		*/
		if(getpixel(ground.bmp, x1, y1+2) == TRANSCOLOR)
			{
			//	there is air under us so just fall
			sh[shotindex].xvel = (sh[shotindex].xvel>0)?0.01: -0.01;			// stop horizontal movement
			sh[shotindex].yvel = +1.35;		// down
			x1; y1++;	// shift out of the dirt by moving down a pixel
			//sh[shotindex].bouncecount = 0;	
			sh[shotindex].onfloor = false;
			{ sh[shotindex].x = x1; sh[shotindex].y = y1; ignoreshot = true; return;}
			}

		if(sh[shotindex].xvel > 0)	// right
			{
			diff = rightheight; // slope right
			}
		else
			{
			diff = leftheight; // slope left
			}
		if( diff > 0 )	++sh[shotindex].bouncecount;	//	count small bumps and die if too many ie running up hill!
		sh[shotindex].yvel = 0;		// on ground so this routine handles y velocity

		if( diff > bumptol ) // we hit a big bump
			{
			sh[shotindex].xvel = (sh[shotindex].xvel>0)?1.5: -1.5;		// make xvel near zero but retain sign
			//y1 -= diff;	//	move to top of bump
			contactx = x1;
			contacty = y1;
			return;
			}

		//  This code replaces original OnGround() which caused rollers to drop tooo fast
		//  also if under dirt caused roller to jump above dirt and continue on: no bang.
		if( diff > 0 )	// we hit a small bump
			{
			sh[shotindex].xvel = (sh[shotindex].xvel>0)?1.5: -1.5;		// make xvel near zero but retain sign
			y1 -= diff;	//	move to top of bump
			if(sh[shotindex].bouncecount > 23)
				{
				contactx = x1;
				contacty = y1;
				}
			{ sh[shotindex].x = x1; sh[shotindex].y = y1; ignoreshot = true; return;}
			}
		}

	// Try and detect water and have the missile stop y velocity
	if((sh[shotindex].flags & MISSILE_FLOAT) && 
		sh[shotindex].yvel < 0 &&
		sh[shotindex].underwater)
		{
		if(getpixel(pwater->dbuffer, x1, y1-1) == TRANSCOLOR)
			{ 
			sh[shotindex].yvel = 0;
			}
		}
}


int		shotdistance;

void callbackcount(BITMAP *bmp, float x1, float y1, int color)
{
	shotdistance++;
}



int FindLineDistance(float x1, float y1, float x2, float y2)
{
	shotdistance=0;
	
	do_float_line(ground.bmp, x1, y1, x2, y2, TRANSCOLOR, callbackcount);

	// Return the distance
	return shotdistance;
}

//
// CheckShot - Checks our single shot to see if we've hit
// anybody. Returns true if contact with a tank has been made
//

int	SHOT::CheckShot(float shotx, float shoty)
{
	int		i;
	float	tankx,tanky,tankw,tankh;

	for (i=0; i<MAX_TANKS; i++)
	{
		// We're either not playing or dead
		if(ta[i].health <= 0)
			continue;

		tankx = ta[i].x-(ta[i].sizex/2);
		tanky = ta[i].y-(ta[i].sizey/2);
		tankw = (ta[i].sizex);
		tankh = (ta[i].sizey);

		if(shotx > tankx-tankw &&
			shotx < tankx+tankw &&
			shoty > tanky-tankh &&
			shoty < tanky+tankh)
		{
			return i;
		}
	}

	return -1;
}


//
// Thinker() - Used only for the physics of the shot. Does not draw anything
//

void SHOT::Thinker(void)
{
	// Ghost? Go no further
	if(flags & MISSILE_GHOST)
		{
		if(sprite)
			sprite->flags |= MF_NOTVISABLE;

		// Run the function hook if this missile has any
		if(projectiles[projectile].func)
			projectiles[projectile].func(indexnum);

		shottics++;
		return;
		}


	switch(state)
		{
		case	SSTATE_INAIR:  // the one and only state
				{
				if(!(flags & MISSILE_NOGRAVITY))
					{
					// Apply slightly different gravity
					if((flags & MISSILE_FLOAT) && underwater)
						{
						if(getpixel(pwater->dbuffer, x, y-1) != TRANSCOLOR)
							{ yvel -= 1.35; }
						}
					else
						yvel += 0.35;
					}

				// Make sure that floating missiles maintain a decent 
				// XVel so they dont take forever or get stuck hitting something
				if(flags & MISSILE_FLOAT)
					{
					if(xvel >= 0 && xvel < 1)
						xvel = 1;
					else if(xvel <= 0 && xvel > -1)
						xvel = -1;
					}

				// Reset contact values (we havent hit anything)
				contactx = contacty = -10;


				// loop over all of the magnetic generators
				{
				sprite_t*	s = sprites;

				while (s)	// scan all maggen's for deflection amount
					{
					// World object, spawn on the client side
					if(s->wobj >= 0)
						{
						if(s->wobj == WO_MAGGEN )
							{
							if(s->health > 0)deflect(s->x, s->y, s->misc1, s->misc2);
							}
						}
						s = s->next;
					}	//	end while
				}
				// protect the gold with a low level mag shield
				if(GOLD != 0)	deflect(goldX, goldY, 50, 3);


				// Set the shotindex
				shotindex = indexnum;

				// Run the function hook if this missile has any
				if(projectiles[projectile].func)
					projectiles[projectile].func(indexnum);

				// Are we underwater? If so, the missile runs at half the speed
				if(Underwater((int)x,(int)y))
					{
#ifdef	_DEBUG
					//  Degauss debug
					///if(shotindex == 0)Printf("Underwater x=%.2f  y=%.2f  tic=%d\n", x, y, shottics);
#endif
					// Calculate sprite angle
					if(projectiles[projectile].rotates == true)
						sprite->angle = FCalculateAngle(x, y, (x+xvel), (y+yvel));

					if(!underwater)
						G_SpawnSprite((int)x, (OnWater((int)x,(int)y)-10), S_SPLASH1, (MF_NORMAL | MF_CENTERCOORD));

					underwater = true;


					// Spawn ambient bubbles under-water (very nice effect)
					if(!(gametics % 2) && !(flags & MISSILE_NOBUBBLES))
						{
						int		bubble=rand()%3;

						if (bubble == 0)
							G_SpawnSprite((int)x, (int)y, S_BUBBLE_SMALL, (MF_NORMAL | MF_CENTERCOORD));
						else if (bubble == 1)
							G_SpawnSprite((int)x, (int)y, S_BUBBLE_MEDIUM, (MF_NORMAL | MF_CENTERCOORD));
						else
							G_SpawnSprite((int)x, (int)y, S_BUBBLE_LARGE, (MF_NORMAL | MF_CENTERCOORD));
						}

					// Trace the shot under water
					do_float_line(ground.bmp, x, y, (x+(xvel/2)), (y+(yvel/2)), TRANSCOLOR, callbackshot);
						if(contactx == -10 && contacty == -10)
							{
							x+= (xvel/2);
							y+= (yvel/2);
							sprite->x = x;
							sprite->y = y;
							}
					
					// Debug for tracers
					if(projectile == W_TRACERS || projectile == W_FUNKYBOMBCHILD || projectile == W_FUNKYNUKECHILD || showtrace)
						line(buffer, x, y, x+(xvel/2), y+(yvel/2), makecol(255,0,0));
					}
				else
					{
					// Calculate sprite angle
					if(projectiles[projectile].rotates == true)
						sprite->angle = FCalculateAngle(x, y, (x+xvel), (y+yvel));

					if(underwater)
    				     G_SpawnSprite((int)x, (OnWater((int)x,(int)y)-10), S_SPLASH1, (MF_NORMAL | MF_CENTERCOORD));

					underwater = false;


					if(!(gametics % 2) && !(flags & MISSILE_NOSMOKE))
						{
						sprite_t	*s;
						int		bubble=rand()%2;
						int		flags = (MF_NORMAL | MF_USEANGLE | MF_TRANSLUC);
	
						if(bubble == 0)
							s = G_SpawnSprite((int)x, (int)y, S_SMOKE_GRAY_XLARGE, flags);
						else
							s = G_SpawnSprite((int)x, (int)y, S_SMOKE_GRAY_LARGE, flags);
					
						s->scale = 0.01;
						s->opacity = 100;
						}

					// Trace the shot
					ignoreshot = false;	//	indicate a new shot segment  --  used by callbackshot
					do_float_line(ground.bmp, x, y, (x+xvel), (y+yvel), TRANSCOLOR, callbackshot);
					if(contactx == -10 && contacty == -10)	// Original contact values (we haven't hit anything)
						{
						if(!ignoreshot)	//	if ignoreshot is true then x & y have been set by callbackshot
							{
							x+= xvel;
							y+= yvel;
							}
						sprite->x = x;
						sprite->y = y;
						}

					// Show tracers
					if(projectile == W_TRACERS || projectile == W_FUNKYBOMBCHILD || projectile == W_FUNKYNUKECHILD || showtrace)
						{
#ifdef	_DEBUG
						line(buffer, (x), (y), (x+(xvel)), (y+(yvel)), (sh[shotindex].onfloor)?makecol(255,64,0):( (gametics % 2)?makecol(64,255,0):makecol(250,250,0) ) );
#else
						line(buffer, (x), (y), (x+(xvel)), (y+(yvel)), makecol(255,0,0) );
#endif
						}
					}

				// Dont let the missile travel off screen
				if(x >= SCREENWIDTH ||  x <= 0)
					{
					// Weapons that have no gravity do not have special 
					// wall attributes since they can sometimes go on forever.
					// (AKA. Machinegun, Laser pulse)
					if((currentlevel.walltype == WALLS_NORMAL ||
						projectiles[projectile].flags & MISSILE_NOGRAVITY)&& 
						!(projectiles[projectile].flags & MISSILE_ALWAYSBOUNCE))
						{
						// Kill the sprite (no explosion)
						exist = 0;
						if(sprite)
							sprite->Kill();
	
						// Have the dirt fall even if the missile left the screen boundries since we
						// dont know if it sliced through land or not.
						if(flags & MISSILE_NODIRTCLIP)
							ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);

						return;
						}

					if(currentlevel.walltype == WALLS_CEMENT && 
						!(projectiles[projectile].flags & MISSILE_ALWAYSBOUNCE))
						{
						if(x >= SCREENWIDTH)
							x = contactx = SCREENWIDTH-1;

						if(x <= 0)
							x = contactx = 1;

						// We've hit the ground itself. Run any "ground impact" functions
						if(projectiles[projectile].groundfunc != NULL)
							{
							// The server will take care of this for us
							//if(!multiplayer || multiplayermode == MP_SERVER)
							projectiles[projectile].groundfunc(shotindex);
							}
						// Make it go boom
						explode();
						return;
						}
					
					// Rubber walls or a "always" bounce
					if(currentlevel.walltype == WALLS_RUBBER ||
						(projectiles[projectile].flags & MISSILE_ALWAYSBOUNCE))
						{
						// If this is a missile that is a land cruiser, make it
						// explode instead of bouncing
						if(flags & MISSILE_CRUISELAND)
							{
							contactx = x;
							contacty = y;
							explode();
							return;
							}
						if(x >= SCREENWIDTH)
							x = SCREENWIDTH-1;

						if(x <= 0)
							x = 1;

						// Bounce the x velocity
						xvel = -xvel;
						}
					
					if(currentlevel.walltype == WALLS_WRAP &&
						!(projectiles[projectile].flags & MISSILE_ALWAYSBOUNCE))
						{
						// If this is a missile that is a land cruiser, make it
						// explode instead of bouncing
						/*if(flags & MISSILE_CRUISELAND)   // ??? Why????
						
							// == cause it just wraps and never stops.
							// == setting life so it dies after a time (500)
							// == set at approx  Tank.cpp(1043).
						{
							contactx = x;
							contacty = y;
							explode();
							return;
						} */

						if(x >= SCREENWIDTH)
							x = 1;

						if(x <= 0)
							x = SCREENWIDTH-1;
						}
					}


				// Little different for no gravity projectiles. If their y coord goes below
				// 0 they are considered gone
				if((flags & MISSILE_NOGRAVITY) && y <= 0)
					{
					// Kill the sprite (no explosion)
					exist = 0;
					if(sprite)
						sprite->Kill();

					// Have the dirt fall even if the missile left the screen boundries since we
					// dont know if it sliced through land or not.
					if(flags & MISSILE_NODIRTCLIP)
						ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);

					return;
					}

				// Missiles can NEVER go below the screen's height
				if(y > SCREENHEIGHT)
					{
					contactx = x;
					contacty = SCREENHEIGHT;
					}

				// We're on the screen at least
				// Make sure the "contact" points are within screen range
				if(contactx >= 0 && contacty >= 0)
					{ 					
					// We've hit the ground itself. Run any "ground impact" functions
					if(projectiles[projectile].groundfunc != NULL)
						{
						//if(!multiplayer || multiplayermode == MP_SERVER)
						projectiles[projectile].groundfunc(shotindex);
						}
Printf("Contact x:%.3f  y:%.3f  cx:%.3f  cy:%.3f\n", x, y, contactx, contacty); // zzz Degauss debug
					x=contactx;
					y=contacty;
					explode(); 
					}

				// shorten life so roller don't roll forever.
				if(sh[shotindex].flags & MISSILE_CRUISELAND)	// Roller or Firestarter
					{
					if(shottics > 700)
						{
						contactx = x;
						contacty = y;
						explode();
						return;
						}
					}
				break;
				}

		default:
			break;
	}

	// Increase life
	shottics++;


};

void SHOT::deflect(int magX, int magY, int decay, int power)
{
/////////////////////////////////////////  mag shield test ////////////
//		 generator location  magX;		// x location of mag gen.
//												 magY;		// y location of mag gen.

float dist, deflection, dx, dy, xdef, ydef;
float	mgpwr = 3;
float	mgdist = 50;

//	the treasure protection is not controlled by the config file.
if(GOLD && (goldX == magX) && (goldY == magY) )
	{
	mgpwr = 3;
	mgdist = 50;
	}
else
	{
	mgpwr  = power;
	mgdist = decay;	//	distance decays for each round
	if(mgdist < 10)mgdist = 10;	// zero effect
	}

if( (y < magY+10) || (yvel > -2) ) // if its under us or moving up fast ignore it
	{
	dx = x-magX; // pos on the right
	dy = y-magY; // pos below

	dist = sqrt( dx*dx + dy*dy );

	if(dist < mgdist) // close enough to do work on
		{
		if(dist < 10) dist = 10; // no divide by zero please!
		deflection = mgpwr*((mgdist-dist)/mgdist);	// calculate the deflection at this distance
		xdef = deflection*dx/dist;                  // scale the deflection vector
		ydef = deflection*dy/dist;
		xvel = xvel + xdef;                         // add vectors
		yvel = yvel + ydef;
		if(abs(xvel) < 0.01) 	xvel = (x < magX)?-0.5:0.5; // so it just don't stay bobing on top
		if(shottics > 500)		// oops we must be stuck
			explode();					// Make it go boom
		}
	}
}
int SHOT::inside(float x1, float y1, float x2, float y2)
{
    if(x>=x1 && y>=y1 && x<=x2 && y<=y2) 
		return 1;
    return 0;
}


void	ApplyNapalmDamage(int tankfrom)
{
	int		i;
	int		napalmdamage;
	sprite_t*	s = sprites;

	for (i=0;i<MAX_TANKS;i++) 
		{
		napalmdamage = ta[i].napalmdamageh + ta[i].napalmdamagee + ta[i].napalmdamageb;
		if(ta[i].health > 0 && napalmdamage > 0)
			{
			ta[i].Damage(ta[i].napalmdamageh,	ta[i].napalmdamagee, ta[i].napalmdamageb,	tankfrom,	false);
			}

		ta[i].napalmdamageh = 0;
		ta[i].napalmdamagee = 0;
		ta[i].napalmdamageb = 0;
		}

		// Damage any shootable sprites as well
		while (s)
			{
			if(s->flags & MF_SHOOTABLE)
				s->CheckReserveDamage();

			s = s->next;
			}

		// Remove all damage
		DudAllParticles();
}

void SHOT::NapalmTanks(void)
{

	life--;
	damagelife--;

	if(damagelife==0)
		ApplyNapalmDamage(tankfrom);

	if(life == 0)
		{
		if(sprite)
			sprite->Kill();
		exist = 0;
		
		//KillTanks();
		}
}


void SHOT::Blowup(BITMAP *bmp, int	damage)
{
	int			i, rad;
	RGB		    rgb;

	rad = projectiles[projectile].expradius;

		for(i=0; i<damagetics; i++)
			{
			damagedone++; 
			if(damagedone > damage)
				{
				damagedone = damage;
				}
			indexrate = 255/damagedone;
			}
		colorindex = 0;

		// Add the dirty rectangle
		AddRectangle(bmp, (int)(x-rad)-1, (int)(y-rad)-1, (rad*2)+1, (rad*2)+1);

		// For RIOT Blasts only, add the 0 color in.
		if(projectile == W_RIOTBLAST || projectile == W_HEAVYRIOTBLAST)
			{
			for(i=damagedone; i>=0; i--)
				{
				if((!(i % 5)) || (!(i % 4)))
					{
					if(projectile == W_RIOTBLAST)
						colorindex = 1;
					else
						colorindex = 2;
					}
				else
					colorindex = 0;

				rgb = ((RGB *)pal)[colorindex];

				//circlefill(boombmp,boombmp->w/2,boombmp->h/2,i,colorindex); 
#ifndef			DEDICATED_SERVER
				circlefill(bmp,x,y,i, makecol(rgb.r*4, rgb.g*4, rgb.b*4)); 
#endif
				}
			}

		// All other projectiles
		else
			{
#ifndef			DEDICATED_SERVER

			for(i=damagedone; i>=1; i--)
				{
				colorindex += (int)indexrate;
	
				// Override the color if this is a dirt (solid color) weapon
				if(flags & MISSILE_DIRT)
					{
					colorindex = makecol(projectiles[projectile].pixelcolorr,
								projectiles[projectile].pixelcolorg,
								projectiles[projectile].pixelcolorb);
					circlefill(boombmp,boombmp->w/2,boombmp->h/2,i,colorindex);
					}	
				else
					{ 
					rgb = ((RGB *)pal)[colorindex];
					//circlefill(boombmp,boombmp->w/2,boombmp->h/2,i,colorindex); 
					circlefill(bmp,x,y,i,
						makecol(rgb.r*4, rgb.g*4, rgb.b*4)); 
					}
				}

#endif
		}

		
		// Add dirty rectangle & draw
		if(flags & MISSILE_DIRT)
			DrawGroundAddition(boombmp, bmp, (int)x-rad,(int)y-rad);
		
		// Check to see if we're done here
		if(damagedone == damage)
			{
			// Create the actual damage into the ground (napalm burns)
			if(!(flags & MISSILE_NAPALM1) && 
					!(flags & MISSILE_NAPALM2) &&
					!(flags & MISSILE_NAPALM)  &&
					!(flags & MISSILE_PLASMA))
				{
				// The radius should always be greater than 1, but however, add checking
				// just in case
				if(rad > 1)
					{
					BITMAP	*burn = create_bitmap(rad*2, rad*2);
					clear_to_color(burn, TRANSCOLOR);

					if(flags & MISSILE_DIRT)
						{
#ifndef			DEDICATED_SERVER
						int		dirtcolor = makecol(projectiles[projectile].pixelcolorr,
							projectiles[projectile].pixelcolorg,
							projectiles[projectile].pixelcolorb);

						circlefill(burn,burn->w/2,burn->h/2,rad,dirtcolor);
						DrawGroundAddition(burn, ground.bmp, x-(burn->w/2),y-(burn->h/2));
#endif
						}
					else
						{
#ifndef			DEDICATED_SERVER
						circlefill(burn,burn->w/2,burn->h/2,rad,makecol(1,1,1) );
						DrawBitmapGroundMask(burn, ground.bmp, x-(burn->w/2),y-(burn->h/2), 200, false);
#endif
						if(ServerClient == '*')ServerClient = 'g'; // generic
						DestroyDirt(x, y, rad-2);
						}

					// Release allocated memory
					destroy_bitmap(burn);

					// Redraw the ground
					//ground.needsupdate = true;
					}
			}

			// If all the computer players are bots, they'll attempt to fire before the ground
			// has time to fall which will leave the tank suspended in air. In order to fix this,
			// call fall() (hey! that ryhmes!) so tanks will begin falling before the next shot can
			// be fired
			for(i=0; i<MAX_TANKS; i++)
				{
				if(ta[i].health <= 0)
					continue;
				ta[i].fall(ground.bmp, pwater->dbuffer);	// Set the 'falling' flag before the next
															// shot can be fired
				}
	
			DeployDamage(damage);

			startcolor=usecolor=done=0;
			state = SSTATE_FINISHING;
		}
}


void SHOT::DeployDamage(int ddamage)
{
	// For checking shootable sprites
	sprite_t*	s = sprites;
	BITMAP		*sbmp;
	int			sx,sy;
	
	// FOr checking tanks
	int		i;
	float	damage,damage_h=0,damage_e=0,damage_b=0;;
	float	radius = (projectiles[projectile].expradius);
	float	damagediff;
	bool	scattertext=false;

	if(projectile == W_SHOTGUNSHOT || projectile == W_MACHINEGUNSHOT)
		scattertext = true;

	// Cool, now figure out if any tanks come near it
	for(i=0; i<MAX_TANKS; i++)
		{ 
		if(ta[i].health <= 0)
			continue;

		damage = FindLineDistance(x, y, ta[i].x, ta[i].y);

		if(damage > 0)
			damagediff = (radius-(float)damage)/radius;

		// Test for a **DIRECT HIT** (11 distance is typical direct hit for shields) <-- VERY ugly hack
		if(damage <= 5 || (damage == 11 && ta[i].energyleft > 0))
			{
			// Calculate damage for each damage class
			damage_h = projectiles[projectile].damageh;
			damage_e = projectiles[projectile].damagee;
			damage_b = projectiles[projectile].damageb;
			}
		else
			{
			// Blast is too far away to his this tank,
			// continue with the next one
			if(damage > radius) 
				{ continue; }

			damage_h = projectiles[projectile].damageh*damagediff;
			damage_e = projectiles[projectile].damagee*damagediff;
			damage_b = projectiles[projectile].damageb*damagediff;
			}

		// Dish out some tank damage (dont kill tanks however)
		ta[i].Damage(damage_h, damage_e, damage_b, tankfrom, false, true, scattertext);
		}

	// Kill the sprite we've been tracking
	if(sprite)
		sprite->Kill();
	
	//
	// Now clip against any sprites that are shootable
	//
	while(s)
		{
		// Not shootable? Forget it!
		if(!(s->flags & MF_SHOOTABLE))
			{
			s = s->next;
			continue;
			}

		// Set our clipping bitmap
		sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);

		// Setup clipping coords
		sx = s->x;
		sy = s->y;	

		// New coordinate if this is isnt' centered
		if(!(s->flags & MF_CENTERCOORD))
			{
			sx = s->x + (sbmp->w/2);
			sy = s->y + (sbmp->h/2);
			}

		// Check pixel perfect collision. Direct hit?
		if(CheckPointInSprite(s, x, y))
			{
			// Calculate damage for each damage class
			damage_h = projectiles[projectile].damageh;
			damage_e = projectiles[projectile].damagee;
			damage_b = projectiles[projectile].damageb;

			s->Damage(damage_h, damage_e, damage_b);
			
			// Stop here. Continue
			s = s->next;
			continue;
			}

	
		damage = FindLineDistance(x, y, sx, sy);

		if(damage > 0)
			damagediff = (radius-(float)damage)/radius;

		// Blast is too far away to his this sprite
		// continue with the next one
		if(damage < radius) 
			{ 
			damage_h = projectiles[projectile].damageh*damagediff;
			damage_e = projectiles[projectile].damagee*damagediff;
			damage_b = projectiles[projectile].damageb*damagediff;

			s->Damage(damage_h, damage_e, damage_b);
			}
		s = s->next;
		}
}
		
void SHOT::ExplodeEffect(BITMAP *buf, int damage, int tics, int speed)
{
	int		i;
	int		rad = projectiles[projectile].expradius;
	RGB	    rgb;

	indexrate=255/damage;

	if(done < tics)
	{
		// Add a dirty rectangle
		AddRectangle(buf, (int)(x-rad), (int)(y-rad), (rad*2)+1, (rad*2)+1);

		for (i=0; i<speed; i++)
		{
			startcolor+=(int)indexrate;
			if(startcolor > 255) { startcolor = 0; }
		}

		usecolor = startcolor;

#ifndef			DEDICATED_SERVER		
		for(i=damage; i>=0; i--)
		{

			// Get the color
			rgb = ((RGB *)pal)[usecolor];
			
			circlefill(buf, x, y, i, makecol(rgb.r*4, rgb.g*4, rgb.b*4));
			usecolor+=(int)indexrate;

			if(usecolor > 255) { usecolor = 1; }
		}
#endif

		done++;
	}
	else
	{

		// Redraw the ground
		ground.needsupdate = true;

		exist = 0;
		
		// 'boombmp' is used by only dirt spawning projectiles
		if(projectiles[projectile].flags & MISSILE_DIRT)
			destroy_bitmap(boombmp);

		// Make zee ground fall
		ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
	}

}




//
// StartSound - Plays a sound that orginates from a projectile
//

void SHOT::StartSound(int sound, int loop, int stopcurrent)
{
	// No negative numbers here
	if(sound < 0)
		return;

	// Disabled sound
	if(nosound > 0)
		return;

	if(stopcurrent > 0)
	{ StopSound(); }

	// Allocate voice channel
	sndvoice = allocate_voice((SAMPLE *)sounddata[sound].dat);

	// No voice channels available
	if(sndvoice == -1)
		return;
	
	// Always high priority
	voice_set_priority(sndvoice, 255);

	// Set volume
	voice_set_volume(sndvoice, Cvar_VariableValue("soundvolume"));

	// Looping?
	if(loop > 0)
		voice_set_playmode(sndvoice, PLAYMODE_LOOP);

	// Start the sound
	voice_start(sndvoice);

	if(loop <= 0)
		release_voice(sndvoice);
}

void SHOT::StopSound(void)
{
	// Already deallocated? (or never allocated?)
	if(!voice_check(sndvoice))
		return;

	// Stop sound & release memory
	deallocate_voice(sndvoice);
}

//
// CheckSteepness - How many pixels does it take to get to the surface
//
int SHOT::CheckSteepness(int destx,int desty)
{
    int a,b;
    int s=0;

    a=(int)destx;
	b=desty;

	while ((getpixel(ground.bmp,a,b--)) != TRANSCOLOR && b >= 0) 
	{ 
		s++; 
	}

    return s;
}


void Idle(int time)
{
	int	donetime=0;

	while(donetime!=time)
	{
		if (update_count > 0)
		{
			update_count = 0;

			ParticleThinkers();

			LoopDrawer(buffer);
			donetime++;
	
			ticker();
			gametics++;
			frame_count++;

			Screen_Blit();
		}
	}
}

//
// KillTanks - Goes through the loop and kills tanks that are flagged to die
//

void KillTanks(bool		advanceturn)// default:advanceturn=true
{

	int			i;
	int			tankdeaths=0;

	// This is all done serverside via multiplayer
	if(multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Shot in air? Player already doing a death sequence?
	if(shotinair() || IsTankDeathSeq() > 0)
		return;

	for (i=0; i<MAX_TANKS; i++)
	{
		if(ta[i].needstodie <= 0)
			continue;

		// Kill any shield if for some reason we still have one
		if(ta[i].shield || ta[i].energyleft > 0)
			ta[i].KillShield();

		// Be sure we haven't been reclassed out of existance
		if(ta[i].exist > 0)
			AddNextTurnQue(ta[i].indexnum);

		// Setup a death animation
		StartRandomDeathSequence(i);
		
		// Destroy any sentry pods working for this tank
		KillTankSentryPods(i);

		tankdeaths++;
	}

	// Multiplayer clients never reach this, server does this for them
	if(!multiplayer || multiplayermode == MP_SERVER)
		Inter_CheckGame(advanceturn);
}


void	Shotgun_Fire(int shot)
{
	int		newshot;
	int		tfrom = sh[shot].tankfrom;
	int		flags = (MF_NORMAL | MF_CENTERCOORD);
	int		length = characters[ta[tfrom].character].cannonlength+
					(characters[ta[tfrom].character].cannonlength/2);
	int		missilepower = ConvertFromGamePower(3000);
	int		fireangle = sh[shot].origdeg; //ta[tfrom].deg;

	int		numshells = 14;
	int		i;

	sprite_t*		shell;
	int				shellpower;
	int				shellangle;

	for (i=0; i<numshells; i++)
		{
		newshot = findshot();
		
		// Something bad happened
		if(newshot < 0)
			return;

		// Calculate a random angle
		fireangle -= 2;
		fireangle += rANDom()%4; 	CheckRandomFunc("Shotgun_Fire shot");


		// Keep track of the last weapon shot
		lasttankshot = W_SHOTGUNSHOT;

		sh[newshot].index = newshot;
		sh[newshot].tankfrom = tfrom;
		sh[newshot].Init(tfrom, W_MACHINEGUNSHOT,
					ta[tfrom].barrelshotx,
					ta[tfrom].barrelshoty,
					(-missilepower*COS(fireangle)),
					(missilepower*-SIN(fireangle)),
					fireangle);

		if(projectiles[W_SHOTGUNSHOT].firesound >= 0)
		{ sh[newshot].StartSound(projectiles[W_MACHINEGUNSHOT].firesound); }

		flags |= MF_USEANGLE;

		// Bind a sprite to our shot
		sh[newshot].sprite = G_SpawnSprite((int)sh[shot].origx,(int)sh[shot].origy, projectiles[W_SHOTGUNSHOT].state, flags);

		ta[tfrom].SetFlareTics(2);


		
		
		// Spawn a empty shell, the direction the tank is facing dictates the
		// direction of the shell
		shellpower = ConvertFromGamePower(100+rand()%50);
		shell = G_SpawnSprite(sh[shot].origx, sh[shot].origy, S_EMPTYSHELL, (MF_NORMAL|MF_GROUNDCLIP|MF_BOUNCE|MF_USEANGLE));
		
		// Tank is facing the left
		if(fireangle <= 64)
		{
			shellangle = 90+rand()%10;
			shell->xvel = (-shellpower*COS(shellangle));
		}
		else	// Tank is facing the right
		{
			shellangle = 30+rand()%10;
			shell->xvel = (-shellpower*COS(shellangle));
		}

		// Add some Y velocity
		shell->yvel = (shellpower*-SIN(shellangle));

	}
}


void	Shotgun_Thinker(int shot)
{
	/*int		newshot;
	int		tfrom = sh[shot].tankfrom;
	int		flags = (MF_NORMAL | MF_CENTERCOORD);
	int		length = characters[ta[tfrom].character].cannonlength+
					(characters[ta[tfrom].character].cannonlength/2);
	int		missilepower = ConvertFromGamePower(3000);
	int		fireangle = ta[tfrom].deg;*/

	sh[shot].xvel = 0;
	sh[shot].yvel = 0;

	if(sh[shot].shottics == 1)
	{ 
		// Play a sound here
		sh[shot].StartSound(SHOTGUN_READY);
	}

	// Fire one!
	if(sh[shot].shottics == (TICRATE/2))
	{
		sh[shot].StartSound(SHOTGUN_FIRE);
		Shotgun_Fire(shot);
	}

	// Fire two!
	if(sh[shot].shottics == ((TICRATE)+(TICRATE/2)))
	{
		sh[shot].StartSound(SHOTGUN_FIRE);
		Shotgun_Fire(shot);
	}

	// Fire three!
	if(sh[shot].shottics == ((TICRATE*2)+(TICRATE/2)))
	{
		sh[shot].StartSound(SHOTGUN_FIRE);
		Shotgun_Fire(shot);

		if(sh[shot].sprite)
			sh[shot].sprite->Kill();

		sh[shot].exist = 0;
	}

	// We're done, clean up and go home
	if(sh[shot].shottics > ((TICRATE*2)+(TICRATE/2)))
	{
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();
		sh[shot].exist = 0;
		return;
	}
}



//
// Machine Gun
//

void	MachineGun_TankHit(int shot, int tankhit)
{
	int		sound = rand()%5;
	int		StartSound;

	switch	(sound)
	{
		case	0:
			StartSound = METALHIT1;
			break;
		case	1:
			StartSound = METALHIT2;
			break;
		case	2:
			StartSound = METALHIT3;
			break;
		case	3:
			StartSound = SHOT_RIC1;
			break;
		case	4:
			StartSound = SHOT_RIC2;
			break;
		default:
			StartSound = METALHIT1;
			break;
	}

	// Make the shield hit sound
	if(ta[tankhit].energyleft > 0)
		StartSound  = SHIELDHIT;

	sh[shot].StartSound(StartSound);
	
}

void	MachineGun_Thinker(int shot)
{
	int		newshot;
	int		tfrom = sh[shot].tankfrom;
	int		flags = (MF_NORMAL | MF_CENTERCOORD);
	int		length = characters[ta[tfrom].character].cannonlength+
					(characters[ta[tfrom].character].cannonlength/2);
	int		missilepower = ConvertFromGamePower(4000);
	int		fireangle = sh[shot].origdeg;
	sprite_t*		shell;
	int				shellpower;
	int				shellangle;

	sh[shot].xvel = 0;
	sh[shot].yvel = 0;

	// Shot is considered done after 3.5 seconds
	if(sh[shot].shottics > 3.5*TICRATE)
	{
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();

		sh[shot].exist = 0;

		//KillTanks();
		return;
	}

	if(!(sh[shot].shottics % 5))
		{
		newshot = findshot();
		
		// Something bad happened
		if(newshot < 0)
			return;

		// Calculate a random angle
		fireangle -= 3;
		fireangle += rANDom()%5; 	CheckRandomFunc("MachineGun_Thinker shot random angle");


		// Keep track of the last weapon shot
		lasttankshot = W_MACHINEGUNSHOT;

		sh[newshot].index = newshot;
		sh[newshot].Init(newshot, W_MACHINEGUNSHOT,
					ta[tfrom].barrelshotx,
					ta[tfrom].barrelshoty,
					(-missilepower*COS(fireangle)),
					(missilepower*-SIN(fireangle)),
					fireangle);

		sh[newshot].tankfrom = tfrom;

		
		if(projectiles[W_MACHINEGUNSHOT].firesound >= 0)
		{ sh[newshot].StartSound(projectiles[W_MACHINEGUNSHOT].firesound); }

		flags |= MF_USEANGLE;

		// Bind a sprite to our shot
		sh[newshot].sprite = G_SpawnSprite((int)sh[shot].origx,(int)sh[shot].origy, projectiles[W_MACHINEGUNSHOT].state, flags);

		// Give it some flare!
		ta[tfrom].SetFlareTics(2);

		// Spawn a empty shell, the direction the tank is facing dictates the
		// direction of the shell
		shellpower = ConvertFromGamePower(100+rand()%50);
		shell = G_SpawnSprite(sh[shot].origx, sh[shot].origy, S_EMPTYSHELL, (MF_NORMAL|MF_GROUNDCLIP|MF_BOUNCE|MF_USEANGLE));
		
		// Tank is facing the left
		if(fireangle <= 64)
			{
			shellangle = 90+rand()%10;
			shell->xvel = (-shellpower*COS(shellangle));
			}
		else	// Tank is facing the right
			{
			shellangle = 30+rand()%10;
			shell->xvel = (-shellpower*COS(shellangle));
			}

		// Add some Y velocity
		shell->yvel = (shellpower*-SIN(shellangle));
		}
}


void	FireFly_ChildThinker(int shot)
{
	sprite_t	*effect;
	int		sx=0;
	int		sy=0;

	if(gametics % 2)
		return;

	sx = sh[shot].x - 5;
	sy = sh[shot].y - 5;

	sx += rANDom()%10; ///CheckRandomFunc
	sy += rANDom()%10; 	CheckRandomFunc("shot FireFly_ChildThinker");
	

	effect = G_SpawnSprite(sx,sy,
		S_FIREFLY_EFFECT, 
		(MF_NORMAL|MF_CENTERCOORD|MF_COOLEFFECT1));

	effect->opacity = 255;
}


void	FireFly_ChildHit(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_USEANGLE | MF_CENTERCOORD);
	
	SetTankTimer(2*TICRATE);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	SetTankTimer(1*TICRATE);

	s = G_SpawnSprite(contactx, contacty, S_FIREFLY_DIE, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;
}

void	FireFly_ChildHitTank(int shot, int tankhit)
{
	FireFly_ChildHit(shot);
}


void	FireFly_Thinker(int shot)
{
	int		newshot;
	int		tfrom = sh[shot].tankfrom;
	int		flags = (MF_NORMAL | MF_CENTERCOORD);
	int		length = characters[ta[tfrom].character].cannonlength+
					(characters[ta[tfrom].character].cannonlength/2);
	int		missilepower = ConvertFromGamePower(800);
	int		fireangle = sh[shot].origdeg;

	sh[shot].xvel = 0;
	sh[shot].yvel = 0;

	// Shot is considered done after 3.5 seconds
	if(sh[shot].shottics >= 1.5*TICRATE)
		{
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();

		sh[shot].exist = 0;

		//KillTanks();
		return;
		}

	if(!(sh[shot].shottics % 5))
		{
		newshot = findshot();
		
		// Something bad happened
		if(newshot < 0)
			return;

		// Calculate a random angle
		fireangle -= 5;
		fireangle += rANDom()%10; 	CheckRandomFunc("random angle FireFly_Thinker");


		// Keep track of the last weapon shot
		lasttankshot = W_FIREFLYCHILD;

		sh[newshot].index = newshot;
		sh[newshot].Init(newshot, W_FIREFLYCHILD,
					ta[tfrom].barrelshotx,
					ta[tfrom].barrelshoty,
					(-missilepower*COS(fireangle)),
					(missilepower*-SIN(fireangle)),
					fireangle);

		sh[newshot].tankfrom = tfrom;

		
		if(projectiles[W_FIREFLYCHILD].firesound >= 0)
		{ sh[newshot].StartSound(projectiles[W_FIREFLYCHILD].firesound); }

		flags |= MF_USEANGLE;

		// Bind a sprite to our shot
		sh[newshot].sprite = G_SpawnSprite((int)sh[shot].origx,(int)sh[shot].origy, projectiles[W_FIREFLYCHILD].state, flags);

		// Give it some flare!
		ta[tfrom].SetFlareTics(2);
		}

}



static  int		numblaserblast;
static	int		laserblast_count;
static	int		laserblast_angle;


void	Heatray_Proc(BITMAP *b, int x, int y, int color)
{
	sprite_t*		s;
	laserblast_count--;

	if(laserblast_count <= 0)
	{

		// Dont spawn sprites off screen, its a waste of memory
		if(OnScreen(ground.bmp, x, y))
		{
			s = G_SpawnSprite(x,y,S_HEATRAY1, (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE));
			s->angle = laserblast_angle;
		}

		if(numblaserblast == 0)
		{
			s = G_SpawnSprite(x,y,S_HEATRAY_START1, (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE));
			s->angle = laserblast_angle;
		}

		numblaserblast++;

		laserblast_count = 30; //15;
	}
}

void	HeatrayBurn_Proc(BITMAP *b, int x, int y, int color)
{
	int		fx,fy;

	if(gametics % 5)
		return;

	fx = x-5;
	fy = y-5;

	if(rand()%3 == 2 && (OnScreen(ground.bmp, x, y)))
		BurnGround(ground.bmp, fx+(rand()%10), fy+(rand()%10), 15, 10);

}

void			Heatray_Thinker(int shot)
{
	sprite_t	*arc;
	int			tank = sh[shot].tankfrom;
	int			length = characters[ta[tank].character].cannonlength+15;
	int			laserlength = SCREENWIDTH+(SCREENWIDTH/2);
	int			fireangle = sh[shot].origdeg; 
	float		x;
	float		y;

	x = sh[shot].origx-COS(fireangle)*length;
	y = sh[shot].origy-SIN(fireangle)*length;
	
	if(sh[shot].groundpixels == 0)	//	save shotlength else its recalc each frame
		{
		sh[shot].groundpixels = ShotLength(x, y, fireangle);
		}
	laserlength = sh[shot].groundpixels;	// just using groundpixels as a storage location.

	if(sh[shot].shottics <= 1)	//	the start flame
		{
		x = sh[shot].origx-COS(fireangle)*(characters[ta[tank].character].cannonlength+5);
		y = sh[shot].origy-SIN(fireangle)*(characters[ta[tank].character].cannonlength+5);
		G_SpawnSprite(x,y,S_HEATRAY_F1, (MF_NORMAL|MF_CENTERCOORD));

		sh[shot].StartSound(TANK_STARTHEATRAY);
		}

	if(sh[shot].shottics < TICRATE)
		return;

	if(sh[shot].shottics == TICRATE)
	//	detect if there is a TOWERpod in the line of fire.

	//	if so the laser stops here
		{
		// Play it again, mac.
		sh[shot].StartSound(TANK_FIREHEATRAY);
	
		arc = G_SpawnSprite(x, y, S_HEATRAY1, (MF_NORMAL|MF_NOTVISABLE));
		arc->angle = fireangle;
		arc->damageh = projectiles[sh[shot].projectile].damageh;
		arc->damagee = projectiles[sh[shot].projectile].damagee;
		arc->damageb = projectiles[sh[shot].projectile].damageb;
		arc->tankfrom = sh[shot].tankfrom;
	
		LaserLine(x, y, fireangle, laserlength, 4, arc->damageh, arc->damagee, arc->damageb, arc->tankfrom, 
			(LASER_NOKILLLAND|LASER_NOBURNLAND));

		numblaserblast = 0;
		laserblast_count = 0;
		laserblast_angle = arc->angle;
		do_line(buffer, x, y, x+(-laserlength*COS(fireangle)), y+(laserlength*-SIN(fireangle)), 0, Heatray_Proc);

		SetTankTimer(1.1*TICRATE);
		}

	// Burn the ground while we're at it
	do_line(buffer, x, y, x+(-laserlength*COS(fireangle)), y+(laserlength*-SIN(fireangle)), 0, HeatrayBurn_Proc);
	
	// Kill the shot out
	if(sh[shot].shottics >= 2*TICRATE)
		{
		if(sh[shot].sprite)	sh[shot].sprite->Kill();
		sh[shot].exist = 0;
		}
}

void	LaserBlast_Proc(BITMAP *b, int x, int y, int color)
{
	sprite_t*		s;
	laserblast_count--;

	if(laserblast_count <= 0)
	{
		if(numblaserblast == 0)
			s = G_SpawnSprite(x,y,S_IONCANNON_FIRE1, (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE));
		else
			s = G_SpawnSprite(x,y,S_IONCANNON1, (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE));
		s->angle = laserblast_angle;

		numblaserblast++;

		laserblast_count = 30; //15;
	}
}

void			LaserBlast_Thinker(int shot)
{
	sprite_t	*arc;
	int			tank = sh[shot].tankfrom;
	int			length = characters[ta[tank].character].cannonlength+15;
	int			laserlength = SCREENWIDTH+(SCREENWIDTH/2);
	int			fireangle = sh[shot].origdeg; 
	float		x;
	float		y;

	x = sh[shot].origx-COS(fireangle)*length;
	y = sh[shot].origy-SIN(fireangle)*length;
	if(sh[shot].groundpixels == 0)	//	save shotlength else its recalc each frame
		{
		sh[shot].groundpixels = ShotLength(x, y, fireangle);
		}
	laserlength = sh[shot].groundpixels;	// just using groundpixels as a storage location.

	if(sh[shot].shottics == 1)
	{
		x = sh[shot].origx-COS(fireangle)*(characters[ta[tank].character].cannonlength+5);
		y = sh[shot].origy-SIN(fireangle)*(characters[ta[tank].character].cannonlength+5);
		G_SpawnSprite(x,y,S_IONCANNON_CHARGE1, (MF_NORMAL|MF_CENTERCOORD));

		sh[shot].StartSound(TANK_LASERBLAST_REV);
	}
	if(sh[shot].shottics < TICRATE)
	{ 
		return;
	}


	// Play it again, mac.
	sh[shot].StartSound(TANK_LASERBLAST);

	arc = G_SpawnSprite(x, y, S_IONCANNON_FIRE1, (MF_NORMAL|MF_NOTVISABLE));
	//arc->angle = sh[shot].sprite->angle;
	arc->angle = fireangle; //(atan((sh[shot].yvel/2)/(sh[shot].xvel/2))*(180/PI)*ACHANGE)-64+(((sh[shot].xvel/2)<0)?128:0);
	arc->damageh = projectiles[sh[shot].projectile].damageh;
	arc->damagee = projectiles[sh[shot].projectile].damagee;
	arc->damageb = projectiles[sh[shot].projectile].damageb;
	arc->tankfrom = sh[shot].tankfrom;

	LaserLine(x, y, fireangle, laserlength, 7, arc->damageh, arc->damagee, arc->damageb, arc->tankfrom, LASER_NORMAL);

	numblaserblast = 0;
	laserblast_count = 0;
	laserblast_angle = arc->angle;
	do_line(buffer, x, y, x+(-laserlength*COS(fireangle)), y+(laserlength*-SIN(fireangle)), 0, LaserBlast_Proc);
	
	//arc = G_SpawnSprite(x,y,S_IONCANNON_FIRE1, (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE));
	//arc->angle = fireangle;

	SetTankTimer(1.1*TICRATE);

	// Kill the shot out
	if(sh[shot].sprite)
		sh[shot].sprite->Kill();

	sh[shot].exist = 0;

}


void	Railgun_Proc(BITMAP *b, int x, int y, int color)
{
	sprite_t*		s;
	laserblast_count--;

	if(OnScreen(x, y, SCREENWIDTH, SCREENHEIGHT) == -1)
	{
		numblaserblast++;
		return;
	}

	if(laserblast_count <= 0)
	{
		s = G_SpawnSprite(x,y,S_RAILGUNSMOKE, (MF_NORMAL|MF_CENTERCOORD|MF_TRANSLUC));

		numblaserblast++;

		laserblast_count = 7; //15;
	}

	BurnGround(ground.bmp, x, y, 15, 15);
}



void			Railgun_Thinker(int shot)
{
	//sprite_t	*arc;
	int			tank = sh[shot].tankfrom;
	int			length = characters[ta[tank].character].cannonlength+15;
	int			laserlength = SCREENWIDTH+(SCREENWIDTH/2);
	int			fireangle = sh[shot].origdeg; 
	float		x;
	float		y;

	x = sh[shot].origx-COS(fireangle)*length;
	y = sh[shot].origy-SIN(fireangle)*length;
	if(sh[shot].groundpixels == 0)	//	save shotlength else its recalc each frame
		{
		sh[shot].groundpixels = ShotLength(x, y, fireangle);
		}
	laserlength = sh[shot].groundpixels;	// just using groundpixels as a storage location.

	// Play it again, mac.
	sh[shot].StartSound(TANK_FIRERAILGUN);

	LaserLine(x, y, fireangle, laserlength, 4, projectiles[sh[shot].projectile].damageh, 
		projectiles[sh[shot].projectile].damagee, projectiles[sh[shot].projectile].damageb, 
		sh[shot].tankfrom, (LASER_NOBURNLAND));

	numblaserblast = 0;
	laserblast_count = 0;
	laserblast_angle = fireangle;
	do_line(buffer, x, y, x+(-laserlength*COS(fireangle)), y+(laserlength*-SIN(fireangle)), 0, Railgun_Proc);
	
	//arc = G_SpawnSprite(x,y,S_IONCANNON_FIRE1, (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE));
	//arc->angle = fireangle;

	SetTankTimer(1.0*TICRATE);

	// Kill the shot out
	if(sh[shot].sprite)
		sh[shot].sprite->Kill();

	sh[shot].exist = 0;

	// Make the ground fall
	ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);

}


void			LaserPulse_Thinker(int shot)
{
	int		laserpower = ConvertFromGamePower(550);
	int		tank = sh[shot].tankfrom;
	int		fireangle = sh[shot].origdeg;

	if(sh[shot].groundpixels > 25)
		{
		sh[shot].sprite->Kill();
		sh[shot].exist = 0;

		G_SpawnSprite(sh[shot].x, sh[shot].y, S_GREENLASER_PULSE_DIE1, (MF_NORMAL|MF_CENTERCOORD));
		sh[shot].StartSound(projectiles[sh[shot].projectile].explodesound);
		}

	if(sh[shot].shottics == 1)
		{
		sh[shot].xvel = (-laserpower*COS(fireangle));
		sh[shot].yvel = (laserpower*-SIN(fireangle));
		}
}


void			TunnelLaser_Thinker(int shot)
{
	int		laserpower = ConvertFromGamePower(550);
	int		tank = sh[shot].tankfrom;
	int		fireangle = sh[shot].origdeg;

	if(sh[shot].groundpixels > 15)	// shot don't go far
		{
		sh[shot].sprite->Kill();
		sh[shot].exist = 0;

		G_SpawnSprite(sh[shot].x, sh[shot].y, S_GREENLASER_PULSE_DIE1, (MF_NORMAL|MF_CENTERCOORD));
		sh[shot].StartSound(projectiles[sh[shot].projectile].explodesound);
		}

	if(sh[shot].shottics == 1)
		{
		sh[shot].xvel = (-laserpower*COS(fireangle));
		sh[shot].yvel = (laserpower*-SIN(fireangle));
		}
}



void			FlameThrower_Thinker(int shot)
{
	sprite_t*	s;
	int			tank = sh[shot].tankfrom;
	int			length = characters[ta[tank].character].cannonlength-5;
	int			fireangle = sh[shot].origdeg;
	float		power;
	float		x = sh[shot].origx-COS(fireangle)*length;
	float		y = sh[shot].origy-SIN(fireangle)*length;

	power = ConvertFromGamePower(250);

	sh[shot].StartSound(TANK_FLAMETHROWER);
	
	s = G_SpawnSprite(x,y, S_FLAME_PARENT, (MF_NORMAL|MF_NOTVISABLE|MF_NOGRAVITY));
	s->tankfrom = sh[shot].tankfrom;
	s->opacity = 255;
	s->xvel = (-power*COS(fireangle));
	s->yvel = (power*-SIN(fireangle));
	s->damageh = projectiles[sh[shot].projectile].damageh/((1*TICRATE)/2);
	s->damagee = projectiles[sh[shot].projectile].damagee/((1*TICRATE)/2);
	s->damageb = projectiles[sh[shot].projectile].damageb/((1*TICRATE)/2);

	SetTankTimer(1.5*TICRATE);

	if(sh[shot].sprite)
		sh[shot].sprite->Kill();

	sh[shot].exist = 0;

}
		
void			ElectricArc_Thinker(int shot)
{
	sprite_t	*arc;
	int			tank = sh[shot].tankfrom;
	int			length = characters[ta[tank].character].cannonlength;
	int			fireangle = sh[shot].origdeg;
	float		x;
	float		y;

	x = sh[shot].origx-COS(fireangle)*length;
	y = sh[shot].origy-SIN(fireangle)*length;
	
	arc = G_SpawnSprite(x, y, S_ELECTRICARC1, (MF_NORMAL));
	arc->rotatex = 7;
	arc->rotatey = 0;
	arc->angle = (atan((sh[shot].yvel/2)/(sh[shot].xvel/2))*(180/PI)*ACHANGE)-64+(((sh[shot].xvel/2)<0)?128:0);
	arc->damageh = projectiles[sh[shot].projectile].damageh;
	arc->damagee = projectiles[sh[shot].projectile].damagee;
	arc->damageb = projectiles[sh[shot].projectile].damageb;
	arc->tankfrom = sh[shot].tankfrom;

	LaserLine(x, y, fireangle, 60, 2, arc->damageh, arc->damagee, arc->damageb, arc->tankfrom, LASER_NORMAL);

	SetTankTimer(0.5*TICRATE);

	if(sh[shot].sprite)
		sh[shot].sprite->Kill();

	sh[shot].exist = 0;
}

void			Drill_Thinker(int shot)
{
	if(sh[shot].groundpixels >= 20)
	{
		sh[shot].explode();
	}
}


void			MiniMIRV_Thinker(int shot)
{
	int		s;
	int		spriteflags;


	// Zero Y velocity, we're about to start falling. This
	// is where the missile will be split off
	if((int)sh[shot].yvel == 0)
	{

		if(sh[shot].sprite)
			spriteflags = sh[shot].sprite->flags;
		else
			spriteflags = (MF_NORMAL | MF_CENTERCOORD);
		
		
		s = findshot();
		if(s>=0)
		{
			sh[s].Init(s, W_MINIMIRVCHILD, 0,0,0,0,0);
			sh[s].tankfrom = sh[shot].tankfrom;
			sh[s].x = sh[shot].x;
			sh[s].y = sh[shot].y;
			sh[s].origxvel = sh[s].xvel = sh[shot].xvel;
			sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
			sh[s].origdeg = sh[shot].angle;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
		}
		
		s = findshot();
		if(s>=0)
		{
			sh[s].Init(s, W_MINIMIRVCHILD, 0,0,0,0,0);
			sh[s].tankfrom = sh[shot].tankfrom;
			sh[s].x = sh[shot].x;
			sh[s].y = sh[shot].y;
			sh[s].origxvel = sh[s].xvel = sh[shot].xvel+1;
			sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
			sh[s].origdeg = sh[shot].angle;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
		}

		s = findshot();
		if(s>=0)
		{
			sh[s].Init(s, W_MINIMIRVCHILD, 0,0,0,0,0);
			sh[s].tankfrom = sh[shot].tankfrom;
			sh[s].x = sh[shot].x;
			sh[s].y = sh[shot].y;
			sh[s].origxvel = sh[s].xvel = sh[shot].xvel-1;
			sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
			sh[s].origdeg = sh[shot].angle;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
		}
		
		//sh[shot].explode();
		sh[shot].exist = 0;
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();
	}
}

void			MIRV_Thinker(int shot)
{
	int		s;
	int		spriteflags;


	// Zero Y velocity, we're about to start falling. This
	// is where the missile will be split off
	if((int)sh[shot].yvel == 0)
	{

		if(sh[shot].sprite)
			spriteflags = sh[shot].sprite->flags;
		else
			spriteflags = (MF_NORMAL | MF_CENTERCOORD);
		
		
		s = findshot();
		if(s>=0)
		{
			sh[s].Init(s, W_MIRVSHOT, 0,0,0,0,0);
			sh[s].tankfrom = sh[shot].tankfrom;
			sh[s].x = sh[shot].x;
			sh[s].y = sh[shot].y;
			sh[s].origxvel = sh[s].xvel = sh[shot].xvel;
			sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
			sh[s].origdeg = sh[shot].angle;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
			//sh[s].flags |= MISSILE_NODIRTCLIP;
		}
		
		s = findshot();
		if(s>=0)
		{
			sh[s].Init(s, W_MIRVSHOT, 0,0,0,0,0);
			sh[s].tankfrom = sh[shot].tankfrom;
			sh[s].x = sh[shot].x;
			sh[s].y = sh[shot].y;
			sh[s].origxvel = sh[s].xvel = sh[shot].xvel+1;
			sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
			sh[s].origdeg = sh[shot].angle;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
			//sh[s].flags |= MISSILE_NODIRTCLIP;
		}

		s = findshot();
		if(s>=0)
		{
			sh[s].Init(s, W_MIRVSHOT, 0,0,0,0,0);
			sh[s].tankfrom = sh[shot].tankfrom;
			sh[s].x = sh[shot].x;
			sh[s].y = sh[shot].y;
			sh[s].origxvel = sh[s].xvel = sh[shot].xvel-1;
			sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
			sh[s].origdeg = sh[shot].angle;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
			//sh[s].flags |= MISSILE_NODIRTCLIP;
		}
		
		s = findshot();
		if(s>=0)
		{
			sh[s].Init(s, W_MIRVSHOT, 0,0,0,0,0);
			sh[s].tankfrom = sh[shot].tankfrom;
			sh[s].x = sh[shot].x;
			sh[s].y = sh[shot].y;
			sh[s].origxvel = sh[s].xvel = sh[shot].xvel-2;
			sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
			sh[s].origdeg = sh[shot].angle;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
			//sh[s].flags |= MISSILE_NODIRTCLIP;
		}

		s = findshot();
		if(s>=0)
		{
			sh[s].Init(s, W_MIRVSHOT, 0,0,0,0,0);
			sh[s].tankfrom = sh[shot].tankfrom;
			sh[s].x = sh[shot].x;
			sh[s].y = sh[shot].y;
			sh[s].origxvel = sh[s].xvel = sh[shot].xvel+2;
			sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
			sh[s].origdeg = sh[shot].angle;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
			//sh[s].flags |= MISSILE_NODIRTCLIP;
		}

		sh[shot].explode();
	}
}


void			PopFlotation(int shot, int tankhit)
{
	if(ta[tankhit].flags & TANKFLAG_FLOATATION)
	{
		ta[tankhit].flags &= ~TANKFLAG_FLOATATION;
	}
}

void			FlakShard_TankHit(int shot, int tankhit)
{
	Flak_Explode(shot);
}

void Maggen_Decay(void)
{
	sprite_t	*gen;

	//	Mag-gen Unit decay each round
	//	scan sprites for any units
	gen=sprites;	//	reset pointer to first sprite in list
	while (gen)
		{
		if(gen->wobj >= 0)
			{
			if(gen->wobj == WO_MAGGEN )
				{
				gen->misc1 -= 5;								//	decay the deflect distance for each shot
				if(gen->misc1 < 10)gen->life--;	//	reduce its life after it has powered down
				}
			}
			gen = gen->next;
		}	//	end while
return;
}

void			Maggen_Release(int shot)
{
//	Hit ground so release the generator
	sprite_t	*gen;
	int		bmpw,bmph;
	int		genx, geny;

	SetTankTimer(2*TICRATE);	//	wait 2 seconds

	genx = sh[shot].x;
	geny = sh[shot].y;

	sh[shot].exist = 0;
	if(sh[shot].sprite)
		sh[shot].sprite->Kill();

	// Client allows the server to remotely do this to ensure 
	// terrain accuracy as much as possible
	if(multiplayer && multiplayermode == MP_CLIENT)
		{
#ifdef	_DEBUG
		Printf("Maggen_Release - wait for server\n");
#endif
		return;
		}

	if(geny > 590)	//	stop it sitting below bottom of screen.
		geny = 590;
		gen = G_SpawnWO(WO_MAGGEN, genx, geny);	// Spawn the mag gen sprite

	if(gen)
		{
#ifdef	_DEBUG
		Printf("Spawned the mag gen sprite ok at [%d,%d]\n", genx, geny );
#endif
		}
	else
		{
		Printf("ERROR: Can't Spawn the mag gen sprite");
		return;  //	something gone wrong !!!
		}
	
	gen->misc1 = 120; // hard coded!!!
	gen->misc2 = 3;
	gen->tankfrom = -1;

	// Update all other clients about the maggen pod
	if(multiplayer && multiplayermode == MP_SERVER)
		{
		SV_SpawnSprite(gen);
		}
		
	return;
}

void			Flak_Explode(int shot)
{
	int		num_shards = 32;
	float	shard_power = ConvertFromGamePower(400);
	int		s,wep;
	int		spriteflags = (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE);
	float	circle_add = (256/(float)num_shards);
	int		length = 35;
	float	i;

	for (i=0; i<256; i+=circle_add)
		{
		s = findshot();
		if(s < 0)		// Problems
			{ return; }

		if(rand()%2==0) ///no change ? shard types are equal
			wep = W_FLAKSHARD1;
		else
			wep = W_FLAKSHARD2;

		sh[s].Init(s, wep,
						sh[shot].x-COS(i)*length,
						sh[shot].y-SIN(i)*length,
						(-shard_power*COS(i)),
						(shard_power*-SIN(i)),
						i);

		sh[s].tankfrom = sh[shot].tankfrom;
		sh[s].groundpixels = 0;
				
		sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
		sh[s].life = 7+rANDom()%5; 	CheckRandomFunc("shot Flak_Explode");
		}
}



void			FlakShard_Thinker(int shot)
{
	sh[shot].life --;

	if(sh[shot].life <= 0)
	{
		sh[shot].exist = 0;
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();
	}
}


void			Rod_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_USEANGLE | MF_CENTERCOORD);
	
	SetTankTimer(2.0*TICRATE);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	SetTankTimer(1*TICRATE);

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_LIGHTNINGROD_FUNC, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;
}


void		Heatwave_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_ONGROUND);
	
	SetTankTimer(2*TICRATE);


	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	SetTankTimer(1*TICRATE);

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_HEATWAVE_VERYSMALL1_1, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;

}


void		EMP_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_CENTERCOORD | MF_ALLOWSMALLSCALE | MF_COOLEFFECT1);
	
	SetTankTimer(2*TICRATE);

	// Play a tune
	sh[shot].StartSound(EXPLODE_EMPMISSILE);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	SetTankTimer(1*TICRATE);

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_EMPEXPLODE, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;
	s->scale = 0.01;

}



void		FireStarter_KillFlames(int shot)
{
	sprite_t*		s = sprites;
	sprite_t*		smoke;
	int				sstate,sx,sy;

	while (s)
	{
		sstate = s->startstate.currentstate;

		if(sstate == S_FIRESTARTER_F1 || 
			sstate == S_FIRESTARTER_F2 || 
			sstate == S_FIRESTARTER_F3 || 
			sstate == S_FIRESTARTER_F4 || 
			sstate == S_FIRESTARTER_F5 || 
			sstate == S_FIRESTARTER_F6)
		{
			s->Kill();

			sx = s->x-5;
			sy = s->y-9;

			sx += rand()%(10*2);
			sy += rand()%(18*2);

			if(rand()%2==0)
				smoke = G_SpawnSprite(sx, sy, S_SMOKE_BLACK_MEDIUM, (MF_NORMAL | MF_USEANGLE | MF_TRANSLUC));
			else
				smoke = G_SpawnSprite(sx, sy, S_SMOKE_BLACK_SMALL, (MF_NORMAL | MF_USEANGLE | MF_TRANSLUC));

			smoke->opacity = 200-(rand()%128);
		}

		s = s->next;
	}
}
		


void		FireStarter_Thinker(int shot)
{
	sprite_t*	flame;
	int			sflags = sh[shot].sprite->flags;

	// Adjust the angle on the ground so the sprite is always
	// upright.
	if(sh[shot].onfloor)
	{
		if(!voice_check(sh[shot].sndvoice))
			sh[shot].StartSound(BURNING, 1, 0);

		if(sflags & MF_USEANGLE)
			sh[shot].sprite->flags &= ~MF_USEANGLE;

		if(sh[shot].xvel < 0)
		{
			if(sflags & MF_FLIPPED)
				sh[shot].sprite->flags &= ~MF_FLIPPED;
		}
		else
		{
			if(!(sflags & MF_FLIPPED))
				sh[shot].sprite->flags |= MF_FLIPPED;
		}

		if((sh[shot].x + sh[shot].xvel) >= SCREENWIDTH || (sh[shot].x + sh[shot].xvel) <= 0)
		{
			sh[shot].StopSound();
			FireStarter_KillFlames(shot);
			return;
		}
			

		// Spawn a flame
		if(!(sh[shot].shottics % 10))
		{
			flame = G_SpawnSprite(sh[shot].x, sh[shot].y, S_FIRESTARTER_F1, (MF_NORMAL | MF_ONGROUND));
			flame->damageh = 10;
			flame->damagee = 0;
			flame->damageb = 0;
			flame->tankfrom = sh[shot].tankfrom;
		}
	}
}



void			TowerPod_Thinker(int shot)
{
	sprite_t	*tower;
	int		bmpw,bmph;
	int		ogl,ogr;
	int		digx, digy;

	// Client allows the server to remotely do this to ensure 
	// terrain accuracy as much as possible
	if(multiplayer && multiplayermode == MP_CLIENT)
	{
		sh[shot].exist = 0;
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();
		return;
	}

	if(sh[shot].shottics > TICRATE/2)
	{
		// Dig a hole
		bmpw = ((BITMAP *)spritedata[states[S_TOWERPOD_BUILD1].datafilelookup].dat)->w;
		bmph = ((BITMAP *)spritedata[states[S_TOWERPOD_BUILD1].datafilelookup].dat)->h;

		ogl = OnGround(sh[shot].x-(bmpw/2), sh[shot].y, ground.bmp);
		ogr = OnGround(sh[shot].x+(bmpw/2), sh[shot].y, ground.bmp);

		digy = MAX(ogl, ogr)-bmph;
		digx = sh[shot].x - (bmpw/2);

		// Create the rectangle into the dirt
		rectfill(ground.bmp, digx, digy, (digx + bmpw), (digy + bmph), TRANSCOLOR);

		SetTankTimer(2*TICRATE);

		ground.needsupdate = true;

		sh[shot].exist = 0;
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();
		
		// Spawn the sprite
		tower = G_SpawnWO(WO_TOWERPODCONSTRUCTION, digx, digy);

		if(tower)
			tower->tankfrom = sh[shot].tankfrom;

		// Collaspe the ground
		ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);

		// Update all other clients about the tower pod
		if(multiplayer && multiplayermode == MP_SERVER)
		{
			BCast_WriteByte(STOC_SPAWNTOWERPOD);
			BCast_WriteByte(tower->netid);
			BCast_WriteByte(tower->wobj);
			BCast_WriteByte(tower->tankfrom);

			BCast_WriteShort(digx);
			BCast_WriteShort(digy);
			BCast_WriteShort(bmpw);
			BCast_WriteShort(bmph);
		}
		
		return;
	}
}


void			MissileTurretShot_Thinker(int shot)
{
	sprite_t	*tower;
	int		bmpw,bmph;
	int		ogl,ogr;
	int		digx, digy;

	// Client allows the server to remotely do this to ensure 
	// terrain accuracy as much as possible
	if(multiplayer && multiplayermode == MP_CLIENT)
	{
		sh[shot].exist = 0;
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();
		return;
	}

	if(sh[shot].shottics > TICRATE/2)
	{
		// Dig a hole
		bmpw = ((BITMAP *)spritedata[states[S_MISSILETURRET_BUILD1].datafilelookup].dat)->w;
		bmph = ((BITMAP *)spritedata[states[S_MISSILETURRET_BUILD1].datafilelookup].dat)->h;

		ogl = OnGround(sh[shot].x-(bmpw/2), sh[shot].y, ground.bmp);
		ogr = OnGround(sh[shot].x+(bmpw/2), sh[shot].y, ground.bmp);

		digy = MAX(ogl, ogr)-bmph;
		digx = sh[shot].x - (bmpw/2);

		// Create the rectangle into the dirt
		rectfill(ground.bmp, digx, digy, (digx + bmpw), (digy + bmph), TRANSCOLOR);

		SetTankTimer(2*TICRATE);

		ground.needsupdate = true;

		sh[shot].exist = 0;
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();
		
		// Spawn the sprite
		tower = G_SpawnWO(WO_MISSILETURRET, digx, digy);

		if(tower)
		{
			tower->tankfrom = sh[shot].tankfrom;
			tower->endlevelshoot = 1;
		}

		// Assign a net id
		tower->netid = ta[sh[shot].tankfrom].lastshotnetid;

		// Collaspe the ground
		ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);

		// Update all other clients about the tower pod
		if(multiplayer && multiplayermode == MP_SERVER)
		{
			BCast_WriteByte(STOC_SPAWNTOWERPOD);
			BCast_WriteByte(tower->netid);
			BCast_WriteByte(tower->wobj);
			BCast_WriteByte(tower->tankfrom);

			BCast_WriteShort(digx);
			BCast_WriteShort(digy);
			BCast_WriteShort(bmpw);
			BCast_WriteShort(bmph);
		}
		
		return;
	}
}


void			CheckTankSentryCount(int	tank, int maxpods)
{
	sprite_t*	s = sprites;
	sprite_t*	oldests=NULL;
	int			numpods=0;
	int			oldesttics=0;

	while (s)
	{
		if(s->wobj >= 0)
		{
			if((s->wobj == WO_HEATSENTRYPOD || 
				s->wobj == WO_ENERGYSENTRYPOD || 
				s->wobj == WO_BALLISTICSENTRYPOD) && 
				s->tankfrom == tank)
			{
				numpods++;

				// There are too many pods. Kill any extra after this out
				if(s->life > oldesttics)
				{
					oldesttics = s->life;
					oldests = s;
				}
			}

		}

		s = s->next;
	}

	if(oldests != NULL && numpods >= maxpods)
	{
		oldests->SetState(wobjects[oldests->wobj].deathstate);

		if(multiplayer && multiplayermode == MP_SERVER)
		{
			BCast_WriteByte(STOC_SPRITESTATE);
			BCast_WriteShort(oldests->netid);
			BCast_WriteShort((int)wobjects[oldests->wobj].deathstate);
		}
	}
}

//
// KillTankSentryPods - If a tank dies, we'll want to kill all his sentry pods
//

void		KillTankSentryPods(int tank)
{
	sprite_t*	s = sprites;

	while (s)
	{
		if (s->wobj >= 0)
		{
			if ((s->wobj == WO_HEATSENTRYPOD || 
				s->wobj == WO_ENERGYSENTRYPOD || 
				s->wobj == WO_BALLISTICSENTRYPOD) && 
				s->tankfrom == tank)
			{
				s->SetState(wobjects[s->wobj].deathstate);

				// Server kills all sentry pods for clients
				if (multiplayer && multiplayermode == MP_SERVER)
				{
					BCast_WriteByte(STOC_SPRITESTATE);
					BCast_WriteShort(s->netid);
					BCast_WriteShort((int)wobjects[s->wobj].deathstate);
				}
			}
		}

		s = s->next;
	}
}


void			HSentry_Spawn(int shot)
{
	sprite_t*		pod;
	int				bmpw,bmph;
	int				ogl,ogr;
	int				digx, digy;

	// First things first, remove the old sprite
	sh[shot].exist = 0;
	if (sh[shot].sprite)
		sh[shot].sprite->Kill();

	// Server will spawn this for us
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// There has to be a tank who shot this so we can give credit
	if (ta[sh[shot].tankfrom].exist <= 0 || ta[sh[shot].tankfrom].health <= 0)
	{
		SetTankTimer(10);
		return;
	}

	// Make sure there is a limit on to how many sentries we can spawn. If 
	// the player tries to spawn more than 3, then destroy the oldest one
	CheckTankSentryCount(sh[shot].tankfrom, 3);

	bmpw = ((BITMAP *)spritedata[states[S_HSENTRY2].datafilelookup].dat)->w;
	bmph = ((BITMAP *)spritedata[states[S_HSENTRY2].datafilelookup].dat)->h;

	ogl = OnGround(sh[shot].x-(bmpw/2), sh[shot].y, ground.bmp);
	ogr = OnGround(sh[shot].x+(bmpw/2), sh[shot].y, ground.bmp);

//	digy = MIN(ogl, ogr)-(bmph/2); // try to position better
	digy = (ogl+ogr)/2-(bmph/2);
	digx = sh[shot].x - (bmpw/2);

	pod = G_SpawnWO(WO_HEATSENTRYPOD, 
		digx, 
		digy);

	if (pod)
	{
		pod->tankfrom = sh[shot].tankfrom;
		pod->endlevelshoot = 1;				// Setup for shot after this turn
		pod->tintr = ta[sh[shot].tankfrom].colorr;
		pod->tintg = ta[sh[shot].tankfrom].colorg;
		pod->tintb = ta[sh[shot].tankfrom].colorb;
		pod->tintvalue = TANK_OPACITY;

		// Assign net id
		pod->netid = ta[sh[shot].tankfrom].lastshotnetid;
	}


	// Send to all clients if we're the server
	if(multiplayer && multiplayermode == MP_SERVER)
		{
		SV_SpawnSprite(pod);
		}

	// End this turn
	SetTankTimer(100);

}


void			ESentry_Spawn(int shot)
{
	sprite_t*		pod;
	int				bmpw,bmph;
	int				ogl,ogr;
	int				digx, digy;

	// First things first, remove the old sprite
	sh[shot].exist = 0;
	if(sh[shot].sprite)
		sh[shot].sprite->Kill();

	// Server will spawn this for us
	if(multiplayer && multiplayermode == MP_CLIENT)
		return;

	// There has to be a tank who shot this so we can give credit
	if(ta[sh[shot].tankfrom].exist <= 0 || ta[sh[shot].tankfrom].health <= 0)
		{
		SetTankTimer(10);
		return;
		}

	// Make sure there is a limit on to how many sentries we can spawn. If 
	// the player tries to spawn more than 3, then destroy the oldest one
	CheckTankSentryCount(sh[shot].tankfrom, 3);

	bmpw = ((BITMAP *)spritedata[states[S_HSENTRY2].datafilelookup].dat)->w;
	bmph = ((BITMAP *)spritedata[states[S_HSENTRY2].datafilelookup].dat)->h;

	ogl = OnGround(sh[shot].x-(bmpw/2), sh[shot].y, ground.bmp);
	ogr = OnGround(sh[shot].x+(bmpw/2), sh[shot].y, ground.bmp);

//	digy = MIN(ogl, ogr)-(bmph/2); // try to position better
	digy = (ogl+ogr)/2-(bmph/2);
	digx = sh[shot].x - (bmpw/2);

	pod = G_SpawnWO(WO_ENERGYSENTRYPOD, 
		digx, 
		digy);

	if(pod)
		{
		pod->tankfrom = sh[shot].tankfrom;
		pod->endlevelshoot = 1;				// Setup for shot after this turn

		// Set color attribs
		pod->tintr = ta[sh[shot].tankfrom].colorr;
		pod->tintg = ta[sh[shot].tankfrom].colorg;
		pod->tintb = ta[sh[shot].tankfrom].colorb;
		pod->tintvalue = TANK_OPACITY;
		
		// Assign net id
		pod->netid = ta[sh[shot].tankfrom].lastshotnetid;
		}

	// Send to all clients if we're the server
	if(multiplayer && multiplayermode == MP_SERVER)
		{
		SV_SpawnSprite(pod);
		}

	// End this turn
	SetTankTimer(100);
}


void			BSentry_Spawn(int shot)
{
	sprite_t*		pod;
	int				bmpw,bmph;
	int				ogl,ogr;
	int				digx, digy;

	// First things first, remove the old sprite
	sh[shot].exist = 0;
	if(sh[shot].sprite)
		sh[shot].sprite->Kill();

	// Server will spawn this for us
	if(multiplayer && multiplayermode == MP_CLIENT)
		return;

	// There has to be a tank who shot this so we can give credit
	if(ta[sh[shot].tankfrom].exist <= 0 || ta[sh[shot].tankfrom].health <= 0)
		{
		SetTankTimer(10);
		return;
		}

	// Make sure there is a limit on to how many sentries we can spawn. If 
	// the player tries to spawn more than 3, then destroy the oldest one
	CheckTankSentryCount(sh[shot].tankfrom, 3);

	bmpw = ((BITMAP *)spritedata[states[S_HSENTRY2].datafilelookup].dat)->w;
	bmph = ((BITMAP *)spritedata[states[S_HSENTRY2].datafilelookup].dat)->h;

	ogl = OnGround(sh[shot].x-(bmpw/2), sh[shot].y, ground.bmp);
	ogr = OnGround(sh[shot].x+(bmpw/2), sh[shot].y, ground.bmp);

//	digy = MIN(ogl, ogr)-(bmph/2); // try to position better
	digy = (ogl+ogr)/2-(bmph/2);
	digx = sh[shot].x - (bmpw/2);

	pod = G_SpawnWO(WO_BALLISTICSENTRYPOD,digx,digy);

	if(pod)
		{
		pod->tankfrom = sh[shot].tankfrom;
		pod->endlevelshoot = 1;				// Setup for shot after this turn

		// Set color attribs
		pod->tintr = ta[sh[shot].tankfrom].colorr;
		pod->tintg = ta[sh[shot].tankfrom].colorg;
		pod->tintb = ta[sh[shot].tankfrom].colorb;
		pod->tintvalue = TANK_OPACITY;

		// Assign net id
		pod->netid = ta[sh[shot].tankfrom].lastshotnetid;
		}

	// Send to all clients if we're the server
	if(multiplayer && multiplayermode == MP_SERVER)
		{
		SV_SpawnSprite(pod);
		}
	// End this turn
	SetTankTimer(100);
}

//
// Molten Rod
//

void			MoltenRod_Thinker(int shot)
{
	int				laserpower = ConvertFromGamePower(550);
	int				fireangle = sh[shot].origdeg;

	// Delete this shot from existance
	if(sh[shot].shottics == 1)
		{
		sh[shot].xvel = (-laserpower*COS(fireangle));
		sh[shot].yvel = (laserpower*-SIN(fireangle));

		// Play it again, Sam.
		sh[shot].StartSound(TANK_FIREMOLTENROD);
		}

	if(sh[shot].groundpixels > 25)
		{
		sh[shot].sprite->Kill();
		sh[shot].exist = 0;
		SetTankTimer(10);

		// Collaspe the ground
		ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
		}

}


void			MoltenRod_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_USEANGLE | MF_CENTERCOORD);
	
	// Set the timer
	SetTankTimer(2.5*TICRATE);

	// Erase the orginal shot
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
		sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_HSENTRY_SHOT1, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;
}

//
// Electric Laser
//

void			ELaser_Thinker(int shot)
{
	int		laserpower = ConvertFromGamePower(550);
	int		tank = sh[shot].tankfrom;
	int		fireangle = sh[shot].origdeg;

	if(sh[shot].groundpixels > 20)
		{
		//sh[shot].sprite->Kill();
		//sh[shot].exist = 0;

		//G_SpawnSprite(sh[shot].x, sh[shot].y, S_GREENLASER_PULSE_DIE1, (MF_NORMAL|MF_CENTERCOORD));
		sh[shot].StartSound(projectiles[sh[shot].projectile].explodesound);
		sh[shot].explode();
		}

	if(sh[shot].shottics == 1)
		{
		sh[shot].xvel = (-laserpower*COS(fireangle));
		sh[shot].yvel = (laserpower*-SIN(fireangle));

		// Play it again, Sam.
		sh[shot].StartSound(TANK_ELECTRICLASERFIRE);
		}
}


//
// Uzi Spray
//

void	Uzi_Thinker(int shot)
{
	int		newshot;
	int		tfrom = sh[shot].tankfrom;
	int		flags = (MF_NORMAL | MF_CENTERCOORD);
	int		length = characters[ta[tfrom].character].cannonlength+
					(characters[ta[tfrom].character].cannonlength/2);
	int		missilepower = ConvertFromGamePower(3000);
	int		fireangle = sh[shot].origdeg; //ta[tfrom].deg;

	sprite_t*		shell;
	int				shellpower;
	int				shellangle;

	sh[shot].xvel = 0;
	sh[shot].yvel = 0;

	if(sh[shot].shottics > 1*TICRATE)
		{
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();

		sh[shot].exist = 0;

		// Kill any tanks
		//KillTanks();

		return;
		}

	if(!(sh[shot].shottics % 4))
		{
		newshot = findshot();
		
		// Something bad happened
		if(newshot < 0)
			return;

		// Calculate a random angle
		fireangle -= 1;
		fireangle += rANDom()%2;			CheckRandomFunc("shot Uzi_Thinker");


		// Keep track of the last weapon shot
		lasttankshot = W_UZISPRAY;

		sh[newshot].index = newshot;
		sh[newshot].Init(newshot, W_UZISPRAYSHOT,
					sh[shot].origx, //ta[tfrom].barrelshotx,
					sh[shot].origy, //ta[tfrom].barrelshoty,
					(-missilepower*COS(fireangle)),
					(missilepower*-SIN(fireangle)),
					fireangle);

		sh[newshot].tankfrom = tfrom;

		
		if(projectiles[W_UZISPRAY].firesound >= 0)
			{
			sh[newshot].StartSound(projectiles[W_UZISPRAY].firesound);
			}
		flags |= MF_USEANGLE;

		// Bind a sprite to our shot
		sh[newshot].sprite = G_SpawnSprite((int)sh[shot].origx,(int)sh[shot].origy, projectiles[W_MACHINEGUNSHOT].state, flags);

		// Spawn a empty shell, the direction the sentry is facing dictates the
		// direction of the shell
		shellpower = ConvertFromGamePower(100+rand()%50);
		shell = G_SpawnSprite(sh[shot].origx, sh[shot].origy, S_EMPTYSHELL, (MF_NORMAL|MF_GROUNDCLIP|MF_BOUNCE|MF_USEANGLE));
		
		// Tank is facing the right
		if(fireangle > 64 && fireangle < 192)
			{
			shellangle = 30+rand()%10;
			shell->xvel = (-shellpower*COS(shellangle));
			}
		else	// Tank is facing the left
			{
			shellangle = 90+rand()%10;
			shell->xvel = (-shellpower*COS(shellangle));
			}
		// Add some Y velocity
		shell->yvel = (shellpower*-SIN(shellangle));
		}
}

void			Deathshead_Thinker(int shot)
{
	int		s;
	int		spriteflags;
	int		i;
	int		num_sideshots=3;	// Number of child missiles that will appear on each side

	// Unit already reached apex and spawned child missiles, return.
	if(sh[shot].sprite)
		{
		if(sh[shot].sprite->startstate.currentstate == S_DEATHSHEADEMPTY_1 || 
			sh[shot].sprite->startstate.currentstate == S_DEATHSHEADEMPTY_2)
			{
			return;
			}
		}

	// Zero Y velocity, we're about to start falling. This
	// is where the missile will be split off
	if((int)sh[shot].yvel == 0)
		{

		if(sh[shot].sprite)
			{
			// Set sprite to the empty shell state
			sh[shot].sprite->SetState(S_DEATHSHEADEMPTY_1);

			// Get sprite flags
			spriteflags = sh[shot].sprite->flags;
			}
		else	// This should never happen
			spriteflags = (MF_NORMAL | MF_CENTERCOORD);


		for (i=0; i<num_sideshots; i++)
			{
			s = findshot();
			if(s>=0)
				{
				sh[s].Init(s, W_DEATHSHEADCHILD, 0,0,0,0,0);
				sh[s].tankfrom = sh[shot].tankfrom;
				sh[s].x = sh[shot].x;
				sh[s].y = sh[shot].y;
				sh[s].origxvel = sh[s].xvel = sh[shot].xvel+(i+1);
				sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
				sh[s].origdeg = sh[shot].angle;
				sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
				}

			s = findshot();
			if(s>=0)
				{
				sh[s].Init(s, W_DEATHSHEADCHILD, 0,0,0,0,0);
				sh[s].tankfrom = sh[shot].tankfrom;
				sh[s].x = sh[shot].x;
				sh[s].y = sh[shot].y;
				sh[s].origxvel = sh[s].xvel = sh[shot].xvel-(i+1);
				sh[s].origyvel = sh[s].yvel = sh[shot].yvel;
				sh[s].origdeg = sh[shot].angle;
				sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, spriteflags);
				}
			}
		}
}

void			Tankgib_Thinker(int shot)
{
	int			state;
	// Choose a different, random sprite based on our 6 different 
	// tank gib sprites. This is done so we don't have 6 different
	// tank-shard missiles
	if(sh[shot].shottics == 1)
		{
		if(sh[shot].sprite)
			{
			state = S_TANKGIB1+rANDom()%5;			CheckRandomFunc("shot Tankgib_Thinker");
			sh[shot].sprite->SetState((_state)state);
			}
		}
}


//
// Mortar Spray
//

void	MonkeyMortar_Thinker(int shot)
{
	BITMAP	*mbase = (BITMAP *)spritedata[MORTARBASE].dat;

	int		newshot;
	int		tfrom = sh[shot].tankfrom;
	int		flags = (MF_NORMAL | MF_CENTERCOORD);
	int		length = 25;
	int		fireangle = sh[shot].origdeg; //ta[tfrom].deg;
	int		firepower = (int)ConvertToGamePower(ta[tfrom].lpower);

	int		spawnx,spawny;

	int		mx,my;

	sh[shot].xvel = 0;
	sh[shot].yvel = 0;

	if(sh[shot].shottics > 300)
		{
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();

		sh[shot].exist = 0;

		// Remove flag
		ta[tfrom].flags &= ~TANKFLAG_MONKEYMORTAR;
		return;
		}

	// Set flag to show graphic and new barrel
	ta[tfrom].flags |= TANKFLAG_MONKEYMORTAR;

	// Server will spawn everything for us
	if(multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Figure out barell positions
	my = ((ta[tfrom].y+ta[tfrom].sizey)-mbase->h)+3;

	if(ta[tfrom].deg <= 64)		// Degauss : fireangle has been truncated and gives wrong results
		{
		mx = (ta[tfrom].x-mbase->w)+(ta[tfrom].sizex/2);

		spawnx = (mx+(29-(ta[tfrom].sizex/2))-COS(fireangle)*length);
		spawny = (my+5-SIN(fireangle)*length);
		}
	else
		{
		mx = ta[tfrom].x-(ta[tfrom].sizex/2);

		spawnx = ((mx+29)-COS(fireangle)*length);
		spawny = (my+5-SIN(fireangle)*length);
		}


	if(!(sh[shot].shottics % 30))
		{
		newshot = findshot();
		
		// Something bad happened
		if(newshot < 0)
			return;
		//ta[tfrom].SetFlareTics(2);
		ta[tfrom].flaretics = 2;

		firepower -= 40;
		firepower += rANDom()%160;

		// Calculate a random angle
		fireangle -= 5;
		fireangle += rANDom()%10;			CheckRandomFunc("random angle MonkeyMortar_Thinker");
		
		// Keep track of the last weapon shot
		lasttankshot = W_MORTARSHELL;

		sh[newshot].index = newshot;
		sh[newshot].Init(newshot, W_MORTARSHELL,
					spawnx,
					spawny,
					/*sh[shot].origxvel,
					sh[shot].origyvel,*/
					(-ConvertFromGamePower(firepower)*COS(fireangle)),
					(ConvertFromGamePower(firepower)*-SIN(fireangle)),
					fireangle);

		sh[newshot].tankfrom = tfrom;

		if(multiplayer && multiplayermode == MP_SERVER)
			{
			sh[newshot].spawndeg = sh[newshot].angle;
			sh[newshot].spawnx = sh[newshot].origx;
			sh[newshot].spawny = sh[newshot].origy;
			sh[newshot].spawnxvel = sh[newshot].xvel;
			sh[newshot].spawnyvel = sh[newshot].yvel;
				
			// Broadcast to all clients
			BCast_Shot(newshot);
			}
		
		if(projectiles[W_MORTARSHELL].firesound >= 0)
			{ sh[newshot].StartSound(projectiles[W_MORTARSHELL].firesound); }

		flags |= MF_USEANGLE;

		// Bind a sprite to our shot
		sh[newshot].sprite = G_SpawnSprite((int)sh[shot].origx,(int)sh[shot].origy, projectiles[W_MORTARSHELL].state, flags);
		}
}


void		MortarShell_Explode(int shot)
{
	particlenode_t	*p;

	int		nump = 30;
	int		i;
	int		plife,pangle;

	// Spawn some particles, just for looks
	for (i=0; i<nump; i++)
	{
		// Set particle life, randomize this a bit
		plife = 60+rand()%60;

		// Randomize the angle as well
		pangle = rand()%255;

		p = pdirt->InsertParticle(sh[shot].x, sh[shot].y, pangle, ConvertFromGamePower(400+rand()%100), makecol(255,237,33), plife, PFLAGS_STATIC, 0, 0);
		if(p)
			p->bounce = 5;
	}
}


void			PolarisBeep_Thinker(int shot)
{
	// Play a sound twice every second
	if(!(gametics % (TICRATE/2)))
		sh[shot].StartSound(POLARIS_BEEP);
}


void			Polaris_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE);
	
	SetTankTimer(7*TICRATE);

	// Start a sound
	sh[shot].StartSound(POLARIS_OPEN);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_POLARISOPEN1, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;
	s->radius = projectiles[sh[shot].projectile].expradius;
}


void	BHBomb_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE);
	
	SetTankTimer(11*TICRATE);

	// Start a sound
	sh[shot].StartSound(BHB_SHELLOPEN);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_BHBOMB_OPEN1, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->tankfrom = sh[shot].tankfrom;
	s->radius = projectiles[sh[shot].projectile].expradius;
}


void	TEL_PositiveExplode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE);
	
	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_TEL_POSITIVE, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->tankfrom = sh[shot].tankfrom;
	s->angle = sh[shot].sprite->angle;
}

void	TEL_NegativeExplode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE);
	
	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_TEL_NEGATIVE, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->tankfrom = sh[shot].tankfrom;
	s->angle = sh[shot].sprite->angle;
}


void			A_FunkyExplode(int shot)
{
	sprite_t*	s;
	int				spriteflags = (MF_NORMAL | MF_NOTVISABLE);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	if(!multiplayer || multiplayermode == MP_SERVER)
	{
		s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_FUNKYBOMB_THINKER, spriteflags);
		if(s) { s->tankfrom = sh[shot].tankfrom; }
	
		SetTankTimer(TICRATE);
	}
}


void			A_FunkyNukeExplode(int shot)
{
	sprite_t*	s;
	int				spriteflags = (MF_NORMAL | MF_NOTVISABLE);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	if(!multiplayer || multiplayermode == MP_SERVER)
	{
		s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_FUNKYNUKE_THINKER, spriteflags);
		if(s) { s->tankfrom = sh[shot].tankfrom; }
	
		SetTankTimer(TICRATE);
	}
}


void	A_AS_FlareExplode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE);
	
	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	// Set how long this will take
	SetTankTimer(6*TICRATE);

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_AS_FLARE, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->tankfrom = sh[shot].tankfrom;
	s->angle = sh[shot].sprite->angle;
}


void	IonBattery_Thinker(int shot)
{
	sprite_t*	s;

	if(sh[shot].shottics == 1)
	{
		s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_IONBATTERY_THINKER, (MF_NORMAL|MF_NOTVISABLE|MF_USEANGLE));

		if(s)
		{
			s->damageh = projectiles[sh[shot].projectile].damageh;
			s->damagee = projectiles[sh[shot].projectile].damagee;
			s->damageb = projectiles[sh[shot].projectile].damageb;
			s->tankfrom = sh[shot].tankfrom;
			s->angle = sh[shot].sprite->angle;
			s->spawnfrom = sh[shot].sprite->spawnfrom;
		}

		sh[shot].exist = 0;
		if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	}
}




void	IonBatteryChild_Thinker(int shot)
{
	int		laserpower = ConvertFromGamePower(750);
	int		tank = sh[shot].tankfrom;
	int		fireangle = sh[shot].origdeg;

	if(sh[shot].groundpixels >= 20)
	{
		// Make it go boom
		sh[shot].explode();

		//G_SpawnSprite(sh[shot].x, sh[shot].y, S_GREENLASER_PULSE_DIE1, (MF_NORMAL|MF_CENTERCOORD));
		sh[shot].StartSound(projectiles[sh[shot].projectile].explodesound);
	}

	if(sh[shot].shottics == 1)
	{
		sh[shot].xvel = (-laserpower*COS(fireangle));
		sh[shot].yvel = (laserpower*-SIN(fireangle));
	}
}


void			HeatCannon_Thinker(int shot)
{
	int		laserpower = ConvertFromGamePower(350);
	int		tank = sh[shot].tankfrom;
	int		fireangle = sh[shot].origdeg;

	if(sh[shot].shottics == 1)
	{
		sh[shot].xvel = (-laserpower*COS(fireangle));
		sh[shot].yvel = (laserpower*-SIN(fireangle));
	}
}


void		HeatCannon_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_ONGROUND);
	
	SetTankTimer(2*TICRATE);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_HEATWAVE_VERYSMALL1_1, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;
}


void	FireToad1_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_USEANGLE | MF_CENTERCOORD | MF_NOGRAVITY);
	
	SetTankTimer(1*TICRATE);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x-sh[shot].xvel, sh[shot].y-sh[shot].yvel, S_FIRETOAD_STAGE1, spriteflags);
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;
	s->xvel = -(sh[shot].xvel*0.75);
	s->yvel = -(sh[shot].yvel*0.75);
	s->misc1 = 1;
}



void	FireToad2_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_USEANGLE | MF_CENTERCOORD | MF_NOGRAVITY);
	
	SetTankTimer(1*TICRATE);

	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x-sh[shot].xvel, sh[shot].y-sh[shot].yvel, S_FIRETOAD_STAGE2, spriteflags);
	s->angle = sh[shot].sprite->angle;
	s->tankfrom = sh[shot].tankfrom;
	s->xvel = -(sh[shot].xvel*0.75);
	s->yvel = -(sh[shot].yvel*0.75);
	s->misc1 = 2;
}


void	Proton_Thinker(int shot)
{
	int		newshot;
	int		tfrom = sh[shot].tankfrom;
	int		flags = (MF_NORMAL | MF_CENTERCOORD);
	int		length = characters[ta[tfrom].character].cannonlength+
					(characters[ta[tfrom].character].cannonlength/2);
	int		missilepower = (int)ConvertToGamePower(ta[tfrom].lpower);
	int		fireangle = sh[shot].origdeg;

	sh[shot].xvel = 0;
	sh[shot].yvel = 0;

	// Shot is considered done after 3.5 seconds
	if(sh[shot].shottics >= 2*TICRATE)
	{
		if(sh[shot].sprite)
			sh[shot].sprite->Kill();

		sh[shot].exist = 0;
		return;
	}

	if(!(sh[shot].shottics % 5))
	{
		newshot = findshot();
		
		// Something bad happened
		if(newshot < 0)
			return;

		missilepower -= 40;
		missilepower += rANDom()%40;			CheckRandomFunc("shot Proton_Thinker");

		// Calculate a random angle
		fireangle -= 3;
		fireangle += rANDom()%6;					CheckRandomFunc("random angle Proton_Thinker");

		// Keep track of the last weapon shot
		lasttankshot = W_PROTONCHILD;

		sh[newshot].index = newshot;
		sh[newshot].Init(newshot, W_PROTONCHILD,
					ta[tfrom].barrelshotx,
					ta[tfrom].barrelshoty,
					(-ConvertFromGamePower(missilepower)*COS(fireangle)),
					(ConvertFromGamePower(missilepower)*-SIN(fireangle)),
					fireangle);

		sh[newshot].tankfrom = tfrom;

		
		if(projectiles[W_PROTONCHILD].firesound >= 0)
		{ sh[newshot].StartSound(projectiles[W_PROTONCHILD].firesound); }

		flags |= MF_USEANGLE;

		// Bind a sprite to our shot
		sh[newshot].sprite = G_SpawnSprite((int)sh[shot].origx,(int)sh[shot].origy, projectiles[W_PROTONCHILD].state, flags);

		// Give it some flare!
		ta[tfrom].SetPhotonTics(1);
	}

}


void	A_OBC_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL | MF_CENTERCOORD);
	
	// Delete this shot from existance
	if(sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	// Set how long this will take
	SetTankTimer(2*TICRATE);

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_OBCBEACON1, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->tankfrom = sh[shot].tankfrom;
	s->radius = projectiles[sh[shot].projectile].expradius;
}


void			SuperRailgun_Thinker(int shot)
{
	int			tank = sh[shot].tankfrom;
	int			length=0;
	int			laserlength = SCREENWIDTH+(SCREENWIDTH/2);
	int			fireangle = sh[shot].origdeg; 
	int			newshot=0;
	int			spriteflags = (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE);
	float		x;
	float		y;

	x = sh[shot].origx-COS(fireangle)*length;
	y = sh[shot].origy-SIN(fireangle)*length;

	// First shot?
	if(sh[shot].misc1 == 0)
	{
		// Play it again, sam.
		sh[shot].StartSound(TANK_FIRESUPERRAIL);

		// Set the length coming from the barrel
		length = characters[ta[tank].character].cannonlength+15;

		sh[shot].angle = fireangle;
	}

	if(!(sh[shot].shottics % 20))
	{
		// Setup a shot, giving the spawner credit
		newshot = findshot();
		if(newshot>=0)
		{
			sh[newshot].Init(newshot, W_SUPERRAILCHILD, sh[shot].x, sh[shot].y, 0,0, sh[shot].angle);
			sh[newshot].tankfrom = sh[shot].tankfrom;
			sh[newshot].origdeg = sh[shot].angle;
			sh[newshot].origx = sh[shot].x;
			sh[newshot].origy = sh[shot].y;
			sh[newshot].misc1 = sh[shot].misc1;
			
			sh[newshot].sprite = G_SpawnSprite(sh[newshot].x,
				sh[newshot].y, 
				projectiles[sh[newshot].projectile].state, 
				spriteflags);

			if(multiplayer && multiplayermode == MP_SERVER)
			{
				sh[newshot].spawndeg = sh[newshot].angle;
				sh[newshot].spawnx = sh[newshot].origx;
				sh[newshot].spawny = sh[newshot].origy;
				
				// Broadcast to all clients
				BCast_Shot(newshot);
			}
		}
		
		// Now, figure out where this shot is going and setup some coordinates
		// to fire the opposite direction
		do
		{
			sh[shot].x += sh[shot].xvel;
			sh[shot].y += sh[shot].yvel;

		} while(OnScreen(sh[shot].x, sh[shot].y, SCREENWIDTH, SCREENHEIGHT) != -1);

		// Make sure we stay inside the screen, so adjust some parameters here
		if (sh[shot].x > SCREENWIDTH)
		{
			sh[shot].x = SCREENWIDTH;
			sh[shot].xvel = -sh[shot].xvel;			
		}
		if (sh[shot].x < 0)
		{
			sh[shot].x = 0;
			sh[shot].xvel = -sh[shot].xvel;			
		}
		if (sh[shot].y > SCREENHEIGHT)
		{
			sh[shot].y = SCREENHEIGHT;
			sh[shot].yvel = -sh[shot].yvel;			
		}
		if (sh[shot].y < 0)
		{
			sh[shot].y = 0;
			sh[shot].yvel = -sh[shot].yvel;			
		}

		// Figure out a new firing angle
		sh[shot].angle = CalculateAngle(sh[shot].x, sh[shot].y, sh[shot].x+sh[shot].xvel, sh[shot].y+sh[shot].yvel);

		SetTankTimer(1.0*TICRATE);
	
		sh[shot].misc1++;
	
		// Kill the shot out
		if (sh[shot].misc1 > 16)
		{
			if (sh[shot].sprite)
				sh[shot].sprite->Kill();
		
			sh[shot].exist = 0;
	
			// Make the ground fall
			ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
		}
	}
}


void	SuperRailgun_Proc(BITMAP *b, int x, int y, int color)
{
	sprite_t*		s;
	laserblast_count--;

	if (OnScreen(x, y, SCREENWIDTH, SCREENHEIGHT) == -1)
	{
		numblaserblast++;
		return;
	}

	if (laserblast_count <= 0)
	{
		s = G_SpawnSprite(x,y,S_SUPERRAILSMOKE2, (MF_NORMAL|MF_CENTERCOORD|MF_TRANSLUC));

		numblaserblast++;

		laserblast_count = 15;
	}
}


void			SuperRailgunChild_Thinker(int shot)
{
	int			tank = sh[shot].tankfrom;
	int			length=0;
	int			laserlength = SCREENWIDTH+(SCREENWIDTH/2);
	int			fireangle = sh[shot].origdeg; 
	int			laserflags=0;
	float		x;
	float		y;

	x = sh[shot].origx-COS(fireangle)*length;
	y = sh[shot].origy-SIN(fireangle)*length;

	// Play it again, sam.
	sh[shot].StartSound(TANK_FIRESUPERRAIL);

	// Set the length coming from the barrel
	length = characters[ta[tank].character].cannonlength+15;

	fireangle = sh[shot].origdeg;

	// Do this since we don't want the first shot to hit ourselves
	if (sh[shot].misc1 == 0)
		laserflags = LASER_NORMAL;
	else
		laserflags = (LASER_NORMAL|LASER_CANDAMAGESELF);
	
	LaserLine(x, y, fireangle, laserlength, 8, projectiles[sh[shot].projectile].damageh, 
		projectiles[sh[shot].projectile].damagee, projectiles[sh[shot].projectile].damageb, 
		sh[shot].tankfrom, laserflags);
	
	numblaserblast = 0;
	laserblast_count = 0;
	laserblast_angle = fireangle;
	do_line(buffer, x, y, x+(-laserlength*COS(fireangle)), y+(laserlength*-SIN(fireangle)), 0, SuperRailgun_Proc);
		
	SetTankTimer(1.0*TICRATE);
	
	if (sh[shot].sprite)
		sh[shot].sprite->Kill();
		
	sh[shot].exist = 0;
	
	// Make the ground fall
	//ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
}



void	ROF_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL);
	
	SetTankTimer(2*TICRATE);

	// Start a sound
	sh[shot].StartSound(BHB_SHELLOPEN);

	// Delete this shot from existance
	if (sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_ROF_OPEN1, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->tankfrom = sh[shot].tankfrom;
	s->radius = projectiles[sh[shot].projectile].expradius;
	s->rotatex = 67;
	s->rotatey = 54;
}

void	SB_Explode(int shot)
{
	sprite_t*		s;
	int				spriteflags = (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE);
	
	SetTankTimer(2*TICRATE);

	// Start a sound
	//sh[shot].StartSound(BHB_SHELLOPEN);

	// Delete this shot from existance
	if (sh[shot].sprite) { sh[shot].sprite->Kill(); }
	sh[shot].exist = 0;

	s = G_SpawnSprite(sh[shot].x, sh[shot].y, S_SB_SILVERBULLET, spriteflags);
	s->damageh = projectiles[sh[shot].projectile].damageh;
	s->damagee = projectiles[sh[shot].projectile].damagee;
	s->damageb = projectiles[sh[shot].projectile].damageb;
	s->tankfrom = sh[shot].tankfrom;
	s->angle = sh[shot].sprite->angle;
}

void Moonshot_Thinker(int shot)
{
	int		laserpower = ConvertFromGamePower(450);
	int		tank = sh[shot].tankfrom;
	int		fireangle = sh[shot].origdeg;

	if (sh[shot].shottics == 1)
	{
		sh[shot].xvel = (-laserpower*COS(fireangle));
		sh[shot].yvel = (laserpower*-SIN(fireangle));
	}
}

//
// numshots - Number of active shots in the air?
//

int		numshots(void)
{
	int		numshots=0;
	int		i;
	for (i=0; i<N_SHOTS; i++)
	{
		if (sh[i].exist > 0)
			numshots++;
	}

	return numshots;
}

int findshot()
{
	int i;
	for (i=0;i<N_SHOTS;i++) 
	{
		if (sh[i].exist <= 0) 
			return i;
	}

	return(-1);
}


bool	shotinair(void)
{
	int		i;

	for(i=0; i<N_SHOTS; i++)
	{
		if (sh[i].exist > 0)
			return true;
	}

	return false;	// We're clear
}

//
// ShotThinker - Run logic for all shots in the air
//

void	ShotThinker(void)
{
	int		i;
	
	for (i=0;i<N_SHOTS;i++) 
	{
		if (sh[i].exist > 0)
		{
			sh[i].Thinker();
		}
	}
	//	Do a test to see if shots can be made faster without screwingup anything else!!
	//	speed up shot flight double (see cheats F3)
	if(fastshot > 0)
		for (i=0;i<N_SHOTS;i++) 
			{
			if(sh[i].exist > 0)
				{
				sh[i].Thinker();
				}
			}
	//	speed up shot flight triple
	if(fastshot > 1)
		for (i=0;i<N_SHOTS;i++) 
			{
			if(sh[i].exist > 0)
				{
				sh[i].Thinker();
				}
			}
	//	speed up shot flight quad
	if(fastshot > 2)
		for (i=0;i<N_SHOTS;i++) 
			{
			if(sh[i].exist > 0)
				{
				sh[i].Thinker();
				}
			}
}

void	BCast_Shot(int s)
{
	if(!multiplayer || multiplayermode != MP_SERVER)
		return;

	// General shot info
	BCast_WriteByte(STOC_UPDATESHOTS);
	BCast_WriteByte(s);
	BCast_WriteByte(sh[s].projectile);
	BCast_WriteShort(sh[s].tankfrom);

	// Position and velocity
	BCast_WriteFloat(sh[s].spawndeg);
	BCast_WriteShort(sh[s].spawnx);
	BCast_WriteShort(sh[s].spawny);
	BCast_WriteFloat(sh[s].spawnxvel);
	BCast_WriteFloat(sh[s].spawnyvel);
}

static bool	TowerHit = false;

void	ShotLength_Proc(BITMAP *b, int x, int y, int color)	//	BITMAP *b, int color	are not used
{
//	detect if there is a TOWERpod in the line of fire.
//	if so the laser stops here


	if(TowerHit)return;			//	all ready found a hit point.

	if(OnScreen(buffer, x, y) == -1)	//	sprites don't live off screen
		{
		//	off screen is a hit point
		lasercontactx = x;
		lasercontacty = y;
		TowerHit = true;
		return;
		}

	//	The linked list of all sprites 
	sprite_t*	s = sprites;
	int		sx;
	int		sy;
	int		sh;
	int		sw = 2;	//	we only want the centre section as a collision area

	//	Now clip against any sprites that are TowerPod
	//	and are shootable. (Do we need shootable)
	while (s)
		{
		if(s->wobj != WO_TOWERPODCONSTRUCTION)
			{
			s = s->next;
			continue;
			}
		if(!(s->flags & MF_SHOOTABLE))
			{
			s = s->next;	//	a non-shootable tower ?? Why ??
			continue;			//	should we assume that all towers block laser
			}	
		// , see if we're inside the bounding box
		sx = s->x+(s->width/2);	//	centre of sprite
		sy = s->y;
		sh = s->height;

		if(x >= (sx) &&
			 x <= (sx + sw ) &&
			 y >= (sy) &&
			 y <= (sy + sh) )
			{
			lasercontactx = x;
			lasercontacty = y;
			TowerHit = true;
			}
		s = s->next;
		}
}

int ShotLength(float x, float y, int fireangle)
{
///	char	msg[128]; 
	int	len=0;

	//
	//	ShotLength_Proc sets lasercontact to the hit point
	//	if no hit point then nothing is done
	//	so they are set to max length to start with. (ie diagonal)
	//	
	lasercontactx = x-COS(fireangle)*(SCREENWIDTH+200);
	lasercontacty = y-SIN(fireangle)*(SCREENWIDTH+200);
	TowerHit = false;

	do_line(buffer, (int)x, (int)y, lasercontactx, lasercontacty, 0, ShotLength_Proc);
/// sprintf(msg, "len=%d [%.1f,%.1f] LX=%d LY=%d", len, x, y, lasercontactx, lasercontacty); G_Printf(msg);

	//if(TowerHit)	//	the 'found it' flag is set
	//	{ // don't need to do anything here		}

	lasercontactx -= x;
	lasercontacty -= y;
	len = sqrt(lasercontactx*lasercontactx + lasercontacty*lasercontacty);
/// sprintf(msg, "len=%d [%.1f,%.1f] LX=%d LY=%d", len, x, y, lasercontactx, lasercontacty); G_Printf(msg);
	return len;
}

///////////////////////////////////////	end of file	//////////////////////////////


