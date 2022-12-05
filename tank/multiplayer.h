#include "netcommon\include\netcommon.h"

//
// COMMON PREPROCESSORS
//

#define	CLIENT_PORT		10665
#define	SERVER_PORT		10666
#define	MASTER_PORT		10667
#define UDP_PORT		  10668		// Server -> Master port

#define	MAX_SERVERLIST		50
#define	UNKNOWN_PING		9999

#define	CLIENT_TIMEOUT_TICS		30*TICRATE
#define CLIENT_SETUPTIME		  45*TICRATE		// How long to give the player to setup their character
#define	CLIENT_BUYSTUFFTIME		60*TICRATE		// How much time to allow players to buy some stuff

// Netstates
#define	NETSTATE_DISCONNECTED		0	// Nothing
#define	NETSTATE_CONNECTED			1	// Connected, waiting for client to finish setting their player up
#define	NETSTATE_BUYSTUFF			2	// Player is currently buying weapons
#define	NETSTATE_BUYSTUFFREADY		3
#define	NETSTATE_LEVELSETUP			4	// Sets up player info, sprite info...etc
#define	NETSTATE_INGAME				5	// In-game playing
#define	NETSTATE_INTERMISSION		6	// Intermission mode
#define	NETSTATE_INTERMISSIONREADY	7	// Client is ready for the next level


#define			JOIN_TIMEOUTTIME	10*TICRATE

#define			JOIN_NOTCONNECTED	0	// Not connected
#define			JOIN_MASTERCONNECT	1	// Connecting...
#define			JOIN_SERVERLISTING	2	// Requesting server info
#define			JOIN_IDLE			3	// All is done, idleing
#define			JOIN_READY			4	// Ready for input from the player
#define			JOIN_CONNECTING		5	// Waiting for feedback from the server
#define			JOIN_PLAYERSETUP	6	// Setting our character up
#define			JOIN_BUYSTUFF		7	// Purchasing stuff
#define			JOIN_BUYSTUFFREADY	8	// Purchasing stuff
#define			JOIN_PLAYING		9	// Currently playing the game
#define			JOIN_INTERMISSION	10	// Client is in intermission mode
#define			JOIN_INTERREADY		11	// Local client is ready for next map
#define			JOIN_SEARCHLAN		12	// Searching for LAN games

// Wait states (waiting for clients to finish these specific actions)
#define			WAITSTATE_IDLE			0
#define			WAITSTATE_SHOT			1
#define			WAITSTATE_AFTERSHOTS	2
#define			WAITSTATE_TANKDEATHS	3
#define			WAITSTATE_INTERMISSION	4

//
// COMMON VARIBLES
//
extern	int				localport;
extern	int				localudpport;
extern	netsock_t		*netsocket, *udpsocket;
extern	int				totics;			// Timeout tics
extern	int				pingwaitstate;
extern	int				pingwaittime;
extern	char			clientmotd[1024];

extern	cvar_t			*num_players;
extern	cvar_t			*num_rounds;
extern	cvar_t			*server_name;
extern	cvar_t			*teamplay;
extern	cvar_t			*friendlyfire;
extern	cvar_t			*motd;
extern	cvar_t			*password;
extern	cvar_t			*requirepassword;
extern	cvar_t			*forcetime;

//
// COMMON FUNCTIONS
//
void	Net_Thinker(void);
void	Net_Update(void);
void	StatBar(char *text);
void	CheckForConnect(void);
void	InitNetwork(void);
void	ResetSocket(bool listening);
void	ShutdownNetwork(void);
void	CL_ConnectError(char *str);
void	AttemptJoinConnection(netaddress_t	*adr);

void	StartJoinGame(char	*address);

void	BeginDedicatedGame(void);
char	*D_ConsoleInput (void);

int		FindClient(netaddress_t	adr);
int		FindNewClient(void);
void	JoinGame_Setup(BITMAP *b);
void	CL_GetPackets(void);
void	Client_ParseData(void);

