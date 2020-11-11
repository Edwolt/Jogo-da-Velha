#include <stdlib.h>  // malloc, rand
#include <time.h>
#include "kbhit.h"
#include "solucao.h"

Solucao** populacao_criar(int n) {
    Solucao** populacao = NULL;
    int i;

    populacao = malloc(n * sizeof(Solucao*));
    if (!populacao) goto falha;
    for (i = 0; i < n; i++) populacao[i] = NULL;

    for (i = 0; i < n; i++) {
        populacao[i] = solucao_criar_random();
        if (!populacao[i]) goto falha;
    }

    return populacao;

falha:
    if (populacao) {
        for (i = 0; i < n; i++) solucao_apagar(&populacao[i]);
        free(populacao);
    }
    return NULL;
}

void populacao_apagar(Solucao*** populacao, int n) {
    if (!populacao || !*populacao) return;

    int i;
    for (i = 0; i < n; i++) solucao_apagar(&(*populacao)[i]);
    free(*populacao);
    *populacao = NULL;
}

int main() {
    srand(time(NULL));

    Solucao** populacao = NULL;
    const int N = 500;

    populacao = populacao_criar(N);
    if (!populacao) goto falha;

    enable_raw_mode();
    while (true) {
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