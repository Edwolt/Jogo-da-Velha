#include "solucao.h"

struct Solucao {
    int* cromossomo;
};

static int simetrias[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                              {2, 1, 0, 5, 4, 3, 8, 7, 6},
                              {2, 5, 8, 1, 4, 7, 0, 3, 6},
                              {8, 5, 2, 7, 4, 1, 6, 3, 0},
                              {8, 7, 6, 5, 4, 3, 2, 1, 0},
                              {6, 7, 8, 3, 4, 5, 0, 1, 2},
                              {6, 3, 0, 7, 4, 1, 8, 5, 2},
                              {0, 3, 6, 1, 4, 7, 2, 5, 8}};

static int simetrias_reversa[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                                      {2, 1, 0, 5, 4, 3, 8, 7, 6},
                                      {6, 3, 0, 7, 4, 1, 8, 5, 2},
                                      {8, 5, 2, 7, 4, 1, 6, 3, 0},
                                      {8, 7, 6, 5, 4, 3, 2, 1, 0},
                                      {6, 7, 8, 3, 4, 5, 0, 1, 2},
                                      {2, 5, 8, 1, 4, 7, 0, 3, 6},
                                      {0, 3, 6, 1, 4, 7, 2, 5, 8}};

/**
 * Retorna x elevado a
 */
static int powi(int x, int n) {
    int y = 1;
    while (n > 0) {
        if (n % 2 != 0) y *= x;
        x *= x;
        n /= 2;
    }
    return y;
}

/**
 * Retorna qual a simtria usada para que o jogo tenha o menor valor possivel
 */
static int calc_simetria(int* jogo) {
    int i, j;
    int minimo = 0x3f3f3f3f;
    int sim = -1;
    int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[i][j]] * powi(3, j);
        if (numero < minimo) {
            minimo = numero;
            sim = i;
        }
    }
    return sim;
}

/**
 * Retorna o menor valor entre os jogos e seus simetricos
 */
static int calc_min(int* jogo, int i) {
    int j, numero = 0;
    for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[i][j]] * powi(3, j);
    return numero;
}

Solucao* solucao_criar() {
    Solucao* solucao = malloc(sizeof(solucao));
    solucao->cromossomo= malloc(tam_cromossomo * sizeof(int));
    int i;
    for (i = 0; i < tam_cromossomo; i++) solucao->cromossomo[i] = -1;
    return solucao;
}

void solucao_apagar(Solucao** solucao) {
    if (!solucao || !*solucao) return;

    free((*solucao)->cromossomo);
    free(*solucao);
    *solucao = NULL;
}

int solucao_get(Solucao* solucao, int* jogo) {
    int sim = calc_simetria(jogo);
    int minimo = calc_min(jogo, sim);
    int gene = mapa[minimo];
    int jogada = solucao->cromossomo[gene];
    return simetrias_reversa[sim][jogada];
}

void solucao_set(Solucao* solucao, int* jogo, int jogada) {
    int sim = calc_simetria(jogo);
    int minimo = calc_min(jogo, sim);
    int gene = mapa[minimo];

    jogada = simetrias[sim][jogada];
    solucao->cromossomo[gene] = jogada;
}

void solucao_salvar(Solucao* solucao, char* path) {
    FILE* file = fopen(path, "w");
    int i;
    for (i = 0; i < tam_cromossomo; i++) {
        fprintf(file, "%d\n", solucao->cromossomo[i]);
    }
    fclose(file);
}