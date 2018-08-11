#include "sprite.h"
#include <iostream>
#include <fstream>
#include <string>
#include <allegro.h>
//#include "ryuindex.h"
//#include "dadosindex.h"   
#define DAT_WIDTH            DAT_ID('X','S','I','Z')
#define DAT_HEIGHT           DAT_ID('Y','S','I','Z')

#define DAT_FRAME_WIDTH           DAT_ID('X','P','O','S')
#define DAT_FRAME_HEIGHT          DAT_ID('Y','P','O','S')

#define VEL_HOR		4
#define POS_VER		320


int vel_vertical, pulo_vel_hor;

int ANIM_PARADO[] = { 0,1,2,3};
int ANIM_ANDANDO[] = {0,1,2,3};
int ANIM_RECUANDO[] = {21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6};
int ANIM_SOCO[] = {34,35,36,36,35,34};
int ANIM_CHUTE[] = {37,38,39,39,38,37};
int ANIM_ABAIXAR[] = {0,1};
int ANIM_SOCO_BAIXO[] = {42,40,41,41,40,42};
int ANIM_CHUTE_BAIXO[] = {42,43,44,44,43,42};
int ANIM_PULO[] = {0,1};
/*DO KARATE*/
using namespace std;

Sprite::Sprite(){
this->x=0;
this->y=320;
this->atacando=false, 
this->pulando=false;
this->dir=0;
this->frame_atual=0;
this->estado = STAND;
this->anim_cont=0;
this->anim_freq=3;
this->quadro_atual=0;
this->anim_ind=0;
this->anim_atual=0;;
this->anim_feita=false;
this->num_anim = 0;
this->trava_ult_quadro = false;

//this->tipo = DATASHEET;
//this->tipo = SPRITESHEET;
this->tipo = RLESHEET;
//armazena o valor da array na varivel para o estado da animação

// alocando de arquivo o que esta acima nome dos arquivos

this->ArqFasesAnimacao[0] = "parado.txt";
this->ArqFasesAnimacao[1] = "andando.txt";
this->ArqFasesAnimacao[2] = "soco.txt";
this->iQtdAnimFases = 3;
this->ANIMACOES[iQtdAnimFases][32];;
this->carregarAnimacoes();

this->anim_parado = Adiciona_Anim(3, ANIM_PARADO);
//this->anim_parado = Adiciona_Anim(10,ANIMACOES[0]);
//this->anim_andando = Adiciona_Anim(16, ANIMACOES[1]);
//this->anim_andando = Adiciona_Anim(16, ANIMACOES[1]);
this->anim_andando = Adiciona_Anim(3, ANIM_ANDANDO);

this->anim_recuando = Adiciona_Anim(16, ANIM_RECUANDO);

this->anim_soco = Adiciona_Anim(6, ANIMACOES[2]);
//this->anim_soco = Adiciona_Anim(6, ANIM_SOCO);

this->anim_chute = Adiciona_Anim(6, ANIM_CHUTE);
this->anim_abaixar = Adiciona_Anim(2, ANIM_ABAIXAR);
this->anim_soco_baixo = Adiciona_Anim(6, ANIM_SOCO_BAIXO);
this->anim_chute_baixo = Adiciona_Anim(6, ANIM_CHUTE_BAIXO);
this->anim_pulo = Adiciona_Anim(2, ANIM_PULO);


}
/*SpriteSheet BMP*/
void Sprite::carregarSprite(string arquivo){
     string extensao =".bmp";
     size_t encontrou = arquivo.find(extensao);
     
     if(encontrou!=string::npos){
BITMAP *temp;
        temp = NULL;
     
temp = load_bitmap(arquivo.c_str(), NULL);
if (!temp) {
		textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "Um erro ocorreu carregando o arquivo %s", arquivo.c_str());
  
  std::size_t encontrado = arquivo.find(extensao);
  if (encontrado!=std::string::npos)
    std::cout << "'extensao .dat' encontrado em: " << encontrado << '\n';
    
		while (!keypressed()) ;
		exit(0);
    }
for (int n=0; n<=44; n++)
{
animations[n] = grabframe(temp,104,124,0,0,9,n);
}
destroy_bitmap(temp);
}
else{
    DATAFILE *dados;
              dados = load_datafile(arquivo.c_str());
              int indice = 0;
              int xcolunas = 0;
              int ylinhas = 0; 
              const char *data_prop;
              const char* busca = " ";
              const int name_type = DAT_ID('N','A','M','E');
              
                   //Verificar se Carregou     
	if (!dados) {
		textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "Um erro ocorreu carregando o arquivo %s", arquivo.c_str());
		while (!keypressed()) ;
		exit(0);
    }
    /*
0-7 parado
9-15 andando
16-25 teia;
26-30 pulando
31 fundo
32 som de teia
*/
    
    for(indice=0; dados[indice].type != DAT_END; indice++){
        animations[indice] = (BITMAP *)dados[indice].dat;
}        
}
}
//----------------------------------------------------------------------------------------------------------------------------------------------
/*RLE BMP*/
void Sprite::carregarSprite(string arquivo,int RLEindex){
  const char *data_prop;
  const int name_type = DAT_ID('N','A','M','E');
  const char* busca = " ";
  int indice = 0;
  int xcolunas = 0;
  int ylinhas = 0;
   
  DATAFILE *dados;
            dados = load_datafile(arquivo.c_str());
            
           //Verificar se Carregou     
	if (!dados) {
		textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "Um erro ocorreu carregando o arquivo %s", arquivo.c_str());
		while (!keypressed()) ;
		exit(0);
    }
    
                    if(RLEindex == entrada) 
                   busca = "entrada";
                   
                   if(RLEindex == parado) 
                   busca = "parado";
                   
                   if(RLEindex == andando) 
                   busca = "andando";
                   
                   if(RLEindex == saltando) 
                   busca = "saltando";
                   if(RLEindex == abaixando) 
                   busca = "abaixando";
                   
                   
                /*if(RLEindex == woody2) 
                   busca = "woody2";
                   
                   
                   else if(RLEindex == soco){
                   busca = "soco";
                   }*/
                      
   for (indice=0; dados[indice].type != DAT_END; indice++) {
      if (stricmp(get_datafile_property(dados+indice, name_type),
                  busca) == 0) {
                  
                           
                                    data_prop = get_datafile_property(dados+indice,DAT_NAME);
                                    if(data_prop){
                                    textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "nome %s", data_prop);
                                    //while (!keypressed()) ;
                                    }
                                    
                                    data_prop = get_datafile_property(dados,DAT_WIDTH);
                                    if(data_prop){
                                                  
                                    textprintf_ex(screen, font, 0, 110, palette_color[15],-1, "IMG width %s", data_prop);
                                    //while (!keypressed()) ;
                                    }
                                    
                                    data_prop = get_datafile_property(dados+indice,DAT_HEIGHT);
                                    if(data_prop){
                                    textprintf_ex(screen, font, 0, 120, palette_color[15],-1, "IMG height %s", data_prop);
                                   // while (!keypressed()) ;
                                    }
                                      
                                    data_prop = get_datafile_property(dados+indice,DAT_FRAME_WIDTH);
                                    if((data_prop != "-1")&&(data_prop != empty_string)){
                                    xcolunas = atoi(data_prop); //PEGA AS COLUNAS
                                    }
                                        
                                    if(data_prop){
                                    textprintf_ex(screen, font, 0, 135, palette_color[15],-1, "xColunas %s", data_prop);//XPOS
                                   // while (!keypressed()) ;
                                    }
                                    
                                    data_prop = get_datafile_property(dados+indice,DAT_FRAME_HEIGHT);
                                   if((data_prop != "-1")&&(data_prop != empty_string)){
                                    ylinhas = atoi(data_prop); // PEGA AS LINHAS
                                    }
                                    
                                    if(data_prop){
                                    textprintf_ex(screen, font, 0, 145, palette_color[15],-1, "yLinhas %s", data_prop);//YPOS
                                    while (!keypressed()) ;
                                    }
   
                                 }
   }

