#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"
//#include "peffect.h"
#include "secrets.h"
#include "agup.h"
#include "fblend.h"

static		int		yellow_pulse=255;
static		int		yellow_pulse_dir=0;

// Animated question mark profile for secret characters
BITMAP		*unknown_profile;

BITMAP		*playersetup_buffer;
BITMAP		*playersetup_background;
char		playername_string[64];
int			current_player_setup;
bool		config_active;
int			multiplayer_setup_time;
int			playersetuptics;
int			charpage;

DIALOG			*ps_dlg;
DIALOG_PLAYER	*ps_plyr;

void		PS_Drawer(void);
void		PS_ReClassDrawer(void);

character_t		characters[NUM_CHARACTERS] = {

	{ 
		"Marie & Ricky",		// Name	
		STACIERICKY_ICON,		// Icon pic
		STACIERICKY_PROFILE,	// Large profile pic
		STACIERICKY_CHART,		// HED Chart
		TANK1_PROFILE,			// Large tank pic
		TANK1_INGAME,			// Ingame tank
		510,  
		167, 
		128, 
		128,
		5,
		15,
		CANNON_L,				// Cannon length
		3,3,3,					// HEB attributes
		-1,3,2,1,				// Move Up,Dn,Left,Right
		true,					// Unlocked?
	},

	{ 
		"Saru",					// Name	
		SARU_ICON,				// Icon pic
		SARU_PROFILE,			// Large profile pic
		SARU_CHART,				// HED Chart
		TANK2_PROFILE,			// Large tank pic
		TANK2_INGAME,			// Ingame tank
		648,  
		167, 
		128, 
		128,
		3,
		8,
		10,						// Cannon length
		2,2,5,					// HEB attributes
		-1,4,0,2,				// Move Up,Dn,Left,Right
		true,					// Unlocked?
	},

	{ 
		"April",				// Name	
		APRIL_ICON,				// Icon pic
		APRIL_PROFILE,			// Large profile pic
		APRIL_CHART,			// HED Chart
		TANK3_PROFILE,			// Large tank pic
		TANK3_INGAME,			// Ingame tank
		372,  
		307, 
		128, 
		128,
		7,
		12,
		CANNON_L,				// Cannon length
		5,3,1,					// HEB attributes
		0,6,5,3,				// Move Up,Dn,Left,Right
		true,					// Unlocked?
	},

	{ 
		"Jack",					// Name	
		JACK_ICON,				// Icon pic
		JACK_PROFILE,			// Large profile pic
		JACK_CHART,				// HED Chart
		TANK4_PROFILE,			// Large tank pic
		TANK4_INGAME,			// Ingame tank
		510,  
		307, 
		128, 
		128,
		5,
		14,
		CANNON_L,				// Cannon length
		3,1,5,					// HEB attributes
		0,7,2,4,				// Move Up,Dn,Left,Right
		true,					// Unlocked?
	},
	
	{ 
		"Mia",					// Name	
		MIA_ICON,				// Icon pic
		MIA_PROFILE,			// Large profile pic
		MIA_CHART,				// HED Chart
		TANK5_PROFILE,			// Large tank pic
		TANK5_INGAME,			// Ingame tank
		648,  
		307, 
		128, 
		128,
		4,
		14,
		CANNON_L,				// Cannon length
		3,3,3,					// HEB attributes
		1,7,3,5,				// Move Up,Dn,Left,Right
		true,					// Unlocked?
	},

	{ 
		"Lock",					// Name	
		LOCK_ICON,				// Icon pic
		LOCK_PROFILE,			// Large profile pic
		LOCK_CHART,				// HED Chart
		TANK6_PROFILE,			// Large tank pic
		TANK6_INGAME,			// Ingame tank
		234,  
		447, 
		128, 
		128,
		4,
		15,
		CANNON_L,				// Cannon length
		2,5,2,					// HEB attributes
		4,2,-1,6,				// Move Up,Dn,Left,Right
		true,					// Unlocked?
	},

	{ 
		"Miki",					// Name	
		MIKI_ICON,				// Icon pic
		MIKI_PROFILE,			// Large profile pic
		MIKI_CHART,				// HED Chart
		TANK7_PROFILE,			// Large tank pic
		TANK7_INGAME,			// Ingame tank
		372,  
		447, 
		128, 
		128,
		4,
		15,
		CANNON_L,				// Cannon length
		3,5,1,					// HEB attributes
		2,-1,5,7,				// Move Up,Dn,Left,Right
		true,					// Unlocked?
	},

	{ 
		"Meshko",				// Name	
		MESHKO_ICON,			// Icon pic
		MESHKO_PROFILE,			// Large profile pic
		MESHKO_CHART,			// HED Chart
		TANK8_PROFILE,			// Large tank pic
		TANK8_INGAME,			// Ingame tank
		510,  
		447, 
		128, 
		128,
		8,
		4,
		10,						// Cannon length
		5,1,3,					// HEB attributes
		3,-1,6,0,				// Move Up,Dn,Left,Right
		true,					// Unlocked?
	},

	{ 
		"Jira",					// Name	
		JIRA_ICON,				// Icon pic
		JIRA_PROFILE,			// Large profile pic
		JIRA_CHART,				// HED Chart
		TANK9_PROFILE,			// Large tank pic
		TANK9_INGAME,			// Ingame tank
		510,  
		167,
		128, 
		128,
		5,
		16,
		13,						// Cannon length
		5,2,2,					// HEB attributes
		10,10,11,9,				// Move Up,Dn,Left,Right
		false,					// Unlocked?
	},

	{ 
		"Richie",				// Name	
		RICHIE_ICON,			// Icon pic
		RICHIE_PROFILE,			// Large profile pic
		RICHIE_CHART,			// HED Chart
		TANK10_PROFILE,			// Large tank pic
		TANK10_INGAME,			// Ingame tank
		648,  
		167, 
		128, 
		128,
		6,
		15,
		CANNON_L,				// Cannon length
		3,3,3,					// HEB attributes
		11,11,8,10,				// Move Up,Dn,Left,Right
		false,					// Unlocked?
	},

	{ 
		"smackbringer",				// Name	
		SMACKBRINGER_ICON,			// Icon pic
		SMACKBRINGER_PROFILE,		// Large profile pic
		SMACKBRINGER_CHART,			// HED Chart
		TANK11_PROFILE,				// Large tank pic
		TANK11_INGAME,				// Ingame tank
		372,  
		307, 
		128, 
		128,
		5,
		12,
		13,						// Cannon length
		1,3,5,					// HEB attributes
		8,8,9,11,				// Move Up,Dn,Left,Right
		false,					// Unlocked?
	},

	{ 
		"ShadowFang",			// Name	
		SHADOWFANG_ICON,		// Icon pic
		SHADOWFANG_PROFILE,		// Large profile pic
		SHADOWFANG_CHART,		// HED Chart
		TANK12_PROFILE,			// Large tank pic
		TANK12_INGAME,			// Ingame tank
		510,  
		307, 
		128, 
		128,
		6,
		5,
		10,						// Cannon length
		1,5,3,					// HEB attributes
		9,9,10,8,				// Move Up,Dn,Left,Right
		false,					// Unlocked?
	},

	{ 
		"Stoopid",				// Name	
		STOOPID_ICON,			// Icon pic
		AI_PROFILE,				// Large profile pic
		STACIERICKY_CHART,		// HED Chart
		AITANK_PROFILE,			// Large tank pic
		TANKai1_INGAME,			// Ingame tank
		510,							// x;
		307,							// y;
		128, 							// w;
		128,							// h;
		2,								// cannonypos;
		10,								// cannonxpos;
		9,								// BOT AI: Configurable	cannonlength;
		0,0,0,						// BOT AI: Configurable	Protection against heat, electric, balistic weapons (x/5)
		0,0,0,0,					// BOT AI: N/A	// Keeping track of where to move useing the keyboard
		true,							// Unlocked?	  // Whether this is an unlocked character or not
	},

	{ 
		"Lobber",				  // Name	
		LOBBER_ICON,			// Icon pic
		AI_PROFILE,				// Large profile pic
		STACIERICKY_CHART,	// HED Chart
		AITANK_PROFILE,			// Large tank pic
		TANKai2_INGAME,			// Ingame tank
		510,  
		307, 
		128, 
		128,
		2,
		10,
		9,				      // BOT AI: Configurable
		0,0,0,					// BOT AI: Configurable
		0,0,0,0,				// BOT AI: N/A
		true,					  // Unlocked?
	},

	{ 
		"Shooter",				// Name	
		SHOOTER_ICON,			// Icon pic
		AI_PROFILE,				// Large profile pic
		STACIERICKY_CHART,	// HED Chart
		AITANK_PROFILE,			// Large tank pic
		TANKai3_INGAME,			// Ingame tank
		510,  
		307, 
		128, 
		128,
		4,
		10,
		9,				// BOT AI: Configurable
		0,0,0,					// BOT AI: Configurable
		0,0,0,0,				// BOT AI: N/A
		true,					// Unlocked?
	},

	{ 
		"Gunner",				// Name	
		GUNNER_ICON,			// Icon pic
		AI_PROFILE,				// Large profile pic
		STACIERICKY_CHART,		// HED Chart
		AITANK_PROFILE,			// Large tank pic
		TANKai4_INGAME,			// Ingame tank
		510,  
		307, 
		128, 
		128,
		4,
		16,
		CANNON_L,				// BOT AI: Configurable
		0,0,0,					// BOT AI: Configurable
		0,0,0,0,				// BOT AI: N/A
		true,					// Unlocked?
	},

	{ 
		"Sniper",				// Name	
		SNIPER_ICON,			// Icon pic
		AI_PROFILE,				// Large profile pic
		STACIERICKY_CHART,		// HED Chart
		AITANK_PROFILE,			// Large tank pic
		TANKai5_INGAME,			// Ingame tank
		510,  
		307, 
		128, 
		128,
		4,
		15,
		CANNON_L,				// BOT AI: Configurable
		0,0,0,					// BOT AI: Configurable
		0,0,0,0,				// BOT AI: N/A
		true,					// Unlocked?
	},
};



