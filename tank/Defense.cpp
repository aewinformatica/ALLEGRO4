#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"


menuitem_t		defenseuse_items[NUM_DEFENSE+NUM_SPECIALS];
int				num_defenseuse_items;

#define			POWER_COLOR		makecol(100,255,100)
#define			SHIELD_COLOR	makecol(100,100,255)

BITMAP			*defense_buffer;

// Global varible whom we're doing the setup dialog for
int		playerdefense_num;

void	D_Drawer(void);
void	ConventionalShieldDrawer(int	player);
void	SuperConventionalDrawer(int	player);
void	ShieldPowerUp(int	player, int refnum);
void	BatteryPowerUp(int	player, int refnum);
void	FloatationActivate(int player, int refnum);

int		currentplayer_reserve_energy;
int		currentplayer_reserve_power;

// Define stuff globally
defensetype_t	defenses[NUM_DEFENSE] = {


	{
		(char *)"Battery Charge",			// Name
		10,									// Energy (+10)
		0,0,0,								// HEB Defense
		0,									// Width
		0,									// Height
		3500,								// Cost
		5,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		BatteryPowerUp,						// Activate function
	},

	{
		(char *)"Parachutes",				// Name
		0,									// Energy
		0,0,0,								// HEB Defense
		0,									// Width
		0,									// Height
		5000,								// Cost
		5,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		NULL,								// Activate function
	},

	{
		(char *)"Flotation Device",		// Name was "Floatation Device", 
		0,									// Energy
		0,0,0,							// HEB Defense
		0,									// Width
		0,									// Height
		7500,								// Cost
		1,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		FloatationActivate,					// Activate function
	},

	{
		(char *)"Conventional Shield",		// Name
		100,								// Total shield energy
		0,0,0,								// (HEB)		
		43,									// Width
		43,									// Height
		4000,								// Cost
		1,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		ShieldPowerUp,						// Activate function
	},
		
	{
		(char *)"Electric Shield",			// Name
		100,								// Total shield energy
		1,5,1,								// (HEB)		
		30,									// Width
		30,									// Height
		6000,								// Cost
		1,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		ShieldPowerUp,						// Activate function
	},

	{
		(char *)"Fire Shield",				// Name
		100,								// Total shield energy
		5,1,1,								// (HEB)
		30,									// Width
		30,									// Height
		6000,								// Cost
		1,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		ShieldPowerUp,						// Activate function
	},

	{
		(char *)"Ballistic Shield",			// Name
		100,								// Total shield energy
		1,1,5,								// (HEB)
		30,									// Width
		30,									// Height
		6000,								// Cost
		1,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		ShieldPowerUp,						// Activate function
	},

	{
		(char *)"Super Conventional Shield",// Name
		150,								// Total shield energy
		2,2,2,								// (HEB)		
		45,									// Width
		45,									// Height
		10000,								// Cost
		1,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		ShieldPowerUp,						// Activate function
	},

	{
		(char *)"N.I.V. Shield",			// Name
		100,								// Total shield energy
		9,9,9,								// (HEB)		
		27,									// Width
		24,									// Height
		20000,								// Cost
		1,									// Per item
		NULL,								// Shot hit function
		NULL,								// Shield Drawer
		ShieldPowerUp,						// Activate function
	},
};

int UseDefenseItem(int	b)
{
	int		i=defenseuse_items[dialog_playerdefense[1].d1].refnum;

	// If we dont have any, don't give any....
	if (ta[playerdefense_num].defense[i].amount < 0 || num_defenseuse_items == 0)
		return D_O_K;

	// Activate via whatever function specified
	if (defenses[i].activateproc)
		defenses[i].activateproc(playerdefense_num, i);

	// Keep track on how many times this item was used
	ta[playerdefense_num].defenseused[i]++;

	// Redraw the dialog
	broadcast_dialog_message(MSG_DRAW, 0);

	// Let all other clients know what we did
	if (multiplayer)
	{
		if (multiplayermode == MP_CLIENT)
		{
			localbuf->WriteByte(CTOS_ACTIVATEDEFENSE);
			localbuf->WriteByte(i);
		}
		else
		{
			BCast_WriteByte(STOC_ACTIVATEDEFENSE);
			BCast_WriteByte(playerdefense_num);
			BCast_WriteByte(i);
		}
	}

	return D_O_K;
}

