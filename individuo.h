#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <stdlib.h>

typedef struct Individuo Individuo;

Individuo* individuo_novo(int n);
void individuo_apagar(Individuo** individuo);

int individuo_get(Individuo* individuo, int* jogo);
void individuo_set(Individuo* individuo, int* jogo, int jogada);

#endif