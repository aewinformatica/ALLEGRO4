#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <allegro.h>


#include "dialogs.h"
#include "scorch.h"
#include "particles.h"
#include "peffect.h"
#include "editor\editor.h"



BITMAP			*jgbuffer;

void			JoinGame_Proc(void);
void			RequestMasterInfo(void);

int				clientport;
int				numservers;
int				net_localplayer=0;

serverlist_t	serverlist[MAX_SERVERLIST];

int				selectedserver;
int				lastselectedserver;
int				joinstate;
int				totics;
char			statustext[64];
netaddress_t	serveraddr;

// For custom addresses
BITMAP			*customip_bitmap;
char			customip_address[65];
char			customip_port[10];
void			customip_cancel(void);
void			customip_join(void);
void			CIP_Drawer(void);		// Custom IP Drawer

BITMAP			*remotepassword_buffer;
void			getpassword_drawer(void);
void			getpassword_start(void);

char	serverdetails[512];
char	remote_password[256];


void	RequestLANInfo(void);


void	StatBar(char *text)
{
	ustrcpy(statustext, text);
}

void	ClearServerList(void)
{
	int		i;

	memset (serverlist, 0, sizeof(serverlist));
	numservers = 0;

	// Set all pings to unknown
	for (i=0; i<MAX_SERVERLIST; i++)
		serverlist[i].ping = UNKNOWN_PING;

}

int		AddServerToList(void)
{
	int		s=1,i;

	for (i=0; i<MAX_SERVERLIST; i++)
	{
		if (serverlist[i].active <= 0)
		{ s = i; break; }
	}

	return s;
}


int		ServerListSize(void)
{
	int	i;
	int	num = 0;

	for (i=0; i<MAX_SERVERLIST; i++)
	{
		if (serverlist[i].active <= 0)
			continue;

		num++;
	}

	return num;
}

int		FindServerInList(netaddress_t	adr)
{
	int		i;

	// Attempt to match a server up by its ip address
	for (i=0; i<numservers; i++)
	{
		if (net_compareadr(adr, serverlist[i].address))
			return i;
	}
	return -1;
}


char *serverlist_getter(int index, int *list_size)
{
   if (index < 0) {
      *list_size = numservers;
      return NULL;
   }
   else
      return serverlist[index].name;
}


int		d_joingame_button(int msg, DIALOG *d, int c)
{
	if (msg == MSG_IDLE)
	{
		JoinGame_Proc();
		return D_O_K;
	}
	else
		return d_agup_push_proc(msg, d, c);
}


void	refreshbutton(void)
{
	if (joinstate == JOIN_SEARCHLAN)
		RequestLANInfo();
	else
	{
		ClearServerList();
		RequestMasterInfo();
	}

	StatBar("Refreshing...");

	broadcast_dialog_message(MSG_DRAW, 0);
	//object_message(&dialog_joingame[0], MSG_DRAW, 0);
}


void	AttemptJoinConnection(netaddress_t	*adr)
{
	char		str[64];

	if (netsocket->Connect(adr) == SOCKET_ERROR)
	{
		errno = WSAGetLastError();
		if (errno != WSAEWOULDBLOCK)
		{
			CL_ConnectError(net_getlasterror());
			return;
		}
	}

	//netsocket->SetNonBlocking();

	// Set timeout tics
	totics = gametics;

	// New game for connecting
	gamestate = GS_CONNECTING;

	// At this point, we no longer need the dialog. So, cut it off
	agup_break_dialog();

	// No menus active
	menuactive = false;

	// New join state
	joinstate = JOIN_CONNECTING;

	// Print a message
	sprintf(str, "Connecting to: %s", net_adrtostring(serveraddr));
	StatBar(str);
}

void	joinbutton(void)
{
	int			sv = dialog_joingame[0].d1;
	int			errno;

	// Setup remote address info
	serveraddr = serverlist[sv].address;

	// Apply the TCP port
	serveraddr.port = ntohs(serverlist[sv].tcpport);

	//netsocket->SetBlocking();

	AttemptJoinConnection(&serveraddr);

};

void	cancelbutton(void)
{
	// Switch back into "local" mode
	CL_Disconnect();

	// Stop the dialog
	agup_break_dialog();

	if (gamestate != GS_INGAME)
	{
		PlayLevelOGG("title.mp3");
	}
}



int d_serverlist_proc(int msg, DIALOG *d, int c)
{
	numservers = ServerListSize();

	selectedserver = dialog_joingame[0].d1;

	// Redraw the server info & player lists
	if (selectedserver != lastselectedserver)
		object_message(&dialog_joingame[1], MSG_DRAW, 0);

	lastselectedserver = selectedserver;

	return d_abeos_serverlist_proc(msg, d, c);
}


int d_serverdetails_proc(int msg, DIALOG *d, int c)
{
	//char		str[64];
	int			needs_redraw = -1;
	//netaddress_t	addr;

	selectedserver = dialog_joingame[0].d1;

	if (serverlist[selectedserver].active > 0)
	{
		// Fill in serverdetails
		sprintf(serverdetails, "IP: %i.%i.%i.%i:%i\nMap: %s\nHost: %s\nPlayers: %d / %d\nStarting cash: %d",
			serverlist[selectedserver].address.ip[0],
			serverlist[selectedserver].address.ip[1],
			serverlist[selectedserver].address.ip[2],
			serverlist[selectedserver].address.ip[3],
			serverlist[selectedserver].tcpport,
			serverlist[selectedserver].levelname,
			serverlist[selectedserver].host,
			serverlist[selectedserver].numplayers,
			serverlist[selectedserver].maxplayers,
			serverlist[selectedserver].startingcash);
	}

	return d_agup_textbox_proc(msg, d, c);
}

void	customip_join(void)
{
	char	address[128];

	agup_break_dialog();

	sprintf(address, "%s:%s", customip_address, customip_port);

	StartJoinGame(address);
}


void	customip_cancel(void)
{
	//int		ret;

	Title_Drawer(buffer);
	GUI_Drawer(buffer);

	clear_to_color(jgbuffer, TRANSCOLOR);

	joinstate = JOIN_NOTCONNECTED;

	agup_break_dialog();

	// Start the dialog
	//ret = do_agup_dialog(dialog_joingame, jgbuffer, JoinGame_Proc);

	//if (ret == 4)
	//	multiplayer = 0;
}


int		d_customip_button(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		CIP_Drawer();
		return D_O_K;
	}

	return d_agup_push_proc(msg, d, c);
}

DIALOG dialog_specifyip[] =
{   /* (dialog proc)			(x)      (y)   (w)   (h)   (fg)  (bg)  (key) (flags) (d1)   (d2)  (dp)                     (dp2) (dp3) */
   { d_agup_window_proc,		217,	 236,  377,   85,  255,	  0,      0,	0,     0,     0,  (char *)"Specify IP" ,    0,   0},
   { d_agup_edit_proc,			227,     262,  254,   20,  255,   0,      0,    0,	  64,     0,  customip_address, 0,   0},
   { d_agup_edit_proc,			485,     262,  97,    20,  255,   0,      0,    0,	   9,     0,  customip_port,    0,   0},
// Server listbox
   { d_customip_button,			429,     290,  76,    23,  255,   0,      0,	0,	   0,     0,  (char *)"Cancel",         0,   (void *)customip_cancel },
   { d_agup_push_proc,			512,     290,  76,    23,  255,   0,      0,	0,	   0,     0,  (char *)"Join",           0,   (void *)customip_join   },
   { NULL,                        0,       0,   0,     0,    0,   0,      0,    0,     0,     0,               0,           0,   0 }
};

void	CIP_Drawer(void)
{
	if (update_count <= 0)
		return;

	update_count=0;

	if (gamestate == GS_INGAME)
	{
		Title_Drawer(buffer);
		if (menuactive)
			GUI_Drawer(buffer);
	}

	// Draw the dialog
	AddRectangle(buffer, dialog_specifyip[0].x,
		dialog_specifyip[0].y,
		dialog_specifyip[0].w,
		dialog_specifyip[0].h);

	draw_sprite(buffer, jgbuffer, 0, 0);

	Screen_Blit(true);
	gametics++;
	frame_count++;
}


void	specifyipbutton(void)
{
	int		ret;

	sprintf(customip_address, "");
	sprintf(customip_port, "%d", SERVER_PORT);

	set_dialog_color(dialog_specifyip, gui_fg_color, gui_bg_color);
	ret = do_agup_dialog(dialog_specifyip, jgbuffer, CIP_Drawer);
}

DIALOG dialog_joingame[] =
{

   //{ d_agup_window_proc,		232,  187,  327,  266,  255,		0,			0,		0,     0,    0,    (char *)"Create Game" },

   // Server listbox
   { d_serverlist_proc,			64,   100,  314,  344,	 255,    0,    0,    0,   0,	0,    (char *)serverlist_getter },
   { d_serverdetails_proc,		406,  100,  314,  344,	   0,    0,    0,    0,   0,    0,    (char *)serverdetails,                 0,     0 },

   // Misc buttons
   { d_agup_push_proc,			595,     481,  125,   25,   255,  0,    0,			0,		 0,       0,    (char *)"Join",   0,  (void *)joinbutton },
   { d_joingame_button,         64,      481,  125,   25,   255,  0,    0,			0,		 0,       0,    (char *)"Cancel", 0,  (void *)cancelbutton },

   { d_agup_push_proc,			463,     481,  125,   25,   255,  0,    0,			0,		 0,       0,    (char *)"Refresh List", 0, (void *)refreshbutton },
   { d_agup_push_proc,			330,     481,  125,   25,   255,  0,    0,			0,		 0,       0,    (char *)"Specify IP",   0, (void *)specifyipbutton },

   //{ d_keyboard_proc,		0,       0,  0,     0,      0,    0,    0,    0,        KEY_ESC,0,    exit_newgame, NULL, NULL  },
   { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }

};


