#include<allegro.h>

int  timer = 0;
void tempo(){timer++;}

int main(){
	
	//inicialização
	allegro_init();
	install_keyboard();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	set_window_title("Mega Menu");
	
	//Variáveis
	int short chx  =  130, chy = 220 , time = 0, som = 0, go = 0, delay;
	int short px[3]= {130,  330, 530}, py[3]= {50, 220, 390};
	
	//bitmaps
	BITMAP *menu  = load_bitmap("menu.bmp" , NULL);
	BITMAP *chave = load_bitmap("chave.bmp", NULL);
	BITMAP *buff  = create_bitmap(      800, 600 );
	
	//SONS
	SAMPLE *select = load_sample("select.wav");
	SAMPLE *intro  = load_sample("intro.wav" );
	SAMPLE *buble  = load_sample("buble.wav" );
	
	//funções
	install_int_ex(tempo, MSEC_TO_TIMER(150));
	
	//Introdução
	play_sample(intro, 255, 128, 1000, 1);
	
	while(!key[KEY_ESC]){
		
		//entradas
		if(!go){
				if(key[KEY_RIGHT]){if(chx == px[0]){chx = px[1];som = 1;} else{if(chx == px[1]){chx = px[2];som = 1;}}}
				 
				if(key[KEY_DOWN ]){if(chy == py[0]){chy = py[1];som = 1;} else{if(chy == py[1]){chy = py[2];som = 1;}}}
				
				if(key[KEY_LEFT ]){if(chx == px[2]){chx = px[1];som = 1;} else{if(chx == px[1]){chx = px[0];som = 1;}}}
				
				if(key[KEY_UP   ]){if(chy == py[2]){chy = py[1];som = 1;} else{if(chy == py[1]){chy = py[0];som = 1;}}}
				
				if(key[KEY_SPACE]){stop_sample(intro);play_sample(buble, 255, 128, 1000, 0);go = 50;}
		
			   if(timer > 2) draw_sprite(buff, chave, chx, chy);
			   if(timer > 4){draw_sprite(buff, chave, -500, chy); timer = 0;}
		}	   
		//relógio
		delay = timer;
		
		if(go > 1)
		{
			if(go == 43 or go == 45 or go == 47 or go == 49)menu = load_bitmap("menu2.bmp", NULL);
			if(go == 42 or go == 44 or go == 46 or go == 48)menu = load_bitmap("menu.bmp" , NULL);
			if(go == 40){menu = load_bitmap("buble1.bmp", NULL);}
			if(go == 37){menu = load_bitmap("buble2.bmp", NULL);}	  
	      if(go == 33){menu = load_bitmap("buble3.bmp", NULL);}    
			if(go == 28){menu = load_bitmap("buble4.bmp", NULL);}	  
	      if(go ==  2){menu = load_bitmap("buble5.bmp", NULL);} 	 
         go--;
		}
		if(som){play_sample(select, 255, 128, 1000, 0); som = 0;}
		
		draw_sprite(screen, buff, 0, 0);
		clear(buff);
		draw_sprite(buff,  menu, 0, 0);
		
		while(timer == delay ){}			
	}
	
	destroy_bitmap(buff  );
	destroy_bitmap(menu  );
	destroy_bitmap(chave );
	destroy_sample(select);
	destroy_sample(intro );
	
	return 0;	
}

END_OF_MAIN();




