#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <stdlib.h>
#include "mapa.h"
#include "util.h"

extern int simetrias[8][9];
extern int simetrias_reversa[8][9];

/**
 * Armazena uma possivel solucao para o jogo da velha
 * Usa o mapa para mapear os possiveis jogos dentro do jogo
 */
typedef struct Solucao Solucao;

/**
 * Valor de mapa.h
 */
extern int* mapa;

/**
 * Valor de mapa.h
 */
extern int tam_cromossomo;

/**
 * Cria um objeto solucao alocando ele na memoria
 */
Solucao* solucao_criar();

/**
 * Destroi um objeto solucao desalocando ele da memoria
 */
void solucao_apagar(Solucao** solucao);

/**
 * Retorna a jogada que a solucao diz que deve ser feita para
 * uma determinada situacao do jogo
 */
int solucao_get(Solucao* solucao, int* jogo);

/**
 * Guarda em solucao que nesse jogo e em seus simetricos
 * deve ser feito a essa jogada
 */
void solucao_set(Solucao* solucao, int* jogo, int jogada);

/**
 * Salva a solucao no arquivo em path
 */
void solucao_salvar(Solucao* solucao, char* path);

#endif