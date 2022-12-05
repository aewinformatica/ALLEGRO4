//		Editor.h
//
typedef		struct
{
	int		x;
	int		y;
	int		flags;
} tank_placement;

typedef		struct
{
	char		name[64];
	sprite_t	spritestate;
	int			wobj;
	int			x;
	int			y;
} levelsprite_t;

extern	levelsprite_t	editorsprites[NUM_WOBJECTS];


extern	tank_placement		tankpos[MAX_TANKS];
//extern	BITMAP	*editor_background;
//extern	BITMAP	*editor_foreground;
extern	BITMAP	*editor_guibuffer;
//extern	int		editor_waterlevel;
//extern	int		editor_watercolor;


extern	DIALOG editor_main[];

void	EditorDrawer(BITMAP	*buf);
void	InitTankPlacement(BITMAP	*bmp);
void	InitEditor(void);
void	CDEdit(void);

///////////////////////////////////////////////////////////////////
////////////////// M E N U   F U N C T I O N S ////////////////////

int		loadlevel_proc(void);
int		savelevel_proc(void);
int		quit_proc(void);
int		selectforeground_proc(void);
int		selectbackground_proc(void);
int		spriteselect_proc(void);
int		placetanks_proc(void);

int		savelevel(AL_CONST char *filename);
//void	loadlevel(AL_CONST char *filename);
bool	loadgamelevel(AL_CONST char *filename);
int		dumplevel(AL_CONST char *fname); 
void	FreeGameData(void);

BITMAP	*loadbmpchunk(BITMAP *bmp, PACKFILE *f);
void	savebmpchunk(BITMAP*	bmp, PACKFILE *f);
extern	int		unusedcolour;

///////// Treasure Chest /////////
extern	char *level_chest_prize(int index, int *list_size);
extern	int get_chest_prize(int old );
extern	char *prize_list[];
extern	int	num_prizes;

