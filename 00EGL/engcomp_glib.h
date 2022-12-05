/*
Biblioteca de Desenvolvimento de Jogos do Curso de Engenharia da Computação da UNISINOS
by Farlei J. Heinen (30/05/2006) - farleih@gmail.com

Funções de Colisão obtidas de: Ivan Baldo - lubaldo@adinet.com.uy (PPCol Library)

10/05/2007
Versão atualizada para suporte ao Dev-Cpp com Allegro 4.2

Versão: 1.2c

*/
#include "library_version.h"
#include <string>

using namespace std;

bool egl_init = false;
BITMAP *tela = NULL;

int res_x, res_y;
bool egl_debug=false;
string msg_erro;

bool egl_inicializar(int w, int h)
{
	allegro_init();

	install_keyboard();
	install_mouse();
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);

	set_color_depth(32);
#ifdef __EGL_DEBUG_MODE
	set_gfx_mode(GFX_DIRECTX_WIN,w,h,0,0);
#else
	set_gfx_mode(GFX_AUTODETECT,w,h,0,0);
#endif
	res_x = w; res_y = h;

	tela = create_bitmap(w,h);
	clear(tela);

	egl_init = true;
	return true;
}

void egl_finalizar()
{
	if(tela) destroy_bitmap(tela);
}

void egl_desenha_frame(bool limpa = true, bool sync = false)
{
	if(!egl_init) return;
	if(egl_debug) textout_ex(tela,font,msg_erro.c_str(),0,0,makecol(255,255,255),-1);

	if(sync) vsync();
	blit(tela, screen, 0, 0, 0, 0, res_x, res_y);
	if(limpa) 
		clear(tela);
}

void egl_texto(string txt, int x, int y)
{
	if(!egl_init) return;
	textout_ex(tela,font,txt.c_str(),x,y,makecol(0,0,0),-1);
}

void egl_sleep(int milisec)
{
	rest(milisec);
}

void egl_pixel(int x,int y, int vermelho, int verde, int azul)
{
	if(!egl_init) return;
	putpixel(tela,x,y,makecol(vermelho, verde, azul));
}

void egl_linha(int x1,int y1, int x2,int y2, int vermelho, int verde, int azul)
{
	if(!egl_init) return;
	line(tela, x1,y1,x2,y2,makecol(vermelho, verde, azul));
}

void egl_retangulo(int x1,int y1, int x2,int y2, int vermelho, int verde, int azul)
{
	if(!egl_init) return;

	int cor = makecol(vermelho, verde, azul);
	line(tela, x1,y1,x2,y1,cor);
	line(tela, x2,y1,x2,y2,cor);
	line(tela, x2,y2,x1,y2,cor);
	line(tela, x1,y2,x1,y1,cor);
}

void egl_erro(string mensagem)
{
	msg_erro += (" " + mensagem);
}

void read_string(char *str_to, int size, int x, int y)
{
	// Little example - By Kronoman - Taking a string from keyboard
	int cur_pos = 0; // position of caret
	int the_key = 0;
	int i;

	for (i = 0; i < size; i++)
		str_to[i] = '\0'; // 'clean' the string


	while (the_key>>8 != KEY_ENTER)
	{
		the_key = readkey();

		if ((the_key & 0xff) >= ' ') // get only valid chars
		{
			str_to[cur_pos] = the_key & 0xff;
			cur_pos++;
			if (cur_pos > size-2) cur_pos = size-2;

		}

		if (the_key >> 8 == KEY_BACKSPACE)
		{
			str_to[cur_pos] = '\0'; // chop the string
			cur_pos --;
			if (cur_pos < 0) cur_pos = 0;
		}

		// lame redraw (use double buffer, whatever)
		//clear(tela);
		//textout(tela,font, str_to, x,y, makecol(255,255,255));
		clear(screen);
		textout(screen,font, str_to, x,y, makecol(255,255,255));
	}

}
void egl_ler_string_teclado(string &buffer, int tamanho_buffer, int x, int y)
{
	char *ptr = new char[tamanho_buffer];
	read_string(ptr, tamanho_buffer - 1, x, y);
	buffer = string(ptr);
	delete[] ptr;
}

class imagem
{
protected:
	BITMAP* bmp[32];
	int index;
	int curr;
	int tempo;
	int vel;


public:
	imagem()
	{
		index = -1;
		curr = 0;
		vel = tempo = 30;
		for(int i =0;i < 32;i++) bmp[i] = NULL;
	}
	~imagem()
	{
		for(int i =0;i <= index;i++)
			if(bmp[i]) destroy_bitmap(bmp[i]);
	}

	BITMAP* obter_bitmap()
	{
		if(!egl_init) return NULL;
		if(index < 0) return NULL;

		return bmp[curr];
	}

	void obter_tamanho(int &w, int &h)
	{
		if(!egl_init) return;
		if(index < 0) return;

		w = bmp[0]->w;
		h = bmp[0]->h;
	}

