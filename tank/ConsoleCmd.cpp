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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "scorch.h"



int		com_argc;
char	*com_argv[MAX_NUM_ARGVS+1];

int		alias_count;		// for detecting runaway loops

typedef struct cmdalias_s
{
	char	name[MAX_ALIAS_NAME];
	char	*value;
	struct	cmdalias_s	*next;
} cmdalias_t, cmdalias_ptr;

cmdalias_t	*cmd_alias;

//===========================================================================

void SZ_Init (sizebuf_t *buf, byte *data, int length)
{
	memset (buf, 0, sizeof(*buf));
	buf->data = data;
	buf->maxsize = length;
}

void SZ_Clear (sizebuf_t *buf)
{
	buf->cursize = 0;
	buf->overflowed = false;
}

void *SZ_GetSpace (sizebuf_t *buf, int length)
{
	void	*data;
	
	if (buf->cursize + length > buf->maxsize)
	{
		if (!buf->allowoverflow)
			Printf("SZ_GetSpace: overflow without allowoverflow set");
		
		if (length > buf->maxsize)
			Printf("SZ_GetSpace: %i is > full buffer size", length);
			
		Printf("SZ_GetSpace: overflow\n");
		SZ_Clear (buf); 
		buf->overflowed = true;
	}

	data = buf->data + buf->cursize;
	buf->cursize += length;
	
	return data;
}

void SZ_Write (sizebuf_t *buf, void *data, int length)
{
	memcpy (SZ_GetSpace(buf,length),data,length);		
}


/*
==============
COM_Parse

Parse a token out of a string
==============
*/
char	com_token[MAX_TOKEN_CHARS];
char *COM_Parse (char **data_p)
{
	int		c;
	int		len;
	char	*data;

	data = *data_p;
	len = 0;
	com_token[0] = 0;
	
	if (!data)
	{
		*data_p = NULL;
		return "";
	}
		
// skip whitespace
skipwhite:
	while ( (c = *data) <= ' ')
	{
		if (c == 0)
		{
			*data_p = NULL;
			return "";
		}
		data++;
	}
	
// skip // comments
	if (c=='/' && data[1] == '/')
	{
		while (*data && *data != '\n')
			data++;
		goto skipwhite;
	}

// handle quoted strings specially
	if (c == '\"')
	{
		data++;
		while (1)
		{
			c = *data++;
			if (c=='\"' || !c)
			{
				com_token[len] = 0;
				*data_p = data;
				return com_token;
			}
			if (len < MAX_TOKEN_CHARS)
			{
				com_token[len] = c;
				len++;
			}
		}
	}

// parse a regular word
	do
	{
		if (len < MAX_TOKEN_CHARS)
		{
			com_token[len] = c;
			len++;
		}
		data++;
		c = *data;
	} while (c>32);

	if (len == MAX_TOKEN_CHARS)
	{
//		Com_Printf ("Token exceeded %i chars, discarded.\n", MAX_TOKEN_CHARS);
		len = 0;
	}
	com_token[len] = 0;

	*data_p = data;
	return com_token;
}


void Com_sprintf (char *dest, int size, char *fmt, ...)
{
	int		len;
	va_list		argptr;
	char	bigbuffer[0x10000];

	va_start (argptr,fmt);
	len = vsprintf (bigbuffer,fmt,argptr);
	va_end (argptr);
	if (len >= size)
		Printf("Com_sprintf: overflow of %i in %i\n", len, size);
	strncpy (dest, bigbuffer, size-1);
}



////////////////////////////////////////
////////////////////////////////////////

int Q_stricmp (char *s1, char *s2)
{
#if defined(WIN32)
	return _stricmp (s1, s2);
#else
	return strcasecmp (s1, s2);
#endif
}


int Q_strncasecmp (char *s1, char *s2, int n)
{
	int		c1, c2;
	
	do
	{
		c1 = *s1++;
		c2 = *s2++;

		if (!n--)
			return 0;		// strings are equal until end point
		
		if (c1 != c2)
		{
			if (c1 >= 'a' && c1 <= 'z')
				c1 -= ('a' - 'A');
			if (c2 >= 'a' && c2 <= 'z')
				c2 -= ('a' - 'A');
			if (c1 != c2)
				return -1;		// strings not equal
		}
	} while (c1);
	
	return 0;		// strings are equal
}

