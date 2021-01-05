#include <math.h>
#include <stdlib.h>  // malloc, rand
#include <time.h>
#include "kbhit.h"
#include "mapa.h"
#include "populacao.h"
#include "solucao.h"

// char path[100] = "";

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
    const int periodo_informacao = 10;
    const double mutacao = 0.002;

    Populacao* populacao = NULL;
    int geracao = -1;
    int ind = 0;

    bool ok;

    ok = carregar_mapa("mapa.txt");
    if (!ok) goto falha;

    populacao = populacao_criar(n);
    if (!populacao) goto falha;

    // populacao_oponentes(n);
    populacao_fitness(populacao);

    enable_raw_mode();
    while (!sair() && geracao < 2500) {
        geracao++;

        ok = populacao_torneio(populacao);
        if (!ok) goto falha;
        populacao_fitness(populacao);

        if (periodo_informacao != 0 && geracao % periodo_informacao == 0) {
            printf("Geracao %3d, %d, %d\n", geracao, populacao->pop[0]->fitness, populacao->pop[n-1]->fitness);
        }

        ok = populacao_predacao_sintese(populacao, predados);
        if (!ok) goto falha;

        if (periodo_predacao != 0 && geracao % periodo_predacao == 0) {
            ok = populacao_predacao_randomica(populacao, predados);
            if (!ok) goto falha;
        }

        populacao_mutacao(populacao, mutacao);
    }
    disable_raw_mode();

    populacao_fitness(populacao);
    solucao_correcao(populacao->pop[0]->sol);
    printf("\n%d geracoes processadas com %d de fitness\n", geracao, populacao->pop[0]->fitness);

    solucao_salvar(populacao->pop[0]->sol, "evolutivo.txt");
    printf("Melhor solucao salvo em elitismo.txt\n");
    populacao_apagar(&populacao);
    return EXIT_SUCCESS;

falha:
    disable_raw_mode();
    populacao_apagar(&populacao);
    free_mapa();
    printf("\nFalha ao rodar algoritmo evolutivo\n");
    return EXIT_FAILURE;
}