void	password_cancel(void)
{
	agup_break_dialog();

	gamestate = GS_TITLE;

	CL_Disconnect();

	GUI_SetupMenu(multiplayer_menu);

	// Set back into the server listing
	joingame();
}

void	password_submit(void)
{
	agup_break_dialog();
}

int		d_getpassword_button(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		getpassword_drawer();
		return D_O_K;
	}

	return d_agup_push_proc(msg, d, c);
}

DIALOG dialog_getpassword[] =
{
   { d_agup_window_proc,		217,	 236,  377,   85,  255,	 0,    0,	0,  0,   0,    (char *)"Password Verification" },
   { d_agup_edit_proc,			227,     262,  274,   20,  255,  0,    0,   0,	64,  0,    (char *)remote_password         },

   // Server listbox
   { d_getpassword_button,		429,     290,  76,    23,   255,  0,   0,	0,	0,   0,    (char *)"Cancel",   0, (void *)password_cancel },
   { d_agup_push_proc,			512,     290,  76,    23,   255,  0,   0,	0,	0,   0,    (char *)"Submit",   0, (void *)password_submit },

   { NULL,                       0,    0,    0,   0,   0,  0,    0,   0,       0,    0,  0,                    0,     0 }
};

void	getpassword_start(void)
{
	remotepassword_buffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(remotepassword_buffer, TRANSCOLOR);

	// Fill in with text
	sprintf(remote_password, "%s", password->string);

	// Do the dialog
	do_agup_dialog(dialog_getpassword, remotepassword_buffer, getpassword_drawer);

	Cvar_Set("password", remote_password);

	// Free some memory
	destroy_bitmap(remotepassword_buffer);

}

void	getpassword_drawer(void)
{
	if (mrbmmg_mode > 0)
		draw_sprite(buffer, (BITMAP *)maindata[TITLESCREEN_BMMG].dat, 0,0);
	else
		draw_sprite(buffer, (BITMAP *)maindata[TITLESCREEN].dat, 0,0);

	textout_centre(buffer, (FONT *)maindata[MS].dat, statustext, (SCREENWIDTH/2)+1, 570+1, 0);
	textout_centre(buffer, (FONT *)maindata[MS].dat, statustext, (SCREENWIDTH/2), 570, makecol(255,255,255));

	// Draw the dialog
	AddRectangle(buffer, dialog_getpassword[0].x, dialog_getpassword[0].y, dialog_getpassword[0].w, dialog_getpassword[0].h);
	draw_sprite(buffer, remotepassword_buffer, 0, 0);

	ticker();
	Screen_Blit(true);
	frame_count++;
	gametics++;
}


void	enablejoinbuttons(void)
{
/*
	int	i;
	for (i=0; i<4; i++)
		dialog_joingame[i].flags |= D_DISABLED;
*/
}

void	disablejoinbuttons(void)
{
/*
	int	i;
	for (i=0; i<4; i++)
		dialog_joingame[i].flags &= ~D_DISABLED;
*/
}

//
// joingame - called by the main GUI
//

void	joingame(void)
{
	int	ret;

	// Reset joinstate
	joinstate = JOIN_NOTCONNECTED;

	// Joining a game, make sure the socket is in non-listen mode
	ResetSocket(false);

	multiplayer = 1;
	multiplayermode = MP_CLIENT;

	// Play a new song for mutliplayer menu
	PlayLevelOGG("multititle.mp3");

	StatBar("Attempting master server query....");

	// Disable the buttons/windows until we are ready to connect
	disablejoinbuttons();

	// Create a new drawer and clear it
	jgbuffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(jgbuffer, TRANSCOLOR);

	// Wipe out in the server list
	ClearServerList();

	// Start the dialog
	ret = do_agup_dialog(dialog_joingame, jgbuffer, JoinGame_Proc);
}

void	joinLANgame(void)
{
	int	ret;

	// Reset joinstate
	joinstate = JOIN_SEARCHLAN;

	// Joining a game, make sure the socket is in non-listen mode
	ResetSocket(false);

	multiplayer = 1;
	multiplayermode = MP_CLIENT;

	// Play a new song for mutliplayer menu
	PlayLevelOGG("multititle.mp3");

	StatBar("Attempting LAN query....");

	// Disable the buttons/windows until we are ready to connect
	disablejoinbuttons();

	// Create a new drawer and clear it
	jgbuffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(jgbuffer, TRANSCOLOR);

	// Wipe out in the server list
	ClearServerList();

	// Query for LAN games
	RequestLANInfo();

	// Start the dialog
	ret = do_agup_dialog(dialog_joingame, jgbuffer, JoinGame_Proc);
}

//
// GetMasterError - OMFG !!!! ERROR!!!?!?!?!?
//

void	GetMasterError(void)
{
	char	*error = inbuf->ReadString();

	StatBar(error);

	joinstate = JOIN_IDLE;
}


void	JoinGame_Drawer(void)
{
	if (mrbmmg_mode > 0)
		draw_sprite(buffer, (BITMAP *)maindata[TITLESCREEN_BMMG].dat, 0,0);
	else
		draw_sprite(buffer, (BITMAP *)maindata[TITLESCREEN].dat, 0,0);

	draw_sprite(buffer, jgbuffer, 0,0);

	// Draw the status text
	textout_centre(buffer, (FONT *)maindata[MS].dat, statustext, (SCREENWIDTH/2)+1, 570+1, 0);
	textout_centre(buffer, (FONT *)maindata[MS].dat, statustext, (SCREENWIDTH/2), 570, makecol(255,255,255));

}

void	RequestLANInfo(void)
{
	packet_t		*mbuf = new packet_t;

#ifdef	_DEBUG
//  Degauss debug
	Printf("RequestLANInfo\n");
#endif

	// Request the server list
	mbuf->Clear();
	mbuf->WriteByte(LAN_REQUESTSERVERINFO);

	net_broadcast(udpsocket, mbuf, localudpport);

	// Free packet
	delete mbuf;
}

void	RequestMasterInfo(void)
{
	packet_t		*mbuf = new packet_t;
	netaddress_t	addr;

 if(!multiplayer) return;	// Don't call master about single player or lan games

#ifdef	_DEBUG
//  Degauss debug
	Printf("RequestMasterInfo\n");
#endif

	// Setup the master server's address
	net_stringtoadr(masteraddr, &addr);
	net_setport(addr, MASTER_PORT);

	// Request the server list
	mbuf->Clear();
	mbuf->WriteByte(CTOM_REQUESTLIST);  //  netcommon.h(40):#define		CTOM_REQUESTLIST	 2
	mbuf->WriteLong(VERSIONNUM);				// netcommon.h(15):#define		VERSIONNUM		86  (0.99c is 85)

	// Fire away!
	udpsocket->SendPacket(addr, mbuf, PACKUDP);

	// Set new joinstate
	joinstate = JOIN_MASTERCONNECT;

	// Set timeout tics
	totics = gametics;

	// Free packet
	delete mbuf;
}


//
// MasterRequestList - Requests the list from the master server
// once we've recived a responce
//

void	MasterRequestList(void)
{
	int	r;

	// Clear out the packet first
	inbuf->Clear();

	r = udpsocket->GetPacket(PACKUDP);

	if (r > 0)
	{
		Client_ParseData();
		joinstate = JOIN_SERVERLISTING;
		totics = gametics;
	}

	// No responce from the master server
	if ((gametics - totics) == JOIN_TIMEOUTTIME)
	{
		StatBar("Master server is not responding.");
		joinstate = JOIN_IDLE;
	}
}

void	GetServerListing(void)
{
	int	r;

	// Clear out the packet first
	inbuf->Clear();

	// Get some packets
	r = udpsocket->GetPacket(PACKUDP);

	if (r > 0)
		Client_ParseData();
}


void	JoinGame_Proc(void)
{
	if (update_count > 0)
	{
		update_count = 0;

		switch(joinstate)
		{
			case	JOIN_NOTCONNECTED:
				RequestMasterInfo();
				break;

			case	JOIN_MASTERCONNECT:
				MasterRequestList();
				break;

			case	JOIN_SERVERLISTING:
			case	JOIN_SEARCHLAN:
				GetServerListing();
				break;

			default:
				break;
		}

		// Draw the screen
		JoinGame_Drawer();

		GameLinesThinker();
		GameLinesDrawer(buffer);

		ticker();
		Screen_Blit(true);
		gametics++;
		frame_count++;
	}

}


