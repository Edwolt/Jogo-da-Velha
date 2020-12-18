#include <stdbool.h>
#include <stdio.h>
#include "mapa.h"
#include "minimax.h"
#include "solucao.h"
#include "util.h"

extern byte vitorias[8][3];

/**
 * Calcula uma solucao testando todas as opcoes possiveis de jogo
 * e salva em minmax.txt
 */
int main() {
    byte** jogos = NULL;
    Solucao* solucao = NULL;

    int i;
    int n;
    bool ok;

    ok = carregar_mapa("mapa.txt");
    if (!ok) goto falha;

    jogos = jogos_possiveis(&n);
    if (!jogos) goto falha;

    solucao = solucao_criar();
    if (!solucao) goto falha;

    for (i = 0; i < n; i++) {
        solucao_set_jogada(solucao, jogos[i], melhor_jogada(jogos[i]));
    }

    ok = solucao_salvar(solucao, "minmax.txt");
    if (!ok) goto falha;
    printf("Solucao salva em minmax.txt\n");

    solucao_apagar(&solucao);
    for (i = 0; i < M; i++) free(jogos[i]);
    free(jogos);
    free_mapa();

    return EXIT_SUCCESS;
falha:
    solucao_apagar(&solucao);
    if (jogos) {
        for (i = 0; i < M; i++) free(jogos[i]);
        free(jogos);
    }
    free_mapa();
    return EXIT_FAILURE;
}