char *skilllevel_getter(int index, int *list_size)
{
   static char *skillstrings[] =
   {
     "Human",
	 characters[CHAR_STOOPID].name,
	 characters[CHAR_LOBBER].name,
	 characters[CHAR_SHOOTER].name,
	 characters[CHAR_GUNNER].name,
	 characters[CHAR_SNIPER].name,
	 "Random AI",
   };

   if (index < 0) {
      *list_size = 7;
      return NULL;
   }
   else
      return skillstrings[index]; 
}


char *reclassskilllevel_getter(int index, int *list_size)
{
   static char *skillstrings[] =
   {
     "Human",
	 characters[CHAR_STOOPID].name,
	 characters[CHAR_LOBBER].name,
	 characters[CHAR_SHOOTER].name,
	 characters[CHAR_GUNNER].name,
	 characters[CHAR_SNIPER].name,
	 "Random AI",
	 "Non-existant"
   };

   if (index < 0) {
      *list_size = 8;
      return NULL;
   }
   else
      return skillstrings[index]; 
}


char *multiplayerskilllevel_getter(int index, int *list_size)
{
   static char *skillstrings[] =
   {
     "Open slot",
	 characters[CHAR_STOOPID].name,
	 characters[CHAR_LOBBER].name,
	 characters[CHAR_SHOOTER].name,
	 characters[CHAR_GUNNER].name,
	 characters[CHAR_SNIPER].name,
	 "Random",
   };

   if (index < 0) {
      *list_size = 7;
      return NULL;
   }
   else
      return skillstrings[index]; 
}


int		done_config(int	i)
{
	config_active = false;

	PS_Refresh(current_player_setup);

	return	D_EXIT;
}


void	edit_config(void)
{
	
	config_active = true;

	PS_Refresh(current_player_setup);
}

int	exit_ps(void)
{
	return D_O_K;
}


int		d_playersetup_button(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		PS_Drawer();
		return D_O_K;
	}

	return d_agup_button_proc(msg, d, c);
}


