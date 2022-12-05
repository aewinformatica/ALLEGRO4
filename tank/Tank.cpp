#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"
#include "dialogs.h"
#include "fblend.h"


//	external routines
void			Maggen_Decay(void);

// Show stats for who now?
int			tank_showstats=-1;

// Is it time to redraw the HUD?
int			checktanktimer;
bool		hudzap=true;

// Shot clock
int			tank_shottimer;
int			tank_orginalshottimer;

int			tankturnque[MAX_TANKS];
int			tankturnnextque[MAX_TANKS];

char		*kill_text[MAX_KILL_TEXT] = {
	"Killing is my business, and business is good!",
	"Who's your daddy?",
	"Victory is my first, middle and last name",
	"Come back! I'm not done PWNING j00!",
	"Guns don't kill people, nukes kill people",
	"OMG I R0X0RZ J000!!!!111one",
	"Another one bites the dust",
	"Boom baby!",
	"Who forgot their shields this morning?",
	"I guess God can sort you out now...",
	"So long, SUCKA",
	"Say hello to my little friend...",
	"You are a waste of pixels",
	"Suck it up!",
	"I guess we know who has the bigger bomb",
	"Yup, yup, yup",
	"How you like dem apples?",
	"Mo money, mo money, mo money",
	"Scratch that as another kill...",
	"Worthless",
};


char		*killed_text[MAX_KILLED_TEXT] = {
	"Whatever...",
	"Don't fear the reaper!",
	"Must...rethink...PLAN!",
	"No fair cheating!",
	"I have no regrets!",
	"*dramatic gasp*",
	"Well isn't that just thspeshil",
	"You want war, I give you war",
	"I tried...  I failed...  You conquered",
	"Bah, try me next round",
	"Can I have your autograph?",
	"Make the hurting stop",
	"Oh the pain, the pain THE PAIN!",
	"You haven't seen the last of me!",
	"Note to self: Buy better weapons",
	"I'm alluding you into a false sense of security",
	"Oww..",
	"I thought you were going to play nice",
	"I shall have my revenge!",
	"Ouch"
};


char		*selfkill_text[MAX_SELFKILL_TEXT] = {
	"All this angle and power changing, sure is tricky",
	"What goes up must come down",
	"I've calculated the power and angle perfectly so that...DOH!",
	"Genius has limits, stupidity does not",
	"So that's what that buttons does...",
	"Goodbye cruel world",
	"Me = N00B!",
	"Meant to do that!",
	"No comment",
	"I hope my warranty covers this",
	"Mamma said there be days like this",
	"Mooo",
	"All part of my master plan",
	"I'll get You for making me do this",
	"Me have sleep & be ready for next round.",
	"!!!!!!!!!!!!!!",
	"??????????????",
	"Oh bother!",
	"I'll be back!!!!",
	"Ouch"
};


char		*fallkill_text[MAX_FALLKILL_TEXT] = {
	"Boy, this sucks",
	"PA-RA-CHUTE!",
	"Down I go!",
	"Go, go Gadget Chopper! Waaait a minute...",
	"Falling is easy, its the stopping that hurts",
	"Aiiieeeee",
	"Tanky go down the hooooole",
	"I've should've gotten the air tank package",
	"Vinnie's Tank-O-Rama: Parachutes, 20%% off!",
	"My boss is gonna kill me",
	"Gravity is such a harsh mistress",
	"Up, up and AWAY!",
	"Oh dear",
	"Anybody hear that strange crunch noise?",
	"Going down",
	"Wheeeeee",
	"Rubber treads don't help!!!!",
	"!!!GAME ERROR!!!    This does not happen to me!",
	"Hey who took the dirt away?????",
	"Ouch"
};


void TANK::FindGoodXPOS(void)
{
	float newx;
	
	// Left direction
	if (index % 2)
	{
		newx=rand()%SCREENWIDTH/2;
		angle=deg=100;
	}
	else
	{
		// Right direction
		newx=rand()%SCREENWIDTH/2;
		newx+=SCREENWIDTH/2;
		angle=deg=30;
	}

	if (newx<sizex) newx=sizex;
	if (newx>= SCREENWIDTH-sizex) newx=SCREENWIDTH-sizex;

	x=(int)newx;

	// Make sure we stay on the screen (prevents hangups when moving tanks)
	if (x <= 10)
	{ x = 10; }
	if (x >= (SCREENWIDTH-14))
	{ x = SCREENWIDTH-14; }
		
}

void TANK::DefaultAngle(void)
{
	
	if (x < (SCREENWIDTH/2))	// Left direction
		deg=75;
	else						// Right direction
		deg=55;
}


void TANK::init(int i, int a, int b, int d, int e,int f, int c)
{     
	indexnum = i;		    
	sizex = 20;
	sizey = 10;
	lpower = 9;

	xvel = 0;
	yvel = 0;
		
	// No additional cheats/attributes
	flags = 0;
	specialflags = TANKSPECIAL_NONE;	
  exist=10;

	// health is rounded so player keeps most of bonus health points
	// it is rounded to a default increment (default was 100)
  starthealth = DEFAULT_TANK_HEALTH*(int)((health+DEFAULT_TANK_HEALTH/2)/DEFAULT_TANK_HEALTH);
	if(starthealth < DEFAULT_TANK_HEALTH) starthealth = DEFAULT_TANK_HEALTH;  //  lower limit
	if(starthealth > 5*DEFAULT_TANK_HEALTH) starthealth = 5*DEFAULT_TANK_HEALTH;  //  upper limit
	//  assuming DEFAULT_TANK_HEALTH=100 then starthealth is now 100 or 200 or 300 or 400 or 500  
	health = starthealth;  // this it the maximum for this round, excluding new bonus points.


	// Set default tank state
	tankstate = TANKSTATE_PLAYING;

	// Clear out shield defenses
	shieldh = shielde = shieldb = 0;

	fallenpixels=0;
	healthbartrans = 0;
	energyleft = 0;
	activedefense=-1;
	targettank = -1;
	targetpower = -1;
	targetangle = -1;
	needstodie = 0;
	frozen = 0;
	falling = false;
	moving = false;
	pingwaitReady = true;

	lastweaponused = -1;
	deathseq = -1;
	deathtics = -1;

	lastinflictor = -1;

	spectator = 0;
	pingwait = 0;

	// Make sure some sprite pointers are clear
	shield = NULL;

	bmp = create_bitmap(21,14);

	// Setup the cannon sprite
	if(barreloutline)  // barrel outline setup in front menu.
		{
		cannon = create_bitmap(characters[character].cannonlength*2,CANNON_W+1);// zaq1 mod to outline cannon
		clear_to_color(cannon, TRANSCOLOR);
		rectfill(cannon,0,0,characters[character].cannonlength,CANNON_W+1,CANNON_C);// outline color
		rectfill(cannon,0,1,characters[character].cannonlength,CANNON_W-1,makecol(colorr,colorg,colorb));
		}
	else
		{
		cannon = create_bitmap(characters[character].cannonlength*2,CANNON_W);
		clear_to_color(cannon, TRANSCOLOR);
		rectfill(cannon,0,0,characters[character].cannonlength,CANNON_W,CANNON_C);
		//drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		//set_trans_blender(colorr, colorg, colorb, TANK_OPACITY);
		rectfill(cannon,0,0,characters[character].cannonlength,CANNON_W,makecol(colorr,colorg,colorb));
		}
	solid_mode();
}

//
// Draw - Draw the tank onto the screen
//

void TANK::draw(BITMAP *b)
{
	BITMAP		*flare = (BITMAP *)maindata[TANK_MUZZLEFLARE].dat;
	int		barrelx;
	float	tankx,tanky,px,py;
	int		rectx,recty,rectw,recth;
	int		tdrawx,tdrawy;

//		top left corner (rectx,recty)
	rectx = tankx = (int)(x-(sizex/2));
	recty = tanky = (int)(y-(sizey/2));
	rectw = sizex;
	recth = sizey;

	barrelx = tankx+characters[character].cannonxpos;

	// Set some varibles
	tdrawx = (x-(sizex/2));
	tdrawy = (y-(sizey/2));

	// Draw the tank shadow before any other drawing is done
	DrawBitmapGroundMask((BITMAP *)maindata[TANK_SHADOW].dat, b, (int)(tankx-2), (int)y+10, 75);
	
	clear_to_color(bmp, TRANSCOLOR);

	// Draw the parachute behind the tank
	if (flags & TANKFLAG_PARACHUTE)
	{
		BITMAP	*parachute = (BITMAP *)maindata[TANK_PARACHUTE].dat;

		px = (float)(x - (parachute->w/2));
		py = (y+(sizey/2)-parachute->h);

		// Dirty rectangles
		AddRectangle(b, (int)px, (int)py, parachute->w, parachute->h);

		draw_sprite(b, parachute, (int)px, (int)py);
	}

	set_trans_blender(colorr, colorg, colorb, 0);
	if (teamplay->value > 0)
		draw_lit_sprite(bmp, (BITMAP *)maindata[characters[character].smalltank].dat, 0, 0, 100);
	else
		draw_lit_sprite(bmp, (BITMAP *)maindata[characters[character].smalltank].dat, 0, 0, TANK_OPACITY);

	if (deg <= 64)
	{ 
		//barrelx = (tankx+sizex)-characters[character].cannonxpos;

		// add dirty rectangle
		AddRectangle(b, tdrawx-45, tdrawy-45, 80, 80);

		if (!(flags & TANKFLAG_PARACHUTE))
			r_s(b,cannon,(int)(barrelx),(tdrawy+characters[character].cannonypos)+3,deg); 
	
		// Draw the tank
		draw_sprite(b, bmp, tdrawx, tdrawy+3); 
	}
	else
	{ 
		barrelx = (tankx+sizex)-characters[character].cannonxpos;

		// add dirty rectangle
		AddRectangle(b, tdrawx-40, tdrawy-40, 80, 80);

		if (!(flags & TANKFLAG_PARACHUTE))	// if floating don't rotate_sprite for cannon
			r_s(b,cannon,(int)(barrelx),(tdrawy+characters[character].cannonypos)+3,deg); 

		// Draw the tank
		draw_sprite_h_flip(b, bmp, tdrawx, tdrawy+3); 
	}

	// Frozen tank in nitrogen?
	if (frozen > 0)
	{
		BITMAP	*iceblock = (BITMAP *)spritedata[ICEBLOCK].dat;
		//float	opacity = ((float)frozen/(float)255);
		int		opacity = frozen;
		if (opacity > 255)
			opacity = 255;

		fblend_trans(iceblock, b, x-(iceblock->w/2), y-(iceblock->h/2), opacity);
		
	}


	// Draw the float under the tank
	if (flags & TANKFLAG_FLOATATION)
	{
		BITMAP	*floatation = (BITMAP *)maindata[TANK_FLOATATION].dat;

		px = (float)(x - (floatation->w/2));
		py = (y+10);

		AddRectangle(b, px, py, floatation->w, floatation->h);
		draw_sprite(b, floatation, (int)px, (int)py);
	}

	if (tankstate == TANKSTATE_DEFENSE)
		textout_centre(b, font, "D", x, y-25, makecol(255,255,255));

	// The 'L' keys shows a laser dot
	if (specialflags & TANKSPECIAL_LASERSIGHT)
		LaserSight(b);

	DefenseDrawer(indexnum);



	if (flags & TANKFLAG_MONKEYMORTAR)
	{
		BITMAP	*mbase = (BITMAP *)spritedata[MORTARBASE].dat;
		BITMAP	*bar = (BITMAP *)spritedata[MORTARBARREL1].dat;
		int		mx,my;

		if (flaretics)
		{
			flaretics --;	//	MORTARBARREL2 has built in flare
			bar = (BITMAP *)spritedata[MORTARBARREL2].dat;
		}

		my = ((y+sizey)-mbase->h)+3;

		if(deg <= 64)
			{
			mx = (x-mbase->w)+(sizex/2);
			AddRectangle(b, mx-55, my-55, mbase->w+55, mbase->h+55);

			// Drawing of the actual barrel
			pivot_sprite(b, bar, mx+(29-(sizex/2)), my+5, 53, 8, ftofix(deg));

			draw_sprite_h_flip(b, mbase, mx, my);
			}
		else
			{
			mx = x-(sizex/2);

			AddRectangle(b, mx-55, my-55, mbase->w+100, mbase->h+100);

			// Drawing of the actual barrel
			pivot_sprite(b, bar, mx+29, my+5, 53, 8, ftofix(deg));

			/*putpixel(b, ((mx+29)-COS(deg)*25),
					(my+5-SIN(deg)*25),
					makecol(255,0,0));
					*/

			draw_sprite(b, mbase, mx, my);
			}
	
	}
}


int TANK::CheckFallHeight(int x,int y, int direction)
{
    int a,b;
    int s=0, col;


    a=(int)(x+sizex*direction/2);
	b=y+sizey;

	while ((col=getpixel(ground.bmp,a,b++)) == TRANSCOLOR && b < SCREENHEIGHT) 
	{ 
		s++; 
	}


    return(s);
}

int TANK::CheckSteepness(int x,int y, int direction)
{
    int a,b;
    int s=0, col;


    a=(int)(x+sizex*direction/2);
	b=y+sizey;

	while ((col=getpixel(ground.bmp,a,b--)) != TRANSCOLOR && b >= 0) 
	{ 
		s++; 
	}


    return(s);
}


int TANK::move(float a, float b)
{
	int s;
	float c,d;

	c=x+a;
	d=y+b;

	s=CheckSteepness((int)c,(int)d,SGN(a));
	if (s>10) 
		return -1;

	if (c+(sizex/2) < SCREENWIDTH && (c-sizex/2) > 0) 
		{ x=c; }

	if (((d-s)+sizey/2) >= SCREENHEIGHT || 
	 ((d-s)+sizey/2) <= 0)
		return 1;
	
	y=d-s;
	return 1;
}


void TANK::load(void)
{
	return;
}

void TANK::GiveMoney(int amount)
{
	money += amount;
}

void TANK::GivePoints(int amount)
{
	score += amount;

	// Dont let their score go below 0
	if (score < 0) { score = 0; }
}


//
// TankThinker - Special routines that are not controlled
// directly by the player
//

