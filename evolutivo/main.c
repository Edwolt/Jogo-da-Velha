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

static inline int populacao_get_fitness(Populacao* populacao, int i) {
    return individuo_get_fitness(populacao_get_individuo(populacao, i));
}

static inline void populacao_salvar(Populacao* populacao, int i, char* path) {
    solucao_salvar(individuo_get_solucao(populacao_get_individuo(populacao, i)), path);
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

    int maior, menor, aux;
    double desvio;  // Desvio padrao
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
                populacao_fitness(populacao);
                aux = populacao_get_fitness(populacao, 0);
                maior = menor = aux;
                desvio = 0;
                for (int i = 0; i < n; i++) {
                    aux = populacao_get_fitness(populacao, 0);
                    maior = max(maior, aux);
                    menor = min(menor, aux);
                    desvio += aux * aux;
                }
                desvio = sqrt(desvio / n);
                printf("Geracao %d: %3d %3d %3.3f\n", geracao, maior, menor, desvio);
            } else {
                printf("Geracao %d\n", geracao);
            }
        }

        // populacao_ordena_fitness(populacao);
        ok = populacao_chave(populacao);
        if (!ok) goto falha;

        ok = populacao_predacao_sintese(populacao, predados);
        if (!ok) goto falha;

        if (periodo_predacao != 0 && geracao % periodo_predacao == 0) {
            ok = populacao_predacao_randomica(populacao, predados);
            if (!ok) goto falha;
        }

        populacao_mutacao(populacao, mutacao);
    }
    disable_raw_mode();

    int melhor = 0;

    populacao_ordena_fitness(populacao);
    printf("\n%d geracoes processadas com %d\n", geracao, populacao_get_fitness(populacao, melhor));
    // printf("\n%d geracoes processadas\n", geracao);

    populacao_salvar(populacao, melhor, "evolutivo.txt");
    populacao_apagar(&populacao);
    return EXIT_SUCCESS;

falha:
    disable_raw_mode();
    populacao_apagar(&populacao);
    free_mapa();
    printf("\nFalha ao rodar algoritmo evolutivo\n");
    return EXIT_FAILURE;
}