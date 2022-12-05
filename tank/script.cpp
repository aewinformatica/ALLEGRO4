/*
Code for script of Charred Dirt levels

		CDscript.cpp
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "scripts.h"

//	only set true if a script file is found when we start the game
//	see Scorch.cpp near line 600
bool	ScriptedGame = false;

static	int ScriptRounds;
static	int ScriptLine;
static	int ScriptReturnLine;
static	bool ScriptEnd;
static	char ScriptLineBuffer[256];
static	char ScriptLevel[64];
static	char ScriptMsg[256];
static	unsigned char ScriptAdd[256][3];
static	unsigned char ScriptOld[256][8];	//	holds the old weapon count
static	int ScriptAddN;
static	char	ScriptFileName[64];
static	FILE*	ScriptFile;
//	do these need to be static
//	should really be a class!!!!!
//===================================================================================
bool	OpenScriptFile(char*	SFN	) // ScriptFileName
{
int num;

if(!ScriptedGame)return false;	//	We are not doing scripts if ScriptedGame is false.

//	If ScriptEnd has been set then the last pass was the final in the script file
//	and we have gone around the loop so that a reset of all items has been done and
//	so we can close up shop now.
if(ScriptEnd)
	{
	Printf("*** Script is at the END!\n");
	ScriptedGame = false;
	ScriptFile = NULL;
	return false;
	}


if(SFN != NULL)	//	The initial request must have a file name
	{
	strncpy(ScriptFileName, SFN, 64);
	ScriptRounds = -1;
	Printf("*** Open Script File [%s]\n", ScriptFileName);
	}

if( (ScriptFileName == NULL) || (ScriptFileName[0] == 0) )
	{
	Printf("*** ScriptFileName is empty!\n");
	ScriptedGame = false;
	ScriptFile = NULL;
	return false;
	}
if(ScriptFile != NULL )fclose(ScriptFile);	//	should really rewind file if open
ScriptFile = fopen(ScriptFileName, "r");		//	but name may have changed if server is active.
if(ScriptFile == NULL )
	{
	Printf("*** Unable to find script file [%s]!\n", ScriptFileName);
	ScriptedGame = false;
	ScriptFile = NULL;
	return false;
	}

if(fgets(ScriptLineBuffer, 250, ScriptFile) == NULL)
	{
	Printf("*** Script file is empty [%s]!\n", ScriptFileName);
	ScriptedGame = false;
	fclose(ScriptFile);	ScriptFile = NULL;
	return false;
	}

num = -1;	//	set a value that should be changed.
sscanf(ScriptLineBuffer, "CDscript %d", &num);
if(num < 1)	//	above line failed or zero rounds
	{
	if(SFN != NULL)	//	print only on startup.
		Printf("*** Script file not valid!\nMust start with 'CDscript ' and the number_of_rounds (>0)\n");
	ScriptedGame = false;
	fclose(ScriptFile);	ScriptFile = NULL;
	return false;
	}
if(SFN != NULL)	//	set ScriptRounds only if its the first open (ie has a file name.)
	{
	ScriptRounds = num;
	if(ScriptRounds > 99)ScriptRounds = 99;
	ScriptReturnLine = 0;	//	zero is no call active
	}

ScriptEnd = false;

//	Scriptfile is now open
return true;
}

//===================================================================================
void FindScriptLine(int num)
	{
	int	i;

	//	sets scriptline to the line number requested.
	if(!OpenScriptFile())
		{
		ScriptedGame = false;
		return;
		}

	for(ScriptLine=0; ScriptLine<num; ScriptLine++)	//	skip to file position
		{
		fgets(ScriptLineBuffer, 250, ScriptFile);
		if(feof(ScriptFile) )
			{
			rewind(ScriptFile);		//	back to start.
			fgets(ScriptLineBuffer, 250, ScriptFile);	//	skip line 0
			ScriptLine = 1;
			break;
			}
		}
	return;
	}
//===================================================================================
void	SetScriptLevel(char* filename)
{
//	if the selected filename is NULL then there is no level directory or its empty
//	which means that its not much point trying to do a script!!

if(!ScriptedGame)return;	//	We are not doing scripts if ScriptedGame is false.

if(filename == NULL)
	{
	Printf("*** No levels found >>> Abandoning Script\n");
	ScriptedGame = false;
	return;
	}

ResetScriptCmds();	//	we may have data from previous level so clear it out.
if(ScriptEnd)				//	the script has ended so return to standard play.
	{
	Printf("*** Exit Script\n");
	ScriptedGame = false;
	return;
	}
GetScriptCmds();	//	Get the next set of script commands.

if(ScriptLevel[0] == 0)	//	no level name
	{
	Printf("*** No Level name found >>> Exit Script\n");
	ScriptedGame = false;
	return;
	}
/*
if(multiplayer && multiplayermode == MP_SERVER)
	{
	BCast_WriteByte(STOC_Script);
	Write_InventInfo();
	SV_SendPackets();
	}
*/
//	filename is of the form: levels\?????.cdl
sprintf(filename,"levels\\%s.cdl", ScriptLevel );
//sprintf(filename,"levels\\V1.0*%s.cdl", ScriptLevel );
return;
}

