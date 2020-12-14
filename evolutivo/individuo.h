#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include "solucao.h"

/**
 * Armazena dados do individuo
 * incluindo a solucao e outros dados pertinentes
 * 
 * Possui metodos para ser individuo do algoritmo evolutivo
 */
typedef struct Individuo Individuo;

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
 * Faz com que os dois individuos joguem o jogo da velha um contra o outro
 * 
 * Ocorre uma partida com o a comecando e outra com o b comecando
 * se o a ganhar aumenta 1 ponto e se perder diminui um ponto do valor retornado
 */
int individuo_jogar(Individuo* a, Individuo* b);

void individuo_jogar_elo(Individuo* a, Individuo* b);

/**
 * O dois individuos jogam atualizando o fitness
 */
void individuo_jogar_fitness(Individuo* a, Individuo* b);

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

//* ===== Getters e Setters =====*//
double individuo_get_elo(Individuo* individuo);
void individuo_set_elo(Individuo* individuo, double val);

byte individuo_get_gene(Individuo* individuo, int i);
void individuo_set_gene(Individuo* individuo, int i, int gene);

Solucao* individuo_get_solucao(Individuo* individuo);

int individuo_get_fitness(Individuo* individuo);
void individuo_set_fitness(Individuo* individuo, int val);

#endif