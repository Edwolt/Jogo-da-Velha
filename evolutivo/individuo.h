#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <math.h>
#include "solucao.h"

/**
 * Armazena dados do individuo
 * incluindo a solucao e outros dados pertinentes
 * 
 * Possui metodos para ser individuo do algoritmo evolutivo
 */
typedef struct Individuo {
    Solucao* sol;
    int fitness;
} Individuo;

//* ===== Criar e Apagar ===== *//

/**
 * Cria objeto individuo alocando ele na memoria
 */
Individuo* individuo_criar();

/**
 * Cria um objeto individuo com valores aleatorios
 */
Individuo* individuo_criar_random();

/**
 * Destroi um objeto individuo desalocando ele na memoria
 */
void individuo_apagar(Individuo** individuo);

//* ===== Jogar ===== *//

/**
 * Faz dois inviduos jogar atualizando o elo deles
 */
int individuo_jogar(Individuo* a, Individuo* b);

//* ===== Outros metodos ===== *//

/**
 * Cria um novo individuo
 * com um mistrura das caracteristicas do pai e da mae
 */
Individuo* individuo_crossover(Individuo* pai, Individuo* mae);

/**
 * Aplica mutacao no individuo
 * Cada gene tem o valor do parametro mutacao de chance de sofrer mutacao
 * E garantido que pelo menos um gene sofrera mutacao
 */
void individuo_mutacao(Individuo* individuo, double mutacao);

#endif