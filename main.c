#include <stdbool.h>
#include <stdio.h>
#include "individuo.h"
#include "mapa.h"
#include "minimax.h"

const int N = 19683;   // powi(3, 9); numero de arranjos possiveis de X e O
const int M = 986410;  //409114;  // 1 + 9 + 9*8 + 9*8*7 + ... + 9*8*7*6*5*4*3*2*1 ; Numemro maximo de recursoes de jogo_possiveis()

static int vitorias[8][3] = {{0, 1, 2},
                             {3, 4, 5},
                             {6, 7, 8},
                             {0, 3, 6},
                             {1, 4, 7},
                             {2, 5, 8},
                             {0, 4, 8},
                             {2, 4, 6}};

static bool venceu(int* jogo) {
    int i;
    int a, b, c;
    for (i = 0; i < 8; i++) {
        a = vitorias[i][0];
        b = vitorias[i][1];
        c = vitorias[i][2];
        if (jogo[a] != 0 && jogo[a] == jogo[b] && jogo[b] == jogo[c]) return true;
    }
    return false;
}

static bool _jogos_possiveis(int** jogos, int* n, int atual, int vez) {
    int i, j, k;
    bool ok;

    for (i = 0; i < 9; i++) {
        if (jogos[atual][i] != 0) continue;  // Essa posicao ja foi jogada
        k = (*n)++;

        // Cria uma copia do jogo atual
        jogos[k] = malloc(9 * sizeof(int));
        if (!jogos[k]) goto falha;
        for (j = 0; j < 9; j++) jogos[k][j] = jogos[atual][j];

        // Faz jogada
        jogos[k][i] = vez;
        if (venceu(jogos[k])) continue;

        // Passa para a funcao fazer outra jogada
        ok = _jogos_possiveis(jogos, n, k, (vez == 1 ? 2 : 1));
        if (!ok) goto falha;
    }
    return true;

falha:
    return false;
}

static inline int** jogos_possiveis(int* n) {
    int i;
    int** jogos = NULL;
    bool ok;

    // Aloca array de jogos
    jogos = malloc(M * sizeof(int*));
    if (!jogos) goto falha;
    for (i = 0; i < M; i++) jogos[i] = NULL;

    // Aloca primeiro jogo e inicializa com tudo 0
    jogos[0] = malloc(9 * sizeof(int));
    for (i = 0; i < 9; i++) jogos[0][i] = 0;

    // Chama a funcao auxiliar
    *n = 1;
    ok = _jogos_possiveis(jogos, n, 0, 1);
    if (!ok) goto falha;

    // Retorna os jogos encontrados
    return jogos;

falha:
    for (i = 0; i < M; i++) free(jogos[i]);
    free(jogos);
    return NULL;
}

int main() {
    int i, j;
    int n;
    int** jogos = NULL;
    Individuo* individuo = NULL;

    carregar_mapa();
    jogos = jogos_possiveis(&n);
    if (!jogos) goto falha;

    individuo = individuo_novo();
    if (!individuo) goto falha;

    for (i = 0; i < n; i++) {
        individuo_set(individuo, jogos[i], melhor_jogada(jogos[i]));
    }

    for (i = 0; i < tam_cromossomo; i++) {
        printf("%d\n", individuo->genes[i]);
    }

    return EXIT_SUCCESS;
falha:
    individuo_apagar(&individuo);
    if (jogos) {
        for (i = 0; i < M; i++) free(jogos[i]);
        free(jogos);
    }
    return EXIT_FAILURE;
}