int UseSpecialItem(int	b)
{
	int		i=defenseuse_items[dialog_playerdefense[1].d1].refnum;

	// If we dont have any, don't give any....
	if (ta[playerdefense_num].specials[i].amount < 0 || num_defenseuse_items == 0)
		return D_O_K;

	// Activate via whatever function specified
	if (specials[i].activateproc)
		specials[i].activateproc(playerdefense_num, i);

	// Keep track on how many times this item was used
	ta[playerdefense_num].specialsused[i]++;

	// Redraw the dialog
	broadcast_dialog_message(MSG_DRAW, 0);

	// Let all other clients know what we did
	if (multiplayer)
	{
		if (multiplayermode == MP_CLIENT)
		{
			localbuf->WriteByte(CTOS_ACTIVATESPECIAL);
			localbuf->WriteByte(i);
		}
		else
		{
			BCast_WriteByte(STOC_ACTIVATESPECIAL);
			BCast_WriteByte(playerdefense_num);
			BCast_WriteByte(i);
		}
	}

	return D_O_K;
}

int	UseItem(int b)
{
	int		i=dialog_playerdefense[1].d1;

	if (defenseuse_items[i].parent == BUYSTUFF_DEFENSE)
		return UseDefenseItem(b);
	else
		return UseSpecialItem(b);
}


//
// itemlist_getter() - this isnt used anymore
//

char *defense_list_getter(int index, int *list_size)
{
	if (index < 0) 
	{
		*list_size = num_defenseuse_items;
		return NULL;
	}
	else
	{ return defenseuse_items[index].name; }

	*list_size = 0;
	return NULL;

}



//
// CalcDefenseList2 - Rebuilds our menu array
//

void CalcDefenseList2(void)
{
	int		i;
	int		ref=0;

	memset(defenseuse_items, 0, sizeof(menuitem_t));

	for (i=0; i<NUM_DEFENSE; i++)
	{
		if (defenses[i].numrecieve <= 0)
			continue;

		if (ta[playerdefense_num].defense[i].amount <= 0)
			continue;

		ustrcpy(defenseuse_items[ref].name, defenses[i].name);
		defenseuse_items[ref].cost = defenses[i].cost;
		defenseuse_items[ref].numrecieve = defenses[i].numrecieve;
		defenseuse_items[ref].refnum = i;
		defenseuse_items[ref].num = ta[playerdefense_num].defense[i].amount;
		defenseuse_items[ref].parent = BUYSTUFF_DEFENSE;

		// Up it up
		ref++;
	}


	for (i=0; i<NUM_SPECIALS; i++)
	{
		if (specials[i].numrecieve <= 0 || !(specials[i].flags & SPECIAL_DEFENSEBOX))
			continue;

		if (ta[playerdefense_num].specials[i].amount <= 0)
			continue;

		ustrcpy(defenseuse_items[ref].name, specials[i].name);
		defenseuse_items[ref].cost = specials[i].cost;
		defenseuse_items[ref].numrecieve = specials[i].numrecieve;
		defenseuse_items[ref].refnum = i;
		defenseuse_items[ref].num = ta[playerdefense_num].specials[i].amount;
		defenseuse_items[ref].parent = BUYSTUFF_SPECIAL;

		// Up it up
		ref++;
	}

	num_defenseuse_items = ref;
}




//
// d_buystuff_list_proc - New routine for displaying information
//						  about weapons and alot of other stuff
// 

