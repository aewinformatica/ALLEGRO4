#ifndef SPRITES_H
#define SPRITES_H
#pragma once

/* estados dos sprites */
#define ESTADO_PARADO               0
#define ESTADO_ANDANDO              1
#define ESTADO_PULANDO              2
#define ESTADO_CAINDO               3
#define ESTADO_ESPIRITO             4 /* nao colide com tiles */
#define ESTADO_INVISIVEL            5
#define ESTADO_MORTO                100 /* a partir daqui eh tudo morto */
#define ESTADO_MORRENDO             101 /* ainda nao morreu, mas vai */
#define ESTADO_PARABOLA             102 /* morrendo e descreve uma parabola ate sair da tela */

/* tipos de mario */
#define MARIO_PEQUENO               0x0000
#define MARIO_GRANDE                0x0001
#define MARIO_PEQUENO_INVENCIVEL    0x0100
#define MARIO_GRANDE_INVENCIVEL     0x0101
#define DURACAO_INVENCIVEL          75

/* definicoes dos estados dos sprites */
#define FSM_COGUMELO_SUBINDO        0
#define FSM_COGUMELO_DIREITA        1
#define FSM_COGUMELO_ESQUERDA       2
#define FSM_GOOMBA_ESQUERDA         0
#define FSM_GOOMBA_DIREITA          1
#define FSM_GOOMBA_SUBINDO          2
#define FSM_GOOMBA_CAINDO           3
#define FSM_SPINY_ESQUERDA          0
#define FSM_SPINY_DIREITA           1
#define FSM_SPINY_SUBINDO           2
#define FSM_SPINY_CAINDO            3
#define FSM_KOOPA_ESQUERDA          0
#define FSM_KOOPA_DIREITA           1
#define FSM_KOOPA_PISADO            2
#define FSM_KOOPA_CHUTADO_ESQUERDA  3
#define FSM_KOOPA_CHUTADO_DIREITA   4
#define FSM_KOOPA_SUBINDO           5
#define FSM_KOOPA_CAINDO            6
#define FSM_MOEDA_PARADA            0
#define FSM_MOEDA_SUBINDO           1
#define FSM_BOLA_DE_FOGO_ESPERA     0
#define FSM_BOLA_DE_FOGO_SUBINDO    1
#define FSM_BOLA_DE_FOGO_CAINDO     2
#define FSM_PEIXE_ESPERA            0
#define FSM_PEIXE_SUBINDO           1
#define FSM_PEIXE_CAINDO            2
#define FSM_PEIXE_MORTO_SUBINDO     3
#define FSM_PEIXE_MORTO_CAINDO      4

/* constantes dos sprites */
#define MARIO_VELOCIDADE            2
#define MARIO_VELOCIDADE_PULO       6
#define DURACAO_PULO                12
#define DURACAO_SUPER_PULO          24
#define VELOCIDADE_GOOMBA           1
#define VELOCIDADE_SPINY            1
#define VELOCIDADE_KOOPA            1
#define VELOCIDADE_KOOPA_CHUTADO    8
#define VELOCIDADE_COGUMELO         1
#define VELOCIDADE_EIXO_X           2
#define MAX_ESPERA_FOGO_PEIXE       100

/* direcoes dos sprites */
#define DIRECAO_DIREITA             0
#define DIRECAO_ESQUERDA            1
#define DIRECAO_CIMA                2
#define DIRECAO_BAIXO               3

