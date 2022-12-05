#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"
#include "particles.h"
#include "secrets.h"

//   PREPROCESSORS
#define	SECRET_FILENAME	"cdirt.ssf"

// GLOBAL VARIBLES
int		cheattimer;
int		testNumber = -1;
void testunlock(int qualify);

char *levelwarp_getter(int index, int *list_size)
{

	if (index < 0) {
      *list_size = level_filecount;
      return NULL;
   }
   else
      return level_filenames[index]; 
}

int	warp_donothing(void)
{
	return D_O_K;
}

DIALOG dialog_levelwarp[] =
{
   { d_agup_window_proc,	247,  133,  292,  342,          255,              0, 0,         0, 0,			   0,  (char *)"Level Warp!" },
   { d_agup_list_proc,		265,  177,  256,  258,          255,              0, 0,         0, 0,			   0,  (char *)levelwarp_getter },
   { d_agup_button_proc,	410,  447,   53,   20,          255,              0, 0,    D_EXIT, 0,		       0,  (char *)"Cancel" },
   { d_agup_button_proc,	473,  447,   53,   20,          255,              0, 0,    D_EXIT, 0,		       0,  (char *)"Warp!" },
 // Some text
   { d_text_proc,			260,  157,	 0,	   0, agup_fg_color,  agup_bg_color, 0,         0, 0,			   0,  (char *)"Select a level to warp to:" },
   { NULL,                    0,    0,   0,    0,             0,              0, 0,         0, 0,              0,  0,                    0,     0 }
   //{ d_keyboard_proc,	0,    0,    0,   0,    0,             0,              0, 0, KEY_ENTER, 0, warp_donothing,	NULL, NULL  },
};

void	cheat_netinfo(void)
{
//	char	str[128];

	/*sprintf(str, "Sockets opened: %d", nlGetInteger(NL_OPEN_SOCKETS));
	G_Printf(str);

	sprintf(str, "Bytes sent: %d", nlGetInteger(NL_BYTES_SENT));
	G_Printf(str);

	sprintf(str, "Bytes recieved: %d", nlGetInteger(NL_BYTES_RECEIVED));
	G_Printf(str);
	*/

	cheattimer = 30;
}

void	cheat_reclassplayers(void)
{
	used_cheat = 1;
	if (!multiplayer)
		PS_RunReclass();
}

void	cheat_moneycheat(void)
{
	int		i;
	int		tankx,tanky;

	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	if (key[KEY_4])
	{
		if (key_shifts & KB_SHIFT_FLAG)	// the $ key is shift 4
		{
			for (i=0; i<MAX_TANKS; i++)
			{
				if (ta[i].health <= 0)
					continue;

				tankx = ((int)ta[i].x-(int)(ta[i].sizex/2));
				tanky = ((int)ta[i].y-(int)(ta[i].sizey/2));

				// Check to see if any tanks are being clicked on
				if (GUI_MouseInside(tankx, tanky, tankx+ta[i].sizex, tanky+ta[i].sizey) &&
					(mouse_b & 1))
				{
					SpawnGameText("+$100000", TICRATE, (int)ta[i].x, (int)ta[i].y, 0,-3, makecol(0,255,0));
					//ta[i].GiveMoney(5000);
					ta[i].money += 100000;
					cheattimer = (int)((1*TICRATE)/2);

					// This is a cheat as well
					used_cheat = 1;

					return;
				}
			}	
		}
	}
}

void	cheat_levelwarp(void)
{

	int ret;

	// Clients cannot warp, the host controls this
	if (multiplayer && multiplayermode == MP_CLIENT)
	{
		G_Printf("Only the host can level warp.");
		cheattimer = 1*TICRATE;
		return;
	}

	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	GenerateLevelList();
	
	//stop_timer();
	ret = Start_Dialog(dialog_levelwarp);
	//start_timer();

	text_mode(-1);

	if (ret == 3)
		{
		// Flag this as cheating
		used_cheat = 1;
		cheattimer = 1*TICRATE;

		// Whoop! Do the warp!
		Inter_Run(level_filenames[dialog_levelwarp[1].d1]);
		InitDefaultTurnQue();
		}
}

void	cheat_tankexplode(void)
{
	// Just a draws a explosion sprite at all the tanks, just a neat
	// effect for testing out alpha sprites
	int		i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health > 0)
		{
			sprite_t	*s;
			s = G_SpawnSprite((int)ta[i].x, (int)ta[i].y-50, S_TURNARROW, (MF_NORMAL | MF_CENTERCOORD));
			if (s)
			{
				s->tintr = ta[i].colorr;
				s->tintg = ta[i].colorg;
				s->tintb = ta[i].colorb;
				s->tintvalue = TANK_OPACITY;
				s->opacity = 255;
				s->misc1 = 0;
			}
		}
	}

	cheattimer = (int)1.5*TICRATE;
	used_cheat = 1;
}


