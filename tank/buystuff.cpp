#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"

int			buystuff_state;

// selection_color - Everytime the player clicks to buy or sell
// an item, this varible is reset and counted back up to 255.
// When the menus are drawn, it resets this value which creates a nice
// pulsing color kinda deal which indicates the item has been actived.
int	buystuff_selection_color=255;
int	buystuff_selection_x;
int buystuff_selection_y;
int	buystuff_selection_w;
int buystuff_selection_h;


void			BuyStuff_Drawer(void);

int				current_buystuff_player;

// Menu item varibles
menuitem_t		weapons_items[NUM_PROJECTILES];
int				num_defense_items;

menuitem_t		defense_items[NUM_DEFENSE];
int				num_weapons_items;

menuitem_t		special_items[NUM_SPECIALS];
int				num_special_items;

menuitem_t		total_items[MAX_SELL_ITEMS];
int				num_total_items;

extern  DIALOG	dialog_buystuff[];


int BuyItem		(int i);
int SellItem	(int i);

//
// SelectionPulse - Setup all the varibles needed to make a blue
// bar pulse in the menu
//

void SelectionPulse(int x, int y, int w, int h)
{
	if (buystuff_state == BUYSTUFF_SELLSTUFF)
		buystuff_selection_color = 0;		// pure red
	else
		buystuff_selection_color = 128;

	buystuff_selection_x = x;
	buystuff_selection_y = y;
	buystuff_selection_w = w;
	buystuff_selection_h = h;
}

//
// CalcWeaponsList - Recalculates the weapons list accordinging
// to what the player can afford.
// 

void CalcWeaponsList(void)
{
	int		i;
	int		ref=0;

	memset(weapons_items, 0, sizeof(menuitem_t));

	for (i=0; i<NUM_PROJECTILES; i++)
	{
		if (projectiles[i].numrecieve <= 0 || (projectiles[i].flags & MISSILE_NOLIST))
			continue;

		// Keep an eye out for character specific weapons
		if (projectiles[i].thischaracter >= 0)
		{
			if (ta[current_buystuff_player].character != projectiles[i].thischaracter)
				continue;
		}

		//if (ta[current_buystuff_player].money < projectiles[i].cost)
		//	continue;

		ustrcpy(weapons_items[ref].name, projectiles[i].name);
		weapons_items[ref].cost = projectiles[i].cost;
		weapons_items[ref].numrecieve = projectiles[i].numrecieve;
		weapons_items[ref].refnum = i;
		weapons_items[ref].num = ta[current_buystuff_player].weapons[i].ammo;
		weapons_items[ref].parent = BUYSTUFF_WEAPONS;
		weapons_items[ref].flags = projectiles[i].flags;


		// Up it up
		ref++;
	}

	num_weapons_items = ref;
}

//
// CalcDefenseList - Recalculates the defense list accordinging
// to what the player can afford.
// 

void CalcDefenseList(void)
{
	int		i;
	int		ref=0;

	memset(defense_items, 0, sizeof(menuitem_t));

	for (i=0; i<NUM_DEFENSE; i++)
	{
		if (defenses[i].numrecieve <= 0)
			continue;

		//if (ta[current_buystuff_player].money < defenses[i].cost)
		//	continue;

		ustrcpy(defense_items[ref].name, defenses[i].name);
		defense_items[ref].cost = defenses[i].cost;
		defense_items[ref].numrecieve = defenses[i].numrecieve;
		defense_items[ref].refnum = i;
		defense_items[ref].num = ta[current_buystuff_player].defense[i].amount;
		defense_items[ref].parent = BUYSTUFF_DEFENSE;

		// Up it up
		ref++;
	}

	num_defense_items = ref;
}


//
// CalcSpecialList - Recalculates the special category list accordinging
// to what the player can afford.
// 

