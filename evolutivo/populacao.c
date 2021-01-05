#include "populacao.h"

//* ===== Criar e Apagar ===== *//

Individuo** oponentes = NULL;
bool populacao_oponentes(int n) {
    int i;
    char path[100] = "";
    oponentes = malloc(n * sizeof(Individuo*));
    if (!oponentes) goto falha;
    for (i = 0; i < n; i++) oponentes[i] = NULL;
    for (i = 0; i < n; i++) {
        oponentes[i] = individuo_criar();
        if (!oponentes[i]) goto falha;
    }

    int ind = 399;
    for (i = 0; i < n; i++) {
        path[0] = '\0';
        sprintf(path, "%d.txt", ind - i);
        oponentes[i]->sol = solucao_carregar(path);
    }
    return true;

falha:
    if (oponentes) {
        for (i = 0; i < n; i++) individuo_apagar(&oponentes[i]);
        free(oponentes);
    }
    return false;
}

Populacao* populacao_criar(int n) {
    Populacao* populacao = NULL;
    int i;

    populacao = malloc(sizeof(Populacao));
    if (!populacao) goto falha;
    populacao->pop = NULL;
    populacao->n = n;

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
    free(*populacao);
    *populacao = NULL;
}

//* ===== Ordena ===== *//

inline static void swap(Individuo** a, Individuo** b) {
    Individuo* aux = *a;
    *a = *b;
    *b = aux;
}

inline static int individuo_fitness(Populacao* populacao, Individuo* individuo) {
    int i;
    register int fitness = 0;

    for (i = 0; i < populacao->n; i++) {
        fitness += individuo_jogar(individuo, populacao->pop[i]);
        // fitness += individuo_jogar(individuo, oponentes[i]);
    }

    return fitness;
}

void populacao_fitness(Populacao* populacao) {
    if (!populacao) return;

    int i, j, gap = 1;
    Individuo* aux;

    // Fitness da populacao
    for (i = 0; i < populacao->n; i++) {
        populacao->pop[i]->fitness = individuo_fitness(populacao, populacao->pop[i]);
    }

    // Ordena com Shell Sort
    while (gap < populacao->n) gap = 3 * gap + 1;
    while (gap > 0) {  // Para cada gap
        for (i = gap; i < populacao->n; i++) {
            // Insertion Sort de i no grupo i % gap
            aux = populacao->pop[i];
            j = i;
            for (j = i; j >= gap && aux->fitness > populacao->pop[j - gap]->fitness; j -= gap) {
                // Estou segurando o pop[i] e deslocando os valores para frente ate achar onde encaixar pop[i]
                populacao->pop[j] = populacao->pop[j - gap];
            }
            populacao->pop[j] = aux;
        }
        gap /= 3;
    }
}

//* ===== Reproducao ===== *//

bool populacao_elitismo(Populacao* populacao) {
    if (!populacao) return false;

    Individuo** nova_pop = NULL;
    int i;

    nova_pop = malloc(populacao->n * sizeof(Individuo*));
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    for (i = 0; i < populacao->n; i++) {
        nova_pop[i] = individuo_crossover(populacao->pop[0], populacao->pop[i]);
        if (!nova_pop[i]) goto falha;
    }

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

bool populacao_torneio(Populacao* populacao) {
    if (!populacao) return false;

    Individuo** nova_pop = NULL;
    int i;
    int a, b;
    int pai, mae;

    nova_pop = malloc(populacao->n * sizeof(Individuo*));
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    nova_pop[0] = individuo_crossover(populacao->pop[0], populacao->pop[0]);  // Guarda o melhor de todos
    if (!nova_pop[0]) goto falha;

    for (i = 1; i < populacao->n; i++) {
        a = rand() % populacao->n;
        b = rand() % populacao->n;
        pai = (populacao->pop[a]->fitness > populacao->pop[b]->fitness ? a : b);

        a = rand() % populacao->n;
        b = rand() % populacao->n;
        mae = (populacao->pop[a]->fitness > populacao->pop[b]->fitness ? a : b);

        nova_pop[i] = individuo_crossover(populacao->pop[pai], populacao->pop[mae]);
        if (!nova_pop[i]) goto falha;
    }

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

    int i;
    for (i = 1; i < populacao->n; i++) {
        individuo_mutacao(populacao->pop[i], mutacao);
    }
}

//* ===== Predacao ===== *//

bool populacao_predacao_sintese(Populacao* populacao, int n) {
    if (!populacao) return false;

    Individuo* sintese = NULL;
    int i, j;
    int freq[9];
    int maior;
    int k = populacao->n - (n + 1);

    sintese = individuo_criar();
    if (!sintese) return false;

    for (i = 0; i < tam_cromossomo; i++) {
        for (j = 0; j < 9; j++) freq[j] = 0;

        for (j = 0; j < populacao->n; j++) {
            freq[populacao->pop[j]->sol->cromossomo[i]]++;
        }

        maior = 0;
        for (j = 0; j < 9; j++) {
            if (freq[j] > freq[maior]) {
                maior = j;
            }
        }
        sintese->sol->cromossomo[i] = maior;
    }

    individuo_fitness(populacao, sintese);
    individuo_apagar(&populacao->pop[k]);
    populacao->pop[k] = sintese;

    return true;
}

bool populacao_predacao_randomica(Populacao* populacao, int n) {
    if (!populacao) return false;

    Individuo* novo = NULL;
    int i;
    int k = populacao->n - 1;

    for (i = 0; i < n; i++) {
        novo = individuo_criar_random();
        if (!novo) goto falha;

        individuo_fitness(populacao, novo);
        individuo_apagar(&populacao->pop[k - i]);
        populacao->pop[k - i] = novo;
    }
    return true;

falha:
    free(novo);
    return false;
}

//* ===== Getters e Setters =====*//

Individuo* populacao_get_individuo(Populacao* populacao, int i) { return (populacao ? populacao->pop[i] : NULL); }
