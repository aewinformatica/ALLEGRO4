#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "dialogs.h"
#include "scorch.h"

/*
 * Ban / kicks
 *
 * For many users and server admins, this is the juicy part
 * Kicking people and banning them as well =)
 *
 * But hey, whos going to protect us from the ass holes?
 */
#define		LOCAL_BANFILE			"cdserv_bans.txt"

#define MAX_BANS	512				// That should be enough :-)

/*
 * Characteristics of a single ban. The IP address is a 4-byte value
 * as usual, with an important difference: for each IP address there
 * is an accompanying 4-byte "mask" array. If a mask byte is 1, then
 * the corresponding byte in the IP address is a wildard (ie., it will
 * match to any value supplied by the client for that particular byte.
 * This lets us use for example ip addresses of the form: 192.168.1.*
 * where the * character means that the ban should apply to any address
 * whose first three bytes are 192.168.1 and the last byte could be
 * anything. In the context of zserv, this can be helpful to admins,
 * since the IP address of a particular player will not remain constant
 * if the player is on a dial-up line.
 */
typedef struct
{
	byte	ip[4];			// ip address
	byte	mask[4];		// if 1 then corresponding byte in ip address is a wildcard
	bool	is_local;
	char	reason[1];		// Why we kick him
} ban_t;

class ban_cl
{
	private:

	ban_t	*bans[MAX_BANS];
	int		nbans;
	int		find(const byte *ip, const byte *mask);
	void	init();
	bool	address_matches(const byte *claddr,const byte *banip, const byte *mask);

	public:
			ban_cl();
			~ban_cl();
	void	flush();
	void	show();
	void	save(const char *fname);
	bool	add(const byte *ip,const byte *mask,const char *reason, bool is_local);
	bool	remove(const byte *ip,const byte *mask);
	void	enforce_to_all(void);
	bool	enforce_to_player(int cl);

	char	*ip_to_str(const byte *ip,const byte *mask);
	bool	str_to_ip(const char *s,byte *ip,byte *mask);
};

/*
 * Concatenate all command arguments into a single string
 */
static char *collect_args(void)
{
	int			i, last;
	char		*p;
	static char	buf[1024];

	memset(buf, 0, sizeof(buf));
	p = buf;
	last = Cmd_Argc()-1;
	for (i=2; i<=last; i++)
	{
		if (i>2) *p++ = ' ';
		p += sprintf(p,"%s",Cmd_Argv(i));
	}
	return buf;
}


/*
 * Convert an ip address/mask to printable form
 */
char *ban_cl::ip_to_str(const byte *ip,const byte *mask)
{
	char		*p;
	int			i;
	static char buf[32];

	p = buf;
	for (i=0; i<4; i++)
	{
		if (i) *p++ = '.';
		if (mask[i])	p += sprintf(p,"*");
		else			p += sprintf(p,"%u",(unsigned)ip[i]);
	}
	return buf;
}

/*
 * Read an ip address/mask from a string. We allow the *
 * character in place of any byte the IP address. Also we
 * allow truncated addresses for compatibility purposes;
 * in those cases, the remaining bytes in the address will
 * be considered as wildcards.
 */
bool ban_cl::str_to_ip(const char *s,byte *ip, byte *mask)
{
	int			i;
	unsigned	v;
	char		c;

	memset(ip,0,4);
	memset(mask,1,4);

	for (i=0; i<4; i++)
	{
		c = *s++;
		if (c=='*')
		{
			mask[i] = 1;
			ip[i] = 0;
			c = *s++;
		}
		else
		{
			mask[i] = 0;
			if (c<'0' || c>'9') return false;
			v = 0;
			while (c>='0' && c<='9')
			{
				v = 10*v + (int)(c-'0');
				c = *s++;
			}
			if (v>255) return false;
			ip[i] = (byte) v;
		}
		if (i<3)
		{
			if (c=='\0')	// Truncated address: accept it for compatibility
			{				// with previous versions of zdaemon
				return true;
			}
			if (c!='.') return false;
		}
		else
		{
			if (c) return false;
		}
	}
	return true;
}

/*
 * Ban list constructor; simply set everything to zero
 */
ban_cl::ban_cl()
{
	init();
}

/*
 * Ban list destructor; clear all used memory and reset to zero
 */
ban_cl::~ban_cl()
{
	flush();
}

/*
 * Initialization stuff: reset all to zsero
 */
void ban_cl::init()
{
	nbans = 0;
	memset(bans,0,sizeof(bans));
}

/*
 * Clear all used memory and reset everything to zero
 */
void ban_cl::flush()
{
	int i;

	for (i=0; i<nbans; i++)
		free(bans[i]);
	init();
}

/*
 * Print the ban list on the screen
 */
void ban_cl::show(void)
{
	int		i;

	Printf("\n *********** BANS ***********\n");
	for (i=0; i<nbans; i++)
	{ Printf("%s (%s)\n", ip_to_str(bans[i]->ip,bans[i]->mask), bans[i]->reason); }
	Printf(" ****************************\n");
}

/*
 * Save the ban list to a text file
 */
