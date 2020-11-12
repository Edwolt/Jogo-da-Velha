#include <stdlib.h>  // malloc, rand
#include <time.h>
#include "kbhit.h"
#include "solucao.h"

extern int tam_cromossomo;
const int N = 500;

inline static randbool() { return rand() % 2; }
inline static void swap(Solucao** a, Solucao** b) {
    Solucao* aux = *a;
    *a = *b;
    *b = aux;
}

static Solucao** _populacao_criar(int n, Solucao* criar()) {
    Solucao** populacao = NULL;
    int i;

    populacao = malloc(n * sizeof(Solucao*));
    if (!populacao) goto falha;
    for (i = 0; i < n; i++) populacao[i] = NULL;

    for (i = 0; i < n; i++) {
        populacao[i] = criar();
        if (!populacao[i]) goto falha;
    }

    return populacao;

falha:
    populacao_apagar(&populacao, n);
    return NULL;
}

inline static Solucao** populacao_criar_vazio(int n) { _populacao_criar(n, solucao_criar); }
inline static Solucao** populacao_criar(int n) { _populacao_criar(n, solucao_criar_random); }

static void populacao_apagar(Solucao*** populacao, int n) {
    if (!populacao || !*populacao) return;

    int i;
    for (i = 0; i < n; i++) solucao_apagar(&(*populacao)[i]);
    free(*populacao);
    *populacao = NULL;
}

static void crossover(Solucao* a, Solucao* b) {
    int i;
    for (i = 0; i < tam_cromossomo; i++) {
        if (randbool()) solucao_gene_set(a, i, solucao_gene_get(b, i));
    }
}

static bool populacao_torneio(Solucao* a, Solucao* b, int n) {
    int* nova_populacao = NULL;

    nova_populacao = populacao_criar_vazio(n);

    if (!nova_populacao) goto falha;

falha:
    populacao_apagar(nova_populacao, n);
    return false;
}

static int jogar(Solucao* a, Solucao* b) {
    int jogo[9];
    int i;
    int vez = 1;
    int vencedor;
    int pontuacao = 0;

    for (i = 0; i < 9; i++) jogo[i] = 0;

    do {
        if (vez == 1) {
            jogo[solucao_get(a, jogo)] = vez;
        } else {
            jogo[solucao_get(b, jogo)] = vez;
        }
        vez = trocar_vez(vez);
    } while (vencedor == 1 || vencedor == 2);

    vencedor = calc_vencedor(jogo);
    if (vencedor == 1) {
        pontuacao++;
    } else if (vencedor == 2) {
        pontuacao--;
    }

    return pontuacao;
}

int encontrar_maior(int* fitness, int n) {
    int i;
    int maior = 0;
    for (i = 1; i < n; i++) {
        if (fitness[i] > fitness[maior]) maior = i;
    }
    return maior;
}

int main() {
    srand(time(NULL));

    Solucao** populacao = NULL;
    int i, j, k;
    int fitness[N];

    for (i = 0; i < N; i++) fitness[i] = 0;

    populacao = populacao_criar(N);
    if (!populacao) goto falha;

    enable_raw_mode();
    while (true) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                fitness[i] += jogar(populacao[i], populacao[j]);
            }
        }

        k = encontrar_maior(fitness, N);
        swap(&populacao[0], &populacao[k]);

        // crossover_todos(a, b);

        if (kbhit()) break;
    }
    disable_raw_mode();

    populacao_apagar(&populacao, N);
    return EXIT_SUCCESS;

falha:
    populacao_apagar(&populacao, N);
    printf("Falha ao rodar algoritmo evolutivo\n");
    return EXIT_FAILURE;
}