	void setar_tempo_animacao(int veloc)
	{
		vel = tempo = veloc;
	}

	bool carregar(string arquivo)
	{
		if(!egl_init) return false;

		index++;
		if(index >= 32) return false;

		bmp[index] = load_bmp(arquivo.c_str(),NULL);
		if(!bmp[index]) 
		{
			index--;
			egl_erro("Erro carregando arquivo: " + arquivo);
			egl_debug = true;
			return false;
		}

		return true;
	}

	bool carregar(string arquivo, int x, int y, int largura, int altura)
	{
		if(!egl_init) return false;

		index++;
		if(index >= 32) return false;

		bmp[index] = create_bitmap(largura,altura);
		BITMAP* bmp_temp = load_bmp(arquivo.c_str(),NULL);

		if(!bmp_temp) 
		{
			index--;
			egl_erro("Erro carregando arquivo: " + arquivo);
			egl_debug = true;
			return false;
		}

		blit(bmp_temp, bmp[index], x,y,0,0,largura,altura);
		destroy_bitmap(bmp_temp);

		return true;
	}

	bool desenha(int x, int y, bool borda=false)
	{
		if(!egl_init) return false;
		if(index < 0) return false;

		draw_sprite(tela,bmp[curr],x,y);

		if(borda) egl_retangulo(x,y,x+bmp[curr]->w,y+bmp[curr]->h,255,255,255);

		tempo--;
		if(!tempo)
		{
			curr++;
			tempo = vel;
			if(curr > index) 
			{
				curr = 0;
				return false;
			}
		}
		return true;
	}

	bool desenha_rotacionado(int x, int y, long rotacao )
	{
		if(!egl_init) return false;
		if(index < 0) return false;

		rotacao = rotacao%256;

		rotate_sprite(tela,bmp[curr],x,y,itofix(rotacao));

		tempo--;
		if(!tempo)
		{
			curr++;
			tempo = vel;
			if(curr > index) 
			{
				curr = 0;
				return false;
			}
		}
		return true;
	}

	bool desenha_espelhado(int x, int y, bool horiz=true, bool vert=false )
	{
		if(!egl_init) return false;
		if(index < 0) return false;


		if(horiz && vert)
		{
			draw_sprite_vh_flip(tela,bmp[curr],x,y);
		}
		else
		{
			if(horiz) draw_sprite_h_flip(tela,bmp[curr],x,y);
			if(vert) draw_sprite_v_flip(tela,bmp[curr],x,y);
		}

		tempo--;
		if(!tempo)
		{
			curr++;
			tempo = vel;
			if(curr > index) 
			{
				curr = 0;
				return false;
			}
		}
		return true;
	}

