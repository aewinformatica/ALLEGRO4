#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"

float CalculateAngle(int ox, int oy, int dx, int dy)
{
	fixed target_angle;
	fixed x1,x2,y1,y2;

	x1 = itofix(dx);
	y1 = itofix(dy);
	x2 = itofix(ox);
	y2 = itofix(oy);

	target_angle = (fatan2(y2-y1, x2-x1));

	target_angle &= 0xFFFFFF;
	
	return fixtof(target_angle);
}


float FCalculateAngle(float ox, float oy, float dx, float dy)
{
	fixed target_angle;
	fixed x1,x2,y1,y2;

	x1 = ftofix(dx);
	y1 = ftofix(dy);
	x2 = ftofix(ox);
	y2 = ftofix(oy);

	target_angle = (fatan2(y2-y1, x2-x1));

	target_angle &= 0xFFFFFF;
	
	return fixtof(target_angle);
}

static int		offscreen;
static int		hitland;
static int		ccontact_x;
static int		ccontact_y;

void	callbackclearshot(BITMAP *bmp, int x1, int y1, int color)
{
	if (hitland > 0 || offscreen > 0 || x1 < 0 || x1 > SCREENWIDTH || ccontact_x >= 0 || ccontact_y >= 0)
		return;

	if (y1 < 0 || y1 > SCREENHEIGHT)
	{
		offscreen = 10;
		return; 
	}

	if (FAST_GETPIXEL(ground.bmp, x1, y1) != TRANSCOLOR)
	{
		ccontact_x = x1;
		ccontact_y = y1;
		hitland = 1;
		return;
	}

	// Debug only
	//FAST_PUTPIXEL(buffer, x1,y1, makecol(255,0,0));
}

void	callbacklasersight(BITMAP *bmp, int x1, int y1, int color)
{
	if (ccontact_x >= 0 && ccontact_y >= 0)
		return;

	if (OnScreen(bmp, (int)x1, (int)y1) == -1)
	{ return; }

	if (FAST_GETPIXEL(ground.bmp, (int)x1, (int)y1) != TRANSCOLOR)
	{
		ccontact_x = x1;
		ccontact_y = y1;
		return;
	}
}


int TANK::AdjustAngleGivenClearance (int targetx, int targety)
{
	int	distx, disty;
	int	angle;

	distx = targetx - x;
	disty = targety - y;
	offscreen = -1;

	if (distx > 0)
		angle = 128;
	else
		angle = 0;
	
	

	while (offscreen < 0)
	{
		hitland = -1;
		ccontact_x = -1;
		ccontact_y = -1;
		
		do_line(ground.bmp, x, y, x+(-SCREENWIDTH*COS(angle)), y+(SCREENHEIGHT*-SIN(angle)), TRANSCOLOR, callbackclearshot);

		if (distx > 0)
		{
			angle-=1;
			if (angle <= 0)
			{
				angle = 0;
				break;
			}
		}
		else
		{
			angle+=1;
			if (angle > 128)
			{
				angle = 128;
				break;
			}
		}

		if (offscreen >= 0)
			break;
	}

	if (offscreen < 0)
		return -1;

	return angle;
}

void	TANK::LaserSight(BITMAP *buf)
{
	float	firex,firey;
	int		length = characters[character].cannonlength+15;
	int		laserlength = SCREENWIDTH+(SCREENWIDTH/2);
	ccontact_x = -1;
	ccontact_y = -1;

	firex = x-COS(deg)*length;
	firey = y-SIN(deg)*length;

	// Trace the line
	do_line(buffer, firex, firey, firex+(-laserlength*COS(deg)), firey+(laserlength*-SIN(deg)), 0, callbacklasersight);

	if (ccontact_x >= 0 && ccontact_y >= 0)
	{
		if (gametics % 2)
		{
			AddRectangle(buf, ccontact_x-2, ccontact_y-2, 5,5);
			circlefill(buf, ccontact_x, ccontact_y, 2, makecol(255,0,0));
		}
	}
}


