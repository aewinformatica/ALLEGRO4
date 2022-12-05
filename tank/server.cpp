#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "dialogs.h"
#include "scorch.h"
#include "scripts.h"

// Server info
char			servername[32]="";
//char			masteraddr[]="cdirt.dyndns.org";
char			masteraddr[]="cdirt.jaj.com";
//char			masteraddr[]="24.123.75.84";	// jajmaster's master ip

int				serverport;
int				maxplayers;
int				ServerToMasterPort;


void	net_error(void)
{
	char		str[128];

	ustrcpy(str, net_getlasterror());

	G_Printf(str);
}


int		FindClient(netaddress_t	adr)
{
	int		i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (multiplayermode == MP_SERVER &&
			ta[i].exist <= 0)
			continue;

		if (net_compareadr(adr, ta[i].address))
			return i;
	}

	return -1;
}


int		FindNewClient(void)
{
	int		i;

	// Test for a full server
	if (multiplayer && numtanks() >= (int)num_players->value)
	{
		Printf("players in game: %d setting: %d\n", numtanks(), (int)num_players->value);
		return -1;
	}

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			return i;
	}

	return -1;
}

//
// SV_UpdateMaster - Sends information about the server. If 'localreply' is true, then
// the request came from another computer on a LAN. So, just reply to the incoming
// address (default=false)
//
void	SV_UpdateMaster(bool localreply)
{
	static char PrintLimit=0;	//	cut down on the volume of log messages.
	packet_t		*mbuf = new packet_t;
	netaddress_t	addr;

	mbuf->Clear();

	if(localreply)
		{
		if(PrintLimit%57 == 0)Printf("SV_UpdateMaster LAN request\n");
		mbuf->WriteByte(LAN_SENDSERVERINFO);
		}
	else
		{
		if(PrintLimit%57 == 0)Printf("SV_UpdateMaster Internet\n");
		net_stringtoadr(masteraddr, &addr);
		net_setport(addr, MASTER_PORT);
		mbuf->WriteByte(STOM_UPDATESERVER);
		}

	char	LevName[96];
	sprintf(LevName, "V1.0--%s", currentlevel.name);	//	Show version number in master server display

	mbuf->WriteShort(localport);		// local TCP port we're using
	mbuf->WriteString(server_name->string);
	mbuf->WriteString(LevName);
	mbuf->WriteByte(numtanks());
	mbuf->WriteByte(maxplayers);

	mbuf->WriteString(ta[net_localplayer].name);
	mbuf->WriteLong(cashstart->value);

	// Fire away!
	if(localreply)
		{
		udpsocket->SendPacket(net_incoming_adr, mbuf, PACKUDP);
///				{char zz[64]; sprintf(zz,"udpsocket->SendPacket(net_incoming_adr/n"); G_Printf(zz); }/// todo
		}
	else
		udpsocket->SendPacket(addr, mbuf, PACKUDP);

	delete mbuf;
	++PrintLimit;	//	this should wrap and print a message aapprox every 2 minutes
}


//
// SV_ShutdownServer - Closes all connections
//

void	SV_ShutdownServer(void)
{
	// Set some varibles
	multiplayer = 0;
	multiplayermode = -1;
	// use this for won't of something better
	for(int ii=0; ii<MAX_TANKS; ii++)
		{
		if(!ta[ii].exist) continue;
		SV_ClientError(ii, "Server has Shutdown");
		}
}

//
// SV_ClientError - Something happened on the server side and this particular client cannot proceed.
// Send a text message explaining what happened
//

void	SV_ClientError(int tank, char *msg)
{
	ta[tank].outbuf->WriteByte(STOC_ERRORTEXT);
	ta[tank].outbuf->WriteString(msg);

	//ta[tank].outbuf->WriteByte(MSG_EOP);

	ta[tank].netsock->SendPacket(ta[tank].address, ta[tank].outbuf);

	// Disconnect
	ta[tank].Disconnect();

	// debug mainly
	{
		char	str[256];
		sprintf(str, "%s experienced an error (%s)", ta[tank].name, msg);
		G_Printf(str);
	}

}

//
// SV_ClientError - Another varient, however this one is for clients who have not been
// assigned to a tank array varible yet

void	SV_ClientError(netsock_t *sock, netaddress_t addr, char *msg)
{
	packet_t	*p = new packet_t;

	p->Clear();
	p->WriteByte(STOC_ERRORTEXT);
	p->WriteString(msg);

	//p->WriteByte(MSG_EOP);

	sock->SendPacket(addr, p);

	// debug mainly
	{
		char	str[256];
		sprintf(str, "%s experienced an error (%s)", net_adrtostring(addr), msg);
		G_Printf(str);
	}

	closesocket(sock->sock);

	delete p;

}

//
//	SV_SendLocalState
//