	bool colide(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
	{
		return (!( ((x1)>=(x2)+(w2)) || 
			((x2)>=(x1)+(w1)) || 
			((y1)>=(y2)+(h2)) || 
			((y2)>=(y1)+(h1)) ));
	}


	bool colide(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, imagem &sprite2)
	{
		if(!egl_init) return false;
		if(index < 0) return false;

		if(!colide(x1,y1,w1,h1,x2,y2,w2,h2)) return false;

		int dx1, dx2, dy1, dy2; //We will use this deltas...
		int fx,fy,sx1,sx2; //Also we will use this starting/final position variables...
		int maxw, maxh; //And also this variables saying what is the maximum width and height...
		int depth; //This will store the color depth value...
		char CHARVAR; //We will use these to store the transparent color for the sprites...
		short SHORTVAR;
		long LONGVAR;
		BITMAP *spr1;
		BITMAP *spr2;
		spr1 = bmp[curr];
		spr2 = sprite2.obter_bitmap();

		//First we need to see how much we have to shift the coordinates of the sprites...
		if(x1>x2) {
			dx1=0;      //don't need to shift sprite 1.
			dx2=x1-x2;  //shift sprite 2 left. Why left? Because we have the sprite 1 being on the right of the sprite 2, so we have to move sprite 2 to the left to do the proper pixel perfect collision...
		} else {
			dx1=x2-x1;  //shift sprite 1 left.
			dx2=0;      //don't need to shift sprite 2.
		}
		if(y1>y2) {
			dy1=0;
			dy2=y1-y2;  //we need to move this many rows up sprite 2. Why up? Because we have sprite 1 being down of sprite 2, so we have to move sprite 2 up to do the proper pixel perfect collision detection...
		} else {
			dy1=y2-y1;  //we need to move this many rows up sprite 1.
			dy2=0;
		}

		//Then, we have to see how far we have to go, we do this seeing the minimum height and width between the 2 sprites depending in their positions:
		if(w1-dx1 > w2-dx2) {
			maxw=w2-dx2;
		} else {
			maxw=w1-dx1;
		}
		if(h1-dy1 > h2-dy2) {
			maxh=h2-dy2;
		} else {
			maxh=h1-dy1;
		}
		maxw--;
		maxh--;

		fy=dy1;
		fx=dx1;
		dy1+=maxh;
		dy2+=maxh;
		sx1=dx1+maxw;
		sx2=dx2+maxw;

		depth=bitmap_color_depth(spr1); //Get the bitmap depth...

		if(depth==8) {
			CHARVAR=bitmap_mask_color(spr1); //Get the transparent color of the sprites...
			for(; dy1>=fy; dy1--,dy2--) { //Go through lines...
				for(dx1=sx1,dx2=sx2; dx1>=fx; dx1--,dx2--) { //Go through the X axis...
					if((spr1->line[dy1][dx1]!=CHARVAR) && (spr2->line[dy2][dx2]!=CHARVAR)) return true; //Both sprites don't have transparent color in that position, so, theres a collision and return collision detected!
				}
			}
		} else {
			if(depth==16 || depth==15) {
				SHORTVAR=bitmap_mask_color(spr1); //Get the transparent color of the sprites...
				for(; dy1>=fy; dy1--,dy2--) { //Go through lines...
					for(dx1=sx1,dx2=sx2; dx1>=fx; dx1--,dx2--) { //Go through the X axis...
						if(( ((short *)spr1->line[dy1])[dx1]!=SHORTVAR) && ( ((short *)spr2->line[dy2])[dx2]!=SHORTVAR)) return true; //Both sprites don't have transparent color in that position, so, theres a collision and return collision detected!
					}
				}
			} else {
				if(depth==32) {
					LONGVAR=bitmap_mask_color(spr1); //Get the transparent color of the sprites...
					for(; dy1>=fy; dy1--,dy2--) { //Go through lines...
						for(dx1=sx1,dx2=sx2; dx1>=fx; dx1--,dx2--) { //Go through the X axis...
							if(( ((long *)spr1->line[dy1])[dx1]!=LONGVAR) && ( ((long *)spr2->line[dy2])[dx2]!=LONGVAR)) return true; //Both sprites don't have transparent color in that position, so, theres a collision and return collision detected!
						}
					}
				} else {
					if(depth==24) {
						CHARVAR=bitmap_mask_color(spr1)>>16; //if the order is RGB, this will contain B...
						SHORTVAR=bitmap_mask_color(spr1)&0xffff; //if the order is RGB, this will contain GR...
						for(; dy1>=fy; dy1--,dy2--) { //Go through lines...
							for(dx1=sx1,dx2=sx2; dx1>=fx; dx1--,dx2--) { //Go through the X axis...
								if( (*((short *)(spr1->line[dy1]+(dx1)*3))!=SHORTVAR) && (spr1->line[dy1][(dx1)*3+2]!=CHARVAR)  &&  (*((short *)(spr2->line[dy2]+(dx2)*3))!=SHORTVAR) && (spr2->line[dy2][(dx2)*3+2]!=CHARVAR) ) return true; //Both sprites don't have transparent color in that position, so, theres a collision and return collision detected!
								//I have tryed to avoid the above multiplications but it seems that GCC optimizes better than I :-))
							}
						}
					}
				}
			}
		}
		//If we have reached here it means that theres not a collision:
		return false; //Return no collision.
	}
};

class fundo
{
protected:
	BITMAP *bmp;

public:
	fundo(){bmp = NULL;}
	~fundo(){if(bmp) destroy_bitmap(bmp);}

	bool carregar(string arquivo)
	{
		if(!egl_init) return false;
		if(bmp) destroy_bitmap(bmp);

		bmp = load_bmp(arquivo.c_str(),NULL);
		if(!bmp) return false;

		return true;
	}

	void desenha(int x, int y)
	{
		if(!egl_init) return;
		if(!bmp) return;

		blit(bmp,tela,x,y,0,0,res_x,res_y);
	}

};


class som
{
protected:
	SAMPLE *smp;

	int volume;
	int posicao;
	int frequencia;

public:
	som()
	{
		smp = NULL;

		volume = 255;
		posicao = 128;
		frequencia = 1000;
	}
	~som(){if(smp) destroy_sample(smp);}

	bool carregar(string arquivo)
	{
		if(!egl_init) return false;
		if(smp) destroy_sample(smp);

		smp = load_wav(arquivo.c_str());
		if(!smp) 
		{
			egl_erro("Erro carregando arquivo: " + arquivo);
			egl_debug = true;
			return false;
		}

		return true;
	}
	void tocar(int repetir = 0)
	{
		if(!smp) return;
		play_sample(smp,volume,posicao,frequencia,repetir);
	}

	void parar()
	{
		if(!smp) return;
		stop_sample(smp);
	}

	void ajustar(int vol, int pan=128, int freq=1000, int loop=0)
	{
		if(!smp) return;
		adjust_sample(smp, vol,  pan, freq, loop);

		volume = vol;
		posicao = pan;
		frequencia = freq;
	}
};

