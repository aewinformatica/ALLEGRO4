#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "dialogs.h"
#include "scorch.h"
#include "particles.h"
#include "editor/editor.h"
#include "secrets.h"
#include "fblend.h"


menu_t	*activemenu;
void		helptext(void);
void		testsetup(void);

int			currentmenu=0;
bool		menuactive;
bool		barreloutline=false;
bool		screenshotBMP=false;
int 		screenshotJPG=2; // medium quality

void	quitgame(void)
{
	// Above all else
	SaveConfig();

	ShutdownNetwork();

	if(maindata  ) unload_datafile(maindata  );
	if(sounddata ) unload_datafile(sounddata );
	if(alphadata ) unload_datafile(alphadata );
	if(spritedata) unload_datafile(spritedata);

	freetanks();

	FreeGameData();

	// Unload particle system from memory
	ShutdownParticles();

	ShutdownMusic();

	ShutDown_Console();

	Shutdown_Dialog_Stuff();
	allegro_exit();

	// Terminate the program
	exit(1);
}

void	newgame(void)
{
	int		ret;

	clear_keybuf();

	// Make sure we're not in multiplayer mode
	multiplayer = 0;

	// Set dialog colors
	set_dialog_color(dialog_newgame, gui_fg_color, gui_bg_color);

	// We cannot have any more than 4 players per team
	// Fix that here.
	if (teamplay->value > 0)
	{
		if(num_players->value > MAX_TANKTEAMS || num_players->value <= 1)
			num_players->value = MAX_TANKTEAMS;	// default to two players
	}

	// Set the dialog boxes based on cvars
	numround_setting = (int)num_rounds->value;
	numplayer_setting = (int)num_players->value;
	ustrcpy(servername, server_name->string);

	// Set the chars to reflect the info grabbed from the cvars
	sprintf(numplayer_string, "%d",(int)num_players->value);
	sprintf(numround_string, "%d",(int)num_rounds->value);

	// Fill in the teamplay/friendly fire box if applicable
	if (Cvar_VariableValue("teamplay") > 0)
	{
		dialog_newgame[10].flags |= D_SELECTED;

		// Default. Makes it visable
		dialog_newgame[11].flags = 0;

		// Friendly fire set?
		if (Cvar_VariableValue("friendlyfire") > 0)
			dialog_newgame[11].flags |= D_SELECTED;

		dialog_newgame[12].flags = D_HIDDEN;
		dialog_newgame[13].flags = 0;

	}
	else
	{
		dialog_newgame[10].flags =  0;
		dialog_newgame[11].flags = D_HIDDEN;

		dialog_newgame[12].flags = 0;
		dialog_newgame[13].flags = D_HIDDEN;
	}

	ret = do_agup_dialog(dialog_newgame, screen, NULL);

	// Record teamplay and friendlyfire back into the cvars
	if (dialog_newgame[10].flags & D_SELECTED)
	{
		Cvar_SetValue("teamplay", 1);
		numplayer_setting = numplayer_setting * 2;
	}
	else
		Cvar_SetValue("teamplay", 0);

	if (dialog_newgame[11].flags & D_SELECTED)
		Cvar_SetValue("friendlyfire", 1);
	else
		Cvar_SetValue("friendlyfire", 0);

	// Reset the cvars based on anything that was changed
    Cvar_SetValue("num_rounds", numround_setting);
	Cvar_SetValue("num_players", numplayer_setting);
	Cvar_Set("server_name", servername);

	if (ret == 8)
	{
		// Set the number of rounds
		numrounds = numround_setting;

		// Reset all tank data
		ResetTanks();

		// Play it again, Sam.
		PlayLevelOGG("playermenu.ogg");

		// Setup tank varibles
		StartNewGame(numplayer_setting);

		// Load the actual level into memory
		LoadLevel(NULL);	// random level

		// Cycle through buystuff screens
		StartBuyStuff();

		// Enter gamestate
		StartLevel(true);
	}
}


