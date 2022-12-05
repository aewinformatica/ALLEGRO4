// drectangles.h



class	drect
{
	public:
		
		// Generic varibles
		int		x,y,w,h;
		int		color;
		int		singlepixel;
		BITMAP	*buf;
		int		needstodie;
		
		// Linked list stuff
		drect	*next;
};


extern	drect	*drectangles;
extern	int		numdrects;

drect*		AddRectangle(BITMAP *sub, int x, int y, int w, int h, int color=-1);
void		RestoreScreen(BITMAP *b);
void		DirtyDrawer(void);
