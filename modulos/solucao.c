#include "solucao.h"

struct Solucao {
    int* cromossomo;
};

Solucao* solucao_criar() {
    Solucao* solucao = malloc(sizeof(solucao));
    if (!solucao) return NULL;

    solucao->cromossomo = malloc(tam_cromossomo * sizeof(int));
    int i;
    for (i = 0; i < tam_cromossomo; i++) solucao->cromossomo[i] = -1;
    return solucao;
}

Solucao* solucao_criar_random() {
    Solucao* solucao = solucao_criar();
    if (!solucao) return NULL;

    int i;
    for (i = 0; i < tam_cromossomo; i++) solucao->cromossomo[i] = rand() % 9;
    return solucao;
}

void solucao_apagar(Solucao** solucao) {
    if (!solucao || !*solucao) return;

    free((*solucao)->cromossomo);
    free(*solucao);
    *solucao = NULL;
}

int solucao_get(Solucao* solucao, int* jogo) {
    if (!solucao) return -1;

    int sim = calc_sim(jogo);
    int minimo = calc_val(jogo, sim);
    int gene = mapa[minimo];
    int jogada = solucao->cromossomo[gene];
    return simetrias_reversa[sim][jogada];
}

void solucao_set(Solucao* solucao, int* jogo, int jogada) {
    if (!solucao) return;

    int sim = calc_sim(jogo);
    int minimo = calc_val(jogo, sim);
    int gene = mapa[minimo];

    jogada = simetrias[sim][jogada];
    solucao->cromossomo[gene] = jogada;
}

int solucao_gene_get(Solucao* solucao, int i) {
    if (!solucao) return -1;
    return solucao->cromossomo[i];
}

void solucao_gene_set(Solucao* solucao, int i, int val) {
    if (!solucao) return;
    solucao->cromossomo[i] = val;
}

bool solucao_salvar(Solucao* solucao, char* path) {
    if (!solucao) return false;

    FILE* arquivo = NULL;
    int i;

    arquivo = fopen(path, "w");
    if (!arquivo) goto falha;

    for (i = 0; i < tam_cromossomo; i++) {
        fprintf(arquivo, "%d\n", solucao->cromossomo[i]);
        if (ferror(arquivo)) goto falha;
    }
    fclose(arquivo);
    return true;

falha:
    fclose(arquivo);
    return false;
}