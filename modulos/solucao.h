#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <stdlib.h>  // malloc, rand
#include "mapa.h"
#include "util.h"

extern byte simetrias[8][9];
extern byte simetrias_reversa[8][9];

/**
 * Armazena uma possivel solucao para o jogo da velha
 * Usa o mapa para mapear os possiveis jogos dentro do jogo
 */
typedef struct Solucao Solucao;

extern int* mapa;
extern int tam_cromossomo;

/**
 * Cria um objeto solucao alocando ele na memoria
 */
Solucao* solucao_criar();

/**
 * Cria um objeto solucao com valores aleatorios
 */
Solucao* solucao_criar_random();

/**
 * Destroi um objeto solucao desalocando ele da memoria
 */
void solucao_apagar(Solucao** Solucao);

/**
 * Retorna a jogada que a solucao diz que deve ser feita para
 * uma determinada situacao do jogo
 */
byte solucao_get(Solucao* solucao, byte* jogo);

/**
 * Guarda na solucao que nesse jogo e em seus simetricos
 * deve ser feito a essa jogada
 */
void solucao_set(Solucao* solucao, byte* jogo, byte jogada);

/**
 * Retorna o valor que esta no gene i
 */
byte solucao_gene_get(Solucao* solucao, int i);

/**
 * O valor do gene i passa a valer val
 */
void solucao_gene_set(Solucao* solucao, int i, byte val);

/**
 * Salva a solucao no arquivo em path
 * Retorna se a operacao foi possivel
 */
bool solucao_salvar(Solucao* solucao, char* path);

#endif