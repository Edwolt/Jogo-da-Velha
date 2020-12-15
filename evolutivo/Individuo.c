#include "individuo.h"

//* ===== Criar e Apagar ===== *//

Individuo* individuo_criar() {
    Individuo* individuo = malloc(sizeof(Individuo));

    individuo->sol = solucao_criar();
    if (!individuo->sol) goto falha;
    individuo->elo = 0;
    return individuo;

falha:
    individuo_apagar(&individuo);
    return NULL;
}

Individuo* individuo_criar_random() {
    Individuo* individuo = malloc(sizeof(Individuo));

    individuo->sol = solucao_criar_random();
    if (!individuo->sol) goto falha;
    individuo->elo = 0;
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

static int _jogar(Solucao* a, Solucao* b) {
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

static int jogar(Individuo* a, Individuo* b) {
    if (!a || !b) return 0;
    return _jogar(a->sol, b->sol) - _jogar(b->sol, a->sol);
}

/*

Ra = Ranking de a
Rb = Ranking de b
Sa = Resultado do duelo
Sb = Resultado do duelo
R'a = Novo ranking de a
R'b = Novo ranking de b

Ea = 1 / (1 + 10 ^ (Rb - Ra) / 400)
Eb = 1 / (1 + 10 ^ (Ra - Rb) / 400)

Qa = 10 ^ (Ra / 400)
Qb = 10 ^ (Rb / 400)

Ea = Qa / (Qa + Qb)
Eb = Qb / (Qa + Qb)

R'a = Ra + K(Sa - Ea)
R'b = Rb + K(Sb - Eb)

Ea + Eb = 1
*/

inline static double resultado_esperado(double Ra, double Rb) {
    double Qa = pow(10, Ra / 400);
    double Qb = pow(10, Rb / 400);
    return Qa / (Qa + Qb);
}

void individuo_jogar_elo(Individuo* a, Individuo* b) {
    if (!a || !b) return;

    double Sa = (double)(jogar(a, b) + 2) / 4;
    double Sb = 1 - Sa;

    double Ea = resultado_esperado(a->elo, b->elo);
    double Eb = 1 - Ea;

    int Ka = 24;
    int Kb = 24;

    a->elo += Ka * (Sa - Ea);
    b->elo += Kb * (Sb - Eb);
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

    filho->elo = (pai->elo + mae->elo) / 2;

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

//* ===== Getters e Setters =====*//

double individuo_get_elo(Individuo* individuo) { return (individuo ? individuo->elo : 0); }
void individuo_set_elo(Individuo* individuo, double val) {
    if (individuo) individuo->elo = val;
}

Solucao* individuo_get_solucao(Individuo* individuo) { return (individuo ? individuo->sol : NULL); }

byte individuo_get_gene(Individuo* individuo, int i) { return (individuo ? individuo->sol->cromossomo[i] : -1); }
void individuo_set_gene(Individuo* individuo, int i, int val) {
    if (individuo) individuo->sol->cromossomo[i] = val;
}