void	createmultiplayergame(void)
{
	int		ret;

	clear_keybuf();

	// Make sure we're not in multiplayer mode
	multiplayer = 0;

	// Set dialog colors
	set_dialog_color(dialog_newgame, gui_fg_color, gui_bg_color);

	// We cannot have any more than 4 players per team
	// Fix that here.
	if (teamplay->value > 0)
		{
		if(num_players->value > MAX_TANKTEAMS || num_players->value <= 1)
			num_players->value = MAX_TANKTEAMS;	// default to two players
		}

	// Set the dialog boxes based on cvars
	numround_setting = (int)num_rounds->value;
	numplayer_setting = (int)num_players->value;
	ustrcpy(servername, server_name->string);

	// Set the chars to reflect the info grabbed from the cvars
	sprintf(numplayer_string, "%d",(int)num_players->value);
	sprintf(numround_string, "%d",(int)num_rounds->value);

	// Fill in the teamplay/friendly fire box if applicable
	if (Cvar_VariableValue("teamplay") > 0)
		{
		dialog_newgame[10].flags |= D_SELECTED;

		// Default. Makes it visable
		dialog_newgame[11].flags = 0;

		// Friendly fire set?
		if (Cvar_VariableValue("friendlyfire") > 0)
			dialog_newgame[11].flags |= D_SELECTED;

		dialog_newgame[12].flags = D_HIDDEN;
		dialog_newgame[13].flags = 0;

		}
	else
		{
		dialog_newgame[10].flags =  0;
		dialog_newgame[11].flags = D_HIDDEN;

		dialog_newgame[12].flags = 0;
		dialog_newgame[13].flags = D_HIDDEN;
		}

	ret = do_agup_dialog(dialog_newgame, screen, NULL);

	// Record teamplay and friendlyfire back into the cvars
	if (dialog_newgame[10].flags & D_SELECTED)
		{
		Cvar_SetValue("teamplay", 1);
		numplayer_setting = numplayer_setting * 2;
		}
	else
		Cvar_SetValue("teamplay", 0);

	if (dialog_newgame[11].flags & D_SELECTED)
		Cvar_SetValue("friendlyfire", 1);
	else
		Cvar_SetValue("friendlyfire", 0);

	// Reset the cvars based on anything that was changed
    Cvar_SetValue("num_rounds", numround_setting);
	Cvar_SetValue("num_players", numplayer_setting);
	Cvar_Set("server_name", servername);


	if (ret == 8)	// clicked OK
		{
		// Reset all tank data
		ResetTanks();

		// Game master will always be in the very first slot
		net_localplayer = 0;

		// Set multiplayer mode
		multiplayer = 1;
		multiplayermode = MP_SERVER;

		// Reset the socket so it's listening for connections
		ResetSocket(true);

		PlayLevelOGG("playermenu.ogg");

		// Setup tank varibles
		StartNewGame(numplayer_setting);

		// Load the actual level into memory
		LoadLevel(NULL);	// random level

		// Cycle through buystuff screens
		StartBuyStuff();

		// Enter gamestate
		StartLevel(true);
		}
	else if (ret == 9)	// clicked CANCEL
		{
		// No multiplayer? Phooey.
		multiplayer = 0;
		}

	// Set the number of rounds
	numrounds = numround_setting;
}

void	soundoptions(void)
	{
	dialog_soundoptions[1].d2 = Cvar_VariableValue("soundvolume");
	dialog_soundoptions[2].d2 = Cvar_VariableValue("musicvolume");

	do_agup_dialog(dialog_soundoptions, screen, NULL);

	// Set FMOD's volume
	SetMusicVolume();

	// Save any chances
	SaveConfig();
	}

