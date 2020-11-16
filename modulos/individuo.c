#include "individuo.h"

struct Individuo {
    byte* cromossomo;
};

Individuo* individuo_criar() {
    Individuo* individuo = malloc(sizeof(Individuo));
    if (!individuo) goto falha;
    individuo->cromossomo = NULL;

    int i;
    individuo->cromossomo = malloc(tam_cromossomo * sizeof(byte));
    if (!individuo->cromossomo) goto falha;

    for (i = 0; i < tam_cromossomo; i++) individuo->cromossomo[i] = -1;
    return individuo;

falha:
    if (individuo) {
        free(individuo->cromossomo);
        free(individuo);
    }
    return NULL;
}

Individuo* individuo_criar_random() {
    Individuo* individuo = individuo_criar();
    if (!individuo) return NULL;

    int i;
    for (i = 0; i < tam_cromossomo; i++) individuo->cromossomo[i] = rand() % 9;
    return individuo;
}

void individuo_apagar(Individuo** individuo) {
    if (!individuo || !*individuo) return;

    free((*individuo)->cromossomo);
    free(*individuo);
    *individuo = NULL;
}

byte individuo_get(Individuo* individuo, byte* jogo) {
    if (!individuo) return -1;

    int sim = calc_sim(jogo);
    int minimo = calc_val(jogo, sim);
    int gene = mapa[minimo];
    byte jogada = individuo->cromossomo[gene];
    return simetrias_reversa[sim][jogada];
}

void individuo_set(Individuo* individuo, byte* jogo, byte jogada) {
    if (!individuo) return;

    int sim = calc_sim(jogo);
    int minimo = calc_val(jogo, sim);
    int gene = mapa[minimo];

    jogada = simetrias[sim][jogada];
    individuo->cromossomo[gene] = jogada;
}

byte individuo_gene_get(Individuo* individuo, int i) {
    if (!individuo || i < 0 || i > tam_cromossomo) return -1;
    return individuo->cromossomo[i];
}

void individuo_gene_set(Individuo* individuo, int i, byte val) {
    if (!individuo) return;
    individuo->cromossomo[i] = val;
}

bool individuo_salvar(Individuo* individuo, char* path) {
    if (!individuo) return false;

    FILE* arquivo = NULL;
    int i;

    arquivo = fopen(path, "w");
    if (!arquivo) goto falha;

    for (i = 0; i < tam_cromossomo; i++) {
        fprintf(arquivo, "%d\n", individuo->cromossomo[i]);
        if (ferror(arquivo)) goto falha;
    }
    fclose(arquivo);
    return true;

falha:
    fclose(arquivo);
    return false;
}