void CalcSpecialList(void)
{
	int		i;
	int		ref=0;

	memset(special_items, 0, sizeof(menuitem_t));

	for (i=0; i<NUM_SPECIALS; i++)
	{
		if (specials[i].numrecieve <= 0)
			continue;

		//if (ta[current_buystuff_player].money < specials[i].cost)
		//	continue;

		ustrcpy(special_items[ref].name, specials[i].name);
		special_items[ref].cost = specials[i].cost;
		special_items[ref].numrecieve = specials[i].numrecieve;
		special_items[ref].refnum = i;
		special_items[ref].num = ta[current_buystuff_player].specials[i].amount;
		special_items[ref].parent = BUYSTUFF_SPECIAL;

		// Up it up
		ref++;
	}

	num_special_items = ref;
}


//
// CalcTotalList - Recalculates the ENTIRE list of everything
// that the player has in inventory.
//

void CalcTotalList(void)
{
	int		i;
	int		ref=0;

	memset(total_items, 0, sizeof(menuitem_t));

	// Fill up the weapons list first
	for (i=0; i<NUM_PROJECTILES; i++)
	{
		if (projectiles[i].numrecieve <= 0)
			continue;

		// We dont own/have enough ammo for it
		if (ta[current_buystuff_player].weapons[i].ammo <= 0)
			continue;

		ustrcpy(total_items[ref].name, projectiles[i].name);
		total_items[ref].cost = projectiles[i].cost;
		total_items[ref].numrecieve = projectiles[i].numrecieve;
		total_items[ref].refnum = i;
		total_items[ref].num = ta[current_buystuff_player].weapons[i].ammo;
		total_items[ref].parent = BUYSTUFF_WEAPONS;

		// Up it up
		ref++;
	}


	// Fill up the defense list next
	for (i=0; i<NUM_DEFENSE; i++)
	{
		if (defenses[i].numrecieve <= 0)
			continue;

		if (ta[current_buystuff_player].defense[i].amount <= 0)
			continue;

		ustrcpy(total_items[ref].name, defenses[i].name);
		total_items[ref].cost = defenses[i].cost;
		total_items[ref].numrecieve = defenses[i].numrecieve;
		total_items[ref].refnum = i;
		total_items[ref].num = ta[current_buystuff_player].defense[i].amount;
		total_items[ref].parent = BUYSTUFF_DEFENSE;

		// Up it up
		ref++;
	}

	// Fill up the special list now
	for (i=0; i<NUM_SPECIALS; i++)
	{
		if (specials[i].numrecieve <= 0)
			continue;

		if (ta[current_buystuff_player].specials[i].amount <= 0)
			continue;

		ustrcpy(total_items[ref].name, specials[i].name);
		total_items[ref].cost = specials[i].cost;
		total_items[ref].numrecieve = specials[i].numrecieve;
		total_items[ref].refnum = i;
		total_items[ref].num = ta[current_buystuff_player].specials[i].amount;
		total_items[ref].parent = BUYSTUFF_SPECIAL;

		// Up it up
		ref++;
	}


	num_total_items = ref;
}









//
// itemlist_getter() - this isnt used anymore
//

char *itemlist_getter(int index, int *list_size)
{
	if (buystuff_state == BUYSTUFF_WEAPONS)
	{
		if (index < 0) 
		{
			*list_size = num_weapons_items;
			return NULL;
		}
		else
		{ return weapons_items[index].name; }
	}
	if (buystuff_state == BUYSTUFF_DEFENSE)
	{
		if (index < 0) 
		{
			*list_size = num_defense_items;
			return NULL;
		}
		else
		{ return defense_items[index].name; }
	}
	if (buystuff_state == BUYSTUFF_SPECIAL)
	{
		if (index < 0) 
		{
			*list_size = num_special_items;
			return NULL;
		}
		else
		{ return defense_items[index].name; }
	}
	if (buystuff_state == BUYSTUFF_SELLSTUFF)
	{
		if (index < 0) 
		{
			*list_size = num_total_items;
			return NULL;
		}
		else
		{ return total_items[index].name; }
	}

	*list_size = 0;
	return NULL;

}

void	Calc_All_Lists(void)
{
	CalcWeaponsList();
	CalcDefenseList();
	CalcSpecialList();
	CalcTotalList();
}


//
// d_buystuff_list_proc - New routine for displaying information
//						  about weapons and alot of other stuff
// 

