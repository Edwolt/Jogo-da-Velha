#include "solucao.h"

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

Solucao* solucao_carregar(char* path) {
    if (!path) return NULL;

    Solucao* solucao = NULL;
    FILE* arquivo = NULL;
    int i;

    solucao = solucao_criar();
    if (!solucao) goto falha;

    arquivo = fopen(path, "r");
    if (!arquivo) goto falha;

    for (i = 0; i < tam_cromossomo; i++) {
        fscanf(arquivo, "%hhd", &solucao->cromossomo[i]);
        if (ferror(arquivo)) goto falha;
    }
    fclose(arquivo);
    return solucao;

falha:
    solucao_apagar(&solucao);
    fclose(arquivo);
    return NULL;
}

bool solucao_correcao(Solucao* solucao) {
    if (!solucao) return false;

    int i;
    int n;
    byte jogada;
    byte** jogos = jogos_possiveis(&n);
    if (!jogos) return false;

    for (i = 0; i < n; i++) {
        jogada = solucao_get_jogada(solucao, jogos[i]);
        if (jogos[i][jogada] != 0) {
            while (jogos[i][jogada] != 0) jogada = rand() % 9;
            solucao_set_jogada(solucao, jogos[i], jogada);
        }
    }

    for (i = 0; i < M; i++) free(jogos[i]);
    free(jogos);

    return true;
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