#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"
#include "editor/editor.h"
#include "peffect.h"
#include "secrets.h"
#include "scripts.h"

level_t		currentlevel;

//
// Below- Code for looking up levels and loading one ramdonly, yes, its messy.
//

int		global_attr = FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_LABEL | FA_DIREC | FA_ARCH;
int		level_filecount;
char	**level_filenames;
void filecounter(const char *filename, int attrib, int param)
{
	level_filecount++;
}

void filearraystack(const char *filename, int attrib, int param)
{
	level_filenames[level_filecount] = (char *)malloc(128);
	strcpy(level_filenames[level_filecount], filename);
	level_filecount++;
}

//
// GenerateLevelList - Generates a dynamic level list in 'level_filenames'
// Stores the number of levels found in /levels under 'level_filecount'
//

void	GenerateLevelList(void)
{
	level_filecount=0;
	for_each_file("levels/*.cdl", global_attr, filecounter, 0);
	level_filenames = (char **)malloc(level_filecount*sizeof(char *));

	level_filecount=0;
	for_each_file("levels/*.cdl", global_attr, filearraystack, 0);
}

//
// FreeLevelList
//

void	FreeLevelList(void)
{
	int		i;
	for (i=0; i<level_filecount; i++)
	{
		free(level_filenames[i]);
	}

	free(level_filenames);
}


char *LoadRandomLevel(void)
{
	static	char	str[256];

	GenerateLevelList();

	// Make sure there are some files to load, if not we cannot continue
	if (level_filecount <= 0)
	{
		if(!demomode)
			alert("Error", "No levels found!", "Please ensure that Charred Dirt is properly installed.", "Ok", NULL, 0,0);
		StopLevelOGG();

		show_mouse(NULL);
		gamestate = GS_TITLE;
		GUI_SetupMenu (main_menu);
		return NULL;
	}

	ustrcpy(str, level_filenames[rand()%level_filecount]);

	// Free our memory array
	FreeLevelList();

	return str;
}

