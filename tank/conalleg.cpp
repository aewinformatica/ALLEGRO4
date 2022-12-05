/*

	conalleg

	a console_c derivate for rendering on allegro bitmaps using the default allegro font

*/

//allegro includes
#include <allegro.h>			

//header include
//#include "conalleg.h"
#include "scorch.h"

//your global console handle (probably you won't need more than one)
conalleg_c *con = 0;

//function for creating the global console handle. returns true of ok, false if console already created
bool create_console(int size) {
	if (con)
		return false;
	else {
		con = new conalleg_c(size);
		return true;
	}
}

//function for deleting the global console handle. if console already deleted, does nothing
void delete_console() {
	if (con) {
		delete con;
		con = 0;
	}
}


//
// Init_Console - Startup console
//

bool	console_active;
BITMAP	*conbuffer;

void	Init_Console(void)
{
	// Create the console first
	create_console();

	conbuffer = create_bitmap(SCREENWIDTH,SCREENHEIGHT/2);
	clear_to_color(conbuffer, TRANSCOLOR);

	con->set_bitmap(conbuffer);

	console_active = false;
}

void	DrawConsole(BITMAP	*b)
{
	AddRectangle(b, 0,0, conbuffer->w, conbuffer->h);
	con->just_set_bitmap(b);
	con->draw_page(true);
}

void	ConsoleInput(void)
{
	con->read_keyboard(KEY_TILDE); 
}

void	ToggleConsole(bool	conOn)
{
	if (console_active)
	{
		console_active = false;
		if (gamestate == GS_INGAME)
			ground.needsupdate = true;
	}
	else
		console_active = true;
}


void	ShutDown_Console(void)
{
	destroy_bitmap(conbuffer);

	delete_console();
}