int Q_strcasecmp (char *s1, char *s2)
{
	return Q_strncasecmp (s1, s2, 99999);
}


/*
============
va

does a varargs printf into a temp buffer, so I don't need to have
varargs versions of all text functions.
FIXME: make this buffer size safe someday
============
*/
char	*va(char *format, ...)
{
	va_list		argptr;
	static char		string[1024];
	
	va_start (argptr, format);
	vsprintf (string, format,argptr);
	va_end (argptr);

	return string;	
}

////////////////////////////////////////
////////////////////////////////////////




/*
================
COM_CheckParm

Returns the position (1 to argc-1) in the program's argument list
where the given parameter apears, or 0 if not present
================
*/
int COM_CheckParm (char *parm)
{
	int		i;
	
	for (i=1 ; i<com_argc ; i++)
	{
		if (!strcmp (parm,com_argv[i]))
			return i;
	}
		
	return 0;
}

int COM_Argc (void)
{
	return com_argc;
}

char *COM_Argv (int arg)
{
	if (arg < 0 || arg >= com_argc || !com_argv[arg])
		return "";
	return com_argv[arg];
}

void COM_ClearArgv (int arg)
{
	if (arg < 0 || arg >= com_argc || !com_argv[arg])
		return;
	com_argv[arg] = "";
}


/*
================
COM_InitArgv
================
*/
void COM_InitArgv (int argc, char **argv)
{
	int		i;

	if (argc > MAX_NUM_ARGVS)
		Printf("argc > MAX_NUM_ARGVS");
	com_argc = argc;
	for (i=0 ; i<argc ; i++)
	{
		if (!argv[i] || strlen(argv[i]) >= MAX_TOKEN_CHARS )
			com_argv[i] = "";
		else
			com_argv[i] = argv[i];
	}
}

/*
================
COM_AddParm

Adds the given string at the end of the current argument list
================
*/
void COM_AddParm (char *parm)
{
	if (com_argc == MAX_NUM_ARGVS)
		Printf("COM_AddParm: MAX_NUM)ARGS");
	com_argv[com_argc++] = parm;
}




/// just for debugging
int	memsearch (byte *start, int count, int search)
{
	int		i;
	
	for (i=0 ; i<count ; i++)
		if (start[i] == search)
			return i;
	return -1;
}


char *CopyString (char *in)
{
	char	*out;
	int		size = strlen(in)+1;
	
	out = (char *)malloc (size);
	
	strcpy (out, in);
	return out;
}










//=============================================================================

/*
============
Cmd_Wait_f

Causes execution of the remainder of the command buffer to be delayed until
next frame.  This allows commands like:
bind g "impulse 5 ; +attack ; wait ; -attack ; impulse 2"
============
*/
void Cmd_Wait_f (void)
{
	//cmd_wait = true;
	Printf("This does nothing");
}


/*
=============================================================================

						COMMAND BUFFER

=============================================================================
*/

sizebuf_t	cmd_text;
byte		cmd_text_buf[8192];

byte		defer_text_buf[8192];

/*
============
Cbuf_Init
============
*/
void Cbuf_Init (void)
{
	SZ_Init (&cmd_text, cmd_text_buf, sizeof(cmd_text_buf));
}

/*
============
Cbuf_AddText

Adds command text at the end of the buffer
============
*/
void Cbuf_AddText (char *text)
{
	int		l;
	
	l = strlen (text);

	if (cmd_text.cursize + l >= cmd_text.maxsize)
	{
		Printf ("Cbuf_AddText: overflow\n");
		return;
	}
	SZ_Write (&cmd_text, text, strlen (text));
}


