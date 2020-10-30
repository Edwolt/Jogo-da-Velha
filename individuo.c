#include "individuo.h"

static int simetrias[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                              {2, 1, 0, 5, 4, 3, 8, 7, 6},
                              {2, 5, 8, 1, 4, 7, 0, 3, 6},
                              {8, 5, 2, 7, 4, 1, 6, 3, 0},
                              {8, 7, 6, 5, 4, 3, 2, 1, 0},
                              {6, 7, 8, 3, 4, 5, 0, 1, 2},
                              {6, 3, 0, 7, 4, 1, 8, 5, 2},
                              {0, 3, 6, 1, 4, 7, 2, 5, 8}};

// TODO revisar simetria reversa
static int simetrias_reversa[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                                      {2, 1, 0, 5, 4, 3, 8, 7, 6},
                                      {6, 3, 0, 7, 4, 1, 8, 5, 2},
                                      {8, 5, 2, 7, 4, 1, 6, 3, 0},
                                      {8, 7, 6, 5, 4, 3, 2, 1, 0},
                                      {6, 7, 8, 3, 4, 0, 0, 1, 2},
                                      {2, 5, 8, 1, 4, 7, 0, 3, 6},
                                      {0, 3, 6, 1, 4, 7, 2, 5, 8}};

static int powi(int x, int n) {
    int y = 1;
    while (n > 0) {
        if (n % 2 != 0) y *= x;
        x *= x;
        n /= 2;
    }
    return y;
}

static int calc_simetria(int* jogo) {
    int i, j;
    int minimo = 0x3f3f3f3f;  // Nao tem como dar um numero maior que N
    int sim = -1;
    int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias[i][j]] * powi(3, j);
        if (numero < minimo) {
            minimo = numero;
            sim = i;
        }
    }
    return sim;
}

static int calc_min(int* jogo, int i) {
    int j, numero = 0;
    for (j = 0; j < 9; j++) numero += jogo[simetrias[i][j]] * powi(3, j);
    return numero;
}

Individuo* individuo_novo() {
    Individuo* individuo = malloc(sizeof(individuo));
    individuo->genes = malloc(tam_cromossomo * sizeof(int));
    return individuo;
}

void individuo_apagar(Individuo** individuo) {
    if (!individuo || !*individuo) return;

    free((*individuo)->genes);
    free(*individuo);
    *individuo = NULL;
}

int individuo_get(Individuo* individuo, int* jogo) {
    int simetria = calc_simetria(jogo);
    int minimo = calc_min(jogo, simetria);
    int gene = mapa[minimo];
    int jogada = individuo->genes[gene];
    return simetrias_reversa[simetria][jogada];
}

void individuo_set(Individuo* individuo, int* jogo, int jogada) {
    int simetria = calc_simetria(jogo);
    int minimo = calc_min(jogo, simetria);
    int gene = mapa[minimo];

    jogada = simetrias[simetria][jogada];
    individuo->genes[gene] = jogada;
}