void	cheat_texttime(void)
{
	int		i;
	char	str[32];

	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health > 0)
		{
			sprintf(str, "%s", ta[ta[i].targettank].name);
			SpawnGameText(str, 1.2*TICRATE, (int)ta[i].x, (int)ta[i].y, 0, -6, makecol(255,50,165), (FONT *)maindata[TINY8].dat);
		}
	}

	// As minor as it is...this is considered a cheat
	used_cheat = 1;

	cheattimer = 10;
}

void	cheat_particles(void)
{
	//int		i;
	//float speed;
	/*particlenode_t	*p;

	for (i=0; i<128; i++)
	{
		//NewParticle(mouse_x, mouse_y, i, ConvertFromGamePower(200), makecol(255,50,0), 10*TICRATE, (PFLAGS_LIQUID|PFLAGS_WATER), 0);
		p = pdirt->InsertParticle(mouse_x, mouse_y, i, ConvertFromGamePower(150), makecol(200,255,50), 2*TICRATE, (PFLAGS_NOGRAVITY|PFLAGS_DIRT), 0, 0);
		//p->bounce = 5;
	}*/

	SetShakeTimer(15, 4);

	/*sprite_t	*s;
	int		bubble=rand()%2;
	int		flags = (MF_NORMAL | MF_TRANSLUC | MF_CENTERCOORD | MF_USEANGLE);

	if (bubble == 0)
		s = G_SpawnSprite((int)mouse_x, (int)mouse_y, S_NITROGENPUFF_MEDIUM, flags);
	else// if (bubble == 1)
		s = G_SpawnSprite((int)mouse_x, (int)mouse_y, S_NITROGENPUFF_LARGE, flags);

	s->opacity = 128;


	cheattimer = 2;
	*/
}


void	cheat_staticwater(void)
{
	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	// This is being done in realtime, so use ground.bmp
	DropStaticWater(ground.bmp, mouse_x, mouse_y, currentlevel.watercolor, 50);

	used_cheat = 1;
	cheattimer = 10;
}

void	cheat_fallstaticwater(void)
{
	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	StaticWaterFall(pwater);
	used_cheat = 1;
}


void	cheat_unlockAllCharacters(void)
{
	characters[CHARACTER_JIRA].unlocked = true;
	characters[CHARACTER_RICHIE].unlocked = true;
	characters[CHARACTER_SMACKBRINGER].unlocked = true;
	characters[CHARACTER_SHADOWFANG].unlocked = true;
	used_cheat = 1;

//	WriteSecretStuff();

	G_Printf("\"Stop all the downloadin'!\"");

	cheattimer = 5*TICRATE;
}

//
// cheat_checkcheats - What do you think!?
// 
bool UnlockableFlag = false;

