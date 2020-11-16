#include "populacao.h"

struct Populacao {
    int n;
    Solucao** pop;
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

//* ===== Fitness ===== *//
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
        while (jogo[jogada] != 0) {  // jogada_invalida
            jogada = rand() % 9;
        }
        solucao_set((vez == 1 ? a : b), jogo, jogada);

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

static void _merge(Populacao* populacao, int* aux_fitness, Solucao** aux_populacao, int inicio, int meio, int fim) {
    int i = inicio;
    int k = inicio;
    int j = meio;

    while (i < meio && j < fim) {
        if (populacao->fitness[i] > populacao->fitness[j]) {
            aux_fitness[k] = populacao->fitness[i];
            aux_populacao[k++] = populacao->pop[i++];
        } else {
            aux_fitness[k] = populacao->fitness[j];
            aux_populacao[k++] = populacao->pop[j++];
        }
    }

    while (i < meio) {
        aux_fitness[k] = populacao->fitness[i];
        aux_populacao[k++] = populacao->pop[i++];
    }
    while (j < fim) {
        aux_fitness[k] = populacao->fitness[j];
        aux_populacao[k++] = populacao->pop[j++];
    }

    for (int i = inicio; i < fim; i++) {
        populacao->fitness[i] = aux_fitness[i];
        populacao->pop[i] = aux_populacao[i];
    }
}

static void _sort(Populacao* populacao, int* aux_fitness, Solucao** aux_populacao, int inicio, int fim) {
    if (inicio + 1 >= fim) {
        aux_fitness[inicio] = populacao->fitness[inicio];
        aux_populacao[inicio] = populacao->pop[inicio];
        return;
    }

    int meio = (inicio + fim) / 2;

    _sort(populacao, aux_fitness, aux_populacao, inicio, meio);
    _sort(populacao, aux_fitness, aux_populacao, meio, fim);

    _merge(populacao, aux_fitness, aux_populacao, inicio, meio, fim);
}

bool sort(Populacao* populacao) {
    int* aux_fitness = NULL;
    Solucao** aux_populacao = NULL;
    int i;

    aux_fitness = (int*)malloc(populacao->n * (sizeof(int)));
    if (!aux_fitness) goto falha;

    aux_populacao = (Solucao**)malloc(populacao->n * sizeof(Solucao*));
    if (!aux_populacao) goto falha;
    for (i = 0; i < populacao->n; i++) aux_populacao[i] = NULL;

    for (int i = 0; i < populacao->n; i++) {
        aux_fitness[i] = populacao->fitness[i];
        aux_populacao[i] = populacao->pop[i];
    }
    _sort(populacao, aux_fitness, aux_populacao, 0, populacao->n);

    free(aux_fitness);
    free(aux_populacao);
    return true;

falha:
    free(aux_fitness);
    free(aux_populacao);
    return false;
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

bool populacao_fitness(Populacao* populacao) {
    if (!populacao) return false;

    int i;
    bool ok;
    int maior = 0, menor = 0;

    for (i = 0; i < populacao->n; i++) populacao->fitness[i] = 0;

    for (i = 0; i < populacao->n; i++) {
        populacao->fitness[i] = solucao_fitness(populacao, populacao->pop[i]);
    }

    for (i = 0; i < populacao->n; i++) {
        if (populacao->fitness[i] > populacao->fitness[maior]) maior = i;
        // if (populacao->fitness[i] < populacao->fitness[menor]) menor = i;
    }

    ok = sort(populacao);
    if (!ok) return false;

    // swap(populacao, 0, maior);
    // swap(populacao, populacao->n - 1, menor);

    // for (i = 0; i < populacao->n; i++) printf("%3d ", populacao->fitness[i]);
    // printf("\n");

    return maior != 0;
}

//* ===== Crossover ===== *//

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

bool populacao_elitismo(Populacao* populacao) {
    if (!populacao) return false;

    int i;
    Solucao* filho = NULL;

    for (i = 1; i < populacao->n; i++) {
        filho = crossover(populacao->pop[0], populacao->pop[i]);
        if (!filho) goto falha;
        solucao_apagar(&populacao->pop[i]);
        populacao->pop[i] = filho;
    }

    return true;

falha:
    solucao_apagar(&filho);
    return false;
}

//* ===== Mutacao ===== *//

void populacao_mutacao(Populacao* populacao, double mutacao) {
    if (!populacao) return;

    int i, j;

    for (i = 1; i < populacao->n; i++) {
        solucao_gene_set(populacao->pop[i], rand() % tam_cromossomo, rand() % 9); // Certificando que houve mutacao
        for (j = 0; j < tam_cromossomo; j++) {
            if (rand() < RAND_MAX * mutacao) {
                solucao_gene_set(populacao->pop[i], j, rand() % 9);
            }
        }
    }
}

//* ===== Predacao ===== *//

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

bool populacao_predacao_randomica(Populacao* populacao, int n) {
    if (!populacao) return false;

    Solucao* novo = NULL;
    int i;
    int k = populacao->n - 2;
    int fitness;

    for (i = 0; i < n; i++) {
        novo = solucao_criar_random();
        if (!novo) goto falha;

        fitness = solucao_fitness(populacao, novo);
        solucao_apagar(&populacao->pop[k - i]);
        populacao->pop[k - i] = novo;
        populacao->fitness[k - i] = fitness;
    }
    return true;

falha:
    free(novo);
    return false;
}

//* ===== Outras operacoes ===== *//

void populacao_salvar(Populacao* populacao, char* melhor, char* todos) {
    solucao_salvar(populacao->pop[0], melhor);
}

int populacao_get_fitness(Populacao* populacao, int i) {
    return populacao->fitness[i];
}