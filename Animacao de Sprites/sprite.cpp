#include "sprite.h"
#include <iostream>
#include <allegro.h>
#include "ryuindex.h"
#include "dadosindex.h"   
#define DAT_WIDTH            DAT_ID('X','S','I','Z')
#define DAT_HEIGHT           DAT_ID('Y','S','I','Z')

#define DAT_FRAME_WIDTH           DAT_ID('X','P','O','S')
#define DAT_FRAME_HEIGHT          DAT_ID('Y','P','O','S')


int ANIM_PARADO[] = { 0,1,2,3,4,5,4,3,2,1 };
int ANIM_ANDANDO[] = {6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
int ANIM_RECUANDO[] = {21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6};
int ANIM_SOCO[] = {34,35,36,36,35,34};
int ANIM_CHUTE[] = {37,38,39,39,38,37};
int ANIM_ABAIXAR[] = {22,23,24};
int ANIM_SOCO_BAIXO[] = {42,40,41,41,40,42};
int ANIM_CHUTE_BAIXO[] = {42,43,44,44,43,42};
int ANIM_PULO[] = {25,26,27,28,29,29,30,31,32,33};
/*DO KARATE*/
using namespace std;

Sprite::Sprite(){
this->x=0;
this->y=200;
this->dir=0;
this->frame_atual=0;
this->estado = STAND;
this->anim_cont=0;
//this->anim_dir=0;
this->anim_freq=3;
this->quadro_atual=0;
this->anim_ind=0;
this->anim_atual=0;;
this->anim_feita=false;
this->num_anim = 0;
this->trava_ult_quadro = false;

//this->tipo = DATESHEET;
this->tipo = SPRITESHEET;
//this->tipo = RLESHEET;
//verificar se vai usar array pra animação
this->AnimArray = false;
//armazena o valor da array na varivel para o estado da animação
this->anim_parado = Adiciona_Anim(10, ANIM_PARADO);
this->anim_andando = Adiciona_Anim(16, ANIM_ANDANDO);
this->anim_recuando = Adiciona_Anim(16, ANIM_RECUANDO);
this->anim_soco = Adiciona_Anim(6, ANIM_SOCO);
this->anim_chute = Adiciona_Anim(6, ANIM_CHUTE);
this->anim_abaixar = Adiciona_Anim(3, ANIM_ABAIXAR);
this->anim_soco_baixo = Adiciona_Anim(6, ANIM_SOCO_BAIXO);
this->anim_chute_baixo = Adiciona_Anim(6, ANIM_CHUTE_BAIXO);
this->anim_pulo = Adiciona_Anim(10, ANIM_PULO);

}
/*SpriteSheet BMP*/
void Sprite::carregarSprite(string arquivo){
BITMAP *temp;
        temp = NULL;
     
temp = load_bitmap(arquivo.c_str(), NULL);
if (!temp) {
		textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "Um erro ocorreu carregando o arquivo %s", arquivo.c_str());
		while (!keypressed()) ;
		exit(0);
    }
for (int n=0; n<=44; n++)
{
animations[n] = grabframe(temp,104,124,0,0,9,n);
}
destroy_bitmap(temp);
}
//------------------------------------------------------------------------------------------------------------------------------------- 
/*DATA BMP*/
void Sprite::carregarSprite(string arquivo,int tipoSprite){
       this->tipo = tipoSprite;
    DATAFILE *dados;
              dados = load_datafile(arquivo.c_str());
  
     //Verificar se Carregou     
	if (!dados) {
		textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "Um erro ocorreu carregando o arquivo %s", arquivo.c_str());
		while (!keypressed()) ;
		exit(0);
    }
    
