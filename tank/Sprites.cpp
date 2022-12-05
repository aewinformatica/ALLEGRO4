#include <stdio.h>
#include <string.h>
#include <math.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"
#include "fblend.h"

#define		HEATWAVE_SPEED		2

int				numsprites;
int				numwobjsprites;

DATAFILE	*alphadata;
sprite_t  *sprites;

sprite_t	*firstsprite;
sprite_t	*lastsprite;

void A_ContactSpriteDamage(sprite_t* s, int tics, bool usebb=false);
// weapon stuff defined in butstuff.cpp
void	Calc_All_Lists(void);
extern menuitem_t		weapons_items[NUM_PROJECTILES];
extern int				num_defense_items;
extern menuitem_t		defense_items[NUM_DEFENSE];
extern int				num_weapons_items;
extern menuitem_t		special_items[NUM_SPECIALS];
extern int				num_special_items;
// end weapon stuff

wobject_t	wobjects[NUM_WOBJECTS] = {

	{
		"Tower Pod Wall",			// Name
		50, 145,					// Width / Height
		300,						// Starting health
		1,1,5,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_TPGRAVITY|
		 MF_SHOOTABLE),
		S_TOWERPOD_BUILD1,			// Spawn state
		S_NULL,						      // Attack state
		S_TOWERPOD_BUILD6,			// Damage state
		S_TOWERPOD_DIE1,			  // Death state
		true,						        // Use in level editor?
		S_TOWERPOD_BUILD9,			// Editor Icon 0
	},

	{
		"Missile Turret",			// Name
		50, 145,					// Width / Height
		400,						// Starting health
		1,1,6,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_TPGRAVITY|
		 MF_SHOOTABLE|
		 MF_POSTTURNATTACK),
		S_MISSILETURRET_BUILD1,		// Spawn state
		S_MISSILETURRET_ATTACK1,	// Attack state
		S_MISSILETURRET_BUILD6,		// Damage state
		S_MISSILETURRET_DIE1,		// Death state
		true,						// Use in level editor?
		S_MISSILETURRET_BUILD11,			// Editor Icon 1
	},

	{
		"Fire Sentry Pod",			// Name
		30, 30,						// Width / Height
		75,							// Starting health
		7,1,1,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_GROUNDCLIP|
		 MF_SHOOTABLE|
		 MF_POSTTURNATTACK),
		S_HSENTRY2,					// Spawn state
		S_HSENTRY_ATTACK1,			// Attack state
		S_HSENTRY5,					// Damage state
		S_HSENTRY_DIE1,				// Death state
		true,						// Use in level editor?
		S_HSENTRY2,			// Editor Icon 2
	},

	{
		"Energy Sentry Pod",			// Name
		30, 30,						// Width / Height
		75,							// Starting health
		1,7,1,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_GROUNDCLIP|
		 MF_SHOOTABLE|
		 MF_POSTTURNATTACK),
		S_ESENTRY2,					// Spawn state
		S_ESENTRY_ATTACK1,			// Attack state
		S_ESENTRY5,					// Damage state
		S_ESENTRY_DIE1,				// Death state
		true,						// Use in level editor?
		S_ESENTRY2,			// Editor Icon 3
	},

	{
		"Ballistic Sentry Pod",		// Name
		30, 30,						// Width / Height
		75,							// Starting health
		1,1,7,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_GROUNDCLIP|
		 MF_SHOOTABLE|
		 MF_POSTTURNATTACK),
		S_BSENTRY2,					// Spawn state
		S_BSENTRY_ATTACK1,			// Attack state
		S_BSENTRY5,					// Damage state
		S_BSENTRY_DIE1,				// Death state
		true,						// Use in level editor?
		S_BSENTRY2,			// Editor Icon 4
	},

	{
		"Lava Rock Spawner",		// Name
		10, 10,						// Width / Height
		100,						// Starting health
		0,0,0,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_GROUNDCLIP|
		 MF_POSTTURNATTACK|
		 MF_NOTVISABLE),
		S_LAVASPAWNER,				// Spawn state
		S_LAVASPAWNER_ATTACK,		// Attack state
		S_NULL,						// Damage state
		S_NULL,						// Death state
		true,						// Use in level editor?
		S_LAVASPAWNER,			// Editor Icon 5
	},

	{
		"Environmental Snow",		// Name
		7, 7,						// Width / Height
		100,						// Starting health
		0,0,0,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_NOTVISABLE),
		S_SNOWFLAKE_THINKER,				// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_NULL,						// Death state
		true,						// Use in level editor?
		S_SNOWFLAKE_THINKER,			// Editor Icon 6
	},

	{
		"Cave Spike 1",				// Name
		36, 158,					// Width / Height
		25,							// Starting health
		0,0,0,						// H E B stats
		0,0,225,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_CAVESPIKE1_IDLE,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_CAVESPIKE1_SHAKE1,		// Death state
		true,						// Use in level editor?
		S_CAVESPIKE1_IDLE,			// Editor Icon 7
	},

	{
		"Cave Spike 2",				// Name
		68, 141,					// Width / Height
		25,							// Starting health
		0,0,0,						// H E B stats
		0,0,225,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_CAVESPIKE2_IDLE,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_CAVESPIKE2_SHAKE1,		// Death state
		true,						// Use in level editor?
		S_CAVESPIKE2_IDLE,			// Editor Icon 8
	},

	{
		"Cave Spike 3",				// Name
		35, 72,						// Width / Height
		25,							// Starting health
		0,0,0,						// H E B stats
		0,0,225,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_CAVESPIKE3_IDLE,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_CAVESPIKE3_SHAKE1,		// Death state
		true,						// Use in level editor?
		S_CAVESPIKE3_IDLE,			// Editor Icon 9
	},

	{
		"Cave Spike 4",				// Name
		68, 125,					// Width / Height
		25,							// Starting health
		0,0,0,						// H E B stats
		0,0,225,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_CAVESPIKE4_IDLE,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_CAVESPIKE4_SHAKE1,		// Death state
		true,						// Use in level editor?
		S_CAVESPIKE4_IDLE,			// Editor Icon 10
	},

	{
		"Cave Spike 5",				// Name
		56, 68,						// Width / Height
		25,							// Starting health
		0,0,0,						// H E B stats
		0,0,225,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_CAVESPIKE5_IDLE,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_CAVESPIKE5_SHAKE1,		// Death state
		true,						// Use in level editor?
		S_CAVESPIKE5_IDLE,			// Editor Icon 11
	},

	{
		"Cherry Blossom Spawner",	// Name
		10, 10,						// Width / Height
		100,						// Starting health
		0,0,0,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_NOGRAVITY|
		 MF_CENTERCOORD|
		 MF_NOTVISABLE),
		S_CHERRYBLOSS_SPAWNER,		// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_NULL,						// Death state
		true,						// Use in level editor?
		S_CHERRYBLOSS_SPAWNER,			// Editor Icon 12
	},

	{
		"Raindrop Spawner",	// Name
		10, 10,						// Width / Height
		100,						// Starting health
		0,0,0,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_NOGRAVITY|
		 MF_CENTERCOORD|
		 MF_NOTVISABLE|
		 MF_POSTTURNATTACK),
		S_RAINDROP_SPAWNER,		// Spawn state
		S_RAINDROP_ATTACK,		// Attack state
		S_NULL,					// Damage state
		S_NULL,					// Death state
		true,					// Use in level editor?
		S_RAINDROP_SPAWNER,			// Editor Icon 13
	},

	{
		"Lava Spawner",	// Name
		10, 10,						// Width / Height
		100,						// Starting health
		0,0,0,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_NOGRAVITY|
		 MF_CENTERCOORD|
		 MF_NOTVISABLE|
		 MF_POSTTURNATTACK),
		S_LAVASHOOTER,				// Spawn state
		S_LAVASHOOTER_ATTACK1,		// Attack state
		S_NULL,						// Damage state
		S_NULL,						// Death state
		true,						// Use in level editor?
		S_LAVASHOOTER,			// Editor Icon 14
	},

	{
		"Heat Corona Spawner",		// Name
		10, 10,						// Width / Height
		100,						// Starting health
		0,0,0,						// H E B stats
		0,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_NOGRAVITY|
		 MF_CENTERCOORD|
		 MF_NOTVISABLE),
		S_HEATCORONA_SPAWNER,		// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_NULL,						// Death state
		true,						// Use in level editor?
		S_HEATCORONA_SPAWNER,			// Editor Icon 15
	},

	{
		"Lava Floor Damage",		// Name
		10, 10,						// Width / Height
		100,						// Starting health
		0,0,0,						// H E B stats
		50,0,0,						// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_NOGRAVITY|
		 MF_CENTERCOORD|
		 MF_NOTVISABLE|
		 MF_POSTTURNATTACK),
		S_LAVAFLOORDAMAGE_IDLE,		// Spawn state
		S_LAVAFLOORDAMAGE,			// Attack state
		S_NULL,						// Damage state
		S_NULL,						// Death state
		true,							// Use in level editor?
		S_HEATRAY_F1,			// Editor Icon	16
	},

	{
		"Ice Spike 1",				// Name
		61, 181,					// Width / Height
		30,							// Starting health
		0,0,0,						// H E B stats
		0,0,250,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_ICESPIKE1_IDLE,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_ICESPIKE1_SHAKE1,		// Death state
		true,						// Use in level editor?
		S_ICESPIKE1_IDLE,			// Editor Icon
	},

	{
		"Ice Spike 2",				// Name
		83, 138,					// Width / Height
		30,							// Starting health
		0,0,0,						// H E B stats
		0,0,250,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_ICESPIKE2_IDLE,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_ICESPIKE2_SHAKE1,		// Death state
		true,						// Use in level editor?
		S_ICESPIKE2_IDLE,			// Editor Icon
	},

	{
		"Ice Spike 3",				// Name
		24, 123,					// Width / Height
		30,							// Starting health
		0,0,0,						// H E B stats
		0,0,250,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_ICESPIKE3_IDLE,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_ICESPIKE3_SHAKE1,			// Death state
		true,						// Use in level editor?
		S_ICESPIKE3_IDLE,			// Editor Icon
	},

	{												//	WO_FUELCYLINDER
		"Fuel Cylinder",			// Name
		21, 41,						// Width / Height
		50,							// Starting health
		0,0,0,						// H E B stats
		0,0,250,					// H E B damage
		(MF_NORMAL|					// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_FUELCYLINDER,				// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_FUELCYLINDER_DIE,			// Death state
		true,						// Use in level editor?
		S_FUELCYLINDER,			// Editor Icon
	},

	{										//	WO_TREASURECHEST
		"Treasure Chest",			// Name
		16, 17,						// Width / Height
		250,							// Starting health
		0,0,0,						// H E B stats
		10,10,10,					// H E B damage
		(MF_NORMAL|				// Sprite Flags
		 MF_SHOOTABLE|
		 MF_CENTERCOORD|
		 MF_NOGRAVITY),
		S_TREASURECHEST,	// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_TREASUREOPEN,		// Death state
		true,							// Use in level editor?
		S_TREASURECHEST,	// Editor Icon
	},

	{										//	WO_MAGGEN
		"Magnetic Generator",			// Name
		18, 33,						// Width / Height
		250,							// Starting health
		0,0,0,						// H E B stats
		10,10,10,					// H E B damage
		(MF_NORMAL|				// Sprite Flags
		 MF_GROUNDCLIP|
		 MF_SHOOTABLE|
		 MF_CENTERCOORD),
		S_MAGCLOSED,			// Spawn state
		S_NULL,						// Attack state
		S_NULL,						// Damage state
		S_NULL,						// Death state
		true,							// Use in level editor?
		S_MAGPULSE1,			// Editor Icon
	},

};

int Sentry_ChooseTarget(sprite_t* s);

void A_Kill(sprite_t	*s);
void A_ResetLife(sprite_t *s);
void A_SmokeThinkerXLarge(sprite_t*	s);
void A_SmallBubbleThinker(sprite_t	*s);
void A_MediumBubbleThinker(sprite_t	*s);
void A_LargeBubbleThinker(sprite_t	*s);
void A_SpawnLargeFlame(sprite_t	*s);

void A_SpawnLeftMediumFlame(sprite_t *s);
void A_SpawnLeftSmallFlame(sprite_t *s);
void A_SpawnLeftVerySmallFlame(sprite_t *s);
void A_FinishLeftHeatWave(sprite_t *s);

void A_SpawnRightMediumFlame(sprite_t *s);
void A_SpawnRightSmallFlame(sprite_t *s);
void A_SpawnRightVerySmallFlame(sprite_t *s);
void A_FinishRightHeatWave(sprite_t *s);

void A_RotateMissile(sprite_t *s);
void A_RotateMissileFast(sprite_t *s);
void A_HeatwaveSound(sprite_t* s);
void A_ElectricArcThinker(sprite_t* s);
void A_ContactTankDamage(sprite_t *s);
void A_FlameThrowerThinker(sprite_t *s);
void A_FlameThinker(sprite_t*	s);
void A_LaserPulseCheckTanks(sprite_t*	s);
void A_GreenLaser(sprite_t* s);
void A_IonChargerThinker(sprite_t* s);
void A_LightingRod_Thinker(sprite_t*	s);
void A_EMPExplodeThinker(sprite_t*	s);
void A_FireStarter_Flame_Burn(sprite_t*	s);
void A_TowerBuild(sprite_t* s);
void A_TowerDie(sprite_t* s);
void A_HSentry_Attack(sprite_t*	s);
void A_SentryOpen(sprite_t* s);
void A_SentryDeath(sprite_t*	s);
void A_HSentryShot_Thinker(sprite_t*	s);
void A_ESentry_Attack(sprite_t*	s);
void A_BSentry_Attack(sprite_t*	s);
void A_ShellThinker(sprite_t* s);
void A_MissileTurretAttack(sprite_t *s);
void A_FollowParent(sprite_t*	s);
void A_LavaSpawnerThinker(sprite_t*	s);
void A_LavaSpawnerSetup(sprite_t* s);
void A_SmallSnowFlakeThinker(sprite_t *s);
void A_MediumSnowFlakeThinker(sprite_t *s);
void A_LargeSnowFlakeThinker(sprite_t *s);
void A_SnowFlakeSpawner(sprite_t* s);
void A_HeatrayThinker(sprite_t* s);
void A_HeatrayFlameThinker(sprite_t* s);
void A_HeatrayEffect(sprite_t* s);
void A_RailGunSmoke(sprite_t* s);
void A_FireFlyEffect(sprite_t *s);
void A_FireFlyDieThinker(sprite_t *s);
void A_SpikeDie(sprite_t *s);
void A_SpikeFall(sprite_t *s);
void A_SpikeShake(sprite_t *s);
void A_ShardThinker(sprite_t *s);
void A_SpikeShakeInit(sprite_t*		s);
void A_FollowTank(sprite_t *s);
void A_CherryBlossThinker(sprite_t* s);
void A_CherrySpawnerThinker(sprite_t* s);
void A_CherryBlossFinish(sprite_t	*s);
void A_RainDropSpawnerThinker(sprite_t *s);
void A_RainDropThinker(sprite_t *s);
void A_RainDropSpawnerAttack(sprite_t* s);
void A_LavaShooterThinker(sprite_t* s);
void A_LavaShootAttack(sprite_t* s);
void A_HeatCoronaThinker(sprite_t* s);
void A_HeatCoronaSpawnerThinker(sprite_t* s);
void A_LavaFloorDamage(sprite_t*	s);
void A_EMPBlastSteal(sprite_t* s);
void A_TurnArrowThinker(sprite_t*	s);
void A_AlienBeamUpThinker(sprite_t*	s);
void A_PolarisMistThinkier(sprite_t* s);
void A_NitrogenThinker(sprite_t* s);
void A_PolarisEMPExplode(sprite_t*	s);
void A_IceShardThinker(sprite_t *s);
void A_BHBomb_Open(sprite_t	*s);
void A_BH_Thinker(sprite_t	*s);
void A_BH_CloudThinker1(sprite_t	*s);
void A_BH_CloudThinker2(sprite_t	*s);
void A_BH_ImplodeWave(sprite_t*		s);
void A_BH_MatterFall(sprite_t*		s);
void A_TEL_PositiveThinker(sprite_t* s);
void A_TEL_NegativeThinker(sprite_t* s);
void A_AirShipThinker(sprite_t*		s);
void A_ASFlareThinker(sprite_t*		s);
void A_ASFlareSmoke(sprite_t*		s);
void A_IB_Thinker(sprite_t*			s);
void A_AS_Move(sprite_t*			s);
void A_HeatCannonThinker(sprite_t*	s);
void A_AS_TankBayThinker(sprite_t*	s);
void A_AS_TankBayDeploy(sprite_t*	s);
void A_AS_FlyerThinker(sprite_t*	s);
void A_FireToad_Thinker(sprite_t*	s);
void A_TankFlareThinker(sprite_t*	s);
void A_NIVBuildSound(sprite_t*		s);
void A_NIVCompleteSound(sprite_t*	s);
void A_FunkyBombThinker(sprite_t*	s);
void A_FunkyNukeThinker(sprite_t*	s);
//void A_OBC_MainThinker(sprite_t*	s);
void A_OBC_BeaconThinker(sprite_t*	s);
void A_OBC_ThinBeamDown(sprite_t*	s);
void A_OBCBigBeamThinker(sprite_t*	s);
void A_OBCMedBeamThinker(sprite_t*	s);
void A_OBCIonFlameThinker(sprite_t*	s);
void A_OBCBeamFinish(sprite_t*		s);
void A_SatRev(sprite_t*				s);
void A_SatFire(sprite_t*			s);
void A_IceSpikeShakeInit(sprite_t*	s);
void A_IceSpikeDie(sprite_t *		s);
void A_IceSpikeShake(sprite_t *		s);
void A_IceSpikeFall(sprite_t *		s);
void A_ROF_Open1Thinker(sprite_t*	s);
void A_ROF_Open2Thinker(sprite_t*	s);
void A_ROF_Open3Thinker(sprite_t*	s);
void A_ROF_Open4Thinker(sprite_t*	s);
void A_ROF_FireThinker(sprite_t*	s);
void A_FullMoonThinker(sprite_t*	s);
void A_SilverBulletThinker(sprite_t* s);
void A_FuelDie(sprite_t*			s);
void A_ChestOpen(sprite_t*			s);
void A_ChestGold(sprite_t* s);
void A_MagPuff(sprite_t* s);

