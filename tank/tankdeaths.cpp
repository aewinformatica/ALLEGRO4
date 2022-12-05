#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"
#include "particles.h"


// Some functions
void		Standard_Seq(int tank);
void		ThreeMissile_Seq(int tank);
void		BabyNuke_Seq(int tank);
void		SeveralExplosions_Seq(int tank);
void		TankGib_Seq(int tank);
void		TankPift_Seq(int tank);
void		AlienAbduct_Seq(int tank);

tankdeath_t		tankdeaths[NUM_TANKDEATHS] = 
{
	{ 
		"Standard",
		1.5*TICRATE,
		Standard_Seq,
	},

	{
		"Baby nuke Explode",
		3*TICRATE,
		BabyNuke_Seq,
	},

	{
		"Baby Nuke & Nuke Explode",
		5*TICRATE,
		ThreeMissile_Seq,
	},

	{
		"Several Explosions",
		2*TICRATE,
		SeveralExplosions_Seq,
	},

	{
		"Tank Gib",
		3000*TICRATE,
		TankGib_Seq,
	},

	{
		"Tank Pift",
		TICRATE/2,
		TankPift_Seq,
	},

	{
		"Alien Abduction",
		TICRATE*4,
		AlienAbduct_Seq,
	},
};


//
// StartDeathSequence - Assigns a sequence function and timer that will be played in the main loop
//

void		StartRandomDeathSequence(int tank)
{
	char msg[512];

	int		seq = rand()%NUM_TANKDEATHS;		///no change - server only routine
	
	// Does not exist, not valid
	if (seq > NUM_TANKDEATHS || seq < 0)
		seq = 0;

	// Break out of the dialog
	if (ta[tank].tankstate == TANKSTATE_DEFENSE)
		agup_break_dialog();


	// Check and see if we killed ourself, if we did
	// display a stupid message. If the value is lower than
	// zero do not output a character message, apparently 
	// it is being taken care of somewhere else (eg. falling kill)
	if (ta[tank].lasthit >= 0 && talkingtanks->value > 0)
		{
		if (ta[tank].lasthit == tank)
			TankSay(selfkill_text[rand()%MAX_SELFKILL_TEXT], tank, (int)(3*TICRATE));
		else// if (rand()%255 <= 128)
			{
			// Spawn a kill messge
			if (rand()%255 < 128)
				TankSay(kill_text[rand()%MAX_KILL_TEXT], ta[tank].lasthit, (int)(3*TICRATE));
			else
				TankSay(killed_text[rand()%MAX_KILLED_TEXT], tank, (int)(3*TICRATE));
			}
		}
	
	//	this tank is exploding so it may kill others
	ta[tank].lastweaponused = 5;	// tank shard

	// Spawn output console text indicating who died
	if (ta[tank].lasthit >= 0)
		{
		if (ta[tank].lasthit == tank)
			sprintf(msg,"Suicide by %s \n", ta[tank].name);
		else
			{
			// lastweaponused will be -1 if the tank has not fired yet
			// ie the tank died and the death explosion takes out another tank.
			if(ta[ta[tank].lasthit].lastweaponused < 0)
				sprintf(msg,"%s was killed by %s's suicide curse\n", ta[tank].name, ta[ta[tank].lasthit].name);
			else
				sprintf(msg,"%s was killed by %s's %s\n", ta[tank].name, ta[ta[tank].lasthit].name, projectiles[ta[ta[tank].lasthit].lastweaponused].name);
			}
		}
	else		// usually by one of the level's sprites
		sprintf(msg,"%s died due to natural causes\n", ta[tank].name);

	// added a message to clients
	BCast_WriteByte(STOC_PRINTF);
	BCast_WriteString(msg);
	Printf(msg);

	// debug
	//seq = 6;

	ta[tank].deathseq = seq;
	ta[tank].deathtics = 1;

	// Set tank timer
	SetTankTimer(TICRATE);

	// Tell all other clients to init this death sequence
	if (multiplayer && multiplayermode == MP_SERVER)
		{
		BCast_WriteByte(STOC_KILLTANK);
		BCast_WriteByte(tank);
		BCast_WriteByte(ta[tank].deathseq);
		}
}