void	RecieveLANServer(void)
{
	int			s;

	s = AddServerToList();
	if (s < 0)
	{
		// Something happened...
		inbuf->ReadShort();
		inbuf->ReadString();
		inbuf->ReadString();
		inbuf->ReadByte();
		inbuf->ReadByte();

		inbuf->ReadByte();
		inbuf->ReadByte();
		inbuf->ReadByte();
		inbuf->ReadByte();
		inbuf->ReadShort();
		return;
	}

	serverlist[s].address = net_incoming_adr;

	// Mark as active
	serverlist[s].active = 1;

	// Read TCP port
	serverlist[s].tcpport = inbuf->ReadShort();

	// Read some data about the server
	ustrcpy(serverlist[s].name, inbuf->ReadString());
	ustrcpy(serverlist[s].levelname, inbuf->ReadString());
	serverlist[s].numplayers = inbuf->ReadByte();
	serverlist[s].maxplayers = inbuf->ReadByte();

	// Read host name
	ustrcpy(serverlist[s].host, inbuf->ReadString());
	serverlist[s].startingcash = inbuf->ReadLong();

	// LAN server?
	serverlist[s].lanserver = true;

	// Fix the name if there is none
	if (ustrlen(serverlist[s].name) <= 0)
		sprintf(serverlist[s].name, "Untitled");

	numservers++;

	//broadcast_dialog_message(MSG_DRAW, 0);
	object_message(&dialog_joingame[0], MSG_DRAW, 0);
	object_message(&dialog_joingame[1], MSG_DRAW, 0);
}

void	RecieveServerIntoList(bool lanserver)
{
	int			s;

	s = AddServerToList();
	if (s < 0)
	{
		// Something happened...
		inbuf->ReadString();
		inbuf->ReadString();
		inbuf->ReadByte();
		inbuf->ReadByte();

		inbuf->ReadByte();
		inbuf->ReadByte();
		inbuf->ReadByte();
		inbuf->ReadByte();
		inbuf->ReadShort();
		return;
	}

	// Mark as active
	serverlist[s].active = 1;

	if (lanserver)
		serverlist[s].tcpport = inbuf->ReadShort();

	// Read some data about the server
	ustrcpy(serverlist[s].name, inbuf->ReadString());
	ustrcpy(serverlist[s].levelname, inbuf->ReadString());
	serverlist[s].numplayers = inbuf->ReadByte();
	serverlist[s].maxplayers = inbuf->ReadByte();

	// Read address & port information
	serverlist[s].address.ip[0] = inbuf->ReadByte();
	serverlist[s].address.ip[1] = inbuf->ReadByte();
	serverlist[s].address.ip[2] = inbuf->ReadByte();
	serverlist[s].address.ip[3] = inbuf->ReadByte();
	serverlist[s].address.port = inbuf->ReadShort();
	serverlist[s].tcpport = inbuf->ReadShort();

	ustrcpy(serverlist[s].host, inbuf->ReadString());
	serverlist[s].startingcash = inbuf->ReadLong();

	// LAN server?
	serverlist[s].lanserver = lanserver;

	// Fix the name if there is none
	if (ustrlen(serverlist[s].name) <= 0)
		sprintf(serverlist[s].name, "Untitled");

	numservers++;

	//broadcast_dialog_message(MSG_DRAW, 0);
	object_message(&dialog_joingame[0], MSG_DRAW, 0);
	object_message(&dialog_joingame[1], MSG_DRAW, 0);
}

void	BuildServerList(void)
{
	int			nservers,i;
	char		str[64];
	packet_t	*p = new packet_t;

	nservers = inbuf->ReadByte();

	// Set number of servers
	sprintf(str, "%d servers found", nservers);
	StatBar(str);

	for (i=0; i<nservers; i++)
		RecieveServerIntoList(false);

	// Now ready to accept input from the player
	joinstate = JOIN_READY;
	enablejoinbuttons();

	// Free memory
	delete p;
}


void	CL_RequestPlayerInfo(void)
{
	// Launch another packet telling the server we're ready for
	// all the other clien't info
	localbuf->Clear();
	localbuf->WriteByte(CTOS_GETPLAYERINFO);
	localbuf->WriteLong(ta[net_localplayer].money);
	CL_SendPackets();
}

void	CL_RequestBuyStuff(void)
{
	// Send some info to the server
	localbuf->WriteByte(CTOS_PLAYERSETUP);
	localbuf->WriteByte(ta[net_localplayer].character);
	localbuf->WriteString(ta[net_localplayer].name);
	CL_SendPackets();
}

void	CL_SetupPlayer(void)
{
	//int		playersetuptime;

	int		localplyr = inbuf->ReadByte();
	int		plyrteam = inbuf->ReadShort();
	int		teammode = inbuf->ReadByte();

	// Duplicate message
	if (joinstate >= JOIN_PLAYERSETUP)
		return;

	// Send a message that will probably never be seen
	StatBar("Entering player setup...");

	// Reset all tank data
	ResetTanks();

	// Read some varibles first
	net_localplayer = localplyr;

	teamplay->value = teammode;

	if (teamplay->value > 0)
		ta[net_localplayer].SetTeamColor(plyrteam);

	// Run player setup
	Initialise_PEffect();
	PS_RunMultiplayerSetup(net_localplayer, -1);
	Shutdown_PEffect();

	// Update timeout tics
	totics = gametics;

	// New message for client
	StatBar("Awaiting response from server...");

	// Set new state
	joinstate = JOIN_PLAYERSETUP;

	// Send a reply back
	CL_RequestBuyStuff();
}


void	CL_SetupLevel(void)
{
	int		cash =			inbuf->ReadLong();
	char	*filename =		inbuf->ReadString();
	int		walltype =		inbuf->ReadShort();
	int		gravityflags =	inbuf->ReadShort();
	int		numshots = inbuf->ReadByte();

	// Duplicate message
	if (joinstate >= JOIN_BUYSTUFF)
		return;

	// Update status
	StatBar("Requesting gamestate...");

	// Setup some definite varibles
	ta[net_localplayer].init(net_localplayer, rand()%(SCREENWIDTH/2),0, 100,15,10,LIGHTGREEN);
	GiveAmmo(net_localplayer, W_BABYMISSILE, UNLIMITED_AMMO);
	ta[net_localplayer].GiveMoney(cash);

	if (!filename && ustrlen(filename) >= 0)
		CL_ConnectError("CL_SetupLevel: Error in filename");

	// Load the level
#ifdef	_DEBUG
Printf("Client [CL_SetupLevel] file name [%s]\n", filename);	//  Degauss debug
#endif
	LoadLevel(filename);

	StartLevel();

	// Modify gravity & wall flags
	currentlevel.gravityflags = gravityflags;
	currentlevel.walltype = walltype;

	gamestate = GS_CONNECTING;

	joinstate = JOIN_BUYSTUFF;

	if (numshots <= 0)
		StartBuyStuff();

	// Send a reply back
	CL_RequestPlayerInfo();

	ta[net_localplayer].address = net_local_adr;

	// Update timeout time
	totics = gametics;
}


void	CL_GetPlayerInfo(void)
{
	int		t =			inbuf->ReadByte();
	char	*name =		inbuf->ReadString();
	int		character = inbuf->ReadByte();
	int		skill =		inbuf->ReadByte();
	int		team =		inbuf->ReadByte();
	int		colorr =	inbuf->ReadByte();
	int		colorg =	inbuf->ReadByte();
	int		colorb =	inbuf->ReadByte();

	// Tank exists
	if (ta[t].exist <= 0)
	{
		ta[t].init(t, 0,0, 0,0,0,0);
		ta[t].exist = 10;
	}

	// Copy player info
	ustrcpy(ta[t].name, name);
	ta[t].character = character;
	ta[t].skill = skill;

	// Get player stats
	ta[t].x = inbuf->ReadShort();
	ta[t].y = inbuf->ReadShort();
	ta[t].health = inbuf->ReadShort();
	ta[t].deg = inbuf->ReadShort();
	ta[t].spectator = inbuf->ReadByte();

	// Get fuel info
	ta[t].startfuel = inbuf->ReadLong();
	ta[t].specials[fuel].amount = inbuf->ReadLong();

	// Setup HEB stats per their character
	ta[t].SetHEB(characters[ta[t].character].attrib_h,
				 characters[ta[t].character].attrib_e,
				 characters[ta[t].character].attrib_b);

	// Apply colorage
	ta[t].colorr = colorr;
	ta[t].colorg = colorg;
	ta[t].colorb = colorb;
}


//
// CL_StartGame - Adjusts the gamestate and actually puts the player into the game
//
void	CL_StartGame(void)
{
	int		spectator;
	int		autod_wait;
	int		tt;

	net_localplayer = inbuf->ReadByte();
	tt = tankturn = inbuf->ReadByte();
	spectator = inbuf->ReadByte();
	autod_wait = inbuf->ReadByte();

	joinstate = JOIN_PLAYING;

	gamestate = GS_INGAME;

	ta[net_localplayer].spectator = spectator;

	Printf("spectator is %d and tankturn is %d\n", spectator, tankturn);

	// Wait for for the server to flag that auto-defense is finished?
	if (autod_wait > 0)
		ta[net_localplayer].specialflags |= TANKSPECIAL_AUTODEFENSEWAIT;
	else
		ta[net_localplayer].specialflags &= ~TANKSPECIAL_AUTODEFENSEWAIT;

	// See if we have autodefense that needs to be setup
	if (autod_wait > 0 &&
		ta[net_localplayer].specials[autodefense].amount > 0)
	{
		// Minus one from inventory
		ta[net_localplayer].specials[autodefense].amount--;

		tankturn = net_localplayer;
		StartDefenseBox(net_localplayer);
		tankturn = tt;

		// When the dialog is closed, send a signal to server
		// indicating that we've finished
		//localbuf->WriteByte(CTOS_AUTODEFENSEDONE);
	}
		// Send a signal to server indicating that we've finished
		// with autodefense ie has been used or not available.
		localbuf->WriteByte(CTOS_AUTODEFENSEDONE);
	  ta[net_localplayer].tankstate = TANKSTATE_PLAYING;

	// Refresh
	ground.needsupdate = true;

	// Break any text messages
	TankMessage_Break();
}