void	videooptions(void)
	{
	if (Cvar_VariableValue("fullscreen") > 0)
		dialog_videooptions[1].flags |= D_SELECTED;
	else
		dialog_videooptions[1].flags =  0;

	if(barreloutline)
		dialog_videooptions[2].flags |= D_SELECTED;
	else
		dialog_videooptions[2].flags =  0;

	if(Cvar_VariableValue("novideoanimation") > 0)
		dialog_videooptions[3].flags |= D_SELECTED;
	else
		dialog_videooptions[3].flags =  0;

	if(screenshotBMP)
		dialog_videooptions[4].flags |= D_SELECTED;
	else
		dialog_videooptions[4].flags =  0;

	if(screenshotJPG == 0)
		dialog_videooptions[6].flags |= D_SELECTED;
	else if(screenshotJPG == 1)
		dialog_videooptions[7].flags |= D_SELECTED;
	else if(screenshotJPG == 2)
		dialog_videooptions[8].flags |= D_SELECTED;
	else if(screenshotJPG == 3)
		dialog_videooptions[9].flags |= D_SELECTED;

	if(Cvar_VariableValue("hideHUD") > 0)
		dialog_videooptions[11].flags |= D_SELECTED;
	else
		dialog_videooptions[11].flags =  0;

	// Run the dialog
	do_agup_dialog(dialog_videooptions, screen, NULL);

	if (dialog_videooptions[1].flags & D_SELECTED)
		Cvar_SetValue("fullscreen", 1);
	else
		Cvar_SetValue("fullscreen", 0);

	barreloutline = (dialog_videooptions[2].flags & D_SELECTED);

	if(dialog_videooptions[3].flags & D_SELECTED)
		Cvar_SetValue("novideoanimation", 1);
	else
		Cvar_SetValue("novideoanimation", 0);

	screenshotBMP = (dialog_videooptions[4].flags & D_SELECTED);

	if(dialog_videooptions[7].flags & D_SELECTED)screenshotJPG = 1;
	else if(dialog_videooptions[8].flags & D_SELECTED)screenshotJPG = 2;
	else if(dialog_videooptions[9].flags & D_SELECTED)screenshotJPG = 3;
	else screenshotJPG = 0;

	if(dialog_videooptions[11].flags & D_SELECTED)
		Cvar_SetValue("hideHUD", 1); 
	else
		Cvar_SetValue("hideHUD", 0); 


	// Save any changes
	SaveConfig();
	}

void	cashoptions(void)
{
	// Fill the varible in
	sprintf(cashstr, "$ %d", (int)cashstart->value);

	do_agup_dialog(dialog_cashoptions, screen, NULL);

	// Save any chances
	SaveConfig();
}

void	mapoptions(void)
{
	int		i;

	int		gravsetflags = (int)Cvar_VariableValue("gravityflags");
	int		wallsettype = (int)Cvar_VariableValue("walltype");

	// Check the box if we're already using map settings
	if (gravsetflags <= 0 ||
		wallsettype <= 0)
	{
		dialog_mapsettings[1].flags = D_SELECTED;

		// Disable the rest of the controls
		for (i=4; i<11; i++)
		{ dialog_mapsettings[i].flags = D_DISABLED; }
	}
	else
	{
		// Fill in the rest
		if (wallsettype == WALLS_NORMAL)
			dialog_mapsettings[4].flags = D_SELECTED;
		else if (wallsettype == WALLS_RUBBER)
			dialog_mapsettings[5].flags = D_SELECTED;
		else if (wallsettype == WALLS_CEMENT)
			dialog_mapsettings[6].flags = D_SELECTED;
		else if (wallsettype == WALLS_WRAP)
			dialog_mapsettings[7].flags = D_SELECTED;

		// Set menu gravity options
		if (gravsetflags & NOGRAVITY_GROUND)
			dialog_mapsettings[8].flags = D_SELECTED;
		if (gravsetflags & NOGRAVITY_TANKS)
			dialog_mapsettings[9].flags = D_SELECTED;
		if (gravsetflags & NOGRAVITY_WATER)
			dialog_mapsettings[10].flags = D_SELECTED;
	}

	// Nil our two varibles out
	gravsetflags = wallsettype = 0;

	// Run teh dialog OMFG LOLOLOLOLOL!11
	do_agup_dialog(dialog_mapsettings, screen, NULL);

	// Default settings? Easy enough & save.
	if (dialog_mapsettings[1].flags & D_SELECTED)
	{
		Cvar_SetValue("walltype", 0);
		Cvar_SetValue("gravityflags", 0);

		SaveConfig();
		return;
	}
	else
	{
		// Plug the rest into the maincfg varible and save
		if (dialog_mapsettings[4].flags & D_SELECTED)
			wallsettype = WALLS_NORMAL;
		else if (dialog_mapsettings[5].flags & D_SELECTED)
			wallsettype = WALLS_RUBBER;
		else if (dialog_mapsettings[6].flags & D_SELECTED)
			wallsettype = WALLS_CEMENT;
		else if (dialog_mapsettings[7].flags & D_SELECTED)
			wallsettype = WALLS_WRAP;

		// Set the new gravity options
		if (dialog_mapsettings[8].flags & D_SELECTED)
			gravsetflags |= NOGRAVITY_GROUND;
		if (dialog_mapsettings[9].flags & D_SELECTED)
			gravsetflags |= NOGRAVITY_TANKS;
		if (dialog_mapsettings[10].flags & D_SELECTED)
			gravsetflags |= NOGRAVITY_WATER;

		Cvar_SetValue("walltype", wallsettype);
		Cvar_SetValue("gravityflags", gravsetflags);
	}

	// Save the config
	SaveConfig();
}

