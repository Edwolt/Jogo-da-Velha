#include "util.h"

static const int N = 19683;   // powi(3, 9); numero de arranjos possiveis de X e O
static const int M = 986410;  //409114;  // 1 + 9 + 9*8 + 9*8*7 + ... + 9*8*7*6*5*4*3*2*1 ; Numemro maximo de recursoes de jogo_possiveis()

int simetrias[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                       {2, 1, 0, 5, 4, 3, 8, 7, 6},
                       {2, 5, 8, 1, 4, 7, 0, 3, 6},
                       {8, 5, 2, 7, 4, 1, 6, 3, 0},
                       {8, 7, 6, 5, 4, 3, 2, 1, 0},
                       {6, 7, 8, 3, 4, 5, 0, 1, 2},
                       {6, 3, 0, 7, 4, 1, 8, 5, 2},
                       {0, 3, 6, 1, 4, 7, 2, 5, 8}};

int simetrias_reversa[8][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                               {2, 1, 0, 5, 4, 3, 8, 7, 6},
                               {6, 3, 0, 7, 4, 1, 8, 5, 2},
                               {8, 5, 2, 7, 4, 1, 6, 3, 0},
                               {8, 7, 6, 5, 4, 3, 2, 1, 0},
                               {6, 7, 8, 3, 4, 5, 0, 1, 2},
                               {2, 5, 8, 1, 4, 7, 0, 3, 6},
                               {0, 3, 6, 1, 4, 7, 2, 5, 8}};

int vitorias[8][3] = {{0, 1, 2},
                      {3, 4, 5},
                      {6, 7, 8},
                      {0, 3, 6},
                      {1, 4, 7},
                      {2, 5, 8},
                      {0, 4, 8},
                      {2, 4, 6}};

int powi(int x, int n) {
    int y = 1;
    while (n > 0) {
        if (n % 2 != 0) y *= x;
        x *= x;
        n /= 2;
    }
    return y;
}

bool venceu(int* jogo) {
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

bool cheio(int* jogo) {
    int i;
    bool zero = false;
    for (i = 0; i < 9; i++) {
        if (jogo[i] == 0) zero = true;
    }
    return !zero;
}

int calc_min(int* jogo) {
    int i, j;
    int minimo = 3 * N;  // Nao tem como dar um numero maior que N
    int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[i][j]] * powi(3, j);
        minimo = min(numero, minimo);
    }
    return minimo;
}

int calc_sim(int* jogo) {
    int i, j;
    int minimo = 3 * N;  // Nao tem como dar um numero maior que N
    int sim = -1;
    int numero;

    for (i = 0; i < 8; i++) {
        numero = 0;
        for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[i][j]] * powi(3, j);
        if (numero < minimo) {
            minimo = numero;
            sim = i;
        }
    }
    return sim;
}

int calc_val(int* jogo, int sim) {
    int j, numero = 0;
    for (j = 0; j < 9; j++) numero += jogo[simetrias_reversa[sim][j]] * powi(3, j);
    return numero;
}

/**
 * jogos deve comecar com o primeiro jogo com tudo 0
 * n deve comecar valendo 1 e sera usado para armazenar dados no vetor
 * Retorna se a operacao foi possivel
 */
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
        if (venceu(jogos[k]) || cheio(jogos[k])) {
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

int** jogos_possiveis(int* n) {
    int** jogos = NULL;
    int i;
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
    if (jogos) {
        for (i = 0; i < M; i++) free(jogos[i]);
        free(jogos);
    }
    return NULL;
}

int* copiar_jogo(int* jogo) {
    int* novo_jogo = NULL;
    int i;

    novo_jogo = malloc(9 * sizeof(int));
    if (!novo_jogo) return NULL;

    for (i = 0; i < 9; i++) novo_jogo[i] = jogo[i];
    return novo_jogo;
}