void	SV_SendLocalState(int		i)
{
	BCast_WriteByte(STOC_UPDATETANKSTATE);
	BCast_WriteByte(i);
	BCast_WriteByte(ta[i].tankstate);

	SV_SendPackets();
}


bool	SV_AllPingReady(void)
{
	int		i;

//	if (checktanktimer > 0)
//		return false;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].pingwaitReady==false)
		{
			//Printf("%d is not ping ready!\n", i);
			return false;
		}
	}

	return true;
}


void	SV_ResetPingWait(void)
{
	int		i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

#ifndef	DEDICATED_SERVER
		if (i == net_localplayer)
		{
			ta[i].pingwaitReady = true;
			continue;
		}
#endif
		if (ta[i].skill != TANKSKILL_HUMAN)
			ta[i].pingwaitReady = true;
		else
		{
			// Those who are in setup do not count, players have to
			// be in the game
   			if (ta[i].netstate == NETSTATE_INGAME) // swapped from != for autodefense  // ???
				ta[i].pingwaitReady = true;
			else
				ta[i].pingwaitReady = false;
		}
	}
}

//
// SV_InitNewPlayer - Sets up a new player on the fly
//

void	SV_InitNewPlayer(int newta)
{
	ta[newta].init(newta, 0,0, 0,0,0,0);

	// Setup the new player and send em some info
	ta[newta].exist = 1;

	ta[newta].DefaultAngle();

	if (currentlevel.numshotsfired > 0)
	{
		ta[newta].spectator = 10;
		ta[newta].health = 0;
	}
	else
	{
		ta[newta].spectator = 0;
		ta[newta].health = 100;
	}

	ta[newta].pingwaitReady = true;

	ta[newta].needstodie = 0;

	// Reset the basics
	ta[newta].kills = 0;
	ta[newta].score = 0;
	ta[newta].money = 0;

	// Make sure tracking varibles are clear
	memset(ta[newta].weaponsused, 0, sizeof(ta[newta].weaponsused));
	memset(ta[newta].defenseused, 0, sizeof(ta[newta].defenseused));
	memset(ta[newta].specialsused, 0, sizeof(ta[newta].specialsused));

	// Add player in for next turn
	AddNextTurnQue(newta);

	// Setup HEB stats per their character
	ta[newta].SetHEB(characters[ta[newta].character].attrib_h,
				 characters[ta[newta].character].attrib_e,
				 characters[ta[newta].character].attrib_b);

	// Assign a team if we're in teamplay mode
	if (teamplay->value > 0)
		ta[newta].AutoAssignTeam();

	// If no shots have been fired so far on a map, assume the incoming client
	// will be joining right in (no spectator). Assign a tank position here
	if (currentlevel.numshotsfired <= 0)
	{
		// Find a spawn point
		ta[newta].FindMapPosition();

		ta[newta].Onground(ground.bmp);
	}


}


void	SV_InitNewClient(int tank)
{
	SV_InitNewPlayer(tank);

	ta[tank].lasttic = gametics;
	ta[tank].netstate = NETSTATE_CONNECTED;
	ta[tank].skill = TANKSKILL_HUMAN;

	// Player now exists in the game, keep track of them
	ta[tank].exist = 10;

	// Tell the client to setup their character
	ta[tank].outbuf->Clear();

	// Does this server require a passwored?
	if (requirepassword->value > 0)
	{
		ta[tank].outbuf->WriteByte(STOC_REQUESTPASSWORD);
		ta[tank].outbuf->WriteByte(tank);
	}
	else
	{
		ta[tank].outbuf->WriteByte(STOC_STARTPLAYERSELCECTION);
		ta[tank].outbuf->WriteByte(tank);						// Send the player number
		ta[tank].outbuf->WriteShort(ta[tank].team);
		if (teamplay->value > 0)
			ta[tank].outbuf->WriteByte(1);
		else
			ta[tank].outbuf->WriteByte(0);
	}

	netsocket->SendPacket(ta[tank].address, ta[tank].outbuf);
}


void	SV_TankThinker(void)
{
	netsock_t	*newsock=NULL;

	// Gotta be the server for this
	if (multiplayermode != MP_SERVER)
		return;

	// TCP stuff
	newsock = netsocket->AcceptConnection();

	if (newsock != NULL)
	{
		int		tank = -1;
		char	str[128];

		// Accept new connection
		tank = FindNewClient();

		// Invalid? No room for a new client then
		if (tank < 0)
		{
			SV_ClientError(newsock, net_incoming_adr, "Server full.");
			return;
		}

		ta[tank].exist = 10;
		ta[tank].netsock = newsock;
		ta[tank].address = net_incoming_adr;

		// Make sure this clien is not banned
		if (SV_EnforceBans(tank) > 0)
			return;

		SV_InitNewClient(tank);

		sprintf(str, "Connect: %s", net_adrtostring(ta[tank].address));
		G_Printf(str);
	}
}

