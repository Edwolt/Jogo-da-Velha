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

    populacao->fitness = malloc(populacao->n * sizeof(int));
    if (!populacao->fitness) goto falha;
    for (i = 0; i < populacao->n; i++) populacao->fitness[i] = 0;

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
    free((*populacao)->fitness);
    free(*populacao);
    *populacao = NULL;
}

static int jogar(Solucao* a, Solucao* b) {
    if (!a || !b) return 0;

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

        jogo[jogada] = vez;

        vez = trocar_vez(vez);
        vencedor = calc_vencedor(jogo);
    } while (vencedor == 0);

    if (vencedor == 1) return 1;
    if (vencedor == 2) return -1;
    return 0;
}

static int solucao_fitness(Populacao* populacao, Solucao* solucao) {
    int i;
    int fitness = 0;

    for (i = 0; i < populacao->n; i++) {
        fitness += jogar(solucao, populacao->pop[i]);
        fitness -= jogar(populacao->pop[i], solucao);
    }
    return fitness;
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

inline static void swap(Populacao* populacao, int i, int j) {
    swapsol(&populacao->pop[i], &populacao->pop[j]);
    swapint(&populacao->fitness[i], &populacao->fitness[j]);
}

void populacao_fitness(Populacao* populacao) {
    if (!populacao) return;

    int i, j;
    int ij, ji;
    int maior = 0, menor = 0;

    for (i = 0; i < populacao->n; i++) populacao->fitness[i] = 0;

    for (i = 0; i < populacao->n; i++) {
        for (j = i; j < populacao->n; j++) {
            ij = jogar(populacao->pop[i], populacao->pop[j]);
            ji = jogar(populacao->pop[j], populacao->pop[i]);
            populacao->fitness[i] += ij - ji;
            populacao->fitness[j] += ji - ij;
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

bool populacao_torneio(Populacao* populacao) {
    if (!populacao) return false;

    Solucao** nova_pop = NULL;
    int i;
    int a, b, pai, mae;

    nova_pop = malloc(populacao->n * sizeof(Solucao*));
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    nova_pop[0] = populacao->pop[0];  // Guarda o melhor de todos

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

void populacao_mutacao(Populacao* populacao, int mutacao) {
    if (!populacao) return;

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

bool populacao_predacao_sintese(Populacao* populacao) {
    if (!populacao) return false;

    Solucao* sintese = NULL;
    int i, j;
    int freq[9];
    int maior;
    int k = populacao->n - 1;
    int fitness;

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

    fitness = solucao_fitness(populacao, sintese);
    solucao_apagar(&populacao->pop[k]);
    populacao->pop[k] = sintese;
    populacao->fitness[k] = fitness;

    return true;
}

bool populacao_predacao_randomica(Populacao* populacao) {
    if (!populacao) return false;

    Solucao* novo = NULL;
    int k = populacao->n - 1;
    int fitness;

    novo = solucao_criar_random();
    if (!novo) return false;

    fitness = solucao_fitness(populacao, novo);
    solucao_apagar(&populacao->pop[k]);
    populacao->pop[k] = novo;
    populacao->fitness[k] = fitness;
    return true;
}

void populacao_salvar(Populacao* populacao, char* melhor, char* todos) {
    solucao_salvar(populacao->pop[0], melhor);
}

int populacao_get_fitness(Populacao* populacao, int i) {
    return populacao->fitness[i];
}