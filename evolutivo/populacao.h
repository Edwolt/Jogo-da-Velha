#ifndef POPULCAO_H
#define POPULCAO_H

#include <stdbool.h>
#include <stdlib.h>
#include "mapa.h"
#include "solucao.h"

extern int tam_cromossomo;

typedef struct Populacao Populacao;

Populacao* populacao_criar(int n);

void populacao_apagar(Populacao** populacao);

/**
 * Calcula fitness de cada individuo na populacao
 * Protege o melhor de todos de mutação e ajuda no algoritmo de sintese
 */
bool populacao_fitness(Populacao* populacao);

bool populacao_torneio(Populacao* populacao);

bool populacao_elitismo(Populacao* populacao);

void populacao_mutacao(Populacao* populacao, double mutacao);

bool populacao_predacao_sintese(Populacao* populacao);

bool populacao_predacao_randomica(Populacao* populacao, int n);

void populacao_salvar(Populacao* populacao, char* melhor, char* todos);

int populacao_get_fitness(Populacao* populacao, int i);

#endif