void		StartDeathSequence(int tank, int seq)
{
	// Does not exist, not valid
	if (seq > NUM_TANKDEATHS || seq < 0)
		seq = 0;

	// Break out of the dialog
	if (ta[tank].tankstate == TANKSTATE_DEFENSE)
		agup_break_dialog();

	// Check and see if we killed ourself, if we did
	// display a stupid message. If the value is lower than
	// zero do not output a character message, apparently 
	// it is being taken care of somewhere else (eg. falling kill)
	if (ta[tank].lasthit >= 0 && talkingtanks->value > 0)
		{
		if (ta[tank].lasthit == tank)
			TankSay(selfkill_text[rand()%MAX_SELFKILL_TEXT], tank, (int)(3*TICRATE));
		else// if (rand()%255 <= 128)
			{
			// Spawn a kill messge
			if (rand()%255 < 128)
				TankSay(kill_text[rand()%MAX_KILL_TEXT], ta[tank].lasthit, (int)(3*TICRATE));
			else
				TankSay(killed_text[rand()%MAX_KILLED_TEXT], tank, (int)(3*TICRATE));
			}
		}


	ta[tank].deathseq = seq;
	ta[tank].deathtics = 1;

	// Set tank timer
	SetTankTimer(TICRATE);

	// Tell all other clients to init this death sequence
	if (multiplayer && multiplayermode == MP_SERVER)
		{
		BCast_WriteByte(STOC_KILLTANK);
		BCast_WriteByte(tank);
		BCast_WriteByte(ta[tank].deathseq);
		}
}

//
// CheckTankDeathSeq
// 

int			CheckTankDeathSeq(void)
{
	int		i;

	if (AreTanksTalking() > 0)
		return 0;

	for (i=0; i<MAX_TANKS; i++)
	{
		//if (ta[i].exist <= 0)
		//	continue;

		if (ta[i].deathtics <= 0)
			continue;

		// Playing some text, don't display the death animation just
		// yet
		//if (ta[i].texttimer > 0)
		//	return 0;

		// Animate the death sequence
		tankdeaths[ta[i].deathseq].proc(i);

		// Increase ticker
		ta[i].deathtics++;

		// Done animating?
		if (ta[i].deathtics > tankdeaths[ta[i].deathseq].time)
		{
			ta[i].deathseq = ta[i].deathtics = -1;

			// Wipe out some varibles he'ar
			ta[i].health = 0.0;
			ta[i].needstodie = 0;

			// Deallocate all memory for this tank since its being
			// removed from the game
			if (ta[i].exist <= 0)
			{
				destroy_bitmap(ta[i].bmp);
				destroy_bitmap(ta[i].cannon);
	
				if (ta[i].shield)
					ta[i].shield->Kill();
			}

			ta[i].needstodie = 0;

			// End here
			return 1;
		}

		break;
	}

	return 0;
}

//
// IsTankDeathSeq - Returns 1 if any tanks are animating death sequences
//

int	IsTankDeathSeq(void)
{
	int		i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].deathtics <= 0)
			continue;

		// This tank is running death animation
		return 1;
	}

	return 0;

}


//
// Standard sprite explosion that was used in all the betas
//

void		Standard_Seq(int tank)
{
	if (ta[tank].deathtics == 1)
	{
		ta[tank].needstodie = 0;
		ta[tank].StartSound(TANK_EXPLODE);
		G_SpawnSprite((int)ta[tank].x, (int)ta[tank].y, S_EXPLODE1_1, (MF_NORMAL | MF_ADVANCETURN | MF_CENTERCOORD | MF_ALPHA));
	}
}


void		BabyNuke_Seq(int tank)
{
	int		s;
	if (ta[tank].deathtics == 1)
	{
		s = findshot();
		if (s>=0)
		{
			sh[s].Init(s, W_BABYNUKE, 0,0,0,0,0);
			sh[s].x = ta[tank].x;
			sh[s].y = ta[tank].y;
			sh[s].tankfrom = tank;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, (MF_NORMAL | MF_CENTERCOORD));
			ServerClient = 'B';
			sh[s].explode();
		}

		ta[tank].needstodie = 0;
	}
}

void		ThreeMissile_Seq(int tank)
{
	int		s;
	if (ta[tank].deathtics == 1)
	{
		s = findshot();
		if (s>=0)
		{
			sh[s].Init(s, W_BABYNUKE, 0,0,0,0,0);
			sh[s].x = ta[tank].x;
			sh[s].y = ta[tank].y;
			sh[s].tankfrom = tank;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, (MF_NORMAL | MF_CENTERCOORD));
			ServerClient = 'T';
			sh[s].explode();
		}

		ta[tank].needstodie = 0;
	}


	if (ta[tank].deathtics == 90)
	{
		s = findshot();
		if (s>=0)
		{
			sh[s].Init(s, W_NUKE, 0,0,0,0,0);
			sh[s].x = ta[tank].x;
			sh[s].y = ta[tank].y;
			sh[s].tankfrom = tank;
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, (MF_NORMAL | MF_CENTERCOORD));
			ServerClient = 'N';
			sh[s].explode();
		}

		ta[tank].needstodie = 0;
	}
}