//PEGA A IMAGEM RLE COMO SPRITE
RLE_SPRITE *spritesheetTemp;
            spritesheetTemp = (RLE_SPRITE*) dados[RLEindex].dat;

BITMAP *RLE_BITMAP = rle_to_bitmap(spritesheetTemp);

int QtdH = xcolunas;
int QtdV = ylinhas;     

int total = (QtdH * QtdV);

  for(int frame=0;frame<total-1;frame++){
    animations[frame] = grabframe(RLE_BITMAP,0,0,QtdH,QtdV,frame);
  }
};

//----------------------------------------------------------------------------------------------------------------------------------------------

void Sprite::setFrame(int primeiro_frame,int quantidade_frames){
this->primeiro_frame = primeiro_frame;
this->frame_atual = primeiro_frame;
this->ultimo_frame = primeiro_frame + quantidade_frames - 1;
};

int Sprite::Adiciona_Anim(int num_pos, int *sequencia)
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

int Sprite::Atribuir_Anim_Atual(int anim, bool trava){
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

void Sprite::Reinicia_Anim()
{
		anim_cont=0;
		anim_ind=0;
		anim_feita=false;
};

int Sprite::Atualizar(void)
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
};
//-----------------------------------------------------------------------------------------------------------------------------------------------
void Sprite::desenhar(BITMAP *Buffer){
     
           textprintf_ex(Buffer, font,70, (animations[quadro_atual]->h*2)-35, WHITE,-1,"Quadro Atual: %2d",quadro_atual);

           if(estado == STAND){
         Atribuir_Anim_Atual(anim_parado);
        }else if(estado == JUMP){
                 
                 Atribuir_Anim_Atual(anim_pulo,true);
               }
               else if(estado == CROUCH){
         Atribuir_Anim_Atual(anim_abaixar,true);
        }else if(estado == FORWARD){
                       Atribuir_Anim_Atual(anim_andando);
               }
                      
                      if(this->dir ==0){
                                   
                      masked_blit (animations[quadro_atual], Buffer, 0, 0, x, y, this->animations[quadro_atual]->w, this->animations[quadro_atual]->h);
                       }else if(this->dir ==1){
                             
                             draw_sprite_h_flip(Buffer,animations[quadro_atual],x,y); 
                         
                        }
};


