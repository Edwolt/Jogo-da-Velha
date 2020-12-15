#include "populacao.h"

struct Populacao {
    int n;
    Individuo** pop;
};

//* ===== Criar e Apagar ===== *//

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

void populacao_ordena(Populacao* populacao) {
    if (!populacao) return;

    int i, j;
    for (i = populacao->n - 1; i > 0; i--) {
        for (j = 0; j < i; j++) {
            if (individuo_jogar(populacao->pop[j], populacao->pop[j + 1]) < 0) {
                swap(&populacao->pop[j], &populacao->pop[j + 1]);
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
            vitoria = individuo_jogar(populacao->pop[n - 2], populacao->pop[n - 1]);
            if (vitoria < 0 || (vitoria == 0 && rand() % 2)) {
                swap(&populacao->pop[n - 2], &populacao->pop[n - 1]);
            }
        }

        meio = n / 2;
        for (i = 0; i < meio; i++) {
            vitoria = individuo_jogar(populacao->pop[i], populacao->pop[i + meio]);
            if (vitoria < 0 || (vitoria == 0 && rand() % 2)) {
                swap(&populacao->pop[i], &populacao->pop[i + meio]);
            }
        }
    }
}

void populacao_ordena_fitness(Populacao* populacao) {
    if (!populacao) return;

    // Shell Sort
    int i, j, gap = 1;
    int aux_fit;
    Individuo* aux_ind;
    while (gap < populacao->n) gap = 3 * gap + 1;
    while (gap > 0) {  // Para cada gap
        for (i = gap; i < populacao->n; i++) {
            // Insertion Sort de i no grupo i % gap
            aux_fit = individuo_get_fitness(populacao->pop[i]);
            aux_ind = populacao->pop[i];
            j = i;
            for (j = i; j >= gap && aux_fit > individuo_get_fitness(populacao->pop[j - gap]); j -= gap) {
                // Estou segurando o pop[i] e deslocando os valores para frente ate achar onde encaixar pop[i]
                populacao->pop[j] = populacao->pop[j - gap];
            }
            populacao->pop[j] = aux_ind;
        }
        gap /= 3;
    }
}

void populacao_ordena_elo(Populacao* populacao) {
    if (!populacao) return;

    // Shell Sort
    int i, j, gap = 1;
    int aux_fit;
    Individuo* aux_ind;
    while (gap < populacao->n) gap = 3 * gap + 1;
    while (gap > 0) {  // Para cada gap
        for (i = gap; i < populacao->n; i++) {
            // Insertion Sort de i no grupo i % gap
            aux_fit = individuo_get_elo(populacao->pop[i]);
            aux_ind = populacao->pop[i];
            j = i;
            for (j = i; j >= gap && aux_fit > individuo_get_elo(populacao->pop[j - gap]); j -= gap) {
                // Estou segurando o pop[i] e deslocando os valores para frente ate achar onde encaixar pop[i]
                populacao->pop[j] = populacao->pop[j - gap];
            }
            populacao->pop[j] = aux_ind;
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

    for (i = 0; i < populacao->n; i++) {
        a = rand() % populacao->n;
        b = rand() % populacao->n;
        pai = (individuo_jogar(populacao->pop[a], populacao->pop[b]) > 0 ? a : b);

        a = rand() % populacao->n;
        b = rand() % populacao->n;
        mae = (individuo_jogar(populacao->pop[a], populacao->pop[b]) > 0 ? a : b);

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

static void competir_elo(Populacao* populacao, int n) {
    int i;
    int a, b;

    for (i = 0; i < populacao->n * n; i++) {
        a = rand() % populacao->n;
        b = rand() % populacao->n;
        individuo_jogar_elo(populacao->pop[a], populacao->pop[b]);
    }
}

static void ajusta_elo(Populacao* populacao) {
    int i;
    double ajuste = individuo_get_elo(populacao->pop[populacao->n - 1]);
    for (i = 0; i < populacao->n; i++) {
        individuo_set_elo(populacao->pop[i], individuo_get_elo(populacao->pop[i]) - ajuste);
    }
}

bool populacao_elitismo_elo(Populacao* populacao, int n) {
    if (!populacao) return false;

    Individuo** nova_pop = NULL;
    int i;

    nova_pop = malloc(populacao->n * sizeof(Individuo*));
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    competir_elo(populacao, n);
    populacao_ordena_elo(populacao);

    ajusta_elo(populacao);

    for (i = 0; i < populacao->n; i++) {
        nova_pop[i] = individuo_crossover(populacao->pop[0], populacao->pop[i]);
        if (!nova_pop[i]) goto falha;
    }

    for (i = 0; i < populacao->n; i++) individuo_apagar(&populacao->pop[i]);
    free(populacao->pop);
    populacao->pop = nova_pop;

    populacao_ordena_elo(populacao);
    ajusta_elo(populacao);

    return true;

falha:
    if (nova_pop) {
        for (i = 0; i < populacao->n; i++) individuo_apagar(&nova_pop[i]);
        free(nova_pop);
    }
    return false;
}

bool populacao_torneio_elo(Populacao* populacao, int n) {
    if (!populacao) return false;

    Individuo** nova_pop = NULL;
    int i;
    int a, b;
    int pai, mae;

    nova_pop = malloc(populacao->n * sizeof(Individuo*));
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    competir_elo(populacao, n);
    populacao_ordena_elo(populacao);
    ajusta_elo(populacao);

    nova_pop[0] = individuo_crossover(populacao->pop[0], populacao->pop[0]);  // Guarda o melhor de todos
    if (!nova_pop[0]) goto falha;

    for (i = 1; i < populacao->n; i++) {
        a = rand() % populacao->n;
        b = rand() % populacao->n;
        pai = (individuo_get_elo(populacao->pop[a]) > individuo_get_elo(populacao->pop[b]) ? a : b);

        a = rand() % populacao->n;
        b = rand() % populacao->n;
        mae = (individuo_get_elo(populacao->pop[a]) > individuo_get_elo(populacao->pop[b]) ? a : b);

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

bool populacao_chave(Populacao* populacao) {
    if (!populacao) return false;

    Individuo** nova_pop = NULL;
    int i, n, meio;
    int vitoria;

    nova_pop = malloc(populacao->n * sizeof(Individuo*));
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;
    if (!nova_pop) goto falha;

    for (n = populacao->n; n > 1; n /= 2) {
        if (n % 2 == 1) {
            vitoria = individuo_jogar(populacao->pop[0], populacao->pop[n - 1]);
            if (vitoria < 0 || (vitoria == 0 && rand() % 2)) {
                swap(&populacao->pop[0], &populacao->pop[n - 1]);
            }
            nova_pop[n - 1] = individuo_crossover(populacao->pop[0], populacao->pop[n - 1]);
            if (!nova_pop[n - 1]) goto falha;
        }

        meio = n / 2;
        for (i = 0; i < meio; i++) {
            vitoria = individuo_jogar(populacao->pop[i], populacao->pop[i + meio]);
            if (vitoria < 0 || (vitoria == 0 && rand() % 2)) {
                swap(&populacao->pop[i], &populacao->pop[i + meio]);
            }
            nova_pop[i + meio] = individuo_crossover(populacao->pop[i], populacao->pop[i + meio]);
            if (!nova_pop[i + meio]) goto falha;
        }
    }

    nova_pop[0] = individuo_crossover(populacao->pop[0], populacao->pop[0]);
    if (!nova_pop[0]) goto falha;

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
            freq[individuo_get_gene(populacao->pop[j], i)]++;
        }

        maior = 0;
        for (j = 0; j < 9; j++) {
            if (freq[j] > freq[maior]) {
                maior = j;
            }
        }
        individuo_set_gene(sintese, i, maior);
    }

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

        individuo_apagar(&populacao->pop[k - i]);
        populacao->pop[k - i] = novo;
    }
    return true;

falha:
    free(novo);
    return false;
}

//* ===== Outras operacoes ===== *//

void populacao_fitness(Populacao* populacao) {
    if (!populacao) return;

    int i, j;
    for (i = 0; i < populacao->n; i++) individuo_set_fitness(populacao->pop[i], 0);

    for (i = 0; i < populacao->n; i++) {
        for (j = i; j < populacao->n; j++) {
            individuo_jogar_fitness(populacao->pop[i], populacao->pop[j]);
        }
    }
}

//* ===== Getters e Setters =====*//

Individuo* populacao_get_individuo(Populacao* populacao, int i) { return (populacao ? populacao->pop[i] : NULL); }