//
// SV_ResetShotClock - Sends a signal to all clients to update
// their shotclocks
//

void	SV_ResetShotClock(int	timer)
{
	BCast_WriteByte(STOC_SHOTCLOCK);
	BCast_WriteLong(timer);
}


void	SV_ProcessPingUpdates(void)
{
	int		i;

	// See if pings need to be updated
	if (!(gametics %(TICRATE*5)))
	{
		for (i=0; i<MAX_TANKS; i++)
		{
			if (ta[i].exist <= 0 ||
				ta[i].skill != TANKSKILL_HUMAN)
			{ continue; }

			BCast_WriteByte(STOC_SENDPINGUPDATE);
			BCast_WriteByte(i);
			BCast_WriteLong(ta[i].ping);
			BCast_WriteLong(ta[i].framedelay);
		}
	}
}

//
// SV_UpdateClients - Sends all clients the current status of whose turn it
// is and other misc info
//

void	SV_UpdateClients(void)
{
	int		i,j;
	int		tflags=0;

	// Send any changed tank player info
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		// Reset
		tflags = 0;

		if (ta[i].x != ta[i].oldx)
			tflags |= TUPDATE_X;

		if (ta[i].y != ta[i].oldy)
			tflags |= TUPDATE_Y;

		if (ta[i].health != ta[i].oldhealth)
			tflags |= TUPDATE_HEALTH;

		if (ta[i].deg != ta[i].olddeg)
			tflags |= TUPDATE_DEG;

		if (ta[i].lpower != ta[i].oldlpower)
			tflags |= TUPDATE_POWER;

		if (ta[i].activeweapon != ta[i].oldactiveweapon)
			tflags |= TUPDATE_ACTIVEWEAPON;

		if (ta[i].flags != ta[i].oldflags)
			tflags |= TUPDATE_FLAGS;

		if (ta[i].specialflags != ta[i].oldspecialflags)
			tflags |= TUPDATE_SPECIALFLAGS;

		if (ta[i].energyleft != ta[i].oldenergyleft)
			tflags |= TUPDATE_ENERGY;

		if (ta[i].specials[fuel].amount != ta[i].oldfuel)
			tflags |= TUPDATE_FUEL;

		// Nothing changed
		if (tflags <= 0)
			continue;

		// Parse and find out what kind of info needs to be sent and send it to
		// all players
		for(j=0; j<MAX_TANKS; j++)
			{
			if(ta[j].exist <= 0 ||
				j == net_localplayer ||
				ta[j].skill != TANKSKILL_HUMAN)
				{ continue; }

			ta[j].outbuf->WriteByte(STOC_UPDATETANK);
			ta[j].outbuf->WriteByte(i);
			ta[j].outbuf->WriteShort(tflags);

			if(tflags & TUPDATE_X)
				ta[j].outbuf->WriteShort(ta[i].x);

			if(tflags & TUPDATE_Y)
				ta[j].outbuf->WriteShort(ta[i].y);

			if(tflags & TUPDATE_HEALTH)
				ta[j].outbuf->WriteShort(ta[i].health);

			if(tflags & TUPDATE_DEG)
				ta[j].outbuf->WriteFloat(ta[i].deg);

			if(tflags & TUPDATE_POWER)
				ta[j].outbuf->WriteFloat(ta[i].lpower);

			if(tflags & TUPDATE_ACTIVEWEAPON)
				{
				ta[j].outbuf->WriteByte(ta[i].activeweapon);
				ta[j].outbuf->WriteLong(ta[i].weapons[ta[i].activeweapon].ammo);
				}

			if(tflags & TUPDATE_FLAGS)
				ta[j].outbuf->WriteLong(ta[i].flags);

			if(tflags & TUPDATE_SPECIALFLAGS)
				ta[j].outbuf->WriteLong(ta[i].specialflags);

			if(tflags & TUPDATE_ENERGY)
				ta[j].outbuf->WriteLong(ta[i].energyleft);

			if(tflags & TUPDATE_FUEL)
				ta[j].outbuf->WriteLong(ta[i].specials[fuel].amount);

		}	// end for()

	}

	SV_ProcessPingUpdates();

	// Do not send 'tankturn' updates when someone is not in the
	// TANKSTATE_PLAYING state. (Eg, their in autodefense mode)
	//if (!SV_IsAllPlaying())
	//	return;

	// Send updated tank-turn and sprite info
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 ||
			ta[i].netstate < NETSTATE_INGAME ||
			ta[i].skill != TANKSKILL_HUMAN)
		{ continue; }

		if (i == net_localplayer)
			continue;

		ta[i].outbuf->WriteByte(STOC_UPDATETANKTURN);
		ta[i].outbuf->WriteByte(tankturn);
	}
}


//
// SV_GetCharacterSelection
//