void ban_cl::save(const char *filename)
{
	FILE	*f;
	int		i;

	if ( (f=fopen(filename, "w")) == NULL )
	{
		Printf("*** Unable to create ban log file!\n");
		return;
	}
	fprintf(f, ";\n");
	fprintf(f, ";--------------------------\n");
	fprintf(f, "; CDSERV BAN FILE\n");
	fprintf(f, ";--------------------------\n");
	fprintf(f, ";\n;\n");

	for (i=0; i<nbans; i++)
		if (bans[i]->is_local)
			fprintf(f, "%s#%s\n", ip_to_str(bans[i]->ip,bans[i]->mask), bans[i]->reason);

	fclose(f);
}

/*
 * Find the ban that corresponds to the specified IP address and mask.
 * Returns -1 if not found.
 */
int ban_cl::find(const byte *ip, const byte *mask)
{
	int i;

	for (i=nbans-1; i>=0; i--)
	{
		if (!memcmp(bans[i]->ip,ip,4) && !memcmp(bans[i]->mask,mask,4))
			break;
	}
	return i;
}

/*
 * Add a ban to the ban linst and complain if something goes wrong
 */
bool ban_cl::add(const byte *ip, const byte *mask, const char *reason, bool is_local)
{
	const char	*r;
	ban_t		*pb;

	if ( find(ip,mask) >=0 )
	{
		Printf("Duplicate ban\n");
		return false;
	}

	if (nbans >= MAX_BANS)
	{
		Printf("Too many bans\n");
		return false;
	}

	r = (!reason || *reason=='\0') ? " " : reason;

	if ( (pb = (ban_t *) malloc(sizeof(ban_t)+strlen(r))) == NULL )
	{
		Printf("Out of memory\n");
		return false;
	}
	memcpy(pb->ip,ip,4);
	memcpy(pb->mask,mask,4);
	strcpy(pb->reason,r);
	pb->is_local = is_local;
	bans[nbans++] = pb;

	Printf("%s added to banlist\n", ip_to_str(ip,mask) );
	return true;
}

/*
 * Remove a ban from the ban list.
 */
bool ban_cl::remove(const byte *ip, const byte *mask)
{
	int		i;

	if ( (i=find(ip,mask)) < 0 ) return false;
	free(bans[i]);
	if (i<nbans-1)	bans[i] = bans[nbans-1];
	bans[nbans-1] = NULL;
	nbans--;
	return true;
}

/*
 * Determine if the client's address matches the IP address and mask of a single ban.
 */
bool ban_cl::address_matches(const byte *claddr,const byte *banip, const byte *mask)
{
	int i;

	for (i=0; i<4; i++)
	{
		if (mask[i]) continue;
		if (claddr[i]!=banip[i]) return false;
	}
	return true;
}

/*
 * Enforce all bans to a single player; if any ban matches, kick him out.
 * Returns true if a match was found, and false otherwise.
 */
bool ban_cl::enforce_to_player(int cl)
{
	int				i;
	byte			*client_ip;
	char			string[1024];
	static byte		mask[4];		// the static attribute initializes it to zero

	if (ta[cl].exist <= 0 ||
		ta[cl].skill != TANKSKILL_HUMAN)
		return false; 

	client_ip = ta[cl].address.ip;

	for (i=0; i<nbans; i++)
	{
		if (address_matches(client_ip,bans[i]->ip,bans[i]->mask))
		{

			sprintf(string, "BANNED: %s", bans[i]->reason);

			Printf("%s banned (%s)\n", 
					(ta[cl].name[0]) ? ta[cl].name : ip_to_str(client_ip,mask),
					bans[i]->reason );

			// Remove from the game
			ta[cl].outbuf->WriteByte(STOC_BANPLAYER);
			ta[cl].outbuf->WriteString(bans[i]->reason);

			ta[cl].Disconnect();

			return true;
		}
	}

	return false;
}

/*
 * Enforce all bans to all players
 */
void ban_cl::enforce_to_all(void)
{
	int i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 ||
			ta[i].skill != TANKSKILL_HUMAN)
			continue;

		enforce_to_player(i);
	}
}

/*
 * The ban list: make it a static variable so that it's not
 * visible from outside this file, and also so that its constructor
 * and destructor are called automatically.
 */
static ban_cl ban_list;

static void
readbans_aux(const char *fname, bool is_local)
{
	FILE	*f;
	int		nadded;
	byte	ip[4],mask[4];
	char	*reason, *pbuf, buf[1024];
	
	Printf("Reading bans from %s...",fname);
	if ( (f=fopen(fname, "r")) == NULL )
	{
		Printf("File not found\n");
		return;
	}
	Printf("\n");
	nadded = 0;
	while (fgets(buf, sizeof(buf)-1, f))
	{
		if ( (pbuf=strchr(buf,'\n')) != NULL )		// remove trailing LF
			*pbuf = '\0';

		if ( (pbuf=strchr(buf,'\r')) != NULL )		// remove trailing CR
			*pbuf = '\0';

		if ( (pbuf=strchr(buf,';')) != NULL )		// remove comments
			*pbuf = '\0';

		pbuf = buf;
		while (*pbuf==' ' || *pbuf=='\t')			// remove whitespace
			pbuf++;
		if (*pbuf == '\0')  continue;				// empty line

		reason = strchr(pbuf,'#');
		if (reason)	*reason++ = '\0';
		else reason = " ";

		if (!ban_list.str_to_ip(pbuf,ip,mask))
		{
			Printf("readbans_aux: Bad IP address (%s)\n",pbuf);
			continue;
		}
		if (ban_list.add(ip,mask,reason,is_local))
			nadded++;
	}
	fclose(f);
	Printf("Added %d bans\n",nadded);
}

