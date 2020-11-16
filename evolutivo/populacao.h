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

/**
 * Cria um objeto populacao alocando ele na memoria
 */
Populacao* populacao_criar(int n);

/**
 * Destroi um objeto populacao desalocando ele na memoria
 */
void populacao_apagar(Populacao** populacao);

/**
 * Calcula fitness de cada individuo na populacao
 * Ordena as solucoes pelo valor do fitness
 * (serve para proteger o melhor de todos de mutacao e de fazer que as predacoes aconteca nos piores)
 */
bool populacao_fitness(Populacao* populacao);

/**
 * Cria uma nova populacao usando torneio de dois
 */
bool populacao_torneio(Populacao* populacao);

/**
 * Aplica mutacao na populacao
 * Cada gene tem o valor do parametro mutacao de chance de sofrer mutacao
 * E garantido que pelo menos um gene de cada individuo sofrera mutacao
 */
void populacao_mutacao(Populacao* populacao, double mutacao);

/**
 * Gera uma nova por predacao por sintese
 * (individuo com os genes mais comuns na populacao)
 * e troca pelo pior de todos
 */
bool populacao_predacao_sintese(Populacao* populacao);

/**
 * Gera n novas solucoes trocando as piores solucoes
 * (Exceto o pior de todos que esperasse ser trocado pela predacao por sintese)
 */
bool populacao_predacao_randomica(Populacao* populacao, int n);

/**
 * Salva o melhor individuo no arquivo em path
 * Retorna se a operacao foi possivel
 */
void populacao_salvar_melhor(Populacao* populacao, char* path);

/**
 * Retorna o fitness da individuo i
 */
int populacao_get_fitness(Populacao* populacao, int i);

#endif