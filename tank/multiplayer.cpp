#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#include <allegro.h>
#include "dialogs.h"
#include "scorch.h"
#include "peffect.h"
#include "particles.h"
#include "editor\editor.h"

netsock_t		*netsocket;
netsock_t		*udpsocket;
int				localport;
int				localudpport;
int				gametime;
int				nettics;
int				pingwaitstate;
int				pingwaittime;

cvar_t			*num_players;
cvar_t			*num_rounds;
cvar_t			*server_name;
cvar_t			*teamplay;
cvar_t			*friendlyfire;
cvar_t			*motd;
cvar_t			*password;
cvar_t			*requirepassword;
cvar_t			*forcetime;

//
// I_GetTime - Stole--ah, "borrowed" from ZDaemon.  =)
//
static	DWORD	basetime = 0;

unsigned int I_GetTime (void)
{
	DWORD tm;

	tm = timeGetTime();
	if (!basetime)
		basetime = tm;

	return tm - basetime;
}

//
// I_GetTime
// returns time in 1/60th second tics
//
int I_GetTimePolled (void)
{
	DWORD tm;

	tm = timeGetTime();
	if (!basetime)
		basetime = tm;

	return ((tm-basetime)*60)/1000;
}


int I_WaitForTicPolled (int prevtic)
{
	int time;

	while ((time = I_GetTimePolled()) <= prevtic)
		;

	return time;
}


//
// D_ConsoleInput - [NightFang] - pulled from the old 0.99 code
//
char *D_ConsoleInput (void)
{
#ifndef	WIN32
	static 	char text[256];
	int	len;
	if (!stdin_ready || !do_stdin)
	{ return NULL; }

	stdin_ready = 0;

	len = read(0, text, sizeof(text));
	if (len < 1)
	{ return NULL; }

	text[len-1] = 0;

	return text;
#else
	
// Windows code
	static char     text[256];
    static int              len;
    int             c;

    // read a line out
    while (_kbhit())
    {
		c = _getch();
        putch (c);
        if (c == '\r')
        {
			text[len] = 0;
            putch ('\n');
            len = 0;
            return text;
        }
        
		if (c == 8)
        {
			if (len)
            {
				putch (' ');
                putch (c);
                len--;
                text[len] = 0;
            }
            continue;
        }
    
		text[len] = c;
        len++;
        text[len] = 0;
        if (len == sizeof(text))
		    len = 0;
	}

    return NULL;
#endif
}


void	InitNetwork(void)
{
	char	str[128];

	// Setup a few cvars
	num_players = Cvar_Get ("num_players", "8", CVAR_NORMAL|CVAR_ARCHIVE);
	num_rounds = Cvar_Get ("num_rounds", "10", CVAR_NORMAL|CVAR_ARCHIVE|CVAR_GAMELOCK);
	server_name = Cvar_Get ("server_name", "Untitled", CVAR_NORMAL|CVAR_ARCHIVE);
	teamplay = Cvar_Get ("teamplay", "0", CVAR_NORMAL|CVAR_ARCHIVE|CVAR_GAMELOCK);
	friendlyfire = Cvar_Get ("friendlyfire", "0", CVAR_NORMAL|CVAR_ARCHIVE);
	motd = Cvar_Get ("motd", "Welcome to Charred Dirt!<br><br>Happy hunting!", CVAR_NORMAL);
	password = Cvar_Get ("password", "default", CVAR_NORMAL);
	requirepassword = Cvar_Get ("requirepassword", "0", CVAR_NORMAL|CVAR_ARCHIVE);
	forcetime = Cvar_Get ("forcetime", "0", CVAR_NORMAL|CVAR_ARCHIVE);

	// Add a few commands
	Cmd_AddCommand ("killban", Cmd_Killban_f);
	Cmd_AddCommand ("addban", Cmd_Addban_f);
	Cmd_AddCommand ("banlist", Cmd_BanList_f);
	Cmd_AddCommand ("kick", Cmd_KickPlayer_f);
	Cmd_AddCommand ("playerlist", Cmd_ListPlayers_f);

	// Attempt to exec "server.cfg"
#ifdef	DEDICATED_SERVER
	Cbuf_AddText("exec server.cfg\n");
#endif

	// Init winsock
	net_init();

	netsocket = new netsock_t(SOCKET_TCP);
	udpsocket = new netsock_t(SOCKET_UDP);

	// No? Okay, use a custom -port
	const char *val = Args.CheckValue ("-port");
	if (val)
		localport = atoi(val);
	else
		localport = SERVER_PORT;


	// Use -udpport to specify what port to commicate to the master server with
	val = Args.CheckValue ("-udpport");
	if (val)
		localudpport = atoi(val);
	else
		localudpport = UDP_PORT;

	// Reset nettics
	nettics = 0;

	// Bind the TCP port
	if (netsocket->BindToLocalPort(localport) == SOCKET_ERROR)
	{
		sprintf(str, "netsocket->BindToLocalport() error: Failed to open TCP port %d", localport);

#ifdef	DEDICATED_SERVER
		Printf("\nERROR:\nSpecify another port by using -port at the command line\n\n");
#else
		alert(str, "", "Specify another port by using -port at the command line", "Exit", NULL, 0,0);
#endif
		exit(-1);
	}
	Printf("Opened TCP port %d\n", localport);

	// Bind the UDP port
	if (udpsocket->BindToLocalPort(localudpport) == SOCKET_ERROR)
	{
#ifdef	DEDICATED_SERVER
		Printf("\nERROR:\nudpsocket->BindToLocalport() error: Failed to open UDP port %d\nSpecify another port by using -udpport at the command line\n\n", localudpport);
#else
		sprintf(str, "udpsocket->BindToLocalport() error: Failed to open UDP port %d", localudpport);
		alert(str, "", "Specify another port by using -udpport at the command line", "Exit", NULL, 0,0);
#endif
		exit(-1);
	}
	Printf("Opened UDP_PORT %d\n", localudpport);

	// Find local IP
	net_getlocaladdress(netsocket->sock);
	Printf("Local TCP address (%s)\n", net_adrtostring(net_local_adr));
	net_getlocaladdress(udpsocket->sock);

	// Print local address
	Printf("Local UDP address (%s)\n", net_adrtostring(net_local_adr));

	// Load bans file
	SV_ReadBans();
}