void TANK::TankThinker(void)
{
	float	shieldopacity;
	int		tankx,tanky;

	// First, calculate potential shot X and Y coords
	tankx = x-(sizex/2);
	tanky = y-(sizey/2);
	barrelshoty = (tanky+characters[character].cannonypos-SIN(deg)*characters[character].cannonlength);
	if (deg <= 64)
		barrelshotx = (tankx+characters[character].cannonxpos)-COS(deg)*characters[character].cannonlength;
	else
		barrelshotx = ((tankx+sizex)-characters[character].cannonxpos)-COS(deg)*characters[character].cannonlength;

	//putpixel(buffer, barrelshotx, barrelshoty, makecol(255,0,0));

	// Bubble ticker, do not spawn ambient bubbles until this is 0
	if (bubbleticker > 0)
		bubbleticker--;

	if (healthbartrans > 0)
	{
		healthbartrans -= 5;
		if (healthbartrans < 0) healthbartrans = 0;
	}

	// Did we take a dive?
	if (Underwater(x,y))
	{
		if (!underwater)
		{ G_SpawnSprite((int)x, (OnWater((int)x,(int)y)-10), S_SPLASH1, (MF_NORMAL | MF_CENTERCOORD)); }

		underwater = true;


		// With the insanley random number generator in the bubbleticker, the chances
		// for a ambient bubble spawn is VERY slim! (the way we like it!)
		if (rand()%(TICRATE*5) <= 1 && bubbleticker <= 0)
		{
			int		bubble=rand()%3;

			if (bubble == 0)
				G_SpawnSprite((int)x, (int)y, S_BUBBLE_SMALL, (MF_NORMAL | MF_CENTERCOORD));
			else if (bubble == 1)
				G_SpawnSprite((int)x, (int)y, S_BUBBLE_MEDIUM, (MF_NORMAL | MF_CENTERCOORD));
			else
				G_SpawnSprite((int)x, (int)y, S_BUBBLE_LARGE, (MF_NORMAL | MF_CENTERCOORD));

			bubbleticker = 5*TICRATE;
		}


		// This will never actually damage the tank, the point of this is to
		// take away a parachute from inventory if one was used to float to
		// the water.
		ApplyFallingDamage();
	}
	else
		underwater = false;


	// Check for damage
	//if (!(gametics % 2))
	{
		sprite_t		*s;
		particlenode_t	*p;
		int				damagechance = (health/10);
		int				flags = (MF_NORMAL | MF_USEANGLE | MF_TRANSLUC);
		int				smoke,i;
		int				tankx = (int)(x-sizex);
		int				tanky = (int)(y-sizey/2);

		// Only spawn smoke if our HP is less than or equal to 50
		if (damagechance <= 5)
		{

			tankx += rand()%(sizex*2);
			tanky += rand()%(sizey*2);

			if (damagechance < 1)
				damagechance = 1;

			if (rand()%(TICRATE*damagechance) <= 6-damagechance)
			{
				smoke = rand()%2;

				if (smoke==0)
					s = G_SpawnSprite((int)tankx, (int)tanky, S_SMOKE_BLACK_MEDIUM, flags);
				else
					s = G_SpawnSprite((int)tankx, (int)tanky, S_SMOKE_BLACK_SMALL, flags);

				// Is the damage enough to generate sparks?
				if (3-damagechance > 0)
				{
					for (i=0; i<4-damagechance; i++)
					{
						p = pdirt->InsertParticle(x, y, rand()%255, ConvertFromGamePower(60), makecol(255,245,0), 30+rand()%10, PFLAGS_DIRT|PFLAGS_TANKSPARK, 0, 0);
						p->bounce = 5;
					}
				}

				// Set opacity (or translucency)
				s->opacity = 128;
			}
		}
	}

	// Make sure if we have a shield, it always follows
	// us
	if (energyleft > 0 && shield)
	{
		//shield->x = x;
		//shield->y = y;

		// Calculate opacity for the amount of damage
		// the shield has taken
		shieldopacity = ((float)energyleft/(float)defenses[activedefense].energy)*255;
		shield->opacity = (int)shieldopacity;
	}
}

void TANK::SetOldVars(void)
{
	// Setup some old varibles for tracking
	oldx = x;
	oldy = y;
	oldhealth = health;
	oldlpower = lpower;
	olddeg = deg;
	oldactiveweapon = activeweapon;
	oldflags = flags;
	oldspecialflags = specialflags;
	oldenergyleft = energyleft;
	oldfuel	= specials[fuel].amount;
	oldtankstate = tankstate;
}



void TANK::handleinput(void)
{

	// Accept no input if the console or menu is active
	if (console_active || menuactive)
		return;

	// Do not accept input if other players are running autodefense
	if (multiplayer && (specialflags & TANKSPECIAL_AUTODEFENSEWAIT))
		return;

	// Do not accept input if the player has already taken his turn
	if (specialflags & TANKSPECIAL_MADESHOT)
		return;

	if (IsTankMessage() > 0 &&
		tankstate == TANKSTATE_PLAYING)
		return;

	// Only human players can set their angle/power manually
	if (skill != TANKSKILL_HUMAN || 
		tankstate != TANKSTATE_PLAYING)
		return;

	// If this is a multiplayer game, do not allow any
	// control if this isn't our turn
	if (multiplayer && (indexnum != net_localplayer))
		return;

	// Don't allow input if there is no-one else present
	if (numtanks() <= 1)
		return;

///	========================= keyboard input ====================
if(usekeyboard)
	{
	if(key[KEY_LEFT]) 
		{ 
		if(deg > 0)
			deg-=0.5;
		}

	if(key[KEY_RIGHT])
		{
		if(deg < 128)
			deg+=0.5;
		}

	if(key[KEY_A] && specials[fuel].amount > 0)
		{
		if(!multiplayer || multiplayermode == MP_SERVER)
			{
			if(move(-1,0) > 0)
				{
				specials[fuel].amount--;
				// Track this special item as being used
				specialsused[fuel]++;
				}
	
			// We no longer need to keep track of who shot us last.
			// If this tank falls to its death, its our fault.
			lasttankturn = lasttankshot = -1;
	
			// We're moving
			moving = true;
			}
		}

	if(key[KEY_S] && specials[fuel].amount > 0) 
		{
		if(!multiplayer || multiplayermode == MP_SERVER)
			{
			if(move(1,0) > 0)
				{
				specials[fuel].amount--;
				// Track this special item as being used
				specialsused[fuel]++;
				}
			moving = true;
			lasttankturn = lasttankshot = -1;
			}
		}

	// If neither are being pressed, we're not moving
	if(!(key[KEY_S]) && !(key[KEY_A]))
		moving = false;

	if(key[KEY_UP])
		{
		lpower+=0.02;
		if (lpower>MAX_LOAD) lpower=MAX_LOAD;
		}

	if(key[KEY_DOWN])
		{
		lpower-=0.02;
		if (lpower <= 0) lpower = 0;
		}
	if(key[KEY_PGUP])
		{
		lpower+=0.1;
		if(lpower>MAX_LOAD) lpower=MAX_LOAD;
		}
	if(key[KEY_PGDN])
		{
		lpower-=0.1;
		if (lpower <= 0) lpower = 0;
		}

	if(IsKeyDn(KEY_SPACE))
		shoot();

	// Check for switching of the weapons (and TAB too! For Jesse)
	if(IsKeyDn(KEY_TAB)) CycleWeapons();
	// Also use pad + and - to cycle
	if(IsKeyDn(KEY_PLUS_PAD)) CycleWeapons();
	if(IsKeyDn(KEY_MINUS_PAD)) CycleWeaponsReverse();

	// The 'T' key, like in the orginal scorched earth will
	// also bring up the player defense box
	if(IsKeyDn(KEY_T))
		StartDefenseBox(indexnum);

	if(IsKeyDn(KEY_Q))
		{ 
		StartWeaponsList(indexnum); 
		}
		
	}

///	========================= mouse input ====================
	if(mouse_x > 0 && mouse_x <= 135 &&
		mouse_y > 0  && mouse_y <= 27 &&
		mouseflags & MOUSE_FLAG_LEFT_DOWN)
			{
			StartDefenseBox(indexnum);
			}

	if(mouse_x > 683 && mouse_x <= SCREENWIDTH &&
		mouse_y > 0 && mouse_y <= 128)
		{ 
		if(mouseflags & MOUSE_FLAG_LEFT_DOWN)
			{
			StartWeaponsList(indexnum);
			mouseflags = 0;
			}
		}

 	if(mouse_x > 105 && mouse_x <= 125 &&
		mouse_y > 30  && mouse_y <= 50 &&
		mouseflags & MOUSE_FLAG_LEFT_DOWN)
			{
			hudzap = false;
			// Refresh the entire screen after doing this
			ground.needsupdate = true;
			}
 	if(mouse_x > 0 && mouse_x <= 30 &&
		mouse_y > 570 && 
		mouseflags & MOUSE_FLAG_LEFT_DOWN)
			{
			hudzap = true;
			// Refresh the entire screen after doing this
			ground.needsupdate = true;
			}

	// Mouse aiming is considered a "cheat" with straight line
	// weapons (aka. Ion Cannon)
	if(!multiplayer)
		if(key[KEY_M] && (mouseflags & MOUSE_FLAG_LEFT_DOWN) )
			{
			deg = CalculateAngle(x, y, mouse_x, mouse_y);
			if(deg < 0)	deg = 0;
			if(deg > 128)	deg = 128;
			}

///	========================= joystick input ====================
	// Special joystick stuff
	if (usejoystick)
		{
		// Moving towards the left
		if (joy[0].stick[0].axis[0].pos/40 < 0) 
			{
			if (deg > 0)
			deg-=0.5;
			}
			

		// Moving toward the right
		if (joy[0].stick[0].axis[0].pos/40 > 0) 
			{
			if (deg < 128)
				deg+=0.5;
			}
			

		// Analog up (power up)
		if (joy[0].stick[0].axis[1].pos/40 < 0) 
			{
			lpower+=0.02;
			if (lpower>MAX_LOAD) 
				lpower=MAX_LOAD;
			}


		// Analog down (power down)
		if (joy[0].stick[0].axis[1].pos/40 > 0) 
			{
			lpower-=0.02;
			if (lpower <= 0) 
				lpower = 0;
			}

		// Fire!
		if (IsJoyButtonDn(0, 0))
			shoot();

		// Cycle weapons forward
		//if (joy[0].button[2].b)
		if (IsJoyButtonDn(0, 2))
			CycleWeapons();

		// Cycle weapons backward
		//if (joy[0].button[1].b)
		if (IsJoyButtonDn(0, 1))
			CycleWeaponsReverse();

		// Defense box
		if (IsJoyButtonDn(0, 3))
			StartDefenseBox(indexnum);

		// Move your tank left
		if (joy[0].button[6].b && specials[fuel].amount > 0)
			{
			if (move(-1,0) > 0)
				specials[fuel].amount--;

			// We no longer need to keep track of who shot us last.
			// If this tank falls to its death, its our fault.
			lasttankturn = lasttankshot = -1;

			// We're moving
			moving = true;
			}

		// Move your tank right
		if (joy[0].button[7].b && specials[fuel].amount > 0)
			{
			if (move(1,0) > 0)
				specials[fuel].amount--;

			moving = true;
			lasttankturn = lasttankshot = -1;
			}
	}
///	========================= end input ====================

	// Shooting the second part of the twin electric lances? Make sure the 
	// weapon does not change
	if (specialflags & TANKSPECIAL_MADETELPOSITIVE)
		{
		activeweapon = W_TWINELECTRICLANCES_NEGATIVE;
		weapons[activeweapon].ammo = 1;
		}
}



//
// CommonPlayerInput - Keys and mice clicks that are processed reguardless of who is playing
// or whos turn it is. (Eg. ESC key for menus, mouse hover for stats...etc)
//

void	CommonPlayerInputDrawer(BITMAP *b)
{
	int		i;
	int		tankx;
	int		tanky;

	// See if the mouse is hovering over any tanks
	if (gamestate == GS_INGAME)
	{
		for (i=0; i<MAX_TANKS; i++)
		{
			if (ta[i].health <= 0)
				continue;

			tankx = ta[i].x - (ta[i].sizex/2);
			tanky = ta[i].y - (ta[i].sizey/2);

			if (GUI_MouseInside(tankx, tanky, tankx+ta[i].sizex, tanky+(ta[i].sizey*2)))
			{ 
				// Dont draw this twice
				if (i != tankturn)
				{
					ta[i].DrawNameBox(b);
					ta[i].DrawPowerEnergy(b,255);
				}

				if (mouse_b & 1)
					tank_showstats = i;
				return;
			}
		}

		if (mouse_b & 1)
			tank_showstats = -1;
	}


	// Make sure the mouse stays within the boundries of the tank when viewing
	// stats
	if (tank_showstats >= 0)
	{
		tankx = ta[tank_showstats].x - (ta[tank_showstats].sizex/2);
		tanky = ta[tank_showstats].y - (ta[tank_showstats].sizey/2);

		if (!GUI_MouseInside(tankx, tanky, tankx+ta[tank_showstats].sizex, tanky+(ta[tank_showstats].sizey*2)))
		{
			tank_showstats = -1;
			return;
		}
	}
}

