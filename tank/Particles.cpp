#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <allegro.h>
#include "scorch.h"
#include "particles.h"
#include "fblend.h"
#include "editor/editor.h"

int					particle_parse=0;

particle_t			*psystem;			// Normal particles (lava)
particle_t			*pwater;			// Static water (not dynamic)
particle_t			*pdirt;				// Dirt particles

int					scalestaticwaterlevel;

int	OnScreen(int x, int y, int w, int h)
{
	if(x > w ||
		x < 0 ||
		y > h ||
		y < 0)
	{ return -1; }

	return 1;
}

int	OnScreen(BITMAP	*bmp, int x, int y)
{
	if(x > bmp->w ||
		x < 0 ||
		y > bmp->h ||
		y < 0)
	{ return -1; }

	return 1;
}

void	BurnGround(BITMAP	*bmp, int x, int y, int size, int opacity)
{
	BITMAP	*burn_bmp = create_bitmap(size,size);//(BITMAP *)maindata[GROUND_BURN].dat;

	int	w,h,color,bmpcolor;

	x-=(burn_bmp->w/2);
	y-=(burn_bmp->h/2);

	clear_to_color(burn_bmp, TRANSCOLOR);
	circlefill(burn_bmp, size/2,size/2,size/2, makecol(0,0,0));
	drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	set_trans_blender(0,0,0,opacity);


	for (h=0; h<burn_bmp->h; h++)
	{
		for (w=0; w<burn_bmp->w; w++)
		{
			color = _getpixel16(burn_bmp,w,h);
			bmpcolor = getpixel(bmp, x+w, y+h);
			if(bmpcolor == -1)
				continue;

			if(color != TRANSCOLOR && bmpcolor != TRANSCOLOR)
				putpixel(bmp, x+w, y+h, color);
		}
	}

	destroy_bitmap(burn_bmp);

	// Make sure we flag the ground as needing to be updated
	ground.needsupdate = true;

	solid_mode();
}

void	ConcreteGround(BITMAP	*bmp, int x, int y, int size)
{		// modified BurnGround
	BITMAP	*burn_bmp = create_bitmap(size,size);

	int	w,h,color,bmpcolor;

	x-=(burn_bmp->w/2);
	y-=(burn_bmp->h/2);

	clear_to_color(burn_bmp, TRANSCOLOR);
	circlefill(burn_bmp, size/2,size/2,size/2, CONCRETE );
	// must leave colour untouched, so can't use set_trans_blender
	drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

	for (h=0; h<burn_bmp->h; h++)
		{
		for (w=0; w<burn_bmp->w; w++)
			{
			color = _getpixel16(burn_bmp,w,h);
			bmpcolor = getpixel(bmp, x+w, y+h);
			if(bmpcolor == -1)	//	off screen
				continue;

			if(color != TRANSCOLOR && bmpcolor != TRANSCOLOR)
				putpixel(bmp, x+w, y+h, color);
			}
		}

	destroy_bitmap(burn_bmp);

	// Make sure we flag the ground as needing to be updated
	ground.needsupdate = true;

	solid_mode();
}


//
// ShutdowParticles - Free's up all the allocated memory from the particle
// system and each of the individual particles, bitmaps..etc
//
void	ShutdownParticles(void)
{
	if(psystem)
		delete psystem;
	if(pwater)
		delete pwater;
	if(pdirt)
		delete pdirt;

	psystem = pwater = pdirt = NULL;
}

void	InitParticles(void)
{
	// Free up memory before we allocate some more
	if(psystem || pwater || pdirt)
		ShutdownParticles();

	// Init a new system
	psystem = new particle_t(2);
	pwater = new particle_t(6);
	pdirt = new particle_t(1);

}


particle_t::particle_t(int scale)
{
	particles = NULL;
	numparticles=0;
	numglowingparticles = 0;
	checkit = 0;
	SCALE_P = scale;
	flags = 0;
	needsupdate = true;

	// Setup the dirty rectangle system for particles
	botx = 0;
	boty = 0;
	topx = SCREENWIDTH;
	topy = SCREENHEIGHT;

	// Setup the scale
	SCALE_W = (SCREENWIDTH/SCALE_P);
	SCALE_H = (SCREENHEIGHT/SCALE_P);

	// For all of our waterbmp needs
	waterbmp = create_bitmap(SCALE_W, SCALE_H);
	clear_to_color(waterbmp, TRANSCOLOR);

	// Ditto for scalebmp (scales ground.bmp)
	scalebmp = create_bitmap(SCALE_W, SCALE_H);
	clear_to_color(scalebmp, TRANSCOLOR);

	// Glowing buffer for glowing particle effect
	glowbmp = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(glowbmp, TRANSCOLOR);

	// Buffer for drawing the particles onto the main buffer
	dbuffer = create_bitmap(SCREENWIDTH, SCREENHEIGHT);
	clear_to_color(dbuffer, TRANSCOLOR);

}