int		d_reclass_button(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		PS_ReClassDrawer();
		return D_O_K;
	}

	return d_agup_button_proc(msg, d, c);
}




// Global dialog that is used in all the main player setup 
// menu's
DIALOG dialog_playersetup[] =
{
   { d_agup_window_proc,		196,	 242,  320,   200,	255,  0,    0,    0,		0,		0,    (char *)"Player Setup" },
   { d_agup_edit_proc,			292,     273,  214,   16,		255,  0,    0,    0,		60,		0,    (void *)playername_string },
   { d_agup_list_proc,			219,     315,  214,   89,		255,  0,    0,    0,		0,		0,    (void *)skilllevel_getter },
   { d_playersetup_button,		457,     417,  47,    18,		255,  0,    0,	  D_EXIT,	0,		0,    (char *)"Done", (void *)done_config },

   // Some text
   { d_agupbuffer_text,			219,	   273,	 0,	    0,	    agup_fg_color,  agup_bg_color,    0,    0,		0,		0,    (char *)"Name" },
   { d_agupbuffer_text,			219,	   300,	 0,	    0,	    agup_fg_color,  agup_bg_color,    0,    0,		0,		0,    (char *)"Player Class" },

   { d_keyboard_proc,			0,       0,  0,     0,      0,    0,    0,    0,        KEY_ESC,0,    (void *)exit_ps, NULL, NULL  },
   { NULL,						0,       0,  0,     0,      0,    0,    0,    0,        0,0,    0, NULL, NULL  }
};

#define	SKILL_LISTBOX		2


int		reclass0(void)
{ PS_RunReClassFor(0); return D_EXIT; }

int		reclass1(void)
{ PS_RunReClassFor(1); return D_EXIT; }

int		reclass2(void)
{ PS_RunReClassFor(2); return D_EXIT; }

int		reclass3(void)
{ PS_RunReClassFor(3); return D_EXIT; }

int		reclass4(void)
{ PS_RunReClassFor(4); return D_EXIT; }

int		reclass5(void)
{ PS_RunReClassFor(5); return D_EXIT; }

int		reclass6(void)
{ PS_RunReClassFor(6); return D_EXIT; }

int		reclass7(void)
{ PS_RunReClassFor(7); return D_EXIT; }

DIALOG dialog_reclassplayersetup[] =
{
   { d_agup_window_proc,		192,     163,  400,   300,			255,  0,    0,    0,				0,		0,    (char *)"Player Reclass" },
   { d_reclass_button,			510,     435,  75,    15,			255,  0,    0,	  D_EXIT,			0,		0,    (char *)"Done", (void *)exit_ps },

   // Player 1
   { d_agup_push_proc,			200,	 200,  50,	  21,			255,  0,    0,	  0,				0,		0,    (char *)"...", 0, (void *)reclass0 },
   { d_agup_edit_proc,			325,     200,  201,   21,			255,  0,    0,    D_DISABLED,		25,		0,    ta[0].name  },

   // Player 2
   { d_agup_push_proc,			200,	 230,  50,	  21,			255,  0,    0,	  D_EXIT,				0,		0,    (char *)"...", 0, (void *)reclass1 },
   { d_agup_edit_proc,			325,     230,  201,   21,			255,  0,    0,    D_DISABLED,		25,		0,    ta[1].name  },

   // Player 3
   { d_agup_push_proc,			200,	 260,  50,	  21,			255,  0,    0,	  D_EXIT,				0,		0,    (char *)"...", 0, (void *)reclass2 },
   { d_agup_edit_proc,			325,     260,  201,   21,			255,  0,    0,    D_DISABLED,		25,		0,    ta[2].name },

   // Player 4
   { d_agup_push_proc,			200,	 290,  50,	  21,			255,  0,    0,	  D_EXIT,				0,		0,    (char *)"...", 0, (void *)reclass3 },
   { d_agup_edit_proc,			325,     290,  201,   21,			255,  0,    0,    D_DISABLED,		25,		0,    ta[3].name },

   // Player 5
   { d_agup_push_proc,			200,	 320,  50,	  21,			255,  0,    0,	  D_EXIT,				0,		0,    (char *)"...", 0, (void *)reclass4 },
   { d_agup_edit_proc,			325,     320,  201,   21,			255,  0,    0,    D_DISABLED,		25,		0,    ta[4].name },

   // Player 6
   { d_agup_push_proc,			200,	 350,  50,	  21,			255,  0,    0,	  D_EXIT,				0,		0,    (char *)"...", 0, (void *)reclass5 },
   { d_agup_edit_proc,			325,     350,  201,   21,			255,  0,    0,    D_DISABLED,		25,		0,    ta[5].name },

   // Player 7
   { d_agup_push_proc,			200,	 380,  50,	  21,			255,  0,    0,	  D_EXIT,				0,		0,    (char *)"...", 0, (void *)reclass6 },
   { d_agup_edit_proc,			325,     380,  201,   21,			255,  0,    0,    D_DISABLED,		25,		0,    ta[6].name },

   // Player 8
   { d_agup_push_proc,			200,	 410,  50,	  21,			255,  0,    0,	  D_EXIT,				0,		0,    (char *)"...", 0, (void *)reclass7 },
   { d_agup_edit_proc,			325,     410,  201,   21,			255,  0,    0,    D_DISABLED,		25,		0,    ta[7].name },

   { NULL,						0,       0,		0,     0,			0,    0,    0,    0,        0,		0,    0 }

   // Some text
   //{ d_text_proc,				219,	   273,	 0,	    0,	    agup_fg_color,  agup_bg_color,    0,    0,		0,		0,    (char *)"Name" },
   //{ d_text_proc,				219,	   300,	 0,	    0,	    agup_fg_color,  agup_bg_color,    0,    0,		0,		0,    (char *)"Player Class" },
};


