#include "util.h"

// Obs: Se X for 1 e O 2 entao eu vou ter mais 1 que 2 no numero ternario, o que vai criar um mapa menor
const int N = 19683;   // pow(3, 9); numero de arranjos possiveis de X e O
const int M = 986410;  //409114;  // 1 + 9 + 9*8 + 9*8*7 + ... + 9*8*7*6*5*4*3*2*1 ; Numero maximo de recursoes de jogo_possiveis()

byte simetrias[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                        {2, 1, 0, 5, 4, 3, 8, 7, 6},
                        {2, 5, 8, 1, 4, 7, 0, 3, 6},
                        {8, 5, 2, 7, 4, 1, 6, 3, 0},
                        {8, 7, 6, 5, 4, 3, 2, 1, 0},
                        {6, 7, 8, 3, 4, 5, 0, 1, 2},
                        {6, 3, 0, 7, 4, 1, 8, 5, 2},
                        {0, 3, 6, 1, 4, 7, 2, 5, 8}};

byte simetrias_reversa[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                                {2, 1, 0, 5, 4, 3, 8, 7, 6},
                                {6, 3, 0, 7, 4, 1, 8, 5, 2},
                                {8, 5, 2, 7, 4, 1, 6, 3, 0},
                                {8, 7, 6, 5, 4, 3, 2, 1, 0},
                                {6, 7, 8, 3, 4, 5, 0, 1, 2},
                                {2, 5, 8, 1, 4, 7, 0, 3, 6},
                                {0, 3, 6, 1, 4, 7, 2, 5, 8}};

byte vitorias[8][3] = {{0, 1, 2},
                       {3, 4, 5},
                       {6, 7, 8},
                       {0, 3, 6},
                       {1, 4, 7},
                       {2, 5, 8},
                       {0, 4, 8},
                       {2, 4, 6}};

int pow3[10] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683};

//* ===== Jogo ===== *//

/**
 * jogos deve comecar com o primeiro jogo com tudo 0
 * n deve comecar valendo 1 e sera usado para armazenar dados no vetor
 * Retorna se a operacao foi possivel
 */
static bool _jogos_possiveis(byte** jogos, int* n, int atual, int vez) {
    int i, k;
    bool ok;

    for (i = 0; i < 9; i++) {
        if (jogos[atual][i] != 0) continue;  // Essa posicao ja foi jogada
        k = (*n)++;

        // Cria uma copia do jogo atual
        jogos[k] = copiar_jogo(jogos[atual]);
        if (!jogos[k]) goto falha;

        // Faz jogada
        jogos[k][i] = vez;
        if (venceu_ou_velha(jogos[k])) {
            free(jogos[k]);
            jogos[k] = NULL;
            (*n)--;
            continue;
        };

        // Passa para a funcao fazer outra jogada
        ok = _jogos_possiveis(jogos, n, k, (vez == 1 ? 2 : 1));
        if (!ok) goto falha;
    }
    return true;

falha:
    return false;
}

byte** jogos_possiveis(int* n) {
    byte** jogos = NULL;
    int i;
    bool ok;

    // Aloca array de jogos
    jogos = malloc(M * sizeof(byte*));
    if (!jogos) goto falha;
    for (i = 0; i < M; i++) jogos[i] = NULL;

    // Aloca primeiro jogo e inicializa com tudo 0
    jogos[0] = malloc(9 * sizeof(byte));
    if (!jogos[0]) goto falha;
    for (i = 0; i < 9; i++) jogos[0][i] = 0;

    // Chama a funcao auxiliar
    *n = 1;
    ok = _jogos_possiveis(jogos, n, 0, 1);
    if (!ok) goto falha;

    // Retorna os jogos encontrados
    return jogos;

falha:
    if (jogos) {
        for (i = 0; i < M; i++) free(jogos[i]);
        free(jogos);
    }
    return NULL;
}