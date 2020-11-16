#ifndef MINIMAX_H
#define MINIMAX_H

#include <stdbool.h>
#include <stdlib.h>
#include "util.h"

extern byte vitorias[8][3];

/**
 * Retorna se aquela jogada resultara em uma vitoria (1), empate (0) ou derrota (-1)
 */
int minimax(byte* jogo, byte vez, byte maximizador);

/**
 * Retorna entre as jogadas possiveis uma que resultara em vitoria
 * Senao pelo menos uma que resultara em empate
 * Senao retorna uma entre as jogadas disponiveis
 */
int melhor_jogada(byte* jogo);

#endif