/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// cmd.c -- Quake script command processing module

#define	MAX_ALIAS_NAME	32

typedef void (*xcommand_t) (void);

////////////
////////////

#define	EXEC_NOW	0		// don't return until completed
#define	EXEC_INSERT	1		// insert at current position, but don't run yet
#define	EXEC_APPEND	2		// add to end of the command buffer

////////////
////////////

#define	MAX_STRING_CHARS	1024	// max length of a string passed to Cmd_TokenizeString
#define	MAX_STRING_TOKENS	256		// max tokens resulting from Cmd_TokenizeString
#define	MAX_TOKEN_CHARS		256		// max length of an individual token

////////////
////////////

#define MAX_NUM_ARGVS	200

////////////
////////////

#define	ALIAS_LOOP_COUNT	16


//
// GENERIC CONSOLE COMMANDS
//
void	Cmd_EndLevel_f(void);
void	Cmd_AddBot_f(void);
void	Cmd_ConsoleChat_f (void);
void	Cmd_ResetLevel_f(void);
void	Cmd_ListLevels_f(void);
void	Cmd_LevelWarp_f(void);



//
// CONSOLE COMMANDS
//

void	Com_sprintf (char *dest, int size, char *fmt, ...);

int		Cmd_Argc (void);
char	*Cmd_Argv (int arg);
char	*Cmd_Args (void);
char	*Cmd_MacroExpandString (char *text);
void	Cmd_TokenizeString (char *text, bool macroExpand);
void	Cmd_AddCommand (char *cmd_name, xcommand_t function);
void	Cmd_RemoveCommand (char *cmd_name);
bool	Cmd_Exists (char *cmd_name);
char	*Cmd_CompleteCommand (char *partial);
void	Cmd_ExecuteString (char *text);
void	Cmd_List_f (void);
void	Cmd_Init (void);

void	Cbuf_AddEarlyCommands (bool clear);
void	Cbuf_Execute (void);
void	Cbuf_Init (void);
void	Cbuf_AddText (char *text);
void	Cbuf_ExecuteText (int exec_when, char *text);

void	Cvar_CheckForChange(void);