int d_defense_list_proc(int msg, DIALOG *d, int c)
{
	FONT	*orginal_font;
	int ret;

	// Make a backup copy of the orginal font, we'll have to restore it
	orginal_font = font;

	CalcDefenseList2();

	// Pull up our new font;
	font = (FONT *)maindata[VER10].dat;

	ret = d_abeos_defenselist_proc(msg, d, c);

	// Restore the font
	font = orginal_font;

	return ret;
}

int		d_defensebutton_proc(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		D_Drawer();

		CheckJoyStick();

		if (usejoystick)
		{
			poll_joystick();

			if (IsJoyButtonDn(0, 8) || IsJoyButtonDn(0, 2))
				return D_EXIT;

			if (IsJoyButtonUp(0, 0))
			{
				UseItem(0);
				return D_O_K;
			}

			// Going up
			if (nojoytime <= 0 && 
				IsJoyStickMoved(0, 0, 1, 30) && 
				(joy[0].stick[0].axis[1].pos) < 0)
			{
				dialog_playerdefense[1].d1--;
				if (dialog_playerdefense[1].d1 <= 0)
					dialog_playerdefense[1].d1=0;

				nojoytime = 5;
				broadcast_dialog_message(MSG_DRAW, 0);
				//return D_REDRAW;
			}

			// Going down
			else if (nojoytime <= 0 && 
				IsJoyStickMoved(0, 0, 1, 30) && 
				(joy[0].stick[0].axis[1].pos) > 0)
			{
				dialog_playerdefense[1].d1++;

				if (dialog_playerdefense[1].d1 > num_defenseuse_items)
					dialog_playerdefense[1].d1=num_defenseuse_items;

				nojoytime = 5;
				broadcast_dialog_message(MSG_DRAW, 0);

				//return D_REDRAW;
			}
		}

		return D_O_K;
	}

	return d_agup_button_proc(msg, d, c);
}


DIALOG dialog_playerdefense[] =
{
   // Misc buttons
   { d_agup_window_proc,				 178,	  98,   426,  300,  255,  0,    0,    0,	   0,		0,    (char *)"Player Defense Setup" },
   { d_defense_list_proc,				 192,     220,  403,  146,  255,  0,    0,    0,       0,		0,    (char *)defense_list_getter },
   //{ d_text_proc,						 194,	  132,  0,    0,    255,  0,    0,    0,       0,       0,    "Power" },
   //{ d_text_proc,						 194,	  182,  0,    0,    255,  0,    0,    0,       0,       0,    "Energy" },

   { d_defensebutton_proc,				 517,	  367,  73,   19,   255,  0,    0,    D_EXIT,  0,		0,    (char *)"Done" },   
   { d_agup_push_proc,					 191,     367,  73,   19,   255,  0,    0,    0,		0,		0,    (char *)"Use",	0,	(void *)UseItem },
   { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }
};



//
// StartDefenseBox - Starts the defense setup dialog for a specified player
//