DIALOG dialog_reclass[] =
{
   { d_agup_window_proc,		196,	 242,  320,   200,	255,  0,    0,    0,		0,		0,		  (char *)"Player Setup" },
   { d_agup_edit_proc,			292,     273,  214,   16,		255,  0,    0,    0,		60,		0,    (void *)playername_string },
   { d_agup_list_proc,			219,     315,  214,   89,		255,  0,    0,    0,		0,		0,    (void *)reclassskilllevel_getter },
   { d_reclass_button,			457,     417,  47,    18,		255,  0,    0,	  D_EXIT,	0,		0,    (char *)"Done", (void *)done_config },

   // Some text
   { d_agupbuffer_text,			219,	   273,	 0,	    0,	    agup_fg_color,  agup_bg_color,    0,    0,		0,		0,    (char *)"Name" },
   { d_agupbuffer_text,			219,	   300,	 0,	    0,	    agup_fg_color,  agup_bg_color,    0,    0,		0,		0,    (char *)"Player Class" },

   { d_keyboard_proc,			0,       0,  0,     0,      0,    0,    0,    0,        KEY_ESC,0,    (void *)exit_ps, NULL, NULL  },
   { NULL,						0,       0,  0,     0,      0,    0,    0,    0,        0,0,    0, NULL, NULL  }
};


DIALOG dialog_multiplayersetup[] =
{
   { d_agup_window_proc,		196,	 242,  320,   200,	255,  0,    0,    0,		0,		0,		  (char *)"Player Setup" },
   { d_agup_edit_proc,			292,     273,  214,   16,		255,  0,    0,    0,		60,		0,    (void *)playername_string },
   { d_agup_list_proc,			219,     315,  214,   89,		255,  0,    0,    0,		0,		0,    (void *)multiplayerskilllevel_getter },
   // Some text
   { d_agupbuffer_text,			219,	 273,	 0,	    0,	    agup_fg_color,  agup_bg_color,    0,    0,		0,		0,    (char *)"Name" },
   { d_agupbuffer_text,			219,	 300,	 0,	    0,	    agup_fg_color,  agup_bg_color,    0,    0,		0,		0,    (char *)"Player Class" },
   { d_playersetup_button,		457,     417,  47,    18,		255,  0,    0,	  D_EXIT,	0,		0,    (char *)"Done", (void *)done_config },

   
   { NULL,						0,       0,  0,     0,      0,    0,    0,    0,        0,0,    0, NULL, NULL  }
};

#define		RECLASSSKILL_LISTBOX	2		

//
// PS_Refresh - Refreshes the screen               ....totally.
//

void PS_Refresh(int player)
{
	int		i;
	int		charselected=0;
	char	str[128];

	draw_sprite(buffer, playersetup_background, 0,0);

	//set_multiply_blender(ta[player].colorr, ta[player].colorg, ta[player].colorb, 0);
    //draw_lit_sprite(buffer, (BITMAP *)maindata[PLAYERSETUP_BACKGROUND].dat, 0, 0, 255);
	
	// Animated the static question mark
	unknown_profile=(BITMAP *)maindata[UNKNOWN_PROFILE1+rand()%3].dat;

	if (charpage <= 0)
	{
		if (charselected > NUM_NORMALCHARACTERS || charselected < 0)
			charselected = 0;

		for (i=0; i<NUM_NORMALCHARACTERS; i++)
		{
			draw_sprite(buffer, 
				(BITMAP *)maindata[characters[i].tankicon].dat, 
				characters[i].x,
				characters[i].y);
	
			if (ta[current_player_setup].character == i)
				charselected = i;
		}
	}
	else	// Secret character page?
	{
		if (charselected > (NUM_NORMALCHARACTERS+NUM_SECRETCHARACTERS) || charselected <= NUM_NORMALCHARACTERS || charselected < 0)
			charselected = NUM_NORMALCHARACTERS+1;

		for (i=NUM_NORMALCHARACTERS; i<(NUM_NORMALCHARACTERS+NUM_SECRETCHARACTERS); i++)
		{
			if (!characters[i].unlocked)
			{
				draw_sprite(buffer, 
					unknown_profile, 
					characters[i].x,
					characters[i].y);
			}
			else
			{
				draw_sprite(buffer, 
					(BITMAP *)maindata[characters[i].tankicon].dat, 
					characters[i].x,
					characters[i].y);
			}
	
			if (ta[current_player_setup].character == i)
				charselected = i;
		}
	}

	// Draw the selected characters profile if one is selected
	if (!characters[ta[player].character].unlocked)
	{
		draw_sprite(buffer,
					(BITMAP *)maindata[BLANK_PROFILE].dat,
					23, 25);
	}
	else if (characters[ta[player].character].profilebmp > -1)
	{
		draw_sprite(buffer,
					(BITMAP *)maindata[characters[ta[player].character].profilebmp].dat,
					23, 25);
	}

	// Draw the selected character's tank if one is selected
	if (!characters[ta[player].character].unlocked)
	{
		draw_sprite(buffer,
					(BITMAP *)maindata[UNKNOWN_CHART].dat,
					24, 447);
	}
	else if (characters[ta[player].character].chartbmp > -1)
	{
		draw_sprite(buffer,
					(BITMAP *)maindata[characters[ta[player].character].chartbmp].dat,
					24, 447);
	}

	// Draw the "next" button
	if (GUI_MouseInside(647, 457, 652+((BITMAP *)maindata[PLAYERSETUP_NEXTOVER].dat)->w, 
		458+((BITMAP *)maindata[PLAYERSETUP_NEXTOVER].dat)->h) &&
		mouse_b & 1)
			draw_sprite(buffer, (BITMAP *)maindata[PLAYERSETUP_NEXTOVER].dat, 647, 457);
	else
		draw_sprite(buffer, (BITMAP *)maindata[PLAYERSETUP_NEXT].dat, 647, 457);

	if (yellow_pulse_dir == 0)
	{
		yellow_pulse += 35;
		if (yellow_pulse > 255)
		{
			yellow_pulse = 255;
			yellow_pulse_dir = 1;
		}
	}
	else
	{
		yellow_pulse -= 30;
		if (yellow_pulse <= 0)
		{
			yellow_pulse = 0;
			yellow_pulse_dir = 0;
		}
	}

	rect(buffer, characters[charselected].x, 
		characters[charselected].y, 
		characters[charselected].x+characters[charselected].w, 
		characters[charselected].y+characters[charselected].h, 
		makecol(255,yellow_pulse,yellow_pulse/2));

	rect(buffer, characters[charselected].x+1, 
		characters[charselected].y+1, 
		(characters[charselected].x+characters[charselected].w)+1, 
		(characters[charselected].y+characters[charselected].h)+1, 
		makecol(255,yellow_pulse,yellow_pulse/2));


	// Draw some text onto the screen
	usprintf(str, "Player %d  (%s)", player+1, playername_string);
	textout_centre(buffer, (FONT *)maindata[TINY8].dat, str, 580, 141, makecol(0,0,0));
	textout_centre(buffer, (FONT *)maindata[TINY8].dat, str, 580-1, 141-1, makecol(255,255,255));

}


