#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"
#include "particles.h"
#include "secrets.h"

#define			SECRETLEVEL_JIRA			0
#define			SECRETLEVEL_RICHIE			1
#define			SECRETLEVEL_SMACKBRINGER	2
#define			SECRETLEVEL_SHADOWFANG		4

// Function preprocessors
bool			jira_instancetest(void);
bool			richie_instancetest(void);
bool			smackbringer_instancetest(void);
bool			shadowfang_instancetest(void);
int				d_intermission_joy(int msg, DIALOG * d, int c);
void			ChallengeDrawer(void);

// Varibles that are sure not to change during game play.
extern int		inter_winner;
int				start_cash;
int				start_rounds;
int				used_cheat;				// Has the player used a cheat to void any of these instances?
BITMAP			*challenge_buffer;
int				challenge_character;	// Reference to character that is being unlocked
										// (for other functions mainly)


secretlevel_t	secretlevels[NUM_SECRETLEVELS] = {

	// Jira's secret level
	{
		"Challenge Arena",								// Name of level
		"Jesse Davis",									// Author
		"",												// Music filename
		SL_FOREGROUND1,									// Foreground
		SL_BACKGROUND,									// Background
		{ 170, 612, 391 },								// Tank X positions
		{ 260, 260, 158 },								// Tank Y positions
	},

	// Richie's secret level
	{
		"Challenge Arena",								// Name of level
		"Jesse Davis",									// Author
		"",												// Music filename
		SL_FOREGROUND2,									// Foreground
		SL_BACKGROUND,									// Background
		{ 170, 612, 158 },								// Tank X positions
		{ 260, 260, 158 },								// Tank Y positions
	},

	// smackbringer's secret level
	{
		"Challenge Arena",								// Name of level
		"Jesse Davis",									// Author
		"",												// Music filename
		SL_FOREGROUND3,									// Foreground
		SL_BACKGROUND,									// Background
		{ 170, 612, 158 },								// Tank X positions
		{ 260, 260, 158 },								// Tank Y positions
	},

	// ShadowFang's secret level
	{
		"Challenge Arena",								// Name of level
		"Jesse Davis",									// Author
		"",												// Music filename
		SL_FOREGROUND4,									// Foreground
		SL_BACKGROUND,									// Background
		{ 170, 612, 158 },								// Tank X positions
		{ 260, 260, 158 },								// Tank Y positions
	},
};


//
// Instance functions. Tests if the player has achieved the necessary requirement
// in order to face a secret character
//

bool			jira_instancetest(void)
{
	// Unlocking Jira conditions
	//
	// Start cash:	50000
	// Rounds:		10
	//
	// Win against another sniper.

	int		i=0,v=0;
	int		human=-1;

	// Verify rounds and cash and player count
	if (start_cash != 50000 || start_rounds != 10 || numtanks() != 2)
		return false;

	// Verify two players in the game. One human, one sniper. Make sure the human
	// won.

	// Verify the human player
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist == 0)
			continue;

		if (ta[i].skill == TANKSKILL_HUMAN)
		{
			v++;
			human = i;
		}
	}

	// Only ONE human player
	if (v != 1)
		return false;

	// Human player must win the game
	if (inter_winner != human)
		return false;

	// Verify the sniper player
	v=0;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist == 0)
			continue;

		if (ta[i].skill == TANKSKILL_SNIPER)
			v++;
	}

	// Only ONE sniper AI player
	if (v != 1)
		return false;

	// Got all the way down here? Everything must be in line then
	return true;
}