int		tcontactx,tcontacty;
float	ttargetx, ttargety;
float	ttrackx,ttracky;

void	callbacktrajectory(BITMAP *bmp, float x1, float y1, int color)
{
	int		i;
	//int		tankx;
	//int		tanky;
	
	if (tcontactx >= 0 && tcontacty >= 0)
		return;

	// Check to see if we hit a tank
	for (i=0; i<MAX_TANKS; i++)
	{
		if (i==tankturn || ta[i].health <= 0)
			continue;

		if (x1 > ttargetx-2 &&
			x1 < ttargetx+2 &&
			y1 > ttargety-2 &&
			y1 < ttargety+2)
		{
			tcontactx = x1;
			tcontacty = y1;
		}
	}

	/*if (OnScreen(bmp,x1,y1)==-1)
	{
		tcontactx = x1;
		tcontacty = y1;
	}*/

	// Uncomment below and see every possible shot 
	// calculated
	//putpixel(buffer, (int)x1,(int)y1, makecol(255,0,0));
	
}


//
// Aim - Will find the best angle and power level that is required to hitting targetx and
// targety by simply trial and error. If instanttarget is set, the tanks varibles are adjusted
// instantly instead of the engine gradiually doing it
//

unsigned char		bestangle;
float				bestpower;

int	Aim(int origx, int origy, int	targetx, int targety, float deg, int length)
{
	int		distx;//,disty;
	int		done=0;
	int		powertries;		// Number of times we've tried different power levels
	//int		length = bcharacters[character].cannonlength+(characters[character].cannonlength/2);

	ttargetx = targetx;
	ttargety = targety;

	// Make sure this shot isn't in the ground, making it impossible
	//if (getpixel(ground.bmp, targetx, targety) != TRANSCOLOR)
	//{ targety = OnGround(targetx, targety, ground.bmp); }
	while ((getpixel(ground.bmp, ttargetx, ttargety)) != TRANSCOLOR)
		{
		if (ttargety <= 0)
			break;
		ttargety--;
		}

	distx = (int)(ttargetx - origx);

	{
		float	startx,starty;
		float	xvel,yvel;
		int		color,cx,cy;

		startx= origx; 
		starty= origy; 

		powertries = 0;

			for(bestpower=0; bestpower<MAX_LOAD; bestpower+=0.05)
				{
				ttrackx = startx;
				ttracky = starty;

				// Setup velocities for the desired power levels
				xvel = -bestpower*COS(deg);
				yvel = bestpower*-SIN(deg);
				
				// Track the shot just like we would normally do
				while(done == 0 && ttrackx+xvel < SCREENWIDTH && ttrackx+xvel > 0 && ttracky+yvel < SCREENHEIGHT)
					{
					tcontactx = -1;
					tcontacty = -1;

					// Be sure that we haven't hit land of any sort (but going
					// off the screen is okay)
					cx = (int)ttrackx;
					cy = (int)ttracky;
					if(OnScreen(ground.bmp, cx, cy) != -1)
						{
						color = getpixel(ground.bmp, cx, cy);
						if(color != TRANSCOLOR && color != -1)
							{
							break;
							}
						}
					// Gravity that all projectiles use
					yvel += 0.35;

					if(!Underwater(ttrackx, ttracky))
						{
						do_float_line(ground.bmp, ttrackx, ttracky, ttrackx+xvel, ttracky+yvel, TRANSCOLOR, callbacktrajectory);
						if(tcontactx >= 0 && tcontacty >= 0)
							{
							done = 1;
							break;
							}
						ttrackx += xvel;
						ttracky += yvel;
						}
					else
						{
						do_float_line(ground.bmp, ttrackx, ttracky, ttrackx+(xvel/2), ttracky+(yvel/2), TRANSCOLOR, callbacktrajectory);
						if(tcontactx >= 0 && tcontacty >= 0)
							{
							done = 1;
							break;
							}
						ttrackx += (xvel/2);
						ttracky += (yvel/2);
						}
					}
				// See if we need to cut out of the for() loop early if we've hit something
				if(done > 0)
					break;

				// Increase number of times we've tried
				powertries++;
				}
/*
			// Increase the angle
			if(done == 0)
				{
				if(distx > 0)
					{
					deg-=3;
					if(deg <= 0)
						{
						deg = 0;
						done = -1;
						}
					}
				else
					{
					deg+=3;
					if(deg > 128)
						{
						deg = 128;
						done = -1;
						}
					}
				}
*/
	}

	if(done != 1)
		{
		//bestangle = CalculateAngle(ttargetx, 0);
		bestpower = MAX_LOAD/2;
		}
	return done;
}