particle_t::~particle_t(void)
{
	// Delete all particles in the list
	DeleteAll();

	// Free bitmap memory
	destroy_bitmap(waterbmp);
	destroy_bitmap(scalebmp);
	destroy_bitmap(dbuffer);
	destroy_bitmap(glowbmp);
}

int particle_t::IsEmpty()
{
	if(!particles)
		return 0;
	return 1;
}

particlenode_t *particle_t::InsertParticle(int x, int y, float angle, float power, int color,
																					 int life, int flags, float gravity, float damageh,
																					 float damagee,float damageb)
{
	particlenode_t	*p = new particlenode_t;
	int				i=0;

	if(!p)
	{
#ifdef	_DEBUG
#ifdef	DEDICATED_SERVER
		Printf("Could not create another particle\n");
#else
		alert("Whoa!", "Could not create another particle", "Aborting.", "Shit!", NULL, 0,0);
#endif
#endif
		return NULL;
	}

	p->x = p->lastx = (x/SCALE_P);
	p->y = p->lasty = (y/SCALE_P);
	p->color = color;
	p->life = life;
	p->flags = flags;
	p->gravity = gravity;
	p->id = numparticles;
	p->angle = angle;
	p->xvel = -power*COS((int)angle);
	p->yvel = power*-SIN((int)angle);
	p->next = NULL;
	p->prev = NULL;
	p->next = particles;
	p->active = 1;
	p->damagecounth = damageh;
	p->damagecounte = damagee;
	p->damagecountb = damageb;

	p->glowr = 0;
	p->glowg = 0;
	p->glowb = 0;
	p->glowopacity = 0;

	// Setup extra varibles ifthis is a napalm particle
	if(flags & PFLAGS_DAMAGETANKS)
	{
		p->colorr = getr(color);
		p->colorg = getg(color);
		p->colorb = getb(color);

		// Set all tank damage to nil
		for (i=0; i<MAX_TANKS; i++)
		{
			p->damagedtank[i] = -1;
		}

	}

	if(OnScreen(waterbmp, p->x, p->y) == -1)
	{
		delete p;
		return NULL;
	}

	if(!(flags & PFLAGS_DIRT))
	{

		while (getpixel(scalebmp, (int)p->x, (int)p->y) != TRANSCOLOR)
		{
			p->y--;
			if(OnScreen(scalebmp, p->x, p->y) == -1)
			{
				delete p;
				return NULL;
			}
		}

		while (getpixel(waterbmp, (int)p->x, (int)p->y) != TRANSCOLOR)
		{
			p->y--;
			p->yvel=0;

			if(OnScreen(waterbmp, p->x, p->y) == -1)
			{
				delete p;
				return NULL;
			}
		}
	}

	particles = p;

	numparticles++;

	return p;
}


void		DropStaticWater(BITMAP	*grnd, int x, int y, int color, int		numparticles)
{
	particlenode_t	*p;

	// Grab the current state of the ground
	clear_to_color(pwater->scalebmp, TRANSCOLOR);
	masked_stretch_blit(grnd, pwater->scalebmp, 0,0, grnd->w, grnd->h, 0,(TANK_LANDSIT/pwater->SCALE_P), pwater->SCALE_W, pwater->SCALE_H);

	while (numparticles--)
	{
		p= pwater->InsertParticle(x,y,0,0,color,1,(PFLAGS_LIQUID|PFLAGS_WATER), 0);

		if(!p)
			continue;

		//StaticParticleWaterThinker(p);
		while (ParticleWaterThinker(pwater, p) > 0)
		{  }

		p->Draw(pwater->waterbmp, pwater->SCALE_P);
	}

	// Flag as needing a redraw
	pwater->needsupdate = true;

}

particlenode_t* particle_t::Delete(particlenode_t *p)
{
	return NULL;
}

void	particle_t::DeleteAll()
{
	particlenode_t	*p;

	while (particles)
		{
		p = particles;
		particles = p->next;
		delete p;
		}

	numparticles=0;

	// Clear some bitmaps
	clear_to_color(waterbmp, TRANSCOLOR);

}

void	NewParticle(int x, int y, float angle, float power, int color, int life, int flags, float gravity, float damageh, float damagee, float damageb)
{
	psystem->InsertParticle(x,y,angle,power,color,life,flags,gravity,damageh,damagee,damageb);
}

particlenode_t	*NewNapalmParticle(int x, int y, float angle, float power, int color, int life, int flags, float gravity, float damageh, float damagee, float damageb, int dlife)
{
	particlenode_t	*p;

	p = psystem->InsertParticle(x,y,angle,power,color,life,flags,gravity,damageh,damagee,damageb);

	// Something bad happened...
	if(!p)
		return NULL;

	p->damagelife = dlife;

	return p;
}



void	ParticleThinkers(void)
{
	psystem->RunThinkers();
	pdirt->RunThinkers();
}