void	masskill(void)
{
	int		i;

	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	if (alert("Mass Kill", "Are you sure?", NULL, "Yes", "No", 0,0)==1)
	{
		// KILL THEM ALL!
		for (i=0; i<MAX_TANKS; i++)
		{
			if (ta[i].health > 0)
			{
				ta[i].health = 0;
				ta[i].needstodie = 1;
				ta[i].lasthit = i;
			}
		}

		// Flag this as cheating
		used_cheat = 1;

		// No more menu
		menuactive = false;

		// Start killing them off
		KillTanks();
	}
}

void	gameoptions(void)
{
	GUI_SetupMenu(game_menu);

	// "Map Settings" and "Starting Cash" cannot be accessed once
	// the game has begun.
	if (gamestate == GS_INGAME)
	{
		game_buttons[0].flags = BUTTON_DISABLED;
		game_buttons[1].flags = BUTTON_DISABLED;
		game_buttons[2].flags = BUTTON_NORMAL;
	}
	else
	{
		game_buttons[0].flags = BUTTON_NORMAL;
		game_buttons[1].flags = BUTTON_NORMAL;
		game_buttons[2].flags = BUTTON_DISABLED;
	}
}

void	tanktalkoptions(void)
{

	if (talkingtanks->value > 0)
		dialog_talkingtanks[1].flags |=  D_SELECTED;
	else
		dialog_talkingtanks[1].flags =  0;

	// Run the dialog
	do_agup_dialog(dialog_talkingtanks, screen, NULL);

	if (dialog_talkingtanks[1].flags & D_SELECTED)
		Cvar_SetValue("talkingtanks", 1);
	else
		Cvar_SetValue("talkingtanks", 0);

	// Save any chances
	SaveConfig();
}

void	mainmenu(void)
{
	if (multiplayer)
	{
		if (teamplay->value > 0)
			GUI_SetupMenu(mpteam_main_menu);
		else
			GUI_SetupMenu(mp_main_menu);
	}
	else
	{
		if (gamestate == GS_INGAME)
			GUI_SetupMenu(sp_main_menu);
		else
			GUI_SetupMenu(main_menu);
	}
}

void	optionsmenu(void)
{ GUI_SetupMenu(options_menu); }

void	multiplayermenu(void)
{ GUI_SetupMenu(multiplayer_menu); }

void	disconnect(void)
{
	CL_Disconnect();

	// Stop any playing music
	StopLevelOGG();

	// Return to the main menu
	gamestate = GS_TITLE;

	GUI_SetupMenu (main_menu);
	PlayLevelOGG("title.mp3");
}

void	end_game(void)	// for dialogs
	{endgame(true);}

void	endgame(bool ShowAlert)
{
	if(ShowAlert)
		if(alert("Alert", "This will end the current game.", "Continue Anyway?", "Yes", "No", 0, 0)==2)
			return;

	// Stop any playing music
	StopLevelOGG();

	// Close the socket
	closesocket(netsocket->sock);

  // No longer in multiplayer mode...set the appropiate varibles
	multiplayermode = -1;
	multiplayer = 0;
	joinstate = JOIN_NOTCONNECTED;

	// Return to the main menu
	gamestate = GS_TITLE;
	GUI_SetupMenu (main_menu);
	PlayLevelOGG("title.mp3");

	// Reset tank varibles
	inittanks();

}