/*
============
Cbuf_InsertText

Adds command text immediately after the current command
Adds a \n to the text
FIXME: actually change the command buffer to do less copying
============
*/
void Cbuf_InsertText (char *text)
{
	char	*temp;
	int		templen;

// copy off any commands still remaining in the exec buffer
	templen = cmd_text.cursize;
	if (templen)
	{
		temp = (char *)malloc (templen);
		memcpy (temp, cmd_text.data, templen);
		SZ_Clear (&cmd_text);
	}
	else
		temp = NULL;	// shut up compiler
		
	// add the entire text of the file
	Cbuf_AddText (text);
	
	// add the copied off data
	if (templen)
	{
		SZ_Write (&cmd_text, temp, templen);
//		free (temp);
		free(temp);
	}
}


/*
============
Cbuf_CopyToDefer
============
*/
void Cbuf_CopyToDefer (void)
{
	memcpy(defer_text_buf, cmd_text_buf, cmd_text.cursize);
	defer_text_buf[cmd_text.cursize] = 0;
	cmd_text.cursize = 0;
}

/*
============
Cbuf_InsertFromDefer
============
*/
void Cbuf_InsertFromDefer (void)
{
	Cbuf_InsertText((char *)defer_text_buf);
	defer_text_buf[0] = 0;
}


/*
============
Cbuf_ExecuteText
============
*/
void Cbuf_ExecuteText (int exec_when, char *text)
{
	switch (exec_when)
	{
	case EXEC_NOW:
		Cmd_ExecuteString (text);
		break;
	case EXEC_INSERT:
		Cbuf_InsertText (text);
		break;
	case EXEC_APPEND:
		Cbuf_AddText (text);
		break;
	default:
		Printf("Cbuf_ExecuteText: bad exec_when");
	}
}

/*
============
Cbuf_Execute
============
*/
void Cbuf_Execute (void)
{
	int		i;
	char	*text;
	char	line[1024];
	int		quotes;

	alias_count = 0;		// don't allow infinite alias loops

	while (cmd_text.cursize)
	{
		// find a \n or ; line break
		text = (char *)cmd_text.data;

		quotes = 0;
		for (i=0 ; i< cmd_text.cursize ; i++)
		{
			if (text[i] == '"')
				quotes++;
			if ( !(quotes&1) &&  text[i] == ';')
				break;	// don't break if inside a quoted string
			if (text[i] == '\n')
				break;
		}
			
				
		if (i > 1023)		//** DMP buffer overrun protection
			i = 1023;
		memcpy (line, text, i);
		line[i] = 0;
		
		// delete the text from the command buffer and move remaining commands down
		// this is necessary because commands (exec, alias) can insert data at the
		// beginning of the text buffer

		if (i == cmd_text.cursize)
			cmd_text.cursize = 0;
		else
		{
			i++;
			cmd_text.cursize -= i;
			memmove (text, text+i, cmd_text.cursize);
		}

		// execute the command line
		Cmd_ExecuteString (line);
		
		/*if (cmd_wait)
		{
			// skip out while text still remains in buffer, leaving it
			// for next frame
			cmd_wait = false;
			break;
		}*/
	}
}


/*
===============
Cbuf_AddEarlyCommands

Adds command line parameters as script statements
Commands lead with a +, and continue until another +

Set commands are added early, so they are guaranteed to be set before
the client and server initialize for the first time.

Other commands are added late, after all initialization is complete.
===============
*/
void Cbuf_AddEarlyCommands (bool clear)
{
	int		i;
	int		j;
	const	char	*s;

	DArgs	*Cmds;
	
	for (i=0; i<Args.NumArgs(); i++)
	{
		s = CopyString(Args.m_ArgV[i]); //Args.GetArg(i);

		if (!strcmp(s, "+set"))
		{
			Cmds = Args.GetArgParams(s, i);
			
			if (Cmds->NumArgs() > 0)
			{
				for (j=0; j<Cmds->NumArgs(); j+=2)
				{
		
					Cbuf_AddText(va("set %s \"%s\"\n", Cmds->GetArg(j), Cmds->GetArg(j+1)));
					//Printf("set %s \"%s\"\n", Cmds->GetArg(j), Cmds->GetArg(j+1));
				}
			}

			delete Cmds;
		}

		else if (!strcmp(s, "+exec"))
		{
			Cmds = Args.GetArgParams(s, i);
			
			if (Cmds->NumArgs() > 0)
			{
				for (j=0; j<Cmds->NumArgs(); j+=1)
				{
					Cbuf_AddText(va("exec %s\n", Cmds->GetArg(j)));
					//Printf("exec %s\n", Cmds->GetArg(j));
				}
			}

			delete Cmds;
			
		}
	}
}

