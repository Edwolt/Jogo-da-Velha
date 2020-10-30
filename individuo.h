#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <stdlib.h>
#include "mapa.h"

typedef struct Individuo Individuo;
struct Individuo {
    int* genes;
};

extern int* mapa;
extern int tam_cromossomo;

Individuo* individuo_novo();
void individuo_apagar(Individuo** individuo);

int individuo_get(Individuo* individuo, int* jogo);
void individuo_set(Individuo* individuo, int* jogo, int jogada);

void individuo_salvar(Individuo* individuo, char* path);

#endif