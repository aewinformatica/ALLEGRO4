
void	DrawBitmapGroundMask(BITMAP *from, BITMAP	*bmp, int x, int y, int trans, bool usedirty=true);
void	DrawGroundAddition(BITMAP *from, BITMAP	*bmp, int x, int y);
void	DrawGroundSubtract(BITMAP *from, BITMAP	*bmp, int x, int y);
void	DestroyDirt(int x, int y, int rad);

class GROUND
{
public:    
	BITMAP *bmp;
    
	int xstart,ystart;
	int xstop,ystop;

	// For background blending/special effects
	int		specialtics;
	int		specialopacity;
	BITMAP	*specialbmp;

	int		checkit;
	bool	needsupdate;
	bool	falling;
	int		fallenpixels;
    //public:

	void restartcheck(int x1, int y1,int x2, int y2);
	void DrawScene(BITMAP *b);
	void fall(void);
    
};


extern GROUND ground;