void particle_t::RunThinkers()
{
	particlenode_t	*p = particles;
	particlenode_t	*prev=NULL;
	particlenode_t	*temp;

	// There arent any particles, why bother?
	if(numparticles <= 0)
		return;

	// Grab the current state of the ground
	clear_to_color(scalebmp, TRANSCOLOR);
	masked_stretch_blit(ground.bmp, scalebmp, 0,0, SCREENWIDTH, SCREENHEIGHT, 0,(TANK_LANDSIT/SCALE_P), SCALE_W, SCALE_H);

	// Reset active particles
	numactive = 0;

	// Reset glowing particles
	numglowingparticles = 0;

	while (p)
	{
		if(p->flags & PFLAGS_LIQUID)
			ParticleWaterThinker(this, p);
		else if(p->flags & PFLAGS_DIRT)
			ParticleDirtThinker(this, p);
		else
		{
			p->ParticleThinker();
			needsupdate = true;
		}

		if(p->flags & PFLAGS_GLOWING)
			numglowingparticles++;

		if(p->active > 0)
			numactive++;

		// Check for dead particles (or ifparticles leave the screen)
		if(p->life < 0 || OnScreen(waterbmp, particles->x, particles->y)==-1)
		{
			// We'll need to redraw
			needsupdate = true;

			//particles = Delete(particles);
			if(prev)
				prev->next = p->next;
			else
				particles = p->next;

			temp = p;
			p = p->next;

			numparticles--;
			delete temp;

		}
		else
		{
			prev = p;
			p = p->next;
		}

	}
}


void particlenode_t::ParticleThinker()
{
	life--;

	yvel += GRAVITY;

	x+=xvel;
	y+=yvel;
}


//
// ParticleWaterThinker
//

float			float_contactx, float_contacty;
int				lastx,lasty;
BITMAP			*contact_bmp;


void	callback_waterparticle(BITMAP *bmp, int x1, int y1, int color)
{
	if(float_contactx >= 0 || float_contacty >= 0)
		return;

	if(x1 >= contact_bmp->w ||
		x1 < 0 ||
		y1 >= contact_bmp->h ||
		y1 < 0)
	{
		float_contactx = x1;
		float_contacty = y1;
		return;
	}

	if(lastx >= 0 && lasty >= 0)
	{

		if(FAST_GETPIXEL(bmp,			(int)x1, (int)y1) != TRANSCOLOR ||
			FAST_GETPIXEL(contact_bmp,	(int)x1, (int)y1) != TRANSCOLOR)
		{
			float_contactx = x1;
			float_contacty = y1;
			return;
		}
	}

	lastx = x1;
	lasty = y1;
}


particlenode_t		*contact_particle;
int					contact_scale;

void	callback_napalmparticle(BITMAP *bmp, int x1, int y1, int color)
{
	sprite_t*	s = sprites;
	int		i;

	if(float_contactx >= 0 || float_contacty >= 0)
		return;

	if(x1 >= contact_bmp->w ||
		x1 < 0 ||
		y1 >= contact_bmp->h ||
		y1 < 0)
	{
		float_contactx = x1;
		float_contacty = y1;
		return;
	}


	// Are we going through any tanks? ifso, tally some damage up
	if(contact_particle->damagelife > 0)
		{
		for (i=0;i<MAX_TANKS;i++)
			{
			if(ta[i].health > 0)
				{
				if(x1*contact_scale >= int(ta[i].x-ta[i].sizex/2) &&
					y1*contact_scale >= int(ta[i].y-ta[i].sizex/2) &&
					x1*contact_scale <= int(ta[i].x+ta[i].sizex/2) &&
					y1*contact_scale <= int(ta[i].y+ta[i].sizex/2) &&
					contact_particle->damagedtank[i] < 0)
				{
					ta[i].napalmdamageh += contact_particle->damagecounth;
					ta[i].napalmdamagee += contact_particle->damagecounte;
					ta[i].napalmdamageb += contact_particle->damagecountb;

					// This particle cannot do any more damage to this tank
					contact_particle->damagedtank[i] = 1;
				}
			}
		}

		// Check all the sprites as well
		if(!(gametics % 30))
			{
			while(s)
				{
				if(s->flags & MF_SHOOTABLE)
					{
					// Apply superficial damage to the sprite that will be
					// applied later
					if(CheckPointInSprite(s, x1*contact_scale, y1*contact_scale))
						{
						s->reservedamageh += contact_particle->damagecounth;
						s->reservedamagee += contact_particle->damagecounte;
						s->reservedamageb += contact_particle->damagecountb;
						}
					}
				s = s->next;
				}
			}

		}


	if(lastx >= 0 && lasty >= 0)
		{

		if(FAST_GETPIXEL(bmp,			(int)x1, (int)y1) != TRANSCOLOR)
			{
			float_contactx = x1;
			float_contacty = y1;
			return;
			}

		if(FAST_GETPIXEL(contact_bmp,	(int)x1, (int)y1) != TRANSCOLOR &&
			!(contact_particle->flags & PFLAGS_NOLIQUIDCLIP))
			{
			float_contactx = x1;
			float_contacty = y1;
			return;
			}
	}

	lastx = x1;
	lasty = y1;
}