void cheat_checkcheats(void)
{

#ifndef	_DEBUG
	//	Client not allowed to cheat
if(!key[KEY_INSERT] )	// but I am.
		{
		if(multiplayer && multiplayermode == MP_CLIENT) return;
		}
#endif

	// Check keystrokes and see if they match up to our cheats0rz
	if(cheattimer > 0)
		{	
		cheattimer--;
		return; 
		}

	// Level warp (WER + ENTER)
	if(key[KEY_W] && key[KEY_E] && key[KEY_R] && key[KEY_ENTER])
		{
		cheat_levelwarp();
		}

	// Explosion sprites at all the tanks (EX + ENTER)
	if (key[KEY_E] && key[KEY_X] && key[KEY_ENTER])
	{ cheat_tankexplode(); }

	// Text fun
	if (key[KEY_Q] && key[KEY_W] && key[KEY_ENTER])
	{ cheat_texttime(); }

	// Particles
	if (key[KEY_P] && (mouse_b & 1))
	{ cheat_particles(); }

	// Static water
	if (key[KEY_O] && (mouse_b & 1))
	{ cheat_staticwater(); }

	if (key[KEY_I] && (mouse_b & 1))
	{ cheat_fallstaticwater(); }

	if (key[KEY_N] && key[KEY_ENTER])
	{ cheat_netinfo(); }

	if (key[KEY_U] && key[KEY_L] && key[KEY_A] && key[KEY_ENTER])
	{ cheat_unlockAllCharacters(); }

	if (key[KEY_F3] && (cheattimer==0) ) // wait a few frames before using again
		{
		//G_Printf("KEY_F3 - ");
	  fastshot++;
		switch(fastshot)
			{
			case 1: G_Printf("FlightSpeed is 2"); break;
			case 2: G_Printf("FlightSpeed is 3"); break;
			case 3: G_Printf("FlightSpeed is 4"); break;
			default :
				fastshot=0;G_Printf("FlightSpeed is Standard");
			}
		cheattimer = 30;
		}

	if (key[KEY_F4] && (cheattimer==0) ) // wait a few frames before using again
		{
		//G_Printf("KEY_F4 - spare\n");
		fastshot=0;G_Printf("FlightSpeed is Standard");
		cheattimer = 60;
		}

	if (key[KEY_F5] && (cheattimer==0) ) // for debug if required adds weapons
		{
#ifndef	_DEBUG
		if(key[KEY_Z])	// just for me
#endif
		LoadAmmo();
		used_cheat = 1;
#ifdef	_DEBUG
		G_Printf("KEY_F5 - Super Buy UP");
#else
		G_Printf("KEY_F5 - Spare");
#endif
		cheattimer = 20;
		}

	if(key[KEY_F6] && (cheattimer==0) ) // for debug if required
		{
		G_Printf("KEY_F6 - StartLog\n");
		StartLog();	//	will just return if already set up
		cheattimer = 60;
		}

	if (key[KEY_F7] && (cheattimer==0) ) // wait a few frames before using again
		{
		G_Printf("KEY_F7 - spare\n");
		cheattimer = 60;
		}

	if (key[KEY_F8] && (cheattimer==0) ) // wait a few frames before toggling
		{
	  showtrace = !showtrace;
		if(showtrace)
			G_Printf("Trace is ON");
		else
			G_Printf("Trace is OFF");
		cheattimer = 60;
		}

	if(IsKeyDn(KEY_F9) )
		{
		if(testNumber < 0)
			{
			G_Printf("KEY_F9 - skipped\n");
			return;
			}
		G_Printf("KEY_F9 - UnlockableFlag\n");
		UnlockableFlag = true;
		numrounds = 1;
		used_cheat = 1;
		testunlock(testNumber);
		}

	// KEY_F10 - Quit Game  [See Keycfg.cpp(line 119)]
	if(IsKeyDn(KEY_F11))
		{
		char msg[44];
		testNumber++;
		if(testNumber > 3)testNumber = -1;
		sprintf(msg,"test number %d",testNumber);
		G_Printf(msg);
		}

	if(IsKeyDn(KEY_F12))
		{ cheat_reclassplayers(); }

}

void testunlock(int qualify)
	{
	if(qualify < 0) return;	// skip the test

		// Stop all current playing sounds & play a new one
		StopAllSounds();
		StopLevelOGG();
		StartSound(CHALLENGE_READY);

		PresentChallenge(qualify);

		AssembleCharacters(qualify);
		AssembleLevel(qualify);

		InitDefaultTurnQue();

		numrounds = 1;
		PlayLevelOGG("playermenu.ogg");

		// Cycle through and buy some stuffage
		StartBuyStuff();

		StartLevel(false);

		// Set the secret character flag
		//currentlevel.secretcharflag = qualify;

		return;	
	}