//
// SelectTarget - Computer will select a random target tank
//

int TANK::SelectTarget(void)
{
	int		newtarget;
	int		distx;

	targettank = -1;

	if (numalivetanks() <= 1)
		return	-1;

	do
	{
		newtarget = rand()%MAX_TANKS;
	} while (ta[newtarget].health <= 0 || newtarget == indexnum || (teamplay->value > 0 && IsSameTeam(newtarget)));

	distx = (int)(ta[newtarget].x - x);
	if (distx >= 0)
		deg = 75;
	else
		deg = 55;

	return newtarget;
}	


//
// MyTurn - It is now my turn to shoot.
// Make sure that my target is still alive.
//

void TANK::MyTurn(void)
{
	int			aimret;			// Results of calculated aim
	int			barrellength = characters[character].cannonlength+(characters[character].cannonlength/2);
	int			ntanks=0;

	if(skill == TANKSKILL_HUMAN)
		return;

	// server does all the bot thinkers
	if(multiplayer)
		{ 
		if(multiplayermode == MP_CLIENT)
			return;

		// Waiting for auto-defense to finish?
		if(SV_IsAllPlaying()==false)
			return;
		}

	// We've already done all this, go directly to setting up shields
	if(shieldidletime > 0)
		goto setupshield;

	// Game is not running, nothing to do
	if(gamestate != GS_INGAME)
		return;

#ifndef	DEDICATED_SERVER
	// Waiting for autodefense...
	if (ta[net_localplayer].specialflags & TANKSPECIAL_AUTODEFENSEWAIT)
		return;
#endif

	// Not enough tanks to take action
	ntanks = numalivetanks();
	if(ntanks <= 1)
		return;

	// If teamplay is set, make sure there are enough players on the opposite
	// team to warrent continuing the AI
	if(teamplay->value > 0)
		{
		if (team == TANKTEAM_RED && numaliveteam(TANKTEAM_BLUE) <= 0)
			{
//#ifdef	_DEBUG
			if(!(gametics % TICRATE))
				Printf("not enough players on the opposite team\n");
//#endif
			return;
			}

		if(team == TANKTEAM_BLUE && numaliveteam(TANKTEAM_RED) <= 0)
			{
//#ifdef	_DEBUG
			if(!(gametics % TICRATE))
				Printf("not enough players on the opposite team\n");
//#endif
			return;
			}
		}

	// Select a target if we dont have one already. We're selecting
	// a target before we activate defenses simply because higher skilled
	// bots need to adjust their defenses/attacks based on who their attacking
	if(targettank < 0 || ta[targettank].health <= 0 || ta[targettank].exist <= 0)
		{
		//char	str[64];
		
		targettank = SelectTarget();
		
		//sprintf(str, "%s selected %s (%d), as target", name, ta[targettank].name, targettank);
		//G_Printf(str);
		}

	// Be sure the target tank (if human) is in the game all the way and not
	// still buying weapons or doing something else. If anything but NETSTATE_INGAME
	// then choose another target.
	if(multiplayer && multiplayermode == MP_SERVER &&
		 ta[targettank].skill == TANKSKILL_HUMAN &&
		 ta[targettank].netstate != NETSTATE_INGAME)
		{ 
//#ifdef	_DEBUG
		if(!(gametics % TICRATE))
			Printf("target tank is not ready (target: %s state: %d)\n", ta[targettank].name, ta[targettank].netstate);
//#endif
		targettank = -1;
		return;
		}

	// Charge up batteries, add shields...etc.
	SetupDefense();

	// We used something that requires a quick pause
	if(idletime > 0)
		{
		idletime--;
		return;
		}

	// Close your eyes, press some buttons and FIRE!  =)
	if(skill == TANKSKILL_STOOPID)
		{
		targetx = rand()%SCREENWIDTH;
		targety = rand()%(SCREENHEIGHT-100)+100;	// 100 >> 600
		}

	// The lobber knows where its aiming and has a target like all the other
	// bot AIs. However, it has a whopping 200 pixel MOE.
	else if(skill == TANKSKILL_LOBBER)
		{
		targetx = ( (ta[targettank].x - (ta[targettank].sizex/2) )-100);
		targety = ( (ta[targettank].y - (ta[targettank].sizey/2) )-100);
	
		targetx += (rand()%(100*2));
		targety += (rand()%(100*2));
		}
	
	// The shooter is exactly like the sniper tank, only it has 100 pixel 
	// error margin.
	else if(skill == TANKSKILL_SHOOTER)
		{
		targetx = ( (ta[targettank].x - (ta[targettank].sizex/2) )-50);
		targety = ( (ta[targettank].y - (ta[targettank].sizey/2) )-50);
	
		targetx += (rand()%(50*2));
		targety += (rand()%(50*2));
		}

	// Gunner has a 50 MOE
	else if(skill == TANKSKILL_GUNNER)
		{
		targetx = ( (ta[targettank].x - (ta[targettank].sizex/2) )-25);
		targety = ( (ta[targettank].y - (ta[targettank].sizey/2) )-25);
	
		targetx += (rand()%(25*2));
		targety += (rand()%(25*2));
		}

	// The sniper will hit you perfectly every time and use the best
	// defenses against you.
	else if(skill == TANKSKILL_SNIPER)
		{
		targetx = ta[targettank].x;
		targety = ta[targettank].y;
		}

	// Make sure we're on the ground level
	if(skill != TANKSKILL_SNIPER)
		targety = OnTopGround(targetx, targety, ground.bmp);

	// Make sure the X and Y coordinates stay within boundries to avoid
	// wierd shots
	if(targetx > SCREENWIDTH) targetx = SCREENWIDTH;
	if(targetx < 0) targetx = 0;
	if(targety > SCREENHEIGHT) targety = SCREENHEIGHT;
	if(targety < 0) targety = SCREENHEIGHT;

	// Select some random off-the-wall weapon. Not very smart right now
	if(skill == TANKSKILL_SNIPER || skill == TANKSKILL_GUNNER)
		SelectTargetWeapon();
	else
		SelectRandomWeapon();

	// If the last weapon fired was a "beam" weapon. Reset the default angle
	if(lastweaponused >= 0 && projectiles[lastweaponused].flags & MISSILE_BEAM)
		{
		if (targetx > x)
			deg = 75;
		else
			deg = 55;
		}

	// If we're shooting a 'sniper' weapon, aim directly at the target.
	// Do not take land into account
	if(projectiles[activeweapon].flags & MISSILE_BEAM)
		{
		deg = CalculateAngle(x, y, targetx, targety);
		}
	else
		{
		//aimret = Aim(targetx, targety);
		aimret = Aim(	barrelshotx,		// Origx
						barrelshoty,		// Origy
						targetx,			// Targetx
						targety,			// Targety	
						deg,				// Firing degree
						barrellength);		// Barrel length (if any)
					
		// Shot was rendered impossible, try adjusting our angle
		// slightly for the next shot
		if(aimret != 1)
			{
			if (deg >= 64)
				deg-=2;
			else
				deg+=2;
			}
		// Collect the two varibles that Aim() generated for the aim
		// trajectory
		lpower = bestpower;
		}
		if(projectiles[activeweapon].flags & MISSILE_BEAM)
			{
			if(targety==0)targety = 600;	// somewhere down low
			if(targety >= y)	// target below tank
				{
				deg = (targetx > x)?128:0;	// shoot at least horizontally
				//Printf("beam modify Deg%f\n",deg);
				}
			}

	// Equip shields?
setupshield:
	SetupShields();

	if(shieldidletime > 0)
		{
		shieldidletime--;
		return;
		}

	// Ready? Yeap. Done? Yup.  FIRE!
	shoot();
}	