int d_buystuff_list_proc(int msg, DIALOG *d, int c)
{
	FONT	*orginal_font;
	int		ret;

	// Make a backup copy of the orginal font, we'll have to restore it
	orginal_font = font;

	// Calculate what we can buy and cannot buy
	Calc_All_Lists();

	// Pull up our new font;
	font = (FONT *)maindata[VER10].dat;

	if (msg == MSG_DRAW || msg == MSG_CLICK) 
	{
		d_abeos_buystufflist_proc(msg, d, current_buystuff_player);
		ret = D_O_K;
	}
	else 
	{
		//ret = lex_list_proc(msg, d, c);
		//ret = lex_buystufflist_proc(msg, d, current_buystuff_player);
		ret = d_abeos_list_proc(msg, d, c);
	}

	//ret = d_abeos_buystufflist_proc(msg, d, current_buystuff_player);

	// Restore the font
	font = orginal_font;

	return ret;
}



int	Weapons_Button(int i)
{
	dialog_buystuff[1].flags |= D_DISABLED;
	dialog_buystuff[2].flags = D_EXIT;
	dialog_buystuff[3].flags = D_EXIT;
	dialog_buystuff[4].flags = D_EXIT;

	// Hide the "Buy" button & make sell button visable
	//dialog_buystuff[5].flags = D_EXIT;		// Buy
	//dialog_buystuff[6].flags |= D_HIDDEN;				// Sell

	// Set the state
	buystuff_state = BUYSTUFF_WEAPONS;

	// Reset at top-of-list
	dialog_buystuff[0].d1 = 0;
	dialog_buystuff[0].d2 = 0;

	broadcast_dialog_message(MSG_DRAW, 0);

	return D_O_K;
}

int	Defense_Button(int i)
{
	dialog_buystuff[2].flags |= D_DISABLED;
	dialog_buystuff[1].flags = D_EXIT;
	dialog_buystuff[3].flags = D_EXIT;
	dialog_buystuff[4].flags = D_EXIT;

	// Hide the "Buy" button & make sell button visable
	//dialog_buystuff[5].flags = D_EXIT;		// Buy
	//dialog_buystuff[6].flags |= D_HIDDEN;				// Sell

	// Set the state
	buystuff_state = BUYSTUFF_DEFENSE;

	// Reset at top-of-list
	dialog_buystuff[0].d1 = 0;
	dialog_buystuff[0].d2 = 0;

	broadcast_dialog_message(MSG_DRAW, 0);

	return D_O_K;
}

int	Special_Button(int i)
{
	dialog_buystuff[3].flags |= D_DISABLED;
	dialog_buystuff[1].flags = D_EXIT;
	dialog_buystuff[2].flags = D_EXIT;
	dialog_buystuff[4].flags = D_EXIT;

	// Hide the "Buy" button & make sell button visable
	//dialog_buystuff[5].flags = D_EXIT;		// Buy
	//dialog_buystuff[6].flags |= D_HIDDEN;				// Sell

	// Set the state
	buystuff_state = BUYSTUFF_SPECIAL;

	// Reset at top-of-list
	dialog_buystuff[0].d1 = 0;
	dialog_buystuff[0].d2 = 0;

	broadcast_dialog_message(MSG_DRAW, 0);

	return D_O_K;
}


int Sell_Button(int i)
{
	// Change the buttons accordingly
	dialog_buystuff[3].flags = D_EXIT;
	dialog_buystuff[1].flags = D_EXIT;
	dialog_buystuff[2].flags = D_EXIT;
	dialog_buystuff[4].flags |= D_DISABLED;

	// Hide the "Buy" button & make sell button visable
	//dialog_buystuff[5].flags |= D_HIDDEN;		// Buy
	//dialog_buystuff[6].flags = D_EXIT;				// Sell
	
	// Set the state
	buystuff_state = BUYSTUFF_SELLSTUFF;

	// Reset at top-of-list
	dialog_buystuff[0].d1 = 0;
	dialog_buystuff[0].d2 = 0;

	broadcast_dialog_message(MSG_DRAW, 0);

	return D_O_K;
}

//
// BuyItem - Simple concept, have enough money? WILL TRAVEL!
// 