void TANK::shoot(void)
{
  int i, flags = (MF_NORMAL | MF_CENTERCOORD | MF_USEANGLE);
	int	length = characters[character].cannonlength+(characters[character].cannonlength/2);

	ground.needsupdate = true;	// in case of TWINELECTRICLANCES

	// Has this tank already used its turn? If so, go no further until
	// we recieve word from the server
	if (specialflags & TANKSPECIAL_MADESHOT)
		{
#ifdef	_DEBUG
		Printf("shoot() TANKSPECIAL_MADESHOT flag still set on %d\n", indexnum);
#endif
		SetTankTimer(2);	//	try this to shut this turn down
		return;
		}

	//	no shooting if no ammo
	if (weapons[activeweapon].ammo <= 0)
		{
		// error must have occured to get here!
		//weapons[activeweapon].ammo = 0;
		G_Printf("ERROR: tank(%d) shooting with no ammo", indexnum);
		StartSound(WOLFHOWL);                      
		CycleWeaponsReverse();
		//return;
		}

    // Find a shot index
	i=findshot();
  if (i < 0) 
		{
#ifdef	_DEBUG
		Printf("shoot() Cannot find a shot array for %d\n", indexnum);
#endif
		return;
		}

	// Shooting the second part of the twin electric lances?
	// Automatically set the weapon
	if (specialflags & TANKSPECIAL_MADETELPOSITIVE)
		{
		activeweapon = W_TWINELECTRICLANCES_NEGATIVE;
		weapons[activeweapon].ammo = 1;
		specialflags &= ~TANKSPECIAL_MADETELPOSITIVE;
		}

	// Shooting the first half of the TEL super? (I hate custom code like this...)
	if (activeweapon == W_TWINELECTRICLANCES)
	{
		specialflags |= TANKSPECIAL_MADETELPOSITIVE;

		// Increase time for a shot
		tank_shottimer += 10*TICRATE;
		if (tank_shottimer > (int)shotclocktime->value)
			tank_shottimer = (int)shotclocktime->value;
	}

	// Players can only shoot 1 tracer per turn
	if (activeweapon == W_TRACERS)
	{
		// Only 1 tracer shot per turn
		if (specialflags & TANKSPECIAL_USEDTRACER)
			return;

		// Apply the flag so that two tracers cannot be shot on the same
		// turn		--  except for debug mode.
#ifndef	_DEBUG
		specialflags |= TANKSPECIAL_USEDTRACER;
#endif

		// Increase time for a shot
		tank_shottimer += 10*TICRATE;
		if (tank_shottimer > (int)shotclocktime->value)
			tank_shottimer = (int)shotclocktime->value;
	}
	//	===================================================
	//	Cleared the jumps so lets fire the shot
	//	===================================================

	// Apply flaretics (tics that determine how long a tank barrel has a muzzle flash)
	if (activeweapon != W_SHOTGUN)
		SetFlareTics(2);

    // Peel 5 points away for shooting, its all about skill....
	score -= 5;

	// ...but dont go below 0...its really hard to get back
	if (score <= 0) { score = 0; }

	// Keep track of the last weapon shot (until the tank moves)
	lasttankshot = activeweapon;

	// Keeps perminent track of the last weapon used
	lastweaponused = activeweapon;

///		N.B.		Cannot set activeweapon to zero before CTOS_MADESHOT
///						else server won't accept the shot
///		move to end of routine
///	// Also, slice some ammo away (unless its unlimited)
///	if (weapons[activeweapon].ammo != UNLIMITED_AMMO)
///		weapons[activeweapon].ammo--;

	sh[i].index = i;
	sh[i].tankfrom = indexnum;
	sh[i].Init(i, activeweapon,
				barrelshotx,
				barrelshoty,
				(-lpower*COS(deg)),
				(lpower*-SIN(deg)),
				deg);

	sh[i].origdeg = deg;
	sh[i].origx = x;
	sh[i].origy = y;
	sh[i].origxvel = sh[i].xvel;
	sh[i].origyvel = sh[i].yvel;
//	sh[i].life = 500;
	sh[i].bouncecount = 0;

	if (projectiles[activeweapon].firesound >= 0)
	{ sh[i].StartSound(projectiles[activeweapon].firesound); }

	if (states[projectiles[activeweapon].state].flags & HAS_ALPHA)
	{ flags |= MF_ALPHA; }

	// Bind a sprite to our shot
	sh[i].sprite = G_SpawnSprite((int)x,(int)y, projectiles[activeweapon].state, flags);

	// Find a net Id
	lastshotnetid = sh[i].sprite->netid = G_GetNetID();

	// If the tank was using laser sight, disable it now
	if (specialflags & TANKSPECIAL_LASERSIGHT)
		specialflags &= ~TANKSPECIAL_LASERSIGHT;
	
	// Set the "made shot" flag indicating that this player
	// has already fired and to NOT allow another shot while waiting
	// for information from the server
	if(activeweapon != W_TRACERS &&
		activeweapon != W_TWINELECTRICLANCES)
		{
		specialflags |= TANKSPECIAL_MADESHOT;
		}

	// Make this weapon as being used
	weaponsused[activeweapon]++;

	// Increase the "number of shots" in the round. So we know to reset the
	// level if everyone leaves in multiplayer
	currentlevel.numshotsfired++;

	// Wait at least 10 tics (or for however many it takes) for all missiles to
	// land
	SetTankTimer(10);

	Printf("Shoot A=%.1f P=%.2f W=%s \n", deg, lpower, projectiles[activeweapon].name );	// Degauss debug

	// Send tracer info?
	if(multiplayer)
		{
		// Send general tank info down
		if(multiplayermode == MP_SERVER)
			{
			for(i=0; i<MAX_TANKS; i++)
				{
				if(ta[i].exist <= 0 || 
					i == net_localplayer || 
					ta[i].skill != TANKSKILL_HUMAN ||
					i == indexnum)
					{ continue; }

				ta[i].outbuf->WriteByte(STOC_MADESHOT);
				ta[i].outbuf->WriteByte(indexnum);
				ta[i].outbuf->WriteByte(deg);
				ta[i].outbuf->WriteFloat(lpower);
				ta[i].outbuf->WriteByte(lastweaponused);
				ta[i].outbuf->WriteLong(weapons[lastweaponused].ammo);
				}
			}

		if (multiplayermode == MP_CLIENT &&
				 indexnum == tankturn)
			{
			localbuf->WriteByte(CTOS_MADESHOT);
			localbuf->WriteByte(deg);
			localbuf->WriteFloat(lpower);
			localbuf->WriteByte(lastweaponused);
			localbuf->WriteLong(weapons[lastweaponused].ammo);
			}
		}
//	
//	N.B.		Cannot set to zero before CTOS_MADESHOT else server
//					won't accept the shot

// Also, slice some ammo away (unless its unlimited)
	if (weapons[activeweapon].ammo != UNLIMITED_AMMO)
		weapons[activeweapon].ammo--;

//   20April2006
//  'CycleWeapons()' modifies 'activeweapon' which is used by DrawHUD to display
//  the active weapon on the screen thus showing the wrong missile at various times.
//  Removing the next two lines of code just leaves the missile display at zero.
//  Bots update their weapon selection every time anyway but the player must
//  select a new weapon before they can fire when this occurs. 
//	If no more ammo is left, cycle the weapons
//	if (weapons[activeweapon].ammo <= 0)
//	CycleWeapons();

}


bool	CheckPointInSprite(sprite_t *s, int x, int y)
{
	BITMAP		*sbmp;
	int			bmpx;
	int			bmpy;
	int			sx,sy;
	int			color;

	// Set the bitmap
	sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);

	// Setup clipping coords
	sx = s->x;
	sy = s->y;	
	// New coordinate if this is centered
	if(s->flags & MF_CENTERCOORD)
		{
		sx = s->x - (sbmp->w/2);
		sy = s->y - (sbmp->h/2);
		}


	// First, see if we're inside the bounding box
	if(x > (sx) &&
		 x < (sx+sbmp->w) &&		///		 x < (x+sbmp->w) &&
		 y > (sy) &&
		 y < (sy+sbmp->h))			///		 y < (y+sbmp->h))
		{
		// Great, figure out if we're hitting the figure or
		// still cruising through transparency
		bmpx = x - sx;
		bmpy = y - sy;

		// We have a hit
		color = getpixel(sbmp, bmpx, bmpy);
		if(color != TRANSCOLOR && color != -1)
			return true;
		}

	return false;
}



sprite_t* CheckForSpriteXY(int x, int y)
{
	sprite_t*	s = sprites;
	//BITMAP		*sbmp;
	//int			sx,sy;

	// Now clip against any sprites that are shootable
	while (s)
	{
		if (!(s->flags & MF_SHOOTABLE) || 
			OnScreen(ground.bmp, x, y) == -1)
		{
			s = s->next;
			continue;
		}

		// Set our clipping bitmap
		//sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);

		
		// Check pixel perfect collision
		if (CheckPointInSprite(s, x, y))
		{
			// Return the pointer
			return s;
		}

		s = s->next;
	}

	return NULL;
}


sprite_t* CheckForSprites(int shot, int x, int y)
{
	sprite_t*	s = sprites;
	BITMAP		*sbmp;
	//int			sx,sy;

	// We're not supposed to clip any sprites at all
	if (sh[shot].flags & MISSILE_NOSPRITECLIP)
		return NULL;

	// Now clip against any sprites that are shootable
	while (s)
	{
		if (!(s->flags & MF_SHOOTABLE))
		{
			s = s->next;
			continue;
		}

		// Set our clipping bitmap
		sbmp = ((BITMAP *)spritedata[states[s->startstate.currentstate].datafilelookup].dat);

		// Setup clipping coords
		//sx = s->x;
		//sy = s->y;	

		// New coordinate if this is centered
		//if (s->flags & MF_CENTERCOORD)
		//{
		//	sx = s->x - (sbmp->w/2);
		//	sy = s->y - (sbmp->h/2);
		//}

		// Check pixel perfect collision
		if (CheckPointInSprite(s, x, y))
		{
			// Make the missile explode
			if (!projectiles[sh[shot].projectile].explodefunc)
				sh[shot].explode();

			// Return the pointer
			return s;
		}

		s = s->next;
	}

	return NULL;
}



int CheckForShots(int	shot)
{
	int ii;


	// This may be a little confusing
	//shotbmp = (BITMAP *)spritedata[states[sh[shot].sprite->startstate.currentstate].datafilelookup].dat;

	// Clip against any tanks
	for(ii=0; ii<MAX_TANKS; ii++)
		{
		if(!ta[ii].exist || ta[ii].health <= 0)
			continue;
                         
		// TANK DIES!!!11 (newbie type)  =)
		if(sh[shot].inside(ta[ii].x-(ta[ii].sizex/2),
			ta[ii].y-(ta[ii].sizey/2),
			ta[ii].x+(ta[ii].sizex/2),
			ta[ii].y+10))
			{ 
			//	don't shoot yourself 
			//	but only if the shell has just left the barrel
			if(ii == sh[shot].tankfrom && sh[shot].shottics < 15)
				{ continue; }

			if(ii == sh[shot].tankfrom && 
				(projectiles[sh[shot].projectile].flags & MISSILE_NOHITSELF))
				continue;

			if(ta[ii].energyleft <= 0)
				{
				sh[shot].x = (float)(ta[ii].x);
				sh[shot].y = (float)(ta[ii].y);
				}

			if(!projectiles[sh[shot].projectile].explodefunc)
				sh[shot].explode();	//	this is a check routine & shouldn't do this-- bad???

			return ii;
			}
    }

	return -1;
}

//
// DrawHUD - Draws player info onto the specified bitmap
//

void TANK::DrawHUD(BITMAP *b)
{
  char	str[128];
	float	realpower = ConvertToGamePower(lpower); //lpower*33.33334;
	BITMAP	*blackbmp = (BITMAP*)maindata[HUD_TOPRIGHTBLACK].dat;
	int		i=0;
	int		hudrightx;
	int		hudrighty;

	// Just in case
	if(gamestate != GS_INGAME)
		return;

	// Create the blackbmp
	/*blackbmp = create_bitmap(((BITMAP*)maindata[HUD_TOPRIGHTBLACK].dat)->w,
		((BITMAP*)maindata[HUD_TOPRIGHTBLACK].dat)->h);
	
	draw_sprite(blackbmp, (BITMAP*)maindata[HUD_TOPRIGHTBLACK].dat, 0,0);
	*/
	/////////////////////////////

	if(multiplayer && ta[net_localplayer].spectator > 0)
		{
		// Set the drawing mode
		AddRectangle(b, 0,545, SCREENWIDTH, 45);
		drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		set_trans_blender(0,0,0,150);
		rectfill(b, 0, 545, SCREENWIDTH, 545+45, makecol(0,0,0));
		solid_mode();

		line(b, 0, 545+3, SCREENWIDTH, 545+3, makecol(255,255,255));
		line(b, 0, (545+45)-3, SCREENWIDTH, (545+45)-3, makecol(255,255,255));

		textout_centre(b,(FONT *)maindata[MS].dat, "Spectator mode", SCREENWIDTH/2, 550, makecol(255,255,0));
		textout_centre(b,(FONT *)maindata[TINY8].dat, "Waiting for next level...", SCREENWIDTH/2, 550+23, makecol(255,255,255));
		}
	else if(multiplayer && (ta[net_localplayer].specialflags & TANKSPECIAL_AUTODEFENSEWAIT))
		{
		// Set the drawing mode
		AddRectangle(b, 0,545, SCREENWIDTH, 45);
		drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		set_trans_blender(0,0,0,150);
		rectfill(b, 0, 545, SCREENWIDTH, 545+45, makecol(0,0,0));
		solid_mode();

		line(b, 0, 545+3, SCREENWIDTH, 545+3, makecol(255,255,255));
		line(b, 0, (545+45)-3, SCREENWIDTH, (545+45)-3, makecol(255,255,255));

		textout_centre(b,(FONT *)maindata[TINY8].dat, "Waiting for autodefense completion", SCREENWIDTH/2, 550+23, makecol(255,255,255));
		}

	// Might as well set the varible here
	angle = (int)(deg*1.4);

	hudleft = (BITMAP *)maindata[HUD_TOPLEFT].dat;
	hudright = (BITMAP *)maindata[HUD_TOPRIGHT].dat;

	hudrightx = (SCREENWIDTH-hudright->w);
	hudrighty = 0;

	// Before we draw the right side of the hud, we have to setup a bitmap with the
	// current missile drawn onto the black background. (with clipping enabled)
	

	// Now, draw the missile
	/*rotate_sprite(blackbmp, 
		(BITMAP *)spritedata[states[projectiles[activeweapon].state].datafilelookup].dat, 
		blackbmp->w/2,
		blackbmp->h/2, 
		itofix(64));
	*/
	
	if(hudzap)
		{
		draw_sprite(b, blackbmp, hudrightx+67, 35);

		// Restore everything back to normal
		//set_clip(b, 0,0,0,0);

		draw_sprite(b, hudleft, 0,0);
		draw_sprite(b, hudright, hudrightx, 0);

		set_trans_blender(colorr, colorg, colorb, 0);
		draw_lit_sprite(b, (BITMAP *)maindata[HUD_TOPLEFT_COLOR].dat, 0, 0, 175);

		// Draw the player's profile (or bots)
		switch(skill)
			{
			case	TANKSKILL_HUMAN:
				draw_sprite(b, (BITMAP *)maindata[characters[character].iconbmp].dat, 26,32); break;
			case	TANKSKILL_STOOPID:
				draw_sprite(b, (BITMAP *)maindata[characters[CHAR_STOOPID].iconbmp].dat, 26,32); break;
			case	TANKSKILL_LOBBER:
				draw_sprite(b, (BITMAP *)maindata[characters[CHAR_LOBBER].iconbmp].dat, 26,32); break;
			case	TANKSKILL_SHOOTER:
				draw_sprite(b, (BITMAP *)maindata[characters[CHAR_SHOOTER].iconbmp].dat, 26,32); break;
			case	TANKSKILL_GUNNER:
				draw_sprite(b, (BITMAP *)maindata[characters[CHAR_GUNNER].iconbmp].dat, 26,32); break;
			case	TANKSKILL_SNIPER:
				draw_sprite(b, (BITMAP *)maindata[characters[CHAR_SNIPER].iconbmp].dat, 26,32); break;
			}
		solid_mode();

		// Angle
		sprintf(str, "%d", angle);
		textout_centre(b,(FONT *)maindata[LCD].dat, str, 224, 16, makecol(64,64,64));	//	add shadow
		textout_centre(b,(FONT *)maindata[LCD].dat, str, 223, 15, makecol(0,0,0));

		// Power
		sprintf(str, "%d", (int)realpower);
		textout_centre(b,(FONT *)maindata[LCD].dat, str, 337, 17, makecol(64,64,64));
		textout_centre(b,(FONT *)maindata[LCD].dat, str, 336, 15, makecol(0,0,0));

		// Ammo
		if(weapons[activeweapon].ammo == UNLIMITED_AMMO)
			draw_sprite(b, (BITMAP *)maindata[UNLIMITED_ICON].dat, 749, 43);
		else
			{
			sprintf(str, "%d", weapons[activeweapon].ammo);
			textout_centre(b,(FONT *)maindata[LCD].dat, str, 767, 44, makecol(64,64,64));
			textout_centre(b,(FONT *)maindata[LCD].dat, str, 766, 43, makecol(192,255,192));
			}

		// Weapon Name
		if(ustrlen(projectiles[activeweapon].gamename) > 0)
			sprintf(str, "%s", projectiles[activeweapon].gamename);
		else
			sprintf(str, "%s", projectiles[activeweapon].name);

		textout_centre(b,(FONT *)maindata[TAHOMA10].dat, str, 741, 6, makecol(64,64,64));
		textout_centre(b,(FONT *)maindata[TAHOMA10].dat, str, 742, 7, makecol(0,0,0));
	
		if(activeweapon == W_TRACERS)	// overwrite tracer name if already used.
			{
			// Only 1 tracer shot per turn
			if(specialflags & TANKSPECIAL_USEDTRACER)
				textout_centre(b,(FONT *)maindata[TAHOMA10].dat, "ONE-SHOT-ONLY", 742, 5, makecol(255,0,0));
			}
 
		// Player Name
		textout_centre(b,(FONT *)maindata[TAHOMA10].dat, name, 63, 4, makecol(64,64,64));
		textout_centre(b,(FONT *)maindata[TAHOMA10].dat, name, 64, 5, makecol(0,0,0));
		}
	else
		{
		char msg[512];

		sprintf(msg, "### %s  Angle:%d  Power:%d  Weapon:%s [%d]",
			name, angle, (int)realpower, projectiles[activeweapon].name, min( weapons[activeweapon].ammo, 999) );
		AddRectangle(b, 0, SCREENHEIGHT-20, 450, 20);
		textout(b, (FONT *)maindata[TINY8].dat, msg, 0  , (SCREENHEIGHT-20)  , makecol(  0,  0,  0) );
		textout(b, (FONT *)maindata[TINY8].dat, msg, 0+1, (SCREENHEIGHT-20)+1, makecol(255,255,255) );
		}
}


