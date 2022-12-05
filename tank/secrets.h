
//
// Secret stuff
//

int	WriteSecretStuff(void);
int	ReadSecretStuff(void);



//
// PREPROCESSORS
//

#define	SECRET_FILENAME	"cdirt.ssf"
#define	NUM_SECRETLEVELS	NUM_SECRETCHARACTERS	// This should be 4
#define	NUM_SECRETINSTANCES	NUM_SECRETLEVELS		// See a pattern here?

//
// STRUCTURES
//

typedef	struct
{
	char	title[128];
	char	author[128];
	char	musicfile[256];
	int		foreground;
	int		background;
	int		startx[MAX_TANKS];
	int		starty[MAX_TANKS];

} secretlevel_t;


typedef	struct
{
	bool            (* InstanceCondition)(void);		// Function that determines if a player
														// qualifies to fight the secret character or not
	
	int				character;							// Index of the unlockable character
	int				secretportait;						// Secret "locked" portrait
	int				secretlevel;						// Index pointer of the secretlevel_t array
	int				startcash;							// How much starting cash the secret character has
	int				starthealth;						// How much starting health to begin with
	bool			unlocked;							// Character already been unlocked? Skip the
														// check then
} secretinstance_t;


//
// VARIBLES
//

extern		int								start_cash;
extern		int								start_rounds;
extern		int								used_cheat;
extern		int								testNumber;
extern		secretlevel_t			secretlevels[NUM_SECRETLEVELS];
extern		secretinstance_t	secretinstances[NUM_SECRETINSTANCES];

//
// FUNCTIONS
//

void	PresentChallenge(int character);
void	AssembleLevel(int	l);
void	AssembleCharacters(int	instance);
void	UnlockCharacter(int character, bool showscreen);
int		CheckForUnlockable(void);
void	StartSecretLevel(int	name);

extern bool UnlockableFlag;
