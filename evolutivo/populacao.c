#include "populacao.h"

struct Populacao {
    int n;
    Solucao** pop;
};

//* ===== Operacoes Basicas ===== *//
Populacao* populacao_criar(int n) {
    Populacao* populacao = NULL;
    int i;

    populacao = malloc(sizeof(Populacao));
    if (!populacao) goto falha;
    populacao->pop = NULL;
    populacao->n = n;

    populacao->pop = malloc(populacao->n * sizeof(Solucao*));
    if (!populacao->pop) goto falha;
    for (i = 0; i < populacao->n; i++) populacao->pop[i] = NULL;

    for (i = 0; i < populacao->n; i++) {
        populacao->pop[i] = solucao_criar_random();
        if (!populacao->pop[i]) goto falha;
    }

    return populacao;

falha:
    populacao_apagar(&populacao);
    return NULL;
}

void populacao_apagar(Populacao** populacao) {
    if (!populacao || !*populacao) return;

    int i;
    if ((*populacao)->pop) {
        for (i = 0; i < (*populacao)->n; i++) solucao_apagar(&(*populacao)->pop[i]);
        free((*populacao)->pop);
    }
    free(*populacao);
    *populacao = NULL;
}

//* ===== Ordena ===== *//
static int _jogar(Solucao* a, Solucao* b) {
    if (!a || !b) return 0;

    byte jogo[9];
    int i;
    byte vez = 1;
    byte vencedor;
    byte jogada;

    for (i = 0; i < 9; i++) jogo[i] = 0;

    do {
        jogada = solucao_get((vez == 1 ? a : b), jogo);
        if (jogo[jogada] != 0) {  // jogada_invalida
            while (jogo[jogada] != 0) jogada = rand() % 9;
            solucao_set((vez == 1 ? a : b), jogo, jogada);
        }

        jogo[jogada] = vez;

        vez = trocar_vez(vez);
        vencedor = calc_vencedor(jogo);
    } while (vencedor == 0);

    if (vencedor == 1) return 1;
    if (vencedor == 2) return -1;
    return 0;
}

inline static int jogar(Solucao* a, Solucao* b) { return _jogar(a, b) - _jogar(b, a); }