void	CL_ConnectError(char *str)
{
	agup_buffer = screen;
	alert("", str, NULL, "Ok", NULL, 0,0);

	gamestate = GS_TITLE;

	// Disconnect
	CL_Disconnect();

	GUI_SetupMenu(multiplayer_menu);

	// Set back into the server listing
	joingame();
}

//
// JoinGame_Setup - Main thinker that is used for GS_CONNECTING that drives the
// various stages of connecting to a server
//

void	JoinGame_Setup(BITMAP *b)
{

	if (mrbmmg_mode > 0)
		draw_sprite(b, (BITMAP *)maindata[TITLESCREEN_BMMG].dat, 0,0);
	else
		draw_sprite(b, (BITMAP *)maindata[TITLESCREEN].dat, 0,0);

	textout_centre(b, (FONT *)maindata[MS].dat, statustext, (SCREENWIDTH/2)+1, 570+1, 0);
	textout_centre(b, (FONT *)maindata[MS].dat, statustext, (SCREENWIDTH/2), 570, makecol(255,255,255));

	// Gamelines thinker & drawer
	GameLinesThinker();
	GameLinesDrawer(b);

	// Recieve anything that is coming in
	CL_GetPackets();

	// Only one key input to check for
	if (IsKeyDn(KEY_ESC))
		CL_ConnectError("... Connection attempt aborted ...");
}

void	CL_SendPackets(void)
{
	char	str[128];
	int		ret;

	if(localbuf->size > 0)
		{
		ret = netsocket->SendPacket(serveraddr, localbuf);
		if(ret == SOCKET_ERROR)
			{
			ret = WSAGetLastError();
			sprintf(str, "Sendpacket(): Winsock error (%d)", ret);
			G_Printf(str);	//	Winsock error
			if(ret == 10054)	// Connection reset by peer. An existing connection was forcibly closed by the remote host.
				{
				// We're no longer in multiplayer else we will loop back here.
				multiplayer = 0;    multiplayermode = -1;
				Printf("The server has closed??");
				ret = alert("Error", "The server has closed!!", "Bummer", "Exit", "Continue?", 0,0);
				if(ret == 1) quitgame();
				if(ret == 2) endgame(false);
				}
			}
		localbuf->Clear();
		}
}

//
// CL_Disconnect - Disconnects a client from the game
//

void	CL_Disconnect(void)
{
	if (multiplayer)
	{
		if (multiplayermode == MP_CLIENT)
		{
			// Send a disconnect flag
			localbuf->WriteByte(CTOS_DISCONNECTED);

			// Send packets to all clients
			CL_SendPackets();
		}

		// Close the socket out
		closesocket(netsocket->sock);
	}

	// Set some varibles to indicate that we're no longer
	// in multiplayer
	multiplayer = 0;
	multiplayermode = -1;
}

//
// CL_DisconnectError - Server sent an error that causes us to have to disconnect
//

void	CL_DisconnectError(void)
{
	char	*reason = inbuf->ReadString();

	// Stop any music
	StopLevelOGG();

	menuactive = false;

	agup_buffer = screen;

	// Display message first
	alert("Connection Error", NULL, reason, "Ok", NULL, 0, 0);

	gamestate = GS_TITLE;

	// Disconnect
	CL_Disconnect();

	GUI_SetupMenu(multiplayer_menu);

	// Set back into the server listing
	joingame();
}

//
// CL_SendChatMessage - Sends a chat message to all other clients
//

void	CL_SendChatMessage(char	*msg)
{
	if (multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_CHATMESSAGE);
		BCast_WriteByte(net_localplayer);
		BCast_WriteString(msg);
	}
	else
	{
		localbuf->WriteByte(CTOS_CHATMESSAGE);
		localbuf->WriteString(msg);
	}

	// Draw it locally here too
	//G_Printf(msg, net_localplayer);
}


//
// CL_GetChatMessage - Display a chat messgae from a particular tank
//

void	CL_GetChatMessage(void)
{
	int		tank = inbuf->ReadByte();
	char	*msg = inbuf->ReadString();

	// This shoulden't ever happen...
	if (tank == net_localplayer)
		return;

	// Pring on screen
	G_Printf(msg, tank);
}


void	CL_ReplyPingUpdate(void)
{
	int		time = inbuf->ReadLong();
	int		gtics = inbuf->ReadLong();

	// Send the message right back and tell the client
	// to calculate ping
	localbuf->WriteByte(CTOS_UPDATEPING);
	localbuf->WriteLong(gtics);
	localbuf->WriteLong(time);
}

void	CL_GetUpdatedTankturn(void)
{
	int		newtankturn = inbuf->ReadByte();

	if (newtankturn != tankturn)
		ClearAllShotFlags();

	if (!(ta[net_localplayer].specialflags & TANKSPECIAL_AUTODEFENSEWAIT))
	{
		if (newtankturn != tankturn)
			SpawnTurnArrow(newtankturn);

		tankturn = newtankturn;
	}
}


void	CL_DamageTank(void)
{
	char	str[32];
	int		tank = inbuf->ReadByte();
	int		damageshields = inbuf->ReadByte();
	int		disp_damage = inbuf->ReadShort();		// amount of damage to display
	int		scattertext = inbuf->ReadByte();
	int		health = inbuf->ReadShort();
	int		energyleft = inbuf->ReadShort();
	int		textcolor;

	int		textx = (ta[tank].x-(ta[tank].sizex/2))-10;
	int		texty = (ta[tank].y-(ta[tank].sizey/2))-20;

	// Figure out what color to use
	if (damageshields > 0)
		textcolor = makecol(255,50,165);
	else
		textcolor = makecol(255,0,0);

	sprintf(str, "-%d", disp_damage);
	if (scattertext)
		SpawnGameText(str, (int)(1.2*TICRATE), textx+(rand()%(10*2)), texty+(rand()%(20*2)), 0, -6, textcolor, (FONT *)(FONT*)maindata[MS].dat, 0);
	else
		SpawnGameText(str, (int)(1.2*TICRATE), (int)ta[tank].x, (int)ta[tank].y, 0, -6, textcolor);

	// Set varibles
	ta[tank].health = health;
	ta[tank].energyleft = energyleft;

	// Waiting for a death sequence
	if (ta[tank].health <= 0)
		ta[tank].needstodie = 10;
}


void	CL_DoFallingDamage(void)
{
	// Same thing as before, just play the sound
	CL_DamageTank();

	// Play sound
	StartSound(TANK_FALLDAMAGE);
}


void	CL_KillTank(void)
{
	int		tank = inbuf->ReadByte();
	int		seq = inbuf->ReadByte();

	// Set the appropiate varibles
	ta[tank].health = 0;
	ta[tank].needstodie = 1;

	//if (ta[tank].deathtics <= 0 && ta[tank].needstodie > 0)
	StartDeathSequence(tank, seq);
}


void	CL_StartFinalIntermission(void)
{
	numrounds = 0;

	// Set new joinstate
	joinstate = JOIN_INTERMISSION;

	// Dont count down
	//tank_shottimer = -1;

	// Close the socket out
	closesocket(netsocket->sock);

	//multiplayer = 1;
	//multiplayermode = MP_CLIENT;

	// Start intermission
	Inter_Run();
}


void	CL_StartIntermission(void)
{
	int		cash;
	char	*newlvlname;
	int		walltype, gravityflags;

	numrounds = inbuf->ReadShort();
	cash = inbuf->ReadLong();
	newlvlname = inbuf->ReadString();
	walltype = inbuf->ReadShort();
	gravityflags = inbuf->ReadShort();

#ifdef	_DEBUG
Printf("Client [CL_StartIntermission] file name [%s]\n", newlvlname);	//  Degauss debug
#endif

	// Attempt to run intermission / buystuff
	if (gamestate == GS_INTERMISSION ||
		gamestate == GS_INTERWAIT)
		return;

	// Set new joinstate
	joinstate = JOIN_INTERMISSION;

	// Get cash amount
	ta[net_localplayer].money = cash;

	// Start intermission
	Inter_Run(newlvlname);

	// Be sure that the game has not ended
	if (joinstate == JOIN_NOTCONNECTED)
		return;

	StartLevel(false);	// dont reset clock shot...still waiting for server
#ifdef	_DEBUG
Printf("Client [CL_StartIntermission] StartLevel(%s)\n", currentlevel.filename);	//  Degauss debug
#endif

	// Modify gravity & wall flags from what the server has setup
//	[Degauss Client stuffs up system if this is done]
//	currentlevel.gravityflags = gravityflags;
//	currentlevel.walltype = walltype;

	// Wait for other clients
	gamestate = GS_INTERWAIT;
	joinstate = JOIN_INTERREADY;

	// Send message to server
	localbuf->WriteByte(CTOS_INTERREADY);
	localbuf->WriteLong(ta[net_localplayer].money);
	localbuf->WriteLong(ta[net_localplayer].defense[parachutes].amount);
	localbuf->WriteLong(ta[net_localplayer].specials[fuel].amount);
	localbuf->WriteLong(ta[net_localplayer].specials[autodefense].amount);

	// Fyah!
	CL_SendPackets();
}