//===================================================================================
void GetScriptCmds(void)
{
int		i,j;
char	token[32];
char*	params;

if(!OpenScriptFile())
	{
	ScriptedGame = false;
	return;
	}

for(i=0; i<ScriptLine; i++)	//	skip to file position
	{
	fgets(ScriptLineBuffer, 250, ScriptFile);
	if(feof(ScriptFile) )
		{
		Printf("*** EOF on Script file at line %d\n", i);
		ScriptedGame = false;
		fclose(ScriptFile);	ScriptFile = NULL;
		return;
		}
	}

//	########################>>>>>> OK so lets start <<<<<<#############################
ScriptLevel[0] = 0;

while(!feof(ScriptFile) )
	{
	fgets(ScriptLineBuffer, 250, ScriptFile);
	ScriptLine++;
	j = strlen(ScriptLineBuffer)-1;
	if(j < 0)j=0;
	ScriptLineBuffer[j] = 0;	//	clear the return on the end of the string

	token[0]=0;	//	clear token of old data
	sscanf(ScriptLineBuffer, "%30s", token);	//	token not changed if ScriptLineBuffer empty
	j = strlen(token);
	if(j == 0) continue;	//	blank line
	params = &ScriptLineBuffer[j];

	j = i = 0;
	switch(toupper(token[0]))
		{
		case 'L' :	//	LEVEL
			if(ScriptLevel[0] != 0)	//	been here before so return
				{
				ScriptLine--;	//	backup and read this again next time around
				fclose(ScriptFile);	ScriptFile = NULL;
				return;
				}
			sscanf(params,"%60s", ScriptLevel);
			for(i=0;ScriptLevel[i];i++)
				{
				if(ScriptLevel[i] == '.')ScriptLevel[i] = ' ';
				}
			Printf("*** LEVEL[%s]\n", ScriptLevel);
			break;

		case 'A' :	//	ADD
			sscanf(params,"%30s %d", token,&j);
			i = GetCodeIndex(token);
			if(i < 0)
				{
				Printf("*** INVALID CODE[%s] ***\n", ScriptLineBuffer);
				}
			else
				{
				if(j>50) j = 50;	//	limit the number that can be added
				Printf("*** ADD[(%d) of %s]\n", j, GetPrizeName(i) );
				ScriptAdd[ScriptAddN][0] = i;	//	CodeIndex of the weapon
				ScriptAdd[ScriptAddN][1] = j;	//	number added
				ScriptAdd[ScriptAddN][2] = 'A';	//	ADD
				ScriptAddN++;
				}
			break;

		case 'S' :	//	SET
			sscanf(params,"%30s %d", token,&j);
			i = GetCodeIndex(token);
			if(i < 0)
				{
				Printf("*** INVALID CODE[%s] ***\n", ScriptLineBuffer);
				}
			else
				{
				if(j>50) j = 50;	//	limit the number that can be added
				Printf("*** SET[%s] to >>> %d <<<\n", GetPrizeName(i), j );
				ScriptAdd[ScriptAddN][0] = i;	//	CodeIndex of the weapon
				ScriptAdd[ScriptAddN][1] = j;	//	number set
				ScriptAdd[ScriptAddN][2] = 'S';	//	SET
				ScriptAddN++;
				}
			break;

		case 'I' :	//	INCrement
			sscanf(params,"%30s %d", token,&j);
				if(j>50) j = 50;	//	limit the number that can be added
			i = GetCodeIndex(token);
			if(i < 0)
				{
				Printf("*** INVALID CODE[%s] ***\n", ScriptLineBuffer);
				}
			else
				{
				}
			break;

		case 'M' :	//	MSG
			Printf("*** MSG[%s]\n", params);
			strcpy(ScriptMsg, params);	//	params is always shorter than ScriptMsg
			break;

		case 'W' :	//	WIN
			//	WIN is treated like a LEVEL command
			if(ScriptLevel[0] != 0)	//	we already have a level so return and run it
				{
				ScriptLine--;	//	backup and read this again next time around
				Printf("*** WIN this level to make the jump\n");
				fclose(ScriptFile);	ScriptFile = NULL;
				return;
				}
			//	to get back here the script must have just run a level from the above return
			//	so test if a win occured. [The first Tank only]
			//	If a win then jump to the label else just continue reading from the next line
			if(Test4Win() == false)
				{
				Printf("*** You LOST !!! no jump for you\n");
				break;	//	skip to next script line.
				}
			Printf("*** A WIN jump to [%s]\n", token);
			sscanf(params,"%30s", token);	//	read in label
			//	FindScriptLabel scans the file and sets ScriptLine
			//	or if label not found sets ScriptEnd
			FindScriptLabel(token);
			if(ScriptEnd)Printf("Label not found\n");
			break;

		case 'J' :	//	JUMP
			sscanf(params,"%30s", token);	//	read in label
			FindScriptLabel(token);	//	this leaves the file at the label
			Printf("*** JUMP[%s]\n", (ScriptEnd?"EXIT":token) );
			break;

		case 'T' :	//	TEST
			{
			char t1[32], t2[32], t3[32], t4[32];
			t1[0] = t2[0] = t3[0] = t4[0] = 0; 
			sscanf(params,"%30s%30s%30s%30s", t1, t2, t3, t4);	//	read in labels
			Printf("*** TEST  [%s]  [%s]  [%s]  [%s]\n", t1, t2, t3, t4);
			}
			break;

		case 'C' :	//	CALL
			Printf("*** CALL[%s] at %d\n", (ScriptEnd?"EXIT":token), ScriptLine );
			ScriptReturnLine = ScriptLine;//	save the return location
			sscanf(params,"%30s", token);	//	read in label
			FindScriptLabel(token);				//	this leaves the file at the label
			break;

		case 'R' :	//	RETURN
			Printf("*** RETURN to %d from %d\n", ScriptReturnLine, ScriptLine);
			if(ScriptReturnLine == 0)break;	//	no call active
			FindScriptLine(ScriptReturnLine);
			ScriptReturnLine = 0;
			Printf("*** RETURN at %d\n", ScriptLine);
			break;

		case 'E' :	//	END
			Printf("*** END\n");
			//	END is not acted on until the level is over so weapon numbers can be restored.
			ScriptEnd = true;	//	just set the flag for now
			fclose(ScriptFile);	ScriptFile = NULL;
			return;

		case '/' :	//	comment
		case '#' :	//	A label but treated as a comment
			Printf("//[%s]\n",ScriptLineBuffer);
			break;

		default:
			Printf("*** Invalid Script file token:%s>>%s>> at line:%d\n", token, params, ScriptLine);
		}
	}

//	end of file so return.
if(feof(ScriptFile) )ScriptEnd = true;	//	run whats been read and then exit script
fclose(ScriptFile);	ScriptFile = NULL;
Printf("EOF\n");
return;
}