//
// DrawNameBox
//

void	TANK::DrawNameBox(BITMAP *b)
{
	int		bx,by,bw,bh;
	char	str[64];

	// Drawing a 105x15 box
	bx = x-(105/2);
	by = y-65;
	bw = 105;
	bh = 40;

	// Add the dirty rectangle first
	AddRectangle(b, bx, by, bw+1, bh+1);

	// Set the drawing mode
	drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	set_trans_blender(0,0,0,150);
	rectfill(b, bx, by, bx+bw, by+bh, makecol(0,0,0));
	solid_mode();


	rect(b, bx+1, by+1, bx+bw-1, by+bh-1, makecol(colorr, colorg, colorb));

	// Name
	textout_centre(b,(FONT *)maindata[TINY8].dat, name, x, y-(65-3), makecol(255,255,255));

	// Money or ping for multiplayer
	if (!multiplayer)
		sprintf(str, "$%d", money);
	else
		sprintf(str, "ping %d (%d)", ping, framedelay);

	textout_centre(b,(FONT *)maindata[TINY8].dat, str, x, y-(45-3), makecol(255,255,128));
	
}

void	DrawPowerEnergyBar(BITMAP *b, int x, int y, 
						   int w, int h, 
						   int opacity,
						   int currenthealth,
						   int startinghealth,
						   int currentenergy,
						   int startingenergy, 
						   int currentfuel,
						   int startingfuel)
{
	int		bx,by,bw,bh;
	int		i, bonus, bonusw, bonusnxt;
	float	barwidth;

	// Keep in mind that we're going to be drawing a 39x9 bitmap
	// directly onto the buffer 'b'
	if(currenthealth > 300)  w=39;  // need to draw large box
	if(w>30)
		{
		w =31;
		bonusw = 5;
		}
	else	//	for small sprites draw small box
		{
		w=16;
		bonusw = 3;
		}
	bonusnxt =bonusw+2;	//	leave a 1 pixel gap ie +2

	bx = x-(w/2);
	by = y;
	bw = w-1;
	bh = h-1;

	if(opacity < 255)
		{
		if(opacity < 0)
			opacity = 0;

		drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		set_trans_blender(0,0,0,opacity);
		}

	// Add the dirty rectangle first
	AddRectangle(b, bx-1, by-1, bw+2, (bh*3)+2);

	if(currenthealth > 500)  currenthealth = 500;  // stop drawing overflow
	if(currenthealth > 100)  // draw bonus points
		{
	  rect(b, bx, by, bx+bw, by+5, makecol(100,100,100));  //  black background
    bonus = bx+1;
		   rectfill(b, bonus, by+1, bonus+bonusw, (by+1)+3, makecol(255,0,0) );bonus +=bonusnxt;currenthealth-=100;  //  100
		if(currenthealth > 100)
			{rectfill(b, bonus, by+1, bonus+bonusw, (by+1)+3, makecol(255,0,0) );bonus +=bonusnxt;currenthealth-=100; }//  200
		if(currenthealth > 100)
			{rectfill(b, bonus, by+1, bonus+bonusw, (by+1)+3, makecol(255,0,0) );bonus +=bonusnxt;currenthealth-=100; }//  300
		if(currenthealth > 100)
			{rectfill(b, bonus, by+1, bonus+bonusw, (by+1)+3, makecol(255,0,0) );bonus +=bonusnxt;currenthealth-=100; }//  400
		}
	//  draw standard health bar
	by += 6;
	rect(b, bx, by, bx+bw, by+5, makecol(0,0,0));
	// Figure out the health and draw it
	barwidth = (((float)currenthealth/100.0)*((float)(bw-2)));//  get % of bar
	rectfill(b, bx+1, by+1, (bx+1)+(int)barwidth, (by+1)+3, makecol(0,255,0));
	// Draw the lines going down
	for (i=0; i<bw; i+=3)
		{
		line(b, bx+i, by, (bx+i), by+5, makecol(0,0,0));
		}

	// Draw shield energy
	if(currentenergy > 0)
		{
		by += 6;
		// Figure out shields if we have any
		barwidth = (((float)currentenergy/(float)startingenergy)*((float)(bw-2)));
		rect(b, bx, by, bx+bw, by+5, makecol(0,0,0));
		rectfill(b, bx+1, by+1, (bx+1)+(int)barwidth, (by+1)+3, makecol(0,0,255)); 

		// Draw the lines going down
		for(i=0; i<bw; i+=3)
			{
			line(b, bx+i, by, (bx+i), by+5, makecol(0,0,0));
			}
		}

	// Draw fuel meter
	if(currentfuel > 0)
		{
		by += 6;

		// Figure out shields if we have any
		barwidth = (((float)currentfuel/(float)startingfuel)*((float)(bw-2)));
		rect(b, bx, by, bx+bw, by+5, makecol(0,0,0));
		rectfill(b, bx+1, by+1, (bx+1)+(int)barwidth, (by+1)+3, makecol(178,115,255)); 

		// Draw the lines going down
		for(i=0; i<bw; i+=3)
			{
			line(b, bx+i, by, (bx+i), by+5, makecol(0,0,0));
			}
		}

	if(opacity != 255)
		solid_mode();
}

//
// DrawPowerEnergy - Draws the players power and energy
//

void	TANK::DrawPowerEnergy(BITMAP *b, int opacity)
{

	DrawPowerEnergyBar(b, x, y+15, 40, 10, opacity, 
		health, starthealth, 
		energyleft, defenses[activedefense].energy,
		specials[fuel].amount,
		startfuel);

	/*int		bx,by,bw,bh;
	int		i;
	float	barwidth;

	// Keep in mind that we're going to be drawing a 39x9 bitmap
	// directly onto the buffer 'b'
	bx = x-(39/2);
	by = y+15;
	bw = 39;
	bh = 9;

	// Add the dirty rectangle first
	AddRectangle(b, bx-1, by-1, bw+1, bh*3+1);

	// Figure out the health and draw it
	barwidth = ((health/100)*((float)(bw-2)));
	rect(b, bx, by, bx+bw, by+5, makecol(0,0,0));
	if (health > 0)
		rectfill(b, bx+1, by+1, (bx+1)+(int)barwidth, (by+1)+3, makecol(0,255,0)); 

	// Draw the lines going down
	for (i=0; i<bw; i+=3)
	{ line(b, bx+i, by, (bx+i), by+5, makecol(0,0,0)); }

	// Draw shield energy
	if (energyleft > 0)
	{
		by += 5;
		// Figure out shields if we have any
		barwidth = (((float)energyleft/(float)defenses[activedefense].energy)*((float)(bw-2)));
		rect(b, bx, by, bx+bw, by+5, makecol(0,0,0));
		rectfill(b, bx+1, by+1, (bx+1)+(int)barwidth, (by+1)+3, makecol(0,0,255)); 

		// Draw the lines going down
		for (i=0; i<bw; i+=3)
		{ line(b, bx+i, by, (bx+i), by+5, makecol(0,0,0)); }
	}

	

	// Draw fuel meter
	if (specials[fuel].amount > 0)
	{
		by += 8;

		// Figure out shields if we have any
		barwidth = (((float)specials[fuel].amount/(float)startfuel)*((float)(bw-2)));
		rect(b, bx, by, bx+bw, by+5, makecol(0,0,0));
		rectfill(b, bx+1, by+1, (bx+1)+(int)barwidth, (by+1)+3, makecol(178,115,255)); 

		// Draw the lines going down
		for (i=0; i<bw; i+=3)
		{ line(b, bx+i, by, (bx+i), by+5, makecol(0,0,0)); }
	}*/
}


void	TANK::ApplyFallingDamage(void)
{
	char		str[32];

	falling = false;

	// Dont apply damage if we've fallen less than 25 pixels
	if (fallenpixels <= 25 || flags & TANKFLAG_PARACHUTE)
	{	
		if (flags & TANKFLAG_PARACHUTE)
			flags &= ~TANKFLAG_PARACHUTE;
		fallenpixels = 0;
		return;
	}

	// We dont take any damage from falling damage
	// However, we do take away a parachute if you used one to fall into
	// the water.
	if (underwater)
	{	
		fallenpixels = 0;
		return;
	}

	// Go no further if we're the client, the server will delegate how
	// much damage to dish out
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Subtract the maximum falling resistance
	fallenpixels -= 25;

	// For each pixel fallen, apply 1 hitpoint of damage
	health -= fallenpixels;

	// Play sound
	StartSound(TANK_FALLDAMAGE);

	// Show how much damage we took from the fall
	sprintf(str, "-%d", (int)fallenpixels);
	SpawnGameText(str, (int)(1.2*TICRATE), (int)x, (int)y, 0, -6, makecol(255,0,0));

	// Send a message to all clients letting them know who has fallen
	if (multiplayer && multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_DAMAGETANKFALL);
		BCast_WriteByte(indexnum);
		BCast_WriteByte(0);	// bypass shields
		BCast_WriteShort(fallenpixels);
		BCast_WriteByte(0);	// no text scatter

		BCast_WriteShort(health);
		BCast_WriteShort(energyleft);
	}

	// Reset
	fallenpixels = 0;

	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Not alive?
	if (health <= 0)
	{
		needstodie = 10;

		if (lasttankturn > -1)
		{
			lasthit = lasttankturn;
			
			// We made ourself fall
			if (lasthit == indexnum)
			{
				//CharacterSay(fallkill_text[rand()%MAX_FALLKILL_TEXT], indexnum, (int)(3*TICRATE));

				lasthit = -1;
				GivePoints(-100);
				GiveMoney(-MONEY_KILL);
				kills--;
			}

			// We made someone else fall
			else
			{
				ta[lasthit].GiveMoney(MONEY_KILL);
				ta[lasthit].GivePoints(100);
				ta[lasthit].kills++;
			}
		}
		else
		{

			//CharacterSay(fallkill_text[rand()%MAX_FALLKILL_TEXT], indexnum, (int)(3*TICRATE));
			lasthit = -1;
			kills--;
			GiveMoney(-MONEY_KILL);
			GivePoints(-100);
		}


		SetTankTimer(10);
		//KillTanks();

	}

}




//
// fall() - Drops the tank pixel by pixel until a non
// transparent pixel is found. Also applies falling damage
// if no parachutes are present
//


void TANK::fall(BITMAP *bmp, BITMAP *wbmp)
{

    int a,b,pixels=6,i;
	int	oldy = (int)y;

	// Server controls this
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Do tanks even fall?
	if (currentlevel.gravityflags & NOGRAVITY_TANKS)
		return;

	// Waiting for something to finish
	//if (checktanktimer > 0)
	//	return;

	// If we've already deployed our parachute, then only fall at
	// 2 pixel speed instead of 6
	if (underwater || (flags & TANKFLAG_PARACHUTE) || (flags & TANKFLAG_FLOATATION))
		pixels = 2;

    for (i=0; i<pixels; i++)
	{
		if ((y+sizey/2) >= SCREENHEIGHT || (y+sizey/2) <= 0)
		{
			ApplyFallingDamage();
			return;
		}

		if (flags <= 0 || (flags & TANKFLAG_PARACHUTE))
		{

			a=getpixel(bmp,(int)(x-sizex/2),(int)(y+sizey));
			b=getpixel(bmp,(int)(x+sizex/2),(int)(y+sizey));
			
			if (!(a != TRANSCOLOR || b != TRANSCOLOR))   
			{ 
				y++; 
				fallenpixels++;	// Count the number of pixels to apply falling damage

				// Is it time to bust out the 'chute? Sure, if we've fallen
				// more than 25 pixels & we even have one
				if (fallenpixels > 25 && defense[parachutes].amount >= 1 && !(flags & TANKFLAG_PARACHUTE))
				{
					flags |= TANKFLAG_PARACHUTE;
					defense[parachutes].amount--;
				}
			}
			else
			{
				// The left side hit something so we would slide
				// to the left
				if (a != TRANSCOLOR && b == TRANSCOLOR && moving == false)
				{
					if (CheckFallHeight((x-(sizex/2)), (y+(sizey/2)), 1) > 3 &&
						CheckFallHeight((x+(sizex/2)), (y+(sizey/2)), -1) > 3)
						x++;//move(1,0);

					fallenpixels++;
				}
	
				// The right side hit something, so we would slide
				// to the right
				else if (b != TRANSCOLOR && a == TRANSCOLOR && moving == false)
				{
					if (CheckFallHeight((x+(sizex/2)), (y+(sizey/2)), -1) > 3 &&
						CheckFallHeight((x-(sizex/2)), (y+(sizey/2)), 1) > 3)
						x--;//move(-1,0);

					fallenpixels++;
				}

				// We've hit land! Now calculate the number of pixels that the tank
				// has fallen and apply the damage accordingly.
				ApplyFallingDamage();
			}
		}

		if (oldy != (int)y)
			falling = true;
		else
			falling = false;

	}

}



