//
// DIALOGS.H header file
// 
#include "agup.h"
#include "agtk.h"
#include "aphoton.h"
#include "awin95.h"
#include "aase.h"
#include "abeos.h"
#include "ans.h"


// External varibles
extern DIALOG dialog_newgame[];
extern DIALOG dialog_creategame[];	
extern DIALOG dialog_soundoptions[];
extern DIALOG dialog_videooptions[];
extern DIALOG dialog_cashoptions[];
extern DIALOG dialog_mapsettings[];
extern DIALOG dialog_talkingtanks[];

void	mainmenu();
void	switchteam();

extern	char	cashstr[64];

// menu non-persistant flags
extern	bool	barreloutline;
extern	bool	screenshotBMP;
extern	int 	screenshotJPG;

// Test
int new_update_dialog(DIALOG_PLAYER *player);

// External functions
void		Init_Dialog_Stuff(void);
int			Start_Dialog(DIALOG *dlg);
void		Kill_Dialog(DIALOG *dlg);

void		Init_Dialog_Stuff(void);
void		Shutdown_Dialog_Stuff(void);
