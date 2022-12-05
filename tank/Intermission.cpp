#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"
#include "editor/editor.h"
#include "secrets.h"

#define		INTRO_MSG		"Round Clear!"
#define		INTRO_COLOR		makecol(255,255,255)

#define		FINAL_MSG		"Final Results!"
#define		FINAL_COLOR		makecol(255,255,255)

BITMAP						*intermission_buffer;
int							inter_winner;
TANK						sortedta[MAX_TANKS];

int	exit_intermission(void)
{
	return D_EXIT;
}

int		d_intermission_button(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		InterDrawer();
		return D_O_K;
	}

	return d_agup_push_proc(msg, d, c);
}

int		d_intermission_joy(int msg, DIALOG * d, int c)
{
	if (!usejoystick)
		return D_O_K;

	// Poll joystick and get new info
	CheckJoyStick();

	if (IsJoyButtonDn(0, 8) || IsJoyButtonDn(0, 0))
		return D_EXIT;

	return D_O_K;
}


DIALOG dialog_intermission[] =
{
	{ d_intermission_joy,		0,       0,		0,	0,      0,	0,    0,	0,       0, 0,    NULL, NULL, NULL  },
    { d_intermission_button,	550,     456,  97,	20,   255,	0,    0,    D_EXIT,  0,			0,    (char *)"OK", 0, (void *)exit_intermission },
    { d_keyboard_proc,			0,       0,		0,	0,      0,	0,    0,	0,       KEY_ENTER, 0,    (void *)exit_intermission, NULL, NULL  },
	{ NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }
};

DIALOG dialog_finalintermission[] =
{
	{ d_intermission_joy,		0,       0,		0,	0,      0,	0,    0,	0,       0, 0,    NULL, NULL, NULL  },
    { d_intermission_button,	550,     456,  97,	20,   255,	0,    0,    D_EXIT,  0,			0,    (char *)"Continue", 0, (void *)exit_intermission },
	{ d_agup_push_proc,			440,     456,  97,	20,   255,	0,    0,    D_EXIT,  0,			0,    (char *)"Disconnect", 0, (void *)exit_intermission },
    { d_keyboard_proc,			0,       0,		0,	0,      0,	0,    0,	0,       KEY_ENTER, 0,    (void *)exit_intermission, NULL, NULL  },
	{ NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }
};


int Inter_FindWinner(void)
{
	int		winner=0, i;
	int		highscore=0;

	for (i=0; i<MAX_TANKS; i++)
	{
		// Dont count if not playing
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].score > highscore)
		{ 
			highscore = ta[i].score;
			winner=i; 
		}
	}

	return winner;
}


int Inter_FindTeamWinner(void)
{
	int		i=0;
	int		redscore=0;
	int		bluescore=0;

	// Not a team game?
	if (teamplay->value <= 0)
		return -1;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;
		
		if (ta[i].team == TANKTEAM_RED)
			redscore += ta[i].score;

		if (ta[i].team == TANKTEAM_BLUE)
			bluescore += ta[i].score;
	}

	if (redscore > bluescore)
		return TANKTEAM_RED;
	if (bluescore < redscore)
		return TANKTEAM_BLUE;

	// A tie?!
	if (redscore == bluescore)
		return -2000;

	// We should never get here
	return -1;
}



int		Inter_GetRedScore(void)
{
	int		i;
	int		redscore=0;

	// Not a team game?
	if (teamplay->value <= 0)
		return -1;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;
		
		if (ta[i].team == TANKTEAM_RED)
			redscore += ta[i].score;
	}

	return redscore;
}


int		Inter_GetBlueScore(void)
{
	int		i;
	int		bluescore=0;

	// Not a team game?
	if (teamplay->value <= 0)
		return -1;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;
		
		if (ta[i].team == TANKTEAM_BLUE)
			bluescore += ta[i].score;
	}

	return bluescore;
}


bool	Inter_AllReady(void)
{
	int		i;

	if (!multiplayer)
		return true;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].netstate != NETSTATE_INTERMISSIONREADY)
			return false;
	}

	// Everyone is ready
	return true;
}


bool	Inter_CheckForAD(void)
{
		int		i;

	if (!multiplayer)
		return true;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].specials[autodefense].amount > 0)
			return true;
	}

	return false;
}


