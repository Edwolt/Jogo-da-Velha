#ifndef MINIMAX_H
#define MINIMAX_H

#include <stdbool.h>
#include "solucao.h"

int minimax(int* jogo, int vez, int maximizador);
int melhor_jogada(int* jogo);

#endif