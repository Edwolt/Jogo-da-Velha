#include "minimax.h"

/**
 * Vetor com todas as opcoes de vitoria
 */
static int vitorias[8][3] = {{0, 1, 2},
                             {3, 4, 5},
                             {6, 7, 8},
                             {0, 3, 6},
                             {1, 4, 7},
                             {2, 5, 8},
                             {0, 4, 8},
                             {2, 4, 6}};

inline static int trocar_vez(int vez) { return (vez == 1 ? 2 : 1); }

/**
 * Retorna quem venceu o jogo
 * 0: Ainda tem jogadas a ser feitas
 * 1: O jogador X ganhou
 * 2: O jogador O ganhou
 * 3: deu velha
 */
static int calc_vencedor(int* jogo) {
    int i;
    int a, b, c;
    for (i = 0; i < 8; i++) {
        a = vitorias[i][0];
        b = vitorias[i][1];
        c = vitorias[i][2];
        if (jogo[a] != 0 && jogo[a] == jogo[b] && jogo[b] == jogo[c]) return jogo[a];
    }
    bool zero = false;
    for (i = 0; i < 9; i++) {
        if (jogo[i] == 0) zero = true;
    }

    return (zero ? 0 : 3);
}

/**
 * Cria uma copia do vetor jogo
 */
static int* copiar_jogo(int* jogo) {
    int i;
    int* novo_jogo = malloc(9 * sizeof(int));
    for (i = 0; i < 9; i++) novo_jogo[i] = jogo[i];
    return novo_jogo;
}

int minimax(int* jogo, int vez, int maximizador) {
    int i;
    int aux;
    int* jogo_aux = NULL;
    bool zero;

    // Se for um nó terminal
    int vencedor = calc_vencedor(jogo);
    if (vencedor == maximizador) {
        return 1;
    } else if (vencedor == 3) {
        return 0;
    } else if (vencedor != 0) {
        return -1;
    }

    if (vez == maximizador) {  // Se for a minha vez
        zero = false;
        for (i = 0; i < 9; i++) {
            if (jogo[i] != 0) continue;

            jogo_aux = copiar_jogo(jogo);
            if (!jogo_aux) goto falha;
            jogo_aux[i] = vez;

            aux = minimax(jogo_aux, trocar_vez(vez), maximizador);
            free(jogo_aux);

            if (aux == 1) {
                return 1;
            } else if (aux == 0) {
                zero = true;
            }
        }
        return (zero ? 0 : -1);
    } else {  // Se for a vez do oponente
        zero = false;
        for (i = 0; i < 9; i++) {
            if (jogo[i] != 0) continue;

            jogo_aux = copiar_jogo(jogo);
            if (!jogo_aux) goto falha;
            jogo_aux[i] = vez;

            aux = minimax(jogo_aux, trocar_vez(vez), maximizador);
            free(jogo_aux);

            if (aux == -1) {
                return -1;
            } else if (aux == 0) {
                zero = true;
            }
        }
        return (zero ? 0 : 1);
    }

falha:
    return 0;
}

/**
 * Calcula qual o proximo jogador a jogar
 */
static int proximo_jogador(int* jogo) {
    int i;
    int x = 0, o = 0;
    for (i = 0; i < 9; i++) {
        if (jogo[i] == 1) {
            x++;
        } else if (jogo[i] == 2) {
            o++;
        }
    }
    return (x == o ? 1 : 2);
}

int melhor_jogada(int* jogo) {
    int i;
    int atual = proximo_jogador(jogo);
    int proximo = trocar_vez(atual);
    int* jogo_aux = NULL;

    int aux, melhor = -2, jogada = -2;
    for (i = 0; i < 9; i++) {
        if (jogo[i] != 0) continue;

        jogo_aux = copiar_jogo(jogo);
        if (!jogo_aux) goto falha;
        jogo_aux[i] = atual;

        aux = minimax(jogo_aux, proximo, atual);
        if (aux > melhor) {
            melhor = aux;
            jogada = i;
        }

        free(jogo_aux);
    }

    return jogada;

falha:
    free(jogo_aux);
    return -1;
}