int BuyItem(int i)
{
	// Keep things organized
	int	pnum;

	if (buystuff_state == BUYSTUFF_WEAPONS)
	{

		pnum = weapons_items[dialog_buystuff[0].d1].refnum;
		if (ta[current_buystuff_player].money >= projectiles[pnum].cost)
		{ // This limits players to purchasing a maximum of one superweapon. It does not apply to computer players, so far as I can tell. 
		if(projectiles[pnum].thischaracter != -1) 
      if(ta[current_buystuff_player].weapons[pnum].ammo>=1) 
        return -1; 

	GiveAmmo(current_buystuff_player, pnum, projectiles[pnum].numrecieve);
			ta[current_buystuff_player].GiveMoney(-(projectiles[pnum].cost));
		}
		else
			return -1;
	}
	
	else if (buystuff_state == BUYSTUFF_DEFENSE)
	{
		pnum = defense_items[dialog_buystuff[0].d1].refnum;
		if (ta[current_buystuff_player].money >= defenses[pnum].cost)
		{
			GiveDefense(current_buystuff_player, pnum, defenses[pnum].numrecieve);
			ta[current_buystuff_player].GiveMoney(-(defenses[pnum].cost));
		}
		else
			return -1;

	}

	else if (buystuff_state == BUYSTUFF_SPECIAL)
	{
		pnum = special_items[dialog_buystuff[0].d1].refnum;
		if (ta[current_buystuff_player].money >= specials[pnum].cost)
		{
			GiveSpecial(current_buystuff_player, pnum, specials[pnum].numrecieve);
			ta[current_buystuff_player].GiveMoney(-(specials[pnum].cost));
		}
		else
			return -1;

	}
	else	// Though, you'll never see this
	{
		alert("HOW ARE YOU GENTLEMEN", "ALL YOUR BASE ARE BELONG TO US!!", NULL, "WHAT YOU SAY!!", NULL, 0,0);
	}

	broadcast_dialog_message(MSG_DRAW, 0);

	return 1;
}


//
// SellItem - Okay, so maybe the players wants to free some
//			  money up. Or didnt like their purchase. We accept
//			  returns! 25% restocking fee though!! MUhaha	
//			  Go ahead! Talk to the manager! See what I care!
//

int SellItem(int i)
{
	// First find out how much money *1* of this particular
	// missile would cost
	int		pnum = total_items[dialog_buystuff[0].d1].refnum;
	int		parent = total_items[dialog_buystuff[0].d1].parent;
	float	cost;
	float	missile_cost;

	if (parent == BUYSTUFF_WEAPONS)
	{
		cost = projectiles[pnum].cost / projectiles[pnum].numrecieve;
		missile_cost = cost * 0.75;

		if (ta[current_buystuff_player].weapons[pnum].ammo)
		{
			ta[current_buystuff_player].weapons[pnum].ammo--;
			ta[current_buystuff_player].money += (int)missile_cost;
		}
	}
	else if (parent == BUYSTUFF_DEFENSE)
	{
		cost = defenses[pnum].cost / defenses[pnum].numrecieve;
		missile_cost = cost * 0.75;

		if (ta[current_buystuff_player].defense[pnum].amount)
		{
			ta[current_buystuff_player].defense[pnum].amount--;
			ta[current_buystuff_player].money += (int)missile_cost;
		}
	}

	else if (parent == BUYSTUFF_SPECIAL)
	{
		cost = specials[pnum].cost / specials[pnum].numrecieve;
		missile_cost = cost * 0.75;

		if (ta[current_buystuff_player].specials[pnum].amount)
		{
			ta[current_buystuff_player].specials[pnum].amount--;
			ta[current_buystuff_player].money += (int)missile_cost;
		}
	}

	broadcast_dialog_message(MSG_DRAW, 0);

	return D_O_K;
}

//
// Dont actually exit, that's what the DONE button is for
//
int	exit_buystuff(void)
{
	return D_O_K;
}


/*int		d_buystuff_button(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		BuyStuff_Drawer();
		return D_O_K;
	}

	return d_agup_button_proc(msg, d, c);
}*/


