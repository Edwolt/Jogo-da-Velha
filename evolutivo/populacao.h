#ifndef POPULCAO_H
#define POPULCAO_H

#include <stdbool.h>
#include <stdlib.h>
#include "individuo.h"
#include "mapa.h"

extern int tam_cromossomo;

/**
 * Armazena uma conjunto de individuos
 */
typedef struct Populacao Populacao;

//* ===== Operacoes Basicas ===== *//

/**
 * Cria um objeto populacao alocando ele na memoria
 */
Populacao* populacao_criar(int n);

/**
 * Destroi um objeto populacao desalocando ele na memoria
 */
void populacao_apagar(Populacao** populacao);

//* ===== Ordena ===== *//

void populacao_ordena(Populacao* populacao);
void populacao_ordena_chave(Populacao* populacao);
void populacao_ordena_fitness(Populacao* populacao);

//* ===== Reproducao ===== *//

bool populacao_elitismo(Populacao* populacao);

/**
 * Cria uma nova populacao usando torneio de dois
 */
bool populacao_torneio(Populacao* populacao);

/**
 * Cria uma nova populacao usando um sistema de chaves
 * onde quem ganhar disputa com os outros que ganhou
 * ate descobri quem eh o melhor de todos
 */
bool populacao_chave(Populacao* populacao);

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

//* ===== Outras operacoes ===== *//

/**
 * Salva o melhor individuo no arquivo em path
 * Retorna se a operacao foi possivel
 */
void populacao_salvar_melhor(Populacao* populacao, char* path);

Individuo* populacao_get_individuo(Populacao* populacao, int i);

int* populacao_fitness(Populacao* populacao);

#endif