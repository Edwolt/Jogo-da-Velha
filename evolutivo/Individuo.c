#include "individuo.h"

//* ===== Criar e Apagar ===== *//

Individuo* individuo_criar() {
    Individuo* individuo = malloc(sizeof(Individuo));

    individuo->sol = solucao_criar();
    if (!individuo->sol) goto falha;
    individuo->fitness = 0;
    return individuo;

falha:
    individuo_apagar(&individuo);
    return NULL;
}

Individuo* individuo_criar_random() {
    Individuo* individuo = malloc(sizeof(Individuo));

    individuo->sol = solucao_criar_random();
    if (!individuo->sol) goto falha;
    individuo->fitness = 0;
    return individuo;

falha:
    individuo_apagar(&individuo);
    return NULL;
}

void individuo_apagar(Individuo** individuo) {
    if (!individuo || !*individuo) return;

    solucao_apagar(&(*individuo)->sol);
    free(*individuo);
    *individuo = NULL;
}

//* ===== Jogar ===== *//

static int jogar(Solucao* a, Solucao* b) {
    if (!a || !b) return 0;

    byte jogo[9];
    int i;
    byte vez = 1;
    byte vencedor;
    byte jogada;

    for (i = 0; i < 9; i++) jogo[i] = 0;

    do {
        jogada = solucao_get_jogada((vez == 1 ? a : b), jogo);
        if (jogo[jogada] != 0) {  // jogada_invalida
            while (jogo[jogada] != 0) jogada = rand() % 9;
            solucao_set_jogada((vez == 1 ? a : b), jogo, jogada);
        }

        jogo[jogada] = vez;

        vez = trocar_vez(vez);
        vencedor = calc_vencedor(jogo);
    } while (vencedor == 0);

    if (vencedor == 1) return 1;
    if (vencedor == 2) return -1;
    return 0;
}

int individuo_jogar(Individuo* a, Individuo* b) {
    if (!a || !b) return 0;
    return jogar(a->sol, b->sol) - jogar(b->sol, a->sol);
}

//* ===== Outros metodos ===== *//

Individuo* individuo_crossover(Individuo* pai, Individuo* mae) {
    if (!pai | !mae) return NULL;

    Individuo* filho = NULL;
    int i;

    filho = individuo_criar();
    if (!filho) return NULL;

    for (i = 0; i < tam_cromossomo; i++) {
        filho->sol->cromossomo[i] = (rand() % 2 ? pai : mae)->sol->cromossomo[i];
    }

    return filho;
}

void individuo_mutacao(Individuo* individuo, double mutacao) {
    if (!individuo) return;

    int i;

    individuo->sol->cromossomo[rand() % tam_cromossomo] = rand() % 9;
    for (i = 0; i < tam_cromossomo; i++) {
        if (rand() < RAND_MAX * mutacao) {
            individuo->sol->cromossomo[i] = rand() % 9;
        }
    }
}
