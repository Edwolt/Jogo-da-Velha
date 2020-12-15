#include "solucao.h"

struct Solucao {
    byte* cromossomo;
};

//* ===== Criar e Apagar =====*//

Solucao* solucao_criar() {
    Solucao* solucao = malloc(sizeof(Solucao));
    if (!solucao) goto falha;
    solucao->cromossomo = NULL;

    int i;
    solucao->cromossomo = malloc(tam_cromossomo * sizeof(byte));
    if (!solucao->cromossomo) goto falha;

    for (i = 0; i < tam_cromossomo; i++) solucao->cromossomo[i] = -1;
    return solucao;

falha:
    solucao_apagar(&solucao);
    return NULL;
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

//* ===== Outros metodos ===== *//

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

//* ===== Getters e Setters =====*//

byte solucao_get_jogada(Solucao* solucao, byte* jogo) {
    if (!solucao) return -1;

    int sim = calc_sim(jogo);
    int minimo = calc_jogo(jogo);
    int gene = mapa[minimo];
    byte jogada = solucao->cromossomo[gene];
    return simetrias_reversa[sim][jogada];
}

void solucao_set_jogada(Solucao* solucao, byte* jogo, byte jogada) {
    if (!solucao) return;

    int sim = calc_sim(jogo);
    int minimo = calc_jogo(jogo);
    int gene = mapa[minimo];

    jogada = simetrias[sim][jogada];
    solucao->cromossomo[gene] = jogada;
}

byte solucao_get_gene(Solucao* solucao, int i) {
    if (!solucao || i < 0 || i > tam_cromossomo) return -1;
    return solucao->cromossomo[i];
}

void solucao_set_gene(Solucao* solucao, int i, byte val) {
    if (!solucao) return;
    solucao->cromossomo[i] = val;
}