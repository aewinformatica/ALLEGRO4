#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"


void  DoRefresh(void);


int		numplayer_setting = 2;
int		numround_setting = 10;
 
char	numplayer_string[32] = "2";
char	numround_string[32] = "10";

char	cashstr[64] ="";


void player_up()
{
	if (teamplay->value > 0)
	{
		numplayer_setting++;
		if (numplayer_setting > MAX_TANKTEAMS)
			numplayer_setting = 1;
	}
	else
	{
		numplayer_setting++;
		if (numplayer_setting > MAX_TANKS)
			numplayer_setting = 2;
	}

	sprintf(numplayer_string, "%d", numplayer_setting);

	DoRefresh();
}

void player_down()
{
	if (teamplay->value > 0)
	{
		numplayer_setting--;
		if (numplayer_setting < 1)
			numplayer_setting = MAX_TANKTEAMS;
	}
	else
	{
		numplayer_setting--;
		if (numplayer_setting < 2)
			numplayer_setting = MAX_TANKS;
	}
	
	sprintf(numplayer_string, "%d", numplayer_setting);

	DoRefresh();
}

void rounds_up()
{
	numround_setting += 5;
	 if (numround_setting > 100)
		numround_setting = 5;

	sprintf(numround_string, "%d", numround_setting);

	DoRefresh();
}

void rounds_down()
{
	numround_setting -= 5;
	if (numround_setting < 5)
		numround_setting = 500;//100;

	sprintf(numround_string, "%d", numround_setting);

	DoRefresh();
}

void cash_up()
{
	cashstart->value += 10000;

	if (cashstart->value > MONEY_MAXCASHSTART)
		Cvar_SetValue("cashstart", 0);

	sprintf(cashstr, "$ %d", (int)cashstart->value);

	// Refresh
	object_message(&dialog_cashoptions[2], MSG_DRAW, 0);
}

void cash_dn()
{
	cashstart->value -= 10000;

	if (cashstart->value < 0)
		Cvar_SetValue("cashstart", MONEY_MAXCASHSTART);

	sprintf(cashstr, "$ %d", (int)cashstart->value);

	// Refresh
	object_message(&dialog_cashoptions[2], MSG_DRAW, 0);
}

int	d_mapsettings_check(int msg, DIALOG *d, int c)
{
	int		ret = d_agup_check_proc(msg, d, c);
	int		i;

	if (msg == MSG_CLICK)
	{
		if (d->flags & D_SELECTED)
		{
			for (i=4; i<11; i++)
			{
				dialog_mapsettings[i].flags = D_DISABLED;
			}
		}
		else
		{
			for (i=4; i<11; i++)
			{
				dialog_mapsettings[i].flags &= ~D_DISABLED;
			}

			// Set default walls
			dialog_mapsettings[4].flags = D_SELECTED;
		}

		broadcast_dialog_message(MSG_DRAW, 0);

	}

	return ret;
}


int	d_soundoptions_proc(int msg, DIALOG *d, int c)
{
	Cvar_SetValue("soundvolume", dialog_soundoptions[1].d2);
	Cvar_SetValue("musicvolume", dialog_soundoptions[2].d2);

	Cvar_CheckForChange();

	return d_agup_button_proc(msg, d, c);
}

int	d_newgame_proc(int msg, DIALOG *d, int c)
{
	int		ret = d_agup_check_proc(msg, d, c);

	if (msg == MSG_CLICK)
	{
		// Fill in the teamplay/friendly fire box if applicable
		if (dialog_newgame[10].flags & D_SELECTED)
		{
			// Default. Makes it visable
			dialog_newgame[11].flags = 0;

			Cvar_SetValue("teamplay", 1);
	
			// Friendly fire set?
			if (Cvar_VariableValue("friendlyfire") > 0)
			{
				dialog_newgame[11].flags |= D_SELECTED;
				Cvar_SetValue("friendlyfire", 1);
			}
			
			if (numplayer_setting > MAX_TANKTEAMS)
			{
				numplayer_setting = MAX_TANKTEAMS;
				sprintf(numplayer_string, "%d", numplayer_setting);
				object_message(&dialog_newgame[2], MSG_DRAW, 0);
			}

			object_message(&dialog_newgame[10], MSG_DRAW, 0);
			object_message(&dialog_newgame[11], MSG_DRAW, 0);

			// Show "number of players" and hide the team text
			dialog_newgame[12].flags = D_HIDDEN;
			dialog_newgame[13].flags = 0;
			object_message(&dialog_newgame[13], MSG_DRAW, 0);
		}
		else
		{
			Cvar_SetValue("teamplay", 0);

			if (numplayer_setting <= 1)
			{
				numplayer_setting = 2;
				sprintf(numplayer_string, "%d", numplayer_setting);
			}

			dialog_newgame[10].flags =  0;
			dialog_newgame[11].flags = D_HIDDEN;

			// Show "number of players" and hide the team text
			dialog_newgame[12].flags = 0;
			dialog_newgame[13].flags = D_HIDDEN;

			broadcast_dialog_message(MSG_DRAW, 0);
		}

	}
	return ret;
}