//
// float() - Drops the tank pixel by pixel until a non
// transparent pixel is found. Also applies falling damage
// if no parachutes are present.
// As we are in water remove any parachute and if float
// then fall upwards.
//


void TANK::tankfloat(BITMAP *bmp, BITMAP *wbmp)
{

    int a,b,c,pixels=2,i;
	int	oldy = (int)y;

	// Server controls this
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// If we've already deployed our parachute, then only fall at
	// 2 pixel speed instead of 6
	if (underwater || (flags & TANKFLAG_PARACHUTE) || (flags & TANKFLAG_FLOATATION))
		pixels = 2;

    for (i=0; i<pixels; i++)
	{

		if ((y+sizey/2) >= SCREENHEIGHT ||	// fell past bottom of screen
			  (y+sizey/2) <= 0)								// float past top of screen
		{
		  // We dont take any damage from falling damage
	    // However, we do take away a parachute if you used one to fall into
	    // the water.
			ApplyFallingDamage();
			return;
		}

		// check below the float 
		a=getpixel(bmp,(int)(x-sizex/2),(int)(y+sizey));	// lower left dirt
		b=getpixel(bmp,(int)(x+sizex/2),(int)(y+sizey));	// lower right dirt
		c=getpixel(wbmp,(int)(x),(int)((y-1)+sizey));			// lower centre water

		if (c != TRANSCOLOR)  // we have water so fall up^^^^
		{
			fallenpixels = 0;
			y--;
			falling = true; // falling up
		}

		else if (!(a != TRANSCOLOR || b != TRANSCOLOR)) // if a is air and b is air 
		{ 
			y++; 
			fallenpixels++;	// Count the number of pixels to apply falling damage
			falling = true;
		
		}
		else
		{
			flags &= ~TANKFLAG_FLOATATION;
			// We've hit land! Now calculate the number of pixels that the tank
			// has fallen and apply the damage accordingly.
			ApplyFallingDamage();
		}
		

		if (oldy != (int)y)
			falling = true;
		else
			falling = false;
		


		
	}

}


//
// Onground - Immediatley set the tank on ground. Normally used for 
// setting up levels and what not. Never used at runtime.
//

void TANK::Onground(BITMAP	*bmp)
{
	//int a,b;

	int		footx1,footx2;
	int		footy;

	int		fcol1,fcol2;

	// Get initial values
	footx1 = (x-(sizex/2))+3;
	footx2 = (x+(sizex))-3;
	footy = (y+(sizey/2));

	// Get the color from each foot
	fcol1 = getpixel(bmp, footx1, footy);
	fcol2 = getpixel(bmp, footx2, footy);

	// Tracing up or down?

	// If there is land pixels on either foot, keep moving up until we're in the air
	if (fcol1 != TRANSCOLOR || fcol2 != TRANSCOLOR)
	{
		while (1)
		{
			fcol1 = getpixel(bmp, footx1, footy-1);
			fcol2 = getpixel(bmp, footx2, footy-1);

			if (fcol1 != TRANSCOLOR || fcol2 != TRANSCOLOR)
				footy--;
			else
			{
				// Free space! Calculate new y value and end the loop
				y = footy-(sizey/2);
				break;
			}

			// Make sure we dont end up in a never-ending loop
			if (footy < 0)
				return;
		}
	}
	else
	{
		while (1)
		{
			fcol1 = getpixel(bmp, footx1, footy+1);
			fcol2 = getpixel(bmp, footx2, footy+1);

			if (fcol1 == TRANSCOLOR && fcol2 == TRANSCOLOR)
				footy++;
			else
			{
				// Free space! Calculate new y value and end the loop
				y = footy-(sizey/2);
				break;
			}

			// Make sure we dont end up in a never-ending loop
			if (footy > SCREENHEIGHT)
				return;
		}

	}

			// Test new values
			/*y--;

			// Get new varibles values (copy/paste action)
			footx1 = (x-(sizex/2))+3;
			footx2 = (x+(sizex))-3;
			footy = (y+(sizey/2));
			fcol1 = getpixel(bmp, footx1, footy);
			fcol2 = getpixel(bmp, footx2, footy);

			// Test the ground again
			if (fcol==*/
			

	/*y=0;

	for (int i=0; i<SCREENHEIGHT; i++)
	{

		if ((y+sizey/2) >= SCREENHEIGHT || (y+sizey/2) <= 0)
			return;

		a=getpixel(bmp,(int)(x-sizex/2),(int)(y+sizey/2));
		b=getpixel(bmp,(int)(x+sizex/2),(int)(y+sizey/2));
	    
		if (a == TRANSCOLOR && b == TRANSCOLOR)
			y++;
		else
			return;
	}
	*/
}

bool	TANK::IsOnGround(BITMAP *bmp)
{
	

	// On the ground? It's as easy as this evaluation...
	

	return false;
}

bool	AreTanksFalling(void)
{
	int		i;
	int		a,b,c;

	// Do tanks even fall?
	if(currentlevel.gravityflags & NOGRAVITY_TANKS) return false;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0 || ta[i].health <= 0)
			continue;

		a=getpixel(ground.bmp,(ta[i].x-(ta[i].sizex/2)),(ta[i].y+ta[i].sizey)); // lower left
		b=getpixel(ground.bmp,(ta[i].x+(ta[i].sizex/2)),(ta[i].y+ta[i].sizey)); // lower right

		if (a == TRANSCOLOR && b == TRANSCOLOR) // air OR water
			{ // fix float freeze bug ie detect water under float 20April2006
			c=getpixel(pwater->dbuffer, ta[i].x, (ta[i].y+ta[i].sizey) ); // lower centre 
			if(c != TRANSCOLOR) continue;	// we have water
			return true;									// Oops we are in air ! ! ! !
			}
	}
	return false;
}


int	OnGround(int x, int y, BITMAP *bmp)
{
	int		newy=y;

	
	if(getpixel(bmp, x, newy) != TRANSCOLOR)  // found dirt so rise to the surface
		{
		while(getpixel(bmp, x, newy) != TRANSCOLOR)  // still dirt
			{
			newy--;
		
			if(OnScreen(bmp, x, newy) == -1)
				{
				return newy+1;
				}
			}
		}
	else	//	we must be in air so look down for dirt
		{
		while(getpixel(bmp, x, newy) == TRANSCOLOR)	//	still air
			{
			newy++;
		
			if(OnScreen(bmp, x, newy) == -1)
				{
				return newy-1;
				}
			}
		}

	return newy;
}

//
// OnTopGround - This uses the old meathod by finding highest pixel from the top of the screen
// down.
//

int	OnTopGround(int x, int y, BITMAP *bmp)
{
	int		a;
	int		i;

	y=0;

	for (i=0; i<SCREENHEIGHT; i++)
	{

		if (y >= SCREENHEIGHT)
			return y;

		a=getpixel(bmp,x,y);
	    
		if (a == TRANSCOLOR)
			y++;
		else
			return y;
	}

	return 0;
}


//
// setcolor - Sets the color attribute of the tank. 
// This identifies the tank apart from the other tanks
//

void TANK::setcolor(int r, int g, int b)
{
	colorr = r;
	colorg = g;
	colorb = b;
}

//
// SetTeamColor - Sets the tank color based on what team we're on
//

void TANK::SetTeamColor(int t)
{
	this->team = t;

	if (team == TANKTEAM_RED)
	{
		colorr = 255;
		colorg = 0;
		colorb = 0;

	}
	else
	{
		colorr = 0;
		colorg = 0;
		colorb = 255;
	}
}


//
// IsSameTeam - Compares another tank to determine if they're on the 
// same team or not 
//

bool	TANK::IsSameTeam(int tank)
{
	if(tank == -1) return false;	//	tank is level weapon
	if (teamplay->value <= 0)
		return false;

	if (ta[tank].team != team)
		return false;

	return true;
}

//
// AutoAssignTeam
//

void	TANK::AutoAssignTeam(void)
{
	int		numred=0;
	int		numblue=0;
	int		assigned_team=-1;
	int		i;

	// Gotta be in teamplay mode
	if (teamplay->value <= 0)
		return;

	// First, find how how many players are on each team. The team
	// that has the least amount of players gets assigned to this
	// tank

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		if (i == indexnum)
			continue;

		if (ta[i].team == TANKTEAM_RED)
			numred++;
		if (ta[i].team == TANKTEAM_BLUE)
			numblue++;
	}

	// Now, based on our findings, assign a team. If there is an
	// even amount on both teams, assign a random team
	if (numred == numblue)
	{	
		// Greater than or equal to 5? Go to the red team
		if (rand()%10 >= 5)
			assigned_team = TANKTEAM_RED;
		else
			assigned_team = TANKTEAM_BLUE;
	}

	if (numred > numblue)
		assigned_team = TANKTEAM_BLUE;

	if (numblue > numred)
		assigned_team = TANKTEAM_RED;

	if (assigned_team < 0)
	{ Printf("AutoAssignTeam: OMFG WTF!!!!! SOEM WIERD AEEORRO HAS OCCREUED!!!!!!!111111111oneoneone\n"); return; }

	SetTeamColor(assigned_team);
}


//
// SwitchTeam - Players are only allowed to switch a team on their turn. When they do switch
// teams, they forfeit their turn
//

void	TANK::SwitchTeam(void)
{
	char	str[128];

	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Not our turn, cannot switch teams
	if (indexnum != tankturn)
		return;

	// Already made a shot, cannot switch teams
	if (specialflags & TANKSPECIAL_MADESHOT)
		return;

	if (team == TANKTEAM_RED)
		SetTeamColor(TANKTEAM_BLUE);
	else
		SetTeamColor(TANKTEAM_RED);

	// Come up with a message
	if (team == TANKTEAM_RED)
		sprintf(str, "%s has switched to the RED team.", name);
	else
		sprintf(str, "%s has switches to the BLUE team.", name);

	// Update all clients about this change
	if (multiplayer && multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_SETTANKTEAM);
		BCast_WriteByte(indexnum);
		BCast_WriteShort(team);
		BCast_WriteByte(colorr);
		BCast_WriteByte(colorg);
		BCast_WriteByte(colorb);

		BCast_Printf(str);
	}
	else
		G_Printf(str); //	switch teams message

	// Any sentry pods that have been spawned also need to be
	// destroyed
	KillTankSentryPods(indexnum);

	// Set the 'madeshot' flag so that the player cannot fire
	// and setup the tank timer to end the turn
	specialflags |= TANKSPECIAL_MADESHOT;
	
	// Start the tank timer to end the turn
	SetTankTimer(20);
}



//
// TANK::KillShield - Removes the shield and takes away the attributes
//

void	TANK::KillShield(void)
{
	// No shield to take away
	if (energyleft <= 0)
		return;

	// Send this signal to all clients
	if (multiplayer && multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_KILLSHIELD);
		BCast_WriteByte(indexnum);
	}

	// Play sound
	if (activedefense == electric_shield)
		StartSound(SHIELD_ENERGY_POWERDOWN);
	else if (activedefense == fire_shield)
		StartSound(SHIELD_HEAT_POWERDOWN);
	else if (activedefense == ballistic_shield)
		StartSound(SHIELD_BALLISTIC_POWERDOWN);
	else if (activedefense == superconventional_shield)
		StartSound(SHIELD_SUPERCONVENTIONALPOWERDOWN);
	else
		StartSound(TANK_SHIELDPOWERDOWN);

	// Yes! Give the rest of our damage to the tank itself
	energyleft = 0;

	// Clear out shield defenses
	shieldh = shielde = shieldb = 0;

	// Remote the shield sprite
	if (shield)
	{
		if (activedefense == niv_shield)
			shield->SetState(S_SHIELD_NIV_DIE1);
		else
			shield->Kill();			

		shield = NULL;
	}
}



//
// TANK::Damage - Applies damage to tanks taking in consideration the shields etc...
//