state_t states[NUM_STATES] = {

// Begin the declaration of the sprite states here
// 	1st state/Next state/sprite function/allegro_BMPnum/flags/tics
   { S_NULL,  			S_NULL,  			NULL, TEST_TANK, 1 },
   { S_EXPLODE1_1,  S_EXPLODE1_2,	NULL, EXPLODE1_1, HAS_ALPHA, 5 },
   { S_EXPLODE1_2,  S_EXPLODE1_3,	NULL, EXPLODE1_2, HAS_ALPHA, 5 },
   { S_EXPLODE1_3,  S_EXPLODE1_4,	NULL, EXPLODE1_3, HAS_ALPHA, 5 },
   { S_EXPLODE1_4,  S_EXPLODE1_5,	NULL, EXPLODE1_4, HAS_ALPHA, 5 },
   { S_EXPLODE1_5,  S_EXPLODE1_6,	NULL, EXPLODE1_5, HAS_ALPHA, 5 },
   { S_EXPLODE1_6,  S_EXPLODE1_7,	NULL, EXPLODE1_6, HAS_ALPHA, 5 },
   { S_EXPLODE1_7,  S_EXPLODE1_8,	NULL, EXPLODE1_7, HAS_ALPHA, 15 },
   { S_EXPLODE1_8,  S_NULL,  		A_Kill, EXPLODE1_7, HAS_ALPHA, 5 },

   // Projectile: Baby Missile
   { S_BABYMISSILE, S_BABYMISSILE, NULL, P_BABYMISSILE,   NORMAL_STATE, 1 },

   // Projectile: Missile
   { S_MISSILE_1, S_MISSILE_2,	NULL, P_MISSILE_1,	  NORMAL_STATE, 1 },
   { S_MISSILE_2, S_MISSILE_1,	NULL, P_MISSILE_2,	  NORMAL_STATE, 1 },

   // Projectile: Blue Missile
   { S_BLUEMISSILE_1, S_BLUEMISSILE_2,	NULL, P_BLUEMISSILE_1,	  NORMAL_STATE, 1 },
   { S_BLUEMISSILE_2, S_BLUEMISSILE_1,	NULL, P_BLUEMISSILE_2,	  NORMAL_STATE, 1 },

   // Projectile: Black Missile
   { S_BLACKMISSILE_1, S_BLACKMISSILE_2, NULL, P_BLACKMISSILE_1,  NORMAL_STATE, 1 },
   { S_BLACKMISSILE_2, S_BLACKMISSILE_1, NULL, P_BLACKMISSILE_2,  NORMAL_STATE, 1 },

   // Projectile: Iron Ball
   { S_IRONBALL,  S_IRONBALL,	 NULL,  P_IRONBALL,	  NORMAL_STATE, 1 },

   // Projectile: Fireball
   { S_FIREBALL,  S_FIREBALL, A_RotateMissileFast, P_FIREBALL, NORMAL_STATE, 1 },

   // Projectile: Black Missile
   { S_FATBOY_1, S_FATBOY_2, NULL, P_FATBOY_1,  NORMAL_STATE, 1 },
   { S_FATBOY_2, S_FATBOY_1, NULL, P_FATBOY_2,  NORMAL_STATE, 1 },

   // Water splashes
   { S_SPLASH1,	 S_SPLASH2,		NULL,	WATER_SPLASH1,  NORMAL_STATE, 7 },
   { S_SPLASH2,	 S_SPLASH3,		NULL,	WATER_SPLASH2,  NORMAL_STATE, 7 },
   { S_SPLASH3,	 S_SPLASH4,		NULL,	WATER_SPLASH3,  NORMAL_STATE, 7 },
   { S_SPLASH4,	 S_SPLASH1,		A_Kill, WATER_SPLASH4,  NORMAL_STATE, 7 },

   // Bubbles
   { S_BUBBLE_LARGE,	 S_BUBBLE_LARGE,		A_LargeBubbleThinker,	BUBBLE_LARGE,  NORMAL_STATE, 1 },
   { S_BUBBLE_MEDIUM,	 S_BUBBLE_MEDIUM,		A_MediumBubbleThinker,BUBBLE_MEDIUM, NORMAL_STATE, 1 },
   { S_BUBBLE_SMALL,	 S_BUBBLE_SMALL,		A_SmallBubbleThinker,	BUBBLE_SMALL,  NORMAL_STATE, 1 },

   // Smoke effect (gray -or- grey)
   { S_SMOKE_GRAY_XLARGE,	 S_SMOKE_GRAY_XLARGE,		A_SmokeThinkerXLarge,		SMOKE_GRAY_XLARGE,	NORMAL_STATE, 1 },
   { S_SMOKE_GRAY_LARGE,	 S_SMOKE_GRAY_LARGE,		A_SmokeThinkerXLarge,		SMOKE_GRAY_LARGE,	NORMAL_STATE, 1 },
   { S_SMOKE_GRAY_MEDIUM,	 S_SMOKE_GRAY_MEDIUM,		A_SmokeThinkerXLarge,		SMOKE_GRAY_MEDIUM,	NORMAL_STATE, 1 },
   { S_SMOKE_GRAY_SMALL,	 S_SMOKE_GRAY_SMALL,		A_SmokeThinkerXLarge,		SMOKE_GRAY_SMALL,	NORMAL_STATE, 1 },

   // More smoke effect (black)
   { S_SMOKE_BLACK_XLARGE,	 S_SMOKE_BLACK_XLARGE,		A_SmokeThinkerXLarge,					SMOKE_BLACK_XLARGE,	NORMAL_STATE, 1 },
   { S_SMOKE_BLACK_LARGE,	 S_SMOKE_BLACK_LARGE,		A_SmokeThinkerXLarge,					SMOKE_BLACK_LARGE,	NORMAL_STATE, 1 },
   { S_SMOKE_BLACK_MEDIUM,	 S_SMOKE_BLACK_MEDIUM,		A_SmokeThinkerXLarge,					SMOKE_BLACK_MEDIUM,	NORMAL_STATE, 1 },
   { S_SMOKE_BLACK_SMALL,	 S_SMOKE_BLACK_SMALL,		A_SmokeThinkerXLarge,					SMOKE_BLACK_SMALL,	NORMAL_STATE, 1 },

   // Electic Shield
   { S_SHIELD_ELECTRIC_1,	 S_SHIELD_ELECTRIC_2,		NULL,			SHIELD_ELECTRIC_1,	NORMAL_STATE, 2*TICRATE },
   { S_SHIELD_ELECTRIC_2,	 S_SHIELD_ELECTRIC_3,		NULL,			SHIELD_ELECTRIC_2,	NORMAL_STATE, 5 },
   { S_SHIELD_ELECTRIC_3,	 S_SHIELD_ELECTRIC_4,		NULL,			SHIELD_ELECTRIC_3,	NORMAL_STATE, 5 },
   { S_SHIELD_ELECTRIC_4,	 S_SHIELD_ELECTRIC_5,		NULL,			SHIELD_ELECTRIC_4,	NORMAL_STATE, 5 },
   { S_SHIELD_ELECTRIC_5,	 S_SHIELD_ELECTRIC_6,		NULL,			SHIELD_ELECTRIC_5,	NORMAL_STATE, 5 },
   { S_SHIELD_ELECTRIC_6,	 S_SHIELD_ELECTRIC_1,		NULL,			SHIELD_ELECTRIC_6,	NORMAL_STATE, 5 },

   // Flame Shield
   { S_SHIELD_FIRE_1,	 S_SHIELD_FIRE_2,				NULL,			SHIELD_FIRE_1,		NORMAL_STATE, 2*TICRATE },
   { S_SHIELD_FIRE_2,	 S_SHIELD_FIRE_3,				NULL,			SHIELD_FIRE_2,		NORMAL_STATE, 5 },
   { S_SHIELD_FIRE_3,	 S_SHIELD_FIRE_4,				NULL,			SHIELD_FIRE_3,		NORMAL_STATE, 5 },
   { S_SHIELD_FIRE_4,	 S_SHIELD_FIRE_5,				NULL,			SHIELD_FIRE_4,		NORMAL_STATE, 5 },
   { S_SHIELD_FIRE_5,	 S_SHIELD_FIRE_1,				NULL,			SHIELD_FIRE_5,		NORMAL_STATE, 5 },

   // Ballistic Shield
   { S_SHIELD_BALLISTIC_1,	 S_SHIELD_BALLISTIC_2,		NULL,			SHIELD_BALLISTIC_1,	NORMAL_STATE, 3*TICRATE },
   { S_SHIELD_BALLISTIC_2,	 S_SHIELD_BALLISTIC_3,		NULL,			SHIELD_BALLISTIC_2,	NORMAL_STATE, 5 },
   { S_SHIELD_BALLISTIC_3,	 S_SHIELD_BALLISTIC_4,		NULL,			SHIELD_BALLISTIC_3,	NORMAL_STATE, 5 },
   { S_SHIELD_BALLISTIC_4,	 S_SHIELD_BALLISTIC_5,		NULL,			SHIELD_BALLISTIC_4,	NORMAL_STATE, 5 },
   { S_SHIELD_BALLISTIC_5,	 S_SHIELD_BALLISTIC_6,		NULL,			SHIELD_BALLISTIC_5,	NORMAL_STATE, 5 },
   { S_SHIELD_BALLISTIC_6,	 S_SHIELD_BALLISTIC_1,		NULL,			SHIELD_BALLISTIC_6,	NORMAL_STATE, 5 },

   // Heatwave missile & flame effects
   { S_HEATWAVE,			 S_HEATWAVE,				A_RotateMissile,			P_HEATWAVE,			NORMAL_STATE, 1 },

   // Xtra-Large flame
   { S_HEATWAVE_VERYSMALL1_1,	 S_HEATWAVE_SMALL1_1,			NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_SMALL1_1,		 S_HEATWAVE_MEDIUM1_1,			NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_MEDIUM1_1,		 S_HEATWAVE_LARGE1_1,			NULL,			HEATWAVE_MEDIUM,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LARGE1_1,		 S_HEATWAVE_XLARGE1_1,			NULL,			HEATWAVE_LARGE,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_XLARGE1_1,		 S_HEATWAVE_LARGE1_2,			NULL,			HEATWAVE_XLARGE,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LARGE1_2,		 S_HEATWAVE_MEDIUM1_2,			A_SpawnLargeFlame,			HEATWAVE_LARGE,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_MEDIUM1_2,		 S_HEATWAVE_SMALL1_2,			NULL,			HEATWAVE_MEDIUM,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_SMALL1_2,		 S_HEATWAVE_VERYSMALL1_2,		NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_VERYSMALL1_2,	 S_HEATWAVE_VERYSMALL1_3,		NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_VERYSMALL1_3,	 S_HEATWAVE_SMALL1_1,			A_Kill,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Large left flame
   { S_HEATWAVE_LEFT_VERYSMALL2_1,	 S_HEATWAVE_LEFT_SMALL2_1,			NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_SMALL2_1,		 S_HEATWAVE_LEFT_MEDIUM2_1,			NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_MEDIUM2_1,		 S_HEATWAVE_LEFT_LARGE2_1,			NULL,			HEATWAVE_MEDIUM,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_LARGE2_1,		 S_HEATWAVE_LEFT_MEDIUM2_2,			NULL,			HEATWAVE_LARGE,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_MEDIUM2_2,		 S_HEATWAVE_LEFT_SMALL2_2,			NULL,			HEATWAVE_MEDIUM,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_SMALL2_2,		 S_HEATWAVE_LEFT_VERYSMALL2_2,		A_SpawnLeftMediumFlame,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_VERYSMALL2_2,	 S_HEATWAVE_LEFT_VERYSMALL2_3,		NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_VERYSMALL2_3,	 S_HEATWAVE_LEFT_SMALL2_1,			A_Kill,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Medium left flame
   { S_HEATWAVE_LEFT_VERYSMALL3_1,	 S_HEATWAVE_LEFT_SMALL3_1,			NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_SMALL3_1,		 S_HEATWAVE_LEFT_MEDIUM3_1,			NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_MEDIUM3_1,		 S_HEATWAVE_LEFT_SMALL3_2,			NULL,			HEATWAVE_MEDIUM,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_SMALL3_2,		 S_HEATWAVE_LEFT_VERYSMALL3_2,		NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_VERYSMALL3_2,	 S_HEATWAVE_LEFT_VERYSMALL3_3,		A_SpawnLeftSmallFlame,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_VERYSMALL3_3,	 S_HEATWAVE_LEFT_SMALL3_1,			A_Kill,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Small left flame
   { S_HEATWAVE_LEFT_VERYSMALL4_1,	 S_HEATWAVE_LEFT_SMALL4_1,			NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_SMALL4_1,		 S_HEATWAVE_LEFT_VERYSMALL4_2,		NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_VERYSMALL4_2,	 S_HEATWAVE_LEFT_VERYSMALL4_3,		A_SpawnLeftVerySmallFlame,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_VERYSMALL4_3,	 S_HEATWAVE_LEFT_SMALL4_1,			A_Kill,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Very small left flame
   { S_HEATWAVE_LEFT_VERYSMALL5_1,	 S_HEATWAVE_LEFT_VERYSMALL5_2,		NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_LEFT_VERYSMALL5_2,	 S_HEATWAVE_LEFT_VERYSMALL5_1,		A_FinishLeftHeatWave,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Large right flame
   { S_HEATWAVE_RIGHT_VERYSMALL2_1,	 S_HEATWAVE_RIGHT_SMALL2_1,		NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_SMALL2_1,		 S_HEATWAVE_RIGHT_MEDIUM2_1,		NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_MEDIUM2_1,	 S_HEATWAVE_RIGHT_LARGE2_1,			NULL,			HEATWAVE_MEDIUM,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_LARGE2_1,		 S_HEATWAVE_RIGHT_MEDIUM2_2,		NULL,			HEATWAVE_LARGE,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_MEDIUM2_2,	 S_HEATWAVE_RIGHT_SMALL2_2,			NULL,			HEATWAVE_MEDIUM,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_SMALL2_2,		 S_HEATWAVE_RIGHT_VERYSMALL2_2,		A_SpawnRightMediumFlame,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_VERYSMALL2_2,	 S_HEATWAVE_RIGHT_VERYSMALL2_3,	NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_VERYSMALL2_3,	 S_HEATWAVE_RIGHT_SMALL2_1,			A_Kill,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Medium right flame
   { S_HEATWAVE_RIGHT_VERYSMALL3_1,	S_HEATWAVE_RIGHT_SMALL3_1,		NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_SMALL3_1,		 	S_HEATWAVE_RIGHT_MEDIUM3_1,		NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_MEDIUM3_1,	 	S_HEATWAVE_RIGHT_SMALL3_2,		NULL,			HEATWAVE_MEDIUM,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_SMALL3_2,		 	S_HEATWAVE_RIGHT_VERYSMALL3_2,NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_VERYSMALL3_2,	S_HEATWAVE_RIGHT_VERYSMALL3_3,A_SpawnRightSmallFlame,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_VERYSMALL3_3,	S_HEATWAVE_RIGHT_SMALL3_1,		A_Kill,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Small right flame
   { S_HEATWAVE_RIGHT_VERYSMALL4_1,	 S_HEATWAVE_RIGHT_SMALL4_1,			NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_SMALL4_1,		 S_HEATWAVE_RIGHT_VERYSMALL4_2,		NULL,			HEATWAVE_SMALL,				NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_VERYSMALL4_2,	 S_HEATWAVE_RIGHT_VERYSMALL4_3,	A_SpawnRightVerySmallFlame,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_VERYSMALL4_3,	 S_HEATWAVE_RIGHT_SMALL4_1,			A_Kill,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Very small right flame
   { S_HEATWAVE_RIGHT_VERYSMALL5_1,	 S_HEATWAVE_RIGHT_VERYSMALL5_2,		NULL,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },
   { S_HEATWAVE_RIGHT_VERYSMALL5_2,	 S_HEATWAVE_RIGHT_VERYSMALL5_1,		A_FinishRightHeatWave,			HEATWAVE_VERYSMALL,			NORMAL_STATE, HEATWAVE_SPEED },

   // Baby nuke
   { S_BABYNUKE,	S_BABYNUKE,		 NULL,	P_BABYNUKE,			NORMAL_STATE, 1 },

   // Nuke
   { S_NUKE,		S_NUKE,			NULL,	P_NUKE,				NORMAL_STATE, 1 },

   // Lead shell
   { S_SHELL,		S_SHELL,		NULL,	P_SHELL,			NORMAL_STATE, 1 },

   // Spiked Ball
   { S_SPIKEDBALL,	S_SPIKEDBALL,	NULL,	P_SPIKEDBALL,		NORMAL_STATE, 1 },

   // Drill Missile
   { S_DRILL,		S_DRILL,		NULL,	P_DRILL,			NORMAL_STATE, 1 },

   // MIRV (parent missile)
   { S_MIRV_PARENT1,	S_MIRV_PARENT2,	NULL, P_MIRV_PARENT1,			NORMAL_STATE, 1 },
   { S_MIRV_PARENT2,	S_MIRV_PARENT1,	NULL, P_MIRV_PARENT2,			NORMAL_STATE, 1 },

   // MIRV (child missile)
   { S_MIRV_CHILD,	S_MIRV_CHILD,		NULL, P_MIRV_CHILD,				NORMAL_STATE, 1 },

   // Electrical arc
   { S_ELECTRICARC1,	S_ELECTRICARC2,	A_ElectricArcThinker, ELECTRIC_ARC1,				NORMAL_STATE, 1 },
   { S_ELECTRICARC2,	S_ELECTRICARC1,	A_ElectricArcThinker, ELECTRIC_ARC2,				NORMAL_STATE, 1 },

   // Flames (no, not *that* kind)
   { S_FLAME_PARENT,	S_FLAME_PARENT,	A_FlameThrowerThinker,	FLAME_BIG,			NORMAL_STATE, 1 },
   { S_FLAME_BIG,		S_FLAME_BIG,	A_FlameThinker,			FLAME_BIG,			NORMAL_STATE, 1 },
   { S_FLAME_MEDIUM,	S_FLAME_BIG,	A_FlameThinker,			FLAME_MEDIUM,		NORMAL_STATE, 1 },
   { S_FLAME_SMALL,		S_FLAME_MEDIUM,	A_FlameThinker,			FLAME_SMALL,		NORMAL_STATE, 1 },

   // Ion Cannon Blast
   { S_IONCANNON1,		S_IONCANNON2,	A_GreenLaser,			P_IONCANNON1,		NORMAL_STATE, 1 },
   { S_IONCANNON2,		S_IONCANNON1,	A_GreenLaser,			P_IONCANNON2,		NORMAL_STATE, 1 },

   // Ion Cannon Death
   { S_IONCANNON_DIE1,		S_IONCANNON_DIE2,	NULL,			P_IONCANNON_DIE1,	NORMAL_STATE, 3 },
   { S_IONCANNON_DIE2,		S_IONCANNON_DIE3,	NULL,			P_IONCANNON_DIE2,	NORMAL_STATE, 3 },
   { S_IONCANNON_DIE3,		S_IONCANNON_DIE4,	NULL,			P_IONCANNON_DIE3,	NORMAL_STATE, 3 },
   { S_IONCANNON_DIE4,		S_IONCANNON_DIE5,	NULL,			P_IONCANNON_DIE4,	NORMAL_STATE, 3 },
   { S_IONCANNON_DIE5,		S_NULL,				NULL,			P_IONCANNON_DIE5,	NORMAL_STATE, 3 },

   // Ion Cannon Burst
   { S_IONCANNON_FIRE1,		S_IONCANNON_FIRE2,	A_GreenLaser,	P_IONCANNON_FIRE1,		NORMAL_STATE, 1 },
   { S_IONCANNON_FIRE2,		S_IONCANNON_FIRE1,	A_GreenLaser,	P_IONCANNON_FIRE2,		NORMAL_STATE, 1 },

   // Ion Cannon Charging
   { S_IONCANNON_CHARGE1,	S_IONCANNON_CHARGE2,	A_IonChargerThinker,		P_IONCANNON_CHARGE1,		NORMAL_STATE, 10 },
   { S_IONCANNON_CHARGE2,	S_IONCANNON_CHARGE3,	A_IonChargerThinker,		P_IONCANNON_CHARGE2,		NORMAL_STATE, 3 },
   { S_IONCANNON_CHARGE3,	S_IONCANNON_CHARGE4,	A_IonChargerThinker,		P_IONCANNON_CHARGE3,		NORMAL_STATE, 3 },
   { S_IONCANNON_CHARGE4,	S_IONCANNON_CHARGE5,	A_IonChargerThinker,		P_IONCANNON_CHARGE4,		NORMAL_STATE, 3 },
   { S_IONCANNON_CHARGE5,	S_IONCANNON_CHARGE1,	A_IonChargerThinker,		P_IONCANNON_CHARGE5,		NORMAL_STATE, 3 },

   // Green laser pulse
   { S_GREENLASER_PULSE,		S_GREENLASER_PULSE,			NULL,						P_GREENLASER_PULSE,			NORMAL_STATE, 1 },
   { S_GREENLASER_PULSE_DIE1,	S_GREENLASER_PULSE_DIE2,	A_LaserPulseCheckTanks,		P_GREENLASER_PULSE_DIE1,	NORMAL_STATE, 6 },
   { S_GREENLASER_PULSE_DIE2,	S_GREENLASER_PULSE_DIE3,	NULL,		P_GREENLASER_PULSE_DIE2,	NORMAL_STATE, 6 },
   { S_GREENLASER_PULSE_DIE3,	S_GREENLASER_PULSE_DIE4,	NULL,		P_GREENLASER_PULSE_DIE3,	NORMAL_STATE, 6 },
   { S_GREENLASER_PULSE_DIE4,	S_GREENLASER_PULSE_DIE5,	NULL,		P_GREENLASER_PULSE_DIE4,	NORMAL_STATE, 6 },
   { S_GREENLASER_PULSE_DIE5,	S_GREENLASER_PULSE_DIE6,	NULL,		P_GREENLASER_PULSE_DIE5,	NORMAL_STATE, 6 },
   { S_GREENLASER_PULSE_DIE6,	S_NULL,						NULL,		P_GREENLASER_PULSE_DIE6,	NORMAL_STATE, 6 },

   // Uranium Shell
   { S_URANIUMSHELL,			S_URANIUMSHELL,				NULL,		P_URANIUMSHELL,		NORMAL_STATE, 1 },

   // Roller
   { S_ROLLER,					S_ROLLER,					A_RotateMissileFast,		P_ROLLER,			NORMAL_STATE, 1 },

   // Canisters
   { S_WATERCANISTER,			S_WATERCANISTER,			A_RotateMissileFast,		P_WATERCANISTER,	NORMAL_STATE, 1 },
   { S_NAPALMCANISTER,			S_NAPALMCANISTER,			A_RotateMissileFast,		P_NAPALMCANISTER,	NORMAL_STATE, 1 },
   { S_PLASMACANISTER,			S_PLASMACANISTER,			A_RotateMissileFast,		P_PLASMACANISTER,	NORMAL_STATE, 1 },
   { S_ACIDCANISTER,			S_ACIDCANISTER,				A_RotateMissileFast,		P_ACIDCANISTER,		NORMAL_STATE, 1 },

   // Machine gun/shotgun tracers
   { S_BULLETTRACER,			S_BULLETTRACER,				NULL,		P_MACHINEGUN_TRACER,			NORMAL_STATE, 1 },

   // Flak detonator & shards
   { S_FLAKSHELL,				S_FLAKSHELL,				A_RotateMissileFast,		P_FLAKSHELL,	NORMAL_STATE, 1 },
   { S_FLAKSHARD1,				S_FLAKSHARD1,				NULL,						P_FLAKSHARD_1,	NORMAL_STATE, 1 },
   { S_FLAKSHARD2,				S_FLAKSHARD2,				NULL,						P_FLAKSHARD_2,	NORMAL_STATE, 1 },

   // EMP Shockwave Missile
   { S_EMPMISSILE1,				S_EMPMISSILE2,				NULL,		P_EMPMISSILE_1,					NORMAL_STATE, 1 },
   { S_EMPMISSILE2,				S_EMPMISSILE1,				NULL,		P_EMPMISSILE_2,					NORMAL_STATE, 1 },

   // EMP Shockwave Explosion
   { S_EMPEXPLODE,				S_EMPEXPLODE,				A_EMPExplodeThinker,		P_EMPEXPLODE_TRANS,				NORMAL_STATE, 1 },

   // Lightning Rod
   { S_LIGHTNINGROD1,			S_LIGHTNINGROD1,			NULL,		P_LIGHTNINGROD_1,				NORMAL_STATE, 3 },

   // Lightning (spawned from lightning rod)
   { S_LIGHTNINGROD_LIGHTNING1,	S_LIGHTNINGROD_LIGHTNING2,	NULL,		LIGHTNINGROD_LIGHTNING,			NORMAL_STATE, 2 },
   { S_LIGHTNINGROD_LIGHTNING2,	S_NULL,						NULL,		LIGHTNINGROD_LIGHTNING,			NORMAL_STATE, 2 },

   // Lightning rod function sprite
   { S_LIGHTNINGROD_FUNC,		S_LIGHTNINGROD_FUNC,		A_LightingRod_Thinker,		P_LIGHTNINGROD_1,				NORMAL_STATE, 1 },

   // Firestarter (Projectile)
   { S_FIRESTARTER,				S_FIRESTARTER,		NULL,	P_FIRESTARTER,				NORMAL_STATE, 1 },

   // Firestarter Flames
   { S_FIRESTARTER_F1,			S_FIRESTARTER_F2,	A_FireStarter_Flame_Burn,	P_FIRESTARTER_F1,			NORMAL_STATE, 1 },
   { S_FIRESTARTER_F2,			S_FIRESTARTER_F3,	A_FireStarter_Flame_Burn,	P_FIRESTARTER_F2,			NORMAL_STATE, 1 },
   { S_FIRESTARTER_F3,			S_FIRESTARTER_F4,	A_FireStarter_Flame_Burn,	P_FIRESTARTER_F3,			NORMAL_STATE, 1 },
   { S_FIRESTARTER_F4,			S_FIRESTARTER_F5,	A_FireStarter_Flame_Burn,	P_FIRESTARTER_F4,			NORMAL_STATE, 1 },
   { S_FIRESTARTER_F5,			S_FIRESTARTER_F6,	A_FireStarter_Flame_Burn,	P_FIRESTARTER_F2,			NORMAL_STATE, 1 },
   { S_FIRESTARTER_F6,			S_FIRESTARTER_F1,	A_FireStarter_Flame_Burn,	P_FIRESTARTER_F3,			NORMAL_STATE, 1 },

   // Tower Pod (not my idea!)
   { S_TOWERPOD1,						S_TOWERPOD2,				NULL,						P_TOWERPOD1,				NORMAL_STATE, 1		},
   { S_TOWERPOD2,						S_TOWERPOD1,				NULL,						P_TOWERPOD2,				NORMAL_STATE, 1		},
   { S_TOWERPOD_BUILD1,			S_TOWERPOD_BUILD2,	NULL,						P_TOWERPOD_BUILD1,	NORMAL_STATE, 15	},
   { S_TOWERPOD_BUILD2,			S_TOWERPOD_BUILD3,	NULL,						P_TOWERPOD_BUILD2,	NORMAL_STATE, 15	},
   { S_TOWERPOD_BUILD3,			S_TOWERPOD_BUILD4,	A_TowerBuild,		P_TOWERPOD_BUILD3,	NORMAL_STATE, 10  },
   { S_TOWERPOD_BUILD4,			S_TOWERPOD_BUILD5,	NULL,						P_TOWERPOD_BUILD4,	NORMAL_STATE, 10  },
   { S_TOWERPOD_BUILD5,			S_TOWERPOD_BUILD6,	A_TowerBuild,		P_TOWERPOD_BUILD5,	NORMAL_STATE, 10  },
   { S_TOWERPOD_BUILD6,			S_TOWERPOD_BUILD7,	NULL,						P_TOWERPOD_BUILD6,	NORMAL_STATE, 10  },
   { S_TOWERPOD_BUILD7,			S_TOWERPOD_BUILD8,	A_TowerBuild,		P_TOWERPOD_BUILD7,	NORMAL_STATE, 10  },
   { S_TOWERPOD_BUILD8,			S_TOWERPOD_BUILD9,	NULL,						P_TOWERPOD_BUILD8,	NORMAL_STATE, 10  },
   { S_TOWERPOD_BUILD9,			S_TOWERPOD_BUILD9,	NULL,						P_TOWERPOD_BUILD9,	NORMAL_STATE, 10  },
   { S_TOWERPOD_DIE1,				S_TOWERPOD_DIE2,		A_ResetLife,		P_TOWERPOD_BUILD9,	NORMAL_STATE, 10  },
   { S_TOWERPOD_DIE2,				S_TOWERPOD_DIE2,		A_TowerDie,			P_TOWERPOD_BUILD9,	NORMAL_STATE, 1   },


   // Missile Turret
   { S_MISSILETURRET1,				S_MISSILETURRET2,					NULL,					P_TOWERPOD1,		NORMAL_STATE, 1 },
   { S_MISSILETURRET2,				S_MISSILETURRET1,					NULL,					P_TOWERPOD2,		NORMAL_STATE, 1 },
   { S_MISSILETURRET_BUILD1,	S_MISSILETURRET_BUILD2,		NULL,					P_TOWERPOD_BUILD1,	NORMAL_STATE, 15 },
   { S_MISSILETURRET_BUILD2,	S_MISSILETURRET_BUILD3,		NULL,					P_TOWERPOD_BUILD2,	NORMAL_STATE, 15 },
   { S_MISSILETURRET_BUILD3,	S_MISSILETURRET_BUILD4,		A_TowerBuild,			P_TOWERPOD_BUILD3,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_BUILD4,	S_MISSILETURRET_BUILD5,		NULL,					P_TOWERPOD_BUILD4,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_BUILD5,	S_MISSILETURRET_BUILD6,		A_TowerBuild,			P_TOWERPOD_BUILD5,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_BUILD6,	S_MISSILETURRET_BUILD7,		NULL,					P_TOWERPOD_BUILD6,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_BUILD7,	S_MISSILETURRET_BUILD8,		A_TowerBuild,			P_TOWERPOD_BUILD7,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_BUILD8,	S_MISSILETURRET_BUILD9,		NULL,					P_TOWERPOD_BUILD8,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_BUILD9,	S_MISSILETURRET_BUILD10,	A_TowerBuild,			P_TOWERPOD_BUILD9,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_BUILD10,	S_MISSILETURRET_BUILD11,	NULL,					P_TOWERPOD_BUILD10,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_BUILD11,	S_MISSILETURRET_BUILD11,	NULL,					P_TOWERPOD_BUILD11,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_ATTACK1,	S_MISSILETURRET_BUILD11,	A_MissileTurretAttack,	P_TOWERPOD_BUILD11,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_DIE1,		S_MISSILETURRET_DIE2,			A_ResetLife,			P_TOWERPOD_BUILD9,	NORMAL_STATE, 10  },
   { S_MISSILETURRET_DIE2,		S_MISSILETURRET_DIE2,			A_TowerDie,				P_TOWERPOD_BUILD9,	NORMAL_STATE, 1   },


   // More explosions!
   { S_EXPLODE_A1,				S_EXPLODE_A2,		NULL,		EXPLODE_A1,			NORMAL_STATE, 3 },
   { S_EXPLODE_A2,				S_EXPLODE_A3,		NULL,		EXPLODE_A2,			NORMAL_STATE, 3 },
   { S_EXPLODE_A3,				S_EXPLODE_A4,		NULL,		EXPLODE_A3,			NORMAL_STATE, 3 },
   { S_EXPLODE_A4,				S_EXPLODE_A5,		NULL,		EXPLODE_A4,			NORMAL_STATE, 3 },
   { S_EXPLODE_A5,				S_EXPLODE_A6,		NULL,		EXPLODE_A5,			NORMAL_STATE, 3 },
   { S_EXPLODE_A6,				S_EXPLODE_A7,		NULL,		EXPLODE_A6,			NORMAL_STATE, 3 },
   { S_EXPLODE_A7,				S_EXPLODE_A8,		NULL,		EXPLODE_A7,			NORMAL_STATE, 3 },
   { S_EXPLODE_A8,				S_NULL,					NULL,		EXPLODE_A8,			NORMAL_STATE, 3 },

   { S_EXPLODE_B1,				S_EXPLODE_B2,		NULL,		EXPLODE_B1,			NORMAL_STATE, 3 },
   { S_EXPLODE_B2,				S_EXPLODE_B3,		NULL,		EXPLODE_B2,			NORMAL_STATE, 3 },
   { S_EXPLODE_B3,				S_EXPLODE_B4,		NULL,		EXPLODE_B3,			NORMAL_STATE, 3 },
   { S_EXPLODE_B4,				S_EXPLODE_B5,		NULL,		EXPLODE_B4,			NORMAL_STATE, 3 },
   { S_EXPLODE_B5,				S_EXPLODE_B6,		NULL,		EXPLODE_B5,			NORMAL_STATE, 3 },
   { S_EXPLODE_B6,				S_EXPLODE_B7,		NULL,		EXPLODE_B6,			NORMAL_STATE, 3 },
   { S_EXPLODE_B7,				S_EXPLODE_B8,		NULL,		EXPLODE_B7,			NORMAL_STATE, 3 },
   { S_EXPLODE_B8,				S_NULL,					NULL,		EXPLODE_B8,			NORMAL_STATE, 3 },

   { S_SMALLEXPLODE_A1,			S_SMALLEXPLODE_A2,	NULL,		SMALLEXPLODE_A1,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_A2,			S_SMALLEXPLODE_A3,	NULL,		SMALLEXPLODE_A2,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_A3,			S_SMALLEXPLODE_A4,	NULL,		SMALLEXPLODE_A3,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_A4,			S_SMALLEXPLODE_A5,	NULL,		SMALLEXPLODE_A4,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_A5,			S_SMALLEXPLODE_A6,	NULL,		SMALLEXPLODE_A5,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_A6,			S_SMALLEXPLODE_A7,	NULL,		SMALLEXPLODE_A6,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_A7,			S_SMALLEXPLODE_A8,	NULL,		SMALLEXPLODE_A7,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_A8,			S_NULL,							NULL,		SMALLEXPLODE_A8,	NORMAL_STATE, 2 },

   { S_SMALLEXPLODE_B1,			S_SMALLEXPLODE_B2,	NULL,		SMALLEXPLODE_B1,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_B2,			S_SMALLEXPLODE_B3,	NULL,		SMALLEXPLODE_B2,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_B3,			S_SMALLEXPLODE_B4,	NULL,		SMALLEXPLODE_B3,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_B4,			S_SMALLEXPLODE_B5,	NULL,		SMALLEXPLODE_B4,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_B5,			S_SMALLEXPLODE_B6,	NULL,		SMALLEXPLODE_B5,	NORMAL_STATE, 2 },
   { S_SMALLEXPLODE_B6,			S_NULL,							NULL,		SMALLEXPLODE_B6,	NORMAL_STATE, 2 },

   // Spark ball (the small one)
   { S_SPARKBALL1,				S_SPARKBALL2,		NULL,		P_SPARKBALL1,		NORMAL_STATE, 1 },
   { S_SPARKBALL2,				S_SPARKBALL1,		NULL,		P_SPARKBALL2,		NORMAL_STATE, 1 },

   // Large Spark ball (the um..large one)
   { S_LARGESPARKBALL1,			S_LARGESPARKBALL2,	NULL,		P_LARGESPARKBALL1,	NORMAL_STATE, 1 },
   { S_LARGESPARKBALL2,			S_LARGESPARKBALL3,	NULL,		P_LARGESPARKBALL2,	NORMAL_STATE, 1 },
   { S_LARGESPARKBALL3,			S_LARGESPARKBALL1,	NULL,		P_LARGESPARKBALL3,	NORMAL_STATE, 1 },

   // Heat Sentry Pod
   //
   // Projectile
   { S_HSENTRY1,					S_HSENTRY1,				NULL,										P_HSENTRY1,				NORMAL_STATE, 1 },
   { S_HSENTRY_SHOT,			S_HSENTRY_SHOT,		NULL,										P_HSENTRY_SHOT,		NORMAL_STATE, 1 },
   { S_HSENTRY_SHOT1,			S_HSENTRY_SHOT1,	A_HSentryShot_Thinker,	P_HSENTRY_SHOT,		NORMAL_STATE, 1 },
   // Opening
   { S_HSENTRY2,				S_HSENTRY3,			A_SentryOpen,	P_HSENTRY2,			NORMAL_STATE, 20 },
   { S_HSENTRY3,				S_HSENTRY4,			NULL,					P_HSENTRY3,			NORMAL_STATE, 20 },
   { S_HSENTRY4,				S_HSENTRY5,			NULL,					P_HSENTRY4,			NORMAL_STATE, 20 },
   { S_HSENTRY5,				S_HSENTRY5,			NULL,					P_HSENTRY5,			NORMAL_STATE, 20 },
   // Attack
   { S_HSENTRY_ATTACK1,			S_HSENTRY5,			A_HSentry_Attack,		P_HSENTRY5,			NORMAL_STATE, 10 },
   // Death
   { S_HSENTRY_DIE1,			S_NULL,				A_SentryDeath,			P_HSENTRY5,			NORMAL_STATE, 1 },


   // Electric Sentry Pod
   //
   // Projectile
   { S_ESENTRY1,				S_ESENTRY1,			NULL,					P_ESENTRY1,			NORMAL_STATE, 1 },
   { S_ESENTRY_SHOT,			S_ESENTRY_SHOT,		NULL,					P_ESENTRY_SHOT,		NORMAL_STATE, 1 },
   // Opening
   { S_ESENTRY2,				S_ESENTRY3,			A_SentryOpen,			P_ESENTRY2,			NORMAL_STATE, 20 },
   { S_ESENTRY3,				S_ESENTRY4,			NULL,					P_ESENTRY3,			NORMAL_STATE, 20 },
   { S_ESENTRY4,				S_ESENTRY5,			NULL,					P_ESENTRY4,			NORMAL_STATE, 20 },
   { S_ESENTRY5,				S_ESENTRY5,			NULL,					P_ESENTRY5,			NORMAL_STATE, 20 },
   // Attack
   { S_ESENTRY_ATTACK1,			S_ESENTRY5,			A_ESentry_Attack,		P_ESENTRY5,			NORMAL_STATE, 10 },
   // Death
   { S_ESENTRY_DIE1,			S_NULL,				A_SentryDeath,			P_ESENTRY5,			NORMAL_STATE, 1 },


   // Ballistic Sentry Pod
   //
   // Projectile
   { S_BSENTRY1,				S_BSENTRY1,			NULL,					P_BSENTRY1,			NORMAL_STATE, 1 },
   // Opening
   { S_BSENTRY2,				S_BSENTRY3,			A_SentryOpen,			P_BSENTRY2,			NORMAL_STATE, 20 },
   { S_BSENTRY3,				S_BSENTRY4,			NULL,					P_BSENTRY3,			NORMAL_STATE, 20 },
   { S_BSENTRY4,				S_BSENTRY5,			NULL,					P_BSENTRY4,			NORMAL_STATE, 20 },
   { S_BSENTRY5,				S_BSENTRY5,			NULL,					P_BSENTRY5,			NORMAL_STATE, 20 },
   // Attack
   { S_BSENTRY_ATTACK1,			S_BSENTRY5,			A_BSentry_Attack,		P_BSENTRY5,			NORMAL_STATE, 10 },
   // Death
   { S_BSENTRY_DIE1,			S_NULL,				A_SentryDeath,			P_BSENTRY5,			NORMAL_STATE, 1 },

   // Shotgun / Machinegun empty shell
   { S_EMPTYSHELL,				S_EMPTYSHELL,		A_ShellThinker,			EMPTYSHELL,			NORMAL_STATE, 1 },

   // Needle Missile
   { S_NEEDLEMISSILE1,			S_NEEDLEMISSILE2,	NULL,					P_NEEDLE1,			NORMAL_STATE, 1 },
   { S_NEEDLEMISSILE2,			S_NEEDLEMISSILE1,	NULL,					P_NEEDLE2,			NORMAL_STATE, 1 },

   // Large lava rock
   { S_BIGLAVAROCK,				S_BIGLAVAROCK,			NULL,						P_LAVAROCKBIG,			NORMAL_STATE, 1 },
   { S_BIGLAVAROCKEFFECT,		S_BIGLAVAROCKEFFECT,	A_FollowParent,				P_LAVAROCKBIG_EFFECT,	NORMAL_STATE, 1 },

   // Small lava rock
   { S_SMALLLAVAROCK,			S_SMALLLAVAROCK,		NULL,						P_LAVAROCKSMALL,		NORMAL_STATE, 1 },
   { S_SMALLLAVAROCKEFFECT,		S_SMALLLAVAROCKEFFECT,	A_FollowParent,				P_LAVAROCKSMALL_EFFECT,	NORMAL_STATE, 1 },

   // Lava rock spawner
   { S_LAVASPAWNER,				S_LAVASPAWNER,			NULL,						P_LAVAROCKSMALL,		NORMAL_STATE, 1 },
   { S_LAVASPAWNER_ATTACK,		S_LAVASPAWNER_ATTACK1,	A_LavaSpawnerSetup,			P_LAVAROCKSMALL,		NORMAL_STATE, 1 },
   { S_LAVASPAWNER_ATTACK1,		S_LAVASPAWNER_ATTACK1,	A_LavaSpawnerThinker,		P_LAVAROCKSMALL,		NORMAL_STATE, 1 },

   // Snow flakes
   { S_SNOWFLAKE_THINKER,		S_SNOWFLAKE_THINKER,	A_SnowFlakeSpawner,			SNOWFLAKE_LARGE,		NORMAL_STATE, 1 },

   { S_SNOWFLAKE_LARGE,			S_SNOWFLAKE_LARGE,		A_LargeSnowFlakeThinker,	SNOWFLAKE_LARGE,		NORMAL_STATE, 1 },
   { S_SNOWFLAKE_MEDIUM,		S_SNOWFLAKE_MEDIUM,		A_MediumSnowFlakeThinker,	SNOWFLAKE_MEDIUM,		NORMAL_STATE, 1 },
   { S_SNOWFLAKE_SMALL,			S_SNOWFLAKE_SMALL,		A_SmallSnowFlakeThinker,	SNOWFLAKE_SMALL,		NORMAL_STATE, 1 },

   // Heatray
   { S_HEATRAY1,				S_HEATRAY2,				A_HeatrayThinker,			P_HEATRAY1,			NORMAL_STATE, 3 },
   { S_HEATRAY2,				S_HEATRAY1,				A_HeatrayThinker,			P_HEATRAY2,			NORMAL_STATE, 3 },
   { S_HEATRAY_START1,			S_HEATRAY_START2,		A_HeatrayThinker,			P_HEATRAY_START1,	NORMAL_STATE, 2 },
   { S_HEATRAY_START2,			S_HEATRAY_START1,		A_HeatrayThinker,			P_HEATRAY_START2,	NORMAL_STATE, 2 },
   { S_HEATRAY_EFFECT,			S_HEATRAY_EFFECT,		A_HeatrayEffect,			P_HEATRAY_EFFECT,	NORMAL_STATE, 1 },

   { S_HEATRAY_F1,				S_HEATRAY_F2,			A_HeatrayFlameThinker,		P_FIRESTARTER_F1,			NORMAL_STATE, 1 },
   { S_HEATRAY_F2,				S_HEATRAY_F3,			A_HeatrayFlameThinker,		P_FIRESTARTER_F2,			NORMAL_STATE, 1 },
   { S_HEATRAY_F3,				S_HEATRAY_F4,			A_HeatrayFlameThinker,		P_FIRESTARTER_F3,			NORMAL_STATE, 1 },
   { S_HEATRAY_F4,				S_HEATRAY_F5,			A_HeatrayFlameThinker,		P_FIRESTARTER_F4,			NORMAL_STATE, 1 },
   { S_HEATRAY_F5,				S_HEATRAY_F6,			A_HeatrayFlameThinker,		P_FIRESTARTER_F2,			NORMAL_STATE, 1 },
   { S_HEATRAY_F6,				S_HEATRAY_F1,			A_HeatrayFlameThinker,		P_FIRESTARTER_F3,			NORMAL_STATE, 1 },

   // Railgun smoke (railgun has no actual missile sprite)
   { S_RAILGUNSMOKE,			S_RAILGUNSMOKE,			A_RailGunSmoke,				P_RAILGUNSMOKE,	NORMAL_STATE, 1 },

   // Firefly
   { S_FIREFLY,					S_FIREFLY,				NULL,						P_FIREFLY,			NORMAL_STATE, 1 },
   { S_FIREFLY_DIE,				S_FIREFLY_DIE,			A_FireFlyDieThinker,		P_FIREFLY,			NORMAL_STATE, 1 },
   { S_FIREFLY_EFFECT,			S_FIREFLY_EFFECT,		A_FireFlyEffect,			P_FIREFLY_EFFECT,	NORMAL_STATE, 1 },

   // Alien blast beam (for tank deaths, like on the orginal scorched earth)
   { S_ALIENBEAM1,				S_ALIENBEAM2,			NULL,						ALIENBEAM_1,		NORMAL_STATE, 3 },
   { S_ALIENBEAM2,				S_ALIENBEAM3,			NULL,						ALIENBEAM_2,		NORMAL_STATE, 3 },
   { S_ALIENBEAM3,				S_ALIENBEAM4,			NULL,						ALIENBEAM_3,		NORMAL_STATE, 3 },
   { S_ALIENBEAM4,				S_ALIENBEAM1,			NULL,						ALIENBEAM_4,		NORMAL_STATE, 3 },

   // Alien abduction beam (oh gnos!!1)
   { S_ALIENBEAMUP1,			S_ALIENBEAMUP2,			A_AlienBeamUpThinker,		ALIENBEAMUP_1,		NORMAL_STATE, 1 },
   { S_ALIENBEAMUP2,			S_ALIENBEAMUP1,			A_AlienBeamUpThinker,		ALIENBEAMUP_2,		NORMAL_STATE, 1 },

   // Small particle effect for alien abduction
   { S_ALIENBEAMUPPARTICLE,		S_ALIENBEAMUPPARTICLE,	NULL,						ALIENBEAMPARTICLE,	NORMAL_STATE, 1 },

   // Cave spikes (cause I can't spell stilagmites, er.. stalagmites, stillagmyte...bah, forget it.)
   { S_CAVESPIKE1_IDLE,			S_CAVESPIKE1_IDLE,		NULL,						CAVESPIKE_1,		NORMAL_STATE, 1 },
   { S_CAVESPIKE1_SHAKE1,		S_CAVESPIKE1_SHAKE2,	A_SpikeShakeInit,			CAVESPIKE_1,		NORMAL_STATE, 1 },
   { S_CAVESPIKE1_SHAKE2,		S_CAVESPIKE1_SHAKE2,	A_SpikeShake,				CAVESPIKE_1,		NORMAL_STATE, 1 },
   { S_CAVESPIKE1_FALL1,		S_CAVESPIKE1_FALL1,		A_SpikeFall,				CAVESPIKE_1,		NORMAL_STATE, 1 },
   { S_CAVESPIKE1_DIE,			S_CAVESPIKE1_DIE,		A_SpikeDie,					CAVESPIKE_1,		NORMAL_STATE, 1 },

   { S_CAVESPIKE2_IDLE,			S_CAVESPIKE2_IDLE,		NULL,						CAVESPIKE_2,		NORMAL_STATE, 1 },
   { S_CAVESPIKE2_SHAKE1,		S_CAVESPIKE2_SHAKE2,	A_SpikeShakeInit,			CAVESPIKE_2,		NORMAL_STATE, 1 },
   { S_CAVESPIKE2_SHAKE2,		S_CAVESPIKE2_SHAKE2,	A_SpikeShake,				CAVESPIKE_2,		NORMAL_STATE, 1 },
   { S_CAVESPIKE2_FALL1,		S_CAVESPIKE2_FALL1,		A_SpikeFall,				CAVESPIKE_2,		NORMAL_STATE, 1 },
   { S_CAVESPIKE2_DIE,			S_CAVESPIKE2_DIE,		A_SpikeDie,					CAVESPIKE_2,		NORMAL_STATE, 1 },

   { S_CAVESPIKE3_IDLE,			S_CAVESPIKE3_IDLE,		NULL,						CAVESPIKE_3,		NORMAL_STATE, 1 },
   { S_CAVESPIKE3_SHAKE1,		S_CAVESPIKE3_SHAKE2,	A_SpikeShakeInit,			CAVESPIKE_3,		NORMAL_STATE, 1 },
   { S_CAVESPIKE3_SHAKE2,		S_CAVESPIKE3_SHAKE2,	A_SpikeShake,				CAVESPIKE_3,		NORMAL_STATE, 1 },
   { S_CAVESPIKE3_FALL1,		S_CAVESPIKE3_FALL1,		A_SpikeFall,				CAVESPIKE_3,		NORMAL_STATE, 1 },
   { S_CAVESPIKE3_DIE,			S_CAVESPIKE3_DIE,		A_SpikeDie,					CAVESPIKE_3,		NORMAL_STATE, 1 },

   { S_CAVESPIKE4_IDLE,			S_CAVESPIKE4_IDLE,		NULL,						CAVESPIKE_4,		NORMAL_STATE, 1 },
   { S_CAVESPIKE4_SHAKE1,		S_CAVESPIKE4_SHAKE2,	A_SpikeShakeInit,			CAVESPIKE_4,		NORMAL_STATE, 1 },
   { S_CAVESPIKE4_SHAKE2,		S_CAVESPIKE4_SHAKE2,	A_SpikeShake,				CAVESPIKE_4,		NORMAL_STATE, 1 },
   { S_CAVESPIKE4_FALL1,		S_CAVESPIKE4_FALL1,		A_SpikeFall,				CAVESPIKE_4,		NORMAL_STATE, 1 },
   { S_CAVESPIKE4_DIE,			S_CAVESPIKE4_DIE,		A_SpikeDie,					CAVESPIKE_4,		NORMAL_STATE, 1 },

   { S_CAVESPIKE5_IDLE,			S_CAVESPIKE5_IDLE,		NULL,						CAVESPIKE_5,		NORMAL_STATE, 1 },
   { S_CAVESPIKE5_SHAKE1,		S_CAVESPIKE5_SHAKE2,	A_SpikeShakeInit,			CAVESPIKE_5,		NORMAL_STATE, 1 },
   { S_CAVESPIKE5_SHAKE2,		S_CAVESPIKE5_SHAKE2,	A_SpikeShake,				CAVESPIKE_5,		NORMAL_STATE, 1 },
   { S_CAVESPIKE5_FALL1,		S_CAVESPIKE5_FALL1,		A_SpikeFall,				CAVESPIKE_5,		NORMAL_STATE, 1 },
   { S_CAVESPIKE5_DIE,			S_CAVESPIKE5_DIE,		A_SpikeDie,					CAVESPIKE_5,		NORMAL_STATE, 1 },

   { S_CAVESPIKESHARD1,			S_CAVESPIKESHARD1,		A_ShardThinker,				CAVESPIKESHARD1,	NORMAL_STATE, 1 },
   { S_CAVESPIKESHARD2,			S_CAVESPIKESHARD2,		A_ShardThinker,				CAVESPIKESHARD2,	NORMAL_STATE, 1 },
   { S_CAVESPIKESHARD3,			S_CAVESPIKESHARD3,		A_ShardThinker,				CAVESPIKESHARD3,	NORMAL_STATE, 1 },
   { S_CAVESPIKESHARD4,			S_CAVESPIKESHARD4,		A_ShardThinker,				CAVESPIKESHARD4,	NORMAL_STATE, 1 },
   { S_CAVESPIKESHARD5,			S_CAVESPIKESHARD5,		A_ShardThinker,				CAVESPIKESHARD5,	NORMAL_STATE, 1 },
   { S_CAVESPIKESHARD6,			S_CAVESPIKESHARD6,		A_ShardThinker,				CAVESPIKESHARD6,	NORMAL_STATE, 1 },

   // Death's Head (child missile)
   { S_DEATHSHEADCHILD,			S_DEATHSHEADCHILD,		NULL,						P_DEATHSHEADCHILD,	NORMAL_STATE, 1 },

   // Death's Head (parent missile - empty/full versions)
   { S_DEATHSHEADEMPTY_1,		S_DEATHSHEADEMPTY_2,	NULL,						P_DEATHSHEADEMPTY_1,NORMAL_STATE, 1 },
   { S_DEATHSHEADEMPTY_2,		S_DEATHSHEADEMPTY_1,	NULL,						P_DEATHSHEADEMPTY_2,NORMAL_STATE, 1 },
   { S_DEATHSHEADFULL_1,		S_DEATHSHEADFULL_2,		NULL,						P_DEATHSHEADFULL_1,	NORMAL_STATE, 1 },
   { S_DEATHSHEADFULL_2,		S_DEATHSHEADFULL_1,		NULL,						P_DEATHSHEADFULL_2,	NORMAL_STATE, 1 },

   // Tank gibs
   { S_TANKGIB1,				S_TANKGIB1,				A_RotateMissileFast,		TANKGIB_1,			NORMAL_STATE, 1 },
   { S_TANKGIB2,				S_TANKGIB2,				A_RotateMissileFast,		TANKGIB_2,			NORMAL_STATE, 1 },
   { S_TANKGIB3,				S_TANKGIB3,				A_RotateMissileFast,		TANKGIB_3,			NORMAL_STATE, 1 },
   { S_TANKGIB4,				S_TANKGIB4,				A_RotateMissileFast,		TANKGIB_4,			NORMAL_STATE, 1 },
   { S_TANKGIB5,				S_TANKGIB5,				A_RotateMissileFast,		TANKGIB_5,			NORMAL_STATE, 1 },

   // Cherry Blossoms
   { S_CHERRYBLOSS_1,			S_CHERRYBLOSS_1,		A_CherryBlossThinker,		CHERRYBLOSS_1,		NORMAL_STATE, 1 },
   { S_CHERRYBLOSS_1SIT,		S_CHERRYBLOSS_1SIT,		A_CherryBlossFinish,		CHERRYBLOSS_1,		NORMAL_STATE, 1 },
   { S_CHERRYBLOSS_2,			S_CHERRYBLOSS_2,		A_CherryBlossThinker,		CHERRYBLOSS_2,		NORMAL_STATE, 1 },
   { S_CHERRYBLOSS_2SIT,		S_CHERRYBLOSS_2SIT,		A_CherryBlossFinish,		CHERRYBLOSS_2,		NORMAL_STATE, 1 },
   { S_CHERRYBLOSS_3,			S_CHERRYBLOSS_3,		A_CherryBlossThinker,		CHERRYBLOSS_3,		NORMAL_STATE, 1 },
   { S_CHERRYBLOSS_3SIT,		S_CHERRYBLOSS_3SIT,		A_CherryBlossFinish,		CHERRYBLOSS_3,		NORMAL_STATE, 1 },
   { S_CHERRYBLOSS_SPAWNER,		S_CHERRYBLOSS_SPAWNER,	A_CherrySpawnerThinker,		CHERRYBLOSS_1,		NORMAL_STATE, 1 },

   // Rain drop
   { S_RAINDROP,				S_RAINDROP,				A_RainDropThinker,			RAINDROP,			NORMAL_STATE, 1 },
   { S_RAINDROP_SPAWNER,		S_RAINDROP_SPAWNER,		A_RainDropSpawnerThinker,	RAINDROP,			NORMAL_STATE, 1 },
   { S_RAINDROP_ATTACK,			S_RAINDROP_SPAWNER,		A_RainDropSpawnerAttack,	RAINDROP,			NORMAL_STATE, 1 },

   // Lava shooter
   { S_LAVASHOOTER,				S_LAVASHOOTER,			NULL,						P_LAVAROCKSMALL,	NORMAL_STATE, 1 },
   { S_LAVASHOOTER_ATTACK1,		S_LAVASHOOTER_ATTACK2,	A_LavaShootAttack,			P_LAVAROCKSMALL,	NORMAL_STATE, 1 },
   { S_LAVASHOOTER_ATTACK2,		S_LAVASHOOTER_ATTACK2,	A_LavaShooterThinker,		P_LAVAROCKSMALL,	NORMAL_STATE, 1 },

   // Heat coronas
   { S_HEATCORONA,				S_HEATCORONA,			A_HeatCoronaThinker,		HEATCORONA,			NORMAL_STATE, 1 },
   { S_HEATCORONA_SPAWNER,		S_HEATCORONA_SPAWNER,	A_HeatCoronaSpawnerThinker,	HEATCORONA,			NORMAL_STATE, 1 },

   // Lava-floor damage
   { S_LAVAFLOORDAMAGE_IDLE,	S_LAVAFLOORDAMAGE_IDLE,	NULL,								P_FIRESTARTER_F1,	NORMAL_STATE, 99 },
   { S_LAVAFLOORDAMAGE,				S_LAVAFLOORDAMAGE_IDLE,	A_LavaFloorDamage,	P_FIRESTARTER_F1,	NORMAL_STATE, 1 },

   // Turn arrow
   { S_TURNARROW,				S_TURNARROW,			A_TurnArrowThinker,		TURNARROW,			NORMAL_STATE, 1 },

   // Giganuke
   { S_GIGANUKE,				S_GIGANUKE,				NULL,					P_GIGANUKE,			NORMAL_STATE, 1 },

   // Conventional Shield
   { S_SHIELD_CONVENTIONAL,	 S_SHIELD_CONVENTIONAL,		NULL,			SHIELD_CONVENTIONAL,	NORMAL_STATE, 5 },

   // Super Conventional Shield
   { S_SHIELD_SUPERCONVENTIONAL,	 S_SHIELD_SUPERCONVENTIONAL,NULL,	SHIELD_SUPERCONVENTIONAL,	NORMAL_STATE, 5 },

   // Mortar shell and base
   { S_MORTARSHELL,				S_MORTARSHELL,			NULL,			P_MORTARSHELL,				NORMAL_STATE, 1 },
   { S_MORTARBASE,				S_MORTARBASE,			NULL,			MORTARBASE,					NORMAL_STATE, 1 },

   // Polaris Missile
   { S_POLARISMISSILE1,			S_POLARISMISSILE2,		NULL,					P_POLARISMISSILE1,			NORMAL_STATE, 1 },
   { S_POLARISMISSILE2,			S_POLARISMISSILE1,		NULL,					P_POLARISMISSILE2,			NORMAL_STATE, 1 },
   { S_POLARISOPEN1,			S_POLARISOPEN2,			NULL,					P_POLARISLAND1,				NORMAL_STATE, 15 },
   { S_POLARISOPEN2,			S_POLARISOPEN3,			NULL,					P_POLARISLAND2,				NORMAL_STATE, 15 },
   { S_POLARISOPEN3,			S_POLARISOPEN3,			A_PolarisMistThinkier,	P_POLARISLAND3,				NORMAL_STATE, 1 },
   { S_POLARISEMPBLAST,			S_POLARISEMPBLAST,		A_PolarisEMPExplode,	P_POLARISEMPBLAST,			NORMAL_STATE, 1 },
   { S_NITROGENPUFF_MEDIUM,		S_NITROGENPUFF_MEDIUM,	A_NitrogenThinker,		NITROGENPUFF_MEDIUM,NORMAL_STATE, 1 },
   { S_NITROGENPUFF_LARGE,		S_NITROGENPUFF_LARGE,	A_NitrogenThinker,		NITROGENPUFF_LARGE,	NORMAL_STATE, 1 },

   // Ice Shards
   { ICESHARD_BOTTOMLEFT,		ICESHARD_BOTTOMLEFT,	A_IceShardThinker,		ICESHARD_BLEFT,		NORMAL_STATE, 1 },
   { ICESHARD_BOTTOMRIGHT,		ICESHARD_BOTTOMRIGHT,	A_IceShardThinker,		ICESHARD_BRIGHT,	NORMAL_STATE, 1 },
   { ICESHARD_BOTTOM,			ICESHARD_BOTTOM,		A_IceShardThinker,		ICESHARD_B,			NORMAL_STATE, 1 },
   { ICESHARD_TOP,				ICESHARD_TOP,			A_IceShardThinker,		ICESHARD_T,			NORMAL_STATE, 1 },
   { ICESHARD_TOPLEFT,			ICESHARD_TOPLEFT,		A_IceShardThinker,		ICESHARD_TLEFT,		NORMAL_STATE, 1 },
   { ICESHARD_TOPRIGHT,			ICESHARD_TOPRIGHT,		A_IceShardThinker,		ICESHARD_TRIGHT,	NORMAL_STATE, 1 },

   // Black hole bomb
   { S_BHBOMB,					S_BHBOMB,				A_RotateMissileFast,	P_BHBOMB,			NORMAL_STATE, 1 },
   { S_BHBOMB_OPEN1,			S_BHBOMB_OPEN1,			A_BHBomb_Open,			P_BHBOMB,			NORMAL_STATE, 1 },
   { S_BHBOMB_OPEN2,			S_BHBOMB_OPEN2,			A_BHBomb_Open,			P_BHBOMB_OPEN1,		NORMAL_STATE, 1 },
   { S_BHBOMB_OPEN3,			S_BHBOMB_OPEN3,			A_BHBomb_Open,			P_BHBOMB_OPEN2,		NORMAL_STATE, 1 },
   { S_BHBOMB_BLOB1,			S_BHBOMB_BLOB1,			A_BH_Thinker,			P_BHBOMB_BLOB1,		NORMAL_STATE, 1 },
   { S_BHBOMB_BLOB2,			S_BHBOMB_BLOB2,			A_BH_Thinker,			P_BHBOMB_BLOB2,		NORMAL_STATE, 1 },
   { S_BHBOMB_BLOB3,			S_BHBOMB_BLOB3,			A_BH_Thinker,			P_BHBOMB_BLOB3,		NORMAL_STATE, 1 },
   { S_BHBOMB_BLOB4,			S_BHBOMB_BLOB4,			A_BH_Thinker,			P_BHBOMB_BLOB4,		NORMAL_STATE, 1 },
   { S_BHBOMB_CLOUDS1,			S_BHBOMB_CLOUDS1,		A_BH_CloudThinker1,		P_BHBOMB_CLOUDS,	NORMAL_STATE, 1 },
   { S_BHBOMB_CLOUDS2,			S_BHBOMB_CLOUDS2,		A_BH_CloudThinker2,		P_BHBOMB_CLOUDS,	NORMAL_STATE, 1 },
   { S_BHBOMB_IMPLODEWAVE,		S_BHBOMB_IMPLODEWAVE,	A_BH_ImplodeWave,		P_POLARISEMPBLAST,	NORMAL_STATE, 1 },
   { S_BHBOMB_SHRINK1,			S_BHBOMB_SHRINK2,		NULL,					P_BHBOMB_BLOB3,		NORMAL_STATE, 10 },
   { S_BHBOMB_SHRINK2,			S_BHBOMB_SHRINK3,		NULL,					P_BHBOMB_BLOB2,		NORMAL_STATE, 10 },
   { S_BHBOMB_SHRINK3,			S_BHBOMB_SHRINK4,		NULL,					P_BHBOMB_BLOB1,		NORMAL_STATE, 10 },
   { S_BHBOMB_SHRINK4,			S_BHBOMB_SHRINK5,		NULL,					P_BHBOMB_MATTER1,	NORMAL_STATE, 10 },
   { S_BHBOMB_SHRINK5,			S_BHBOMB_SHRINK6,		NULL,					P_BHBOMB_MATTER2,	NORMAL_STATE, 30 },
   { S_BHBOMB_SHRINK6,			S_BHBOMB_SHRINK6,		A_BH_MatterFall,		P_BHBOMB_MATTER2,	NORMAL_STATE, 1 },

   // Twin Electric Lances
   { S_TEL_NEGATIVECHARGE,		S_TEL_NEGATIVECHARGE,	NULL,					P_TEL_NEGATIVE,		NORMAL_STATE, 1 },
   { S_TEL_POSITIVECHARGE,		S_TEL_POSITIVECHARGE,	NULL,					P_TEL_POSITIVE,		NORMAL_STATE, 1 },
   { S_TEL_NEGATIVE,			S_TEL_NEGATIVE,			A_TEL_NegativeThinker,	P_TEL_NEGATIVE,		NORMAL_STATE, 1 },
   { S_TEL_POSITIVE,			S_TEL_POSITIVE,			A_TEL_PositiveThinker,	P_TEL_POSITIVE,		NORMAL_STATE, 1 },

   // Lightning (hell strike)
   { S_LIGHTNINGROD_HELLSTRIKE1,	S_LIGHTNINGROD_HELLSTRIKE2,	NULL,		LIGHTNINGROD_HELLSTRIKE,	NORMAL_STATE, 2 },
   { S_LIGHTNINGROD_HELLSTRIKE2,	S_NULL,						NULL,		LIGHTNINGROD_HELLSTRIKE,	NORMAL_STATE, 2 },

   // Airstrike
   { S_AS_AIRSHIP,					S_AS_AIRSHIP,		A_AirShipThinker,	P_AS_AIRSHIP,		NORMAL_STATE, 1 },
   { S_AS_FLARESHOT,				S_AS_FLARESHOT,		NULL,				P_AS_FLARE,			NORMAL_STATE, 1 },
   { S_AS_FLARE,					S_AS_FLARE,			A_ASFlareThinker,	P_AS_FLARE,			NORMAL_STATE, 1 },
   { S_AS_FLARESMOKE,				S_AS_FLARESMOKE,	A_ASFlareSmoke,		P_AS_FLARESMOKE,	NORMAL_STATE, 1 },

   // Ion Battery
   { S_IONBATTERY_THINKER,			S_IONBATTERY_THINKER,	A_IB_Thinker,	P_IONBATTERY,			NORMAL_STATE, 1 },
   { S_IONBATTERY,					S_IONBATTERY,			A_AS_Move,		P_IONBATTERY,			NORMAL_STATE, 1 },
   { S_IONBATTERY_FLARE1,			S_IONBATTERY_FLARE2,	A_AS_Move,		P_IONBATTERY_FLARE1,	NORMAL_STATE, 10 },
   { S_IONBATTERY_FLARE2,			S_NULL,					NULL,			P_IONBATTERY_FLARE2,	NORMAL_STATE, 10 },

   // Heat cannon
   { S_HEATCANNON_THINKER,		S_HEATCANNON_THINKER,	A_HeatCannonThinker,	P_AS_HEATGUN,		NORMAL_STATE, 1},
   { S_HEATCANNON_SHOT1,		S_HEATCANNON_SHOT2,		NULL,					P_AS_FIREBURST1,	NORMAL_STATE, 2},
   { S_HEATCANNON_SHOT2,		S_HEATCANNON_SHOT1,		NULL,					P_AS_FIREBURST2,	NORMAL_STATE, 2},

   // Tank bay
   { S_AS_TANKBAY1,				S_AS_TANKBAY1,			A_AS_TankBayThinker,	P_AS_TANKBAY,		NORMAL_STATE, 1},
   { S_AS_TANKBAY2,				S_AS_TANKBAY2,			A_AS_TankBayDeploy,		P_AS_TANKBAY,		NORMAL_STATE, 1},

   // Assault Flyers
   { S_AS_FLYER,				S_AS_FLYER,				A_AS_FlyerThinker,		P_AS_FIGHTER,			NORMAL_STATE, 1},

   // Funky bomb
   { S_FUNKYBOMB,				S_FUNKYBOMB,			NULL,					P_FUNKYBOMB,			NORMAL_STATE, 1 },

   // Funky nuke (Richie's super)
   { S_FUNKYNUKE,				S_FUNKYNUKE,			NULL,					P_FUNKYNUKE,			NORMAL_STATE, 1 },

   // Fire Toad
   { S_FIRETOAD,				S_FIRETOAD,				NULL,					P_FIRETOAD,				NORMAL_STATE, 1 },
   { S_FIRETOAD_STAGE1,			S_FIRETOAD_STAGE1,		A_FireToad_Thinker,		P_FIRETOAD_COMPRESSED,	NORMAL_STATE, 1 },
   { S_FIRETOAD_STAGE2,			S_FIRETOAD_STAGE2,		A_FireToad_Thinker,		P_FIRETOAD_COMPRESSED,	NORMAL_STATE, 1 },

   // Proton flare stuff
   { S_PROTONFLARE,				S_PROTONFLARE,			NULL,					P_PROTON,				NORMAL_STATE, 1 },

   // Tank muzzle flares
   { S_TANKMUZZLEFLARE,			S_TANKMUZZLEFLARE,		A_TankFlareThinker,	TANK_MUZZLEFLARE,		NORMAL_STATE, 1 },
   { S_TANKPHOTONFLARE,			S_TANKPHOTONFLARE,		A_TankFlareThinker,	TANK_PHOTONFLARE,		NORMAL_STATE, 1 },

   // NIV Shield
   { S_SHIELD_NIV_B1,			S_SHIELD_NIV_B2,		A_NIVBuildSound,	SHIELD_NIV_B1,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_B2,			S_SHIELD_NIV_B3,		A_NIVBuildSound,	SHIELD_NIV_B2,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_B3,			S_SHIELD_NIV_B4,		A_NIVBuildSound,	SHIELD_NIV_B3,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_B4,			S_SHIELD_NIV_B5,		A_NIVBuildSound,	SHIELD_NIV_B4,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_B5,			S_SHIELD_NIV_1,			A_NIVCompleteSound,	SHIELD_NIV_B5,			NORMAL_STATE, 5 },

   { S_SHIELD_NIV_1,			S_SHIELD_NIV_2,			NULL,				SHIELD_NIV_1,			NORMAL_STATE, 3*TICRATE },
   { S_SHIELD_NIV_2,			S_SHIELD_NIV_3,			NULL,				SHIELD_NIV_2,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_3,			S_SHIELD_NIV_4,			NULL,				SHIELD_NIV_3,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_4,			S_SHIELD_NIV_1,			NULL,				SHIELD_NIV_4,			NORMAL_STATE, 5 },

   { S_SHIELD_NIV_DIE1,			S_SHIELD_NIV_DIE2,		A_NIVBuildSound,	SHIELD_NIV_B5,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_DIE2,			S_SHIELD_NIV_DIE3,		A_NIVBuildSound,	SHIELD_NIV_B4,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_DIE3,			S_SHIELD_NIV_DIE4,		A_NIVBuildSound,	SHIELD_NIV_B3,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_DIE4,			S_SHIELD_NIV_DIE5,		A_NIVBuildSound,	SHIELD_NIV_B2,			NORMAL_STATE, 5 },
   { S_SHIELD_NIV_DIE5,			S_NULL,					NULL,				SHIELD_NIV_B1,			NORMAL_STATE, 5 },

   // Funky bomb thinker
   { S_FUNKYBOMB_THINKER,		S_FUNKYBOMB_THINKER,	A_FunkyBombThinker,	P_FUNKYBOMB,			NORMAL_STATE, 1 },

   // Funky bomb thinker
   { S_FUNKYNUKE_THINKER,		S_FUNKYNUKE_THINKER,	A_FunkyNukeThinker,	P_FUNKYNUKE,			NORMAL_STATE, 1 },

   // OBC Beacon
   { S_OBCBEACON_SHOT,			S_OBCBEACON_SHOT,		NULL,				P_OBCBEACON1,			NORMAL_STATE, 1 },
   { S_OBCBEACON1,				S_OBCBEACON1,			A_OBC_BeaconThinker,P_OBCBEACON1,			NORMAL_STATE, 1 },
   { S_OBCBEACON2,				S_OBCBEACON2,			A_OBC_BeaconThinker,P_OBCBEACON2,			NORMAL_STATE, 1 },

   // Main OBC Thinker
   { S_OBCTHINKER,				S_OBCTHINKER,			NULL,				P_OBCBEACON1,			NORMAL_STATE, 1 },

   // Thin beam going down
   { S_OBCTHINBEAM1,			S_OBCTHINBEAM1,			A_OBC_ThinBeamDown,	P_OBCTHINBEAM1,			NORMAL_STATE, 1 },

   // Thin beam dissapearing
   { S_OBCTHINBEAMFINISH1,		S_OBCTHINBEAMFINISH2,	NULL,				P_OBCTHINBEAM1,			NORMAL_STATE, 20 },
   { S_OBCTHINBEAMFINISH2,		S_OBCTHINBEAMFINISH2,	A_OBCBeamFinish,	P_OBCTHINBEAM2,			NORMAL_STATE, 1 },

   // Medium beam
   { S_OBCMEDIUMBEAM,			S_OBCBIGBEAM1,			A_OBCMedBeamThinker,P_OBCMEDIUMBEAM,		NORMAL_STATE, 1 },
   { S_OBCBIGBEAM1,				S_OBCBIGBEAM2,			A_OBCBigBeamThinker,P_OBCFULLBEAM1,			NORMAL_STATE, 1 },
   { S_OBCBIGBEAM2,				S_OBCBIGBEAM1,			A_OBCBigBeamThinker,P_OBCFULLBEAM2,			NORMAL_STATE, 1 },

   // Ion Flare
   { S_OBCIONFLAME1,			S_OBCIONFLAME2,			A_OBCIonFlameThinker,	P_OBCIONFLAME1,			NORMAL_STATE, 1 },
   { S_OBCIONFLAME2,			S_OBCIONFLAME3,			A_OBCIonFlameThinker,	P_OBCIONFLAME2,			NORMAL_STATE, 1 },
   { S_OBCIONFLAME3,			S_OBCIONFLAME1,			A_OBCIonFlameThinker,	P_OBCIONFLAME3,			NORMAL_STATE, 1 },

   // OBC Animation
   { S_OBCANI1,					S_OBCANI2,				NULL,					ANI_OBC1,				NORMAL_STATE, 8 },
   { S_OBCANI2,					S_OBCANI3,				NULL,					ANI_OBC2,				NORMAL_STATE, 8 },
   { S_OBCANI3,					S_OBCANI4,				NULL,					ANI_OBC1,				NORMAL_STATE, 8 },
   { S_OBCANI4,					S_OBCANI5,				NULL,					ANI_OBC2,				NORMAL_STATE, 8 },
   { S_OBCANI5,					S_OBCANI6,				NULL,					ANI_OBC1,				NORMAL_STATE, 8 },
   { S_OBCANI6,					S_OBCANI7,				NULL,					ANI_OBC2,				NORMAL_STATE, 8 },

   { S_OBCANI7,					S_OBCANI8,				NULL,					ANI_OBC3,				NORMAL_STATE, 8},
   { S_OBCANI8,					S_OBCANI9,				NULL,					ANI_OBC4,				NORMAL_STATE, 8 },
   { S_OBCANI9,					S_OBCANI10,				NULL,					ANI_OBC5,				NORMAL_STATE, 8 },
   { S_OBCANI10,				S_OBCANI11,				NULL,					ANI_OBC6,				NORMAL_STATE, 8 },
   { S_OBCANI11,				S_OBCANI12,				NULL,					ANI_OBC7,				NORMAL_STATE, 8 },
   { S_OBCANI12,				S_OBCANI13,				NULL,					ANI_OBC8,				NORMAL_STATE, 8 },
   { S_OBCANI13,				S_OBCANI14,				NULL,					ANI_OBC9,				NORMAL_STATE, 8 },
   { S_OBCANI14,				S_OBCANI15,				NULL,					ANI_OBC10,				NORMAL_STATE, 8 },
   { S_OBCANI15,				S_OBCANI16,				A_SatRev,				ANI_OBC11,				NORMAL_STATE, 8 },
   { S_OBCANI16,				S_OBCANI17,				NULL,					ANI_OBC12,				NORMAL_STATE, 8 },
   { S_OBCANI17,				S_OBCANI18,				NULL,					ANI_OBC13,				NORMAL_STATE, 8 },
   { S_OBCANI18,				S_OBCANI19,				NULL,					ANI_OBC14,				NORMAL_STATE, 8 },
   { S_OBCANI19,				S_OBCANI20,				NULL,					ANI_OBC15,				NORMAL_STATE, 8 },
   { S_OBCANI20,				S_OBCANI21,				NULL,					ANI_OBC16,				NORMAL_STATE, 8 },
   { S_OBCANI21,				S_OBCANI22,				A_SatFire,				ANI_OBC17,				NORMAL_STATE, 8 },
   { S_OBCANI22,				S_NULL,					NULL,					ANI_OBC18,				NORMAL_STATE, 60 },

   // Super Railgun smoke
   { S_SUPERRAILSMOKE1,			S_SUPERRAILSMOKE2,		NULL,					P_SUPERRAILSMOKE1,	NORMAL_STATE, 4 },
   { S_SUPERRAILSMOKE2,			S_SUPERRAILSMOKE3,		NULL,					P_SUPERRAILSMOKE2,	NORMAL_STATE, 4 },
   { S_SUPERRAILSMOKE3,			S_SUPERRAILSMOKE4,		NULL,					P_SUPERRAILSMOKE3,	NORMAL_STATE, 4 },
   { S_SUPERRAILSMOKE4,			S_SUPERRAILSMOKE5,		NULL,					P_SUPERRAILSMOKE4,	NORMAL_STATE, 4 },
   { S_SUPERRAILSMOKE5,			S_SUPERRAILSMOKE6,		NULL,					P_SUPERRAILSMOKE5,	NORMAL_STATE, 4 },
   { S_SUPERRAILSMOKE6,			S_NULL,					NULL,					P_SUPERRAILSMOKE6,	NORMAL_STATE, 4 },

   // Icicle Spikes #1
   { S_ICESPIKE1_IDLE,			S_ICESPIKE1_IDLE,		NULL,						ICESPIKE1,		NORMAL_STATE, 1 },
   { S_ICESPIKE1_SHAKE1,		S_ICESPIKE1_SHAKE2,		A_IceSpikeShakeInit,		ICESPIKE1,		NORMAL_STATE, 1 },
   { S_ICESPIKE1_SHAKE2,		S_ICESPIKE1_SHAKE2,		A_IceSpikeShake,			ICESPIKE1,		NORMAL_STATE, 1 },
   { S_ICESPIKE1_FALL1,			S_ICESPIKE1_FALL1,		A_IceSpikeFall,				ICESPIKE1,		NORMAL_STATE, 1 },
   { S_ICESPIKE1_DIE,			S_ICESPIKE1_DIE,		A_IceSpikeDie,				ICESPIKE1,		NORMAL_STATE, 1 },

   // Icicle Spikes #2
   { S_ICESPIKE2_IDLE,			S_ICESPIKE2_IDLE,		NULL,						ICESPIKE2,		NORMAL_STATE, 1 },
   { S_ICESPIKE2_SHAKE1,		S_ICESPIKE2_SHAKE2,		A_IceSpikeShakeInit,		ICESPIKE2,		NORMAL_STATE, 1 },
   { S_ICESPIKE2_SHAKE2,		S_ICESPIKE2_SHAKE2,		A_IceSpikeShake,			ICESPIKE2,		NORMAL_STATE, 1 },
   { S_ICESPIKE2_FALL1,			S_ICESPIKE2_FALL1,		A_IceSpikeFall,				ICESPIKE2,		NORMAL_STATE, 1 },
   { S_ICESPIKE2_DIE,			S_ICESPIKE2_DIE,		A_IceSpikeDie,				ICESPIKE2,		NORMAL_STATE, 1 },

   // Icicle Spikes #3
   { S_ICESPIKE3_IDLE,			S_ICESPIKE3_IDLE,		NULL,						ICESPIKE3,		NORMAL_STATE, 1 },
   { S_ICESPIKE3_SHAKE1,		S_ICESPIKE3_SHAKE2,		A_IceSpikeShakeInit,		ICESPIKE3,		NORMAL_STATE, 1 },
   { S_ICESPIKE3_SHAKE2,		S_ICESPIKE3_SHAKE2,		A_IceSpikeShake,			ICESPIKE3,		NORMAL_STATE, 1 },
   { S_ICESPIKE3_FALL1,			S_ICESPIKE3_FALL1,		A_IceSpikeFall,				ICESPIKE3,		NORMAL_STATE, 1 },
   { S_ICESPIKE3_DIE,			S_ICESPIKE3_DIE,		A_IceSpikeDie,				ICESPIKE3,		NORMAL_STATE, 1 },

   // Icicle Shards
   { S_ICESPIKESHARD1_1,		S_ICESPIKESHARD1_1,		A_ShardThinker,				ICESPIKE1_1,	NORMAL_STATE, 1 }, // TL
   { S_ICESPIKESHARD1_2,		S_ICESPIKESHARD1_2,		A_ShardThinker,				ICESPIKE1_2,	NORMAL_STATE, 1 }, // TR
   { S_ICESPIKESHARD1_3,		S_ICESPIKESHARD1_3,		A_ShardThinker,				ICESPIKE1_3,	NORMAL_STATE, 1 }, // BL
   { S_ICESPIKESHARD1_4,		S_ICESPIKESHARD1_4,		A_ShardThinker,				ICESPIKE1_4,	NORMAL_STATE, 1 }, // BR
   { S_ICESPIKESHARD2_1,		S_ICESPIKESHARD2_1,		A_ShardThinker,				ICESPIKE2_1,	NORMAL_STATE, 1 }, // TL
   { S_ICESPIKESHARD2_2,		S_ICESPIKESHARD2_2,		A_ShardThinker,				ICESPIKE2_2,	NORMAL_STATE, 1 }, // TR
   { S_ICESPIKESHARD2_3,		S_ICESPIKESHARD2_3,		A_ShardThinker,				ICESPIKE2_3,	NORMAL_STATE, 1 }, // BL
   { S_ICESPIKESHARD2_4,		S_ICESPIKESHARD2_4,		A_ShardThinker,				ICESPIKE2_4,	NORMAL_STATE, 1 }, // BR
   { S_ICESPIKESHARD3_1,		S_ICESPIKESHARD3_1,		A_ShardThinker,				ICESPIKE3_1,	NORMAL_STATE, 1 }, // T
   { S_ICESPIKESHARD3_2,		S_ICESPIKESHARD3_2,		A_ShardThinker,				ICESPIKE3_2,	NORMAL_STATE, 1 }, // B

   // Ring of Fire
   { S_ROF_PROJECTILE,			S_ROF_PROJECTILE,		A_RotateMissileFast,	P_ROF_PROJECTILE,	NORMAL_STATE, 1 },
   { S_ROF_OPEN1,				S_ROF_OPEN1,			A_ROF_Open1Thinker,		P_ROF_PROJECTILE,	NORMAL_STATE, 1 },
   { S_ROF_OPEN2,				S_ROF_OPEN2,			A_ROF_Open2Thinker,		P_ROF_OPEN1,		NORMAL_STATE, 1 },
   { S_ROF_OPEN3,				S_ROF_OPEN3,			A_ROF_Open3Thinker,		P_ROF_OPEN2,		NORMAL_STATE, 1 },
   { S_ROF_OPEN4,				S_ROF_OPEN4,			A_ROF_Open4Thinker,		P_ROF_OPEN3,		NORMAL_STATE, 1 },
   { S_ROF_FIRE1,				S_ROF_FIRE1,			A_ROF_FireThinker,		P_ROF_FLAME1,		NORMAL_STATE, 1 },
   { S_ROF_FIRE2,				S_ROF_FIRE2,			A_ROF_FireThinker,		P_ROF_FLAME2,		NORMAL_STATE, 1 },
   { S_ROF_FIRE3,				S_ROF_FIRE3,			A_ROF_FireThinker,		P_ROF_FLAME3,		NORMAL_STATE, 1 },

   // Silver Bullet
   { S_SB_PROJECTILE,			S_SB_PROJECTILE,		NULL,					P_SB_PROJECTILE,	NORMAL_STATE, 1 },
   { S_SB_SILVERBULLET,			S_SB_SILVERBULLET,		A_SilverBulletThinker,	P_SB_PROJECTILE,	NORMAL_STATE, 1 },
   { S_SB_MOONSHOT,				S_SB_MOONSHOT,			A_RotateMissileFast,	P_SB_MOONSHOT,	    NORMAL_STATE, 1 },
   { S_SB_FULLMOON,				S_SB_FULLMOON,			A_FullMoonThinker,		P_SB_FULLMOON,	    NORMAL_STATE, 1 },

   // Fuel cylinder
   { S_FUELCYLINDER,			S_FUELCYLINDER,			NULL,					FUELCYLINDER,		NORMAL_STATE, 1 },
   { S_FUELCYLINDER_DIE,	S_FUELCYLINDER_DIE,	A_FuelDie,		FUELCYLINDER,		NORMAL_STATE, 1 },

   // Treasure Chest
   { S_TREASURECHEST,			S_TREASURECHEST,		NULL,					TREASURECLOSED,		NORMAL_STATE, 1 },
   { S_TREASUREOPEN,			S_TREASUREOPEN,			A_ChestOpen,		TREASUREOPEN,		NORMAL_STATE, 1 },

   // float treasure symbol 507
	 { S_TREASURESYMBOL,		S_TREASURESYMBOL,	  A_ChestGold,	CHEST_DOLLAR,			NORMAL_STATE, 15 },

// 	1st state            /Next state       /sprite function  /allegro_BMPnum      /flags       /tics

   // Mag Gen Pod 
   { S_MAGCLOSED,			S_MAGOPEN_1,	A_NIVBuildSound,			MAGCLOSED,	NORMAL_STATE, 10  },
   { S_MAGOPEN_1,			S_MAGOPEN_2,	NULL,									MAGOPEN1,		NORMAL_STATE, 5  },
   { S_MAGOPEN_2,			S_MAGOPEN_3,	A_NIVBuildSound,			MAGOPEN2,		NORMAL_STATE, 5  },
   { S_MAGOPEN_3,			S_MAGOPEN_4,	NULL,									MAGOPEN3,		NORMAL_STATE, 5  },
   { S_MAGOPEN_4,			S_MAGOPEN_5,	A_NIVBuildSound,			MAGOPEN4,		NORMAL_STATE, 5  },
   { S_MAGOPEN_5,			S_MAGOPEN_6,	NULL,									MAGOPEN5,		NORMAL_STATE, 5  },
   { S_MAGOPEN_6,			S_MAGPULSE1,	A_NIVCompleteSound,		MAGOPEN6,		NORMAL_STATE, 5  },
   { S_MAGPULSE1,			S_MAGPULSE2,	NULL,									MAGPULSE1,	NORMAL_STATE, 7  },
   { S_MAGPULSE2,			S_MAGPULSE3,	NULL,									MAGPULSE2,	NORMAL_STATE, 7  },
   { S_MAGPULSE3,			S_MAGPULSE4,	NULL,									MAGPULSE3,	NORMAL_STATE, 7  },
   { S_MAGPULSE4,			S_MAGPULSE5,	NULL,									MAGPULSE4,	NORMAL_STATE, 7  },
   { S_MAGPULSE5,			S_MAGPULSE6,	NULL,									MAGPULSE3,	NORMAL_STATE, 7  },
   { S_MAGPULSE6,			S_MAGPULSE1,	A_MagPuff,						MAGPULSE2,	NORMAL_STATE, 15 },

   { S_MAGDEAD,				S_MAGDEAD,		NULL,									MAGOPEN6,		NORMAL_STATE, 30  },

};

//		====================================================
void A_MagPuff(sprite_t* s) // called when maggen destroyed or open
{ 
// Play us a sound
if( (s->misc1) < 20)s->SetState(S_MAGDEAD);
else
	if( (s->life%30) == 1)StartSound(TANK_PIFT);
}


void A_ChestOpen(sprite_t* s) // called when chest destroyed or chest open
{ 
int ChestType;
int	LifeOfChest = 300;// in frames
int i,j;
float	D,D2,H,HS,S,SW,W,percent[7];


if(s->health == 42)	// Kill the sprite
	{
	if(s->life > LifeOfChest)	// when time is up
		{
		s->Kill();// kill chest NOTE: GOLD = 0  will kill multiple golds;
		} 
	return;
	}
// set health and life values so sprite (open chest) dies regardless of server
// it is display only
s->health = 42; // any number used as a terminate flag.
s->life   = 0;	//	restart life as an open chest
s->yvel   = 2.0; // float away

D=D2=H=HS=S=SW=W=0;

//	the prize is set by the server when game is loaded
//	misc2 is set to the value as used in prize_list
//	the client should not set a random prize.

j = GetPrizeCode(s->misc2);
 // convert index into prize_list into prize code
if(j < 0) // a random selection
	{
	s->misc1 = -j%100; //	ChestType with random index
	if(s->misc1 == 1)	s->misc1 = rand()%7;	// random ChestType 
	s->misc2 = rand()%99;		// random prize value
	}
else
	{
	s->misc1 = j%100;	//	ChestType 0 to 6 inclusive; 7 is empty
	s->misc2 = j/100;	//	prizeIndex 0 to 99
	}
ChestType = s->misc1;

if(ChestType > 6)		// No treasure
	{
	G_Printf("Oh! the Chest is empty!!\n");
	s->life   = LifeOfChest - 200;	// set a short life
	return;
	}

//		dynamically change the sprite to match chest type to the following
// 0:CHEST_DOLLAR 1:CHEST_DOLLAR2 2:CHEST_HEALTH 3:CHEST_HEALTHSHIELD 4:CHEST_SHIELD 5:CHEST_SUPERWEAPON 6:CHEST_WEAPON.
states[S_TREASURESYMBOL].datafilelookup = ChestType+CHEST_DOLLAR;
sprite_t* sgold =	G_SpawnSprite(s->x, s->y , S_TREASURESYMBOL, MF_NORMAL );


// created sprite (but check) and float it upwards
if(!sgold)
	{
	Printf("***** Failed to create Treasure sprite *****\n");
	return;	// Oops no work!!
	}

sgold->tankfrom = tankturn;	//	the winner even if it was done by others
sgold->xvel = 0;
sgold->yvel = -0.5;
sgold->misc1 = ChestType;	// pass on the prize type
sgold->misc2 = s->misc2;	// pass on the prize value
return;
}

void A_ChestGold(sprite_t* sgold)
{ 
int		prizeValue;
int		ChestType;				// the prize catagory
int		winner;
char*	PrizeName;				// as it says
int		LifeOfGold = 300;	// in frames ==> 30 fps by 10 seconds
int		i= -1,j= -1,k= -1;

//	data passed to this sprite
//	tankfrom = tankturn;		//	the winner even if it was done by others
//	misc1 = ChestType;			//	prize type
//	misc2 = s->misc2/100;		//	prize value

PrizeName = "Nothing!! The chest has been robbed!!";
if(sgold->life > LifeOfGold)
	{
	sgold->Kill();	// time out
//	GOLD = 0; // kill all gold stuff
	}

sgold->yvel = -0.3;
if(sgold->opacity == 255)	// this will blink as this is called every 15 frames
	sgold->opacity = 64;
else
	sgold->opacity = 255;
	
if(sgold->life < 90 ) return;	// Show the Prize after 3 seconds
if(sgold->life > 99 ) return;	// life value is not stable 90 < life < 99
	{
	char msg[128];

	sgold->life = 100;	//	make this bit of code more stable and repeatable

	Calc_All_Lists(); // get a list of all weapons,defense & specials.

	ChestType		= sgold->misc1;	//	0 to 6 inclusive
	prizeValue  = sgold->misc2;	//	0 to 99
	winner			= sgold->tankfrom;

	switch(ChestType+CHEST_DOLLAR)
		{
		case CHEST_DOLLAR :
			ta[winner].GiveMoney(2000); PrizeName = "$2,000";
			break;
		case CHEST_DOLLAR2 :
			ta[winner].GiveMoney(20000); PrizeName = "$20,000";
			break;
		case CHEST_HEALTH :
			ta[winner].health +=250; PrizeName = "250 Health Points";
			ta[winner].starthealth = 500;
			if(ta[winner].health > ta[winner].starthealth)
				ta[winner].health = ta[winner].starthealth;	//	so display doesn't overrun.
			break;
		case CHEST_HEALTHSHIELD :
			// get 5 conventional_shields plus get 150 health
			GiveDefense(winner,superconventional_shield , 5);
			ta[winner].health += 150; PrizeName = "150 Health & 5 Shields";
			ta[winner].starthealth = 500;
			if(ta[winner].health > ta[winner].starthealth)
				ta[winner].health = ta[winner].starthealth; 
			break;
		case CHEST_SHIELD :
			if(prizeValue < NUM_DEFENSE)
				{
				j = prizeValue%NUM_DEFENSE;
				GiveDefense(winner,j , 10);
				PrizeName = defenses[j].name;
				}
			if(prizeValue == 9)	//	 Fuel
				{
				ta[winner].specials[prizeValue].amount += 100;
				PrizeName = "Fuel";
				}
			if(prizeValue == 10)	// Laser Sight
				{
				ta[winner].specials[prizeValue].amount += 5;
				PrizeName = "Laser Sights";
				}
			if(prizeValue == 11)	//	Auto Defense
				{
				ta[winner].specials[prizeValue].amount += 5;
				PrizeName = "Auto Defense";
				}
			break;
		case CHEST_SUPERWEAPON :
			j = prizeValue%11;
			switch(j)
				{
				case 0: i = W_GIGANUKE;						break;
				case 1: i = W_MONKEYMORTAR;				break;
				case 2: i = W_POLARISMISSILE;			break;
				case 3: i = W_BLACKHOLEBOMB;			break;
				case 4: i = W_TWINELECTRICLANCES; break;
				//W_TWINELECTRICLANCES_NEGATIVE,
				case 5: i = W_FUNKYNUKE;					break;
				//W_FUNKYNUKECHILD,
				case 6: i = W_AIRSTRIKE;					break;
				case 7: i = W_OBCANNON;						break;
				case 8: i = W_SUPERRAIL;					break;
				//W_SUPERRAILCHILD,
				case 9: i = W_RINGOFFIRE;					break;
				case 10: i = W_SILVERBULLET;			break;
				default : i = W_DEATHSHEAD;
				}
			PrizeName = projectiles[i].name;
			GiveAmmo(winner, i, 1);
			break;
		case CHEST_WEAPON :
			i = prizeValue%num_weapons_items;
			PrizeName = weapons_items[i].name;
			i = weapons_items[i].refnum;
			GiveAmmo(winner, i, 9);	//	give 9 of em
			break;
		default : PrizeName = "Nothing!! The Treasure Chest is Empty!";
			// nothing....
		}
	sprintf(msg,"%s won by %s",PrizeName, ta[winner].name);
	Printf("%s\n",msg);
	k=SpawnGameText(msg, (int)(LifeOfGold), (int)sgold->x, (int)sgold->y, 0.3, -6, makecol(255,50,165) );
	gametext[k].flags = TEXT_FADEOUT;
	}
return;
}

//	======================================================

void A_FuelDie(sprite_t*	s)
{
	int		news;

	news = findshot();
	if (news < 0)
		return;

	sh[news].Init(news, W_BABYNUKE, s->x, s->y, 0, 0, 0);
	sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
									projectiles[sh[news].projectile].state,
									(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));

	// Play a sound
	sh[news].StartSound(projectiles[sh[news].projectile].explodesound);

	// Set state
	sh[news].state = SSTATE_EXPLODING;

	// Reset all counters and set damage varibles. Prepare for explosion
	sh[news].damagedone=1;
	sh[news].damagetics=projectiles[W_BABYNUKE].detonatespeed;
	sh[news].indexrate=0;
	sh[news].colorindex=0;
	sh[news].done=0;

	// Sprite that the missile is attached to is no longer visable
	sh[news].sprite->flags |= MF_NOTVISABLE;

	// Kill the sprite
	s->Kill();

}

void A_SilverBulletThinker(sprite_t* s)
{
	sprite_t*	moon;
	BITMAP*		moonbmp = ((BITMAP *)spritedata[states[S_SB_FULLMOON].datafilelookup].dat);
	int			targetx=0,varx=0;
	int			targety=0,vary=0;
	int			news=-1;
	float		angle;

	// Spawn the moon
	if (s->life == 1*TICRATE)
	{
		moon = G_SpawnSprite((SCREENWIDTH/2)-(moonbmp->w/2), -50, S_SB_FULLMOON, (MF_NORMAL));
		if (moon)
			moon->opacity = 0;

		// Wolf howl
		moon->StartSound(WOLFHOWL);
	}

	// Make the bullet dissapear
	if (s->life == 3*TICRATE)
		s->flags |= MF_NOTVISABLE;

	if (s->life >= 3*TICRATE && s->life <= 7*TICRATE)
	{
		// Fire a shot!
		if (!(s->life % 20))
		{
			// Set variance
			varx = -125+rANDom()%250;		///CheckRandomFunc
			vary = -125+rANDom()%250;		CheckRandomFunc("A_SilverBulletThinker Set variance");

			// Get target coords
			targetx = s->x+varx;
			targety = s->y+vary;

			// Find the angle
			angle = FCalculateAngle(SCREENWIDTH/2, 50, targetx, targety);

			// Spawn a shot
			news = findshot();
			if (news < 0)
				return;

			// Fire lazers straight down
			sh[news].Init(news, W_MOONSHOT,
						  SCREENWIDTH/2,
							         50,
							(-ConvertFromGamePower(300)*COS(angle)),
							(ConvertFromGamePower(300)*-SIN(angle)),
							angle);

			sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
									projectiles[sh[news].projectile].state,
									(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
			if (sh[news].sprite)
				sh[news].sprite->spawnfrom = s;

			sh[news].tankfrom = s->tankfrom;

			// Play a sound
			sh[news].StartSound(projectiles[sh[news].projectile].firesound);
		}
	}

	// Keep the tank timer update
	SetTankTimer(10);

	if (s->life > 9*TICRATE)
		s->Kill();
}

void A_FullMoonThinker(sprite_t* s)
{
	// Fade moon in
	if (s->life < 1.5*TICRATE)
	{
		s->opacity += 3;
		if (s->opacity > 255) { s->opacity = 255; }
	}

	// Fade moon out
	if (s->life > (9-1.5)*TICRATE)
	{
		s->opacity -= 3;
		if (s->opacity < 0) { s->opacity = 0; }
	}
}

void A_ROF_FireThinker(sprite_t* s)
{
	int		burnx,burny;
	int		burna;
	int		varx,vary;
	float	tadist,damagediff;
	float	damage_h=0,damage_e=0,damage_b=0;
	int		i;
	_state	newstate = s->startstate.currentstate;

	// Animate the fire manually
	if (!(s->life % 5))
	{
		if (s->startstate.currentstate == S_ROF_FIRE1)
			newstate = S_ROF_FIRE2;
		else if (s->startstate.currentstate == S_ROF_FIRE2)
			newstate = S_ROF_FIRE3;
		else if (s->startstate.currentstate == S_ROF_FIRE3)
			newstate = S_ROF_FIRE1;

		s->SetState(newstate);
	}

	burna = rand()%255;		// ring o fire does not modify dirt
	burnx = (s->x-COS(burna)*240);
	burny = (s->y-SIN(burna)*240);

	// Factor in some variance
	varx = -20;
	vary = -20;
	varx += rand()%40;
	vary += rand()%40;

	BurnGround(ground.bmp, burnx+varx, burny+vary, 115, 25);

	if (s->life == 0.5*TICRATE)
		s->StartSound(ROF_FIREBURN);

	if (s->life > 1*TICRATE)
	{
		s->opacity += 8;
		if (s->opacity > 255)
			s->opacity = 255;
	}

	// Bring on the PAIN!
	if (s->life == 2*TICRATE)
	{
		// Now, test for radius damage
		// Calculate damage for the tanks (pulled froms shot.cpp)
		for (i=0; i<MAX_TANKS; i++)
		{
			// Make sure we dont meet any of these conditions
			if (ta[i].exist <= 0 || ta[i].health <= 0 || s->tankdamaged[i] == true)
				continue;

			tadist = FindLineDistance(s->x, s->y, ta[i].x, ta[i].y);

			if (tadist > 0)
				damagediff = ((float)s->radius-(float)tadist)/(float)s->radius;

			damagediff = fabs(damagediff-1);

			// Blast is too far away to his this tank,
			// continue with the next one
			if (tadist > (float)s->radius)
			{ continue; }

			damage_h = s->damageh*damagediff;
			damage_e = s->damagee*damagediff;
			damage_b = s->damageb*damagediff;

			// Do the actual damage to zee tank
			ta[i].Damage(damage_h, damage_e, damage_b, s->tankfrom, false);
		}
	}

	// Fade out, ending life
	if (s->life > 4*TICRATE)
	{
		s->opacity -= 10;
		if (s->opacity <= 0)
			s->Kill();
	}
}

void A_ROF_Open1Thinker(sprite_t*	s)
{
	s->angle += 3;

	SetTankTimer(5);

	if (s->life > 1*TICRATE)
	{
		s->life = 0;
		s->SetState(S_ROF_OPEN2);
		s->rotatex = 67;
		s->rotatey = 54;
	}
}

void A_ROF_Open2Thinker(sprite_t*	s)
{
	s->angle += 5;

	SetTankTimer(5);

	if (s->life > 1*TICRATE)
	{
		s->life = 0;
		s->SetState(S_ROF_OPEN3);
		s->rotatex = 159;
		s->rotatey = 55;
	}
}

void A_ROF_Open3Thinker(sprite_t*	s)
{
	s->angle += 7;

	SetTankTimer(5);

	if (s->life > 1*TICRATE)
	{
		s->life = 0;
		s->SetState(S_ROF_OPEN4);
		s->rotatex = 237;
		s->rotatey = 55;
	}
}

void A_ROF_Open4Thinker(sprite_t*	s)
{
	sprite_t	*flame;

	s->angle += 10;

	SetTankTimer(5);

	if (s->life == 1)
	{
		flame = G_SpawnSprite(s->x, s->y, S_ROF_FIRE1, (MF_NORMAL|MF_CENTERCOORD|MF_TRANSLUC));

		// For fade-in effect
		if (flame)
		{
			flame->opacity = 0;

			// Pass the properties over
			flame->damageh = s->damageh;
			flame->damagee = s->damagee;
			flame->damageb = s->damageb;
			flame->radius = s->radius;
			flame->tankfrom = s->tankfrom;
		}
	}

	if (s->life > 4*TICRATE)
		s->Kill();

}

void A_IceSpikeShakeInit(sprite_t*		s)
{
	A_ResetLife(s);

	s->StartSound(CAVESPIKE_SHAKE);
}

void A_IceSpikeDie(sprite_t *s)
{
	BITMAP	*sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	int		sx,sy;

	sprite_t*	shard;

	float		i;
	int			t;
	int			estate;
	int			numshards=0;
	int			shardpower = ConvertFromGamePower(275);
	float		circle_add;

	int			tankx;
	int			tanky;

	// "Suspend" the sprite until its done exploding
	if (!(s->flags & MF_NOGRAVITY))
		s->flags |= MF_NOGRAVITY;

	sx=(s->x)-(sbmp->w/2);
	sy=(s->y)-(sbmp->h/2);

	if (s->life == 1)
	{
		s->flags |= MF_NOTVISABLE;

		/*if (s->startstate.currentstate == S_ICESPIKE1_SHAKE2 ||
			s->startstate.currentstate == S_ICESPIKE2_SHAKE2)
			numshards = 4;
		else if (s->startstate.currentstate == S_ICESPIKE1_SHAKE3)
			numshards = 5;*/

		numshards = 5;

		circle_add = (128/(float)numshards);

		for (i=0; i<128; i+=circle_add)
		{
			estate = S_ICESPIKESHARD1_1+rand()%10;

			shard = G_SpawnSprite(sx+rand()%(sbmp->w), sy+rand()%(sbmp->w), (_state)estate, (MF_NORMAL | MF_CENTERCOORD ));
			shard->xvel = (-shardpower*COS(i));
			shard->yvel = (shardpower*-SIN(i));
		}

		// Spawn an explosion
		if (rand()%2 == 0)
			G_SpawnSprite(s->x, s->y, S_EXPLODE_A1, (MF_NORMAL | MF_CENTERCOORD ));
		else
			G_SpawnSprite(s->x, s->y, S_EXPLODE_B1, (MF_NORMAL | MF_CENTERCOORD ));

		// Dish out some damage
		A_ContactSpriteDamage(s, 10, true);

		// Play a tune
		s->StartSound(CAVESPIKE_SHATTER);

		// Check for any tank contact
		for (t=0; t<MAX_TANKS; t++)
		{
			if (ta[t].health <= 0 || s->tankdamaged[t] == true)
				continue;

			tankx = ta[t].x - (ta[t].sizex/2);
			tanky = ta[t].y - (ta[t].sizey/2);
			if (check_pp_collision(ta[t].bmp,
									sbmp,
									tankx,
									tanky,
									sx,
									sy) > 0)
			{
				ta[t].Damage(s->damageh, s->damagee, s->damageb, s->tankfrom, false);
				s->tankdamaged[t] = true;
			}

		}

		// Do not advance the turn yet
		SetTankTimer(30);

		// Remove the sprite
		s->Kill();

	}
}

void A_IceSpikeShake(sprite_t *s)
{
	_state	newstate;

	if (rand()%2 == 0)
		s->x += 1;
	else
		s->x -= 1;

	SetTankTimer(10);

	// Ready to move on?
	if (s->life >= 1*TICRATE)
	{
		if (s->startstate.currentstate == S_ICESPIKE1_SHAKE2)
			newstate = S_ICESPIKE1_FALL1;

		if (s->startstate.currentstate == S_ICESPIKE2_SHAKE2)
			newstate = S_ICESPIKE2_FALL1;

		if (s->startstate.currentstate == S_ICESPIKE3_SHAKE2)
			newstate = S_ICESPIKE3_FALL1;

		s->SetState(newstate);
	}

}

void A_IceSpikeFall(sprite_t *s)
{
	BITMAP	*sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	_state	newstate;

	// For effects
	sprite_t*		smoke;
	particlenode_t*	p;


	// Remove the "no gravity" flag if not
	// already done so...
	if (s->flags & MF_NOGRAVITY)
		s->flags &= ~MF_NOGRAVITY;

	// Apply some gravity
	s->yvel += GRAVITY;

	if (!(s->life % 2))
	{
		smoke = G_SpawnSprite((int)(s->x-(sbmp->w/2))+rand()%sbmp->w,
							(int)s->y-(sbmp->h/2),
							S_SMOKE_BLACK_MEDIUM,
							(MF_NORMAL | MF_USEANGLE | MF_TRANSLUC));
		if (smoke)
			smoke->opacity = 128;
	}

	if (rand()%4 == 1)
	{
		p = pdirt->InsertParticle((int)(s->x-(sbmp->w/2))+rand()%sbmp->w, (int)s->y-(sbmp->h/2), 64, ConvertFromGamePower(60), makecol(232,218,168), 30+rand()%10, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
		if (p)
			p->bounce = 3;
	}

	// Check to see if this has hit ground yet
	if (s->y >= 0 && getpixel(ground.bmp, s->x, s->y) != TRANSCOLOR)
	{
		if (s->startstate.currentstate == S_ICESPIKE1_FALL1)
			newstate = S_ICESPIKE1_DIE;

		if (s->startstate.currentstate == S_ICESPIKE2_FALL1)
			newstate = S_ICESPIKE2_DIE;

		if (s->startstate.currentstate == S_ICESPIKE3_FALL1)
			newstate = S_ICESPIKE3_DIE;

		s->SetState(newstate);

		A_ResetLife(s);
	}

	SetTankTimer(10);

}

void A_SatRev(sprite_t*	s)
{
	s->StartSound(OBC_SATREV);
}

void A_SatFire(sprite_t*	s)
{
	s->StartSound(OBC_SATFIRE);
}

void A_OBCBeamFinish(sprite_t*	s)
{
	s->opacity -= 20;
	if (s->opacity <= 0)
		s->Kill();
}

void A_OBCIonFlameThinker(sprite_t*	s)
{
	if (s->life > 4*TICRATE)
		s->Kill();

	// Tear the ground up some more
	if (s->life == 1 || s->life == 2 || s->life == 3)
	{
		if (s->life == 1)
			DrawBitmapGroundMask((BITMAP *)spritedata[P_OBCFLAMEBURN1].dat,
									ground.bmp,
									s->x,
									s->y,
									200,
									false);

		DrawGroundSubtract((BITMAP *)spritedata[s->startstate.datafilelookup].dat, ground.bmp, s->x, s->y);
		ground.needsupdate = true;
	}
}

void A_OBCBigBeamThinker(sprite_t*	s)
{
	BITMAP	*bmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;
	BITMAP	*flamebmp = (BITMAP *)spritedata[states[S_OBCIONFLAME1].datafilelookup].dat;
	sprite_t*	ionflame;

	int		i=0;
	int		origx = s->spawnfrom->x;
	float	damage,damagediff;
	float	damage_h=0,damage_e=0,damage_b=0;


	// First, make sure it likes up
	if (s->life == 1)
	{
		s->x = s->spawnfrom->x - (bmp->w/2);

		// Spawn the ion flame for effect
		ionflame = G_SpawnSprite(s->spawnfrom->x-(flamebmp->w/2),
			(s->y+bmp->h)-(flamebmp->h/2), S_OBCIONFLAME1, MF_NORMAL);

		// Burn the ground away from the big beam
		{
			BITMAP	*be = create_bitmap(bmp->w, bmp->h);
			clear_to_color(be, makecol(0,0,0));

			// Make the actual burn into the ground
			DrawBitmapGroundMask(be, ground.bmp, s->x, s->y, 200, false);

			// Remove all the dirt that has gotten into the big beam's way...
			rectfill(ground.bmp, s->x+5, s->y, ((s->x+bmp->w)-5), s->y+bmp->h, TRANSCOLOR);	//	Dirt Sync

			// Refresh the ground so we can see the results
			ground.needsupdate = true;

			// Clear some memory
			destroy_bitmap(be);
		}
	}

	// Time to apply tank damage
	if (s->life == 2)
	{
		// First, find out if any tanks are inside the big beam. If so, apply full
		// damage.
		for (i=0; i<MAX_TANKS; i++)
		{
			// Make sure this is set just as a failsafe
			s->tankdamaged[i] = false;

			// Skip the tank if not valid for damage
			if (ta[i].exist <= 0 || ta[i].health <= 0)
				continue;

			if (ta[i].x > s->x &&
				ta[i].x <= (s->x+bmp->w) &&
				ta[i].y > s->y &&
				ta[i].y <= (s->y+bmp->h))
			{
				// Do full damage
				ta[i].Damage(projectiles[W_OBCANNON].damageh,
							 projectiles[W_OBCANNON].damagee,
							 projectiles[W_OBCANNON].damageb,
							 s->spawnfrom->tankfrom,
							 false);

				// Mark the tank as being damaged so the splash damage from the
				// spark will not damage the tank again
				s->tankdamaged[i] = true;
			}
		}

		// Now, test for radius damage
		// Calculate damage for the tanks (pulled froms shot.cpp)
		for (i=0; i<MAX_TANKS; i++)
		{
			// Make sure we dont meet any of these conditions
			if (ta[i].exist <= 0 || ta[i].health <= 0 || s->tankdamaged[i] == true)
				continue;

			damage = FindLineDistance(s->spawnfrom->x, s->spawnfrom->y, ta[i].x, ta[i].y);

			if (damage > 0)
				damagediff = ((float)s->radius-(float)damage)/(float)s->radius;

			// Test for a **DIRECT HIT** (11 distance is typical direct hit for shields) <-- VERY ugly hack
			if (damage <= 5 || (damage == 11 && ta[i].energyleft > 0))
			{
				// Calculate damage for each damage class
				damage_h = s->damageh;
				damage_e = s->damagee;
				damage_b = s->damageb;
			}
			else
			{
				// Blast is too far away to his this tank,
				// continue with the next one
				if (damage > (float)s->radius)
				{ continue; }

				damage_h = s->damageh*damagediff;
				damage_e = s->damagee*damagediff;
				damage_b = s->damageb*damagediff;
			}

			// Do the actual damage to zee tank
			ta[i].Damage(damage_h, damage_e, damage_b, s->tankfrom, false);
		}

	}



	if (s->life > 4*TICRATE)
	{
		s->spawnfrom->Kill();

		// Find the new bitmap and calculate where to center the new state
		bmp = (BITMAP *)spritedata[states[S_OBCTHINBEAMFINISH1].datafilelookup].dat;
		s->SetState(S_OBCTHINBEAMFINISH1);
		s->x = origx - (bmp->w/2);

		// Make the ground fall
		ground.restartcheck(0,0,0,0);
	}

}

void A_OBCMedBeamThinker(sprite_t*	s)
{
	BITMAP	*bmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;
	BITMAP	*bbbmp = (BITMAP *)spritedata[states[S_OBCBIGBEAM1].datafilelookup].dat;
	sprite_t*	bigbeam;

	int	origx = s->spawnfrom->x+(bmp->w/2);
	int	spawny=0;

	spawny = s->y+30;

	// Make sure it stays in the boundries of the screen limits
	if ((spawny+bbbmp->h) > SCREEN_H)
		spawny = 0;


	if (s->life > 30)
	{
		bigbeam = G_SpawnSprite(origx-(bmp->w/2), spawny, S_OBCBIGBEAM1, MF_NORMAL);
		bigbeam->tankfrom = s->spawnfrom->tankfrom;
		bigbeam->damageh = s->damageh;
		bigbeam->damagee = s->damagee;
		bigbeam->damageb = s->damageb;
		bigbeam->radius = s->radius;
		s->Kill();
	}
}



void A_OBC_ThinBeamDown(sprite_t*	s)
{
	BITMAP	*bmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;

	int	origx = s->x+(bmp->w/2);

	// Stop when misc1 matches up or exceeds the y value
	if ((s->y+bmp->h) < s->misc1)
		s->y += 20;
	else
	{
		// Switch the reference bitmap up...
		bmp = (BITMAP *)spritedata[states[S_OBCMEDIUMBEAM].datafilelookup].dat;

		// ...and set the new state!
		s->SetState(S_OBCMEDIUMBEAM);
		s->x = origx-(bmp->w/2);
		s->life = 0;

		// Make the big beam explosion sound
		s->StartSound(OBC_BEAMFIRE, false, true);
	}
}

void A_OBC_BeaconThinker(sprite_t*	s)
{
	sprite_t*	thinbeam;
	BITMAP*		thinbeambmp = (BITMAP *)spritedata[states[S_OBCTHINBEAM1].datafilelookup].dat;


	// Keep the tank timer updated
	SetTankTimer(30);

	// Time to spawn the animation?
	if (s->life == 30)
		G_SpawnSprite(317, 76, S_OBCANI1, MF_NORMAL);

	if (!(s->life % 60))
		s->StartSound(OBC_BEACONBEEP);


	if (s->life == 6*TICRATE)
	{
		// Spawn the thin beam off the screen and have it quicky come down from the sky
		thinbeam = G_SpawnSprite(s->x-(thinbeambmp->w/2), 0-thinbeambmp->h, S_OBCTHINBEAM1, (MF_NORMAL));
		if (thinbeam)
		{
			thinbeam->misc1 = s->y;
			thinbeam->spawnfrom = s;
			thinbeam->damageh = s->damageh;
			thinbeam->damagee = s->damagee;
			thinbeam->damageb = s->damageb;
			thinbeam->radius = s->radius;
		}

		// Play the beam-down sound here
		s->StartSound(OBC_BEAMREV);
	}

	// Animate the sprite manually
	if (!(s->life % 60) && s->life < 8*TICRATE)
	{
		if (s->startstate.currentstate == S_OBCBEACON1)
			s->SetState(S_OBCBEACON2);
		else
			s->SetState(S_OBCBEACON1);
	}

}

void A_FunkyNukeThinker(sprite_t*	s)
{
	int			numshots = 10;
	int			angle;
	int			length = 10;
	int			power;
	int			i,color;
	int			x;
	int			y;
	int			news;

	if(s->life == 1)
		{
		// Servers will broadcast this info to us
		if(multiplayer && multiplayermode == MP_CLIENT)
			{
			// Start a sound
			s->StartSound(projectiles[W_FUNKYNUKE].explodesound);
			return;
			}

		// Start a sound
		s->StartSound(projectiles[W_FUNKYNUKE].explodesound);

		// Get a good x and y position to spawn from
		x = s->x;
		y = s->y;

		for(i=0; i<numshots; i++)
			{
			news = findshot();

			if(news < 0)		// Problems
				{ return; }

			angle = 40+(rANDom()%(128-80));		///CheckRandomFunc
			power = ConvertFromGamePower(250+rANDom()%300);		CheckRandomFunc("A_FunkyNukeThinker ConvertFromGamePower");

			sh[news].Init(news, W_FUNKYNUKECHILD,
						x,
						y-length,
						(-power*COS(angle)),
						(power*-SIN(angle)),
						angle);

			sh[news].sprite = G_SpawnSprite(x,y, projectiles[sh[news].projectile].state, (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
			sh[news].tankfrom = s->tankfrom;
			//sh[news].InstantHit();

			// Choose a random color
			color = rand()%4;
			if (color == 0)
				sh[news].pal = (PALETTE *)fpurplepal;
			if (color == 1)
				sh[news].pal = (PALETTE *)fredpal;
			if (color == 2)
				sh[news].pal = (PALETTE *)forangepal;
			if (color == 3)
				sh[news].pal = (PALETTE *)fvoiletpal;

			// Send signal to all other clients
			if(multiplayer && multiplayermode == MP_SERVER)
				BCast_Shot(news);
			}

		if(multiplayer && multiplayermode == MP_SERVER)
			{
			BCast_WriteByte(STOC_SPAWNFUNKYTHINKER);
			BCast_WriteByte(1);
			}
		}
	else if(s->life > 1)
		{
		// Once all the shots are gone...
		if(!shotinair())
			{
			news = findshot();
			if(news < 0)
				return;

			sh[news].Init(news, W_NUKE, s->x, s->y, 0, 0, 0);
			sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
											projectiles[sh[news].projectile].state,
											(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));

			// Play a sound
			sh[news].StartSound(projectiles[sh[news].projectile].explodesound);

			// Set state
			sh[news].state = SSTATE_EXPLODING;

			// Reset all counters and set damage varibles. Prepare for explosion
			sh[news].damagedone=1;
			sh[news].damagetics=projectiles[W_BABYNUKE].detonatespeed;
			sh[news].indexrate=0;
			sh[news].colorindex=0;
			sh[news].done=0;

			// Sprite that the missile is attached to is no longer visable
			sh[news].sprite->flags |= MF_NOTVISABLE;

			if(multiplayer && multiplayermode == MP_SERVER)
				{
				BCast_Shot(news);
				}

			// Kill the sprite
			s->Kill();
			}
		}
}

void A_FunkyBombThinker(sprite_t*	s)
{
	int			numshots = 10;
	int			angle;
	int			length = 10;
	int			power;
	int			i,color;
	int			x;
	int			y;
	int			news;

	if(s->life == 1)
		{
		// Servers will broadcast this info to us
		if(multiplayer && multiplayermode == MP_CLIENT)
			{
			// Start a sound
			s->StartSound(projectiles[W_FUNKYBOMB].explodesound);
			return;
			}

		// Start a sound
		s->StartSound(projectiles[W_FUNKYBOMB].explodesound);

		// Get a good x and y position to spawn from
		x = s->x;
		y = s->y;

		for(i=0; i<numshots; i++)
			{
			news = findshot();
			if(news < 0)		// Problems
				{ return; }

			angle = 32+(rANDom()%(128-64));		///CheckRandomFunc
			power = ConvertFromGamePower(225+rANDom()%100);		CheckRandomFunc("A_FunkyBombThinker ConvertFromGamePower");

			sh[news].Init(news, W_FUNKYBOMBCHILD,
						x,
						y-length,
						(-power*COS(angle)),
						(power*-SIN(angle)),
						angle);

			sh[news].sprite = G_SpawnSprite(x,y, projectiles[sh[news].projectile].state, (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
			sh[news].tankfrom = s->tankfrom;
			sh[news].InstantHit();

			// Choose a random color
			color = rand()%4;
			if(color == 0)
				sh[news].pal = (PALETTE *)fpurplepal;
			if(color == 1)
				sh[news].pal = (PALETTE *)fredpal;
			if(color == 2)
				sh[news].pal = (PALETTE *)forangepal;
			if(color == 3)
				sh[news].pal = (PALETTE *)fvoiletpal;

			// Send signal to all other clients
			if(multiplayer && multiplayermode == MP_SERVER)
				{
				BCast_Shot(news);
				}
			}

		if(multiplayer && multiplayermode == MP_SERVER)
			{
			BCast_WriteByte(STOC_SPAWNFUNKYTHINKER);
			BCast_WriteByte(0);
			}
		}
	else if(s->life > 1)
		{
		// Once all the shots are gone..
		if(!shotinair())
			{
			news = findshot();
			if(news < 0)
				return;

			sh[news].Init(news, W_BABYNUKE, s->x, s->y, 0, 0, 0);
			sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
											projectiles[sh[news].projectile].state,
											(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));

			// Play a sound
			sh[news].StartSound(projectiles[sh[news].projectile].explodesound);

			// Set state
			sh[news].state = SSTATE_EXPLODING;

			// Reset all counters and set damage varibles. Prepare for explosion
			sh[news].damagedone=1;
			sh[news].damagetics=projectiles[W_BABYNUKE].detonatespeed;
			sh[news].indexrate=0;
			sh[news].colorindex=0;
			sh[news].done=0;

			// Sprite that the missile is attached to is no longer visable
			sh[news].sprite->flags |= MF_NOTVISABLE;

			if(multiplayer && multiplayermode == MP_SERVER)
				{
				BCast_Shot(news);
				}

			// Kill the sprite
			s->Kill();
			}
		}
}

void A_NIVBuildSound(sprite_t*	s)
{
	s->StartSound(SHIELD_NIVBUILD);
}

void A_NIVCompleteSound(sprite_t*	s)
{
	s->StartSound(SHIELD_NIVREADY);
}

void A_TankFlareThinker(sprite_t*	s)
{
	if (s->life >= s->misc1)
		s->Kill();
}

void A_FireToad_Thinker(sprite_t*	s)
{
	int		news;
	int		stype;
	int		sound = -1;

	if (s->misc1 == 1)
		stype = W_FIRETOAD_P1;
	else
		stype = W_FIRETOAD_P2;

	SetTankTimer(15);//zaq1 was 30

	if (s->life == 15)//zaq1 was 40
	{
		news = findshot();
		if (news < 0)
			return;

		// Fire shells straight down
		sh[news].Init(news, stype, s->x, s->y, s->xvel, s->yvel, 0);

		sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
										projectiles[sh[news].projectile].state,
										(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));

		sh[news].tankfrom = s->tankfrom;

		// Play a bounce sound
		s->StartSound(FIRETOAD_BOUNCE);

		// Spawn an explosion where the firetoad used to be based on the old sprite
		if (s->misc1 == 1)
		{
			stype = W_FIRETOAD;
			sound = TANK_HEAVYEXPLODE1;
		}
		else if (s->misc1 == 2)
		{
			stype = W_FIRETOAD_P1;
			sound = EXPLODE_SMALL;
		}
		else
		{
			stype = W_FIRETOAD_P2;
			sound = EXPLODE_SMALL;
		}

		news = findshot();
		if (news < 0)
			return;

		sh[news].Init(news, stype, s->x, s->y, 0, 0, 0);
		sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
										projectiles[sh[news].projectile].state,
										(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));

		// Play a sound
		if (sound >= 0)
			sh[news].StartSound(sound);

		// Set state
		sh[news].state = SSTATE_EXPLODING;

		// Reset all counters and set damage varibles. Prepare for explosion
		sh[news].damagedone=1;
		sh[news].damagetics=projectiles[stype].detonatespeed;
		sh[news].indexrate=0;
		sh[news].colorindex=0;
		sh[news].done=0;

		// Sprite that the missile is attached to is no longer visable
		sh[news].sprite->flags |= MF_NOTVISABLE;

		// Kill the sprite
		s->Kill();
	}
}

int	GetAirShipX(sprite_t* s, int x)
{
	BITMAP		*bmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;
	static	int	cpos;

	// Moving left to right
	if (s->xvel > 0)
	{
		cpos = bmp->w - x;
		cpos = s->x+cpos;
	}

	else
	{
		cpos = x;
		cpos = s->x+cpos;
	}

	return cpos;
}

void	MP_UpdateAirShip(sprite_t*	s)
{
	if (!multiplayer || multiplayermode != MP_SERVER)
		return;

	BCast_WriteByte(STOC_UPDATEAIRSHIP);
	BCast_WriteByte(s->netid);
	BCast_WriteShort(s->x);
	BCast_WriteShort(s->y);
}

void A_AS_Move(sprite_t*	s)
{
	s->x += s->xvel;
}

void A_AS_FlyerThinker(sprite_t*	s)
{
	// Move with da velocity
	A_AS_Move(s);

	if (!(gametics % 30))
	{
		if (rANDom()%2 == 0)
			s->y ++;
		else
			s->y --;
	}
CheckRandomFunc("A_AS_FlyerThinker");

	// Time to remove from existance?
	if (s->xvel > 0)
	{
		if (s->x > (SCREENWIDTH+50))
			s->Kill();

		s->xvel += GRAVITY/5;
	}
	else
	{
		if (s->x < -50)
			s->Kill();

		s->xvel -= GRAVITY/5;
	}
}


void A_AS_TankBayThinker(sprite_t*	s)
{
	int		dist;
	int		power;
	int		angle;
	int		news;
	int		spawnx,spawny;

	// Figure out some distance stuff
	dist = abs(s->spawnfrom->misc1-(GetAirShipX(s->spawnfrom, 800)));

	// Move the tank bay along with the main flyer
	A_AS_Move(s);

	// Deploy the tank bay?
	if (dist < projectiles[W_AIRSTRIKE].expradius)
	{
		// Reset life counter so we know how long to keep
		// this up for.
		if (s->y < (s->spawnfrom->y+199))
		{
			s->life = 0;

			s->SetState(S_AS_TANKBAY2);
		}
		else if (!(gametics % 45))
		{
			// Power is always the same
			power = ConvertFromGamePower(125);

			if (s->misc2 != 1)
			{
				spawnx = GetAirShipX(s->spawnfrom, 965);
				spawny = s->spawnfrom->y + 208;
				angle = 32;
				s->misc2 = 1;
			}
			else
			{
				spawnx = GetAirShipX(s->spawnfrom, 1055);
				spawny = s->spawnfrom->y + 208;
				angle = 96;
				s->misc2 = 0;
			}


			if (!multiplayer || multiplayermode == MP_SERVER)
			{
				news = findshot();
				if (news < 0)
					return;

				// Fire shells straight down
				sh[news].Init(news, W_URANIUMSHELL,
							spawnx,
							spawny,
							(-power*COS(angle)),
							(power*-SIN(angle)),
							angle);

				sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
												projectiles[sh[news].projectile].state,
												(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
				if (sh[news].sprite)
					sh[news].sprite->spawnfrom = s->spawnfrom;

				sh[news].tankfrom = s->spawnfrom->tankfrom;

				// Play a sound
				sh[news].StartSound(projectiles[W_LEADSHELL].firesound, 0, 10);

				// Update multiplayer position & shot
				MP_UpdateAirShip(s->spawnfrom);
				BCast_Shot(news);
			}
		}
	}

	// Time to remove from existance?
	if (s->xvel > 0)
	{
		if (s->x > (SCREENWIDTH+100))
			s->Kill();
	}
	else
	{
		if (s->x < -100)
			s->Kill();
	}
}

void A_AS_TankBayDeploy(sprite_t*	s)
{
	A_AS_Move(s);

	s->y += 4;

    if (s->y >= (s->spawnfrom->y+199))
	{
		s->y = (s->spawnfrom->y+199);

		s->SetState(S_AS_TANKBAY1);
		s->misc1 = 10;
	}
}

void A_HeatCannonThinker(sprite_t*	s)
{
	int	power=10;
	int	news;
	int	spawnx,spawny;
	int	length = 150;

	// Move the heat barrel with the air ship
	A_AS_Move(s);

	// Fire!
	if (s->life == 59)
	{
		if (!multiplayer || multiplayermode == MP_SERVER)
		{
			spawnx = (s->x-COS(s->angle)*length);
			spawny = (s->y-SIN(s->angle)*length);

			if (getpixel(ground.bmp, spawnx, spawny) != TRANSCOLOR || spawny < s->y)
				return;

			news = findshot();
			if (news < 0)
				return;

			sh[news].Init(news, W_HEATCANNON,
						spawnx,
						spawny,
						(-power*COS(s->angle)),
						(power*-SIN(s->angle)),
						s->angle);

			sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
											projectiles[sh[news].projectile].state,
											(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
			if (sh[news].sprite)
				sh[news].sprite->spawnfrom = s->spawnfrom;

			sh[news].tankfrom = s->tankfrom;

			// Play a sound
			s->StartSound(TANK_FIREHEATCANNON);

			// Update multiplayer position & shot
			MP_UpdateAirShip(s->spawnfrom);
			BCast_Shot(news);
		}

	}
	// Aquire a new target
	else if (s->life == 60)
	{
		float		newangle;

		// Aquire a new target
		s->targetx = s->spawnfrom->misc1 - (projectiles[W_AIRSTRIKE].expradius);
		s->targetx += rANDom()%(projectiles[W_AIRSTRIKE].expradius*2);	CheckRandomFunc("A_HeatCannonThinker Aquire a new target");
		s->targety = OnGround(s->targetx, 0, ground.bmp);

		// Figure out how much the barrel needs to move on each tic
		// to face the new target position
		newangle = FCalculateAngle(s->x, s->y, s->targetx, s->targety);

		s->misc1 = (int)newangle;
		s->deginc = (float)(newangle - s->angle)/60.0;

		// Reset the counter
		s->life = 0;
	}
	else
		s->angle += s->deginc;

	// Time to remove from existance?
	if (s->xvel > 0)
	{
		if (s->x > (SCREENWIDTH+200))
			s->Kill();
	}
	else
	{
		if (s->x < -200)
			s->Kill();
	}
}

void A_AirShipThinker(sprite_t*	s)
{
	sprite_t	*flyer;
	int			flyerflags = (MF_NORMAL);
	BITMAP		*bmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;
	int			dist=0;
	int			power=0;
	int			angle=0;
	int			news=0;

	// Move the airship according to the velocity
	A_AS_Move(s);

	// Keep the timer current
	SetTankTimer(5);

	// Fire lasers?
	dist = abs(s->misc1-(GetAirShipX(s, 615)));

	if (dist < projectiles[W_AIRSTRIKE].expradius)
	{
		if (!multiplayer || multiplayermode == MP_SERVER)
		{
			angle = 192;
			power = ConvertFromGamePower(300);

			if (!(gametics % 60))
			{
				news = findshot();
				if (news < 0)
					return;
				// Fire lazers straight down
				sh[news].Init(news, W_IONBATTERY,
							GetAirShipX(s, 612),
							s->y+178,
							(-power*COS(angle)),
							(power*-SIN(angle)),
							angle);

				sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
												projectiles[sh[news].projectile].state,
												(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
				if (sh[news].sprite)
					sh[news].sprite->spawnfrom = s;

				sh[news].tankfrom = s->tankfrom;

				// Update multiplayer position & shot
				MP_UpdateAirShip(s);
				BCast_Shot(news);

			}
		}
	}


	// Time to drop a nuke?
	if ((abs(GetAirShipX(s, 1175) - s->misc1) <= 2) && s->misc2 != 10)
	{
		if (!multiplayer || multiplayermode == MP_SERVER)
		{
			angle = 192;
			power = ConvertFromGamePower(0);

			news = findshot();
			if (news < 0)
				return;
			// Fire lazers straight down
			sh[news].Init(news, W_NUKE,
						GetAirShipX(s, 1175),
						s->y+161,
						(-power*COS(angle)),
						(power*-SIN(angle)),
						angle);

			sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
											projectiles[sh[news].projectile].state,
											(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
			if (sh[news].sprite)
				sh[news].sprite->spawnfrom = s;

			sh[news].tankfrom = s->tankfrom;

			// Update multiplayer position & shot
			MP_UpdateAirShip(s);
			BCast_Shot(news);

			// This dosen't happen. But give misc2 a value to make
			// certain that two or more nukes aren't dropped
			s->misc2 = 10;
		}
	}


	if (!(gametics %30))
	{
		// Random enough for ya? Spawn a flyer then
	CheckRandomFunc("A_AirShipThinker Spawn a flyer");
		if (rANDom()%3 == 1)		
		{
			flyer = G_SpawnSprite(GetAirShipX(s, 1300), s->y+rANDom()%128, S_AS_FLYER, flyerflags);		///CheckRandomFunc

			if (flyer)
			{
				// Fly from left to right
				if (s->xvel > 0)
				{
					flyer->xvel = 0.15;
					flyer->flags |= MF_FLIPPED;
				}
				else
				{
					flyer->xvel = -0.15;
				}

				// Play flyby sound
				//flyer->StartSound(MINIFIGHTER_FLYBY);
			}
		}
	}



	// Gone off the screen?
	if (s->xvel > 0)	// Moving from left to right
	{
		if (s->x > SCREENWIDTH)
			s->Kill();

		// Stop any playing sounds
		//s->StopSound();
	}
	else				// Moving from right to left
	{
		if ((s->x+bmp->w) < 0)
			s->Kill();

		// Stop any playing sounds
		//s->StopSound();
	}
}


void A_IB_MuzzleFlare(sprite_t*	s, int x, int y)
{
	sprite_t	*mf;
	BITMAP		*flare = (BITMAP *)spritedata[P_IONBATTERY_FLARE1].dat;

	mf = G_SpawnSprite(x-(flare->w/2), y, S_IONBATTERY_FLARE1, MF_NORMAL|MF_NOGRAVITY);
	if (mf)
		mf->xvel = s->xvel;
}

void A_IB_Thinker(sprite_t*	s)
{
	int			power = ConvertFromGamePower(300);
	int			news;

	news = findshot();

	if (news < 0)		// Problems
		{ return; }

	// Server will be spawning this
	if (multiplayer && multiplayermode == MP_CLIENT)
	{
		// Remove the invisable sprite
		s->Kill();
		return;
	}

	if (s->life == 1)
	{

		if (s->spawnfrom)
		{
			s->x = GetAirShipX(s->spawnfrom, 662);
			s->y = s->spawnfrom->y+180;
		}

		// Fire lazers straight down
		sh[news].Init(news, W_IONBATTERYCHILD,
					s->x,
					s->y,
					(-power*COS(s->angle)),
					(power*-SIN(s->angle)),
					s->angle);

		sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
										projectiles[sh[news].projectile].state,
										(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
		sh[news].tankfrom = s->tankfrom;

		// Spawn a muzzle flare on top
		if (s->spawnfrom)
			A_IB_MuzzleFlare(s->spawnfrom, s->x, s->spawnfrom->y+175);

		// Play a sound
		s->StartSound(projectiles[W_IONBATTERYCHILD].firesound);

		if (multiplayer && multiplayermode == MP_SERVER)
			BCast_Shot(news);
	}


	if (s->life == 15)
	{
		if (s->spawnfrom)
		{
			s->x = GetAirShipX(s->spawnfrom, 612);
			s->y = s->spawnfrom->y+180;
		}

		// Fire lazers straight down
		sh[news].Init(news, W_IONBATTERYCHILD,
					s->x,
					s->y,
					(-power*COS(s->angle)),
					(power*-SIN(s->angle)),
					s->angle);

		sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
										projectiles[sh[news].projectile].state,
										(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
		sh[news].tankfrom = s->tankfrom;

		// Spawn a muzzle flare on top
		if (s->spawnfrom)
			A_IB_MuzzleFlare(s->spawnfrom, s->x, s->spawnfrom->y+175);

		// Play a sound
		s->StartSound(projectiles[W_IONBATTERYCHILD].firesound);

		if (multiplayer && multiplayermode == MP_SERVER)
			BCast_Shot(news);
	}


	if (s->life == 30)
	{
		if (s->spawnfrom)
		{
			s->x = GetAirShipX(s->spawnfrom, 562);
			s->y = s->spawnfrom->y+180;
		}

		// Fire lazers straight down
		sh[news].Init(news, W_IONBATTERYCHILD,
					s->x,
					s->y,
					(-power*COS(s->angle)),
					(power*-SIN(s->angle)),
					s->angle);

		sh[news].sprite = G_SpawnSprite(sh[news].x,sh[news].y,
										projectiles[sh[news].projectile].state,
										(MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
		sh[news].tankfrom = s->tankfrom;

		// Spawn a muzzle flare on top
		if (s->spawnfrom)
			A_IB_MuzzleFlare(s->spawnfrom, s->x, s->spawnfrom->y+175);

		// Play a sound
		s->StartSound(projectiles[W_IONBATTERYCHILD].firesound);

		if (multiplayer && multiplayermode == MP_SERVER)
			BCast_Shot(news);
	}

	// Keep updating
	checktanktimer = 5;

	if (s->life > 31)
		s->Kill();
}

void A_ASFlareThinker(sprite_t*	s)
{
	sprite_t	*smoke;
	sprite_t	*as;
	sprite_t	*fc;
	sprite_t	*tb;
	BITMAP		*bmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;
	BITMAP		*airship = (BITMAP *)spritedata[P_AS_AIRSHIP].dat;
	int			asx,asy;

	int			sx,sy;

	// Set some varibles
	if (!(gametics % 2))
	{
		sx = s->x - (bmp->w/2);
		sy = s->y - (bmp->h/2);

		smoke = G_SpawnSprite(sx+(rand()%bmp->w),
			sy+(rand()%bmp->h),
			S_AS_FLARESMOKE,
			(MF_NORMAL|MF_USEANGLE|MF_TRANSLUC));
		if (smoke)
		{
			smoke->opacity = 192;
		}
	}

	// Time to spawn the airship?
	if (s->life == 3*TICRATE && (!multiplayer || multiplayermode == MP_SERVER))
	{
		// Spawn the actual air ship that will carry out the rest of the
		// sequence.

		// First, figure out which side the airship will enter from.
		// If the flare is on the left side of the screen, enter from
		// the left.
		if (s->x < (SCREENWIDTH/2))
		{
			asx = SCREENWIDTH+1;
			asy = -60;

			fc = G_SpawnSprite(asx+876, asy+164, S_HEATCANNON_THINKER, (MF_NORMAL));
			tb = G_SpawnSprite(asx+954, asy+165, S_AS_TANKBAY1, (MF_NORMAL));
			as = G_SpawnSprite(asx, asy, S_AS_AIRSHIP, (MF_NORMAL));

			if (as)
			{
				as->xvel = -0.93;
				as->spawnfrom = s;
				as->tankfrom = s->tankfrom;
				as->misc1 = s->x;

				if (fc)
				{
					// Inherit same attributes
					fc->spawnfrom = as;
					fc->xvel = as->xvel;
					fc->tankfrom = as->tankfrom;

					fc->rotatex = 136;
					fc->rotatey = 16;
					fc->angle = 230;
				}

				if (tb)
				{
					// Inherit some more attributes
					tb->spawnfrom = as;
					tb->xvel = as->xvel;
					tb->tankfrom = as->tankfrom;
				}

				// Start the looped air strike sound
				//as->StartSound(AIRSHIP, 1);

				if (multiplayer && multiplayermode == MP_SERVER)
				{
					BCast_WriteByte(STOC_SPAWNAIRSHIP);
					BCast_WriteShort(as->netid);
					BCast_WriteByte(as->wobj);
					BCast_WriteShort(as->x);
					BCast_WriteShort(as->y);
					BCast_WriteFloat(as->xvel);
					BCast_WriteFloat(as->yvel);
					BCast_WriteShort((int)as->startstate.currentstate);
					BCast_WriteShort(as->tankfrom);
					BCast_WriteShort(as->flags);
					BCast_WriteShort(as->misc1);
				}
			}
		}
		else
		{
			asx = 0-(airship->w);
			asy = -60;

			fc = G_SpawnSprite(asx+(airship->w-876), asy+164, S_HEATCANNON_THINKER, (MF_NORMAL));
			tb = G_SpawnSprite(asx+(airship->w-(954+113)), asy+165, S_AS_TANKBAY1, (MF_NORMAL|MF_FLIPPED));
			as = G_SpawnSprite(asx, asy, S_AS_AIRSHIP, (MF_NORMAL|MF_FLIPPED));

			if (as)
			{
				as->xvel = 0.93;
				as->spawnfrom = s;
				as->tankfrom = s->tankfrom;
				as->misc1 = s->x;


				if (fc)
				{
					// Inherit same attributes
					fc->spawnfrom = as;
					fc->xvel = as->xvel;
					fc->tankfrom = as->tankfrom;

					fc->rotatex = 136;
					fc->rotatey = 16;
					fc->angle = 230;
				}

				if (tb)
				{
					// Inherit some more attributes
					tb->spawnfrom = as;
					tb->xvel = as->xvel;
					tb->tankfrom = as->tankfrom;
				}

				// Start the looped air strike sound
				//as->StartSound(AIRSHIP, 1);


				if (multiplayer && multiplayermode == MP_SERVER)
				{
					BCast_WriteByte(STOC_SPAWNAIRSHIP);
					BCast_WriteShort(as->netid);
					BCast_WriteByte(as->wobj);
					BCast_WriteShort(as->x);
					BCast_WriteShort(as->y);
					BCast_WriteFloat(as->xvel);
					BCast_WriteFloat(as->yvel);
					BCast_WriteShort((int)as->startstate.currentstate);
					BCast_WriteShort(as->tankfrom);
					BCast_WriteShort(as->flags);
					BCast_WriteShort(as->misc1);
				}
			}
		}
	}


	// Finished? (do this first to prevent smoke from being spawned after 5 secs)
	if (s->life > 5*TICRATE)
	{
		s->Kill();
		//s->flags = MF_NOTVISABLE;
		return;
	}
}


void A_ASFlareSmoke(sprite_t*	s)
{
	s->angle+=1;
	if (!(s->life % 2))
		s->opacity-=1;
	s->scale += 0.015;

	if (s->opacity <= 0)
	{
		s->Kill();
		return;
	}

	if (s->angle >= 256)
		s->angle = 0;

	// Go into the sky really really fast
	s->y -= 8.25;

	// Effects
	s->opacity-=3;
	s->scale += 0.045;
}

void A_TEL_PositiveThinker(sprite_t*	s)
{
	BITMAP			*damagebmp = (BITMAP *)spritedata[LIGHTNINGROD_HSDAMAGE].dat;
	sprite_t*		check = sprites;
	particlenode_t	*p;
	int				sstate;
	float			sa;
	int				sx,sy;
	int				whitevalue;
	int				dist;
	int				numburn = 15;


	// For lightning strikes
	int		spritew,spriteh;
	int		spritex,spritey;
	int		sdamageh,sdamagee,sdamageb;
	int		i;
	int		tankx,tanky;
	int		spriteunderwater=0;

	// Particle effect
	int		nump = 40;
	int		plife;
	int		pangle;
	//////////////////////////////////////////////

	// Already found our mate?
	if (s->spawnfrom)
		goto proceed;

	while (check)
	{
		sstate = check->startstate.currentstate;

		if (sstate == S_TEL_NEGATIVE)
		{
			// Link the two together so its easier to keep track of them
			s->spawnfrom = check;
			check->spawnfrom = s;

			// Finish up
			goto proceed;
		}

		check = check->next;
	}

	// Reset the life so as to keep in sync with our "mate"
	s->life = 0;
	return;

// There is a negative charge? Proceed then...
proceed:

	if (s->life == 1)
	{
		SetTankTimer(10*TICRATE);

		StartSound(TEL_THUNDER);
	}

	// Calculate distances
	dist = s->spawnfrom->x - s->x;

	if (dist == 0)
		dist = 1;

	// Between 1 tic and 60 tics (before 2 seconds) Spawn electric particles and darken
	// the scenery
	if (s->life > 1 &&
		s->life < 2*TICRATE)
	{

		if (s->life <= (TICRATE/3))
		{
			//currentlevel.background_lightlevel = 40;
			currentlevel.background_lightlevel -= 11;

			//currentlevel.foreground_lightlevel = 100;
			currentlevel.foreground_lightlevel -= 9;

			ground.needsupdate = true;
			return;
		}

		// Spawn a particle
		if (rand()%3 == 0 &&
			dist != 0)
		{
			// Positive is on the left side?
			if (dist > 10)
			{
				sx = s->x;// + rand()%dist;
				//sy = (s->y+(5+rand()%10));
				sy = s->y-rand()%5;

				sa = CalculateAngle(s->x, s->y, s->spawnfrom->x, s->spawnfrom->y);

				// Spawn the particle
				p = pdirt->InsertParticle(sx, sy, sa,
					ConvertFromGamePower(500+rand()%400),
					makecol(128,128,255),
					((8*TICRATE - s->life)-rand()%(1*TICRATE)),
					PFLAGS_STATIC|PFLAGS_TELBOUNCE|PFLAGS_NOGRAVITY|PFLAGS_DIRT, 0, 0);

				if (p)
				{
					p->telbounce1x = s->x;
					p->telbounce1y = s->y;
					p->telbounce2x = s->spawnfrom->x;
					p->telbounce2y = s->spawnfrom->y;
				}
			}

			// Positive is on the right side?
			else
			{
				sx = s->spawnfrom->x;// + rand()%dist;
				sy = s->spawnfrom->y-rand()%5;

				sa = CalculateAngle(s->spawnfrom->x, s->spawnfrom->y, s->x, s->y);

				// Calculate white value
				whitevalue = 128+rand()%128;

				// Spawn the particle
				p = pdirt->InsertParticle(sx, sy, sa,
					ConvertFromGamePower(500+rand()%400),
					makecol(whitevalue,whitevalue,255),
					((8*TICRATE - s->life)-rand()%(1*TICRATE)),
					PFLAGS_STATIC|PFLAGS_TELBOUNCE|PFLAGS_NOGRAVITY|PFLAGS_DIRT, 0, 0);

				if (p)
				{
					p->telbounce1x = s->spawnfrom->x;
					p->telbounce1y = s->spawnfrom->y;
					p->telbounce2x = s->x;
					p->telbounce2y = s->y;
				}
			}
		}
	}

	// Spawn some random lightning strikes between 3 and 7 seconds (3.75 strikes per second)
	if (!multiplayer || multiplayermode == MP_SERVER)
	{
		if (s->life > 3*TICRATE && s->life < 7*TICRATE)
		{

			if (!(gametics % 16))
			{
				sprite_t* l;
				int		groundy;

				// Positve rod on the left side? Find a random spot to spawn lightning
				if (dist >= 1)
				{
					sx = (s->x + rANDom()%dist)-15;		CheckRandomFunc("left side?  random spot to spawn lightning");
					groundy = OnGround(sx, 0, ground.bmp);
					sy = (groundy-SCREENHEIGHT);
				}

				// Positive rod on the right side?
				else
				{
					sx = (s->spawnfrom->x + rANDom()%dist)-15;	CheckRandomFunc("right side?  random spot to spawn lightning");
					groundy = OnGround(sx, 0, ground.bmp);
					sy = (groundy-SCREENHEIGHT);
				}

				l = G_SpawnSprite(sx, sy, S_LIGHTNINGROD_HELLSTRIKE1, (MF_NORMAL));

				// Spawning underwater causes tanks to take much more damage (1/3 more)
				if (Underwater(sx, groundy))
					spriteunderwater = 1;

				// Define all the base varibles here since they can be changed
				spritew = ((BITMAP *)spritedata[l->startstate.datafilelookup].dat)->w;
				spriteh = ((BITMAP *)spritedata[l->startstate.datafilelookup].dat)->h;
				spritex = l->x;
				spritey = l->y;
				sdamageh = projectiles[W_TWINELECTRICLANCES_NEGATIVE].damageh;
				sdamagee = projectiles[W_TWINELECTRICLANCES_NEGATIVE].damagee;
				sdamageb = projectiles[W_TWINELECTRICLANCES_NEGATIVE].damageb;

				// Broadcast exact lightning strike positions to other clients
				if (multiplayer && multiplayermode == MP_SERVER)
				{
					BCast_WriteByte(STOC_HELLSTRIKE);
					BCast_WriteShort(sx);
					BCast_WriteShort(sy);
				}

				// Adjust some varibles if shooting lightning underwater
				if (spriteunderwater > 0)
				{
					// Wider radius
					spritew *= 2;
					spritex = sx-(spritew/2);
				}

				// Oh yeah, might wanna burn the ground
				for (i=0; i<numburn; i++)
				{
					int		burnx;
					int		burny;

					burnx = (sx-(damagebmp->w/2));
					burny = groundy;//(groundy-(damagebmp->h/2));

					burnx += rand()%damagebmp->w;
					burny += rand()%damagebmp->h;

					BurnGround(ground.bmp, burnx, burny, 20+rand()%10, 125+rand()%35);
				}

				// White flash!
				currentlevel.whiteflashtics = 1;

				DrawGroundSubtract(damagebmp, ground.bmp, sx-(damagebmp->w/2), groundy+5);

				for (i=0; i<MAX_TANKS; i++)
				{
					if (ta[i].health <= 0)// || l->tankdamaged[i] == true)
						continue;

					tankx = ta[i].x - (ta[i].sizex/2);
					tanky = ta[i].y - (ta[i].sizey/2);
					if (check_bb_collision_general( tankx,
											tanky,
											20,//ta[i].sizex,
											14,//ta[i].sizey,
											spritex,
											spritey,
											spritew,
											spriteh) == true)
					{
						// The
						if (ta[i].underwater && spriteunderwater > 0)
							ta[i].Damage(sdamageh+(sdamageh/3), sdamagee+(sdamagee/3), sdamageb+(sdamageb/3), s->tankfrom, false);
						else
							ta[i].Damage(sdamageh, sdamagee, sdamageb, s->tankfrom, false);

						s->tankdamaged[i] = true;
					}
				}

				// Attack other sprites as well
				A_ContactSpriteDamage(s, 100, true);

				// Spawn some particles, just for looks
				for (i=0; i<nump; i++)
				{
					// Set particle life, randomize this a bit
					plife = 120+rand()%30;

					// Randomize the angle as well
					pangle = 45+rand()%40;

					p = pdirt->InsertParticle(sx, groundy-10, rand()%128, ConvertFromGamePower(400+rand()%100), makecol(200,200,255), plife, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
					if (p)
						p->bounce = 5;
				}

				// Boom sound!
				StartSound(TEL_LIGHTNING);

			} // end if multiplayer or server
		}
	}

	// Return the light level to normal
	if (s->life > 8*TICRATE)
	{
		if (s->life > (TICRATE*8) && s->life < (TICRATE*10))
		{
			currentlevel.background_lightlevel +=7;
			currentlevel.foreground_lightlevel +=5;
			ground.needsupdate = true;
			return;
		}
	}

	// Finish up
	if (s->life == 10*TICRATE)
	{
		int		nump = 16;
		currentlevel.background_lightlevel = 255;
		currentlevel.foreground_lightlevel = 255;
		ground.needsupdate = true;

		// Collaspe the ground
		ground.restartcheck(0,0,0,0);

		// Pift effect for the two lances
		//
		// Play us a sound
		StartSound(TANK_PIFT);

		// Spawn some particles, just for looks
		for (i=0; i<nump; i++)
		{
			// Set particle life, randomize this a bit
			plife = 30+rand()%30;

			// Randomize the angle as well
			pangle = 45+rand()%40;

			p = pdirt->InsertParticle(s->x, s->y-10, rand()%128, ConvertFromGamePower(300+rand()%100), makecol(255,255,255), plife, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
			if (p)
				p->bounce = 5;

			p = pdirt->InsertParticle(s->spawnfrom->x, s->spawnfrom->y-10, rand()%128, ConvertFromGamePower(300+rand()%100), makecol(255,255,255), plife, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
			if (p)
				p->bounce = 5;
		}

		// Kill off both lances
		s->spawnfrom->Kill();
		s->Kill();

	}

}

void A_TEL_NegativeThinker(sprite_t*	s)
{
	if (s->life == 1)
		SetTankTimer(10*TICRATE);
}

void A_BH_MatterFall(sprite_t* s)
{
	float		damage,damagediff;
	float		damage_h=0,damage_e=0,damage_b=0;
	int			i;

	s->yvel += (GRAVITY/2);

	s->y += s->yvel;

	SetTankTimer(30);

	if (getpixel(ground.bmp, s->x, s->y) != TRANSCOLOR)
	{
		// Shake the screen a bit
		SetShakeTimer(15, 4);

		// Spawn a explosion
		G_SpawnSprite(s->x, s->y, S_SMALLEXPLODE_B1, (MF_NORMAL | MF_CENTERCOORD ));

		// Play a sound
		s->StartSound(BHB_SMASH);

		// Collaspe the groundage
		ground.restartcheck(0,0,0,0);

		// Calculate damage for the tanks (pulled froms shot.cpp)
		for (i=0; i<MAX_TANKS; i++)
		{
			if (ta[i].exist <= 0 || ta[i].health <= 0)
				continue;

			damage = FindLineDistance(s->x, s->y, ta[i].x, ta[i].y);

			if (damage > 0)
				damagediff = ((float)s->radius-(float)damage)/(float)s->radius;

			// Test for a **DIRECT HIT** (11 distance is typical direct hit for shields) <-- VERY ugly hack
			if (damage <= 5 || (damage == 11 && ta[i].energyleft > 0))
			{
				// Calculate damage for each damage class
				damage_h = s->damageh;
				damage_e = s->damagee;
				damage_b = s->damageb;
			}
			else
			{
				// Blast is too far away to his this tank,
				// continue with the next one
				if (damage > (float)s->radius)
				{ continue; }

				damage_h = s->damageh*damagediff;
				damage_e = s->damagee*damagediff;
				damage_b = s->damageb*damagediff;
			}

			// Do the actual damage to zee tank
			ta[i].Damage(damage_h, damage_e, damage_b, s->tankfrom, false);
		}


		// Kill off the sprite
		s->Kill();
	}
}

void A_BH_ImplodeWave(sprite_t *s)
{
	s->scale-= 0.07;

	s->opacity += 3;

	if (s->scale <= 0)
		s->Kill();
}

void A_BH_CloudThinker1(sprite_t	*s)
{
	// Make the sprite visable
	if (s->life < TICRATE &&
		s->opacity < 128)
	{
		s->opacity += 2;
		if (s->opacity > 128)
			s->opacity = 128;
	}


	if (s->life > ((7.5*TICRATE)-(90+TICRATE)))
	{
		s->opacity -= 2;


		if (s->opacity <= 0)
		{
			s->opacity = 0;
			s->Kill();
			return;
		}
	}

	s->angle += s->xvel;
	if (s->angle > 255)
		s->angle = 0;
}


void A_BH_CloudThinker2(sprite_t	*s)
{
	// Make the sprite visable
	if (s->life < TICRATE &&
		s->opacity < 192)
	{
		s->opacity += 2;
		if (s->opacity > 192)
			s->opacity = 192;
	}

	if (s->life > ((7.5*TICRATE)-(120+TICRATE)))
	{
		ground.specialbmp = (BITMAP *)maindata[BHBOMB_BACKGROUND].dat;
		ground.specialopacity -= 5;
		if (ground.specialopacity < 0)
		{
			ground.specialopacity = 0;
			ground.specialtics = 0;
		}

		s->opacity -= 2;
		if (s->opacity <= 0)
		{
			s->opacity = 0;
			s->Kill();
			return;
		}
	}

	s->angle -= s->xvel;
	if (s->angle < 0)
		s->angle = 255;
}

void A_BH_Thinker(sprite_t	*s)
{
	sprite_t*	cl;
	int			i;
	int			pullx;
	int			pully;
	int			pullw,pullh;

	s->angle += 0.25;
	if (s->angle > 255)
		s->angle = 0;

	if (s->startstate.currentstate == S_BHBOMB_BLOB1 && s->life >= 30)
		s->SetState(S_BHBOMB_BLOB2);

	if (s->startstate.currentstate == S_BHBOMB_BLOB2 && s->life >= 60)
		s->SetState(S_BHBOMB_BLOB3);

	if (s->startstate.currentstate == S_BHBOMB_BLOB3 && s->life >= 90)
		s->SetState(S_BHBOMB_BLOB4);

	if (s->life > 60 && s->life < 60+64)
	{
		ground.specialbmp = (BITMAP *)maindata[BHBOMB_BACKGROUND].dat;
		ground.specialtics = 9*TICRATE;
		ground.specialopacity += 4;
		if (ground.specialopacity > 255)
			ground.specialopacity = 255;
	}

	if (s->life == 61)
		s->StartSound(BHB_OPEN);

	if (s->life == 61+36)
		s->StartSound(BHB_LOOP, true, true);

	if (s->life == 7.5*TICRATE)
		s->StartSound(BHB_CLOSE, false, true);

	// Calculate the pull radius
	pullx = s->x-((s->radius*3.5)/2);
	pully = s->y-((s->radius*3.5)/2);
	pullw = pullh = s->radius*3.5;

	// Pull tanks toward us
	if (!(gametics % 3) && (!multiplayer || multiplayermode == MP_SERVER))
	{
		for (i=0; i<MAX_TANKS; i++)
		{
			if (ta[i].exist <= 0 || ta[i].health <= 0)
					continue;

			// If tanks are inside the pull box and outside the black hole box,
			// pull them towards the center of the screen via move.

			// Pull towards the right
			if (ta[i].x > pullx &&
				ta[i].x < (s->x-25))
			{
				ta[i].move(1,0);
			}

			// Pull towards the right
			if (ta[i].x < pullx+pullw &&
				ta[i].x > (s->x+25))
			{
				ta[i].move(-1,0);
			}

		}
	}

	// Tear some land away
	if (!(s->life % 2) && s->life > 90)
	{
		int		px,py,pa;
		int		i;
		particlenode_t	*p;
		int		pcolor;

		for (i=0; i<256; i+=10)
		{
			int		extral = (rANDom()%50);		CheckRandomFunc("A_BH_Thinker blackhole radius");

			px = ((s->x)-COS(i)*(s->radius+extral));
			py = ((s->y)-SIN(i)*(s->radius+extral));

			//if (OnScreen(0, 0, SCREEN_W-1,SCREEN_H-1) <= 0)
			//	continue;

			pcolor = getpixel(ground.bmp, px, py);

			if (pcolor == -1 || pcolor == TRANSCOLOR)
				continue;

			pa = CalculateAngle(px, py, s->x, s->y);

			// Spawn the particle
			p = pdirt->InsertParticle(px, py, pa,
				ConvertFromGamePower(400+rand()%100),
				pcolor,
				3*TICRATE,
				PFLAGS_STATIC|PFLAGS_USEDESTINATION|PFLAGS_NOGRAVITY|PFLAGS_DIRT, 0, 0);

			if (p)
			{
				p->destx = s->x - 10;
				p->desty = s->y - 10;
				p->destw = p->desth = 20;
			}
		}

		s->radius+=1;

		// Pull some pixels
		DestroyDirt(s->x, s->y, s->radius);	//	blackhole bomb
		ground.needsupdate = true;
	}


	// Spawn some spinning clouds
	/*if (s->life == 90)
	{
		cl = G_SpawnSprite(s->x, s->y, S_BHBOMB_CLOUDS1, (MF_NORMAL|MF_USEANGLE|MF_TRANSLUC|MF_NOGRAVITY));
		if (cl)
		{
			cl->opacity = 0;
			cl->xvel = 1.2;
		}
	}*/

	if (s->life == 120)
	{
		cl = G_SpawnSprite(s->x, s->y, S_BHBOMB_CLOUDS2, (MF_NORMAL|MF_USEANGLE|MF_TRANSLUC|MF_NOGRAVITY));
		if (cl)
		{
			cl->opacity = 0;
			cl->xvel = 0.5;
			cl->scale = 1.5;
		}
	}


	if (s->life == 7.5*TICRATE)
	{
		s->SetState(S_BHBOMB_SHRINK1);

		SetTankTimer(2*TICRATE);
	}
}

void A_BHBomb_Open(sprite_t	*s)
{
	sprite_t*	blob;

	if (s->startstate.currentstate == S_BHBOMB_OPEN1)
	{
		s->angle += 15;
		if (s->angle > 255)
			s->angle = 255-s->angle;

		if (s->life > 25)
			s->SetState(S_BHBOMB_OPEN2);
	}
	else if (s->startstate.currentstate == S_BHBOMB_OPEN2)
	{
		s->angle += 20;
		if (s->angle > 255)
			s->angle = 255-s->angle;

		if (s->life > 50)
			s->SetState(S_BHBOMB_OPEN3);
	}
	else if (s->startstate.currentstate == S_BHBOMB_OPEN3)
	{
		s->angle += 30;
		if (s->angle > 255)
			s->angle = 255-s->angle;
		if (s->life == 1.5*TICRATE)
			{
			blob = G_SpawnSprite(s->x, s->y, S_BHBOMB_BLOB1, (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE));
			if (blob)
			{
				blob->damageh = s->damageh;
				blob->damagee = s->damagee;
				blob->damageb = s->damageb;
				blob->tankfrom = s->tankfrom;
				blob->radius = 0;
			}
		}

		if (s->life > 2.5*TICRATE)
			s->Kill();
	}
}


void A_IceShardThinker(sprite_t *s)
{
	s->yvel += GRAVITY;

	// Do some velocity stuffage
	s->x += s->xvel;
	s->y += s->yvel;

	// Check to see if this has hit ground yet
	if (!OnScreen(buffer, s->x, s->y))
	{
		s->Kill();
	}
}

void	A_NitrogenThinker(sprite_t*	s)
{
	int			i;
	int			sx,sy;
	float		sw;
	float		sh;
	BITMAP		*bmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;

	s->opacity-=2;
	s->scale += 0.045;

	s->x += s->xvel;
	s->y += s->yvel;

	if (s->angle >= 256)
		s->angle = 0;

	if (s->opacity <= 0)
	{
		s->Kill();
		return;
	}

	// Not enough opacity to realisticly do any damage
	if (s->opacity < 30)
		return;

	// Determine we're hitting any tanks (bounding box only)
	// First, find out what the sprite's actual width and height are
	sw = bmp->w*s->scale;
	sh = bmp->h*s->scale;

	// Now, determine where the X and Y coordinates are for rendering
	sx = s->x - (sw/2);
	sy = s->y - (sh/2);

	// Now, find out if any tanks are within the smoke
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || ta[i].health <= 0)
			continue;

		if (sx < ta[i].x &&
			sy < ta[i].y &&
			sx+sw > ta[i].x &&
			sy+sh > ta[i].y &&
			s->tankdamaged[i] == false)
		{
			// Yeah, apply smoke nitrogen damage
			//ta[i].reservedamageh += 2;

			s->tankdamaged[i] = true;

			ta[i].frozen += 13;

			if (ta[i].frozen > 255)
				ta[i].frozen = 255;
		}
	}
}


void A_ApplyFrozenDamage(int tank, int tankfrom)
{
	float		tdamage;
	sprite_t	*ice;
	int			numice = 6;
	int			angle;
	int			power;
	int			i;
	_state		state;

	// Calculate some damage
	tdamage = (float)ta[tank].frozen/(float)255;
	tdamage = tdamage * projectiles[W_POLARISMISSILE].damageh;

	ta[tank].Damage((int)tdamage,
			0,
			0,
			tankfrom,
			false);

	ta[tank].frozen = 0;

	// Play a sound
	ta[tank].StartSound(POLARIS_ICESHATTER);

	// Spawn some ice shards for special effects
	for (i=0; i<numice; i++)
	{
		angle = rand()%(128);
		power = ConvertFromGamePower(150+rand()%50);

		state = (_state)(ICESHARD_BOTTOMLEFT+i);

		ice = G_SpawnSprite(ta[tank].x, ta[tank].y, state, (MF_NORMAL|MF_TRANSLUC));
		if (ice)
		{
			ice->xvel = (-power*COS(angle));
			ice->yvel = (power*-SIN(angle));
			ice->opacity = 192;
		}
	}
}

void A_PolarisEMPExplode(sprite_t*	s)
{
	int			i;
	float		spritew;
	float		spriteh;
	float		sx,sy;
	int			tankx, tanky;

	sprite_t	*sp = sprites;

	//if (s->life == 0)
	//	SetTankTimer(2*TICRATE);

	// Make the explosion bigger
	s->scale+= 0.07;

	if (s->scale < 1.40)
	{
		// Check to see what tanks we've come in contact with and kill their shield
		if (s->opacity == 255)
		{
			spritew = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->w*s->scale;
			spriteh = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h*s->scale;

			// Get coordinates
			sx = s->x - (spritew/2);
			sy = s->y - (spriteh/2);

			for (i=0; i<MAX_TANKS; i++)
			{
				if (ta[i].health <= 0)// || l->tankdamaged[i] == true)
					continue;

				tankx = ta[i].x - (ta[i].sizex/2);
				tanky = ta[i].y - (ta[i].sizey/2);
				if (check_bb_collision_general( tankx,
										tanky,
										20,//ta[i].sizex,
										14,//ta[i].sizey,
										s->x-(spritew/2),
										s->y-(spriteh/2),
										spritew,
										spriteh) == true)
				{
					if (ta[i].frozen > 0)
						A_ApplyFrozenDamage(i, s->tankfrom);
				}
			}
		}
	}
	else
	{

		s->opacity -= 15;
		if (s->opacity <= 0)
		{
			s->opacity = 0;
			s->Kill();

			// If for some reason the EMP blast did not shatter
			// the tanks ice, take care of that here
			for (i=0; i<MAX_TANKS; i++)
			{
				if (ta[i].exist <= 0 || ta[i].health <= 0 || ta[i].frozen <= 0)
					continue;
				A_ApplyFrozenDamage(i, s->tankfrom);
			}

		}
	}

}

void A_PolarisMistThinkier(sprite_t*	s)
{
//	int			i;
	int			deg;
	int			spawnx,spawny;
	float		sxvel,syvel;
	int			power;
	sprite_t	*sm;
	sprite_t	*emp;

	if (s->life == 35)
		s->StartSound(POLARIS_MIST);

	if (s->life <= 3.9 * TICRATE)
	{
		// Play two sounds every second
		if (!(gametics % (TICRATE/2)))
			s->StartSound(POLARIS_BEEP);

		if (!(s->life % 2))
		{
			deg = rand()%255;

			spawnx = s->x;
			spawny = s->y;

			power = 35+rand()%20;

			sxvel = (-ConvertFromGamePower(power)*COS(deg));
			syvel = (ConvertFromGamePower(power)*-SIN(deg));

			if (rand()%2 == 0)
				sm = G_SpawnSprite(spawnx, spawny, S_NITROGENPUFF_MEDIUM, (MF_NORMAL | MF_TRANSLUC | MF_CENTERCOORD | MF_ALLOWSMALLSCALE));
			else
				sm = G_SpawnSprite(spawnx, spawny, S_NITROGENPUFF_LARGE, (MF_NORMAL | MF_TRANSLUC | MF_CENTERCOORD | MF_ALLOWSMALLSCALE));

			if (sm)
			{
				sm->opacity = 192;
				sm->xvel = sxvel;
				sm->yvel = syvel;
				sm->scale = 0.5;
			}
		}
	}

	if (s->life == (int)(4.9*TICRATE))
		s->StartSound(POLARIS_DETONATE);

	if (s->life > 5.9*TICRATE)
	{
		//s->flags |= MF_NOTVISABLE;


		// Spawn the EMP blast
		emp = G_SpawnSprite(s->x, s->y,
			S_POLARISEMPBLAST,
			(MF_NORMAL | MF_CENTERCOORD | MF_ALLOWSMALLSCALE));

		if (emp)
		{
			emp->tankfrom = s->tankfrom;
			emp->scale = 0.2;
		}

		// Play a blast sound
		s->StartSound(POLARIS_EMP);

		// Spawn a explosion
		G_SpawnSprite(s->x, s->y, S_SMALLEXPLODE_B1, (MF_NORMAL | MF_CENTERCOORD ));

		// Setup the tank timer
		SetTankTimer(2*TICRATE);

		// Kill the sprite, we're done with it
		s->Kill();
		return;
	}
}


void A_AlienBeamUpThinker(sprite_t*	s)
{
	if (s->life == 1)
		s->StartSound(ALIEN_BEAM);

	if (s->life > 3*TICRATE)
	{
		s->opacity -= 3;
		if (s->opacity <= 0)
		{
			s->Kill();
			return;
		}
	}
}


void A_TurnArrowThinker(sprite_t*	s)
{
	s->opacity -= 3;

	// Over?
	if (s->opacity <= 0)
	{
		s->Kill();
		return;
	}

	if (s->misc1 == 0)
	{
		s->yvel += GRAVITY;

		if (s->yvel > 2)
		{
			s->yvel = 0;
			s->misc1 = 1;
		}
	}
	else
	{
		s->yvel -= GRAVITY;

		if (s->yvel < -2)
		{
			s->yvel = 0;
			s->misc1 = 0;
		}
	}

	// Always 0
	s->xvel = 0;

	s->x += s->xvel;
	s->y += s->yvel;
}

void A_LavaFloorDamage(sprite_t*	s)
{
	int	range=25;
	int	i, dist;
			
	for (i=0; i<MAX_TANKS; i++)
		{
		if (ta[i].exist <= 0 || ta[i].health <= 0)
			continue;

		dist = SCREENHEIGHT - (ta[i].y+(ta[i].sizey/2));
		if (dist < range)
			{
			ta[i].Damage((range-dist), 0, 0, -1, false, true, true);
			G_Printf("LavaFloorDamage");
			}
		}

	SetTankTimer(5);
}


void A_HeatCoronaThinker(sprite_t* s)
{
	s->yvel += 0.01;

	s->x += s->xvel;
	s->y += s->yvel;

	if (getpixel(ground.bmp, s->x, s->y) == TRANSCOLOR)
		s->flags &= ~MF_NOTVISABLE;
	else
		s->flags |= MF_NOTVISABLE;

	if (s->y > SCREENHEIGHT)
		s->Kill();
}


void A_HeatCoronaSpawnerThinker(sprite_t*	s)
{

	sprite_t	*corona;
	float		power;
	int			deg;

	if (gametics % 7)
		return;

	corona = G_SpawnSprite(rand()%SCREENWIDTH, SCREENHEIGHT-1, S_HEATCORONA, MF_NORMAL|MF_CENTERCOORD|MF_COOLEFFECT1);

	if (!corona)
		return;

	power = ConvertFromGamePower(70+rand()%10);
	deg	= rand()%255;

	corona->opacity = 100;

	// Set velocity
	corona->xvel = (-power*COS(deg));
	corona->yvel = (power*-SIN(deg));
}

void A_LavaShootAttack(sprite_t* s)
{
CheckRandomFunc("A_LavaShootAttack");
	if(rANDom()%5 != 0)		///CheckRandomFunc
		{
		s->SetState(S_LAVASHOOTER); //	reset to idle
		SetTankTimer(1);
		}
	else
		{
		A_ResetLife(s);
		// Play a sound
		s->StartSound(LAVASHOOT_FIRE);

		if(multiplayer && multiplayermode == MP_SERVER)
			{
			BCast_WriteByte(STOC_LAVASHOOT);
			BCast_WriteShort(s->netid);
			}

		SetTankTimer(8);
		}

}

void A_LavaShooterThinker(sprite_t* s)
{
	BITMAP	*sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	particlenode_t*		p;
	int					fflags=(PFLAGS_LIQUID|PFLAGS_DAMAGETANKS|PFLAGS_NOLIQUIDCLIP|PFLAGS_NAPALMEFFECT1);
	int					spawnangle;
	int					spawnpower;
	int					spawnlife;
	int					damagelife = 1*TICRATE;
	int					numparticles = 5, i;

	int					sx=(s->x)-(sbmp->w/2);
	int					sy=(s->y)-(sbmp->h/2);

	spawnlife = 2*TICRATE;
	SetTankTimer(3);

	for (i=0; i<numparticles; i++)
		{
		spawnangle = 56+rANDom()%12;	// between 56 - 72		///CheckRandomFunc
		spawnpower = ConvertFromGamePower(375+rANDom()%25);		CheckRandomFunc("A_LavaShooterThinker");

		p = NewNapalmParticle(sx+rANDom()%sbmp->w, sy+rANDom()%sbmp->h,		///CheckRandomFunc
								spawnangle,
								spawnpower,
								makecol(projectiles[W_NAPALM].pixelcolorr,
										projectiles[W_NAPALM].pixelcolorg,
										projectiles[W_NAPALM].pixelcolorb),
								spawnlife+(rANDom()%TICRATE),		///CheckRandomFunc
								fflags, 0,
								projectiles[W_NAPALM].pixeldamageh,
								projectiles[W_NAPALM].pixeldamagee,
								projectiles[W_NAPALM].pixeldamageb,
								damagelife);
		}

	if (s->life > damagelife*2)
		{
		ApplyNapalmDamage(-1);	//	damage from level (-1)
		s->SetState(S_LAVASHOOTER);

		SetTankTimer(10);	//(TICRATE/2);
		}
}

void A_RainDropSpawnerAttack(sprite_t* s)
{
	int		spritew,spriteh;
	int		spritex,spritey;
	int		sdamageh,sdamagee,sdamageb;
	int		i;
	int		tankx,tanky;
	int		spriteunderwater=0;

	// Particle effect
	particlenode_t	*p;
	int		nump = 15;
	int		plife;
	int		pangle;

	// Rain drop position
	int		lightx = rand()%SCREENWIDTH;
	int		lighty = OnGround(lightx, SCREENHEIGHT/2, ground.bmp);

	// Run normal function
	A_RainDropSpawnerThinker(s);

	// 1 and 6 chance of striking
	if (rand()%6 != 1)
	{
		SetTankTimer(1);
		return;
	}


	sprite_t* l = G_SpawnSprite(lightx-25, lighty-SCREENHEIGHT, S_LIGHTNINGROD_LIGHTNING1, (MF_NORMAL));

	// Update all clients about this
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	if (multiplayer && multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_LIGHTNINGSTRIKE);
		BCast_WriteShort(lightx);
		BCast_WriteShort(lighty);
	}

	// Spawning underwater causes tanks to take much more damage (1/3 more)
	if (Underwater(lightx, lighty))
		spriteunderwater = 1;

	// Define all the base varibles here since they can be changed
	spritew = ((BITMAP *)spritedata[l->startstate.datafilelookup].dat)->w;
	spriteh = ((BITMAP *)spritedata[l->startstate.datafilelookup].dat)->h;
	spritex = l->x;
	spritey = l->y;
	sdamageh = projectiles[W_LIGHTNINGROD].damageh;
	sdamagee = projectiles[W_LIGHTNINGROD].damagee;
	sdamageb = projectiles[W_LIGHTNINGROD].damageb;

	// Adjust some varibles if shooting lightning underwater
	if (spriteunderwater > 0)
	{
		// Wider radius
		spritew *= 2;
		spritex = lightx-(spritew/2);
	}

	// Oh yeah, might wanna burn the ground
	BurnGround(ground.bmp, lightx, lighty, 40, 50);
	BurnGround(ground.bmp, lightx, lighty, 20, 100);
	BurnGround(ground.bmp, lightx, lighty, 10, 150);
	BurnGround(ground.bmp, lightx, lighty, 5,  200);

	// White flash!
	currentlevel.whiteflashtics = 1;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health <= 0)// || l->tankdamaged[i] == true)
			continue;

		tankx = ta[i].x - (ta[i].sizex/2);
		tanky = ta[i].y - (ta[i].sizey/2);
		if (check_bb_collision_general( tankx,
								tanky,
								20,//ta[i].sizex,
								14,//ta[i].sizey,
								spritex,
								spritey,
								spritew,
								spriteh) == true)
		{
			// The
			if (ta[i].underwater && spriteunderwater > 0)
				ta[i].Damage(sdamageh+(sdamageh/3), sdamagee+(sdamagee/3), sdamageb+(sdamageb/3), s->tankfrom, false);
			else
				ta[i].Damage(sdamageh, sdamagee, sdamageb, s->tankfrom, false);

			s->tankdamaged[i] = true;
		}


	}

	// Attack other sprites as well
	A_ContactSpriteDamage(s, 100, true);

	if (rand()%2==0)
		StartSound(THUNDERHIT_1);
	else
		StartSound(THUNDERHIT_2);

	// Spawn some particles, just for looks
	for (i=0; i<nump; i++)
	{
		// Set particle life, randomize this a bit
		plife = 30+rand()%30;

		// Randomize the angle as well
		pangle = 45+rand()%40;

		p = pdirt->InsertParticle(lightx, lighty-10, rand()%128, ConvertFromGamePower(300+rand()%100), makecol(200,200,255), plife, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
		if (p)
			p->bounce = 5;
	}

	// Advance turn...
	SetTankTimer(TICRATE);

}

void A_RainDropSpawnerThinker(sprite_t*	s)
{
	sprite_t*	rain;

	if (gametics % 2)
		return;

	rain = G_SpawnSprite(rand()%SCREENWIDTH, 0, S_RAINDROP, (MF_NORMAL|MF_CENTERCOORD));
	rain->xvel = -3;
	rain->yvel = 4;

	rain = G_SpawnSprite(SCREENWIDTH, rand()%SCREENHEIGHT, S_RAINDROP, (MF_NORMAL|MF_CENTERCOORD));
	rain->xvel = -3;
	rain->yvel = 4;


}

void A_RainDropThinker(sprite_t*	s)
{
	s->yvel += GRAVITY;
	s->x += s->xvel;
	s->y += s->yvel;

	if (getpixel(ground.bmp, s->x, s->y) != TRANSCOLOR)
		s->Kill();
}

void A_CherrySpawnerThinker(sprite_t*	s)
{
	sprite_t	*bloss;
	float		power;
	int			deg;
	_state		state;

	if (gametics % 2)
		return;

	state = (_state)(S_CHERRYBLOSS_1+rand()%3);

	bloss = G_SpawnSprite(s->x, s->y, state, MF_NORMAL|MF_CENTERCOORD);

	if (!bloss)
		return;

	power = ConvertFromGamePower(33);
	deg	= rand()%255;

	// Set velocity
	bloss->xvel = (-power*COS(deg));
	bloss->yvel = (power*-SIN(deg));
}

void A_CherryBlossThinker(sprite_t*	s)
{
	s->yvel += 0.01;

	s->x += s->xvel;
	s->y += s->yvel;

	if (OnScreen(buffer, s->x, s->y) <= 0)
		s->Kill();

	if (getpixel(ground.bmp, s->x, s->y) != TRANSCOLOR)
	{
		//s->Kill();
		if (s->startstate.currentstate == S_CHERRYBLOSS_1)
			s->SetState(S_CHERRYBLOSS_1SIT);
		if (s->startstate.currentstate == S_CHERRYBLOSS_2)
			s->SetState(S_CHERRYBLOSS_2SIT);
		if (s->startstate.currentstate == S_CHERRYBLOSS_3)
			s->SetState(S_CHERRYBLOSS_3SIT);

		// Set some flags and varibles
		s->flags |= MF_NOGRAVITY;
		s->xvel = s->yvel = 0;

		A_ResetLife(s);
	}
}

void A_CherryBlossFinish(sprite_t	*s)
{
	if (s->life == 3*TICRATE || getpixel(ground.bmp, s->x, s->y) == TRANSCOLOR)
		s->Kill();
}

void A_FollowTank(sprite_t *s)
{
	s->x = ta[s->tankfrom].x;
	s->y = ta[s->tankfrom].y;
}

void A_ShardThinker(sprite_t *s)
{
	s->yvel += GRAVITY;

	// Don't advance the turn yet
	SetTankTimer(10);

	// Check to see if this has hit ground yet
	if (getpixel(ground.bmp, s->x, s->y) != TRANSCOLOR ||
		!OnScreen(buffer, s->x, s->y))
	{
		s->Kill();
	}
}

void A_SpikeShakeInit(sprite_t*		s)
{
	A_ResetLife(s);

	s->StartSound(CAVESPIKE_SHAKE);
}

void A_SpikeDie(sprite_t *s)
{
	BITMAP	*sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	int		sx,sy;

	sprite_t*	shard;

	float		i;
	int			t;
	int			estate;
	int			numshards=5;
	int			shardpower = ConvertFromGamePower(200);
	float		circle_add = (128/(float)numshards);

	int			tankx;
	int			tanky;

	// "Suspend" the sprite until its done exploding
	if (!(s->flags & MF_NOGRAVITY))
		s->flags |= MF_NOGRAVITY;

	sx=(s->x)-(sbmp->w/2);
	sy=(s->y)-(sbmp->h/2);

	if (s->life == 1)
	{
		s->flags |= MF_NOTVISABLE;

		for (i=0; i<128; i+=circle_add)
		{
			estate = S_CAVESPIKESHARD1+rand()%6;

			shard = G_SpawnSprite(sx+rand()%(sbmp->w), sy+rand()%(sbmp->w), (_state)estate, (MF_NORMAL | MF_CENTERCOORD ));
			shard->xvel = (-shardpower*COS(i));
			shard->yvel = (shardpower*-SIN(i));
		}

		// Spawn an explosion
		if (rand()%2 == 0)
			G_SpawnSprite(s->x, s->y, S_EXPLODE_A1, (MF_NORMAL | MF_CENTERCOORD ));
		else
			G_SpawnSprite(s->x, s->y, S_EXPLODE_B1, (MF_NORMAL | MF_CENTERCOORD ));

		// Dish out some damage
		A_ContactSpriteDamage(s, 10, true);

		// Play a tune
		s->StartSound(CAVESPIKE_SHATTER);

		// Check for any tank contact
		for (t=0; t<MAX_TANKS; t++)
		{
			if (ta[t].health <= 0 || s->tankdamaged[t] == true)
				continue;

			tankx = ta[t].x - (ta[t].sizex/2);
			tanky = ta[t].y - (ta[t].sizey/2);
			if (check_pp_collision(ta[t].bmp,
									sbmp,
									tankx,
									tanky,
									sx,
									sy) > 0)
			{
				ta[t].Damage(s->damageh, s->damagee, s->damageb, s->tankfrom, false);
				s->tankdamaged[t] = true;
			}

		}

		// Do not advance the turn yet
		SetTankTimer(30);

		// Remove the sprite
		s->Kill();

	}
}

void A_SpikeShake(sprite_t *s)
{
	_state	newstate;

	if (rand()%2 == 0)
		s->x += 1;
	else
		s->x -= 1;

	SetTankTimer(10);

	// Ready to move on?
	if (s->life >= 1*TICRATE)
	{
		if (s->startstate.currentstate == S_CAVESPIKE1_SHAKE2)
			newstate = S_CAVESPIKE1_FALL1;

		if (s->startstate.currentstate == S_CAVESPIKE2_SHAKE2)
			newstate = S_CAVESPIKE2_FALL1;

		if (s->startstate.currentstate == S_CAVESPIKE3_SHAKE2)
			newstate = S_CAVESPIKE3_FALL1;

		if (s->startstate.currentstate == S_CAVESPIKE4_SHAKE2)
			newstate = S_CAVESPIKE4_FALL1;

		if (s->startstate.currentstate == S_CAVESPIKE5_SHAKE2)
			newstate = S_CAVESPIKE5_FALL1;

		s->SetState(newstate);
	}

}

void A_SpikeFall(sprite_t *s)
{
	BITMAP	*sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	_state	newstate;

	// For effects
	sprite_t*		smoke;
	particlenode_t*	p;


	// Remove the "no gravity" flag if not
	// already done so...
	if (s->flags & MF_NOGRAVITY)
		s->flags &= ~MF_NOGRAVITY;

	// Apply some gravity
	s->yvel += GRAVITY;

	if (!(s->life % 2))
	{
		smoke = G_SpawnSprite((int)(s->x-(sbmp->w/2))+rand()%sbmp->w,
							(int)s->y-(sbmp->h/2),
							S_SMOKE_BLACK_MEDIUM,
							(MF_NORMAL | MF_USEANGLE | MF_TRANSLUC));
		if (smoke)
			smoke->opacity = 128;
	}

	if (rand()%4 == 1)
	{
		p = pdirt->InsertParticle((int)(s->x-(sbmp->w/2))+rand()%sbmp->w, (int)s->y-(sbmp->h/2), 64, ConvertFromGamePower(60), makecol(232,218,168), 30+rand()%10, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
		if (p)
			p->bounce = 3;
	}

	// Check to see if this has hit ground yet
	if (s->y >= 0 && getpixel(ground.bmp, s->x, s->y) != TRANSCOLOR)
	{
		if (s->startstate.currentstate == S_CAVESPIKE1_FALL1)
			newstate = S_CAVESPIKE1_DIE;

		if (s->startstate.currentstate == S_CAVESPIKE2_FALL1)
			newstate = S_CAVESPIKE2_DIE;

		if (s->startstate.currentstate == S_CAVESPIKE3_FALL1)
			newstate = S_CAVESPIKE3_DIE;

		if (s->startstate.currentstate == S_CAVESPIKE4_FALL1)
			newstate = S_CAVESPIKE4_DIE;

		if (s->startstate.currentstate == S_CAVESPIKE5_FALL1)
			newstate = S_CAVESPIKE5_DIE;

		s->SetState(newstate);

		A_ResetLife(s);
	}

	SetTankTimer(10);

}


void A_FireFlyDieThinker(sprite_t *s)
{
	int				i;
	int				tankx, tanky;
	int				spritex, spritey, spritew, spriteh;
	int				numexp = 2;
	int				numburn=5;
	int				numholes=2;
	int				expsound;

	// Only need a really small spot. Make a square around
	// the point of impact
	spritex = s->x - 5;
	spritey = s->y - 5;
	spritew = 10;
	spriteh = 10;

	SetTankTimer(30);

	if (s->life < 2*TICRATE)
		return;

	// Check for tank deaths
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health <= 0)// && ta[i].needstodie <= 0)
			continue;

		tankx = ta[i].x - (ta[i].sizex/2);
		tanky = ta[i].y - (ta[i].sizey/2);
		if (check_bb_collision_general( tankx,
								tanky,
								20,//ta[i].sizex,
								14,//ta[i].sizey,
								spritex,
								spritey,
								spritew,
								spriteh) == true)
		{
			ta[i].Damage(s->damageh, s->damagee, s->damageb, s->tankfrom, false, true, true);
		}
	}

	// Attack other sprites as well
	A_ContactSpriteDamage(s, 10, true);

	// Throw some sparks around
	for (i=0; i<numburn; i++)
		BurnGround(ground.bmp, spritex+rand()%spritew, spritey+rand()%spriteh, 15, 50);

	CheckRandomFunc("FireFly");
	for (i=0; i<numholes; i++)
		DestroyDirt(spritex+rANDom()%spritew, spritey+rANDom()%spriteh, 2+rANDom()%3);	//	FireFly

	for (i=0; i<numexp; i++)
	{
		if (rand()%2 == 0)
			G_SpawnSprite(spritex+rand()%spritew, spritey+rand()%spriteh, S_SMALLEXPLODE_B1, (MF_NORMAL | MF_CENTERCOORD ));
		else
			G_SpawnSprite(spritex+rand()%spritew, spritey+rand()%spriteh, S_SMALLEXPLODE_A1, (MF_NORMAL | MF_CENTERCOORD ));

		expsound = rand()%4;
		if (expsound == 0)
			StartSound(EXPLODE_MEDIUM1);
		else if (expsound == 1)
			StartSound(EXPLODE_MEDIUM2);
	}

	// Collaspe the ground
	ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);

	// Wipe out the sprite
	s->Kill();

}

void A_FireFlyEffect(sprite_t *s)
{
	s->opacity -= 5;

	if (s->opacity <= 0)
		s->Kill();
}

void A_RailGunSmoke(sprite_t* s)
{
	s->opacity-=8;

	if (s->opacity <= 0)
		s->Kill();
}


void A_HeatrayEffect(sprite_t* s)
{
	if (s->opacity == 100)
		s->opacity = 255;
	else
		s->opacity = 100;

	if (s->life > 1*TICRATE)
		s->Kill();
}

void A_HeatrayFlameThinker(sprite_t* s)
{
	if (s->life > 1*TICRATE)
		s->Kill();
}

void A_HeatrayThinker(sprite_t* s)
{
	//if (!(s->life % 10))
	//	BurnGround(ground.bmp, s->x, s->y, 7, 125);

	if (s->life > 1*TICRATE)
	{
		//s->SetState(S_IONCANNON_DIE1);
		ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
		s->Kill();
	}

}

void	A_SnowFlakeSpawner(sprite_t* s)
{
	int		chance = rand()%10;

	if (chance == 5)
	{
		G_SpawnSprite(rand()%SCREENWIDTH, 0, S_SNOWFLAKE_SMALL, (MF_NORMAL|MF_CENTERCOORD));
		G_SpawnSprite(SCREENWIDTH, rand()%SCREENHEIGHT, S_SNOWFLAKE_SMALL, (MF_NORMAL|MF_CENTERCOORD));
	}
	else if (chance == 3)
	{
		G_SpawnSprite(rand()%SCREENWIDTH, 0, S_SNOWFLAKE_MEDIUM, (MF_NORMAL|MF_CENTERCOORD));
		G_SpawnSprite(SCREENWIDTH, rand()%SCREENHEIGHT, S_SNOWFLAKE_MEDIUM, (MF_NORMAL|MF_CENTERCOORD));
	}
	else if (chance == 1)
	{
		G_SpawnSprite(rand()%SCREENWIDTH, 0, S_SNOWFLAKE_LARGE, (MF_NORMAL|MF_CENTERCOORD));
		G_SpawnSprite(SCREENWIDTH, rand()%SCREENHEIGHT, S_SNOWFLAKE_LARGE, (MF_NORMAL|MF_CENTERCOORD));
	}

}




void	A_SmallSnowFlakeThinker(sprite_t *s)
{
	int		direction;

	//if (!(gametics % 2))
	{
		direction = rand()%3;
		if (direction==0)
			s->xvel = -0.75;
		else if (direction == 1)
			s->xvel = 0.75;
		else
			s->xvel = 0;
	}


	if (Underwater((int)s->x, (int)s->y) || !OnScreen(buffer, s->x, s->y))
	{
		s->Kill();
		return;
	}
	else
	{
		s->x--;
		s->x += s->xvel;
		s->y++;

		if (s->y >= SCREENHEIGHT)
			s->Kill();
	}
}


void	A_MediumSnowFlakeThinker(sprite_t *s)
{
	int		direction;

	//if (!(gametics % 2))
	{
		direction = rand()%3;
		if (direction==0)
			s->xvel = -0.5;
		else if (direction == 1)
			s->xvel = 0.5;
		else
			s->xvel = 0;
	}


	if (Underwater((int)s->x, (int)s->y) || !OnScreen(buffer, s->x, s->y))
	{
		s->Kill();
		return;
	}
	else
	{
		s->x--;
		s->x += s->xvel;
		s->y+=1.5;

		if (s->y >= SCREENHEIGHT)
			s->Kill();
	}
}


void	A_LargeSnowFlakeThinker(sprite_t *s)
{
	int		direction;

	//if (!(gametics % 2))
	{
		direction = rand()%3;
		if (direction==0)
			s->xvel = -0.25;
		else if (direction == 1)
			s->xvel = 0.25;
		else
			s->xvel = 0;
	}


	if (Underwater((int)s->x, (int)s->y) || !OnScreen(buffer, s->x, s->y))
	{
		s->Kill();
		return;
	}
	else
	{
		s->x--;
		s->x += s->xvel;
		s->y+=2;

		if (s->y >= SCREENHEIGHT)
			s->Kill();
	}
}


void A_FollowParent(sprite_t*	s)
{
	if (s->spawnfrom)
	{
		s->x = s->spawnfrom->x;
		s->y = s->spawnfrom->y;
	}
}

void A_LavaSpawnerSetup(sprite_t* s)
{
	int		chance = rANDom()%4;		CheckRandomFunc("A_LavaSpawnerSetup");

	// There is a 1 in 4 chance that lava will spawn
	if (chance != 2)
	{
		SetTankTimer(5);
		s->SetState(S_LAVASPAWNER);
		return;
	}

	// Reset counter
	s->life = 0;

	SetTankTimer(12);	//TICRATE/2);
}

void A_LavaSpawnerThinker(sprite_t*	s)
{
	BITMAP	*sbmp;
	//sprite_t	*effect;
	int		spriteflags = (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE);
	int		targetx = rANDom()%SCREENWIDTH;		///CheckRandomFunc
	int		targety = OnGround(targetx, SCREENHEIGHT, ground.bmp);
	int		origx, origy, deg;
	int		newshot;

	CheckRandomFunc("A_LavaSpawnerThinker targetx");
	if (s->life % 20)
		{
		//G_Printf("LavaSpawnerThinker skip");
		return;
		}

	{
	sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	origx = s->x+(sbmp->w/2);
	origy = OnGround(origx, s->y, ground.bmp) - 5; //s->y;

	if (targetx > origx)
		deg = 75;
	else
		deg = 55;

	// Calculate trajectory
	Aim(origx,					// Origx
			origy,					// Origy
			targetx,				// Targetx
			targety,				// Targety
			deg,					// Firing degree
			10);

	// Setup a shot, giving the spawner credit
	newshot = findshot();		//G_Printf("LavaSpawnerThinker shot");
	if(newshot>=0)
			{
			//effect = G_SpawnSprite(origx, origy, S_BIGLAVAROCKEFFECT, (MF_NORMAL|MF_CENTERCOORD|MF_COOLEFFECT1));

			sh[newshot].Init(newshot, W_BIGLAVAROCK, 0,0,0,0,0);

			// Environmental hazard
			sh[newshot].tankfrom = -1;
			sh[newshot].origdeg = sh[newshot].angle = deg;
			sh[newshot].origx = sh[newshot].x = origx;
			sh[newshot].origy = sh[newshot].y = origy;
			sh[newshot].sprite = G_SpawnSprite(sh[newshot].x,sh[newshot].y, projectiles[sh[newshot].projectile].state, spriteflags);
			//sh[newshot].sprite->spawnfrom = s;

			// Set velocity
			sh[newshot].origxvel = sh[newshot].xvel = (-bestpower*COS(deg)),
			sh[newshot].origyvel = sh[newshot].yvel = (bestpower*-SIN(deg));

			// Play a song if we have one
			if(projectiles[sh[newshot].projectile].firesound >= 0)
				sh[newshot].StartSound(projectiles[sh[newshot].projectile].firesound);

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
			}
	}

	if(s->life > 39)
		s->SetState(S_LAVASPAWNER);
}


void A_MissileTurretAttack(sprite_t *s)
{
	BITMAP	*sbmp;
	int		spriteflags = (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE);
	int		targetx = rANDom()%SCREENWIDTH;		///CheckRandomFunc
	int		targety = OnGround(targetx, SCREENHEIGHT, ground.bmp);
	int		origx, origy, deg;
	int		newshot;

CheckRandomFunc("A_MissileTurretAttack targetx");
	sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);
	origx = s->x+(sbmp->w/2);
	origy = s->y;

	if (targetx > origx)
		deg = 75;
	else
		deg = 55;

	// Calculate trajectory
	Aim(origx,					// Origx
		origy,					// Origy
		targetx,				// Targetx
		targety,				// Targety
		deg,					// Firing degree
		5);

	// Setup a shot, giving the spawner credit
	newshot = findshot();
	if (newshot>=0)
	{
		sh[newshot].Init(newshot, W_NEEDLEMISSILE, 0,0,0,0,0);
		sh[newshot].tankfrom = s->tankfrom;
		sh[newshot].origdeg = sh[newshot].angle = deg;
		sh[newshot].origx = sh[newshot].x = origx;
		sh[newshot].origy = sh[newshot].y = origy;
		sh[newshot].sprite = G_SpawnSprite(sh[newshot].x,sh[newshot].y, projectiles[sh[newshot].projectile].state, spriteflags);
		sh[newshot].sprite->spawnfrom = s;

		// Set velocity
		sh[newshot].origxvel = sh[newshot].xvel = (-bestpower*COS(deg)),
		sh[newshot].origyvel = sh[newshot].yvel = (bestpower*-SIN(deg));

		// Play a song if we have one
		if (projectiles[sh[newshot].projectile].firesound >= 0)
			sh[newshot].StartSound(projectiles[sh[newshot].projectile].firesound);

		if (multiplayer && multiplayermode == MP_SERVER)
		{
			sh[newshot].spawndeg = sh[newshot].angle;
			sh[newshot].spawnx = sh[newshot].origx;
			sh[newshot].spawny = sh[newshot].origy;
			sh[newshot].spawnxvel = sh[newshot].xvel;
			sh[newshot].spawnyvel = sh[newshot].yvel;

			// Broadcast to all other clients
			BCast_Shot(newshot);
		}
	}
}

void A_ShellThinker(sprite_t* s)
{
	s->angle += 5;

	// Kill sprite after its bounced 4 times
	if (s->bouncecount >=4)
		s->Kill();
}

void A_BSentry_Attack(sprite_t*	s)
{
	float	targeta;
	int		targetx, targety;
	int		ttankx, ttanky;
	int		shotdifference=10;
	int		newshot;
	int		spriteflags = (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE);

	// Acquire a target if we haven't already
	if (s->targettank < 0 ||
		ta[s->targettank].health <= 0 ||
		ta[s->targettank].exist <= 0)
		s->targettank = Sentry_ChooseTarget(s);

	// Setup some varibles
	ttankx = ta[s->targettank].x - (ta[s->targettank].sizex/2);
	ttanky = ta[s->targettank].y - (ta[s->targettank].sizey/2);

	// Shot difference is the parameter of error to which we
	// will shoot at
	targetx = (ta[s->targettank].x-shotdifference)+rANDom()%(shotdifference*2);		CheckRandomFunc("A_BSentry_Attack");
	targety = (ta[s->targettank].y-shotdifference)+rANDom()%(shotdifference*2);		///CheckRandomFunc

	// Based off the newly calculated targetx & y, figure out the new angle
	// for the straight shot weapon
	targeta = CalculateAngle(s->x+(wobjects[s->wobj].width/2), s->y, targetx, targety);

	// Adjust the angle to whomever we are aiming at
	if (targeta > 64 && targeta < 192)
		{
		if (!(s->flags & MF_FLIPPED))
			s->flags |= MF_FLIPPED;
		}
	else
		{
		if (s->flags & MF_FLIPPED)
			s->flags &= ~MF_FLIPPED;
		}

	// Setup a shot, giving the spawner credit
	newshot = findshot();
	if (newshot>=0)
		{
		sh[newshot].Init(newshot, W_UZISPRAY, 0,0,0,0,0);
		sh[newshot].tankfrom = s->tankfrom;
		sh[newshot].origdeg = sh[newshot].angle = targeta;
		sh[newshot].origx = sh[newshot].x = s->x+(wobjects[s->wobj].width/2);
		sh[newshot].origy = sh[newshot].y = s->y;
		sh[newshot].sprite = G_SpawnSprite(sh[newshot].x,sh[newshot].y, projectiles[sh[newshot].projectile].state, spriteflags);
		sh[newshot].sprite->spawnfrom = s;

		if (multiplayer && multiplayermode == MP_SERVER)
			{
			sh[newshot].spawndeg = sh[newshot].angle;
			sh[newshot].spawnx = sh[newshot].origx;
			sh[newshot].spawny = sh[newshot].origy;

			// Broadcast to all clients
			BCast_Shot(newshot);
			}
		}

}


void A_ESentry_Attack(sprite_t*	s)
{
	float	targeta;
	int		targetx, targety;
	int		ttankx, ttanky;
	int		shotdifference=10;
	int		newshot;
	int		spriteflags = (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE);

	// Acquire a target if we haven't already
	if (s->targettank < 0 ||
		ta[s->targettank].health <= 0 ||
		ta[s->targettank].exist <= 0)
		s->targettank = Sentry_ChooseTarget(s);

	// Setup some varibles
	ttankx = ta[s->targettank].x - (ta[s->targettank].sizex/2);
	ttanky = ta[s->targettank].y - (ta[s->targettank].sizey/2);

	// Shot difference is the parameter of error to which we
	// will shoot at
	targetx = (ta[s->targettank].x-shotdifference)+rANDom()%(shotdifference*2);			CheckRandomFunc("A_ESentry_Attack");
	targety = (ta[s->targettank].y-shotdifference)+rANDom()%(shotdifference*2);		///CheckRandomFunc

	// Based off the newly calculated targetx & y, figure out the new angle
	// for the straight shot weapon
	targeta = CalculateAngle(s->x+(wobjects[s->wobj].width/2), s->y, targetx, targety);

	// Adjust the angle to whomever we are aiming at
	if (targeta > 64 && targeta < 192)
	{
		if (!(s->flags & MF_FLIPPED))
			s->flags |= MF_FLIPPED;
	}
	else
	{
		if (s->flags & MF_FLIPPED)
			s->flags &= ~MF_FLIPPED;
	}

	// Setup a shot, giving the spawner credit
	newshot = findshot();
	if (newshot>=0)
	{
		sh[newshot].Init(newshot, W_ELECTRICLASER, 0,0,0,0,0);
		sh[newshot].tankfrom = s->tankfrom;
		sh[newshot].origdeg = sh[newshot].angle = targeta;
		sh[newshot].origx = sh[newshot].x = s->x+(wobjects[s->wobj].width/2);
		sh[newshot].origy = sh[newshot].y = s->y;
		sh[newshot].sprite = G_SpawnSprite(sh[newshot].x,sh[newshot].y, projectiles[sh[newshot].projectile].state, spriteflags);
		sh[newshot].sprite->spawnfrom = s;

		if (multiplayer && multiplayermode == MP_SERVER)
		{
			sh[newshot].spawndeg = sh[newshot].angle;
			sh[newshot].spawnx = sh[newshot].origx;
			sh[newshot].spawny = sh[newshot].origy;

			// Broadcast to clients
			BCast_Shot(newshot);
		}
	}

}



void A_HSentryShot_Thinker(sprite_t*	s)
{
	int				i;
	int				tankx, tanky;
	int				spritex, spritey, spritew, spriteh;
	int				nump = 7;
	particlenode_t	*p;

	// Only need a really small spot. Make a square around
	// the point of impact
	spritex = s->x - 10;
	spritey = s->y - 10;
	spritew = 20;
	spriteh = 20;

	if (s->life % 10)
		return;

	// Check for tank deaths
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health <= 0)// && ta[i].needstodie <= 0)
			continue;

		tankx = ta[i].x - (ta[i].sizex/2);
		tanky = ta[i].y - (ta[i].sizey/2);
		if (check_bb_collision_general( tankx,
								tanky,
								20,//ta[i].sizex,
								14,//ta[i].sizey,
								spritex,
								spritey,
								spritew,
								spriteh) == true)
		{
			ta[i].Damage(s->damageh, s->damagee, s->damageb, s->tankfrom, false, true, true);
		}
	}

	// Attack other sprites as well
	A_ContactSpriteDamage(s, 10, true);

	// Throw some sparks around
	for (i=0; i<nump; i++)
	{
		p = pdirt->InsertParticle(s->x, s->y, rand()%255, ConvertFromGamePower(300), makecol(255,255,128), (TICRATE/3), PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
		p->bounce = 3;
	}

	G_SpawnSprite(spritex+rANDom()%spritew, spritey+rANDom()%spriteh, S_SMALLEXPLODE_A1, (MF_NORMAL | MF_CENTERCOORD ));
	if (rand()%2 == 0)
		StartSound(EXPLODE_MEDIUM1);
	else
		StartSound(EXPLODE_MEDIUM2);

	if (s->life > 2*TICRATE)
	{
		// Collaspe the ground
		ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);

		// Wipe out the sprite
		s->Kill();
	}

}

int Sentry_ChooseTarget(sprite_t* s)
{
	int		newtarget;
	int		closest = SCREENWIDTH*2;	// Nothing can be farther than that;
	int		dist;
	int		i;

	s->targettank = -1;
	if(s->x > 1000)s->x=0;  //  check the values are sane.
	if(s->y > 1000)s->y=0;  //  negative numbers can wrap to 65k

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || ta[i].health <= 0 || i == s->tankfrom)
			continue;

		// Dont attack players of the same team
		if (teamplay->value > 0 && ta[s->tankfrom].IsSameTeam(i))
			continue;

		dist = FindLineDistance(s->x, s->y, ta[i].x, ta[i].y);

		// This is the closest tank so far
		if (dist < closest)
		{
			newtarget = i;
			closest = dist;
		}
	}
#ifdef	_DEBUG
//  Degauss debug
Printf("Sentry_ChooseTarget %d\n",newtarget);  //  Degauss
#endif
	return newtarget;
}

void	A_SentryDeath(sprite_t*	s)
{
	int				nump = 50;
	int				i;
	particlenode_t	*p;
	BITMAP			*sprbmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;

	// Spawn an explosion and play a sound
	G_SpawnSprite(s->x+(sprbmp->w/2), s->y+(sprbmp->h/2), S_SMALLEXPLODE_A1, (MF_NORMAL | MF_CENTERCOORD ));
	s->StartSound(EXPLODE_TOWERPOD);

	// Throw some sparks around
	for (i=0; i<nump; i++)
	{
		p = pdirt->InsertParticle(s->x+(sprbmp->w/2), s->y+(sprbmp->h/2), rand()%255, ConvertFromGamePower(300), makecol(255,255,128), TICRATE, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
		if (p)
			p->bounce = 5;
	}
}


void	A_SentryOpen(sprite_t* s)
{
	// So tanks don't fire again
	SetTankTimer(2*TICRATE);

	s->StartSound(SENTRYPOD_OPEN);
}


void A_HSentry_Attack(sprite_t*	s)
{
	float	targeta;
	int		targetx, targety;
	int		ttankx, ttanky;
	int		shotdifference=10;
	int		newshot;
	int		spriteflags = (MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE);

	// Acquire a target if we haven't already
	if (s->targettank < 0 ||
		ta[s->targettank].health <= 0 ||
		ta[s->targettank].exist <= 0 ||
		s->targettank == s->tankfrom)
		s->targettank = Sentry_ChooseTarget(s);

	// Setup some varibles
	ttankx = ta[s->targettank].x - (ta[s->targettank].sizex/2);
	ttanky = ta[s->targettank].y - (ta[s->targettank].sizey/2);

	// Shot difference is the parameter of error to which we
	// will shoot at
	targetx = (ta[s->targettank].x-shotdifference)+rANDom()%(shotdifference*2);		///CheckRandomFunc
	targety = (ta[s->targettank].y-shotdifference)+rANDom()%(shotdifference*2);		CheckRandomFunc("A_HSentry_Attack");


	// Based off the newly calculated targetx & y, figure out the new angle
	// for the straight shot weapon
	targeta = CalculateAngle(s->x+(wobjects[s->wobj].width/2), s->y, targetx, targety);

	// Adjust the angle to whomever we are aiming at
	if (targeta > 64 && targeta < 192)
	{
		if (!(s->flags & MF_FLIPPED))
			s->flags |= MF_FLIPPED;
	}
	else
	{
		if (s->flags & MF_FLIPPED)
			s->flags &= ~MF_FLIPPED;
	}

	// Setup a shot, giving the spawner credit
	newshot = findshot();
	if (newshot>=0)
	{
		sh[newshot].Init(newshot, W_MOLTENROD, 0,0,0,0,0);
		sh[newshot].tankfrom = s->tankfrom;
		sh[newshot].origdeg = sh[newshot].angle = targeta;
		sh[newshot].origx = sh[newshot].x = s->x+(wobjects[s->wobj].width/2);
		sh[newshot].origy = sh[newshot].y = s->y;
		sh[newshot].sprite = G_SpawnSprite(sh[newshot].x,sh[newshot].y, projectiles[sh[newshot].projectile].state, spriteflags);
		sh[newshot].sprite->spawnfrom = s;

		if (multiplayer && multiplayermode == MP_SERVER)
		{
			sh[newshot].spawndeg = sh[newshot].angle;
			sh[newshot].spawnx = sh[newshot].origx;
			sh[newshot].spawny = sh[newshot].origy;

			// Broadcast to all clients
			BCast_Shot(newshot);
		}
	}

}

void A_TowerBuild(sprite_t* s)
{
	s->StartSound(TOWERPOD_BUILD);
}

void A_TowerDie(sprite_t* s)
{
	int		sx,sy;
	int		sw,sh;
	int		exp;
	_state	expstate;

	SetTankTimer(20);

	if (gametics % 5)
		return;

	sw = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->w;
	sh = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h;

	sx=s->x;
	sy=s->y;

	exp = rANDom()%3;

	if (exp == 0)
		expstate = S_EXPLODE_A1;
	else if (exp == 1)
		expstate = S_EXPLODE_B1;
	else
		expstate = S_SMALLEXPLODE_A1;


	G_SpawnSprite(sx+rANDom()%sw, sy+rANDom()%sh, expstate, (MF_NORMAL | MF_CENTERCOORD ));

	s->StartSound(EXPLODE_TOWERPOD);

	if (s->life > 2*TICRATE)
		s->Kill();


}

void A_FireStarter_Flame_Burn(sprite_t*	s)
{
	// Tank contact varibles
	int		i;
	int		tankx,tanky;

	// Burn varibles
	int		difference = 7;
	int		numburnmarks = 3;
	int		burnx;

	if (s->life % 10)
		return;


	// Tank clipping code, this is just cut/pasted from A_ContactTankDamage with the exception
	// of the sprite damage flag. The same sprite will damage a tank over and over again.
	for (i=0; i<MAX_TANKS; i++)
	{
		int		spritew,spriteh;

		spritew = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->w;
		spriteh = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h;

		if (ta[i].health <= 0)// || l->tankdamaged[i] == true)
			continue;

		tankx = ta[i].x - (ta[i].sizex/2);
		tanky = ta[i].y - (ta[i].sizey/2);
		if (check_bb_collision_general( tankx,
										tanky,
										20,//ta[i].sizex,
										14,//ta[i].sizey,
										s->x-(spritew/2),
										s->y-(spriteh/2),
										spritew,
										spriteh) == true)
		{
			ta[i].Damage(s->damageh, s->damagee, s->damageb, s->tankfrom, false, true, true);
		}
	}

	// Damage any other shootable sprites
	A_ContactSpriteDamage(s,10);


	// Leave some burn marks now
	for (i=0; i<numburnmarks; i++)
	{
		// To the left
		if (rand()%2 == 0)
			burnx = s->x - rand()%difference;
		else
			burnx = s->x + rand()%difference;
		BurnGround(ground.bmp, burnx, (s->y-5)+rand()%7, 5);
	}
}


void A_EMPExplodeThinker(sprite_t*	s)
{
	int			i;
	float		spritew;
	float		spriteh;
	float		sx,sy;
	int			tankx, tanky;

	sprite_t	*sp = sprites;

	if (s->life == 0)
		SetTankTimer(0.5*TICRATE);

	// Make the explosion bigger
	s->scale+= 0.03;

	if (s->scale > 0.45)
	{
		// Check to see what tanks we've come in contact with and kill their shield
		if (s->opacity == 255)
		{
			spritew = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->w*s->scale;
			spriteh = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h*s->scale;

			// Get coordinates
			sx = s->x - (spritew/2);
			sy = s->y - (spriteh/2);

			for (i=0; i<MAX_TANKS; i++)
			{
				if (ta[i].health <= 0)// || l->tankdamaged[i] == true)
					continue;

				tankx = ta[i].x - (ta[i].sizex/2);
				tanky = ta[i].y - (ta[i].sizey/2);
				if (check_bb_collision_general( tankx,
										tanky,
										20,//ta[i].sizex,
										14,//ta[i].sizey,
										s->x-(spritew/2),
										s->y-(spriteh/2),
										spritew,
										spriteh) == true)
				{
					ta[i].KillShield();
				}
			}
		}

		A_EMPBlastSteal(s);

		s->opacity -= 20;
		if (s->opacity <= 0)
		{
			s->opacity = 0;
			s->Kill();
		}
	}

}


void A_LightingRod_Thinker(sprite_t *s)
{
	float	spritew,spriteh;
	int		spritex,spritey;
	int		sdamageh,sdamagee,sdamageb;
	int		i;
	int		tankx,tanky;
	int		spriteunderwater=0;

	// Particle effect
	particlenode_t	*p;
	int		nump = 15;
	int		plife;
	int		pangle;

	if (s->life == 1)
	{
		SetTankTimer(4.5*TICRATE);

		if (rand()%2==0)
			StartSound(THUNDERRUMBLE_1);
		else
			StartSound(THUNDERRUMBLE_2);
	}


	if (s->life <= (TICRATE/3))
	{
		//currentlevel.background_lightlevel = 40;
		currentlevel.background_lightlevel -= 10;

		//currentlevel.foreground_lightlevel = 100;
		currentlevel.foreground_lightlevel -= 6;

		ground.needsupdate = true;
		return;
	}

	if (s->life == (TICRATE*2))
	{
		sprite_t* l = G_SpawnSprite(s->x-25, s->y-SCREENHEIGHT, S_LIGHTNINGROD_LIGHTNING1, (MF_NORMAL));

		// Spawning underwater causes tanks to take much more damage (1/3 more)
		if (Underwater(s->x, s->y))
			spriteunderwater = 1;

		// Define all the base varibles here since they can be changed
		spritew = ((BITMAP *)spritedata[l->startstate.datafilelookup].dat)->w*(1.5);
		spriteh = ((BITMAP *)spritedata[l->startstate.datafilelookup].dat)->h;
		spritex = l->x;
		spritey = l->y;
		sdamageh = s->damageh;
		sdamagee = s->damagee;
		sdamageb = s->damageb;

		// Adjust some varibles if shooting lightning underwater
		if (spriteunderwater > 0)
		{
			// Wider radius
			spritew *= 3;
			spritex = s->x-(spritew/2);
		}

		// Oh yeah, might wanna burn the ground
		BurnGround(ground.bmp, s->x, s->y, 40, 50);
		BurnGround(ground.bmp, s->x, s->y, 20, 100);
		BurnGround(ground.bmp, s->x, s->y, 10, 150);
		BurnGround(ground.bmp, s->x, s->y, 5,  200);

		// White flash!
		currentlevel.whiteflashtics = 1;

		for (i=0; i<MAX_TANKS; i++)
		{
			if (ta[i].health <= 0)// || l->tankdamaged[i] == true)
				continue;

			tankx = ta[i].x - (ta[i].sizex/2);
			tanky = ta[i].y - (ta[i].sizey/2);
			if (check_bb_collision_general( tankx,
									tanky,
									20,//ta[i].sizex,
									14,//ta[i].sizey,
									spritex,
									spritey,
									spritew,
									spriteh) == true)
			{
				// The
				if (ta[i].underwater && spriteunderwater > 0)
					ta[i].Damage(sdamageh+(sdamageh/3), sdamagee+(sdamagee/3), sdamageb+(sdamageb/3), s->tankfrom, false);
				else
					ta[i].Damage(sdamageh, sdamagee, sdamageb, s->tankfrom, false);

				s->tankdamaged[i] = true;
			}


		}

		// Attack other sprites as well
		A_ContactSpriteDamage(s, 100, true);

		if (rand()%2==0)
			StartSound(THUNDERHIT_1);
		else
			StartSound(THUNDERHIT_2);

		// Spawn some particles, just for looks
		for (i=0; i<nump; i++)
		{
			// Set particle life, randomize this a bit
			plife = 30+rand()%30;

			// Randomize the angle as well
			pangle = 45+rand()%40;

			p = pdirt->InsertParticle(s->x, s->y-10, rand()%128, ConvertFromGamePower(300+rand()%100), makecol(200,200,255), plife, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
			if (p)
				p->bounce = 5;
		}

		s->flags |= MF_NOTVISABLE;
		return;
	}

	if (s->life > (TICRATE*4) && s->life < (TICRATE*6))
	{
		currentlevel.background_lightlevel +=6;
		currentlevel.foreground_lightlevel +=4;
		ground.needsupdate = true;
		return;
	}

	if (s->life == TICRATE*6)
	{
		currentlevel.background_lightlevel = 255;
		currentlevel.foreground_lightlevel = 255;
		ground.needsupdate = true;
		s->Kill();
		return;
	}
}

void A_GreenLaser(sprite_t* s)
{
	if (s->life > 1*TICRATE)
	{
		s->SetState(S_IONCANNON_DIE1);
		ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
	}

}

void A_IonChargerThinker(sprite_t *s)
{
	/*int waittimer = (states[S_IONCANNON_CHARGE1].tics +
					states[S_IONCANNON_CHARGE2].tics +
					states[S_IONCANNON_CHARGE3].tics +
					states[S_IONCANNON_CHARGE4].tics +
					states[S_IONCANNON_CHARGE5].tics)*2;*/

	int		waittimer = TICRATE;

	if (s->life >= waittimer)
		s->Kill();

}


void A_LaserPulseCheckTanks(sprite_t*	s)
{
	ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
	SetTankTimer(0.5*TICRATE);
}


void	A_FlameThrowerThinker(sprite_t* s)
{
	sprite_t	*f;

	if (!(s->life%2) || s->life == 1)
	{
		f = G_SpawnSprite(s->x, s->y, S_FLAME_BIG, (MF_NORMAL | MF_USEANGLE | MF_CENTERCOORD | MF_TRANSLUC));
		f->xvel = s->xvel;
		f->yvel = s->yvel;
		f->scale = 0.0;
		f->spawnfrom = s;
		f->damageh = s->damageh;
		f->damagee = s->damagee;
		f->damageb = s->damageb;
	}

	if (s->life > 0.5*TICRATE)
	{
		CheckTankReserveDamage(s->tankfrom);
		s->Kill();
	}
}

void	A_FlameThinker(sprite_t*	s)
{
	int		i;
	int		tankx,tanky;
	int		burnx=0, burny=0;		// Shut the stupid warnings up

	s->opacity-=2;
	s->scale +=0.20;

	if (s->angle >= 256)
		s->angle = 0;


	// Cycle through and see if we hit any tanks
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health <= 0 || s->tankdamaged[i] == true || s->spawnfrom->tankfrom == i)
			continue;

		tankx = ta[i].x - (ta[i].sizex/2);
		tanky = ta[i].y - (ta[i].sizey/2);
		if (check_pp_collision(ta[i].bmp,
								(BITMAP *)spritedata[s->startstate.datafilelookup].dat,
								tankx,
								tanky,
								s->x,
								s->y-((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h) > 0)
		{
			ta[i].reservedamageh += s->damageh;
			ta[i].reservedamagee += s->damagee;
			ta[i].reservedamageb += s->damageb;
			s->tankdamaged[i] = true;
		}

	}

	// Burn some sprites
	A_ContactSpriteDamage(s, 5);



	if (rand()%2 == 0)
		burnx = s->x + rand()%10;
	else
		burnx = s->x - rand()%10;

	if (rand()%2 == 0)
		burny = s->y + rand()%10;
	else
		burny = s->y - rand()%10;

	BurnGround(ground.bmp, burnx, burny, 15*s->scale, 10);

	if (s->life > 0.10*TICRATE)
	{
		s->Kill();
		return;
	}

	// Adjust physics
	s->x += s->xvel;
	s->yvel -= 0.025;
	s->y += s->yvel;

}

void	A_ElectricArcThinker(sprite_t* s)
{
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, TICRATE*60);

	if (s->life > 0.4*TICRATE)
	{
		ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
		s->Kill();
	}
}

void	A_HeatwaveSound(sprite_t* s)
{
	if (nosound > 0)		 return;	// Disabled sound
	play_sample((SAMPLE *)sounddata[EXPLODE_HEATWAVE].dat, 255, 128, 1000, 0);
}

//
// A_ContactTankDamage - Checks to see if any of the tanks clip with this sprite.
// If so, apply the appropiate damage
//

void A_ContactTankDamage(sprite_t *s)
{
	int			i;
	int			tankx,tanky;

	// Check for any tank contact
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health <= 0 || s->tankdamaged[i] == true)
			continue;

		tankx = ta[i].x - (ta[i].sizex/2);
		tanky = ta[i].y - (ta[i].sizey/2);
		if (check_pp_collision(ta[i].bmp,
								(BITMAP *)spritedata[s->startstate.datafilelookup].dat,
								tankx,
								tanky,
								s->x,
								s->y-((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h) > 0)
		{
			ta[i].Damage(s->damageh, s->damagee, s->damageb, s->tankfrom, false);
			s->tankdamaged[i] = true;
		}

	}
}


void A_ContactSpriteDamage(sprite_t* s, int tics, bool usebb)
{
	BITMAP	*	spbmp;
	sprite_t*	sp = sprites;
	int			sx,sy;
	int			spritex,spritey,spritew,spriteh;

	// No damage to dish?
	if ((s->damageh+s->damagee+s->damageb) <= 0)
		return;

	spritew = ((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->w;
	spriteh = ((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h;
	spritex = s->x;
	spritey = s->y;

	// Check for any shootable sprite contact
	while (sp)
	{
		if ((sp->flags & MF_SHOOTABLE) &&
			sp->invulntics <= 0 &&
			sp->health > 0)
		{
			// Define the bitmap
			spbmp = ((BITMAP *)spritedata[states[sp->startstate.currentstate].datafilelookup].dat);

			// Setup coordinates
			sx = sp->x;
			sy = sp->y;

			// Adjust to the upper left if we haven center coords
			if (sp->flags & MF_CENTERCOORD)
			{
				sx = sp->x - (spbmp->w/2);
				sy = sp->y - (spbmp->h/2);
			}


			if ((check_bb_collision_general( sx,
									sy,
									spbmp->w,
									spbmp->h,
									spritex,
									spritey,
									spritew,
									spriteh) == true && usebb==true) ||
				(check_pp_collision(spbmp,
								(BITMAP *)spritedata[s->startstate.datafilelookup].dat,
								sx,
								sy,
								s->x,
								s->y-((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h) > 0))
			{
				sp->Damage(s->damageh, s->damagee, s->damageb);

				// Mark this sprite as having damaged another sprite.
				// Its "stinger" will be removed so it cannot damage anything anymore.
				// Sameway tankdamaged[] works with tanks.
				sp->invulntics = tics;
			}

		}

		sp = sp->next;
	}

}


void A_EMPBlastSteal(sprite_t* s)
{
	BITMAP	*	spbmp;
	sprite_t*	sp = sprites;
	float		sx,sy;
	float		spritex,spritey,spritew,spriteh;

	// Clients don't deligate who loses their shield or sentry pods
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	spritew = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->w * (float)s->scale;
	spriteh = (float)((BITMAP *)spritedata[s->startstate.datafilelookup].dat)->h * (float)s->scale;
	spritex = s->x;
	spritey = s->y;

	spritex -= (float)(spritew/2);
	spritey -= (float)(spriteh/2);

	// Check for any shootable sprite contact
	while (sp)
	{
		if (sp->wobj >= 0)
		{

			if (sp->wobj == WO_HEATSENTRYPOD ||
				sp->wobj == WO_ENERGYSENTRYPOD ||
				sp->wobj == WO_BALLISTICSENTRYPOD ||
				sp->wobj == WO_MISSILETURRET)
			{
				// Define the bitmap
				spbmp = ((BITMAP *)spritedata[states[sp->startstate.currentstate].datafilelookup].dat);

				// Setup coordinates
				sx = sp->x;
				sy = sp->y;

				// Adjust to the upper left if we haven center coords
				if (sp->flags & MF_CENTERCOORD)
				{
					sx = sp->x - (spbmp->w/2);
					sy = sp->y - (spbmp->h/2);
				}

				// Debug
				//rect(buffer, sx, sy, sx+spbmp->w, sy+spbmp->h, makecol(0,255,0));
				//rect(buffer, spritex, spritey, spritex+spritew, spritey+spriteh, makecol(255,255,255));

				if (check_bb_collision_general(sx,
										sy,
										spbmp->w,
										spbmp->h,
										spritex,
										spritey,
										spritew,
										spriteh) == true)

				{
					// Convert it
					if (sp->tankfrom != s->tankfrom)
					{
						sp->tankfrom = s->tankfrom;

						sp->endlevelshoot = 10;

						if (sp->wobj == WO_ENERGYSENTRYPOD ||
							sp->wobj == WO_HEATSENTRYPOD ||
							sp->wobj == WO_BALLISTICSENTRYPOD)
						{
							// Choose a new target for our new master
							Sentry_ChooseTarget(sp);

							sp->StartSound(SENTRYPOD_EMP);

							sp->tintr = ta[s->tankfrom].colorr;
							sp->tintg = ta[s->tankfrom].colorg;
							sp->tintb = ta[s->tankfrom].colorb;
						}

						// Send a signal to all other clients
						if (multiplayer && multiplayermode == MP_SERVER)
						{
							BCast_WriteByte(STOC_CONVERTSENTRY);
							BCast_WriteByte(sp->netid);
							BCast_WriteByte(s->tankfrom);
						}
					}
				}
			}

		}

		sp = sp->next;
	}
}


void A_HeatwaveFlameBurnGround(sprite_t* s, int	numburnmarks)
{
	// 7 pixels to the right and left
	int		difference = 14;
	int		i;
	int		burnx;

	for (i=0; i<numburnmarks; i++)
	{
		// To the left
		if (rand()%2 == 0)
			burnx = s->x - rand()%difference;
		else
			burnx = s->x + rand()%difference;
		BurnGround(ground.bmp, burnx, (s->y-5)+rand()%7);
	}
}


void A_RotateMissile(sprite_t *s)
{
	s->angle+=8;
	if (s->angle > 255)
		s->angle = 0;
}

void A_RotateMissileFast(sprite_t *s)
{
	s->angle+=15;
	if (s->angle > 255)
		s->angle = 0;
}

void A_FinishRightHeatWave(sprite_t *s)
{
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, HEATWAVE_SPEED);
	s->Kill();
}

void A_FinishLeftHeatWave(sprite_t *s)
{
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, HEATWAVE_SPEED);
	s->Kill();
}

void A_SpawnLargeFlame(sprite_t	*s)
{
	sprite_t*	news;
	int			i;
	int		flags = (MF_NORMAL | MF_ONGROUND);
	int		spawnx,spawny;

	// Damage the tank & sprites
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, TICRATE*2);

	// Spawn flame to the left
	spawnx = (s->x-20);
	spawny = s->y;
	news = G_SpawnSprite(spawnx, spawny, S_HEATWAVE_LEFT_VERYSMALL2_1, flags);
	news->damageh = (s->damageh * 0.6);
	news->damagee = (s->damagee * 0.6);
	news->damageb = (s->damageb * 0.6);
	news->tankfrom = s->tankfrom;
	for (i=0; i<MAX_TANKS; i++)
		news->tankdamaged[i] = s->tankdamaged[i];

	// And spawn flame to the right
	spawnx = (s->x+20);
	spawny = s->y;
	news = G_SpawnSprite(spawnx, spawny, S_HEATWAVE_RIGHT_VERYSMALL2_1, flags);
	news->damageh = (s->damageh * 0.6);
	news->damagee = (s->damagee * 0.6);
	news->damageb = (s->damageb * 0.6);
	news->tankfrom = s->tankfrom;
	for (i=0; i<MAX_TANKS; i++)
		news->tankdamaged[i] = s->tankdamaged[i];

	A_HeatwaveFlameBurnGround(s, 40);

	// Set a flag here since we cannot kill the tank
	// within a sprite function after 1 seconds
	SetTankTimer(TICRATE);
}


void A_SpawnLeftMediumFlame(sprite_t	*s)
{
	sprite_t*	news;
	int		flags = (MF_NORMAL | MF_ONGROUND);
	int		spawnx,spawny;
	int		i;

	// Damage the tank
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, TICRATE*2);

	// Spawn flame to the left
	spawnx = (s->x-20);
	spawny = s->y;
	news = G_SpawnSprite(spawnx, spawny, S_HEATWAVE_LEFT_VERYSMALL3_1, flags);
	news->damageh = (s->damageh * 0.6);
	news->damagee = (s->damagee * 0.6);
	news->damageb = (s->damageb * 0.6);
	news->tankfrom = s->tankfrom;
	for (i=0; i<MAX_TANKS; i++)
		news->tankdamaged[i] = s->tankdamaged[i];

	A_HeatwaveFlameBurnGround(s, 20);

}

void A_SpawnRightMediumFlame(sprite_t	*s)
{
	sprite_t *	news;
	int		flags = (MF_NORMAL | MF_ONGROUND);
	int		spawnx,spawny;
	int		i;

	// Damage the tank
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, TICRATE*2);

	// And spawn flame to the right
	spawnx = (s->x+20);
	spawny = s->y;
	news = G_SpawnSprite(spawnx, spawny, S_HEATWAVE_RIGHT_VERYSMALL3_1, flags);
	news->damageh = (s->damageh * 0.6);
	news->damagee = (s->damagee * 0.6);
	news->damageb = (s->damageb * 0.6);
	news->tankfrom = s->tankfrom;
	for (i=0; i<MAX_TANKS; i++)
		news->tankdamaged[i] = s->tankdamaged[i];

	A_HeatwaveFlameBurnGround(s, 20);
}

void A_SpawnLeftSmallFlame(sprite_t	*s)
{
	sprite_t	*news;
	int		flags = (MF_NORMAL | MF_ONGROUND);
	int		spawnx,spawny;
	int		i;

	// Damage the tank
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, TICRATE*2);

	// Spawn flame to the left
	spawnx = (s->x-20);
	spawny = s->y;
	news = G_SpawnSprite(spawnx, spawny, S_HEATWAVE_LEFT_VERYSMALL4_1, flags);
	news->damageh = (s->damageh * 0.6);
	news->damagee = (s->damagee * 0.6);
	news->damageb = (s->damageb * 0.6);
	news->tankfrom = s->tankfrom;
	for (i=0; i<MAX_TANKS; i++)
		news->tankdamaged[i] = s->tankdamaged[i];

	A_HeatwaveFlameBurnGround(s, 20);
}

void A_SpawnRightSmallFlame(sprite_t	*s)
{
	sprite_t	*news;
	int		flags = (MF_NORMAL | MF_ONGROUND);
	int		spawnx,spawny;
	int		i;

	// Damage the tank
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, TICRATE*2);

	// And spawn flame to the right
	spawnx = (s->x+20);
	spawny = s->y;
	news = G_SpawnSprite(spawnx, spawny, S_HEATWAVE_RIGHT_VERYSMALL4_1, flags);
	news->damageh = (s->damageh * 0.6);
	news->damagee = (s->damagee * 0.6);
	news->damageb = (s->damageb * 0.6);
	news->tankfrom = s->tankfrom;
	for (i=0; i<MAX_TANKS; i++)
		news->tankdamaged[i] = s->tankdamaged[i];

	A_HeatwaveFlameBurnGround(s, 20);
}

void A_SpawnLeftVerySmallFlame(sprite_t	*s)
{
	sprite_t	*news;
	int		flags = (MF_NORMAL | MF_ONGROUND);
	int		spawnx,spawny;
	int		i;

	// Damage the tank
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, TICRATE*2);

	// Spawn flame to the left
	spawnx = (s->x-20);
	spawny = s->y;
	news = G_SpawnSprite(spawnx, spawny, S_HEATWAVE_LEFT_VERYSMALL5_1, flags);
	news->damageh = (s->damageh * 0.6);
	news->damagee = (s->damagee * 0.6);
	news->damageb = (s->damageb * 0.6);
	news->tankfrom = s->tankfrom;
	for (i=0; i<MAX_TANKS; i++)
		news->tankdamaged[i] = s->tankdamaged[i];

	A_HeatwaveFlameBurnGround(s, 10);

}

void A_SpawnRightVerySmallFlame(sprite_t	*s)
{
	sprite_t *news;
	int		flags = (MF_NORMAL | MF_ONGROUND);
	int		spawnx,spawny;
	int		i;

	// Damage the tank
	A_ContactTankDamage(s);
	A_ContactSpriteDamage(s, TICRATE*2);

	// And spawn flame to the right
	spawnx = (s->x+20);
	spawny = s->y;
	news = G_SpawnSprite(spawnx, spawny, S_HEATWAVE_RIGHT_VERYSMALL5_1, flags);
	news->damageh = (s->damageh * 0.6);
	news->damagee = (s->damagee * 0.6);
	news->damageb = (s->damageb * 0.6);
	news->tankfrom = s->tankfrom;
	for (i=0; i<MAX_TANKS; i++)
		news->tankdamaged[i] = s->tankdamaged[i];

	A_HeatwaveFlameBurnGround(s, 10);
}


void	A_SmokeThinkerXLarge(sprite_t*	s)
{
	s->angle+=3;
	s->opacity-=3;
	s->scale += 0.075;

	if (s->opacity <= 0)
	{
		s->Kill();
		return;
	}

	if (s->angle >= 256)
		s->angle = 0;

	s->y-=0.75;

}


void	A_SmallBubbleThinker(sprite_t *s)
{
	int		direction;

	//if (!(gametics % 2))
	{
		direction = rand()%3;
		if (direction==0)
			s->xvel = -2;
		else if (direction == 1)
			s->xvel = 2;
		else
			s->xvel = 0;
	}


	if (Underwater((int)s->x, (int)s->y))
	{
		s->x += s->xvel;
		s->y--;
	}
	else
	{
		s->Kill();
		return;
	}
}

void	A_MediumBubbleThinker(sprite_t *s)
{
	int		direction;

	//if (!(gametics % 2))
	{
		direction = rand()%3;
		if (direction==0)
			s->xvel = -1;
		else if (direction == 1)
			s->xvel = 1;
		else
			s->xvel = 0;
	}


	if (Underwater((int)s->x, (int)s->y))
	{
		s->x += s->xvel;
		s->y--;
	}
	else
	{
		s->Kill();
		return;
	}
}


void	A_LargeBubbleThinker(sprite_t *s)
{
	int		direction;

	//if (!(gametics % 2))
	{
		direction = rand()%3;
		if (direction==0)
			s->xvel = -0.5;
		else if (direction == 1)
			s->xvel = 0.5;
		else
			s->xvel = 0;
	}


	if (Underwater((int)s->x, (int)s->y))
	{
		s->x += s->xvel;
		s->y--;
	}
	else
	{
		s->Kill();
		return;
	}
}


void Setup_Sprites(int bpp)
{

	int		i,x,y,c,a;

	set_color_depth(32);

	alphadata = load_datafile("data\\alphas.dat");
  if(!alphadata)
		return;  // load didn't work -- calling routine to do exit.

	for (i=0; i<NUM_STATES; i++)
	{
		if (states[i].flags & HAS_ALPHA)
		{
			// Setup alpha masking here
			drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
			set_write_alpha_blender();

			// Setup the alpha sprite in the state struct
			states[i].alpha_sprite = (BITMAP *)alphadata[states[i].datafilelookup].dat;


			for (y=0; y<states[i].alpha_sprite->h; y++)
			{
				for (x=0; x<states[i].alpha_sprite->w; x++)
				{
					c = getpixel(states[i].alpha_sprite, x, y);
					a = getr(c) + getg(c) + getb(c);
					a = MID(0, a/2-128, 255);

					putpixel(states[i].alpha_sprite, x, y, a);
				}
			}
		}

	} // end for()


	drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

	set_color_depth(bpp);
}



// Slap a transparent flag on there!
void A_GoTrans(sprite_t	*s)
{
        s->flags |= MF_TRANSLUC;
}


// Real simple test function to make sure this works!

void A_TestMove(sprite_t	*s)
{
        s->x+=4;

        if (s->x >= SCREENWIDTH)
        {
                s->x = 0;
        }
}

void A_Kill(sprite_t	*s)
{
	s->Kill();
}

void A_ResetLife(sprite_t *s)
{ s->life = 0;
}

// While we're on the subject here...
// Cycle through all the sprites and see if we're waiting for a sprite
// to finish for advanceturn()
//
bool G_IsAdvanceTurnSprite(void)
{

	sprite_t	*s = sprites;

	while (s)
	{
		if (s->flags & MF_ADVANCETURN)
			return true;
		s = s->next;
	}
	return false;
}


//
// G_DeleteAllSprites - Clears the entire sprites array, cleaing them all
//

void G_DeleteAllSprites(void)
{
	sprite_t	*p;

	while (sprites)
	{
		p = sprites;
		sprites = p->next;
		delete p;
	}

	sprites = NULL;
	lastsprite = firstsprite = NULL;
	numsprites = 0;
}


void G_InitSprites(void)
{
		spritedata = load_datafile("data\\sprites.dat");

		if (!spritedata)
			{
#ifdef		DEDICATED_SERVER
			Printf("Error  --  Unable to load sprites.dat\n");
			getchar();
#else
			alert("Error", "Unable to load sprites.dat", NULL, "Exit", NULL, 0,0);
#endif
			exit(-1);
			}

		// Wipe out the entire sprite array if we
		// havent already done so
		if (sprites)
			G_DeleteAllSprites();
}


int			G_GetNetID(void)
{
	sprite_t*	s = sprites;
	int			id = 0;

nextid:

	id++;
	while (s)
		{
		if(s->netid == id)
			goto nextid;
		s = s->next;
		}
	return id;
}


sprite_t*	G_FindNetID(int id)
{
	sprite_t*	s = sprites;

	while (s)
	{
		if (s->netid == id)
			return s;

		s = s->next;
	}

	// Cannot find a match
	return NULL;
}

sprite_t* G_SpawnWO(_wobject objectnum, int x, int y)
{
	sprite_t*		s;

	// Error checking
	if (objectnum > NUM_WOBJECTS)
		return NULL;

	if(novideoanimation->value > 0) // disable any nonplay animation
		{
		if(objectnum == WO_SNOWFLAKESPAWNER  )return NULL;
		if(objectnum == WO_CHERRYBLOSSSPAWNER)return NULL;
		if(objectnum == WO_RAINSPAWNER       )return NULL;
		if(objectnum == WO_HEATCORONASPAWNER )return NULL;
//		G_Printf("skip animation");
		}
//	else
//		G_Printf("run animation");

	// Call a spawn sprite and setup the varibles
	s = G_SpawnSprite(x, y, wobjects[objectnum].spawnstate, wobjects[objectnum].flags);

	// Setup some extra varibles
	s->spawnstate = wobjects[objectnum].spawnstate;
	s->damagestate = wobjects[objectnum].damagestate;
	s->deathstate = wobjects[objectnum].deathstate;
	s->starthealth = s->health = wobjects[objectnum].health;

	// Set general width, height for status bar
	s->width = wobjects[objectnum].width;
	s->height = wobjects[objectnum].height;

	// Setup shootable sprite HEB
	s->h = wobjects[objectnum].h;
	s->e = wobjects[objectnum].e;
	s->b = wobjects[objectnum].b;

	// Setup HEB contact damage
	s->damageh = wobjects[objectnum].damageh;
	s->damagee = wobjects[objectnum].damagee;
	s->damageb = wobjects[objectnum].damageb;

	s->wobj = objectnum;

	if (multiplayer && multiplayermode == MP_SERVER)
		s->netid = G_GetNetID();

	return s;
}

//
// G_SpawnSprites - Spawns any top layer sprites
//

sprite_t* G_SpawnSprite(int x, int y, _state state, int flags)
{
	sprite_t*		s = new sprite_t;
	int				i;

	if (!s)
	{
#ifdef	DEDICATED_SERVER
		Printf("Unable to spawn sprite (memory error)\n");
#else
		alert("Whoa!", "Unable to spawn sprite (not enough memory)", NULL, "Ok", NULL, 0,0);
#endif
		return NULL;
	}

	// Set everything to 0
	memset(s, 0, sizeof(sprite_t));

	s->x = s->oldx = x;
	s->y = s->oldy = y;
	s->h = s->e = s->b = 0;
	s->reservedamageh = s->reservedamagee = s->reservedamageb = 0;
	s->tankfrom = -1;
	s->targettank = -1;
	s->startstate = states[state];
	s->wobj = -1;
	s->tics = states[state].tics;
	s->active = 10;
	s->xvel = s->oldxvel = 0;
	s->yvel = s->oldyvel = 0;
	s->opacity = 255;
	s->scale = 1;
	s->id = numsprites;
	s->flags = flags;
	s->life = 0;
	s->invulntics = 0;
	s->angle = 0;
	s->needstodie = -1;
	s->rotatex = -1;
	s->rotatey = -1;
	s->healthbartrans = 0;
	s->endlevelshoot = 0;
	s->bouncecount = 0;
	s->netid = -1;
	s->spawnfrom = NULL;

	for (i=0; i<MAX_TANKS; i++)
		s->tankdamaged[i] = false;

	s->spritedamaged = false;

	if (s->flags & MF_ONGROUND)
		{
		s->y = OnGround(x, y, ground.bmp);
		s->y+=5;
		}

	// Old code
	/*s->next = sprites;
	s->prev = NULL;
	if (sprites)
		sprites->prev = s;

	sprites = s;*/

	// New code  =)
	s->next = NULL;
	s->prev = NULL;

	// Nothing in the list
	if (sprites == NULL)
	{
		sprites = lastsprite = s;
	}

	// Exactly one item in the list
	else if (sprites != NULL && (sprites == lastsprite))
	{
		sprites->next = s;
		lastsprite = s;
	}

	// In the middle
	else
	{
		lastsprite->next = s;
		lastsprite = s;
	}

	numsprites++;

	return s;
}



void G_KillSprite(sprite_t*	s)
{
	// Use the class function
	s->Kill();
}


//
// Change a state on the fly!
//

void	sprite_t::SetState(_state state)
{
	if (state > NUM_STATES)
		return;

	startstate = states[state];
	tics = startstate.tics;

	if (startstate.currentstate == S_NULL)
	{ Kill(); }
}


//
// Damage a sprite
//

void	sprite_t::Damage(float damageh, float damagee, float damageb, bool scattertext)
{
	char		str[32];
	int			amount;

	if (invulntics > 0)
		return;

	// The server will do our damage for us
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	damageh = ((10-h)*.1)*damageh;
	damagee = ((10-e)*.1)*damagee;
	damageb = ((10-b)*.1)*damageb;

	amount = damageh + damagee + damageb;

	// Decrease health
	health -= amount;

	// Inform all other players of this
	if (multiplayer && multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_DAMAGESPRITE);
		BCast_WriteShort(netid);
		BCast_WriteShort(amount);
		if (scattertext)
			BCast_WriteByte(1);
		else
			BCast_WriteByte(0);
	}

	// Need to die?
	if (health <= 0)
	{
		// No longer shootable
		if (flags & MF_SHOOTABLE)
			flags &= ~MF_SHOOTABLE;

		// Set in death state if we have one. Otherwise, just wipe em out
		if (wobj >= 0)
		{
			SetState(wobjects[wobj].deathstate);

			if (multiplayer && multiplayermode == MP_SERVER)
			{
				BCast_WriteByte(STOC_SPRITESTATE);
				BCast_WriteShort(netid);
				BCast_WriteShort((int)wobjects[wobj].deathstate);
			}
		}
		else
			Kill();
	}

	healthbartrans = 255;

	// Display damage amount
	sprintf(str, "-%d", abs((int)(amount)));
	if (scattertext || wobj != -1)
	{
		SpawnGameText(str, (int)(1.2*TICRATE), x+rand()%width, y+rand()%(height/4), 0, -6, makecol(255,0,0), (FONT *)(FONT*)maindata[MS].dat, 0);
	}
	else
		SpawnGameText(str, (int)(1.2*TICRATE), x, y, 0, -6, makecol(255,0,0));
}

//
// CheckReserveDamage - Checks for any reserve damage and applied it to the sprite now
//

void	sprite_t::CheckReserveDamage(void)
{
	int		totaldamage;

	// Calculate the total damage with all three damage classes
	totaldamage = reservedamageh +
					reservedamagee +
					reservedamageb;

	// Nothing to appliy
	if (totaldamage <= 0)
		return;

	// Okay, apply the damage then
	Damage(reservedamageh, reservedamagee, reservedamageb);

	reservedamageh = reservedamagee = reservedamageb = 0;
}


//
// StartSound - Plays a sound that orginates from a projectile
//

void	sprite_t::StartSound(int sound, int loop, int stopcurrent)
{
	// Disabled sound
	if (nosound > 0)
		return;

	if (stopcurrent > 0)
		StopSound();

	// Allocate voice channel
	sndvoice = allocate_voice((SAMPLE *)sounddata[sound].dat);

	// No voice channels available
	if (sndvoice == -1)
		return;

	// Always high priority
	voice_set_priority(sndvoice, 255);

	// Set volume
	voice_set_volume(sndvoice, Cvar_VariableValue("soundvolume"));

	// Looping?
	if (loop > 0)
		voice_set_playmode(sndvoice, PLAYMODE_LOOP);

	// Start the sound
	voice_start(sndvoice);

	if (loop <= 0)
	release_voice(sndvoice);
}

void	sprite_t::StopSound(void)
{
	// Already deallocated? (or never allocated?)
	if (!voice_check(sndvoice))
		return;

	//release_voice(sndvoice);

	// Stop sound & release memory
	deallocate_voice(sndvoice);
}

//
// OnGround - Exact same code for tanks, just a little cuttie-cuttie/paste paste job
// Add some extra varibles and you've got a new function!  =)
//

void	sprite_t::OnGround2(BITMAP	*bmp)
{
	int			a,b;
	int			sizex,sizey;
	BITMAP		*sbmp;

	// Set the bitmap pointer
	sbmp = ((BITMAP *)spritedata[states[startstate.currentstate].datafilelookup].dat);

	// Set sizes
	sizex = sbmp->w;
	sizey = sbmp->h;

	y=0;
	for (int i=0; i<SCREENHEIGHT; i++)
	{

		if ((y+sizey) >= SCREENHEIGHT || (y+sizey) <= 0)
			return;

		a=getpixel(bmp,(int)(x),(int)(y+sizey));
		b=getpixel(bmp,(int)(x+sizex),(int)(y+sizey));


	    //if (!(a != TRANSCOLOR || b != TRANSCOLOR))   y++;
		if (a == TRANSCOLOR && b == TRANSCOLOR)
			y++;
		else
			return;
	}
}

//
// Sets a flag to remove the this sprite on the next tic
//

sprite_t	*sprite_t::Kill(void)
{
	sprite_t*		s = this;

	//s = KillSprite(s);
	needstodie = 10;

	return NULL;


}


//
// DrawDescBox - Like the tanks description box, but this will let you
// customize what you want each box to say. Usually (name/owner) for shootable
// sprites
//

void	sprite_t::DrawDescBox(BITMAP *b, char	*line1, char *line2)
{
	BITMAP	*sbmp = ((BITMAP *)spritedata[states[startstate.currentstate].datafilelookup].dat);
	int		bx,by,bw,bh;
	int		dx,dy;		// Drawing x & drawing y
	int		colorr,colorg,colorb;
	char	str[64];

	// Default coordinates
	dx = x + (sbmp->w/2);
	dy = y;

	// Special if this sprite is centered
	if (flags & MF_CENTERCOORD)
	{
		dx = x;
		dy = y - (sbmp->h/2);
	}

	// Drawing a 105x15 box
	bx = dx-(105/2);
	by = dy-65;
	bw = 105;
	bh = 40;

	// Add the dirty rectangle first
	AddRectangle(b, bx, by, bw+1, bh+1);

	// Set the drawing mode
	drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	set_trans_blender(0,0,0,150);

	rectfill(b, bx, by, bx+bw, by+bh, makecol(0,0,0));
	solid_mode();

	if (tankfrom >= 0)
	{
		colorr = ta[tankfrom].colorr;
		colorg = ta[tankfrom].colorg;
		colorb = ta[tankfrom].colorb;
	}
	else
	{
		colorr = colorg = colorb = 255;
	}
	rect(b, bx+1, by+1, bx+bw-1, by+bh-1, makecol(colorr, colorg, colorb));

	// Name
	textout_centre(b,(FONT *)maindata[TINY8].dat, line1, dx, dy-(65-3), makecol(255,255,255));

	// Money
	sprintf(str, "(%s)", line2);
	textout_centre(b,(FONT *)maindata[TINY8].dat, str, dx, dy-(45-3), makecol(255,255,128));
}


//
// G_DestroyAllSprites - Same function as above, this should really be taken out
//
void G_DestroyAllSprites(void)
{
	G_DeleteAllSprites();
}

//
// G_SpriteTicker -
//

void G_SpriteTicker(void)
{
	sprite_t	*s = sprites;
	sprite_t	*prev = NULL;
	sprite_t	*temp = NULL;

	// Reset
	numwobjsprites = 0;
	while (s)
	{
		// Keep track of world object sprites (tracked for multiplayer)
		if (s->wobj >= 0)
			numwobjsprites++;

		if (s->flags & MF_FOLLOWTANK)
			A_FollowTank(s);

		// Process xvel and yvel
		if (!(s->flags & MF_NOGRAVITY))
		{
			if (s->flags & MF_GROUNDCLIP)
			{
				int		bmpw, bmph;
				int		x,y;

				// See if we've hit land of any sort
				bmpw = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat)->w;
				bmph = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat)->h;

				// Setup basic coordinates
				x = s->x;
				y = s->y;

				// New coordinate if this is centered
				if (s->flags & MF_CENTERCOORD)
				{
					x = s->x - (bmpw/2);
					y = s->y - (bmpw/2);
				}

				// If both sides of the sprite are embedded into the ground, not much place
				// we can go. Stop y velocity here.
				if (getpixel(ground.bmp, x, y+bmph) != TRANSCOLOR ||
					getpixel(ground.bmp, x+bmpw, y+bmph) != TRANSCOLOR)
				{
					// Does this sprite bounce off the ground?
					if (s->flags & MF_BOUNCE)
						{
						s->yvel = -(s->yvel/2);
						s->bouncecount++;
						}
					else
						s->yvel = 0;
				}
				else
				{	// Apply normal gravity
					s->yvel += GRAVITY;
				}
			}

			// Tower Pod Gravity. Any sprite will be affected by gravity while this will also
			// check to see if contact with land has been made
			else if (s->flags & MF_TPGRAVITY)
			{
				int		bmpw, bmph;
				int		x,y;

				// See if we've hit land of any sort
				bmpw = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat)->w;
				bmph = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat)->h;

				// Setup basic coordinates
				x = s->x;
				y = s->y;

				// New coordinate if this is centered
				if (s->flags & MF_CENTERCOORD)
				{
					x = s->x - (bmpw/2);
					y = s->y - (bmpw/2);
				}

				// If both sides of the sprite are embedded into the ground, not much place
				// we can go. Stop y velocity here.
				if (getpixel(ground.bmp, x, y+bmph) != TRANSCOLOR &&
					getpixel(ground.bmp, x+bmpw, y+bmph) != TRANSCOLOR)
				{
					// Does this sprite bounce off the ground?
					if (s->flags & MF_BOUNCE)
					{
						s->yvel = -(s->yvel/2);
						s->bouncecount++;
					}
					else
						s->yvel = 0;
				}
				else
				{	// Apply normal gravity
					s->yvel += GRAVITY;
				}


			}

			// Apply veloctity
			s->x += s->xvel;
			s->y += s->yvel;
		}

		if (s->tics == s->startstate.tics && states[s->startstate.currentstate].func != NULL)
			states[s->startstate.currentstate].func(s);

		// Shave tics off to determine when it is time to change states
		s->tics--;

		// Keep track of how long the sprite has been in play
		s->life ++;

		// Health bar transparency
		if (s->healthbartrans > 0)
		{
			s->healthbartrans -= 5;
			if (s->healthbartrans < 0)
				s->healthbartrans = 0;
		}

		// Count down invulnerablity tics
		if (s->invulntics > 0)
			s->invulntics--;

		// Record some old varibles
		s->oldx = s->x;
		s->oldy = s->y;
		s->oldxvel = s->xvel;
		s->oldyvel = s->yvel;

		// Go on to the next state
		if (s->tics <= 0)
		{
			s->startstate = states[s->startstate.nextstate];
			s->tics = s->startstate.tics;

			if (s->startstate.currentstate == S_NULL)
				{
				s->Kill();
				}
		}

		// See if the sprite is ready to be wiped out
		if (s->needstodie > 0)
		{
			// Old code
			/*if (prev)
				prev->next = s->next;
			else
				sprites = s->next;

			temp = s;
			s = s->next;

			numsprites--;
			delete temp;*/

			// New code

			// Only one element
			if (sprites == lastsprite)
			{
				delete sprites;
				sprites = lastsprite = NULL;

				numsprites = 0;
				return;
			}

			// Front of the list
			else if (s == sprites)
			{
				temp = s;
				s = sprites = sprites->next;

				numsprites--;
				delete temp;
			}

			// End of the list
			else if (s == lastsprite)
			{
				prev->next = NULL;
				lastsprite = prev;

				temp = s;
				s = s->next;

				numsprites--;
				delete temp;
			}

			// In the middle of the list
			else
			{
				if (prev)
					prev->next = s->next;
				else
					sprites = s->next;

				temp = s;
				s = s->next;

				numsprites--;
				delete temp;

			}

		}
		else		// Advance the sprite
		{
			prev = s;
			s = s->next;
		}
	}

}


//
// G_SpriteDrawer
//

void    G_SpriteDrawer(BITMAP *buf)
{
    int         x,y;
		int				  rscale;
		BITMAP			*bmp, *temp=NULL, *temp2=NULL;
		BITMAP			*temptint=NULL;
#ifdef	_DEBUG
		int		index=0;
#endif

		sprite_t*		s = sprites;

		while (s)
		{
#ifdef	_DEBUG
			index++;
#endif

			if ((s->flags & MF_NORMAL) && !(s->flags & MF_NOTVISABLE))
			{
				// Setup inital x y coords
				x = s->x;
				y = s->y;

				// Is this a alpha sprite or a normal sprite?
				if (s->flags & MF_ALPHA)
					bmp = (BITMAP *)alphadata[s->startstate.datafilelookup].dat;
				else
					bmp = (BITMAP *)spritedata[s->startstate.datafilelookup].dat;

				// Is the sprite flipped?
				/*if (s->flags & MF_FLIPPED)
				{
					temp = create_bitmap(bmp->w, bmp->h);
					clear_to_color(temp, TRANSCOLOR);
					//blit(bmp, temp, 0,0,0,0, bmp->w, bmp->h)
					draw_sprite_flipped(temp, bmp, 0,0);

					bmp = temp;
				}*/

				// Before we do any coordinate adjusting, we should check and see
				// if any changes need to be done to the orginal bitmap. If so, do them and
				// then proceed to setup things such as MF_CENTERCOORD

				// Is the image being scaled up or down?
				if (s->scale != 1)
				{
					float	scalew,scaleh;

					// Error checking
					if (s->scale < 1 && !(s->flags & MF_ALLOWSMALLSCALE))
						s->scale = 1;

					scalew = (float)bmp->w*s->scale;
					scaleh = (float)bmp->h*s->scale;

					// Too small
					if (scalew < 1 || scaleh < 1)
						return;

					temp = create_bitmap(scalew, scaleh);
					clear_to_color(temp, TRANSCOLOR);

					// Scale the bitmap
					masked_stretch_blit(bmp, temp, 0, 0, bmp->w, bmp->h, 0, 0, temp->w, temp->h);

					// Make temp our new bitmap
					bmp = temp;

				}

				// Setup the x and y coords so that they point to the center of
				// the sprite
				if (s->flags & MF_CENTERCOORD)
				{
					x = s->x-(bmp->w/2);
					y = s->y-(bmp->h/2);
				}

				// Draw the sprite sitting on the ground
				if (s->flags & MF_ONGROUND)
				{
					y = s->y-(bmp->h);
					y+=5;
				}


				if ((s->flags & MF_SHOOTABLE) && gamestate == GS_INGAME)
				{
					if (GUI_MouseInside(x, y, x+s->width, y+s->height) || key[KEY_ALT])
					{
						s->healthbartrans = 255;

						if (s->wobj >= 0 && s->tankfrom >= 0)
							s->DrawDescBox(buf, wobjects[s->wobj].name, ta[s->tankfrom].name);
					}
					else
						s->healthbartrans = 0;

					if (s->healthbartrans > 0)
					{
						DrawPowerEnergyBar(buf, x+(s->width/2), (y+s->height)+3,
							s->width-1, 10,
							s->healthbartrans,
							s->health, wobjects[s->wobj].health);
					}

				}


				// Do not blit the screen, since we may still have transparancy or
				// alpha channels to deal with
				if (s->flags & MF_USEANGLE)
				{
					// The mess you see below is only rigged for sprites that rotate
					// and also have some form of transparency. We cannot blit to the
					// screen at this point, so we have to rotate on a memory bitmap
					// and blit the translucent sprite in a bit
					if (bmp->w > bmp->h)
							rscale = bmp->w;
						else
							rscale = bmp->h;

					if (s->flags & MF_TRANSLUC)
					{
						// Create a huge bitmap
						temp2 = create_bitmap(rscale, rscale);
						clear_to_color(temp2, TRANSCOLOR);

						// Draw the sprite in the middle
						rotate_sprite(temp2,
									bmp,
									0, 0,
									itofix((int)s->angle));

						bmp = temp2;

						x = s->x-(temp2->w/2);
						y = s->y-(temp2->h/2);

					}
					else
					{
						AddRectangle(buf, (s->x-(rscale)), (s->y-(rscale)), rscale*2, rscale*2);
						rotate_sprite(buf,
									bmp,
									x, y,
									itofix((int)s->angle));
						goto end;
					}

				}

				// There is a color tint
				if (s->tintr > 0 ||
				    s->tintg > 0 ||
				    s->tintb > 0)
				{
					temptint = create_bitmap(bmp->w, bmp->h);
					clear_to_color(temptint, TRANSCOLOR);

					set_trans_blender(s->tintr,s->tintg,s->tintb,0);
					draw_lit_sprite(temptint, bmp, 0, 0, s->tintvalue);
					solid_mode();

					bmp = temptint;
				}

				if (s->rotatex >= 0 && s->rotatey >= 0)
				{
					if (bmp->w > bmp->h)
						rscale = bmp->w;
					else
						rscale = bmp->h;

					//AddRectangle(buf, x, y, bmp->w, bmp->h);
					AddRectangle(buf, (s->x-(rscale)), (s->y-(rscale)), rscale*2, rscale*2);
					pivot_sprite(buf, bmp, s->x, s->y, s->rotatex, s->rotatey, ftofix(s->angle));

					goto end;
				}

				// Actual blitting
				if (s->flags & MF_COOLEFFECT1)
				{
					// Add dirty rectangle
					AddRectangle(buf, x,y,bmp->w, bmp->h);

					set_add_blender(255,255,255,s->opacity);
					draw_trans_sprite(buf, bmp, x, y);
					solid_mode();
				}
				else if (s->flags & MF_TRANSLUC || s->opacity < 255)
				{
					// Add dirty rectangle
					AddRectangle(buf, x,y,bmp->w, bmp->h);

					//set_trans_blender(0, 0, 0, s->opacity);
					//draw_trans_sprite(buf, bmp, /*(BITMAP *)spritedata[s->startstate.datafilelookup].dat,*/ x, y);
					fblend_trans(bmp, buf, x, y, s->opacity);
				}
				else if (s->flags & MF_ALPHA)
				{
					// Add dirty rectangle
					AddRectangle(buf, x,y,bmp->w, bmp->h);

					set_alpha_blender();
					drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);

					draw_trans_sprite(buf, bmp, x, y);

					drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
				}
				else if (s->flags & MF_FLIPPED)
				{
					AddRectangle(buf, x,y,bmp->w, bmp->h);
					draw_sprite_h_flip(buf, bmp, x, y);
				}
				else
				{
					// Add dirty rectangle
					AddRectangle(buf, x,y,bmp->w, bmp->h);
					draw_sprite(buf, bmp, x, y);
                }

			}
end:

			if (temp)
			{
				destroy_bitmap(temp);
				temp = NULL;
			}

			if (temp2)
			{
				destroy_bitmap(temp2);
				temp2 = NULL;
			}

			if (temptint)
			{
				destroy_bitmap(temptint);
				temptint = NULL;
			}
			s = s->next;
		}
}