/*static int compare (const void *arg1, const void *arg2)
{
	return (*(TANK **)arg2).score - (*(TANK **)arg1).score;
}*/

void	Inter_SetupScoreBoard(void)
{
	// Find a winner first
	if (numrounds == 0)
		inter_winner = Inter_FindWinner();

	memcpy(sortedta, ta, sizeof(ta));

	//qsort (sortedta, MAX_TANKS, sizeof(ta), compare);
}


void Inter_Refresh(void)
{
	char	buf[64];
	int		x=140;
	int		y=170;
	int		i;

	//show_mouse(NULL);

	if (gamestate == GS_INTERWAIT)
		clear_to_color(buffer, 0);

	if (numrounds == 0)
	{
		set_trans_blender(ta[inter_winner].colorr, ta[inter_winner].colorg, ta[inter_winner].colorb, 0);
		draw_lit_sprite(buffer, (BITMAP *)maindata[INTERMISSION_BACK].dat, 106, 85, 128);

		// Toss some title text
		textout_centre(buffer, (FONT *)maindata[TAHOMA].dat, FINAL_MSG, 287+5, 103+5, makecol(0,0,0));
		textout_centre(buffer, (FONT *)maindata[TAHOMA].dat, FINAL_MSG, 287, 103, FINAL_COLOR);
	}
	else
	{
		// Setup the background
		draw_sprite(buffer, (BITMAP *)maindata[INTERMISSION_BACK].dat,
					106,85);

		// Toss some title text
		textout_centre(buffer, (FONT *)maindata[TAHOMA].dat, INTRO_MSG, 287+5, 103+5, makecol(0,0,0));
		textout_centre(buffer, (FONT *)maindata[TAHOMA].dat, INTRO_MSG, 287, 103, INTRO_COLOR);
	}
	

	textout(buffer, (FONT *)maindata[MS].dat, "Player", x, y, INTRO_COLOR);
	textout(buffer, (FONT *)maindata[MS].dat, "Kills", x+315, y, INTRO_COLOR);
	textout(buffer, (FONT *)maindata[MS].dat, "Score", x+415, y, INTRO_COLOR);

	
	// More than 1 round left
	if (numrounds > 1)
	{
		sprintf(buf, "%d rounds remain!", numrounds);
		textout(buffer, font, buf, x+215, 85+380, INTRO_COLOR);
	}
	else if (numrounds == 1) // 1 Round Remains
	{
		sprintf(buf, "Approaching FINAL round!");
		textout(buffer, font, buf, x+215, 85+380, INTRO_COLOR);
	}
	else if (numrounds == 0) // End of game
	{
		sprintf(buf, "Congratulations %s!", ta[inter_winner].name);
		textout(buffer, font, buf, x+115, 85+380, INTRO_COLOR);
	}

	// Increase the font indent to make room and display which
	// players are ready or not
	if (multiplayer)
		x += 25;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (sortedta[i].exist)
		{
			y += text_height((FONT *)maindata[MS].dat);

			if (multiplayer && 
				(ta[i].netstate == NETSTATE_INTERMISSIONREADY || (i == net_localplayer && joinstate == JOIN_INTERREADY)))
			{
				// Ready?
				sprintf(buf, "r");
				textout(buffer, (FONT *)maindata[MS].dat, buf, x-25, y, 
					makecol(0,200,0));
			}
			
			// Print their name
			sprintf(buf, "%s", ta[i].name);
			textout(buffer, (FONT *)maindata[MS].dat, buf, x+2, y+2, 
				makecol(0,0,0));

			textout(buffer, (FONT *)maindata[MS].dat, buf, x, y, 
				makecol(ta[i].colorr, ta[i].colorg, ta[i].colorb));

			// Print the # of kills
			sprintf(buf, "%d", ta[i].kills);
			textout(buffer, (FONT *)maindata[MS].dat, buf, x+2+315, y+2, 
				makecol(0,0,0));

			textout(buffer, (FONT *)maindata[MS].dat, buf, x+315, y, 
				makecol(ta[i].colorr, ta[i].colorg, ta[i].colorb));

			// Print the score
			sprintf(buf, "%d", ta[i].score);
			textout(buffer, (FONT *)maindata[MS].dat, buf, x+2+415, y+2, 
				makecol(0,0,0));

			textout(buffer, (FONT *)maindata[MS].dat, buf, x+415, y, 
				makecol(ta[i].colorr, ta[i].colorg, ta[i].colorb));
		

			y+=2;
		}
	}

	// Gamelines thinker & drawer
	GameLinesThinker();
	GameLinesDrawer(buffer);

	if (multiplayer)
	{

		// Draw how much time is left
		if (tank_shottimer > 0)
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
			textout_centre(buffer, (FONT *)maindata[MS].dat, str, (SCREENWIDTH/2)+1, 1, 0);
			textout_centre(buffer, (FONT *)maindata[MS].dat, str, SCREENWIDTH/2, 0, makecol(255,255,255));
			}


		// The enter (or return) key displays the message box
		// allowing the player to say something
		if (IsKeyDn(KEY_ENTER) && IsTankMessage() <= 0)
			Start_TankMessage();
	
		if (IsTankMessage() > 0)
		{ TankMessage_Input(); }

		if (IsTankMessage() > 0)
		{ TankMessage_Drawer(buffer); }
	}
}