/* macros para tipos de sprites e tiles */
#define TILE_INTERROGACAO(s) (s >= INTERROGACAO_01 && s <= INTERROGACAO_04)
#define TILE_INTERROGACAO_ESCONDIDO(s) (s == 0)
#define TILE_BLOCO_ANIMADO(s) (s >= BLOCO_ANIMADO_01 && s <= BLOCO_ANIMADO_04)
#define TILE_BLOCO_AZULADO(s) (s >= BLOCO_AZULADO_01 && s <= BLOCO_AZULADO_04)
#define TILE_BLOCO_PARADO(s) (s == BLOCO_PARADO)
#define TILE_BLOCO(s) (TILE_BLOCO_ANIMADO(s) || TILE_BLOCO_AZULADO(s) || TILE_BLOCO_PARADO(s))
#define TILE_NOTA(s) (s >= NOTA_01 && s <= NOTA_04)
#define TILE_CHAO(s) (s >= CHAO_ESQUERDA && s <= CHAO_DIREITA)
#define TILE_CANO_TOPO(s) (s >= CANO_TOPO_01 && s <= CANO_TOPO_02)
#define TILE_LAVA(s) (s >= LAVA_01 && s <= LAVA_CENTRO)
#define TILE_ESTACA(s) (s >= ESTACA_CORPO && s <= ESTACA_TOPO)
#define TILE_SAIDA(s) (s == PLACA_SAIDA || s == CABO_SAIDA)
#define SPRITE_BLOCO_PARTIDO(s) (s < 0)
#define SPRITE_MARIO_PEQUENO_ANDANDO(s) (s >= MARIO_PEQUENO_01 && s <= MARIO_PEQUENO_02)
#define SPRITE_MARIO_GRANDE_ANDANDO(s) (s >= MARIO_GRANDE_01 && s <= MARIO_GRANDE_04)
#define SPRITE_MOEDA(s) (s >= MOEDA_01 && s <= MOEDA_04)
#define SPRITE_FUMACA(s) (s >= FUMACA_01 && s <= FUMACA_05)
#define SPRITE_COGUMELO(s) (s == COGUMELO)
#define SPRITE_GOOMBA(s) (s >= GOOMBA_01 && s <= GOOMBA_02)
#define SPRITE_SPINY(s) (s >= SPINY_01 && s <= SPINY_02)
#define SPRITE_KOOPA(s) (s >= KOOPA_01 && s <= KOOPA_02)
#define SPRITE_BOLA_DE_FOGO(s) (s >= BOLA_DE_FOGO_01 && s <= BOLA_DE_FOGO_04)
#define SPRITE_PEIXE(s) (s >= PEIXE_01 && s <= PEIXE_02)

/* maximo numero de sprites numa mesma fase */
#define MAX_SPRITE                  20000

typedef struct SPRITE
{
	int x,y,x_ant,y_ant,id,contador;
   short familia,estado,quadro_atual,delay;
   char direcao;

   /* flags de colisao */
   char colisao[4];

   /* estado da maquina de estados */
   short fsm;

} SPRITE;

SPRITE sprite[MAX_SPRITE],*mario;

/* minimo alcance do pulo do mario */
char min_alcance_pulo;

/* valores de incremento/decremento para pulo/queda */
int pulo[DURACAO_PULO],super_pulo[DURACAO_SUPER_PULO];

/* multiplicador de pulo (default = 1, nota = 2) */
char mult_pulo;

void cria_sprite(int x, int y, int quadro, int delay);
void pega_cogumelo(SPRITE *cogumelo);
void pega_moeda(SPRITE *moeda);
void move_moeda(SPRITE *moeda);
void cabecada_interrogacao(int i, int j, int tipo);
void pisa_goomba(SPRITE *goomba);
void pisa_peixe(SPRITE *peixe);
void chuta_koopa(SPRITE *koopa);
void pisa_koopa(SPRITE *koopa);
void desenha_sprite(SPRITE *_sprite);
void desenha_sprites(void);
void desenha_sprites_frente(void);
void mario_anda(void);
void mario_para(void);
void mario_pula(void);
void mario_morre(int morre_direto);
void destroi_bloco(int i, int j, int k);
void morre_goomba(SPRITE *goomba);
void morre_koopa(SPRITE *koopa);
void morre_spiny(SPRITE *spiny);
void morre_peixe(SPRITE *peixe);
void move_cogumelo(SPRITE *cogumelo);
void move_goomba(SPRITE *goomba);
void move_spiny(SPRITE *spiny);
void move_koopa(SPRITE *koopa);
void move_bloco(SPRITE *bloco);
void move_bola_de_fogo(SPRITE *bola);
void move_peixe(SPRITE *peixe);
void move_sprites(void);
int fora_da_tela(SPRITE *_sprite);
int fora_da_fase(SPRITE *_sprite);

#endif