int		d_buystuff_weaponsbutton(int msg, DIALOG *d, int c)
{
	int	sprite;
	
	if (msg == MSG_DRAW)
	{
		sprite = BUYSTUFF_WEAPONS_UP;

		if (d->flags & D_GOTFOCUS)
			sprite = BUYSTUFF_WEAPONS_OVER;
	    if (d->flags & D_SELECTED)
			sprite = BUYSTUFF_WEAPONS_DOWN;
		if (d->flags & D_DISABLED)
			sprite = BUYSTUFF_WEAPONS_OVER;

		// Draw the sprite
		draw_sprite(agup_buffer, (BITMAP *)maindata[sprite].dat, d->x, d->y);

		return D_O_K;
	}
	else
		return d_agup_push_proc(msg, d, c);
}

int		d_buystuff_defensebutton(int msg, DIALOG *d, int c)
{
	int	sprite;
	
	if (msg == MSG_DRAW)
	{
		sprite = BUYSTUFF_DEFENSE_UP;

		if (d->flags & D_GOTFOCUS)
			sprite = BUYSTUFF_DEFENSE_OVER;
	    if (d->flags & D_SELECTED)
			sprite = BUYSTUFF_DEFENSE_DOWN;
		if (d->flags & D_DISABLED)
			sprite = BUYSTUFF_DEFENSE_OVER;

		// Draw the sprite
		draw_sprite(agup_buffer, (BITMAP *)maindata[sprite].dat, d->x, d->y);

		return D_O_K;
	}
	else
		return d_agup_push_proc(msg, d, c);
}

int		d_buystuff_specialbutton(int msg, DIALOG *d, int c)
{
	int	sprite;
	
	if (msg == MSG_DRAW)
	{
		sprite = BUYSTUFF_SPECIAL_UP;

		if (d->flags & D_GOTFOCUS)
			sprite = BUYSTUFF_SPECIAL_OVER;
	    if (d->flags & D_SELECTED)
			sprite = BUYSTUFF_SPECIAL_DOWN;
		if (d->flags & D_DISABLED)
			sprite = BUYSTUFF_SPECIAL_OVER;

		// Draw the sprite
		draw_sprite(agup_buffer, (BITMAP *)maindata[sprite].dat, d->x, d->y);

		return D_O_K;
	}
	else
		return d_agup_push_proc(msg, d, c);
}

int		d_buystuff_sellbutton(int msg, DIALOG *d, int c)
{
	int	sprite;
	
	if (msg == MSG_DRAW)
	{
		sprite = BUYSTUFF_SELL_UP;

		if (d->flags & D_GOTFOCUS)
			sprite = BUYSTUFF_SELL_OVER;
	    if (d->flags & D_SELECTED)
			sprite = BUYSTUFF_SELL_DOWN;
		if (d->flags & D_DISABLED)
			sprite = BUYSTUFF_SELL_OVER;

		// Draw the sprite
		draw_sprite(agup_buffer, (BITMAP *)maindata[sprite].dat, d->x, d->y);

		return D_O_K;
	}
	else
		return d_agup_push_proc(msg, d, c);
}

int		d_buystuff_donebutton(int msg, DIALOG *d, int c)
{
	int	sprite;
	
	if (msg == MSG_DRAW)
	{
		sprite = BUYSTUFF_DONE_UP;

		if (d->flags & D_GOTFOCUS)
			sprite = BUYSTUFF_DONE_OVER;
	    if (d->flags & D_SELECTED)
			sprite = BUYSTUFF_DONE_DOWN;
		if (d->flags & D_DISABLED)
			sprite = BUYSTUFF_DONE_OVER;

		// Draw the sprite
		draw_sprite(agup_buffer, (BITMAP *)maindata[sprite].dat, d->x, d->y);

		return D_O_K;
	}
	if (msg == MSG_IDLE)
	{
		BuyStuff_Drawer();
		return D_O_K;
	}
	else
		return d_agup_button_proc(msg, d, c);
}