/*
=================
Cbuf_AddLateCommands

Adds command line parameters as script statements
Commands lead with a + and continue until another + or -
quake +vid_ref gl +map amlev1

Returns true if any late commands were added, which
will keep the demoloop from immediately starting
=================
*/
bool Cbuf_AddLateCommands (void)
{
	int		i, j;
	int		s;
	char	*text, *build, c;
	int		argc;
	bool	ret;

// build the combined string to parse from
	s = 0;
	argc = COM_Argc();
	for (i=1 ; i<argc ; i++)
	{
		s += strlen (COM_Argv(i)) + 1;
	}
	if (!s)
		return false;
		
	text = (char *)malloc (s+1);
	text[0] = 0;
	for (i=1 ; i<argc ; i++)
	{
		strcat (text,COM_Argv(i));
		if (i != argc-1)
			strcat (text, " ");
	}
	
// pull out the commands
	build = (char *)malloc (s+1);
	build[0] = 0;
	
	for (i=0 ; i<s-1 ; i++)
	{
		if (text[i] == '+')
		{
			i++;

			for (j=i ; (text[j] != '+') && (text[j] != '-') && (text[j] != 0) ; j++)
				;

			c = text[j];
			text[j] = 0;
			
			strcat (build, text+i);
			strcat (build, "\n");
			text[j] = c;
			i = j-1;
		}
	}

	ret = (build[0] != 0);
	if (ret)
		Cbuf_AddText (build);
	
	free (text);
	free (build);

	return ret;
}


/*
==============================================================================

						SCRIPT COMMANDS

==============================================================================
*/

/*
================
FS_filelength
================
*/
int FS_filelength (FILE *f)
{
	int		pos;
	int		end;

	pos = ftell (f);
	fseek (f, 0, SEEK_END);
	end = ftell (f);
	fseek (f, pos, SEEK_SET);

	return end;
}

int FS_FOpenFile (char *filename, FILE **file)
{
	*file = fopen (filename, "rb");
	if (!*file)
	{
		*file = NULL;
		return 0;
	}
		
	//Com_DPrintf ("FindFile: %s\n",netpath);

	return FS_filelength (*file);
}


#define	MAX_READ	0x10000		// read in blocks of 64k
void FS_Read (void *buffer, int len, FILE *f)
{
	int		block, remaining;
	int		read;
	byte	*buf;
	int		tries;

	buf = (byte *)buffer;

	// read in chunks for progress bar
	remaining = len;
	tries = 0;
	while (remaining)
	{
		block = remaining;
		if (block > MAX_READ)
			block = MAX_READ;
		read = fread (buf, 1, block, f);
		if (read == 0)
		{
			// we might have been trying to read from a CD
			if (!tries)
			{
				tries = 1;
				//CDAudio_Stop();
			}
			else
				Printf("FS_Read: 0 bytes read");
		}

		if (read == -1)
			Printf("FS_Read: -1 bytes read");

		// do some progress bar thing here...

		remaining -= read;
		buf += read;
	}
}

int FS_LoadFile (char *path, void **buffer)
{
	FILE	*h;
	byte	*buf;
	int		len;

	buf = NULL;	// quiet compiler warning

// look for it in the filesystem or pack files
	len = FS_FOpenFile (path, &h);
	if (!h)
	{
		if (buffer)
			*buffer = NULL;
		return -1;
	}
	
	if (!buffer)
	{
		fclose (h);
		return len;
	}

	buf = (byte *)malloc(len);
	*buffer = buf;

	FS_Read (buf, len, h);

	fclose (h);

	return len;
}


