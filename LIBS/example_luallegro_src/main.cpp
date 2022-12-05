/*!
 * \author H8OR
 * \author Edmar
 * \date 2007-02-12-19.03
 */




#include "ScriptInterface.h"
#include <allegro.h>

volatile int	g_speedCounter;


/* */
void timerProc()
{
	g_speedCounter++;
}

END_OF_FUNCTION(timerProc)


/// Initialize Allegro
void Init()
{
	allegro_init();
	set_uformat(U_ASCII);
	install_keyboard();
	install_timer();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
}


/// Destroy Allegro
void Clear()
{
	clear_keybuf();
	allegro_exit();
}


/* */
int main()
{
	Init();

	ScriptInterface *si = ScriptInterface::GetInstance();
	install_int_ex(timerProc, BPS_TO_TIMER(32));

	bool	gameOver = false;
	if (si->DoScript("teste.lua"))
	{	allegro_message("Error: can't load 'teste.lua' script.");
		exit(1);
	}

	while (!gameOver)
	{	while (g_speedCounter > 0)
		{	// Atualiza a lógica
			if (key[KEY_ESC])
				gameOver = true;

			si->Update();

			g_speedCounter--;
		}

		si->Draw();
		blit(si->buffer, screen, 0, 0, 0, 0, si->buffer->w, si->buffer->h);
	}

	Clear();
	return 0;
}
END_OF_MAIN()