//
// PS_RunMouse - Checks where the mouse is and updates stuff

int PS_RunMouse(int player)
{
	BITMAP	*nextbutton_dn = (BITMAP *)maindata[PLAYERSETUP_NEXT].dat;
	BITMAP	*nextbutton_up = (BITMAP *)maindata[PLAYERSETUP_NEXTOVER].dat;
	int		i;

	// Check to see if we're hovering over the NEXT button which lights up
	if (GUI_MouseInside(647, 457, 652+nextbutton_up->w, 458+nextbutton_up->h))
	{
		if (mouseflags & MOUSE_FLAG_LEFT_DOWN)
		{ }
		else if (mouseflags & MOUSE_FLAG_LEFT_UP)
		{
			// Before we return, a -1 value to end this loop, make sure the 
			// player has selected a "unlocked" character
			if (characters[ta[player].character].unlocked)
				return -1;
		}
		
	}
	

	if (charpage <= 0)
	{
		// Do normal characters first
		for (i=0; i<NUM_NORMALCHARACTERS; i++)
		{
			// Is this character even unlockable?
			//if (!characters[i].unlocked)
			//	continue;

			// Check to see if the mouse is hovering over anything
			if (mouse_x > characters[i].x &&
				mouse_x < (characters[i].x + characters[i].w) &&
				mouse_y > characters[i].y &&
				mouse_y < (characters[i].y + characters[i].h))
			{
	
				// First and foremost, check for a mouse click
				if (mouse_b & 1)
				{
					// Make sure the selection isnt the same thing
					if (ta[player].character != i)
						ta[player].character = i;
				}
			}
		
		}
	}
	else
	{
		// Now check for secret characters
		for (i=NUM_NORMALCHARACTERS; i<(NUM_NORMALCHARACTERS+NUM_SECRETCHARACTERS); i++)
		{
			// Is this character even unlockable?
			//if (!characters[i].unlocked)
			//	continue;

			// Check to see if the mouse is hovering over anything
			if (mouse_x > characters[i].x &&
				mouse_x < (characters[i].x + characters[i].w) &&
				mouse_y > characters[i].y &&
				mouse_y < (characters[i].y + characters[i].h))
			{
	
				// First and foremost, check for a mouse click
				if (mouse_b & 1)
				{
					// Make sure the selection isnt the same thing
					if (ta[player].character != i)
						ta[player].character = i;
				}
			}
		
		}
	}


	return 0;
}


//
// PS_RunKeyboard - All keyboard functions
//

int		PS_RunKeyboard(int player)
{

	// Arrow keys for moving around?
	if (IsKeyDn(KEY_UP) && characters[ta[player].character].mu >= 0)
		ta[player].character = characters[ta[player].character].mu;

	if (IsKeyDn(KEY_DOWN) && characters[ta[player].character].md >= 0)
		ta[player].character = characters[ta[player].character].md;

	if (IsKeyDn(KEY_LEFT) && characters[ta[player].character].ml >= 0)
		ta[player].character = characters[ta[player].character].ml;

	if (IsKeyDn(KEY_RIGHT) && characters[ta[player].character].mr >= 0)
		ta[player].character = characters[ta[player].character].mr;

	// Activate secret character display?
	if (IsKeyDn(KEY_ALT))
	{
		charpage = !charpage;
		if (charpage > 0)
			StartSound(SECRET_CHAR_SWITCH);
	}

	// Make sure the player is selecting a unlocked character before
	// continuing
	if (IsKeyDn(KEY_ENTER) && characters[ta[player].character].unlocked)
		return -1;

	return 0;

}

void PS_Drawer(void)
{
	PS_Refresh(current_player_setup);

	ticker();
	playersetuptics++;
	gametics++;
	frame_count++;

	draw_sprite(buffer, playersetup_buffer, 0,0);
	Screen_Blit(true);
}