button_t	main_buttons[NUM_MAIN_BUTTONS] =
{
	{ 230,	210,	newgame,		    "Single player",	BUTTON_NORMAL },
	{ 230,	250,	multiplayermenu,"Multi player",		BUTTON_NORMAL },
	{ 230,	290,	optionsmenu,	  "Options",				BUTTON_NORMAL },
	{ 630,	210,	helptext,	      "Help",						BUTTON_NORMAL },
#ifdef	_DEBUG
	{ 230,	290,	testsetup,	    "Test",						BUTTON_NORMAL },
#else
	{ 230,	290,	optionsmenu,	  "",								BUTTON_NORMAL },
#endif
	{ 280,	370,	quitgame,		    "Quit",						BUTTON_NORMAL },
};

button_t	sp_main_buttons[NUM_SPMAIN_BUTTONS] =
{
	{ 230,	210,	end_game,		"End Game",		BUTTON_NORMAL },
	{ 230,	290,	optionsmenu,	"Options",		BUTTON_NORMAL },
	{ 230,	330,	quitgame,		"Quit",			BUTTON_NORMAL },
};

button_t	mp_main_buttons[NUM_MPMAIN_BUTTONS] =
{
	{ 230,	210,	disconnect,		"Disconnect",	BUTTON_NORMAL },
	{ 230,	290,	optionsmenu,	"Options",		BUTTON_NORMAL },
	{ 230,	330,	quitgame,		"Quit",			BUTTON_NORMAL },
};

button_t	mpteam_main_buttons[NUM_MPTEAM_BUTTONS] =
{
	{ 230,	210,	disconnect,		"Disconnect",	BUTTON_NORMAL },
	{ 230,	210,	switchteam,		"Switch Teams",	BUTTON_NORMAL },
	{ 230,	290,	optionsmenu,	"Options",		BUTTON_NORMAL },
	{ 230,	330,	quitgame,		"Quit",			BUTTON_NORMAL },
};

button_t	options_buttons[NUM_OPTIONS_BUTTONS] =
{
	{ 230,	210,	gameoptions,		"Game",		BUTTON_NORMAL },
	{ 230,	250,	soundoptions,		"Sound",	BUTTON_NORMAL },
	{ 230,	290,	videooptions,		"Video",	BUTTON_NORMAL },
	{ 230,	330,	mainmenu,			"Back",		BUTTON_NORMAL },
};

button_t	game_buttons[NUM_GAME_BUTTONS] =
{
	{ 230,	180,	cashoptions,		"Starting Cash",		BUTTON_NORMAL },
	{ 230,	220,	mapoptions,			"Map Settings",			BUTTON_NORMAL },
	{ 230,	260,	masskill,			"Mass Kill",			BUTTON_NORMAL },
	{ 230,	300,	tanktalkoptions,	"Talking Tanks",		BUTTON_NORMAL },
	{ 230,	340,	optionsmenu,		"Back",					BUTTON_NORMAL },
};

button_t	multiplayer_buttons[NUM_MULTIPLAYER_BUTTONS] =
{
	{ 230,	210,	joingame,				"Join Internet Game",	BUTTON_NORMAL },
	{ 230,	210,	joinLANgame,			"Join LAN Game",		BUTTON_NORMAL },
	{ 230,	250,	createmultiplayergame,	"Create Game",			BUTTON_NORMAL },
	{ 230,	290,	mainmenu,				"Back",					BUTTON_NORMAL },
};


menu_t		main_menu[] = {
	"Main",					// Title
	main_buttons,			// Button varible
	NUM_MAIN_BUTTONS,		// Number of buttons
	NULL,					// In process
	NULL,					// Out process
};

menu_t		sp_main_menu[] = {
	"Single Player Main",		// Title
	sp_main_buttons,		// Button varible
	NUM_SPMAIN_BUTTONS,		// Number of buttons
	NULL,					// In process
	NULL,					// Out process
};

