#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "dialogs.h"
#include "particles.h"
#include "secrets.h"
#include "scripts.h"

BITMAP *background;
BITMAP *buffer;
DATAFILE	*sounddata;
DATAFILE	*maindata;
DATAFILE	*spritedata;
PALETTE		firepal;
PALETTE		riotpal;
PALETTE		greenpal;
PALETTE		blueflame;
PALETTE		darkbluepal;
PALETTE		fpurplepal, forangepal, fredpal, fvoiletpal;	// For the funky bomb

int		quit;
int		tankturn;
int		lasttankturn;
int		lasttankshot;
int		gamestate;
int		numplayers;
int		numrounds;
int		multiplayer;
int		multiplayermode;
int		drawscoreboard;

int		mrbmmg_mode;

// Timer stuff
volatile  int		gametics;

GROUND ground;
SHOT sh[N_SHOTS];
TANK ta[MAX_TANKS];

DArgs	Args;
char	logfilename[64];
FILE	*logfile;

int		switchstate;		// Running in background or foreground?

// in case editor crashes
int		quit_proc(void);

// Background/forground state trackers

void	switchin_proc(void)
{
	switchstate = SWITCHSTATE_IN;
}

void	switchout_proc(void)
{
	switchstate = SWITCHSTATE_OUT;
}