bool			richie_instancetest(void)
{
	// Unlocking Richie conditions
	//
	// Start cash:	50000
	// Rounds:		10
	//
	// Win 10 rounds against 4 shooters using only AFRAMMS

	int		i=0,v=0;
	int		human=-1;

	// Verify rounds and cash and player count
	if (start_cash != 50000 || start_rounds != 10 || numtanks() != 5)
		return false;

	// Verify five players in the game. One human, four shooters. 
	// Make sure the human won.

	// Verify the human player
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist == 0)
			continue;

		if (ta[i].skill == TANKSKILL_HUMAN)
		{
			v++;
			human = i;
		}
	}

	// Only ONE human player
	if (v != 1)
		return false;

	// Human player must win the game
	if (inter_winner != human)
		return false;

	// Verify the shooter player
	v=0;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist == 0)
			continue;

		if (ta[i].skill == TANKSKILL_SHOOTER)
			v++;
	}

	// Verify 4 other shooters
	if (v != 4)
		return false;

	// Make sure the human player only used AFRAMMS. Any other
	// missile type will void this instance out.
	for (i=0; i<NUM_PROJECTILES; i++)
	{
		// Ingore counting AFRAMMs
		if (i==W_AFRAMM)
			continue;

		// Any other missile voids this out
		if (ta[human].weaponsused[i] > 0)
			return false;
	}

	// Got all the way down here? Everything must be in line then
	return true;
}


bool			smackbringer_instancetest(void)
{
	// Unlocking smackbringer conditions
	//
	// Start cash:	50000
	// Rounds:		10
	//
	// Win 10 rounds against 7 other shooters using no batteries or shields

	int		i=0,v=0;
	int		human=0;

	// Verify rounds and cash and player count
	if (start_cash != 50000 || start_rounds != 10 || numtanks() != 8)
		return false;

	// Verify players in the game.
	// Make sure the human won.

	// Verify the human player
	for (i=0; i<MAX_TANKS; i++)
		{
		if (ta[i].exist == 0)
			continue;

		if (ta[i].skill == TANKSKILL_HUMAN)
			{
			v++;
			human = i;
			}
		}

	// Only ONE human player
	if (v != 1)
		return false;

	// Human player must win the game
	if (inter_winner != human)
		return false;

	// Verify the shooter player
	v=0;
	for (i=0; i<MAX_TANKS; i++)
		{
		if (ta[i].exist == 0)
			continue;

		if (ta[i].skill == TANKSKILL_SHOOTER)
			v++;
		}

	// Verify 7 other shooters
	if (v != 7)
		return false;

	// Verify that the player has not used any batteries, shields or parachutes
	for (i=0; i<NUM_DEFENSE; i++)
		{
		// Ingore things the player can actually use...
		if (i==floatation)
			continue;

		// Any other missile voids this out
		if (ta[human].defenseused[i] > 0)
			return false;
		}

	if (human != inter_winner)
		return false;

	// Got all the way down here? Everything must be in line then
	return true;
}

bool			shadowfang_instancetest(void)
{
	// Unlocking smackbringer conditions
	//
	// Start cash:	50000
	// Rounds:		15
	//
	// Win in a team battle with 4v4snipers.
	// Player's team must win.

	int		i=0,v=0;
	int		human=0;

	// Verify rounds and cash and player count
	if (start_cash != 50000 || start_rounds != 15 || numtanks() != 8 || teamplay->value <= 0)
		return false;

	// Verify the human player
	for (i=0; i<MAX_TANKS; i++)
		{
		if (ta[i].exist == 0)
			continue;

		if (ta[i].skill == TANKSKILL_HUMAN)
			{
			v++;
			human = i;
			}
		}

	// Only ONE human player
	if (v != 1)
		return false;

	// Verify all the other sniper players
	v=0;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist == 0)
			continue;

		if (ta[i].skill == TANKSKILL_SNIPER)
			v++;
	}

	// Verify 7
	if (v != 7)
		return false;

	// Make sure the human's team won
	if (ta[human].team != Inter_FindTeamWinner())
		return false;

	// Got all the way down here? Everything must be in line then
	return true;
}