//
// BuyDefense - Buys a defense item if the tank can afford it. Returns negative
// if the bot has insufficent funds
//

int		TANK::BuyDefense(int pnum, int amount)
{
	int		numamount=amount;
	while (numamount > 0)
		{
		if(money >= defenses[pnum].cost)
			{
			GiveDefense(indexnum, pnum, defenses[pnum].numrecieve);
			GiveMoney(-(defenses[pnum].cost));
			}
		else
			return -1;	// No more money

		numamount--;
		}

	return 1;
}


//
// BuyOffense - Buys a offense item if the tank can afford it.
// Returns negative if the bot has insufficent funds.
//

int		TANK::BuyOffense(int pnum, int amount)
{
	int		numamount=amount;

	// This item not listed? Bot use only? 
	// Find out if we can purchase this weapon or not.
	if ((projectiles[pnum].flags & MISSILE_NOLIST) && // weapon child -- nobody uses
		!(projectiles[pnum].flags & MISSILE_BOTUSEONLY))// Bots only are allowed to buy this item
		return 1;

	// Bots are not allowed to buy/use this weapon
	if (skill != TANKSKILL_HUMAN && projectiles[pnum].flags & MISSILE_NOBOTUSE)
		return 1;

	// Keep an eye out for character specific weapons
	if (projectiles[pnum].thischaracter >= 0) // is -1 for anybody
		{
		if (character != projectiles[pnum].thischaracter)
		return 1;
		}

	while (numamount > 0)
		{
		if (money >= projectiles[pnum].cost)
			{
			GiveAmmo(indexnum, pnum, projectiles[pnum].numrecieve);
			GiveMoney(-(projectiles[pnum].cost));
			}
		else
			return -1;	// No more money

		numamount--;
		}

	return 1;
}