void	SV_GetCharacterSelection(int tank)
{
	int		character = inbuf->ReadByte();
	char	*name = inbuf->ReadString();
	char	str[64];
	char	V1filename[128];


	// Keepalive the client
	ta[tank].lasttic = gametics;

	// Client is now loading the level and buying items
	ta[tank].netstate = NETSTATE_BUYSTUFF;

	// Copy the player name
	ustrcpy(ta[tank].name, name);

	// ...and character too
	ta[tank].character = character;
	ta[tank].money = cashstart->value;

	//	Add version number to file name.
	sprintf(V1filename,"V1.0*%s",currentlevel.filename);

#ifdef	_DEBUG
//  Degauss debug
Printf("Server STOC_SETUPLEVEL [SV_GetCharacterSelection] file name [%s]\n", V1filename);
#endif

	ta[tank].outbuf->Clear();
	ta[tank].outbuf->WriteByte(STOC_SETUPLEVEL);
	ta[tank].outbuf->WriteLong(cashstart->value);
	ta[tank].outbuf->WriteString(V1filename);
	ta[tank].outbuf->WriteShort(currentlevel.walltype);
	ta[tank].outbuf->WriteShort(currentlevel.gravityflags);

	// Joining in game or will the player become a spectator?
	if (currentlevel.numshotsfired <= 0)
		ta[tank].outbuf->WriteByte(0);
	else
		ta[tank].outbuf->WriteByte(1);

	ta[tank].outbuf->WriteByte(STOC_SHOTCLOCK);
	ta[tank].outbuf->WriteLong(tank_shottimer);

	ta[tank].outbuf->WriteByte(STOC_SYNC);	// sync gametics
	ta[tank].outbuf->WriteLong(gametics);		// helps in network debug (I hope?)


	// Fire!
	ta[tank].netsock->SendPacket(ta[tank].address, ta[tank].outbuf);

	AddNextTurnQue(tank);

	sprintf(str, "%s connecting as %s", ta[tank].name, characters[ta[tank].character].name);
	G_Printf(str);

	//	New Player has joined the game so update the inventory
	//	and send any script info
//	if(ScriptedGame)	//	Only if we are doing scripts.
		{
	//	BCast_WriteByte(STOC_Script);
	//	Write_InventInfo();
	//	SV_SendPackets();
		}
}

void	SV_SendTurnQue(int tank)
{
	int		i;
	ta[tank].outbuf->WriteByte(STOC_UPDATETURNQUE);

	for (i=0; i<MAX_TANKS; i++)
	{
		ta[tank].outbuf->WriteShort(tankturnque[i]);
	}
}

void	SV_SendFullUpdate(int tank)
{
	int		i;

	// First send all player info
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		ta[tank].outbuf->WriteByte(STOC_PLAYERINFO);
		ta[tank].outbuf->WriteByte(i);
		ta[tank].outbuf->WriteString(ta[i].name);
		ta[tank].outbuf->WriteByte(ta[i].character);
		ta[tank].outbuf->WriteByte(ta[i].skill);

		// Send color & team
		ta[tank].outbuf->WriteByte(ta[i].team);
		ta[tank].outbuf->WriteByte(ta[i].colorr);
		ta[tank].outbuf->WriteByte(ta[i].colorg);
		ta[tank].outbuf->WriteByte(ta[i].colorb);

		// Send x,y coords
		ta[tank].outbuf->WriteShort(ta[i].x);
		ta[tank].outbuf->WriteShort(ta[i].y);
		ta[tank].outbuf->WriteShort(ta[i].health);
		ta[tank].outbuf->WriteShort(ta[i].deg);
		ta[tank].outbuf->WriteByte(ta[i].spectator);

		// Send fuel info
		ta[tank].outbuf->WriteLong(ta[i].startfuel);
		ta[tank].outbuf->WriteLong(ta[i].specials[fuel].amount);
	}

	SV_SendTurnQue(tank);

	SV_SendFullSpriteUpdate(tank);

	// Send the current checktanktimer
	ta[tank].outbuf->WriteByte(STOC_SETPINGWAIT);
	ta[tank].outbuf->WriteLong(checktanktimer);
}


void	SV_SendFullSpriteUpdate(int tank)
{
	sprite_t*	s = sprites;

	//ta[tank].outbuf->WriteByte(STOC_FULLSPRITEUPDATE);
Printf("SV_SpawnSprite tank=%d\n", tank);	///Degauss debug

	// Secondly, send messages to spawn sprites client side
	while(s)
		{
		// World object, spawn on the client side
		if(s->wobj >= 0)
			{
			SV_SpawnSprite(s);
			}
		s = s->next;
		}
}