void	CL_Write (packet_t *p, void *ddata, int length);
void	CL_Write (packet_t *p, byte *ddata, int startpos, int length);
void	CL_WriteChar (packet_t *p, int c);
void	CL_WriteByte (packet_t *p, int c);
void	CL_WriteShort (packet_t *p, int c);
void	CL_WriteWord (packet_t *p, short v);
void	CL_WriteLong (packet_t *p, int c);
void	CL_WriteFloat (packet_t *p, float f);
void	CL_WriteString (packet_t *p, char *s);

void	BCast_Write (void *ddata, int length);
void	BCast_Write (byte *ddata, int startpos, int length);
void	BCast_WriteChar (int c);
void	BCast_WriteByte (int c);
void	BCast_WriteShort (int c);
void	BCast_WriteWord (short v);
void	BCast_WriteLong (int c);
void	BCast_WriteFloat (float f);
void	BCast_WriteString (char *s);

void	BCast_Printf(char	*str);


//
// SERVER VARIBLES
//
extern	char			masteraddr[];
extern	char			servername[32];
extern	int				maxplayers;


//
// CLIENT STRUCTURES
//

typedef	struct
{
	char			name[64];
	int				aitype;
	int				score;
} playerlist_t;

typedef	struct
{
	char			name[64];
	char			levelname[64];
	int				numplayers;
	int				maxplayers;
	int				active;
	int				lasttics;
	netaddress_t	address;		// has udp port
	int				tcpport;		// tcp port

	int				wallflags;
	int				gravityflags;
	int				startingcash;
	char			host[64];
	int				numroundsleft;
	int				totalrounds;
	int				ping;

	bool			lanserver;

	// Players
	playerlist_t	tanks[MAX_TANKS];

} serverlist_t;


//
// CLIENT VARIBLES
//
extern	DIALOG			dialog_joingame[];
extern	int				selectedserver;
extern	int				numservers;
extern	serverlist_t	serverlist[MAX_SERVERLIST];
extern	netaddress_t	serveraddr;
extern	char			statustext[64];
extern	int				joinstate;
extern	int				net_localplayer;


//
// CLIENT FUNCTIONS
//
void	ClientTank_Thinker(void);
void	CL_SendChatMessage(char	*msg);
void	CL_Disconnect(void);
void	CL_SendPackets(void);
void	CL_SendLocalState(void);
void	clientmotd_start(char	*text);

//
// SERVER FUNCTIONS
//
void	SV_MasterConnect(void);
void	SV_UpdateMaster(bool localreply=false);
void	SV_ShutdownServer(void);
void	net_error(void);
void	SV_TankThinker(void);
void	SV_InitNewClient(int tank);
void	SV_UpdateClients(void);
void	SV_GetPlayerInfo(int tank);
void	SV_SendFullSpriteUpdate(int tank);
void	SV_ClientError(netsock_t* sock, netaddress_t addr, char *msg);
void	SV_ClientError(int tank, char *msg);
void	SV_SendUpdate(void);
void	SV_SendPackets(void);
void	SV_GetPackets(void);
void	SV_ProcessPingUpdates(void);
bool	SV_IsAllPlaying(void);
void	SV_CheckAutoDefenseClosed(void);
void	SV_SendLocalState(int i);
void	SV_ResetShotClock(int timer);
bool	SV_AllPingReady(void);
void	SV_ResetPingWait(void);
unsigned int	I_GetTime (void);
int				I_GetTimePolled (void);

void	SV_ReadBans(void);
void	SV_KickPlayer(int pnum, char *reason);
int		SV_EnforceBans(int cl);
void	SV_ReReadBans(void);

// Server console functions
void	Cmd_ListPlayers_f(void);
void	Cmd_Killban_f(void);
void	Cmd_Addban_f(void);
void	Cmd_BanList_f(void);
void	Cmd_KickPlayer_f(void);

// Server packet stuff
void	SV_SendFullUpdate(int tank);
void	SV_GetCharacterSelection(int tank);
void	SV_ClientReady(int tank);
