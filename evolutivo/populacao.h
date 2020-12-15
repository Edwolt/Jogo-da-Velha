#ifndef POPULCAO_H
#define POPULCAO_H

#include <stdbool.h>
#include <stdlib.h>
#include "individuo.h"
#include "mapa.h"

extern int tam_cromossomo;

/**
 * Armazena uma conjunto de solucoes,
 * onde cada solucao eh um individuo da populacao
 * 
 * Possui metodos para ser populacao do algoritmo evolutivo
 */
typedef struct Populacao Populacao;

//* ===== Criar e Apagar ===== *//

/**
 * Cria um objeto populacao alocando ele na memoria
 */
Populacao* populacao_criar(int n);

/**
 * Destroi um objeto populacao desalocando ele na memoria
 */
void populacao_apagar(Populacao** populacao);

//* ===== Ordena ===== *//

/**
 * Ordena a populacao pelo elo de cada individuo
 */
void populacao_ordena_elo(Populacao* populacao);

//* ===== Reproducao ===== *//

/**
 * Cria uma nova populacao usando o metodo elitismo
 * baseado no elo dos individuos
 */
bool populacao_elitismo_elo(Populacao* populacao, int n);

/**
 * Cria uma nova populacao usando o metodo torneio de dois
 * baseado no elo dos individuos
 */
bool populacao_torneio_elo(Populacao* populacao, int n);

//* ===== Mutacao ===== *//

/**
 * Aplica mutacao na populacao
 * Cada gene tem o valor do parametro mutacao de chance de sofrer mutacao
 * E garantido que pelo menos um gene de cada individuo sofrera mutacao
 */
void populacao_mutacao(Populacao* populacao, double mutacao);

//* ===== Predacao ===== *//

/**
 * Gera uma nova por predacao por sintese
 * (individuo com os genes mais comuns na populacao)
 * e troca pelo pior de todos exceto os n pior que eh da predacaoo radomica
 */
bool populacao_predacao_sintese(Populacao* populacao, int n);

/**
 * Gera n novas solucoes aleatorias trocando as piores solucoes
 */
bool populacao_predacao_randomica(Populacao* populacao, int n);

//* ===== Getters e Setters =====*//

Individuo* populacao_get_individuo(Populacao* populacao, int i);

#endif