void	LoadLevel(char	*file_name) // file_name maybe NULL
{
	int		i;
	char	*filename;

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

	// Load the level if one is given, if not...randomize it
	//	if LoadRandomLevel() did not find any levels then return NULL
	if(!file_name)
		file_name = LoadRandomLevel();

	if(file_name != NULL)
		{
		if(file_name[4] == '*')	//	strip off "V1.0*"
			filename = &file_name[5];
		else
			filename = file_name;
		}
#ifdef	_DEBUG
//  Degauss debug
Printf("LoadLevel(%s) set to [%s]\n", file_name, filename);
#endif

	SetScriptLevel(filename);	//%%	override any existing name

  if(!loadgamelevel(filename))
		{
    // do something here
		// if the filename was sent by the server then this player doesn't have the level map to play.
		// Could just create solid bitmaps (ie all dirt) and continue or
		//  set as spectator (open to cheating by having only selected levels available).
		// Could try "loadgamelevel(LoadRandomLevel())" again in case one file was bad but
		//  if the level directory is the wrong version it won't help.
		// Next version need to include the option to dynamically download the level.
		//
		// So for now we'll just create something and the player just suffers!!
    CreateRandomLevel(filename);
		}
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
// ResetLevel - Reloads the level and resets all varibles but continue gameplay
//

void	ResetLevel(void)
{
	Printf(".....Reloading level......\n\n");

	if (exists(currentlevel.filename)==0)
	{
		Printf("Orginal level filename '%s' not found! Attempting to load a random level...\n", currentlevel.filename);
		LoadLevel(LoadRandomLevel());
	}
	else
	{
		LoadLevel(currentlevel.filename);
	}

	// Refresh
	ground.needsupdate = true;
}


void	StartNewGame(int nplayers)
{
	int		i;

	numplayers = 0;

	// Start the cool particle effect thingie
	Initialise_PEffect();

	// Setup start varibles used for tracking certain unlockable aspects of
	// the game
	start_cash = (int)cashstart->value;
	start_rounds = numrounds;

	// Reset cheat indicator
	used_cheat = 0;
	testNumber = -1;

	maxplayers = numplayers = nplayers;
	for (i=0; i<numplayers; i++)
	{
		ta[i].exist = 10;

		// Reset all tracking varibles
		memset(ta[i].weaponsused, 0, sizeof(ta[i].weaponsused));
		memset(ta[i].defenseused, 0, sizeof(ta[i].defenseused));
		memset(ta[i].specialsused, 0, sizeof(ta[i].specialsused));

		// Give em some money based on what is configured
		ta[i].GiveMoney((int)cashstart->value);

		// Set default health
		ta[i].starthealth = DEFAULT_TANK_HEALTH;

		// If this is a team game, assign team colors
		if (teamplay->value > 0)
		{
			// First half of players are on the red team, second half are on the blue
			// team by default
			if (i < (numplayers/2))
				ta[i].SetTeamColor(TANKTEAM_RED);
			else
				ta[i].SetTeamColor(TANKTEAM_BLUE);
		}

		// Run player setup
		if (multiplayer)
			PS_RunMultiplayerSetup(i, -1);
		else
			PS_RunSetup(i);

		if (ta[i].skill == TANKSKILL_SHOOTER ||
			ta[i].skill == TANKSKILL_SNIPER ||
			ta[i].skill == TANKSKILL_GUNNER)
			GiveAmmo(i, W_MINIMIRV, UNLIMITED_AMMO);
		else
			GiveAmmo(i, W_BABYMISSILE, UNLIMITED_AMMO);

		// Default netstate in case we're playing multiplayer
		ta[i].netstate = NETSTATE_DISCONNECTED;
	}

	// We're done with the particles (for now)
	Shutdown_PEffect();

	// Default turn que
	InitDefaultTurnQue();
}


//
// StartLevel
//

void	StartLevel(bool	resetShotClock)
{
	int		i;

	// Play the music track if we have one
	PlayLevelOGG(currentlevel.song);

	// Software will be drawing the mouse now
	show_mouse(NULL);

	// Dont show anybodys stats
	tank_showstats = -1;

	// There are no "after turn" sprites that are active
	currentlevel.afterturnsprite_active = 0;

	// Set new gamestate and shut down the menu
	menuactive = false;
	gamestate = GS_INGAME;

	// Break any messages
	TankMessage_Break();

	// Reset any "wait" flags
	for(i=0; i<MAX_TANKS; i++)
		{
		ta[i].pingwaitReady = true;
		ta[i].texttimer = -1;	// Degauss: Clear any talk text (can occur during warp)
		}
	checktanktimer = -10;
	// Refresh the screen
	ground.needsupdate = true;

	// No special effect...yet
	ground.specialtics = 0;

	///%%		script setup
	//	do it here so player can use autodefence if allocated
	DoScriptCmds();

	// Setup auto defense for those who have it
	if(!multiplayer)
		SetupAutoDefense();

	// Reset shot clock
	if(!multiplayer || multiplayermode == MP_SERVER)
		{
		if(resetShotClock)
			Tank_ResetShotClock((int)shotclocktime->value);
		}

#ifndef	DEDICATED_SERVER
	// Make the local player NETSTATE_INGAME
	ta[net_localplayer].netstate = NETSTATE_INGAME;
#endif

	// Default for all levels
	currentlevel.secretcharflag = -1;
}


//
// Cmd_ListLevels_f - Console command for listing all levels
//

void	Cmd_ListLevels_f(void)
{
	int		i;

	// Probe levels directory
	GenerateLevelList();

	Printf("\n   Available local levels\n");
	Printf("=========================\n");

	for (i=0; i<level_filecount; i++)
	{
		Printf("%s\n", get_filename(level_filenames[i]));
	}

	// Free memory
	FreeLevelList();

	Printf("\n");
}


//
// Cmd_LevelWarp_f - Warps to a specific level
//

void	Cmd_LevelWarp_f(void)
{
	char	filename[512];
	char	*p;

	if (Cmd_Argc () < 2)
	{
		Printf("      levelwarp <filename>\n\n");
		Printf("desc: Ends the current level and begins the specified level\n");
		return;
	}

	if (multiplayer && multiplayermode == MP_CLIENT)
	{
		Printf("Only the host can switch levels\n");
		return;
	}

	if (gamestate != GS_INGAME)
	{
		Printf("This command can only be used while the game is in progress\n");
		return;
	}

	p = Cmd_Args();

	// Remove quotation marks
	if (*p == '"')
	{
		p++;
		p[strlen(p)-1] = 0;
	}

	// Generate filename name
	sprintf(filename, "levels\\%s", p);

	Printf("Attempting: %s\n", filename);
	if (exists(filename)==0)
	{
		Printf("'%s' not found!\n\n", filename);
		return;
	}

	Inter_Run(filename);
	InitDefaultTurnQue();
}

//
// This just fills in the currentlevel structure if the selected level failed to load
//

void CreateRandomLevel(const char *filename)
{
// Ripped from newlevel_proc() -- fill the currentlevel structure

//
// default_foreground - creates a really crappy default foreground
//
	if (!ground.bmp)
		ground.bmp = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(ground.bmp, TRANSCOLOR);
	rectfill(ground.bmp, 0, (SCREENHEIGHT*0.98), SCREENWIDTH, SCREENHEIGHT, makecol(0,100,0));
  CONCRETE = 150;	// give it a colour other than black or green

	// Clear background to black
	if (!background)
		background = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(background, makecol(0,0,0));
	// add code to show the error message.
	{
	char msg[234];
	if(filename)
	 sprintf(msg,"Selected level [%s] did not load",filename);
	else // to get here LoadRandomLevel must have returned NULL
	 sprintf(msg,"Level not found or has errors");

	textout_centre(background, (FONT *)maindata[MS].dat, msg,
				SCREENWIDTH/2, 100, makecol(255,255,255));
	}

	// Setup the level desc flags
	sprintf(currentlevel.name, "A Really Crappy Level");	// Name of level
	sprintf(currentlevel.author, "CDirt");	// Author of this level
	sprintf(currentlevel.leveldesc, "The selected level failed to load.");// Short description of the level

	currentlevel.song[0] = 0;
	currentlevel.filename[0] = 0;

   // Set tank position data
	int gap = SCREENWIDTH/(MAX_TANKS+2); // 800/(8+2) = 80
	for (int i=0; i<MAX_TANKS; i++)
		{
		currentlevel.tankxpos[i] = gap*(i+1);
		currentlevel.tankypos[i] = SCREENHEIGHT/2;
		currentlevel.posoccupied[i] = false;
		}

	currentlevel.flags = 0;
	currentlevel.foreground_lightlevel = 255.0;
	currentlevel.background_lightlevel = 255.0;
	currentlevel.whiteflashtics = 0;

	// Make sure the water buffer is clear
	pwater->DeleteAll();
	currentlevel.staticwaterlevel = 0;
	currentlevel.watercolor = makecol(0, 50, 255);

	currentlevel.afterturnsprite_active = 0;
	currentlevel.numshotsfired = 0;

	// Set default wall & gravity options
	currentlevel.walltype     = WALLS_NORMAL;
	currentlevel.gravityflags = NOGRAVITY_NORMAL;

	// Secret stuff
	currentlevel.secretcharflag = 0;

	// Clear out all sprites
	G_DestroyAllSprites();

}
////////////////////////////// End-of-Levels.cpp /////////////////////////////