/*
 * SV_ReadBans - Reads & parses the ban list into memory
 */
void SV_ReadBans(void)
{
	readbans_aux(LOCAL_BANFILE,true);
}

/*
 * Kick out a player
 */
void SV_KickPlayer(int pnum, char *reason)
{
	char	str[512];

	if (ta[pnum].exist <= 0 || pnum < 0 || pnum > MAX_TANKS)
	{
		Printf("Player does not exist.\n");
		return;
	}

	// Make sure there is some kind of reason filled in
	if (ustrlen(reason) <= 0)
		sprintf(reason, "...");

	if (ta[pnum].skill != TANKSKILL_HUMAN)
	{
		sprintf(str, "Bot '%s' has been removed from the game. (%s)", ta[pnum].name, reason);
		BCast_Printf(str);
		Printf("\n");

		ta[pnum].Disconnect();
		return;
	}

	ta[pnum].outbuf->WriteByte(STOC_KICKPLAYER);
	ta[pnum].outbuf->WriteString(reason);

	ta[pnum].Disconnect();

	sprintf(str, "%s was kicked from the game (%s)", ta[pnum].name, reason);

	BCast_Printf(str);
	Printf("\n");

	
}


void	Cmd_ListPlayers_f(void)
{
	int		i;

	Printf("\n========= PLAYER LIST =========\n\n");

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		Printf("%d    %s", i, ta[i].name);

		if (ta[i].skill != TANKSKILL_HUMAN)
			Printf("(BOT)");
		else
		{
			if (multiplayer && multiplayermode == MP_SERVER)
			{ Printf(" (%s)", net_adrtostring(ta[i].address)); }
		}

		Printf("\n");
	}

	Printf("\n");
}


/*
 * COMMAND - kick Sends a simple message to the client to disconnect
 */
void	Cmd_KickPlayer_f(void)
{
	int		pnum;
	//char	string[1024];
	//int		i;

	if (Cmd_Argc () < 2)
	{
		USAGE:
		Printf ("\nKICK <player num> <reason>\n");
		Printf ("    Kicks a player off the server with a kick message\n");
		return;
	}

	if (sscanf(Cmd_Argv(1),"%d",&pnum)!=1)	goto USAGE;
	
	if (ta[pnum].exist <= 0 || pnum<0 || pnum>MAX_TANKS)
	{
		Printf("\n ** Player %d not found!\n", pnum);
		return;
	}

	// Can't kick human players in single player mode
	if (!multiplayer && ta[pnum].skill == TANKSKILL_HUMAN)
		return;

	SV_KickPlayer(pnum, collect_args());
}

/*
 * COMMAND - banlist Displays the current list of IP bans
 */
void	Cmd_BanList_f(void)
{
	ban_list.show();
}

/*
 * COMMAND - addban Adds an ip address to the ban list
 */
void	Cmd_Addban_f(void)
{
	byte ip[4],mask[4];

	if (Cmd_Argc() < 3)
	{
		USAGE:
		Printf ("\nADDBAN <ip> <reason>\n");
		Printf ("     Bans an IP address from this server.\n\n");
		return;
	}
	if (!ban_list.str_to_ip(Cmd_Argv(1),ip,mask))
		goto USAGE;
	
	
	if (ban_list.add(ip,mask,collect_args(),true))
	{
		ban_list.enforce_to_all();
		ban_list.save(LOCAL_BANFILE);
	}
}

/*
 * COMMAND - killban Removes an ip address from the ban list
 */
void	Cmd_Killban_f(void)
{
	byte ip[4],mask[4];

	if (Cmd_Argc() < 2)
	{
		USAGE:
		Printf("\nKILLBAN <ip>\n");
		Printf("      Removes a banned IP from the ban list.\n\n");
		return;
	}
	if (!ban_list.str_to_ip(Cmd_Argv(1),ip,mask))
		goto USAGE;
	if (ban_list.remove(ip,mask))
	{
		Printf("%s unbanned.\n", ban_list.ip_to_str(ip,mask));
		ban_list.save(LOCAL_BANFILE);
	}
	else
	{
		Printf("unable to locate ban\n");
	}
}

/*
 * Enforce all bans to a specific player. Returns 1 if we banned him
 * and 0 otherwise.
 */
int SV_EnforceBans(int cl)
{
	return (ban_list.enforce_to_player(cl)) ? 1 : 0;
}

void SV_ReReadBans(void)
{
	ban_list.flush();
	SV_ReadBans();
	ban_list.enforce_to_all();
}
