#define		NUM_TANKDEATHS		7

typedef		struct
{
	char	name[64];
	int		time;
	void	(* proc)(int tank);
} tankdeath_t;

extern	tankdeath_t		tankdeaths[NUM_TANKDEATHS];


// EXTERNAL FUNCTIONS
void		StartRandomDeathSequence(int tank);
void		StartDeathSequence(int tank, int seq);
int			CheckTankDeathSeq(void);
int			IsTankDeathSeq(void);