menu_t		mp_main_menu[] = {
	"Multiplayer Main",		// Title
	mp_main_buttons,		// Button varible
	NUM_MPMAIN_BUTTONS,		// Number of buttons
	NULL,					// In process
	NULL,					// Out process
};

menu_t		mpteam_main_menu[] = {
	"Multiplayer Main",
	mpteam_main_buttons,
	NUM_MPTEAM_BUTTONS,
	NULL,
	NULL,
};

menu_t		options_menu[] = {
	"Options",				// Title
	options_buttons,		// Button varible
	NUM_OPTIONS_BUTTONS,	// Number of buttons
	NULL,					// In process
	NULL,					// Out process
};

menu_t		game_menu[] = {
	"Game",					// Title
	game_buttons,			// Button varible
	NUM_GAME_BUTTONS,		// Number of buttons
	NULL,					// In process
	NULL,					// Out process
};

menu_t		multiplayer_menu[] = {
	"Multiplayer",			// Title
	multiplayer_buttons,	// Button varible
	NUM_MULTIPLAYER_BUTTONS,// Number of buttons
	NULL,					// In process
	NULL,					// Out process
};


//
// Title_Drawer
//

void	Title_Drawer(BITMAP *b)
{
	if (mrbmmg_mode > 0)
		draw_sprite(b, (BITMAP *)maindata[TITLESCREEN_BMMG].dat, 0,0);
	else
		draw_sprite(b, (BITMAP *)maindata[TITLESCREEN].dat, 0,0);
}


// Title_Input
void	Title_Input(void)
{
	if (IsKeyDn(KEY_ESC))
	{
		GUI_SetupMenu(main_menu);
	}
}



//
// GUI_MouseInside - Simple enough. Checks to see if the mouse is inside
// a set of points
//

bool GUI_MouseInside(int x, int y, int w, int h)
{
	if (mouse_x > x &&
		mouse_x < w &&
		mouse_y > y &&
		mouse_y < h)
	{ return true; }

	return false;
}

//
// GUI_InitButtons - Draw the font onto the buttons and stores the
//					 BITMAP in the array
//

void GUI_InitButtons(void)
{
}

//
// GUI_DrawButtons - Just draws the buttons according to what menu
//					 is set.
//

void GUI_DrawButtons(BITMAP *b)
{
	int		i;
	int		bh = text_height((FONT *)maindata[MS].dat);
	int		buttonHeight = activemenu->numbuttons * (bh+10);

	int		boxx = 0;
	int		boxy = ((SCREENHEIGHT/2)-(buttonHeight/2));

	int		buttonx;
	int		buttony;
	char	buf[128];
  int		ll;

	// Add dirty rectangle
	AddRectangle(b, 0, boxy, SCREENWIDTH, buttonHeight);

	// First, draw a background
	rectfill(b, 0, boxy, SCREENWIDTH, boxy + buttonHeight, makecol(64,64,64));

	// Draw some white lines
	line (b, 0, boxy+5, SCREENWIDTH, boxy+5, makecol(255,255,255));
	line (b, 0, (boxy+buttonHeight)-5, SCREENWIDTH, (boxy+buttonHeight)-5, makecol(255,255,255));

	boxy += bh/2;


	for(i=0; i<activemenu->numbuttons; i++)
		{
		// Figure out where the button is going to be blitted
		ll = text_length( (FONT *)maindata[MS].dat, activemenu->buttons[i].text );
		buttonx = (SCREENWIDTH/2)-(ll/2);
		buttony = boxy;

		// The "pressed down" button state
		if(activemenu->buttons[i].active)
			{
			//draw_sprite(b, buttons[i].downbmp, (int)(buttons[i].x), (int)(buttons[i].y));
			rectfill(b, buttonx, boxy, buttonx+ll, boxy + bh, makecol(64,11,64)); // Degauss
			textout_centre(b,
				(FONT *)maindata[MS].dat, activemenu->buttons[i].text,
                SCREENWIDTH/2,
				boxy,
				makecol(255,255,0));
			}
		else
			{
			if(activemenu->buttons[i].flags & BUTTON_DISABLED)
				{
				textout_centre(b,
				(FONT *)maindata[MS].dat, activemenu->buttons[i].text,
                SCREENWIDTH/2,
				boxy,
				makecol(0,0,0));
				}
			else
				{
				textout_centre(b,
					(FONT *)maindata[MS].dat, activemenu->buttons[i].text,
					SCREENWIDTH/2,
					boxy,
					makecol(255,255,255));
				}
			}

		boxy += bh+5;
		}

	sprintf(buf, "%s - " __DATE__ "", VERSION);

	// Shadow effect before the real text
	textout_centre(b, (FONT *)maindata[MS].dat, buf,
				(SCREEN_W/2)+1,
				(SCREEN_H-25)+1, 0);

	textout_centre(b, (FONT *)maindata[MS].dat, buf,
				SCREEN_W/2,
				SCREEN_H-25,
				makecol(255,255,255));
}