int		d_buystuff_joystick(int msg, DIALOG *d, int c)
{
	int		cascii=0;
	int		cscan=0;
	int		ccombo=0;
	int		i;


	if (!usejoystick)
		return D_O_K;

	CheckJoyStick();

	if (IsJoyButtonDn(0,8))
		return D_EXIT;

	else if (IsJoyButtonDn(0, 0))
	{
		if (buystuff_state == BUYSTUFF_SELLSTUFF)
		{
			SellItem(d->d1);
			SelectionPulse(buystuff_selection_x, 
							buystuff_selection_y,
							buystuff_selection_w,
							buystuff_selection_h);
		}
		else
		{
			if (BuyItem(d->d1)==1)
			{
				SelectionPulse(buystuff_selection_x, 
							buystuff_selection_y,
							buystuff_selection_w,
							buystuff_selection_h);
			}
		}

		return D_O_K;
	}

	else if (IsJoyButtonDn(0, 2))
	{
		// Simulate the DOWN arrow being pressed 5 times
		for (i=0; i<5; i++)
		{
			cascii = 0;
			cscan = KEY_DOWN;

			if ((!cascii) && (!cscan))
				cascii = ureadkey(&cscan);

			ccombo = (cscan<<8) | ((cascii <= 255) ? cascii : '^');
	
			SEND_MESSAGE(&dialog_buystuff[0], MSG_CHAR, ccombo);

			SEND_MESSAGE(&dialog_buystuff[0], MSG_UCHAR, cascii);

		}
		return D_O_K;
	}

	else if (IsJoyButtonDn(0, 1))
	{
		// Simulate the UP arrow being pressed 5 times
		for (i=0; i<5; i++)
		{
			cascii = 0;
			cscan = KEY_UP;

			if ((!cascii) && (!cscan))
				cascii = ureadkey(&cscan);

			ccombo = (cscan<<8) | ((cascii <= 255) ? cascii : '^');
	
			SEND_MESSAGE(&dialog_buystuff[0], MSG_CHAR, ccombo);

			SEND_MESSAGE(&dialog_buystuff[0], MSG_UCHAR, cascii);

		}
		return D_O_K;
	}

	// Simulate the "up arrow" key
	if (nojoytime <= 0 && 
		IsJoyStickMoved(0, 0, 1, 20) && 
		(joy[0].stick[0].axis[1].pos) < 0)
	{
		cascii = 0;
		cscan = KEY_UP;
	}

	// Simulate the "down arrow" key
	else if (nojoytime <= 0 && 
		IsJoyStickMoved(0, 0, 1, 20) && 
		(joy[0].stick[0].axis[1].pos) > 0)
	{
		cascii = 0;
		cscan = KEY_DOWN;
	}

	// Move to the next category to the left
	else if (nojoytime <= 0 && 
		IsJoyStickMoved(0, 0, 0, 128) && 
		(joy[0].stick[0].axis[0].pos/128) < 0)
	{
		buystuff_state--;

		if (buystuff_state < 0)
			buystuff_state = BUYSTUFF_SELLSTUFF;

		if (buystuff_state == BUYSTUFF_WEAPONS)
			Weapons_Button(0);
		else if (buystuff_state == BUYSTUFF_DEFENSE)
			Defense_Button(0);
		else if (buystuff_state == BUYSTUFF_SPECIAL)
			Special_Button(0);
		else if (buystuff_state == BUYSTUFF_SELLSTUFF)
			Sell_Button(0);

		return D_REDRAW;

	}

	// Move to the next category to the right
	else if (nojoytime <= 0 && 
		IsJoyStickMoved(0, 0, 0, 128) && 
		(joy[0].stick[0].axis[0].pos/128) > 0)
	{
		buystuff_state++;

		if (buystuff_state > BUYSTUFF_SELLSTUFF)
			buystuff_state = BUYSTUFF_WEAPONS;

		if (buystuff_state == BUYSTUFF_WEAPONS)
			Weapons_Button(0);
		else if (buystuff_state == BUYSTUFF_DEFENSE)
			Defense_Button(0);
		else if (buystuff_state == BUYSTUFF_SPECIAL)
			Special_Button(0);
		else if (buystuff_state == BUYSTUFF_SELLSTUFF)
			Sell_Button(0);

		return D_REDRAW;

	}

	// Was a "simulated" key pressed that needs to be processed
	// to the main buystuff list?
	if (cscan != 0)
	{
		if ((!cascii) && (!cscan))
			cascii = ureadkey(&cscan);

		ccombo = (cscan<<8) | ((cascii <= 255) ? cascii : '^');

		SEND_MESSAGE(&dialog_buystuff[0], MSG_CHAR, ccombo);

		SEND_MESSAGE(&dialog_buystuff[0], MSG_UCHAR, cascii);
	

		return D_O_K;
	}

	return D_O_K;
}