//===================================================================================
int	ScriptAmmo(int player, int index, int num)
	{
	int	pz,group,seq, old;
	int	i,j;

	////////////////////////////////////////////////////////
	//	Modification (ie a hack)
	//	if index is negative then the item is set to the num
	//	  else num is added to the existing value.
	////////////////////////////////////////////////////////

	old		= 0;
	pz    = GetPrizeCode(index);
	group = pz%100;		//	ChestType 0 to 6 inclusive; 7 is empty
	seq   = pz/100;		//	sequenceIndex 0 to 99

	switch(group)
		{
		case 4 :	//	****** Defense Item ******
			if(seq < NUM_DEFENSE)
				{
				old = ta[player].defense[seq].amount;
				if(index < 0)	ta[player].defense[seq].amount = 0;
				ta[player].defense[seq].amount += num;
				}
			if((seq == 9)	//	[FUE] Fuel
					|| (seq == 10)	//	[SIG] Laser Sight
					||	(seq == 11)	)//	[AUT] Auto Defense
				{
				old = ta[player].specials[seq].amount;
				if(index < 0)	ta[player].specials[seq].amount = 0;
				ta[player].specials[seq].amount += num;
				}
			break;
		case 5 :	//	****** Super Weapon ******
			j = seq%11;
			switch(j)
				{
				case 0: i = W_GIGANUKE;						break;
				case 1: i = W_MONKEYMORTAR;				break;
				case 2: i = W_POLARISMISSILE;			break;
				case 3: i = W_BLACKHOLEBOMB;			break;
				case 4: i = W_TWINELECTRICLANCES; break;
				//W_TWINELECTRICLANCES_NEGATIVE,
				case 5: i = W_FUNKYNUKE;					break;
				//W_FUNKYNUKECHILD,
				case 6: i = W_AIRSTRIKE;					break;
				case 7: i = W_OBCANNON;						break;
				case 8: i = W_SUPERRAIL;					break;
				//W_SUPERRAILCHILD,
				case 9: i = W_RINGOFFIRE;					break;
				case 10: i = W_SILVERBULLET;			break;
				default : i = W_DEATHSHEAD;
				}
			old = ta[player].weapons[i].ammo;
			if(index < 0)	ta[player].weapons[i].ammo = 0;
			ta[player].weapons[i].ammo += num;
			break;
		case 6 :	//	****** Weapon ******
			i = seq%NUM_PROJECTILES;
			old = ta[player].weapons[i].ammo;
			if(index < 0)	ta[player].weapons[i].ammo = 0;
			ta[player].weapons[i].ammo += num;
			break;
		default :
			// nothing....
			old = -1;
		}
	return old;
	}