void	InterDrawer(void)
{

	if (update_count <= 0)
		return;

	update_count = 0;

	if (multiplayer)
		InterTicker();

	Inter_Refresh();

	draw_sprite(buffer, intermission_buffer, 0,0);

	ticker();
	gametics++;
	frame_count++;

	Screen_Blit(true);
}


void	InterTicker(void)
{
	int		i;

	if(multiplayer)
		{
		// Get tank messages
		if(IsTankMessage() > 0)
			{ TankMessage_Input(); }

		// Routine for both master and server to get packets
		if(multiplayermode == MP_CLIENT)
			CL_GetPackets();
		else
			SV_GetPackets();
		
		// Server loop		
		if(multiplayermode == MP_SERVER && gamestate == GS_INTERWAIT)
			{
			// Keep alive all clients
			SV_ProcessPingUpdates();

			// See if everyone is ready, ( checks NETSTATE_INTERMISSIONREADY )
			// if so, jump into the game
			if(Inter_AllReady())
				{
				// Reset the shot clock
				//SV_ResetShotClock(tank_shottimer);
				Tank_ResetShotClock((int)shotclocktime->value);

				// Check if any tank has AutoDefense
				bool autod = Inter_CheckForAD();
				// Send a message to all clients telling them to join the game
				for (i=0; i<MAX_TANKS; i++)
					{
					if(ta[i].exist <= 0 ||
						i == net_localplayer)
						{ 
						if(i == net_localplayer)
							{
							ta[i].netstate = NETSTATE_INGAME;
							ta[i].tankstate = TANKSTATE_PLAYING;
							}

						continue; 
						}

					// Set bots into the "playing" state so that we're not waiting for
					// autodefense from them
					if(ta[i].skill != TANKSKILL_HUMAN)
						{
						ta[i].tankstate = TANKSTATE_PLAYING;
						continue;
						}

					// Send full update
					SV_SendFullUpdate(i);

					ta[i].outbuf->WriteByte(STOC_ENTERGAME);
					ta[i].outbuf->WriteByte(i);
					ta[i].outbuf->WriteByte(tankturn);
					ta[i].outbuf->WriteByte(0);		// not in spectator mode

					// Send auto-defense flag?
					//if (Inter_CheckForAD())
					if(autod)
						ta[i].outbuf->WriteByte(10);
					else
						ta[i].outbuf->WriteByte(0);

					if(ta[i].specials[autodefense].amount > 0)
						ta[i].tankstate = TANKSTATE_DEFENSE;

					// Set net-state
					ta[i].netstate = NETSTATE_INGAME;
					}

				// Set new gamestate & enter the game!
				gamestate = GS_INGAME;

				// Actually physically send the updates etc.
				if(multiplayermode == MP_SERVER)
					SV_SendPackets();
				else
					CL_SendPackets();

				// if (Inter_CheckForAD())
				if(autod) // set up local player
					{
#ifndef	DEDICATED_SERVER
					ta[net_localplayer].tankstate = TANKSTATE_DEFENSE;
#endif

					SV_CheckAutoDefenseClosed();

#ifndef	DEDICATED_SERVER
					ta[net_localplayer].specialflags |= TANKSPECIAL_AUTODEFENSEWAIT;
#endif
					}
				
				// Check now for auto defense for the server. If the host has
				// autodefense, enable it here
#ifndef	DEDICATED_SERVER
				if(ta[net_localplayer].specials[autodefense].amount > 0)
					{
					// Minus one from inventory
					ta[net_localplayer].specials[autodefense].amount--;
	
					tankturn = net_localplayer; // Why is this statment here???
					StartDefenseBox(net_localplayer);

					// Refresh
					ground.needsupdate = true;
					}
				// Just to make sure
				ta[net_localplayer].tankstate = TANKSTATE_PLAYING;
				ta[net_localplayer].netstate = NETSTATE_INGAME;

				// Check and see if it is good to continue
				SV_CheckAutoDefenseClosed();
#endif
				}
			}

		if(multiplayermode == MP_SERVER)
			{
			SV_SendPackets();
			}
		else
			CL_SendPackets();

		if(tank_orginalshottimer > 0)
			{
			// Shave away intermission tics
			tank_shottimer--;
	
			// Time is up, automatically make ourselves "ready" for the next
			// round
			if(tank_shottimer < 0)
					agup_break_dialog();
			}

		// Refresh the ground again
		ground.needsupdate = true;
		}
}