void D_Drawer(void)
{
	float	barwidth = 266;
	float	fillwidth_power, fillwidth_energy;
	float	percent_shield;
	float	percent_power;

	int		x = dialog_playerdefense[0].x;
	int		y = dialog_playerdefense[0].y;

	char	str[64];

	if (update_count <= 0)
		return;

	update_count=0;

	if (gamestate == GS_INGAME)
		InGame_Runner();

	// Draw the dialog
	AddRectangle(buffer, x, y, dialog_playerdefense[0].w, dialog_playerdefense[0].h);
	//blit(defense_buffer, buffer, x,y, x, y, dialog_playerdefense[0].w, dialog_playerdefense[0].h);
	draw_sprite(buffer, defense_buffer, 0, 0);

	// Make some calculations first
	percent_shield = (float )(((float)ta[playerdefense_num].energyleft/(float)defenses[ta[playerdefense_num].activedefense].energy)*100);
	percent_power = (float)(((float)ta[playerdefense_num].health/(float)ta[playerdefense_num].starthealth)*100);

	text_mode(getpixel(buffer, x+14,y+34));
	//text_mode(lex_skin.dialog.bgcol);

	// Textout, our friend  =D
	//sprintf(str, "Power (%d%%)", (int)percent_power);
	sprintf(str, "Power (%d/%d)", (int)ta[playerdefense_num].health, (int)ta[playerdefense_num].starthealth);
	textout(buffer, font, str, x+14, y+34, 255);

	// Display the word "Sheid" and what type of shield we're using
	if (ta[playerdefense_num].energyleft <= 0)
		sprintf(str, "Shield");
	else
		{
//Printf("playerdefense_num %d\n", playerdefense_num);	/// Degauss Getting an "Unhandled exception" near here!!
if(playerdefense_num <          0) playerdefense_num = 0;
if(playerdefense_num >= MAX_TANKS) playerdefense_num = 0;
//Printf("ta[playerdefense_num].activedefense [%d]\n", ta[playerdefense_num].activedefense);	/// Degauss
if(ta[playerdefense_num].activedefense <            0) ta[playerdefense_num].activedefense = 0;
if(ta[playerdefense_num].activedefense >= NUM_DEFENSE) ta[playerdefense_num].activedefense = 0;

		sprintf(str, "Shield (%s - %d/%d)", 
		defenses[ta[playerdefense_num].activedefense].name, 
		(int)ta[playerdefense_num].energyleft,
		defenses[ta[playerdefense_num].activedefense].energy);
		}
	textout(buffer, font, str, x+14, y+84, 255);

	// Calculate how many pixels we need to use
	fillwidth_power = (float )(barwidth * (ta[playerdefense_num].health/ta[playerdefense_num].starthealth));
	fillwidth_energy = (float )(barwidth * ((float)ta[playerdefense_num].energyleft/(float)defenses[ta[playerdefense_num].activedefense].energy));

	// Fill up the power and shield indicators
	rectfill(buffer, (int)((x+16)+1), (int)((y+54)+1), (int)((x+16)+(fillwidth_power)+1), (int)(y+65-1), POWER_COLOR);
	if (ta[playerdefense_num].energyleft)
		rectfill(buffer, (int)((x+16)+1), (int)((y+104)+1), (int)((x+16)+(fillwidth_energy)+1), (int)(y+115-1), SHIELD_COLOR);

	text_mode(-1);
	
	
	// Draw the black border
	rect(buffer, (x+16), (y+54),  (x+283), (y+65), makecol(0,0,0));
	rect(buffer, (x+16), (y+104), (x+283), (y+115), makecol(0,0,0));

	// Check our reserve energy
	if (currentplayer_reserve_energy > 0)
	{
		ta[playerdefense_num].energyleft+=6;
		currentplayer_reserve_energy-=6;

		// Make sure we dont go over
		if (ta[playerdefense_num].energyleft > defenses[ta[playerdefense_num].activedefense].energy)
			ta[playerdefense_num].energyleft = defenses[ta[playerdefense_num].activedefense].energy;

	}


	// Check our reserve power
	if (currentplayer_reserve_power > 0)
	{
		ta[playerdefense_num].health++;
		currentplayer_reserve_power--;

		if (ta[playerdefense_num].health >= 100)
			ta[playerdefense_num].health = 100;
	}

	//show_mouse(buffer);
	ticker();
	Screen_Blit(true);
	

	frame_count++;
	gametics++;
}

void StartDefenseBox(int	player, int deftype)
{
	// Setup the defense buffer
	defense_buffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(defense_buffer, TRANSCOLOR);
	
	// Define our global varible
	playerdefense_num = player;

	// Clear the keyboard buffer
	clear_keybuf();

	// Set new tank-state
	ta[playerdefense_num].tankstate = deftype;

	// Multiplayer? Other clients need to know about this
	// state change
	if (multiplayer)
	{
		if (multiplayermode == MP_SERVER)
			SV_SendLocalState(playerdefense_num);
		else
			CL_SendLocalState();
	}

	// Launch the dialog
	do_agup_dialog(dialog_playerdefense, defense_buffer, D_Drawer);

	// Free memory
	destroy_bitmap(defense_buffer);

	// Set tankstate back
	ta[playerdefense_num].tankstate = TANKSTATE_PLAYING;

	if (multiplayer)
	{
		if (multiplayermode == MP_SERVER)
			SV_SendLocalState(playerdefense_num);
		else
			CL_SendLocalState();
	}

	// Refresh the entire screen after doing this
	ground.needsupdate = true;
}