void	CL_GetTankScore(void)
{
	int		t;

	t = inbuf->ReadByte();

	ta[t].kills = inbuf->ReadShort();
	ta[t].score = inbuf->ReadLong();
	ta[t].netstate = inbuf->ReadByte();
}


void	CL_SpawnSprite(void)
{
	int				netid;
	_wobject	wobj;
	int				x;
	int				y;
	_state		state;
	int				tankfrom;
	int				health;
	int				m1;
	int				m2;

	sprite_t	*s = NULL;
	sprite_t	*olds = NULL;

	// Read some varibles
	netid			= inbuf->ReadShort();
	wobj			= (_wobject)inbuf->ReadByte();
	x					= inbuf->ReadShort();
	y					= inbuf->ReadShort();
	state			= (_state)inbuf->ReadShort();
	tankfrom	= inbuf->ReadShort();
	health		= inbuf->ReadShort();
	m1				= inbuf->ReadShort();
	m2				= inbuf->ReadShort();

//#ifdef	_DEBUG
		Printf("CL_SpawnSprite\n");//  Degauss debug
//#endif

		if (gamestate != GS_INGAME &&
		gamestate != GS_INTERWAIT &&
		gamestate != GS_CONNECTING)
			{
			Printf("CL_SpawnSprite: Attempting to spawn a sprite in an invalid gamestate. (state=%d)\n", gamestate);
			return;
			}

  // If the incoming netid has already been assigned via
	// client side, remove the old sprite first before
	// assigning a new number
	olds = G_FindNetID(netid);
	if (olds != NULL)
		olds->Kill();

	// Spawn the sprite and set the state
	s = G_SpawnWO(wobj, x, y);

	// Error?
	if (!s)
		{
		Printf("Spawn sprite [%d] in %s Failed near line:%d", wobj, __FILE__ ,__LINE__);
		return;
		}

	s->netid = netid;
	s->SetState(state);
	s->tankfrom = tankfrom;
	s->health = health;
	s->misc1 = m1;
	s->misc2 = m2;

	// If this is a sentry pod, tint the sprite
  if(tankfrom >= 0)	// Only tint non-system objects (ie tankfrom not equal -1).
	 if(wobj == WO_HEATSENTRYPOD ||
		wobj == WO_ENERGYSENTRYPOD ||
		wobj == WO_BALLISTICSENTRYPOD)
			{
			s->tintr = ta[tankfrom].colorr;
			s->tintg = ta[tankfrom].colorg;
			s->tintb = ta[tankfrom].colorb;
			s->tintvalue = TANK_OPACITY;
			}
}

void	CL_SetSpriteState(void)
{
	int		id = inbuf->ReadShort();
	int		state = inbuf->ReadShort();
	sprite_t*	s = G_FindNetID(id);

	if (gamestate != GS_INGAME)
		return;

	if (!s)
		{
		char	str[64];

		sprintf(str, "CL_SetSpriteState: Unknown sprite %d", id);
		G_Printf(str);
		return;
		}
	s->SetState((_state)state);
}


void	CL_DamageSprite(void)
{
	int		id = inbuf->ReadShort();
	int		amount = inbuf->ReadShort();
	int		scattertext = inbuf->ReadByte();
	char	str[64];

	if(gamestate != GS_INGAME)
		return;

	sprite_t*	s = G_FindNetID(id);

	if(!s)
		{
		sprintf(str, "CL_DamageSprite: Unknown sprite %d", id);
		G_Printf(str);
		return;
		}

	s->health -= amount;

	// Need to die?
	if(s->health <= 0)
		{
		// No longer shootable
		if(s->flags & MF_SHOOTABLE)
			s->flags &= ~MF_SHOOTABLE;

		// Server kills the sprite
		s->health = 0;
		}

	s->healthbartrans = 255;

	// Display damage amount
	sprintf(str, "-%d", amount);
	if(scattertext > 0 || s->wobj != -1)
		{
		SpawnGameText(str, (int)(1.2*TICRATE), s->x+rand()%s->width, s->y+rand()%(s->height/4), 0, -6, makecol(255,0,0), (FONT *)(FONT*)maindata[MS].dat, 0);
		}
	else
		SpawnGameText(str, (int)(1.2*TICRATE), s->x, s->y, 0, -6, makecol(255,0,0));
}


void	CL_ClearAllWOBJS(void)
{
	sprite_t*	s = sprites;

	while (s)
	{
		if (s->wobj >= 0)
			s->Kill();
		s = s->next;
	}

	G_SpriteTicker();
}


void	CL_UpdateTank(void)
{
	int		tank = inbuf->ReadByte();
	int		tflags = inbuf->ReadShort();

	if (tflags & TUPDATE_X)
		ta[tank].x = inbuf->ReadShort();

	if (tflags & TUPDATE_Y)
		ta[tank].y = inbuf->ReadShort();

	if (tflags & TUPDATE_HEALTH)
		ta[tank].health = inbuf->ReadShort();

	if (tflags & TUPDATE_DEG)
	{
		if (tank == net_localplayer && tank == tankturn)
			inbuf->ReadFloat();
		else
			ta[tank].deg = inbuf->ReadFloat();
	}

	if (tflags & TUPDATE_POWER)
	{
		if (tank == net_localplayer && tank == tankturn)
			inbuf->ReadFloat();
		else
			ta[tank].lpower = inbuf->ReadFloat();
	}

	if (tflags & TUPDATE_ACTIVEWEAPON)
	{
		if (tank == net_localplayer && tank == tankturn)
		{
			inbuf->ReadByte();
			inbuf->ReadLong();
		}
		else
		{
			ta[tank].activeweapon = inbuf->ReadByte();
			ta[tank].weapons[ta[tank].activeweapon].ammo = inbuf->ReadLong();
		}
	}

	if (tflags & TUPDATE_FLAGS)
		ta[tank].flags = inbuf->ReadLong();

	if (tflags & TUPDATE_SPECIALFLAGS)
		ta[tank].specialflags = inbuf->ReadLong();

	if (tflags & TUPDATE_ENERGY)
		ta[tank].energyleft = inbuf->ReadLong();

	if (tflags & TUPDATE_FUEL)
		ta[tank].specials[fuel].amount = inbuf->ReadLong();
}

void	CL_GetTankPing(void)
{
	int		tank = inbuf->ReadByte();
	int		ping = inbuf->ReadLong();
	int		framedelay = inbuf->ReadLong();

	ta[tank].ping = ping;
	ta[tank].framedelay = framedelay;
}

void	CL_GetPlayerShot(void)
{
	// Get some varibles from the buffer
	int		tank;
	int		activeweapon;
	float	lpower;
	int		deg;
	int		activeammo;

	tank = inbuf->ReadByte();
	deg = inbuf->ReadByte();
	lpower = inbuf->ReadFloat();
	activeweapon = inbuf->ReadByte();
	activeammo = inbuf->ReadLong();

	if (gamestate != GS_INGAME)
		return;

	if (tank == net_localplayer)
	{
		G_Printf("CL_GetPlayerShot: net_localplayer cannot make a shot!");
		return;
	}

	// Specify last weapon used
	ta[tank].weapons[activeweapon].ammo = activeammo;
	ta[tank].activeweapon = activeweapon;
	ta[tank].deg = deg;
	ta[tank].lpower = lpower;

	ta[tank].shoot();
}


void	CL_ActivateDefense(void)
{
	int		t = inbuf->ReadByte();
	int		def = inbuf->ReadByte();

	if (defenses[def].activateproc != NULL)
		defenses[def].activateproc(t, def);
}

void	CL_ActivateSpecial(void)
{
	int		t = inbuf->ReadByte();
	int		spec = inbuf->ReadByte();

	if (specials[spec].activateproc)
		specials[spec].activateproc(t, spec);
}



void	CL_Printf(void)
{
	char	*str = inbuf->ReadString();

	G_Printf(str);
}


