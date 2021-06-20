#include "minimax.h"

int minimax(byte* jogo, byte vez, byte maximizador) {
    byte* jogo_aux = NULL;

    int i;
    int aux;
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
static byte proximo_jogador(byte* jogo) {
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

int melhor_jogada(byte* jogo) {
    byte* jogo_aux = NULL;

    int i;
    byte atual = proximo_jogador(jogo), proximo = trocar_vez(atual);
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
