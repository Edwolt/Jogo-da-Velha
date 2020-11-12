#include <stdlib.h>  // malloc, rand
#include <time.h>
#include "kbhit.h"
#include "populacao.h"
#include "solucao.h"

static inline sair() { return kbhit() && getchar() == 'q'; }

int main() {
    srand(time(NULL));

    Populacao* populacao = NULL;
    int i = 0;
    int mutacao = RAND_MAX * 0.02;  // 2%

    populacao = populacao_criar(500);
    if (!populacao) goto falha;

    enable_raw_mode();
    while (!sair()) {
        populacao_fitness(populacao);
        populacao_predacao_sintese(populacao);
        if (i % 10 == 0) populacao_predacao_randomica(populacao);

        populacao_torneio(populacao);
        populacao_mutacao(populacao, mutacao);

        printf("Geracao %d\n", i++);
    }
    disable_raw_mode();

    populacao_salvar(populacao, "evolutivo", NULL);
    populacao_apagar(&populacao);

    printf("%d geracoes processadas\n", i);
    return EXIT_SUCCESS;

falha:
    populacao_apagar(&populacao);
    printf("Falha ao rodar algoritmo evolutivo\n");
    return EXIT_FAILURE;
}