void	InitSwitchState(void)
{
#ifdef	DEDICATED_SERVER
	return;
#else
	Printf("InitSwitchState: Installing background hooks\n");
	// Attempt to find a switch-mode 
	if(set_display_switch_mode(SWITCH_BACKAMNESIA) == -1)
		{
		Printf("     Unable to set SWITCH_BACKAMNESIA!\n");
		Printf("     Attempting SWITCH_BACKGROUND...\n");
		if(set_display_switch_mode(SWITCH_BACKGROUND) == -1)
			{
			Printf("          Unable to set alternative SWITCH_BACKGROUND!\n");
			Printf("          ...game will not be able to run in the background...\n");
			}
		}

	if(set_display_switch_callback(SWITCH_IN, switchin_proc)==-1)
		Printf("Unable to set SWITCHSTATE_IN");

	if(set_display_switch_callback(SWITCH_OUT, switchout_proc)==-1)
		Printf("Unable to set SWITCHSTATE_OUT");
#endif
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

/* counters for speed control and frame rate measurement */
volatile int update_count = 1;
volatile int frame_count = 0;
volatile int fps = 0;

/* timer callback for controlling the program speed */
void timer_proc(void)
{
   update_count++;
   //frame_count++;
   //rest(1);
}
END_OF_FUNCTION(timer_proc);



/* timer callback for measuring the framerate */
void fps_proc(void)
{
   fps = frame_count;
   frame_count = 0;
}

END_OF_FUNCTION(fps_proc);


void start_timer(void)
{
	/* install timer handlers to control and measure the program speed */
	LOCK_VARIABLE(update_count);
	LOCK_VARIABLE(frame_count);
	LOCK_VARIABLE(fps);
	LOCK_FUNCTION(timer_proc);
	LOCK_FUNCTION(fps_proc);

	install_int_ex(timer_proc, BPS_TO_TIMER(TICRATE));
	install_int_ex(fps_proc, SECS_TO_TIMER(1));
}

void stop_timer(void)
{
	remove_int(timer_proc);
	remove_int(fps_proc);
}


void ticker()
{
#ifndef	DEDICATED_SERVER
	char	s[128];

	// Process input
	Input_Thinker();
#endif

	// Always execute console commands
	Cbuf_Execute();

	Cvar_CheckForChange();

#ifndef	DEDICATED_SERVER
	if(console_active)
		DrawConsole(buffer);

	if(drawscoreboard > 0)
		return;
	
	sprintf(s,"FPS: %d", fps);
	AddRectangle(buffer, (SCREENWIDTH-75), (SCREENHEIGHT-15), 75, 15);

	text_mode(makecol(255,255,255));
	textout(buffer, font, s, (SCREENWIDTH-75)+1, (SCREENHEIGHT-15)+1, makecol(0,0,0));
	text_mode(-1);
#endif

}


void	StartLog(void)
{
	int		i=0;
	FILE	*f;

	if(	logfilename[0] != 0) return; // already set up
	
	// Create the directory if not already there
	_mkdir(".\\logs");

	for(i=0; i<100; i++)
		{
		sprintf(logfilename, ".\\logs\\CDIRT%d.log", i);
		f = fopen(logfilename, "r");
		if(!f)
			break;
		fclose(f);
		}
		

	// Attempt to open the file
	logfile = fopen(logfilename, "w");
	if(!logfile)	// openning a writable file failed.
		return;
	fprintf(logfile, "LOG: %s\n", logfilename);

	if(!logfile)
#ifdef	DEDICATED_SERVER
		fprintf(logfile, "Unable to create log file!\n"); 
#else
		alert("Non-crital error", "Unable to create log file", NULL, "Ok", NULL, 0,0);
#endif
	else
		{
		fprintf(logfile, "-------------------------------------------------------------------\n");
		fprintf(logfile, "CHARRED DIRT LOG\n");
		fprintf(logfile, "-------------------------------------------------------------------\n\n");
		}

	// We're done for now
	fclose(logfile);
}

void	LogStr(char	*str)
{
if(str == NULL) return;
#ifdef	_DEBUG
	if(	logfilename[0] == 0) StartLog(); // auto log in debug mode
#else
		if(	logfilename[0] == 0) return; // not set up
#endif

	logfile = fopen(logfilename, "a");
	if(!logfile)
		{
		G_Printf("*** UNABLE TO WRITE TO LOG!");
		logfilename[0]=0;
		}
	else
		{
///	Added gametics marker at begining of all logged messages		Degauss
		if(strlen(str) < 5)
			fprintf(logfile, str);	// don't tag "/n"
		else
			fprintf(logfile,"%8d::%s", gametics, str);
		fclose(logfile);
		}
}


#ifndef	DEDICATED_SERVER
//
// ScreenShot - Take a screenshot
//

void ScreenShot(void)
{
	BITMAP	*shotbuf=buffer;
	char	basename[8]="CHARRED";
	char	filename[64];
	char	msg[64];
	int		filenum=0;

//	Check if we do screenshots at all
if(!screenshotBMP && (screenshotJPG==0) )
	return;

	
	// Attempt to make a directory (doesn't matter if it already exists)
	_mkdir("screenshots");

	// First, find a appropiate filename
	for(filenum=0; filenum<998; filenum++) // filenum set to 999 if dir full
		{
		sprintf(filename, "screenshots\\%s%.3d.bmp", basename, filenum);
		
		if(exists(filename) == 0) // no bmp file
			{
			sprintf(filename, "screenshots\\%s%.3d.jpg", basename, filenum);
		
			if(exists(filename) == 0) // no jpg either -- found a slot
				break;
			}
		}

	if(gamestate == GS_INGAME)
		{
		// Update everything
		ground.needsupdate = true;
		LoopDrawer(shotbuf);
		}

if(screenshotBMP)
	{
	// We have the filename, store the bitmap
	sprintf(filename, "screenshots\\%s%.3d.bmp", basename, filenum);
	save_bmp(filename, shotbuf, NULL);

	// Spawn a message to console (don't upset the screen.)
	Printf("Wrote '%s'\n", filename);
	}

if(screenshotJPG != 0)
	{
	// we save jpeg as same file name as bmp to keep in sync.
	sprintf(filename, "screenshots\\%s%.3d.jpg", basename, filenum);
	save_jpeg(filename, shotbuf);
	// Spawn a message to console (don't upset the screen; maybe a video sequence)
	Printf("Wrote '%s'\n", filename);
	}
}
#endif


void	StartDemoMode(void)
{
	int		num_demorounds = 1000;

	numrounds = num_demorounds;

	// Setup tank varibles
	StartNewGame(MAX_TANKS);

	// Default que
	InitDefaultTurnQue();
		
	// Load the actual level into memory
	LoadLevel(NULL);	// random level

	PlayLevelOGG("playermenu.ogg");
		
	// Cycle through buystuff screens
	StartBuyStuff();

	// Enter gamestate
	StartLevel(true);
}

static	float GravityValue = 0.4;
float	Gravity(float gv)
{
if(gv == 0 )return GravityValue; // default call

// no negative or zero values as this will cause a crash
//  or a freeze (waiting for missile to hit something).
if(gv > 0.05 )GravityValue = gv;
if(gv > 50 )GravityValue = 0.4; // be realistic else self kill
return GravityValue;
}

///		#####################################################################
///		#####################################################################
///		#####################################################################
///		########################  MAIN  #####################################
///		#####################################################################
///		#####################################################################
///		#####################################################################

int main(int argc, char **argv)
{
	// Setup command line stuff
	Args.SetArgs(__argc, __argv);

	logfilename[0]=0; // set logfilename to null
	if(Args.CheckParm("-logfile"))
		StartLog();

	// Set game state to "loading"
	gamestate = GS_LOADINGGAME;

	Cbuf_Init ();

	Cmd_Init ();
	Cvar_Init ();

	Printf("Console ready\n");

	LoadConfig();

	// we need to add the early commands twice, because
	// a basedir or cddir needs to be set before execing
	// config files, but we want other parms to override
	// the settings of the config files
	Cbuf_AddEarlyCommands (false);
	Cbuf_Execute ();

	// need the clients to start from the same point
	srand(654321);	// srand((int)time(0));

#ifndef	DEDICATED_SERVER	
	allegro_init();
#else
	install_allegro(SYSTEM_AUTODETECT, &errno, atexit);
#endif

	install_timer();
	install_mouse();
	mouse_callback = mouse_callback2;
	
#ifndef	DEDICATED_SERVER
	install_keyboard();
#endif


	// No sound option?
	if(Args.CheckParm("-nosound"))
		nosound = 1;

	// No music option?
	if(Args.CheckParm("-nomusic"))
		nomusic = 1;

	// Setup sound playback
#ifndef	DEDICATED_SERVER
	if(nomusic <= 0)
		InitMusic();

	if(nosound <= 0)
		{
		reserve_voices(32, 0);
		set_volume_per_voice(3);
		install_sound(DIGI_DIRECTX(0), MIDI_NONE, NULL);
		}
#else
	nosound = 1;
	nomusic = 1;
#endif

	// Set the screen resolution to 800x600x16
	set_color_depth(SCREEN_BPP);

	// Set the screen mode based on the current configuration
#ifndef	DEDICATED_SERVER
	if(fullscreen->value > 0)
		{
		if(set_gfx_mode(GFX_DIRECTX_ACCEL, 800,600,0,0) != 0)
			{ 
			set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); 
			clear_to_color(screen, 0);
			}
		}
	else
		set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
#endif

	// Reset the fullscreen cvar so the engine does not attempt to change
	// the resolution twice when starting up
	fullscreen->modified = false;

	// Startup the console
#ifndef	DEDICATED_SERVER
	Init_Console();
#endif

	// Set new gamestate
	gamestate = GS_TITLE;

	// Print welcome text in the console
	Printf("%s %s\n--------------------------------------------\n\n", WINDOW_TEXT, VERSION);
  Printf("Color Depth of Screen is %d bits. Game Color Depth is 16 bits.\n", desktop_color_depth() );

	InitSwitchState();

	// Set window title
	set_window_title(WINDOW_TEXT);

	// Setup joystick support if we have it
	Printf("InitJoyStick: ");
	InitJoyStick();

	// Load sprite data
	Printf("Setup_Sprites: Loading alphas.dat\n");
	Setup_Sprites(SCREEN_BPP);
  if(!alphadata)
		{
#ifdef	DEDICATED_SERVER
		Printf("Unable to load alphas.dat!\n"); 
		getchar();
#else
		alert("Error", "Unable to load data\\alphas.dat!", NULL, "Exit", NULL, 0,0);
#endif
    exit(-1);
		}

	// Allocate memory for dialogs
	Printf("Init_Dialog_Stuff: Initalizing AGUP\n");
	Init_Dialog_Stuff();

	// Startup the network
	Printf("InitNetwork: ");  // will exit if network not available.
	InitNetwork();

	// Load data-file
	Printf("Loading scorch.dat...\n");
	maindata = load_datafile("data\\scorch.dat");
	if(!maindata)
		{ 
#ifdef	DEDICATED_SERVER
		Printf("Unable to load scorch.dat!\n");
		getchar();
#else
		alert("Error", "Unable to load scorch.dat!", NULL, "Exit", NULL, 0,0);
#endif
		exit(-1);
		}

	// Set the mouse cursor before anything else
	set_mouse_sprite((BITMAP *)maindata[MOUSE_CURSOR].dat);

	// Load sound-data (dont require sounds.dat if -nosound is used)
	sounddata = NULL;
	if( !nosound ) // sound has not been turned off
		{
	  Printf("Loading sounds.dat...\n");
	  sounddata = load_datafile("data\\sounds.dat");
	  if(!sounddata)
			{ 
			int ans=0;
		  Printf("    !!!!Unable to load sounds.dat!!!!\n");// check if need to exit
#ifndef	DEDICATED_SERVER
		  ans = alert("Error", "Unable to load sounds.dat!", NULL, "Exit", "Continue!", 0,0);
#endif
			if(ans == 2)				// Continue selected
				{
				nosound = 1;
				Printf("Sound has been turned off!\n");
				}
			else
				{
				Printf("Exit due to NO Sound files!\n");
				getchar();
			  exit(-1);
				}
			}
		}
	// Setup sprites -- abort game if it fails
	Printf("G_InitSprites: Loading sprites.dat\n");
	G_InitSprites();

	// Init our fire palette
	Printf("InitFire: Creating fire palette\n");
	InitFire();

	// Read secret data (secret character) stuff as well
	ReadSecretStuff();  // if no file -- just skip it

	// Create the main buffer
	Printf("Creating primary screen buffer\n");
	buffer=create_bitmap(SCREENWIDTH,SCREENHEIGHT);

	if(!buffer)
		{
		alert("Error", "Unable to create screen buffer.", NULL, "Exit", NULL, 0,0);
		exit(-1);
		}

	start_timer();

	Printf("GUI_InitButtons: Creating GUI dimensions\n");
	GUI_InitButtons();

	// Init the startup menu..etc
	GUI_SetupMenu (main_menu);
	PlayLevelOGG("title.mp3");

	// [Sean] - this *REALLY* needs to be cleaned up
	ground.needsupdate = true;

	// Set our default font
	font = (FONT *)maindata[VER10].dat;

	// Set some tank varibles
	Printf("inittanks: Setting global tank varibles\n");
	inittanks();

	// Setup FPS timer
	start_timer();

	// Dirty rectangle system
	drectangles = NULL;

	// Not in multiplayer (...yet!)
	multiplayer = 0;

	// If this is running a dedicated server, setup all the varibles
	// necessary for multiplayer and jump right into the game
#ifdef	DEDICATED_SERVER
	Printf("BeginDedicatedGame: Auto-init multiplayer values\n");
	BeginDedicatedGame();
#endif


// The following command line checks are for the client only running a full graphics
// mode GUI. They dont have any revelance

#ifndef	DEDICATED_SERVER
	// Running in demo mode?
	if(Args.CheckParm("-demoloop"))
		{
		demomode = 1;
		StartDemoMode();
		}

	if( Args.CheckParm("-script") )
		{
		char	*a;
		a = Args.CheckValue("-script");
		if(a == NULL) a = "----.txt";
		Printf("Look for script\n");
		ScriptedGame = true;
		if(OpenScriptFile(a) != true)
			{
			ScriptedGame = false;
			Printf("No script found!!\n");
			}
		else
			{
			ScriptedGame = true;
			//	true only if not a client in a multiplayer game
			}
		}
	else
		{
		ScriptedGame = false;
		}

	if(Args.CheckParm("-bmmg"))
		{
		int		bmsnd = rand()%3;
		
		// Play a random sound
		if(bmsnd == 0)
			StartSound(EASTEREGG_1);
		if(bmsnd == 1)
			StartSound(EASTEREGG_2);
		if(bmsnd == 2)
			StartSound(EASTEREGG_3);
		
		mrbmmg_mode = 10;
		}
	else
		mrbmmg_mode = -1;
	
	// Detects and sets up the existance of the -connect
	// parameter
	CheckForConnect();

#endif
	
	Printf("Entering game loop...\n\n");

	// CHARRED DIRT: OUR MAIN LOOP
	while(1)
		{
		if(update_count > 0)
			{
			update_count=0;

			CheckJoyStick();

			switch(gamestate)
				{
				case	GS_INGAME:
					{
					if(menuactive)
						{
						GUI_GetInput();

						// Pause the gameplay only in single player
						if(!multiplayer)
							{
							LoopDrawer(buffer);
							break;
							}
						}
					// Main thinker / drawer for the game
					InGame_Runner();
	 				break;
					}	// end case

				case	GS_TITLE:
					{
					Title_Drawer(buffer);
					if(menuactive)
						{
						GUI_GetInput();
						GUI_Drawer(buffer);
						}
					else
						Title_Input();
					break;
					}

				case	GS_CONNECTING:
					{
					JoinGame_Setup(buffer);
					break;
					}
					
				case	GS_INTERWAIT:
					{
					InterTicker();
					Inter_Refresh();
					break;
					}

				case GS_EDITOR:
					{
					Printf("GS_EDITOR\n");
					quit_proc();
					ClearAllKeys(); // clear out any editor abort junk
					// activate the title
					menuactive = true;
					gamestate = GS_TITLE;
					break;
					}

				// Should never happen
				default:
					Printf("gamestate is unknown\n");
					break;
				}	// end switch

			ticker();

			Screen_Blit();

 			gametics++;
			frame_count++;

#ifdef DEDICATED_SERVER
				{
				// console input
				char *cmd = D_ConsoleInput();
				if (cmd)
					Cbuf_ExecuteText (EXEC_NOW, cmd); 

				net_doselect(netsocket->sock);
				}
#endif

			}	// end update_count if()

		}	// 	end while(1)
	///////////////////// END CHARRED DIRT MAIN LOOP

	return 0;
	
}
//#ifndef	DEDICATED_SERVER
END_OF_MAIN()
//#endif

