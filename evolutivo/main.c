#include <stdlib.h>  // malloc, rand
#include <time.h>
#include "kbhit.h"
#include "mapa.h"
#include "populacao.h"

static inline bool sair() {
    char c;
    if (kbhit()) {
        c = getchar();
        if (c == 'q' || c == 'Q') return true;
    }
    return false;
}

/**
 * Calcula uma solucao usando algoritmo evolutivo
 * e salva em evolutivo.txt
 */
int main() {
    srand(time(NULL));
    const int n = 200;
    const int predados = 2;
    const int periodo_predacao = 25;
    const double mutacao = 0.02;

    Populacao* populacao = NULL;
    int i = 0;
    bool ok;

    ok = carregar_mapa("mapa.txt");
    if (!ok) goto falha;

    populacao = populacao_criar(n);
    if (!populacao) goto falha;

    enable_raw_mode();
    while (!sair()) {
        ok = populacao_fitness(populacao);
        if (!ok) goto falha;
        printf("Geracao %d, %d, %d\n", i++, populacao_get_fitness(populacao, 0), populacao_get_fitness(populacao, n - 1));

        ok = populacao_predacao_sintese(populacao, predados);
        if (!ok) goto falha;

        if (i % periodo_predacao == 0) {
            ok = populacao_predacao_randomica(populacao, predados);
            if (!ok) goto falha;
        }

        populacao_torneio(populacao);
        populacao_mutacao(populacao, mutacao);
    }
    disable_raw_mode();

    populacao_fitness(populacao);
    printf("\n%d geracoes processadas alcancando %d\n", i, populacao_get_fitness(populacao, 0));
    populacao_salvar_melhor(populacao, "evolutivo.txt");
    populacao_apagar(&populacao);

    return EXIT_SUCCESS;

falha:
    disable_raw_mode();
    populacao_apagar(&populacao);
    free_mapa();
    printf("\nFalha ao rodar algoritmo evolutivo\n");
    return EXIT_FAILURE;
}