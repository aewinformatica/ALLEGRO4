//%%	#include "scripts.h"

extern	bool	ScriptedGame;
//extern	int ScriptRounds;
//extern	int ScriptLine;
//extern	int Scripta;
//extern	int Scriptb;
//extern	int Scriptc;
//extern	int Scriptd;
//extern	int Scripte;

bool	OpenScriptFile(char*	SFN	= NULL); 
void	SetScriptLevel(char* file_name);
void	GetScriptCmds(void);
void	DoScriptCmds(void);
void	ResetScriptCmds(void);

//	network 
void Write_InventInfo(int Player);
void Read_InventInfo(void);

//	script helper functions
int		ScriptAmmo(int player, int index, int num);
void	FindScriptLabel(char* label);
bool	Test4Win(void);
void	FindScriptLine(int num);

//===================================================================================
