#ifndef CANIM_H
#define CANIM_H
#pragma once
#define MAX_ANIM 32

//define the sprite structure
typedef struct SPRITE
{
int x,y;
int width,height;
int xspeed,yspeed;
int xdelay,ydelay;
int xcount,ycount;
int curframe,maxframe,animdir;
int framecount,framedelay;
int largura;
int quadro_atual;
}SPRITE;

class CAnim
{
	public:	
		CAnim(void);//construtor
		virtual ~CAnim(void);//destrutor - libera as animações alocadas
		int Atualizar(void); //calcula e retorna o quadro_atual
		int Quadro_Atual(void); //apenas retorna o quadro_atual
		int Atribuir_Anim_Freq(int freq);
		int Anim_Freq(void);
		int Adiciona_Anim(int num_pos, int *sequencia);//retorna o indice da anim
		int Anim_Ind(void);
		int Anim_Atual(void);
		int Atribuir_Anim_Atual(int anim, bool trava=false);
		bool Anim_Feita(void);
		void Reinicia_Anim(void);
		int Num_Anim(void);
		void Prox_Quadro(void);
		

	protected:
		//so podem ser acessados por filhos ou pelos seus metodos
		int anim_cont;
		int anim_freq;
		int quadro_atual;
		int anim_ind;
		int anim_dir;
		int *anim_seq[MAX_ANIM];
		int anim_atual;
		bool anim_feita;
		int num_anim;
		bool trava_ult_quadro;

};

#endif