secretinstance_t	secretinstances[NUM_SECRETINSTANCES] = {

	{
		jira_instancetest,		// Instance function
		CHARACTER_JIRA,			// Secret character index ref
		JIRA_SECRET,			// Secret locked profile
		SECRETLEVEL_JIRA,		// Index ref to secret character's level
		50000,					// Starting cash for the secret character
		200,					// Starting HP
		false					// Unlocked?
	},


	{
		richie_instancetest,	// Instance function
		CHARACTER_RICHIE,		// Secret character index ref
		RICHIE_SECRET,			// Secret locked profile
		SECRETLEVEL_RICHIE,		// Index ref to secret character's level
		70000,					// Starting cash for the secret character
		300,					// Starting HP
		false					// Unlocked?
	},

	{
		smackbringer_instancetest,		// Instance function
		CHARACTER_SMACKBRINGER,			// Secret character index ref
		SMACKBRINGER_SECRET,			// Secret locked profile
		SECRETLEVEL_SMACKBRINGER,		// Index ref to secret character's level
		100000,							// Starting cash for the secret character
		400,							// Starting HP
		false							// Unlocked?
	},

	{
		shadowfang_instancetest,		// Instance function
		CHARACTER_SHADOWFANG,			// Secret character index ref
		SHADOWFANG_SECRET,				// Secret locked profile
		SECRETLEVEL_SHADOWFANG,			// Index ref to secret character's level
		200000,							// Starting cash for the secret character
		500,							// Starting HP
		false							// Unlocked?
	},

};

	


//
// CheckForUnlockable
//

int	CheckForUnlockable(void)
{
	int		i;
	int		qualify=-1;

	// If this flag is set, than dont even bother checking for instances
	if(used_cheat > 0)
		{
		Printf("Cheats used.\n");
		return -1;
		}
/// Degauss

	if(UnlockableFlag)
		qualify = 2;
	else

	for(i=0; i<NUM_SECRETINSTANCES; i++)
		{
		// Don't even attempt to evaluate this if the
		// instance condition does not even exist!
		if (!secretinstances[i].InstanceCondition)
			continue;

		// Unlocked already?
		if(characters[secretinstances[i].character].unlocked > 0)
			continue;

		// A secret instance has found to be true. 
		if(secretinstances[i].InstanceCondition() == true)
			{
			qualify = i;
			break;
			}
		}

	if(qualify >= 0)
		{

		UnlockableFlag = false; /// Degauss

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
		currentlevel.secretcharflag = qualify;

		return 1;	
		}
	else
		return -1;
}


//
// UnlockCharacter
//

void	UnlockCharacter(int character, bool showscreen)
{
	char	str[128];

	// Flag the character as being unlocked
	characters[character].unlocked = true;

	// Save the file
	WriteSecretStuff();

	// Show the congrats screen?
	if (showscreen == true)
	{
		// Stop all current playing sounds & play a new one
		StopAllSounds();
		StopLevelOGG();
		StartSound(CHALLENGE_WIN);

		sprintf(str, "You can now play as %s!", characters[character].name);
		alert("Congratulations!", str, "Use <ALT> at the character selection menu.", "Ok", NULL, 0, 0);

		// Omfg, stop them again!  =)
		StopAllSounds();

		mouseflags = 0;
	}
}

//
// AssembleCharacters - Sets up the challenger and removes all other computer players in the game
// except the challenger and the player
//

