#include "populacao.h"

struct Populacao {
    int n;
    Individuo** pop;
};

//* ===== Criar e apagar populacao ===== *//
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

//* ===== Fitness ===== *//
static int _jogar(Individuo* a, Individuo* b) {
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

inline static int jogar(Individuo* a, Individuo* b) { return _jogar(a, b) - _jogar(b, a); }

inline static void swap(Individuo** a, Individuo** b) {
    Individuo* aux = *a;
    *a = *b;
    *b = aux;
}

void populacao_ordena(Populacao* populacao) {
    int i, j;
    for (i = 0; i < populacao->n; i++) {
        for (j = i + 1; j < populacao->n; j++) {
            if (jogar(populacao->pop[i], populacao->pop[i + 1]) < 0) {
                swap(&populacao->pop[i], &populacao->pop[i + 1]);
            }
        }
    }
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

bool populacao_elitismo(Populacao* populacao) {
    if (!populacao) return false;

    Individuo** nova_pop = NULL;
    int i;

    nova_pop = malloc(populacao->n * sizeof(Individuo*));
    if (!nova_pop) goto falha;
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;

    nova_pop[0] = crossover(populacao->pop[0], populacao->pop[0]);  // Guarda o melhor de todos
    if (!nova_pop[0]) goto falha;

    for (i = 1; i < populacao->n; i++) {
        nova_pop[i] = crossover(populacao->pop[0], populacao->pop[1]);
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

    nova_pop[0] = crossover(populacao->pop[0], populacao->pop[0]);  // Guarda o melhor de todos
    if (!nova_pop[0]) goto falha;

    for (i = 1; i < populacao->n; i++) {
        a = rand() % populacao->n;
        b = rand() % populacao->n;
        pai = (jogar(populacao->pop[a], populacao->pop[b]) > 0 ? a : b);

        a = rand() % populacao->n;
        b = rand() % populacao->n;
        mae = (jogar(populacao->pop[a], populacao->pop[b]) > 0 ? a : b);

        nova_pop[i] = crossover(populacao->pop[pai], populacao->pop[mae]);
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

static bool _populacao_chave(Populacao* populacao, Individuo** nova_pop, int n) {
    if (!populacao) return false;

    if (n == 1) {
        nova_pop[0] = crossover(populacao->pop[0], populacao->pop[0]);
        if (!nova_pop[0]) return false;
        return true;
    }

    int meio = n / 2;
    int i;
    int vitoria;

    for (i = 0; i < meio; i++) {
        vitoria = jogar(populacao->pop[i], populacao->pop[i + meio]);
        if (vitoria > 0) {
            // Nao faz nada
        } else if (vitoria < 0) {
            swap(&populacao->pop[i], &populacao->pop[i + meio]);
        } else if (rand() % 2) {
            swap(&populacao->pop[i], &populacao->pop[i + meio]);
        }

        nova_pop[i + meio] = crossover(populacao->pop[i], populacao->pop[i + meio]);
        if (!nova_pop[i + meio]) return false;
    }

    if (n % 2 == 1) {
        vitoria = jogar(populacao->pop[0], populacao->pop[n - 1]);
        if (vitoria > 0) {
            // Nao faz nada
        } else if (vitoria < 0) {
            swap(&populacao->pop[0], &populacao->pop[n - 1]);
        } else if (rand() % 2) {
            swap(&populacao->pop[0], &populacao->pop[n - 1]);
        }

        nova_pop[n - 1] = crossover(populacao->pop[0], populacao->pop[n - 1]);
        if (!nova_pop[n - 1]) return false;
    }

    return _populacao_chave(populacao, nova_pop, meio);
}

bool populacao_chave(Populacao* populacao) {
    if (!populacao) return false;

    Individuo** nova_pop = NULL;
    int i;
    bool ok;

    nova_pop = malloc(populacao->n * sizeof(Individuo*));
    for (i = 0; i < populacao->n; i++) nova_pop[i] = NULL;
    if (!nova_pop) goto falha;

    ok = _populacao_chave(populacao, nova_pop, populacao->n);
    if (!ok) goto falha;

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

void populacao_salvar_melhor(Populacao* populacao, char* path) {
    individuo_salvar(populacao->pop[0], path);
}