void	TANK::Damage(float damageh, float damagee, float damageb, int inflictor, bool killtank, bool damageshields, bool scattertext)
{
	char	str[64];
	float	totaldamage;
	float	shielddamage_h,shielddamage_e,shielddamage_b;

	int		textx = (x-(sizex/2))-10;
	int		texty = (y-(sizey/2))-20;

	int		shielddamage=-1;		// total damage with shields
	int		tankdamage=-1;			// total damage to tank

	// The server will do our damage for us
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// No damage points to inflict
	if (damageh <= 0 && damagee <= 0 && damageb <= 0)
		return;

	// Teamplay enabled friendly fire? (tanks CAN damage themselves)
	if (teamplay->value > 0 && 
		friendlyfire->value <= 0 &&
		inflictor != indexnum && 
		IsSameTeam(inflictor))
	{ return; }

	// Set the inflictor varible so we know who last hit us. 
	// (used mainly for botAI)  --  skip level weapon & self
	if (inflictor >= 0 && inflictor != indexnum)
	{
		// Do NOT set this varible if teamplay is enabled with friendly
		// fire ON. Last think the bots need are to be attacking members
		// of their same team.
		if (IsSameTeam(inflictor) == false)
		{
			// Gunners will now target this tank
			if (skill == TANKSKILL_GUNNER || skill == TANKSKILL_SNIPER)
			{
				int		distx = (int)(ta[inflictor].x - x);

				targettank = inflictor;
			
				if (distx >= 0)
					deg = 75;
				else
					deg = 55;
			}

			lastinflictor = inflictor;
		}
	}

	// First, see if we can damage their shields first
	if (energyleft > 0 && damageshields)
		{
		shielddamage_h = ((10-(shieldh+h))*.1)*damageh;
		shielddamage_e = ((10-(shielde+e))*.1)*damagee;
		shielddamage_b = ((10-(shieldb+b))*.1)*damageb;

		totaldamage = (shielddamage_h+shielddamage_e+shielddamage_b);
		
		// No damage to deploy
		if (totaldamage <= 0)
			return;

		// Once we deplete the shield, is there still damage points
		// left to the action tank?
		if (energyleft-totaldamage <= 0)
			{
			// Calculate the remaining damage classes to be passed onto
			// the actual tank itself
			health -= abs((int)(energyleft-totaldamage));
			
			// Destroy the shield
			KillShield();
						
			// Display damage amount
			shielddamage = abs((int)(energyleft-totaldamage));
			sprintf(str, "-%d", shielddamage);
//			if (scattertext)
//				SpawnGameText(str, (int)(1.2*TICRATE), textx+(rand()%(10*2)), texty+(rand()%(20*2)), 0, -6, makecol(255,50,165), (FONT *)(FONT*)maindata[MS].dat, 0);
//			else
//				SpawnGameText(str, (int)(1.2*TICRATE), (int)x, (int)y, 0, -6, makecol(255,50,165));
			}
		else
			{
			// No, only damage the shields
			energyleft -= (int)totaldamage;

			shielddamage = abs((int)(totaldamage));

			sprintf(str, "-%d", shielddamage);
//			if (scattertext)
//				SpawnGameText(str, (int)(1.2*TICRATE), textx+(rand()%(10*2)), texty+(rand()%(20*2)), 0, -6, makecol(255,50,165), (FONT *)(FONT*)maindata[MS].dat, 0);
//			else
//				SpawnGameText(str, (int)(1.2*TICRATE), (int)x, (int)y, 0, -6, makecol(255,50,165));
			}

		}
	else // No shields? Thats too bad. Do raw damage.
		{
		
		damageh = ((10-h)*.1)*damageh;
		damagee = ((10-e)*.1)*damagee;
		damageb = ((10-b)*.1)*damageb;

		totaldamage = (damageh+damagee+damageb);

		// No damage to deploy
		//if (totaldamage <= 0)
		//	return;

		tankdamage = totaldamage;

		health -= totaldamage;
		sprintf(str, "-%d", (int)tankdamage);
		
//		if (scattertext)
//			SpawnGameText(str, (int)(1.2*TICRATE), textx+(rand()%(10*2)), texty+(rand()%(20*2)), 0, -6, makecol(255,0,0), (FONT *)(FONT*)maindata[MS].dat, 0);
//		else
//			SpawnGameText(str, (int)(1.2*TICRATE), (int)x, (int)y, 0, -6, makecol(255,0,0));
		}

//	if (scattertext)
		{
		int xvel = -2+rand()%5; // 2---6
		int yvel = 2+rand()%9; // 4---12
	//	if(textx > 400) xvel = -xvel; // if on right side float left 
		if(texty > 100) yvel = -yvel; // if near HUD float down
		SpawnGameText(str, (int)(5*TICRATE), textx+(rand()%(10*2)), texty+(rand()%(20*2)), xvel, yvel,
			(tankdamage>0)?makecol(255,0,0):makecol(255,50,165), (FONT *)(FONT*)maindata[MS].dat,TEXT_FADEOUT);// 0); 
		}
//	else
//		SpawnGameText(str, (int)(1.2*TICRATE), (int)x, (int)y, -6, -6, makecol(255,50,165));


	// Send a message to all clients letting them know who has been hit
	if (multiplayer && multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_DAMAGETANK);
		BCast_WriteByte(indexnum);
		
		if (shielddamage > 0)
		{
			BCast_WriteByte(1);
			BCast_WriteShort(shielddamage);
		}
		else
		{
			BCast_WriteByte(0);
			BCast_WriteShort(tankdamage);
		}

		if (scattertext)
			BCast_WriteByte(1);
		else
			BCast_WriteByte(0);

		BCast_WriteShort(health);
		BCast_WriteShort(energyleft);
	}


	// Let the tank know who hit them
	lasthit = inflictor;

	// Are we dead?
	if (health <= 0)
	{
		// You killed yourself?! It happens...
		if (inflictor == indexnum)
		{
			ta[inflictor].kills--;
			ta[inflictor].GivePoints(-100);

			// It costs you alot of money to get your 
			// tank fixed (called - irresponsibilty)
			ta[inflictor].GiveMoney(-MONEY_KILL);
		}
		else
		{
			ta[inflictor].kills++;
			ta[inflictor].GivePoints(100);
	
			// Give the player a reward
			ta[inflictor].GiveMoney(MONEY_KILL);
		}
			
		// Wipe out some varibles he'ar
		needstodie = 10;
		
		// Kill the tank instantly?
		if (killtank)
			Kill();
	}
	else
		healthbartrans = 255;

}



//
// Kill - Kills the tank, obviously.  =/
// 

void TANK::Kill(void)
{
	if (needstodie <= 0)
		return;

	// Check and see if we killed ourself, if we did
	// display a stupid message. If the value is lower than
	// zero do not output a character message, apparently 
	// it is being taken care of somewhere else (eg. falling kill)
	if (lasthit >= 0)
	{
		if (lasthit == indexnum)
			TankSay(selfkill_text[rand()%MAX_SELFKILL_TEXT], indexnum, (int)(3*TICRATE));
		else// if (rand()%255 <= 128)
		{
			// Spawn a kill messge
			if (rand()%255 < 128)
				TankSay(kill_text[rand()%MAX_KILL_TEXT], lasthit, (int)(3*TICRATE));
			else
				TankSay(killed_text[rand()%MAX_KILLED_TEXT], indexnum, (int)(3*TICRATE));
		}
	}

	if (energyleft > 0)
		KillShield();

	StartSound(TANK_EXPLODE);
	G_SpawnSprite((int)x, (int)y, S_EXPLODE1_1, (MF_NORMAL | MF_ADVANCETURN | MF_CENTERCOORD | MF_ALPHA));

	// Be sure we haven't been reclassed out of existance
	if (exist > 0)
		AddNextTurnQue(indexnum);

	StartSound(TANK_EXPLODE);
	G_SpawnSprite((int)x, y, S_EXPLODE1_1, (MF_NORMAL | MF_ADVANCETURN | MF_CENTERCOORD | MF_ALPHA));
	
	// Wipe out some varibles he'ar
	health = 0.0;
	needstodie = 0;

	// Deallocate all memory for this tank since its being
	// removed from the game
	if (exist <= 0)
	{
		destroy_bitmap(bmp);
		destroy_bitmap(cannon);

		if (shield)
			shield->Kill();
	}

	//Idle(75);
	
	// Time for intermission?
	//Inter_CheckGame();
}


//
// SetHEB - Set Tank attributes releating to (H)eat, (E)nergy, and (B)alistic damage
//

void TANK::SetHEB(int sh, int se, int sb)
{
	h = sh;	
	e = se;	
	b = sb;	

	// Error checking.
	if (h > 5) { h = 5; }
	if (e > 5) { e = 5; }
	if (b > 5) { b = 5; }

	if (h < 0) { h = 0; }
	if (e < 0) { e = 0; }
	if (b < 0) { b = 0; }
}

//
// SetHEB - Set Tank attributes releating to (H)eat, (E)nergy, and (B)alistic damage (for shields!)
//

void TANK::SetShieldHEB(int sh, int se, int sb)
{
	shieldh = sh;
	shielde = se;
	shieldb = sb;

	// Error checking.
	if ((shieldh+h) > 10) { shieldh = 10-h; }
	if ((shielde+e) > 10) { shielde = 10-e; }
	if ((shieldb+b) > 10) { shieldb = 10-b; }

	if (shieldh < 0) { shieldh = 0; }
	if (shielde < 0) { shielde = 0; }
	if (shieldb < 0) { shieldb = 0; }
}

//
// StartSound - Plays a sound that orginates from a tank
//

void TANK::StartSound(int sound, int loop, int stopcurrent)
{
	// Disabled sound
	if (nosound > 0)
		return;

	if (stopcurrent > 0)
		stop_sample(snd);

	snd = (SAMPLE *)sounddata[sound].dat;
	
	play_sample(snd, Cvar_VariableValue("soundvolume"), 128, 1000, loop);
}

//
//	FindMapPosition - Finds a map position for a tank to spawn at
//

void	TANK::FindMapPosition(void)
{
	int		i;

	// Assign our tank to a random XY coord
	while (1)
	{
		i = rand()%MAX_TANKS;
					
		// See if this is a valid spot
		if (currentlevel.posoccupied[i] != true)
			break;
	}
		
	// Assign the tank to this position
	x = currentlevel.tankxpos[i];
	y = currentlevel.tankypos[i];
	
	// Keep record of what position in the array this is filling. 
	// Just in case this tank leaves the game and another one joins
	tankpos = i;

	currentlevel.posoccupied[i] = true;

	// Set on the ground
	Onground(ground.bmp);
	move(1,0);
	move(-1,0);
	
}


//
// Disconnect - What else? Disconnects from any running game
//

void	TANK::Disconnect(void)
{
	char	str[92];
	int		oldtt = tankturn;

	// Kill out the shield
	if (energyleft > 0)
		KillShield();

	if (skill == TANKSKILL_HUMAN)
	{
		sprintf(str, "%s disconnected", (name[0]) ? name : net_adrtostring(address));
		BCast_Printf(str);
		Printf("\n");
	}

	// Waiting for this player?
	if (multiplayer && 
		multiplayermode == MP_SERVER && 
		!pingwaitReady)
	{
		pingwaitReady = true;
	
//		if (SV_AllPingReady())                     // << delete
if (SV_AllPingReady()	&& (checktanktimer <= 0) ) // << add
		{
			checktanktimer = 1;
			CheckTankDeaths();
		}
	}

	// Clear out the netstate
	netstate = NETSTATE_DISCONNECTED;

	// Clear out essential varibles so the engine does not
	// track this player anymore
	money = 0;
	kills = 0;
	score = 0;
	startfuel = 0;
	specials[fuel].amount = 0;
	defense[parachutes].amount = 0;

	// Clear out tracking varibles as well
	memset(weaponsused, 0, sizeof(weaponsused));
	memset(defenseused, 0, sizeof(defenseused));
	memset(specialsused, 0, sizeof(specialsused));

	activeweapon = W_BABYMISSILE;
	
	pingwaitReady = true;
	exist = -10;
	health = 0;

	// Free the map spawn point array so other tanks can use this point
	currentlevel.posoccupied[tankpos] = false;

	// Display a message
	if (multiplayermode == MP_SERVER)
	{
		// Bots dont have sockets to close
		if (skill == TANKSKILL_HUMAN)
		{
			if (tankstate == TANKSTATE_DEFENSE)
				SV_CheckAutoDefenseClosed();
	
			// Send the final packet
			netsock->SendPacket(address, outbuf);
	
			// Be sure to close the connection
			closesocket(netsock->sock);
		}

		/*if (numalivetanks() >= 1 && 
			indexnum == tankturn &&
			gamestate == GS_INGAME)
		{ 
			Printf("advancing turn from (%d)...\n", tankturn);
//			advanceturn(true); 
			Printf("new tankturn is: %d\n", tankturn);
		}*/

		// Advance the turn
		if (indexnum == tankturn && gamestate == GS_INGAME)
			SetTankTimer(10);

		BCast_WriteByte(STOC_PLAYERDISCONNECT);
		BCast_WriteByte(indexnum);

		memset(&address, 0, sizeof(address));
		memset(weapons, 0, sizeof(playerweapons_t));
		memset(defense, 0, sizeof(playerdefense_t));

		GiveAmmo(indexnum, W_BABYMISSILE, UNLIMITED_AMMO);
	}

	// Remove from current/future tank ques
	RemoveFromTurnQue(indexnum);


	// No more players left in the server? If this is the case,
	// reset the level so others can start fresh
	if (multiplayer && 
		multiplayermode == MP_SERVER && 
		skill == TANKSKILL_HUMAN &&
		gamestate == GS_INGAME)
	{
		if (numtanks() <= 0 || isallbots())
		{
			if (currentlevel.numshotsfired > 0)
				ResetLevel();
		}
	}
}




void	TANK::SetFlareTics(int tics)
{
	sprite_t*	flare;
	BITMAP*		bmp;

	flare = G_SpawnSprite(barrelshotx, barrelshoty, S_TANKMUZZLEFLARE, (MF_NORMAL));

	if (flare)
	{
		bmp = (BITMAP *)spritedata[flare->startstate.datafilelookup].dat;
		flare->rotatex = bmp->w;
		flare->rotatey = bmp->h/2;
		flare->misc1 = tics;
		flare->angle = deg;
	}
}


void	TANK::SetPhotonTics(int tics)
{
	sprite_t*	flare;
	BITMAP*		bmp;

	flare = G_SpawnSprite(barrelshotx, barrelshoty, S_TANKPHOTONFLARE, (MF_NORMAL));

	if (flare)
	{
		bmp = (BITMAP *)spritedata[flare->startstate.datafilelookup].dat;
		flare->rotatex = bmp->w;
		flare->rotatey = bmp->h/2;
		flare->misc1 = tics;
		flare->angle = deg;
	}
}


void	TANK::SetupBotCharacter(int c)
{
	int	inheritfrom = rand()%NUM_NORMALCHARACTERS;

	// Set who this tank is.
//	character = inheritfrom;
	character = skill+CHAR_STOOPID - 1;
Printf("Bot:%d  inheritfrom=%d  character=%d skill=%d\n",c,inheritfrom,character,skill); // Degauss

	// Inherit the HEB stats from another tank
	SetHEB(characters[inheritfrom].attrib_h, characters[inheritfrom].attrib_e, characters[inheritfrom].attrib_b);
}
	

//
// inittanks - Sets up the fixed colors of all the tanks in the array
//

void	inittanks(void)
{
	memset(ta, 0, sizeof(ta));

	// Setup fixed tank colors
	ta[0].setcolor(255,0,0);
	ta[1].setcolor(255,255,0);
	ta[2].setcolor(0,255,0);
	ta[3].setcolor(0,255,255);
	ta[4].setcolor(0,0,255);
	
	ta[5].setcolor(255,50,255); // was 255,0,255 ie transparent
	ta[6].setcolor(255,255,255);
	ta[7].setcolor(180,9,251);
	//ta[8].setcolor(180,9,251);
	//ta[9].setcolor(255,128,0);

	//ta[10].setcolor(198,255,0);
	//ta[11].setcolor(0,199,255);
	//ta[12].setcolor(192,192,192);
	//ta[13].setcolor(191,139,89);
	//ta[14].setcolor(70,147,152);
	//ta[15].setcolor(126,165,95);

	for (int i=0; i<MAX_TANKS; i++)
	{
		ta[i].exist = 0;

		ta[i].inbuf = new packet_t;
		ta[i].outbuf = new packet_t;
	}
}


void	ResetTanks(void)
{
	freetanks();

	memset(ta, 0, sizeof(ta));

	inittanks();
}

void	freetanks(void)
{
	int	i;

	for (i=0; i<MAX_TANKS; i++)
	{
		delete ta[i].outbuf;
		delete ta[i].inbuf;
	}
}


//
// numtanks - Returns the number of tanks playing
//

int numtanks(void)
{
	int		ntanks=0;

	for (int i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist > 0)
			ntanks++;
	}

	return ntanks;
}

//
// numalivetanks - Returns the number of tanks left alive
//

int	numalivetanks(void)
{
	int		ntanks=0;

	for (int i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist > 0 && 
			ta[i].health > 0 && 
			ta[i].spectator <= 0)
		{ 
			ntanks++; 
		}

		else if (ta[i].exist > 0 &&
			     ta[i].needstodie > 0)
		{ ntanks++; }
	}

	return ntanks;
}