DIALOG dialog_buystuff[] =
{	/* (dialog proc)			(x)      (y)   (w)   (h)   (fg)  (bg)  (key) (flags) (d1)   (d2)  (dp)              (dp2) (dp3) */
   { d_buystuff_list_proc,		286,     119,  482,  376,   255,  0,    0,    0,       0,	  0,  (void*)itemlist_getter },

   { d_buystuff_weaponsbutton, 294,      88,  86 ,   22,   255,  0,    0,    0,       -1,     0,  (char *)"Weapons",	0,	(void*)Weapons_Button },
   { d_buystuff_defensebutton, 383,      88,  86 ,   22,   255,  0,    0,    0,       -1,     0,  (char *)"Defense",	0,	(void*)Defense_Button },
   { d_buystuff_specialbutton, 472,      88,  86 ,   22,   255,  0,    0,    0,       -1,     0,  (char *)"Special",	0,	(void*)Special_Button },
   { d_buystuff_sellbutton,    686,      88,  86 ,   22,   255,  0,    0,    0,		  -1,     0,  (char *)"Sell Items", 0,	(void*)Sell_Button },
   { d_buystuff_donebutton,    679,     510,  100,   32,   255,  0,    0,    D_EXIT,   0,     0,  (char *)"DONE"                   },
   { d_keyboard_proc,			 0,       0,    0,    0,     0,  0,    0,    0,  KEY_ESC,     0,  (void*)exit_buystuff,	  NULL,  NULL  },
   { d_buystuff_joystick,		 0,       0,    0,    0,     0,  0,    0,    0,  KEY_ESC,     0,  (void*)exit_buystuff,	  NULL,  NULL  },
   { NULL,                       0,       0,    0,    0,     0,  0,    0,    0,        0,     0,  0,                     0,     0 }

};




BITMAP	*buystuff_buffer;