//
// DubAllParticles - When the global damage ticker has run out, set all particle
// damage to 0 so tanks do not continue to take damage even ifdamagelife > 0
//

void	DudAllParticles(void)
{
	particlenode_t*	p = psystem->particles;

	while (p)
		{
		p->damagecounth = 0;
		p->damagecounte = 0;
		p->damagecountb = 0;

		p = p->next;
		}
}

//
//  NapalmGlow - ifthis is a napalm partcile, make it pulse slightly
//

void NapalmGlow(particlenode_t	*p)
{
	if(p->flags & PFLAGS_NAPALMEFFECT2)
		{
		if(p->colorphase == 0)
			{
			p->colorg+=4;
			if(p->colorg >= 125)
				{
				p->colorphase = 1;
				p->colorg = 125;
				}
			}
		else
			{
			p->colorg-=4;
			if(p->colorg <= 0)
				{
				p->colorg = 0;
				p->colorphase = 0;
				}
			}
		}

	else if(p->flags & PFLAGS_NAPALMEFFECT1)
		{
		if(p->colorphase == 0)
			{
			p->colorg+=3;
			if(p->colorg >= 100)
				{
				p->colorphase = 1;
				p->colorg = 100;
				}
			}
		else
			{
			p->colorg-=3;
			if(p->colorg <= 0)
				{
				p->colorg = 0;
				p->colorphase = 0;
				}
			}
		}

	p->color = makecol(p->colorr, p->colorg, p->colorb);
}


void	PlasmaGlow(particlenode_t*	p)
{
	if(p->colorphase == 0)
		{
		p->glowopacity+=3;

		if(p->glowopacity > 200)
			{
			p->glowopacity = 200;
			p->colorphase = 1;
			}
		}
	else
		{
		p->glowopacity-=3;

		if(p->glowopacity < 100)
			{
			p->glowopacity = 100;
			p->colorphase = 0;
			}
		}
}


