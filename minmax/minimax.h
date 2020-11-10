#ifndef MINIMAX_H
#define MINIMAX_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * Retorna se aquela jogada resultara em uma vitoria, empate ou derrota
 */
int minimax(int* jogo, int vez, int maximizador);

/**
 * Retorna entre as jogadas possiveis uma que resultara em vitoria
 * Senao pelo menos uma que resultara em empate
 * Senao retorna uma entre as jogadas disponiveis
 */
int melhor_jogada(int* jogo);

#endif