//===================================================================================
void	SetAmmo(int player, int index, int num)
	{
	//	if index is negative then the item is set to the num rather than added to existing amount.
	ScriptAmmo(player, -index, num);
	}
//===================================================================================
void DoScriptCmds(void)
	{
	int	old, extra, wep, AorS;
	int	i,j,k;
	int	player=0;	//	one tank only

	if(ScriptAddN == 0)return;	//	no commands; could be just a level change
	Printf("DoScriptCmds [%d]\n", ScriptAddN);	///%%

	if(multiplayer)
		{
		ScriptAddN = 0;
		return;	//	no commands in multiplayer, just level changes.
		}

	for(player=0; player<MAX_TANKS; player++)	//	Add to all live players before level start.
		{
		if(ta[player].exist > 0)
			{
			Printf("********* Tank %s ***********\n",ta[player].name);
			for(i=0; i < ScriptAddN; i++)
				{
				wep = ScriptAdd[i][0];		//	CodeIndex of the weapon
				extra = ScriptAdd[i][1];	//	number added or set
				AorS = ScriptAdd[i][2];		//	ADD or SET or INC
Printf("   [%d]  wep:[%6s]  added:%d  AorS:[%c]\n",	i, GetPrizeName(wep), extra, AorS);

				if(AorS == 'A')	//	just ADD the extras; remove at end of level
					{
					ScriptAmmo(player, wep, extra);
					}
				else if(AorS == 'I')	//	Increment the count; we don't take it off
					{
					ScriptAmmo(player, wep, extra);
					}
				else if(AorS == 'S')
					{
					old = ScriptAmmo(player, wep, 0);	//	add zero to get current count
					ScriptOld[i][player] = old;		//	save current count for reset at end of level
					extra = extra - old;
					ScriptAmmo(player, wep, extra);		//	delete current & add extra
					}
				}
			}
		}

	//	display message if there is one
	if(ScriptMsg[0] != '\0')
		{
		ShowMsg(ScriptMsg);
		if(multiplayer && multiplayermode == MP_SERVER)
			{
			BCast_WriteByte(STOC_MOTD);
			BCast_WriteString(ScriptMsg);
			}
		}

	if(numrounds <= ScriptRounds)	///	very messy Degauss ??
		{
		numrounds = ScriptRounds;
		ScriptRounds--;
		}
	return;
	}