void	ResetSocket(bool listening)
{
	// Close the socket
	closesocket(netsocket->sock);
	
	// Reopen the socket
	delete	netsocket;
	netsocket = new netsock_t(SOCKET_TCP);

	if (listening)
	{
		netsocket->Listen(localport);
	}
	else if (netsocket->BindToLocalPort(localport) == SOCKET_ERROR)
	{
		char	str[128];
		sprintf(str, "ResetSocket() error: Failed to open port %d", localport);

		alert(str, "", "Specify another port by using -port at the command line", "Exit", NULL, 0,0);
		exit(-1);
	}
}


void	ShutdownNetwork(void)
{
	if (multiplayer && multiplayermode == MP_CLIENT)
		CL_Disconnect();

	// Close the main socket
	closesocket(netsocket->sock);
	closesocket(udpsocket->sock);

	// Clear memory
	delete netsocket;
	delete udpsocket;

	// Shut winsock down
	net_shutdown();
}


void	StartJoinGame(char	*address)
{
	char	str[128];

	// Set some multiplayer varibles
	multiplayer = 10;
	multiplayermode = MP_CLIENT;

	// No scripting for a client in multiplayer mode. the server does it all
	ScriptedGame = false;

	// Set timeout tics
	totics = gametics;

	// New game for connecting
	gamestate = GS_CONNECTING;

	// New join state
	joinstate = JOIN_CONNECTING;

	// Set address
	if (!net_stringtoadr(address, &serveraddr))
		CL_ConnectError("... Unable to resolve address ...");

	// Make sure that a port number has been specified
	if (serveraddr.port <= 0)
		CL_ConnectError("A port number must specified seperated by a colon. Eg. servername.com:port");

	// Print a message
	sprintf(str, "Connecting to: %s", net_adrtostring(serveraddr));
	StatBar(str);
}

void	CheckForConnect(void)
{
	char	*a;

	a = Args.CheckValue("-connect");

	if (a)
		StartJoinGame(a);
}



//
// Net_Update - Keeps tics in sync
//

void	Net_Update(void)
{
	int             entertic;
    static int      oldentertics;
    int             realtics;
	int				i;

	entertic = I_WaitForTicPolled (oldentertics);
    realtics = entertic - oldentertics;
    oldentertics = entertic;

	if (realtics <= 1)
		return;

	if (realtics > TICRATE)
		realtics = TICRATE;

	realtics--;

	nettics++;

#ifdef	DEDICATED_SERVER
	if (!(nettics % 5))
		SV_UpdateMaster();

	SV_SendUpdate();
#endif

	while(realtics--)
	{
		// Make the ground fall
		ground.fall();

		// Water particle thinker
		StaticWaterThinker();

		CheckTankDeaths();

		// Run the shotclock
		Tank_ShotClockTicker();

		// Run thinkers for all the players in the game
		TankThinker();

		// Missile thinker
		ShotThinker();

		// Water particles, napalm...etc
		ParticleThinkers();

		// Stuff from loop drawer
		// Draw the backgroud & foreground as needed
		ground.DrawScene(buffer);

		// Draw all tanks
		for (i=0;i<MAX_TANKS;i++) 
		{
			if (ta[i].exist <= 0)
				continue;
	
			if (ta[i].health > 0 || ta[i].needstodie)
			{ ta[i].draw(buffer); }
		}

		// Draw all the current shots in the air
		for (i=0;i<N_SHOTS;i++) 
		{
			if (sh[i].exist > 0)
			{
				sh[i].draw(buffer);
			}
		}

#ifdef	DEDICATED_SERVER
			SV_GetPackets();
#endif

		// Process sprites
		G_SpriteTicker();

		ticker();
		gametics++;
	}
}