void PS_RunSetup(int player)
{
	int				done=0;
	int				ret=0;

	// Always start on page 0 (normal characters)
	charpage = 0;

	// Reset player setup tics
	playersetuptics = 0;

	// Create a bitmap for the dialogs
	playersetup_buffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(playersetup_buffer, TRANSCOLOR);

	//ChangePalette(ta[player].colorr, ta[player].colorg, ta[player].colorb);
	
	// Create the color tinted background image. This only needs to be done once to
	// reduce strain on CPU cycles
	{
		//BITMAP	*backbmp = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
		//clear_to_color(backbmp, TRANSCOLOR);

		playersetup_background = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
		clear_to_color(playersetup_background, TRANSCOLOR);

		// First, start the background with the standard um...background
		//draw_sprite(playersetup_background, (BITMAP *)maindata[PLAYERSETUP_BACKGROUND].dat, 0, 0);

		// First, tint the plate BMP to a full color to its own bitmap (backbmp)
		set_multiply_blender(ta[player].colorr, ta[player].colorg, ta[player].colorb, 0);
		draw_lit_sprite(playersetup_background, (BITMAP *)maindata[PLAYERSETUP_BACKGROUND].dat, 0,0,255);

		draw_sprite(playersetup_background, (BITMAP *)maindata[PLAYERSETUP_CHOOSECHAR].dat, 375, 16);
		//solid_mode();
	
		//set_multiply_blender(ta[player].colorr, ta[player].colorg, ta[player].colorb, 0);
	    //draw_lit_sprite(playersetup_background, backbmp, 0, 0, 255);

		//destroy_bitmap(backbmp);
	}

	// Make sure we have transparent text drawing
	text_mode(-1);

	// Clear out the keyboard buffer
	clear_keybuf();

	// Gobal
	current_player_setup = player;
	config_active = true;

	// Clear out the name
	usprintf(playername_string, "");
	
	// Refresh the screen
	PS_Refresh(player);

	// Run the dialog, and grab the return value
	//ret = do_dialog(dialog_playersetup, -1);//, playersetup_buffer, PS_Drawer);
	if (demomode <= 0)
		ret = do_agup_dialog(dialog_playersetup, playersetup_buffer, PS_Drawer);

	// Figure out if this is a human or computer player and set their skill
	if (dialog_playersetup[SKILL_LISTBOX].d1 > 0 || demomode > 0)
	{
		ta[player].skill = dialog_playersetup[SKILL_LISTBOX].d1;

		// Random?
		if (ta[player].skill == 6 || demomode > 0)
			ta[player].skill = rand()%TANKSKILL_SNIPER + 1;

		// If no name has been given, assign one
		if (ustrlen(playername_string) <= 0)
		{
			switch(ta[player].skill)
			{
				case	TANKSKILL_STOOPID:
					sprintf(playername_string, "Stoopid %d", player+1);
					break;
				case	TANKSKILL_LOBBER:
					sprintf(playername_string, "Lobber %d", player+1);
					break;
				case	TANKSKILL_SHOOTER:
					sprintf(playername_string, "Shooter %d", player+1);
					break;
				case	TANKSKILL_GUNNER:
					sprintf(playername_string, "Gunner %d", player+1);
					break;
				case	TANKSKILL_SNIPER:
					sprintf(playername_string, "Sniper %d", player+1);
					break;
				default:
					sprintf(playername_string, "Unknown");
					break;
			}
		}

		// Setup bot character (inherit stats from another character)
		ta[player].SetupBotCharacter(ta[player].skill);
	}
	else
	{
		ta[player].skill = TANKSKILL_HUMAN;

		// If the ESC key was used to exit out of the above menu, then
		// dont run the tank selection menu
		done = 0;
		while (done==0)
		{
			PS_Refresh(player);
			done = PS_RunMouse(player);
			if (done==0)
				done = PS_RunKeyboard(player);
			
			ticker();

			Screen_Blit();
		}
	
		Screen_Blit();

		// Check for no name
		if (ustrlen(playername_string) <= 0)
		{ sprintf(playername_string, "Player %d", player+1); }
	}

	// Setup the user info
	ustrcpy(ta[player].name, playername_string);		// Copy the name

	// Free some memory
	destroy_bitmap(playersetup_buffer);
	destroy_bitmap(playersetup_background);
}


void PS_RunMultiplayerSetup(int player, int time)
{
	int				done=0;
	int				ret=0;

	// Always start on page 0 (normal characters)
	charpage = 0;

	// Reset player setup tics
	playersetuptics = 0;
	if (multiplayer && multiplayermode == MP_CLIENT)
		multiplayer_setup_time = time;

	// Create a bitmap for the dialogs
	playersetup_buffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(playersetup_buffer, TRANSCOLOR);
  //! PATCH BEGIN: Multiplayer fix AEC 12/29/05 
  // Create the color tinted background image. This only needs to be done once to 
  // reduce strain on CPU cycles 
  { 
  	//BITMAP   *backbmp = create_bitmap(SCREENWIDTH, SCREENHEIGHT); 
    //clear_to_color(backbmp, TRANSCOLOR); 

    playersetup_background = create_bitmap(SCREENWIDTH, SCREENHEIGHT); 
    clear_to_color(playersetup_background, TRANSCOLOR); 

    // First, start the background with the standard um...background 
    //draw_sprite(playersetup_background, (BITMAP *)maindata[PLAYERSETUP_BACKGROUND].dat, 0, 0); 

    // First, tint the plate BMP to a full color to its own bitmap (backbmp) 
    set_multiply_blender(ta[player].colorr, ta[player].colorg, ta[player].colorb, 0); 
    draw_lit_sprite(playersetup_background, (BITMAP *)maindata[PLAYERSETUP_BACKGROUND].dat, 0,0,255); 

    draw_sprite(playersetup_background, (BITMAP *)maindata[PLAYERSETUP_CHOOSECHAR].dat, 375, 16); 
    //solid_mode(); 
    
    //set_multiply_blender(ta[player].colorr, ta[player].colorg, ta[player].colorb, 0); 
    //draw_lit_sprite(playersetup_background, backbmp, 0, 0, 255); 

    //destroy_bitmap(backbmp); 
   } 
   //! PATCH END Multiplayer fix AEC 12/29/05 
   
	// Make sure we have transparent text drawing
	text_mode(-1);

	// Clear out the keyboard buffer
	clear_keybuf();

	// Gobal
	current_player_setup = player;
	config_active = true;

	// Clear out the name
	usprintf(playername_string, "");
	
	// Refresh the screen
	PS_Refresh(player);

	
	// When setting up a multiplayer game, the game master HAS to be a 
	// human player. So, in light of this, we have to disable the 
	// player class listbox and only allow the gamemaster to enter 
	// their name.
	
	//if (net_localplayer == player)
	if (multiplayermode == MP_CLIENT)
	{
		dialog_multiplayersetup[SKILL_LISTBOX].flags |= (D_DISABLED|D_HIDDEN);
		dialog_multiplayersetup[SKILL_LISTBOX].d1 = 0;
	}
	else
		dialog_multiplayersetup[SKILL_LISTBOX].flags &= ~(D_DISABLED|D_HIDDEN);
	
	


	// Run the dialog, and grab the return value
	ret = do_agup_dialog(dialog_multiplayersetup, playersetup_buffer, PS_Drawer);

	// Figure out if this is a human or computer player and set their skill
	if (dialog_multiplayersetup[SKILL_LISTBOX].d1 > 0)
	{
		ta[player].skill = dialog_multiplayersetup[SKILL_LISTBOX].d1;

		// Random?
		if (ta[player].skill == 6)
			ta[player].skill = rand()%TANKSKILL_SNIPER + 1;
	
		// If no name has been given, assign one
		if (ustrlen(playername_string) <= 0)
		{
			switch(ta[player].skill)
			{
				case	TANKSKILL_STOOPID:
					sprintf(playername_string, "Stoopid %d", player+1);
					break;
				case	TANKSKILL_LOBBER:
					sprintf(playername_string, "Lobber %d", player+1);
					break;
				case	TANKSKILL_SHOOTER:
					sprintf(playername_string, "Shooter %d", player+1);
					break;
				case	TANKSKILL_GUNNER:
					sprintf(playername_string, "Gunner %d", player+1);
					break;
				case	TANKSKILL_SNIPER:
					sprintf(playername_string, "Sniper %d", player+1);
					break;
				default:
					sprintf(playername_string, "Unknown");
					break;
			}
		}

		// Give the bot random stats from someone else
		ta[player].SetupBotCharacter(player);
	}
	else
	{
		//if (ta[player].exist > 0)
		{
			ta[player].skill = TANKSKILL_HUMAN;
		
			// Check for no name
			if (ustrlen(playername_string) <= 0)
			{ sprintf(playername_string, "Player %d", player+1); }
		}
	}
	
	// If the ESC key was used to exit out of the above menu, then
	// dont run the tank selection menu
	if (net_localplayer == player /*|| ta[player].skill != TANKSKILL_HUMAN*/ ||
		multiplayermode == MP_CLIENT)
	{
		done = 0;
		while (done==0)
		{
			PS_Refresh(player);
			done = PS_RunMouse(player);
			if (done==0)
				done = PS_RunKeyboard(player);
			ticker();
		
			Screen_Blit();
		}
	}

	// Do not allow the gamemaster to select a tank. If a slot is open for another
	// player to join then until someone joins, this tank is non-existant to the
	// engine.
	if (multiplayermode == MP_SERVER &&
		ta[player].skill == TANKSKILL_HUMAN &&
		player != net_localplayer)
	{
		ta[player].exist = 0;
		ta[player].health = 0;
	}
	else
	{
		// Setup the user info
		ustrcpy(ta[player].name, playername_string);		// Copy the name
	}


	// Free some memory & we're done
	destroy_bitmap(playersetup_buffer);
}


