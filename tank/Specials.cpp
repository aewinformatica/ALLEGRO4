#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"


void	ActivateLaserSight(int player, int ref);
void	ActivateFuel(int player, int ref);

// The main special array
specialtype_t		specials[NUM_SPECIALS] = {
	
	{ 
		"Fuel",						// Name
		1500,						// Cost
		100,						// Quantity recieved
		SPECIAL_DEFENSEBOX,			// Flags
		ActivateFuel,				// Activate function
	},
	
	{ 
		"Laser Sight",				// Name
		3000,						// Cost
		1,							// Quantity recieved
		SPECIAL_DEFENSEBOX,			// Flags
		ActivateLaserSight,			// Activate function
	},

	{ 
		"Auto Defense",				// Name
		5000,						// Cost
		1,							// Quantity recieved
		SPECIAL_DEFENSEBOX,			// Flags
		NULL,						// Activate function
	},
};


//
// GiveSpecial - Power Extreme!
//
void GiveSpecial(int	player, int ammotype, int	amount)
{
	ta[player].specials[ammotype].amount += amount;
}

//
// ActivateLaserSight - Sets the flag to activate laser sight on a tank
//

void	ActivateLaserSight(int player, int ref)
{
	// Already activated
	if (!(ta[player].specialflags & TANKSPECIAL_LASERSIGHT))
		ta[player].specials[ref].amount--;

	// Set the flag and take some from inventory
	ta[player].specialflags |= TANKSPECIAL_LASERSIGHT;
	
}

//
// ActivateFuel - Sets the flag to activate laser sight on a tank
//

void	ActivateFuel(int player, int ref)
{
}


//
// SetupAutoDefense - Cycles through all the players and setting up their autodefense for them
//
extern		int			tankturnque[MAX_TANKS];
void SetupAutoDefense(void)
{
	int		i;

	if (multiplayer)
		return;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist && 
			ta[i].health > 0 && 
			ta[i].skill == TANKSKILL_HUMAN)
		{
			if (ta[i].specials[autodefense].amount > 0)
			{
				// Minus one from inventory
				ta[i].specials[autodefense].amount--;

				tankturn = i;
				StartDefenseBox(i);
			}
		}
	}

	tankturn = FindFirstTurn();
}
