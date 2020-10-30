#include "minimax.h"

static int vitorias[8][3] = {{0, 1, 2},
                             {3, 4, 5},
                             {6, 7, 8},
                             {0, 3, 6},
                             {1, 4, 7},
                             {2, 5, 8},
                             {0, 4, 8},
                             {2, 4, 6}};

inline static int trocar_vez(int vez) { return (vez == 1 ? 2 : 1); }

static int calc_vencedor(int* jogo) {
    int i;
    int a, b, c;
    for (i = 0; i < 8; i++) {
        a = vitorias[i][0];
        b = vitorias[i][1];
        c = vitorias[i][2];
        if (jogo[a] != 0 && jogo[a] == jogo[b] && jogo[b] == jogo[c]) return jogo[a];
    }
    return 0;
}

static int* copiar_jogo(int* jogo) {
    int i;
    int* novo_jogo = malloc(9 * sizeof(int));
    for (i = 0; i < 9; i++) novo_jogo[i] = jogo[i];
    return novo_jogo;
}

int minimax(int* jogo, int vez, int maximizador) {
    int i;

    // Se for um nó terminal
    int vencedor = calc_vencedor(jogo);
    if (vencedor == maximizador) {
        return 1;
    } else if (vencedor == 0) {
        return 0;
    } else {
        return -1;
    }

    if (vez == maximizador) {  // Se for a minha vez
        bool flag;
        for (i = 0; i < 9; i++) {
            flag = false;
            int* novo_jogo = copiar_jogo(jogo);
            novo_jogo[i] = vez;
            int mm = minimax(novo_jogo, trocar_vez(vez), maximizador);
            free(novo_jogo);
            if (mm == 1) {
                return 1;
            } else if (mm == 0) {
                flag = true;
            }
        }
        return (flag ? 0 : -1);
    } else {  // Se for a vez do oponente
        bool flag;
        for (i = 0; i < 9; i++) {
            flag = false;
            int* novo_jogo = copiar_jogo(jogo);
            novo_jogo[i] = vez;
            int mm = minimax(novo_jogo, trocar_vez(vez), maximizador);
            free(novo_jogo);
            if (mm == -1) {
                return -1;
            } else if (mm == 0) {
                flag = true;
            }
        }
        return (flag ? 0 : 1);
    }
}

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
    int proximo = proximo_jogador(jogo);
    int* jogo_aux = NULL;

    int mm, melhor = -2, jogada = -2;
    for (i = 0; i < 9; i++) {
        if (jogo[i] == 0) {
            jogo_aux = copiar_jogo(jogo);
            if (!jogo_aux) goto falha;

            jogo_aux[i] = proximo;
            mm = minimax(jogo_aux, trocar_vez(proximo), proximo);
            if (mm > melhor) {
                melhor = mm;
                jogada = i;
            }

            free(jogo_aux);
        }
    }

    return jogada;

falha:
    free(jogo_aux);
    return -1;
}