void	CL_SpawnShot(void)
{

	int		s = inbuf->ReadByte();
	int		projectile = inbuf->ReadByte();
	int		tankfrom = inbuf->ReadShort();
	float	deg = inbuf->ReadFloat();
	int		spawnx = inbuf->ReadShort();
	int		spawny = inbuf->ReadShort();
	float	xvel = inbuf->ReadFloat();
	float	yvel = inbuf->ReadFloat();
	int		ns;

	if (gamestate != GS_INGAME)
		return;

	// Attempt to create the shot
	ns = findshot();
	if(ns < 0)
		return;

	sh[ns].Init(ns, projectile, 0,0,0,0,0);

	// Environmental hazard
	sh[ns].tankfrom = tankfrom;
	sh[ns].origdeg = sh[ns].angle = deg;
	sh[ns].origx = sh[ns].x = spawnx;
	sh[ns].origy = sh[ns].y = spawny;
	sh[ns].sprite = G_SpawnSprite(sh[ns].x,
										sh[ns].y,
										projectiles[sh[ns].projectile].state,
										(MF_NORMAL|MF_CENTERCOORD|MF_USEANGLE));
	// Set velocity
	sh[ns].origxvel = sh[ns].xvel = xvel;
	sh[ns].origyvel = sh[ns].yvel = yvel;

	// Play a song if we have one
	if(projectiles[sh[ns].projectile].firesound >= 0)
		{
		sh[ns].StartSound(projectiles[sh[ns].projectile].firesound);
		}

	// Just for the funky bomb
	if(projectile == W_FUNKYBOMBCHILD || projectile == W_FUNKYNUKECHILD)
		{
		int	color;

		if(projectile == W_FUNKYBOMBCHILD)
			sh[ns].InstantHit();	// why not W_FUNKYNUKECHILD ? ? ?

		// Choose a random color
		color = rand()%4;
		if(color == 0)
			sh[ns].pal = (PALETTE *)fpurplepal;
		if(color == 1)
			sh[ns].pal = (PALETTE *)fredpal;
		if(color == 2)
			sh[ns].pal = (PALETTE *)forangepal;
		if(color == 3)
			sh[ns].pal = (PALETTE *)fvoiletpal;
		}

	// Flare tics?
	if(tankfrom >= 0)
		{
		if(projectile == W_MORTARSHELL)
			ta[tankfrom].flaretics = 2;
		else
			ta[tankfrom].SetFlareTics(1);
		}
}


void	CL_MoveTank(void)
{
	int		tank = inbuf->ReadByte();
	int		x = inbuf->ReadShort();
	int		y = inbuf->ReadShort();
	int		famount = inbuf->ReadLong();

	ta[tank].x = x;
	ta[tank].y = y;
	ta[tank].specials[fuel].amount = famount;
}


void	CL_GetClientReady(void)
{
	int	tank = inbuf->ReadByte();

	// Mark player as "ready"
	ta[tank].netstate = NETSTATE_INTERMISSIONREADY;
}


void	CL_GetTurnQue(void)
{
	int		i;
	for (i=0; i<MAX_TANKS; i++)
	{
		tankturnque[i] = inbuf->ReadShort();
	}
}

void	CL_AutoDefenseFinished(void)
{
	// Autodefense has finished for all clients, remove
	// the TANKSPECIAL_AUTODEFENSEWAIT flags
	ta[net_localplayer].specialflags &= ~TANKSPECIAL_AUTODEFENSEWAIT;
}

void	CL_UpdateTankState(void)
{
	int		tank = inbuf->ReadByte();
	int		state = inbuf->ReadByte();

	ta[tank].tankstate = state;
}

void	CL_SendLocalState(void)
{
	localbuf->WriteByte(CTOS_UPDATETANKSTATE);
	localbuf->WriteByte(ta[net_localplayer].tankstate);

	CL_SendPackets();
}

void	CL_TankGib(void)
{
	int		tank = inbuf->ReadByte();

	// Spawn a sprite
	G_SpawnSprite(ta[tank].x, ta[tank].y, S_SMALLEXPLODE_B1, (MF_NORMAL | MF_CENTERCOORD ));

	// ...and play a sound
	ta[tank].StartSound(EXPLODE_TOWERPOD);

	// No longer draw the dying tank...
	ta[tank].needstodie = 0;
}

void	CL_KillShield(void)
{
	int		tank = inbuf->ReadByte();

	ta[tank].KillShield();
}


void	CL_TankText(void)
{
	int		tank = inbuf->ReadByte();
	char	*str = inbuf->ReadString();
	int		timer = inbuf->ReadShort();

	CharacterSay(str, tank, timer);
}

void	CL_ResetShotClock(void)
{
	int		time = inbuf->ReadLong();

	tank_shottimer = time;
	tank_orginalshottimer = time;
}

void	CL_UpdateTankShotTimer(void)
{
	int		time = inbuf->ReadLong();

//	tank_shottimer = time;
#ifdef	_DEBUG
	Printf("CL_Timer:%d Server:%d\n",tank_shottimer,time);	//  Degauss debug
#endif
}

