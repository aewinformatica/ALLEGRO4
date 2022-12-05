#define	PSYSTEM_NORMAL			0
#define	PSYSTEM_RESETWATERLIFE	1

#define	PFLAGS_STATIC			0
#define	PFLAGS_WATER			1
#define	PFLAGS_NOGRAVITY		2
#define	PFLAGS_FROZEN			4
#define	PFLAGS_DAMAGETANKS		8
#define	PFLAGS_DIRT				16
#define	PFLAGS_NAPALMEFFECT1	32
#define	PFLAGS_NAPALMEFFECT2	64
#define	PFLAGS_SUCIDIAL			128
#define	PFLAGS_SLOWDOWN			256
#define	PFLAGS_TANKSPARK		512
#define	PFLAGS_GLOWING			1024
#define	PFLAGS_GLOWPULSE		2048
#define	PFLAGS_LIQUID			4096
#define	PFLAGS_NOLIQUIDCLIP		8192
#define	PFLAGS_SINGLEPIXEL		16384
#define	PFLAGS_USEDESTINATION	32768		// Particles will be deleted once they reach their destination
#define	PFLAGS_TELBOUNCE		65536		// Bounce between the two electric rods

extern	int						num_particles;

#define M_PI					3.14159265358979323846

#define		FAST_PUTPIXEL(bitmap, x,y,color)	(((unsigned short*)(bitmap->line[(y)]))[(x)]=(color))
#define		FAST_GETPIXEL(bitmap,x,y)			(((unsigned short*)(bitmap)->line[y])[x])

class	particlenode_t 
{
	public:
		//particlenode_t(int x=0, int y=0, float angle=0, float power=0, int color=0, int life=60, int flags=0, float gravity=0);
		int		id;
		float	x, lastx;
		float	y, lasty;
		int		color, colorr, colorg, colorb, colorphase;
		int		glowr,glowg,glowb,glowopacity;
		int		life;
		int		flags;
		int		active;								// Still moving around?
		float	damagecounth,damagecounte,damagecountb;
		int		damagelife;
		int		damagedtank[MAX_TANKS];				// Keep track of what tanks we've made contact with (naplam..etc)
		int		bounce;
	
		float	gravity;
		float	xvel;
		float	yvel;
		int		destx,desty;						// Particle destination. When land particles reach their
		int		destw,desth;						// destination (bounding box), delete them from existance
		int		telbounce1x,telbounce1y;			// Twin electric lance bouncing
		int		telbounce2x,telbounce2y;
		float	angle;
		int		burnmark;							// Have we left a burn mark yet?

		int		waterlife;							// How long has the particle been active
		int		watercount;							// How many pixels the particle needs to travel before hitting something

		particlenode_t	*next;
		particlenode_t	*prev;

		
		void	Kill(void);
		void	WaterThinker();						// Thinker for water particles
		void	ParticleThinker();					// Thinker for normal "test" particles
		void	Draw(BITMAP	*bmp, int scale);		// Draw the particle onto waterbmp
};

class	particle_t {

	public:

		particle_t(int scale);
		~particle_t(void);
		
		int					SCALE_W, SCALE_H, SCALE_P;
		int					flags;
		BITMAP				*glowbmp;
		BITMAP				*waterbmp;
		BITMAP				*scalebmp;
		BITMAP				*dbuffer;

		bool				needsupdate;

		int					IsEmpty();							// Is the class empty?
		particlenode_t *	InsertParticle(int x=0,				// Insert a new particle
											int y=0, 
											float angle=0, 
											float power=0, 
											int color=0, 
											int life=60, 
											int flags=0, 
											float gravity=0,
											float	damageh=0,
											float damagee=0,
											float damageb=0);
		void				RunThinkers();						// Function that determines what thinker to use
		void				callback_waterparticle(BITMAP *bmp, int x1, int y1, int color);

		void				RunStaticThinkers();								// Thinker for static water
		void				Draw(int trans, BITMAP *bmp, int waterlevel=0);		// Draws all the particles on the bitmap (bmp)
		particlenode_t*		Delete(particlenode_t *p);							// Deletes one particle
		void				DeleteAll();										// Deletes all particles

		particlenode_t		*particles;
		int					numparticles;										// Total number of particles
		int					numglowingparticles;								// Number of glowing particles
		int					numactive;											// How many particles are still moving around
		int					checkit;											// Do we need to run the thinker?

		// The below varibles are used for dirty rectangles
		// and try to minimize the amount of drawing that needs
		// to be done to the buffer
		int					topx, topy;
		int					botx, boty;		// That should form our rectangle
};

extern			particle_t	*psystem, *pwater, *pdirt;

void						NewParticle(int x=0, int y=0, float angle=0, float power=0, int color=0, int life=60, int flags=0, float gravity=0, float damageh=0, float damagee=0, float damageb=0);
particlenode_t	*			NewNapalmParticle(int x=0, int y=0, float angle=0, float power=0, int color=0, int life=60, int flags=0, float gravity=0, float damageh=0, float damagee=0, float damageb=0, int dlife=0);
void						DropStaticWater(BITMAP	*grnd, int x, int y, int color, int		numparticles);

int				ParticleWaterThinker(particle_t*	system, particlenode_t	*p);
void			ParticleDirtThinker(particle_t*	system, particlenode_t	*p);

void			FlattenStaticParticles(void);
int				StaticWaterFall(particle_t	*system);
int				StaticWaterThinker(void);
void			ParticleThinkers(void);
void			ParticleDrawer(BITMAP	*bmp);
void			DirtDrawer(BITMAP	*bmp);
void			InitParticles(void);
void			ShutdownParticles(void);
//void			DealOutTankDamage(particlenode_t	*p, int scale);

bool			Underwater(int x, int y);
int				OnWater(int x, int y);

int				OnScreen(int x, int y, int w, int h);
int				OnScreen(BITMAP	*bmp, int x, int y);

void			BurnGround(BITMAP	*bmp, int x, int y, int size=7, int opacity=50);
void			ConcreteGround(BITMAP	*bmp, int x, int y, int size=14 );
void			DudAllParticles(void);

