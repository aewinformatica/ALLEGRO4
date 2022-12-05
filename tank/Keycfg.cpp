#include <time.h>
#include <string.h>
#include <allegro.h>
#include "allegro/internal/aintern.h"

#include "scorch.h"
#include "keycfg.h"

bool		usejoystick;
bool		usekeyboard;

volatile		char	oldkey[KEY_MAX];
JOYSTICK_INFO	*oldjoy;
int				nojoytime;

int		oldmousex;
int		oldmousey;
int		oldmousez;
int		oldmouseb;
int		mouseflags;

//int		oldspacebarstate;
int		keyboardflags;
int		old_escdown;
int		old_spacedown;
int		old_tabdown;
int		old_f1down;
int		old_ldown;
int		old_rdown;
int		old_udown;
int		old_ddown;


//
// InitJoyStick - Attempt to locate and initialize any joysticks
//

void	InitJoyStick(void)
{
	// Init
	if(install_joystick(JOY_TYPE_AUTODETECT) != 0)
		{
		usejoystick = false;
		
		// Report in console
		Printf("No joysticks found\n");
		return;
		}

	// Check to see how many joysticks are connected
	if(!num_joysticks)
		{
		// None.
		usejoystick = false;

		// Report in console
		Printf("No joysticks found\n");

		return;
		}


	// Attempt to calabrate to joystick(s)
	while(joy[0].flags & JOYFLAG_CALIBRATE) 
		{
		if(calibrate_joystick(0) != 0) 
			{
			usejoystick = false;
			return;
			}
		}

	// Setup the old joystick varible
	oldjoy = (JOYSTICK_INFO *)malloc(sizeof(JOYSTICK_INFO) * num_joysticks);

	usejoystick = true;

	nojoytime = 0;

	// Report in console
	Printf("Found %d joysticks attached\n", num_joysticks);
}


void ClearAllKeys(void)
{
	int	c;
	clear_keybuf();
	//memcpy(oldkey, (const char *)key, KEY_MAX);
	for (c=0; c<KEY_MAX; c++)
		oldkey[c] = key[c];

}

/*void clear_key(void)
{
   int c;

   for (c=0; c<KEY_MAX; c++) {
      key[c] = 0;
      _key[c] = 0;
   }
}*/

void CheckKeys(void)
{
	

#ifdef	DEDICATED_SERVER
	return;
}
#else

	int		c;

	// Debug purposes only
	if (IsKeyUp(KEY_F10))
		quitgame();

	if (IsKeyUp(KEY_F2))
		ToggleScoreBoard();

	if (IsKeyUp(KEY_TILDE))
		ToggleConsole(true);

	//memcpy(oldkey, (const char *)key, KEY_MAX);
	for (c=0; c<KEY_MAX; c++)
		oldkey[c] = key[c];

	// Console takes presidence over everything else
	if (console_active)
		ConsoleInput();


	// Get new input
	poll_keyboard();
	
}
#endif


void		CheckJoyStick(void)
{
#ifdef	DEDICATED_SERVER
	return;
}
#else
	int		i;

	if (usejoystick)
	{
		if (nojoytime > 0)
			nojoytime--;

		for (i=0; i<num_joysticks; i++)
			memcpy(oldjoy+i, joy+i, sizeof(JOYSTICK_INFO));

		poll_joystick();
	}
}
#endif


bool		IsKeyUp(int kbutton)
{
	// Button is currently being pressed
	if (key[kbutton])
		return false;

	// It was pressed, just last tic
	if (oldkey[kbutton])
		return true;

	// Everything else... (should never get here)
	return false;
}


bool		IsKeyDn(int kbutton)
{
	// Button is currently being pressed
	if (oldkey[kbutton])
		return false;

	// It was pressed, just last tic
	if (key[kbutton])
		return true;

	// Everything else... (should never get here)
	return false;
}


bool		IsJoyButtonDn(int j, int b)
{
	if (oldjoy[j].button[b].b)
		return false;

	if (joy[j].button[b].b)
		return true;

	// Everything else
	return false;
}


bool		IsJoyButtonUp(int j, int b)
{
	if (joy[j].button[b].b)
		return false;

	if (oldjoy[j].button[b].b)
		return true;

	// Everything else
	return false;
}

bool		IsJoyStickMoved(int j, int s, int a, int sensitivity)
{
	if (sensitivity <= 0)
		sensitivity = 1;

	if ((joy[j].stick[s].axis[a].pos/sensitivity) != 0 && 
		(oldjoy[j].stick[s].axis[a].pos/sensitivity) == 0)
		return true;

	return false;
}


void mouse_callback2(int flags)
{
	mouseflags = flags;
}


//
// Input_Thinker - Main function that controls the relay of keys
//

void		Input_Thinker(void)
{
	CheckKeys();
}