int	ParticleWaterThinker(particle_t*	system, particlenode_t	*p)
{
	// For all water particles (not napalm)
	if(p->active)
		p->waterlife++;

	// Run some effects ifneeded
	if(p->flags & PFLAGS_DAMAGETANKS)
		{
		if((p->flags & PFLAGS_NAPALMEFFECT1) || (p->flags & PFLAGS_NAPALMEFFECT2))
			NapalmGlow(p);

		if(p->flags & PFLAGS_GLOWING)
			PlasmaGlow(p);

		// Takeith life away
		p->life--;

		// Takeith damage life away
		p->damagelife--;
		}

	// Particle is active, just not moving
	//if(p->damagelife-1 <= 0 && (p->flags & PFLAGS_DAMAGETANKS))
	//	return 0;

	//  Setup our bitmap
	contact_bmp = system->waterbmp;

	// Setup contact particle (for napalm)
	contact_particle = p;
	contact_scale = system->SCALE_P;

	// Reset counters
	float_contactx = float_contacty = -1;
	lastx = lasty = -1;


	// Particle frozen?
	if(p->flags & PFLAGS_FROZEN)
		{

		if(p->y+1 >= system->SCALE_H || p->y+1 <= 0)
			{
			p->active = 0;
			return 0;
			}

		// We've hit solid ground
		if(p->flags & PFLAGS_DAMAGETANKS)
			{
			if(FAST_GETPIXEL(system->scalebmp, (int)p->x, (int)p->y+1) != TRANSCOLOR)
				{
				if((p->flags & PFLAGS_DAMAGETANKS) && p->burnmark != 1)
					{
					BurnGround(ground.bmp, (int)(p->x*system->SCALE_P), (int)((p->y*system->SCALE_P)-(TANK_LANDSIT/system->SCALE_P)), 10, 7);
					p->burnmark = 1;
					}
				//p->active = 0;
				//return 0;
				}
			}

		// There is room below us! Un-freeze and continue physics
		if(FAST_GETPIXEL(system->waterbmp, (int)p->x, (int)p->y+1) == TRANSCOLOR && FAST_GETPIXEL(system->scalebmp, (int)p->x, (int)p->y+1) == TRANSCOLOR)
			{
			p->flags &= ~PFLAGS_FROZEN;
			goto phys;
			}
		else
			{
			// Okay, the particle is on the ground (or another particle), see ifwe can move around
			if(p->xvel >= 0)
				{
				// Hits the edge of the screen
				if(p->x+1 >= system->SCALE_W)
					{
					p->watercount = 0;		// Reset pixel tracker
					p->xvel = -p->xvel;
					p->active = 0;
					return 0;
					}

				// We hit ground, go the other way
				if(FAST_GETPIXEL(system->scalebmp, (int)p->x+1, (int)p->y) != TRANSCOLOR)
					{
					// Leave a burn mark ifneeded
					if(p->flags & PFLAGS_DAMAGETANKS)
						{
						BurnGround(ground.bmp, (int)(p->x*system->SCALE_P), (int)((p->y*system->SCALE_P)+(system->SCALE_P)-2), 10, 7);
						}

					p->watercount = 0;		// Reset pixel tracker

					p->xvel = -p->xvel;
					p->active = 0;

					if(p->flags & PFLAGS_SUCIDIAL)
						p->life = -1;
					return 0;
					}

				if(FAST_GETPIXEL(system->waterbmp, (int)p->x+1, (int)p->y) == TRANSCOLOR)
					{
					p->watercount++;		// Pixel tracker
					p->x++;
					return 1;
					}
				else
					{
					p->watercount = 0;		// Reset Pixel tracker
					p->xvel = -p->xvel;
					p->active = 0;

					if(p->flags & PFLAGS_SUCIDIAL)
						p->life = -1;
					return 0;
					}
				}
			else
				{
				if(p->x-1 <= 0)
					{
					p->watercount = 0;		// Reset Pixel tracker
					p->xvel = -p->xvel;
					p->active = 0;
					return 0;
					}

				// We hit ground, go the other way
				if(FAST_GETPIXEL(system->scalebmp, (int)p->x-1, (int)p->y) != TRANSCOLOR)
					{
					// Leave a burn mark ifneeded
					if(p->flags & PFLAGS_DAMAGETANKS)
						{
						BurnGround(ground.bmp, (int)(p->x*system->SCALE_P), (int)((p->y*system->SCALE_P)+(system->SCALE_P)-2), 10, 7);
						}
					p->watercount = 0;		// Reset Pixel tracker

					p->xvel = -p->xvel;
					p->active = 0;

					if(p->flags & PFLAGS_SUCIDIAL)
						p->life = -1;
					return 0;
					}

				if(FAST_GETPIXEL(system->waterbmp, (int)p->x-1, (int)p->y) == TRANSCOLOR )
					{
					p->watercount++;		// Pixel tracker
					p->x--;
					return 1;
					}
				else
					{
					p->watercount = 0;		// Reset Pixel tracker
					p->xvel = -p->xvel;
					p->active = 0;

					if(p->flags & PFLAGS_SUCIDIAL)
						p->life = -1;
					return 0;
					}

			}

		}
	}
	else
	{
phys:

		// Particles need to be redrawn
		system->needsupdate = true;

		// Reset waterlife so the particle does not time out
		p->waterlife = 0;

		// Applith gravity
		p->yvel += GRAVITY;

		if(p->flags & PFLAGS_DAMAGETANKS)
			do_line(system->scalebmp, (int)p->x, (int)p->y, (int)(p->x+p->xvel), (int)(p->y+p->yvel), TRANSCOLOR, callback_napalmparticle);
		else
			do_line(system->scalebmp, (int)p->x, (int)p->y, (int)(p->x+p->xvel), (int)(p->y+p->yvel), TRANSCOLOR, callback_waterparticle);

		if(float_contactx >= 0 || float_contacty >= 0)
		{
			p->flags |= PFLAGS_FROZEN;
			p->x = lastx;
			p->y = lasty;

			// Find out which way the particle needs to go
			if(p->xvel == 0)
				p->xvel = 1;
			if(particle_parse==0)		// To the right (+ values)
			{
				if(p->xvel < 0)
					p->xvel = -p->xvel;
				particle_parse = 1;
			}
			else
			{
				if(p->xvel >= 0)		// To the left (- values)
					p->xvel = -p->xvel;
				particle_parse = 0;
			}
			return 1;
		}
		else
		{	p->x += p->xvel;
			p->y += p->yvel;
			return 1;
		}
	}
	return 1;
}

particlenode_t*	waterfall_p;
int		waternumactive;			// Number of water pixels still moving around
float	percentwateractive;		// Percentage of water pixels still moving around

int		wateractivetimer;		// Count-down timer. ifthe active percentage
								// is lower than 1.5% for more than 3 seconds
								// than begin either deleting the particles or
								// haulting their movement


int TimeoutWaterParticle(particlenode_t* p, particle_t	*s)
{
	if(p->waterlife <= 3*TICRATE)
		return 0;

	p->flags |= PFLAGS_SUCIDIAL;
	return 0;
}

