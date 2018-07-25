#ifndef IMAGENS_H
#define IMAGENS_H
#pragma once
/* cor de fundo do bitmap com 16 bits de cores e preto para 256 cores */
#define FUNDO_16_BITS                        253
#define COR_PRETO                            73

/* dimensoes dos tiles e total de imagens */
#define TILE_SIZE                            16
#define TILE_SIZE_EXTRA                      27
#define MAX_TILE                             120
#define MAX_IMAGEM                           160

/* identificadores */
#define BLOCO_PARTIDO                        -1
#define INTERROGACAO_01                      1
#define INTERROGACAO_02                      2
#define INTERROGACAO_03                      3
#define INTERROGACAO_04                      4
#define INTERROGACAO_MORTO                   5
#define BLOCO_ANIMADO_01                     6
#define BLOCO_ANIMADO_02                     7
#define BLOCO_ANIMADO_03                     8
#define BLOCO_ANIMADO_04                     9 
#define COGUMELO                             10
#define PLACA_SAIDA                          11
#define NOTA_01                              12
#define NOTA_02                              13
#define NOTA_03                              14
#define NOTA_04                              15
#define GOOMBA_01                            16
#define GOOMBA_02                            17
#define GOOMBA_MORTO                         18
#define PEIXE_01                             19
#define PEIXE_02                             20
#define MOEDA_01                             21
#define MOEDA_02                             22
#define MOEDA_03                             23
#define MOEDA_04                             24
#define FUMACA_01                            25
#define FUMACA_02                            26
#define FUMACA_03                            27
#define FUMACA_04                            28
#define FUMACA_05                            29
#define SPINY_01                             30
#define SPINY_02                             31
#define KOOPA_CHUTADO_01                     32
#define KOOPA_CHUTADO_02                     33
#define KOOPA_CHUTADO_03                     34
#define KOOPA_CHUTADO_04                     35
#define CANO_TOPO_01                         36
#define CANO_TOPO_02                         37
#define CANO_CORPO_01                        38
#define CANO_CORPO_02                        39
#define BLOCO_PARADO                         40
#define CHAO_ESQUERDA                        41
#define CHAO_CENTRO                          42
#define CHAO_DIREITA                         43
#define PAREDE_ESQUERDA                      44
#define PAREDE_CENTRO                        45
#define PAREDE_DIREITA                       46
#define MEIO_ESQUERDA                        47
#define MEIO_DIREITA                         48
#define GRAMA_01_01                          49
#define GRAMA_01_02                          50
#define GRAMA_01_03                          51
#define GRAMA_02_01                          52
#define GRAMA_02_02                          53
#define GRAMA_02_03                          54
#define GRAMA_03_01                          55
#define GRAMA_03_02                          56
#define GRAMA_03_03                          57
#define ARVORE_TOPO                          58
#define ARVORE_CORPO                         59
#define PALMEIRA_CORPO                       60
#define PALMEIRA_CENTRO_01                   61
#define PALMEIRA_CENTRO_02                   62
#define PALMEIRA_CENTRO_03                   63
#define PALMEIRA_ESQUERDA_01                 64
#define PALMEIRA_ESQUERDA_02                 65
#define PALMEIRA_ESQUERDA_03                 66
#define PALMEIRA_DIREITA_01                  67
#define PALMEIRA_DIREITA_02                  68
#define PALMEIRA_DIREITA_03                  69
#define PALMEIRA_TOPO_01                     70
#define PALMEIRA_TOPO_02                     71
#define PALMEIRA_TOPO_03                     72
#define ESTACA_CORPO                         73
#define ESTACA_TOPO                          74
#define MARIO_PEQUENO_01                     75
#define MARIO_PEQUENO_02                     76
#define MARIO_PEQUENO_PULANDO                77
#define MARIO_PEQUENO_MORTO                  78
#define MARIO_PEQUENO_PARADO                 79
#define BLOCO_MADEIRA                        80
#define BLOCO_AZULADO_01                     81
#define BLOCO_AZULADO_02                     82
#define BLOCO_AZULADO_03                     83
#define BLOCO_AZULADO_04                     84
#define FOLHA_ESQUERDA_01                    85
#define FOLHA_DIREITA_01                     86
#define FOLHA_ESQUERDA_02                    87
#define FOLHA_DIREITA_02                     88
#define FOLHA_ESQUERDA_03                    89
#define FOLHA_DIREITA_03                     90
#define FOLHA_ESQUERDA_04                    91
#define FOLHA_DIREITA_04                     92
#define CHAO_ESCURO_ESQUERDA                 93
#define CHAO_ESCURO_CENTRO                   94
#define CHAO_ESCURO_DIREITA                  95
#define MEIO_ESCURO_ESQUERDA                 96
#define MEIO_ESCURO_DIREITA                  97
#define PAREDE_ESCURA_ESQUERDA               98
#define PAREDE_ESCURA_CENTRO                 99
#define PAREDE_ESCURA_DIREITA                100
#define CHAO_PEDRA_ESQUERDA                  101
#define CHAO_PEDRA_CENTRO                    102
#define CHAO_PEDRA_DIREITA                   103
#define MEIO_PEDRA_ESQUERDA                  104
#define MEIO_PEDRA_DIREITA                   105
#define PAREDE_PEDRA_ESQUERDA                106
#define PAREDE_PEDRA_CENTRO                  107
#define PAREDE_PEDRA_DIREITA                 108
#define LAVA_01                              109
#define LAVA_02                              110
#define LAVA_03                              111
#define LAVA_04                              112
#define LAVA_05                              113
#define LAVA_CENTRO                          114
#define BLOCO_PEDRA                          115
#define BOLA_DE_FOGO_01                      116
#define BOLA_DE_FOGO_02                      117
#define BOLA_DE_FOGO_03                      118
#define BOLA_DE_FOGO_04                      119
#define CABO_SAIDA                           120               
#define MARIO_GRANDE_01                      121
#define MARIO_GRANDE_02                      122
#define MARIO_GRANDE_03                      123
#define MARIO_GRANDE_04                      124
#define MARIO_GRANDE_PULANDO                 125
#define MARIO_GRANDE_PARADO                  126
#define KOOPA_01                             127
#define KOOPA_02                             128
#define NUVEM_ESQUERDA                       129
#define NUVEM_CENTRO                         130
#define NUVEM_DIREITA                        131

#define MAX_PILHA_PARTIDOS                   200

/* biblioteca de imagens */
BITMAP *imagem[MAX_IMAGEM];

/* pilha de blocos partidos */
BITMAP *blocos_partidos[MAX_PILHA_PARTIDOS];
int topo_blocos_partidos;

/* para animacao dos tiles e sprites */
int proximo_quadro[MAX_IMAGEM];

void le_imagens(void);

#endif
