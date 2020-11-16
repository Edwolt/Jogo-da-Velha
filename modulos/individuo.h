#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <stdlib.h>  // malloc, rand
#include "mapa.h"
#include "util.h"

extern byte simetrias[8][9];
extern byte simetrias_reversa[8][9];

/**
 * Armazena uma possivel solucao para o jogo da velha
 * Usa o mapa para mapear os possiveis jogos dentro do jogo
 */
typedef struct Individuo Individuo;

extern int* mapa;
extern int tam_cromossomo;

/**
 * Cria um objeto individuo alocando ele na memoria
 */
Individuo* individuo_criar();

/**
 * Cria um objeto individuo com valores aleatorios
 */
Individuo* individuo_criar_random();

/**
 * Destroi um objeto solucao desalocando ele da memoria
 */
void individuo_apagar(Individuo** individuo);

/**
 * Retorna a jogada que a individuo diz que deve ser feita para
 * uma determinada situacao do jogo
 */
byte individuo_get(Individuo* individuo, byte* jogo);

/**
 * Guarda no individuo que nesse jogo e em seus simetricos
 * deve ser feito a essa jogada
 */
void individuo_set(Individuo* individuo, byte* jogo, byte jogada);

/**
 * Retorna o valor que esta no gene i
 */
byte individuo_gene_get(Individuo* individuo, int i);

/**
 * O valor do gene i passa a valer val
 */
void individuo_gene_set(Individuo* individuo, int i, byte val);

/**
 * Salva o individuo no arquivo em path
 * Retorna se a operacao foi possivel
 */
bool individuo_salvar(Individuo* individuo, char* path);

#endif