inline static void swap(int* a, int* b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

inline static void swapS(Solucao** a, Solucao** b) {
    Solucao* aux = *a;
    *a = *b;
    *b = aux;
}

void populacao_ordena(Populacao* populacao) {
    if (!populacao) return;

    int i, j;
    for (i = populacao->n - 1; i > 0; i--) {
        for (j = 0; j < i; j++) {
            if (jogar(populacao->pop[j], populacao->pop[j + 1]) < 0) {
                swapS(&populacao->pop[j], &populacao->pop[j + 1]);
            }
        }
    }
}

void populacao_ordena_chave(Populacao* populacao) {
    if (!populacao) return;

    int n, i, meio;
    int vitoria;

    for (n = populacao->n; n > 1; n /= 2) {
        if (n % 2 == 1) {
            vitoria = jogar(populacao->pop[n - 2], populacao->pop[n - 1]);
            if (vitoria < 0 || (vitoria == 0 && rand() % 2)) {
                swapS(&populacao->pop[n - 2], &populacao->pop[n - 1]);
            }
        }

        meio = n / 2;
        for (i = 0; i < meio; i++) {
            vitoria = jogar(populacao->pop[i], populacao->pop[i + meio]);
            if (vitoria < 0 || (vitoria == 0 && rand() % 2)) {
                swapS(&populacao->pop[i], &populacao->pop[i + meio]);
            }
        }
    }
}

void populacao_ordena_fitness(Populacao* populacao) {
    if (!populacao) return;

    int* fitness = populacao_fitness(populacao);
    if (!fitness) return;

    int i, j;

    for (i = populacao->n - 1; i > 0; i--) {
        for (j = 0; j < i; j++) {
            if (fitness[j] < fitness[j + 1]) {
                swap(&fitness[j], &fitness[j + 1]);
                swapS(&populacao->pop[j], &populacao->pop[j + 1]);
            }
        }
    }
    free(fitness);
}

//* ===== Reproducao ===== *//

static Solucao* crossover(Solucao* pai, Solucao* mae) {
    if (!pai | !mae) return NULL;

    Solucao* filho = NULL;
    int i;

    filho = solucao_criar();
    if (!filho) return NULL;

    for (i = 0; i < tam_cromossomo; i++) {
        solucao_gene_set(filho, i, solucao_gene_get((rand() % 2 ? pai : mae), i));
    }

    return filho;
}

bool populacao_elitismo(Populacao* populacao) {
    if (!populacao) return false;

    Solucao** nova_pop = NULL;
    int i;

    nova_pop = malloc(populacao->n * sizeof(Solucao*));
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    nova_pop[0] = crossover(populacao->pop[0], populacao->pop[0]);  // Guarda o melhor de todos
    if (!nova_pop[0]) goto falha;

    for (i = 1; i < populacao->n; i++) {
        nova_pop[i] = crossover(populacao->pop[0], populacao->pop[1]);
        if (!nova_pop[i]) goto falha;
    }

    for (i = 0; i < populacao->n; i++) solucao_apagar(&populacao->pop[i]);
    free(populacao->pop);
    populacao->pop = nova_pop;

    return true;

falha:
    if (nova_pop) {
        for (i = 0; i < populacao->n; i++) solucao_apagar(&nova_pop[i]);
        free(nova_pop);
    }
    return false;
}

bool populacao_torneio(Populacao* populacao) {
    if (!populacao) return false;

    Solucao** nova_pop = NULL;
    int i;
    int a, b;
    int pai, mae;

    nova_pop = malloc(populacao->n * sizeof(Solucao*));
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    nova_pop[0] = crossover(populacao->pop[0], populacao->pop[0]);  // Guarda o melhor de todos
    if (!nova_pop[0]) goto falha;

    for (i = 0; i < populacao->n; i++) {
        a = rand() % populacao->n;
        b = rand() % populacao->n;
        pai = (jogar(populacao->pop[a], populacao->pop[b]) > 0 ? a : b);

        a = rand() % populacao->n;
        b = rand() % populacao->n;
        mae = (jogar(populacao->pop[a], populacao->pop[b]) > 0 ? a : b);

        nova_pop[i] = crossover(populacao->pop[pai], populacao->pop[mae]);
        if (!nova_pop[i]) goto falha;
    }

    for (i = 0; i < populacao->n; i++) solucao_apagar(&populacao->pop[i]);
    free(populacao->pop);
    populacao->pop = nova_pop;

    return true;

falha:
    if (nova_pop) {
        for (i = 0; i < populacao->n; i++) solucao_apagar(&nova_pop[i]);
        free(nova_pop);
    }
    return false;
}

bool populacao_chave(Populacao* populacao) {
    if (!populacao) return false;

    Solucao** nova_pop = NULL;
    int i, n, meio;
    int vitoria;

    nova_pop = malloc(populacao->n * sizeof(Solucao*));
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;
    if (!nova_pop) goto falha;

    for (n = populacao->n; n > 1; n /= 2) {
        if (n % 2 == 1) {
            vitoria = jogar(populacao->pop[0], populacao->pop[n - 1]);
            if (vitoria < 0 || (vitoria == 0 && rand() % 2)) {
                swapS(&populacao->pop[0], &populacao->pop[n - 1]);
            }
            nova_pop[n - 1] = crossover(populacao->pop[0], populacao->pop[n - 1]);
            if (!nova_pop[n - 1]) goto falha;
        }

        meio = n / 2;
        for (i = 0; i < meio; i++) {
            vitoria = jogar(populacao->pop[i], populacao->pop[i + meio]);
            if (vitoria < 0 || (vitoria == 0 && rand() % 2)) {
                swapS(&populacao->pop[i], &populacao->pop[i + meio]);
            }
            nova_pop[i + meio] = crossover(populacao->pop[i], populacao->pop[i + meio]);
            if (!nova_pop[i + meio]) goto falha;
        }
    }

    nova_pop[0] = crossover(populacao->pop[0], populacao->pop[0]);
    if (!nova_pop[0]) goto falha;

    for (i = 0; i < populacao->n; i++) solucao_apagar(&populacao->pop[i]);
    free(populacao->pop);

    populacao->pop = nova_pop;

    return true;

falha:
    if (nova_pop) {
        for (i = 0; i < populacao->n; i++) solucao_apagar(&nova_pop[i]);
        free(nova_pop);
    }
    return false;
}

//* ===== Mutacao ===== *//

void populacao_mutacao(Populacao* populacao, double mutacao) {
    if (!populacao) return;

    int i, j;

    for (i = 1; i < populacao->n; i++) {
        solucao_gene_set(populacao->pop[i], rand() % tam_cromossomo, rand() % 9);  // Certificando que houve mutacao
        for (j = 0; j < tam_cromossomo; j++) {
            if (rand() < RAND_MAX * mutacao) {
                solucao_gene_set(populacao->pop[i], j, rand() % 9);
            }
        }
    }
}

//* ===== Predacao ===== *//

bool populacao_predacao_sintese(Populacao* populacao, int n) {
    if (!populacao) return false;

    Solucao* sintese = NULL;
    int i, j;
    int freq[9];
    int maior;
    int k = populacao->n - (n + 1);

    sintese = solucao_criar();
    if (!sintese) return false;

    for (i = 0; i < tam_cromossomo; i++) {
        for (j = 0; j < 9; j++) freq[j] = 0;

        for (j = 0; j < populacao->n; j++) {
            freq[solucao_gene_get(populacao->pop[j], i)]++;
        }

        maior = 0;
        for (j = 0; j < 9; j++) {
            if (freq[j] > freq[maior]) {
                maior = j;
            }
        }
        solucao_gene_set(sintese, i, maior);
    }

    solucao_apagar(&populacao->pop[k]);
    populacao->pop[k] = sintese;

    return true;
}

bool populacao_predacao_randomica(Populacao* populacao, int n) {
    if (!populacao) return false;

    Solucao* novo = NULL;
    int i;
    int k = populacao->n - 1;

    for (i = 0; i < n; i++) {
        novo = solucao_criar_random();
        if (!novo) goto falha;

        solucao_apagar(&populacao->pop[k - i]);
        populacao->pop[k - i] = novo;
    }
    return true;

falha:
    free(novo);
    return false;
}

//* ===== Outras operacoes ===== *//

void populacao_salvar_melhor(Populacao* populacao, char* path) { solucao_salvar(populacao->pop[0], path); }

Solucao* populacao_get_solucao(Populacao* populacao, int i) { return (populacao ? populacao->pop[i] : NULL); }

int* populacao_fitness(Populacao* populacao) {
    if (!populacao) return NULL;

    int* fitness = malloc(populacao->n * sizeof(int));
    if (!fitness) return NULL;

    int i, j;
    for (i = 0; i < populacao->n; i++) {
        fitness[i] = 0;
        for (j = 0; j < populacao->n; j++) {
            fitness[i] += jogar(populacao->pop[i], populacao->pop[j]);
        }
    }

    return fitness;
}