//
// GUI_Drawer - Draws stuff, depending on what menu we're running
//

void GUI_Drawer(BITMAP *b)
{
	GUI_DrawButtons(b);

	// Draw the mouse
	if (gamestate == GS_MENU)
		masked_blit(mouse_sprite, b, 0,0, mouse_x, mouse_y, mouse_sprite->w, mouse_sprite->h);
	else
		Blit_Mouse(b);
}


//
// GUI_GetInput - Handle the mouse and keyboard
//

void GUI_GetInput(void)
{
	int		i;
	int		bh = text_height((FONT *)maindata[MS].dat);
	int		buttonHeight = activemenu->numbuttons * (bh+10);

	int		boxx = 0;
	int		boxy = ((SCREENHEIGHT/2)-(buttonHeight/2));

	int		buttonx;
	int		buttony;
	int		ll;

	boxy += bh/2;

	// First, handle key input
	if(IsKeyDn(KEY_ESC))
		{
		// If we're in the middle of the game, refresh the screen
		if (gamestate == GS_INGAME)
			ground.needsupdate = true;

		menuactive = false;

		// Clear the keyboard buffer so the menu dosen't pop up again
		// later on in the game tic
		ClearAllKeys();

		return;
		}

	if(activemenu == main_menu)
		{
		if (key[KEY_ALT])
			{ ustrcpy(main_buttons[1].text, "Editor"); }
		else
			{ ustrcpy(main_buttons[1].text, "Multi player"); }
		}


	for (i=0; i<activemenu->numbuttons; i++)
		{
		// Figure out where the button is going to be blitted
		ll = text_length( (FONT *)maindata[MS].dat, activemenu->buttons[i].text );
		buttonx = (SCREENWIDTH/2)-(text_length((FONT *)maindata[MS].dat, activemenu->buttons[i].text)/2);
		buttony = boxy;

		if (GUI_MouseInside(buttonx,
				buttony,
				buttonx+text_length((FONT *)maindata[MS].dat, activemenu->buttons[i].text),
				buttony+bh) &&
				(!(activemenu->buttons[i].flags & BUTTON_DISABLED)))
			{
			//if (mouseflags & MOUSE_FLAG_LEFT_DOWN)
			if (activemenu->buttons[i].active == false)
				{
				activemenu->buttons[i].active = true;
				StartSound(MOUSE_OVER);
				}

			if (mouseflags & MOUSE_FLAG_LEFT_UP)
				{
				mouseflags = 0;
				activemenu->buttons[i].active = false;
				if (activemenu->buttons[i].proc)
					{
					StartSound(MOUSE_SELECT);

					// Again, check hack to get into the editor
					if (activemenu == main_menu &&
						key[KEY_ALT] && i == 1)
						{
						CDEdit();
						}
					else
						activemenu->buttons[i].proc();
					}
				}
			return;
			}
		else
			{ activemenu->buttons[i].active = false; }
		boxy += bh+5;
	}
}

//
// GUI_SetupMenu
//

void GUI_SetupMenu (menu_t	*menu)
{
	//currentmenu = menu;
	activemenu = menu;

	menuactive = true;
}


//
// GUI_Thinker - The guts of our GUI
//

void GUI_Thinker(void)
{
	GUI_GetInput();

	GUI_Drawer(buffer);
}



