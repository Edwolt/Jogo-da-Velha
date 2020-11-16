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

int main() {
    srand(time(NULL));
    int n = 200;
    int predados = 2;
    int preiodo_predacao = 10;
    double mutacao = 0.01;

    Populacao* populacao = NULL;
    int i = 0;
    bool ok;
    bool mudou;
    int nao_muda = 0;
    int muda = 0;
    int zero = 0;

    ok = carregar_mapa("mapa.txt");
    if (!ok) goto falha;

    populacao = populacao_criar(n);
    if (!populacao) goto falha;

    enable_raw_mode();
    while (!sair()) {
        mudou = populacao_fitness(populacao);
        // mutacao = mutacao * (mudou ? 0.99 : 1.01);
        // if (muda > 3) {
        //     mutacao *= 0.8;
        //     muda = 0;
        // } else if (nao_muda > 5) {
        //     mutacao *= 1 / 0.8;
        //     nao_muda = 0;
        // }
        // nao_muda = (mudou ? 0 : nao_muda + 1);
        // muda = (mudou ? muda + 1 : 0);

        ok = populacao_predacao_sintese(populacao);
        if (!ok) goto falha;

        if (i % preiodo_predacao == 0) {
            ok = populacao_predacao_randomica(populacao, predados);
            if (!ok) goto falha;
        }
        // if (-5 < populacao_get_fitness(populacao, 0) && populacao_get_fitness(populacao, 0) < 5) {
        //     zero++;
        // } else {
        //     zero = 0;
        // }
        // if (zero > 3) {
        //     ok = populacao_predacao_randomica(populacao, predados);
        //     if (!ok) goto falha;
        // }

        populacao_torneio(populacao);
        populacao_mutacao(populacao, mutacao);
        // printf("Geracao %d: %3d;\tMutacao %.10lf: %d %d %d\n", i++, populacao_get_fitness(populacao, 0), mutacao, muda, nao_muda, zero);
        printf("Geracao %d: %3d\n", i++, populacao_get_fitness(populacao, 0));
        if (zero > 3) zero = 0;
    }
    disable_raw_mode();

    populacao_fitness(populacao);
    printf("\n%d geracoes processadas alcancando %d\n", i, populacao_get_fitness(populacao, 0));
    populacao_salvar(populacao, "evolutivo.txt", NULL);
    populacao_apagar(&populacao);

    return EXIT_SUCCESS;

falha:
    disable_raw_mode();
    populacao_apagar(&populacao);
    free_mapa();
    printf("\nFalha ao rodar algoritmo evolutivo\n");
    return EXIT_FAILURE;
}