void	AssembleCharacters(int	instance)
{
	int		i=0;
	int		human=-1;
	int		player;

	// First of all, find the human player (should ONLY be one!)
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;
		
		// Flag this player as being the human
		if (ta[i].skill == TANKSKILL_HUMAN && human < 0)
			human = i;

		// Clear out all varibles that are relative
		ta[i].score = 0;
		ta[i].kills = 0;

		if (i != human)
		{
			ta[i].exist = 0;
			memset(ta[i].weapons, 0, sizeof(ta[i].weapons));
			memset(ta[i].defense, 0, sizeof(ta[i].defense));
			memset(ta[i].specials, 0, sizeof(ta[i].specials));
		}
	}

	// Now that everything is at it should be, add the "challenger" in
	//

	// First, find an open slot
	player = FindNewClient();

	if (player < 0)
	{
		Printf("No player slots left to add a bot\n");
		return;
	}

	ta[player].exist = 1;
	ta[player].needstodie = 0;

	// Reset the basics
	ta[player].kills = 0;
	ta[player].score = 0;
	ta[player].money = 0;

	// Reset all tracking varibles
	memset(ta[player].weaponsused, 0, sizeof(ta[player].weaponsused));
	memset(ta[player].defenseused, 0, sizeof(ta[player].defenseused));
	memset(ta[player].specialsused, 0, sizeof(ta[player].specialsused));

	// Give our new player a little something to live off of
	GiveAmmo(player, W_BABYMISSILE, UNLIMITED_AMMO);
	GiveAmmo(player, W_MISSILE, 10);
		
	// Give the new player a little coin
	ta[player].GiveMoney(secretinstances[instance].startcash);
		
	// Assign a random character
	ta[player].character = secretinstances[instance].character;

	ta[player].init(player, rand()%(SCREENWIDTH/2),0, 100,15,10,LIGHTGREEN);
	ta[player].skill = TANKSKILL_SNIPER;
		
	// This player will be brought back on the next round
	ta[player].starthealth = ta[player].health = secretinstances[instance].starthealth;

	// Assign the name
	ustrcpy(ta[player].name, characters[secretinstances[instance].character].name);
}

//
// AssembleLevel - Assembles a level into the currentlevel varibles based on the
// secret character level pre-set level stats
//

void	AssembleLevel(int	l)
{
	int		i;

	// Error checking, keep within the boundries
	if (l < 0 || l > NUM_SECRETLEVELS)
		return;

	// The ground is not falling
	ground.falling = false;

	// Since shots are linked with sprites, lets kill those as well
	G_DeleteAllSprites();

	// Clear out all the shots that are hanging in the
	// air.
	for (i=0;i<N_SHOTS;i++)
	{
		sh[i].exist = 0;
		sh[i].sprite = NULL;
	}

	// Free any tank memory
	DeAllocateTankMemory();

	// Clear out any lingering text messages on the screen
	DestroyAllGameText();

	// Init our particle engine
	InitParticles();

	// Assign title and author name
	strcpy(currentlevel.name, secretlevels[l].title);
	strcpy(currentlevel.author, secretlevels[l].author);

	// Assign the foreground and background bitmaps
	if (!ground.bmp)
		create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	blit((BITMAP *)maindata[secretlevels[l].foreground].dat, 
		ground.bmp, 0,0,0,0, SCREENWIDTH, SCREEN_H);
	blit((BITMAP *)maindata[secretlevels[l].background].dat, 
		background, 0,0,0,0, SCREENWIDTH, SCREEN_H);

	// Assign music filename
	strcpy(currentlevel.song, secretlevels[l].musicfile);

	// Assign the tank starting X and Y positions
	//

	// Clear both arrays first with unusable -1 values
	memset(currentlevel.tankxpos, -1, sizeof(currentlevel.tankxpos));
	memset(currentlevel.tankypos, -1, sizeof(currentlevel.tankypos));

	// Fill in positive values from the secret level arrays
	for (i=0; i<MAX_TANKS; i++)
	{
		if (secretlevels[l].startx[i] > 0 && secretlevels[l].starty[i] > 0)
		{
			currentlevel.tankxpos[i] = secretlevels[l].startx[i];
			currentlevel.tankypos[i] = secretlevels[l].starty[i];
			currentlevel.posoccupied[i] = false;
		}
		else
			currentlevel.posoccupied[i] = true;
	}

	// Set flags. Secret levels are always normal gravity with normal walls and 0 waterlevel
	currentlevel.gravityflags = NOGRAVITY_GROUND;
	currentlevel.walltype = WALLS_NORMAL;
	currentlevel.staticwaterlevel = 0;

	// Set default lightlevels
	currentlevel.background_lightlevel = 255;
	currentlevel.foreground_lightlevel = 255;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		// Setup some default varibles
		ta[i].init(i, 0,0,0,0,0,0);

		// Assign tank positions
		if (!multiplayer || multiplayermode == MP_SERVER)
		{
			ta[i].FindMapPosition();
	
			ta[i].DefaultAngle();
	
			ta[i].SetHEB(characters[ta[i].character].attrib_h, 
						characters[ta[i].character].attrib_e, 
						characters[ta[i].character].attrib_b);
		}
	}
}

