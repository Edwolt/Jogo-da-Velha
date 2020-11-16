#include "populacao.h"

struct Populacao {
    int n;
    Individuo** pop;
    int* fitness;
};

//* ===== Criar e apagar populacao ===== *//
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

    populacao->pop = malloc(populacao->n * sizeof(Individuo*));
    if (!populacao->pop) goto falha;
    for (i = 0; i < populacao->n; i++) populacao->pop[i] = NULL;

    for (i = 0; i < populacao->n; i++) {
        populacao->pop[i] = individuo_criar_random();
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
        for (i = 0; i < (*populacao)->n; i++) individuo_apagar(&(*populacao)->pop[i]);
        free((*populacao)->pop);
    }
    free((*populacao)->fitness);
    free(*populacao);
    *populacao = NULL;
}

//* ===== Fitness ===== *//
static int jogar(Individuo* a, Individuo* b) {
    if (!a || !b) return 0;

    byte jogo[9];
    int i;
    byte vez = 1;
    byte vencedor;
    byte jogada;

    for (i = 0; i < 9; i++) jogo[i] = 0;

    do {
        jogada = individuo_get((vez == 1 ? a : b), jogo);
        if (jogo[jogada] != 0) {  // jogada_invalida
            while (jogo[jogada] != 0) jogada = rand() % 9;
            individuo_set((vez == 1 ? a : b), jogo, jogada);
        }

        jogo[jogada] = vez;

        vez = trocar_vez(vez);
        vencedor = calc_vencedor(jogo);
    } while (vencedor == 0);

    if (vencedor == 1) return 1;
    if (vencedor == 2) return -1;
    return 0;
}

static int individuo_fitness(Populacao* populacao, Individuo* individuo) {
    int i;
    int fitness = 0;

    for (i = 0; i < populacao->n; i++) {
        fitness += jogar(individuo, populacao->pop[i]);
        fitness -= jogar(populacao->pop[i], individuo);
    }
    return fitness;
}

static void _merge(Populacao* populacao, int* aux_fit, Individuo** aux_pop, int inicio, int meio, int fim) {
    int i = inicio;
    int k = inicio;
    int j = meio;

    while (i < meio && j < fim) {
        if (populacao->fitness[i] > populacao->fitness[j]) {
            aux_fit[k] = populacao->fitness[i];
            aux_pop[k++] = populacao->pop[i++];
        } else {
            aux_fit[k] = populacao->fitness[j];
            aux_pop[k++] = populacao->pop[j++];
        }
    }

    while (i < meio) {
        aux_fit[k] = populacao->fitness[i];
        aux_pop[k++] = populacao->pop[i++];
    }
    while (j < fim) {
        aux_fit[k] = populacao->fitness[j];
        aux_pop[k++] = populacao->pop[j++];
    }

    for (int i = inicio; i < fim; i++) {
        populacao->fitness[i] = aux_fit[i];
        populacao->pop[i] = aux_pop[i];
    }
}

static void _sort(Populacao* populacao, int* aux_fit, Individuo** aux_pop, int inicio, int fim) {
    if (inicio + 1 >= fim) {
        aux_fit[inicio] = populacao->fitness[inicio];
        aux_pop[inicio] = populacao->pop[inicio];
        return;
    }

    int meio = (inicio + fim) / 2;

    _sort(populacao, aux_fit, aux_pop, inicio, meio);
    _sort(populacao, aux_fit, aux_pop, meio, fim);

    _merge(populacao, aux_fit, aux_pop, inicio, meio, fim);
}

bool sort(Populacao* populacao) {
    int* aux_fit = NULL;
    Individuo** aux_pop = NULL;
    int i;

    aux_fit = (int*)malloc(populacao->n * (sizeof(int)));
    if (!aux_fit) goto falha;

    aux_pop = (Individuo**)malloc(populacao->n * sizeof(Individuo*));
    if (!aux_pop) goto falha;
    for (i = 0; i < populacao->n; i++) aux_pop[i] = NULL;

    for (int i = 0; i < populacao->n; i++) {
        aux_fit[i] = populacao->fitness[i];
        aux_pop[i] = populacao->pop[i];
    }
    _sort(populacao, aux_fit, aux_pop, 0, populacao->n);

    free(aux_fit);
    free(aux_pop);
    return true;

falha:
    free(aux_fit);
    free(aux_pop);
    return false;
}

