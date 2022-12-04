#include <allegro.h>
#include <stdio.h>
#include "main.h"

//---------------------- MAIN --------------------------//
int main() {
	//Fun��o que faz as inicializa��es dos hardwares utilizados
	init(); 
	
	int varMenu=1;
	MIDI *musica;
		
	//Carega a m�sica inicio.mid e come�a a toc�-la (com loop de repeti��o)
	musica = load_midi("musicas/inicio.mid");
	play_midi(musica, TRUE);
	
	//Fun��o que exibe a primeira imagem de abertura
	inicio();
	
	//Loop do menu principal do jogo
	while (varMenu) {
		varMenu = menu(); //fun��o Menu exibe a imagem de menu e fica esperando a entrada de uma 'letra' pelo usuario
		switch(varMenu){
			case 1: jogo(); break;  //fun��o jogo exibe uma imagem e deveria permitir a utiliza��o de pause, stop e play, mas isso n�o foi implementado at� agora
			case 2: option(); break; //fun��o option exibe uma outra imagem e deveria
		}
	}
	
	stop_midi(); //para a m�sica que estava tocando
	destroy_midi(musica); //descarrega ela da mem�ria
	sair(); //fun��o sa�r carrega outra imagem e toca outra m�sica pedindo para apertar 'Esc' para sair do programa
	
	deinit(); //Retira os dominios do Allegro
	return 0;
}
END_OF_MAIN()


//---------------------------------------------------//
//-------Fun��es-------//

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	
	/*Parametros da fun��o install_sound:
		- o primeiro indica o controlador de som digital a ser usado pelo Allegro;
		- o segundo indica o controlador de som MIDI;
		- o terceiro existe apenas por motivos de compatibilidade com vers�es antigas do Allegro, e deve ser ignorado passando-se o valor NULL.
	*/
	if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)){
		allegro_message(allegro_error);
		exit(-1);
	}
	/* add other initializations here */
}

//---------------------------------------------
void deinit() {
	clear_keybuf();
	allegro_exit();
	/* add other deinitializations here */
}



//------------------
//Fun��o do in�cio do jogo
void inicio(){
	PALETTE  paleta;
	BITMAP *quadro;

	quadro = load_bitmap("imagens/inicio.bmp", paleta);
	if (!quadro)	allegro_message(allegro_error);
	draw_sprite(screen, quadro, 0, 0);
	fade_in (paleta, 1);
	
	destroy_bitmap(quadro);
	fade_out (1);
	clear(screen);
}


//--------------------
//Fun��o do menu principal do jogo
int menu(){
	PALETTE  paleta;
	BITMAP *quadro;
	char flag='f';

	quadro = load_bitmap("imagens/menu.bmp", paleta);
	if (!quadro)	allegro_message(allegro_error);
	draw_sprite(screen, quadro, 0, 0);
	
	clear_keybuf( );
	do{
		if(key[KEY_1])	return 1;
		if(key[KEY_2])	return 2;
		if(key[KEY_0])	return 0;	
	}while('0' == '0');
	destroy_bitmap(quadro);
	clear(screen);
		
}



//------------------------------
//------Fun��o de sa�da do jogo
void sair(){
	PALETTE  paleta;
	BITMAP *quadro;
	MIDI *musica;
	
	set_volume(255, 255);
	quadro = load_bitmap("imagens/final.bmp", paleta);
	draw_sprite(screen, quadro, 0, 0);
	musica = load_midi("musicas/fim.mid");
	if (!musica)	allegro_message(allegro_error);
	play_midi(musica, FALSE);

	clear_keybuf();
	while(!key[KEY_ESC]);

	stop_midi();
	destroy_midi(musica);
	
}

//---------------
//Fun��o que deveria poder pausar, parar e tocar novamente o som de uma parte pausada.
void jogo(){
	PALETTE  paleta;
	BITMAP *quadro;
	char flag='1';
	
	quadro = load_bitmap("imagens/jogo.bmp", paleta);
	draw_sprite(screen, quadro, 0, 0);

	clear_keybuf();
	do{
		if(key[KEY_P]){ //caso a m�sica nao estaja tocando d� play
		
		}
		if(key[KEY_B]){ //caso a m�sica esteja tocando d� pause
		
		}
		if(key[KEY_S]){ //caso a m�sica esteja tocando d� stop, e se estiver em pause, retira a 'condi��o' de pause
		
		}
		if(key[KEY_Q])
			flag = '0';
		clear_keybuf();
	} while(flag != '0');
}



//---------------------------
//Fun��o de op��es de som
void option(){
	PALETTE  paleta;
	BITMAP *quadro;
	char flag='1';
	int *digi_volume, *midi_volume; //Volume de sons digitais e midi's
	
	quadro = load_bitmap("imagens/option.bmp", paleta);
	draw_sprite(screen, quadro, 0, 0);

	digi_volume = (int *) malloc(1 * sizeof(int));
	midi_volume = (int *) malloc(1 * sizeof(int));
	clear_keybuf();
	do{
		//Tentei fazer isto agora um pouco antes de te enviar mas n�o deu muito certo... o volume desce at� 0 ou sobe at� 255
		get_volume(digi_volume, midi_volume);
		if(key[KEY_U]){ //aumenta o volume da m�sica
			*digi_volume += 25;
			*midi_volume += 25;
			if((*digi_volume) > 255)	*digi_volume = 255; //volume m�ximo poss�vel
			if((*midi_volume) > 255)	*midi_volume = 255; //volume m�ximo poss�vel
			set_volume(*digi_volume, *midi_volume);
		}
		if(key[KEY_D]){ //diminui o volume da m�sica
			*digi_volume -= 25;
			*midi_volume -= 25;
			if((*digi_volume) < 0)	*digi_volume = 0; //volume m�nimo poss�vel
			if((*midi_volume) < 0)	*midi_volume = 0; //volume m�nimo poss�vel
			set_volume(*digi_volume, *midi_volume);
		}
		if(key[KEY_B]) //op��o para sair do option
			flag = '0';
		clear_keybuf();
	} while(flag != '0');

}