//
// GiveDefense - power up!
//
void GiveDefense(int	player, int ammotype, int	amount)
{
	ta[player].defense[ammotype].amount += amount;
}

//
// ShieldPowerUp - power up! (SO FAR THIS FUNCTION ONLY APPLIES FOR SHIELDS)
//

void ShieldPowerUp(int	player, int	deftype)
{
	int		i = 0;

	// Make sure the player even HAS enough
	if (ta[player].defense[deftype].amount <= 0 && !multiplayer)
		return;

	//playerdefense_num = player; // ???

	// If the current shield is at full power, then it will be placed back
	// into the players inventory. However, if not, the shield is reset and
	// another is used
	if (ta[player].energyleft >= defenses[deftype].energy)
		ta[player].defense[ta[player].activedefense].amount++;

	ta[player].energyleft = 0;

	// Take one from inventory
	ta[player].defense[deftype].amount--;

	// Set defense properties
	ta[player].activedefense = deftype;

	// Remove the old sprite before adding another
	if (ta[player].shield)
		ta[player].shield->Kill();

	// Set the tank shield sprite
	if (deftype == electric_shield)
	{
		ta[player].shield = G_SpawnSprite(ta[player].x, ta[player].y, S_SHIELD_ELECTRIC_1, (MF_NORMAL | MF_CENTERCOORD | MF_NOGRAVITY | MF_FOLLOWTANK));
		ta[player].StartSound(SHIELD_ENERGY_POWERUP);
	}
	else if (deftype == fire_shield)
	{
		ta[player].shield = G_SpawnSprite(ta[player].x, ta[player].y, S_SHIELD_FIRE_1, (MF_NORMAL | MF_CENTERCOORD | MF_NOGRAVITY | MF_FOLLOWTANK));
		ta[player].StartSound(SHIELD_HEAT_POWERUP);
	}
	else if (deftype == ballistic_shield)
	{
		ta[player].shield = G_SpawnSprite(ta[player].x, ta[player].y, S_SHIELD_BALLISTIC_1, (MF_NORMAL | MF_CENTERCOORD | MF_NOGRAVITY | MF_FOLLOWTANK));
		ta[player].StartSound(SHIELD_BALLISTIC_POWERUP);
	}
	else if (deftype == conventional_shield)
	{
		// Play sound
		ta[player].shield = G_SpawnSprite(ta[player].x, ta[player].y, S_SHIELD_CONVENTIONAL, (MF_NORMAL | MF_CENTERCOORD | MF_NOGRAVITY | MF_FOLLOWTANK));
		ta[player].StartSound(TANK_SHIELDPOWERUP);
	}
	else if (deftype == superconventional_shield)
	{
		// Play sound
		ta[player].shield = G_SpawnSprite(ta[player].x, ta[player].y, S_SHIELD_SUPERCONVENTIONAL, (MF_NORMAL | MF_CENTERCOORD | MF_NOGRAVITY | MF_FOLLOWTANK));
		ta[player].StartSound(SHIELD_SUPERCONVENTIONALPOWERUP);
	}
	else if (deftype == niv_shield)
	{
		// Play sound
		ta[player].shield = G_SpawnSprite(ta[player].x, ta[player].y, S_SHIELD_NIV_B1, (MF_NORMAL | MF_CENTERCOORD | MF_NOGRAVITY | MF_FOLLOWTANK));

		// Shield stats are adjusted to give the player a (9,9,9) in all categories
		ta[player].shieldh = 9-ta[player].h;
		ta[player].shielde = 9-ta[player].e;
		ta[player].shieldb = 9-ta[player].b;
	}
	else
	{
		// Play sound
		ta[player].StartSound(TANK_SHIELDPOWERUP);
	}

	// Let the sprite know who it belongs to
	if (ta[player].shield)
		ta[player].shield->tankfrom = player;


	// Setup shield defense classes
	if (deftype != niv_shield)
		ta[player].SetShieldHEB(defenses[deftype].h, defenses[deftype].e, defenses[deftype].b);

	if (multiplayer)
		ta[player].energyleft = defenses[deftype].energy;
	else
	{
		if (ta[player].skill == TANKSKILL_HUMAN)
			currentplayer_reserve_energy = defenses[deftype].energy;
		else
			ta[player].energyleft = defenses[deftype].energy;
	}
}


