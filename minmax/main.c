#include <stdbool.h>
#include <stdio.h>
#include "solucao.h"
#include "mapa.h"
#include "minimax.h"
#include "util.h"

// static const int N = 19683;   // powi(3, 9); numero de arranjos possiveis de X e O
static const int M = 986410;  //409114;  // 1 + 9 + 9*8 + 9*8*7 + ... + 9*8*7*6*5*4*3*2*1, Numemro maximo de recursoes de jogo_possiveis()

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
        solucao_set(solucao, jogos[i], melhor_jogada(jogos[i]));
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