void	helptext(void)
{ 
	BITMAP *b;
	int		bh = text_height((FONT *)maindata[MS].dat);
	int		nxt = bh+0;
	int		pageHeight = 25*nxt;
	int		i;
	int		boxx = 8;
	int		boxy = ((SCREENHEIGHT/2)-(pageHeight/2));
	char  *txt[] = 
		{
		"* Up/Down Arrow - Increases/decreases power",
		"* Page Up/Down - Powers up/down quickly",
		"* Right/Left Arrow - Changes angle",
		"* Spacebar - Fires weapon",
		"* T key - Brings up the inventory menu in game",
		"* A/S key - Moves tank left or right",
		"* Tab key - Cycles through weapons",
		"* Left Alt key - Shows health and shields of all tanks",
		"* Clicking on a tank shows HEB stats",
		"* W+E+R+<Enter> keys - Allows you to end the level and switch to another level",
		"* Esc key - Exits out of menus",
		"* F1 key - Takes screenshots",
		"* F2 key - Toggle in-game scoreboard/frame rate",
		"* F3 key - Cycle Flight speed",
		"* F6 key - Start Logging (Debug)",
		"* F8 key - Toggle Missile Trace",
		"* F10 key - Completely exits out of game",
		"* F12 key - Player re-class setup (single player only)",
		"* ` key - Brings up the console display",
		"* P key - Pause",
		" ",
		"Out of Game Controls:",
		"* Left Alt key - Toggles the Editor menu item to replace the Multiplayer menu item",
		"."
		};

	
	ClearAllKeys();
	b = buffer;
		
	while(! IsKeyDn(KEY_ESC) )  // ESC to exit help screen
		{
		boxy = ((SCREENHEIGHT/2)-(pageHeight/2));


		// Add dirty rectangle
		AddRectangle(b, 0, boxy, SCREENWIDTH, pageHeight);

		// First, draw a background
		rectfill(b, 0, boxy, SCREENWIDTH, boxy + pageHeight, makecol(64,64,128));

		// Draw some white lines
		line (b, 0, boxy+5, SCREENWIDTH, boxy+5, makecol(255,255,255));
		line (b, 0, (boxy+pageHeight)-5, SCREENWIDTH, (boxy+pageHeight)-5, makecol(255,255,255));

		boxy += bh/2;

		textout(b,	(FONT *)maindata[MS].dat,
			"Keyboard Controls:",  boxx, boxy, makecol(255,255,0));		boxy += nxt+5;
		
		i=0;
		while(txt[i][0] != '.' )
			{
			textout(b, (FONT *)maindata[MS].dat, txt[i], boxx, boxy, makecol(255,255,0));
			boxy += nxt;
			i++;
			}

		Screen_Blit();
		Sleep(100);
		CheckKeys();
		if (mouse_b & 1)break;  // click mouse to exit help screen
		}
}

void	testsetup(void)
{
	int		ret;

	clear_keybuf();

	// Make sure we're not in multiplayer mode
	multiplayer = 0;


	// Set the dialog boxes based on cvars
	numround_setting  = (int)num_rounds->value;
	numplayer_setting = (int)num_players->value;
	ustrcpy(servername, server_name->string);

// Set the number of rounds
	numrounds = 10;
	demomode = 1;				//	turn off dialogs

	ResetTanks();				// Reset all tank data
	StartNewGame(2);		// Setup tank varibles

// Setup bot character (inherit stats from another character)
	ta[0].skill = TANKSKILL_HUMAN;
	ta[1].skill = TANKSKILL_GUNNER;
	ta[1].SetupBotCharacter(ta[1].skill);
	ustrcpy(ta[0].name, "Tester");

//	this will set what random level is used and what accuracy the bot will fire the first shot.
	srand(6543);

	// Load the actual level into memory
	LoadLevel(NULL);	// test pesudo random level
	LoadAmmo();
	ShieldPowerUp(0, superconventional_shield);
	ta[0].activeweapon = W_HEAVYROLLER;
	ta[0].lpower = ConvertFromGamePower(700);
	ta[0].deg = 17 /1.4;
			
	demomode = 0;	// turn player control back on.

	// Enter gamestate
	StartLevel(true);
}