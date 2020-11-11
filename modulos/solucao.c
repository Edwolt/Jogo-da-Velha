#include "solucao.h"

struct Solucao {
    int* cromossomo;
};

Solucao* solucao_criar() {
    Solucao* solucao = malloc(sizeof(solucao));
    solucao->cromossomo = malloc(tam_cromossomo * sizeof(int));
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
    int sim = calc_sim(jogo);
    int minimo = calc_val(jogo, sim);
    int gene = mapa[minimo];
    int jogada = solucao->cromossomo[gene];
    return simetrias_reversa[sim][jogada];
}

void solucao_set(Solucao* solucao, int* jogo, int jogada) {
    int sim = calc_sim(jogo);
    int minimo = calc_val(jogo, sim);
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