int StaticWaterFall(particle_t	*ssystem)
{
	particlenode_t	*prev=NULL;
	particlenode_t	*temp=NULL;
	int				particleprocess;
	int				nactive;
	int				ret;

	// Grab the current state of the ground
	clear_to_color(pwater->scalebmp, TRANSCOLOR);
	masked_stretch_blit(ground.bmp, pwater->scalebmp, 0,0, SCREENWIDTH, SCREENHEIGHT, 0,(TANK_LANDSIT/pwater->SCALE_P), pwater->SCALE_W, pwater->SCALE_H);

	particleprocess=2;

	while (particleprocess--)
	{
		clear_to_color(pwater->waterbmp, TRANSCOLOR);

		//clear_to_color(system->waterbmp, TRANSCOLOR);
		waterfall_p = pwater->particles;
		while(waterfall_p)
		{
			waterfall_p->active = 1;

			waterfall_p->Draw(pwater->waterbmp, pwater->SCALE_P);
			waterfall_p = waterfall_p->next;
		}

		prev = NULL;
		waterfall_p = pwater->particles;
		nactive=0;

		while (waterfall_p)
		{
			if(pwater->flags & PSYSTEM_RESETWATERLIFE)
				waterfall_p->waterlife = -1;

			//if(waterfall_p->active > 0)
			ParticleWaterThinker(pwater, waterfall_p);

			// Make sure water dosen't flow into static water. ifit does,
			// the dynamic water must be killed
			if(currentlevel.staticwaterlevel > 0)
			{
				if((waterfall_p->y*pwater->SCALE_P) >= currentlevel.staticwaterlevel)
				{ waterfall_p->life = -1; }
			}

			// We've had less than 3% active particles for more
			// than 3 seconds.
			if(wateractivetimer > 3*TICRATE && waterfall_p->active > 0)
			//if(waterfall_p->waterlife > 3*TICRATE &&
			//	waterfall_p->active > 0)
			{
				ret = TimeoutWaterParticle(waterfall_p, pwater);
				if(ret == -1)
				{
					waterfall_p->life = -1;
				}
				else if(ret == 1)
				{
					waterfall_p->active = 0;
				}
				else
					nactive++;
			}

			else if(waterfall_p->active > 0)
			{
				nactive++;
			}

			//if(OnScreen(waterfall_p->x,
			//FAST_PUTPIXEL(pwater->waterbmp, (int)waterfall_p->x, (int)waterfall_p->y, waterfall_p->color);
			putpixel(pwater->waterbmp, (int)waterfall_p->x, (int)waterfall_p->y, waterfall_p->color);

			if(waterfall_p->life < 0)
			{
				if(prev)
					prev->next = waterfall_p->next;
				else
					pwater->particles = waterfall_p->next;

				temp = waterfall_p;
				waterfall_p = waterfall_p->next;
				pwater->numparticles--;
				delete temp;
			}
			else
			{
				prev = waterfall_p;
				waterfall_p = waterfall_p->next;
			}
		}

		if(pwater->flags & PSYSTEM_RESETWATERLIFE)
			pwater->flags &= ~PSYSTEM_RESETWATERLIFE;
	}

	pwater->needsupdate = true;

	// Same as nactive, only in a static varible
	pwater->numactive = waternumactive = nactive;

	// Calculate the percentage of water pixels still moving around
	percentwateractive = (float)waternumactive/(float)pwater->numparticles;

	// ifthe percentage is below 3% than begin the timer that will
	// eventually delete/halt active particles
	if(percentwateractive <= 0.03)
		wateractivetimer++;
	else
		wateractivetimer=0;		// More than 3% of active particles, reset our timer

	return nactive;
}


int	StaticWaterThinker(void)
{
	if(!pwater)
		return -1;

	if(pwater->checkit <= 0)
		return -1;

	// No water particles to move around
	if(pwater->numparticles <= 0)
		return 0;

	waternumactive = StaticWaterFall(pwater);

	if(waternumactive==0)
		pwater->checkit = 0;

	return waternumactive;
}

void particlenode_t::WaterThinker()
{
}


void	callback_dirtparticle(BITMAP *bmp, int x1, int y1, int color)
{

	if(float_contactx >= 0 && float_contacty >= 0)
		return;

	if(lastx >= 0 && lasty >= 0)
	{
		if(getpixel(bmp, (int)x1, (int)y1) != TRANSCOLOR)
		{
			// Stop checking after this
			float_contactx = x1;
			float_contacty = y1;
		}
	}

	lastx = x1;
	lasty = y1;
}

static int		pdirt_direction=0;

