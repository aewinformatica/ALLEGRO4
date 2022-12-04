#include <allegro.h>
#include <stdio.h>
#include "main.h"

//---------------------- MAIN --------------------------//
int main() {
	//Função que faz as inicializações dos hardwares utilizados
	init(); 
	
	int varMenu=1;
	MIDI *musica;
		
	//Carega a música inicio.mid e começa a tocá-la (com loop de repetição)
	musica = load_midi("musicas/inicio.mid");
	play_midi(musica, TRUE);
	
	//Função que exibe a primeira imagem de abertura
	inicio();
	
	//Loop do menu principal do jogo
	while (varMenu) {
		varMenu = menu(); //função Menu exibe a imagem de menu e fica esperando a entrada de uma 'letra' pelo usuario
		switch(varMenu){
			case 1: jogo(); break;  //função jogo exibe uma imagem e deveria permitir a utilização de pause, stop e play, mas isso não foi implementado até agora
			case 2: option(); break; //função option exibe uma outra imagem e deveria
		}
	}
	
	stop_midi(); //para a música que estava tocando
	destroy_midi(musica); //descarrega ela da memória
	sair(); //função saír carrega outra imagem e toca outra música pedindo para apertar 'Esc' para sair do programa
	
	deinit(); //Retira os dominios do Allegro
	return 0;
}
END_OF_MAIN()


//---------------------------------------------------//
//-------Funções-------//

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
	
	/*Parametros da função install_sound:
		- o primeiro indica o controlador de som digital a ser usado pelo Allegro;
		- o segundo indica o controlador de som MIDI;
		- o terceiro existe apenas por motivos de compatibilidade com versões antigas do Allegro, e deve ser ignorado passando-se o valor NULL.
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
//Função do início do jogo
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
//Função do menu principal do jogo
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
//------Função de saída do jogo
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
//Função que deveria poder pausar, parar e tocar novamente o som de uma parte pausada.
void jogo(){
	PALETTE  paleta;
	BITMAP *quadro;
	char flag='1';
	
	quadro = load_bitmap("imagens/jogo.bmp", paleta);
	draw_sprite(screen, quadro, 0, 0);

	clear_keybuf();
	do{
		if(key[KEY_P]){ //caso a música nao estaja tocando dá play
		
		}
		if(key[KEY_B]){ //caso a música esteja tocando dá pause
		
		}
		if(key[KEY_S]){ //caso a música esteja tocando dá stop, e se estiver em pause, retira a 'condição' de pause
		
		}
		if(key[KEY_Q])
			flag = '0';
		clear_keybuf();
	} while(flag != '0');
}



//---------------------------
//Função de opções de som
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
		//Tentei fazer isto agora um pouco antes de te enviar mas não deu muito certo... o volume desce até 0 ou sobe até 255
		get_volume(digi_volume, midi_volume);
		if(key[KEY_U]){ //aumenta o volume da música
			*digi_volume += 25;
			*midi_volume += 25;
			if((*digi_volume) > 255)	*digi_volume = 255; //volume máximo possível
			if((*midi_volume) > 255)	*midi_volume = 255; //volume máximo possível
			set_volume(*digi_volume, *midi_volume);
		}
		if(key[KEY_D]){ //diminui o volume da música
			*digi_volume -= 25;
			*midi_volume -= 25;
			if((*digi_volume) < 0)	*digi_volume = 0; //volume mínimo possível
			if((*midi_volume) < 0)	*midi_volume = 0; //volume mínimo possível
			set_volume(*digi_volume, *midi_volume);
		}
		if(key[KEY_B]) //opção para sair do option
			flag = '0';
		clear_keybuf();
	} while(flag != '0');

}