//===================================================================================
void ResetScriptCmds(void)
	{
	int	cc, count, wep, AorS;
	int	i,j,k;
	int	player=0;	//	 tank

	//	reset all players to the number of items
	//	each had at level start before we added/cleared any..

	if(ScriptAddN == 0)return;	//	no commands
#ifdef	_DEBUG
	Printf("ResetScriptCmds Start[%d]\n", ScriptAddN);
#endif

	for(player=0; player<MAX_TANKS; player++)
		{
		if(ta[player].exist > 0)
			{
			for(i=0; i < ScriptAddN; i++)
				{
				wep = ScriptAdd[i][0];		//	CodeIndex of the weapon
				count = ScriptAdd[i][1];	//	number added or set
				AorS = ScriptAdd[i][2];		//	ADD or SET (don't do INC)
				cc = ScriptAmmo(player, wep, 0);	//	add zero & return current count
#ifdef	_DEBUG
Printf("[%d]  wep:%d[%6s]  added:%d  AorS:[%c]  current:%d\n", i, wep, GetPrizeName(wep), count, AorS, cc);
#endif

				if(AorS == 'A')	//	we now subtract the extras we added
					{
					k = cc - count;	//	current count minus number added but not negative
					if(k < 0)k = 0;	// this is the final value
					ScriptAmmo(player, wep, k - cc);
#ifdef	_DEBUG
Printf("unADD wep:%d => %d\n", wep, k-cc);	///%%
#endif
					}
				else if(AorS == 'S')
					{
					count = ScriptOld[i][player] - cc;	//	 Add old value and subtract the current count.
					ScriptAmmo(player, wep, count);		//	restore original count
#ifdef	_DEBUG
Printf("unSET wep:%d => %d\n", wep, count);	///%%
#endif
					}
				}
			}
		}

for(i=0; i < ScriptAddN; i++)	//	Clear array
	{
	ScriptAdd[i][0] = 0;
	ScriptAdd[i][1] = 0;
	ScriptAdd[i][2] = 0;
	}

	ScriptMsg[0] = '\0';
	ScriptAddN = 0;	//	All done so don't do it again.
	Printf("ResetScriptCmds done\n");
	return;
	}

//===================================================================================
void FindScriptLabel(char* label)
	{
	char	token[32];

	//	sets scriptline to the label position
if(!OpenScriptFile())
	{
	ScriptedGame = false;
	return;
	}

	ScriptLine =0;
	while( !feof(ScriptFile) )
		{
		fgets(ScriptLineBuffer, 250, ScriptFile);
		ScriptLine++;
		if(ScriptLineBuffer[0] == '#')
			{
			sscanf(ScriptLineBuffer,"#%30s", token);	//	read in label
			Printf("LABEL:[%s]\n", token);
			if(strcmp(label,token) == 0) return;	//	found it
			}
		}
	//	didn't find the label so set END to cleanup script and exit
	ScriptEnd = true;;
	return;
	}

//===================================================================================
void Read_InventInfo(void) // Read_InventInfo.needs to be in sync with Write_InventInfo
	{
	//	This is used by client to read in the data sent from the server by Write_InventInfo Read_
	int		i,items,WepCode,num;

#ifdef	_DEBUG
	Printf("Read_InventInfo\n");
#endif

	items = inbuf->ReadByte();
	for(i=0; i<items; i++)
		{
		WepCode = inbuf->ReadByte();
		num			= inbuf->ReadShort();
		SetAmmo(net_localplayer, WepCode, num);
		}
	return;
	}

//===================================================================================
void Clear_InventInfo(void) // Clear the Inventory
	{
	//	This is used by the client to clear the inventory
	int		i, items;

	items = NumberOfPrizes();
#ifdef	_DEBUG
	Printf("Clear_InventInfo\n");
#endif

	for(i=0; i<items; i++)
		{
		SetAmmo(net_localplayer, i, 0);
		}
	return;
	}

//===================================================================================
void Write_InventInfo(int Player) // Write_InventInfo.needs to be in sync with Read_InventInfo
	{
	//	This is used by the server to send the invertory to the client
	//	The client will read in the data via Read_InventInfo()

	int		i,items,WepCode,num, group, num_prizes;

#ifdef	_DEBUG
	Printf("Read_InventInfo\n");
#endif

	//	check if player is on network as client?
	if((ta[Player].exist <= 0)
		|| (Player == net_localplayer)
		|| (ta[Player].skill != TANKSKILL_HUMAN)
		|| (ta[Player].netstate <= NETSTATE_LEVELSETUP)
		)	return;

	num_prizes = NumberOfPrizes();
	items = 0;
	for(WepCode=0;WepCode<num_prizes;WepCode++)
		{
		num = ScriptAmmo(Player, WepCode, 0);
		if(num < 1) continue;
		++items;	//	only count weapon stock
		}
	ta[Player].outbuf->WriteByte(items);

	for(WepCode=0;WepCode<num_prizes;WepCode++)
		{
		num = ScriptAmmo(Player, WepCode, 0);
		if(num < 1) continue;
		ta[Player].outbuf->WriteByte(WepCode);
		ta[Player].outbuf->WriteShort(num);
		}
	return;
	}

//===================================================================================
bool Test4Win(void)
	{
	//	test is for tank zero only (the first tank in the list)
	//	this is always human if multiplayer

Printf("Test4Win:[%f %d]\n", ta[0].health, ta[0].needstodie );

	if(ta[0].health > 0 && ta[0].needstodie <= 0) return true;
	return false;
	}

//===================================================================================