void	ParticleDirtThinker(particle_t*	system, particlenode_t	*p)
{
	float_contactx = float_contacty = -1;
	lastx = lasty = -1;

	p->life--;

	// Do not go outside the boundries of the screenz0rz
	if(p->x >= SCREEN_W ||
		p->x <= 0 ||
		p->y >= SCREEN_H ||
		p->y <= 0)
	{
		p->life = -1;
		return;
	}

	if(p->flags & PFLAGS_FROZEN)
	{
		if(getpixel(system->scalebmp, (int)p->x, (int)p->y+1) != TRANSCOLOR)
		{
			p->yvel = -(p->yvel/2);
			p->flags &= ~PFLAGS_FROZEN;
		}
		else
		{
			p->yvel = -(p->yvel/2);
			//p->xvel = -p->xvel;
			p->flags &= ~PFLAGS_FROZEN;
		}

		p->bounce--;

		return;
	}

	// No gravity? No problem
	if(p->flags & PFLAGS_NOGRAVITY)
	{
		p->x += p->xvel;
		p->y += p->yvel;
	}
	else
	{
		p->yvel += GRAVITY;

		do_line(ground.bmp, (int)p->x, (int)p->y, (int)(p->x+p->xvel), (int)(p->y+p->yvel), TRANSCOLOR, callback_dirtparticle);

		if(float_contactx >= 0 || float_contacty >= 0)
		{
			if(p->bounce > 0)
			{
				p->flags |= PFLAGS_FROZEN;
				p->x = lastx;
				p->y = lasty;
			}
			else
			{
				// Particle will be deleted
				p->life = -1;
			}
		}
		else
		{
			p->x += p->xvel;
			p->y += p->yvel;
		}
	}


	if(p->flags & PFLAGS_USEDESTINATION)
	{
		// Check to see ifdestination particles are within their
		// "bounding" box. ifthey are, delete them
		if(p->x > p->destx &&
			p->x < p->destx+p->destw &&
			p->y > p->desty &&
			p->y < p->desty+p->desth)
		{
			p->life = -1;
			return;
		}
	}

	if(p->flags & PFLAGS_TELBOUNCE)
	{
		if(p->x <= p->telbounce1x || p->x >= p->telbounce2x)
		{
			p->xvel = -p->xvel;
			p->yvel = -p->yvel;
		}
	}
}

//
// particlenode_t::Draw() - Draws one particle in a linked list
//

void particlenode_t::Draw(BITMAP	*bmp, int scale)
{
	if(OnScreen(bmp, x, y) == -1)
		return;

	//FAST_PUTPIXEL(bmp, (int)x, (int)y, color);
	putpixel(bmp, (int)x, (int)y, color);

	this->lastx = x;
	this->lasty = y;
}


//
// particle_t::Draw() - Draws the entire system
//
void particle_t::Draw(int trans, BITMAP	*bmp, int waterlevel)
{
	int				glowopacity=0;
	int				particleflags = -1;
	particlenode_t	*p = particles;

	if(!p && waterlevel <= 0)
		return;

	// Clear the mini-buffer and the big buffer
	clear_to_color(waterbmp, TRANSCOLOR);
	clear_to_color(dbuffer, TRANSCOLOR);

	if(numglowingparticles > 0)
		clear_to_color(glowbmp, TRANSCOLOR);

	topx = SCREENWIDTH;
	topy = SCREENHEIGHT;
	botx = boty = 0;

	while (p)
	{
		// Fill 'particleflags' with the first active varible
		if(particleflags == -1)
			particleflags = p->flags;

		// Attempt to form a rectangle based apon where the particles are
		if(p->x <= topx)
			topx = p->x;
		if(p->y <= topy)
			topy = p->y;
		if(p->x >= botx)
			botx = p->x;
		if(p->y >= boty)
			boty = p->y;

		// Draw onto the waterbmp
		p->Draw(waterbmp, SCALE_P);

		if(p->flags & PFLAGS_GLOWING)
		{
			circlefill(glowbmp, (int)(p->x*SCALE_P), (int)(p->y*SCALE_P), SCALE_P*3, makecol(p->glowr, p->glowg, p->glowb));
			glowopacity = p->glowopacity;
		}

		// Draw to the actual buffer
		circlefill(dbuffer, (int)(p->x*SCALE_P), (int)(p->y*SCALE_P), SCALE_P, p->color);

		p = p->next;

	}

	// Scale to the screen
	topx *= SCALE_P;
	topy *= SCALE_P;
	botx *= SCALE_P;
	boty *= SCALE_P;

	// Take in account the radius of the circles
	if(numglowingparticles > 0)
	{
		topx -= SCALE_P*3;
		topy -= SCALE_P*3;
		botx += SCALE_P*3;
		boty += SCALE_P*3;
	}
	else
	{
		topx -= SCALE_P;
		topy -= SCALE_P;
		botx += SCALE_P;
		boty += SCALE_P;
	}

	// Adjust ifthere is a static water level
	if(waterlevel > 0)
	{
		// Draw it here in the buffer
		rectfill(dbuffer, 0, waterlevel, SCREENWIDTH, SCREENHEIGHT, currentlevel.watercolor);

		topx = 0;
		botx = SCREENWIDTH;
		boty = SCREENHEIGHT;
		if(topy > waterlevel)
			topy = waterlevel;
	}

	// Some standard clipping
	// This will be done later in AddRectangle() but its needed
	// here now since we'll be using the same coordinates to blit onto
	// the buffer
	if(topx < 0) { topx = 0; }
	if(topy < 0) { topy = 0; }

	AddRectangle(bmp, topx, topy, botx-topx, boty-topy);

	if(numglowingparticles > 0)
	{
		// Smaller blitting = faster blitting
		BITMAP	*glowparticles = create_sub_bitmap(glowbmp, topx, topy, (botx-topx), (boty-topy));

		// FAST blitting! Thank you fblend!
		//fblend_trans(glowparticles, bmp, topx, topy, glowopacity);
		set_trans_blender(0, 0, 0, glowopacity);
		draw_trans_sprite(bmp, glowparticles, topx, topy);

		// Free memory
		destroy_bitmap(glowparticles);

	}

	// Transparent pixels?
	if(trans)
	{
		// Smaller blitting = faster blitting
		BITMAP	*transparticles = create_sub_bitmap(dbuffer, topx, topy, (botx-topx), (boty-topy));

		// FAST blitting! Thank you fblend!
		fblend_trans(transparticles, bmp, topx, topy, 100);

		// Free memory
		destroy_bitmap(transparticles);
	}
	else
		masked_blit(dbuffer, bmp, topx,topy,topx,topy, (botx-topx), (boty-topy));





}