//
// ComputerBuyStuff
//

void	TANK::ComputerBuyStuff(void)
{
	int		done = 1;
	int		rs;

	// "Stoopid" computer players dont NEED any defense! Muaha!
	if (skill > TANKSKILL_STOOPID)
	{
		// First off, buy some basic defense (10 batteries and parachutes) if we
		// do not already have them
		// Attempt to buy some batteries
		if (defense[battery_charge].amount < 10)
			BuyDefense(battery_charge, 2);
	
		// Attempt to buy some parachutes (starting at shooter skill)
		if (skill >= TANKSKILL_SHOOTER && defense[parachutes].amount < 3)
			BuyDefense(parachutes, 1);

		// Attempt to buy a floatation device ONLY if the next level has any water
		// Only elites like, snipers can make use of floatation devices
		//if (pwater->numparticles > 0 && skill == TANKSKILL_SNIPER)
		//	BuyDefense(floatation, 1);

		// Gunners will buy random HEB shields
		if (skill == TANKSKILL_GUNNER)
			BuyDefense(electric_shield+rand()%5, 1);

		if (skill == TANKSKILL_SNIPER)
		{
			// Fire protection
			if (h > e && h > b)
			{
				if (defense[fire_shield].amount <= 0)
				{ BuyDefense(fire_shield, 3); }
			}
			
			// Energy protection
			else if (e > h && e > b)
			{
				if (defense[electric_shield].amount <= 0)
				{ BuyDefense(electric_shield, 3); }
			}

			// Ballistic protection
			else if (b > h && b > e)
			{
				if (defense[ballistic_shield].amount <= 0)
				{ BuyDefense(ballistic_shield, 3); }
			}

			// Buy a random HEB shield
			else
			{
				rs = rand()%4;
				
				if (rs==0)
				{
					if (defense[fire_shield].amount <= 0)
					{ BuyDefense(fire_shield, 3); }
				}
				else if (rs==1)
				{
					if (defense[electric_shield].amount <= 0)
					{ BuyDefense(electric_shield, 3); }
				}
				else if (rs==2)
				{
					if (defense[ballistic_shield].amount <= 0)
					{ BuyDefense(ballistic_shield, 3); }
				}
				else
				{
					if (defense[superconventional_shield].amount <= 0)
					{ BuyDefense(superconventional_shield, 3); }
				}
			}

			// Snipers should always have a NIV shield ready to go
			if (defense[niv_shield].amount <= 0)
				BuyDefense(niv_shield, 1);
		}	
		
		if (skill == TANKSKILL_SHOOTER)
		{
			// Tanks will only buy conventional shields,
			// or else the game would most likely never end
			if (defense[conventional_shield].amount <= 0)
				BuyDefense(conventional_shield, 1);
		}

	}
	
	// Buy some random missiles, however many we can afford
	while ((BuyOffense(rand()%NUM_PROJECTILES, 1)) != -1)
	{ 
		// Save at least 5000 dollars
		if (money < 5000)
			break;
	}
}