void BuyStuff_Drawer(void)
{
	char	buf[128];
	int	color;

	if (update_count <= 0)
		return;

	update_count = 0;

	// Keep recieving/sending packets
	if (multiplayer)
		InterTicker();

	object_message(&dialog_buystuff[0], MSG_DRAW, 0);

	// First before anything, draw the background
	stretch_blit((BITMAP *)maindata[BUYSTUFF_BACKGROUND].dat,
						buffer,
						0, 0, 800, 600,
						0, 0, SCREENWIDTH, SCREENHEIGHT);

	draw_sprite(buffer, 
			(BITMAP *)maindata[characters[ta[current_buystuff_player].character].tankicon].dat, 
			70+8,
			115);

	// Next level....
	textout_centre(buffer, (FONT *)maindata[TINY8].dat, "Upcoming map:", 556, 27, makecol(255,255,255));

	// (Insert level name here)
	textout_centre(buffer, (FONT *)maindata[TAHOMA10].dat, currentlevel.name, 556+2, 50+2, makecol(0,0,0));
	textout_centre(buffer, (FONT *)maindata[TAHOMA10].dat, currentlevel.name, 556, 50, makecol(255,255,255));

	
	textout_centre(buffer, (FONT *)maindata[MS].dat, ta[current_buystuff_player].name, 132+3+8, 245+3, makecol(0,0,0));
	textout_centre(buffer, (FONT *)maindata[MS].dat, ta[current_buystuff_player].name, 132+8, 245, makecol(255,255,255));

	sprintf(buf, "( $ %d )", ta[current_buystuff_player].money);
	textout_centre(buffer, (FONT *)maindata[MS].dat, buf, 132+2, 300+2, makecol(0,0,0));
	textout_centre(buffer, (FONT *)maindata[MS].dat, buf, 132, 300, makecol(255,255,255));

		
	// Blit it all to the screen, finishing up
	draw_sprite(buffer, buystuff_buffer, 0,0);

	rect(buffer, 
			buystuff_selection_x, 
			buystuff_selection_y, 
			buystuff_selection_w, 
			buystuff_selection_h, 
			makecol(192,192,192));

	// Selection color
	if (buystuff_selection_color < 255)
	{
		if (buystuff_state == BUYSTUFF_SELLSTUFF)
			buystuff_selection_color+=9;
		else
			buystuff_selection_color+=3;

		// Check
		if (buystuff_selection_color > 255)
			buystuff_selection_color = 255;

		if (buystuff_state == BUYSTUFF_SELLSTUFF)
		{ color = makecol(255, buystuff_selection_color, buystuff_selection_color); }
		else
		{ color = makecol(buystuff_selection_color, buystuff_selection_color, 255); }

		rect(buffer, 
			buystuff_selection_x, 
			buystuff_selection_y, 
			buystuff_selection_w, 
			buystuff_selection_h, 
			color);

	}


	// Draw how much time is left
	if (multiplayer && tank_orginalshottimer > 0)
	{
		int		timeleft;
		int		hours, minutes, seconds;
		char	str[128];

		//timeleft = (int)(tank_shottimer) - playersetuptics;
		timeleft = tank_shottimer;
	
		if (timeleft < 0)
			timeleft = 0;
	
		hours = timeleft / (TICRATE * 3600);
		timeleft -= hours * TICRATE * 3600;
		minutes = timeleft / (TICRATE * 60);
		timeleft -= minutes * TICRATE * 60;
		seconds = timeleft / TICRATE;

		if (hours)
			usprintf (str, "%02d:%02d:%02d", hours, minutes, seconds);
		else
			usprintf (str, "%02d:%02d", minutes, seconds);

		//textout_centre(buffer, (FONT *)maindata[MS].dat, str, 100+1, 5+1, makecol(0,0,0));
		AddRectangle(buffer,0,0,SCREENWIDTH,25);
		//textout_centre(buffer, (FONT *)maindata[MS].dat, str, 0, 0, makecol(0,0,0));
		textout_centre(buffer, (FONT *)maindata[MS].dat, str, (SCREENWIDTH/2)+1, 1, 0);
		textout_centre(buffer, (FONT *)maindata[MS].dat, str, SCREENWIDTH/2, 0, makecol(255,255,255));
	}

	// Gamelines thinker & drawer
	GameLinesThinker();
	GameLinesDrawer(buffer);


	// Run some timer stuff
	ticker();
	gametics++;
	frame_count++;
	
	Screen_Blit(true);
}



void BuyStuffFor(int player)
{
	int				focus=0;

	if (ta[player].skill != TANKSKILL_HUMAN)
	{
		ta[player].ComputerBuyStuff();
		return;
	}

	current_buystuff_player = player;

	clear_keybuf();

	buystuff_buffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(buystuff_buffer, TRANSCOLOR);

	// Setup some varibles, we start in the weapons tab always
	Weapons_Button(0);

	// Reset selection color to its max
	buystuff_selection_color = 255;

	do_agup_dialog(dialog_buystuff, buystuff_buffer, BuyStuff_Drawer);//, buystuff_buffer, BuyStuff_Drawer);

	// Keep track of how much fuel we're going into the game with
	ta[current_buystuff_player].startfuel = ta[current_buystuff_player].specials[fuel].amount;

	// Free some memory
	destroy_bitmap(buystuff_buffer);
}



void StartBuyStuff(void)
{
	int		i;

	if (!multiplayer)
	{
		for (i=0; i<MAX_TANKS; i++)
		{
			if (ta[i].exist <= 0) 
				{ continue; }
	
			BuyStuffFor(i);	
		}
	}
	else
	{
		if (multiplayermode == MP_CLIENT)
			BuyStuffFor(net_localplayer);
		else
		{
			// First, let the localclient buystuff
#ifndef	DEDICATED_SERVER
			if (ta[net_localplayer].skill == TANKSKILL_HUMAN)
				BuyStuffFor(net_localplayer);
#endif

			// Allow bots to purchase items as well
			for (i=0; i<MAX_TANKS; i++)
			{
				if (ta[i].exist > 0 && 
					ta[i].skill != TANKSKILL_HUMAN)
				{ BuyStuffFor(i); }
			}
		}
	}

}