void	CL_LightningStrike(void)
{
	// Particle effect
	particlenode_t	*p;
	int		nump = 15;
	int		plife;
	int		pangle;

	int		i;

	int		lightx = inbuf->ReadShort();
	int		lighty = inbuf->ReadShort();

    sprite_t* l = G_SpawnSprite(lightx-25, lighty-SCREENHEIGHT, S_LIGHTNINGROD_LIGHTNING1, (MF_NORMAL));

	// Oh yeah, might wanna burn the ground
	BurnGround(ground.bmp, lightx, lighty, 40, 50);
	BurnGround(ground.bmp, lightx, lighty, 20, 100);
	BurnGround(ground.bmp, lightx, lighty, 10, 150);
	BurnGround(ground.bmp, lightx, lighty, 5,  200);

	// White flash!
	currentlevel.whiteflashtics = 1;

	if (rand()%2==0)
		StartSound(THUNDERHIT_1);
	else
		StartSound(THUNDERHIT_2);

	// Spawn some particles, just for looks
	for (i=0; i<nump; i++)
	{
		// Set particle life, randomize this a bit
		plife = 30+rand()%30;

		// Randomize the angle as well
		pangle = 45+rand()%40;

		p = pdirt->InsertParticle(lightx, lighty-10, rand()%128, ConvertFromGamePower(300+rand()%100), makecol(200,200,255), plife, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
		if (p)
			p->bounce = 5;
	}
}


void		CL_LavaShoot(void)
{
	sprite_t*		s = G_FindNetID(inbuf->ReadByte());

	if (!s)
	{ G_Printf("CL_LavaShoot: Cannot find sprite!"); return; }

	s->life = 0;

	// Play a sound
	s->StartSound(LAVASHOOT_FIRE);

	s->SetState(S_LAVASHOOTER_ATTACK2);
}


void	CL_DisconnectClient(void)
{
	int		tank = inbuf->ReadByte();

	ta[tank].Disconnect();
}


void	CL_ControlSprite(void)
{
	sprite_t	*s = G_FindNetID(inbuf->ReadByte());
	int			newowner = inbuf->ReadByte();

	if (!s)
	{
		G_Printf("CL_ControlSprite: Cannot find sprite!");
		return;
	}

	// Convert it
	s->tankfrom = newowner;

	if (s->wobj == WO_ENERGYSENTRYPOD ||
		s->wobj == WO_HEATSENTRYPOD ||
		s->wobj == WO_BALLISTICSENTRYPOD)
	{
		s->StartSound(SENTRYPOD_EMP);
		s->tintr = ta[newowner].colorr;
		s->tintg = ta[newowner].colorg;
		s->tintb = ta[newowner].colorb;
	}
}


void	CL_SpawnTowerPod(void)
{
	int		sid;
	int		wobj;
	int		tankfrom;
	int		digx;
	int		digy;
	int		bmpw,bmph;

	sprite_t	*tower;

	sid = inbuf->ReadByte();
	wobj = inbuf->ReadByte();
	tankfrom = inbuf->ReadByte();
	digx = inbuf->ReadShort();
	digy = inbuf->ReadShort();
	bmpw = inbuf->ReadShort();
	bmph = inbuf->ReadShort();

	if (gamestate != GS_INGAME)
		return;

	// Create the rectangle into the dirt
	rectfill(ground.bmp, digx, digy, (digx + bmpw), (digy + bmph), TRANSCOLOR);	//	Dirt Sync

	ground.needsupdate = true;

	// Spawn the sprite
	tower = G_SpawnWO((_wobject)wobj, digx, digy);

	if (tower)
	{
		tower->tankfrom = tankfrom;
		tower->netid = sid;
	}

	// Collaspe the ground
	ground.restartcheck(0,0,SCREENWIDTH, SCREENHEIGHT);
}

/* not used any more
void	CL_SpawnTreasure(void)
{
	int		sid;
	int		wobj;
	int		goldx;
	int		goldy;
	int		m1,m2;
//	process a STOC_TREASURE request here

	sprite_t	*gold;

	sid   = inbuf->ReadShort();
	wobj  = inbuf->ReadByte();
	goldx = inbuf->ReadShort();
	goldy = inbuf->ReadShort();
	m1    = inbuf->ReadShort();
	m2    = inbuf->ReadShort();

	if(gamestate != GS_INGAME) // clear buffer before testing
		return;

	// Spawn the sprite
	gold = G_SpawnWO((_wobject)wobj, goldx, goldy);

	if(gold)
		{
		gold->tankfrom	= -1; // belongs to system not a tank
		gold->netid			= sid;
		gold->misc1			= m1;
		gold->misc2			= m2;
		}
}
*/

/* not used any more
void	CL_MagGen(void)
{
	int		sid;
	int		wobj;
	int		genx;
	int		geny;
	int		m1,m2;

	sprite_t	*gen;
	sid  = inbuf->ReadByte();
	wobj = inbuf->ReadByte();
	genx = inbuf->ReadShort();
	geny = inbuf->ReadShort();
	m1   = inbuf->ReadShort();
	m2   = inbuf->ReadShort();

	if(gamestate != GS_INGAME)
		return;

	// Spawn the sprite
	gen = G_SpawnWO((_wobject)wobj, genx, geny);

	if(gen)
		{
		gen->tankfrom = -1;
		gen->netid = sid;
		gen->misc1 = m1;	// decay  default: 120
		gen->misc2 = m2;	// power	default: 3
		}
}
*/
void Read_InventInfo(void);

void	CL_Script(void)
{
	//	sets up ScriptAdd[][]
	Read_InventInfo();
}

void	CL_DoMissingFrames(void)
{
	int		gtics = inbuf->ReadLong();

	char	str[64];
	sprintf(str, "delay: %d", gametics - gtics);
	G_Printf(str);

}

void	CL_SetBlowUp(void)	//	client explode
{
//	process STOC_BLOWUP
	int bangx = inbuf->ReadShort();
	int bangy = inbuf->ReadShort();
	int bangr = inbuf->ReadShort();	//	the only place to destroy dirt

	ServerClient = 's';							// mark as a server request
	DestroyDirt(bangx,bangy,bangr);
	ServerClient = 'c';						// reset as a client request
	return;
}

void	CL_SetPingWait(void)
{
	int	time	= inbuf->ReadLong();

	if (time <= 0)
		time = 1;

	checktanktimer = time;
}

void	CL_SwitchTankTeam(void)
{
	int		tank = inbuf->ReadByte();
	int		t = inbuf->ReadShort();
	int		r = inbuf->ReadByte();
	int		g = inbuf->ReadByte();
	int		b = inbuf->ReadByte();

	//if (gamestate != GS_INGAME)
	//	return;

	ta[tank].SetTeamColor(t);

	ta[tank].colorr = r;
	ta[tank].colorg = g;
	ta[tank].colorb = b;
}

void	motdtext_callback(int t)
{
	if (gametext[t].timer <= 2*TICRATE)
	{
		gametext[t].opacity -= 3;
	}
}

//	a more generic message routine.
void ShowMsg(char* motd)
{
	char	clientmotd[1024];
	char	string[1024];
	int		i,linepos=0;

	int		textx = SCREENWIDTH/2;
	int		texty = 180;
	int		gt;

	// Print it to the console first
	memcpy(clientmotd, motd, 1024);

//	Printf("MOTD:\n%s\n\n", clientmotd);

	for (i=0; i<1024; i++)
	{
		if (clientmotd[i]=='<' &&
			clientmotd[i+1]=='b' &&
			clientmotd[i+2]=='r' &&
			clientmotd[i+3]=='>')
		{
			i+=3;

			string[linepos] = '\0';
			Printf("%s\n", string);

			linepos = 0;

			// Add this string into game text
			gt = SpawnGameText(string, (10*TICRATE), (int)textx+2, (int)texty+2, 0, 0, 0);
			gametext[gt].loopfunc = motdtext_callback;

			gt = SpawnGameText(string, (10*TICRATE), (int)textx, (int)texty, 0, 0, makecol(255,255,255));
			gametext[gt].loopfunc = motdtext_callback;

			texty += (5 + text_height((FONT *)maindata[MS].dat));
		}
		else
		{
			string[linepos] = clientmotd[i];
			linepos++;
		}
	}

	if (linepos > 0)
	{
		string[linepos] = '\0';
		Printf("%s\n", string);

		linepos = 0;

		// Add this string into game text
		gt = SpawnGameText(string, (10*TICRATE), (int)textx+2, (int)texty+2, 0, 0, 0);
		gametext[gt].loopfunc = motdtext_callback;

		gt = SpawnGameText(string, (10*TICRATE), (int)textx, (int)texty, 0, 0, makecol(255,255,255));
		gametext[gt].loopfunc = motdtext_callback;
		texty += (5 + text_height((FONT *)maindata[MS].dat));
	}

}

void	CL_StartMOTD(void)
{
	char	*motd = inbuf->ReadString();
	ShowMsg(motd);
}


//
// CL_KickPlayer - Kicks us from the game. (we've been naughty)
//

void	CL_KickPlayer(void)
{
	char	*reason = inbuf->ReadString();

	// Stop any music
	StopLevelOGG();

	agup_buffer = screen;

	menuactive = false;

	// Display message first
	alert("You've been kicked from the game.", NULL, reason, "Ok", NULL, 0, 0);

	gamestate = GS_TITLE;

	// Disconnect
	CL_Disconnect();

	GUI_SetupMenu(multiplayer_menu);

	// Set back into the server listing
	joingame();
}

void	CL_BanPlayer(void)
{
	char	*reason = inbuf->ReadString();

	// Stop any music
	StopLevelOGG();

	menuactive = false;

	agup_buffer = screen;

	// Display message first
	alert("BANNED", NULL, reason, "Ok", NULL, 0, 0);

	gamestate = GS_TITLE;

	// Disconnect
	CL_Disconnect();

	GUI_SetupMenu(multiplayer_menu);

	// Set back into the server listing
	joingame();
}


void	CL_PasswordRequest(void)
{
	int		us = inbuf->ReadByte();

	net_localplayer = us;

	// Updating
	StatBar("Server is locked with a password.");

	// Start the dialog and collect the password
	getpassword_start();

	// Submit it
	localbuf->WriteByte(CTOS_SENDPASSWORD);
	localbuf->WriteString(password->string);

	totics = gametics;

	// Updating
	StatBar("Verifing password...");

	CL_SendPackets();
}


void	CL_SpawnAirShip(void)
{
	sprite_t	*as;
	int			id;
	int			wobj;
	int			x,y;
	float		xvel,yvel;
	_state		state;
	int			tankfrom;
	int			flags;
	int			misc1;

	// Recieve some varibles
	id = inbuf->ReadShort();
	wobj = inbuf->ReadByte();
	x = inbuf->ReadShort();
	y = inbuf->ReadShort();
	xvel = inbuf->ReadFloat();
	yvel = inbuf->ReadFloat();
	state = (_state)inbuf->ReadShort();
	tankfrom = inbuf->ReadShort();
	flags = inbuf->ReadShort();
	misc1 = inbuf->ReadShort();

	if (gamestate != GS_INGAME)
		return;

	as = G_SpawnSprite(x,y, state, flags);

	if (as)
	{
		as->xvel = xvel;
		as->yvel = yvel;
		as->tankfrom = tankfrom;
		as->misc1 = misc1;
	}
}


void	CL_UpdateAirShip(void)
{
	sprite_t	*s = G_FindNetID(inbuf->ReadByte());
	int			x = inbuf->ReadShort();
	int			y = inbuf->ReadShort();

	if (s)
	{
		s->x = x;
		s->y = y;
	}
}

//
// ClientTank_Thinker
//

void	ClientTank_Thinker(void)
{
	int		tflags=0;

	if (gamestate != GS_INGAME || !multiplayer)
		return;

	// Defense mode?
	if (tankturn == net_localplayer)
	{
		//BCast_WriteByte(net_localplayer, CTOC_TANKSTATE);
		//BCast_WriteByte(net_localplayer, ta[net_localplayer].tankstate);

		if (key[KEY_A] || key[KEY_S])
		{
			if (multiplayermode == MP_CLIENT && IsTankMessage() <= 0)
			{
				// Send request for server to move us
				localbuf->WriteByte(CTOS_MOVETANK);

				// Determine which way to request
				if (key[KEY_A])
					localbuf->WriteShort(-1);
				else if (key[KEY_S])
					localbuf->WriteShort(1);
			}
		}
	}

	// Our turn?
	if (tankturn == net_localplayer &&
		multiplayermode == MP_CLIENT)
	{
		// Reset
		tflags = 0;

		if (ta[tankturn].deg != ta[tankturn].olddeg)
			tflags |= TUPDATE_DEG;

		if (ta[tankturn].lpower != ta[tankturn].oldlpower)
			tflags |= TUPDATE_POWER;

		if (ta[tankturn].activeweapon != ta[tankturn].oldactiveweapon)
			tflags |= TUPDATE_ACTIVEWEAPON;

		// Nothing changed
		if (tflags > 0)
		{

			// Parse and find out what kind of info needs to be sent and send it to
			// all players
			localbuf->WriteByte(CTOS_TANKTURNUPDATE);
			localbuf->WriteShort(tflags);

			if (tflags & TUPDATE_DEG)
				localbuf->WriteFloat(ta[tankturn].deg);

			if (tflags & TUPDATE_POWER)
				localbuf->WriteFloat(ta[tankturn].lpower);

			if (tflags & TUPDATE_ACTIVEWEAPON)
			{
				localbuf->WriteByte(ta[tankturn].activeweapon);
				localbuf->WriteLong(ta[tankturn].weapons[ta[tankturn].activeweapon].ammo);
			}
		}
	}

	// Send packets
	if (multiplayermode == MP_SERVER)
		SV_SendPackets();
	else
		CL_SendPackets();
}


void	CL_HellStrike(void)
{
	sprite_t*		l;
	BITMAP			*damagebmp = (BITMAP *)spritedata[LIGHTNINGROD_HSDAMAGE].dat;
	int				sx = inbuf->ReadShort();
	int				sy = inbuf->ReadShort();
	int				groundy = OnGround(sx, 0, ground.bmp);
	int				i;
	particlenode_t	*p;
	int				numburn = 15;

	// Particle effect
	int		nump = 40;
	int		plife;
	int		pangle;
	//////////////////////////////////////////////

	l = G_SpawnSprite(sx, sy, S_LIGHTNINGROD_HELLSTRIKE1, (MF_NORMAL));

	// Oh yeah, might wanna burn the ground
	for (i=0; i<numburn; i++)
	{
		int		burnx;
		int		burny;

		burnx = (sx-(damagebmp->w/2));
		burny = groundy;//(groundy-(damagebmp->h/2));

		burnx += rand()%damagebmp->w;
		burny += rand()%damagebmp->h;

		BurnGround(ground.bmp, burnx, burny, 20+rand()%10, 125+rand()%35);
	}

	// Play da sound...
	StartSound(TEL_LIGHTNING);

	// White flash!
	currentlevel.whiteflashtics = 1;

	DrawGroundSubtract(damagebmp, ground.bmp, sx-(damagebmp->w/2), groundy+5);

	// Spawn some particles, just for looks
	for (i=0; i<nump; i++)
	{
		// Set particle life, randomize this a bit
		plife = 120+rand()%30;

		// Randomize the angle as well
		pangle = 45+rand()%40;

		p = pdirt->InsertParticle(sx, groundy-10, rand()%128, ConvertFromGamePower(400+rand()%100), makecol(200,200,255), plife, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
		if (p)
			p->bounce = 5;
	}

}





//
// Client_ParseData
//

void	Client_ParseData(void)
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
			// Master to client stuff
			case	MTOC_SERVERLIST:
				BuildServerList();
				break;

			case	LAN_SENDSERVERINFO:
				RecieveLANServer();
				break;

			// We're attempting to listen to ourself, ingore it
			case	LAN_REQUESTSERVERINFO:
				break;

			case	MTOC_ERROR:
				GetMasterError();
				break;

			// Server to client
			case	STOC_PINGUPDATE:
				CL_ReplyPingUpdate();
				break;

			case	STOC_STARTPLAYERSELCECTION:
				CL_SetupPlayer();
				break;

			case	STOC_SETUPLEVEL:
				CL_SetupLevel();
				break;

			case	STOC_PLAYERINFO:
				CL_GetPlayerInfo();
				break;

			case	STOC_ENTERGAME:
				CL_StartGame();
				break;

			case	STOC_UPDATETANKTURN:
				CL_GetUpdatedTankturn();
				break;

			case	STOC_DAMAGETANK:
				CL_DamageTank();
				break;

			case	STOC_KILLTANK:
				CL_KillTank();
				break;

			case	STOC_DAMAGETANKFALL:
				CL_DoFallingDamage();
				break;

			case	STOC_STARTINTERMISSION:
				CL_StartIntermission();
				break;

			case	STOC_UPDATETANKSCORE:
				CL_GetTankScore();
				break;

			case	STOC_SPAWNSPRITE:
				CL_SpawnSprite();
				break;

			case	STOC_SPRITESTATE:
				CL_SetSpriteState();
				break;

			case	STOC_DAMAGESPRITE:
				CL_DamageSprite();
				break;

			case	STOC_FULLSPRITEUPDATE:
				CL_ClearAllWOBJS();
				break;

			case	STOC_ERRORTEXT:
				CL_DisconnectError();
				break;

			case	STOC_CHATMESSAGE:
				CL_GetChatMessage();
				break;

			case	STOC_UPDATETANK:
				CL_UpdateTank();
				break;

			case	STOC_SENDPINGUPDATE:
				CL_GetTankPing();
				break;

			case	STOC_MADESHOT:
				CL_GetPlayerShot();
				break;

			case	STOC_ACTIVATEDEFENSE:
				CL_ActivateDefense();
				break;

			case	STOC_ACTIVATESPECIAL:
				CL_ActivateSpecial();
				break;

			case	STOC_PRINTF:
				CL_Printf();
				break;

			case	STOC_UPDATESHOTS:
				CL_SpawnShot();
				break;

			case	STOC_MOVETANK:
				CL_MoveTank();
				break;

			case	STOC_CLIENTREADY:
				CL_GetClientReady();
				break;

			case	STOC_UPDATETURNQUE:
				CL_GetTurnQue();
				break;

			case	STOC_AUTODEFENSEFINISHED:
				CL_AutoDefenseFinished();
				break;

			case	STOC_UPDATETANKSTATE:
				CL_UpdateTankState();
				break;

			case	STOC_TANKGIB:
				CL_TankGib();
				break;

			case	STOC_KILLSHIELD:
				CL_KillShield();
				break;

			case	STOC_FINALINTERMISSION:
				CL_StartFinalIntermission();
				return;

			case	STOC_TANKTALK:
				CL_TankText();
				break;

			case	STOC_SHOTCLOCK:
				CL_ResetShotClock();
				break;

			case	STOC_LIGHTNINGSTRIKE:
				CL_LightningStrike();
				break;

			case	STOC_LAVASHOOT:
				CL_LavaShoot();
				break;

			case	STOC_PLAYERDISCONNECT:
				CL_DisconnectClient();
				break;

			case	STOC_CONVERTSENTRY:
				CL_ControlSprite();
				break;

			case	STOC_SPAWNTOWERPOD:
				CL_SpawnTowerPod();
				break;

			case	STOC_FRAMEUPDATE:
				CL_DoMissingFrames();
				break;

			case	STOC_SETPINGWAIT:
				CL_SetPingWait();
				break;

			case	STOC_SETTANKTEAM:
				CL_SwitchTankTeam();
				break;

			case	STOC_MOTD:
				CL_StartMOTD();
				break;

			case	STOC_KICKPLAYER:
				CL_KickPlayer();
				break;

			case	STOC_BANPLAYER:
				CL_BanPlayer();
				break;

			case	STOC_REQUESTPASSWORD:
				CL_PasswordRequest();
				break;

			case	STOC_HELLSTRIKE:
				CL_HellStrike();
				break;

			case	STOC_SPAWNAIRSHIP:
				CL_SpawnAirShip();
				break;

			case	STOC_UPDATEAIRSHIP:
				CL_UpdateAirShip();
				break;

			case	STOC_SPAWNFUNKYTHINKER:
				inbuf->ReadByte();	//	delete byte and ignore.
				//	this is done as a standard shot under STOC_UPDATESHOTS
				//G_Printf("SPAWN FUNKY THINKER missing");
				break;

			case	STOC_UPDATETIMER:
				CL_UpdateTankShotTimer();
				break;

			case	STOC_BLOWUP:
				CL_SetBlowUp();
				break;

			case	STOC_SYNC:	// sync gametics
				{
				int gt = inbuf->ReadLong();
				gametics = gt;
				Printf("SYNC");
				}
				break;

			case	STOC_SPAWNMAGGEN:	// spare not used
			//	CL_MagGen();
				break;

			case	STOC_Script:
					CL_Script();
				break;

			case	STOC_DropDirt:
					ground.falling = true;
					ground.fall();
					Printf("Drop ");
				break;

			case	MSG_EOP:
				break;

			// Unknown
			default:
				{
					sprintf(str, "Unknown message: %d (last: %d)", msg, lastmsg);
					G_Printf(str);
					return;
				}
		}

		lastmsg = msg;
	}

}