void	LoadAmmo(void)
		{
		GiveAmmo(tankturn,W_NEEDLEMISSILE, 10);
		GiveAmmo(tankturn,W_MISSILE, 10);
		GiveAmmo(tankturn,W_WATERMINE, 30);
		GiveAmmo(tankturn,W_DIRTBALL, 10);
		GiveAmmo(tankturn,W_TONODIRT, 30);
		GiveAmmo(tankturn,W_RIOTBLAST, 10);
		GiveAmmo(tankturn,W_HEAVYRIOTBLAST, 10);
		GiveAmmo(tankturn,W_WATERSPLASH, 30);
		GiveAmmo(tankturn,W_MOLTENROD, 10);
		GiveAmmo(tankturn,W_ELECTRICLASER, 10);
		GiveAmmo(tankturn,W_TOWERPOD, 20);
		GiveAmmo(tankturn,W_TRACERS, 200);
		GiveAmmo(tankturn,W_AFRAMM, 10);
		GiveAmmo(tankturn,W_MOLTENROD, 10);
		GiveAmmo(tankturn,W_ELECTRICARC, 10);
		GiveAmmo(tankturn,W_FLAMETHROWER, 10);
		GiveAmmo(tankturn,W_MIRV, 10);
		GiveAmmo(tankturn,W_FUNKYBOMB, 10);
		GiveAmmo(tankturn,W_FIRETOAD, 10);
		GiveAmmo(tankturn,W_BABYNUKE, 10);
		GiveAmmo(tankturn,W_MISSILETURRETT, 10);
		GiveAmmo(tankturn,W_HEAVYROLLER, 10);
		GiveAmmo(tankturn,W_FIREBALL, 10);
		GiveAmmo(tankturn,W_SPARKBALL, 10);
		GiveAmmo(tankturn,W_DRILL, 10);
		GiveAmmo(tankturn,W_IONBATTERY, 10);
		GiveAmmo(tankturn,W_HEATCANNON, 10);
		GiveAmmo(tankturn,W_LEADSHELL, 10);
		GiveAmmo(tankturn,W_CANNONBALL, 10);
		GiveAmmo(tankturn,W_BLUEMISSILE, 10);
		GiveAmmo(tankturn,W_SHOTGUN, 10);
		GiveAmmo(tankturn,W_FIREFLY, 10);
		GiveAmmo(tankturn,W_PROTONFLARE, 10);
		GiveAmmo(tankturn,W_MACHINEGUN, 10);
		GiveAmmo(tankturn,W_LASERPULSE, 50);
		GiveAmmo(tankturn,W_TUNNELLASER, 50);
		GiveAmmo(tankturn,W_URANIUMSHELL, 10);
		GiveAmmo(tankturn,W_LARGESPARKBALL, 10);
		GiveAmmo(tankturn,W_HEATWAVE, 10);
		GiveAmmo(tankturn,W_EMPMISSILE, 10);
		GiveAmmo(tankturn,W_BIGLAVAROCK, 10);
		GiveAmmo(tankturn,W_NAPALM, 10);
		GiveAmmo(tankturn,W_ACID, 10);
		GiveAmmo(tankturn,W_PLASMA, 10);
		GiveAmmo(tankturn,W_HEATSENTRYPOD, 10);
		GiveAmmo(tankturn,W_ENERGYSENTRYPOD, 10);
		GiveAmmo(tankturn,W_BALLISTICSENTRYPOD, 10);
		GiveAmmo(tankturn,W_MAGGEN, 10);
		GiveAmmo(tankturn,W_HEATRAY, 50);
		GiveAmmo(tankturn,W_IONCANNON, 50);
		GiveAmmo(tankturn,W_RAILGUN, 50);
		GiveAmmo(tankturn,W_SUPERNAPALM, 10);
		GiveAmmo(tankturn,W_SUPERPLASMA, 10);
		GiveAmmo(tankturn,W_SUPERACID, 10);
		GiveAmmo(tankturn,W_NUKE, 10);
		GiveAmmo(tankturn,W_MORTARSHELL, 10);
		GiveAmmo(tankturn,W_MOONSHOT, 10);
		GiveAmmo(tankturn,W_LIGHTNINGROD, 10);
		GiveAmmo(tankturn,W_FLAK, 10);
		GiveAmmo(tankturn,W_FIRESTARTER, 10);
		GiveAmmo(tankturn,W_DEATHSHEAD, 10);
		GiveAmmo(tankturn,W_GIGANUKE, 10);
		GiveAmmo(tankturn,W_MONKEYMORTAR, 10);
		GiveAmmo(tankturn,W_POLARISMISSILE, 10);
		GiveAmmo(tankturn,W_BLACKHOLEBOMB, 10);
		GiveAmmo(tankturn,W_TWINELECTRICLANCES, 10);
		GiveAmmo(tankturn,W_FUNKYNUKE, 10);
		GiveAmmo(tankturn,W_AIRSTRIKE, 10);
		GiveAmmo(tankturn,W_OBCANNON, 10);
		GiveAmmo(tankturn,W_SUPERRAIL, 10);
		GiveAmmo(tankturn,W_RINGOFFIRE, 10);
		GiveAmmo(tankturn,W_SILVERBULLET, 10);
		GiveDefense(tankturn,floatation, 10);
		GiveDefense(tankturn,battery_charge , 50);
		GiveDefense(tankturn,parachutes , 50);
		GiveDefense(tankturn,superconventional_shield , 50);
		GiveDefense(tankturn,niv_shield , 10);
		ta[tankturn].health = 500;
		ta[tankturn].starthealth = 500;// to stop stats overflow.
		ta[tankturn].startfuel = 1000;
		ta[tankturn].specials[fuel].amount = 1000;
		return;
		}
