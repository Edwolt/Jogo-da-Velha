#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdlib.h>

inline static int min(int a, int b) { return (a < b ? a : b); }
inline static int max(int a, int b) { return (a > b ? a : b); }

/**
 * jogada -> jogada'
 * Pega uma jogada e retorna onde ela seria naquela simtria
 */
extern int simetrias[8][9];

/**
 * jogada <- jogada'
 * Pega uma jogada em um jogo que passou por simtria e 
 * a tranforma em uma jogada antes de passar pela simtria
 */
extern int simetrias_reversa[8][9];

/**
 * Vetor com todas as opcoes de vitoria
 */
extern int vitorias[8][3];

/**
 * Retorna x elevado n
 */
int powi(int x, int n);

/**
 * Retorna se alguém venceu o jogo
 */
bool venceu(int* jogo);

/**
 * Retorna se o jogo deu velha
 */
bool cheio(int* jogo);

/**
 * Retorna o menor valor entre os jogos e seus simetricos
 */
int calc_min(int* jogo);

/**
 * Retorna qual a simtria usada para que o jogo tenha o menor valor possivel
 */
int calc_sim(int* jogo);

/**
 * Retorna o valor so jogo na simetria sim
 */
int calc_val(int* jogo, int sim);

/**
 * Retorna em jogos uma lista de todos os jogos possiveis de acontecer
 * Retorna em n quantas posicoes foram usadas
 */
int** jogos_possiveis(int* n);

/**
 * Cria uma copia do vetor jogo
 */
int* copiar_jogo(int* jogo);

#endif