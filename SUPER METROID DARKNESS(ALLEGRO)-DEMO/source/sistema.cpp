#include "sistema.h"

void BotaoFechar(void) 
{ 
   FecharJogo = TRUE; 
} 

void inicia (void)
{
	allegro_init();
	install_timer();
	install_mouse();
	alpng_init();
	algif_init();
	set_alpha_blender();//transparencia nas imagens
    set_uformat(U_ASCII); // acentuação
    set_window_title("METROID DARKNESS                                                                                       WAGNER G JESUS(2018)");
	set_color_depth(COLOR_BITS);
    set_close_button_callback(BotaoFechar);  
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,MAX_X,MAX_Y,0,0) < 0)
	{
	    textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "Erro ao inicializar o modo grafico.\n");
		while (!keypressed()) ;
		exit(0);
	}

	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
	{
     textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "Erro ao inicializar o som.\n");
		while (!keypressed()) ;
		exit(0);
	}
	set_volume(250, 250);
	set_display_switch_mode(SWITCH_BACKGROUND);//continuar tocando musica ao minimizar

}
void finaliza() 
{
    stop_midi(); 
	clear_keybuf();
	remove_keyboard(); 
	remove_mouse(); 
	remove_timer();
	allegro_exit();
	/* add other deinitializations here */
}