/*
===============
Cmd_Exec_f
===============
*/
void Cmd_Exec_f (void)
{
	char	*f, *f2;
	int		len;

	if (Cmd_Argc() != 2)
	{
		Printf("exec <filename> : execute a script file\n");
		return;
	}

	len = FS_LoadFile (Cmd_Argv(1), (void **)&f);
	if (!f)
	{
		Printf("couldn't exec %s\n",Cmd_Argv(1));
		return;
	}
	Printf("execing %s\n",Cmd_Argv(1));
	
	// the file doesn't have a trailing 0, so we need to copy it off
	f2 = (char *)malloc(len+1);
	memcpy (f2, f, len);
	f2[len] = 0;

	Cbuf_InsertText (f2);
	//Printf(f2);

	free (f2);
	free (f);
}


/*
===============
Cmd_Echo_f

Just prints the rest of the line to the console
===============
*/
void Cmd_Echo_f (void)
{
	int		i;
	
	for (i=1 ; i<Cmd_Argc() ; i++)
		Printf ("%s ",Cmd_Argv(i));
	Printf ("\n");
}

/*
===============
Cmd_Alias_f

Creates a new command that executes a command string (possibly ; seperated)
===============
*/
void Cmd_Alias_f (void)
{
	cmdalias_t	*a;
	char		cmd[1024];
	int			i, c;
	char		*s;

	if (Cmd_Argc() == 1)
	{
		Printf ("Current alias commands:\n");
		for (a = cmd_alias ; a ; a=a->next)
			Printf ("%s : %s\n", a->name, a->value);
		return;
	}

	s = Cmd_Argv(1);
	if (strlen(s) >= MAX_ALIAS_NAME)
	{
		Printf ("Alias name is too long\n");
		return;
	}

	// if the alias already exists, reuse it
	for (a = cmd_alias ; a ; a=a->next)
	{
		if (!strcmp(s, a->name))
		{
			free (a->value);
			break;
		}
	}

	if (!a)
	{
		a = (cmdalias_t *)malloc (sizeof(cmdalias_t));
		a->next = cmd_alias;
		cmd_alias = a;
	}
	strcpy (a->name, s);	

// copy the rest of the command line
	cmd[0] = 0;		// start out with a null string
	c = Cmd_Argc();
	for (i=2 ; i< c ; i++)
	{
		strcat (cmd, Cmd_Argv(i));
		if (i != (c - 1))
			strcat (cmd, " ");
	}
	strcat (cmd, "\n");
	
	a->value = CopyString (cmd);
}

/*
=============================================================================

					COMMAND EXECUTION

=============================================================================
*/

typedef struct cmd_function_s
{
	struct cmd_function_s	*next;
	char					*name;
	xcommand_t				function;
} cmd_function_t, *cmd_function_ptr;


static	int			cmd_argc;
static	char		*cmd_argv[MAX_STRING_TOKENS];
static	char		*cmd_null_string = "";
static	char		cmd_args[MAX_STRING_CHARS];

static	cmd_function_t	*cmd_functions;		// possible commands to execute

/*
============
Cmd_Argc
============
*/
int		Cmd_Argc (void)
{
	return cmd_argc;
}

/*
============
Cmd_Argv
============
*/
char	*Cmd_Argv (int arg)
{
	if (arg >= cmd_argc )
		return cmd_null_string;
	return cmd_argv[arg];	
}

/*
============
Cmd_Args

Returns a single string containing argv(1) to argv(argc()-1)
============
*/
char		*Cmd_Args (void)
{
	return cmd_args;
}


/*
======================
Cmd_MacroExpandString
======================
*/

