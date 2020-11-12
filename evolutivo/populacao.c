#include "populacao.h"

struct Populacao {
    int n;
    Solucao** pop;
    int* fitness;
};

Populacao* populacao_criar(int n) {
    Populacao* populacao = NULL;
    int i;

    populacao = malloc(sizeof(Populacao));
    if (!populacao) goto falha;
    populacao->pop = NULL;
    populacao->fitness = NULL;
    populacao->n = n;

    populacao->fitness = malloc(n * sizeof(int));
    if (!populacao->fitness) goto falha;
    for (i = 0; i < populacao->n; i++) populacao->fitness[i] = 0;

    populacao->pop = malloc(n * sizeof(Solucao*));
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
    free((*populacao)->fitness);
    free(*populacao);
    *populacao = NULL;
}

static int jogar(Solucao* a, Solucao* b) {
    int jogo[9];
    int i;
    int vez = 1;
    int vencedor;
    int jogada;

    for (i = 0; i < 9; i++) jogo[i] = 0;

    do {
        jogada = solucao_get((vez == 1 ? a : b), jogo);
        if (jogo[jogada] != 0) {  // jogada_invalida
            if (vez == 1) {
                return -1;
            } else {
                return 1;
            }
        }
        jogada = protege_jogada_invalida(jogo, jogada);
        jogo[jogada] = vez;

        vez = trocar_vez(vez);
    } while (vencedor == 1 || vencedor == 2);

    vencedor = calc_vencedor(jogo);
    if (vencedor == 1) return 1;
    if (vencedor == 2) return -1;
    return 0;
}

static int encontrar_maior(int* fitness, int n) {
    int i;
    int maior = 0;
    for (i = 1; i < n; i++) {
        if (fitness[i] > fitness[maior]) maior = i;
    }
    return maior;
}

inline static void swapint(int* a, int* b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

inline static void swapsol(Solucao** a, Solucao** b) {
    Solucao* aux = *a;
    *a = *b;
    *b = aux;
}

static void swap(Populacao* populacao, int i, int j) {
    swapsol(&populacao->pop[i], &populacao->pop[j]);
    swapint(&populacao->fitness[i], &populacao->fitness[j]);
}

void populacao_fitness(Populacao* populacao) {
    int i, j;
    int maior = 0;
    int menor = 0;

    for (i = 0; i < populacao->n; i++) {
        populacao->fitness[i] = 0;
        for (j = 0; j < populacao->n; j++) {
            populacao->fitness[i] += jogar(populacao->pop[i], populacao->pop[j]);
        }
    }

    for (i = 0; i < populacao->n; i++) {
        if (populacao->fitness[i] > populacao->fitness[maior]) maior = i;
        if (populacao->fitness[i] < populacao->fitness[menor]) menor = i;
    }

    swap(populacao, 0, maior);
    swap(populacao, populacao->n - 1, menor);
}

static Solucao* crossover(Solucao* pai, Solucao* mae) {
    Solucao* filho = NULL;
    int i;

    filho = solucao_criar();
    if (!filho) return NULL;

    for (i = 0; i < tam_cromossomo; i++) {
        solucao_gene_set(filho, i, solucao_gene_get((rand() % 2 ? pai : mae), i));
    }

    return filho;
}

bool populacao_torneio(Populacao* populacao) {
    if (!populacao) return false;
    Solucao** nova_pop = NULL;
    int i;

    nova_pop = vetor_solucao(populacao->n);
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    nova_pop[0] = populacao->pop[0];  // Guarda o melhor de todos
    int i;
    int a, b, pai, mae;

    for (i = 1; i < populacao->n; i++) {
        a = rand() % populacao->n;
        b = rand() % populacao->n;
        pai = (populacao->fitness[a] > populacao->fitness[b] ? a : b);

        a = rand() % populacao->n;
        b = rand() % populacao->n;
        mae = (populacao->fitness[a] > populacao->fitness[b] ? a : b);
        nova_pop[i] = crossover(populacao->pop[pai], populacao->pop[mae]);
        if (!nova_pop[i]) goto falha;
    }

    populacao->pop[0] = NULL;
    for (i = 0; i < populacao->n; i++) solucao_apagar(populacao->pop[i]);
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

void populacao_mutacao(Populacao* populacao, int mutacao) {
    int i, j;
    // Nao tenho certeza se todos vao mutar

    for (i = 1; i < populacao->n; i++) {
        for (j = 0; j < tam_cromossomo; j++) {
            if (rand() < mutacao) {
                solucao_gene_set(populacao->pop[i], j, rand() % 9);
            }
        }
    }
}