void	SV_GetPlayerInfo(int tank)
{
	int		tankcash = inbuf->ReadLong();
	char	str[64];
	int		i;
	int		turnque = -1;

	// Clear the buffer first
	ta[tank].outbuf->Clear();

	// Keepalive the client
	ta[tank].lasttic = gametics;

	// Setup how much cash this player has left
	ta[tank].money = tankcash;

	// Still in spectator?
	if (currentlevel.numshotsfired <= 0)
	{
		// If this is the only tank joining the game. Reset the turn que and set
		// the incoming player up to bat.
		if (numtanks() <= 1)
		{
			InitDefaultTurnQue();

			// Reset the shot clock
			Tank_ResetShotClock((int)shotclocktime->value);
		}

		ta[tank].spectator = 0;
	}
	else
		ta[tank].spectator = 10;

	// Find a spot in the turn que for our new friend if they are not a spectator
	// and joining right into the game
	if (currentlevel.numshotsfired <= 0 && numtanks() > 1)
	{
		// Add the bot into the current que
		for (i=0; i<MAX_TANKS; i++)
		{
			if (tankturnque[i] < 0)
			{
				tankturnque[i] = tank;
				turnque = i;
				break;
			}
		}
	}

	// Send a full update about the level/tank status..etc
	SV_SendFullUpdate(tank);

	// Send a simple header telling the client that the server is
	// ready for you to actually join in on the game loop.
	// Client must send a reply back before the server actually
	// does this
	ta[tank].outbuf->WriteByte(STOC_ENTERGAME);
	ta[tank].outbuf->WriteByte(tank);
	ta[tank].outbuf->WriteByte(tankturn);
	ta[tank].outbuf->WriteByte(ta[tank].spectator);
	ta[tank].outbuf->WriteByte(0);		// not waiting for autodefense

	// Send MOTD
	ta[tank].outbuf->WriteByte(STOC_MOTD);
	ta[tank].outbuf->WriteString(motd->string);

	// New connecting flags
	ta[tank].netstate = NETSTATE_INGAME;

	// Send the packet to the client
	ta[tank].netsock->SendPacket(ta[tank].address, ta[tank].outbuf);
	ta[tank].outbuf->Clear();


	// Update all other clients about the new player coming in
	// First send all player info
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 ||
			i == net_localplayer ||
			i == tank ||
			ta[i].skill != TANKSKILL_HUMAN)
			continue;

		ta[i].outbuf->WriteByte(STOC_PLAYERINFO);
		ta[i].outbuf->WriteByte(tank);
		ta[i].outbuf->WriteString(ta[tank].name);
		ta[i].outbuf->WriteByte(ta[tank].character);
		ta[i].outbuf->WriteByte(ta[tank].skill);

		// Send color & team
		BCast_WriteByte(ta[tank].team);
		BCast_WriteByte(ta[tank].colorr);
		BCast_WriteByte(ta[tank].colorg);
		BCast_WriteByte(ta[tank].colorb);

		// Send x,y coords
		ta[i].outbuf->WriteShort(ta[tank].x);
		ta[i].outbuf->WriteShort(ta[tank].y);
		ta[i].outbuf->WriteShort(ta[tank].health);
		ta[i].outbuf->WriteShort(ta[tank].deg);
		ta[i].outbuf->WriteByte(ta[tank].spectator);

		// Send fuel info
		ta[i].outbuf->WriteLong(ta[tank].startfuel);
		ta[i].outbuf->WriteLong(ta[tank].specials[fuel].amount);
	}

	// Tell everyone
	if (teamplay->value > 0)
	{
		if (ta[tank].team == TANKTEAM_RED)
			sprintf(str, "%s has joined the game on the RED team.", ta[tank].name);
		else
			sprintf(str, "%s has joined the game on the BLUE team.", ta[tank].name);
	}
	else
		sprintf(str, "%s has joined the game.", ta[tank].name);

	BCast_Printf(str);
}


void	SV_ClientReady(int tank)
{
	ta[tank].netstate = NETSTATE_INTERMISSIONREADY;
	ta[tank].money = inbuf->ReadLong();
	ta[tank].defense[parachutes].amount = inbuf->ReadLong();
	ta[tank].oldfuel = ta[tank].startfuel = ta[tank].specials[fuel].amount = inbuf->ReadLong();
	ta[tank].specials[autodefense].amount = inbuf->ReadLong();

	// Set the defense box flag until the player has sent the signal indicating
	// that they are finished with autodefense
	if (ta[tank].specials[autodefense].amount > 0)
		ta[tank].tankstate = TANKSTATE_DEFENSE;

	// Inform all other clients
	BCast_WriteByte(STOC_CLIENTREADY);
	BCast_WriteByte(tank);
}


//
// CL_SendUpdate - An ping heatbeat that is sent to all players to
// mainly calculate ping
//

void	SV_SendUpdate(void)
{
	if (!(gametics % TICRATE))
	{
		BCast_WriteByte(STOC_PINGUPDATE);
		BCast_WriteLong(I_GetTime());
		BCast_WriteLong(gametics);
	}
}