//
// WriteSecretStuff - Write all the secret varibles to a file that will be 
// referenced later on for secret characters, easter eggs, etc.
//		>>>>>>  easter eggs  ???? where are they ????

int	WriteSecretStuff(void)
{
	PACKFILE	*f;
	int			i;

	f = pack_fopen(SECRET_FILENAME, F_WRITE);
	if (!f) 
		return *allegro_errno;

	// Store what characters are unlocked
	for (i=NUM_NORMALCHARACTERS; i<(NUM_NORMALCHARACTERS+NUM_SECRETCHARACTERS); i++)
		pack_iputw((int)characters[i].unlocked, f);

	// Close the file
	pack_fclose(f);

	return 1;
}

//
// ReadSecretStuff - Reads all secret information including secret character
// information and other stuff.
//

int	ReadSecretStuff(void)
{
	PACKFILE	*f;
	int			i;

	f = pack_fopen(SECRET_FILENAME, F_READ);
	if (!f) 
		return *allegro_errno;

	// Store what characters are unlocked
	for (i=NUM_NORMALCHARACTERS; i<(NUM_NORMALCHARACTERS+NUM_SECRETCHARACTERS); i++)
		characters[i].unlocked = (int)pack_igetw(f);

	// Close the file
	pack_fclose(f);

	return 1;
}

int	exit_challenge(void)
{ return D_EXIT; }

int		d_challenge_button(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		ChallengeDrawer();
		return D_O_K;
	}

	return d_agup_push_proc(msg, d, c);
}

DIALOG dialog_secretchallenge[] =
{
	{ d_intermission_joy,		0,       0,		0,	0,      0,	0,    0,	0,       0, 0,    NULL, NULL, NULL  },
    { d_challenge_button,		680,     549,  116,	47,   255,	0,    0,    D_EXIT,  0,			0,    (char *)"Next", 0, (void *)exit_challenge },
    { d_keyboard_proc,			0,       0,		0,	0,      0,	0,    0,	0,       KEY_ENTER, 0,    (void *)exit_challenge, NULL, NULL  },
	{ NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }
};


//
// ChallengeDrawer - Drawer for the challenge screen
//

void	ChallengeDrawer(void)
{

	if (update_count <= 0)
		return;

	update_count = 0;

	// Draw the main wallpaper (i guess you could call it that)
	draw_sprite(buffer, (BITMAP *)maindata[CHALLENGER].dat, 0,0);

	// Draw the secret portrait of the character to be unlocked
	draw_sprite(buffer, (BITMAP *)maindata[secretinstances[challenge_character].secretportait].dat,
		227, 221);

	draw_sprite(buffer, challenge_buffer, 0,0);

	ticker();
	gametics++;
	frame_count++;

	Screen_Blit(true);
}

//
// PresentChallenge
//

void	PresentChallenge(int character)
{
	// Set the global interget
	challenge_character = character;

	// Create some temp bitmaps
	challenge_buffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(challenge_buffer, TRANSCOLOR);

	do_agup_dialog(dialog_secretchallenge, challenge_buffer, ChallengeDrawer);

	// Free memory
	destroy_bitmap(challenge_buffer);
}