void	PS_ReClassDrawer(void)
{
	if (update_count <= 0)
		return;

	update_count=0;

	if (gamestate == GS_INGAME)
	{
		LoopDrawer(buffer);
	}

	// Draw the dialog
	AddRectangle(buffer, dialog_reclassplayersetup[0].x, 
		dialog_reclassplayersetup[0].y, 
		dialog_reclassplayersetup[0].w, 
		dialog_reclassplayersetup[0].h);

	draw_sprite(buffer, playersetup_buffer, 0, 0);

	Screen_Blit(true);
	gametics++;
	frame_count++;
}



void		PS_RunReClassFor(int player)
{
	int		ret;

	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	// This is considered a cheat.
	used_cheat = 1;

	// Reset player setup tics
	playersetuptics = 0;

	// Gobal
	current_player_setup = player;

	// Clear out the name
	usprintf(playername_string, ta[player].name);

	dialog_reclass[RECLASSSKILL_LISTBOX].d1 = ta[player].skill;
	

	// Run the dialog, and grab the return value
	ret = do_agup_dialog(dialog_reclass, playersetup_buffer, PS_ReClassDrawer);

	// Figure out if this is a human or computer player and set their skill
	if (dialog_reclass[RECLASSSKILL_LISTBOX].d1 > 0)
	{
		ta[player].skill = dialog_reclass[RECLASSSKILL_LISTBOX].d1;

		// Random?
		if (ta[player].skill == 6)
			ta[player].skill = rand()%TANKSKILL_SNIPER + 1;

		// If no name has been given, assign one
		if (ustrlen(playername_string) <= 0)
		{
			switch(ta[player].skill)
			{
				case	TANKSKILL_STOOPID:
					sprintf(playername_string, "Stoopid %d", player+1);
					break;
				case	TANKSKILL_LOBBER:
					sprintf(playername_string, "Lobber %d", player+1);
					break;
				case	TANKSKILL_SHOOTER:
					sprintf(playername_string, "Shooter %d", player+1);
					break;
				case	TANKSKILL_GUNNER:
					sprintf(playername_string, "Gunner %d", player+1);
					break;
				case	TANKSKILL_SNIPER:
					sprintf(playername_string, "Sniper %d", player+1);
					break;
				default:
					sprintf(playername_string, "Unknown");
					break;
			}
		}

		// Give the bot random stats from someone else
		ta[player].SetupBotCharacter(player);
	}
	else
	{
		ta[player].skill = TANKSKILL_HUMAN;

		// Check for no name
		if (ustrlen(playername_string) <= 0)
		{ sprintf(playername_string, "Player %d", player+1); }
	}

	// Moving the tank out of existance
	if (dialog_reclass[RECLASSSKILL_LISTBOX].d1 == 6 && ta[player].exist > 0)
	{
		if (numtanks() >= 2)
		{
			// Remove from current/future tank ques
			RemoveFromTurnQue(player);

			// Free the map spawn point array so other tanks can use this point
			currentlevel.posoccupied[ta[player].tankpos] = false;

			ta[player].exist = 0;
			ta[player].health = 0;
			ta[player].needstodie = 1;
			ta[player].lasthit = player;
			ustrcpy(ta[player].name, "");
			numplayers--;
		}
		return;
	}

	// Bringing a tank back from the dead?
	if (ta[player].exist <= 0 && dialog_reclass[RECLASSSKILL_LISTBOX].d1 != 4)
	{
		numplayers++;

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
		ta[player].GiveMoney(MONEY_STARTCASH-2000);
		
		// Assign a random character
		ta[player].character = rand()%NUM_NORMALCHARACTERS;

		ta[player].init(player, rand()%(SCREENWIDTH/2),0, 100,15,10,LIGHTGREEN);
		
		// This player will be brought back on the next round
		ta[player].health = 0;

		// Find a team if we're in teamplay mode
		if (teamplay->value > 0)
			ta[player].AutoAssignTeam();

		AddNextTurnQue(player);
	}

	// Setup the user info
	ustrcpy(ta[player].name, playername_string);		// Copy the name

	// Start the reclass dialog again
	do_agup_dialog(dialog_reclassplayersetup, playersetup_buffer, PS_ReClassDrawer);

}


