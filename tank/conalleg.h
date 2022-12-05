/*

	conalleg

	a console_c derivate for rendering on allegro bitmaps using the default allegro font

*/

#ifndef _CONALLEG_H_
#define _CONALLEG_H_

//zig engine base console
#include "console.h"

//your global console handle (probably you won't need more than one)
class conalleg_c;
extern conalleg_c *con;

// [ShadowFang] - need this
extern	BITMAP	*conbuffer;

//function for creating the global console handle. returns true of ok, false if console already created
//size: size of console, in kilobytes
bool create_console(int size = 64);

//function for deleting the global console handle. if console already deleted, does nothing
void delete_console();

//allegro console class
class conalleg_c : public console_c {
public:

	//ctor - passa bitmap: colunas e linhas do console expandem para ocupar todo o bitmap!
	// se quiser passar uma parte da tela apenas, fornecer um sub-bitmap.
	conalleg_c(BITMAP *b, int size) : console_c(size, (b->w-20)/8, (b->h-20)/10)
	{
		set_bitmap(b);
		set_font(font);	// [Sean] - set default font
	}

	//ctor sem bitmap : vai precisar passar o bitmap depois
	conalleg_c(int size) : console_c(size)
	{
		set_bitmap(0);
		set_font(font);	// [Sean] - set default font
	}

	// muda o bitmap para desenhos. 0 = nenhum
	void set_bitmap(BITMAP *b)
	{
		bmp = b;
		if (bmp)
		{

			// *** CHANGE THIS TO ANY COLORS YOU LIKE BETTER ***
			bcol = makecol(50,50,50);
			tcol = makecol(255,255,255);
			pcol = makecol(0,255,0);

			text_mode(-1);
			enable_display((bmp->w - 20) / 8, (bmp->h - 20) / 10);
			draw_page(true);
		}
		else
			disable_display();
	}

	virtual void set_font(FONT *f)
	{
		cfont = f;
		cfont_h = text_height(cfont);
	}

	//muda apenas o bitmap (o bitmap eh igual ao anterior em altura e largura,
	// mudando apenas o ponteiro mesmo)
	//usar antes de chamar draw_page(), se estiver usando um esquema de PAGE FLIPPING
	void just_set_bitmap(BITMAP *b)
	{
		bmp = b;
	}


	// read allegro keyboard. treats PAGE UP, PAGE DOWN and END as console
	// scroll-control keys
	// returns 'true' if the special_key (by scancode) has been hit (used it to
	// implement a key that toggles the console showing on/off)
	bool read_keyboard(int special_key = -1);

protected:

	//bgcolor / text color for drawing
	int bcol, tcol, pcol;

	// console command-line processing
	//	cmdstr: command line typed by console user (or maybe some automated script)
	//
	// *** IMPLEMENT THIS METHOD SOMEWHERE IN YOUR PROGRAM TO DO YOUR OWN COMMAND PROCESSING! ***
	//
	virtual void interprete_command(char* cmdstr);

	// render a console line. line: line offset onscreen. buf: line of text.
	virtual void draw_line(int line, char *buf);

  // called when console must be redrawn.
	virtual void clear();

  // called when console input must be redrawn.
	virtual void clear_prompt();

	// bitmap to draw & font to use
	BITMAP *bmp;
	FONT   *cfont;
	int	   cfont_h;
};

#endif
