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

static inline double populacao_get_elo(Populacao* populacao, int i) {
    return individuo_get_elo(populacao_get_individuo(populacao, i));
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
    const int n = 200;
    const int predados = 2;
    const int periodo_predacao = 25;
    const int periodo_informacao = 100;
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

        ok = populacao_torneio_elo(populacao, 3);
        if (!ok) goto falha;

        if (geracao != 0 && geracao % periodo_informacao == 0) printf("Geracao %d\n", geracao);

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

    populacao_ordena_elo(populacao);
    printf("\n%d geracoes processadas com %lf de elo\n", geracao, populacao_get_elo(populacao, melhor));

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