bool populacao_fitness(Populacao* populacao) {
    if (!populacao) return false;

    int i;
    bool ok;

    for (i = 0; i < populacao->n; i++) {
        populacao->fitness[i] = individuo_fitness(populacao, populacao->pop[i]);
    }

    ok = sort(populacao);
    if (!ok) return false;

    return true;
}

//* ===== Crossover ===== *//

static Individuo* crossover(Individuo* pai, Individuo* mae) {
    if (!pai | !mae) return NULL;

    Individuo* filho = NULL;
    int i;

    filho = individuo_criar();
    if (!filho) return NULL;

    for (i = 0; i < tam_cromossomo; i++) {
        individuo_gene_set(filho, i, individuo_gene_get((rand() % 2 ? pai : mae), i));
    }

    return filho;
}

bool populacao_torneio(Populacao* populacao) {
    if (!populacao) return false;

    Individuo** nova_pop = NULL;
    int i;
    int a, b, pai, mae;

    nova_pop = malloc(populacao->n * sizeof(Individuo*));
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
    for (i = 0; i < populacao->n; i++) individuo_apagar(&populacao->pop[i]);
    free(populacao->pop);
    populacao->pop = nova_pop;

    return true;

falha:
    if (nova_pop) {
        for (i = 0; i < populacao->n; i++) individuo_apagar(&nova_pop[i]);
        free(nova_pop);
    }
    return false;
}

//* ===== Mutacao ===== *//

void populacao_mutacao(Populacao* populacao, double mutacao) {
    if (!populacao) return;

    int i, j;

    for (i = 1; i < populacao->n; i++) {
        individuo_gene_set(populacao->pop[i], rand() % tam_cromossomo, rand() % 9);  // Certificando que houve mutacao
        for (j = 0; j < tam_cromossomo; j++) {
            if (rand() < RAND_MAX * mutacao) {
                individuo_gene_set(populacao->pop[i], j, rand() % 9);
            }
        }
    }
}

//* ===== Predacao ===== *//

bool populacao_predacao_sintese(Populacao* populacao) {
    if (!populacao) return false;

    Individuo* sintese = NULL;
    int i, j;
    int freq[9];
    int maior;
    int k = populacao->n - 1;
    int fitness;

    sintese = individuo_criar();
    if (!sintese) return false;

    for (i = 0; i < tam_cromossomo; i++) {
        for (j = 0; j < 9; j++) freq[j] = 0;

        for (j = 0; j < populacao->n; j++) {
            freq[individuo_gene_get(populacao->pop[j], i)]++;
        }

        maior = 0;
        for (j = 0; j < 9; j++) {
            if (freq[j] > freq[maior]) {
                maior = j;
            }
        }
        individuo_gene_set(sintese, i, maior);
    }

    fitness = individuo_fitness(populacao, sintese);
    individuo_apagar(&populacao->pop[k]);
    populacao->pop[k] = sintese;
    populacao->fitness[k] = fitness;

    return true;
}

bool populacao_predacao_randomica(Populacao* populacao, int n) {
    if (!populacao) return false;

    Individuo* novo = NULL;
    int i;
    int k = populacao->n - 2;
    int fitness;

    for (i = 0; i < n; i++) {
        novo = individuo_criar_random();
        if (!novo) goto falha;

        fitness = individuo_fitness(populacao, novo);
        individuo_apagar(&populacao->pop[k - i]);
        populacao->pop[k - i] = novo;
        populacao->fitness[k - i] = fitness;
    }
    return true;

falha:
    free(novo);
    return false;
}

//* ===== Outras operacoes ===== *//

void populacao_salvar_melhor(Populacao* populacao, char* path) {
    individuo_salvar(populacao->pop[0], path);
}

int populacao_get_fitness(Populacao* populacao, int i) {
    return populacao->fitness[i];
}