void	WaitState_Thinker(void)
{
}


//
// Multiplayer_Thinker
//

void Net_Thinker(void)
{
#ifdef	DEDICATED_SERVER
	Net_Update();
#else

	if (!multiplayer) // skip network stuff for singleplayer game.	 Degauss
		return;

	if(forcetime->value > 0)
		Net_Update();

	if(!(gametics % (TICRATE*5)))
		{
		if(multiplayermode == MP_SERVER)	//	Running a multiplayer server?		Update the master
			if(notifymasterserver->value > 0)//	but only do this if the user said ok
				{
				SV_UpdateMaster();
				}

		if(!(gametics % TICRATE))	//	ping the client
			SV_SendUpdate();
		}

	// Get packets
	if(multiplayermode == MP_SERVER)
		SV_GetPackets();
	else
		CL_GetPackets();

#endif
}


void	BeginDedicatedGame(void)
{
	// Set number of rounds & players
	numrounds = (int)num_rounds->value;
	maxplayers = (int)num_players->value;

	numplayers = 0;

	// Setup multiplayer varibles
	multiplayer = 1;
	multiplayermode = MP_SERVER;

	// Local host won't be playing
	net_localplayer = -1;

	ResetTanks();

	// Reset the socket so it's listening for connections
	ResetSocket(true);

	// Setup tank varibles
	InitDefaultTurnQue();
	
	// Load the actual level into memory
	LoadLevel(NULL);	// random level

	menuactive = false;

	// Enter gamestate
	StartLevel(true);
}


void	CL_Write (packet_t* p, void *ddata, int length)
{ p->Write(ddata, length); }

void	CL_Write (packet_t* p, byte *ddata, int startpos, int length)
{ p->Write(ddata, startpos, length); }
		
void	CL_WriteChar (packet_t* p, int c)
{ p->WriteChar(c); }

void	CL_WriteByte (packet_t* p, int c)
{ p->WriteByte(c); }

void	CL_WriteShort (packet_t* p, int c)
{ p->WriteShort(c); }

void	CL_WriteWord (packet_t* p, short v)
{ p->WriteWord(v); }

void	CL_WriteLong (packet_t* p, int c)
{ p->WriteLong(c); }

void	CL_WriteFloat (packet_t *p, float f)
{ p->WriteFloat(f); }

void	CL_WriteString (packet_t *p, char *s)
{ p->WriteString(s); }






//
// Broadcast message sent to all clients (used by the server)
//

void	BCast_Write (void *ddata, int length)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->Write(ddata, length);
	}
}

void	BCast_Write (byte *ddata, int startpos, int length)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->Write(ddata, startpos, length);
	}
}
		
void	BCast_WriteChar (int c)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->WriteChar(c);
	}
}

void	BCast_WriteByte (int c)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;
		
		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->WriteByte(c);
	}
}

void	BCast_WriteShort (int c)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->WriteShort(c);
	}
}

void	BCast_WriteWord (short v)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->WriteWord(v);
	}
}

void	BCast_WriteLong (int c)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->WriteLong(c);
	}
}

void	BCast_WriteFloat (float f)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;
		
		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->WriteFloat(f);
	}
}

void	BCast_WriteString (char *s)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;

		if (ta[i].netstate <= NETSTATE_LEVELSETUP)
			continue;

		ta[i].outbuf->WriteString(s);
	}
}



//
// BCast_Printf (SV Only) - Sends a text message to spit out on the "console" for all clients
//

void	BCast_Printf(char	*str)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || i == net_localplayer || ta[i].skill != TANKSKILL_HUMAN)
			continue;
		ta[i].outbuf->WriteByte(STOC_PRINTF);
		ta[i].outbuf->WriteString(str);
	}

	G_Printf(str);
}

// ===================== modified random number generator  =========
static unsigned int LastNumber = 42;
int		rANDom(void)
	{

	srand(LastNumber);
	LastNumber = rand();
//	Printf("!!%d!!", LastNumber );	// Degauss RND
	return LastNumber;
	}