void	SV_ActivateDefense(int tank)
{
	int		def = inbuf->ReadByte();
	int		i;

	// Activate via whatever function specified
	if (defenses[def].activateproc)
		defenses[def].activateproc(tank, def);

	// Inform all other tanks
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 ||
			ta[i].skill != TANKSKILL_HUMAN ||
			i == tank ||
			i == net_localplayer)
		{ continue; }

		ta[i].outbuf->WriteByte(STOC_ACTIVATEDEFENSE);
		ta[i].outbuf->WriteByte(tank);
		ta[i].outbuf->WriteByte(def);
	}
}


void	SV_ActivateSpecial(int tank)
{
	int		i;
	int		spec = inbuf->ReadByte();

	// Activate via whatever function specified
	if (specials[spec].activateproc)
		specials[spec].activateproc(tank, spec);

	// Inform all other tanks
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 ||
			ta[i].skill != TANKSKILL_HUMAN ||
			i == tank ||
			i == net_localplayer)
		{ continue; }

		ta[i].outbuf->WriteByte(STOC_ACTIVATESPECIAL);
		ta[i].outbuf->WriteByte(tank);
		ta[i].outbuf->WriteByte(spec);
	}
}


//
// SV_GetChatMessage - Display a chat messgae from a particular tank
// and rebroadcasts it
//

void	SV_GetChatMessage(int tank)
{
	char	*msg = inbuf->ReadString();
	int		i;

	// Print it
	G_Printf(msg, tank);

	// Send to other clients
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 ||
			ta[i].skill > TANKSKILL_HUMAN ||
			i == tank ||
			ta[i].netstate < NETSTATE_INGAME)
		{ continue; }

		ta[i].outbuf->WriteByte(STOC_CHATMESSAGE);
		ta[i].outbuf->WriteByte(tank);
		ta[i].outbuf->WriteString(msg);
	}
}


void	SV_GetClientShot(int tank)
{
	// Get some varibles from the buffer
	int		activeweapon;
	float	lpower;
	int		deg;
	int		activeammo;

	deg = inbuf->ReadByte();
	lpower = inbuf->ReadFloat();
	activeweapon = inbuf->ReadByte();
	activeammo = inbuf->ReadLong();

	// Should never happen
	if (tank == net_localplayer)
	{
		G_Printf("*** net_localplayer tried to fire!");
		return;
	}

	// Dont allow a shot if its not their turn
	if (tank != tankturn)
		return;

	// Specify last weapon used
	ta[tank].weapons[activeweapon].ammo = activeammo;
	ta[tank].activeweapon = activeweapon;
	ta[tank].deg = deg;
	ta[tank].lpower = lpower;

	ta[tank].shoot();
}


void	SV_UpdateClientPing(int tank)
{
	int	framedelay = inbuf->ReadLong();
	int	tics = inbuf->ReadLong();

	//ta[tank].ping = gametics - tics;
	ta[tank].ping = I_GetTime() - tics;
	ta[tank].framedelay = gametics - framedelay;
}


void	SV_GetTankturnStats(int tank)
{
	int		tflags = inbuf->ReadShort();

	if (tflags == net_localplayer)
	{
		G_Printf("SV_GetTankturnStats: Called on localplayer!");
		return;
	}

	if (tflags & TUPDATE_DEG)
		ta[tank].deg = inbuf->ReadFloat();

	if (tflags & TUPDATE_POWER)
		ta[tank].lpower = inbuf->ReadFloat();

	if (tflags & TUPDATE_ACTIVEWEAPON)
	{
		ta[tank].activeweapon = inbuf->ReadByte();
		ta[tank].weapons[ta[tank].activeweapon].ammo = inbuf->ReadLong();
	}
}


void	SV_MoveTank(int tank)
{
	int	direction	= inbuf->ReadShort();

	if (direction <= 0 && ta[tank].specials[fuel].amount > 0)
	{
		if (ta[tank].move(-1,0) > 0)
			ta[tank].specials[fuel].amount--;

		lasttankturn = lasttankshot = -1;

		// We're moving
		ta[tank].moving = true;
	}

	if (direction > 0 && ta[tank].specials[fuel].amount > 0)
	{
		if (ta[tank].move(1,0) > 0)
			ta[tank].specials[fuel].amount--;

		lasttankturn = lasttankshot = -1;

		// We're moving
		ta[tank].moving = true;
	}

	// Let all other clients know about the move
	BCast_WriteByte(STOC_MOVETANK);
	BCast_WriteByte(tank);
	BCast_WriteShort(ta[tank].x);
	BCast_WriteShort(ta[tank].y);
	BCast_WriteLong(ta[tank].specials[fuel].amount);
}


