#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "scorch.h"
#include "secrets.h"


// General console commands
void	Printf(char	*txt, ...)
{ 
	va_list		argptr;
	static char	msg[CONSOLE_PRINTF_MAXPRINTMSGSIZE];
	
	va_start (argptr, txt);
	vsprintf (msg, txt, argptr);
	va_end (argptr);
	
#ifndef	DEDICATED_SERVER
	if (gamestate != GS_LOADINGGAME)
		con->printf(msg); 
#else
	printf(msg);
#endif

	// Logging?
	//	if (Args.CheckParm("-logfile")) // check is now done in LogStr
	//	allows dynamic toggle of log
		LogStr(msg);
}


void	Cmd_EndLevel_f(void)
{
	if (gamestate != GS_INGAME)
		return;

	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	// Flag this as cheating
	used_cheat = 1;

	// Jump right into intermission. This doesn't award "end of round" money.
	start_timer();
	Inter_Run();
	InitDefaultTurnQue();
}


void	Cmd_AddBot_f(void)
{
	int		aitype;
	char	*botname;

	if (multiplayer && multiplayermode == MP_CLIENT)
		return;


	if (Cmd_Argc() < 2)
	{
		Printf("addbot: Instantly adds a bot into the game.\n");
		Printf("     usage: addbot <stoopid|lobber|shooter|gunner|sniper|random> <name>\n\n");
		return;
	}
	else
	{
		if (ustrcmp(ustrlwr(Cmd_Argv(1)), "stoopid")==0)
			aitype = TANKSKILL_STOOPID;
		else if (ustrcmp(ustrlwr(Cmd_Argv(1)), "lobber")==0)
			aitype = TANKSKILL_LOBBER;
		else if (ustrcmp(ustrlwr(Cmd_Argv(1)), "shooter")==0)
			aitype = TANKSKILL_SHOOTER;
		else if (ustrcmp(ustrlwr(Cmd_Argv(1)), "gunner")==0)
			aitype = TANKSKILL_GUNNER;
		else if (ustrcmp(ustrlwr(Cmd_Argv(1)), "sniper")==0)
			aitype = TANKSKILL_SNIPER;
		else if (ustrcmp(ustrlwr(Cmd_Argv(1)), "random")==0)
			aitype = TANKSKILL_STOOPID+rand()%5;
		else
		{
			Printf("Unknown bot type. Valid bot types are <stoopid|lobber|shooter|gunner|sniper|random>\n\n");
			return;
		}
			
	}

	if (Cmd_Argc() >= 3)
	{
		botname = CopyString(Cmd_Argv(2));
		AddBot_Realtime(aitype, botname);
	}
	else
		AddBot_Realtime(aitype, NULL);
}


void Cmd_ConsoleChat_f (void)
{
	char	*p;
	char	text[1024];

	if (Cmd_Argc() < 2)
	{
		Printf("say: Sends a chat message to all other clients\n");
		Printf("usage: say <message>\n");
		return;
	}

	if (multiplayer && multiplayermode == MP_SERVER)
		strcpy (text, "CONSOLE: ");

	p = Cmd_Args();

	// Remove quotation marks
	if (*p == '"')
	{
		p++;
		p[strlen(p)-1] = 0;
	}

	if (multiplayer && multiplayermode == MP_SERVER)
		strcat(text, p);
	
	if (multiplayer)
	{
		if (multiplayermode == MP_SERVER)
		{
			BCast_WriteByte(STOC_PRINTF);
			BCast_WriteString(text);
			Printf ("%s\n", text);
		}
		else
		{
			G_Printf(p, net_localplayer);

			localbuf->WriteByte(CTOS_CHATMESSAGE);
			localbuf->WriteString(p);
		}
	}
}

void	Cmd_ResetLevel_f(void)
{
	// Prevent this at all if this is a secret level
	if (currentlevel.secretcharflag >= 0)
		return;

	ResetLevel();

	// Flag this as cheating
	used_cheat = 1;
}


//define a command parser
void conalleg_c::interprete_command(char *cmdstr) 
{
	Cbuf_ExecuteText (EXEC_NOW, cmdstr);
}
