#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdlib.h>

typedef signed char byte;

extern const int N;  // Numero de arranjos possiveis de X e O
extern const int M;  // Numero maximo de recursoes de jogo_possiveis()

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

/**
 * Retorna 3 elevado n
 * pow3[i] = (int) pow(3, i)
 */
extern int pow3[10];

//* ===== Inline ===== *//

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
inline static byte calc_vencedor(byte* jogo) {
    int i;
    byte a, b, c;
    for (i = 0; i < 8; i++) {
        a = vitorias[i][0];
        b = vitorias[i][1];
        c = vitorias[i][2];
        if (jogo[a] != 0 && jogo[a] == jogo[b] && jogo[b] == jogo[c]) return jogo[a];
    }

    for (i = 0; i < 9; i++) {
        if (jogo[i] == 0) return 0;
    }

    return 3;
}

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

/*
calc_min, calc_sim e calc_val usa simetrias_reversa e nao simetrias
a ideia é que eu passasse o jogo pela simetria e depois percorresse calculando o valor

for i in 0..8
    jogo_simetrico = aplica simetria[i] no jogo
    for j in 0..9
        numero += jogo_simetrico[j]

Para conseguir fazer isso sem ter que aplicar a simetria ao jogo
eu posso transformar a posicao j de uma posicao j dentro da simetria i
para a posicao equivalente no jogo especifico que foi passado,
e para isso usamos simtreias_reversa
*/

/**
 * Retorna o valor do jogo
 */
inline static int calc_jogo(byte* jogo) {
    int i;
    register int numero = 0;
    for (i = 0; i < 9; i++) numero += jogo[i] * pow3[i];
    return numero;
}

/**
 * Retorna o menor valor entre os jogos e seus simetricos
 */
inline static int calc_min(byte* jogo) {
    int i, j;
    int minimo = 3 * N;  // Nao tem como dar um numero maior que N
    register int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[i][j]] * pow3[j];
        minimo = min(numero, minimo);
    }
    return minimo;
}

/**
 * Retorna qual a simtria usada para que o jogo tenha o menor valor possivel
 */
inline static int calc_sim(byte* jogo) {
    int i, j;
    int minimo = 3 * N;  // Nao tem como dar um numero maior que N
    int sim = -1;
    register int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[i][j]] * pow3[j];
        if (numero < minimo) {
            minimo = numero;
            sim = i;
        }
    }
    return sim;
}

/**
 * Retorna o valor so jogo na simetria sim
 */
inline static int calc_val(byte* jogo, int sim) {
    int j;
    register int numero = 0;
    for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[sim][j]] * pow3[j];
    return numero;
}

//* ===== Jogo ===== *//

/**
 * Retorna em jogos uma lista de todos os jogos possiveis de acontecer
 * Retorna em n quantas posicoes foram usadas
 */
byte** jogos_possiveis(int* n);

/**
 * Cria uma copia do vetor jogo
 */
inline static byte* copiar_jogo(byte* jogo) {
    byte* novo_jogo = NULL;
    int i;

    novo_jogo = malloc(9 * sizeof(byte));
    if (!novo_jogo) return NULL;

    for (i = 0; i < 9; i++) novo_jogo[i] = jogo[i];
    return novo_jogo;
}

#endif
