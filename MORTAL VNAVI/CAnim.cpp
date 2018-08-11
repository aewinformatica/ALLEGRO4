#include "CAnim.h"

////////////////////////////////////
CAnim::CAnim(void)
{
		anim_cont=0;
		anim_dir=0;
		anim_freq=3;
		quadro_atual=0;
		anim_ind=0;
		anim_atual=0;;
		anim_feita=false;
		num_anim = 0;
		trava_ult_quadro = false;
}
////////////////////////////////////
CAnim::~CAnim(void)
{
	//destrutor
	for(int i= 0; i < num_anim; i++)
	{
		delete [] anim_seq[i];
	}
}
////////////////////////////////////
int CAnim::Atualizar(void)
{
	if(num_anim == 0) return(0);

	anim_cont++;
	if(anim_cont >= anim_freq)
	{
		anim_cont = 0;

		anim_ind++;

		//se fim da animação
		if(anim_seq[anim_atual][anim_ind] == -1)
		{
			anim_feita = true;

			if(trava_ult_quadro)
			{
				//manter o último quadro
				anim_ind--;
			}
			else
			{
				//reinicia
				anim_ind = 0;
			}
		}
	}

	quadro_atual = anim_seq[anim_atual][anim_ind];

	return(quadro_atual);
}
////////////////////////////////////
int CAnim::Quadro_Atual(void) //apenas retorna o quadro_atual
{
	if(num_anim == 0) return(0);

	//verificar quadro atual através dos outros índices
	quadro_atual = anim_seq[anim_atual][anim_ind];
	return(quadro_atual);
}
////////////////////////////////////
int CAnim::Atribuir_Anim_Freq(int freq)
{
	if(freq > 0)
	{
		anim_freq = freq;
		return(1);
	}
	else
	{
		return(0);
	}
}
////////////////////////////////////
int CAnim::Anim_Freq(void)
{
	return(anim_freq);
}
////////////////////////////////////
int CAnim::Adiciona_Anim(int num_pos, int *sequencia)
{
	//retorna o indice da anim

	int quadro_seq;

		if ((num_pos < 1) || (!sequencia) 
				 || (num_anim >= MAX_ANIM )) return(0);

	
		anim_seq[num_anim] = new int[num_pos + 1];

		//testa se foi alocada a memória
		if(!anim_seq[num_anim]) return(0);

		//copia dados da sequencia
		for(int i=0; i < num_pos; i++)
		{
			
			quadro_seq = sequencia[i];

			if((quadro_seq <0) ) return(0);

			anim_seq[num_anim][i] = quadro_seq;
		}
		anim_seq[num_anim][num_pos] = -1; //fim de animação

		num_anim++;
		return(num_anim-1);//retorna indice da anim
	
}
////////////////////////////////////
int CAnim::Anim_Ind(void)
{
	return(anim_ind);
}
////////////////////////////////////
int CAnim::Anim_Atual(void)
{
	return(anim_atual);
}
////////////////////////////////////
int CAnim::Atribuir_Anim_Atual(int anim, bool trava){
	if((anim>=0) && (anim < num_anim))
	{
		//se animação for diferente da atual então atribui e reinicia
		if(anim_atual != anim)
		{
			anim_atual = anim;
			Reinicia_Anim();
		}

		trava_ult_quadro = trava;

		return(1);
	}
	else
	{
		return(0);
	}
}
////////////////////////////////////
bool CAnim::Anim_Feita(void)
{
	return(anim_feita);
}
////////////////////////////////////
void CAnim::Reinicia_Anim(void)
{
		anim_cont=0;
		anim_ind=0;
		anim_feita=false;
}

////////////////////////////////////
int CAnim::Num_Anim(void)
{
	return(num_anim);
};
////////////////////////////////////
void CAnim::Prox_Quadro(void)
{
	//força a passagem p/ o próximo quadro

	anim_cont = anim_freq;

	Atualizar();

};