//
// SelectRandomWeapon - Selects a random weapon of any kind
//

void	TANK::SelectRandomWeapon(void)
{
	int		done = -1;
	do
	{
		activeweapon = rand()%NUM_PROJECTILES;

		if (weapons[activeweapon].ammo > 0)
			done = 1;

		if (projectiles[activeweapon].flags & MISSILE_BEAM)
		{
			if (targety > y)
				done = -1;
		}

	} while (done == -1);

}


//
// AttemptSniperWeapon - Based on the computers skill, a sniper weapon
// such as the laser pulse and the laser blast will be used. The accuracy
// depends on the bots skill level
//

bool	TANK::AttemptSniperWeapon(int	wep)
{
	int		sniperweapon = -1;
	float	testangle;

	// This isn't a straight shot weapon
	if (!(projectiles[wep].flags & MISSILE_BEAM))
		return false;

	// Target isn't below us, shot is impossible
	testangle = CalculateAngle(x, y, targetx, targety+5);
	if (testangle > 128)	/// 128 equal 180 degrees
		return false;

	return true;
}


//
// SelectTargetWeapon - Selects the best weapon that will dish out the most
// damage to the bot's selected target based on tank and shield attributes
// 

void	TANK::SelectTargetWeapon(void)
{
	int		totalh,totale,totalb;
	int		highestdamage=0; // The most damage a missile can cause based on target protection
	int		selectedweapon;
	int		i;

	// Get the standard tank attributes
	totalh = ta[targettank].h;
	totale = ta[targettank].e;
	totalb = ta[targettank].b;

	// Get additonal shield attributes
	if(ta[targettank].energyleft > 0)
		{
		totalh += ta[targettank].shieldh;
		totale += ta[targettank].shielde;
		totalb += ta[targettank].shieldb;
		}
///	===========================================================
///	replaced by mod by Caspin
/*

	// They're more protected against heat attacks. Choose the best energy+ballistic weapon
	if(totalh > totale && totalh > totalb)
		{
		for(i=0; i<NUM_PROJECTILES; i++)
			{
			if(weapons[i].ammo <= 0)
				continue;

			// Found something that will do more damage
			if((projectiles[i].damagee+projectiles[i].damageb) >= highestdamage)
				{
				if((projectiles[i].flags & MISSILE_BEAM) && !AttemptSniperWeapon(i))
					continue;

				selectedweapon = i;
				highestdamage = (projectiles[i].damagee+projectiles[i].damageb);
				}
			}

		activeweapon = selectedweapon;
		}

	// They're protected against energy attacks. Choose the best heat+ballistic weapon.
	else if(totale > totalh && totale > totalb)
		{
		for(i=0; i<NUM_PROJECTILES; i++)
			{
			if(weapons[i].ammo <= 0)
				continue;

			// Found something that will do more damage
			if((projectiles[i].damageh+projectiles[i].damageb) >= highestdamage)
				{
				if((projectiles[i].flags & MISSILE_BEAM) && !AttemptSniperWeapon(i))
					continue;

				selectedweapon = i;
				highestdamage = (projectiles[i].damageh+projectiles[i].damageb);
				}
			}

		activeweapon = selectedweapon;
		}
	
	// They're protected against ballistic attacks. Choose the best heat+energy weapon.
	else if(totalb > totalh && totalb > totale)
		{
		for(i=0; i<NUM_PROJECTILES; i++)
			{
			if(weapons[i].ammo <= 0)
				continue;

			// Found something that will do more damage
			if((projectiles[i].damageh+projectiles[i].damagee) >= highestdamage)
				{
				if((projectiles[i].flags & MISSILE_BEAM) && !AttemptSniperWeapon(i))
					continue;

				selectedweapon = i;
				highestdamage = (projectiles[i].damageh+projectiles[i].damagee);
				}
			}

		activeweapon = selectedweapon;
		}
	// Can't decide?
	else
	{
		// See if a sniper weapon won't suffice
		SelectRandomWeapon();
	}
	*/
///	===========================================================
/*	Mod from Caspin
	Here is a better solution.
	It's simpler and more effective. Not only does it work for all cases it correctly handles weapons
	that have the damage spread between the three attributes, rather than just considering the best two. 
*/
	int damage;

	for( i=0; i<NUM_PROJECTILES; ++i ) 
		{ 
    if( weapons[i].ammo <= 0 ) continue;	// we don't have this weapon

    damage = projectiles[i].damageh*(10-totalh); 
    damage +=   projectiles[i].damagee*(10-totale); 
    damage +=   projectiles[i].damageb*(10-totalb); 

    // Found something that will do more damage 
    if( damage >= highestdamage) 
      { 
      if((projectiles[i].flags & MISSILE_BEAM) && !AttemptSniperWeapon(i)) 
        { continue; } 

      selectedweapon = i; 
      highestdamage = damage; 
      } 
		} 
///	===========================================================
	activeweapon = selectedweapon;

}

