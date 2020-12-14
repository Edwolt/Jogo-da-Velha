#include "individuo.h"

struct Individuo {
    Solucao* sol;
    double elo;
    int fitness;
};

//* ===== Criar e Apagar ===== *//

Individuo* individuo_criar() {
    Individuo* individuo = malloc(sizeof(Individuo));

    individuo->sol = solucao_criar();
    if (!individuo->sol) goto falha;

    individuo->elo = 1000;
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
    individuo->elo = 1000;
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

void individuo_jogar_elo(Individuo* a, Individuo* b);

void individuo_jogar_fitness(Individuo* a, Individuo* b) {
    int vitoria = individuo_jogar(a, b);
    a->fitness += vitoria;
    b->fitness -= vitoria;
}

//* ===== Outros metodos ===== *//

Individuo* individuo_crossover(Individuo* pai, Individuo* mae) {
    if (!pai | !mae) return NULL;

    Individuo* filho = NULL;
    int i;

    filho = individuo_criar();
    if (!filho) return NULL;

    for (i = 0; i < tam_cromossomo; i++) {
        individuo_set_gene(filho, i, individuo_get_gene((rand() % 2 ? pai : mae), i));
    }

    filho->elo = (pai->elo + mae->elo) / 2;

    return filho;
}

void individuo_mutacao(Individuo* individuo, double mutacao) {
    int i;

    individuo_set_gene(individuo, rand() % tam_cromossomo, rand() % 9);  // Certificando que houve mutacao
    for (i = 0; i < tam_cromossomo; i++) {
        if (rand() < RAND_MAX * mutacao) {
            individuo_set_gene(individuo, i, rand() % 9);
        }
    }
}

//* ===== Getters e Setters =====*//

double individuo_get_elo(Individuo* individuo) { return (individuo ? individuo->elo : 0); }
void individuo_set_elo(Individuo* individuo, double val) {
    if (individuo) individuo->elo = val;
}

Solucao* individuo_get_solucao(Individuo* individuo) { return (individuo ? individuo->sol : NULL); }

byte individuo_get_gene(Individuo* individuo, int i) { return (individuo ? solucao_get_gene(individuo->sol, i) : -1); }
void individuo_set_gene(Individuo* individuo, int i, int val) {
    if (individuo) solucao_set_gene(individuo->sol, i, val);
}

int individuo_get_fitness(Individuo* individuo) { return (individuo ? individuo->fitness : 0); }
void individuo_set_fitness(Individuo* individuo, int val) {
    if (individuo) individuo->fitness = val;
}
