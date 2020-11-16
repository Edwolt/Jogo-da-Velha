#include <stdbool.h>
#include <stdio.h>
#include "individuo.h"
#include "mapa.h"
#include "minimax.h"
#include "util.h"

// static const int N = 19683;   // powi(3, 9); numero de arranjos possiveis de X e O
static const int M = 986410;  //409114;  // 1 + 9 + 9*8 + 9*8*7 + ... + 9*8*7*6*5*4*3*2*1, Numemro maximo de recursoes de jogo_possiveis()

extern byte vitorias[8][3];

int main() {
    byte** jogos = NULL;
    Individuo* individuo = NULL;

    int i;
    int n;
    bool ok;

    ok = carregar_mapa("mapa.txt");
    if (!ok) goto falha;

    jogos = jogos_possiveis(&n);
    if (!jogos) goto falha;

    individuo = individuo_criar();
    if (!individuo) goto falha;

    for (i = 0; i < n; i++) {
        individuo_set(individuo, jogos[i], melhor_jogada(jogos[i]));
    }

    ok = individuo_salvar(individuo, "minmax.txt");
    if (!ok) goto falha;
    printf("Solucao salva em minmax.txt\n");

    individuo_apagar(&individuo);
    for (i = 0; i < M; i++) free(jogos[i]);
    free(jogos);
    free_mapa();

    return EXIT_SUCCESS;
falha:
    individuo_apagar(&individuo);
    if (jogos) {
        for (i = 0; i < M; i++) free(jogos[i]);
        free(jogos);
    }
    free_mapa();
    return EXIT_FAILURE;
}