if(this->tipo== DATASHEET){
                
//carrega as figuras com loop for para fazer a animação parado                                                  
for(int i=0;i<8;i++){
        parado[i] = (BITMAP *)dados[i].dat;
}        

 
 //carrega as figuras para fazer a animação andando                                        
 andando[0] = (BITMAP *)dados[9].dat;                                                 
 andando[1] = (BITMAP *)dados[10].dat;  
 andando[2] = (BITMAP *)dados[11].dat; 
 andando[3] = (BITMAP *)dados[12].dat;  
 andando[4] = (BITMAP *)dados[13].dat;  
 andando[5] = (BITMAP *)dados[14].dat;  
 andando[6] = (BITMAP *)dados[15].dat;  
 
 t_teia[0] = (BITMAP *)dados[16].dat;
 t_teia[1] = (BITMAP *)dados[17].dat;
 t_teia[2] = (BITMAP *)dados[18].dat;
 t_teia[3] = (BITMAP *)dados[19].dat;
 t_teia[4] = (BITMAP *)dados[20].dat;
 t_teia[5] = (BITMAP *)dados[21].dat;
 t_teia[6] = (BITMAP *)dados[22].dat;
 t_teia[7] = (BITMAP *)dados[23].dat;
 t_teia[8] = (BITMAP *)dados[24].dat;
 t_teia[9] = (BITMAP *)dados[25].dat;

 
 pulando[0] = (BITMAP *)dados[26].dat;
 pulando[1] = (BITMAP *)dados[27].dat;
 pulando[2] = (BITMAP *)dados[28].dat;
 pulando[3] = (BITMAP *)dados[29].dat;
 pulando[4] = (BITMAP *)dados[30].dat;
 /*//carrega o fundo
 fundo = (BITMAP *)dados[31].dat;
 
 som_teia = (SAMPLE *)dados[32].dat;
*/
}
};
//----------------------------------------------------------------------------------------------------------------------------------------------
/*RLE BMP*/
void Sprite::carregarSprite(string arquivo,int tipoSprite,int RLEindex){
  const char *data_prop;
  int indice = 0;
  DATAFILE *dados;
            dados = load_datafile(arquivo.c_str());
            
            int xcolunas = 0;
            int ylinhas = 0; 
            
            this->tipo = tipoSprite;
  
           //Verificar se Carregou     
	if (!dados) {
		textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "Um erro ocorreu carregando o arquivo %s", arquivo.c_str());
		while (!keypressed()) ;
		exit(0);
    }
    
  const int name_type = DAT_ID('N','A','M','E');
  const char* busca = " ";
       
    if(this->tipo== RLESHEET){
                if(RLEindex == woody){ 
                   busca = "woody";
                   }
                   else if(RLEindex == woody2){
                   busca = "woody2";
                   }
                   else if(RLEindex == soco){
                   busca = "soco";
                   }
                   else if(RLEindex == RYU){
                        busca = "RYU";
                   }   
   for (indice=0; dados[indice].type != DAT_END; indice++) {
      if (stricmp(get_datafile_property(dados+indice, name_type),
                  busca) == 0) {
                  
                           
    data_prop = get_datafile_property(dados+indice,DAT_NAME);
    if(data_prop){
    textprintf_ex(screen, font, 0, 100, palette_color[15],-1, "nome %s", data_prop);
    //while (!keypressed()) ;
    }
    /*
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
     */ 
    data_prop = get_datafile_property(dados+indice,DAT_FRAME_WIDTH);
    if((data_prop != "-1")&&(data_prop != empty_string)){
    xcolunas = atoi(data_prop);
    }
        
    if(data_prop){
    textprintf_ex(screen, font, 0, 135, palette_color[15],-1, "xColunas %s", data_prop);
   // while (!keypressed()) ;
    }
    
    data_prop = get_datafile_property(dados+indice,DAT_FRAME_HEIGHT);
   if((data_prop != "-1")&&(data_prop != empty_string)){
    ylinhas = atoi(data_prop);
    }
    
    if(data_prop){
    textprintf_ex(screen, font, 0, 145, palette_color[15],-1, "yLinhas %s", data_prop);
    while (!keypressed()) ;
    }
   
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
    rleAnimations[frame] = grabframe(RLE_BITMAP,0,0,QtdH,QtdV,frame);
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
}
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
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Sprite::desenhar(BITMAP *Buffer,int tipoSprite){
     
     this->tipo = tipoSprite;
     
     if(this->frame_atual < this->ultimo_frame)
     this->frame_atual++;
     else
     frame_atual = primeiro_frame;
     
     //
     if(this->tipo == SPRITESHEET){
    masked_blit (animations[frame_atual], Buffer, 0, 0, x, y, animations[frame_atual]->w, this->animations[frame_atual]->h);
     textprintf_ex(Buffer, font, 0, 70, WHITE,-1,"FRAME ATUAL: %2d",frame_atual);
     }else 
     if(this->tipo == RLESHEET){
     textprintf_ex(Buffer, font, 0, 70, WHITE,-1,"FRAME ATUAL: %2d",frame_atual);

     masked_blit (rleAnimations[frame_atual], Buffer, 0, 0, x, y, this->rleAnimations[frame_atual]->w, this->rleAnimations[frame_atual]->h);
     }
     
};
void Sprite::desenhar(BITMAP *Buffer,bool AnimArray){
     
     this->AnimArray = AnimArray;
      if( this->AnimArray == true){
      if(estado == STAND){
         Atribuir_Anim_Atual(anim_parado);
      }else if(estado == JUMP)
       {
         Atribuir_Anim_Atual(anim_pulo,true);
       }else if(estado == FORWARD)
        {
          Atribuir_Anim_Atual(anim_andando);
        }
     
     if(this->tipo == SPRITESHEET){
    masked_blit (animations[quadro_atual], Buffer, 0, 0, x, y, animations[quadro_atual]->w, this->animations[quadro_atual]->h);
    
     textprintf_ex(Buffer, font, animations[quadro_atual]->w -96, (animations[quadro_atual]->h*2)-35, WHITE,-1,"FRAME ATUAL: %2d",quadro_atual);
     }else 
     if(this->tipo == RLESHEET){
     masked_blit (rleAnimations[quadro_atual], Buffer, 0, 0, x, y, this->rleAnimations[quadro_atual]->w, this->rleAnimations[quadro_atual]->h);
     }
     }
};
//---------------------------------------------------------------------------------------------------------------------------------------------------
//USADO PARA DAT BITMAPS
void Sprite::desenhar(BITMAP *Buffer){
     if(this->frame_atual < this->ultimo_frame)
     this->frame_atual++;
     else
     frame_atual = primeiro_frame;
     
     
        if(estado == STAND){
              if(this->dir == 0){
              //NORMAL      
              masked_blit (parado[frame_atual], Buffer, 0, 0, x, y, parado[frame_atual]->w, this->parado[frame_atual]->h);
              //AMPLIADO POREM NAO INVERTE
              //stretch_sprite(Buffer, parado[frame_atual], 0, 0, 300, 300);
            }else
                 if(this->dir == 1){
                 //INVERTIDO
                 draw_sprite_h_flip(Buffer,parado[frame_atual],x,y);
            }

        }else if(estado == JUMP){
               //mas nao anima;
               setFrame(0,8);
                  if(this->dir == 0){
                   //NORMAL
              masked_blit (pulando[frame_atual], Buffer, 0, 0, x, y, pulando[frame_atual]->w, this->pulando[frame_atual]->h);
                      //AMPLIADO POREM NAO INVERTE
                    //stretch_sprite(Buffer, pulando[frame_atual], 0, 0, 300, 300);
              }else
                 if(this->dir == 1){
            //INVERTIDO
            draw_sprite_h_flip(Buffer,pulando[frame_atual],x,y);
            }

        }
        else if(estado == FORWARD){
               setFrame(0,5);
                  if(this->dir == 0){
                   //NORMAL
              masked_blit (andando[frame_atual], Buffer, 0, 0, x, y, andando[frame_atual]->w, this->andando[frame_atual]->h);
                      //AMPLIADO POREM NAO INVERTE
                    //stretch_sprite(Buffer, pulando[frame_atual], 0, 0, 300, 300);
              }else
                 if(this->dir == 1){
            //INVERTIDO
            draw_sprite_h_flip(Buffer,andando[frame_atual],x,y);
            }

        }
        textprintf_ex(Buffer, font, parado[frame_atual]->w -140, (parado[frame_atual]->h*2)+6, WHITE,-1,"FRAME ATUAL: %2d",frame_atual);
     
     
};
void Sprite::controle(){
if(key[KEY_SPACE]){
        this->estado = JUMP;
        }else
        this->estado = STAND;
        if(key[KEY_LEFT]){
                          this->estado = BACKWARD;
                          this->dir =1;
        }
                if(key[KEY_RIGHT]){
                          this->estado = FORWARD;
                          this->dir =0;
        }
}


