#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <stdlib.h>
#include "mapa.h"

typedef struct Solucao Solucao;

extern int* mapa;
extern int tam_cromossomo;

Solucao* solucao_criar();
void solucao_apagar(Solucao** solucao);

int solucao_get(Solucao* solucao, int* jogo);
void solucao_set(Solucao* solucao, int* jogo, int jogada);

void solucao_salvar(Solucao* solucao, char* path);

#endif