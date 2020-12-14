#include <math.h>
#include <stdlib.h>  // malloc, rand
#include <time.h>
#include "kbhit.h"
#include "mapa.h"
#include "populacao.h"
#include "solucao.h"

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
    const int n = 10;
    const int predados = 1;
    const int periodo_predacao = 25;
    const int periodo_grafico = -100;
    const double mutacao = 0.02;

    Populacao* populacao = NULL;
    int geracao = -1;
    bool ok;

    ok = carregar_mapa("mapa.txt");
    if (!ok) goto falha;

    populacao = populacao_criar(n);
    if (!populacao) goto falha;

    enable_raw_mode();
    while (!sair()) {
        geracao++;

        if (periodo_grafico != 0 && geracao % periodo_grafico == 0) {
            if (periodo_grafico > 0) {
                int* fitness = populacao_fitness(populacao);
                if (!fitness) goto quebra;
                int maior = fitness[0];
                int menor = fitness[0];
                double desvio = 0;
                for (int i = 0; i < n; i++) {
                    maior = max(maior, fitness[i]);
                    menor = min(menor, fitness[i]);
                    desvio += fitness[i] * fitness[i];
                }
                desvio = sqrt(desvio / n);
                printf("Geracao %d: %3d %3d %3.3f\n", geracao, maior, menor, desvio);
            } else {
            quebra:
                printf("Geracao %d\n", geracao);
            }
        }

        // populacao_ordena_fitness(populacao);
        ok = populacao_chave(populacao);
        if (!ok) goto falha;

        // ok = populacao_predacao_sintese(populacao, predados);
        // if (!ok) goto falha;

        // if (periodo_predacao != 0 && geracao % periodo_predacao == 0) {
        //     ok = populacao_predacao_randomica(populacao, predados);
        //     if (!ok) goto falha;
        // }

        populacao_mutacao(populacao, mutacao);
    }
    disable_raw_mode();

    int melhor = 0;
    int* fitness = populacao_fitness(populacao);

    if (fitness) {
        for (int i = 0; i < n; i++) {
            if (fitness[i] > fitness[melhor]) melhor = i;
        }
        printf("\n%d geracoes processadas com %d\n", geracao, fitness[melhor]);
    } else {
        printf("\n%d geracoes processadas\n", geracao);
    }
    solucao_salvar(populacao_get_solucao(populacao, melhor), "evolutivo.txt");
    populacao_apagar(&populacao);
    free(fitness);
    return EXIT_SUCCESS;

falha:
    disable_raw_mode();
    populacao_apagar(&populacao);
    free_mapa();
    printf("\nFalha ao rodar algoritmo evolutivo\n");
    return EXIT_FAILURE;
}