//
// SetupDefense - Sets up shields, checks batteries...etc
//

void	TANK::SetupDefense(void)
{
	int			pnum =-1;

	// We're either human controlled or a stupid bot
	if (skill <= 1)
		return;

	// Not at maximum health, use some batteries
	if (health < 100 && defense[battery_charge].amount > 0)
	{
		BatteryPowerUp(indexnum, battery_charge);

		// Let all other clients know whilst in multiplayer mode
		if (multiplayer && multiplayermode == MP_SERVER)
		{
			BCast_WriteByte(STOC_ACTIVATEDEFENSE);
			BCast_WriteByte(indexnum);
			BCast_WriteByte(battery_charge);
		}

		idletime=20;
		return;
	}
}


//
// SetupShields - Sets up shields, done instantly
//

void	TANK::SetupShields(void)
{
	int			pnum =-1;

	// We're either human controlled or a stupid bot
	if (skill <= 1)
		return;

	if (skill == TANKSKILL_SHOOTER && energyleft <= 0)
	{
		pnum = conventional_shield;

		// We have some! Activate!
		if (defense[pnum].amount > 0)
		{
			// Let all other clients know whilst in multiplayer mode
			if (multiplayer && multiplayermode == MP_SERVER)
			{
				BCast_WriteByte(STOC_ACTIVATEDEFENSE);
				BCast_WriteByte(indexnum);
				BCast_WriteByte(pnum);
			}

			// Activate the shield
			ShieldPowerUp(indexnum, pnum);
			shieldidletime = 40;
			return;
		}
	}
	else if (skill == TANKSKILL_GUNNER && energyleft <= 33)
	{
		// Pick any other available shield
		if (defense[fire_shield].amount > 0)
			pnum = fire_shield;

		else if (defense[electric_shield].amount > 0)
			pnum = electric_shield;

		else if (defense[ballistic_shield].amount > 0)
			pnum = ballistic_shield;

		else if (defense[superconventional_shield].amount > 0)
			pnum = superconventional_shield;

		else if (defense[conventional_shield].amount > 0)
			pnum = conventional_shield;

		else if (defense[niv_shield].amount > 0)
			pnum = niv_shield;

		// We have something! Activate!
		if (pnum != -1)
		{
			// Let all other clients know whilst in multiplayer mode
			if (multiplayer && multiplayermode == MP_SERVER)
			{
				BCast_WriteByte(STOC_ACTIVATEDEFENSE);
				BCast_WriteByte(indexnum);
				BCast_WriteByte(pnum);
			}

			// Activate the shield
			ShieldPowerUp(indexnum, pnum);
			shieldidletime = 40;
			return;
		}

	}
	else if (skill == TANKSKILL_SNIPER)
	{
		// Is the sniper about to shoot a "super" weapon? If so, equipt a NIV shield
		if ((projectiles[activeweapon].iconflags & MICON_SUPERBALLISTIC) ||
			(projectiles[activeweapon].iconflags & MICON_SUPERENERGY) ||
			(projectiles[activeweapon].iconflags & MICON_SUPERHEAT) ||
			(projectiles[activeweapon].iconflags & MICON_SUPERCONVENTIONAL))
		{
			if (activedefense != niv_shield && defense[niv_shield].amount > 0)
			{
				// Let all other clients know whilst in multiplayer mode
				if (multiplayer && multiplayermode == MP_SERVER)
				{
					BCast_WriteByte(STOC_ACTIVATEDEFENSE);
					BCast_WriteByte(indexnum);
					BCast_WriteByte(niv_shield);
				}

				// Activate the shield
				ShieldPowerUp(indexnum, niv_shield);
				shieldidletime = 40;
				return;
			}
		}
		if (energyleft <= 33)
		{
			// Check for a heat shield
			if (h > e && h > b && defense[fire_shield].amount > 0)
				pnum = fire_shield;

			// Check for a energy shield
			else if (e > h && e > b && defense[electric_shield].amount > 0)
				pnum = electric_shield;

			// Check for a ballistic shield
			else if (b > e && b > h && defense[ballistic_shield].amount > 0)
				pnum = ballistic_shield;

			else
			{
				// Pick any other available shield
				if (defense[fire_shield].amount > 0)
					pnum = fire_shield;

				else if (defense[electric_shield].amount > 0)
					pnum = electric_shield;

				else if (defense[ballistic_shield].amount > 0)
					pnum = ballistic_shield;

				else if (defense[superconventional_shield].amount > 0)
					pnum = superconventional_shield;

				else if (defense[conventional_shield].amount > 0)
					pnum = conventional_shield;

				else if (defense[niv_shield].amount > 0)
					pnum = niv_shield;

			}

			// We have something! Activate!
			if (pnum != -1)
			{
				// Let all other clients know whilst in multiplayer mode
				if (multiplayer && multiplayermode == MP_SERVER)
				{
					BCast_WriteByte(STOC_ACTIVATEDEFENSE);
					BCast_WriteByte(indexnum);
					BCast_WriteByte(pnum);
				}

				// Activate the shield
				ShieldPowerUp(indexnum, pnum);
				shieldidletime = 40;
				return;
			}
		}
	}
}


//
// AllPlayerBots - Tests to see if all the players are being controlled by the
// computer. If so, return true. This is mainly for skipping intermissions when
// we just want to let the game run on demo mode.
//

bool	AllPlayerBots(void)
{
	int		i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].skill == TANKSKILL_HUMAN)		// Just found a human player
			return false;
	}

	return true;
}