void	CL_GetPackets(void)
{
	int		numbytes;
	int		err;
	char	errortext[64];
	char	str[128];

	// Get number of bytes
	while((numbytes = netsocket->GetPacket()) > 0)
	{

	// If we're not in multiplayer mode, recieve the packet still so that the socket buffer
	// isn't clogged with crap
	if (!multiplayer)
		return;

	// Detect any errors encountered
	if (numbytes == SOCKET_ERROR)
	{
		err = WSAGetLastError();

		if (err == WSAECONNRESET)
			sprintf(errortext, "Connection reset by peer");
		else if (err == WSAETIMEDOUT)
			sprintf(errortext, "Socket timed out");
		else if (err == WSAECONNABORTED)
			sprintf(errortext, "Server host has closed the game.");	//	"Software caused connection abort");
		else if (err == WSAENETRESET)
			sprintf(errortext, "Network dropped the connection on reset");
		else
			sprintf(errortext, "Unknown winsock error: %d", err);

		if (gamestate == GS_INTERMISSION)
		{
			G_Printf("Host has disconnected.");

			// Stop the music!
			StopLevelOGG();

			// Close the socket
			closesocket(netsocket->sock);

        	// No longer in multiplayer mode...set the appropiate varibles
			multiplayermode = -1;
			multiplayer = 0;
			joinstate = JOIN_NOTCONNECTED;

			gamestate = GS_TITLE;
			GUI_SetupMenu (main_menu);
		}
		else
		{
			sprintf(str, "Server error (%s)", errortext);
			CL_ConnectError(str);
		}

		return;
	}

	// Go no futher if we didn't recieve anything
	if (numbytes > 0)
	{
		// Update timeout tics
		totics = gametics;

		// Parse server data
		Client_ParseData();
	}

	}

	// UDP socket now
	numbytes = udpsocket->GetPacket(PACKUDP);
	if (numbytes > 0)
	{
		totics = gametics;
		Client_ParseData();
	}
}
