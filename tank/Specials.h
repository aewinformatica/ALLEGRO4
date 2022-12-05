
#define			SPECIAL_NORMAL				0
#define			SPECIAL_DEFENSEBOX			1		// This will show up in the players 'defense box'


typedef struct
{
	char	name[64];
	int		cost;
	int		numrecieve;
	int		flags;
	void	(*activateproc) (int	player, int refnum);
} specialtype_t;


enum
{
	fuel,
	lasersight,
	autodefense,
	NUM_SPECIALS
};


// VARIBLES
extern	specialtype_t		specials[NUM_SPECIALS];


// FUNCTIONS
void GiveSpecial(int	player, int ammotype, int	amount);
void SetupAutoDefense(void);