//
// BatteryPowerUp - Add +10% health back to our tank
//

void BatteryPowerUp(int player, int deftype)
{
	int		textx = (ta[player].x-(ta[player].sizex/2))-10;
	int		texty = (ta[player].y-(ta[player].sizey/2))-20;

	//G_Printf("battery power up!");

	// Make sure the player even HAS enough
	if(ta[player].defense[deftype].amount <= 0 && !multiplayer)
		return;

	// We could be mean and waste their battery, but how many
	// times has this happened to you in Scorched Earth? 
	if(ta[player].health >= ta[player].starthealth)	//	already fully charged
		{
		ta[player].StartSound(FIRETOAD_BOUNCE  );	//	don't have a "your fully charged" sound
		ta[player].StartSound(METALHIT2);	//	alternative sound 
		return;
		}

	//G_Printf("1!");

	//if ((ta[player].health+currentplayer_reserve_power) >= 100 && 
	//	ta[player].skill == TANKSKILL_HUMAN) // Check reserve power too
	//	return;

	//G_Printf("2!");

	// Play sound
	ta[player].StartSound(TANK_BATTERYCHARGEUP);

	// Take one from inventory...
	ta[player].defense[deftype].amount--;

	// ...in exchange for some (much needed?) energy
	ta[player].health += defenses[deftype].energy;

	// Cheap hack, dont go above 100
	if(ta[player].health >= ta[player].starthealth)
		ta[player].health = ta[player].starthealth;

	// Draw some text
	SpawnGameText("+10", 1.5*TICRATE, textx+(rand()%(10*2)), texty+(rand()%(20*2)), 0, 0, makecol(255,255,192), (FONT *)maindata[TINY8].dat, (TEXT_INVERSEGRAVITY | TEXT_FADEOUT));

}

//
// DefenseDrawer - Draws the shield and does all the special effects for the tanks
//

void DefenseDrawer(int	player)
{
	int			defnum = ta[player].activedefense;
	
	// No energy left!
	if (ta[player].energyleft <= 0)
		return;

	if (defenses[defnum].drawerproc)
		defenses[defnum].drawerproc(player);
}




////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
/////////////////  SHIELD DRAWING ROUTINES  ////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////




void	ConventionalShieldDrawer(int	player)
{
	BITMAP		*shield=NULL;
	int			defnum = ta[player].activedefense;
	int			x,y,radius;
	float		blender;

	shield = create_bitmap(defenses[defnum].width, defenses[defnum].height);
	
	// Theres a problem
	if (!shield)
		return;

	radius = (shield->w/2)-1;

	clear_to_color(shield, TRANSCOLOR);

	// Todo: customize shield colors and density
	circlefill(shield, (shield->w/2), (shield->h/2), radius, makecol(255,255,255));
	circlefill(shield, (shield->w/2), (shield->h/2), radius-2, TRANSCOLOR);
	
	x = (int)ta[player].x-(shield->w/2);
	y = (int)ta[player].y-(shield->h/2);

	blender = (float)255*((float)ta[player].energyleft / (float)defenses[defnum].energy);

	// Check hack, stupid 0s
	if (ta[player].energyleft == defenses[defnum].energy)
		blender = 255;

	set_trans_blender(0, 0, 0, 0);
	draw_lit_sprite(buffer, shield, x, y, (int)(255-blender));

	destroy_bitmap(shield);
}