bool	SV_IsAllPlaying(void)
{
	int		i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 ||
			ta[i].spectator > 0 ||
			ta[i].skill != TANKSKILL_HUMAN)
		{ continue; }

		if (ta[i].tankstate != TANKSTATE_PLAYING)
		{
			//Printf("%d not playing", i);
			return false;
		}
	}

	return true;
}



void	SV_CheckAutoDefenseClosed(void)
{
	int		i;

	// Send an update to all clients indicating what everyones state is
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		BCast_WriteByte(STOC_UPDATETANKSTATE);
		BCast_WriteByte(i);
		BCast_WriteByte(ta[i].tankstate);
	}

	// Check to see if all players are in the TANKSTATE_PLAYING
	// state. If so, send the signal to all clients to remove
	// the TANKSPECIAL_AUTODEFENSEWAIT flag to proceed with the
	// game
	if (SV_IsAllPlaying()==false)
		return;

	// Send the signal to all clients
	BCast_WriteByte(STOC_AUTODEFENSEFINISHED);

	// Reset the shot clock
	Tank_ResetShotClock((int)shotclocktime->value);

	// Remove the flag locally
	ta[net_localplayer].specialflags &= ~TANKSPECIAL_AUTODEFENSEWAIT;

	// Refresh tankturn
	tankturn = FindFirstTurn();
}


void	SV_AutoDefenseClosed(int tank)
{
	// Set new state when the auto-defense box has been closed
	ta[tank].tankstate = TANKSTATE_PLAYING;

	// Check to see if all players are finished now
	SV_CheckAutoDefenseClosed();
}


void	SV_UpdateTankState(int tank)
{
	int		state = inbuf->ReadByte();
	int		i;

	ta[tank].tankstate = state;

	// Update all other clients about this
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 ||
			ta[i].skill != TANKSKILL_HUMAN ||
			i == net_localplayer)
		{ continue; }

		ta[i].outbuf->WriteByte(STOC_UPDATETANKSTATE);
		ta[i].outbuf->WriteByte(tank);
		ta[i].outbuf->WriteByte(state);
	}
}


void	SV_FrameUpdateReply(int	tank)
{
	int		gtics = inbuf->ReadLong();

	ta[tank].outbuf->WriteByte(STOC_FRAMEUPDATE);
	ta[tank].outbuf->WriteLong(gtics);
}


void	SV_UpdateClientPingWait(int tank)
{
	bool	oldReady = ta[tank].pingwaitReady;

	ta[tank].pingwaitReady = true;

//	if (!oldReady && SV_AllPingReady())
 	if (!oldReady && SV_AllPingReady() && (checktanktimer <= 0) )
	{
		checktanktimer = 1;
		CheckTankDeaths();
	}
}

void	SV_VerifyClientPassword(int tank)
{
	char	*cpass = ustrlwr(inbuf->ReadString());
	char	*serverpass = ustrlwr(password->string);

	// Compare
	if (ustrcmp(cpass, serverpass) == 0)
	{
		// We have a match!
		ta[tank].outbuf->WriteByte(STOC_STARTPLAYERSELCECTION);
		ta[tank].outbuf->WriteByte(tank);						// Send the player number
		ta[tank].outbuf->WriteShort(ta[tank].team);
		if (teamplay->value > 0)
			ta[tank].outbuf->WriteByte(1);
		else
			ta[tank].outbuf->WriteByte(0);
	}
	else
	{
		// Sorry...
		SV_ClientError(tank, "Invalid password");
	}
}




void	SV_SendPackets(void)
{
	int		i;
	int		ret;
	char	str[128];

	// Send packets out to all clients
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || ta[i].skill != TANKSKILL_HUMAN || i == net_localplayer)
			continue;

		// Nothing to send
		if (ta[i].outbuf->size <= 0)
			continue;

		// Send
		ret = ta[i].netsock->SendPacket(ta[i].address, ta[i].outbuf);
		ta[i].outbuf->Clear();

		if (ret == SOCKET_ERROR)
		{
			ret = WSAGetLastError();
			sprintf(str, "Client %d -> Sendpacket(): Winsock error (%d)", i, ret);
			G_Printf(str);
		}
	}
}


//
// ParseHeaderPacket - When someone unknown sends packets, try and figure out if
// if they are another player trying to join or if its a launcher requesting server
// info. The server is the only one who needs this function
//  This isa UDP data stream...

void	ParseHeaderPacket(void)
{
	char	str[128];

	// Read byte header
	int	header = inbuf->ReadByte();

	// Anything...?
	switch(header)
		{
		case	LAN_REQUESTSERVERINFO:
			SV_UpdateMaster(true);
			break;

		// Wierd...
		default:
			{
			sprintf(str, "Unknown client challenge: %d", header);
			G_Printf(str);
			break;
			}
		}
}

