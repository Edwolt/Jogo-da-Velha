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
    const int N = 500;
    const int MUTACAO = RAND_MAX * 0.02;  // 2%

    Populacao* populacao = NULL;
    int i = 0;
    bool ok;

    ok = carregar_mapa("mapa.txt");
    if (!ok) {
        printf("mapa\n");
        goto falha;
    }

    populacao = populacao_criar(N);
    if (!populacao) {
        printf("populacao\n");
        goto falha;
    }

    enable_raw_mode();
    while (!sair() && i < 10000000) {
        populacao_fitness(populacao);
        if (i % 10 == 0) {
            ok = populacao_predacao_randomica(populacao);
            if (!ok) {
                printf("sintese\n");
                goto falha;
            }
        } else {
            ok = populacao_predacao_sintese(populacao);
            if (!ok) {
                printf("random\n");
                goto falha;
            }
        }

        populacao_torneio(populacao);
        populacao_mutacao(populacao, MUTACAO);
        printf("Geracao %d: %d\n", i++, populacao_get_fitness(populacao, 0));
    }
    disable_raw_mode();

    populacao_fitness(populacao);
    printf("%d geracoes processadas alcancando %d\n", i, populacao_get_fitness(populacao, 0));
    populacao_salvar(populacao, "evolutivo.txt", NULL);
    populacao_apagar(&populacao);

    return EXIT_SUCCESS;

falha:
    disable_raw_mode();
    populacao_apagar(&populacao);
    free_mapa();
    printf("Falha ao rodar algoritmo evolutivo\n");
    return EXIT_FAILURE;
}