//
// numaliveteam - Counts the number of players alive on a specified team
//

int		numaliveteam(int team)
{
	int		ntanks=0;

	for (int i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].team != team)
			continue;

		if (ta[i].exist > 0 && 
			ta[i].health > 0 && 
			ta[i].spectator <= 0)
		{ 
			ntanks++; 
		}

		else if (ta[i].exist > 0 &&
				 ta[i].needstodie > 0)
		{ ntanks++; }
	}

	return ntanks;
}


//
// numingametanks - Returns the number of tanks who are actually
// playing. (and are not spectator)
//

int	numingametanks(void)
{
	int		ntanks=0;

	for (int i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist > 0 && 
			ta[i].spectator <= 0)
		{ ntanks++; }
	}

	return ntanks;
}

//
// isallbots - Checks to see if all players are bots. If they are, returns TRUE
//

bool	isallbots(void)
{
	for (int i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].skill == TANKSKILL_HUMAN)
			return false;
	}

	// All bots!
	return true;
}

//
// SpawnTurnArrow - Spawns the bouncing arrow above a tanks head to let them
// know its their turn
//

void	SpawnTurnArrow(int tank)
{
	sprite_t	*s;
	s = G_SpawnSprite((int)ta[tank].x, (int)ta[tank].y-50, S_TURNARROW, (MF_NORMAL | MF_CENTERCOORD));
	if (s)
	{
		s->tintr = ta[tank].colorr;
		s->tintg = ta[tank].colorg;
		s->tintb = ta[tank].colorb;
		s->tintvalue = TANK_OPACITY;
		s->opacity = 255;
		s->misc1 = 0;
	}

	// Play a alert sound only if the game is running in the background
	if (multiplayer && 
		switchstate == SWITCHSTATE_OUT && 
		tank == net_localplayer)
			{
			StartSound(TANKTURN_ALERT);
			}

//	Maggen power decays with each shot.
//	This is not the best place for this routine to be called but
//	this occurs only once per tank turn and it occurs
//	for both local and all network players.
//	Had problems in trying to get the maggen to power sync across the network.
	Maggen_Decay();
	
}

//
// CheckAfterTurnSprites - Goes through the sprite list for any tank spawned
// sprites and allows them to attack before advancing the turn
//

bool	CheckAfterTurnSprites(int tank)
{
	// At least one shot must have been fired
	if(currentlevel.numshotsfired <= 0)
		return false;

	sprite_t	*tas=sprites;

	while (tas) //	Fire off any of this tank's pods before anything else
		{					//	endlevelshoot is set in SetAfterTurnSprites() at start of turn 
		if( (tas->tankfrom == tank) && (tas->flags & MF_POSTTURNATTACK)	)	//	an attack sprite belongs to active tank
			{
			if(tas->wobj >= 0 &&							//	that is alive
				tas->wobj < NUM_WOBJECTS &&			//	and legal
				tas->endlevelshoot > 0 &&				//	and has not had a go
				wobjects[tas->wobj].attackstate != S_NULL &&	// and has an attack function
				tas->health > 0)								//	and is not fatally wounded
				{
				// One of the sentry pods?
				if(tas->wobj == WO_HEATSENTRYPOD ||
						tas->wobj == WO_ENERGYSENTRYPOD ||
						tas->wobj == WO_BALLISTICSENTRYPOD || // or missile turret
						tas->wobj == WO_MISSILETURRET)
					{
					tas->SetState(wobjects[tas->wobj].attackstate);	//	start attack and then
					//	mark as done for this round -- this routine only fires one pod per pass
					tas->endlevelshoot = 0;	
					SetTankTimer(10);
					return true;					//	we'll be back for the rest next pass
					}
				}
			}
		tas = tas->next;
		}
	//	If we drop through then tank has finished with pods
	//	so check if something else (like the missile turret or lava)
	
	tas=sprites;	//	reset pointer to first sprite in list
	while (tas)
		{
		if( (tas->tankfrom == -1) && (tas->flags & MF_POSTTURNATTACK)	)	//	an attack sprite belongs to the level
			{
			if(tas->wobj >= 0 &&							//	that is alive
					tas->wobj < NUM_WOBJECTS &&			//	and legal
					tas->endlevelshoot > 0 &&				//	and has not had a go
					wobjects[tas->wobj].attackstate != S_NULL &&	// and has an attack function
					tas->health > 0)								//	and is not fatally wounded
				{
#ifdef	_DEBUG
				char msg[240];
				sprintf(msg,"Check sprite State[%d]%s is ON\n",tas->wobj, wobjects[tas->wobj].name);
					G_Printf(msg);
#endif
				tas->SetState(wobjects[tas->wobj].attackstate);
				tas->endlevelshoot = 0;
				SetTankTimer(30);
				return true;
				}
			}
		tas = tas->next;
		}

//	All fired so return false
	return false;
}

void	ClearAllShotFlags(void)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{
		ta[i].specialflags &= ~TANKSPECIAL_MADESHOT;
		ta[i].specialflags &= ~TANKSPECIAL_USEDTRACER;
		ta[i].specialflags &= ~TANKSPECIAL_MADETELPOSITIVE;
	}
}

//
// SetAfterTurnSprite - Goes through the sprite list and marks
// any sprites that are setup to fight for us
//

void	SetAfterTurnSprites(int tank)
{
	sprite_t*		tas = sprites;
	while (tas)
		{
		tas->endlevelshoot = 0;
		if (tas->flags & MF_POSTTURNATTACK)
			{
			if (tas->wobj >= 0 && 
				tas->wobj < NUM_WOBJECTS &&
				wobjects[tas->wobj].attackstate != S_NULL)
				{
				// Finally, make sure the pod was spawned from
				// the current tank
				// One of the sentry pods?
				if (tas->wobj == WO_HEATSENTRYPOD ||
					tas->wobj == WO_ENERGYSENTRYPOD ||
					tas->wobj == WO_BALLISTICSENTRYPOD)
					{
					if(tas->tankfrom == tank)
						{
						tas->endlevelshoot = 1;
						}
					else if(tas->tankfrom == -1)	// level spawned pods
						{
						tas->endlevelshoot = (rand()%4 == 0);		/// random number ok - server only routine
						}
					}
				else
					{
					// Something else (like the missile turret)
					tas->endlevelshoot = 1;
					}
				}
			}
		tas = tas->next;
		}

}

//
// advanceturn - Switches the turn over to another person
//

void advanceturn(bool	onlyadvance)// default:	onlyadvance=false
{
	int	oldturn = tankturn;
	int t, i=1;	//	need to set i as a jump to advancing: caused crash

	// Never advance the turn locally on a multiplayer-client game
	if(multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Skip all the "if"s and just get to the meat? (default: off)
	if(onlyadvance==true)
		goto	advancing;

	// Tracers do not advance turns
	if((ta[tankturn].lastweaponused == W_TWINELECTRICLANCES || ta[tankturn].lastweaponused == W_TRACERS) && ta[tankturn].exist > 0)
		return;

	// Any projectiles still left?
	if(shotinair())
		return;

	// Too few tanks to go into the loop
	if(numtanks() <= 1)
		return;

	// Waiting on somthing...
	if(checktanktimer > 0 || IsTankDeathSeq() > 0)
		return;

	// Any tank-spawned sprites that need to animate?
	if(CheckAfterTurnSprites(tankturn))
		{
		// After turn sprites are active
		currentlevel.afterturnsprite_active = 1;
		return;
		}
	else
		currentlevel.afterturnsprite_active = 0;	// No longer active

	i = 1;

advancing:

  	ClearAllShotFlags();
		ta[i].idletime = 60;

	// If the current tank is for some reason still in the defense mode box, kill that
	// out now. (This can happen if the shotclock runs out while in the inventory
	// setup
	if (ta[tankturn].tankstate == TANKSTATE_DEFENSE)
		agup_break_dialog();

	// First, find out where 'tankturn' is on the turn que
	for (i=0; i<MAX_TANKS; i++)
	{ if (tankturnque[i] == tankturn) { break; } }

	// Discontinue any messages
	if (!multiplayer)
		TankMessage_Break();

	t = i;

	do
		{
		t++;

		if (t >= MAX_TANKS)
		{ t = 0; }

		if (t >= 0 &&
			i != t && 
			tankturnque[t] >= 0 &&
			ta[tankturnque[t]].health > 0 && 
			ta[tankturnque[t]].exist > 0) 
			{
			// If the player used up all his ammo on the last turn, cycle
			// the weapons until we find something suitable. Why here? If
			// anywhere else the missiles either are inaccurate (info) or
			// activeweapon gets confused and launches the wrong kind of weapon
			if (ta[tankturnque[t]].weapons[ta[tankturnque[t]].activeweapon].ammo == 0 &&
				!multiplayer)
				{
				ta[tankturnque[t]].CycleWeapons();
				}

			// We have our new tankturn, return
			lasttankturn = oldturn;
			tankturn = tankturnque[t];

			// Flag any sprites that are setup to fight for this tank
			SetAfterTurnSprites(tankturn);

			// Reset the shot clock
			Tank_ResetShotClock((int)shotclocktime->value);

			// Spawn turn arrow
			SpawnTurnArrow(tankturn);

			// If the new player has a NIV shield equipped, remove it
			if(ta[tankturn].activedefense == niv_shield)
				ta[tankturn].KillShield();

			return;
			}

		} while (1);
}

//
// FindFirstTurn - Returns the number of the tank who has first shot
// on the turn que
//

int	FindFirstTurn(void)
{
	int		i;

	for(i=0; i<MAX_TANKS; i++)
		{
		if (tankturnque[i] >= 0)
			return tankturnque[i];
		}

	//G_Printf("**** ERROR: FindFirstTurn() - Unable to find a valid que!");
	return 0;
}

//
// InitNextTurnQue - Setups a empty turn que to for use on a new level
//

void		InitNextTurnQue(void)
{
	int		i;

	for(i=0; i<MAX_TANKS; i++)
		{ 
		if(ta[tankturnnextque[i]].exist > 0)
			tankturnque[i] = tankturnnextque[i]; 
		else
			tankturnque[i] = -33;	// Nobody occupying this
		}

	for(i=0; i<MAX_TANKS; i++)
		{
		tankturnnextque[i] = -33;
		}

	lasttankturn = tankturn = FindFirstTurn();
}

//
// InitDefaultTurnQue - Default turn que, players take linear turns
//

void		InitDefaultTurnQue(void)
{
	int	i;
	for (i=0; i<MAX_TANKS; i++)
	{ 
		if (ta[i].exist > 0)
			tankturnque[i] = i; 
		else
			tankturnque[i] = -33;
	}

	// Clear out next round que
	for (i=0; i<MAX_TANKS; i++)
	{ tankturnnextque[i] = -33; }

	lasttankturn = tankturn = FindFirstTurn();
}

//
// RemoveFromTurnQue - When a player disconnects/leaves for whatever reason. 
// The que from the current round *and* next round need to be reset to 0 so that
// other incoming players can make use of it.
//

void		RemoveFromTurnQue(int	tank)
{
	int		i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (tankturnque[i] == tank)
			tankturnque[i] = -1;

		if (tankturnnextque[i] == tank)
			tankturnnextque[i] = -1;
	}
}

//
// AddNextTurnQue - Adds a player for the next round que
//

void AddNextTurnQue(int player)
{
	int		i;

	// First make sure this tank dosen't already have a slot in the que
	// for the next round. (Allows you to call this function more than once)
	for (i=0; i<MAX_TANKS; i++)
	{ if(tankturnnextque[i] == player) { return; } }

	// Okay, we're good, find a open spot for the next turn que
	for (i=0; i<MAX_TANKS; i++)
	{
		if (tankturnnextque[i] < 0)
		{
			tankturnnextque[i] = player;
			return;
		}
	}
}

//
// TankStatDrawer - Draws the tankturn's stats. Displaying health, energy levels
//

void	TankStatDrawer(BITMAP *b)
{
	int		i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health <= 0)
			continue;

		if (ta[i].exist <= 0)
			continue;

		// If the ALT key is being pressed, then display stats
		// for all the players
		//if (i != tankturn && !(key[KEY_ALT] || (usejoystick && joy[0].button[5].b)) && ta[i].healthbartrans <= 0)
		//	continue;

		// Draw their power and energy indicator
		if (i == tankturn || 
			(key[KEY_ALT] || (usejoystick && joy[0].button[5].b)) )
			ta[i].DrawPowerEnergy(b,255);
		else
			ta[i].DrawPowerEnergy(b,ta[i].healthbartrans);

		if (key[KEY_ALT] || (usejoystick && joy[0].button[5].b))
			ta[i].DrawNameBox(b);
	}

	// Joystick button exclusive. Draw ALL heb stats?
	if (usejoystick && 	joy[0].button[4].b)
		DrawAllHebStats(b);
}

//
// DrawHEBStats
//

void	DrawHEBStats(BITMAP *buf)
{
	BITMAP	*hebchart = (BITMAP *)maindata[REALTIMEHEB].dat;
	float	barfill;
	int		x;
	int		y;

	// Not setup to show anybody
	if (tank_showstats < 0)
		return;

	ta[tank_showstats].DrawPowerEnergy(buf, 255);

	x = ta[tank_showstats].x-(hebchart->w/2);
	if (ta[tank_showstats].energyleft <= 0)
		y = ta[tank_showstats].y+(10+9+3);
	else
		y = ta[tank_showstats].y+(15+9+3);

	// Screen clipping
	if (y+hebchart->h > SCREENHEIGHT)
		y = ta[tank_showstats].y-(hebchart->h+(ta[tank_showstats].sizey*2));
	if (x < 0)
		x = 0;
	if (x+hebchart->w > SCREENWIDTH)
		x = SCREENWIDTH-hebchart->w;

	// Add rectangle for buffering
	AddRectangle(buf, x, y, hebchart->w, hebchart->h);
	draw_sprite(buf, hebchart, x, y);

	// Fill in heat values
	barfill = ((float)(ta[tank_showstats].h+ta[tank_showstats].shieldh) / 10)*74;		// 74 is the max height of the HEB bar on the chart
	rectfill(buf, x+16, y+83, x+(16+8), (y+83)-(int)barfill, makecol(255,0,0));

	// Fill in energy values
	barfill = ((float)(ta[tank_showstats].e+ta[tank_showstats].shielde) / 10)*74;		
	rectfill(buf, x+35, y+83, x+(35+8), (y+83)-(int)barfill, makecol(0,0,255));

	// Fill in ballistic values
	barfill = ((float)(ta[tank_showstats].b+ta[tank_showstats].shieldb) / 10)*74;		
	rectfill(buf, x+53, y+83, x+(53+8), (y+83)-(int)barfill, makecol(255,255,0));
}

//
// DrawAllHebStats - Shows all the stats at once. This is joystick exclusive
//