void	Server_ParseData(int tank)
{
	char	str[128];
	int		msg, done = -1;
	int		lastmsg=-1;

	// Nothing to read
	if (inbuf->size <= 0)
		return;

	while (done <= 0)
	{
		msg = inbuf->ReadByte();

		// Done reading?
		if (msg <= 0)
		{
			done = 1;
			break;
		}

		// Find out what this means
		switch(msg)
		{
			// Client to server stuff
			case	CLIENT_CHALLENGE:		// <-- There in case of duplicate packets
				SV_InitNewClient(tank);
				break;

			case	CTOS_PLAYERSETUP:
				SV_GetCharacterSelection(tank);
				break;

			case	CTOS_GETPLAYERINFO:
				SV_GetPlayerInfo(tank);
				break;

			case	CTOS_INTERREADY:
				SV_ClientReady(tank);
				break;

			case	CTOS_DISCONNECTED:
				ta[tank].Disconnect();
				break;

			case	CTOS_REQUESTSPRITEUPDATE:
				SV_SendFullSpriteUpdate(tank);
				break;

			case	CTOS_UPDATEPING:
				SV_UpdateClientPing(tank);
				break;

			// Client to client messages
			//////////////////////////////////////////////
			case	CTOS_CHATMESSAGE:
				SV_GetChatMessage(tank);
				break;

			case	CTOS_ACTIVATEDEFENSE:
				SV_ActivateDefense(tank);
				break;

			case	CTOS_ACTIVATESPECIAL:
				SV_ActivateSpecial(tank);
				break;

			case	CTOS_MADESHOT:
				SV_GetClientShot(tank);
				break;

			case	CTOS_TANKTURNUPDATE:
				SV_GetTankturnStats(tank);
				break;

			case	CTOS_MOVETANK:
				SV_MoveTank(tank);
				break;

			case	CTOS_AUTODEFENSEDONE:
				SV_AutoDefenseClosed(tank);
				break;

			case	CTOS_UPDATETANKSTATE:
				SV_UpdateTankState(tank);
				break;

			case	CTOS_FRAMEUPDATE:
				SV_FrameUpdateReply(tank);
				break;

			case	CTOS_PINGWAITREADY:
				SV_UpdateClientPingWait(tank);
				break;

			case	CTOS_SWITCHTEAM:
				ta[tank].SwitchTeam();
				break;

			case	CTOS_SENDPASSWORD:
				SV_VerifyClientPassword(tank);
				break;

			case	MSG_EOP:
				break;//return;


			///////////////////////////////////////////////

			// Unknown
			default:
				{
					sprintf(str, "Unknown message: %d (last: %d)", msg, lastmsg);
					G_Printf(str);
					break;
				}
		}

		lastmsg = msg;
	}

}

void	SV_GetPackets(void)
{
	char		str[128];
	char		errortext[64];
	int			i;
	int			numbytes;
	int			err;
	bool		advturn = false;

	if(!multiplayer)
		return;

	for(i=0; i<MAX_TANKS; i++)
		{
		if(ta[i].exist <= 0 ||
			ta[i].skill != TANKSKILL_HUMAN ||
			i == net_localplayer)
		{ continue; }

		numbytes = ta[i].netsock->GetPacket();

		// Detect any errors encountered
		if(numbytes == SOCKET_ERROR)
			{
			err = WSAGetLastError();

			if(err == WSAECONNRESET)
				sprintf(errortext, "Connection reset by peer");
			else if(err == WSAETIMEDOUT)
				sprintf(errortext, "Socket timed out");
			else if(err == WSAECONNABORTED)
				sprintf(errortext, "Network Player has quit the game.");	//	"Software caused connection abort");
			else if(err == WSAENETRESET)
				sprintf(errortext, "Network dropped the connection on reset");
			else
				sprintf(errortext, "Unknown winsock error: %d", err);

			sprintf(str, "(%s) %s", ta[i].name, errortext);
			BCast_Printf(str);

			ta[i].Disconnect();

			advturn = true;

			continue;
			}

		if(numbytes <= 0)
			continue;


		// Keep client up to date
		ta[i].lasttic = gametics;

		// Parse packet
		Server_ParseData(i);
		}

		// UDP socket now
		numbytes = udpsocket->GetPacket(PACKUDP);
		if(numbytes > 0)
			{
//			G_Printf(" UDP socket read"); /// zz
			ParseHeaderPacket();
			}
}

void SV_SpawnSprite(sprite_t*	sp)
{
	BCast_WriteByte(STOC_SPAWNSPRITE);
	BCast_WriteShort(sp->netid);
	BCast_WriteByte(sp->wobj);
	BCast_WriteShort(sp->x);
	BCast_WriteShort(sp->y);
	BCast_WriteShort((int)sp->startstate.currentstate);
	BCast_WriteShort(sp->tankfrom);
	BCast_WriteShort(sp->health);
	BCast_WriteShort(sp->misc1);
	BCast_WriteShort(sp->misc2);
}