void Sprite::controle(){
     
 if(pulando){
     this->y += vel_vertical;
     vel_vertical++;
     this->estado = JUMP;
     
     
     //VERIFICA SE ESTA NO SOLO 
     if(this->y >= POS_VER){

     this->y = POS_VER;
     this->pulando = false;
     this->estado = STAND;
     
     
     }
    }
     
 if(key[KEY_LEFT]){
                  if(this->estado != JUMP)
                  carregarSprite("gfx/woody.dat",andando);
                  //this->estado = BACKWARD;
                  this->estado = FORWARD;
                  this->dir =1;
                  this->x -=VEL_HOR;
                   
}
else if(key[KEY_RIGHT]){
                   carregarSprite("gfx/woody.dat",andando);
                   if(this->estado != JUMP)
                  this->estado = FORWARD;
                  
                  this->dir =0;
                  this->x +=VEL_HOR;
}
else if(key[KEY_DOWN]){
      carregarSprite("gfx/woody.dat",abaixando);
 this->estado = CROUCH;

}else 
if(pulando == false){
           
      this->estado = STAND;
      carregarSprite("gfx/woody.dat",parado);
}

    if(key[KEY_SPACE]){
     
     if(pulando == false){
                carregarSprite("gfx/woody.dat",saltando);
    pulando = true;
    vel_vertical = -15;
    
    if(key[KEY_LEFT]) pulo_vel_hor = - VEL_HOR;
    else
    if(key[KEY_RIGHT]) pulo_vel_hor = VEL_HOR;
    else pulo_vel_hor = 0;
    }
}

};
void Sprite::carregarAnimacoes(){
     int aFase = 0;
     int aux = 0;
     string conteudo;
     
        //O FOR abaixo é para preencher todas as fases.
   for (aFase=0; aFase<this->iQtdAnimFases; aFase++)
   {
     
    std::ifstream oLendoFase(ArqFasesAnimacao[aFase],ios::in );
    oLendoFase >> noskipws;
    
    int iMov = 0;
    while(!oLendoFase.eof()){
   
    getline(oLendoFase,conteudo);
    cout <<conteudo;
    
    aux = std::atoi(conteudo.c_str());
    //cout << aux;
    ANIMACOES[aFase][iMov] = aux;     
    
    iMov++;
    }
        
    for(int i = 0;i<9;i++){
    cout << ANIMACOES[1][i] + " ";
    } 
    cout << endl;
    oLendoFase.close(); 
   }

};  
