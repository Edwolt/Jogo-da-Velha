#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdlib.h>

typedef signed char byte;

/**
 * jogada -> jogada'
 * Pega uma jogada e retorna onde ela seria naquela simtria
 */
extern byte simetrias[8][9];

/**
 * jogada <- jogada'
 * Pega uma jogada em um jogo que passou por simtria e 
 * a tranforma em uma jogada antes de passar pela simtria
 */
extern byte simetrias_reversa[8][9];

/**
 * Vetor com todas as opcoes de vitoria
 */
extern byte vitorias[8][3];

//* ===== Inline ===== *//

/**
 * Retorna x elevado n
 */
inline static int powi(int x, int n) {
    int y = 1;
    while (n > 0) {
        if (n % 2 != 0) y *= x;
        x *= x;
        n /= 2;
    }
    return y;
}

inline static int min(int a, int b) { return (a < b ? a : b); }
inline static int max(int a, int b) { return (a > b ? a : b); }
// inline static int abs(int a) { return (a > 0 ? a : -a); }
inline static byte trocar_vez(byte vez) { return (vez == 1 ? 2 : 1); }

//* ===== Calc ===== *//

/**
 * Retorna quem venceu o jogo
 * 0: Ainda tem jogadas a ser feitas
 * 1: O jogador X ganhou
 * 2: O jogador O ganhou
 * 3: deu velha
 */
byte calc_vencedor(byte* jogo);

/**
 * Retorna se alguém venceu o jogo
 */
inline static bool venceu(byte* jogo) {
    int vencedor = calc_vencedor(jogo);
    return vencedor == 1 || vencedor == 2;
}

/**
 * Retorna se o jogo deu velha
 */
inline static bool velha(byte* jogo) { return calc_vencedor(jogo) == 3; }

/**
 * Retorna true se alguem deu venceu o jogo
 * Retorna true se deu velha
 */
inline static bool venceu_ou_velha(byte* jogo) {
    byte vencedor = calc_vencedor(jogo);
    return vencedor == 1 || vencedor == 2 || vencedor == 3;
}

/**
 * Retorna o menor valor entre os jogos e seus simetricos
 */
int calc_min(byte* jogo);

/**
 * Retorna qual a simtria usada para que o jogo tenha o menor valor possivel
 */
int calc_sim(byte* jogo);

/**
 * Retorna o valor so jogo na simetria sim
 */
int calc_val(byte* jogo, int sim);

//* ===== Jogo ===== *//

/**
 * Retorna em jogos uma lista de todos os jogos possiveis de acontecer
 * Retorna em n quantas posicoes foram usadas
 */
byte** jogos_possiveis(int* n);

/**
 * Cria uma copia do vetor jogo
 */
byte* copiar_jogo(byte* jogo);

#endif