void		SeveralExplosions_Seq(int tank)
{
	int		sx,sy;
	int		sw,sh;
	int		exp;
	_state	expstate;
	
	if ((ta[tank].deathtics % 5) || ta[tank].deathtics > 1*TICRATE)
	{
		ta[tank].needstodie = 0;
		return;
	}

	sw = ta[tank].sizex;
	sh = ta[tank].sizey;

	sx = ta[tank].x - (ta[tank].sizex/2);
	sy = ta[tank].y - (ta[tank].sizey/2);

	exp = rand()%2;

	if (exp == 0)
		expstate = S_SMALLEXPLODE_A1;
	else
		expstate = S_SMALLEXPLODE_B1;

	G_SpawnSprite(sx+rand()%sw, sy+rand()%sh, expstate, (MF_NORMAL | MF_CENTERCOORD ));

	ta[tank].StartSound(EXPLODE_TOWERPOD);

}


void		TankGib_Seq(int tank)
{
	int		numgibs = 10;
	int		i;
	int		angle;
	int		length = 10;
	int		power;
	int		s;

	if (ta[tank].deathtics < 30)
		return;

	// Server will spawn all the random gibs for us
	if (ta[tank].deathtics == 30)
	{
		SetTankTimer(30);

		// Spawn a sprite
		G_SpawnSprite(ta[tank].x, ta[tank].y, S_SMALLEXPLODE_B1, (MF_NORMAL | MF_CENTERCOORD ));
	
		// ...and play a sound
		ta[tank].StartSound(EXPLODE_TOWERPOD);

		// No longer draw the dying tank...
		ta[tank].needstodie = 0;

		if (multiplayer && multiplayermode == MP_CLIENT)
			return;
	
		for (i=0; i<numgibs; i++)		// Server spawns the shots
		{
			s = findshot();
			if (s < 0)		// Problems
			{ return; }
	
			angle = rand()%128;
			power = ConvertFromGamePower(300+rand()%50);
	
			sh[s].Init(s, W_TANKGIB,
						ta[tank].x,
						ta[tank].y-length,
						(-power*COS(angle)),
						(power*-SIN(angle)),
						angle);
	
			sh[s].sprite = G_SpawnSprite(sh[s].x,sh[s].y, projectiles[sh[s].projectile].state, (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE));
			sh[s].tankfrom = tank;

			// Send signal to all other clients
			if (multiplayer && multiplayermode == MP_SERVER)
				BCast_Shot(s);
		}
	}

	if (ta[tank].deathtics > 35)
	{
		if (!shotinair())
		{
			ta[tank].deathtics = tankdeaths[ta[tank].deathseq].time;
			return;
		}
	}
}

void		TankPift_Seq(int tank)
{
	particlenode_t		*p;
	int					i;
	int					nump = 16;
	int					pangle, plife;


	if (ta[tank].deathtics == (tankdeaths[ta[tank].deathseq].time/2))
	{
		// No longer draw tank
		ta[tank].needstodie = 0;

		// Play us a sound
		ta[tank].StartSound(TANK_PIFT);

		// Spawn some particles, just for looks
		for (i=0; i<nump; i++)
		{
			// Set particle life, randomize this a bit
			plife = 30+rand()%30;

			// Randomize the angle as well
			pangle = 45+rand()%40;

			p = pdirt->InsertParticle(ta[tank].x, ta[tank].y-10, rand()%128, ConvertFromGamePower(300+rand()%100), makecol(255,255,255), plife, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
			if (p)
				p->bounce = 5;
		}
	}
}

void	AlienAbduct_Seq(int tank)
{
	sprite_t*	s;
	if (ta[tank].deathtics == 1)
	{
		s = G_SpawnSprite(ta[tank].x-17, 0, S_ALIENBEAMUP1, (MF_NORMAL));
		s->opacity = 128;
	}

	if (ta[tank].deathtics > (TICRATE/2))
	{
		ta[tank].yvel -= .25;

		ta[tank].y += ta[tank].yvel;
		if (ta[tank].y < 0)
			ta[tank].needstodie = 0;
	}

	if (ta[tank].deathtics == (tankdeaths[ta[tank].deathseq].time-1))
		ta[tank].needstodie = 0;
	
}

	