void	SuperConventionalDrawer(int	player)
{
	BITMAP		*shield;
	int			defnum = ta[player].activedefense;
	int			x,y,radius;
	float		blender;

	shield = create_bitmap(defenses[defnum].width, defenses[defnum].height);
	radius = (shield->w/2)-1;

	clear_to_color(shield, TRANSCOLOR);

	// Todo: customize shield colors and density
	circlefill(shield, (shield->w/2), (shield->h/2), radius, makecol(247,255,207));
	circlefill(shield, (shield->w/2), (shield->h/2), radius-3, TRANSCOLOR);
	
	x = (int)ta[player].x-(shield->w/2);
	y = (int)ta[player].y-(shield->h/2);

	blender = (float)255*((float)ta[player].energyleft / (float)defenses[defnum].energy);

	// Check hack, stupid 0s
	if (ta[player].energyleft == defenses[defnum].energy)
		blender = 255;

	set_trans_blender(0, 0, 0, 0);
	draw_lit_sprite(buffer, shield, x, y, (int)(255-blender));

	destroy_bitmap(shield);
}


/*void	MediumShieldDrawer(int	player)
{
	BITMAP		*shield;
	int			defnum = ta[player].activedefense;
	int			x,y,radius;
	float		blender;

	shield = create_bitmap(defenses[defnum].width, defenses[defnum].height);
	radius = (shield->w/2)-1;

	clear_to_color(shield, TRANSCOLOR);

	// Todo: customize shield colors and density
	circlefill(shield, (shield->w/2), (shield->h/2), radius, makecol(0,255,0));
	circlefill(shield, (shield->w/2), (shield->h/2), radius-2, TRANSCOLOR);
	
	x = (int)ta[player].x-(shield->w/2);
	y = (int)ta[player].y-(shield->h/2);

	blender = (float)255*((float)ta[player].energyleft / (float)defenses[defnum].energy);

	// Check hack, stupid 0s
	if (ta[player].energyleft == defenses[defnum].energy)
		blender = 255;

	set_trans_blender(0, 0, 0, 0);
	draw_lit_sprite(buffer, shield, x, y, (int)(255-blender));

	destroy_bitmap(shield);
}

void	HeavyShieldDrawer(int	player)
{
	BITMAP		*shield;
	int			defnum = ta[player].activedefense;
	int			x,y,radius;
	float		blender;

	shield = create_bitmap(defenses[defnum].width, defenses[defnum].height);
	radius = (shield->w/2)-1;

	clear_to_color(shield, TRANSCOLOR);

	// Todo: customize shield colors and density
	circlefill(shield, (shield->w/2), (shield->h/2), radius, makecol(255,255,0));
	circlefill(shield, (shield->w/2), (shield->h/2), radius-2, TRANSCOLOR);
	
	x = (int)ta[player].x-(shield->w/2);
	y = (int)ta[player].y-(shield->h/2);

	blender = (float)255*((float)ta[player].energyleft / (float)defenses[defnum].energy);

	// Check hack, stupid 0s
	if (ta[player].energyleft == defenses[defnum].energy)
		blender = 255;

	set_trans_blender(0, 0, 0, 0);
	draw_lit_sprite(buffer, shield, x, y, (int)(255-blender));

	destroy_bitmap(shield);
}*/



void		FloatationActivate(int player, int refnum)
{
	if (ta[player].flags & TANKFLAG_PARACHUTE)
		return;

	if (ta[player].flags & TANKFLAG_FLOATATION)
		return;

	if (!ta[player].underwater)
		return;

	ta[player].flags |= TANKFLAG_FLOATATION;
	ta[player].defense[refnum].amount--;
}