void	Inter_Run(char	*filename)
{
	int		i,t;
	int		ret=0;

#ifdef	_DEBUG
//  Degauss debug
Printf("Inter_Run(%s)\n", filename);
#endif

	// Load the next level now so this information can be relayed 
	// when sending info to other clients via multiplayer
	if(numrounds > 0)	// Degauss: Was 1 but last level was never loaded..?
		{
		if(!filename)
			LoadLevel(NULL);
		else
			LoadLevel(filename);
	
		// Init next que
		if(!multiplayer || multiplayermode == MP_SERVER)
			InitNextTurnQue();
		}

	// Stop all sounds
	StopAllSounds();

	// Create some temp bitmaps
	intermission_buffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(intermission_buffer, TRANSCOLOR);

	// Set new gamestate
	gamestate = GS_INTERMISSION;

	// Play a tune
	PlayLevelOGG("roundclear.mp3");

	clear_keybuf();

	// Reset shotclock manually
	if(!multiplayer || multiplayermode == MP_SERVER)
		{
		tank_shottimer = (int)intermissiontime->value;
		}

	// Set all clients into the intermission state
	if(multiplayer)
		{
		for(i=0; i<MAX_TANKS; i++)
			{
			// Set into intermission mode
			ta[i].netstate = NETSTATE_INTERMISSION;

			// End spectator mode here
			ta[i].spectator = 0;
			
			// The server automatically readies any computer players
			if(multiplayermode == MP_SERVER
			 && ta[i].exist > 0 
			 && ta[i].skill != TANKSKILL_HUMAN)
				{ 
				ta[i].netstate = NETSTATE_INTERMISSIONREADY; 
				ta[i].tankstate = TANKSTATE_PLAYING;

				// Inform all other clients that this bot is ready
				BCast_WriteByte(STOC_CLIENTREADY);
				BCast_WriteByte(i);
				}

			// Send all other human players the signal to end the level
			if(multiplayermode == MP_SERVER
			  && ta[i].exist > 0
			  && ta[i].skill == TANKSKILL_HUMAN
			  && i != net_localplayer)
				{
				// Clear the buffer
				ta[i].outbuf->Clear();

				// Tell the client all the other players' stats
				for(t=0; t<MAX_TANKS; t++)
					{
					if(ta[t].exist <= 0)
						continue;
	
					ta[i].outbuf->WriteByte(STOC_UPDATETANKSCORE);
					ta[i].outbuf->WriteByte(t);
					ta[i].outbuf->WriteShort(ta[t].kills);
					ta[i].outbuf->WriteLong(ta[t].score);
					ta[i].outbuf->WriteByte(ta[t].netstate);
					}

				// Send updated shotclock info...only this time its
				// extra time for the intermission
				ta[i].outbuf->WriteByte(STOC_SHOTCLOCK);
				ta[i].outbuf->WriteLong((int)intermissiontime->value);

				// Send level info
				if(numrounds == 0)
					{
					ta[i].outbuf->WriteByte(STOC_PRINTF);
					ta[i].outbuf->WriteString("FINALINTERMISSION\n");
					Printf ("FINALINTERMISSION\n");

					ta[i].outbuf->WriteByte(STOC_FINALINTERMISSION);
					ta[i].Disconnect();
					continue;
					}
				else
					{
					//	Add version number to file name.
					char	V1filename[128];
					sprintf(V1filename,"V1.0*%s",currentlevel.filename);

#ifdef	_DEBUG
//  Degauss debug
Printf("Server [Inter_Run write to network] file name [%s]\n", currentlevel.filename);
#endif
					ta[i].outbuf->WriteByte(STOC_STARTINTERMISSION);
					ta[i].outbuf->WriteShort(numrounds-1);
					ta[i].outbuf->WriteLong(ta[i].money);
					ta[i].outbuf->WriteString(V1filename);
					ta[i].outbuf->WriteShort(currentlevel.walltype);
					ta[i].outbuf->WriteShort(currentlevel.gravityflags);
					}

				// Fire away!
				ta[i].netsock->SendPacket(ta[i].address, ta[i].outbuf);
				}
			}
		}

	// Remove 1 round from the que here.
	if(!multiplayer || multiplayermode == MP_SERVER)
		numrounds--;

	Printf("%d rounds left!\n", numrounds);
	
	Inter_SetupScoreBoard();

#ifndef	DEDICATED_SERVER
	if(!AllPlayerBots() || numrounds == 0)
		{
		if (numrounds > 0 || !multiplayer)
			do_agup_dialog(dialog_intermission, intermission_buffer, InterDrawer);
		else
			{ // Second, ShadowFang plumb forgot to call the code that checks if you should get a secret character
			ret = do_agup_dialog(dialog_finalintermission, intermission_buffer, InterDrawer);
      if(!multiplayer && numrounds == 0) 
        if(currentlevel.secretcharflag>-1) 
          { 
          int human=-1; 
          for(int cz=0; cz<MAX_TANKS; cz++) 
            { 
            if (ta[cz].exist == 0) 
                continue; 

            if(ta[cz].skill == TANKSKILL_HUMAN) 
              { 
              human = cz; 
              } 
            } 
          // Human player must win the game 
          if (inter_winner != human) goto q;  // is this required?????
          UnlockCharacter(currentlevel.secretcharflag,TRUE); 
          } 
        else 
            CheckForUnlockable(); 
q:    // Second, ShadowFang plumb forgot to call the code that checks if you should get a secret character
			// Continue, don't disconnect
			if(ret == 1)
				{
				if(multiplayermode == MP_CLIENT)
					{
					// Reconnect
					// Joining a game, make sure the socket is in non-listen mode
					ResetSocket(false);

					AttemptJoinConnection(&serveraddr);
					return;
					}
				else
					{
					// Server will just simply reset the number of rounds that are to be played
					numrounds = num_rounds->value;

					for(i=0; i<MAX_TANKS; i++)
						{
						if(ta[i].exist <= 0)
							continue;
						
						// Reset ALL stats
						ta[i].money = cashstart->value;
						ta[i].kills = 0;
						
						// Reset all tracking varibles
						memset(ta[i].weaponsused, 0, sizeof(ta[i].weaponsused));
						memset(ta[i].defenseused, 0, sizeof(ta[i].defenseused));
						memset(ta[i].specialsused, 0, sizeof(ta[i].specialsused));
						
						ta[i].score = 0;
						ta[i].startfuel = 0;
						ta[i].specials[fuel].amount = 0;
						ta[i].defense[parachutes].amount = 0;
						ta[i].health = 100;

			
						memset(ta[i].weapons, 0, sizeof(playerweapons_t)); // this only clears the first entry??
						memset(ta[i].defense, 0, sizeof(playerdefense_t));

						GiveAmmo(i, W_BABYMISSILE, UNLIMITED_AMMO);

						ta[i].activeweapon = W_BABYMISSILE;
						}
        LoadLevel(NULL);
				}
			}
		}
	}
#endif

	// Free memory
	destroy_bitmap(intermission_buffer);

#ifdef	DEDICATED_SERVER
	if(numrounds <= 0)
		{
		numrounds = num_rounds->value;
		Printf("Resetting rounds to %d for a new game\n", numrounds);
		}
#endif

	// Start a new level if we have some more rounds to run!
	if(numrounds > 0)
		{
		PlayLevelOGG("playermenu.ogg");

		// Cycle through and buy some stuffage
		StartBuyStuff();
		
		// Begin level (dont reset shot clock) ...yet
		StartLevel(false);

		if(multiplayer && multiplayermode == MP_SERVER)
			{
#ifndef	DEDICATED_SERVER
			// Inform all other players that we're done
			BCast_WriteByte(STOC_CLIENTREADY);
			BCast_WriteByte(net_localplayer);
	
			// Flag ourselves as ready
			ta[net_localplayer].netstate = NETSTATE_INTERMISSIONREADY;
#endif
			// Wait for other clients
			gamestate = GS_INTERWAIT;
			}
		}
	else
		{
		// Disconnect socket on multiplayer
		if(multiplayer)
			CL_Disconnect();
		else
			{
			// This is in single player mode. Check for special instances that will
			// trigger the challenge of a secret character
			/*for (t=0; t<NUM_PROJECTILES; t++)
			{
				if (ta[i].weaponsused[t] > 0)
					Printf("%s fired %s %d times\n", ta[i].name, projectiles[t].name, ta[i].weaponsused[t]);
			}*/

			// Finished a secret level? 
			if(currentlevel.secretcharflag >= 0)
				{
				if(ta[inter_winner].skill == TANKSKILL_HUMAN)
					UnlockCharacter(secretinstances[currentlevel.secretcharflag].character, true);
				}
			
			if(CheckForUnlockable() > 0)
				return;
			}

		// Stop any playing music
		StopLevelOGG();

		// Return to the main menu
		show_mouse(NULL);
		gamestate = GS_TITLE;
		GUI_SetupMenu (main_menu);
		}
}


