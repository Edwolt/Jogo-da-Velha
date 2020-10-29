#include "individuo.h"

// static int simetrias[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
//                               {2, 1, 0, 5, 4, 3, 8, 7, 6},
//                               {2, 5, 8, 1, 4, 7, 0, 3, 6},
//                               {8, 5, 2, 7, 4, 1, 6, 3, 0},
//                               {8, 7, 6, 5, 4, 3, 2, 1, 0},
//                               {6, 7, 8, 3, 4, 5, 0, 1, 2},
//                               {6, 3, 0, 7, 4, 1, 8, 5, 2},
//                               {0, 3, 6, 1, 4, 7, 2, 5, 8}};

struct Individuo {
    int* genes;
};

Individuo* individuo_novo(int n) {
    Individuo* individuo = malloc(sizeof(individuo));
    individuo->genes = malloc(n * sizeof(int));
    return individuo;
}

void individuo_apagar(Individuo** individuo) {
    if (!individuo || !*individuo) return;

    free((*individuo)->genes);
    free(*individuo);
    *individuo = NULL;
}

// int individuo_get(Individuo* individuo, int* jogo) {
//     int simetria = calc_simetria(jogo);
//     int minimo = calc_min(jogo, simetria);
//     int gene = mapa[minimo];
//     int jogada = individuo->genes[gene];
//     return simetria_reversa[simetria][jogada];
// }

// void individuo_set(Individuo* individuo, int* jogo, int jogada) {
//     int simetria = calc_simetria(jogo);
//     int minimo = calc_min(jogo, simetria);
//     int gene = mapa[minimo];

//     jogada = simetrias[simetria][jogada];
//     individuo->genes = jogada;
// }