// Dirt particles are different, they have a much
// more simpler way of being drawn. That is to say,
// they are drawn directly onto the buffer
//
// Normally, particles are drawn in ParticleDrawer, but
// since we want the dirt particles to appear *behind*
// the explosion graphics, this is called in a different
// spot.

void	DirtDrawer(BITMAP	*bmp)
{
	particlenode_t	*p = pdirt->particles;
	int		x,y;

	// Nothing to draw
	if(!p)
		return;

	while (p)
	{
		if(p->life <= 0)
		{
			p = p->next;
		}
		else
		{
			x = (int)(p->x*pdirt->SCALE_P);
			y = (int)(p->y*pdirt->SCALE_P);

			if(p->flags & PFLAGS_SINGLEPIXEL)
			{
				if(OnScreen(x, y, SCREEN_W-1, SCREEN_H-1) > 0)
				{
					// Single pixel particle for simplicity
					AddRectangle(bmp, x, y, 1, 1, FAST_GETPIXEL(bmp, x, y));

					//putpixel(bmp, x, y, p->color);
					FAST_PUTPIXEL(bmp, x, y, p->color);
				}
			}
			else
			{
				// Store rectangle
				AddRectangle(bmp, x-1, y-1, pdirt->SCALE_P*3, pdirt->SCALE_P*3);

				// Draw to the actual buffer
				circlefill(bmp, x, y, pdirt->SCALE_P, p->color);
			}

			// NEEEEXT!
			p = p->next;
		}
	}
}


//
// Underwater - This is to determine iftanks or missiles are
//				underwater or not
//

bool Underwater(int x, int y)
{
	//int color = getpixel(pwater->dbuffer, (x/pwater->SCALE_P), (y/pwater->SCALE_P));
	int color = getpixel(pwater->dbuffer, x, y);

	// Either its really out of the water or its off the screen.
	if(color == TRANSCOLOR || color == -1)
		return false;
	return true;
}

//
// OnWater - Tracks the x,y coordinates to either to find the surface
//			 of any type of water contained in pwater->waterbmp
//

int	OnWater(int x, int y)
{
	int	bmpx = x;
	int	bmpy = y;

	// We're already underwater, so just keep going up until
	// we've hit land
	if(Underwater(x,y))
	{
		while ((getpixel(pwater->dbuffer, bmpx, bmpy) != TRANSCOLOR) &&
				bmpy >= 0 &&
				bmpx <= SCREENHEIGHT)
		{
			bmpy--;

			// We've gone off screen
			if(!OnScreen(pwater->dbuffer, bmpx, bmpy))
				return -1;
		}

		return (bmpy);
	}
	else	// We've above water, go down until we hit something
	{
		while (getpixel(pwater->dbuffer, bmpx, bmpy) == TRANSCOLOR)
		{
			bmpy++;

			// We've gone off screen
			if(!OnScreen(pwater->dbuffer, bmpx, bmpy))
				return -1;
		}

		return (bmpy);
	}

	return -1;

}


//
// ParticleDrawer - Well, it draws the particles.  =)
//

void	ParticleDrawer(BITMAP *bmp)
{
	//if(psystem->needsupdate)
	{
		psystem->Draw(0, bmp);
		psystem->needsupdate = false;
	}

	//if(pwater->needsupdate)
	{
		pwater->Draw(1, bmp, currentlevel.staticwaterlevel);
		pwater->needsupdate = false;
	}

	// debug
	/*char	str[64];
	float	percentage;

	if(psystem->numparticles == 0)
		percentage = 0;
	else
		percentage = psystem->numactive/psystem->numparticles;
	sprintf(str, "Napalm: %d (%2f active)", psystem->numparticles, percentage);
	textout(buffer, font, str, 5, 40, makecol(0,0,0));

	if(pwater->numparticles > 0)
	{
		percentage = ((float)waternumactive/(float)pwater->numparticles);
		sprintf(str, "Water: %d (%d active: %.5f)", pwater->numparticles, pwater->numactive, percentage);
		textout(buffer, font, str, 5, 55, makecol(0,0,0));
	}*/
}