char *Cmd_MacroExpandString (char *text)
{
	int		i, j, count, len;
	bool	inquote;
	char	*scan;
	static	char	expanded[MAX_STRING_CHARS];
	char	temporary[MAX_STRING_CHARS];
	char	*token, *start;

	inquote = false;
	scan = text;

	len = strlen (scan);
	if (len >= MAX_STRING_CHARS)
	{
		Printf ("Line exceeded %i chars, discarded.\n", MAX_STRING_CHARS);
		return NULL;
	}

	count = 0;

	for (i=0 ; i<len ; i++)
	{
		if (scan[i] == '"')
			inquote ^= 1;
		if (inquote)
			continue;	// don't expand inside quotes
		if (scan[i] != '$')
			continue;
		// scan out the complete macro
		start = scan+i+1;
		token = COM_Parse (&start);
		if (!start)
			continue;
	
		token = Cvar_VariableString (token);

		j = strlen(token);
		len += j;
		if (len >= MAX_STRING_CHARS)
		{
			Printf ("Expanded line exceeded %i chars, discarded.\n", MAX_STRING_CHARS);
			return NULL;
		}

		strncpy (temporary, scan, i);
		strcpy (temporary+i, token);
		strcpy (temporary+i+j, start);

		strcpy (expanded, temporary);
		scan = expanded;
		i--;

		if (++count == 100)
		{
			Printf ("Macro expansion loop, discarded.\n");
			return NULL;
		}
	}

	if (inquote)
	{
		Printf ("Line has unmatched quote, discarded.\n");
		return NULL;
	}

	return scan;
}


/*
============
Cmd_TokenizeString

Parses the given string into command line tokens.
$Cvars will be expanded unless they are in a quoted token
============
*/
void Cmd_TokenizeString (char *text, bool macroExpand)
{
	int		i;
	char	*com_token;

// clear the args from the last string
	for (i=0 ; i<cmd_argc ; i++)
		free (cmd_argv[i]);
		
	cmd_argc = 0;
	cmd_args[0] = 0;
	
	// macro expand the text
	if (macroExpand)
		text = Cmd_MacroExpandString (text);
	if (!text)
		return;

	while (1)
	{
// skip whitespace up to a /n
		while (*text && *text <= ' ' && *text != '\n')
		{
			text++;
		}
		
		if (*text == '\n')
		{	// a newline seperates commands in the buffer
			text++;
			break;
		}

		if (!*text)
			return;

		// set cmd_args to everything after the first arg
		if (cmd_argc == 1)
		{
			int		l;

			strcpy (cmd_args, text);

			// strip off any trailing whitespace
			l = strlen(cmd_args) - 1;
			for ( ; l >= 0 ; l--)
				if (cmd_args[l] <= ' ')
					cmd_args[l] = 0;
				else
					break;
		}
			
		com_token = COM_Parse (&text);
		if (!text)
			return;

		if (cmd_argc < MAX_STRING_TOKENS)
		{
			cmd_argv[cmd_argc] = (char *)malloc (strlen(com_token)+1);
			strcpy (cmd_argv[cmd_argc], com_token);
			cmd_argc++;
		}
	}
	
}


/*
============
Cmd_AddCommand
============
*/
void	Cmd_AddCommand (char *cmd_name, xcommand_t function)
{
	cmd_function_t	*cmd;
	
// fail if the command is a variable name
	if (Cvar_VariableString(cmd_name)[0])
	{
		Printf ("Cmd_AddCommand: %s already defined as a var\n", cmd_name);
		return;
	}
	
// fail if the command already exists
	for (cmd=cmd_functions ; cmd ; cmd=cmd->next)
	{
		if (!strcmp (cmd_name, cmd->name))
		{
			Printf ("Cmd_AddCommand: %s already defined\n", cmd_name);
			return;
		}
	}

	cmd = (cmd_function_ptr)malloc (sizeof(cmd_function_t));
	cmd->name = cmd_name;
	cmd->function = function;
	cmd->next = cmd_functions;
	cmd_functions = cmd;
}

/*
============
Cmd_RemoveCommand
============
*/
void	Cmd_RemoveCommand (char *cmd_name)
{
	cmd_function_t	*cmd, **back;

	back = &cmd_functions;
	while (1)
	{
		cmd = *back;
		if (!cmd)
		{
			Printf ("Cmd_RemoveCommand: %s not added\n", cmd_name);
			return;
		}
		if (!strcmp (cmd_name, cmd->name))
		{
			*back = cmd->next;
			free (cmd);
			return;
		}
		back = &cmd->next;
	}
}