void	DrawAllHebStats(BITMAP *buf)
{
	int		i;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].health <= 0)
			continue;

		tank_showstats = i;

		DrawHEBStats(buf);
	}

	// Reset tank_showstats varible
	tank_showstats = -1;

}


//
// TanksFalling - If any tanks are falling, returns true
//

bool	TanksFalling(void)
{
	int		i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].health <= 0)
			continue;
		
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].falling == true)
			return true;
	}
	return false;
}

void Tank_SetOldVars(void)
{
	int		i;
	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		// Record old vars
		ta[i].SetOldVars();
	}
}


void	Tank_ResetShotClock(int	timer)
{
	if (!multiplayer || multiplayermode == MP_SERVER)
	{
		tank_shottimer = timer;
		tank_orginalshottimer = timer;

		if (multiplayer && multiplayermode == MP_SERVER)
			SV_ResetShotClock(timer);
	}
}


void	Tank_ShotClockTicker(void)
{
	// Tank has already shot, do not decrease the time
	if(ta[tankturn].specialflags & TANKSPECIAL_MADESHOT)
		return;

	if(!multiplayer)
		return;

	// There has to be at least two tanks for this to count down
	if(numtanks() <= 1)
		return;

	// At least one shot must have been fired
	if(currentlevel.numshotsfired <= 0)
		return;

	// Do not process the tank timer if a negative interger
	// is set
	if(tank_shottimer < 0)
		return;

	tank_shottimer--;

	// Never go below zero
	if(tank_shottimer == 1)
		{
		if(multiplayer && (ta[net_localplayer].specialflags & TANKSPECIAL_AUTODEFENSEWAIT))
			{
			if(multiplayermode == MP_SERVER)
				{
				if(ta[net_localplayer].tankstate == TANKSTATE_DEFENSE)
					agup_break_dialog();

				ta[net_localplayer].tankstate = TANKSTATE_PLAYING;
				}
			else
				{
				if(ta[net_localplayer].tankstate == TANKSTATE_DEFENSE)
					agup_break_dialog();

				ta[net_localplayer].tankstate = TANKSTATE_PLAYING;
				}
			// Go no further here in autodefense
			return;
			}
				
		if(!multiplayer || multiplayermode == MP_SERVER)
			{
			if(numalivetanks() >= 2)
				advanceturn(true);	//	shot-timer forced loss of turn.
			}
		}

	if(tank_shottimer < 0)
		tank_shottimer = 0;
}

void	Tank_ShotClockDrawer(BITMAP *b)
{
	int		w=14;
	int		h=74;
	float	drawh;

if(!multiplayer) return;

	// Figure out how much of the gradient needs to be filled
	if (tank_shottimer < 0)
		drawh = h;
	else
		drawh = ((float)tank_shottimer/(float)tank_orginalshottimer)*h;
  if(drawh > 74) drawh = 74;

	// Add rectangle
	AddRectangle(b, 4,28, w, h);

	// Set the drawing mode 
	drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	set_trans_blender(0,0,0,150);

	// Fill in rectangle of shot time left
	rectfill(b, 4, 28+h, 4+w, 28+(h-drawh), 
		makecol(ta[tankturn].colorr,ta[tankturn].colorg,ta[tankturn].colorb));
	solid_mode();
}

//
// TankThinker - Runs the logic rountines for all players in the game
//

void TankThinker(void)
{
	int		i;

	// Obviously dont run tank thinkers if we aren't in the game
	if (gamestate != GS_INGAME)
		return;

	// Do the server thang
	if (multiplayer && multiplayermode == MP_SERVER)
		SV_TankThinker();  // this looks for a new client. Nothing to do with tank thinking!!

	// Animate any tank deaths
	CheckTankDeathSeq();

	for(i=0;i<MAX_TANKS;i++) 
		{
		if(ta[i].health > 0 && ta[i].exist > 0)
			{
			ta[i].TankThinker();

			// Go no further if death animation is being played
			if(IsTankDeathSeq() > 0)
				continue;

			if(multiplayer && !SV_AllPingReady())
				continue;

			// Text animation is being played, go no further
			if(ta[i].texttimer > 0)
				continue;

			if(ta[i].flags & TANKFLAG_FLOATATION)
				ta[i].tankfloat(ground.bmp, pwater->dbuffer);

			// If there are any missiles that are in play
			// do not apply physics to tanks until after they're
			// done. (modeled after the orginal scorched earth)
			if(shotinair())
				continue;

			ta[i].fall(ground.bmp, pwater->dbuffer);

			if(i==tankturn)
				{		
				// Get input
				if(!multiplayer)
					ta[i].handleinput();

				// Use computer AI if we're not a human and there are no
				// tanks falling
				if(ta[i].skill != TANKSKILL_HUMAN && 
						!AreTanksFalling() && 
						!ground.falling && 
						currentlevel.afterturnsprite_active <= 0 &&
						checktanktimer <= 0)
					{ 
					ta[i].MyTurn(); // bots only
					}
				}	
			}
		}

	if(multiplayer &&
			tankturn == net_localplayer)
			ta[tankturn].handleinput();

	// Reguardless of client or server, both do this
	if (multiplayer)
		{
		ClientTank_Thinker();

		// Servers send updates about turn status
		if (multiplayermode == MP_SERVER)
			SV_UpdateClients();
		}
}

//
// CheckTankReserveDamage
//

void CheckTankReserveDamage(int inflictor)
{
	int		i;
	int		totaldamage;

	for (i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		if (ta[i].health <= 0)
			continue;

		totaldamage = ta[i].reservedamageh + ta[i].reservedamagee + ta[i].reservedamageb;

		if (totaldamage > 0)
			ta[i].Damage(ta[i].reservedamageh, ta[i].reservedamagee, ta[i].reservedamageb, inflictor, false);

		ta[i].reservedamageh = ta[i].reservedamagee = ta[i].reservedamageb = 0;
	}
}


//
// SetTankTimer
//

void	SetTankTimer(int time, bool dedicated)
{
	if (multiplayer && multiplayermode == MP_CLIENT)
		return;

	// Already waiting more than the alloted time
	if (checktanktimer > time)
		return;

	SV_ResetPingWait();

	if (multiplayer > 0 && 
		multiplayermode == MP_SERVER)
	{
		BCast_WriteByte(STOC_SETPINGWAIT);
		BCast_WriteLong(time);
	}

	checktanktimer = time;
}


//
// CheckTankDeaths
// 

void CheckTankDeaths(void)
{
	if (gamestate != GS_INGAME)
		return;

	if (shotinair() && checktanktimer <= 0)
		checktanktimer = 5;

	// None of these can be happening
	if (shotinair() || 
		IsTankDeathSeq() || 
		ground.falling || 
		AreTanksFalling() || 
		TanksFalling())
		return;

	checktanktimer--;

	if (checktanktimer == 0)
		{
		if (multiplayer)
			{
			if (multiplayermode == MP_SERVER)
				{
				if(checktanktimer > 0)return;
				if (!SV_AllPingReady())
					return;
				}
			else
				{
				localbuf->WriteByte(CTOS_PINGWAITREADY);
				return;
				}
			}
		KillTanks();
		}
}


//
// DeAllocateTankMemory - The small bitmaps that the tanks used are freed here
//

void DeAllocateTankMemory(void)
{
	int		i;

	for	(i=0; i<MAX_TANKS; i++)
	{
		if (ta[i].exist <= 0)
			continue;

		destroy_bitmap(ta[i].bmp);
		destroy_bitmap(ta[i].cannon);
	}
}


void	ToggleScoreBoard(void)
{
	if (drawscoreboard <= 0)
		drawscoreboard = 10;
	else
	{
		drawscoreboard = -10;

		// Refresh screen
		ground.needsupdate = true;
	}
}

void	DrawTankScoreBoard(BITMAP *b)
{
	int		tx,ty,i;

	// Not toggled?
	if (drawscoreboard <= 0)
		return;

	// Write out all other players in the game, highlighting whose turn it is
	tx = 780;
	ty = 580;

	for (i=MAX_TANKS-1; i>=0; i--)
	{
		if (tankturnque[i] < 0 || tankturnque[i] > MAX_TANKS)
			continue;
	
		if (ta[tankturnque[i]].health <= 0)
			continue;
		
		textout_right_ex(b, (FONT *)maindata[SNOOT10].dat, ta[tankturnque[i]].name, tx+1, ty+1, makecol(0,0,0), -1);
		if (tankturnque[i] == tankturn)
			textout_right_ex(b, (FONT *)maindata[SNOOT10].dat, ta[tankturnque[i]].name, tx, ty, makecol(0,255,0), -1);
		else
			textout_right_ex(b, (FONT *)maindata[SNOOT10].dat, ta[tankturnque[i]].name, tx, ty, makecol(200,200,200), -1);

		ty -= text_height((FONT *)maindata[SNOOT10].dat);
	}
}




static	char	talkstring[256];
static	int		istankmessage;


void	Start_TankMessage(void)
{
	if (console_active)
		return;

	// Set varible
	istankmessage = 10;

	//memset(talkstring, 0, sizeof(talkstring));
	sprintf(talkstring, "");

	ClearAllKeys();
}

void	TankMessage_Input(void)
{
	int		k;
	char	str[32];

	if (keypressed())
	{
		k = ureadkey(NULL);
		
		// Enter key was pressed
		if (k == 0x0d)
		{
			istankmessage = -1;

			ClearAllKeys();

			// See if we got any input
			if (ustrlen(talkstring) > 0)
			{
				if (!multiplayer)
					G_Printf(talkstring, tankturn);
				else
				{
					G_Printf(talkstring, net_localplayer);
					CL_SendChatMessage(talkstring);
				}
			}
			return;
		}

		// Backspace key was pressed
		else if (k == 0x08)
		{
			uremove(talkstring, -1);
			return;
		}

		// ESC key was pressed
		else if (k == 0x1b)
		{
			istankmessage = -1;
			ClearAllKeys();
			return;
		}
		
		// Don't allow text to get too long and to go off
		// the screen
		if (ustrlen(talkstring) >= sizeof(talkstring) ||
			text_length(font, talkstring) >= 500)
			return;

		// Add the character
		sprintf(str, "%c", k);
		ustrcat(talkstring, str);
	}
}


void	TankMessage_Drawer(BITMAP *b)
{
	int		endoftextx;
	int		endoftexty;

	if (IsTankMessage() <= 0)
		return;

	endoftextx = text_length(font, talkstring) + 170;
	endoftexty = text_height(font) + 77;


	AddRectangle(b, 162, 74, 514, 23);

	// Draw a transparent background
	drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	set_trans_blender(0,0,0,150);
	rectfill(b, 162, 74, 162+514-1, 74+23-1, makecol(0,0,0));
	solid_mode();

	line(b, endoftextx+1, endoftexty+1, 
		endoftextx+5+1, endoftexty+1, makecol(0,0,0));
	line(b, endoftextx, endoftexty, 
		endoftextx+5, endoftexty, makecol(0,255,0));
	
	// Draw the actual text
	textout(b, font, talkstring, 170+1, 77+1, makecol(0,0,0));
	textout(b, font, talkstring, 170, 77, makecol(0,255,0));
}


void	TankMessage_Break(void)
{
	istankmessage = 0;
}



//
// IsTankMessage - Is the tank currently typing something?
//

int		IsTankMessage(void)
{
	return istankmessage;
}



// 
// DIALOG BOX TO SWITCH WEAPONS
//


BITMAP			*weapbuf;
int				numweapons;
weaponinv_t		weaponinv[NUM_PROJECTILES];

extern			DIALOG dialog_weaponselect[];
void			WeaponsListDrawer(void);

char *weaponlist_getter(int index, int *list_size)
{
	if (index < 0) 
	{
		*list_size = numweapons;
		return NULL;
	}
	else
	{ return weaponinv[index].name; }
}

void	CalcCurrentWeapons(int p)
{
	int		i;
	int		ref=0;

	// Clear memory
	memset(weaponinv, 0, sizeof(weaponinv));

	numweapons = 0;

	for (i=0; i<NUM_PROJECTILES; i++)
	{
		if (ta[p].weapons[i].ammo > 0)
		{
			if (ustrlen(projectiles[i].gamename) > 0)
				weaponinv[ref].name = CopyString(projectiles[i].gamename);
			else
				weaponinv[ref].name = CopyString(projectiles[i].name);

			weaponinv[ref].count = ta[p].weapons[i].ammo;
			weaponinv[ref].refnum = i;

			ref++;
			numweapons++;
		}
	}
}

int	exit_weapsel(void)
{
	//agup_break_dialog();
	return D_O_K;
}

int		d_weaponsel_list(int msg, DIALOG * d, int c)
{
	if (msg == MSG_IDLE)
	{
		if (!GUI_MouseInside(dialog_weaponselect[0].x, 
			dialog_weaponselect[0].y,
			(dialog_weaponselect[0].x+dialog_weaponselect[0].w), 
			(dialog_weaponselect[0].y+dialog_weaponselect[0].h)))
		{
			if (mouseflags & MOUSE_FLAG_LEFT_DOWN)
			{
				agup_break_dialog();
				return D_EXIT;
			}
		}

		WeaponsListDrawer();
		return D_O_K;
	}

	return d_abeos_weaponsel_proc(msg, d, c);
}

DIALOG dialog_weaponselect[] =
{
	  { d_weaponsel_list,	  595,   0,  205,  246,255,    0,    0,   0,		0,	  0,  (void *)weaponlist_getter },
	  { NULL,                   0,    0,    0,   0,   0,    0,    0,   0,       0,    0,  0,                    0,     0 }
};

void	WeaponsListDrawer(void)
{
	if (update_count <= 0)
		return;

	update_count=0;

	if (gamestate == GS_INGAME)
		InGame_Runner();

	// Allocate a dirty rectangle
	AddRectangle(buffer, dialog_weaponselect[0].x, 
		dialog_weaponselect[0].y,
		dialog_weaponselect[0].w, 
		dialog_weaponselect[0].h);

	
	// Draw the bitmap
	draw_sprite(buffer, weapbuf, 0, 0);

	ticker();
	Screen_Blit(true);
	frame_count++;
	gametics++;

}

void	StartWeaponsList(int	player)
{
	// Allocate some memory for zee buffer
	weapbuf = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(weapbuf, TRANSCOLOR);

	// Figure out whats in inventory
	CalcCurrentWeapons(player);

	ta[player].tankstate = TANKSTATE_WEAPONSEL;

	// Start the dialog
	do_agup_dialog(dialog_weaponselect, weapbuf, WeaponsListDrawer);

	// Whatever weapon is highlighted, make that our selected weapon
	ta[player].activeweapon = weaponinv[dialog_weaponselect[0].d1].refnum;

	// We're active again
	ta[player].tankstate = TANKSTATE_PLAYING;

	// Free some memory
	destroy_bitmap(weapbuf);
}