// Prevent players from hitting the ESC key to exit the new game setup
int	exit_newgame() { return D_O_K; }


DIALOG dialog_newgame[] =
{
  
   { d_agup_window_proc,		188,  135,  426,  261,  255,		0,			0,		0,       0,    0,    (char *)"Single player" },

   // Game name edit box
   { d_agup_edit_proc,          228,  186,  344,  22,	 0,			-1,			0,		0,		30,    0,    servername, NULL, NULL },

   // Players
   { d_agup_edit_proc,          266,  249,  33,   15,   0,    -1,		0,			D_DISABLED,		30,    0,    numplayer_string, NULL, NULL },
   { d_agup_push_proc,			300,  241,  25,   17,   255,  0,		0,			D_EXIT,       0,    0,    (char *)"Up", 0, (void *)player_up},
   { d_agup_push_proc,			300,  266,  25,   17,   255,  0,		0,			D_EXIT,       0,    0,    (char *)"Dn", 0, (void *)player_down},

   // Rounds
   { d_agup_edit_proc,			500,  249,  33,   15,   0,    -1,    0,				D_DISABLED,	   30,   0,    numround_string, NULL, NULL },
   { d_agup_push_proc,			534,  241,  25,   17,   255,  0,    0,				D_EXIT,       0,    0,    (char *)"Up", 0, (void *)rounds_up},
   { d_agup_push_proc,			534,  266,  25,   17,   255,  0,    0,				D_EXIT,       0,    0,    (char *)"Dn", 0, (void *)rounds_down},

   // Misc buttons
   { d_agup_button_proc,        446,     361,  78,   26,   255,  0,    0,			D_EXIT,  0,       0,    (char *)"Ok" },
   { d_agup_button_proc,	    527,     361,  78,   26,   255,  0,    0,			D_EXIT,  0,       0,    (char *)"Cancel" },

   // Teamplay box
   { d_newgame_proc,			228,     311,  125,  12,  	0,			0,			0,		0,			255,	0,	 (char *)"Enable Teamplay",   0,     0 },
   { d_agup_check_proc,			411,     311,  125,  12,  	0,			0,			0,		0,			255,	0,	 (char *)"Enable Friendly Fire",   0,     0 },

   // Text
   { d_text_proc,				216,     224,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Number of Players   " },
   { d_text_proc,				216,     224,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Players Per Team   " },
	//
   { d_text_proc,				450,     224,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Number of Rounds" },
   { d_text_proc,				228,     166,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Game name..." },

   { d_keyboard_proc,		0,       0,  0,     0,      0,    0,    0,    0,        KEY_ESC,0,    (void *)exit_newgame, NULL, NULL  },
   { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }

};


DIALOG dialog_creategame[] =
{
  
   { d_agup_window_proc,		232,  187,  327,  266,  255,		0,			0,		0,     0,    0,    (char *)"Create Game" },

   // Players
   { d_agup_edit_proc,          337,  289,  58,   15,   0,    -1,		0,			D_DISABLED,		30,    0,    numplayer_string, NULL, NULL },

   { d_agup_push_proc,			398,  277,  25,   17,   255,  0,		0,			D_EXIT,       0,    0,    (char *)"Up", 0, (void *)player_up},
   { d_agup_push_proc,			398,  302,  25,   17,   255,  0,		0,			D_EXIT,       0,    0,    (char *)"Dn", 0, (void *)player_down},

   // Rounds
   { d_agup_edit_proc,			337,  374,  58,   15,   0,    -1,    0,				D_DISABLED,	   30,   0,    numround_string, NULL, NULL },
   { d_agup_push_proc,			398,  362,  25,   17,   255,  0,    0,				D_EXIT,       0,    0,    (char *)"Up", 0, (void *)rounds_up},
   { d_agup_push_proc,			398,  388,  25,   17,   255,  0,    0,				D_EXIT,       0,    0,    (char *)"Dn", 0, (void *)rounds_down},

   // Misc buttons
   { d_agup_button_proc,        392,     417,  78,   26,   255,  0,    0,			D_EXIT,  0,       0,    (char *)"Create" },
   { d_agup_button_proc,	    473,     417,  78,   26,   255,  0,    0,			D_EXIT,  0,       0,    (char *)"Cancel" },

   { d_text_proc,				260,     270,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Number of Players" },
   { d_text_proc,				260,     355,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Number of Rounds" },
   { d_text_proc,				260,     220,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Server name..." },

   // Game name edit box
   { d_agup_edit_proc,          262,	 237,  262,		22,		 0,	  -1,	   0,		0,	30,    0,    (void *)servername, NULL, NULL },

   { d_keyboard_proc,		0,       0,  0,     0,      0,    0,    0,    0,        KEY_ESC,0,    (void *)exit_newgame, NULL, NULL  },
   { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }

};



DIALOG dialog_soundoptions[] =
{
   { d_agup_window_proc,		230,  223,  318,  144,		255,		0,			0,		0,		0,    0,    (char *)"Sound Options" },
   { d_agup_slider_proc,		249,  274,  275,   12,		0,			0,			0,		0,      255,  0,  0,   0,     0 },
   { d_agup_slider_proc,		249,  318,  275,   12,		0,			0,			0,		0,      255,  0,  0,   0,     0 },
   { d_text_proc,				249,  254,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Sound Effects Volume" },
   { d_text_proc,				249,  298,  0,		0,   agup_fg_color,  agup_bg_color,    0,		0,		 0,       0,    (char *)"Music Volume" },
   { d_soundoptions_proc,	    472,  344,  66,    17,		255,		0,			0,		D_EXIT,   0,  0,   (char *)"Ok" },
   { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }
};


DIALOG dialog_videooptions[] =
{
   { d_agup_window_proc,		230,  200,  425,  200,	255,		0,		0,	0,				0,		0,	(char *)"Video Options" },
   { d_agup_check_proc,			250,  235,  175,   12,		0,		0,		0,	0,			255,		0,	(char *)"Fullscreen",   0,     0 },
   { d_agup_check_proc,			250,  255,  175,   12,		0,		0,		0,	0,			255,		0,	(char *)"Outline Barrel",0,     0 },  // ??
   { d_agup_check_proc,			250,  275,  175,   12,		0,		0,		0,	0,			255,		0,	(char *)"No Video Animation",0,     0 },  // ??
   { d_agup_check_proc,			250,  315,  175,   12,		0,		0,		0,	0,			255,		0,	(char *)"ScreenShot use BMP",0,  0 },// ??
   { d_text_proc,						250,  335,    0,		0, agup_fg_color, agup_bg_color,0,0,0,0,	(char *)"ScreenShot JPEG Quality" },// ??
	 { d_agup_radio_proc,			250,  355,   24,   20,		0,		0,    0,    0,		 1,			0,	(char *)"None",   0,     0 },
	 { d_agup_radio_proc,			310,  355,   24,   20,		0,		0,    0,    0,		 1,			0,	(char *)"Low",    0,     0 },
	 { d_agup_radio_proc,			365,  355,   24,   20,		0,		0,    0,    0,		 1,			0,	(char *)"Medium", 0,     0 },
	 { d_agup_radio_proc,			440,  355,   24,   20,		0,		0,    0,    0,		 1,			0,	(char *)"High",   0,     0 },
   { d_agup_button_proc,	  525,  380,   66,   17,	255,		0,		0,	D_EXIT,	 0,			0,	(char *)"Ok" },
   { d_agup_check_proc,			440,  235,  175,   12,		0,		0,		0,		0,	 255,			0,	(char *)"Hide HUD on Shot",   0,     0 },
   { NULL,                    0,    0,    0,    0,    0,    0,    0,		0,		 0,			0,	0,                     0,     0 }
};

DIALOG dialog_talkingtanks[] =
{
   { d_agup_window_proc,		230,  223,  318,  144,		255,		0,			0,		0,			0,		0,   (char *)"Talking Tanks" },
   { d_agup_check_proc,			249,  294,  275,   12,		0,			0,			0,		0,			255,	0,	 (char *)"Enable talking tanks",   0,     0 },
   { d_agup_button_proc,	    472,  344,  66,    17,		255,		0,			0,		D_EXIT,		0,		0,   (char *)"Ok" },
   { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }
};


DIALOG dialog_cashoptions[] =
{
   { d_agup_window_proc,		230,  223,  318,  144,		255,		0,			0,		0,		0,    0,    (char *)"Game Options" },
   
   // Starting cash
   { d_text_proc,				249,  254,  0,		0,   agup_fg_color,  agup_bg_color,     0,				0,		 0,       0,    (char *)"Starting Cash" },
   { d_agup_edit_proc,			249,  274,  150,    15,		0,			0,			0,		D_DISABLED,     60,     0,	  cashstr,     NULL, NULL },
   //{ d_agup_edit_proc,			337,  336,  58,    15,		0,			-1,			0,		D_DISABLED,	    30,     0,    numround_string, NULL, NULL },
   
   // Mass kill
   { d_agup_push_proc,			427,  268,  25,    17,		0,			0,			0,		D_EXIT,			0,  0,   (char *)"Up", 0, (void *)cash_up },
   { d_agup_push_proc,			427,  287,  25,    17,		0,			0,			0,		D_EXIT,			0,  0,   (char *)"Dn", 0, (void *)cash_dn },
   
   //{ d_text_proc,				249,  298,  0,		0,   agup_fg_color,  agup_bg_color,    0,				0,		 0,       0,    (char *)"Music Volume" },
   { d_agup_button_proc,	    472,  344,  66,    17,		255,		0,			0,		D_EXIT,			0,  0,   (char *)"Ok" },
   { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                   0,     0 }
};


// The wall type/gravity dialog
DIALOG dialog_mapsettings[] =
{
	  { d_agup_window_proc,		125,   150,  500,  320,   255,  0,    0,    0,         0,	   0,    (char *)"Wall & Gravity Options" },
	  { d_mapsettings_check,    167,   189,  240,   20,		0,  0,    0,    0,		 0,    0,	 (char *)"Use Map Settings",  0,     0 },
	  { d_text_proc,			167,   209,    0,    0,     agup_fg_color,  agup_bg_color,    0,    0,       0,			 0,    (char *)"Wall Types" },
	  { d_text_proc,			167,   348,    0,    0,     agup_fg_color,  agup_bg_color,    0,    0,       0,			 0,    (char *)"Gravity Options" },
	  { d_agup_radio_proc,      167,   230,  240,   20,		0,  0,    0,    0,		 1,    0,	 (char *)"Normal Walls",    0,     0 },
	  { d_agup_radio_proc,      167,   257,  240,   20,		0,  0,    0,    0,		 1,    0,	 (char *)"Rubber Walls",    0,     0 },
	  { d_agup_radio_proc,      167,   284,  240,   20,		0,  0,    0,    0,		 1,    0,	 (char *)"Cement Walls",    0,     0 },
	  { d_agup_radio_proc,      167,   311,  240,   20,		0,  0,    0,    0,		 1,    0,	 (char *)"Wrap-Around Walls",    0,     0 },
	  { d_agup_check_proc,      167,   370,  240,   20,		0,  0,    0,    0,		 0,    0,	 (char *)"No Dirt Gravity",  0,     0 },
	  { d_agup_check_proc,      167,   395,  240,   20,		0,  0,    0,    0,		 0,    0,	 (char *)"No Tank Gravity",  0,     0 },
	  { d_agup_check_proc,      167,   420,  240,   20,		0,  0,    0,    0,		 0,    0,	 (char *)"No Water Gravity",  0,     0 },
	  { d_agup_button_proc,		531,   445,  88,    20,   255,  0,    0,    D_EXIT,	 0,    0,    (char *)"Done" },
	  { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }
};

//
// DoRefresh - Refreshes the current dialog
//

void DoRefresh(void)
{
	object_message(&dialog_newgame[2], MSG_DRAW, 0);
	object_message(&dialog_newgame[5], MSG_DRAW, 0);
}


//
// Starts up the dialog system
//


void Init_Dialog_Stuff(void)
{
	// Init the AGUP add-on
	//agup_init(agtk_theme);
	//agup_init(aphoton_theme);

	// Nil text background
	text_mode(-1);

	// Init AGUP
	agup_init(abeos_theme);

	gui_fg_color = agup_fg_color;
    gui_bg_color = agup_bg_color;
    gui_shadow_box_proc = d_agup_shadow_box_proc;
    gui_button_proc = d_agup_button_proc;
    gui_edit_proc = d_agup_edit_proc;
    gui_text_list_proc = d_agup_text_list_proc;

	// Set default buffer
	agup_buffer = screen;
}

void	Shutdown_Dialog_Stuff(void)
{
	agup_shutdown();
}


//
// Start_Dialogs - The core...the meat! This does it ALL!
//

int	Start_Dialog(DIALOG *dlg)
{
	int	ret;
	
	set_dialog_color(dlg, gui_fg_color, gui_bg_color);
	ret = do_agup_dialog(dlg, screen, NULL);

	// New game!
	if (ret == 8)
	{
		PlayLevelOGG("playermenu.ogg");

		// Reset all tank data
		ResetTanks();

		// Setup tank varibles
		StartNewGame(numplayer_setting);
		
		// Load the actual level into memory
		LoadLevel(NULL);	// random level

		// Init default que
		InitDefaultTurnQue();

		// Cycle through buystuff screens
		StartBuyStuff();

		// Enter gamestate
		StartLevel(true);
	}

	return ret;
}


void	Kill_Dialog(DIALOG *dlg)
{
	broadcast_dialog_message(MSG_END, 0);
}