/*
============
Cmd_Exists
============
*/
bool	Cmd_Exists (char *cmd_name)
{
	cmd_function_t	*cmd;

	for (cmd=cmd_functions ; cmd ; cmd=cmd->next)
	{
		if (!strcmp (cmd_name,cmd->name))
			return true;
	}

	return false;
}



/*
============
Cmd_CompleteCommand
============
*/
char *Cmd_CompleteCommand (char *partial)
{
	cmd_function_t	*cmd;
	int				len;
	cmdalias_t		*a;
	
	len = strlen(partial);
	
	if (!len)
		return NULL;
		
// check for exact match
	for (cmd=cmd_functions ; cmd ; cmd=cmd->next)
		if (!strcmp (partial,cmd->name))
			return cmd->name;
	for (a=cmd_alias ; a ; a=a->next)
		if (!strcmp (partial, a->name))
			return a->name;

// check for partial match
	for (cmd=cmd_functions ; cmd ; cmd=cmd->next)
		if (!strncmp (partial,cmd->name, len))
			return cmd->name;
	for (a=cmd_alias ; a ; a=a->next)
		if (!strncmp (partial, a->name, len))
			return a->name;

	return NULL;
}


/*
============
Cmd_ExecuteString

A complete command line has been parsed, so try to execute it
FIXME: lookupnoadd the token to speed search?
============
*/
void	Cmd_ExecuteString (char *text)
{	
	cmd_function_t	*cmd;
	cmdalias_t		*a;

	Cmd_TokenizeString (text, true);
			
	// execute the command line
	if (!Cmd_Argc())
		return;		// no tokens

	// check functions
	for (cmd=cmd_functions ; cmd ; cmd=cmd->next)
	{
		if (!Q_strcasecmp (cmd_argv[0],cmd->name))
		{
			if (!cmd->function)
			{	// forward to server command
				Cmd_ExecuteString (va("cmd %s", text));
			}
			else
				cmd->function ();
			return;
		}
	}

	// check alias
	for (a=cmd_alias ; a ; a=a->next)
	{
		if (!Q_strcasecmp (cmd_argv[0], a->name))
		{
			if (++alias_count == ALIAS_LOOP_COUNT)
			{
				Printf ("ALIAS_LOOP_COUNT\n");
				return;
			}
			Cbuf_InsertText (a->value);
			return;
		}
	}
	
	// check cvars
	if (Cvar_Command ())
		return;

	// send it as a server command if we are connected
	//Cmd_ForwardToServer ();
}

/*
============
Cmd_List_f
============
*/
void Cmd_List_f (void)
{
	cmd_function_t	*cmd;
	int				i;

	i = 0;
	for (cmd=cmd_functions ; cmd ; cmd=cmd->next, i++)
		Printf ("%s\n", cmd->name);
	Printf ("%i commands\n", i);
}

//
// Cmd_Quit - Quits the game, ...obviously
//

void	Cmd_Quit_f(void)
{
	quitgame();
}

/*
============
Cmd_Init
============
*/
void Cmd_Init (void)
{
//
// register our commands
//
	Cmd_AddCommand ("cmdlist",Cmd_List_f);
	Cmd_AddCommand ("exec",Cmd_Exec_f);
	Cmd_AddCommand ("echo",Cmd_Echo_f);
	Cmd_AddCommand ("alias",Cmd_Alias_f);
	Cmd_AddCommand ("wait", Cmd_Wait_f);
	Cmd_AddCommand ("quit", Cmd_Quit_f);
	Cmd_AddCommand ("endlevel", Cmd_EndLevel_f);
	Cmd_AddCommand ("addbot", Cmd_AddBot_f);
	Cmd_AddCommand ("say", Cmd_ConsoleChat_f);
	Cmd_AddCommand ("resetlevel", Cmd_ResetLevel_f);
	Cmd_AddCommand ("listlevels", Cmd_ListLevels_f);
	Cmd_AddCommand ("warp", Cmd_LevelWarp_f);
}

