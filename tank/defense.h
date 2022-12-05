
typedef	struct
{
	char	*name;								// Whats my name? Whos your daddy?
	int		energy;								// How much additional HP will we get?
	int		h, e, b;							// (H)eat, (E)nergy, and (B)allisitic protection
	int		width;								// Width (duh)
	int		height;								// Height (duh, again)
	int		cost;								// The cost of this
	int		numrecieve;							// How many you get w/ each purchase
	void	(*hitproc) (SHOT	sh);
	void	(*drawerproc) (int	player);
	void	(*activateproc)(int player, int refnum);

	int		ref;								// From what array did this come from? Defense? Special?

} defensetype_t;


enum
{
	battery_charge,
	parachutes,
	floatation,
	conventional_shield,
	electric_shield,
	fire_shield,
	ballistic_shield,
	superconventional_shield,
	niv_shield,
	NUM_DEFENSE
};


extern		defensetype_t	defenses[NUM_DEFENSE];
extern		DIALOG			dialog_playerdefense[];


void StartDefenseBox(int	player, int defstate=2);
void GiveDefense(int	player, int ammotype, int	amount);
void DefensePowerUp(int	player, int		deftype);
void DefenseDrawer(int	player);

void BatteryPowerUp(int player, int deftype);
void ShieldPowerUp(int	player, int		deftype);