void	Inter_CheckGame(bool	advturn)
{
	char	text[1024];
	int			i;

	// Don't check if tanks are still animating death sequences
	if(IsTankDeathSeq() > 0)
		return;

	// Only one player is in the game, don't allow the level to change
	// in multiplayer
	if(multiplayer && numtanks() == 1 && numalivetanks() == 1)
		return;

	if(teamplay->value > 0 && numaliveteam(TANKTEAM_RED) >= 1 && numaliveteam(TANKTEAM_BLUE) >= 1)
		{
		if(advturn)
			advanceturn();	//	teamplay advance
		}
	else if(teamplay->value <= 0 && numalivetanks() > 1)
		{ 
		if(advturn)
			advanceturn();	//	single player advance
		}
	else
		{
		// Find the last tank standing (if there is one)
		// And award 125 pts and $10,000 for being the last
		// soul alive
		char star;
		for (i=0; i<MAX_TANKS; i++)
			{
			if(ta[i].exist <= 0)
				continue;

			// Our winner!
			star = ' ';
			if(ta[i].health > 0 && ta[i].needstodie <= 0)
				{
				// There has to be at least 2 players in the game actually
				// playing for end points/money to be rewarded
				if(numingametanks() > 1)
					{
					// Give the winner extra cash and extra points
					ta[i].GiveMoney(MONEY_ROUNDWIN);
					ta[i].GivePoints(125);
					star = '*';	//	****** End of Round Winner *****
					}

				// Add the winning player to the end of the que for the
				// next round
				AddNextTurnQue(i);
				}
			else
				{
				// Same rule as above. We gotta have at least 2 players to award
				// points/money
				if(numingametanks() > 1)
					ta[i].GiveMoney(MONEY_NOROUNDWIN);
				}

			sprintf(text,"%c Kills:%5d\tScore:%5d\t$:%7d\t\t%s\n",star, ta[i].kills, ta[i].score, ta[i].money, ta[i].name);
			BCast_WriteByte(STOC_PRINTF);
			BCast_WriteString(text);
			Printf ("%s\n", text);
			}
		
		Inter_Run();
	}
}
