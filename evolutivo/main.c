#include <stdlib.h>  // malloc, rand
#include <time.h>
#include "solucao.h"

Solucao* random_solucao() {
    Solucao* sol = solucao_novo();
    if (!sol) return NULL;
}

int* gerar_populacao(int n) {
    int i;
    Solucao** populacao = malloc(n * sizeof(Solucao*));

    for (i = 0; i < n; i++) {
        populacao = random_solucao();
    }
}

int main() {
    srand(time(NULL));
}