void		PS_RunReclass(void)
{
	int				done=0;
	int				ret=0;

	int				oldnumtanks;

	// Number of tanks in the game before we reclass
	oldnumtanks = numtanks();

	// Create a bitmap for the dialogs
	playersetup_buffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(playersetup_buffer, TRANSCOLOR);

	// Make sure we have transparent text drawing
	text_mode(-1);

	// Clear out the keyboard buffer
	clear_keybuf();

	// Run the dialog, and grab the return value
	ret = do_agup_dialog(dialog_reclassplayersetup, playersetup_buffer, PS_ReClassDrawer);

	// Free some memory
	destroy_bitmap(playersetup_buffer);

	// If the number of tanks in the game is different we need
	// to review and visually kill those who got deleted
	if (ta[tankturn].exist <= 0)
		KillTanks(true);
}



void	AddBot_Realtime(int	skill, char	*name)
{
	int		player=-1;
	int		i;
	int		turnque = -1;
	char	playername_string[64];

	if (gamestate != GS_INGAME)
		return;

	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	// Flag this as cheating
	used_cheat = 1;

	// Cannot spawn bots on the client side
	if (multiplayer && multiplayermode == MP_CLIENT)
	{ Printf("Only a server can add bots\n"); return; }
	
	// First, find an open slot
	player = FindNewClient();

	if (player < 0)
	{
		Printf("No player slots left to add a bot\n");
		return;
	}


	// Add the bot into the current que
	for (i=0; i<MAX_TANKS; i++)
	{
		if (tankturnque[i] < 0)
		{
			tankturnque[i] = player;
			turnque = i;
			break;
		}
	}

	if (turnque < 0)
	{
		Printf("No queue slots for tank!\n");
		return;
	}

	numplayers++;

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
	ta[player].GiveMoney(cashstart->value);
		
	// Assign a random character
	ta[player].character = rand()%NUM_NORMALCHARACTERS;

	ta[player].init(player, rand()%(SCREENWIDTH/2),0, 100,15,10,LIGHTGREEN);
	ta[player].skill = skill;
		
	// This player will be brought back on the next round
	ta[player].health = 100;

	// Find a team if we're in teamplay mode
	if (teamplay->value > 0)
		ta[player].AutoAssignTeam();
	
	// Come up with a name for the bot (if there already isn't one)
	if (!name || ustrlen(name) <= 0)
	{
		switch(ta[player].skill)
		{
			case	TANKSKILL_STOOPID:
					sprintf(playername_string, "Stoopid %d", player+1);
					break;
				case	TANKSKILL_LOBBER:
					sprintf(playername_string, "Lobber %d", player+1);
					break;
				case	TANKSKILL_SHOOTER:
					sprintf(playername_string, "Shooter %d", player+1);
					break;
				case	TANKSKILL_GUNNER:
					sprintf(playername_string, "Gunner %d", player+1);
					break;
				case	TANKSKILL_SNIPER:
					sprintf(playername_string, "Sniper %d", player+1);
					break;
				default:
					sprintf(playername_string, "Unknown");
					break;
		}

		ustrcpy(ta[player].name, playername_string);

		// Give the bot random stats from someone else
		ta[player].SetupBotCharacter(player);
	}
	else
		ustrcpy(ta[player].name, name);

	// Find a spawn point
	ta[player].FindMapPosition();

	ta[player].Onground(ground.bmp);
	
	ta[player].DefaultAngle();
	
	ta[player].SetHEB(characters[ta[player].character].attrib_h, 
			characters[ta[player].character].attrib_e, 
			characters[ta[player].character].attrib_b);

	// Active turn?
	if (tankturn < 0 || ta[tankturn].exist <= 0)
		{
		advanceturn(true);	// bot on!! We're ready to play
		}

	// Announce
	if (teamplay->value <= 0)
		Printf("Bot '%s' has joined the game.\n", ta[player].name);
	else
	{
		if (ta[player].team == TANKTEAM_RED)
			Printf("Bot '%s' has joined the RED team.\n", ta[player].name);
		else
			Printf("Bot '%s' has joined the BLUE team.\n", ta[player].name);
	}

	// Update all other clients if we're the server
	if (multiplayer && multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_PLAYERINFO);
		BCast_WriteByte(player);
		BCast_WriteString(ta[player].name);
		BCast_WriteByte(ta[player].character);
		BCast_WriteByte(ta[player].skill);

		// Send color & team
		BCast_WriteByte(ta[player].team);
		BCast_WriteByte(ta[player].colorr);
		BCast_WriteByte(ta[player].colorg);
		BCast_WriteByte(ta[player].colorb);

		// Send x,y coords
		BCast_WriteShort(ta[player].x);
		BCast_WriteShort(ta[player].y);
		BCast_WriteShort(ta[player].health);
		BCast_WriteShort(ta[player].deg);
		BCast_WriteByte(ta[player].spectator);

		// Send fuel info
		BCast_WriteLong(ta[player].startfuel);
		BCast_WriteLong(ta[player].specials[fuel].amount);
	}

}



//
// switchteam - does exactly what you would think. Only works for multiplayer though
//

void	switchteam(void)
{
	// No more menu
	menuactive = false;

	// Can only switch teams in multiplayer
	if (!multiplayer)
		return;

	if (tankturn != net_localplayer)
	{
		G_Printf("You can only switch teams when it is your turn.");
		return;
	}

	if (multiplayermode == MP_CLIENT)
	{
		localbuf->WriteByte(CTOS_SWITCHTEAM);

		// Dont allow the local player to fire anymore
		ta[net_localplayer].specialflags |= TANKSPECIAL_MADESHOT;
	}
	else
	{
		// Only the local player can switch teams (bots cannot)
		if (tankturn == net_localplayer)
			ta[tankturn].SwitchTeam();
	}
}