void	InGame_Runner(void)
{
	if(gamestate != GS_INGAME)
		return;

	// ESC key for main menu
	if(IsKeyDn(KEY_ESC) && 
		IsTankMessage() <= 0 && 
		!menuactive)
			{ 
			mainmenu();
			}

	// Pause the game
	if(IsKeyDn(KEY_P) && 
		IsTankMessage() <= 0 && 
		!menuactive &&
		!multiplayer &&
		!console_active )
			{ 
			int i = alert3("Pause", "The current game has stopped!", "Do you want to?",
										 "Continue", "Stop Game", "Exit CDirt",
										 0, 0, 0);
			switch(i)
				{
				case 1:
					// do nothing
					break;

				case 2:
					// Stop
					endgame();
					break;

				case 3:
					// Exit
					quitgame();
					break;

				default:
					return;
				}
			}


	// The enter (or return) key displays the message box
	// allowing the player to say something
	if(IsKeyDn(KEY_ENTER) && IsTankMessage() <= 0)
		Start_TankMessage();

	if(IsTankMessage() > 0 &&
		ta[net_localplayer].tankstate == TANKSTATE_PLAYING)
		{
		TankMessage_Input();
		}

	if(multiplayer)
		Tank_SetOldVars();

	// Common multiplayer thinker
	Net_Thinker();	//	get network packets

	// Cheats?! OMFG
	cheat_checkcheats();

/*/////////////////////////////////////////zzz Degauss added stuff
	if(multiplayer && multiplayermode == MP_CLIENT)
		{
		// The ground is not falling unless the server said so.
		ground.falling = false;
		}

	//	if required, update client dirt at each frame
	if(multiplayermode == MP_SERVER)
		{
		if(ground.falling)
			{
			BCast_WriteByte(STOC_DropDirt);
			Printf("DropDirt\n");
			}
		SV_SendPackets();
		}
	///////////////////////////////////////////*/

	// Make the ground fall
	ground.fall();

	// Water particle thinker
	StaticWaterThinker();

//**************************************************
	// All system setup and ready to go.
	// Now we start the action!

	CheckTankDeaths(); // also gets new turn if required

	// Action all done so we update the dynamic parts
	// like timeouts tanks shots water etc.
//**************************************************

	// Run the shotclock for multiplayer
	Tank_ShotClockTicker();

	// Run thinkers for all the players in the game
	TankThinker();

	// Missile thinker
	ShotThinker();

	// Water particles, napalm...etc
	ParticleThinkers();

	// Blit all to screen
	LoopDrawer(buffer);
}
